#include "../Globals.h"
#include "../AnalysisGlobals.h"
#include "../FUNCTIONS/Fits.h"
#include "../FUNCTIONS/SetHisto.h"
#include <TLine.h>
#include <TText.h>

TH1F* AnalyzeDs1pToDstPiPi(TChain* fChain,TString DATAorMC,Int_t MatterOrAntiMatter,TString Mode,TString NtupleDir,Int_t WhichCuts){

  
  //
  Bool_t TruthMatch=false;
  if(DATAorMC=="MC")TruthMatch=true;
 
  // Declaration of leave types
  Float_t beamSX;
  Float_t beamSY;
  Float_t beamSZ;

  Int_t   nDs1p;
  Float_t Ds1pChi2[400];   //[nDs1p]
  Float_t Ds1pMass[400];   //[nDs1p]
  Float_t Ds1pcosth[400];   //[nDs1p]
  Float_t Ds1pcosthCM[400];   //[nDs1p]
  Float_t Ds1pp3CM[400];   //[nDs1p]
  Float_t Ds1pphiCM[400];   //[nDs1p]
  Int_t   Ds1pLund[400];   //[nDs1p]
  Int_t   Ds1pMCIdx[400];   //[nDs1p]
  Int_t   Ds1pd1Lund[400];   //[nDs1p]
  Int_t   Ds1pd1Idx[400];   //[nDs1p]
  Int_t   Ds1pd2Lund[400];   //[nDs1p]
  Int_t   Ds1pd2Idx[400];   //[nDs1p]
  Int_t   Ds1pd3Lund[400];   //[nDs1p]
  Int_t   Ds1pd3Idx[400];   //[nDs1p]



  Int_t   nDstar;
  Float_t DstarMass[400];   //[nDstar]
  Float_t DstarMassErr[400];   //[nDstar]
  Float_t Dstarcosth[400];   //[nDstar]
  Float_t Dstarp3[400];   //[nDstar]
  Float_t Dstarp3CM[400];   //[nDstar]
  Int_t   DstarLund[400];   //[nDstar]
  Int_t   Dstard1Lund[400];   //[nDstar]
  Int_t   Dstard1Idx[400];   //[nDstar]
  Int_t   Dstard2Lund[400];   //[nDstar]
  Int_t   Dstard2Idx[400];   //[nDstar]
  Int_t   DstarMCIdx[400];
 
  Int_t   nD0;
  Float_t D0Mass[400];   //[nD0]
  Float_t D0MassErr[400];   //[nD0]
  Float_t D0p3CM[400];   //[nD0]
  Int_t   D0Lund[400];   //[nD0]
  Int_t   D0d1Lund[400];   //[nD0]
  Int_t   D0d1Idx[400];   //[nD0]
  Int_t   D0d2Lund[400];   //[nD0]
  Int_t   D0d2Idx[400];   //[nD0] 
  Int_t   D0d3Lund[400];   //[nD0]
  Int_t   D0d3Idx[400];
  Int_t   D0d4Lund[400];   //[nD0]
  Int_t   D0d4Idx[400];
  Int_t   D0MCIdx[400];
  Float_t D0Chi2[400];
  Int_t   D0nDof[400];

  Int_t   nPi;
  Float_t Pip3[400];   //[nPi]
  Int_t   PiLund[400];    //[nPi]
  Int_t   PiMCIdx[400];
  Int_t   PiTrkIdx[400];
  Int_t   PiSelectorsMap[400];
  
  Int_t   nK;
  Float_t Kp3[400];   //[nK]
  Int_t   KLund[400];   //[nK]
  Int_t   KMCIdx[400];
  Int_t   KTrkIdx[400];

  Int_t   nPi0;
  Float_t Pi0Mass[400];
  Float_t Pi0p3[400];   //[nPi]
  Int_t   Pi0Lund[400];   //[nPi]
  Int_t   Pi0MCIdx[400];

  Float_t GammaECal[400];
  Int_t   GammaMCIdx[400];

  Int_t TRKnSvt[400];
  Int_t TRKLund[400];
  

  ////MC block
  Int_t mcLen;
  Int_t mcLund[400];
  Int_t mothIdx[400];
  Int_t dauLen[400];
  Int_t dauIdx[400];
  Float_t mcmass[400]; 
  Float_t mccosth[400];
  Float_t mcp3[400];
  Float_t mccosthCM[400];
  Float_t mcp3CM[400]; 

  Float_t D0Probab;
  Int_t MCDs1pCounterPerEvent=0;
  Int_t MCDs1pCounterTotal=0;
  Int_t RecoDs1pCounterTotal=0;


  Int_t   Ds1pPi1Idx;
  Int_t   Ds1pPi2Idx;
  Int_t   Ds1pIdx;
  Int_t   DstarIdx;
  Int_t   D0Idx;
  Int_t   PiIdx;
  Int_t   SlowPiIdx;
  Int_t   KIdx;
  Int_t   Pi0Idx;
  Int_t   GammaIdx;  
  Int_t   PitrkIdx;
  Int_t   SlowPitrkIdx;
  Int_t   Ds1pPi1trkIdx;
  Int_t   Ds1pPi2trkIdx;

  //TChain* fChain=DstToD0PiChain(firstfile,lastfile, DATAorMC,Mode, ModeSubDir);
  if(fChain==NULL){
    cout<<"No chain."<<endl;
    return NULL;
  }
  fChain->SetBranchAddress("beamSX",&beamSX);
  fChain->SetBranchAddress("beamSY",&beamSY);
  fChain->SetBranchAddress("beamSZ",&beamSZ);

  fChain->SetBranchAddress("nDs1p",&nDs1p);
  fChain->SetBranchAddress("Ds1pMass",Ds1pMass);
  fChain->SetBranchAddress("Ds1pcosthCM",Ds1pcosthCM);
  fChain->SetBranchAddress("Ds1pp3CM",Ds1pp3CM);
  fChain->SetBranchAddress("Ds1pLund",Ds1pLund); 
  fChain->SetBranchAddress("Ds1pd1Lund",Ds1pd1Lund);
  fChain->SetBranchAddress("Ds1pd1Idx",Ds1pd1Idx);
  fChain->SetBranchAddress("Ds1pd2Lund",Ds1pd2Lund);
  fChain->SetBranchAddress("Ds1pd2Idx",Ds1pd2Idx);
  fChain->SetBranchAddress("Ds1pd3Lund",Ds1pd3Lund);
  fChain->SetBranchAddress("Ds1pd3Idx",Ds1pd3Idx);
 

  fChain->SetBranchAddress("nDstar",&nDstar);
  fChain->SetBranchAddress("DstarMass",DstarMass);
  fChain->SetBranchAddress("DstarLund",DstarLund);
  fChain->SetBranchAddress("Dstard1Lund",Dstard1Lund);
  fChain->SetBranchAddress("Dstard1Idx",Dstard1Idx);
  fChain->SetBranchAddress("Dstard2Lund",Dstard2Lund);
  fChain->SetBranchAddress("Dstard2Idx",Dstard2Idx);
  fChain->SetBranchAddress("nD0",&nD0);
  fChain->SetBranchAddress("D0Mass",D0Mass);
  fChain->SetBranchAddress("D0p3CM",D0p3CM);  
  fChain->SetBranchAddress("D0Lund",D0Lund);
  fChain->SetBranchAddress("D0d1Lund",D0d1Lund);
  fChain->SetBranchAddress("D0d1Idx",D0d1Idx);
  fChain->SetBranchAddress("D0d2Lund",D0d2Lund);
  fChain->SetBranchAddress("D0d2Idx",D0d2Idx);
  fChain->SetBranchAddress("D0Chi2",D0Chi2);
  fChain->SetBranchAddress("D0nDof",D0nDof);
  fChain->SetBranchAddress("nPi",&nPi);
  fChain->SetBranchAddress("Pip3",Pip3);
  fChain->SetBranchAddress("PiLund",PiLund);
  fChain->SetBranchAddress("PiTrkIdx",PiTrkIdx);
  fChain->SetBranchAddress("nK",&nK);
  fChain->SetBranchAddress("Kp3",Kp3);
  fChain->SetBranchAddress("KLund",KLund);
  fChain->SetBranchAddress("KTrkIdx",KTrkIdx);
  fChain->SetBranchAddress("TRKnSvt",TRKnSvt);
  fChain->SetBranchAddress("TRKLund",TRKLund);
  fChain->SetBranchAddress("piSelectorsMap",PiSelectorsMap);

 
  if(Mode=="D0ToKPiPi0"){
    fChain->SetBranchAddress("D0d3Lund",D0d3Lund);
    fChain->SetBranchAddress("D0d3Idx",D0d3Idx);
    fChain->SetBranchAddress("nPi0",&nPi0);
    fChain->SetBranchAddress("Pi0Mass",Pi0Mass);
    fChain->SetBranchAddress("Pi0p3",Pi0p3);
    fChain->SetBranchAddress("Pi0Lund",Pi0Lund);
    fChain->SetBranchAddress("GammaECal",GammaECal);
    fChain->SetBranchAddress("GammaMCIdx", GammaMCIdx);

    if(TruthMatch){
      fChain->SetBranchAddress("Pi0MCIdx",Pi0MCIdx);     
    }

  }
  if(Mode=="D0ToK3Pi"){
    fChain->SetBranchAddress("D0d3Lund",D0d3Lund);
    fChain->SetBranchAddress("D0d3Idx",D0d3Idx);
    fChain->SetBranchAddress("D0d4Lund",D0d4Lund);
    fChain->SetBranchAddress("D0d4Idx",D0d4Idx);
  }

  if(TruthMatch){
    fChain->SetBranchAddress("Ds1pMCIdx",Ds1pMCIdx);
    fChain->SetBranchAddress("DstarMCIdx",DstarMCIdx);
    fChain->SetBranchAddress("D0MCIdx",D0MCIdx);
    fChain->SetBranchAddress("KMCIdx",KMCIdx);
    fChain->SetBranchAddress("PiMCIdx",PiMCIdx);
    fChain->SetBranchAddress("mcLund",mcLund);
    fChain->SetBranchAddress("mcLen",&mcLen);
    fChain->SetBranchAddress("mcp3",mcp3);
    fChain->SetBranchAddress("mccosth",mccosth);
    fChain->SetBranchAddress("mcp3CM",mcp3CM);
    fChain->SetBranchAddress("mccosthCM",mccosthCM); 
  }
  

  //Histosgrams
  Float_t DstarMassWindow=.090;  
  Float_t D0MassWindow=.090;
  Int_t NMassBins=182;

  Float_t Ds1pMassLo=2.500;  
  Float_t Ds1pMassHi=2.570;  
  Int_t NDs1pMassBins=70;


  //////Cuts
  ///Loose
  Float_t D0p3CMCut=0.;
  //////////////
  

  if(Mode=="D0ToKPi"){
    //loose
    if(WhichCuts==1){       
      D0p3CMCut=2.4;  
    }

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
  
  /////pion 
  TH1F HPiMomentum;
  SetHistoXY(&HPiMomentum," Pion Momentum ",80,0,8,"p (GeV/c)","Entries/100MeV");

  TH1F HPiMomentumTruthMatched;
  SetHistoXY(&HPiMomentumTruthMatched," Pion Momentum ",80,0,8,"p (GeV/c)","Entries/100MeV");

  ///////////Mode dependent histos
  ////////////Mode=="D0ToKPiPi0"
  ////Gamma
  TH1F HGammaEnergy;
  SetHistoXY(&HGammaEnergy,"Gamma Energy",30,0,1.2,"calor. energy (GeV/c^{2})","Entries/40MeV");

  TH1F HGammaEnergyTruthMatched;
  SetHistoXY(&HGammaEnergyTruthMatched,"Gamma Energy",30,0,1.2,"calor. energy (GeV/c^{2})","Entries/40MeV");
  
  ////Pi0 
  TH1F HPi0MassPreCuts;
  SetHistoXY(&HPi0MassPreCuts,"Pi0 Mass After Cuts ",120,.075,.195," mass (GeV/c^{2})","Entries/1MeV");

  TH1F HPi0Mass;
  SetHistoXY(&HPi0Mass,"Pi0 Mass After Cuts ",120,.075,.195," mass (GeV/c^{2})","Entries/1MeV");

  TH1F HPi0MassTruthMatched;
  SetHistoXY(&HPi0MassTruthMatched,"Pi0 Mass After Cuts ",120,.075,.195," mass (GeV/c^{2})","Entries/1MeV");
  
  /////D0 
  TH1F HD0p3CM;
  SetHistoXY(&HD0p3CM,"D0 CM Momentum ",80,0,8,"p* (GeV/c)","Entries/100MeV");

  TH1F HD0MassPreCuts;
  SetHistoXY(&HD0MassPreCuts,"D0 Mass Before Cuts",NMassBins,D0PDGMass-D0MassWindow-.001,D0PDGMass+D0MassWindow+.001,"D0 Cand. Mass (GeV/c^{2})","Entries/1MeV");

  TH1F HD0Mass;
  SetHistoXY(&HD0Mass,"D0 Mass After Cuts ",NMassBins,D0PDGMass-D0MassWindow-.001,D0PDGMass+D0MassWindow+.001,"D0 Cand. Mass (GeV/c^{2})","Entries/1MeV");

  TH1F HD0MassTruthMatched;
  SetHistoXY(&HD0MassTruthMatched,"D0 Mass After Cuts ",NMassBins,D0PDGMass-D0MassWindow-.001,D0PDGMass+D0MassWindow+.001,"D0 Cand. Mass (GeV/c^{2})","Entries/1MeV");

  //slow pion
  TH1F HSlowPiMomentum;
  SetHistoXY(&HSlowPiMomentum,"Slow Pion Momentum ",30,0,1.2,"p (GeV/c)","Entries/40MeV");

  TH1F HSlowPiMomentumTruthMatched;
  SetHistoXY(&HSlowPiMomentumTruthMatched,"Slow Pion Momentum ",30,0,1.2,"p (GeV/c)","Entries/40MeV");

  ////Dstar
  TH1F HDstarMassPreCuts;
  SetHistoXY(&HDstarMassPreCuts,"D* Mass Before Cuts",NMassBins,DstarPDGMass-DstarMassWindow-.001,DstarPDGMass+DstarMassWindow+.001,"D* Cand. Mass (GeV/c^{2})","Entries/1MeV");
  HDstarMassPreCuts.SetLineColor(1);

  TH1F HDstarMass;
  SetHistoXY(&HDstarMass,"D* Mass After Cuts",NMassBins,DstarPDGMass-DstarMassWindow-.001,DstarPDGMass+DstarMassWindow+.001,"D* Cand. Mass (GeV/c^{2})","Entries/1MeV");
  HDstarMass.SetLineColor(1);
  
  TH1F HDstarMassTruthMatched;
  SetHistoXY(&HDstarMassTruthMatched,"D* Mass After Cuts and Truth Matched",NMassBins,DstarPDGMass-DstarMassWindow-.001,DstarPDGMass+DstarMassWindow+.001,"D* Cand. Mass (GeV/c^{2})","Entries/1MeV");
  HDstarMassTruthMatched.SetLineColor(1);

  TH1F HMassDiff;
  SetHistoXY(&HMassDiff,"#Delta M After Cuts",300,.139,.160,"D* Cand. Mass - D0 Cand. Mass (GeV/c^{2})","Entries/.1MeV");  


  /////pion1 
  TH1F HPi1Momentum;
  SetHistoXY(&HPi1Momentum," Pion1 Momentum ",80,0,8,"p (GeV/c)","Entries/100MeV");

  TH1F HPi1MomentumTruthMatched;
  SetHistoXY(&HPi1MomentumTruthMatched," Pion1 Momentum ",80,0,8,"p (GeV/c)","Entries/100MeV");

  ////Ds1p
  TH1F HDs1pMassPreCuts;
  SetHistoXY(&HDs1pMassPreCuts,"D'_{s1} Mass Before Cuts",NDs1pMassBins,Ds1pMassLo,Ds1pMassHi,"Ds1p Cand. Mass (GeV/c^{2})","Entries/1MeV");
  HDs1pMassPreCuts.SetLineColor(1);

  TH1F HDs1pMassPID;
  SetHistoXY(&HDs1pMassPID,"D'_{s1} Mass After Cuts",NDs1pMassBins,Ds1pMassLo,Ds1pMassHi,"Ds1p Cand. Mass (GeV/c^{2})","Entries/1MeV");
  HDs1pMassPID.SetLineColor(1);

  TH1F HDs1pMass;
  SetHistoXY(&HDs1pMass,"D'_{s1} Mass After Cuts",NDs1pMassBins,Ds1pMassLo,Ds1pMassHi,"Ds1p Cand. Mass (GeV/c^{2})","Entries/1MeV");
  HDs1pMass.SetLineColor(1);
  
  TH1F HDs1pMassTruthMatched;
  SetHistoXY(&HDs1pMassTruthMatched,"D'_{s1} Mass After Cuts and Truth Matched",NDs1pMassBins,Ds1pMassLo,Ds1pMassHi,"Ds1p Cand. Mass (GeV/c^{2})","Entries/1MeV");
  HDs1pMassTruthMatched.SetLineColor(1);



  /////////MC 
  TH1F HMCDs1pMass;
  SetHistoXY(&HMCDs1pMass,"MC D* Mass",NDs1pMassBins,Ds1pMassLo,Ds1pMassHi,"Ds1p Cand. Mass (GeV/c^{2})","Entries/1MeV");

  TH1F HMCDs1pp3CM;
  SetHistoXY(&HMCDs1pp3CM,"MC D* Momentum ",80,0,8,"p* (GeV/c)","Entries/100MeV");

  TH1F HMCDs1pcosthCM;
  SetHistoXY(&HMCDs1pcosthCM,"MC D* Angular Distribution ",100,-1,1,"cos(#theta)*","Entries/.02");
  
  TH2F H2MCDs1pCMPvsTheta;
  SetHisto2D(&H2MCDs1pCMPvsTheta,"MC p* vs #theta*",100,-1,1,"cos(#theta)*",50,0,5,"p* (GeV/c)","entries/(.02x100MeV)");

  TH1F HMCNDs1p;
  SetHistoXY(&HMCNDs1p,"MC Number of D* Generated",10,-.5,10.5,"nDs1p/event","Counts");
 
  //check particle id 
  TH1F HLundCheck;SetHisto(&HLundCheck,"Lund Id Check",15,-.5,14.5,"ParticleLund - LundExp");

  ////////+++++++++Important
  // Dstard1=D0,Dstard2=Pi;D0d1=K;D0d2=Pi
  //Lunds:
 //  Int_t myDs1pLund=20433;
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
         
    if(nDs1p>400){cout<<"Too many cands."<<endl;continue;}
    ///Loop over the reconstructed
    Ds1pIdx=-1;
    while( Ds1pIdx< nDs1p-1){
      Ds1pIdx++;

      
      Ds1pPi1Idx=Ds1pd2Idx[Ds1pIdx];
      Ds1pPi2Idx=Ds1pd3Idx[Ds1pIdx];
      DstarIdx=Ds1pd1Idx[Ds1pIdx];
      D0Idx=Dstard1Idx[DstarIdx];
      SlowPiIdx=Dstard2Idx[DstarIdx];
      PiIdx=D0d2Idx[D0Idx];
      KIdx=D0d1Idx[D0Idx];
      Pi0Idx=D0d3Idx[D0Idx];
      PitrkIdx=PiTrkIdx[PiIdx];
      SlowPitrkIdx=PiTrkIdx[SlowPiIdx];
      Ds1pPi1trkIdx=PiTrkIdx[Ds1pPi1Idx];
      Ds1pPi2trkIdx=PiTrkIdx[Ds1pPi2Idx];
 
      ////For Monte Carlo decide to analyze matter or antimatter
      if(!(DstarLund[DstarIdx]==MatterOrAntiMatter*myDstarLund || MatterOrAntiMatter==0))continue;

      //Check that Im using proper indexes
      if(Mode=="D0ToKPi"){
	if(abs(Dstard1Lund[DstarIdx]-D0Lund[D0Idx])>0)HLundCheck.Fill(1);
	else if(abs(Dstard2Lund[DstarIdx]-PiLund[SlowPiIdx])>0)HLundCheck.Fill(2);
	else if(abs(D0d1Lund[D0Idx]-KLund[KIdx])>0)HLundCheck.Fill(3);
	else if(abs(D0d2Lund[D0Idx]-PiLund[PiIdx])>0)HLundCheck.Fill(4);
	else if(abs(TRKLund[SlowPitrkIdx]-PiLund[SlowPiIdx])>0)HLundCheck.Fill(5);
	else if(abs(TRKLund[PitrkIdx]-PiLund[PiIdx])>0)HLundCheck.Fill(6);
	else if(abs(Ds1pd1Lund[Ds1pIdx]-DstarLund[DstarIdx])>0)HLundCheck.Fill(7);
	else if(abs(Ds1pd2Lund[Ds1pIdx]-PiLund[Ds1pPi1Idx])>0)HLundCheck.Fill(8);
	else if(abs(Ds1pd3Lund[Ds1pIdx]-PiLund[Ds1pPi2Idx])>0)HLundCheck.Fill(9);
	else if(abs(TRKLund[Ds1pPi1trkIdx]-PiLund[Ds1pPi1Idx])>0)HLundCheck.Fill(10);
	else if(abs(TRKLund[Ds1pPi2trkIdx]-PiLund[Ds1pPi2Idx])>0)HLundCheck.Fill(11);
	else HLundCheck.Fill(0);
      }      

          
      ///Fill Distributions  
      if(Mode=="D0ToKPiPi0"){        
	HPi0MassPreCuts.Fill(Pi0Mass[Pi0Idx]);  
      }
      
      HD0p3CM.Fill(D0p3CM[D0Idx]);
      HD0MassPreCuts.Fill(D0Mass[D0Idx]);
  
      HDstarMassPreCuts.Fill(DstarMass[DstarIdx]);   

      HDs1pMassPreCuts.Fill(Ds1pMass[Ds1pIdx]);  


      //Apply Cuts       
      if((PiSelectorsMap[Ds1pPi1trkIdx] & (1<<4) ) != 0 && (PiSelectorsMap[Ds1pPi2trkIdx] & (1<<4) ) != 0){
	HDs1pMassPID.Fill(Ds1pMass[Ds1pIdx]);
	if(D0p3CM[D0Idx] > D0p3CMCut){

	  HDs1pMass.Fill(Ds1pMass[Ds1pIdx]);
	  if(Ds1pMCIdx[DstarIdx]>0){
	    HDs1pMassTruthMatched.Fill(Ds1pMass[DstarIdx]); 		
	  }	  
	  
	  HPi1Momentum.Fill(Pip3[Ds1pPi1Idx]);
	  if(PiMCIdx[Ds1pPi1Idx]>0)
	    HPi1MomentumTruthMatched.Fill(Pip3[Ds1pPi1Idx]);	

	  HDstarMass.Fill(DstarMass[DstarIdx]); 		  
	  if(DstarMCIdx[DstarIdx]>0){
	    HDstarMassTruthMatched.Fill(DstarMass[DstarIdx]); 		
	  }	  
	  HMassDiff.Fill(DstarMass[DstarIdx]-D0Mass[D0Idx]);   

	  
	  HD0Mass.Fill(D0Mass[D0Idx]); 
	  if(D0MCIdx[D0Idx]>0)
	    HD0MassTruthMatched.Fill(D0Mass[D0Idx]); 

	  HSlowPiMomentum.Fill(Pip3[SlowPiIdx]);	
	  if(PiMCIdx[SlowPiIdx]>0)
	    HSlowPiMomentumTruthMatched.Fill(Pip3[SlowPiIdx]);	    

	  HKMomentum.Fill(Kp3[KIdx]);
	  if(KMCIdx[KIdx]>0)
	    HKMomentumTruthMatched.Fill(Kp3[KIdx]);

	  HPiMomentum.Fill(Pip3[PiIdx]);
	  if(PiMCIdx[PiIdx]>0)
	    HPiMomentumTruthMatched.Fill(Pip3[PiIdx]);	

	  if(Mode=="D0ToKPiPi0"){ 
	    HGammaEnergy.Fill(GammaECal[GammaIdx]);
	    if(GammaMCIdx[GammaIdx]>0)
	      HGammaEnergyTruthMatched.Fill(GammaECal[GammaIdx]);

	    HPi0Mass.Fill(Pi0Mass[Pi0Idx]);
	    if(Pi0MCIdx[Pi0Idx]>0)
	      HPi0MassTruthMatched.Fill(Pi0Mass[Pi0Idx]);
	  }
	 

	  RecoDs1pCounterTotal++;	 	  
	}//D0 p* cut        
      }//PID cut
      
    }//Ds1p loop
	 

    //now loop over MC    
    MCDs1pCounterPerEvent=0; 
    Int_t mcid=-1;
    while(mcid<mcLen){
      mcid++;
      if(mcLund[mcid]==MatterOrAntiMatter*myDs1pLund||MatterOrAntiMatter==0){
	MCDs1pCounterPerEvent++; 
	MCDs1pCounterTotal++;
	
      }
    }

        
  }

  //print summary
  cout<<"--------Summary-------"<<endl;
  cout<<"Total events="<<eventid<<endl;
  cout<<"Total Generated="<<MCDs1pCounterTotal<<" Reconstructed="<<RecoDs1pCounterTotal<<endl;
  cout<<"--------End Summary---"<<endl;

 
  ////Save histograms
  TString filename;
  filename=NtupleDir+"/"+"Plots.ps";
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
  HKMomentum.Draw();
  Canvas.Print(filename);
  TH1F* HKMomentumTruthDifference=(TH1F*)HKMomentum.Clone();
  HKMomentumTruthDifference->Add(&HKMomentumTruthMatched,-1.);
  HKMomentumTruthDifference->SetTitle("Truth Difference");
  HKMomentumTruthDifference->Draw();
  Canvas.Print(filename);

  //ThePion
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
    HPi0MassPreCuts.GetYaxis()->SetRangeUser(0,1.05*HPi0MassPreCuts.GetMaximum());
    HPi0MassPreCuts.Draw();
    HPi0Mass.Draw("same");
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
  HD0p3CM.Draw();
  cutline.DrawLine(D0p3CMCut,0,D0p3CMCut, HD0p3CM.GetMaximum());
  Canvas.Print(filename);
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
  HDstarMassPreCuts.GetYaxis()->SetRangeUser(0,1.05*HDstarMassPreCuts.GetMaximum());
  HDstarMassPreCuts.Draw();
  HDstarMass.Draw("same");
  //HDstarMassTruthMatched.Draw("same");
  Canvas.Print(filename); 
  Canvas.Clear();
  TH1F* HDstarMassTruthDifference=(TH1F*)HDstarMass.Clone();
  HDstarMassTruthDifference->Add(&HDstarMassTruthMatched,-1.);
  HDstarMassTruthDifference->SetTitle("Truth Difference");
  HDstarMassTruthDifference->Draw();
  Canvas.Print(filename);
  Canvas.Clear();
  HMassDiff.Draw();
  Canvas.Print(filename);


  //Pi1 and Pi2
  Canvas.Clear();
  HPi1Momentum.Draw();
  Canvas.Print(filename);
  Canvas.Clear();
  TH1F* HPi1MomentumTruthDifference=(TH1F*)HPi1Momentum.Clone();
  HPi1MomentumTruthDifference->Add(&HPi1MomentumTruthMatched,-1.);
  HPi1MomentumTruthDifference->SetTitle("Truth Difference");
  HPi1MomentumTruthDifference->Draw();
  Canvas.Print(filename);


  //Ds1p
  Canvas.Clear();
  HDs1pMassPreCuts.GetYaxis()->SetRangeUser(0,1.05*HDs1pMassPreCuts.GetMaximum());
  HDs1pMassPreCuts.Draw();
  HDs1pMassPID.Draw("same");  
  HDs1pMass.Draw("same");  
  HDs1pMassTruthMatched.Draw("same");
  Canvas.Print(filename); 
  Canvas.Clear();
  TH1F* HDs1pMassTruthDifference=(TH1F*)HDs1pMass.Clone();
  HDs1pMassTruthDifference->Add(&HDs1pMassTruthMatched,-1.);
  HDs1pMassTruthDifference->SetTitle("Truth Difference");
  HDs1pMassTruthDifference->Draw();
  Canvas.Print(filename);


  ///Make plot of TruthMatched vs particle: 
  Float_t KTruthRatio=0;
  Float_t PiTruthRatio=0;
  Float_t GammaTruthRatio=0;
  Float_t Pi0TruthRatio=0;
  Float_t D0TruthRatio=0;
  Float_t SlowPiTruthRatio=0;
  Float_t DstarTruthRatio=0;
  Float_t Pi1TruthRatio=0;
  Float_t Ds1pTruthRatio=0;

  if(HKMomentum.Integral()>0) KTruthRatio=HKMomentumTruthMatched.Integral()/HKMomentum.Integral();
  if(HPiMomentum.Integral()>0) PiTruthRatio=HPiMomentumTruthMatched.Integral()/HPiMomentum.Integral();
  if(HGammaEnergy.Integral()>0) GammaTruthRatio=HPiMomentumTruthMatched.Integral()/HPiMomentum.Integral();
  if(HPi0Mass.Integral()>0) Pi0TruthRatio=HPi0MassTruthMatched.Integral()/HPi0Mass.Integral();
  if(HD0Mass.Integral()>0) D0TruthRatio=HD0MassTruthMatched.Integral()/HD0Mass.Integral();
  if(HSlowPiMomentum.Integral()>0) SlowPiTruthRatio=HSlowPiMomentumTruthMatched.Integral()/HSlowPiMomentum.Integral();
  if(HDstarMass.Integral()>0) DstarTruthRatio=HDstarMassTruthMatched.Integral()/HDstarMass.Integral();
  if(HPi1Momentum.Integral()>0) Pi1TruthRatio=HPi1MomentumTruthMatched.Integral()/HPi1Momentum.Integral();
  if(HDs1pMass.Integral()>0) Ds1pTruthRatio=HDs1pMassTruthMatched.Integral()/HDs1pMass.Integral();
  

  TH1F HTruthRatioVsParticle;
  SetHistoXY(&HTruthRatioVsParticle,"Truth Match Ratios",9,.5,9.5,"Particle Type","TruthMatched/Reconstructed");
  HTruthRatioVsParticle.SetBinContent(1,KTruthRatio);
  HTruthRatioVsParticle.SetBinContent(2,PiTruthRatio);
  HTruthRatioVsParticle.SetBinContent(3,GammaTruthRatio);
  HTruthRatioVsParticle.SetBinContent(4,Pi0TruthRatio); 
  HTruthRatioVsParticle.SetBinContent(5,D0TruthRatio);
  HTruthRatioVsParticle.SetBinContent(6,SlowPiTruthRatio);
  HTruthRatioVsParticle.SetBinContent(7,DstarTruthRatio); 
  HTruthRatioVsParticle.SetBinContent(8,PiTruthRatio);   
  HTruthRatioVsParticle.SetBinContent(9,Ds1pTruthRatio); 
  HTruthRatioVsParticle.GetYaxis()->SetRangeUser(0,1);
  HTruthRatioVsParticle.SetStats(kFALSE);
  HTruthRatioVsParticle.SetBarWidth(.9);
  
  TText text;
  text.SetTextSize(.03);

  Canvas.Clear();
  HTruthRatioVsParticle.Draw("b");
  text.DrawText(1-.2,.1,"K");
  text.DrawText(2-.2,.1,"Pi");
  text.DrawText(3-.4,.1,"Gamma");
  text.DrawText(4-.2,.1,"Pi0");
  text.DrawText(5-.2,.1,"D0");
  text.DrawText(6-.4,.1,"SlowPi");
  text.DrawText(7-.2,.1,"D*");
  text.DrawText(8-.2,.1,"Pi1");
  text.DrawText(9-.2,.1,"Ds1");
  text.DrawText(1-.35,KTruthRatio*.9,TString("")+long(100*KTruthRatio)+"."+long(1000*KTruthRatio)%10+"%");
  text.DrawText(2-.35,PiTruthRatio*.9,TString("")+long(100*PiTruthRatio)+"."+long(1000*PiTruthRatio)%10+"%");
  text.DrawText(3-.35,GammaTruthRatio*.9,TString("")+long(100*GammaTruthRatio)+"."+long(1000*GammaTruthRatio)%10+"%");
  text.DrawText(4-.35,Pi0TruthRatio*.9,TString("")+long(100*Pi0TruthRatio)+"."+long(1000*Pi0TruthRatio)%10+"%");
  text.DrawText(5-.35,D0TruthRatio*.9,TString("")+long(100*D0TruthRatio)+"."+long(1000*D0TruthRatio)%10+"%");
  text.DrawText(6-.35,SlowPiTruthRatio*.9,TString("")+long(100*SlowPiTruthRatio)+"."+long(1000*SlowPiTruthRatio)%10+"%");
  text.DrawText(7-.35,DstarTruthRatio*.9,TString("")+long(100*DstarTruthRatio)+"."+long(1000*DstarTruthRatio)%10+"%");
  text.DrawText(8-.35,Pi1TruthRatio*.9,TString("")+long(100*Pi1TruthRatio)+"."+long(1000*Pi1TruthRatio)%10+"%");
  text.DrawText(9-.35,Ds1pTruthRatio*.9,TString("")+long(100*Ds1pTruthRatio)+"."+long(1000*Ds1pTruthRatio)%10+"%");
  Canvas.Print(filename);

  Canvas.Clear(); 
  HLundCheck.SetBarOffset(0);
  HLundCheck.SetBarWidth(.05);
  HLundCheck.Draw();
  Canvas.Print(filename);
   

  Canvas.Print(filename+"]");  
  return (TH1F*) HMassDiff.Clone(); 
}


  
