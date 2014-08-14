#ifndef JResiduals_hh
#define JResiduals_hh
#include "../Globals.h"
#include "RooAbsPdf.h"
#include "RooRealVar.h"
#include "RooPlot.h"
#include <TPad.h>
#include <TLine.h>

class JResiduals:public TNamed{
  ClassDef(JResiduals,1);

 public:
  JResiduals(const char* name,TH1* H, TF1* F);
  JResiduals(const char* name,TH1* H, RooAbsPdf* F,RooRealVar* Var);
  JResiduals(const char* name,TH1* H, TH1* h);
  ~JResiduals();

  TH1F* GetResidualHisto(){return (TH1F*)H1Residuals->Clone();}
  TH1F* GetNormResidualHisto(){return (TH1F*)H1NormResiduals->Clone();}
  TH1F* GetChisquareHisto(){return (TH1F*)HChisquare->Clone();}
  TH1F* GetNormResidualProjection();
  Float_t GetChisquare(){return chisquare;}
  Float_t GetChisquarePerNdof(){return chisquare/ndof;}
  void MakeFitPlotWithResiduals(TCanvas* C,RooPlot* rooplot);
  void MakeFitPlotWithResiduals(TCanvas* C,RooPlot* rooplot,TH1* Hist);

 private: 

  void DrawNormResiduals();
  
  TH1* Histo;
  TF1* FitFunc;
  RooAbsPdf* FitPdf;
  RooRealVar* IntegralVariable;
  TH1* FitHisto;
  TH1F* H1Residuals;
  TH1F* H1NormResiduals;
  TH1F* HNormResProj;
  Float_t chisquare;
  Int_t ndof;
  TString name;
  void ComputeResiduals();
  Float_t GetIntegralPdf(Float_t xlow,Float_t xhigh);
  TH1F* HChisquare;

  //
  TPad* fitpad;
  TPad* respad;

};
  
#endif

