
#ifndef RooPolynomialFixEdgeNew_hh
#define RooPolynomialFixEdgeNew_hh

#include "RooAbsPdf.h"
#include "RooRealProxy.h"

#include "TH1.h"
#include "RooFormula.h"

class RooRealVar;

class RooPolynomialFixEdgeNew : public RooAbsPdf {
public:
  RooPolynomialFixEdgeNew(const char *name, const char *title,RooAbsReal& _x,
			  TH1*Histo,RooFormula* Yield,RooAbsReal& _c2,RooAbsReal& _c3,RooAbsReal& _c4,RooAbsReal& _c5,
			  RooAbsReal& _c6,RooAbsReal& _c7,RooAbsReal& _c8,RooAbsReal& _c9,RooAbsReal& _c10);
  RooPolynomialFixEdgeNew(const RooPolynomialFixEdgeNew& other, const char* name=0) ;
  virtual TObject* clone(const char* newname) const { return new RooPolynomialFixEdgeNew(*this,newname); }
  inline virtual ~RooPolynomialFixEdgeNew(){ }
  void setDependentProxy(RooRealProxy _x){x=_x;}
  void setHisto(TH1*Histo){histo=Histo;InitParameters();}
  Bool_t InitParameters();///!!if changing edge parameters for multiple fits need to reinitialize

  Bool_t selfNormalized() const { return kTRUE ; }
  
 protected:

  RooRealProxy x;  
  TH1*histo;
  RooFormula* yield;  
  RooRealProxy c2;
  RooRealProxy c3;
  RooRealProxy c4;
  RooRealProxy c5;
  RooRealProxy c6;
  RooRealProxy c7;
  RooRealProxy c8;
  RooRealProxy c9;
  RooRealProxy c10;
  
  Double_t evaluate() const ;
 
private:    
    
  Double_t x0;
  Double_t y0;
  Double_t x1;
  Double_t y1;

  //constant terms
  Double_t delta;
/*   Double_t delta2; */
/*   //Double_t delta3; */
/*   Double_t diffx0; */
/*   Double_t diffx1; */
  //Double_t denom;
  Double_t x0pow2;
  Double_t x0pow3;
  Double_t x0pow4;
  Double_t x0pow5;
  Double_t x0pow6;
  Double_t x0pow7;
  Double_t x0pow8;
  Double_t x0pow9;
  Double_t x0pow10;
  Double_t x0pow11;
  Double_t x1pow2;
  Double_t x1pow3;
  Double_t x1pow4;
  Double_t x1pow5;
  Double_t x1pow6;
  Double_t x1pow7;
  Double_t x1pow8;
  Double_t x1pow9;
  Double_t x1pow10;
  Double_t x1pow11;
  
  ClassDef(RooPolynomialFixEdgeNew,0) 
};

#endif
