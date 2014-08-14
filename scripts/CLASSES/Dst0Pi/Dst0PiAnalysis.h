#ifndef Dst0PiAnalysis_hh
#define Dst0PiAnalysis_hh
#include "../../Globals.h"
#include "../../AnalysisGlobals.h"
#include "../../FUNCTIONS/SetHisto.h"
#include "../../FUNCTIONS/Fits.h"
#include "../../FUNCTIONS/MoveStatsBox.h"
#include "../../CLASSES/JResiduals.h"
#include "../MassFitter.h"
#include "../RooResolutionPdf.h"
#include "../Roo2BodyMassPhaseSpace.h"
#include "../Dstar0/Dst0Analysis.h"

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


#define Dst0PDGMass 2.007

#define MAXNCANDS 100

class Dst0PiAnalysis:public Dst0Analysis{
  ClassDef(Dst0PiAnalysis,1);

 public:
  Dst0PiAnalysis(Bool_t MCorDATA,Bool_t TruthMatch,Int_t MatterOrAntiMatter,TString OutputDir);
  ~Dst0PiAnalysis();
  
  void SetChain(TChain*chain){_fChain=chain;}
  Int_t ReduceNtuple();


  //
  Int_t MakeRawDst0PiMass();
  Int_t PlotRawDst0PiMass();
  
  //Int_t MakeHistosForFit();
  Int_t FitMass(TString name,TString subname);

/*   Int_t PlotDMVsPstarCosHel(); */

        
 private:
  
  Int_t SetDst0PiBranches();
  Int_t MakeDst0PiReducedNtupleBranches(TTree*Ntuple);
  Int_t SetDst0PiReducedNtupleBranches();

  void FillDst0PiVars();

  Float_t ComputeDst0Angle();
  Float_t ComputePiAngle();//angle of extra pion in mothers frame
  Float_t ComputePiDst0Angle();//angle between extra pion and D*  in CM frame
  Float_t ComputePiHelicityAngle();//


  Int_t  _MyDstPiLund;


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
  Int_t   Xd1Lund[MAXNCANDS];  
  Int_t   Xd1Idx[MAXNCANDS];  
  Int_t   Xd2Lund[MAXNCANDS];  
  Int_t   Xd2Idx[MAXNCANDS];  
  Float_t XVtxx[MAXNCANDS];  
  Float_t XVtxy[MAXNCANDS];  
  Float_t XVtxz[MAXNCANDS];     





  //--------------------------------------
  //////My derived variables 
  //-------------------------------------
  
 

  ////indexes
  Int_t   XIdx;
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
    
  Float_t dstarcostheta;

  Float_t pi1p3; 
  Int_t   pi1charge;
  Int_t   pi1mctrue;
  Int_t   pi1lh; 
  Int_t   pi1lhK;  
  Float_t pi1dedx;
  Float_t pi1thetac;
  Float_t pi1cosine;
  Float_t pi1costheta;
  Float_t pi1helicity;//angle of pi0 in D* frame


  ////for fitting 
  TString phasefactor;
  TString qfactor;


  //editing 
  //a cut drawing line
  TLine cutline;
  TText text;
  char textlabel[100];

  TString Dst0PiXLabel;
  TString Dst0PiYLabel;


};
  

#endif

