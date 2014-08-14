#ifndef DstAnalysis_hh
#define DstAnalysis_hh

#include "../D0/D0Analysis.h"

#define MYDSTLUND 413
#define DstMeanDeltaM .14542

class DstAnalysis:public D0Analysis{
  ClassDef(DstAnalysis,1);

 public:
  DstAnalysis(Bool_t MCorDATA,TString OutputDir);
  ~DstAnalysis();


  //
  virtual Int_t ReduceNtuple();
 

  Int_t MakeDstMass(); 
  Int_t PlotDstMass(); 
   

 private:

  void FillDstVars();  
  Int_t SetDstBranches();
  Int_t MakeDstReducedNtupleBranches(TTree* Ntuple);
  Int_t SetDstReducedNtupleBranches();

  
  Int_t   nDst;
  Float_t DstMass[MAXNCANDS];  
  Float_t DstMassErr[MAXNCANDS];  
  Float_t Dstp3CM[MAXNCANDS];  
  Float_t DstcosthCM[MAXNCANDS];  
  Float_t DstphiCM[MAXNCANDS];  
  Float_t Dstp3[MAXNCANDS];  
  Float_t Dstcosth[MAXNCANDS];  
  Float_t Dstphi[MAXNCANDS];  
  Int_t   DstLund[MAXNCANDS];  
  Int_t   Dstd1Lund[MAXNCANDS];  
  Int_t   Dstd1Idx[MAXNCANDS];  
  Int_t   Dstd2Lund[MAXNCANDS];  
  Int_t   Dstd2Idx[MAXNCANDS];   
  Int_t   Dstd3Lund[MAXNCANDS];  
  Int_t   Dstd3Idx[MAXNCANDS];
  Int_t   DstMCIdx[MAXNCANDS];
  Float_t DstChi2[MAXNCANDS];
  Int_t   DstnDof[MAXNCANDS];
  Float_t DstVtxx[MAXNCANDS];
  Float_t DstVtxy[MAXNCANDS];
  Float_t DstVtxz[MAXNCANDS];
  Int_t   DstVtxStatus[MAXNCANDS];
  Float_t DstFlightBSLen[MAXNCANDS];


  //------------------------------------
  //////My derived variables 
  //----------------------------------- 
  
  ////indexes
  Int_t   DstIdx;
  Int_t   PiSlowIdx;

  Float_t dstmass;
  Float_t dstdeltam;
  Float_t dstpstar; 
  Float_t dstp3; 
  Int_t   dstcharge; 
  Float_t dstlogvtxprob;

  Float_t pislowp3;  
  Int_t   pislowcharge; 

};
  

#endif

