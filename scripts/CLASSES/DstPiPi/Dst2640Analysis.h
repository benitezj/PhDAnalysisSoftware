#ifndef Dst2640Analysis_hh
#define Dst2640Analysis_hh
#include "../../Globals.h"
#include "../../AnalysisGlobals.h"
#include "../../FUNCTIONS/SetHisto.h"
#include "../../FUNCTIONS/Fits.h"
#include "../../FUNCTIONS/MoveStatsBox.h"
#include "../JCutSetup.h"
#include "../JCut.h"
#include "../JCutFloat.h"
#include "../JCutInt.h"
#include "../JCutList.h"
#include "../JCutEllipse.h"
#include "JDataBase2640.h"
#include <TVector3.h>
#include <TLorentzRotation.h>
#include <TLorentzVector.h>
#include <TLine.h>
#include <TGraphErrors.h>
#include <TCut.h>
#include <TBox.h>


#define Npbins 6 //number of momentum bins in Dst2640 deltaM resolution

#define MAXNCANDS 50

class Dst2640Analysis:public TObject{
  ClassDef(Dst2640Analysis,1);

 public:
  Dst2640Analysis(TChain* ntuple,Bool_t truthMatch,Int_t MatterOrAntiMatter, TString Mode,TString OutputDir,Int_t WhichCuts,Bool_t RunInTestMode);
  ~Dst2640Analysis();
  
  Int_t ReduceNtuple();
  Int_t ReadReducedNtuple();
/*   Int_t FitDMResolution(); */
/*   Int_t FitDst2640DM(Int_t FitType,TString DMResParsDir,Float_t helcut,Float_t pcut); */
/*   Int_t DrawBkgPlot(Bool_t signal,Bool_t ccbar,Bool_t uds,Bool_t bbbar,Bool_t tau); */
/*   Int_t DrawBkgSubMass(Float_t binsize,Float_t lowpstar,Float_t dstarhel); */
  Int_t DrawDstKs();


 private:
  
  Int_t SetBranches();
  Int_t SetModeBranches();

  Int_t MakeReducedNtupleBranches();
  Int_t MakeModeReducedNtupleBranches();

  Int_t InitHistos();
  Int_t InitModeHistos();
  
  Int_t OpenReducedNtuple();
  Int_t SetReducedNtupleBranches();
  Int_t SetModeReducedNtupleBranches();
  Int_t Close();

  Int_t OpenBkgNtuple();
  Int_t CloseBkgNtuple();

  Int_t PrintPlots();
  Int_t PrintModePlots(TCanvas* Canvas,TString filename);

  void DrawTruthRatios(TCanvas* Canvas,TString filename);
  void DrawCutSummary(TCanvas* Canvas,TString filename);

  Float_t ComputeDstarAngle();
  Float_t ComputeMsqDstarPi(Int_t WhichPiIdx);
  void DrawDalitzPlots(TCanvas* Canvas,TString filename);
  Int_t  DrawMassPlots(TCanvas* Canvas,TString filename,Int_t WhichMassRange,Int_t SubtractBkg);
  Float_t ComputePiPiMass();
  void ComputeDstarPiMass(Float_t *rs,Float_t *ws);
  void ComputeDstarPiPiMass(Float_t *rs,Float_t *ws);
  Float_t ComputeDstarPiPiCosine();
  Float_t ComputePiPiCosine();
  void ComputeDstarPiCosHel(Float_t* cosiners, Float_t* cosinews);
  void ComputeDstarPipstar(Float_t* pstarrs, Float_t* pstarws);


  TChain* _fChain;
  Int_t _MatterOrAntiMatter;
  TString _Mode;
  TString _OutputDir;
  Bool_t TruthMatch;
  Int_t _WhichCuts;

  TFile* OutPutFile;
  TTree* ReducedNtuple;

  //Backgrounds
  TString ModeDir;
  TFile* Fccbar;
  TTree* ccbartree;


  // Declaration of leave types
  Float_t beamSX;
  Float_t beamSY;
  Float_t beamSZ;
  Float_t FoxWolfMom0;
  Float_t FoxWolfMom1;
  Float_t FoxWolfMom2;
  Float_t FoxWolfMom3;
  Float_t Thrust;
  Float_t Thrustx;
  Float_t Thrusty;
  Float_t Thrustz;
  Float_t primVtxX;
  Float_t primVtxY;
  Float_t primVtxZ;

