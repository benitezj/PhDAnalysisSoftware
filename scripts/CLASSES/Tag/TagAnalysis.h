#ifndef TagAnalysis_hh
#define TagAnalysis_hh

#include "../AbsAnalysis.h"

#define MAXNCANDS 100


class TagAnalysis:public AbsAnalysis{
  ClassDef(TagAnalysis,1);

 public:
  TagAnalysis(Bool_t MCorDATA,Bool_t TruthMatch,Int_t MatterOrAntiMatter,TString OutputDir);
  ~TagAnalysis();


  //
  virtual Int_t ReduceNtuple();
 
  void SetReduceCuts(Int_t _cutcharge,
		     Int_t _cuttagmass, 
		     Int_t _cuttagpstar,
		     Int_t _cuttagvtxprob,
		     Int_t _cuttagmode){
    cutcharge=_cutcharge;
    cuttagmass=_cuttagmass;
    cuttagpstar=_cuttagpstar;
    cuttagvtxprob=_cuttagvtxprob;
    cuttagmode=_cuttagmode;
  }
  

  //Int_t MakeTagMass();
  //Int_t PlotTagMass();
  
  Int_t OptimizeCuts();
  

 private:

  void FillTagVars();  
  Int_t SetTagBranches();
  Int_t MakeTagReducedNtupleBranches(TTree* Ntuple);
  Int_t SetTagReducedNtupleBranches();
  Bool_t TagModeIsGen(Int_t tagLund,Int_t tagMode);


  Int_t cutcharge;
  Int_t cuttagmass;
  Int_t cuttagpstar;
  Int_t cuttagvtxprob;
  Int_t cuttagmode;

  RooRealVar mass;
  MassFitter massfitter;
  

  Int_t   nTag;
  Float_t TagMass[MAXNCANDS];  
  Float_t TagMassErr[MAXNCANDS];  
  Float_t Tagp3CM[MAXNCANDS];  
  Float_t TagcosthCM[MAXNCANDS];  
  Float_t TagphiCM[MAXNCANDS];  
  Float_t TagenergyCM[MAXNCANDS];  
  Float_t TagChi2[MAXNCANDS];
  Int_t   TagnDof[MAXNCANDS];  
  Int_t   TagVtxStatus[MAXNCANDS];
  Float_t TagVtxx[MAXNCANDS];  
  Float_t TagVtxy[MAXNCANDS];  
  Float_t TagVtxz[MAXNCANDS];  
  Int_t   TagLund[MAXNCANDS];  
  Int_t   TagnDaus[MAXNCANDS];
  Int_t   Tagd1Lund[MAXNCANDS];  
  Int_t   Tagd1Idx[MAXNCANDS];  
  Int_t   Tagd2Lund[MAXNCANDS];  
  Int_t   Tagd2Idx[MAXNCANDS];   
  Int_t   Tagd3Lund[MAXNCANDS];  
  Int_t   Tagd3Idx[MAXNCANDS];
  Int_t   Tagd4Lund[MAXNCANDS];  
  Int_t   Tagd4Idx[MAXNCANDS];
  Int_t   Tagd5Lund[MAXNCANDS];  
  Int_t   Tagd5Idx[MAXNCANDS];
  Float_t TagFlightBSLen[MAXNCANDS];
  Float_t TagFlightBSErr[MAXNCANDS];
  Int_t   TagMCIdx[MAXNCANDS];
  
  Int_t   nPi;
  Float_t PienergyCM[MAXNCANDS]; 
  Float_t Pip3CM[MAXNCANDS]; 
  Float_t PicosthCM[MAXNCANDS]; 
  Float_t PiphiCM[MAXNCANDS]; 
  Int_t   PiLund[MAXNCANDS];   
  Int_t   PiMCIdx[MAXNCANDS];
  Int_t   PiTrkIdx[MAXNCANDS]; 

  Int_t   nK;
  Float_t Kp3CM[MAXNCANDS]; 
  Float_t KcosthCM[MAXNCANDS]; 
  Float_t KphiCM[MAXNCANDS]; 
  Float_t KenergyCM[MAXNCANDS]; 
  Int_t   KLund[MAXNCANDS]; 
  Int_t   KMCIdx[MAXNCANDS];
  Int_t   KTrkIdx[MAXNCANDS];

  Int_t   nPi0;
  Float_t Pi0Mass[MAXNCANDS];  
  Float_t Pi0energyCM[MAXNCANDS]; 
  Float_t Pi0p3CM[MAXNCANDS]; 
  Float_t Pi0costhCM[MAXNCANDS]; 
  Float_t Pi0phiCM[MAXNCANDS]; 
  Int_t   Pi0Lund[MAXNCANDS];   
  Int_t   Pi0MCIdx[MAXNCANDS];

  Int_t   nKs;
  Float_t KsMass[MAXNCANDS];  
  Float_t Ksp3CM[MAXNCANDS]; 
  Float_t KsenergyCM[MAXNCANDS]; 
  Float_t KscosthCM[MAXNCANDS]; 
  Float_t KsphiCM[MAXNCANDS]; 
  Int_t   KsLund[MAXNCANDS];   
  Int_t   KsMCIdx[MAXNCANDS];
  Float_t KsFlightBSLen[MAXNCANDS];//fligth from moth vertex not filled in
  Float_t KsFlightBSErr[MAXNCANDS];

  Int_t   PiSelectorsMap[MAXNCANDS];
  Int_t   KSelectorsMap[MAXNCANDS];
  Int_t   TRKnSvt[MAXNCANDS];
  Int_t   TRKLund[MAXNCANDS];
  Float_t TRKdedxdch[MAXNCANDS];
  Float_t TRKDrcTh[MAXNCANDS];

 
  //--------------------------------------
  //////My derived variables 
  //-------------------------------------  
  
  ////indexes
  Int_t   TagIdx;
  Int_t   d1Idx;
  Int_t   d2Idx;
  Int_t   d3Idx;
  Int_t   d4Idx;
  Int_t   d5Idx;
 
  //
  Int_t   GenTagIdx;

  Float_t tagmass;
  Float_t tagmassdiff;
  Float_t tagpstar; 
  Float_t tagenergy; 
  Int_t   tagcharge; 
  Float_t tagflights;
  Int_t   tagmode;
  Int_t   taglund;
  Int_t   tagndaus;
  Float_t taglogvtxprob;
  Int_t   tagmctrue;
  Int_t   tagmctruebad;
  Int_t   tagmodewasgen;
  Float_t TagPDGMass;
  Float_t TagMassCut;

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
  Int_t   tagqual;
  Int_t   tagpid;

  Float_t tagksflights;
   
};
  

#endif

