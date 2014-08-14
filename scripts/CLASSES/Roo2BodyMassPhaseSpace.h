#ifndef Roo2BodyMassPhaseSpace_hh
#define Roo2BodyMassPhaseSpace_hh

#include "RooAbsPdf.h"
#include "RooAbsReal.h"
#include "RooRealProxy.h"

class Roo2BodyMassPhaseSpace : public RooAbsPdf {
public:
  Roo2BodyMassPhaseSpace(const char *name, const char *title,
		     RooAbsReal& _x, RooAbsReal& _dau1Mass, RooAbsReal& _dau2Mass);
  Roo2BodyMassPhaseSpace(const Roo2BodyMassPhaseSpace& other, const char* name=0) ;
  virtual TObject* clone(const char* newname) const { return new Roo2BodyMassPhaseSpace(*this,newname); }
  ~Roo2BodyMassPhaseSpace(){ }
/*   void setDependentProxy(RooRealProxy _x){x=_x;} */
/*   void setDependentVal(Double_t val){x=val;} */
/*   Double_t getDependentVal(){return (double)x;} */
  
  void setOffset(Float_t xoffset){_xoffset=xoffset;}
  void setInverse(Bool_t invert){_inverse=invert;}
  
protected:

  RooRealProxy x ; 
  RooRealProxy dau1Mass;
  RooRealProxy dau2Mass;
  
  
  Double_t evaluate() const ;
 
private:
  Float_t _xoffset;
  Bool_t _inverse;

  ClassDef(Roo2BodyMassPhaseSpace,0) //Relativistic Breit Wigner PDF
};

#endif
