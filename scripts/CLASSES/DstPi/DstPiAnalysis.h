#ifndef DstPiAnalysis_hh
#define DstPiAnalysis_hh
#include "../../Globals.h"
#include "../../AnalysisGlobals.h"
#include "../../FUNCTIONS/SetHisto.h"
#include "../../FUNCTIONS/Fits.h"
#include "../../FUNCTIONS/MoveStatsBox.h"
#include "../../FUNCTIONS/RebinHisto.h"

#include "../MassFitter.h"
#include "../ParameterRead.h"

#include "../JResiduals.h"

//#include "JDataBaseDstPi.h"



//
#include <TVector3.h>
#include <TLorentzRotation.h>
#include <TLorentzVector.h>
#include <TLine.h>
#include <TGraphErrors.h>
#include <TCut.h>
#include <TBox.h>
#include <TH3F.h>
#include <TStyle.h>
#include <TNamed.h>

//////needed for RooFit
#include <TLine.h>
#include <TGraphErrors.h>
#include <TLatex.h>
#include "RooRealVar.h"
#include "RooCategory.h"
#include "RooPlot.h"
#include "RooDataSet.h"
#include "RooDataHist.h"
#include "RooAddPdf.h"
#include "RooProdPdf.h"
#include "RooFitResult.h"
#include "RooBreitWigner.h"
#include "../RooGenericPdfN.h"
#include "../RooProdPdfN.h"
#include "../RooAddPdfN.h"
#include "RooGenericPdf.h"
#include "RooArgSet.h"
//#include "RooCmdArg.h"
#include "RooArgList.h"
#include "RooDataHist.h"
#include "RooAbsReal.h"
#include "RooNumConvPdf.h"
#include "RooAbsData.h"
#include "RooGlobalFunc.h"
#include "RooChebychev.h"
#include "../Roo2BodyMassPhaseSpace.h"
using namespace RooFit ;
////////////////////////


#define MAXX 60
#define MAXNCANDS 300
#define MAXMCLEN 200
//#define MYDSTPILUND 10423 //10423=D2420, 425=D2460, 523 for B*+ 
#define MYDSTARLUND 413


#define YAxisLabel "Events / 5 MeV/c^{2}"
//#define XAxisLabel "m(D^{_{*+}}#pi^{-}) - m(D^{_{*+}}) + 2.01    (GeV/c^{2})"
#define XAxisLabel "M(D^{_{*+}}#pi^{-})   (GeV/c^{2})"


#define HistDMassMin 1.80
#define HistDMassMax 1.93
#define HistDMassBins 130
#define HistDstDMMin .140
#define HistDstDMMax .152
#define HistDstDMBins 120

#define D0MassMean D0PDGMass
#define D0MassCut .03
#define DstarDMMean .1454
#define DstarDMCut .002
#define PstarCut 3.0
#define CosCut -0.8


#define NSysts 15
#define NSysts2 20
#define NSystsFinal 20
#define NSystsFinalHeli 20
#define NSig 5

class DstPiAnalysis:public TNamed{
  ClassDef(DstPiAnalysis,1);

 public:
  DstPiAnalysis(Int_t MCorDATA,TString OutputDir);
  ~DstPiAnalysis();
  

  void SetChain(TChain* ntuple){_fChain=ntuple;}
  //reduction of BtaTupleMaker Ntuples
  void SetReduceCuts(Int_t _cuttruth,Int_t _cutcharge,Int_t _cutdmass,Int_t _cutd0mode,
		     Int_t _cutdeltam,Int_t _cutpstar,Int_t _cutdcos){
    cuttruth=_cuttruth;cutcharge=_cutcharge;cutdmass=_cutdmass;cutd0mode=_cutd0mode;
    cutdeltam=_cutdeltam;cutpstar=_cutpstar;cutdcos=_cutdcos;
  }
  Int_t ReduceNtuple();
  
  void DefaultPlots();

  Int_t DrawEfficiencyVsMass();
  Int_t DrawResolutionVsMass();  

  
  Int_t CreateHistosForFit();

