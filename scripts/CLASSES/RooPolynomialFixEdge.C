
#include "RooFit.h"

#include "Riostream.h"
#include "Riostream.h"
#include <math.h>

#include "RooPolynomialFixEdge.h"
#include "RooAbsReal.h"
#include "RooRealVar.h"

ClassImp(RooPolynomialFixEdge)

RooPolynomialFixEdge::RooPolynomialFixEdge(const char *name, const char *title,RooAbsReal& _x,
					   RooAbsReal& _x0,RooAbsReal& _y0,RooAbsReal& _x1,RooAbsReal& _y1,
					   RooAbsReal& _yield,RooAbsReal& _c3,RooAbsReal& _c4,RooAbsReal& _c5,
					   RooAbsReal& _c6,RooAbsReal& _c7,RooAbsReal& _c8,RooAbsReal& _c9,RooAbsReal& _c10) :
  RooAbsPdf(name,title),
  x("x","Dependent",this,_x),
  x0("x0","x0",this,_x0),
  y0("y0","y0",this,_y0),
  x1("x1","x1",this,_x1),
  y1("y1","y1",this,_y1),
  yield("yield","yield",this,_yield),
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
    cout<<"Warning:"<<name<<" paramaters are not right at initialization"<<endl;
}

RooPolynomialFixEdge::RooPolynomialFixEdge(const RooPolynomialFixEdge& other, const char* name) : 
  RooAbsPdf(other,name),x("x",this,other.x),
  x0("x0",this,other.x0),y0("y0",this,other.y0),
  x1("x1",this,other.x1),y1("y1",this,other.y1),
  yield("yield",this,other.yield),
  c3("c3",this,other.c3),c4("c4",this,other.c4),c5("c5",this,other.c5),
  c6("c6",this,other.c6),c7("c7",this,other.c7),c8("c8",this,other.c8),
  c9("c9",this,other.c9),c10("c10",this,other.c10)
{
  if(!InitParameters())
    cout<<"Warning:"<<name<<" paramaters are not right at initialization"<<endl;
}

Bool_t RooPolynomialFixEdge::InitParameters(){

  //fix background to average of first and last 3 bins    
  delta=x1-x0;
  delta2=x1*x1-x0*x0;
  delta3=x1*x1*x1-x0*x0*x0;
  diffx0=delta*x0-delta2/2.;
  if(diffx0==0){cout<<"The diffx0 is 0"<<endl;return 0;}
  diffx1=delta*x1-delta2/2.;
  if(diffx1==0){cout<<"The diffx1 is 0"<<endl;return 0;}
  denom=(delta*x1*x1-delta3/3.)*diffx0-(delta*x0*x0-delta3/3.)*diffx1;
  if(denom==0){cout<<"The denominator is 0"<<endl;return 0;}
  
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

Double_t RooPolynomialFixEdge::evaluate() const
{
  ///---------------------------
  //calculate coef2
  //----------------------------
  Double_t Coef2=(
		 (delta*y1-yield)*diffx0 
		 - (delta*y0-yield)*diffx1		 
		 -(c3*(delta*x1pow3-(x1pow4-x0pow4)/4)
		   +c4*(delta*x1pow4-(x1pow5-x0pow5)/5)
		   +c5*(delta*x1pow5-(x1pow6-x0pow6)/6)
		   +c6*(delta*x1pow6-(x1pow7-x0pow7)/7)
		   +c7*(delta*x1pow7-(x1pow8-x0pow8)/8)
		   +c8*(delta*x1pow8-(x1pow9-x0pow9)/9)
		   +c9*(delta*x1pow9-(x1pow10-x0pow10)/10)
		   +c10*(delta*x1pow10-(x1pow11-x0pow11)/11)
		   )*diffx0
		 
		 +(c3*(delta*x0pow3-(x1pow4-x0pow4)/4)
		   +c4*(delta*x0pow4-(x1pow5-x0pow5)/5)
		   +c5*(delta*x0pow5-(x1pow6-x0pow6)/6)
		   +c6*(delta*x0pow6-(x1pow7-x0pow7)/7)
		   +c7*(delta*x0pow7-(x1pow8-x0pow8)/8)
		   +c8*(delta*x0pow8-(x1pow9-x0pow9)/9)
		   +c9*(delta*x0pow9-(x1pow9-x0pow9)/9)
		   +c10*(delta*x0pow10-(x1pow10-x0pow10)/10)
		   )*diffx1
		 )/denom;
  

  ///---------------------------
  //calculate coef1
  //----------------------------
  Double_t Coef1=(
		 delta*y0-yield
		 -(Coef2*(delta*x0pow2-(x1pow3-x0pow3)/3) 
		   +c3*(delta*x0pow3-(x1pow4-x0pow4)/4)
		   +c4*(delta*x0pow4-(x1pow5-x0pow5)/5)
		   +c5*(delta*x0pow5-(x1pow6-x0pow6)/6)
		   +c6*(delta*x0pow6-(x1pow7-x0pow7)/7)
		   +c7*(delta*x0pow7-(x1pow8-x0pow8)/8)
		   +c8*(delta*x0pow8-(x1pow9-x0pow9)/9)
		   +c9*(delta*x0pow9-(x1pow10-x0pow10)/10)
		   +c10*(delta*x0pow10-(x1pow11-x0pow11)/11)
		   )
		 )/diffx0;
  

  //------------------------
  //calculate Coef0
  //------------------------
  Double_t Coef0=(
		 yield
		 -(Coef1*(x1pow2-x0pow2)/2
		   +Coef2*(x1pow3-x0pow3)/3
		   +c3*(x1pow4-x0pow4)/4
		   +c4*(x1pow5-x0pow5)/5
		   +c5*(x1pow6-x0pow6)/6
		   +c6*(x1pow7-x0pow7)/7
		   +c7*(x1pow8-x0pow8)/8
		   +c8*(x1pow9-x0pow9)/9
		   +c9*(x1pow9-x0pow10)/10
		   +c10*(x1pow10-x0pow11)/11
		   )
		 )/delta;    
  
  ///calculate the polynomial
  return Coef0+Coef1*x+Coef2*x*x+c3*x*x*x+c4*x*x*x*x+c5*x*x*x*x*x+c6*x*x*x*x*x*x+c7*x*x*x*x*x*x*x+c8*x*x*x*x*x*x*x*x+c9*x*x*x*x*x*x*x*x*x+c10*x*x*x*x*x*x*x*x*x*x;
}

