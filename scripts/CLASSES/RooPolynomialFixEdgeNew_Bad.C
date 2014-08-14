
#include "RooFit.h"

#include "Riostream.h"
#include "Riostream.h"
#include <math.h>

#include "RooPolynomialFixEdgeNew.h"
#include "RooAbsReal.h"
#include "RooRealVar.h"

ClassImp(RooPolynomialFixEdgeNew);
  
RooPolynomialFixEdgeNew::RooPolynomialFixEdgeNew(const char *name, const char *title,RooAbsReal& _x,TH1*Histo,RooFormula* Yield,RooAbsReal& _c2,RooAbsReal& _c3,RooAbsReal& _c4,RooAbsReal& _c5,RooAbsReal& _c6,RooAbsReal& _c7,RooAbsReal& _c8,RooAbsReal& _c9,RooAbsReal& _c10) :
  RooAbsPdf(name,title),
  x("x","Dependent",this,_x),
  histo(Histo),
  yield(Yield),
  c2("c2","c2",this,_c2),
  c3("c3","c3",this,_c3),
  c4("c4","c4",this,_c4),
  c5("c5","c5",this,_c5),
  c6("c6","c6",this,_c6),
  c7("c7","c7",this,_c7),
  c8("c8","c8",this,_c8),
  c9("c9","c9",this,_c9),
  c10("c10","c10",this,_c10)
{
  if(!InitParameters())
    cout<<"Warning:"<<name<<" paramaters are not right at initialization"<<endl<<endl<<endl;
}

RooPolynomialFixEdgeNew::RooPolynomialFixEdgeNew(const RooPolynomialFixEdgeNew& other, const char* name) : 
  RooAbsPdf(other,name),x("x",this,other.x),
  histo(other.histo),yield(other.yield),
  c2("c2",this,other.c2),c3("c3",this,other.c3),c4("c4",this,other.c4),c5("c5",this,other.c5),
  c6("c6",this,other.c6),c7("c7",this,other.c7),c8("c8",this,other.c8),
  c9("c9",this,other.c9),c10("c10",this,other.c10)
{
  if(!InitParameters())
    cout<<"Warning:"<<name<<" paramaters are not right at initialization"<<endl<<endl<<endl;
}

Bool_t RooPolynomialFixEdgeNew::InitParameters(){

  if(!histo){cout<<" Histogram is NULL"<<endl; return 0;}

  x0=histo->GetBinCenter(1)-histo->GetBinWidth(1)/2.;
//   y0=(histo->GetBinContent(1)
//       +histo->GetBinContent(2)
//       +histo->GetBinContent(3))/(3.*histo->Integral()*histo->GetBinWidth(1));//*histo->Integral()
  y0=histo->GetBinContent(1)/(histo->GetBinWidth(1)*histo->Integral());
  x1=histo->GetBinCenter(histo->GetNbinsX())+histo->GetBinWidth(1)/2.;
//   y1=(histo->GetBinContent(histo->GetNbinsX())
//       +histo->GetBinContent(histo->GetNbinsX()-1)
//       +histo->GetBinContent(histo->GetNbinsX()-2))/(3.*histo->Integral()*histo->GetBinWidth(1));//*histo->Integral()
  y1=histo->GetBinContent(histo->GetNbinsX())/(histo->GetBinWidth(1)*histo->Integral());
    

  //fix background to average of first and last 3 bins    
  delta=x1-x0;

  x0pow2=pow(x0,2);
  x0pow3=pow(x0,3);
  x0pow4=pow(x0,4);
  x0pow5=pow(x0,5);
  x0pow6=pow(x0,6);
  x0pow7=pow(x0,7);
  x0pow8=pow(x0,8);
  x0pow9=pow(x0,9);
  x0pow10=pow(x0,10);
  x0pow11=pow(x0,11);

  x1pow2=pow(x1,2);
  x1pow3=pow(x1,3);
  x1pow4=pow(x1,4);
  x1pow5=pow(x1,5);
  x1pow6=pow(x1,6);
  x1pow7=pow(x1,7);
  x1pow8=pow(x1,8);
  x1pow9=pow(x1,9);
  x1pow10=pow(x1,10);
  x1pow11=pow(x1,11);


  

  return 1;
}