  //Simple fitting method which can fit any histogram in HistorForFit.root
  Int_t FitMassHeliCut();
  Int_t FitMassHeliCut2();
  Int_t FitMass();
  Int_t FitMassVsPstar();

  //systematics 
  Int_t SystematicFitsHeliCut(TString id="All");
  Int_t ShowSystematicsHeliCut();
  Int_t SystematicFitsHeliCut2(TString id="All");
  Int_t ShowSystematicsHeliCut2();
  Int_t SystematicFitsFinal(TString id="All");
  Int_t ShowSystematicsFinal();
  Int_t ShowSignificances(Int_t fittype);

  Int_t SystematicFitsVsHelicity(Int_t bin=5, TString id="All");

  //helicity shapes
  Int_t PlotHelicityEfficiency();
  Int_t FitMassVsHelicity();
  Int_t PlotMassVsHelicity();
  Int_t FitMassVsHelicityNewSig();
  Int_t PlotMassVsHelicityNewSig();
  Int_t FitMassVsHelicityNewSig2(Bool_t fit);
  Int_t PlotMassVsHelicityNewSig2();
  


  //Branching Ratios
  Int_t ComputeBRatio(TString signal);
  Float_t GetEfficiencyKpi(Float_t mass){return effoffsetKpi+mass*effslopeKpi;}
  Float_t GetEfficiencyKpiEr(Float_t mass){return .001+.007*fabs(mass-2.440)/0.6;}
  Float_t GetEfficiencyK3pi(Float_t mass){return effoffsetK3pi+mass*effslopeK3pi;}
  Float_t GetEfficiencyK3piEr(Float_t mass){return .001+.007*fabs(mass-2.440)/0.6;}
  

  //BFRatio vs Pstar
  void PlotBFRatioVsPstar();


  TString roofitfileid;
  TString roofitconfigfile;
 

 private:
  
  Int_t SetBranches();
  Int_t MakeReducedNtupleBranches(TTree* Ntuple);

  Int_t SetReducedNtupleBranches();
  //Int_t SetModeReducedNtupleBranches();

  Int_t OpenReducedNtuple();
  Int_t CloseReducedNtuple();
  Int_t OpenReducedFile();
  Int_t CloseReducedFile();
  

  //-----------------------------------
  //Angular distributions
  //-----------------------------------
  
  //angles in the X rest frame
  Float_t ComputeDstarAngle();//angle of the D* w.r.t the  X direction in the X frame
  Float_t ComputeGenDstarAngle();
  Float_t ComputeMCDstarAngle(Int_t mcid);
  Float_t ComputePiAngle();//angle of the extra Pi wrt the X direction in the X frame
  Float_t ComputeGenPiAngle(); 

  //angles in the CM frame
  Float_t ComputePiMotherAngle();//angle of extra pion w.r.t. Mother Momentum  in CM frame
  Float_t ComputeGenPiMotherAngle();
  Float_t ComputeDstarMotherAngle();//angle of D* wrt. Mother Momentum in CM frame
  Float_t ComputeGenDstarMotherAngle();
  Float_t ComputePiDstarAngle();//angle between extra pion and D*  in CM frame

  //helicity= angle of Y daughter wrt to Y mother in Y rest frame
  Float_t ComputeD0HelicityAngle();//Angle of Kaon w.r.t D* direction D0 frame
  Float_t ComputeDstarHelicityAngle();//Angle of D0 w.r.t X direction in D* frame
  
  //angles in D* frame
  Float_t ComputeD0DstarAngleDstarFrame();
  Float_t ComputeMCDstarHelicity(Int_t mcid);
  Float_t ComputeGenDstarHelicity();  
  Float_t ComputePi1D0AngleDstarFrame();


 
  ///----------------------------------
  //Reflection tests
  Float_t ComputexyMass(Float_t xp3,Float_t xcosth,Float_t xphi,Float_t xmass,  
			Float_t yp3,Float_t ycosth,Float_t yphi,Float_t ymass);
  Float_t ComputexyzMass(Float_t xp3,Float_t xcosth,Float_t xphi,Float_t xmass,  
			 Float_t yp3,Float_t ycosth,Float_t yphi,Float_t ymass,
			 Float_t zp3,Float_t zcosth,Float_t zphi,Float_t zmass);
    



