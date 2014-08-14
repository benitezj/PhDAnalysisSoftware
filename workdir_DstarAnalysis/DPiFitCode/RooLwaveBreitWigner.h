
#ifndef RooLwaveBreitWigner_hh
#define RooLwaveBreitWigner_hh

#include "RooAbsPdf.h"
#include "RooRealProxy.h"
class RooRealVar;

class RooLwaveBreitWigner : public RooAbsPdf {
public:
  RooLwaveBreitWigner(const char *name, const char *title,RooAbsReal& _x, RooAbsReal& _mean, RooAbsReal& _width);
  RooLwaveBreitWigner(const RooLwaveBreitWigner& other, const char* name=0);
  virtual TObject* clone(const char* newname) const { return new RooLwaveBreitWigner(*this,newname); }
  //inline virtual
  ~RooLwaveBreitWigner();

  //configure 
  void Configure(Double_t mdau1,Double_t mdau2,Double_t mdau3,Double_t R,Int_t wave,Int_t decayType){
    md1=mdau1;
    md2=mdau2;
    md3=mdau3;
    radius=R;
    L=wave;
    decay=decayType;
  }
  
 protected:

  RooRealProxy x;
  RooRealProxy mean;
  RooRealProxy width;
  
  Double_t md1;
  Double_t md2;
  Double_t md3;
  Double_t radius;
  Int_t L;//0, 1, 2
  Int_t decay;//1,2=2-body phase space ,3=3-body phase space

  Double_t evaluate() const ;

private:
 
  Double_t qfactor(Double_t m) const ;
  Double_t bwfactor(Double_t q) const ;
  ClassDef(RooLwaveBreitWigner,0)
};

#endif
