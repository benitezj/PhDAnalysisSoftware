#ifndef DsInclusiveAnalysis_hh
#define DsInclusiveAnalysis_hh

#include "../Tag/TagAnalysis.h"

#define myDsLund 431
#define myDsStLund 433
#define myNuLund 14
#define NFragDaus 20
#define NDsMassBins 50
#define DsMassMin 1.82
#define DsMassMax 2.12
#define NXR 6
#define NXT 7
#define DsPDGMass 1.9693

class DsInclusiveAnalysis:public TagAnalysis{
  ClassDef(DsInclusiveAnalysis,1);

 public:
  DsInclusiveAnalysis(Bool_t MCorDATA,Bool_t TruthMatch,Int_t MatterOrAntiMatter,TString OutputDir);
  ~DsInclusiveAnalysis();
  
  Int_t ReduceNtuple();
  void SetReduceCuts(Int_t _cutcharge,
		     Int_t _cuttagmass, 
		     Int_t _cuttagpstar,
		     Int_t _cutdsstmass,
		     Int_t _cutevtvtxprob,
		     Int_t _cutdspstar,
		     Int_t _cutfragngams,
		     Int_t _cutfragexgame){
    cutcharge=_cutcharge;
    cuttagmass=_cuttagmass;
    cuttagpstar=_cuttagpstar;
    cutdsstmass=_cutdsstmass;
    cutevtvtxprob=_cutevtvtxprob;
    cutdspstar=_cutdspstar;
    cutfragngams=_cutfragngams;
    cutfragexgame=_cutfragexgame;
  }
  

  Int_t ReduceNtupleGen(Long_t MaxNumberEvents);
  
  void FitDsMass(); 
  void CreateSigPdfs();
  void PlotSigPdfs();

  void CreateBkgPdfs();
  void PlotBkgPdfs();

  void RooFitDsMass();
  void RooFitDsMassSimple();


 private:
  
  void  FillDsInclusiveVars(); 
  Int_t SetDsInclusiveBranches();
  Int_t MakeDsInclusiveReducedNtupleBranches(TTree* Ntuple);
  Int_t SetDsInclusiveReducedNtupleBranches();

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
  Float_t Evtp3CM[MAXNCANDS];  
  Float_t EvtcosthCM[MAXNCANDS];  
  Float_t EvtphiCM[MAXNCANDS];
  Float_t EvtenergyCM[MAXNCANDS];  
  Int_t   EvtLund[MAXNCANDS];  
  Int_t   EvtMyGamMCIdx[MAXNCANDS];  
  Int_t   EvtMyKMCIdx[MAXNCANDS];  
  Int_t   EvtMyTagMCIdx[MAXNCANDS];  
  Int_t   EvtMyXMCIdx[MAXNCANDS];  
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
  Float_t EvtDsStUncMass[MAXNCANDS];
  Float_t EvtDsUncMass[MAXNCANDS];
  Float_t EvtTagUncMass[MAXNCANDS];
  Float_t Evtexgampx[MAXNCANDS];  
  Float_t Evtexgampy[MAXNCANDS];  
  Float_t Evtexgampz[MAXNCANDS]; 
  Float_t Evtexgame[MAXNCANDS]; 
  Int_t Evtexgamn[MAXNCANDS];
  Int_t EvtqualD[MAXNCANDS]; 
  Int_t EvtqualK[MAXNCANDS]; 
  Int_t EvtqualX[MAXNCANDS]; 
  Float_t EvtDsStPxRaw[MAXNCANDS];
  Float_t EvtDsStPyRaw[MAXNCANDS];
  Float_t EvtDsStPzRaw[MAXNCANDS];
  Float_t EvtDsStERaw[MAXNCANDS];
  Float_t EvtDsStPxCorr[MAXNCANDS];
  Float_t EvtDsStPyCorr[MAXNCANDS];
  Float_t EvtDsStPzCorr[MAXNCANDS];
  Float_t EvtDsStECorr[MAXNCANDS];
  Float_t EvtDsMassUnFit[MAXNCANDS];


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