  //-----------------------------------------------------------------------------
  TChain* _fChain;
  Int_t _MCorDATA;//DATA=2, GenericMC=1, sigMC=0
  Int_t  _MyDstPiLund;
  TString _OutputDir;//directory where results will be dumped
  Int_t _D0Mode;//1=Kpi, 2=K3pi
  TFile* OutPutFile;//temporary file where results are saved.
  TFile* ReducedFile;//file where reduced ntuple is save
  TTree* ReducedNtuple;//ntuple derived from BtuTupleMaker ntuples
  Bool_t AlreadyTriedClean;


  Int_t cuttruth;
  Int_t cutcharge;
  Int_t cutdmass;
  Int_t cutd0mode;
  Int_t cutdeltam;
  Int_t cutpstar;
  Int_t cutdcos;

  
  ///lumi scaling
  Long_t Nccbar;
  Float_t Crossccbar;//nanobarns
  Float_t lumiccbar;
  Float_t lumidata;
  Float_t scalefactorccbar;

  //reading parameters
  ParameterRead inputpars; 
  

  /////need the database
  //JDataBaseDstPi database;// holds cut definitions and constant numbers.

  //MC produced
  TH1F* HMCXMass;
  TH3F* H3MCPvsThetaVsDstAngle;
  TH3F* H3MCPvsMassVsDstAngle;
  TH3F* H3MCPvsMassVsHelicity;
  TH2F* H2MCPvsMass;
  TH2F* H2MCPvsMassFine;
  TH1F* HMCXp3CM;
  TH1F* HMCXcosthCM;
  TH2F* H2MCXCMPvsTheta;
  TH1F* HMCNX;
  TH2F* H2MCXDalitzPlot;
  TH1F* HMCDstarHelicity;
  TH1F* HMCDstarAngle;
  TH1F* HMCXMassDiff;

  ////for fitting 
  RooRealVar mass;
  TString phasefactor;//D*pi 2-body phase space factor
  TString qfactor;//phasespace x mass
  RooRealVar dau1m;
  RooRealVar dau2m;
  Roo2BodyMassPhaseSpace TwoBodyPS;
  RooRealVar radius;
  TH1F* HMassFit;

 


  //resolution
//   Float_t resmean;
//   Float_t resoffset;
//   Float_t resslope;
//   Float_t resnormratio2;
//   Float_t resmean2;
//   Float_t ressigmaratio2; 
//   Float_t resnormratio3;
//   Float_t resmean3;
//   Float_t ressigmaratio3; 
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
  
  //Efficiency 
  RooRealVar effslope;//average for fit
  RooAbsPdf* effpdf; 
  Float_t effoffsetKpi;//for BF ratio
  Float_t effslopeKpi;
  Float_t effoffsetK3pi;
  Float_t effslopeK3pi;

   

  //The object which does the fits
  MassFitter massfitter;   

  //Eff shape pars
  Float_t helioffset;
  Float_t helislope;
  Float_t cosdstoffset;
  Float_t cosdstslope;



  //editing 
  TLine cutline;
  TLatex text;
  char textlabel[10000];

  TString DstarPiXLabel;
  TString DstarPiYLabel;  

  //for saving plots
  TCanvas Canvas;
  TString filename;
  
  //reding parameters
  ifstream readpars;

  //temp tag to avoid interference between in files 
  TString tempfittag;


  // Declaration of leave types
  Int_t runNumber;
  
  Float_t beamSX;
  Float_t beamSY;
  Float_t beamSZ;
  Float_t primVtxX;
  Float_t primVtxY;
  Float_t primVtxZ;

