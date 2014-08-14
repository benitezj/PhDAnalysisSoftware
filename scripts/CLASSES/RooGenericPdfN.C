/*****************************************************************************
 * Project: RooFit                                                           *
 * Package: RooFitCore                                                       *
 * @(#)root/roofitcore:$Id: RooGenericPdf.cxx 21379 2007-12-14 15:12:29Z wouter $
 * Authors:                                                                  *
 *   WV, Wouter Verkerke, UC Santa Barbara, verkerke@slac.stanford.edu       *
 *   DK, David Kirkby,    UC Irvine,         dkirkby@uci.edu                 *
 *                                                                           *
 * Copyright (c) 2000-2005, Regents of the University of California          *
 *                          and Stanford University. All rights reserved.    *
 *                                                                           *
 * Redistribution and use in source and binary forms,                        *
 * with or without modification, are permitted according to the terms        *
 * listed in LICENSE (http://roofit.sourceforge.net/license.txt)             *
 *****************************************************************************/

// -- CLASS DESCRIPTION [PDF] --
// RooGenericPdf is a concrete implementation of a probability density function,
// which takes a RooArgList of servers and a C++ expression string defining how
// its value should be calculated from the given list of servers.
// A fully numerical integration is automatically performed to normalize the given
// expression. RooGenericPdf uses a RooFormula object to perform the expression evaluation
//
// The string expression can be any valid TFormula expression referring to the
// listed servers either by name or by their ordinal list position:
//
//   RooGenericPdf("gen","x*y",RooArgList(x,y))  or
//   RooGenericPdf("gen","@0*@1",RooArgList(x,y)) 
//
// The latter form, while slightly less readable, is more versatile because it
// doesn't hardcode any of the variable names it expects

#include "RooFit.h"
#include "Riostream.h"

#include "RooGenericPdfN.h"
#include "RooStreamParser.h"
#include "RooMsgService.h"
#include "RooArgList.h"



ClassImp(RooGenericPdfN)


RooGenericPdfN::RooGenericPdfN(const char *name, const char *title, const RooArgList& dependents) : 
  RooAbsPdf(name,title), 
  _actualVars("actualVars","Variables used by PDF expression",this),
  _formula(name,title,dependents)
{  
  // Constructor with formula expression and list of input variables
  _actualVars.add(dependents) ; 

  if (_actualVars.getSize()==0) _value = traceEval(0) ;
}


RooGenericPdfN::RooGenericPdfN(const char *name, const char *title, 
			     const char* formula, const RooArgList& dependents) : 
  RooAbsPdf(name,title), 
  _actualVars("actualVars","Variables used by PDF expression",this),
  _formula(name,formula,dependents)
{  
  _actualVars.add(dependents) ; 

  if (_actualVars.getSize()==0) _value = traceEval(0) ;
}


RooGenericPdfN::RooGenericPdfN(const RooGenericPdfN& other, const char* name) : 
  RooAbsPdf(other, name), 
  _actualVars("actualVars",this,other._actualVars),
  _formula(other._formula)
{
  // Copy constructor
}


RooGenericPdfN::~RooGenericPdfN() 
{
  // Destructor
}


Double_t RooGenericPdfN::evaluate() const
{
  // Calculate current value of this object
  return _formula.eval(_normSet) ;
}