  Int_t   nDst2640;
  Float_t Dst2640Chi2[MAXNCANDS];  //Must be a float!
  Int_t Dst2640nDof[MAXNCANDS];  
  Int_t Dst2640VtxStatus[MAXNCANDS];
  Float_t Dst2640Mass[MAXNCANDS];
  Float_t Dst2640p3[MAXNCANDS];    
  Float_t Dst2640costh[MAXNCANDS];
  Float_t Dst2640phi[MAXNCANDS];   
  Float_t Dst2640p3CM[MAXNCANDS];  
  Float_t Dst2640costhCM[MAXNCANDS];  
  Float_t Dst2640phiCM[MAXNCANDS];  
  Int_t   Dst2640Lund[MAXNCANDS];  
  Int_t   Dst2640MCIdx[MAXNCANDS];  
  Int_t   Dst2640d1Lund[MAXNCANDS];  
  Int_t   Dst2640d1Idx[MAXNCANDS];  
  Int_t   Dst2640d2Lund[MAXNCANDS];  
  Int_t   Dst2640d2Idx[MAXNCANDS];  
  Int_t   Dst2640d3Lund[MAXNCANDS];  
  Int_t   Dst2640d3Idx[MAXNCANDS];  
  Float_t Dst2640Vtxx[MAXNCANDS];  
  Float_t Dst2640Vtxy[MAXNCANDS];  
  Float_t Dst2640Vtxz[MAXNCANDS];     
  Int_t   nDstar;
  Float_t DstarChi2[MAXNCANDS];  
  Int_t DstarnDof[MAXNCANDS];  
  Int_t DstarVtxStatus[MAXNCANDS];
  Float_t DstarMass[MAXNCANDS];  
  Float_t DstarMassErr[MAXNCANDS];
  Float_t Dstarp3[MAXNCANDS];    
  Float_t Dstarcosth[MAXNCANDS];  
  Float_t Dstarphi[MAXNCANDS];   
  Float_t Dstarp3CM[MAXNCANDS];  
  Float_t DstarcosthCM[MAXNCANDS];  
  Float_t DstarphiCM[MAXNCANDS]; 
  Int_t   DstarLund[MAXNCANDS];  
  Int_t   Dstard1Lund[MAXNCANDS];  
  Int_t   Dstard1Idx[MAXNCANDS];  
  Int_t   Dstard2Lund[MAXNCANDS];  
  Int_t   Dstard2Idx[MAXNCANDS];    
  Int_t   DstarMCIdx[MAXNCANDS];
  Float_t DstarD0VtxProbUsr[MAXNCANDS];
  Int_t   nD0;
  Float_t D0Mass[MAXNCANDS];  
  Float_t D0MassErr[MAXNCANDS];  
  Float_t D0p3CM[MAXNCANDS];  
  Float_t D0costhCM[MAXNCANDS];  
  Float_t D0phiCM[MAXNCANDS];  
  Int_t   D0Lund[MAXNCANDS];  
  Int_t   D0d1Lund[MAXNCANDS];  
  Int_t   D0d1Idx[MAXNCANDS];  
  Int_t   D0d2Lund[MAXNCANDS];  
  Int_t   D0d2Idx[MAXNCANDS];   
  Int_t   D0d3Lund[MAXNCANDS];  
  Int_t   D0d3Idx[MAXNCANDS];
  Int_t   D0d4Lund[MAXNCANDS];     
  Int_t   D0d4Idx[MAXNCANDS];
  Int_t   D0MCIdx[MAXNCANDS];
  Float_t D0Chi2[MAXNCANDS];
  Int_t   D0nDof[MAXNCANDS];
  Int_t   D0VtxStatus[MAXNCANDS];
  Int_t   nPi;
  Float_t Pip3[MAXNCANDS];  
  Float_t Picosth[MAXNCANDS]; 
  Float_t Piphi[MAXNCANDS]; 
  Float_t Pip3CM[MAXNCANDS]; 
  Float_t PicosthCM[MAXNCANDS]; 
  Float_t PiphiCM[MAXNCANDS]; 
  Int_t   PiLund[MAXNCANDS];   
  Int_t   PiMCIdx[MAXNCANDS];
  Int_t   PiTrkIdx[MAXNCANDS];
  Int_t   PiSelectorsMap[MAXNCANDS];
  Int_t   nK;
  Float_t Kp3[MAXNCANDS]; 
  Float_t Kp3CM[MAXNCANDS]; 
  Float_t KcosthCM[MAXNCANDS]; 
  Float_t KphiCM[MAXNCANDS]; 
  Int_t   KLund[MAXNCANDS]; 
  Int_t   KMCIdx[MAXNCANDS];
  Int_t   KTrkIdx[MAXNCANDS];
  Int_t   KSelectorsMap[MAXNCANDS];
  Int_t TRKnSvt[MAXNCANDS];
  Int_t TRKLund[MAXNCANDS];
  ////MC block
  Int_t mcLen;
  Int_t mcLund[MAXNCANDS*2];
  Int_t mothIdx[MAXNCANDS*2];
  Int_t dauLen[MAXNCANDS*2];
  Int_t dauIdx[MAXNCANDS*2];
  Float_t mcmass[MAXNCANDS*2]; 
  Float_t mccosth[MAXNCANDS*2];
  Float_t mcphi[MAXNCANDS*2];
  Float_t mcp3[MAXNCANDS*2];
  Float_t mccosthCM[MAXNCANDS*2];
  Float_t mcphiCM[MAXNCANDS*2];
  Float_t mcp3CM[MAXNCANDS*2]; 