  Int_t   nX;
  Float_t XChi2[MAXX];  //Must be a float!
  Int_t   XnDof[MAXX];  
  Int_t   XVtxStatus[MAXX];
  Float_t XMass[MAXX];
  Float_t Xp3[MAXX];    
  Float_t Xcosth[MAXX];
  Float_t Xphi[MAXX];   
  Float_t Xp3CM[MAXX];  
  Float_t XcosthCM[MAXX];  
  Float_t XphiCM[MAXX];  
  Float_t Xpi1pidw[MAXX];  
  Float_t Xpi1pidwe[MAXX];  
  Float_t XKpidw[MAXX];  
  Float_t XKpidwe[MAXX];  
  Float_t Xpipidw[MAXX];  
  Float_t Xpipidwe[MAXX];  
  Int_t   XLund[MAXX];  
  Int_t   XMCIdx[MAXX];  
  Int_t   XMyMCIdx[MAXX];
  Int_t   Xd1Lund[MAXX];  
  Int_t   Xd1Idx[MAXX];  
  Int_t   Xd2Lund[MAXX];  
  Int_t   Xd2Idx[MAXX];  
  Int_t   Xd3Lund[MAXX];  
  Int_t   Xd3Idx[MAXX];  
  Float_t XVtxx[MAXX];  
  Float_t XVtxy[MAXX];  
  Float_t XVtxz[MAXX];     
  Int_t   nDstar;
  Float_t DstarChi2[MAXX];  
  Int_t   DstarnDof[MAXX];  
  Int_t   DstarVtxStatus[MAXX];
  Float_t DstarMass[MAXX];  
  Float_t DstarMassErr[MAXX];
  Float_t Dstarp3[MAXX];    
  Float_t Dstarcosth[MAXX];  
  Float_t Dstarphi[MAXX];   
  Float_t Dstarp3CM[MAXX];  
  Float_t DstarcosthCM[MAXX];  
  Float_t DstarphiCM[MAXX]; 
  Int_t   DstarLund[MAXX];  
  Int_t   Dstard1Lund[MAXX];  
  Int_t   Dstard1Idx[MAXX];  
  Int_t   Dstard2Lund[MAXX];  
  Int_t   Dstard2Idx[MAXX];    
  Int_t   DstarMCIdx[MAXX];
  Float_t DstarD0VtxProbUsr[MAXX];
  Int_t   nD0;
  Float_t D0Mass[MAXX];  
  Float_t D0MassErr[MAXX];  
  Float_t D0p3CM[MAXX];  
  Float_t D0costhCM[MAXX];  
  Float_t D0phiCM[MAXX];  
  Int_t   D0Lund[MAXX];  
  Int_t   D0d1Lund[MAXX];  
  Int_t   D0d1Idx[MAXX];  
  Int_t   D0d2Lund[MAXX];  
  Int_t   D0d2Idx[MAXX];   
  Int_t   D0d3Lund[MAXX];  
  Int_t   D0d3Idx[MAXX];
  Int_t   D0d4Lund[MAXX];     
  Int_t   D0d4Idx[MAXX];
  Int_t   D0MCIdx[MAXX];
  Float_t D0Chi2[MAXX];
  Int_t   D0nDof[MAXX];
  Int_t   D0VtxStatus[MAXX];
  Int_t   nK;
  Float_t Kp3[MAXX]; 
  Float_t Kp3CM[MAXX]; 
  Float_t KcosthCM[MAXX]; 
  Float_t KphiCM[MAXX]; 
  Int_t   KLund[MAXX]; 
  Int_t   KMCIdx[MAXX];
  Int_t   KTrkIdx[MAXX];
  Int_t   KSelectorsMap[MAXX];

  Int_t   nPi;
  Float_t Pip3[MAXNCANDS];  
  Float_t Picosth[MAXNCANDS]; 
  Float_t Piphi[MAXNCANDS]; 
  Float_t Pip3CM[MAXNCANDS]; 
  Float_t PicosthCM[MAXNCANDS]; 
  Float_t PiphiCM[MAXNCANDS]; 
  Int_t   PiLund[MAXNCANDS];   
  Int_t   PiMCIdx[MAXNCANDS];
  Int_t   PiTrkIdx[MAXNCANDS];
  Int_t   PiSelectorsMap[MAXNCANDS]; 


  Int_t   TRKnSvt[MAXNCANDS];  
  Int_t   TRKLund[MAXNCANDS];
  Float_t TRKdedxdch[MAXNCANDS];
  Float_t TRKDrcTh[MAXNCANDS];

