
#ifndef ROO_NRBREITWIGNER
#define ROO_NRBREITWIGNER

#include "RooAbsPdf.h"
#include "RooRealProxy.h"
class RooAbsReal;
class RooRealVar;

class RooNRBreitWigner : public RooAbsPdf {
public:
  RooNRBreitWigner(const char *name, const char *title,
		   RooRealVar* _x, RooAbsReal& _mean, RooAbsReal& _width);
  RooNRBreitWigner(const RooNRBreitWigner& other, const char* name=0) ;
  virtual TObject* clone(const char* newname) const { return new RooNRBreitWigner(*this,newname); }
  inline virtual ~RooNRBreitWigner() { }
  Bool_t selfNormalized() const { return kTRUE ; }

protected:

  RooRealProxy x ;
  RooRealVar* xp ;
  RooRealProxy mean ;
  RooRealProxy width ;
  
  Double_t evaluate() const ;

private:

  ClassDef(RooNRBreitWigner,0) //
};

#endif
