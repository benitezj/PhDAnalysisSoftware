#ifndef DstPiAnalysis_hh
#define DstPiAnalysis_hh
#include "../Globals.h"
#include "../AnalysisGlobals.h"
#include "../FUNCTIONS/SetHisto.h"
#include "../FUNCTIONS/Fits.h"
#include "../FUNCTIONS/MoveStatsBox.h"
#include "JCutSetup.h"
#include "JCut.h"
#include "JCutFloat.h"
#include "JCutInt.h"
#include "JCutList.h"
#include "JCutEllipse.h"
#include "JDataBaseDstPi.h"
#include <TVector3.h>
#include <TLorentzRotation.h>
#include <TLorentzVector.h>
#include <TLine.h>
#include <TGraphErrors.h>
#include <TCut.h>
#include <TBox.h>

#define MAXNCANDS 50
#define MYDSTPILUND 523 //this should change in the future from B*+ to B*0 



class DstPiAnalysis:public TObject{
  ClassDef(DstPiAnalysis,1);

 public:
  DstPiAnalysis(TChain* ntuple,Bool_t TruthMatch,Int_t MatterOrAntiMatter,TString OutputDir);
  ~DstPiAnalysis();
  
  Int_t ReduceNtuple(Bool_t RunInTestMode);
  Int_t RooFitMassPeak();
  Int_t Resolution();
    
 private:
  
  Int_t SetBranches();
  Int_t SetModeBranches();

  Int_t MakeReducedNtupleBranches();
  Int_t MakeModeReducedNtupleBranches();

  Int_t SetReducedNtupleBranches();
  Int_t SetModeReducedNtupleBranches();

  Int_t OpenReducedNtuple(Int_t WhichOne);
  Int_t CloseReducedNtuple();
 
  Float_t ComputeDstarAngle();
  Float_t CalculateGenMass();

  TChain* _fChain;
  Int_t _MatterOrAntiMatter;

  TString _OutputDir;
  Bool_t _TruthMatch;

  TFile* OutPutFile;
  TTree* ReducedNtuple;


  // Declaration of leave types
  Float_t beamSX;
  Float_t beamSY;
  Float_t beamSZ;
  Float_t primVtxX;
  Float_t primVtxY;
  Float_t primVtxZ;

  Int_t   nDstPi;
  Float_t DstPiChi2[MAXNCANDS];  //Must be a float!
  Int_t   DstPinDof[MAXNCANDS];  
  Int_t   DstPiVtxStatus[MAXNCANDS];
  Float_t DstPiMass[MAXNCANDS];
  Float_t DstPip3[MAXNCANDS];    
  Float_t DstPicosth[MAXNCANDS];
  Float_t DstPiphi[MAXNCANDS];   
  Float_t DstPip3CM[MAXNCANDS];  
  Float_t DstPicosthCM[MAXNCANDS];  
  Float_t DstPiphiCM[MAXNCANDS];  
  Int_t   DstPiLund[MAXNCANDS];  
  Int_t   DstPiMCIdx[MAXNCANDS];  
  Int_t   DstPid1Lund[MAXNCANDS];  
  Int_t   DstPid1Idx[MAXNCANDS];  
  Int_t   DstPid2Lund[MAXNCANDS];  
  Int_t   DstPid2Idx[MAXNCANDS];  
  Int_t   DstPid3Lund[MAXNCANDS];  
  Int_t   DstPid3Idx[MAXNCANDS];  
  Float_t DstPiVtxx[MAXNCANDS];  
  Float_t DstPiVtxy[MAXNCANDS];  
  Float_t DstPiVtxz[MAXNCANDS];     
  Int_t   nDstar;
  Float_t DstarChi2[MAXNCANDS];  
  Int_t   DstarnDof[MAXNCANDS];  
  Int_t   DstarVtxStatus[MAXNCANDS];
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
  Int_t   TRKnSvt[MAXNCANDS];
  Int_t   TRKLund[MAXNCANDS];
  ////MC block
  Int_t   mcLen;
  Int_t   mcLund[MAXNCANDS*2];
  Int_t   mothIdx[MAXNCANDS*2];
  Int_t   dauLen[MAXNCANDS*2];
  Int_t   dauIdx[MAXNCANDS*2];
  Float_t mcmass[MAXNCANDS*2]; 
  Float_t mccosth[MAXNCANDS*2];
  Float_t mcphi[MAXNCANDS*2];
  Float_t mcp3[MAXNCANDS*2];
  Float_t mccosthCM[MAXNCANDS*2];
  Float_t mcphiCM[MAXNCANDS*2];
  Float_t mcp3CM[MAXNCANDS*2]; 


