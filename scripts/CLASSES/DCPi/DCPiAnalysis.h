#ifndef DCPiAnalysis_hh
#define DCPiAnalysis_hh

#include "../DC/DCAnalysis.h"
#include "../MassFitter.h"
#include "../RooResolutionPdf.h"
#include "../Roo2BodyMassPhaseSpace.h"
#include <TF2.h>

#define EffMin 2.0
#define EffMax 3.1
#define EffNbins 44
#define EffPMin 2.5
#define EffPMax 5.0
#define EffPNbins 25
#define CutPMin 2.8

#define XMassMin 2.0
#define XMassMax 3.4
#define XMassBins 280
//#define XMassLabel "m(D^{+}_{K^{-}#pi^{+}#pi^{+}}#pi^{-})-m(K^{-}#pi^{+}#pi^{+})+M_{D^{+}}   (GeV/c^{2})"
#define XMassLabel "M(D^{+}#pi^{-})  (GeV/c^{2})"

#define NFITVars 20
#define NSysts 10
#define NSig 5

class DCPiAnalysis:public DCAnalysis{
  ClassDef(DCPiAnalysis,1);

 public:
  DCPiAnalysis(Int_t MCorDATA,TString OutputDir);
  ~DCPiAnalysis();
  
  Int_t ReduceNtuple();
  void SetReduceCuts(Int_t _cuttruth,Int_t _cutcharge,Int_t _cutpstar,Int_t _cutdflight,Int_t _cutdmass,Int_t _cutcone,Int_t _cutdcos){
    cuttruth=_cuttruth;cutcharge=_cutcharge;cutpstar=_cutpstar;cutdflight=_cutdflight;cutdmass=_cutdmass;cutcone=_cutcone;cutdcos=_cutdcos;
  }


  Int_t DefaultPlots();


  //
  Int_t MakeHistosForFit();
  //Int_t FitMass(TString name);
  Int_t FitMass();
  Int_t FitD3000();
  Int_t FitVariations(TString id);
  Int_t ShowSignificances();
  Int_t ShowSystematics();

  Int_t FitMassVsPstar();

  ///Analysis of Signal MC
  Int_t MakeResolution();
  Int_t FitResolution();
  Int_t FitResolutionTotal();
  
  //mass dependent effiency
  Int_t MakeEfficiency();
  Int_t PlotEfficiency();
//   Int_t PlotPstarCutEfficiency();
//   Int_t PlotDetectorEfficiency();
  
  Float_t GetEfficiency(Float_t mass){return effoffset+mass*effslope;}
  Float_t GetEfficiencyEr(Float_t mass){return .001+.015*fabs(mass-2.460)/.6;}

  //
  void MakeFeedResolution();


  //
  void PlotRandFits(TString Dir,TString signame,TString parname,Int_t Nfiles);
  

  //access components
  MassFitter* GetMassFitter(){return &massfitter;}

//   //
//   TH1* GetMassHisto(TString hname){
//     TFile file(_OutputDir+"/HistosForFit.root","read");
//     gROOT->cd();
//     return (TH1*)(file.Get(hname)->Clone());    
//   }



 private:
  
  void FillDCPiVars(); 
  Int_t SetDCPiBranches();
  Int_t MakeDCPiReducedNtupleBranches(TTree* Ntuple);
  Int_t SetDCPiReducedNtupleBranches();

  Float_t ComputePiAngle();
  Float_t ComputeKAngle();
  Float_t ComputeDAngle();
  Float_t ComputeMCDAngle();
  Float_t ComputePiMotherAngle();
  Float_t ComputeDSFDistance();

//   void DetermineBestCand();
//   Int_t bestcandid;


  //final cuts
  Int_t cuttruth;
  Int_t cutcharge;
  Int_t cutpstar;
  Int_t cutdflight;
  Int_t cutdmass;
  Int_t cutcone;
  Int_t cutdcos;

  
  //
  Float_t PstarMin;
  Float_t DCosMin;
   

  TH2F* HMCMassVsPstar;
  TH1F* HMCMass;
  TH1F* HMCDCos;
  TH1F* HMCCosstar;
  

  RooRealVar dau1m;
  RooRealVar dau2m;
  Roo2BodyMassPhaseSpace TwoBodyPS;


