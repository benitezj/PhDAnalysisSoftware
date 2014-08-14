
#ifndef RooExpPiecePolyPdf_hh
#define RooExpPiecePolyPdf_hh

#include "RooAbsPdf.h"
#include "RooRealProxy.h"
#include "RooAbsReal.h"

class RooExpPiecePolyPdf : public RooAbsPdf {
 public:
  RooExpPiecePolyPdf(const char *name, const char *title,RooAbsReal &x,RooAbsReal &x0,RooAbsReal &c1,RooAbsReal &c2,RooAbsReal &d2);
  RooExpPiecePolyPdf(const RooExpPiecePolyPdf& other, const char* name=0) ;
  virtual TObject* clone(const char* newname) const { return new RooExpPiecePolyPdf(*this,newname); }
  inline virtual ~RooExpPiecePolyPdf(){ }

  void RotateCoefs(Float_t theta_in_rads){_theta=theta_in_rads;}

 protected:

  RooRealProxy _x;  

  Double_t evaluate() const ;
 
 private:    
    
  RooRealProxy _x0 ;
  RooRealProxy _c1 ;
  RooRealProxy _c2 ;
  RooRealProxy _d2 ;

  Float_t _theta;

  ClassDef(RooExpPiecePolyPdf,0) 
};
#endif
