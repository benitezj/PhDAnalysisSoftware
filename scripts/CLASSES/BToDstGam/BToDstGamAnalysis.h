#ifndef BToDstGamAnalysis_hh
#define BToDstGamAnalysis_hh

#include "../Dstar/DstAnalysis.h"

class BToDstGamAnalysis:public DstAnalysis{
  ClassDef(BToDstGamAnalysis,1);

 public:
  BToDstGamAnalysis(Bool_t MCorDATA,Bool_t TruthMatch,Int_t MatterOrAntiMatter,TString OutputDir);
  ~BToDstGamAnalysis();
  
  Int_t ReduceNtuple(Bool_t RunInTestMode);
  void SetReduceCuts(Int_t _cutdmass,Int_t _cutdstdeltam,Int_t _cutthrust,Int_t _cutmes){
    cutdmass=_cutdmass;cutdstdeltam=_cutdstdeltam;cutthrust=_cutthrust;cutmes=_cutmes;
   }


  //Mes 
  Int_t FitMES();


 private:
  
  void FillBToDstGamVars(); 
  Int_t SetBToDstGamBranches();
  Int_t MakeBToDstGamReducedNtupleBranches(TTree* Ntuple);
  Int_t SetBToDstGamReducedNtupleBranches();
  
  Int_t cutdmass;
  Int_t cutdstdeltam;
  Int_t cutthrust;
  Int_t cutmes;



  
  // Declaration of leave types
  Int_t   nB;
  Float_t BChi2[MAXNCANDS];  //Must be a float!
  Int_t   BnDof[MAXNCANDS];  
  Int_t   BVtxStatus[MAXNCANDS];
  Float_t BMass[MAXNCANDS];
  Float_t Bp3[MAXNCANDS];    
  Float_t Bcosth[MAXNCANDS];
  Float_t Bphi[MAXNCANDS];   
  Float_t Bp3CM[MAXNCANDS];  
  Float_t BcosthCM[MAXNCANDS];  
  Float_t BphiCM[MAXNCANDS];  
  Int_t   BLund[MAXNCANDS];  
  Int_t   BMCIdx[MAXNCANDS];  
  Int_t   Bd1Lund[MAXNCANDS];  
  Int_t   Bd1Idx[MAXNCANDS];  
  Int_t   Bd2Lund[MAXNCANDS];  
  Int_t   Bd2Idx[MAXNCANDS];  
  Float_t BVtxx[MAXNCANDS];  
  Float_t BVtxy[MAXNCANDS];  
  Float_t BVtxz[MAXNCANDS];     
  Float_t BmES[MAXNCANDS];    
  Float_t BMyMCIdx[MAXNCANDS];    
  Float_t BDeltaE[MAXNCANDS];    
  Float_t Bthrust[MAXNCANDS];
  Float_t BFlightBSCosA[MAXNCANDS]; 
  Float_t BFlightBSLen[MAXNCANDS]; 
  Float_t BFlightBSErr[MAXNCANDS]; 

  ////indexes
  Int_t   BIdx;
  Int_t   GamIdx;

  ///////Variables to be saved into reduced ntuple
  Float_t bmass;
  Float_t bmes;
  Float_t benergy;
  Float_t bdeltae;
  Float_t bpstar;
  Float_t bphistar;
  Float_t bcosstar;
  Float_t blogvtxprob;
  Int_t   bcharge;
  Float_t bmesres;
  Int_t   bbestcand;
  Int_t   bmctrue;
  Int_t   bvtxstatus;
  Float_t bflights;
  Float_t bthrust;
};
  

#endif

