#ifndef Ds1Analysis_hh
#define Ds1Analysis_hh
#include "../Globals.h"
#include "../AnalysisGlobals.h"
#include "../FUNCTIONS/SetHisto.h"
#include "../FUNCTIONS/Fits.h"
#include "JCutSetup.h"
#include "JCutFloat.h"
#include "JCutInt.h"
#include "JCutList.h"
#include "JCut.h"
#include "JDataBase.h"
#include <TVector3.h>
#include <TLine.h>
#include <TGraphErrors.h>
#include <TCut.h>
#include <TBox.h>

#define Npbins  12 //number of momentum bins in Ds1 deltaM resolution

class Ds1Analysis:public TObject{
  ClassDef(Ds1Analysis,1);

 public:
  Ds1Analysis(TChain* ntuple,Bool_t truthMatch,Int_t MatterOrAntiMatter, TString Mode,TString OutputDir,Int_t WhichCuts);
  ~Ds1Analysis();
  

  Int_t ReduceNtuple();
  Int_t ReadReducedNtuple();
  Int_t FitDMResolution();
  Int_t FitDs1DM(Int_t FitType,TString DMResParsDir,TString PstarComparisonFile);

 private:
  
  Int_t SetBranches();
  Int_t SetModeBranches();

  Int_t MakeReducedNtupleBranches();
  Int_t MakeModeReducedNtupleBranches();

  Int_t SaveReducedNtuple();

  Int_t InitHistos();
  Int_t InitModeHistos();
  
  Int_t PrintPlots();
  Int_t PrintModePlots(TCanvas* Canvas,TString filename);


  Int_t OpenReducedNtuple();
  Int_t SetReducedNtupleBranches();
  Int_t SetModeReducedNtupleBranches();
  Int_t Close();

  void DrawTruthRatios(TCanvas* Canvas,TString filename);
  void DrawCutSummary(TCanvas* Canvas,TString filename);
  Int_t DrawPstarComparison(TCanvas* Canvas,TString filename,TString PstarComparisonFile,Int_t Nsignal);
  Int_t DetermineEfficiencyVspstar(Int_t Nsignal);  

  TChain* _fChain;
  Int_t _MatterOrAntiMatter;
  TString _Mode;
  TString _OutputDir;
  Bool_t TruthMatch;
  Int_t _WhichCuts;

  TFile* OutPutFile;
  TTree* ReducedNtuple;



  // Declaration of leave types
  Float_t beamSX;
  Float_t beamSY;
  Float_t beamSZ;

  Float_t primVtxX;
  Float_t primVtxY;
  Float_t primVtxZ;


  Int_t   nDs1p;
  Int_t Ds1pVtxStatus[500];//NOTE: array size must match the reconstructed arrays from BtuTupleMaker!!
  Float_t Ds1pChi2[500];  
  Int_t Ds1pnDof[500];  
  Float_t Ds1pMass[500];  
  Float_t Ds1pcosth[500];  
  Float_t Ds1pcosthCM[500];  
  Float_t Ds1pp3CM[40];  
  Float_t Ds1pphiCM[500];  
  Int_t   Ds1pLund[500];  
  Int_t   Ds1pMCIdx[500];  
  Int_t   Ds1pd1Lund[500];  
  Int_t   Ds1pd1Idx[500];  
  Int_t   Ds1pd2Lund[500];  
  Int_t   Ds1pd2Idx[500];  
  Float_t Ds1pVtxx[500];  
  Float_t Ds1pVtxy[500];  
  Float_t Ds1pVtxz[500];     
  Float_t Ds1pKsMassUsr[500]; 

  Int_t   nKs;
  Float_t KsMass[500]; 
  Float_t Ksp3CM[500]; 
  Int_t   KsLund[500]; 
  Int_t   Ksd1Lund[500]; 
  Int_t   Ksd1Idx[500]; 
  Int_t   Ksd2Lund[500]; 
  Int_t   Ksd2Idx[500];  
  Int_t   KsMCIdx[500];
  Float_t KsChi2[500];
  Int_t   KsnDof[500];
  Int_t   KsVtxStatus[500];
  Float_t KsVtxx[500]; 
  Float_t KsVtxy[500]; 
  Float_t KsVtxz[500]; 
  Float_t Kscosth[500]; 
  Float_t Ksphi[500]; 
  Float_t KsFlightBSLen[500];
  Float_t KsFlightBSCosA[500];

