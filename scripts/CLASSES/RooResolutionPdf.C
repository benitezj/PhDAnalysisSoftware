#include "RooFit.h"

#include "RooResolutionPdf.h"

#include "Riostream.h"
#include <math.h>

ClassImp(RooResolutionPdf);
RooResolutionPdf::RooResolutionPdf(const char *name, const char *title,RooRealVar* _resvar,
				   RooAbsReal& _mass,RooAbsReal& _offset,RooAbsReal& _slope,
				   RooAbsReal& _M1,
				   RooAbsReal& _N2,RooAbsReal& _M2,RooAbsReal& _S2,
				   RooAbsReal& _N3,RooAbsReal& _M3,RooAbsReal& _S3) :
  RooAbsPdf(name,title),
  resvar("resvar","resvar",this,*_resvar),
  resvarp(_resvar),
  mass("mass","mass",this,_mass),         //Assume linear parametrization of width of resolution
  offset("offset","offset",this,_offset), //==> use offset+slope*mass = sigma of main gaussian
  slope("slope","slope",this,_slope),     //
  M1("M1","M1",this,_M1),//Mean value of main Gaussian
  N2("N2","N2",this,_N2),//fraction in the second term (Breit Wigner)
  M2("M2","M2",this,_M2),//Mean value of  second term (Breit Wigner) with respect to Main Gaussian
  S2("S2","S2",this,_S2),//Ratio of HWHM of   second term (Breit Wigner) with respect to Main Gaussian sigma
  N3("N3","N3",this,_N3),//fraction in the 3rd term (Breit Wigner)
  M3("M3","M3",this,_M3),//Mean value of  3rd term (Breit Wigner) with respect to Main Gaussian
  S3("S3","S3",this,_S3),//Ratio of HWHM of 3rd term (Breit Wigner) with respect to Main Gaussian sigma
  normcorr(1)
{
  
  //aprox correction factor for long tail leakage: Note this may not be much better as pdf will change with mass values
  Float_t origxval=resvarp->getVal();
  Float_t step=(resvarp->getMax()-resvarp->getMin())/300.;
  Float_t integ=0;
  for(Int_t i=0;i<300;i++){
    resvarp->setVal(resvarp->getMin()+step*(i+.5));
    integ+=evaluate()*step;
  }  
  normcorr=integ;
  resvarp->setVal(origxval);
  cout<<"RooResolutionPdf:: will be corrected for wrong normalization by factor ="<<normcorr<<endl; 
 
  
}

RooResolutionPdf::RooResolutionPdf(const RooResolutionPdf& other, const char* name) : 
  RooAbsPdf(other,name),
  resvar("resvar",this,other.resvar),
  resvarp(other.resvarp),
  mass("mass",this,other.mass),
  offset("offset",this,other.offset),
  slope("slope",this,other.slope),
  M1("M1",this,other.M1),
  N2("N2",this,other.N2),
  M2("M2",this,other.M2),
  S2("S2",this,other.S2),
  N3("N3",this,other.N3),
  M3("M3",this,other.M3),
  S3("S3",this,other.S3),
  normcorr(other.normcorr)
{
}

Double_t RooResolutionPdf::evaluate() const
{
  Double_t sigma=offset+slope*mass;
  
  return ((1.-N2-N3)*exp(-.5*(resvar-M1)*(resvar-M1)/(sigma*sigma))/(sigma*2.507)
    + N2*(S2*sigma/3.14159)/((resvar-M1-M2)*(resvar-M1-M2)+(S2*sigma)*(S2*sigma))
    + N3*(S3*sigma/3.14159)/((resvar-M1-M3)*(resvar-M1-M3)+(S3*sigma)*(S3*sigma)))/normcorr;
}

