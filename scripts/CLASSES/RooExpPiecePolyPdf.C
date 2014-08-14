
#include "RooFit.h"
#include "Riostream.h"
#include <math.h>
#include "RooExpPiecePolyPdf.h"
#include "RooAbsReal.h"

ClassImp(RooExpPiecePolyPdf)

RooExpPiecePolyPdf::RooExpPiecePolyPdf(const char *name, const char *title,RooAbsReal &x,RooAbsReal &x0,RooAbsReal &c1,RooAbsReal &c2,RooAbsReal &d2) :
  RooAbsPdf(name,title),
  _x("_x","_x",this,x),
  _x0("_x0","_x0",this,x0),
  _c1("_c1","_c1",this,c1),
  _c2("_c2","_c2",this,c2),
  _d2("_d2","_d2",this,d2),
  _theta(0.)
{                                                                                                                                      

}

RooExpPiecePolyPdf::RooExpPiecePolyPdf(const RooExpPiecePolyPdf& other, const char* name) : 
  RooAbsPdf(other,name),_x("_x",this,other._x),_x0("_x0",this,other._x0),
  _c1("_c1",this,other._c1),
  _c2("_c2",this,other._c2),
  _d2("_d2",this,other._d2),
  _theta(other._theta)
{
}


Double_t RooExpPiecePolyPdf::evaluate() const
{ 
  
  Double_t C1=_c1;
  Double_t C2=_c2;
  
  //In case of strong correlation rotate coeficients
  if(_theta!=0.){
    C1=cos(_theta)*_c1-sin(_theta)*_c2;
    C2=sin(_theta)*_c1+cos(_theta)*_c2;
  }


  if(_x<=_x0){
    return exp(C1*_x+C2*_x*_x);
  }

  Double_t d1=C1+2.*(C2-_d2)*_x0;   
  Double_t d0=(C1-d1)*_x0+(C2-_d2)*_x0*_x0;
    
  return exp(d0+d1*_x+_d2*_x*_x);
  
}