  Int_t   nDstar;
  Float_t DstarChi2[500];  
  Int_t DstarnDof[500];  
  Int_t DstarVtxStatus[500];
  Float_t DstarMass[500];  
  Float_t DstarMassErr[500];
  Float_t Dstarp3[500];   
  Float_t Dstarcosth[500];   
  Float_t Dstarphi[500];  
  Float_t Dstarp3CM[500];  
  Int_t   DstarLund[500];  
  Int_t   Dstard1Lund[500];  
  Int_t   Dstard1Idx[500];  
  Int_t   Dstard2Lund[500];  
  Int_t   Dstard2Idx[500];    
  Int_t   DstarMCIdx[500];
  Float_t DstarD0VtxProbUsr[500];
  Float_t DstarVtxx[500];  
  Float_t DstarVtxy[500];  
  Float_t DstarVtxz[500];

  Int_t   nD0;
  Float_t D0Mass[500];  
  Float_t D0MassErr[500];
  Float_t D0p3[500];  
  Float_t D0costh[500];  
  Float_t D0phi[500];    
  Float_t D0p3CM[500];  
  Int_t   D0Lund[500];  
  Int_t   D0d1Lund[500];  
  Int_t   D0d1Idx[500];  
  Int_t   D0d2Lund[500];  
  Int_t   D0d2Idx[500];   
  Int_t   D0d3Lund[500];  
  Int_t   D0d3Idx[500];
  Int_t   D0d4Lund[500];     
  Int_t   D0d4Idx[500];
  Int_t   D0MCIdx[500];
  Float_t D0Chi2[500];
  Int_t   D0nDof[500];
  Int_t   D0VtxStatus[500];
  Float_t D0Vtxx[500];  
  Float_t D0Vtxy[500];  
  Float_t D0Vtxz[500];

  Int_t   nPi;
  Float_t Pip3[500];  
  Int_t   PiLund[500];   
  Int_t   PiMCIdx[500];
  Int_t   PiTrkIdx[500];
  Int_t   PiSelectorsMap[500];
  Int_t   nK;
  Float_t Kp3[500]; 
  Int_t   KLund[500]; 
  Int_t   KMCIdx[500];
  Int_t   KTrkIdx[500];
  Int_t   KSelectorsMap[500];
  Int_t TRKnSvt[500];
  Int_t TRKLund[500];
  ////MC block
  Int_t mcLen;
  Int_t mcLund[500];
  Int_t mothIdx[500];
  Int_t dauLen[500];
  Int_t dauIdx[500];
  Float_t mcmass[500]; 
  Float_t mccosth[500];
  Float_t mcp3[500];
  Float_t mccosthCM[500];
  Float_t mcp3CM[500]; 



  //////My derived variables
  Float_t D0Probab;
  Float_t KsProbab;
  Int_t MCDs1pCounterPerEvent;
  Int_t MCDs1pCounterTotal;
  Int_t RecoDs1pCounterTotal;
  Int_t RecoDs1pCounterPerEvent;
  
  ////indexes
  Int_t   Ds1pIdx;
  Int_t   DstarIdx;
  Int_t   KsIdx;
  Int_t   D0Idx;
  Int_t   PiIdx;
  Int_t   D0Pi2Idx;
  Int_t   D0Pi3Idx;
  Int_t   D0Pi2trkIdx;
  Int_t   D0Pi3trkIdx;
  Int_t   SlowPiIdx;
  Int_t   KIdx;
  Int_t   PitrkIdx;
  Int_t   KtrkIdx;
  Int_t   SlowPitrkIdx;
  Int_t   KsPi1Idx;
  Int_t   KsPi2Idx;
  Int_t   KsPi1trkIdx;
  Int_t   KsPi2trkIdx;

