#ifndef D0PiAnalysis_hh
#define D0PiAnalysis_hh

#include "../D0/D0Analysis.h"
#include "../MassFitter.h"
#include "../RooResolutionPdf.h"
#include "../Roo2BodyMassPhaseSpace.h"
#include <TF2.h>

#define EffMin 2.0
#define EffMax 3.1
#define EffNbins 44

#define XMassMin 2.0
#define XMassMax 3.4
#define XMassBins 280
//#define XMassLabel "m(D^{+}_{K^{-}#pi^{+}#pi^{+}}#pi^{-})-m(K^{-}#pi^{+}#pi^{+})+M_{D^{+}}   (GeV/c^{2})"
#define XMassLabel "M(D^{0}#pi^{+})  (GeV/c^{2})"

#define NFITVars 20
#define NSysts 15
#define NSig 5


class D0PiAnalysis:public D0Analysis{
  ClassDef(D0PiAnalysis,1);

 public:
  D0PiAnalysis(Bool_t MCorDATA,TString OutputDir);
  ~D0PiAnalysis();
  
  Int_t ReduceNtuple();
  void SetReduceCuts(Int_t _cutcharge,Int_t _cutpstar,Int_t _cutdmass,Int_t _cutdcos){
    cutcharge=_cutcharge;cutpstar=_cutpstar;cutdmass=_cutdmass;cutdcos=_cutdcos;
  }


  //
  Int_t DefaultPlots();

  //
  Int_t MakeHistosForFit();

  //
  Int_t PlotD0PiMass();

  //resolution
  Int_t MakeResolution();
  Int_t FitResolution();
  Int_t FitResolutionTotal();
  
  //mass dependent effiency
  Int_t MakeEfficiency();
  Int_t PlotEfficiency();
  
  //
  //Int_t CreateHistosForFit();
  //Int_t FitMass();
  Int_t FitMassAntimo();
  

  Int_t FitVariations(TString id);
  Int_t ShowSignificances();
  Int_t ShowSystematics();



 private:
  
  void FillD0PiVars(); 
  Int_t SetD0PiBranches();
  Int_t MakeD0PiReducedNtupleBranches(TTree* Ntuple);
  Int_t SetD0PiReducedNtupleBranches();

  Float_t ComputeDAngle();
  Float_t ComputePiAngle();
  Float_t ComputeMCDAngle();
  Float_t ComputePiMotherAngle();

  //final cuts
  Int_t cutcharge;
  Int_t cutpstar;
  Int_t cutdmass;
  Int_t cutdcos;

 
  //
  MassFitter massfitter;

  //
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

  //efficiency
  RooRealVar effoffset;
  RooRealVar effslope;
  RooAbsPdf* effpdf;


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
  Int_t   XDstParent[MAXNCANDS];
           
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
  Int_t   xmcidx;
  Int_t   xlund;
  Int_t   xvtxstatus;
  Int_t   xmultiplicityid;


  Float_t dcostheta;
  Int_t   dparent;// 0=D**, 1=D*+,2=D*0,
  Float_t kcosine;

  Float_t pi1p3;
  Int_t   pi1charge;
  Float_t pi1dedx;
  Float_t pi1thetac;
  Float_t pi1costheta;
  Float_t pi1mothcosine;
  Float_t pi1Kmass;
  Float_t pi1pimass;


  ////
  TString SystDirs[NSysts]; 
  TString SystName[NSysts];
  TString SigName[NSig];
  TString SigTitle[NSig];
};
  

#endif

