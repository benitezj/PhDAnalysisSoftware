/*****************************************************************************
 *****************************************************************************/



#include "RooFit.h"
#include "RooMsgService.h"

#include "TIterator.h"
#include "TIterator.h"
#include "TList.h"
#include "DRecoilToolsUser/NtupleCode/DRecoAddPdf.hh"
#include "DRecoilToolsUser/NtupleCode/DRecoHistPdf.hh"
#include "DRecoilToolsUser/NtupleCode/DRecoProdPdf.hh"
#include "DRecoilToolsUser/NtupleCode/DRecoTranslationPdf.hh"
#include "RooAddPdf.h"
#include "RooDataSet.h"
#include "RooRealProxy.h"
#include "RooPlot.h"
#include "RooRealVar.h"
#include "RooAddGenContext.h"
#include "RooRealConstant.h"
#include "RooNameReg.h"
#include "RooMsgService.h"

#include "Riostream.h"

#include "TString.h"
#include "TH3F.h"
#include "TH2F.h"
#include "TH1F.h"


ClassImp(DRecoAddPdf)
;


DRecoAddPdf::DRecoAddPdf(const char *name, const char *title,RooArgSet& vars,
			 RooAbsPdf& pdf1, RooAbsPdf& pdf2, RooAbsReal& coef1) : 
  RooAbsPdf(name,title),
  _pdfList("pdfs","List of PDFs",this),
  _coefList("coefficients","List of coefficients",this),
  _normvars(vars)
{
  // Special constructor with two PDFs and one coefficient (most frequent use case)

  _pdfIter  = _pdfList.createIterator() ;
  _coefIter = _coefList.createIterator() ;

  _pdfList.add(pdf1) ;  
  _pdfList.add(pdf2) ;
  _coefList.add(coef1) ; 
  
}


DRecoAddPdf::DRecoAddPdf(const char *name, const char *title,RooArgSet& vars, const RooArgList& pdfList, const RooArgList& coefList) :
  RooAbsPdf(name,title),
  _pdfList("pdfs","List of PDFs",this),
  _coefList("coefficients","List of coefficients",this),
  _normvars(vars)
{ 
  // Generic constructor from list of PDFs and list of coefficients.
  // Each pdf list element (i) is paired with coefficient list element (i).
  // The number of coefficients must be either equal to the number of PDFs,
  // in which case extended MLL fitting is enabled, or be one less.
  //
  // All PDFs must inherit from RooAbsPdf. All coefficients must inherit from RooAbsReal

  if (pdfList.getSize()>coefList.getSize()+1||pdfList.getSize()<coefList.getSize()) {
    coutE(InputArguments) << "DRecoAddPdf::DRecoAddPdf(" << GetName() 
			  << ") number of pdfs and coefficients inconsistent, must have Npdf=Ncoef or Npdf=Ncoef+1" << endl ;
    assert(0) ;
  }

  _pdfIter  = _pdfList.createIterator() ;
  _coefIter = _coefList.createIterator() ;
 
  // Constructor with N PDFs and N or N-1 coefs
  TIterator* pdfIter = pdfList.createIterator() ;
  TIterator* coefIter = coefList.createIterator() ;
  RooAbsPdf* pdf ;
  RooAbsReal* coef ;

  while((coef = (RooAbsPdf*)coefIter->Next())) {
    pdf = (RooAbsPdf*) pdfIter->Next() ;
    if (!pdf) {
      coutE(InputArguments) << "DRecoAddPdf::DRecoAddPdf(" << GetName() 
			    << ") number of pdfs and coefficients inconsistent, must have Npdf=Ncoef or Npdf=Ncoef+1" << endl ;
      assert(0) ;
    }
    if (!dynamic_cast<RooAbsReal*>(coef)) {
      coutE(InputArguments) << "DRecoAddPdf::DRecoAddPdf(" << GetName() << ") coefficient " << coef->GetName() << " is not of type RooAbsReal, ignored" << endl ;
      continue ;
    }
    if (!dynamic_cast<RooAbsReal*>(pdf)) {
      coutE(InputArguments) << "DRecoAddPdf::DRecoAddPdf(" << GetName() << ") pdf " << pdf->GetName() << " is not of type RooAbsPdf, ignored" << endl ;
      continue ;
    }
    _pdfList.add(*pdf) ;
    _coefList.add(*coef) ;    
  }

  pdf = (RooAbsPdf*) pdfIter->Next() ;
  if (pdf) {//add the last pdf
    if (!dynamic_cast<RooAbsReal*>(pdf)) {
      coutE(InputArguments) << "DRecoAddPdf::DRecoAddPdf(" << GetName() << ") last pdf " << pdf->GetName() << " is not of type RooAbsPdf, fatal error" << endl ;
      assert(0) ;
    }
    _pdfList.add(*pdf) ;  
  } 
  delete pdfIter ;
  delete coefIter  ;
  

}




