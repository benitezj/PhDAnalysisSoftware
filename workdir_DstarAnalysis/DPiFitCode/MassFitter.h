
#ifndef MASSFITTER_H 
#define MASSFITTER_H 
#include "../Globals.h"
#include "../AnalysisGlobals.h"
#include "JResiduals.h"
#include "../FUNCTIONS/RebinHisto.h"
#include "ParameterRead.h"

#include "TGraph.h"
#include "RooChi2Var.h"
#include "RooHistPdf.h"
#include "RooAbsOptTestStatistic.h"
#include "RooHistPdf.h"
#include "RooRealVar.h"
#include "RooCategory.h"
#include "RooPlot.h"
#include "RooDataSet.h"
#include "RooDataHist.h"
#include "RooFitResult.h"
#include "RooArgSet.h"
#include "RooCmdArg.h"
#include "RooArgList.h"
#include "RooDataHist.h"
#include "RooAbsReal.h"
#include "RooAbsData.h"
#include "RooGlobalFunc.h"
#include "RooRealProxy.h"
#include "RooMinuit.h"
#include "RooFormula.h"
#include "RooFormulaVar.h"
#include "RooNLLVar1D.h"
#include "RooChi2VarIntegral.h"
#include "RooAddPdf.h"
#include "RooGenericPdf.h"

using namespace RooFit ;

#define BkgPolNpowers 10
#define NSIGNALS 10
#define NBACKGROUNDS 10

class MassFitter:public TNamed{
  ClassDef(MassFitter,1);

 public:
  MassFitter(RooRealVar*MassVar);
  ~MassFitter();

  //
  Bool_t Initialize(TH1* histo,TString configfile);
  Int_t Fit();
  void Save(TString outputdir);


  //
  RooPlot* GetFitPlot();
  //
  RooAbsPdf* GetTotalPdf(){return totalpdf;}
  RooAbsPdf* GetBkgPdf(){return totbkgpdf;}


  ///Beware that yields within this class are fit fractions < 1. so must multiply by Histo integral
  //
  Int_t GetBkgYield(){ return (int)(Histo->Integral()-GetSigYield());}

  Int_t GetSigYield(){
    Float_t syield=0;
    for(Int_t sig=0;sig<NSIGNALS;sig++)
      if( signame[sig]!="0")
	syield+=yield[sig]->getVal();
    
    return (int)(syield*Histo->Integral());
  }


  //Eff pdf
  void SetEfficiencyPdf(RooAbsPdf* pdf){
    effPdf=pdf;
  }
  
  //Resolution
  void SetResolution(RooAbsPdf* ResPdf,RooRealVar* ResVar){respdf=ResPdf;resVar=ResVar;}

  //
  void SetPhaseSpace(RooAbsPdf* PS){TwoBodyPS=PS;}

  //Set dau masses for S and D wave BreitWigners
  void SetDauMasses(RooRealVar* d1mass,RooRealVar* d2mass){dau1Mass=d1mass;dau2Mass=d2mass;}

  //
  void SetEps(Float_t eps){Eps=eps;}

  //
  TString GetFileTag(){return filetag;}
  void SetFileTag(TString tag){filetag=tag;}

 
  //
  TGraph* ScanParameter(TString Signal,TString ParName);
  void ScanParameter2D(TString sig1,TString par1,TString sig2,TString par2,TH2F* H2);

  TH1F* GetNormResidualHisto();
  
  RooPlot* GetContour(TString sig1,TString par1,TString sig2,TString par2);

  //
  Bool_t FixParameter(TString SigName,TString ParName,Float_t value);

  //
  void DifferencePars(TString oldfile,TString newfile="0",TString TableOption="H");
  void MakeParTable(TString TableOption="V",Float_t scale=1.);


  //
  RooPlot* GetBkgSubPlot();
  TH1* GetBkgSubHist();

 private:
  
  //
  void Clean();

  RooRealVar* mass;  
  TString filetag;  

