/*****************************************************************************
 * Project: RooFit                                                           *
 * Package: RooFitCore                                                       *
 *    File: $Id: RooGenericPdf.h,v 1.20 2007/05/11 09:11:30 verkerke Exp $
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
#ifndef ROO_GENERIC_PDFN
#define ROO_GENERIC_PDFN

#include "RooAbsPdf.h"
#include "RooFormula.h"
#include "RooListProxy.h"

class RooArgList ;

class RooGenericPdfN : public RooAbsPdf {
public:
  // Constructors, assignment etc
  inline RooGenericPdfN() { }
  RooGenericPdfN(const char *name, const char *title, const RooArgList& dependents);
  RooGenericPdfN(const char *name, const char *title, const char* formula, const RooArgList& dependents);
  RooGenericPdfN(const RooGenericPdfN& other, const char* name=0);
  virtual TObject* clone(const char* newname) const { return new RooGenericPdfN(*this,newname); }
  virtual ~RooGenericPdfN();

  // I/O streaming interface (machine readable)
  virtual Bool_t readFromStream(istream& is, Bool_t compact, Bool_t verbose=kFALSE) ;
  virtual void writeToStream(ostream& os, Bool_t compact) const ;

  // Printing interface (human readable)
  //virtual void printToStream(ostream& os, PrintOption opt=Standard, TString indent= "") const ;

  // Debugging
  void dumpFormula() { _formula.dump() ; }

  Double_t getVal(const RooArgSet* set=0) const ;


protected:

  // Function evaluation
  RooListProxy _actualVars ; 
  virtual Double_t evaluate() const ;

  Bool_t setFormula(const char* formula) ;

  // Post-processing of server redirection
  virtual Bool_t redirectServersHook(const RooAbsCollection& newServerList, Bool_t mustReplaceAll, Bool_t nameChange, Bool_t isRecursive) ;

  virtual Bool_t isValidReal(Double_t value, Bool_t printError) const ;

  mutable RooFormula _formula ; // Formula engine 

  ClassDef(RooGenericPdfN,1) // Generic PDF defined by string expression and list of variables
};

#endif
