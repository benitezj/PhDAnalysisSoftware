#ifndef Dst0Analysis_hh
#define Dst0Analysis_hh

#include "../D0/D0Analysis.h"

#define MYDSTLUND 413

class Dst0Analysis:public D0Analysis{
  ClassDef(Dst0Analysis,1);

 public:
  Dst0Analysis(Bool_t MCorDATA,TString OutputDir);
  ~Dst0Analysis();


  //
  virtual Int_t ReduceNtuple();
 

/*   Int_t MakeDst0Mass();  */
/*   Int_t PlotDst0Mass();  */
   

 private:

  void FillDst0Vars();  
  Int_t SetDst0Branches();
  Int_t MakeDst0ReducedNtupleBranches(TTree* Ntuple);
  Int_t SetDst0ReducedNtupleBranches();

  
  Int_t   nDst0;
  Float_t Dst0Mass[MAXNCANDS];  
  Float_t Dst0MassErr[MAXNCANDS];  
  Float_t Dst0p3CM[MAXNCANDS];  
  Float_t Dst0costhCM[MAXNCANDS];  
  Float_t Dst0phiCM[MAXNCANDS];  
  Float_t Dst0p3[MAXNCANDS];  
  Float_t Dst0costh[MAXNCANDS];  
  Float_t Dst0phi[MAXNCANDS];  
  Int_t   Dst0Lund[MAXNCANDS];  
  Int_t   Dst0d1Lund[MAXNCANDS];  
  Int_t   Dst0d1Idx[MAXNCANDS];  
  Int_t   Dst0d2Lund[MAXNCANDS];  
  Int_t   Dst0d2Idx[MAXNCANDS];   
  Int_t   Dst0d3Lund[MAXNCANDS];  
  Int_t   Dst0d3Idx[MAXNCANDS];
  Int_t   Dst0MCIdx[MAXNCANDS];
  Float_t Dst0Chi2[MAXNCANDS];
  Int_t   Dst0nDof[MAXNCANDS];
  Float_t Dst0Vtxx[MAXNCANDS];
  Float_t Dst0Vtxy[MAXNCANDS];
  Float_t Dst0Vtxz[MAXNCANDS];
  Int_t   Dst0VtxStatus[MAXNCANDS];
  Float_t Dst0FlightBSLen[MAXNCANDS];


  //------------------------------------
  //////My derived variables 
  //----------------------------------- 
  
  ////indexes
  Int_t   Dst0Idx;
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