  ////MC block
  Int_t   mcLen;
  Int_t   mcLund[MAXMCLEN];
  Int_t   mothIdx[MAXMCLEN];
  Int_t   dauLen[MAXMCLEN];
  Int_t   dauIdx[MAXMCLEN];
  Float_t mcmass[MAXMCLEN]; 
  Float_t mccosth[MAXMCLEN];
  Float_t mcphi[MAXMCLEN];
  Float_t mcp3[MAXMCLEN];
  Float_t mccosthCM[MAXMCLEN];
  Float_t mcphiCM[MAXMCLEN];
  Float_t mcp3CM[MAXMCLEN];
  Float_t mcVtxx[MAXMCLEN]; 
  Float_t mcVtxy[MAXMCLEN]; 
  Float_t mcVtxz[MAXMCLEN]; 


  //--------------------------------------
  //////My derived variables 
  //-------------------------------------  
  //Counters
  Int_t MCXCounterPerEvent;
  Int_t MCXCounterTotal;
  Int_t RecoXCounterTotal;
  Int_t RecoXCounterPerEvent;
  
  ////indexes
  Int_t   XIdx;
  Int_t   Xmcidx;
  Int_t   DstarIdx;
  Int_t   Dstarmcidx;
  Int_t   D0Idx;
  Int_t   D0mcidx;
  Int_t   PiIdx;
  Int_t   SlowPiIdx;
  Int_t   KIdx;
  Int_t   PitrkIdx;
  Int_t   KtrkIdx;
  Int_t   D0Pi2Idx;
  Int_t   D0Pi3Idx;
  Int_t   D0Pi2trkIdx;
  Int_t   D0Pi3trkIdx;
  Int_t   SlowPitrkIdx;
  Int_t   Pi1Idx;
  Int_t   Pi1trkIdx;

  ///////Variables to be saved into reduced ntuple
  Int_t runnumber;//1-6
  Int_t eventnumber;//event


  Float_t dstpimass;
  Float_t dstpideltam;
  Float_t dstpimdm;
  Float_t dstpipstar;
  Float_t dstpiphistar;
  Float_t dstpicosstar;
  Float_t dstpilogvtxprob;
  Int_t   dstpicharge;
  Float_t dstpimcpstar;
  Float_t dstpimccosstar;
  Float_t dstpidmres;
  Float_t dstpicosstarres;
  Float_t dstpiphistarres;
  Int_t   dstpimctrue;
  Int_t   dstpimctruebad;
  Int_t   dstpimclund;
  Int_t   dstpinX;
  Int_t   dstpiIdx;
  Float_t dstpipidw;
  Float_t dstpipidwe;
  Float_t dstpigenmass;//D* with rand pi
  Float_t dstpigendist;//D* with rand pi
  Float_t masspislowpi1;
  Float_t masspipi1;
  Float_t massKpislow;
  Float_t massKpislowpi1;
  Float_t massKpipi1;
  Float_t masspipislowpi1;
  Int_t   dstpibestcand;
    
    
  Float_t dstarmass;//D*
  Float_t dstardeltam;
  Float_t dstarm;
  Float_t dstarpstar;
  Float_t dstarlogvtxprob; 
  Float_t dstarcostheta;//angle of D* in X rest-frame
  Float_t dstarcosthetares;
  Int_t   dstarcharge;
  Int_t   dstarmctrue;
  Float_t dstarmothcosine;
  Float_t dstarmothcosineres;
  Float_t dstarhelicity;
  Float_t dstarhelicityres;

  Float_t d0mass;//D0
  Float_t d0pstar; 
  Float_t d0logvtxprob;
  Int_t   d0mctrue; 
  Float_t d0costheta;// angle of D0 in D* rest-frame
  Int_t   d0charge;
  Int_t   d0mode;
  Float_t d0helicity;
  Float_t d0dstarangle;

  Float_t kp3;   
  Int_t   kcharge;
  Int_t   kmctrue;
  Float_t kcostheta;//angle of K+ in D0 rest-frame
  Int_t   klh;

  Float_t pip3;  
  Int_t   picharge; 
  Int_t   pimctrue; 
  Int_t   pilh;
 