DRecoAddPdf::DRecoAddPdf(const DRecoAddPdf& other, const char* name) :
  RooAbsPdf(other,name),
  _pdfList("pdfs",this,other._pdfList),
  _coefList("coefficients",this,other._coefList),
  _normvars(other._normvars)
{
  // Copy constructor

  _pdfIter  = _pdfList.createIterator() ;
  _coefIter = _coefList.createIterator() ; 

}


DRecoAddPdf::~DRecoAddPdf()
{
  // Destructor
  delete _pdfIter ;
  delete _coefIter ;

}




Double_t DRecoAddPdf::getVal(const RooArgSet* set) const
{
  return evaluate();
  //return RooAbsPdf::getVal(set);
}
 


Double_t DRecoAddPdf::evaluate() const 
{
  
  
  if(_pdfList.getSize()==_coefList.getSize()+1){
    //update the coefficients
    Int_t k(0);
    Double_t lastCoef(1) ;
    _coefIter->Reset() ;
    RooAbsReal* c;
    while((c=(RooAbsReal*)_coefIter->Next())){
      _coefCache[k] = c->getVal() ;//RooAbsPdf nset
      lastCoef -= _coefCache[k];
      k++;
    }	
    _coefCache[k] = lastCoef ;

  }

  if(_pdfList.getSize()==_coefList.getSize()){
    //update the coefficients
    Int_t k(0);
    Double_t CoefNorm(0) ;
    _coefIter->Reset() ;
    RooAbsReal* c;
    while((c=(RooAbsReal*)_coefIter->Next())){
      _coefCache[k] = c->getVal() ;//RooAbsPdf nset
      CoefNorm += _coefCache[k];
      k++;
    }
    if(CoefNorm!=1.){
      _coefIter->Reset() ; k=0; 
      while((c=(RooAbsReal*)_coefIter->Next())){
	_coefCache[k] =  _coefCache[k]/CoefNorm;//normalize to 1.
	k++;
      }	
    }    
  }
  
  // Do running sum of coef/pdf pairs, calculate lastCoef.
  _pdfIter->Reset() ;
  RooAbsPdf* pdf ;
  Double_t value(0) ;
  Int_t i(0) ;
  while((pdf = (RooAbsPdf*)_pdfIter->Next())) {
    Double_t pdfVal = pdf->getVal(&_normvars) ;//
    value += pdfVal*_coefCache[i];
    //if(TString(GetName())=="TotRSBkgPdf")
    //cout<<"DRecoAddPdf::"<<GetName()<<" p="<<pdf->GetName()<<"="<<pdfVal<<"  c="<<_coefCache[i]<<endl;
    i++ ;
  }


  return value ;
}

