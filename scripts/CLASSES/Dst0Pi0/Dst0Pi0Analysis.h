#ifndef Dst0Pi0Analysis_hh
#define Dst0Pi0Analysis_hh
#include "../../Globals.h"
#include "../../AnalysisGlobals.h"
#include "../../FUNCTIONS/SetHisto.h"
#include "../../FUNCTIONS/Fits.h"
#include "../../FUNCTIONS/MoveStatsBox.h"
#include "../../CLASSES/JResiduals.h"
#include "../JCutSetup.h"
#include "../JCut.h"
#include "../JCutFloat.h"
#include "../JCutInt.h"
#include "../JCutList.h"
#include "../JCutEllipse.h"
#include "JDataBaseDst0Pi0.h"
#include <TVector3.h>
#include <TLorentzRotation.h>
#include <TLorentzVector.h>
#include <TLine.h>
#include <TGraphErrors.h>
#include <TCut.h>
#include <TBox.h>

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
using namespace RooFit ;
////////////////////////



#define ARRAYSIZE 100
#define MAXNCANDS 100

//#define MYDSTPILUND 10423 //10423=D2420, 425=D2460, 523 for B*+ 
#define MYDSTARLUND 413


class Dst0Pi0Analysis:public TObject{
  ClassDef(Dst0Pi0Analysis,1);

 public:
  Dst0Pi0Analysis(TChain* ntuple,Bool_t TruthMatch,Int_t MatterOrAntiMatter,Int_t DstPiLund,TString OutputDir,Int_t D0Mode);
  ~Dst0Pi0Analysis();
  
  Int_t ReduceNtuple(Bool_t RunInTestMode);
  //Int_t RooFitMassPeak(Bool_t fit,Int_t whichshape1,Int_t whichshape2,Bool_t CorrRes,Bool_t CorrEff,Bool_t AddK3pi,Int_t fileid);
  //Int_t FitMassPeak(Bool_t fit1,Bool_t fit2,Bool_t fit3,Bool_t fit4,Bool_t fit5,Bool_t fit6);
  //Int_t ResolutionEff();
  Int_t PlotDMVsPstarCosHel();
  //Int_t RooFitWS();
  //Int_t RooFitDMLowCosHel(Int_t WhichBumpShape);
    
 private:
  
  Int_t SetBranches();
  Int_t SetModeBranches();

  Int_t MakeReducedNtupleBranches();
  Int_t MakeModeReducedNtupleBranches();

  Int_t SetReducedNtupleBranches();
  Int_t SetModeReducedNtupleBranches();

  Int_t OpenReducedNtuple(TString filename);
  Int_t CloseReducedNtuple();
 
  Float_t ComputeDstarAngle();
  Float_t ComputePiAngle();//angle of extra pion in mothers frame
  Float_t ComputePiDstarAngle();//angle between extra pion and D*  in CM frame
  Float_t ComputePiHelicityAngle();//
  Float_t ComputeSlowPiHelicityAngle();//

  TH1* K3piDstarBkg(Int_t nbins,Float_t min,Float_t max);


  ///resolution
  //void Dst0Pi0Analysis::DrawResolutionVsMass(TCanvas *Canvas,TString filename,TTree*tree);
  ///efficiency
  //void Dst0Pi0Analysis::DrawEfficiencyVsMass(TCanvas *Canvas,TString filename,TTree*tree);


  TChain* _fChain;
  Bool_t _TruthMatch;
  Int_t _MatterOrAntiMatter;
  Int_t  _MyDstPiLund;
  TString _OutputDir;
  Int_t _D0Mode;
  TFile* OutPutFile;
  TTree* ReducedNtuple;


  // Declaration of leave types
  Float_t beamSX;
  Float_t beamSY;
  Float_t beamSZ;
  Float_t primVtxX;
  Float_t primVtxY;
  Float_t primVtxZ;

