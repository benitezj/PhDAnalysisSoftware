
#ifndef RooExpPolyPolyPdf_hh
#define RooExpPolyPolyPdf_hh

#include "RooAbsPdf.h"
#include "RooRealProxy.h"
#include "RooListProxy.h"

class RooExpPolyPolyPdf : public RooAbsPdf {
 public:
  RooExpPolyPolyPdf(const char *name, const char *title,RooArgList &_coeflist);
  RooExpPolyPolyPdf(const RooExpPolyPolyPdf& other, const char* name=0) ;
  virtual TObject* clone(const char* newname) const { return new RooExpPolyPolyPdf(*this,newname); }
  inline virtual ~RooExpPolyPolyPdf(){ }

 protected:

  RooRealProxy x;  

  Double_t evaluate() const ;
 
 private:    
    
  RooListProxy coefList ;  //  List of coefficients
  TIterator* coefIter ;    //! Iterator over coefficient list
  RooListProxy coefList2 ;  //  List of coefficients
  TIterator* coefIter2 ;    //! Iterator over coefficient list
  RooRealProxy x0 ;
 
  Double_t EvalSum(Double_t y) const;
  Double_t EvalSumN(Double_t y) const;
  Double_t EvalSum2(Double_t y) const;
  Double_t EvalSumN2(Double_t y) const;

  ClassDef(RooExpPolyPolyPdf,0) 
};
#endif