  //////My derived variables
  Float_t D0Probab;
  Int_t MCDst2640CounterPerEvent;
  Int_t MCDst2640CounterTotal;
  Int_t RecoDst2640CounterTotal;
  Int_t RecoDst2640CounterPerEvent;
  

  ////indexes
  Int_t   Dst2640Idx;
  Int_t   DstarIdx;
  Int_t   D0Idx;
  Int_t   PiIdx;
  Int_t   SlowPiIdx;
  Int_t   KIdx;
  Int_t   Pi0Idx;
  Int_t   GammaIdx;  
  Int_t   PitrkIdx;
  Int_t   KtrkIdx;
  Int_t   D0Pi2Idx;
  Int_t   D0Pi3Idx;
  Int_t   D0Pi2trkIdx;
  Int_t   D0Pi3trkIdx;
  Int_t   SlowPitrkIdx;
  Int_t   Pi1Idx;
  Int_t   Pi2Idx;
  Int_t   Pi1trkIdx;
  Int_t   Pi2trkIdx;

  ///////Variables to be saved into reduced ntuple
  Int_t eventnumber;//event
  Float_t eventfwm0;
  Float_t eventfwm1;
  Float_t eventfwm2;
  Float_t eventfwm3;
  Float_t eventthrust;
  Float_t eventthrustx;
  Float_t eventthrusty;
  Float_t eventthrustz;
  Float_t dst2640mass;//D*pipi
  Float_t dst2640deltam;
  Float_t dst2640pstar;
  Float_t dst2640mcpstar;
  Float_t dst2640cosstar;
  Float_t dst2640mccosstar;
  Float_t dst2640logvtxprob;
  Float_t dst2640dmres;
  Int_t dst2640charge;
  Int_t dst2640mctrue;
  Int_t dst2640bestvtxprob;
  Int_t dst2640nDst2640;
  Float_t dstarmass;//D*
  Float_t dstardeltam;
  Float_t dstarpstar;
  Float_t dstarlogvtxprob;
  Int_t dstarmctrue;
  Float_t dstarcostheta;
  Float_t d0mass;//D0
  Float_t d0pstar;
  Float_t d0vtxprob;
  Float_t d0logvtxprob;
  Int_t d0mctrue;  
  Int_t klh;//D0 Kaon
  Int_t kmctrue;
  Float_t kp3;    
  Int_t pilh;//prim. D0 pion
  Float_t pip3;  
  Int_t pimctrue;  
  Int_t d0pi2mctrue;//other D0 pions
  Int_t d0pi2lh;
  Int_t d0pi3lh;
  Int_t d0pi3mctrue;
  Int_t slowpimctrue;//slow pion
  Float_t slowpip3;  
  Int_t pi1lh;//pi1
  Int_t pi1mctrue;
  Float_t pi1p3;
  Int_t pi1charge;
  Int_t pi2lh;//pi2
  Int_t pi2mctrue;
  Float_t pi2p3;
  Int_t pi2charge;
  Float_t msqdstarpiminus;//other quatities
  Float_t msqdstarpiplus;
  Float_t dstarpipicosine;
  Float_t pipimass;
  Float_t pipicosine;
  Float_t dstarpideltam;
  Float_t dstarpideltamws;
  Float_t mdstarpirs;
  Float_t mdstarpiws;
  Float_t mdstarpipirs;
  Float_t mdstarpipiws;
  Int_t pi1pluspi2charge;
  Float_t dstarpipstarrs;
  Float_t dstarpipstarws;
  Float_t dstarpicoshelrs;
  Float_t dstarpicoshelws;
  Int_t d0mode;
  
