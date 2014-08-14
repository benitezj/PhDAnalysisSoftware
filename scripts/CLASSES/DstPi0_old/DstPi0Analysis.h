#ifndef DstPi0Analysis_hh
#define DstPi0Analysis_hh
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
#include "JDataBaseDstPi0.h"
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



#define MAXNCANDS 100
//#define MYDSTPILUND 10423 //10423=D2420, 425=D2460, 523 for B*+ 
#define MYDSTARLUND 413


class DstPi0Analysis:public TObject{
  ClassDef(DstPi0Analysis,1);

 public:
  DstPi0Analysis(TChain* ntuple,Bool_t TruthMatch,Int_t MatterOrAntiMatter,Int_t DstPiLund,TString OutputDir,Int_t D0Mode);
  ~DstPi0Analysis();
  
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

  TH1* K3piDstarBkg(Int_t nbins,Float_t min,Float_t max);


  ///resolution
  //void DstPi0Analysis::DrawResolutionVsMass(TCanvas *Canvas,TString filename,TTree*tree);
  ///efficiency
  //void DstPi0Analysis::DrawEfficiencyVsMass(TCanvas *Canvas,TString filename,TTree*tree);


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
  Int_t   Xd1Lund[MAXNCANDS];  
  Int_t   Xd1Idx[MAXNCANDS];  
  Int_t   Xd2Lund[MAXNCANDS];  
  Int_t   Xd2Idx[MAXNCANDS];  
  Int_t   Xd3Lund[MAXNCANDS];  
  Int_t   Xd3Idx[MAXNCANDS];  
  Float_t XVtxx[MAXNCANDS];  
  Float_t XVtxy[MAXNCANDS];  
  Float_t XVtxz[MAXNCANDS];     
  Int_t   nDstar;
  Float_t DstarChi2[MAXNCANDS];  
  Int_t   DstarnDof[MAXNCANDS];  
  Int_t   DstarVtxStatus[MAXNCANDS];
  Float_t DstarMass[MAXNCANDS];  
  Float_t DstarMassErr[MAXNCANDS];
  Float_t Dstarp3[MAXNCANDS];    
  Float_t Dstarcosth[MAXNCANDS];  
  Float_t Dstarphi[MAXNCANDS];   
  Float_t Dstarp3CM[MAXNCANDS];  
  Float_t DstarcosthCM[MAXNCANDS];  
  Float_t DstarphiCM[MAXNCANDS]; 
  Int_t   DstarLund[MAXNCANDS];  
  Int_t   Dstard1Lund[MAXNCANDS];  
  Int_t   Dstard1Idx[MAXNCANDS];  
  Int_t   Dstard2Lund[MAXNCANDS];  
  Int_t   Dstard2Idx[MAXNCANDS];    
  Int_t   DstarMCIdx[MAXNCANDS];
  Int_t   nPi0;
  Float_t Pi0Chi2[MAXNCANDS];  
  Int_t   Pi0nDof[MAXNCANDS];  
  Int_t   Pi0VtxStatus[MAXNCANDS];
  Float_t Pi0Mass[MAXNCANDS];  
  Float_t Pi0MassErr[MAXNCANDS];
  Float_t Pi0p3[MAXNCANDS];    
  Float_t Pi0costh[MAXNCANDS];  
  Float_t Pi0phi[MAXNCANDS];   
  Float_t Pi0p3CM[MAXNCANDS];  
  Float_t Pi0costhCM[MAXNCANDS];  
  Float_t Pi0phiCM[MAXNCANDS]; 
  Int_t   Pi0Lund[MAXNCANDS];  
  Int_t   Pi0d1Lund[MAXNCANDS];  
  Int_t   Pi0d1Idx[MAXNCANDS];  
  Int_t   Pi0d2Lund[MAXNCANDS];  
  Int_t   Pi0d2Idx[MAXNCANDS];    
  Int_t   Pi0MCIdx[MAXNCANDS];
  Int_t   nGam;
  Float_t Gamp3[MAXNCANDS];    
  Float_t Gamcosth[MAXNCANDS];  
  Float_t Gamphi[MAXNCANDS];   
  Float_t Gamp3CM[MAXNCANDS];  
  Float_t GamcosthCM[MAXNCANDS];  
  Float_t GamphiCM[MAXNCANDS]; 
  Int_t   GamLund[MAXNCANDS];  
  Int_t   GamMCIdx[MAXNCANDS];
  Int_t   nD0;
  Float_t D0Mass[MAXNCANDS];  
  Float_t D0MassErr[MAXNCANDS];  
  Float_t D0p3CM[MAXNCANDS];  
  Float_t D0costhCM[MAXNCANDS];  
  Float_t D0phiCM[MAXNCANDS];  
  Int_t   D0Lund[MAXNCANDS];  
  Int_t   D0d1Lund[MAXNCANDS];  
  Int_t   D0d1Idx[MAXNCANDS];  
  Int_t   D0d2Lund[MAXNCANDS];  
  Int_t   D0d2Idx[MAXNCANDS];   
  Int_t   D0d3Lund[MAXNCANDS];  
  Int_t   D0d3Idx[MAXNCANDS];
  Int_t   D0d4Lund[MAXNCANDS];     
  Int_t   D0d4Idx[MAXNCANDS];
  Int_t   D0MCIdx[MAXNCANDS];
  Float_t D0Chi2[MAXNCANDS];
  Int_t   D0nDof[MAXNCANDS];
  Int_t   D0VtxStatus[MAXNCANDS];
  //Int_t   D0nDaus[MAXNCANDS];  
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
  Int_t   nK;
  Float_t Kp3[MAXNCANDS]; 
  Float_t Kp3CM[MAXNCANDS]; 
  Float_t KcosthCM[MAXNCANDS]; 
  Float_t KphiCM[MAXNCANDS]; 
  Int_t   KLund[MAXNCANDS]; 
  Int_t   KMCIdx[MAXNCANDS];
  Int_t   KTrkIdx[MAXNCANDS];
  Int_t   KSelectorsMap[MAXNCANDS];
  Int_t   TRKnSvt[MAXNCANDS];
  Int_t   TRKLund[MAXNCANDS];
  ////MC block
  Int_t   mcLen;
  Int_t   mcLund[MAXNCANDS*2];
  Int_t   mothIdx[MAXNCANDS*2];
  Int_t   dauLen[MAXNCANDS*2];
  Int_t   dauIdx[MAXNCANDS*2];
  Float_t mcmass[MAXNCANDS]; 
  Float_t mccosth[MAXNCANDS*2];
  Float_t mcphi[MAXNCANDS*2];
  Float_t mcp3[MAXNCANDS*2];
  Float_t mccosthCM[MAXNCANDS*2];
  Float_t mcphiCM[MAXNCANDS*2];
  Float_t mcp3CM[MAXNCANDS*2];
  Float_t mcVtxx[MAXNCANDS*2]; 
  Float_t mcVtxy[MAXNCANDS*2]; 
  Float_t mcVtxz[MAXNCANDS*2]; 


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
  Int_t   D0Idx;
  Int_t   Gam1Idx;
  Int_t   Gam2Idx;
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
  Float_t pi0pstar;
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

  Float_t slowpip3; 
  Int_t   slowpicharge;
  Int_t   slowpimctrue;//slow pion
  Int_t   slowpilh;
  

  /////need the database
  JDataBaseDstPi0 database;

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

