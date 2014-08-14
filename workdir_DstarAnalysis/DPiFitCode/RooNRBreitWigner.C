
#include "RooFit.h"

#include "Riostream.h"
#include <math.h>

#include "RooNRBreitWigner.h"
#include "RooAbsReal.h"
#include "RooRealVar.h"

ClassImp(RooNRBreitWigner);
  
RooNRBreitWigner::RooNRBreitWigner(const char *name, const char *title,
				   RooRealVar* _x, RooAbsReal& _mean, RooAbsReal& _width) :
  RooAbsPdf(name,title),
  x("x","Dependent",this,*_x),
  xp(_x),
  mean("mean","Mean",this,_mean),
  width("width","Width",this,_width)
{
}


RooNRBreitWigner::RooNRBreitWigner(const RooNRBreitWigner& other, const char* name) : 
  RooAbsPdf(other,name), x("x",this,other.x),xp(other.xp),mean("mean",this,other.mean), width("width",this,other.width)
{
}


Double_t RooNRBreitWigner::evaluate() const
{
  
  //normalize value
  return 1.0 / ( (pow(x-mean,2) + pow(.5*width,2)) * (2/width) * (atan(2*(xp->getMax()-mean)/width) - atan(2*(xp->getMin()-mean)/width) ));
}

