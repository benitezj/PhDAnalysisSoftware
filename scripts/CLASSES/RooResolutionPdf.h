
#ifndef RooResolutionPdf_hh
#define RooResolutionPdf_hh

#include "RooAbsPdf.h"
#include "RooRealProxy.h"
#include "RooAbsReal.h"
#include "RooRealVar.h"


class RooResolutionPdf : public RooAbsPdf {
public:
  RooResolutionPdf(const char *name, const char *title,RooRealVar* _resvar,
		   RooAbsReal& _mass,RooAbsReal& _offset,RooAbsReal& _slope,
		   RooAbsReal& _M1,
		   RooAbsReal& _N2,RooAbsReal& _M2,RooAbsReal& _S2,
		   RooAbsReal& _N3,RooAbsReal& _M3,RooAbsReal& _S3);

  RooResolutionPdf(const RooResolutionPdf& other, const char* name=0) ;

  virtual TObject* clone(const char* newname) const { return new RooResolutionPdf(*this,newname); }

  inline virtual ~RooResolutionPdf(){}
  
  Bool_t selfNormalized() const { return kTRUE ; }
 
 protected:

  RooRealProxy resvar;  
  RooRealVar* resvarp;
  RooRealProxy mass;  
  RooRealProxy offset;
  RooRealProxy slope; 
  RooRealProxy M1;
  RooRealProxy N2;
  RooRealProxy M2; 
  RooRealProxy S2;  
  RooRealProxy N3;
  RooRealProxy M3;
  RooRealProxy S3;    
  Double_t evaluate() const ;
 
private:    

  Double_t normcorr;

  ClassDef(RooResolutionPdf,0);
};

#endif
