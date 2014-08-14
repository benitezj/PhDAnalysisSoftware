/*****************************************************************************
 * Project: RooFit                                                           *
 * Package: RooFitCore                                                       *
 * @(#)root/roofitcore:$Id: DRecoTranslationPdf.cc,v 1.2 2009/08/19 21:36:27 benitezj Exp $
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

#include "RooFit.h"
#include "Riostream.h"

#include "TIterator.h"
#include "DRecoilToolsUser/NtupleCode/DRecoTranslationPdf.hh"
#include "DRecoilToolsUser/NtupleCode/DRecoAddPdf.hh"
#include "DRecoilToolsUser/NtupleCode/DRecoHistPdf.hh"
#include "DRecoilToolsUser/NtupleCode/DRecoProdPdf.hh"
#include "RooRealVar.h"
#include "TString.h"


ClassImp(DRecoTranslationPdf);


DRecoTranslationPdf::DRecoTranslationPdf(const char *name, const char *title, RooArgSet& NormVars, RooAbsPdf& pdf, RooRealVar& var, RooRealVar& delta) : 
  RooAbsPdf(name,title), 
  _pdf("_pdf","_pdf",this,pdf),
  _depList("_depList","List of dependents",this),
  _var(&var),
  _delta(&delta),
  nset(0)
{

  if(delta.getVal()>(var.getMax()-var.getMin())){
    cout<<"DRecoTranslationPdf::DRecoTranslationPdf:  delta is greater than the range. "<<endl;
    exit(0);
  }
  
  _depList.add(NormVars);
    
}


DRecoTranslationPdf::DRecoTranslationPdf(const DRecoTranslationPdf& other, const char* name) :
  RooAbsPdf(other,name), 
  _pdf("_pdf",this,other._pdf),
  _depList("depList",this,other._depList),
  _var(other._var),
  _delta(other._delta),
  nset(0)
{


}

DRecoTranslationPdf::~DRecoTranslationPdf()
{
  
}

Double_t DRecoTranslationPdf::getVal(const RooArgSet* set) const 
{
  
  nset=(RooArgSet*)set;
  return evaluate();
}


Double_t DRecoTranslationPdf::evaluate() const 
{
  //Calculate current unnormalized value of object
     
  RooRealVar* depvar=(RooRealVar*)_depList.find(_var->GetName());
  if(!depvar){
    cout<<"DRecoTranslationPdf::evaluate  variable for translation not found in _depList???"<<endl;
    exit(0);
  }

  Double_t origval=depvar->getVal();
  Double_t transval=origval+_delta->getVal();
  if(transval<depvar->getMin()) transval+= depvar->getMax()-depvar->getMin();
  if(transval>depvar->getMax()) transval-= depvar->getMax()-depvar->getMin();
  depvar->setVal(transval);
  
  Double_t pvalue=((RooAbsPdf*)&(_pdf.arg()))->getVal(nset);//; 

  depvar->setVal(origval);

  return pvalue;
}




Double_t DRecoTranslationPdf::getHistError() const {
  //This method returns the fractional error on the bin content

  
  RooRealVar* depvar=(RooRealVar*)_depList.find(_var->GetName());
  if(!depvar){
    cout<<"DRecoTranslationPdf::getHistError  variable for translation not found in _depList???"<<endl;
    exit(0);
  }
  Double_t origval=depvar->getVal();

  Double_t transval=origval+_delta->getVal();
  if(transval<depvar->getMin()) transval+= depvar->getMax()-depvar->getMin();
  if(transval>depvar->getMax()) transval-= depvar->getMax()-depvar->getMin();
  depvar->setVal(transval);
  
  Double_t _pdfValErr=0.;
  if(TString((_pdf.arg()).ClassName())=="DRecoAddPdf")_pdfValErr = ((DRecoAddPdf*)&(_pdf.arg()))->getHistError() ;
  if(TString((_pdf.arg()).ClassName())=="DRecoHistPdf")_pdfValErr = ((DRecoHistPdf*)&(_pdf.arg()))->getHistError() ;
  if(TString((_pdf.arg()).ClassName())=="DRecoProdPdf")_pdfValErr = ((DRecoProdPdf*)&(_pdf.arg()))->getHistError() ;
  if(TString((_pdf.arg()).ClassName())=="DRecoTranslationPdf")_pdfValErr = ((DRecoTranslationPdf*)&(_pdf.arg()))->getHistError() ;

  depvar->setVal(origval);


  return _pdfValErr;
}



