#ifndef BToDstEtaAnalysis_hh
#define BToDstEtaAnalysis_hh

#include "../Dstar/DstAnalysis.h"

class BToDstEtaAnalysis:public DstAnalysis{
  ClassDef(BToDstEtaAnalysis,1);

 public:
  BToDstEtaAnalysis(Bool_t MCorDATA,Bool_t TruthMatch,Int_t MatterOrAntiMatter,TString OutputDir);
  ~BToDstEtaAnalysis();
  
  Int_t ReduceNtuple(Bool_t RunInTestMode);
  void SetReduceCuts(Int_t _cuttruth,Int_t _cutcharge,Int_t _cutdmass,Int_t _cutdstdeltam,Int_t _cutthrust,Int_t _cutmes,Int_t _cutdeltae){
    cuttruth=_cuttruth;cutcharge=_cutcharge;cutdmass=_cutdmass;cutdstdeltam=_cutdstdeltam;cutthrust=_cutthrust;cutmes=_cutmes;cutdeltae=_cutdeltae;
   }


  //Mes 
  Int_t FitMES();


  Int_t MakeEtaMass();
  Int_t PlotEtaMass();


 private:
  
  void FillBToDstEtaVars(); 
  Int_t SetBToDstEtaBranches();
  Int_t MakeBToDstEtaReducedNtupleBranches(TTree* Ntuple);
  Int_t SetBToDstEtaReducedNtupleBranches();
  
  Int_t cuttruth;
  Int_t cutcharge;
  Int_t cutdmass;
  Int_t cutdstdeltam;
  Int_t cutthrust;
  Int_t cutmes;
  Int_t cutdeltae;



  //angles
  Float_t ComputeEtaAngle();//angle of the extra Eta wrt the X direction in the X frame
  Float_t ComputeBDstJointAngle();//Angle of D* pion w.r.t Eta direction in D* frame
  
 

  
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
  Int_t   BIdx;
  Int_t   EtaIdx;
  Int_t   Gam1Idx;
  Int_t   Gam2Idx;

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
  Float_t bdecayangle;
  Float_t bdstjointangle;
  Int_t   bselectindex;

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