  //mass windows
  Float_t DstarMassWindow; 
  Float_t Dst2640p3CMmax;
  Float_t Dst2640p3CMmin;
  Int_t Dst2640p3CMNbins;
  Float_t Dst2640MassWindow;
  Float_t Dst2640PDGDeltaM;
  Int_t Dst2640DeltaMNbins;
  Float_t DstarDeltaMmin;
  Float_t DstarDeltaMmax;
  Int_t DstarDeltaMNbins;
  Float_t Dst2640Cosstarmin;
  Float_t Dst2640Cosstarmax;
  Int_t Dst2640CosstarNbins;
  Int_t NDalitzPlotBins;
  Float_t DalitzMin;
  Float_t DalitzMax;


  /////need the database
  JDataBase2640 database;


  ///Dst2640 Distributions as function of cuts
  JCutSetup PiMomentum;
  JCutSetup KMomentum;
  JCutSetup Pi1Momentum;
  JCutSetup Pi2Momentum;
  JCutSetup SlowPiMomentum;
  JCutSetup D0Pi2Momentum;
  JCutSetup D0Pi3Momentum;
  JCutSetup Dst2640MassCuts;
  JCutSetup Dst2640cosstarCuts;
  
  //distributions used as discriminating variables
  JCutSetup Dst2640DeltaMCuts; 
  JCutSetup D0PstarCuts;
  JCutSetup DstarDeltaMCuts;
  JCutSetup KLH;
  JCutSetup PiLH;
  JCutSetup D0MassCuts;
  JCutSetup D0ProbCuts;
  JCutSetup Dst2640ProbCuts;
  JCutSetup Dst2640pstarCuts;
  JCutSetup DstarCosThetaCuts;


  //the cuts
  JCutInt Dst2640MCTrueCut;
  JCutFloat Dst2640DeltaMCut;
  JCutFloat D0pstarCut;
  JCutFloat DstarDeltaMCut;
  JCutInt KLHCut;
  JCutInt PiLHCut;  
  JCutFloat D0MassCut;
  JCutFloat D0ProbCut;
  JCutFloat Dst2640ProbCut;
  JCutInt Pi1LHCut;
  JCutInt Pi2LHCut;
  JCutInt Pi1Pi2ChargeCut; 
  JCutInt D0Pi2LHCut;
  JCutInt D0Pi3LHCut; 
  JCutFloat Dst2640pstarCut;
  JCutFloat HelicityCut;


  //other cuts
  JCutEllipse D0MassAndDstarDMCut;
  JCutInt Dst2640BestVtxProbCut;
 

  ///a list of cuts
  JCutList Dst2640Cuts;
  


  //2D Distribution  
  TH2F* H2Dst2640CMPvsTheta;
  TH2F* H2DstarDeltaMVsD0Mass;
  TH2F* H2D0pstarVsDst2640pstar;

  //MC produced
  TH1F* HMCDst2640Mass;
  TH1F* HMCDst2640DeltaM;
  TH1F* HMCDst2640p3CM;
  TH1F* HMCDst2640costhCM;
  TH2F* H2MCDst2640CMPvsTheta;
  TH1F* HMCNDst2640;
  TH2F* H2MCDst2640DalitzPlot;

  //Reconstructed per event
  TH1F HRecoNDst2640;


  ///MC scale factors
  //totalscalefactor for each MC type
  Float_t lumidata;
  Float_t Crossccbar;

  Float_t scalefactorccbar;
  Float_t scalefactoruds;
  Float_t scalefactorbbbar;
  Float_t scalefactortau;
  


  //check indexes 
  TH1F* HLundCheck;

  //a cut drawing line
  TLine cutline;
  TText text;
  char textlabel[100];

  TCanvas Canvas;
  TString filename;

  //a box
  TBox cutbox;
 
  // axis label
  char DstarPiPiXLabel[100];
  char DstarPiPiYLabel[100];
  
  //
  Bool_t _RunInTestMode;

  // 
  TCut DstPiPipstarcut;
 


};
  

#endif