  Int_t   nX;
  Float_t XChi2[ARRAYSIZE];  //Must be a float!
  Int_t   XnDof[ARRAYSIZE];  
  Int_t   XVtxStatus[ARRAYSIZE];
  Float_t XMass[ARRAYSIZE];
  Float_t Xp3[ARRAYSIZE];    
  Float_t Xcosth[ARRAYSIZE];
  Float_t Xphi[ARRAYSIZE];   
  Float_t Xp3CM[ARRAYSIZE];  
  Float_t XcosthCM[ARRAYSIZE];  
  Float_t XphiCM[ARRAYSIZE];  
  Int_t   XLund[ARRAYSIZE];  
  Int_t   XMCIdx[ARRAYSIZE];  
  Int_t   Xd1Lund[ARRAYSIZE];  
  Int_t   Xd1Idx[ARRAYSIZE];  
  Int_t   Xd2Lund[ARRAYSIZE];  
  Int_t   Xd2Idx[ARRAYSIZE];  
  Int_t   Xd3Lund[ARRAYSIZE];  
  Int_t   Xd3Idx[ARRAYSIZE];  
  Float_t XVtxx[ARRAYSIZE];  
  Float_t XVtxy[ARRAYSIZE];  
  Float_t XVtxz[ARRAYSIZE];     
  Int_t   nDstar;
  Float_t DstarChi2[ARRAYSIZE];  
  Int_t   DstarnDof[ARRAYSIZE];  
  Int_t   DstarVtxStatus[ARRAYSIZE];
  Float_t DstarMass[ARRAYSIZE];  
  Float_t DstarMassErr[ARRAYSIZE];
  Float_t Dstarp3[ARRAYSIZE];    
  Float_t Dstarcosth[ARRAYSIZE];  
  Float_t Dstarphi[ARRAYSIZE];   
  Float_t Dstarp3CM[ARRAYSIZE];  
  Float_t DstarcosthCM[ARRAYSIZE];  
  Float_t DstarphiCM[ARRAYSIZE]; 
  Int_t   DstarLund[ARRAYSIZE];  
  Int_t   Dstard1Lund[ARRAYSIZE];  
  Int_t   Dstard1Idx[ARRAYSIZE];  
  Int_t   Dstard2Lund[ARRAYSIZE];  
  Int_t   Dstard2Idx[ARRAYSIZE];    
  Int_t   DstarMCIdx[ARRAYSIZE];
  Int_t   nPi0;
  Float_t Pi0Chi2[ARRAYSIZE];  
  Int_t   Pi0nDof[ARRAYSIZE];  
  Int_t   Pi0VtxStatus[ARRAYSIZE];
  Float_t Pi0Mass[ARRAYSIZE];  
  Float_t Pi0MassErr[ARRAYSIZE];
  Float_t Pi0p3[ARRAYSIZE];    
  Float_t Pi0costh[ARRAYSIZE];  
  Float_t Pi0phi[ARRAYSIZE];   
  Float_t Pi0p3CM[ARRAYSIZE];  
  Float_t Pi0costhCM[ARRAYSIZE];  
  Float_t Pi0phiCM[ARRAYSIZE]; 
  Int_t   Pi0Lund[ARRAYSIZE];  
  Int_t   Pi0d1Lund[ARRAYSIZE];  
  Int_t   Pi0d1Idx[ARRAYSIZE];  
  Int_t   Pi0d2Lund[ARRAYSIZE];  
  Int_t   Pi0d2Idx[ARRAYSIZE];    
  Int_t   Pi0MCIdx[ARRAYSIZE];
  Int_t   nGam;
  Float_t Gamp3[ARRAYSIZE];    
  Float_t Gamcosth[ARRAYSIZE];  
  Float_t Gamphi[ARRAYSIZE];   
  Float_t Gamp3CM[ARRAYSIZE];  
  Float_t GamcosthCM[ARRAYSIZE];  
  Float_t GamphiCM[ARRAYSIZE]; 
  Int_t   GamLund[ARRAYSIZE];  
  Int_t   GamMCIdx[ARRAYSIZE];
  Int_t   nD0;
  Float_t D0Mass[ARRAYSIZE];  
  Float_t D0MassErr[ARRAYSIZE];  
  Float_t D0p3CM[ARRAYSIZE];  
  Float_t D0costhCM[ARRAYSIZE];  
  Float_t D0phiCM[ARRAYSIZE];  
  Int_t   D0Lund[ARRAYSIZE];  
  Int_t   D0d1Lund[ARRAYSIZE];  
  Int_t   D0d1Idx[ARRAYSIZE];  
  Int_t   D0d2Lund[ARRAYSIZE];  
  Int_t   D0d2Idx[ARRAYSIZE];   
  Int_t   D0d3Lund[ARRAYSIZE];  
  Int_t   D0d3Idx[ARRAYSIZE];
  Int_t   D0d4Lund[ARRAYSIZE];     
  Int_t   D0d4Idx[ARRAYSIZE];
  Int_t   D0MCIdx[ARRAYSIZE];
  Float_t D0Chi2[ARRAYSIZE];
  Int_t   D0nDof[ARRAYSIZE];
  Int_t   D0VtxStatus[ARRAYSIZE];
  //Int_t   D0nDaus[ARRAYSIZE];  
  Int_t   nPi;
  Float_t Pip3[ARRAYSIZE];  
  Float_t Picosth[ARRAYSIZE]; 
  Float_t Piphi[ARRAYSIZE]; 
  Float_t Pip3CM[ARRAYSIZE]; 
  Float_t PicosthCM[ARRAYSIZE]; 
  Float_t PiphiCM[ARRAYSIZE]; 
  Int_t   PiLund[ARRAYSIZE];   
  Int_t   PiMCIdx[ARRAYSIZE];
  Int_t   PiTrkIdx[ARRAYSIZE];
  Int_t   PiSelectorsMap[ARRAYSIZE];
  Int_t   nK;
  Float_t Kp3[ARRAYSIZE]; 
  Float_t Kp3CM[ARRAYSIZE]; 
  Float_t KcosthCM[ARRAYSIZE]; 
  Float_t KphiCM[ARRAYSIZE]; 
  Int_t   KLund[ARRAYSIZE]; 
  Int_t   KMCIdx[ARRAYSIZE];
  Int_t   KTrkIdx[ARRAYSIZE];
  Int_t   KSelectorsMap[ARRAYSIZE];
  Int_t   TRKnSvt[ARRAYSIZE];
  Int_t   TRKLund[ARRAYSIZE];
  ////MC block
  Int_t   mcLen;
  Int_t   mcLund[ARRAYSIZE*2];
  Int_t   mothIdx[ARRAYSIZE*2];
  Int_t   dauLen[ARRAYSIZE*2];
  Int_t   dauIdx[ARRAYSIZE*2];
  Float_t mcmass[ARRAYSIZE]; 
  Float_t mccosth[ARRAYSIZE*2];
  Float_t mcphi[ARRAYSIZE*2];
  Float_t mcp3[ARRAYSIZE*2];
  Float_t mccosthCM[ARRAYSIZE*2];
  Float_t mcphiCM[ARRAYSIZE*2];
  Float_t mcp3CM[ARRAYSIZE*2];
  Float_t mcVtxx[ARRAYSIZE*2]; 
  Float_t mcVtxy[ARRAYSIZE*2]; 
  Float_t mcVtxz[ARRAYSIZE*2]; 


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
  Int_t   DstarIdx; 
  Int_t   Pi0Idx;
  Int_t   Gam1Idx;
  Int_t   Gam2Idx;
  Int_t   D0Idx;
  Int_t   SlowPiIdx;
  Int_t   SlowGam1Idx;
  Int_t   SlowGam2Idx;
  Int_t   PiIdx;
  Int_t   PitrkIdx;
  Int_t   KIdx;
  Int_t   KtrkIdx;
  Int_t   D0Pi2Idx;
  Int_t   D0Pi2trkIdx;  
  Int_t   D0Pi3Idx;  
  Int_t   D0Pi3trkIdx;
 
 

