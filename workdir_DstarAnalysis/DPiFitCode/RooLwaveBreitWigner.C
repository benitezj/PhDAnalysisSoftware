
#include "RooFit.h"

#include "Riostream.h"
#include "Riostream.h"
#include <math.h>

#include "RooLwaveBreitWigner.h"
#include "RooAbsReal.h"
#include "RooRealVar.h"

ClassImp(RooLwaveBreitWigner)
  
RooLwaveBreitWigner::RooLwaveBreitWigner(const char *name, const char *title,RooAbsReal& _x, RooAbsReal& _mean,RooAbsReal& _width) :
  RooAbsPdf(name,title),
  x("x","Dependent",this,_x),
  mean("mean","Mean",this,_mean),
  width("width","Width",this,_width),
  md1(1),
  md2(1),
  md3(1),
  radius(3),
  L(0),
  decay(1)
{

}


RooLwaveBreitWigner::RooLwaveBreitWigner(const RooLwaveBreitWigner& other, const char* name) : 
  RooAbsPdf(other,name), x("x",this,other.x), mean("mean",this,other.mean),width("width",this,other.width),
  md1(other.md1),md2(other.md2),md3(other.md3),radius(other.radius),L(other.L),decay(other.decay)
{

}

RooLwaveBreitWigner::~RooLwaveBreitWigner(){
}

Double_t RooLwaveBreitWigner::qfactor(Double_t m) const
{
  //phase space factor
  if(decay==2)return sqrt((m*m-(md1+md2)*(md1+md2))*(m*m-(md1-md2)*(md1-md2)))/(2*m);
  if(decay==3)return 1;//3 body
  return 1;
}

Double_t RooLwaveBreitWigner::bwfactor(Double_t q) const
{
  if(L==1)return 1./sqrt(1. + pow(radius*q,2));
  if(L==2)return 1./sqrt(1. + pow(radius*q,2)/3. + pow(radius*q,4)/9.);
  return 1.;
}

Double_t RooLwaveBreitWigner::evaluate() const
{
      
  Double_t Width=(width*mean/x)
    *(pow(qfactor(x)/qfactor(mean),2*L+1))
    *(bwfactor(qfactor(x))/bwfactor(qfactor(mean)));
  
  Double_t num=pow(qfactor(x),2*L)
    *pow(bwfactor(qfactor(x))/bwfactor(qfactor(mean)),2);

  Double_t den=(x*x-mean*mean)*(x*x-mean*mean) + (mean*Width)*(mean*Width);

  if(den<=0)return 0;
  return num/den;
}

