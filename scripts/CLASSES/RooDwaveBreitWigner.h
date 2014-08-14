
#ifndef RooDwaveBreitWigner_hh
#define RooDwaveBreitWigner_hh

#include "RooAbsPdf.h"
#include "RooRealProxy.h"
class RooAbsReal;
class RooRealVar;

class RooDwaveBreitWigner : public RooAbsPdf {
public:
  RooDwaveBreitWigner(const char *name, const char *title,
		      RooRealVar* _x, RooAbsReal& _mean, RooAbsReal& _width,
		      RooAbsReal& _dau1Mass, RooAbsReal& _dau2Mass, RooAbsReal& _radius);
  RooDwaveBreitWigner(const RooDwaveBreitWigner& other, const char* name=0);
  virtual TObject* clone(const char* newname) const { return new RooDwaveBreitWigner(*this,newname); }
  inline virtual ~RooDwaveBreitWigner(){}

protected:
  
  RooRealProxy x;
  RooRealVar* xp;
  RooRealProxy mean;
  RooRealProxy width;
  RooRealProxy dau1Mass;
  RooRealProxy dau2Mass;
  RooRealProxy radius;
  
  Double_t evaluate() const ;

private:
  ClassDef(RooDwaveBreitWigner,0)
};

#endif