  RooAbsPdf* SignalPdfraw[NSIGNALS];
  RooAbsPdf* SignalPdfcut[NSIGNALS];
  RooAbsPdf* SignalPdfprod[NSIGNALS];
  RooAbsPdf* SignalPdfeff[NSIGNALS];
  RooAbsPdf* SignalPdfres[NSIGNALS];
  RooAbsPdf* SignalPdf[NSIGNALS];
  RooRealVar* yield[NSIGNALS];
  RooRealVar* mean[NSIGNALS];
  RooRealVar* width[NSIGNALS];
  RooAbsPdf* CutOffPdf[NSIGNALS];
  RooRealVar* cutoff[NSIGNALS];
  TString signame[NSIGNALS];
  TString sigshape[NSIGNALS]; 
  Int_t applyps[NSIGNALS];
  Int_t applyres[NSIGNALS];
  Int_t sigdraw[NSIGNALS];
  RooAbsPdf* GetPDF(TString type,Int_t id);
 
  //Background
  Int_t NBkgs;
  RooAbsPdf* BkgPdfraw[NBACKGROUNDS];
  RooAbsPdf* BkgPdfprod[NBACKGROUNDS];
  RooAbsPdf* BkgPdf[NBACKGROUNDS];
  RooRealVar* BkgYield[NBACKGROUNDS];
  RooArgList coeflist[NBACKGROUNDS];
  RooRealVar* coef[BkgPolNpowers][NBACKGROUNDS];
  TString bkgname[NBACKGROUNDS];
  TString bkgtype[NBACKGROUNDS];
  Bool_t applybkgps[NBACKGROUNDS];
  Int_t bkgdraw[NBACKGROUNDS];
  TString GetBackgroundFormula(TString type,TString name);    

  //Background Model as a histogram modified by a polynomial
  TH1* BkgHisto[NBACKGROUNDS];
  RooDataHist* BkgDataSet[NBACKGROUNDS];
  RooAbsPdf* BkgHistPdf[NBACKGROUNDS];
  RooAbsPdf* BkgModifyPdf[NBACKGROUNDS];

  //total background
  RooArgList flist;
  RooArgList ylist;
  RooAbsPdf* totbkgpdf;

  //Phase space factor 
  RooAbsPdf* TwoBodyPS;   

  //Radius for D-wave and S-wave Breit-Wigners
  RooRealVar* dau1Mass;
  RooRealVar* dau2Mass;
  RooRealVar radius;


  //Efficiency pdf
  RooAbsPdf* effPdf;

  //Resolution PDF
  RooAbsPdf* respdf;
  RooRealVar* resVar;
  Bool_t reswasused;

  RooDataHist* histresdat;
  RooHistPdf*  histrespdf;
  RooRealVar*  histresVar;
  Bool_t histreswasused;

  ///Full fit function
  RooArgList pdflist;
  RooArgList yieldlist;
  RooAbsPdf* totalpdfsig; 
  RooAbsPdf* totalpdfprod;
  RooAbsPdf* totalpdfcut;
  RooAbsPdf* totalpdf;  
  TH1* HistoNoCut;
  TH1* Histo;
  RooDataHist* datasetNoCut;
  RooDataHist* dataset;
  RooFitResult* result;
  RooArgList* floatedParameters;  
  RooPlot* massframe;
  RooMinuit* minuit;
  RooAbsOptTestStatistic* chi2; 

  Float_t fitmin;
  Float_t fitmax;
  Int_t nfitbins;   

  RooRealVar cutrangemin;
  RooRealVar cutrangemax;
  RooGenericPdf CutRangePdf;
  Bool_t RemoveCutRange;


  Int_t ndof;
  Float_t Eps;
  Int_t NIntegbins;


  TString YaxisLabel;
  
  char textlabel[10000];
  Int_t ncpu;
  char fitopt[10];

  ULong_t time;

  
  ifstream InputStream;

  Int_t ProcessCommand(TString command);

  TH1* HFitDstPiDM;
};
#endif


