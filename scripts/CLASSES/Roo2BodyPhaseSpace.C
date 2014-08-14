
#include "RooFit.h"
#include "Riostream.h"
#include <math.h>

#include "Roo2BodyPhaseSpace.h"
#include "RooAbsReal.h"

ClassImp(Roo2BodyPhaseSpace)

Roo2BodyPhaseSpace::Roo2BodyPhaseSpace(const char *name, const char *title,RooAbsReal& _x, RooAbsReal& _dau1Mass, RooAbsReal& _dau2Mass) :
  RooAbsPdf(name,title),
  x("x","Dependent",this,_x),
  dau1Mass("dau1Mass","dau1Mass",this,_dau1Mass),
  dau2Mass("dau2Mass","dau2Mass",this,_dau2Mass)
{
}


Roo2BodyPhaseSpace::Roo2BodyPhaseSpace(const Roo2BodyPhaseSpace& other, const char* name) : 
  RooAbsPdf(other,name), x("x",this,other.x),
  dau1Mass("dau1Mass",this,other.dau1Mass),
  dau2Mass("dau2Mass",this,other.dau2Mass)
{
}


Double_t Roo2BodyPhaseSpace::evaluate() const
{
  if(x<=0
     ||x<=dau1Mass+dau2Mass
     ||x*x<=(dau1Mass-dau2Mass)*(dau1Mass-dau2Mass))return 0.;  

  return sqrt((x*x-(dau1Mass+dau2Mass)*(dau1Mass+dau2Mass))*(x*x-(dau1Mass-dau2Mass)*(dau1Mass-dau2Mass)))/(2*x*x);
}




