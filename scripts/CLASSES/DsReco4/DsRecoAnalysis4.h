#ifndef DsRecoAnalysis4_hh
#define DsRecoAnalysis4_hh

#include "../Tag/TagAnalysis.h"

#define myDsLund 431
#define myDsStLund 433
#define myNuLund 14
#define NFragDaus 20

class DsRecoAnalysis4:public TagAnalysis{
  ClassDef(DsRecoAnalysis4,1);

 public:
  DsRecoAnalysis4(Bool_t MCorDATA,Bool_t TruthMatch,Int_t MatterOrAntiMatter,TString OutputDir);
  ~DsRecoAnalysis4();
  
  Int_t ReduceNtuple(Bool_t RunInTestMode);
  void SetReduceCuts(Int_t _cuttruth,
		     Int_t _cutcharge,
		     Int_t _cuttagmass, 
		     Int_t _cuttagpstar,
		     Int_t _cutdsstmass,
		     Int_t _cutevtvtxprob,
		     Int_t _cutdspstar,
		     Int_t _cutfragngams,
		     Int_t _cutfragexgame){
    cuttruth=_cuttruth;
    cutcharge=_cutcharge;
    cuttagmass=_cuttagmass;
    cuttagpstar=_cuttagpstar;
    cutdsstmass=_cutdsstmass;
    cutevtvtxprob=_cutevtvtxprob;
    cutdspstar=_cutdspstar;
    cutfragngams=_cutfragngams;
    cutfragexgame=_cutfragexgame;
  }
  
  
  void FitDsMass(); 



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
  Int_t cuttagmass;
  Int_t cuttagpstar;
  Int_t cutdsstmass;
  Int_t cutevtvtxprob;
  Int_t cutdspstar;
  Int_t cutfragngams;
  Int_t cutfragexgame;

  
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
  Float_t Evtgampx[MAXNCANDS];
  Float_t Evtgampy[MAXNCANDS];
  Float_t Evtgampz[MAXNCANDS];
  Float_t EvtDsStUncMass[MAXNCANDS];
  Float_t Evtexgampx[MAXNCANDS];  
  Float_t Evtexgampy[MAXNCANDS];  
  Float_t Evtexgampz[MAXNCANDS]; 
  Int_t Evtexgamn[MAXNCANDS];  

  Int_t   nDsSt;
  Float_t DsStMass[MAXNCANDS];
  Float_t DsStp3[MAXNCANDS];    
  Float_t DsStcosth[MAXNCANDS];
  Float_t DsStphi[MAXNCANDS];   
  Float_t DsStenergy[MAXNCANDS];  
  Float_t DsStp3CM[MAXNCANDS];  
  Float_t DsStcosthCM[MAXNCANDS];  
  Float_t DsStphiCM[MAXNCANDS];
  Float_t DsStenergyCM[MAXNCANDS];  
  Int_t   DsStLund[MAXNCANDS];  

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
  Int_t   FragdauLund[NFragDaus][MAXNCANDS];  
  Int_t   FragdauIdx[NFragDaus][MAXNCANDS];  
  

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
  Int_t   DsStIdx;
  Int_t   TagKIdx; 
  Int_t   FragIdx;
  
  //MC indexes
  Int_t  DsGenMCIdx;
  Int_t  DsStGenMCIdx;

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
  Float_t evtvtxprob;

  Float_t tagsidemass;
  Float_t tagsideenergy;
  Float_t tagsideenergycm;
  Float_t tagsidepstar;
  Float_t tagsidep;
  Float_t tagsidecosthcm;
  Float_t tagsidedeltae;
  Float_t tagsideangle;
  Int_t   tagnpi;
  Int_t   tagnpi0;
  Int_t   tagngam;
  Int_t   tagmcnpi;
  Int_t   tagmcnpi0;
  Int_t   tagmcngam;

  Float_t sigsidemass;
  Float_t sigsideenergy;
  Float_t sigsideenergycm;
  Float_t sigsidepstar;
  Float_t sigsidep;
  Float_t sigsidecosthcm;
  Float_t sigsidedeltae;
  Float_t sigsideangle;
  Int_t   signpi;
  Int_t   signpi0;
  Int_t   signgam;
  Int_t   sigmcnpi;
  Int_t   sigmcnpi0;
  Int_t   sigmcngam;

   
  Int_t   fragcharge;
  Float_t fragpstar;
  Int_t   fragndaus;
  Float_t fragangle;
  Float_t fragtagangle;
  Float_t fragsigangle;
  Float_t fragenergy; 
  Int_t   fragmctrue;
  Int_t   fragnpis;
  Int_t   fragnpi0s;
  Float_t fragpienergy;
  Int_t   fragngams;
  Float_t fraggamenergy;
  Float_t fragexgame;
  Int_t   fragexgamn;

  Float_t dsmass;
  Float_t dsenergycm;
  Float_t dsp;
  Float_t dspstar;
  Float_t dsangle;

  Float_t dsstmass;
  Float_t dsstuncmass;
  Float_t dsstdeltam;
  Float_t dsstp;
  Float_t dsstpstar;
  Float_t dsstenergycm;
  Float_t dsstangle;

  Float_t gamenergy;
  Float_t gamangle;
  Float_t gamdsstangle;

  Float_t Kangle;
  Float_t Ke;
  Float_t Kecm;
  Int_t   Kmctrue;
  Int_t   Klund;
};
  

#endif