  Float_t d0pi2p3;  
  Int_t   d0pi2charge; 
  Int_t   d0pi2mctrue;//other D0 pions
  Int_t   d0pi2lh;
    
  Float_t d0pi3p3;  
  Int_t   d0pi3charge; 
  Int_t   d0pi3mctrue;
  Int_t   d0pi3lh;

  Float_t slowpip3; 
  Int_t   slowpicharge;
  Int_t   slowpimctrue;//slow pion
  Int_t   slowpilh;
  
  Float_t pi1p3;
  Int_t   pi1charge;
  Int_t   pi1mctrue;
  Int_t   pi1lh;  
  Int_t   pi1lhK;  
  Float_t pi1dedx;
  Float_t pi1thetac;
  Float_t pi1cosine;
  Float_t pi1mothcosine;
  Float_t pi1mothcosineres;
  Float_t pi1costheta;
  Float_t pi1costhetares;
  Float_t pi1helicity;//angle of pi0 in D* frame
  Float_t pi1D0angle;//angle of pi1 wrt D0 in D* frame




  ///////////////Systematics
  TString SystDirs[NSysts]; 
  TString SystName[NSysts];
  TString SigName[NSig];
  TString SigTitle[NSig];

  TString SystDirs2[NSysts2]; 
  TString SystName2[NSysts2];

  TString SystDirsFinal[NSystsFinal]; 
  TString SystNameFinal[NSystsFinal];


  TString SystDirsFinalHeli[NSystsFinalHeli]; 
  TString SystNameFinalHeli[NSystsFinalHeli];



};
  

#endif



 //main fitting method
  //TObject* RooFitMassPeak(Int_t WhichHisto);
  //Int_t CreateDeltaMHistogramForFit();    
  //Int_t CreateNotTruthCCbarHistogramForFit();
  //TH1* GetMassHistoForFit(Int_t whichone,Int_t nfitbins,Float_t fitmin,Float_t fitmax);
  //Int_t ShowLineShapes();  

  //Int_t FitMassPeak(Bool_t fit1,Bool_t fit2,Bool_t fit3,Bool_t fit4,Bool_t fit5,Bool_t fit6);

  //Signal MC 
//   Int_t ResolutionEff();  
//   Int_t EffAfterEffCorr2();
  
  //reflection fit for D+pi-
  //Int_t FitReflection();


  //Legendre Projections and efficiency correction event by event
  //  Int_t WeightEvents();
  //Int_t PlotLegendreWeightedEvents();
  //Int_t WeightK3piBkgEvents(Int_t WhichSideBand);

  
/*   // */
/*   Int_t PlotDMVsPstarCosHel(Float_t dmmin,Float_t dmmax,Int_t nbins); */
/*   Int_t PlotMassVsPstar(Float_t dmmin,Float_t dmmax,Int_t nbins,Float_t min,Float_t max,Int_t Nslices); */
/*   Int_t PlotMassVsCosDstar(Float_t massmin,Float_t massmax,Int_t nbins,Float_t min,Float_t max,Int_t Nslices); */
/*   Int_t PlotMassVsHelicity(Float_t massmin,Float_t massmax,Int_t nbins,Float_t min,Float_t max,Int_t Nslices); */
  
/*   //wrong sign fit */
/*   Int_t RooFitWS(Bool_t CreateNewHistos,Int_t whichpdf); */

 
  
/*   //special fit */
/*   Int_t RooFitDMLowCosHel(Int_t WhichBumpShape); */
  
/*   //plots extra pion PID */
/*   Int_t PlotExtraPionPID(); */
  
/*   //background subtraction using ccbar */
/*   Int_t CompareCCbar(); */

/*   //Legendre projections */
/*   Int_t PlotWeightedMass(Int_t WhichHistos); */

/*   //reflection tests */
/*   Int_t FinalStateMasses(); */
/*   Int_t PlotSlowPiPi1Interference(); */

/*   //angular distribution of D0 wrt to the X in the D* frame   */
/*   Int_t DstarHelicity(); */
/*   Int_t DstarHelicityEffCorr(); */

