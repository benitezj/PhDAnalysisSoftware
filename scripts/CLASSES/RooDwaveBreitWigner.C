
#include "RooFit.h"

#include "Riostream.h"
#include <math.h>

#include "RooDwaveBreitWigner.h"
#include "RooAbsReal.h"
#include "RooRealVar.h"

ClassImp(RooDwaveBreitWigner)
  
RooDwaveBreitWigner::RooDwaveBreitWigner(const char *name, const char *title,
					 RooRealVar* _x, RooAbsReal& _mean,RooAbsReal& _width,
					 RooAbsReal& _dau1Mass, RooAbsReal& _dau2Mass,RooAbsReal& _radius) :
  RooAbsPdf(name,title),
  x("x","Dependent",this,*_x),
  xp(_x),
  mean("mean","Mean",this,_mean),
  width("width","Width",this,_width),
  dau1Mass("dau1Mass","dau1Mass",this,_dau1Mass),
  dau2Mass("dau2Mass","dau2Mass",this,_dau2Mass),
  radius("radius","radius",this,_radius)
{
  
  if(!_x)cout<<" RooSwaveBreitWigner:ERROR:   bad inputs _x "<<endl;
  if(mean<=0
     ||mean<=dau1Mass+dau2Mass
     ||mean*mean<=(dau1Mass-dau2Mass)*(dau1Mass-dau2Mass))
    cout<<" RooSwaveBreitWigner:ERROR:   bad inputs mean, dau1Mass or dau2Mass"<<endl;; 
}


RooDwaveBreitWigner::RooDwaveBreitWigner(const RooDwaveBreitWigner& other, const char* name) : 
  RooAbsPdf(other,name), x("x",this,other.x), 
  xp(other.xp),
  mean("mean",this,other.mean),
  width("width",this,other.width),
  dau1Mass("dau1Mass",this,other.dau1Mass),
  dau2Mass("dau2Mass",this,other.dau2Mass),
  radius("radius",this,other.radius)
{

}


Double_t RooDwaveBreitWigner::evaluate() const
{
  if(!xp)return 0;
  
  if(x<=0
     ||x<=dau1Mass+dau2Mass
     ||x*x<=(dau1Mass-dau2Mass)*(dau1Mass-dau2Mass)
     )return 0.; 

//   Double_t xorigval=xp->getVal();
//   xp->setVal(mean);
//   Double_t psmean=phasespace->evaluate();
//   Double_t qfmean=x*psmean;
//   Double_t bwfmean=9. + 3.*pow(qfmean*radius,2) + pow(qfmean*radius,4);
//   xp->setVal(xorigval);
//   Double_t ps=phasespace->evaluate();
//   Double_t qf=x*ps;
//   Double_t bwf=9. + 3.*pow(qf*radius,2) + pow(qf*radius,4);    
//   Double_t Width=width*(ps/psmean)*pow(qf/qfmean,4)*(bwfmean/bwf);//
//   Double_t num=x*pow(qf/qfmean,4)*(bwfmean/bwf);
//   Double_t den=(x*x-mean*mean)*(x*x-mean*mean) + (mean*Width)*(mean*Width);


  Double_t psmean=(sqrt((mean*mean-(dau1Mass+dau2Mass)*(dau1Mass+dau2Mass))*(mean*mean-(dau1Mass-dau2Mass)*(dau1Mass-dau2Mass)))/(2*mean*mean));
  Double_t qfmean=mean*psmean;
  Double_t bwfmean=9. + 3.*pow(qfmean*radius,2) + pow(qfmean*radius,4);

  Double_t ps=(sqrt((x*x-(dau1Mass+dau2Mass)*(dau1Mass+dau2Mass))*(x*x-(dau1Mass-dau2Mass)*(dau1Mass-dau2Mass)))/(2*x*x));
  Double_t qf=x*ps;
  Double_t bwf=9. + 3.*pow(qf*radius,2) + pow(qf*radius,4);    

  Double_t Width=width*(ps/psmean)*pow(qf/qfmean,4)*(bwfmean/bwf);
  Double_t num=x*pow(qf/qfmean,4)*(bwfmean/bwf);
  Double_t den=(x*x-mean*mean)*(x*x-mean*mean) + (mean*Width)*(mean*Width);

  if(den<=0.){cout<<"RooDwaveBreitWigner::Warning denominator evaluates to 0"<<endl; return 0;}
  return num/den;//Note that this formula does not include a phase-space factor in front this must be applied separately
}