Double_t RooGenericPdfN::getVal(const RooArgSet* nset) const
{
  // Return current value, normalizated by integrating over
  // the dependents in 'nset'. If 'nset' is 0, the unnormalized value. 
  // is returned. All elements of 'nset' must be lvalues

  // Unnormalized values are not cached
  // Doing so would be complicated as _norm->getVal() could
  // spoil the cache and interfere with returning the cached
  // return value. Since unnormalized calls are typically
  // done in integration calls, there is no performance hit.

  if (!nset) {
    Double_t val = evaluate() ;
    Bool_t error =  isnan(val);//traceEvalPdf(val) ;
    //    cxcoutD(Tracing) << IsA()->GetName() << "::getVal(" << GetName() < "): value = " << val << " (unnormalized)" << endl ;
    if (error) {
      raiseEvalError() ;
      return 0 ;
    }
    return val ;
  }

  // Process change in last data set used
  Bool_t nsetChanged(kFALSE) ;
  if (nset!=_normSet || _norm==0) {
    nsetChanged = syncNormalization(nset) ;
  }

  // Return value of object. Calculated if dirty, otherwise cached value is returned.
  if ((isValueDirty() || nsetChanged || _norm->isValueDirty()) && operMode()!=AClean) {

    // cout << "doEval" << endl ;
    
    // Evaluate numerator
    Double_t rawVal = evaluate() ;
    Bool_t error = 0;//traceEvalPdf(rawVal) ; // Error checking and printing

    // Evaluate denominator
    Double_t normVal(_norm->getVal()) ;

    //cxcoutD(Tracing) << "RooAbsPdf::getVal(" << GetName() << ") normalization integral is " << (_norm?_norm->GetName():"none") << endl ;

    Double_t normError(kFALSE) ;
    if (normVal==0.) normError=kTRUE ;

    // Raise global error flag if problems occur
    if (normError||error) raiseEvalError() ;

    _value = normError ? 0 : (rawVal / normVal) ;

    //if (_verboseEval>1) cxcoutD(Tracing) << IsA()->GetName() << "::getVal(" << GetName() << "): value = " << _value << " (normalized)" << endl ;
    
    //cxcoutD(Tracing) << "RooAbsPdf::getVal(" << GetName() << ") recalculating, new value = " << rawVal << "/" << normVal << " = " << _value << endl ;

    clearValueDirty() ; //setValueDirty(kFALSE) ;
    clearShapeDirty() ; //setShapeDirty(kFALSE) ;    
  } 

  if (_traceCount>0) {
    //cxcoutD(Tracing) << "[" << _traceCount << "] " ;
    Int_t tmp = _traceCount ;
    _traceCount = 0 ;
    //printToStream(ccoutD(Tracing)) ;
    _traceCount = tmp-1  ;
  }

  return _value ;
}



Bool_t RooGenericPdfN::setFormula(const char* formula) 
{
  // Change formula expression to given expression
  if (_formula.reCompile(formula)) return kTRUE ;
  
  SetTitle(formula) ;
  setValueDirty() ;
  return kFALSE ;
}


Bool_t RooGenericPdfN::isValidReal(Double_t /*value*/, Bool_t /*printError*/) const {
  // Check if given value is valid
  return kTRUE ;
}



Bool_t RooGenericPdfN::redirectServersHook(const RooAbsCollection& newServerList, Bool_t mustReplaceAll, Bool_t nameChange, Bool_t /*isRecursive*/)
{
  // Propagate server changes to embedded formula objecy
  return _formula.changeDependents(newServerList,mustReplaceAll,nameChange) ;
}



// void RooGenericPdfN::printToStream(ostream& os, PrintOption opt, TString indent) const
// {
//   // Print info about this object to the specified stream. 
//   RooAbsPdf::printToStream(os,opt,indent);
//   if(opt >= Verbose) {
//     indent.Append("  ");
//     os << indent;
//     _formula.printToStream(os,opt,indent);
//   }
// }



Bool_t RooGenericPdfN::readFromStream(istream& is, Bool_t compact, Bool_t /*verbose*/)
{
  // Read object contents from given stream
  if (compact) {
    coutE(InputArguments) << "RooGenericPdfN::readFromStream(" << GetName() << "): can't read in compact mode" << endl ;
    return kTRUE ;
  } else {
    RooStreamParser parser(is) ;
    return setFormula(parser.readLine()) ;
  }
}

void RooGenericPdfN::writeToStream(ostream& os, Bool_t compact) const
{
  // Write object contents to given stream
  if (compact) {
    os << getVal() << endl ;
  } else {
    os << GetTitle() ;
  }
}



