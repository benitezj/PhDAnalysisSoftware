#ifndef RooConvolutionPdf_hh
#define RooConvolutionPdf_hh

#include "RooAbsPdf.h"
#include "RooRealVar.h"
#include "RooRealProxy.h"


class RooConvolutionPdf : public RooAbsPdf {
public:

  RooConvolutionPdf(const char *name, const char *title,
		    RooRealVar* Var, RooAbsPdf* pdf,
		    RooRealVar* resVar, RooAbsPdf* respdf) ;
  RooConvolutionPdf(const RooConvolutionPdf& other, const char* name=0) ;

  virtual TObject* clone(const char* newname) const { return new RooConvolutionPdf(*this,newname) ; }
  ~RooConvolutionPdf(){}

  Bool_t selfNormalized() const { return kTRUE ; }
   
  Double_t evaluate() const;

protected:

private:
 
  RooRealVar* _Var ;       //  convolution variable
  RooAbsPdf* _Pdf ;        //  input PDF
  RooRealVar* _resVar ;    //  resolution variable
  RooAbsPdf* _resPdf ;     //  resolution model; 

  Float_t resvarstep;
  Float_t varstep;

  RooRealProxy x;          //  independent variable for this pdf
  RooRealProxy _origpdf;   //  need because it passes the parameters for fits, otherwise parameters stay constant


  ClassDef(RooConvolutionPdf,0)     
};

#endif
