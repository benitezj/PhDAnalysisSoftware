
#include "RooFit.h"

#include "Riostream.h"
#include "Riostream.h"
#include <math.h>

#include "RooRelBreitWigner.h"
#include "RooAbsReal.h"
#include "RooRealVar.h"

ClassImp(RooRelBreitWigner)

RooRelBreitWigner::RooRelBreitWigner(const char *name, const char *title,
			 RooAbsReal& _x, RooAbsReal& _mean,
			 RooAbsReal& _width) :
  RooAbsPdf(name,title),
  x("x","Dependent",this,_x),
  mean("mean","Mean",this,_mean),
  width("width","Width",this,_width)
{
}


RooRelBreitWigner::RooRelBreitWigner(const RooRelBreitWigner& other, const char* name) : 
  RooAbsPdf(other,name), x("x",this,other.x), mean("mean",this,other.mean),
  width("width",this,other.width)
{
}


Double_t RooRelBreitWigner::evaluate() const
{
  return 1.0 / (pow(x*x-mean*mean,2) + pow(mean*width,2));
}

