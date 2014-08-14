#ifndef EtaAnalysis_hh
#define EtaAnalysis_hh

#include "../AbsAnalysis.h"

#define MAXNCANDS 100


class EtaAnalysis:public AbsAnalysis{
  ClassDef(EtaAnalysis,1);

 public:
  EtaAnalysis(Bool_t MCorDATA,Bool_t TruthMatch,Int_t MatterOrAntiMatter,TString OutputDir);
  ~EtaAnalysis();


  //
  virtual Int_t ReduceNtuple(Bool_t RunInTestMode);
 

  Int_t MakeMass();
  Int_t PlotMass();
  

 private:

  void FillEtaVars();  
  Int_t SetEtaBranches();
  Int_t MakeEtaReducedNtupleBranches(TTree* Ntuple);
  Int_t SetEtaReducedNtupleBranches();

 
  Int_t   nEta;
  Float_t EtaMass[MAXNCANDS];  
  Float_t EtaMassErr[MAXNCANDS];  
  Float_t Etap3CM[MAXNCANDS];  
  Float_t EtacosthCM[MAXNCANDS];  
  Float_t EtaphiCM[MAXNCANDS];  
  Float_t Etap3[MAXNCANDS];  
  Float_t Etacosth[MAXNCANDS];  
  Float_t Etaphi[MAXNCANDS];  
  Int_t   EtaLund[MAXNCANDS];  
  Int_t   Etad1Lund[MAXNCANDS];  
  Int_t   Etad1Idx[MAXNCANDS];  
  Int_t   Etad2Lund[MAXNCANDS];  
  Int_t   Etad2Idx[MAXNCANDS];   
  Int_t   EtaMCIdx[MAXNCANDS];
  Float_t EtaChi2[MAXNCANDS];
  Int_t   EtanDof[MAXNCANDS];
  Float_t EtaVtxx[MAXNCANDS];
  Float_t EtaVtxy[MAXNCANDS];
  Float_t EtaVtxz[MAXNCANDS];
  Int_t   EtaVtxStatus[MAXNCANDS];
  
  Int_t   nGam;
  Float_t Gamp3[MAXNCANDS];  
  Float_t Gamcosth[MAXNCANDS]; 
  Float_t Gamphi[MAXNCANDS]; 
  Float_t Gamp3CM[MAXNCANDS]; 
  Float_t GamcosthCM[MAXNCANDS]; 
  Float_t GamphiCM[MAXNCANDS]; 
  Int_t   GamLund[MAXNCANDS];   
  Int_t   GamMCIdx[MAXNCANDS];


  //--------------------------------------
  //////My derived variables 
  //-------------------------------------  
  
  ////indexes
  Int_t   EtaIdx;
  Int_t   Gam1Idx;
  Int_t   Gam2Idx;
 
  Float_t etamass;//ETA
  Float_t etapstar; 
  Float_t etap3; 
  Float_t etalogvtxprob;
  
  Float_t gam1p3;  
  Int_t   gam1mctrue; 

  Float_t gam2p3;  
  Int_t   gam2mctrue; 
  

};
  

#endif

