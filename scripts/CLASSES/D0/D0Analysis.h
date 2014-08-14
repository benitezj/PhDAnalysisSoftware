#ifndef D0Analysis_hh
#define D0Analysis_hh

#include "../AbsAnalysis.h"

#define MAXNCANDS 100


class D0Analysis:public AbsAnalysis{
  ClassDef(D0Analysis,1);

 public:
  D0Analysis(Bool_t MCorDATA,TString OutputDir);
  ~D0Analysis();


  //
  virtual Int_t ReduceNtuple();
 
  void ShowDMassRegions(TH1* Histo);
  Int_t MakeDMass();
  Int_t PlotDMass();
  Int_t FitDMass();
  

 private:

  void FillD0Vars();  
  Int_t SetD0Branches();
  Int_t MakeD0ReducedNtupleBranches(TTree* Ntuple);
  Int_t SetD0ReducedNtupleBranches();

  
  Float_t DMassCutCenter;
  Float_t DMassCutWindow;
  Float_t DMassCutSBMin;

  RooRealVar mass;
  MassFitter massfitter;
  
  TString DNtupleTag;

  Int_t   nD;
  Float_t DMass[MAXNCANDS];  
  Float_t DMassErr[MAXNCANDS];  
  Float_t Dp3CM[MAXNCANDS];  
  Float_t DcosthCM[MAXNCANDS];  
  Float_t DphiCM[MAXNCANDS];  
  Float_t Dp3[MAXNCANDS];  
  Float_t Dcosth[MAXNCANDS];  
  Float_t Dphi[MAXNCANDS];  
  Int_t   DLund[MAXNCANDS];  
  Int_t   Dd1Lund[MAXNCANDS];  
  Int_t   Dd1Idx[MAXNCANDS];  
  Int_t   Dd2Lund[MAXNCANDS];  
  Int_t   Dd2Idx[MAXNCANDS];   
  Int_t   Dd3Lund[MAXNCANDS];  
  Int_t   Dd3Idx[MAXNCANDS];
  Int_t   Dd4Lund[MAXNCANDS];  
  Int_t   Dd4Idx[MAXNCANDS];
  Int_t   DMCIdx[MAXNCANDS];
  Float_t DChi2[MAXNCANDS];
  Int_t   DnDof[MAXNCANDS];
  Float_t DVtxx[MAXNCANDS];
  Float_t DVtxy[MAXNCANDS];
  Float_t DVtxz[MAXNCANDS];
  Int_t   DVtxStatus[MAXNCANDS];
  Float_t DFlightBSLen[MAXNCANDS];
  
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

  Int_t   nK;
  Float_t Kp3[MAXNCANDS]; 
  Float_t Kcosth[MAXNCANDS]; 
  Float_t Kphi[MAXNCANDS]; 
  Float_t Kp3CM[MAXNCANDS]; 
  Float_t KcosthCM[MAXNCANDS]; 
  Float_t KphiCM[MAXNCANDS]; 
  Int_t   KLund[MAXNCANDS]; 
  Int_t   KMCIdx[MAXNCANDS];
  Int_t   KTrkIdx[MAXNCANDS];

  Int_t   PiSelectorsMap[MAXNCANDS];
  Int_t   KSelectorsMap[MAXNCANDS];
  Int_t   TRKnSvt[MAXNCANDS];
  Int_t   TRKLund[MAXNCANDS];
  Float_t TRKdedxdch[MAXNCANDS];
  Float_t TRKDrcTh[MAXNCANDS];

 
  //--------------------------------------
  //////My derived variables 
  //-------------------------------------  
  
  ////indexes
  Int_t   DIdx;
  Int_t   PiIdx;
  Int_t   KIdx;
  Int_t   PitrkIdx;
  Int_t   DPi2Idx;
  Int_t   DPi3Idx;
  
 
  Float_t dmass;//D
  Float_t dpstar; 
  Float_t dp3; 
  Int_t   dcharge; 
  Float_t dlogvtxprob;
  Float_t dflightbs;
  Int_t   dmode;
  Int_t   dmctrue;
  
  Float_t kp3;   
  Int_t   kcharge;
  Int_t   kmctrue;
  Float_t kcostheta;//angle of K+ in D rest-frame
  Int_t   klh;

  Float_t pip3;  
  Int_t   picharge; 
  Int_t   pimctrue; 
  Int_t   pilh;

  Float_t dpi2p3;  
  Int_t   dpi2charge; 
  Int_t   dpi2mctrue;//other D pions
  Int_t   dpi2lh;

  Float_t Kpimass;
  Float_t Kpi2mass;
  Float_t pipi2mass;

  Float_t KpimassFold;
  Float_t Kpi2massFold;

};
  

#endif

