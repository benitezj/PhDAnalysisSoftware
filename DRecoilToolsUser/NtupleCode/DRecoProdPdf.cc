/*****************************************************************************
 * Project: RooFit                                                           *
 * Package: RooFitCore                                                       *
 * @(#)root/roofitcore:$Id: DRecoProdPdf.cc,v 1.2 2009/08/19 21:36:27 benitezj Exp $
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
// DRecoProdPdf is an efficient implementation of a product of PDFs of the form 
//
//  PDF_1 * PDF_2 * ... * PDF_N
//
// PDFs may share observables. If that is the case any irreducable subset
// of PDFS that share observables will be normalized with explicit numeric
// integration as any built-in normalization will no longer be valid.
//
// Alternatively, products using conditional PDFs can be defined, e.g.
//
//    F(x|y) * G(y)
//
// meaning a pdf F(x) _given_ y and a PDF G(y). In this contruction F is only
// normalized w.r.t x and G is normalized w.r.t y. The product in this construction
// is properly normalized.
//
// If exactly one of the component PDFs supports extended likelihood fits, the
// product will also be usable in extended mode, returning the number of expected
// events from the extendable component PDF. The extendable component does not
// have to appear in any specific place in the list.
// 

#include "RooFit.h"
#include "Riostream.h"

#include "TIterator.h"
#include "DRecoilToolsUser/NtupleCode/DRecoProdPdf.hh"
#include "DRecoilToolsUser/NtupleCode/DRecoAddPdf.hh"
#include "DRecoilToolsUser/NtupleCode/DRecoHistPdf.hh"
#include "DRecoilToolsUser/NtupleCode/DRecoTranslationPdf.hh"
#include "RooRealVar.h"
#include "TString.h"

#include "RooRealProxy.h"
#include "TH3F.h"
#include "TH2F.h"
#include "TH1F.h"

ClassImp(DRecoProdPdf);


DRecoProdPdf::DRecoProdPdf(const char *name, const char *title,
			   RooArgSet& NormVars, RooAbsPdf& pdf1, RooAbsPdf& pdf2, Double_t cutOff) : 
  RooAbsPdf(name,title), 
  _cutOff(cutOff),
  _pdfList("pdfs","List of PDFs",this),
  _pdfIter(_pdfList.createIterator()),
  _depList("depList","List of dependents",this),
  _normvars(&NormVars),
  _Norm(0.)//force normalization on first getVal
{

  _pdfList.add(pdf1) ;
  _pdfList.add(pdf2) ;
    
  _parset=getParameters(_normvars);
  _parsetIter=_parset->createIterator();
  
  _depList.add(NormVars) ;
  _normvarIter=_depList.createIterator();
}


DRecoProdPdf::DRecoProdPdf(const DRecoProdPdf& other, const char* name) :
  RooAbsPdf(other,name), 
  _cutOff(other._cutOff),
  _pdfList("pdfs",this,other._pdfList),
  _pdfIter(_pdfList.createIterator()),
  _depList("depList",this,other._depList), 
  _normvars(other._normvars),
  //_normvarIter(_normvars->createIterator()),
  _Norm(0.)
{

  //cout<<" DRecoProdPdf clone "<<GetName()<<"   v1="<<_normvars->first()<<endl;

  _parset=getParameters(_normvars);
  _parsetIter=_parset->createIterator();

  _normvarIter=_depList.createIterator();
}

DRecoProdPdf::~DRecoProdPdf()
{
  delete _pdfIter;
  delete _normvarIter;
  delete _parset;
  delete _parsetIter;
}

Double_t DRecoProdPdf::getVal(const RooArgSet* set) const 
{
  
  if(!set)return evaluate();
 
  //perform own normalization
  if(parsChanged()||_Norm==0.)  updateNorm();
 
  if(_Norm<=0.){
    cout<<" DRecoProdPdf::getVal  "<< GetName()<<"     !!!!!!!!!!  _Norm="<<_Norm<<endl;
    exit(0);
  }
  
  return evaluate()/_Norm;
}


Double_t DRecoProdPdf::evaluate() const 
{
  //Calculate current unnormalized value of object
  
  Double_t prodvalue=1.;
  RooAbsPdf* pdf;
  _pdfIter->Reset();
  while((pdf=(RooAbsPdf*)_pdfIter->Next())&&prodvalue>_cutOff){
    //Double_t pVal = pdf->getVal();
    //cout<<"DRecoProdPdf::"<<GetName()<<" pdf="<<pdf->GetName()<<" "<<pVal<<endl;
    prodvalue*= pdf->getVal() ;
  } 


  return prodvalue;
}


Bool_t DRecoProdPdf::parsChanged() const {
  Bool_t changed=0;

  _parsetIter->Reset();
  RooRealVar* par;
  Int_t i(0);
  while((par=(RooRealVar*)_parsetIter->Next())){
    if(_param[i]!=par->getVal())
      changed=1;
    i++;
  }
  
  return changed;
}

void DRecoProdPdf::updateNorm() const {


  // Process change in last parameter change
  _Norm=0.;
  _normvarIter->Reset();
  RooRealVar* v1=(RooRealVar*)_normvarIter->Next();
  RooRealVar* v2=(RooRealVar*)_normvarIter->Next();
  RooRealVar* v3=(RooRealVar*)_normvarIter->Next();
  Double_t origv1=v1->getVal();
  Double_t origv2=0.;
  if(v2)origv2=v2->getVal();
  Double_t origv3=0.;
  if(v3)origv3=v3->getVal();
  for(Int_t b1=0;b1<v1->getBins();b1++){
    v1->setVal(v1->getMin()+(b1+.5)*(v1->getMax()-v1->getMin())/v1->getBins());
    if(v2){
      for(Int_t b2=0;b2<v2->getBins();b2++){
	v2->setVal(v2->getMin()+(b2+.5)*(v2->getMax()-v2->getMin())/v2->getBins());
 	if(v3){
 	  for(Int_t b3=0;b3<v3->getBins();b3++){
 	    v3->setVal(v3->getMin()+(b3+.5)*(v3->getMax()-v3->getMin())/v3->getBins());
	    _Norm+=evaluate();
	    //cout<<"_Norm"<<_Norm<<endl;
 	  }
 	}else _Norm+=evaluate();
      }    
    }else _Norm+=evaluate();
  }
  _Norm*=((v1->getMax()-v1->getMin())/v1->getBins());
  if(v2)_Norm*=((v2->getMax()-v2->getMin())/v2->getBins());
  if(v3)_Norm*=((v3->getMax()-v3->getMin())/v3->getBins());

  v1->setVal(origv1);
  if(v2)v2->setVal(origv2);
  if(v3)v3->setVal(origv3);
  
  //record new parameter values
  _parsetIter->Reset();
  RooRealVar* par;
  Int_t i(0);
  while((par=(RooRealVar*)_parsetIter->Next())){
    _param[i]=par->getVal();
    i++;
  }
    
}


Double_t DRecoProdPdf::getHistError() const {
  //This method returns the fractional erro on the total pdf from the fractional error on the individual pdfs
  //if C=A*B then errC=sigmaC/C=sqrt(sigmaA*sigmaA/A*A + sigmaB*sigmaB/B*B)=sqrt(errA*errA + errB*errB)
  
  Double_t histerror=0.;
  RooAbsPdf* pdf;
  _pdfIter->Reset() ;
  while((pdf=(RooAbsPdf*)_pdfIter->Next())) {
    Double_t pdfValErr=0.;
    if(TString(pdf->ClassName())=="DRecoAddPdf")pdfValErr = ((DRecoAddPdf*)pdf)->getHistError() ;
    if(TString(pdf->ClassName())=="DRecoHistPdf")pdfValErr = ((DRecoHistPdf*)pdf)->getHistError() ;
    if(TString(pdf->ClassName())=="DRecoProdPdf")pdfValErr = ((DRecoProdPdf*)pdf)->getHistError() ;
    if(TString(pdf->ClassName())=="DRecoTranslationPdf")pdfValErr = ((DRecoTranslationPdf*)pdf)->getHistError() ;
    //cout<<"DRecoProdPdf::"<<pdfValErr<<endl;
    histerror+=pdfValErr*pdfValErr;
  }
 
  return sqrt(histerror);
}




TH1* DRecoProdPdf::CreateHistogram(const char *name) const 
{
  TH1* histo=NULL;

  _normvarIter->Reset();
  RooRealVar* x1 =(RooRealVar*)_normvarIter->Next();
  RooRealVar* x2 =(RooRealVar*)_normvarIter->Next();
  RooRealVar* x3 =(RooRealVar*)_normvarIter->Next();

  if(x3&&x2&&x1&&!histo){//3D -----------------------------------------------------------------

    histo=new TH3F(name,GetTitle(),
			x1->getBins(),x1->getMin(),x1->getMax(),
			x2->getBins(),x2->getMin(),x2->getMax(),
			x3->getBins(),x3->getMin(),x3->getMax());


    Double_t origx1=x1->getVal();
    Double_t origx2=x2->getVal();
    Double_t origx3=x3->getVal();
    for(Int_t b1=0;b1<x1->getBins();b1++){
      x1->setVal(x1->getMin()+(b1+.5)*(x1->getMax()-x1->getMin())/x1->getBins());
      for(Int_t b2=0;b2<x2->getBins();b2++){
	x2->setVal(x2->getMin()+(b2+.5)*(x2->getMax()-x2->getMin())/x2->getBins());
	for(Int_t b3=0;b3<x3->getBins();b3++){
	  x3->setVal(x3->getMin()+(b3+.5)*(x3->getMax()-x3->getMin())/x3->getBins());
	  Float_t pVal=getVal(_normvars)
	    *((x1->getMax()-x1->getMin())/x1->getBins())
	    *((x2->getMax()-x2->getMin())/x2->getBins())
	    *((x3->getMax()-x3->getMin())/x3->getBins());
	  histo->SetBinContent(b1+1,b2+1,b3+1,pVal);
	  histo->SetBinError(b1+1,b2+1,b3+1,getHistError()*pVal);
	}
      }    
    }
    x1->setVal(origx1);
    x2->setVal(origx2);    
    x2->setVal(origx3);    

  }


  

  if(x2&&x1&&!histo){//2D -----------------------------------------------------------------
    histo=new TH2F(name,GetTitle(),
			x1->getBins(),x1->getMin(),x1->getMax(),
			x2->getBins(),x2->getMin(),x2->getMax());

    
    Double_t origx1=x1->getVal();
    Double_t origx2=x2->getVal();
    for(Int_t b1=0;b1<x1->getBins();b1++){
      x1->setVal(x1->getMin()+(b1+.5)*(x1->getMax()-x1->getMin())/x1->getBins());
      for(Int_t b2=0;b2<x2->getBins();b2++){
	x2->setVal(x2->getMin()+(b2+.5)*(x2->getMax()-x2->getMin())/x2->getBins());
	Float_t pVal=getVal(_normvars)
	  *((x1->getMax()-x1->getMin())/x1->getBins())
	  *((x2->getMax()-x2->getMin())/x2->getBins());
	histo->SetBinContent(b1+1,b2+1,pVal);
	histo->SetBinError(b1+1,b2+1,getHistError()*pVal);
      }    
    }
    x1->setVal(origx1);
    x2->setVal(origx2);    
  }



  if(x1&&!histo){//1D -----------------------------------------------------------------
    histo=new TH1F(name,GetTitle(),
			x1->getBins(),x1->getMin(),x1->getMax());

    Double_t origx1=x1->getVal();
    for(Int_t b1=0;b1<x1->getBins();b1++){
      x1->setVal(x1->getMin()+(b1+.5)*(x1->getMax()-x1->getMin())/x1->getBins());
      Float_t pVal=getVal(_normvars)
	*((x1->getMax()-x1->getMin())/x1->getBins());
      histo->SetBinContent(b1+1,pVal);
      histo->SetBinError(b1+1,getHistError()*pVal);
    }
    x1->setVal(origx1);

  }

  
  if(!histo)cout<<"DRecoAddPdf::createHistogram Wrong dimensionality"<<endl;


  return histo ;
}
