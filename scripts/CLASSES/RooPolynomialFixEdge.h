
#ifndef RooPolynomialFixEdge_hh
#define RooPolynomialFixEdge_hh

#include "RooAbsPdf.h"
#include "RooRealProxy.h"

class RooRealVar;

class RooPolynomialFixEdge : public RooAbsPdf {
public:
  RooPolynomialFixEdge(const char *name, const char *title,RooAbsReal& _x,
		       RooAbsReal& _x0, RooAbsReal& _y0, RooAbsReal& _x1, RooAbsReal& _y1,
		       RooAbsReal& _yield,RooAbsReal& _c3,RooAbsReal& _c4,RooAbsReal& _c5,
		       RooAbsReal& _c6,RooAbsReal& _c7,RooAbsReal& _c8,RooAbsReal& _c9,RooAbsReal& _c10);
  RooPolynomialFixEdge(const RooPolynomialFixEdge& other, const char* name=0) ;
  virtual TObject* clone(const char* newname) const { return new RooPolynomialFixEdge(*this,newname); }
  inline virtual ~RooPolynomialFixEdge() { }
  void setDependentProxy(RooRealProxy _x){x=_x;}
  void setEdges(RooRealProxy _x0,RooRealProxy _y0,RooRealProxy _x1,RooRealProxy _y1){x0=_x0;y0=_y0; x1=_x1;y1=_y1;}
  Bool_t InitParameters();///!!if changing edger parameters for multiple fits need to reinitialize

 protected:

  RooRealProxy x; 
  RooRealProxy x0;
  RooRealProxy y0;
  RooRealProxy x1;
  RooRealProxy y1;
  RooRealProxy yield;
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
    
  //constant terms
  Double_t delta;
  Double_t delta2;
  Double_t delta3;
  Double_t diffx0;
  Double_t diffx1;
  Double_t denom;
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
  
  ClassDef(RooPolynomialFixEdge,0) 
};

#endif
