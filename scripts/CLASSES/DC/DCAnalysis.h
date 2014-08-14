#ifndef DCAnalysis_hh
#define DCAnalysis_hh

#include "../AbsAnalysis.h"
//#include "DataBaseDC.h"


#define MAXNCANDS 100


class DCAnalysis:public AbsAnalysis{
  ClassDef(DCAnalysis,1);

 public:
  DCAnalysis(Int_t MCorDATA,TString OutputDir);
  ~DCAnalysis();


  //
  virtual Int_t ReduceNtuple();
 
  Int_t MakeDMass();
  Int_t PlotDMass();

  Int_t MakeDFlight();
  Int_t PlotDFlight();
  
  //
  Int_t MakeDalitzPlot();
  Int_t PlotDalitzPlot();

  //
  void ShowDMassRegions(TH1* Histo);

 protected:

  ////////cuts 
  Float_t LowMassHist;
  Float_t HighMassHist;
  Int_t MassHistBins;

  Float_t MeanMass;
  Float_t LowMassCut;
  Float_t HighMassCut;
  Float_t LowSideLowMassCut;
  Float_t LowSideHighMassCut;
  Float_t HighSideLowMassCut;
  Float_t HighSideHighMassCut;

  Float_t DFlightCut;

 private:

  void FillDCVars();  
  Int_t SetDCBranches();
  Int_t MakeDCReducedNtupleBranches(TTree* Ntuple);
  Int_t SetDCReducedNtupleBranches();

  Float_t ComputeKCosTheta();
  
  //DataBaseDC databaseDC;
  
  RooRealVar mass; 
  

  Int_t   nD;
  Float_t DMass[MAXNCANDS];  
  Float_t DMassErr[MAXNCANDS];  
  Float_t Dp3CM[MAXNCANDS];  
  Float_t DcosthCM[MAXNCANDS];  
  Float_t DphiCM[MAXNCANDS];  
  Float_t Dp3[MAXNCANDS];  
  Float_t Dcosth[MAXNCANDS];  
  Float_t Dphi[MAXNCANDS];  
  Int_t   DLund[MAXNCANDS];  
  Int_t   Dd1Lund[MAXNCANDS];  
  Int_t   Dd1Idx[MAXNCANDS];  
  Int_t   Dd2Lund[MAXNCANDS];  
  Int_t   Dd2Idx[MAXNCANDS];   
  Int_t   Dd3Lund[MAXNCANDS];  
  Int_t   Dd3Idx[MAXNCANDS];
  Int_t   DMCIdx[MAXNCANDS];
  Float_t DChi2[MAXNCANDS];
  Int_t   DnDof[MAXNCANDS];
  Float_t DVtxx[MAXNCANDS];
  Float_t DVtxy[MAXNCANDS];
  Float_t DVtxz[MAXNCANDS];
  Int_t   DVtxStatus[MAXNCANDS];
  Float_t DFlightBSLen[MAXNCANDS];
  Float_t DFlightBSErr[MAXNCANDS];
  
  Int_t   nPi;
  Float_t Pip3[MAXNCANDS];  
  Float_t Picosth[MAXNCANDS]; 
  Float_t Piphi[MAXNCANDS]; 
  Float_t Pip3CM[MAXNCANDS]; 
  Float_t PicosthCM[MAXNCANDS]; 
  Float_t PiphiCM[MAXNCANDS]; 
  Int_t   PiLund[MAXNCANDS];   
  Int_t   PiMCIdx[MAXNCANDS];
/*   Int_t   PiTrkIdx[MAXNCANDS]; */

  Int_t   nK;
  Float_t Kp3[MAXNCANDS]; 
  Float_t Kp3CM[MAXNCANDS]; 
  Float_t KcosthCM[MAXNCANDS]; 
  Float_t KphiCM[MAXNCANDS]; 
  Int_t   KLund[MAXNCANDS]; 
  Int_t   KMCIdx[MAXNCANDS];
/*   Int_t   KTrkIdx[MAXNCANDS]; */

/*   Int_t   PiSelectorsMap[MAXNCANDS];   */
/*   Int_t   KSelectorsMap[MAXNCANDS]; */
/*   Int_t   TRKnSvt[MAXNCANDS];   */
/*   Int_t   TRKLund[MAXNCANDS]; */
/*   Float_t TRKdedxdch[MAXNCANDS]; */
/*   Float_t TRKDrcTh[MAXNCANDS]; */

 
  //--------------------------------------
  //////My derived variables 
  //-------------------------------------  
  
  ////indexes
  Int_t   DIdx;
  Int_t   PiIdx;
  Int_t   KIdx;
  Int_t   PitrkIdx;
  Int_t   KtrkIdx;
  Int_t   DPi2Idx;
  Int_t   DPi2trkIdx;
 
  Float_t dmass;//D
  Float_t dpstar; 
  Float_t dp3; 
  Int_t   dcharge; 
  Float_t dlogvtxprob;
  Float_t dflight;
  Float_t dflights;
  Int_t   dmctrue;
  
  Float_t kp3;   
  Int_t   kcharge;
  Int_t   kmctrue;
  Float_t kcostheta;//angle of K+ in D rest-frame
  Int_t   klh;

  Float_t pip3;  
  Int_t   picharge; 
  Int_t   pimctrue; 
  Int_t   pilh;

  Float_t dpi2p3;  
  Int_t   dpi2charge; 
  Int_t   dpi2mctrue;//other D pions
  Int_t   dpi2lh;

  Float_t Kpimass;
  Float_t Kpi2mass;
  Float_t pipi2mass;

  Float_t KpimassFold;
  Float_t Kpi2massFold;

};
  

#endif

