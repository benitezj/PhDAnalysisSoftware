#ifndef Roo2BodyPhaseSpace_hh
#define Roo2BodyPhaseSpace_hh

#include "RooAbsPdf.h"
#include "RooAbsReal.h"
#include "RooRealProxy.h"

class Roo2BodyPhaseSpace : public RooAbsPdf {
public:
  Roo2BodyPhaseSpace(const char *name, const char *title,
		     RooAbsReal& _x, RooAbsReal& _dau1Mass, RooAbsReal& _dau2Mass);
  Roo2BodyPhaseSpace(const Roo2BodyPhaseSpace& other, const char* name=0) ;
  virtual TObject* clone(const char* newname) const { return new Roo2BodyPhaseSpace(*this,newname); }
  ~Roo2BodyPhaseSpace(){ }
protected:

  RooRealProxy x ; 
  RooRealProxy dau1Mass;
  RooRealProxy dau2Mass;
  
  Double_t evaluate() const ;
 
private:
  ClassDef(Roo2BodyPhaseSpace,0) //Relativistic Breit Wigner PDF
};

#endif
