#include "../Globals.h"
#include "../AnalysisGlobals.h"
#include "../MACROS/DstToD0PiChain.C"
#include "../FUNCTIONS/Fits.h"
#include "../FUNCTIONS/SetHisto.h"
#include <TLine.h>
#include <TText.h>

TH1F* AnalyzeDstToD0Pi(Int_t firstfile, Int_t lastfile,TString DATAorMC,Int_t MatterOrAntiMatter,TString Mode,TString ModeSubDir,Int_t WhichCuts){


  //
//   Int_t mode=0;
//   if(Mode=="D0ToKPi")mode=1;
//   if(Mode=="D0ToKPiPi0")mode=2;
//   if(Mode=="D0ToK3Pi")mode=3;
  
  //
  Bool_t TruthMatch=false;
  if(DATAorMC=="MC")TruthMatch=true;


 
  // Declaration of leave types
  Float_t beamSX;
  Float_t beamSY;
  Float_t beamSZ;

  Int_t   nDstar;
  Float_t DstarMass[500];   //[nDstar]
  Float_t DstarMassErr[500];   //[nDstar]
  Float_t DstarVtxx[500];   //[nDstar]
  Float_t DstarVtxy[500];   //[nDstar]
  Float_t DstarVtxz[500];   //[nDstar]
  Float_t Dstarcosth[500];   //[nDstar]
  Float_t Dstarp3[500];   //[nDstar]
  Float_t DstarcosthCM[500];   //[nDstar]
  Float_t Dstarp3CM[500];   //[nDstar]
  Float_t Dstarphi[500];   //[nDstar]
  Int_t   DstarLund[500];   //[nDstar]
  Int_t   Dstard1Lund[500];   //[nDstar]
  Int_t   Dstard1Idx[500];   //[nDstar]
  Int_t   Dstard2Lund[500];   //[nDstar]
  Int_t   Dstard2Idx[500];   //[nDstar]
  Float_t DstarChi2[500];
  Int_t   DstarnDof[500];
  Int_t   DstarMCIdx[500];
 

  Int_t   nD0;
  Float_t D0Mass[500];   //[nD0]
  Float_t D0MassErr[500];   //[nD0]
  Float_t D0Vtxx[500];   //[nD0]
  Float_t D0Vtxy[500];   //[nD0]
  Float_t D0Vtxz[500];   //[nD0]
  Float_t D0costh[500];   //[nD0]
  Float_t D0p3[500];   //[nD0]
  Float_t D0p3CM[500];   //[nD0]
  Float_t D0phi[500];   //[nD0]
  Int_t   D0Lund[500];   //[nD0]
  Int_t   D0d1Lund[500];   //[nD0]
  Int_t   D0d1Idx[500];   //[nD0]
  Int_t   D0d2Lund[500];   //[nD0]
  Int_t   D0d2Idx[500];   //[nD0] 
  Int_t   D0d3Lund[500];   //[nD0]
  Int_t   D0d3Idx[500];
  Int_t   D0d4Lund[500];   //[nD0]
  Int_t   D0d4Idx[500];
  Float_t D0Chi2[500];
  Int_t   D0nDof[500];
  Int_t   D0MCIdx[500];
 
  Int_t   nPi;
  Float_t Picosth[500];   //[nPi]
  Float_t Pip3[500];   //[nPi]
  Float_t Piphi[500];   //[nPi]
  Int_t   PiLund[500];    //[nPi]
  Float_t PiPoca_x[500];
  Float_t PiPoca_y[500];
  Float_t PiPoca_z[500];
  Int_t   PiMCIdx[500];
  Int_t   PiTrkIdx[500];
  Int_t   PiSelectorsMap[500];
  
  Int_t   nK;
  Float_t Kcosth[500];   //[nK]
  Float_t Kp3[500];   //[nK]
  Float_t Kphi[500];   //[nK]
  Int_t   KLund[500];   //[nK]
  Float_t KPoca_x[500]; 
  Float_t KPoca_y[500];
  Float_t KPoca_z[500];
  Int_t   KMCIdx[500];
  Int_t   KTrkIdx[500];
  Int_t   KSelectorsMap[500];

  Int_t   nPi0;
  Float_t Pi0Mass[500];
  Float_t Pi0costh[500];   //[nPi]
  Float_t Pi0p3[500];   //[nPi]
  Float_t Pi0phi[500];   //[nPi]
  Int_t   Pi0Lund[500];   //[nPi]
  Float_t Pi0Chi2[500];
  Int_t   Pi0nDof[500];
  Int_t   Pi0d1Lund[500];   //[nD0]
  Int_t   Pi0d1Idx[500];   //[nD0]
  Int_t   Pi0d2Lund[500];   //[nD0]
  Int_t   Pi0d2Idx[500];   //[nD0]
  Int_t   Pi0MCIdx[500];

  Int_t   nGamma;
  Float_t Gammacosth[500];   //[nPi]
  Float_t Gammap3[500];   //[nPi]
  Float_t Gammaphi[500];   //[nPi]
  Int_t   GammaLund[500];   //[nPi]
  Float_t GammaLat[500];
  Float_t GammaECal[500];
  Int_t   GammaMCIdx[500];

  Int_t TRKnSvt[500];
  Int_t TRKLund[500];
  

  ////MC block
  Int_t mcLen;
  Int_t mcLund[500];
  Int_t mothIdx[500];
  Int_t dauLen[500];
  Int_t dauIdx[500];
  Float_t mcmass[500]; 
  Float_t mccosth[500];
  Float_t mcp3[500];
  Float_t mccosthCM[500];
  Float_t mcp3CM[500];


  //my derived variables
  Float_t SlowPiRadius;
  Float_t KRadius;
  Float_t PiRadius;
  Float_t D0Radius;
  Float_t DstarRadius;
  Float_t D0Probab;
  Float_t DstarProbab;
  Float_t Pi0Probab=2;

  Int_t MCDstarCounterPerEvent=0;
  Int_t MCDstarCounterTotal=0;
  Int_t RecoDstarCounterTotal=0;

  Int_t   DstarIdx;
  Int_t   D0Idx;
  Int_t   PiIdx;
  Int_t   SlowPiIdx;
  Int_t   KIdx;
  Int_t   Pi0Idx;
  Int_t   GammaIdx;  
  Int_t   KtrkIdx;
  Int_t   PitrkIdx;
  Int_t   SlowPitrkIdx;
  

  TChain* fChain=DstToD0PiChain(firstfile,lastfile, DATAorMC,Mode, ModeSubDir);
  if(fChain==NULL){
    cout<<"No chain."<<endl;
    return NULL;
  }
 
  fChain->SetBranchAddress("beamSX",&beamSX);
  fChain->SetBranchAddress("beamSY",&beamSY);
  fChain->SetBranchAddress("beamSZ",&beamSZ);
  fChain->SetBranchAddress("nDstar",&nDstar);
  fChain->SetBranchAddress("DstarMass",DstarMass);
  fChain->SetBranchAddress("DstarMassErr",DstarMassErr);
  fChain->SetBranchAddress("DstarVtxx",DstarVtxx);
  fChain->SetBranchAddress("DstarVtxy",DstarVtxy);
  fChain->SetBranchAddress("DstarVtxz",DstarVtxz);
  fChain->SetBranchAddress("Dstarcosth",Dstarcosth);  
  fChain->SetBranchAddress("Dstarp3",Dstarp3);
  fChain->SetBranchAddress("DstarcosthCM",DstarcosthCM);  
  fChain->SetBranchAddress("Dstarp3CM",Dstarp3CM);
  fChain->SetBranchAddress("Dstarphi",Dstarphi);
  fChain->SetBranchAddress("DstarLund",DstarLund);
  fChain->SetBranchAddress("Dstard1Lund",Dstard1Lund);
  fChain->SetBranchAddress("Dstard1Idx",Dstard1Idx);
  fChain->SetBranchAddress("Dstard2Lund",Dstard2Lund);
  fChain->SetBranchAddress("Dstard2Idx",Dstard2Idx);
  fChain->SetBranchAddress("DstarChi2",DstarChi2);
  fChain->SetBranchAddress("DstarnDof",DstarnDof);
  fChain->SetBranchAddress("nD0",&nD0);
  fChain->SetBranchAddress("D0Mass",D0Mass);
  fChain->SetBranchAddress("D0MassErr",D0MassErr);
  fChain->SetBranchAddress("D0Vtxx",D0Vtxx);
  fChain->SetBranchAddress("D0Vtxy",D0Vtxy);
  fChain->SetBranchAddress("D0Vtxz",D0Vtxz);
  fChain->SetBranchAddress("D0costh",D0costh);
  fChain->SetBranchAddress("D0p3",D0p3);
  fChain->SetBranchAddress("D0p3CM",D0p3CM);  
  fChain->SetBranchAddress("D0phi",D0phi); 
  fChain->SetBranchAddress("D0Lund",D0Lund);
  fChain->SetBranchAddress("D0d1Lund",D0d1Lund);
  fChain->SetBranchAddress("D0d1Idx",D0d1Idx);
  fChain->SetBranchAddress("D0d2Lund",D0d2Lund);
  fChain->SetBranchAddress("D0d2Idx",D0d2Idx);
  fChain->SetBranchAddress("D0Chi2",D0Chi2);
  fChain->SetBranchAddress("D0nDof",D0nDof);
  fChain->SetBranchAddress("nPi",&nPi);
  fChain->SetBranchAddress("Picosth",Picosth);
  fChain->SetBranchAddress("Pip3",Pip3);
  fChain->SetBranchAddress("Piphi",Piphi);
  fChain->SetBranchAddress("PiLund",PiLund);
  fChain->SetBranchAddress("PiPoca_x",PiPoca_x);
  fChain->SetBranchAddress("PiPoca_y",PiPoca_y);
  fChain->SetBranchAddress("PiPoca_z",PiPoca_z);
  fChain->SetBranchAddress("PiTrkIdx",PiTrkIdx);
  fChain->SetBranchAddress("nK",&nK);
  fChain->SetBranchAddress("Kcosth",Kcosth);
  fChain->SetBranchAddress("Kp3",Kp3);
  fChain->SetBranchAddress("Kphi",Kphi);
  fChain->SetBranchAddress("KLund",KLund);
  fChain->SetBranchAddress("KPoca_x",KPoca_x);
  fChain->SetBranchAddress("KPoca_y",KPoca_y);
  fChain->SetBranchAddress("KPoca_z",KPoca_z);
  fChain->SetBranchAddress("KTrkIdx",KTrkIdx);
  fChain->SetBranchAddress("TRKnSvt",TRKnSvt);
  fChain->SetBranchAddress("TRKLund",TRKLund);
  fChain->SetBranchAddress("KSelectorsMap",KSelectorsMap);
  fChain->SetBranchAddress("piSelectorsMap",PiSelectorsMap);

 
  if(Mode=="D0ToKPiPi0"){
    fChain->SetBranchAddress("D0d3Lund",D0d3Lund);
    fChain->SetBranchAddress("D0d3Idx",D0d3Idx);
    fChain->SetBranchAddress("nPi0",&nPi0);
    fChain->SetBranchAddress("Pi0Mass",Pi0Mass);
    fChain->SetBranchAddress("Pi0costh",Pi0costh);
    fChain->SetBranchAddress("Pi0p3",Pi0p3);
    fChain->SetBranchAddress("Pi0phi",Pi0phi);
    fChain->SetBranchAddress("Pi0Lund",Pi0Lund);
    fChain->SetBranchAddress("Pi0Chi2",Pi0Chi2);
    fChain->SetBranchAddress("Pi0nDof",Pi0nDof);
    fChain->SetBranchAddress("Pi0d1Lund",Pi0d1Lund);
    fChain->SetBranchAddress("Pi0d1Idx",Pi0d1Idx);
    fChain->SetBranchAddress("Pi0d2Lund",Pi0d2Lund);
    fChain->SetBranchAddress("Pi0d2Idx",Pi0d2Idx);
    fChain->SetBranchAddress("nGamma",&nGamma);
    fChain->SetBranchAddress("Gammacosth",Gammacosth);
    fChain->SetBranchAddress("Gammap3",Gammap3);
    fChain->SetBranchAddress("Gammaphi",Gammaphi);
    fChain->SetBranchAddress("GammaLund",GammaLund);
    fChain->SetBranchAddress("GammaLat",GammaLat);
    fChain->SetBranchAddress("GammaECal",GammaECal);

    if(TruthMatch){
      fChain->SetBranchAddress("Pi0MCIdx",Pi0MCIdx);
      fChain->SetBranchAddress("GammaMCIdx",GammaMCIdx);
    }

  }
  if(Mode=="D0ToK3Pi"){
    fChain->SetBranchAddress("D0d3Lund",D0d3Lund);
    fChain->SetBranchAddress("D0d3Idx",D0d3Idx);
    fChain->SetBranchAddress("D0d4Lund",D0d4Lund);
    fChain->SetBranchAddress("D0d4Idx",D0d4Idx);
  }

  if(TruthMatch){
    fChain->SetBranchAddress("DstarMCIdx",DstarMCIdx);
    fChain->SetBranchAddress("D0MCIdx",D0MCIdx);
    fChain->SetBranchAddress("KMCIdx",KMCIdx);
    fChain->SetBranchAddress("PiMCIdx",PiMCIdx);
    fChain->SetBranchAddress("mcLund",mcLund);
    fChain->SetBranchAddress("mcLen",&mcLen);
    fChain->SetBranchAddress("mcmass",mcmass);
    fChain->SetBranchAddress("mcp3",mcp3);
    fChain->SetBranchAddress("mccosth",mccosth);
    fChain->SetBranchAddress("mcp3CM",mcp3CM);
    fChain->SetBranchAddress("mccosthCM",mccosthCM); 
  }
  

  //Histosgrams
  Float_t DstarMassWindow=.090;  
  Float_t D0MassWindow=.090;
  Float_t PDGMassDiff=DstarPDGMass-D0PDGMass;
  Int_t NMassBins=182;

  ///////////////////List of Cuts  
  ////Pre-Cuts
  //SlowPion,Pion,Kaon: RDocaMax=.5;ZDocaMax=4;
  //Pion,Kaon:LHTight
  //D0Masscut:1.7,2
  //SlowPi svt,dch,drc consis:>1e-15
  //DstarMassCut:2.01-.5,2.01+.5
  //DeltaM:.130,.160
  //Pi0MassCut:.090,.165

  //////post-Cuts
  ///Loose
  Float_t DeltaMassCut=0.;
  Float_t DeltaMassCutLo=0.;
  Float_t DeltaMassCutHi=0.;
  Float_t D0p3CMCut=0.;
  Float_t D0ProbabCut=0.; 
  ///Define Signal Region
  Float_t DstarMassCutLo=0.;
  Float_t DstarMassCutHi=0.;
  Float_t DstarMassResolution=0.;
  //////////////
  

  if(Mode=="D0ToKPi"){
    //loose
    if(WhichCuts==1){      
      DeltaMassCutLo=.139;
      DeltaMassCutHi=.16;
      D0p3CMCut=2.4;  
      D0ProbabCut=5e-3; 
    }
    //tight
    if(WhichCuts==2){      
      DeltaMassCutLo=.139;
      DeltaMassCutHi=.16;
      D0p3CMCut=2.4;  
      D0ProbabCut=10e-3; 
    }
    DstarMassResolution=.007;
    DstarMassCutLo=DstarPDGMass-5*DstarMassResolution;
    DstarMassCutHi=DstarPDGMass+5*DstarMassResolution;
  }
  if(Mode=="D0ToKPiPi0"){
    //loose
    if(WhichCuts==1){
      DeltaMassCut=.0018;
      DeltaMassCutLo=PDGMassDiff-DeltaMassCut;
      DeltaMassCutHi=PDGMassDiff+DeltaMassCut;
      D0p3CMCut=2.2;
      D0ProbabCut=5e-3;
    }
    /////Tight
    if(WhichCuts==2){
      DeltaMassCut=.001;
      DeltaMassCutLo=PDGMassDiff-DeltaMassCut;
      DeltaMassCutHi=PDGMassDiff+DeltaMassCut;
      D0p3CMCut=2.2;
      D0ProbabCut=15e-3;
    }
    DstarMassResolution=.015;
    DstarMassCutLo=DstarPDGMass-5*DstarMassResolution;
    DstarMassCutHi=DstarPDGMass+5*DstarMassResolution;
  }
  if(Mode=="D0ToK3Pi"){
    //loose
    if(WhichCuts==1){
      DeltaMassCut=.0015;
      DeltaMassCutLo=PDGMassDiff-DeltaMassCut;
      DeltaMassCutHi=PDGMassDiff+DeltaMassCut;
      D0p3CMCut=2.2;
      D0ProbabCut=5e-3;
    }
    /////Tight
    if(WhichCuts==2){
      DeltaMassCut=.0009;
      DeltaMassCutLo=PDGMassDiff-DeltaMassCut;
      DeltaMassCutHi=PDGMassDiff+DeltaMassCut;
      D0p3CMCut=2.2;   
      D0ProbabCut=15e-3;
    }
    DstarMassResolution=.005;
    DstarMassCutLo=DstarPDGMass-5*DstarMassResolution;
    DstarMassCutHi=DstarPDGMass+5*DstarMassResolution;

  }
  

  //////////////  
  //BeamPlots  
  TH1F HBeamRadius;
  SetHistoXY(&HBeamRadius,"Beam R",100,0,.5,"R (cm)","Entries/50#mu m");
  
  TH1F HBeamZ;
  SetHistoXY(&HBeamZ,"Beam Z",100,-4.,4.,"Z (cm)","Entries/1mm");
   
  ////Kaon
  TH1F HKMomentum;
  SetHistoXY(&HKMomentum,"K Momentum ",80,0,8,"p (GeV/c)","Entries/100MeV");

  TH1F HKMomentumTruthMatched;
  SetHistoXY(&HKMomentumTruthMatched,"K Momentum ",80,0,8,"p (GeV/c)","Entries/100MeV");
   
  TH1F HKRadius;
  SetHistoXY(&HKRadius,"K Radius",100,0,.5,"Radius (cm)","Entries/50#mu m");
  
  TH1F HKZ;
  SetHistoXY(&HKZ,"K Z",100,-4.,4.,"Z (cm)","Entries/1mm");
  
  /////pion 
  TH1F HPiMomentum;
  SetHistoXY(&HPiMomentum," Pion Momentum ",80,0,8,"p (GeV/c)","Entries/100MeV");

  TH1F HPiMomentumTruthMatched;
  SetHistoXY(&HPiMomentumTruthMatched," Pion Momentum ",80,0,8,"p (GeV/c)","Entries/100MeV");

  TH1F HPiRadius;
  SetHistoXY(&HPiRadius,"Pion Radius",100,0,.5,"Radius (cm)","Entries/50#mu m");
  
  TH1F HPiZ;
  SetHistoXY(&HPiZ,"Pion Z",100,-4.,4.,"Z (cm)","Entries/1mm");
   
  ///////////Mode dependent histos
  ////////////Mode=="D0ToKPiPi0"
  ////Gamma
  TH1F HGammaMomentum;
  SetHistoXY(&HGammaMomentum,"Gamma Momentum",30,.1,1.2," p (GeV/c)","Entries/40MeV");
 
  TH1F HGammaEnergy;
  SetHistoXY(&HGammaEnergy,"Gamma Energy",30,0,1.2,"calor. energy (GeV/c^{2})","Entries/40MeV");

  TH1F HGammaEnergyTruthMatched;
  SetHistoXY(&HGammaEnergyTruthMatched,"Gamma Energy",30,0,1.2,"calor. energy (GeV/c^{2})","Entries/40MeV");
  
  TH1F HGammaLat;
  SetHistoXY(&HGammaLat,"Gamma Lateral Moment",100,0,1,"moment","Entries/.01");

  ////Pi0
  TH1F HPi0MassPreCuts;
  SetHistoXY(&HPi0MassPreCuts,"Pi0 Mass Before Cuts",120,.075,.195," mass (GeV/c^{2})","Entries/1MeV");

  TH1F HPi0Mass;
  SetHistoXY(&HPi0Mass,"Pi0 Mass After Cuts ",120,.075,.195," mass (GeV/c^{2})","Entries/1MeV");

  TH1F HPi0MassTruthMatched;
  SetHistoXY(&HPi0MassTruthMatched,"Pi0 Mass After Cuts ",120,.075,.195," mass (GeV/c^{2})","Entries/1MeV");

  TH1F HPi0Momentum;
  SetHistoXY(&HPi0Momentum,"Pi0 Momentum",80,0,8,"p (GeV/c)","Entries/100MeV");

  TH1F HPi0VtxProbab;
  SetHisto(&HPi0VtxProbab,"Pi0 Vtx Probab ",200,-8,0,"log(p(#chi^{2}))");
  
  /////D0 
  TH1F HD0VtxR;
  SetHistoXY(&HD0VtxR,"D0 Radius",100,0,.5,"Radius (cm)","Entries/50#mu m");
  
  TH1F HD0VtxZ;
  SetHistoXY(&HD0VtxZ,"D0 Z",100,-4.,4.,"Z (cm)","Entries/1mm");

  TH1F HD0p3;
  SetHistoXY(&HD0p3,"D0 Momentum ",80,0,8,"p (GeV/c)","Entries/100MeV");

  TH1F HD0p3CM;
  SetHistoXY(&HD0p3CM,"D0 CM Momentum ",80,0,8,"p* (GeV/c)","Entries/100MeV");

  TH1F HD0MassPreCuts;
  SetHistoXY(&HD0MassPreCuts,"D0 Mass Before Cuts",NMassBins,D0PDGMass-D0MassWindow-.001,D0PDGMass+D0MassWindow+.001,"D0 Cand. Mass (GeV/c^{2})","Entries/1MeV");

  TH1F HD0Mass;
  SetHistoXY(&HD0Mass,"D0 Mass After Cuts ",NMassBins,D0PDGMass-D0MassWindow-.001,D0PDGMass+D0MassWindow+.001,"D0 Cand. Mass (GeV/c^{2})","Entries/1MeV");

  TH1F HD0MassTruthMatched;
  SetHistoXY(&HD0MassTruthMatched,"D0 Mass After Cuts ",NMassBins,D0PDGMass-D0MassWindow-.001,D0PDGMass+D0MassWindow+.001,"D0 Cand. Mass (GeV/c^{2})","Entries/1MeV");

  TH1F HD0Probab;
  SetHisto(&HD0Probab,"D0 Vtx Probab ",100,-6,0,"log(p(#chi^{2}))");

  //slow pion
  TH1F HSlowPiMomentum;
  SetHistoXY(&HSlowPiMomentum,"Slow Pion Momentum ",30,0,1.2,"p (GeV/c)","Entries/40MeV");

  TH1F HSlowPiMomentumTruthMatched;
  SetHistoXY(&HSlowPiMomentumTruthMatched,"Slow Pion Momentum ",30,0,1.2,"p (GeV/c)","Entries/40MeV");

  TH1F HSlowPiRadius;
  SetHistoXY(&HSlowPiRadius,"SlowPion Radius",400,0,2,"Radius (cm)","Entries/50#mu m");
 
  TH1F HSlowPiZ;
  SetHistoXY(&HSlowPiZ,"SlowPion Z",80,-4.,4.,"Z (cm)","Entries/1mm");
 
  TH1F HSlowPinSvt;
  SetHistoXY(&HSlowPinSvt,"SlowPion nSvt hits",20,-.5,19.5,"n Svt hits","Entries");  

  ////Dstar
  TH1F HDstarRadius;
  SetHistoXY(&HDstarRadius,"D* Radius",100,0,.5,"Radius (cm)","Entries/50#mu m");
  
  TH1F HDstarZ;
  SetHistoXY(&HDstarZ,"D* Z",100,-4.,4.,"Z (cm)","Entries/1mm");
  
  Float_t Dstarp3CMmax=8;
  Float_t Dstarp3CMmin=0;
  Int_t Dstarp3CMNbins=80;
  if(TruthMatch){
     Dstarp3CMmax=5;
     Dstarp3CMmin=0;
     Dstarp3CMNbins=50;
  }
  
  TH1F HDstarp3CMPreCuts;
  SetHistoXY(&HDstarp3CMPreCuts,"D* CM Momentum ",Dstarp3CMNbins,Dstarp3CMmin,Dstarp3CMmax,"p* (GeV/c)","Entries/100MeV");

  TH1F HDstarp3CM;
  SetHistoXY(&HDstarp3CM,"D* CM Momentum ",Dstarp3CMNbins,Dstarp3CMmin,Dstarp3CMmax,"p* (GeV/c)","Entries/100MeV");

  TH1F HDstarp3CMPIDCut;
  SetHistoXY(&HDstarp3CMPIDCut,"D* CM Momentum ",Dstarp3CMNbins,Dstarp3CMmin,Dstarp3CMmax,"p* (GeV/c)","Entries/100MeV");
  HDstarp3CMPIDCut.SetLineColor(2);

  TH1F HDstarp3CMPIDCutD0PCut;
  SetHistoXY(&HDstarp3CMPIDCutD0PCut,"D* CM Momentum ",Dstarp3CMNbins,Dstarp3CMmin,Dstarp3CMmax,"p* (GeV/c)","Entries/100MeV");
  HDstarp3CMPIDCutD0PCut.SetLineColor(3);

  TH1F HDstarp3CMPIDCutD0PCutDeltaMCut;
  SetHistoXY(&HDstarp3CMPIDCutD0PCutDeltaMCut,"D* CM Momentum ",Dstarp3CMNbins,Dstarp3CMmin,Dstarp3CMmax,"p* (GeV/c)","Entries/100MeV");
  HDstarp3CMPIDCutD0PCutDeltaMCut.SetLineColor(4);

  TH1F HDstarp3CMPIDCutD0PCutDeltaMCutD0Probab;
  SetHistoXY(&HDstarp3CMPIDCutD0PCutDeltaMCutD0Probab,"D* CM Momentum ",Dstarp3CMNbins,Dstarp3CMmin,Dstarp3CMmax,"p* (GeV/c)","Entries/100MeV");
  HDstarp3CMPIDCutD0PCutDeltaMCutD0Probab.SetLineColor(6);    


  TH1F HDstarcosthCMPreCuts;
  SetHistoXY(&HDstarcosthCMPreCuts,"D* Angular Distribution  ",100,-1,1,"cos(#theta)*","Entries/.02");

  TH1F HDstarcosthCM;
  SetHistoXY(&HDstarcosthCM,"D* Angular Distribution  ",100,-1,1,"cos(#theta)*","Entries/.02");

  TH1F HDstarcosthCMPIDCut;
  SetHistoXY(&HDstarcosthCMPIDCut,"D* Angular Distribution  ",100,-1,1,"cos(#theta)*","Entries/.02");
  HDstarcosthCMPIDCut.SetLineColor(2);

  TH1F HDstarcosthCMPIDCutD0PCut;
  SetHistoXY(&HDstarcosthCMPIDCutD0PCut,"D* Angular Distribution  ",100,-1,1,"cos(#theta)*","Entries/.02");
  HDstarcosthCMPIDCutD0PCut.SetLineColor(3);

  TH1F HDstarcosthCMPIDCutD0PCutDeltaMCut;
  SetHistoXY(&HDstarcosthCMPIDCutD0PCutDeltaMCut,"D* Angular Distribution  ",100,-1,1,"cos(#theta)*","Entries/.02");
  HDstarcosthCMPIDCutD0PCutDeltaMCut.SetLineColor(4);

  TH1F HDstarcosthCMPIDCutD0PCutDeltaMCutD0Probab;
  SetHistoXY(&HDstarcosthCMPIDCutD0PCutDeltaMCutD0Probab,"D* Angular Distribution  ",100,-1,1,"cos(#theta)*","Entries/.02");
  HDstarcosthCMPIDCutD0PCutDeltaMCutD0Probab.SetLineColor(6);
    
  TH2F H2DstarCMPvsTheta;
  SetHisto2D(&H2DstarCMPvsTheta," #theta* vs  p* ",50,0,5,"p* (GeV/c)",100,-1,1,"cos(#theta)*","entries/(.01x100MeV)");

  TH1F HDstarMassPreCuts;
  SetHistoXY(&HDstarMassPreCuts,"D* Mass Before Cuts",NMassBins,DstarPDGMass-DstarMassWindow-.001,DstarPDGMass+DstarMassWindow+.001,"D* Cand. Mass (GeV/c^{2})","Entries/1MeV");
  HDstarMassPreCuts.SetLineColor(1);

  TH1F HDstarMassPIDCut;
  SetHistoXY(&HDstarMassPIDCut,"D* Mass PID Cuts",NMassBins,DstarPDGMass-DstarMassWindow-.001,DstarPDGMass+DstarMassWindow+.001,"D* Cand. Mass (GeV/c^{2})","Entries/1MeV");
  HDstarMassPIDCut.SetLineColor(2);

  TH1F HDstarMassD0PCut;
  SetHistoXY(&HDstarMassD0PCut,"D* Mass D0 p* Cuts",NMassBins,DstarPDGMass-DstarMassWindow-.001,DstarPDGMass+DstarMassWindow+.001,"D* Cand. Mass (GeV/c^{2})","Entries/1MeV");
  HDstarMassD0PCut.SetLineColor(3);

  TH1F HDstarMassD0PCutDeltaMCut;
  SetHistoXY(&HDstarMassD0PCutDeltaMCut,"D* Mass DeltaM Cuts",NMassBins,DstarPDGMass-DstarMassWindow-.001,DstarPDGMass+DstarMassWindow+.001,"D* Cand. Mass (GeV/c^{2})","Entries/1MeV");
  HDstarMassD0PCutDeltaMCut.SetLineColor(4);
  
  TH1F HDstarMassFinal;
  SetHistoXY(&HDstarMassFinal,"D* Mass After Cuts",NMassBins,DstarPDGMass-DstarMassWindow-.001,DstarPDGMass+DstarMassWindow+.001,"D* Cand. Mass (GeV/c^{2})","Entries/1MeV");
  HDstarMassFinal.SetLineColor(1);
  
  TH1F HDstarMassSignal;
  SetHistoXY(&HDstarMassSignal,"D* Mass After Cuts",NMassBins,DstarPDGMass-DstarMassWindow-.001,DstarPDGMass+DstarMassWindow+.001,"D* Cand. Mass (GeV/c^{2})","Entries/1MeV");
  HDstarMassSignal.SetLineColor(1);

  TH1F HDstarMassTruthMatched;
  SetHistoXY(&HDstarMassTruthMatched,"D* Mass After Cuts and Truth Matched",NMassBins,DstarPDGMass-DstarMassWindow-.001,DstarPDGMass+DstarMassWindow+.001,"D* Cand. Mass (GeV/c^{2})","Entries/1MeV");
  HDstarMassTruthMatched.SetLineColor(1);
  
  TH1F HDstarMassBkgTotal;
  SetHistoXY(&HDstarMassBkgTotal,"Total Bkg Removed",NMassBins,DstarPDGMass-DstarMassWindow-.001,DstarPDGMass+DstarMassWindow+.001,"D* Cand. Mass (GeV/c^{2})","Entries/1MeV");
  HDstarMassBkgTotal.SetLineColor(1);

  TH1F HDstarMassBkgPID;
  SetHistoXY(&HDstarMassBkgPID,"D* Mass BkgD0p3CM",NMassBins,DstarPDGMass-DstarMassWindow-.001,DstarPDGMass+DstarMassWindow+.001,"D* Cand. Mass (GeV/c^{2})","Entries/1MeV");
  HDstarMassBkgPID.SetLineColor(2);

  TH1F HDstarMassBkgD0p3CM;
  SetHistoXY(&HDstarMassBkgD0p3CM,"D* Mass BkgD0p3CM",NMassBins,DstarPDGMass-DstarMassWindow-.001,DstarPDGMass+DstarMassWindow+.001,"D* Cand. Mass (GeV/c^{2})","Entries/1MeV");
  HDstarMassBkgD0p3CM.SetLineColor(3);

  TH1F HDstarMassBkgD0p3CMDeltaM;
  SetHistoXY(&HDstarMassBkgD0p3CMDeltaM,"D* Mass BkgMassDiff ",NMassBins,DstarPDGMass-DstarMassWindow-.001,DstarPDGMass+DstarMassWindow+.001,"D* Cand. Mass (GeV/c^{2})","Entries/1MeV");
  HDstarMassBkgD0p3CMDeltaM.SetLineColor(4);
  
  TH1F HDstarMassBkgD0p3CMDeltaMD0Probab;
  SetHistoXY(&HDstarMassBkgD0p3CMDeltaMD0Probab,"D* Mass BkgDstarProbab",NMassBins,DstarPDGMass-DstarMassWindow-.001,DstarPDGMass+DstarMassWindow+.001,"D* Cand. Mass (GeV/c^{2})","Entries/1MeV");
  HDstarMassBkgD0p3CMDeltaMD0Probab.SetLineColor(6);
 
  TH1F HMassDiffPreCuts;
  SetHistoXY(&HMassDiffPreCuts,"#Delta M Before Cuts",300,.139,.160,"D* Cand. Mass - D0 Cand. Mass (GeV/c^{2})","Entries/.1MeV");  

  TH1F HMassDiff;
  SetHistoXY(&HMassDiff,"#Delta M After Cuts",300,.139,.160,"D* Cand. Mass - D0 Cand. Mass (GeV/c^{2})","Entries/.1MeV");  

  TH1F HDstarProbab;
  SetHisto(&HDstarProbab,"D* Vtx Probab ",200,-8,0,"log(p(#chi^{2}))");
  

  /////////MC 
  TH1F HMCDstarMass;
  SetHistoXY(&HMCDstarMass,"MC Generated D* Mass",NMassBins,DstarPDGMass-DstarMassWindow-.001,DstarPDGMass+DstarMassWindow+.001,"D* Cand. Mass (GeV/c^{2})","Entries/1MeV");

  TH1F HMCDstarp3CM;
  SetHistoXY(&HMCDstarp3CM,"MC D* Momentum ",Dstarp3CMNbins,Dstarp3CMmin,Dstarp3CMmax,"p* (GeV/c)","Entries/100MeV");

  TH1F HMCDstarcosthCM;
  SetHistoXY(&HMCDstarcosthCM,"MC D* Angular Distribution ",100,-1,1,"cos(#theta)*","Entries/.02");
  
  TH2F H2MCDstarCMPvsTheta;
  SetHisto2D(&H2MCDstarCMPvsTheta,"MC  #theta* vs p* ",Dstarp3CMNbins,Dstarp3CMmin,Dstarp3CMmax,"p* (GeV/c)",100,-1,1,"cos(#theta)*","entries/(.02x100MeV)");

  TH1F HMCNDstar;
  SetHistoXY(&HMCNDstar,"MC Number of D* Generated",10,-.5,10.5,"nDstar/event","Counts");
 
  //check particle id 
  TH1F HLundCheck;SetHisto(&HLundCheck,"Lund Id Check",10,-.5,9.5,"ParticleLund - LundExp");

 
  ////////+++++++++Important
  // Dstard1=D0,Dstard2=Pi;D0d1=K;D0d2=Pi
  //Lunds:
//   Int_t myDstarLund=413;
//   Int_t myD0Lund=421;
//   Int_t myKLund=321;
//   Int_t myPiLund=211;
//   Int_t myPi0Lund=111;
//   Int_t myGammaLund=22;
    
  Int_t test;
  //Start the event loop;
  Int_t eventid=0;
  while(fChain->GetEntry(eventid,0)>0){   
    eventid++;
    if(eventid%5000==0)cout<<eventid<<" Events done."<<endl;
          
    HBeamRadius.Fill(sqrt( beamSX*beamSX + beamSY*beamSY));
    HBeamZ.Fill(beamSZ);     
         
    ///Loop over the reconstructed
    Int_t DstarIdx=-1;
    while(DstarIdx<nDstar-1){
      DstarIdx++;
 
      D0Idx=Dstard1Idx[DstarIdx];
      SlowPiIdx=Dstard2Idx[DstarIdx];
      PiIdx=D0d2Idx[D0Idx];
      KIdx=D0d1Idx[D0Idx];
      Pi0Idx=D0d3Idx[D0Idx];
      GammaIdx=Pi0d1Idx[Pi0Idx];
      KtrkIdx=KTrkIdx[KIdx];
      PitrkIdx=PiTrkIdx[PiIdx];
      SlowPitrkIdx=PiTrkIdx[SlowPiIdx];

      if(!(DstarLund[DstarIdx]==MatterOrAntiMatter*myDstarLund || MatterOrAntiMatter==0))continue;

      //Check that Im using proper indexes
      if(Mode=="D0ToKPi"){
	if(abs(Dstard1Lund[DstarIdx]-D0Lund[D0Idx])>0)HLundCheck.Fill(1);
	else if(abs(Dstard2Lund[DstarIdx]-PiLund[SlowPiIdx])>0)HLundCheck.Fill(2);
	else if(abs(D0d1Lund[D0Idx]-KLund[KIdx])>0)HLundCheck.Fill(3);
	else if(abs(D0d2Lund[D0Idx]-PiLund[PiIdx])>0)HLundCheck.Fill(4);
	else if(abs(TRKLund[SlowPitrkIdx]-PiLund[SlowPiIdx])>0)HLundCheck.Fill(5);
	else if(abs(TRKLund[PitrkIdx]-PiLund[PiIdx])>0)HLundCheck.Fill(6);
	else if(abs(TRKLund[KtrkIdx]-KLund[KIdx])>0)HLundCheck.Fill(7);
	else HLundCheck.Fill(0);
      }      
      if(Mode=="D0ToKPiPi0"){
        if(abs(Dstard1Lund[DstarIdx]-D0Lund[D0Idx])>0)HLundCheck.Fill(1);
	else if(abs(Dstard2Lund[DstarIdx]-PiLund[SlowPiIdx])>0)HLundCheck.Fill(2);
	else if(abs(D0d1Lund[D0Idx]-KLund[KIdx])>0)HLundCheck.Fill(3);
	else if(abs(D0d2Lund[D0Idx]-PiLund[PiIdx])>0)HLundCheck.Fill(4);
	else if(abs(D0d3Lund[D0Idx]-Pi0Lund[Pi0Idx])>0)HLundCheck.Fill(5);
	else if(abs(Pi0d1Lund[Pi0Idx]-GammaLund[GammaIdx])>0)HLundCheck.Fill(6);
	else if(abs(TRKLund[SlowPitrkIdx]-PiLund[SlowPiIdx])>0)HLundCheck.Fill(7);
	else if(abs(TRKLund[PitrkIdx]-PiLund[PiIdx])>0)HLundCheck.Fill(8);
	else if(abs(TRKLund[KtrkIdx]-KLund[KIdx])>0)HLundCheck.Fill(9);
	else HLundCheck.Fill(0);
      }
      if(Mode=="D0ToK3Pi"){
	if(abs(Dstard1Lund[DstarIdx]-D0Lund[D0Idx])>0)HLundCheck.Fill(1);
	else if(abs(Dstard2Lund[DstarIdx]-PiLund[SlowPiIdx])>0)HLundCheck.Fill(2);
	else if(abs(D0d1Lund[D0Idx]-KLund[KIdx])>0)HLundCheck.Fill(3);
	else if(abs(D0d2Lund[D0Idx]-PiLund[PiIdx])>0)HLundCheck.Fill(4);
	else if(abs(D0d3Lund[D0Idx]-PiLund[Pi0Idx])>0)HLundCheck.Fill(5);
	else if(abs(D0d4Lund[D0Idx]-PiLund[D0d4Idx[D0Idx]])>0)HLundCheck.Fill(6);
	else if(abs(TRKLund[SlowPitrkIdx]-PiLund[SlowPiIdx])>0)HLundCheck.Fill(7);
	else if(abs(TRKLund[PitrkIdx]-PiLund[PiIdx])>0)HLundCheck.Fill(8);
	else if(abs(TRKLund[KtrkIdx]-KLund[KIdx])>0)HLundCheck.Fill(9);
	else HLundCheck.Fill(0);
      }


      //Pocas are supposed to be referenced to the beamspot but I find they are not      
      KPoca_x[KIdx]-=beamSX;
      PiPoca_x[PiIdx]-=beamSX;
      D0Vtxx[D0Idx]-=beamSX;
      PiPoca_x[SlowPiIdx]-=beamSX; 
      DstarVtxx[DstarIdx]-=beamSX;

      KPoca_y[KIdx]-=beamSY;
      PiPoca_y[PiIdx]-=beamSY;
      D0Vtxy[D0Idx]-=beamSY;
      PiPoca_y[SlowPiIdx]-=beamSY; 
      DstarVtxy[DstarIdx]-=beamSY;

      KPoca_z[KIdx]-=beamSZ;
      PiPoca_z[PiIdx]-=beamSZ;
      D0Vtxz[D0Idx]-=beamSZ;
      PiPoca_z[SlowPiIdx]-=beamSZ; 
      DstarVtxz[DstarIdx]-=beamSZ;
      

      KRadius=sqrt( KPoca_x[KIdx]*KPoca_x[KIdx] +
		    KPoca_y[KIdx]*KPoca_y[KIdx]);
      PiRadius=sqrt( PiPoca_x[PiIdx]*PiPoca_x[PiIdx] +
		     PiPoca_y[PiIdx]*PiPoca_y[PiIdx]);
      D0Radius=sqrt( D0Vtxx[D0Idx]*D0Vtxx[D0Idx] +
		     D0Vtxy[D0Idx]*D0Vtxy[D0Idx]);
      SlowPiRadius=sqrt( PiPoca_x[SlowPiIdx]*PiPoca_x[SlowPiIdx] +
			 PiPoca_y[SlowPiIdx]*PiPoca_y[SlowPiIdx]); 
      DstarRadius=sqrt( DstarVtxx[DstarIdx]*DstarVtxx[DstarIdx] + DstarVtxy[DstarIdx]*DstarVtxy[DstarIdx]);

    
      ///////Vertex Chisquared Probabilities
      DstarProbab=TMath::Prob(DstarChi2[DstarIdx],DstarnDof[DstarIdx]);
      D0Probab=TMath::Prob(D0Chi2[D0Idx],D0nDof[D0Idx]);          
      if(Mode=="D0ToKPiPi0")Pi0Probab=TMath::Prob(Pi0Chi2[Pi0Idx],Pi0nDof[Pi0Idx]);  


      ///Fill Distributions  
      if(Mode=="D0ToKPiPi0"){        
	HPi0MassPreCuts.Fill(Pi0Mass[Pi0Idx]);  
      }
      
      HD0p3CM.Fill(D0p3CM[D0Idx]);
      HD0Probab.Fill(log10(D0Probab));      
      HD0MassPreCuts.Fill(D0Mass[D0Idx]);
  
      HDstarMassPreCuts.Fill(DstarMass[DstarIdx]);   
      HMassDiffPreCuts.Fill(DstarMass[DstarIdx]-D0Mass[D0Idx]);   
      if( DstarMassCutLo<DstarMass[DstarIdx]&&DstarMass[DstarIdx]<DstarMassCutHi&& DstarMCIdx[DstarIdx]>0){
	HDstarcosthCMPreCuts.Fill(DstarcosthCM[DstarIdx]);
	HDstarp3CMPreCuts.Fill(Dstarp3CM[DstarIdx]);          
      }

      

      //Apply Cuts       
      if((KSelectorsMap[KtrkIdx] & (1<<13)) != 0  && (PiSelectorsMap[PitrkIdx] & (1<<4) ) != 0){
	HDstarMassPIDCut.Fill(DstarMass[DstarIdx]); 
	if(DstarMassCutLo<DstarMass[DstarIdx]&&DstarMass[DstarIdx]<DstarMassCutHi && DstarMCIdx[DstarIdx]>0){	  
	  HDstarcosthCMPIDCut.Fill(DstarcosthCM[DstarIdx]);
	  HDstarp3CMPIDCut.Fill(Dstarp3CM[DstarIdx]);
	}
	if(D0p3CM[D0Idx] > D0p3CMCut){
	  HDstarMassD0PCut.Fill(DstarMass[DstarIdx]); 
	  if(DstarMassCutLo<DstarMass[DstarIdx]&&DstarMass[DstarIdx]<DstarMassCutHi&& DstarMCIdx[DstarIdx]>0){
	    HDstarcosthCMPIDCutD0PCut.Fill(DstarcosthCM[DstarIdx]);
	    HDstarp3CMPIDCutD0PCut.Fill(Dstarp3CM[DstarIdx]);
	  }
	  if(DeltaMassCutLo< DstarMass[DstarIdx] - D0Mass[D0Idx] && DstarMass[DstarIdx]-D0Mass[D0Idx]<DeltaMassCutHi){
	    HDstarMassD0PCutDeltaMCut.Fill(DstarMass[DstarIdx]);  
	    if(DstarMassCutLo<DstarMass[DstarIdx]&&DstarMass[DstarIdx]<DstarMassCutHi&& DstarMCIdx[DstarIdx]>0){
	      HDstarcosthCMPIDCutD0PCutDeltaMCut.Fill(DstarcosthCM[DstarIdx]);
	      HDstarp3CMPIDCutD0PCutDeltaMCut.Fill(Dstarp3CM[DstarIdx]);
	    }
	    if(D0Probab > D0ProbabCut){	            
	      HDstarMassFinal.Fill(DstarMass[DstarIdx]);           
	      if(DstarMassCutLo<DstarMass[DstarIdx]&&DstarMass[DstarIdx]<DstarMassCutHi&& DstarMCIdx[DstarIdx]>0){
		HDstarcosthCMPIDCutD0PCutDeltaMCutD0Probab.Fill(DstarcosthCM[DstarIdx]);
		HDstarp3CMPIDCutD0PCutDeltaMCutD0Probab.Fill(Dstarp3CM[DstarIdx]);
	      }

	      ///Fill Histograms only for the signal regin in Dstar mass	   
	      if(DstarMassCutLo<DstarMass[DstarIdx]&&DstarMass[DstarIdx]<DstarMassCutHi){

		if(Mode=="D0ToKPiPi0"){ 
		  HGammaEnergy.Fill(GammaECal[GammaIdx]);
		  if(GammaMCIdx[GammaIdx]>0)
		    HGammaEnergyTruthMatched.Fill(GammaECal[GammaIdx]);
		  HGammaLat.Fill(GammaLat[GammaIdx]);  
		  HGammaMomentum.Fill(Gammap3[GammaIdx]);

		  HPi0Mass.Fill(Pi0Mass[Pi0Idx]);
		  if(Pi0MCIdx[Pi0Idx]>0)
		    HPi0MassTruthMatched.Fill(Pi0Mass[Pi0Idx]);
		  HPi0Momentum.Fill(Pi0p3[Pi0Idx]);
		  HPi0VtxProbab.Fill(log10(Pi0Probab));
		}

		HKMomentum.Fill(Kp3[KIdx]);
		if(KMCIdx[KIdx]>0)
		  HKMomentumTruthMatched.Fill(Kp3[KIdx]);
		HKRadius.Fill(KRadius);
		HKZ.Fill(KPoca_z[KIdx]); 

		HPiMomentum.Fill(Pip3[PiIdx]);
		if(PiMCIdx[PiIdx]>0)
		  HPiMomentumTruthMatched.Fill(Pip3[PiIdx]);	
		HPiRadius.Fill(PiRadius);
		HPiZ.Fill(PiPoca_z[PiIdx]); 

		HD0Mass.Fill(D0Mass[D0Idx]); 
		if(D0MCIdx[D0Idx]>0)
		  HD0MassTruthMatched.Fill(D0Mass[D0Idx]); 
		HD0p3.Fill(D0p3[D0Idx]);   
		HD0VtxR.Fill(D0Radius);
		HD0VtxZ.Fill(D0Vtxz[D0Idx]); 

		HSlowPiMomentum.Fill(Pip3[SlowPiIdx]);	
		if(PiMCIdx[SlowPiIdx]>0)
		  HSlowPiMomentumTruthMatched.Fill(Pip3[SlowPiIdx]);	
		HSlowPiRadius.Fill(SlowPiRadius);   
		HSlowPiZ.Fill(PiPoca_z[SlowPiIdx]);
		HSlowPinSvt.Fill(TRKnSvt[SlowPitrkIdx]);
    
		HMassDiff.Fill(DstarMass[DstarIdx]-D0Mass[D0Idx]); 
		HDstarRadius.Fill(DstarRadius);
		HDstarZ.Fill(DstarVtxz[DstarIdx]); 
		HDstarProbab.Fill(log10(DstarProbab));	       
		HDstarMassSignal.Fill(DstarMass[DstarIdx]); 		  
		if(DstarMCIdx[DstarIdx]>0||!TruthMatch){
		  HDstarMassTruthMatched.Fill(DstarMass[DstarIdx]); 		
		  HDstarp3CM.Fill(Dstarp3CM[DstarIdx]); 
		  HDstarcosthCM.Fill(DstarcosthCM[DstarIdx]); 		  
		  H2DstarCMPvsTheta.Fill(Dstarp3CM[DstarIdx],DstarcosthCM[DstarIdx]);
		}


		RecoDstarCounterTotal++;	 	  
	      }//signal region cut	      	     
	    }//D0 prob cut	  
	  }//delta M cut	
	}//D0 p* cut        
      }//PID cut
    }//dstar loop
	 

    //now loop over MC    
    MCDstarCounterPerEvent=0; 
    Int_t mcid=-1;
    while(mcid<mcLen){
      mcid++;
      if(mcLund[mcid]==MatterOrAntiMatter*myDstarLund||MatterOrAntiMatter==0){
	MCDstarCounterPerEvent++; 
	MCDstarCounterTotal++;
	
	HMCDstarMass.Fill(mcmass[mcid]);
	HMCDstarp3CM.Fill(mcp3CM[mcid]); 
	HMCDstarcosthCM.Fill(mccosthCM[mcid]); 	
	H2MCDstarCMPvsTheta.Fill(mcp3CM[mcid],mccosthCM[mcid]);
      }
    }
    HMCNDstar.Fill(MCDstarCounterPerEvent);
        
  }

  //print summary
  cout<<"--------Summary-------"<<endl;
  cout<<"Total events="<<eventid<<endl;
  cout<<"Total Dstar: Generated="<<MCDstarCounterTotal<<" Reconstructed="<<RecoDstarCounterTotal<<endl;
  cout<<"--------End Summary---"<<endl;

 
  ////Save histograms
  TString filename;
  filename=Mode+"/"+ModeSubDir+"/"+"Plots.ps";
  TCanvas Canvas(filename,filename);
  Canvas.Print(filename+"[");

  TLine cutline;
  cutline.SetLineColor(2);


  //beam
  Canvas.Clear();
  HBeamRadius.Draw();
  Canvas.Print(filename);
  Canvas.Clear();
  HBeamZ.Draw();
  Canvas.Print(filename);    

  ///The Kaon
  Canvas.Clear();
  HKRadius.Draw();
  Canvas.Print(filename);
  Canvas.Clear();
  HKZ.Draw();
  Canvas.Print(filename);  
  Canvas.Clear();
  HKMomentum.Draw();
  Canvas.Print(filename);
  TH1F* HKMomentumTruthDifference=(TH1F*)HKMomentum.Clone();
  HKMomentumTruthDifference->Add(&HKMomentumTruthMatched,-1.);
  HKMomentumTruthDifference->SetTitle("Truth Difference");
  HKMomentumTruthDifference->Draw();
  Canvas.Print(filename);


  //ThePion
  Canvas.Clear();
  HPiRadius.Draw();
  Canvas.Print(filename);
  Canvas.Clear();
  HPiZ.Draw();
  Canvas.Print(filename);
  Canvas.Clear();
  HPiMomentum.Draw();
  Canvas.Print(filename);
  Canvas.Clear();
  TH1F* HPiMomentumTruthDifference=(TH1F*)HPiMomentum.Clone();
  HPiMomentumTruthDifference->Add(&HPiMomentumTruthMatched,-1.);
  HPiMomentumTruthDifference->SetTitle("Truth Difference");
  HPiMomentumTruthDifference->Draw();
  Canvas.Print(filename);



  
  if(Mode=="D0ToKPiPi0"){ 
    ///Gamma
    Canvas.Clear();
    HGammaMomentum.Draw();
    Canvas.Print(filename);
    Canvas.Clear();
    HGammaLat.Draw();
    Canvas.Print(filename);
    Canvas.Clear();
    HGammaEnergy.Draw();
    Canvas.Print(filename);
    Canvas.Clear();
    TH1F* HGammaEnergyTruthDifference=(TH1F*)HGammaEnergy.Clone();
    HGammaEnergyTruthDifference->Add(&HGammaEnergyTruthMatched,-1.);
    HGammaEnergyTruthDifference->SetTitle("Truth Difference");
    HGammaEnergyTruthDifference->Draw();
    Canvas.Print(filename);



    //The Pi0
    Canvas.Clear();
    HPi0Momentum.Draw();
    Canvas.Print(filename);
    Canvas.Clear();
    HPi0VtxProbab.Draw();
    Canvas.Print(filename);
    Canvas.Clear();
    HPi0MassPreCuts.GetYaxis()->SetRangeUser(0,1.05*HPi0MassPreCuts.GetMaximum());
    HPi0MassPreCuts.Draw();
    Canvas.Print(filename);
    Canvas.Clear();
    HPi0Mass.Draw();
    Canvas.Print(filename);
    Canvas.Clear();
    TH1F* HPi0MassTruthDifference=(TH1F*)HPi0Mass.Clone();
    HPi0MassTruthDifference->Add(&HPi0MassTruthMatched,-1.);
    HPi0MassTruthDifference->SetTitle("Truth Difference");
    HPi0MassTruthDifference->Draw();
    Canvas.Print(filename);
  }


  //The D0 
  Canvas.Clear();
  HD0VtxR.Draw();
  Canvas.Print(filename);
  Canvas.Clear();
  HD0VtxZ.Draw();
  Canvas.Print(filename);
  Canvas.Clear();
  HD0p3.Draw();
  Canvas.Print(filename);
  Canvas.Clear();
  HD0p3CM.Draw();
  cutline.DrawLine(D0p3CMCut,0,D0p3CMCut, HD0p3CM.GetMaximum());
  Canvas.Print(filename);
  Canvas.Clear();
  HD0Probab.Draw();
  cutline.DrawLine(log10(D0ProbabCut),0,log10(D0ProbabCut),HD0Probab.GetMaximum());
  Canvas.Print(filename);
  //Float_t D0MassFitLow=1.78;
  //Float_t D0MassFitHigh=1.94;
  //FitHistogram1gausPlusPolNFixRange(&HD0MassPreCuts,D0MassFitLow,D0MassFitHigh,1); 
  Canvas.Clear();
  HD0MassPreCuts.GetYaxis()->SetRangeUser(0,1.05*HD0MassPreCuts.GetMaximum());
  HD0MassPreCuts.Draw();
  HD0Mass.Draw("same");
  Canvas.Print(filename);
  Canvas.Clear();
  TH1F* HD0MassTruthDifference=(TH1F*)HD0Mass.Clone();
  HD0MassTruthDifference->Add(&HD0MassTruthMatched,-1.);
  HD0MassTruthDifference->SetTitle("Truth Difference");
  HD0MassTruthDifference->Draw();
  Canvas.Print(filename);

  ///The Slow Pion
  Canvas.Clear();
  HSlowPiRadius.Draw();
  Canvas.Print(filename);
  Canvas.Clear();
  HSlowPiZ.Draw();
  Canvas.Print(filename); 
  Canvas.Clear();
  HSlowPinSvt.Draw();
  Canvas.Print(filename);
  Canvas.Clear();
  HSlowPiMomentum.Draw();
  Canvas.Print(filename);
  Canvas.Clear();
  TH1F* HSlowPiMomentumTruthDifference=(TH1F*)HSlowPiMomentum.Clone();
  HSlowPiMomentumTruthDifference->Add(&HSlowPiMomentumTruthMatched,-1.);
  HSlowPiMomentumTruthDifference->SetTitle("Truth Difference");
  HSlowPiMomentumTruthDifference->Draw();
  Canvas.Print(filename);


  ///The Dstar
  Canvas.Clear();
  HDstarRadius.Draw();
  Canvas.Print(filename);
  Canvas.Clear();
  HDstarZ.Draw();
  Canvas.Print(filename);
//   Canvas.Clear();
//   HDstarp3CM.Draw();
//   Canvas.Print(filename);
//   Canvas.Clear();
//   HMCDstarp3CM.Draw();
//   Canvas.Print(filename);

  Canvas.Clear();
  if(TruthMatch){
    HMCDstarp3CM.GetYaxis()->SetRangeUser(0,1.05*HMCDstarp3CM.GetMaximum());
    HMCDstarp3CM.Draw();  
    HDstarp3CMPreCuts.Draw("same");
  }else {
    HDstarp3CMPreCuts.GetYaxis()->SetRangeUser(1,1.05*HDstarp3CMPreCuts.GetMaximum());
    HDstarp3CMPreCuts.Draw();
  }       
  HDstarp3CMPIDCut.Draw("same");
  HDstarp3CMPIDCutD0PCut.Draw("same");
  HDstarp3CMPIDCutD0PCutDeltaMCut.Draw("same");
  HDstarp3CMPIDCutD0PCutDeltaMCutD0Probab.Draw("same");
  HDstarp3CM.Draw("same");
  Canvas.Print(filename);

  //For bins were MC is less than 100 set efficiency to 0
  for(Int_t bin=0;bin<=HMCDstarp3CM.GetNbinsX();bin++)
    if(HMCDstarp3CM.GetBinContent(bin)<100)
      HMCDstarp3CM.SetBinContent(bin,1e30);  

  Canvas.Clear();  
  TH1F HDstarp3CMEfficiency0=*(TH1F*)HDstarp3CMPreCuts.Clone();
  HDstarp3CMEfficiency0.Divide(&HMCDstarp3CM);
  HDstarp3CMEfficiency0.SetTitle("CM Momentum Efficiency");
  HDstarp3CMEfficiency0.GetYaxis()
    //->SetRangeUser(0,1.05*HDstarp3CMEfficiency0.GetBinContent(HDstarp3CMEfficiency0.GetMaximumBin()));
    ->SetRangeUser(0,1);
  HDstarp3CMEfficiency0.Draw();
  TH1F HDstarp3CMEfficiency1=*(TH1F*)HDstarp3CMPIDCut.Clone();
  HDstarp3CMEfficiency1.Divide(&HMCDstarp3CM);
  HDstarp3CMEfficiency1.Draw("same");
  TH1F HDstarp3CMEfficiency2=*(TH1F*)HDstarp3CMPIDCutD0PCut.Clone();
  HDstarp3CMEfficiency2.Divide(&HMCDstarp3CM);
  HDstarp3CMEfficiency2.Draw("same");
  TH1F HDstarp3CMEfficiency3=*(TH1F*)HDstarp3CMPIDCutD0PCutDeltaMCut.Clone();
  HDstarp3CMEfficiency3.Divide(&HMCDstarp3CM);
  HDstarp3CMEfficiency3.Draw("same");
  TH1F HDstarp3CMEfficiency4=*(TH1F*)HDstarp3CMPIDCutD0PCutDeltaMCutD0Probab.Clone();
  HDstarp3CMEfficiency4.Divide(&HMCDstarp3CM);
  HDstarp3CMEfficiency4.Draw("same");  
  TH1F HDstarp3CMEfficiencyFinal=*(TH1F*)HDstarp3CM.Clone();
  HDstarp3CMEfficiencyFinal.Divide(&HMCDstarp3CM);  
  HDstarp3CMEfficiencyFinal.Draw("same");
  Canvas.Print(filename);
  
  Canvas.Clear();
  if(TruthMatch){
    HMCDstarcosthCM.GetYaxis()->SetRangeUser(0,1.05*HMCDstarcosthCM.GetMaximum());
    HMCDstarcosthCM.Draw();  
    HDstarcosthCMPreCuts.Draw("same");
  }else {
    HDstarcosthCMPreCuts.GetYaxis()->SetRangeUser(1,1.05*HDstarcosthCMPreCuts.GetMaximum());
    HDstarcosthCMPreCuts.Draw();
  }       
  HDstarcosthCMPIDCut.Draw("same");
  HDstarcosthCMPIDCutD0PCut.Draw("same");
  HDstarcosthCMPIDCutD0PCutDeltaMCut.Draw("same");
  HDstarcosthCMPIDCutD0PCutDeltaMCutD0Probab.Draw("same");
  HDstarcosthCM.Draw("same");
  Canvas.Print(filename);
  
  Canvas.Clear();  
  TH1F HDstarcosthCMEfficiency0=*(TH1F*)HDstarcosthCMPreCuts.Clone();
  HDstarcosthCMEfficiency0.Divide(&HMCDstarcosthCM);
  HDstarcosthCMEfficiency0.SetTitle("Angular Efficiency");
  HDstarcosthCMEfficiency0.GetYaxis()
    //->SetRangeUser(0,1.05*HDstarcosthCMEfficiency0.GetBinContent(HDstarcosthCMEfficiency0.GetMaximumBin()));
    ->SetRangeUser(0,1);
  HDstarcosthCMEfficiency0.Draw();
  TH1F HDstarcosthCMEfficiency1=*(TH1F*)HDstarcosthCMPIDCut.Clone();
  HDstarcosthCMEfficiency1.Divide(&HMCDstarcosthCM);
  HDstarcosthCMEfficiency1.Draw("same");
  TH1F HDstarcosthCMEfficiency2=*(TH1F*)HDstarcosthCMPIDCutD0PCut.Clone();
  HDstarcosthCMEfficiency2.Divide(&HMCDstarcosthCM);
  HDstarcosthCMEfficiency2.Draw("same");
  TH1F HDstarcosthCMEfficiency3=*(TH1F*)HDstarcosthCMPIDCutD0PCutDeltaMCut.Clone();
  HDstarcosthCMEfficiency3.Divide(&HMCDstarcosthCM);
  HDstarcosthCMEfficiency3.Draw("same");
  TH1F HDstarcosthCMEfficiency4=*(TH1F*)HDstarcosthCMPIDCutD0PCutDeltaMCutD0Probab.Clone();
  HDstarcosthCMEfficiency4.Divide(&HMCDstarcosthCM);
  HDstarcosthCMEfficiency4.Draw("same");  
  TH1F HDstarcosthCMEfficiencyFinal=*(TH1F*)HDstarcosthCM.Clone();
  HDstarcosthCMEfficiencyFinal.Divide(&HMCDstarcosthCM);  
  HDstarcosthCMEfficiencyFinal.Draw("same");
  Canvas.Print(filename);

  
  Canvas.Clear();
  H2DstarCMPvsTheta.Draw("colz");
  Canvas.Print(filename);
  Canvas.Clear();
  H2MCDstarCMPvsTheta.Draw("colz");
  Canvas.Print(filename);
  Canvas.Clear();
  //For bins were MC is less than 5 set efficiency to 0
  for(Int_t binx=0;binx<=H2MCDstarCMPvsTheta.GetNbinsX();binx++)
    for(Int_t biny=0;biny<=H2MCDstarCMPvsTheta.GetNbinsY();biny++)
      if(H2MCDstarCMPvsTheta.GetBinContent(binx,biny)<20)
	H2MCDstarCMPvsTheta.SetBinContent(binx,biny,1e30); 

  TH2F H2DstarPvsThetaEfficiency=*(TH2F*)H2DstarCMPvsTheta.Clone();
  H2DstarPvsThetaEfficiency.Divide(&H2MCDstarCMPvsTheta);
  H2DstarPvsThetaEfficiency.SetTitle("D* Efficiency");
  H2DstarPvsThetaEfficiency.GetZaxis()->SetTitle("efficiency");
  H2DstarPvsThetaEfficiency.SetTitleOffset(.68,"Z");
  H2DstarPvsThetaEfficiency.SetStats(kFALSE);
  H2DstarPvsThetaEfficiency.Draw("colz");
  Canvas.Print(filename);


  //Float_t DstarMassFitLow=1.93;
  //Float_t DstarMassFitHigh=2.09;
  //FitHistogram1gausPlusPolNFixRange(&HDstarMassPreCuts,DstarMassFitLow,DstarMassFitHigh,1); 
  delete HDstarMassPreCuts.GetListOfFunctions()->FindObject("stats");
  //FitHistogram1gausPlusPolNFixRange(&HMassDiff,.142,.150,0); 
  Canvas.Clear();
  HMassDiffPreCuts.GetYaxis()->SetRangeUser(0,1.05*HMassDiffPreCuts.GetMaximum());
  HMassDiffPreCuts.Draw();
  cutline.DrawLine(DeltaMassCutLo,0,DeltaMassCutLo,HMassDiff.GetMaximum());
  cutline.DrawLine(DeltaMassCutHi,0,DeltaMassCutHi,HMassDiff.GetMaximum());
  HMassDiff.Draw("same");
  Canvas.Print(filename);
  Canvas.Clear();
  HDstarProbab.Draw();
  Canvas.Print(filename);


  TH1F* HSignals[5];
  HSignals[0]=&HDstarMassPIDCut;
  HSignals[1]=&HDstarMassD0PCut;
  HSignals[2]=&HDstarMassD0PCutDeltaMCut;  
  HSignals[3]=&HDstarMassFinal;
  HSignals[4]=NULL;//make last null for while loops to work below
  Canvas.Clear();
  HDstarMassPreCuts.GetYaxis()->SetRangeUser(0,1.05*HDstarMassPreCuts.GetMaximum());
  HDstarMassPreCuts.Draw(); 
  Int_t sigid=0;
  while(HSignals[sigid]!=NULL){
    sigid++;    
    HSignals[sigid-1]->Draw("same");
  }
  //HMCDstarMass.Draw("same");
  Canvas.Print(filename); 

  //FitHistogram1gausPlusPolNFixRange(&HDstarMassFinal,DstarMassFitLow,DstarMassFitHigh,1); 
  //delete HDstarMassFinal.GetListOfFunctions()->FindObject("stats");
  Canvas.Clear();
  HDstarMassFinal.GetYaxis()->SetRangeUser(0,1.05*HDstarMassFinal.GetMaximum());
  HDstarMassFinal.Draw();
  cutline.DrawLine(DstarMassCutLo,0,DstarMassCutLo,HDstarMassFinal.GetMaximum());
  cutline.DrawLine(DstarMassCutHi,0,DstarMassCutHi,HDstarMassFinal.GetMaximum());
  Canvas.Print(filename);

  HDstarMassBkgPID.Add(&HDstarMassPreCuts,1);
  HDstarMassBkgPID.Add(&HDstarMassPIDCut,-1);
  HDstarMassBkgD0p3CM.Add(&HDstarMassPIDCut,1);
  HDstarMassBkgD0p3CM.Add(&HDstarMassD0PCut,-1);  
  HDstarMassBkgD0p3CMDeltaM.Add(&HDstarMassD0PCut,1);
  HDstarMassBkgD0p3CMDeltaM.Add(&HDstarMassD0PCutDeltaMCut,-1);
  HDstarMassBkgD0p3CMDeltaMD0Probab.Add(&HDstarMassD0PCutDeltaMCut,1);
  HDstarMassBkgD0p3CMDeltaMD0Probab.Add(&HDstarMassFinal,-1);
  HDstarMassBkgTotal.Add(&HDstarMassPreCuts,1);
  HDstarMassBkgTotal.Add(&HDstarMassFinal,-1);
  
  TH1F* HBkgs[5]; 
  HBkgs[0]=&HDstarMassBkgPID;
  HBkgs[1]=&HDstarMassBkgD0p3CM;
  HBkgs[2]=&HDstarMassBkgD0p3CMDeltaM;
  HBkgs[3]=&HDstarMassBkgD0p3CMDeltaMD0Probab;
  HBkgs[4]=NULL;//make last null for while loops to work below
  Canvas.Clear();
  HDstarMassBkgTotal.GetYaxis()->SetRangeUser(0,1.05*HDstarMassBkgTotal.GetMaximum());
  HDstarMassBkgTotal.Draw();
  Int_t bkgid=0;
  while(HBkgs[bkgid]!=NULL){
    bkgid++;
    HBkgs[bkgid-1]->Draw("same");    
  } 
  Canvas.Print(filename);


  
  Canvas.Clear();
  Canvas.Divide(2,2);
  Canvas.cd(1);
  HDstarMassSignal.Draw();
  HDstarMassTruthMatched.Draw("same");
  Canvas.cd(2);
  TH1F* HDstarMassTruthDifference=(TH1F*)HDstarMassSignal.Clone();
  HDstarMassTruthDifference->Add(&HDstarMassTruthMatched,-1.);
  HDstarMassTruthDifference->SetTitle("Truth Difference");
  HDstarMassTruthDifference->Draw();
  Canvas.cd(3);
  TH1F* HDstarMassTruthRatio=(TH1F*)HDstarMassTruthMatched.Clone();
  HDstarMassTruthRatio->Divide(&HDstarMassSignal);
  HDstarMassTruthRatio->SetTitle("Truth Ratio"); 
  HDstarMassTruthRatio->GetYaxis()->SetTitle("TruthMatched/Reconstructed");
  HDstarMassTruthRatio->Draw();
  Canvas.Print(filename);
 


  ///Make plot of TruthMatched vs particle: 
  Float_t KTruthRatio=0;
  Float_t PiTruthRatio=0;
  Float_t GammaTruthRatio=0;
  Float_t Pi0TruthRatio=0;
  Float_t D0TruthRatio=0;
  Float_t SlowPiTruthRatio=0;
  Float_t DstarTruthRatio=0;

  if(HKMomentum.Integral()>0) KTruthRatio=HKMomentumTruthMatched.Integral()/HKMomentum.Integral();
  if(HPiMomentum.Integral()>0) PiTruthRatio=HPiMomentumTruthMatched.Integral()/HPiMomentum.Integral();
  if(HGammaEnergy.Integral()>0) GammaTruthRatio=HPiMomentumTruthMatched.Integral()/HPiMomentum.Integral();
  if(HPi0Mass.Integral()>0) Pi0TruthRatio=HPi0MassTruthMatched.Integral()/HPi0Mass.Integral();
  if(HD0Mass.Integral()>0) D0TruthRatio=HD0MassTruthMatched.Integral()/HD0Mass.Integral();
  if(HSlowPiMomentum.Integral()>0) SlowPiTruthRatio=HSlowPiMomentumTruthMatched.Integral()/HSlowPiMomentum.Integral();
  if(HDstarMassSignal.Integral()>0) DstarTruthRatio=HDstarMassTruthMatched.Integral()/HDstarMassSignal.Integral();

  TH1F HTruthRatioVsParticle;
  SetHistoXY(&HTruthRatioVsParticle,"Truth Match Ratios",7,.5,7.5,"Particle Type","TruthMatched/Reconstructed");
  HTruthRatioVsParticle.SetBinContent(1,KTruthRatio);
  HTruthRatioVsParticle.SetBinContent(2,PiTruthRatio);
  HTruthRatioVsParticle.SetBinContent(3,GammaTruthRatio);
  HTruthRatioVsParticle.SetBinContent(4,Pi0TruthRatio); 
  HTruthRatioVsParticle.SetBinContent(5,D0TruthRatio);
  HTruthRatioVsParticle.SetBinContent(6,SlowPiTruthRatio);
  HTruthRatioVsParticle.SetBinContent(7,DstarTruthRatio);    
  HTruthRatioVsParticle.GetYaxis()->SetRangeUser(0,1);
  HTruthRatioVsParticle.SetStats(kFALSE);
  HTruthRatioVsParticle.SetBarWidth(.9);
  
  TText text;
  text.SetTextSize(.04);

  Canvas.Clear();
  HTruthRatioVsParticle.Draw("b");
  text.DrawText(1-.2,.1,"K");
  text.DrawText(2-.2,.1,"Pi");
  text.DrawText(3-.4,.1,"Gamma");
  text.DrawText(4-.2,.1,"Pi0");
  text.DrawText(5-.2,.1,"D^0");
  text.DrawText(6-.4,.1,"SlowPi");
  text.DrawText(7-.2,.1,"D*");
  text.DrawText(1-.35,KTruthRatio*.9,TString("")+long(100*KTruthRatio)+"."+long(1000*KTruthRatio)%10+"%");
  text.DrawText(2-.35,PiTruthRatio*.9,TString("")+long(100*PiTruthRatio)+"."+long(1000*PiTruthRatio)%10+"%");
  text.DrawText(3-.35,GammaTruthRatio*.9,TString("")+long(100*GammaTruthRatio)+"."+long(1000*GammaTruthRatio)%10+"%");
  text.DrawText(4-.35,Pi0TruthRatio*.9,TString("")+long(100*Pi0TruthRatio)+"."+long(1000*Pi0TruthRatio)%10+"%");
  text.DrawText(5-.35,D0TruthRatio*.9,TString("")+long(100*D0TruthRatio)+"."+long(1000*D0TruthRatio)%10+"%");
  text.DrawText(6-.35,SlowPiTruthRatio*.9,TString("")+long(100*SlowPiTruthRatio)+"."+long(1000*SlowPiTruthRatio)%10+"%");
  text.DrawText(7-.35,DstarTruthRatio*.9,TString("")+long(100*DstarTruthRatio)+"."+long(1000*DstarTruthRatio)%10+"%");
  Canvas.Print(filename);


  Canvas.Clear();
  Canvas.SetLogy(1);
  HMCNDstar.Draw();
  Canvas.Print(filename);
  Canvas.SetLogy(0);

  Canvas.Clear(); 
  HLundCheck.SetBarOffset(0);
  HLundCheck.SetBarWidth(.05);
  HLundCheck.Draw();
  Canvas.Print(filename);
   

  Canvas.Print(filename+"]");  
  return (TH1F*) HMassDiff.Clone(); 
}