  ///////Variables to be saved into reduced ntuple
  Int_t eventnumber;//event

  Float_t dstpimass;
  Float_t dstpideltam;
  Float_t dstpipstar;
  Float_t dstpimcpstar;
  Float_t dstpicosstar;
  Float_t dstpimccosstar;
  Float_t dstpilogvtxprob;
  Float_t dstpidmres;
  Int_t   dstpicharge;
  Int_t   dstpimctrue;
  Int_t   dstpimclund;
  Int_t   dstpinX;
  Int_t   dstpiIdx;
  Float_t dstpigenmass;//D* with rand pi
  Float_t dstpigendist; //D* with rand pi
    
  Float_t dstarmass;//D*
  Float_t dstardeltam;
  Float_t dstarpstar;
  Float_t dstarlogvtxprob; 
  Float_t dstarcostheta;//angle of D* in X rest-frame
  Int_t   dstarcharge;
  Int_t   dstarmctrue;
 
  Float_t pi0mass;
  Float_t pi0p3; 
  Float_t pi0energy;
  Int_t   pi0mctrue; 
  Float_t pi0costheta;
  Float_t pi0cosine;//angle between pi0 and D*
  Float_t pi0helicity;//angle of pi0 in D* frame

  Float_t gam1energy;
  Float_t gam2energy; 
  Float_t gam1cos;
  Float_t gam2cos;

  Float_t d0mass;//D0
  Float_t d0pstar; 
  Float_t d0logvtxprob;
  Int_t   d0mctrue; 
  Float_t d0costheta;// angle of D0 in D* rest-frame
  Int_t   d0charge;
  Int_t   d0mode;

  Float_t slowpimass;
  Float_t slowpip3;
  Float_t slowpienergy;
  Int_t   slowpimctrue;
  Float_t slowpihelicity;//angle of pi0 in D* frame;

  Float_t slowgam1energy;
  Float_t slowgam2energy; 
  Float_t slowgam1cos;
  Float_t slowgam2cos;

  Float_t pip3;
  Int_t   picharge;
  Int_t   pimctrue;
  Int_t   pilh;  

  Float_t kp3;   
  Int_t   kcharge;
  Int_t   kmctrue;
  Float_t kcostheta;//angle of K+ in D0 rest-frame
  Int_t   klh;

  Float_t d0pi2p3;  
  Int_t   d0pi2charge; 
  Int_t   d0pi2mctrue;//other D0 pions
  Int_t   d0pi2lh;
    
  Float_t d0pi3p3;  
  Int_t   d0pi3charge; 
  Int_t   d0pi3mctrue;
  Int_t   d0pi3lh;


  

  /////need the database
  JDataBaseDst0Pi0 database;

  //MC produced
  TH1F* HMCXMass;
  TH1F* HMCXDeltaM;
  TH1F* HMCXp3CM;
  TH1F* HMCXcosthCM;
  TH2F* H2MCXCMPvsTheta;
  TH1F* HMCNX;

  ////for fitting 
  TString phasefactor;
  TString qfactor;


  //editing 
  //a cut drawing line
  TLine cutline;
  TText text;
  char textlabel[100];

  TString DstarPiXLabel;
  TString DstarPiYLabel;


};
  

#endif

