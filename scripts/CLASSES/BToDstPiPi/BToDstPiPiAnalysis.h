#ifndef BToDstPiPiAnalysis_hh
#define BToDstPiPiAnalysis_hh

#include "../Dstar/DstAnalysis.h"

class BToDstPiPiAnalysis:public DstAnalysis{
  ClassDef(BToDstPiPiAnalysis,1);

 public:
  BToDstPiPiAnalysis(Bool_t MCorDATA,Bool_t TruthMatch,Int_t MatterOrAntiMatter,TString OutputDir);
  ~BToDstPiPiAnalysis();
  
  Int_t ReduceNtuple(Bool_t RunInTestMode);
  void SetReduceCuts(Int_t _cuttruth,Int_t _cutcharge,Int_t _cutdmass,Int_t _cutdstdeltam,Int_t _cutthrust,Int_t _cutmes,Int_t _cutdeltae){
    cuttruth=_cuttruth;cutcharge=_cutcharge;cutdmass=_cutdmass;cutdstdeltam=_cutdstdeltam;cutthrust=_cutthrust;cutmes=_cutmes;cutdeltae=_cutdeltae;
   }


  //Mes 
  Int_t FitMES();

  Int_t PlotDalits();

  Int_t PlotSquareDalits();


 private:
  
  void FillBToDstPiPiVars(); 
  Int_t SetBToDstPiPiBranches();
  Int_t MakeBToDstPiPiReducedNtupleBranches(TTree* Ntuple);
  Int_t SetBToDstPiPiReducedNtupleBranches();

  TF1* FRandomizePions;

  Float_t ComputeDststDecayAngle(Int_t BPiIdx,Int_t DPiIdx);
  Float_t ComputeDstHelicityAngle(Int_t DPiIdx);
  
  Int_t cuttruth;
  Int_t cutcharge;
  Int_t cutdmass;
  Int_t cutdstdeltam;
  Int_t cutthrust;
  Int_t cutmes;
  Int_t cutdeltae;


  
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
  Int_t   Bd3Lund[MAXNCANDS];  
  Int_t   Bd3Idx[MAXNCANDS];  
  Float_t BVtxx[MAXNCANDS];  
  Float_t BVtxy[MAXNCANDS];  
  Float_t BVtxz[MAXNCANDS];     
  Float_t BmES[MAXNCANDS];    
  Float_t BMyMCIdx[MAXNCANDS];    
  Float_t BDeltaE[MAXNCANDS];    
  Float_t Bsph[MAXNCANDS]; 
  Float_t BROEthrust[MAXNCANDS];
  Float_t Bthrust[MAXNCANDS];
  Float_t BR2[MAXNCANDS];
  Float_t BMsq12[MAXNCANDS]; 
  Float_t BMsq31[MAXNCANDS]; 
  Float_t BFlightBSCosA[MAXNCANDS]; 
  Float_t BFlightBSLen[MAXNCANDS]; 
  Float_t BFlightBSErr[MAXNCANDS]; 

  ////indexes
  Int_t   BIdx;
  Int_t   Pi1Idx;
  Int_t   Pi2Idx;


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
  Float_t bdstpi1mass;
  Float_t bdstpi2mass;
  Float_t bdstpi1massfolded;
  Float_t bdstpi2massfolded;
  Float_t bflights;
  Float_t bthrust;
  Float_t eventsph;
  Float_t eventr2;
  Float_t eventthrust;
  Float_t bdstpi1decayangle;
  Float_t bdstpi2decayangle;
  Float_t bdstpi1decayanglefolded;
  Float_t bdstpi2decayanglefolded;



};
  

#endif

