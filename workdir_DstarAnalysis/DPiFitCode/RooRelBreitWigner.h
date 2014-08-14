
#ifndef ROO_RELBREITWIGNER
#define ROO_RELBREITWIGNER

#include "RooAbsPdf.h"
#include "RooRealProxy.h"

class RooRealVar;

class RooRelBreitWigner : public RooAbsPdf {
public:
  RooRelBreitWigner(const char *name, const char *title,
	      RooAbsReal& _x, RooAbsReal& _mean, RooAbsReal& _width);
  RooRelBreitWigner(const RooRelBreitWigner& other, const char* name=0) ;
  virtual TObject* clone(const char* newname) const { return new RooRelBreitWigner(*this,newname); }
  inline virtual ~RooRelBreitWigner() { }

protected:

  RooRealProxy x ;
  RooRealProxy mean ;
  RooRealProxy width ;
  
  Double_t evaluate() const ;

private:

  ClassDef(RooRelBreitWigner,0) //Relativistic Breit Wigner PDF
};

#endif