  Int_t   nDs;
  Float_t DsMass[MAXNCANDS];
  Float_t Dsp3CM[MAXNCANDS];  
  Float_t DscosthCM[MAXNCANDS];  
  Float_t DsphiCM[MAXNCANDS];
  Float_t DsenergyCM[MAXNCANDS];  
  Int_t   DsLund[MAXNCANDS];  
  Int_t   DsStd1Lund[MAXNCANDS];  
  Int_t   DsStd1Idx[MAXNCANDS];  
  Int_t   DsStd2Lund[MAXNCANDS];  
  Int_t   DsStd2Idx[MAXNCANDS];  


  Int_t   nFrag;
  Int_t   FragnDaus[MAXNCANDS];  
  Float_t FragChi2[MAXNCANDS];  //Must be a float!
  Int_t   FragnDof[MAXNCANDS];  
  Int_t   FragVtxStatus[MAXNCANDS];
  Float_t FragMass[MAXNCANDS];
  Float_t Fragp3CM[MAXNCANDS];  
  Float_t FragcosthCM[MAXNCANDS];  
  Float_t FragphiCM[MAXNCANDS]; 
  Float_t FragenergyCM[MAXNCANDS];   
  Int_t   FragLund[MAXNCANDS];  
  Int_t   FragMCIdx[MAXNCANDS];  
  Int_t   FragdauLund[NFragDaus][MAXNCANDS];  
  Int_t   FragdauIdx[NFragDaus][MAXNCANDS];  
  

  Int_t   nGam;
  Float_t Gamp3CM[MAXNCANDS];  
  Float_t GamcosthCM[MAXNCANDS];  
  Float_t GamphiCM[MAXNCANDS]; 
  Float_t GamenergyCM[MAXNCANDS];  
  Int_t   GamLund[MAXNCANDS];  


  ////indexes
  Int_t   EvtIdx;
  Int_t   DsStIdx;
  Int_t   DsIdx;
  Int_t   GamIdx;
  Int_t   TagKIdx; 
  Int_t   FragIdx;
  
  //MC indexes
  Int_t  DsGenMCIdx;
  Int_t  DsStGenMCIdx;
  Int_t  GamGenMCIdx;
  Int_t  KGenMCIdx;

  ///////Variables to be saved into reduced ntuple
  Float_t evtmass;
  Float_t evtenergy;
  Float_t evtpx;
  Float_t evtpy;
  Float_t evtpz;
  Float_t evtcmenergy;
  Float_t evtpstar;
  Float_t evtphistar;
  Float_t evtcosstar;
  Int_t   evtcharge;
  Int_t   evtbestcand;
  Int_t   evtmctrue;
  Float_t evtvtxprob;
  Float_t evtvtxx;
  Float_t evtvtxy;
  Float_t evtvtxz;
  Float_t evtfit1vtxprob;
  
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
  Int_t   fragqual;
  Int_t   fragnpartgen;
  Int_t   fragnpigen;
  Int_t   fragnpi0gen;
  Int_t   fragngamgen;

  Float_t dsmass;
  Float_t dsenergycm;
  Float_t dsp;
  Float_t dspstar;
  Int_t   dscharge; 
  Float_t dsangle;
  Float_t dspxdiff;
  Float_t dspydiff;
  Float_t dspzdiff;
  Float_t dspdiff;
  Float_t dsuncmass;
  Float_t dsunfitmass;
 

  Float_t dsstmass;
  Float_t dsstuncmass;
  Float_t dsstdeltam;
  Float_t dsstp;
  Float_t dsstpstar;
  Float_t dsstenergycm;
  Float_t dsstangle;
  Float_t dsstdecayangle;
  Float_t dsstcorrpdiff;
  Float_t dsstfitpdiff;
  

  Float_t gamenergy;
  Float_t gamenergycm;
  Float_t gamangle;
  Float_t gamdsstangle;
  Int_t   gammctrue;
    
  Float_t Kangle;
  Float_t Kecm;
  Int_t   Kmctrue;
  Int_t   Klund;
  Int_t   Kqual;
};
  

#endif

