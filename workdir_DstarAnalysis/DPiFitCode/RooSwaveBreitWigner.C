
#include "RooFit.h"

#include "Riostream.h"
#include "Riostream.h"
#include <math.h>

#include "RooSwaveBreitWigner.h"
#include "RooAbsReal.h"
#include "RooRealVar.h"

ClassImp(RooSwaveBreitWigner)

RooSwaveBreitWigner::RooSwaveBreitWigner(const char *name, const char *title,
					 RooRealVar* _x, RooAbsReal& _mean,RooAbsReal& _width,
					 RooAbsReal& _dau1Mass, RooAbsReal& _dau2Mass) :
  RooAbsPdf(name,title),
  X(_x),
  x("x","Dependent",this,*_x),
  mean("mean","Mean",this,_mean),
  width("width","Width",this,_width),
  dau1Mass("dau1Mass","dau1Mass",this,_dau1Mass),
  dau2Mass("dau2Mass","dau2Mass",this,_dau2Mass)
{

  if(!_x)cout<<" RooSwaveBreitWigner:ERROR:   bad inputs _x "<<endl;
  if(mean<=0
     ||mean<=dau1Mass+dau2Mass
     ||mean*mean<=(dau1Mass-dau2Mass)*(dau1Mass-dau2Mass))
    cout<<" RooSwaveBreitWigner:ERROR:   bad inputs mean, dau1Mass or dau2Mass"<<endl;; 
}


RooSwaveBreitWigner::RooSwaveBreitWigner(const RooSwaveBreitWigner& other, const char* name) : 
  RooAbsPdf(other,name),X(other.X),
  x("x",this,other.x),
  mean("mean",this,other.mean),
  width("width",this,other.width),
  dau1Mass("dau1Mass",this,other.dau1Mass),
  dau2Mass("dau2Mass",this,other.dau2Mass)
{

}


Double_t RooSwaveBreitWigner::evaluate() const
{
   
  if(x<=0
     ||x<=dau1Mass+dau2Mass
     ||x*x<=(dau1Mass-dau2Mass)*(dau1Mass-dau2Mass)
     )return 0.; 

  Double_t Width=width
                 *(sqrt((x*x-(dau1Mass+dau2Mass)*(dau1Mass+dau2Mass))*(x*x-(dau1Mass-dau2Mass)*(dau1Mass-dau2Mass)))/(2*x*x))
                 /(sqrt((mean*mean-(dau1Mass+dau2Mass)*(dau1Mass+dau2Mass))*(mean*mean-(dau1Mass-dau2Mass)*(dau1Mass-dau2Mass)))/(2*mean*mean));

  return x/((x*x-mean*mean)*(x*x-mean*mean) + (mean*Width)*(mean*Width) );
}

