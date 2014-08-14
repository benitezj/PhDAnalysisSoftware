#ifndef AbsAnalysis_hh
#define AbsAnalysis_hh
#include "../Globals.h"
#include "../AnalysisGlobals.h"
#include "../FUNCTIONS/SetHisto.h"
#include "../FUNCTIONS/Fits.h"
#include "../FUNCTIONS/MoveStatsBox.h"
#include "../FUNCTIONS/DetermineFWHM.C"
#include "../FUNCTIONS/RebinHisto.h"
#include "MassFitter.h"

#include "ParameterRead.h"

#include "JResiduals.h"
//#include "AbsDataBase.h"
#include <TVector3.h>
#include <TLorentzRotation.h>
#include <TLorentzVector.h>
#include <TLine.h>
#include <TGraphErrors.h>
#include <TCut.h>
#include <TBox.h>
#include <TStyle.h>
#include <TNamed.h>

//////needed for RooFit
#include <TLine.h>
#include <TGraphErrors.h>
#include <TLatex.h>
#include "RooRealVar.h"
#include "RooCategory.h"
#include "RooPlot.h"
#include "RooDataSet.h"
#include "RooDataHist.h"
#include "RooAddPdf.h"
#include "RooProdPdf.h"
#include "RooFitResult.h"
#include "RooBreitWigner.h"
#include "./RooGenericPdfN.h"
#include "./RooProdPdfN.h"
#include "./RooAddPdfN.h"
#include "RooGenericPdf.h"
#include "RooArgSet.h"
#include "RooCmdArg.h"
#include "RooArgList.h"
#include "RooDataHist.h"
#include "RooAbsReal.h"
//#include "RooNumConvPdf.h"
#include "RooAbsData.h"
#include "RooGlobalFunc.h"
#include "RooChebychev.h"
#include "./Roo2BodyPhaseSpace.h"
#include "RooHist.h"


using namespace RooFit ;
////////////////////////


#define MAXMCLEN 200

class AbsAnalysis:public TNamed{
  ClassDef(AbsAnalysis,1);

 public:
  AbsAnalysis(Int_t MCorDATA,TString OutputDir);
  ~AbsAnalysis();
  
  //chain of ntuples to be reduced 
  void SetChain(TChain* ch){_fChain=ch;}

  //
  virtual Int_t ReduceNtuple();
 
  //
  TTree* GetNtuple(){OpenReducedNtuple();return ReducedNtuple;}

  //
  TString GetOutPutDir(){return _OutputDir;}
 
  //
  //AbsDataBase* GetDataBase(){return topdatabase;}

  //
  TObject* GetMCHisto(TString name){
    TFile F(_OutputDir+"/Ntuple.root");
    gROOT->cd();
    TObject*H=F.Get(name);
    if(!H)return NULL;
    H=H->Clone();
    F.Close();
    return H;
  }


 private:
   
  void FillEventVars();  
  Int_t SetEventBranches();
  Int_t MakeEventReducedNtupleBranches(TTree* Ntuple);
  Int_t SetEventReducedNtupleBranches();

  Int_t OpenReducedNtuple();
  Int_t CloseReducedNtuple();
  Int_t OpenReducedFile();
  Int_t CloseReducedFile();
  
  Double_t ComputexyzMass(Double_t xp3,Double_t xcosth,Double_t xphi,Double_t xmass,
			  Double_t yp3,Double_t ycosth,Double_t yphi,Double_t ymass,
			  Double_t zp3,Double_t zcosth,Double_t zphi,Double_t zmass);
  
  Double_t ComputexyMass(Double_t xp3,Double_t xcosth,Double_t xphi,Double_t xmass,
			 Double_t yp3,Double_t ycosth,Double_t yphi,Double_t ymass);
  


  //-----------------------------------------------------------------------------
  TChain* _fChain;
  Int_t _MCorDATA;//0=signal, 1=generic 2=data
  TString _OutputDir;//directory where results will be dumped
  TFile* OutPutFile;//temporary file where results are saved.
  TFile* ReducedFile;//file where reduced ntuple and clean ntuple are  saved
  TChain* ReducedNtuple;//ntuple derived from BtuTupleMaker ntuples
  //AbsDataBase* topdatabase;



  //
  TLine cutline;
  TLatex text;
  char textlabel[10000];

  TString XLabel;
  TString YLabel;  

  //for saving plots
  TCanvas Canvas;
  TString filename;
  

  //reading parameters
  ParameterRead inputpars; 
  

  // ntuple leaves
  Int_t runNumber;
  Float_t eePx;      
  Float_t eePy;   
  Float_t eePz;    
  Float_t eeE;      
  Float_t beamSX;   
  Float_t beamSY;   
  Float_t beamSZ;    
/*   Float_t primVtxX; */
/*   Float_t primVtxY; */
/*   Float_t primVtxZ; */


  ////MC block
  Int_t   mcLen;
  Int_t   mcLund[MAXMCLEN];
  Int_t   mothIdx[MAXMCLEN];
  Int_t   dauLen[MAXMCLEN];
  Int_t   dauIdx[MAXMCLEN];
  Float_t mcmass[MAXMCLEN]; 
  Float_t mccosth[MAXMCLEN];
  Float_t mcphi[MAXMCLEN];
  Float_t mcp3[MAXMCLEN];
  Float_t mcenergy[MAXMCLEN];
  Float_t mccosthCM[MAXMCLEN];
  Float_t mcphiCM[MAXMCLEN];
  Float_t mcp3CM[MAXMCLEN];
  Float_t mcenergyCM[MAXMCLEN];
  Float_t mcVtxx[MAXMCLEN]; 
  Float_t mcVtxy[MAXMCLEN]; 
  Float_t mcVtxz[MAXMCLEN]; 
  
  ///////Variables to be saved into reduced ntuple
  Int_t eventid;//event id analyzed in the framework
  Int_t runnumber;//1-6
  Float_t beamenergy;
  Float_t beamcmenergy;

  Int_t ngoodthisevent;
  Int_t goodeventid;
};
  

#endif

