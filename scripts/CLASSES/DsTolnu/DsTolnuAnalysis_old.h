#ifndef DsTolnuAnalysis_hh
#define DsTolnuAnalysis_hh

#include "../Tag/TagAnalysis.h"

class DsTolnuAnalysis:public TagAnalysis{
  ClassDef(DsTolnuAnalysis,1);

 public:
  DsTolnuAnalysis(Bool_t MCorDATA,Bool_t TruthMatch,Int_t MatterOrAntiMatter,TString OutputDir);
  ~DsTolnuAnalysis();
  
  Int_t ReduceNtuple(Bool_t RunInTestMode);
  void SetReduceCuts(Int_t _cuttruth,Int_t _cutcharge,Int_t _cutlepangle,Int_t _cutgamangle,Int_t _cutKangle){
    cuttruth=_cuttruth;cutcharge=_cutcharge;cutlepangle=_cutlepangle;cutgamangle=_cutgamangle;cutKangle=_cutKangle;
  }


 private:
  
  void FillDsTolnuVars(); 
  Int_t SetDsTolnuBranches();
  Int_t MakeDsTolnuReducedNtupleBranches(TTree* Ntuple);
  Int_t SetDsTolnuReducedNtupleBranches();

  Float_t ComputeEvtMisMass();
  Float_t ComputeLepAngle();
  Float_t ComputeGamAngle();
  Float_t ComputeKAngle();
  Float_t ComputeDsRecoilMass();
  Float_t ComputeDsStRecoilMass();

  Int_t cuttruth;
  Int_t cutcharge;
  Int_t cutlepangle;
  Int_t cutgamangle;
  Int_t cutKangle;


  
  // Declaration of leave types
  Int_t   nEvt;
  Int_t   EvtnDaus[MAXNCANDS]; 
  Float_t EvtChi2[MAXNCANDS];  //Must be a float!
  Int_t   EvtnDof[MAXNCANDS];  
  Int_t   EvtVtxStatus[MAXNCANDS];
  Float_t EvtMass[MAXNCANDS];
  Float_t Evtp3[MAXNCANDS];    
  Float_t Evtcosth[MAXNCANDS];
  Float_t Evtphi[MAXNCANDS];   
  Float_t Evtenergy[MAXNCANDS];  
  Float_t Evtp3CM[MAXNCANDS];  
  Float_t EvtcosthCM[MAXNCANDS];  
  Float_t EvtphiCM[MAXNCANDS];
  Float_t EvtenergyCM[MAXNCANDS];  
  Int_t   EvtLund[MAXNCANDS];  
  Int_t   EvtMyDsMCIdx[MAXNCANDS];  
  Int_t   EvtMyKMCIdx[MAXNCANDS];  
  Int_t   EvtMyTagMCIdx[MAXNCANDS];  
  Int_t   EvtMyFragMCIdx[MAXNCANDS];  
  Int_t   Evtd1Lund[MAXNCANDS];  
  Int_t   Evtd1Idx[MAXNCANDS];  
  Int_t   Evtd2Lund[MAXNCANDS];  
  Int_t   Evtd2Idx[MAXNCANDS];  
  Int_t   Evtd3Lund[MAXNCANDS];  
  Int_t   Evtd3Idx[MAXNCANDS];  
  Int_t   Evtd4Lund[MAXNCANDS];  
  Int_t   Evtd4Idx[MAXNCANDS];  
  Int_t   Evtd5Lund[MAXNCANDS];  
  Int_t   Evtd5Idx[MAXNCANDS];  
  Float_t EvtVtxx[MAXNCANDS];  
  Float_t EvtVtxy[MAXNCANDS];  
  Float_t EvtVtxz[MAXNCANDS];     