  //Resolution
  RooRealVar resmean;
  RooRealVar resoffset;
  RooRealVar resslope;
  RooRealVar resnormratio2;
  RooRealVar resmean2;
  RooRealVar ressigmaratio2;
  RooRealVar resnormratio3;
  RooRealVar resmean3;
  RooRealVar ressigmaratio3;
  RooRealVar resVar;
  RooAbsPdf* respdf;

//   //Efficiency=offset+slope*xmdm
  Float_t effoffset;
  Float_t effslope;
  TH1F* effHisto;

//   TFile* EffFile;
//   Float_t effpstarstep,effpstarmin;
//   Float_t effmassstep,effmassmin;
//   Int_t  OpenEfficiency();
//   Float_t GetEfficiency();
//   Float_t GetEfficiencyErr();
//   void CloseEfficiency();
//   TH2F* H2Efficiency;
//   TH2F* H2EfficiencyErr;
//   TF2* FPCutEff;

//   //
//   TF1* FDCos;


  //
  MassFitter massfitter;

  //
  Float_t scalefactorccbar;


  // Declaration of leave types
  Int_t   nX;
  Float_t XChi2[MAXNCANDS];  //Must be a float!
  Int_t   XnDof[MAXNCANDS];  
  Int_t   XVtxStatus[MAXNCANDS];
  Float_t XMass[MAXNCANDS];
  Float_t Xp3[MAXNCANDS];    
  Float_t Xcosth[MAXNCANDS];
  Float_t Xphi[MAXNCANDS];   
  Float_t Xp3CM[MAXNCANDS];  
  Float_t XcosthCM[MAXNCANDS];  
  Float_t XphiCM[MAXNCANDS];  
  Int_t   XLund[MAXNCANDS];  
  Int_t   XMCIdx[MAXNCANDS];  
  Int_t   XMyMCIdx[MAXNCANDS];
  Int_t   Xd1Lund[MAXNCANDS];  
  Int_t   Xd1Idx[MAXNCANDS];  
  Int_t   Xd2Lund[MAXNCANDS];  
  Int_t   Xd2Idx[MAXNCANDS];  
  Float_t XVtxx[MAXNCANDS];  
  Float_t XVtxy[MAXNCANDS];  
  Float_t XVtxz[MAXNCANDS];     

  Float_t DFlightLen[MAXNCANDS];
  Float_t XDmass[MAXNCANDS];
  Float_t XKpimass[MAXNCANDS];
  Float_t XKpi2mass[MAXNCANDS];
  Float_t XDflight[MAXNCANDS];
  Float_t XDflighte[MAXNCANDS];
  Float_t Xexpartpx[MAXNCANDS];
  Float_t Xexpartpy[MAXNCANDS];
  Float_t Xexpartpz[MAXNCANDS];
  Float_t Xexparte[MAXNCANDS];
  Int_t Xexpartn[MAXNCANDS];

  ////indexes
  Int_t   XIdx;
  Int_t   Pi1Idx;
  Int_t   Pi1trkIdx;

  ///////Variables to be saved into reduced ntuple
  Float_t xmass;
  Float_t xdeltam;
  Float_t xmdm;
  Float_t xenergy;
  Float_t xenergyfrac;
  Float_t xpstar;
  Float_t xpstarfrac;
  Float_t xphistar;
  Float_t xcosstar;
  Float_t xlogvtxprob;
  Int_t   xcharge;
  Float_t xdmres;
  Int_t   xbestcand;
  Int_t   xmctrue;
  Int_t   xlund;
  Int_t   xvtxstatus;
  Float_t xlogL;
  Float_t xlogLM;
  Float_t xlogLF;
  Float_t xlogLD;
  Float_t xlogLP;
  Float_t xlogLC;


  Float_t dcostheta;
  Float_t kcosine;

  Float_t pi1p3;
  Int_t   pi1charge;
  Float_t pi1dedx;
  Float_t pi1thetac;
  Int_t   pi1lh;
  Float_t pi1costheta;
  Float_t pi1mothcosine;
  Float_t pi1Kmass;
  Float_t pi1Kpimass;
  Float_t pi1Kpi2mass;  
  Float_t pi1pimass;
  Float_t pi1pipi2mass;
  Float_t pi1pi2mass;


  Int_t evtmultiplicity;//# of tracks in signal side
  

  Float_t recoile;
  Float_t recoilp;
  Float_t recoilm;
  Float_t recoildmass;
  Float_t recoilpimass;
  Float_t recoilcosine;
  Float_t recoilpicosine;



  ////
  TString SystDirs[NSysts]; 
  TString SystName[NSysts];
  TString SigName[NSig];
  TString SigTitle[NSig];


};
  

#endif