/*   //Mass vs DstarHelicity for Legendre projections */
/*   Int_t CreateMassVsHelicity(Int_t DataOrToy); */


/*   Int_t FitMassVsHelicityMW(Bool_t fit); */
/*   Int_t PlotMassVsHelicityMW();   */
//   Int_t FitMassVsEnergy(Bool_t fit);
//   Int_t PlotMassVsEnergy();
//   Int_t FitMassVsCosDstar(Bool_t fit);
//   Int_t PlotMassVsCosDstar();
//   Int_t FitMassVsCosDstarMW(TString sigtitle,Bool_t fit);
//   Int_t PlotMassVsCosDstarMW(TString sigtitle);
//   Int_t PlotFitMassVsCosDstarMW(TString sigtitle);


/*   //cut optimization */
/*   Int_t FitCutOptimization(Bool_t fit); */
/*   Int_t PlotCutOptimization(); */
  
/*   //temporary */
/*   Int_t FitBumps(Float_t pmin,Float_t pmax,Float_t cmin,Float_t cmax,Float_t hmin,Float_t hmax,Int_t nbins,Float_t mmin,Float_t mmax,Int_t fit); */
/*   Int_t FitBumpsVsHelicity(Bool_t fit); */
/*   Int_t PlotBumpsVsHelicity(); */
/*   Int_t FitWS(Float_t pmin,Float_t pmax,Float_t cmin,Float_t cmax,Float_t hmin,Float_t hmax); */
  
/*   //Determine best pol. order */
/*   Int_t FitVsPolOrder(TString sigtitle,Bool_t fit); */

/*   //background to be subtracted in the K3pi mode determined from the D* sidebands */
/*   Int_t CompareWithK3pi(Bool_t getsideband); */
  

/*   // */

/*   //Int_t CleanNtuple(); */

/*   // */
/*   Int_t  FitGeneratedMass(Int_t nfiles); */

/*   // */
/*   Int_t DrawTruthRatios(); */

/*   //Toy MCs */
/*   Int_t GenerateToySigOnBkg(Bool_t generate,Int_t ntoys,Int_t fileid); */
/*   Int_t PlotToySigOnBkg(Int_t fileid); */
/*   //Int_t GenerateToySmearing(); */
/*   //Int_t FitToySmearing(); */

  
 

/*   //basic mass plots for presentation */
/*   Int_t MakeD0DstarMass(); */
/*   Int_t PlotD0DstarMass(); */
/*   Int_t MakeRSWithNotTruthedandWS(); */
/*   Int_t PlotRSWithNotTruthedandWS(); */
/*   Int_t FitDstarMass(); */
/*   Int_t DrawMassVsHelicity2D(Int_t nbins,Float_t massmin,Float_t massmax,TCut cut); */
/*   Int_t FitD0Mass(); */
  
  ///systematics
//   Int_t Systematics(TString sigtitle,Bool_t fit);
//   Int_t OpenFitPars(TString filename); 
//   Int_t CloseFitPars(); 
//   Float_t GetFitPar(TString signalname,TString parametername); 
//   Float_t GraphXvsY(Int_t N,Double_t* X,Double_t*Y,Double_t* XE,Double_t*YE,TString xtitle,TString ytitle,Float_t finalres,Float_t finalresEr,Float_t gen); 
 

//   //-------------
//   //Miscellanous methods
//   //------------ 
//   TH1* CreateRebinnedDstPiHisto(TH1*,Int_t nbins,Float_t min,Float_t max);
//   TH2F* CreateRebinnedDstPiHisto2D(TH2* oldhisto,Int_t nbins,Float_t min,Float_t max,Int_t prevBinning); 


  //-----------
  //old  things
  //-----------
  //Int_t EffAfterEffCorr();//check 3-D eff 
  //Int_t CreateEffCorrectedHistos();//Creation of efficiency corrected histograms, including Mass vs DstarHelicity


//   //K3pi background from D* sidebands
//   TH1* K3piDstarBkg(Int_t nbins,Float_t min,Float_t max);
//   TH1F* K3piDstarBkgEffWeighted(Int_t nbins,Float_t min,Float_t max);

  
//   //histograms to be saved for later fitting  
//   TString GetBackgroundFormula(Int_t type);
 
 