Double_t DRecoAddPdf::getHistError() const 
{
  //This method returns the fractional error of the total pdf from the fractional error on the individual pdfs
  //if C=A+B then errC = sigmaC/C =  sqrt( sigmaA*sigmaA + sigmaB*sigmaB ) / C = sqrt(A*A*errA*errA +B*B*errB*errB) / (A+B)

  
  //update the coefficients
  if(_pdfList.getSize()==_coefList.getSize()){
    Int_t k(0);
    Double_t lastCoef(1) ;
    _coefIter->Reset() ;
    RooAbsReal* c;
    while((c=(RooAbsReal*)_coefIter->Next())){
      _coefCache[k] = c->getVal() ;//RooAbsPdf nset
      lastCoef -= _coefCache[k];
      k++;
    }	
    _coefCache[k] = lastCoef ;
  }
  if(_pdfList.getSize()==_coefList.getSize()){
    //update the coefficients
    Int_t k(0);
    Double_t CoefNorm(0) ;
    _coefIter->Reset() ;
    RooAbsReal* c;
    while((c=(RooAbsReal*)_coefIter->Next())){
      _coefCache[k] = c->getVal() ;//RooAbsPdf nset
      CoefNorm += _coefCache[k];
      k++;
    }
    if(CoefNorm!=1.){
      _coefIter->Reset() ; k=0; 
      while((c=(RooAbsReal*)_coefIter->Next())){
	_coefCache[k] =  _coefCache[k]/CoefNorm;//normalize to 1.
	k++;
      }	
    }
    
  }


  // Do running sum of coef/pdf pairs, calculate lastCoef.
  _pdfIter->Reset() ;
  _coefIter->Reset() ;
  RooAbsPdf* pdf ;

  Double_t value(0) ;
  Double_t normsum(0) ;
  Int_t i(0) ;
  while((pdf = (RooAbsPdf*)_pdfIter->Next())) {
    if (_coefCache[i]!=0.) {     
      //if (pdf->isSelectedComp()) {
      Double_t pdfVal = pdf->getVal(&_normvars);
      Double_t pdfValErr = 0.;
      //cout<<pdf->ClassName()<<" "<<pdf->GetName()<<endl;
      if(TString(pdf->ClassName())=="DRecoAddPdf")pdfValErr = ((DRecoAddPdf*)pdf)->getHistError() ;
      if(TString(pdf->ClassName())=="DRecoHistPdf")pdfValErr = ((DRecoHistPdf*)pdf)->getHistError() ;
      if(TString(pdf->ClassName())=="DRecoProdPdf")pdfValErr = ((DRecoProdPdf*)pdf)->getHistError() ;
      if(TString(pdf->ClassName())=="DRecoTranslationPdf")pdfValErr = ((DRecoTranslationPdf*)pdf)->getHistError() ;
      value += pdfValErr*_coefCache[i]*pdfVal * pdfValErr*_coefCache[i]*pdfVal;
      normsum +=_coefCache[i]*pdfVal;
      //}
    }
    i++ ;
  }


  if(value<0.)value=-value;
  if(normsum==0.)return 0.;
  return sqrt(value)/normsum ;//
}



TH1* DRecoAddPdf::CreateHistogram(const char *name) const 
{
  TH1* histo=NULL;

  TIterator* variter=_normvars.createIterator();
  RooRealVar* x1 =(RooRealVar*)variter->Next();
  RooRealVar* x2 =(RooRealVar*)variter->Next();
  RooRealVar* x3 =(RooRealVar*)variter->Next();

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
	  Float_t pVal=getVal(&_normvars)
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
	Float_t pVal=getVal(&_normvars)
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
      Float_t pVal=getVal(&_normvars)
	*((x1->getMax()-x1->getMin())/x1->getBins());
      histo->SetBinContent(b1+1,pVal);
      histo->SetBinError(b1+1,getHistError()*pVal);
    }
    x1->setVal(origx1);

  }

  
  if(!histo)cout<<"DRecoAddPdf::createHistogram Wrong dimensionality"<<endl;

  delete variter;

  return histo ;
}


