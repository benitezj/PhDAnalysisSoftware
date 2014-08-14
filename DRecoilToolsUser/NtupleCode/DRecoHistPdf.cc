/*****************************************************************************
 * Project: RooFit                                                           *
 * Package: RooFitCore                                                       *
 * @(#)root/roofit:$Id: DRecoHistPdf.cc,v 1.1 2009/05/26 18:00:28 benitezj Exp $
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
// DRecoHistPdf implements a probablity density function sample from a 
// multidimensional histogram. The histogram distribution is explicitly
// normalized by DRecoHistPdf and can have an arbitrary number of real or 
// discrete dimensions.

#include "RooFit.h"
#include "Riostream.h"

#include "DRecoilToolsUser/NtupleCode/DRecoHistPdf.hh"
#include "RooDataHist.h"
#include "RooMsgService.h"
#include "RooAbsData.h"
#include "RooRealVar.h"


ClassImp(DRecoHistPdf)
;


DRecoHistPdf::DRecoHistPdf(const char *name, const char *title, 
			   RooArgSet& vars, RooDataHist& dhist, Int_t intOrder) :
  RooAbsPdf(name,title), 
  _depList("depList","List of dependents",this),
  _dataHist((RooDataHist*)&dhist),  
  _intOrder(intOrder)    
{


  // Verify that vars and dhist.get() have identical contents
  const RooArgSet* dvars = dhist.get();
  if (vars.getSize()!=dvars->getSize()){
    coutE(InputArguments) << "DRecoHistPdf::ctor(" << GetName() 
			  << ") ERROR variable list and RooDataHist must contain the same variables." << endl ;
    assert(0) ;
  }
  TIterator* iter = vars.createIterator() ;
  RooAbsArg* arg ;
  while((arg=(RooAbsArg*)iter->Next())) {
    if (!dvars->find(arg->GetName())) {
      coutE(InputArguments) << "DRecoHistPdf::ctor(" << GetName() 
			    << ") ERROR variable list and RooDataHist must contain the same variables." << endl ;
      assert(0) ;
    }
  }
  delete iter ;
  

  _depList.add(vars) ;


}


DRecoHistPdf::DRecoHistPdf(const DRecoHistPdf& other, const char* name) :
  RooAbsPdf(other,name), 
  _depList("depList",this,other._depList), 
  _dataHist(other._dataHist),  
  _intOrder(other._intOrder)  
{  // Copy constructor

}

Double_t DRecoHistPdf::getVal(const RooArgSet* set) const
{
  //gets stuck if you call the RooAbsPdf::getVal 
  return evaluate();
}

Double_t DRecoHistPdf::evaluate() const
{
  // Return the current value: The value of the bin enclosing the current coordinates
  // of the dependents,

  if(_dataHist->sumEntries()<=0.){
    cout<<"DRecoHistPdf::"<<GetName()
	<<" sumEntries()<=0.!!"<<_dataHist->sumEntries()
	<<endl;
    exit(0);  
  }
  
//   if(TString(GetName())=="WSBkgnXRSelectPdf0")
//     cout<<"DRecoHistPdf::evaluate"<<GetName()<<" v1="<<_depList.first()<<"  dep&="<<&_depList<<endl;

  return _dataHist->weight(_depList,_intOrder,kTRUE)/_dataHist->sumEntries();//normalized value
}


Double_t DRecoHistPdf::getHistError() const {

  _dataHist->get(_depList);//update RooDataSet before getting error or weight
  Double_t binerror=_dataHist->weightError(RooAbsData::SumW2);
  Double_t bincontent = _dataHist->weight() ;
  if(binerror==bincontent)binerror=0.;//if histo has 0 error sumw2=bincontent*bincontent 
  if(bincontent==0.)return 0.;
  //cout<<"DRecoHistPdf::"<<this->GetName()<<" "<<binerror<<" "<<bincontent<<" "<<fabs(binerror/bincontent)<<endl;  
  return fabs(binerror/bincontent);  //return the fractional errors for this bin content
}