//   //wrong sign histograms to be fitted 
//   Int_t CreateHistosRooFitWS();
//   TH1F* RooFitWSHisto(TH1* HDstarPi,TCanvas* Canvas,TString filename,Int_t whichpdf);

  //---------------
  //ccbar correction to D* angle discrepency
//   Int_t PlotMassVsDstarAngleCut(TFile* DataFile,TString plotname,TFile* MCFile,TString mcplotname,TCanvas* Canvas,TString filename,Bool_t accumulate,Int_t whichscale);
//   Int_t CreateDstarAngleCorrection(TFile* DataFile,TFile* MCFile);
//   Int_t ApplyDstarAngleCorrection(TFile* DataFile,TFile* MCFile);
//   Int_t Scale2DMass(TH2*h2,TH2*h2c,TCanvas* Canvas,TString filename);
//   TH1F* AverageBins(TH1F*h,Int_t group);  
//   Int_t ShowRatios(TFile* File,TCanvas*C,TString filename);
//   TH1F* ExtrapolateRatio(TH1F*h,Int_t split);
//   Int_t FitMassTails(TH2*h2,TCanvas*C,TString filename);
//   Int_t PlotXSlices(TH2*h2,TCanvas*C,TString filename);
//   Int_t PlotCorrectedData(TFile* DataFile,TFile* MCFile,TCanvas* Canvas,TString filename);
//   TH2F* ExtrapolateInAngle(TH1F* H1D[8]);
//   //Int_t CreateCCbarCorrectionHistos(RooAbsPdf*pdf,RooRealVar* var,Float_t scale,TFile*file);


  ///-------------------------------------------------------------
  //Signal MC
  ///resolution ///efficiency
  //  Int_t DrawResolutionVsMass();  
  //  TH1F* GetResolutionHisto(TString name);
//   Int_t DrawEfficiencyVsMass(); 
//   Int_t DrawEfficiencyVsPstar();
//   Int_t DrawEfficiencyVsDstarAngle();
//   Int_t DrawEfficiencyVsDstarHelicity();
//   TH1* Plot1DEfficiency(TH1* HGen,TH1* HRec,Bool_t fit);
  
//   TH2F* Interpolate2DEfficiencyFitY(TH2F* h2,TH1F* HEffChiY);
//   TH2F* Interpolate2DEfficiencyFitX(TH2F* h2,TH1F* HEffChiX);
//   Int_t Draw2DEfficiencyFit();  
//   Int_t Compute2DEfficiencyFit(TH2F*H2Reco,TH2F*H2Gen);    
//   //for extracting the efficiency map.  
//   Int_t  OpenEfficiency();
//   Float_t GetEfficiency();
//   Float_t GetEfficiencyNoShape();
//   Float_t GetEfficiencyErr();
//   void CloseEfficiency();
//   TH2F* H2Efficiency;
//   TH3F* H3Efficiency;
//   TFile* EffFile;
//   TF1* FHelEff;
//   TF1* FCosDstEff;
//   Float_t effpstarstep,effpstarmin;
//   Float_t effmassstep,effmassmin;

//   Int_t DrawEfficiencyVsCosstar();
//   Int_t Draw2DEfficiency();

/*   Int_t Draw3DEfficiency(); */
/*   TH2F* Interpolate2DEfficiencyX(TH2F* h2); */
/*   TH2F* Interpolate2DEfficiencyY(TH2F* h2); */
/*   TH3F* Interpolate2DEfficiencyZ(TH2F* h2[4]); */

/*   Int_t Draw3DEfficiencyFit(); */
/*   TH3F* Interpolate2DEfficiencyFitZ(TH2F* h2[4],TH2F* HEffChiZ); */
  


//   //Histograms not corrected for efficiency for Legendre projection
//   TH2F* GetToyCosHelVsMass();
//   TH2F* GetCosHelVsMass();


  //-------------
  //Miscellanous methods
  //------------ 
  //void Correct1DEfficiencyOn2DPlot(TH2F* h2,TH1F* heff);
 