  Int_t   nFrag;
  Int_t   FragnDaus[MAXNCANDS];  
  Float_t FragChi2[MAXNCANDS];  //Must be a float!
  Int_t   FragnDof[MAXNCANDS];  
  Int_t   FragVtxStatus[MAXNCANDS];
  Float_t FragMass[MAXNCANDS];
  Float_t Fragp3[MAXNCANDS];    
  Float_t Fragcosth[MAXNCANDS];
  Float_t Fragphi[MAXNCANDS];   
  Float_t Fragenergy[MAXNCANDS];   
  Float_t Fragp3CM[MAXNCANDS];  
  Float_t FragcosthCM[MAXNCANDS];  
  Float_t FragphiCM[MAXNCANDS]; 
  Float_t FragenergyCM[MAXNCANDS];   
  Int_t   FragLund[MAXNCANDS];  
  Int_t   FragMCIdx[MAXNCANDS];  
  Int_t   Fragd1Lund[MAXNCANDS];  
  Int_t   Fragd1Idx[MAXNCANDS];  
  Int_t   Fragd2Lund[MAXNCANDS];  
  Int_t   Fragd2Idx[MAXNCANDS];  
  Int_t   Fragd3Lund[MAXNCANDS];  
  Int_t   Fragd3Idx[MAXNCANDS];  
  Int_t   Fragd4Lund[MAXNCANDS];  
  Int_t   Fragd4Idx[MAXNCANDS];  
  Int_t   Fragd5Lund[MAXNCANDS];  
  Int_t   Fragd5Idx[MAXNCANDS];  
  Int_t   Fragd6Lund[MAXNCANDS];  
  Int_t   Fragd6Idx[MAXNCANDS];  
  Int_t   Fragd7Lund[MAXNCANDS];  
  Int_t   Fragd7Idx[MAXNCANDS];  
  Int_t   Fragd8Lund[MAXNCANDS];  
  Int_t   Fragd8Idx[MAXNCANDS];  
  Int_t   Fragd9Lund[MAXNCANDS];  
  Int_t   Fragd9Idx[MAXNCANDS];  
  Float_t FragVtxx[MAXNCANDS];  
  Float_t FragVtxy[MAXNCANDS];  
  Float_t FragVtxz[MAXNCANDS];     

  Int_t   nMu;
  Float_t Mup3[MAXNCANDS];    
  Float_t Mucosth[MAXNCANDS];
  Float_t Muphi[MAXNCANDS];   
  Float_t Muenergy[MAXNCANDS]; 
  Float_t Mup3CM[MAXNCANDS];  
  Float_t MucosthCM[MAXNCANDS];  
  Float_t MuphiCM[MAXNCANDS];  
  Float_t MuenergyCM[MAXNCANDS]; 
  Int_t   MuLund[MAXNCANDS];  

  Int_t   nGam;
  Float_t Gamp3[MAXNCANDS];    
  Float_t Gamcosth[MAXNCANDS];
  Float_t Gamphi[MAXNCANDS];   
  Float_t Gamenergy[MAXNCANDS]; 
  Float_t Gamp3CM[MAXNCANDS];  
  Float_t GamcosthCM[MAXNCANDS];  
  Float_t GamphiCM[MAXNCANDS]; 
  Float_t GamenergyCM[MAXNCANDS];  
  Int_t   GamLund[MAXNCANDS];  

  ////indexes
  Int_t   EvtIdx;
  Int_t   LepIdx;
  Int_t   GamIdx;
  Int_t   TagKIdx; 
  Int_t   FragIdx;

  ///////Variables to be saved into reduced ntuple
  Float_t evtmass;
  Float_t evtmism;
  Float_t evtenergy;
  Float_t evtcmenergy;
  Float_t evtpstar;
  Float_t evtphistar;
  Float_t evtcosstar;
  Int_t   evtcharge;
  Int_t   evtbestcand;
  Int_t   evtmctrue;
  Float_t evtdsmass;
  Float_t evtdsstmass;
  Float_t evtdsstdeltam;

  Int_t   fragcharge;
  Float_t fragpstar;
  Float_t fragndaus;
  Float_t fragangle;
  Float_t fragenergy; 
  Int_t   fragmctrue;

  Float_t lepangle;
  Float_t lepenergy;

  Float_t gamenergy;
  Float_t gamangle;

  Float_t Kangle;
  Int_t   Kmctrue;
};
  

#endif

