
#include "RooFit.h"
#include "Riostream.h"
#include <math.h>
#include "RooPolyExpPdf.h"
#include "RooAbsReal.h"
#include "RooRealVar.h"

ClassImp(RooPolyExpPdf)

RooPolyExpPdf::RooPolyExpPdf(const char *name, const char *title,RooAbsReal& _x,RooArgList &_coeflist) :
  RooAbsPdf(name,title),
  x("x","Dependent",this,_x),
  coefList("coefficients","List of coefficients",this),
  x0(0)
{ 
  
  TIterator* _coefIter = _coeflist.createIterator() ;
  RooAbsReal* _coef ;
  _coefIter->Next();//separate x variable
  x0=(RooAbsReal*)_coefIter->Next();//separate first coefficient
  while((_coef = (RooAbsReal*)_coefIter->Next())){
    coefList.add(*_coef);
  }

  coefIter = coefList.createIterator() ;
}

RooPolyExpPdf::RooPolyExpPdf(const RooPolyExpPdf& other, const char* name) : 
  RooAbsPdf(other,name),x("x",this,other.x),coefList("coefficients",this,other.coefList),
  x0(other.x0)
{
  coefIter = coefList.createIterator() ;
}

Double_t RooPolyExpPdf::EvalSum(Double_t y) const{
  coefIter->Reset();
  RooAbsReal* c=0;
  Double_t sum=0;
  Int_t power=0;
  Double_t ypower=1;
  while((c=(RooAbsReal*)coefIter->Next())){//sum starts from 0
    ypower=1;
    for(Int_t i=0;i<power;i++)ypower*=y;
    sum+=c->getVal()*ypower;
    power++;
  }
	
  return sum;
}
Double_t RooPolyExpPdf::EvalSumN(Double_t y) const{
  coefIter->Reset() ;
  RooAbsReal* c=0;
  Double_t sum=0;
  Int_t power=0;//sum starts from 1
  Double_t ypower=1;
  while((c=(RooAbsReal*)coefIter->Next())){
    ypower=1;
    for(Int_t i=0;i<power;i++)ypower*=y;
    sum+=power*c->getVal()*ypower;
    power++;
  }
	
  return sum;

}

Double_t RooPolyExpPdf::evaluate() const
{

  if(!x0)return 0;
  if(x0->getVal()==0)return 1;//something is wrong here
  
  ///---------------------------
  //calculate coef2
  //----------------------------
  Double_t Coef2=(
		  (1-exp(EvalSum(x0->getVal())))
		  +EvalSumN(x0->getVal())*(1+exp(EvalSum(x0->getVal())))
		  )/(x0->getVal()*x0->getVal());
  

  ///---------------------------
  //calculate coef1
  //----------------------------
  Double_t Coef1=(
		  Coef2*x0->getVal()*(EvalSumN(x0->getVal())-2)
		  +EvalSumN(x0->getVal())/x0->getVal()
		  )/(1-EvalSumN(x0->getVal()));
  


  ///calculate the polynomial
  if(x<=x0->getVal())return (1+Coef1*x+Coef2*x*x);
  return exp(EvalSum(x));
}