Double_t RooPolynomialFixEdgeNew::evaluate() const
{

  if(yield->eval()==0)return 0;

  Float_t f0=y0/yield->eval();
  Float_t f1=y1/yield->eval();


  ///---------------------------
  //calculate coef1
  //----------------------------
  Double_t Coef1=(c2*((1-f0*delta)*(f1*(x1pow3-x0pow3)/3-x1pow2) - (1-f1*delta)*(f0*(x1pow3-x0pow3)/3-x0pow2)) 
		  +c3*((1-f0*delta)*(f1*(x1pow4-x0pow4)/4-x1pow3) - (1-f1*delta)*(f0*(x1pow4-x0pow4)/4-x0pow3))
		  +c4*((1-f0*delta)*(f1*(x1pow5-x0pow5)/5-x1pow4) - (1-f1*delta)*(f0*(x1pow5-x0pow5)/5-x0pow4))
		  +c5*((1-f0*delta)*(f1*(x1pow6-x0pow6)/6-x1pow5) - (1-f1*delta)*(f0*(x1pow6-x0pow6)/6-x0pow5))
		  +c6*((1-f0*delta)*(f1*(x1pow7-x0pow7)/7-x1pow6) - (1-f1*delta)*(f0*(x1pow7-x0pow7)/7-x0pow6))
		  +c7*((1-f0*delta)*(f1*(x1pow8-x0pow8)/8-x1pow7) - (1-f1*delta)*(f0*(x1pow8-x0pow8)/8-x0pow7))
		  +c8*((1-f0*delta)*(f1*(x1pow9-x0pow9)/9-x1pow8) - (1-f1*delta)*(f0*(x1pow9-x0pow9)/9-x0pow8))
		  +c9*((1-f0*delta)*(f1*(x1pow10-x0pow10)/10-x1pow9) - (1-f1*delta)*(f0*(x1pow10-x0pow10)/10-x0pow9))
		  +c10*((1-f0*delta)*(f1*(x1pow11-x0pow11)/11-x1pow10) - (1-f1*delta)*(f0*(x1pow11-x0pow11)/11-x0pow10))
		  )/((1-f1*delta)*(f0*(x1pow2-x0pow2)/2-x0)-(1-f1*delta)*(f1*(x1pow2-x0pow2)/2-x1));
  




  //------------------------
  //calculate Coef0
  //------------------------
  Double_t Coef0=(Coef1*(f0*(x1pow2-x0pow2)/2-x0)
		  +c2*(f0*(x1pow3-x0pow3)/3-x0pow2)
		  +c3*(f0*(x1pow4-x0pow4)/4-x0pow3)
		  +c4*(f0*(x1pow5-x0pow5)/5-x0pow4)
		  +c5*(f0*(x1pow6-x0pow6)/6-x0pow5)
		  +c6*(f0*(x1pow7-x0pow7)/7-x0pow6)
		  +c7*(f0*(x1pow8-x0pow8)/8-x0pow7)
		  +c8*(f0*(x1pow9-x0pow9)/9-x0pow8)
		  +c9*(f0*(x1pow10-x0pow10)/10-x0pow9)
		  +c10*(f0*(x1pow11-x0pow11)/11-x0pow10)
		 )/(1-f0*delta);    
  
 
  //calculate normalization
  Float_t norm=(Coef0*delta
		+Coef1*(x1pow2-x0pow2)/2
		+c2*(x1pow3-x0pow3)/3
		+c3*(x1pow4-x0pow4)/4
		+c4*(x1pow5-x0pow5)/5
		+c5*(x1pow6-x0pow6)/6
		+c6*(x1pow7-x0pow7)/7
		+c7*(x1pow8-x0pow8)/8
		+c8*(x1pow9-x0pow9)/9
		+c9*(x1pow10-x0pow10)/10
		+c10*(x1pow11-x0pow11)/11);
  
  if(norm==0)return 0;
  //cout<<x<<" "<<yield->eval()<<" "<<x0<<" "<<y0<<" "<<f0<<" "<<x1<<" "<<y1<<" "<<f1<<" "<<Coef1<<" "<<Coef0<<" "<<norm<<endl;

  ///calculate the polynomial
  return (Coef0
	  +Coef1*x
	  +c2*x*x
	  +c3*x*x*x
	  +c4*x*x*x*x
	  +c5*x*x*x*x*x
	  +c6*x*x*x*x*x*x
	  +c7*x*x*x*x*x*x*x
	  +c8*x*x*x*x*x*x*x*x
	  +c9*x*x*x*x*x*x*x*x*x
	  +c10*x*x*x*x*x*x*x*x*x*x
	  )/norm;
}

