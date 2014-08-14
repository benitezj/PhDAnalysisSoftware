
#ifndef RooPolyExpPdf_hh
#define RooPolyExpPdf_hh

#include "RooAbsPdf.h"
#include "RooRealProxy.h"
#include "RooListProxy.h"

class RooPolyExpPdf : public RooAbsPdf {
 public:
  RooPolyExpPdf(const char *name, const char *title,RooAbsReal &_x,RooArgList &_coeflist);
  RooPolyExpPdf(const RooPolyExpPdf& other, const char* name=0) ;
  virtual TObject* clone(const char* newname) const { return new RooPolyExpPdf(*this,newname); }
  inline virtual ~RooPolyExpPdf(){ }

 protected:

  RooRealProxy x;  

  Double_t evaluate() const ;
 
 private:    
    
  RooListProxy coefList ;  //  List of coefficients
  TIterator* coefIter ;    //! Iterator over coefficient list
  RooAbsReal* x0 ;
 
  Double_t EvalSum(Double_t y) const;
  Double_t EvalSumN(Double_t y) const;

  ClassDef(RooPolyExpPdf,0) 
};
#endif