  ///////Variables to be saved into reduced ntuple
  Int_t eventnumber;
  Float_t ds1pmass;
  Float_t ds1pdeltam1;
  Float_t ds1pdeltam2;
  Float_t ds1ppstar;
  Float_t ds1pmcpstar;
  Float_t ds1pcosstar;
  Float_t ds1pmccosstar;
  Int_t ds1pvtxstatus;
  Float_t ds1plogvtxprob;
  Float_t ds1pdmres;
  Int_t ds1pcharge;
  Int_t ds1pmctrue;
  Int_t ds1pbestvtxprob;
  Int_t ds1pnDs1p;
  Float_t dstarmass;
  Float_t dstardeltam;
  Float_t dstarvtxprob;
  Int_t dstarmctrue;
  Float_t d0mass;
  Float_t d0pstar;
  Float_t d0logvtxprob;
  Float_t d0cosine;
  Float_t d0cosine1;
  Int_t d0mctrue;
  Float_t ksmass;
  Float_t ksflightbslen;
  Float_t ksflightbscosa;
  Float_t kscosine;
  Float_t kscosine1;
  Float_t kscosine2;
  Int_t ksmctrue;
  Float_t ksvtxprob;
  Int_t klh;
  Int_t kmctrue;
  Int_t pilh;
  Int_t pimctrue;
  Int_t d0pi2lh;
  Int_t d0pi3lh;
  Int_t pi1lh;
  Int_t pi1mctrue;
  Int_t pi2lh;
  Int_t pi2mctrue;
  Float_t kp3;
  Float_t pip3;
  Float_t slowpip3;
  Float_t pi1p3;
  Float_t pi2p3;
  Int_t slowpimctrue;
  
  
  //mass windows
  Float_t DstarMassWindow; 
  Float_t D0MassWindow;
  Float_t KsMassWindow;
  Float_t Ds1pp3CMmax;
  Float_t Ds1pp3CMmin;
  Int_t Ds1pp3CMNbins;
  Float_t Ds1pMassWindow;
  Float_t Ds1pPDGDeltaM1;
  Float_t Ds1pPDGDeltaM2;
  Int_t Ds1DeltaMNbins;
  Float_t DstarDeltaMmin;
  Float_t DstarDeltaMmax;
  Int_t DstarDeltaMNbins;
  Float_t Ds1Cosstarmin;
  Float_t Ds1Cosstarmax;
  Int_t Ds1CosstarNbins;
  

  /////need the database
  JDataBase database;


  ///Ds1 Distributions as function of cuts
  JCutSetup PiMomentum;
  JCutSetup KMomentum;
  JCutSetup Pi1Momentum;
  JCutSetup Pi2Momentum;
  JCutSetup KsMassCuts;
  JCutSetup KsCosineCuts;
  JCutSetup D0PstarCuts;
  JCutSetup D0MassCuts;
  JCutSetup SlowPiMomentum;
  JCutSetup DstarDeltaMCuts;
  JCutSetup Ds1pMassCuts;
  JCutSetup Ds1pDeltaM1Cuts;
  JCutSetup Ds1pDeltaM2Cuts;
  JCutSetup Ds1ppstarCuts;
  JCutSetup Ds1pcosstarCuts;
  JCutSetup D0ProbCuts;
  JCutSetup Ds1pProbCuts;
  JCutSetup KLH;
  JCutSetup PiLH;
  

  //the cuts
  JCutFloat Ds1pDeltaM2Cut;
  JCutInt Ds1pBestVtxProbCut;
  JCutFloat D0pstarCut;
  JCutFloat DstarDeltaMCut;
  JCutInt KLHCut;
  JCutInt PiLHCut;  
  JCutFloat D0MassCut;
  JCutFloat D0ProbCut;
  JCutFloat KsMassCut;
  JCutFloat KsCosineCut;
  JCutFloat Ds1pProbCut;
  JCutFloat Ds1ppstarCut;
  JCutInt Pi1LHCut;
  JCutInt Pi2LHCut;
  JCutInt Ds1pMCTrueCut;
  JCutInt D0Pi2LHCut;
  JCutInt D0Pi3LHCut;
 

  ///a list of cuts
  JCutList Ds1pCuts;
  


  //2D Distribution  
  TH2F* H2Ds1pCMPvsTheta;
  TH2F* H2DstarDeltaMVsD0Mass;

  //momentum distributions of 2 other D0 pions
  /*  TH1F* HD0Pi1Momentum; */
  /*   TH1F* HD0Pi2Momentum; */


  //MC produced
  TH1F* HMCDs1pMass;
  TH1F* HMCDs1pp3CM;
  TH1F* HMCDs1pp3CMTorsten;
  TH1F* HMCDs1pcosthCM;
  TH2F* H2MCDs1pCMPvsTheta;
  TH1F* HMCNDs1p;

  //Reconstructed per event
  TH1F HRecoNDs1p;

  //check indexes 
  TH1F* HLundCheck;


  //a cut drawing line
  TLine cutline;
  TText text;
  char textlabel[100];

  TBox cutbox;
 
 
};
  

#endif

