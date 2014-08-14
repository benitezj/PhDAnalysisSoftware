
#ifndef RooSwaveBreitWigner_hh
#define RooSwaveBreitWigner_hh

#include "RooAbsPdf.h"
#include "RooRealProxy.h"

class RooAbsReal;
class RooRealVar;

class RooSwaveBreitWigner : public RooAbsPdf {
public:
  RooSwaveBreitWigner(const char *name, const char *title,
		      RooRealVar* _x, RooAbsReal& _mean, RooAbsReal& _width,
		      RooAbsReal& _dau1Mass, RooAbsReal& _dau2Mass );
  RooSwaveBreitWigner(const RooSwaveBreitWigner& other, const char* name=0) ;
  virtual TObject* clone(const char* newname) const { return new RooSwaveBreitWigner(*this,newname); }
  inline virtual ~RooSwaveBreitWigner(){}

protected:

  RooRealVar* X;
  RooRealProxy x;
  RooRealProxy mean;
  RooRealProxy width;
  RooRealProxy dau1Mass;
  RooRealProxy dau2Mass;
  Double_t evaluate() const ;

private:
  
  ClassDef(RooSwaveBreitWigner,0)
};

#endif
