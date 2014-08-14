#ifndef DsRecoAnalysis2_hh
#define DsRecoAnalysis2_hh

#include "../Tag/TagAnalysis.h"

#define myDsLund 431
#define myDsStLund 433
#define myNuLund 14

class DsRecoAnalysis2:public TagAnalysis{
  ClassDef(DsRecoAnalysis2,1);

 public:
  DsRecoAnalysis2(Bool_t MCorDATA,Bool_t TruthMatch,Int_t MatterOrAntiMatter,TString OutputDir);
  ~DsRecoAnalysis2();
  
  Int_t ReduceNtuple(Bool_t RunInTestMode);
  void SetReduceCuts(Int_t _cuttruth,Int_t _cutcharge,Int_t _cuttagmass,Int_t _cutgamangle,Int_t _cutKangle){
    cuttruth=_cuttruth;cutcharge=_cutcharge;cuttagmass=_cuttagmass;cutgamangle=_cutgamangle;cutKangle=_cutKangle;
  }
  
  
  Int_t ReduceNtupleGen(Bool_t RunInTestMode);

  //void FitDsStDeltaM();
  
  //
  void PlotGenTagSide();



 private:
  
  void  FillDsRecoVars(); 
  Int_t SetDsRecoBranches();
  Int_t MakeDsRecoReducedNtupleBranches(TTree* Ntuple);
  Int_t SetDsRecoReducedNtupleBranches();

  Float_t ComputeGamAngle();
  Float_t ComputeKAngle();
  Float_t ComputeFragAngle();


  Int_t cuttruth;
  Int_t cutcharge;
  Int_t cutgamangle;
  Int_t cutKangle;
  Int_t cuttagmass;

  
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

  Int_t   nB;
  Float_t BMass[MAXNCANDS];    
  Float_t Bp3[MAXNCANDS];    
  Float_t Bcosth[MAXNCANDS];
  Float_t Bphi[MAXNCANDS];   
  Float_t Benergy[MAXNCANDS]; 
  Float_t Bp3CM[MAXNCANDS];  
  Float_t BcosthCM[MAXNCANDS];  
  Float_t BphiCM[MAXNCANDS];  
  Float_t BenergyCM[MAXNCANDS]; 
  Int_t   BLund[MAXNCANDS]; 
  Int_t   Bd1Lund[MAXNCANDS];  
  Int_t   Bd1Idx[MAXNCANDS];  
  Int_t   Bd2Lund[MAXNCANDS];  
  Int_t   Bd2Idx[MAXNCANDS];  
  Int_t   Bd3Lund[MAXNCANDS];  
  Int_t   Bd3Idx[MAXNCANDS];  

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
  Int_t   BIdx;
  Int_t   GamIdx;
  Int_t   TagKIdx; 
  Int_t   FragIdx;

  //MC indexes
  Int_t   DsGenMCIdx;
  Int_t   DsStGenMCIdx;

  ///////Variables to be saved into reduced ntuple
  Float_t evtmass;
  Float_t evtenergy;
  Float_t evtcmenergy;
  Float_t evtpstar;
  Float_t evtphistar;
  Float_t evtcosstar;
  Int_t   evtcharge;
  Int_t   evtbestcand;
  Int_t   evtmctrue;

  Float_t bmass;
  Float_t benergy;
  Float_t benergycm;
  Float_t bpstar;
  Float_t bp;
  Float_t bcosthcm;
  Float_t bdeltae;
  
  Int_t   fragcharge;
  Float_t fragpstar;
  Int_t   fragndaus;
  Float_t fragangle;
  Float_t fragenergy; 
  Int_t   fragmctrue;
  Int_t   fragnpis;
  Int_t   fragnpi0s;
  Int_t   fragngams;
  Int_t   fragnKs;
  Int_t   fragnKss;
  Int_t   fragnEs;
  Int_t   fragnMus;
  Int_t   fragnENus;
  Int_t   fragnMuNus;
  Float_t fragenergyK; 
  Float_t fragenergyKs; 
  Float_t fragenergypi;
  Float_t fragenergypi0;
  Float_t fragenergygam;   
  Float_t fragenergyE; 
  Float_t fragenergyMu;
  Float_t fragenergyENu;
  Float_t fragenergyMuNu;   

  Float_t mismass;
  Float_t misp;

  Float_t dsmass;
  Float_t dsp;

  Float_t dsstmass;
  Float_t dsstdeltam;
  Float_t dsstp;

  Float_t gamenergy;
  Float_t gamangle;

  Float_t Kangle;
  Int_t   Kmctrue;
};
  

#endif