  //--------------------------------------
  //////My derived variables 
  //-------------------------------------
  
  //Counters
  Int_t MCDstPiCounterPerEvent;
  Int_t MCDstPiCounterTotal;
  Int_t RecoDstPiCounterTotal;
  Int_t RecoDstPiCounterPerEvent;
  

  ////indexes
  Int_t   DstPiIdx;
  Int_t   DstarIdx;
  Int_t   D0Idx;
  Int_t   PiIdx;
  Int_t   SlowPiIdx;
  Int_t   KIdx;
  Int_t   PitrkIdx;
  Int_t   KtrkIdx;
  Int_t   D0Pi2Idx;
  Int_t   D0Pi3Idx;
  Int_t   D0Pi2trkIdx;
  Int_t   D0Pi3trkIdx;
  Int_t   SlowPitrkIdx;
  Int_t   Pi1Idx;
  Int_t   Pi1trkIdx;

  ///////Variables to be saved into reduced ntuple
  Int_t eventnumber;//event
  Float_t dstpimass;
  Float_t dstpideltam;
  Float_t dstpipstar;
  Float_t dstpimcpstar;
  Float_t dstpicosstar;
  Float_t dstpimccosstar;
  Float_t dstpilogvtxprob;
  Float_t dstpidmres;
  Int_t   dstpicharge;
  Int_t   dstpimctrue;
  Int_t   dstpinDstPi;
  Int_t   dstpiIdx;
  Float_t dstpigenmass;

  Float_t dstarmass;//D*
  Float_t dstardeltam;
  Float_t dstarpstar;
  Float_t dstarlogvtxprob; 
  Float_t dstarcostheta;//angle of D* in DstPi rest-frame
  Int_t   dstarcharge;
  Int_t   dstarmctrue;

  Float_t d0mass;//D0
  Float_t d0pstar; 
  Float_t d0logvtxprob;
  Int_t   d0mctrue; 
  Float_t d0costheta;// angle of D0 in D* rest-frame
  Int_t   d0charge;
  Int_t   d0mode;

  Float_t kp3;   
  Int_t   kcharge;
  Int_t   kmctrue;
  Float_t kcostheta;//angle of K+ in D0 rest-frame

  Float_t pip3;  
  Int_t   picharge; 
  Int_t   pimctrue; 
 
  Float_t d0pi2p3;  
  Int_t   d0pi2charge; 
  Int_t   d0pi2mctrue;//other D0 pions
  
  Float_t d0pi3p3;  
  Int_t   d0pi3charge; 
  Int_t   d0pi3mctrue;

  Float_t slowpip3; 
  Int_t   slowpicharge;
  Int_t   slowpimctrue;//slow pion
  
  Float_t pi1p3;
  Int_t   pi1charge;
  Int_t   pi1mctrue;
  
  /////need the database
  JDataBaseDstPi database;

  //MC produced
  TH1F* HMCDstPiMass;
  TH1F* HMCDstPiDeltaM;
  TH1F* HMCDstPip3CM;
  TH1F* HMCDstPicosthCM;
  TH2F* H2MCDstPiCMPvsTheta;
  TH1F* HMCNDstPi;
  TH2F* H2MCDstPiDalitzPlot;


  ////for fitting 
  TString phasefactor;
  TString qfactor;


  //editing 
  //a cut drawing line
  TLine cutline;
  TText text;
  char textlabel[100];



};
  

#endif

