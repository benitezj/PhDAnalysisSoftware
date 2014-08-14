#ifndef DstPi0Analysis_hh
#define DstPi0Analysis_hh
#include "../../Globals.h"
#include "../../AnalysisGlobals.h"
#include "../../FUNCTIONS/SetHisto.h"
#include "../../FUNCTIONS/Fits.h"
#include "../../FUNCTIONS/MoveStatsBox.h"
#include "../../CLASSES/JResiduals.h"
#include "../MassFitter.h"
#include "../RooResolutionPdf.h"
#include "../Roo2BodyMassPhaseSpace.h"
#include "../Dstar/DstAnalysis.h"

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


#define DstPDGMass 2.0099
#define Pi0MeanMass .1333 //from a 1 gauss + pol2 fit

#define MAXNCANDS 100

class DstPi0Analysis:public DstAnalysis{
  ClassDef(DstPi0Analysis,1);

 public:
  DstPi0Analysis(Bool_t MCorDATA,TString OutputDir);
  ~DstPi0Analysis();
  
  void SetChain(TChain*chain){_fChain=chain;}
  void SetReduceCuts(Int_t cutdmass,Int_t cutdstdeltam,Int_t cutpstar,Int_t cutpi0mass,Int_t cutvtx,Int_t cutdstcos){
    _cutdmass=cutdmass;_cutdstdeltam=cutdstdeltam;_cutpstar=cutpstar;_cutpi0mass=cutpi0mass;_cutvtx=cutvtx;_cutdstcos=cutdstcos;
  }
  Int_t ReduceNtuple();
  

  //
  Int_t MakeRawPi0Mass();
  Int_t PlotRawPi0Mass();  
  Int_t MakeRawDstMassPi0Mass();
  Int_t PlotRawDstMassPi0Mass();  
  Int_t MakeRawDstPi0Mass();
  Int_t PlotRawDstPi0Mass();  
  //Int_t MakeHistosForFit();
  Int_t FitMass(TString name);

        
 private:
  
  Int_t SetDstPi0Branches();
  Int_t MakeDstPi0ReducedNtupleBranches(TTree*Ntuple);

  void FillDstPi0Vars();

  Float_t ComputeDstAngle();
  Float_t ComputePiAngle();//angle of extra pion in mothers frame
  Float_t ComputePiDstAngle();//angle between extra pion and D*  in CM frame
  Float_t ComputePiHelicityAngle();//


  Int_t _cutdmass;
  Int_t _cutdstdeltam;
  Int_t _cutpstar;
  Int_t _cutpi0mass;
  Int_t _cutvtx;
  Int_t _cutdstcos;


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


  Int_t   nPi0;
  Float_t Pi0Mass[MAXNCANDS];
  Float_t Pi0p3[MAXNCANDS];    
  Float_t Pi0costh[MAXNCANDS];
  Float_t Pi0phi[MAXNCANDS];   
  Float_t Pi0p3CM[MAXNCANDS];  
  Float_t Pi0costhCM[MAXNCANDS];  
  Float_t Pi0phiCM[MAXNCANDS];  
  Int_t   Pi0Lund[MAXNCANDS];  
  Int_t   Pi0MCIdx[MAXNCANDS];  
  Int_t   Pi0d1Lund[MAXNCANDS];  
  Int_t   Pi0d1Idx[MAXNCANDS];  
  Int_t   Pi0d2Lund[MAXNCANDS];  
  Int_t   Pi0d2Idx[MAXNCANDS];  
  Float_t Pi0Vtxx[MAXNCANDS];  
  Float_t Pi0Vtxy[MAXNCANDS];  
  Float_t Pi0Vtxz[MAXNCANDS];     





  //--------------------------------------
  //////My derived variables 
  //-------------------------------------
  
 

  ////indexes
  Int_t   XIdx;
  Int_t   Pi1Idx;


  ///////Variables to be saved into reduced ntuple
  Int_t eventnumber;//event

  Float_t dstpimass;
  Float_t dstpideltam;
  Float_t dstpimdm;//computed using mass constrained D* and pi0
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
    
  Float_t dstcostheta;

  Float_t pi1p3; 
  Float_t pi1mass;
  Int_t   pi1charge;
  Int_t   pi1mctrue;
  Int_t   pi1lh; 
  Int_t   pi1lhK;  
  Float_t pi1dedx;
  Float_t pi1thetac;
  Float_t pi1cosine;
  Float_t pi1costheta;
  Float_t pi1helicity;//angle of pi0 in D* frame


};
  

#endif

