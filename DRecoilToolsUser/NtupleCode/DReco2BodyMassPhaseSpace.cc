
#include "RooFit.h"
#include "Riostream.h"
#include "RooAbsReal.h"
#include <math.h>

#include "DRecoilToolsUser/NtupleCode/DReco2BodyMassPhaseSpace.hh"


ClassImp(DReco2BodyMassPhaseSpace)

DReco2BodyMassPhaseSpace::DReco2BodyMassPhaseSpace(const char *name, const char *title,RooAbsReal& _x, RooAbsReal& _dau1Mass, RooAbsReal& _dau2Mass) :
  RooAbsPdf(name,title),
  x("x","Dependent",this,_x),
  dau1Mass("dau1Mass","dau1Mass",this,_dau1Mass),
  dau2Mass("dau2Mass","dau2Mass",this,_dau2Mass),
  _xoffset(0.),
  _inverse(0)
{
}


DReco2BodyMassPhaseSpace::DReco2BodyMassPhaseSpace(const DReco2BodyMassPhaseSpace& other, const char* name) : 
  RooAbsPdf(other,name),
  x("x",this,other.x),
  dau1Mass("dau1Mass",this,other.dau1Mass),
  dau2Mass("dau2Mass",this,other.dau2Mass),
  _xoffset(other._xoffset),
  _inverse(other._inverse)
{
}

Double_t DReco2BodyMassPhaseSpace::evaluate() const
{
  Float_t y=x+_xoffset;

  if(_inverse){
    if(y>=dau1Mass+dau2Mass)return 0.;
    y=dau1Mass+dau2Mass+(dau1Mass+dau2Mass-y);//flip about the threshold
    //res=sqrt((dau1Mass+dau2Mass)*(dau1Mass+dau2Mass)-y*y)/(2*(dau1Mass+dau2Mass-y));
  }else{
    if(y<=dau1Mass+dau2Mass)return 0.;      
  }

  return sqrt((y*y-(dau1Mass+dau2Mass)*(dau1Mass+dau2Mass))*(y*y-(dau1Mass-dau2Mass)*(dau1Mass-dau2Mass)))/(2*y);
}




