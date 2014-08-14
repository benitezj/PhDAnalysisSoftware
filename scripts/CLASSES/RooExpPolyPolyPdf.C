
#include "RooFit.h"
#include "Riostream.h"
#include <math.h>
#include "RooExpPolyPolyPdf.h"
#include "RooAbsReal.h"
#include "RooRealVar.h"

ClassImp(RooExpPolyPolyPdf)

RooExpPolyPolyPdf::RooExpPolyPolyPdf(const char *name, const char *title,RooArgList &_coeflist) :
  RooAbsPdf(name,title),
  x("x","x",this,*((RooAbsReal*)_coeflist.at(0))),
  coefList("coefficients","List of coefficients",this),
  coefList2("coefficients2","List of coefficients2",this),
  x0("x0","x0",this,*((RooAbsReal*)_coeflist.at(1)))
{                                                                                                                                      

  TIterator* _coefIter = _coeflist.createIterator() ;
  
  RooAbsReal* X=(RooAbsReal*)_coefIter->Next();//separate x variable
  if(X) cout<<"RooExpPolyPolyPdf::"<<" x="<<X->GetName()<<endl;
  else cout<<"RooExpPolyPolyPdf Error---------------: No x variable"<<endl;

  RooAbsReal* X0=(RooAbsReal*)_coefIter->Next();//separate first coefficient
  if(X0) cout<<"RooExpPolyPolyPdf::"<<" X0="<<X0->GetName()<<endl;
  else cout<<"RooExpPolyPolyPdf Error---------------: No X0"<<endl;
 

  //--------------------
  //separate remaining list into two halves: c's and d's
  //-----------------
  
  RooAbsReal* _coef;

  //-----------------
  //First count how many we have left
  Int_t N=0;
  while(_coefIter->Next())
    N++;
 
  Int_t Nc=0;
  Int_t Nd=0;
  if(N%2==0){Nc=N/2;Nd=N/2;}
  if(N%2==1){Nc=N/2+1;Nd=N/2;}
  
  _coefIter->Reset();
  _coefIter->Next();//skip x variable
  _coefIter->Next();//skip first : x0
  //---------------------------
  //put first half into first list
  N=0;
  while((_coef = (RooAbsReal*)_coefIter->Next())){
    coefList.add(*_coef);
    N++;
    cout<<" RooExpPolyPolyPdf: c"<<N<<" ="<<_coef->GetName()<<" Constant="<<_coef->isConstant()<<endl;
    if(N==Nc)break;
  }
  
  //---------------------------
  //put second half into second list
  N=0;
  while((_coef = (RooAbsReal*)_coefIter->Next())){
    coefList2.add(*_coef);
    N++; 
    cout<<" RooExpPolyPolyPdf: d"<<N<<" ="<<_coef->GetName()<<" Constant="<<_coef->isConstant()<<endl;
    if(N==Nd)break;
  }
  
  coefIter = coefList.createIterator() ;
  coefIter2 = coefList2.createIterator() ;
}

RooExpPolyPolyPdf::RooExpPolyPolyPdf(const RooExpPolyPolyPdf& other, const char* name) : 
  RooAbsPdf(other,name),x("x",this,other.x),
  coefList("coefficients",this,other.coefList),
  coefList2("coefficients2",this,other.coefList2),
  x0(other.x0)
{
  coefIter = coefList.createIterator();
  coefIter2 = coefList2.createIterator();
}

Double_t RooExpPolyPolyPdf::EvalSum(Double_t y) const{
  //This calculates Sum(c_n * y^n) n=1->Nc
  
  coefIter->Reset();
  RooAbsReal* c=0;
  Double_t sum=0;
  Int_t n=1;
  Double_t yn=1;
  while((c=(RooAbsReal*)coefIter->Next())){//sum starts from 0
    yn=1;for(Int_t i=0;i<n;i++)yn*=y;//compute y^n

    sum+=c->getVal()*yn;
    n++;
  }
	
  return sum;
}
Double_t RooExpPolyPolyPdf::EvalSumN(Double_t y) const{
  //This calculates Sum((n-1)* c_n * y^n) n=1->Nc
  
  coefIter->Reset();
  RooAbsReal* c=0;
  Double_t sum=0;
  Int_t n=1;//sum starts from 1
  Double_t yn=1;
  while((c=(RooAbsReal*)coefIter->Next())){
    yn=1;for(Int_t i=0;i<n;i++)yn*=y;//compute y^n

    sum+=n*c->getVal()*yn;
    n++;
  }
	
  return sum;

}

Double_t RooExpPolyPolyPdf::EvalSum2(Double_t y) const{
  //This calculates Sum(c_n * y^(n+2)) n=1->Nd
  
  coefIter2->Reset();
  RooAbsReal* c=0;
  Double_t sum=0;
  Int_t n=2;
  Double_t yn=1;
  while((c=(RooAbsReal*)coefIter2->Next())){
    yn=1;for(Int_t i=0;i<n;i++)yn*=y;//compute y^(n+2)

    sum+=c->getVal()*yn;
    n++;
  }
	
  return sum;
}
Double_t RooExpPolyPolyPdf::EvalSumN2(Double_t y) const{
  //This calculates Sum((n-1)* c_n * y^(n+2)) n=1->Nd
  
  coefIter2->Reset();
  RooAbsReal* c=0;
  Double_t sum=0;
  Int_t n=2;//sum starts from 1
  Double_t yn=1;
  while((c=(RooAbsReal*)coefIter2->Next())){
    yn=1;for(Int_t i=0;i<n;i++)yn*=y;//compute y^(n+2)

    sum+=n*c->getVal()*yn;
    n++;
  }
	
  return sum;
}

Double_t RooExpPolyPolyPdf::evaluate() const
{ 
  
  if(x0==0)return 1;
 
  //
  Double_t Poly=0;
  if(x<=x0){
    Poly=EvalSum(x);
    return exp(Poly);
  }

 
  ///---------------------------
  //calculate coef2
  //----------------------------
  Double_t Coef1=(EvalSumN(x0)-EvalSumN2(x0))/x0;
  
  ///---------------------------
  //calculate coef1
  //----------------------------
  Double_t Coef0=EvalSum(x0)-EvalSum2(x0)-Coef1*x0;
  
  
  Poly=Coef0+Coef1*x+EvalSum2(x);
  return exp(Poly);
  
}

