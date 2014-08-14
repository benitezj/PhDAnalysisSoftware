
#include "../Globals.h"
#include "../AnalysisGlobals.h"
#include "../MACROS/MyDstarChain.C"
#include "../FUNCTIONS/Fits.h"
#include "../FUNCTIONS/SetHisto.h"
#include <TLine.h>

TH1F* AnalyzeDstar(Int_t firstfile, Int_t lastfile,Int_t DATAorMC){

 
  // Declaration of leave types
  Int_t   nDstar;
  Float_t DstarMass[20];   //[nDstar]
  Float_t DstarMassErr[20];   //[nDstar]
  Float_t DstarVtxx[20];   //[nDstar]
  Float_t DstarVtxy[20];   //[nDstar]
  Float_t DstarVtxz[20];   //[nDstar]
  Float_t Dstarcosth[20];   //[nDstar]
  Float_t Dstarp3[20];   //[nDstar]
  Float_t Dstarphi[20];   //[nDstar]
  Int_t   DstarLund[20];   //[nDstar]
  Int_t   Dstard1Lund[20];   //[nDstar]
  Int_t   Dstard1Idx[20];   //[nDstar]
  Int_t   Dstard2Lund[20];   //[nDstar]
  Int_t   Dstard2Idx[20];   //[nDstar]

  Int_t   nD0;
  Float_t D0Mass[20];   //[nD0]
  Float_t D0MassErr[20];   //[nD0]
  Float_t D0Vtxx[20];   //[nD0]
  Float_t D0Vtxy[20];   //[nD0]
  Float_t D0Vtxz[20];   //[nD0]
  Float_t D0costh[20];   //[nD0]
  Float_t D0p3[20];   //[nD0]
  Float_t D0p3CM[20];   //[nD0]
  Float_t D0phi[20];   //[nD0]
  Int_t   D0Lund[20];   //[nD0]
  Int_t   D0d1Lund[20];   //[nD0]
  Int_t   D0d1Idx[20];   //[nD0]
  Int_t   D0d2Lund[20];   //[nD0]
  Int_t   D0d2Idx[20];   //[nD0] 
   
  Int_t   nPi;
  Float_t Picosth[40];   //[nPi]
  Float_t Pip3[40];   //[nPi]
  Float_t Piphi[40];   //[nPi]
  Int_t   PiLund[40];    //[nPi]
  Float_t PiRPoca[20];   //[nDstar]
  Float_t PiZPoca[20];   //[nDstar]

  Int_t   nK;
  Float_t Kcosth[20];   //[nK]
  Float_t Kp3[20];   //[nK]
  Float_t Kphi[20];   //[nK]
  Int_t   KLund[20];   //[nK]
  Float_t KRPoca[20];   //[nDstar]
  Float_t KZPoca[20];   //[nDstar]
    
  ///Usr Variables
  Int_t   PinDch[20];
  Int_t   PinSvt[20];  
  Float_t SlowPiRPoca[20];  //[nDstar]
  Float_t SlowPiZPoca[20];   //[nDstar]
  Int_t   SlowPinDch[20];
  Int_t   SlowPinSvt[20]; 
  Float_t SlowPiSVTConsis[20]; 
  Int_t   KnDch[20]; 
  Int_t   KnSvt[20]; 
  Float_t D0VtxProbab[20]; 

  TChain* fChain=MyDstarChain(firstfile,lastfile,DATAorMC);
  fChain->SetBranchStatus("*",0);

  fChain->SetBranchAddress("nDstar",&nDstar);
  fChain->SetBranchAddress("DstarMass",DstarMass);
  fChain->SetBranchAddress("DstarMassErr",DstarMassErr);
  fChain->SetBranchAddress("DstarVtxx",DstarVtxx);
  fChain->SetBranchAddress("DstarVtxy",DstarVtxy);
  fChain->SetBranchAddress("DstarVtxz",DstarVtxz);
  fChain->SetBranchAddress("Dstarcosth",Dstarcosth);  
  fChain->SetBranchAddress("Dstarp3",Dstarp3);
  fChain->SetBranchAddress("Dstarphi",Dstarphi);
  fChain->SetBranchAddress("DstarLund",DstarLund);
  fChain->SetBranchAddress("Dstard1Lund",Dstard1Lund);
  fChain->SetBranchAddress("Dstard1Idx",Dstard1Idx);
  fChain->SetBranchAddress("Dstard2Lund",Dstard2Lund);
  fChain->SetBranchAddress("Dstard2Idx",Dstard2Idx);
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
  fChain->SetBranchAddress("nPi",&nPi);
  fChain->SetBranchAddress("Picosth",Picosth);
  fChain->SetBranchAddress("Pip3",Pip3);
  fChain->SetBranchAddress("Piphi",Piphi);
  fChain->SetBranchAddress("PiLund",PiLund);
  fChain->SetBranchAddress("nK",&nK);
  fChain->SetBranchAddress("Kcosth",Kcosth);
  fChain->SetBranchAddress("Kp3",Kp3);
  fChain->SetBranchAddress("Kphi",Kphi);
  fChain->SetBranchAddress("KLund",KLund);

  //UsrData
  fChain->SetBranchAddress("DstardPionnDch",PinDch);
  fChain->SetBranchAddress("DstarpiSlownDch",SlowPinDch);
  fChain->SetBranchAddress("DstardKaonnDch",KnDch);
  fChain->SetBranchAddress("DstardPionnSvt",PinSvt);
  fChain->SetBranchAddress("DstarpiSlownSvt",SlowPinSvt);
  fChain->SetBranchAddress("DstardKaonnSvt",KnSvt);
  fChain->SetBranchAddress("DstarPiRPoca",PiRPoca);
  fChain->SetBranchAddress("DstarSlowPiRPoca",SlowPiRPoca);
  fChain->SetBranchAddress("DstarPiZPoca",PiZPoca);
  fChain->SetBranchAddress("DstarSlowPiZPoca",SlowPiZPoca);
  fChain->SetBranchAddress("DstarKRPoca",KRPoca);
  fChain->SetBranchAddress("DstarKZPoca",KZPoca);
  fChain->SetBranchAddress("DstarD0VtxProbab",D0VtxProbab);
  fChain->SetBranchAddress("DstarpiSlowSVTConsis",SlowPiSVTConsis);

  //////////////Branch Status
  fChain->SetBranchStatus("nDstar",1);
  fChain->SetBranchStatus("DstarMass",1);
  fChain->SetBranchStatus("DstarMassErr",1);
  fChain->SetBranchStatus("DstarVtxx",1);
  fChain->SetBranchStatus("DstarVtxy",1);
  fChain->SetBranchStatus("DstarVtxz",1);
  fChain->SetBranchStatus("Dstarcosth",1);  
  fChain->SetBranchStatus("Dstarp3",1);
  fChain->SetBranchStatus("Dstarphi",1);
  fChain->SetBranchStatus("DstarLund",1);
  fChain->SetBranchStatus("Dstard1Lund",1);
  fChain->SetBranchStatus("Dstard1Idx",1);
  fChain->SetBranchStatus("Dstard2Lund",1);
  fChain->SetBranchStatus("Dstard2Idx",1);  
  fChain->SetBranchStatus("nD0",1);
  fChain->SetBranchStatus("D0Mass",1);
  fChain->SetBranchStatus("D0MassErr",1);
  fChain->SetBranchStatus("D0Vtxx",1);
  fChain->SetBranchStatus("D0Vtxy",1);
  fChain->SetBranchStatus("D0Vtxz",1);
  fChain->SetBranchStatus("D0costh",1);
  fChain->SetBranchStatus("D0p3",1); 
  fChain->SetBranchStatus("D0p3CM",1);  
  fChain->SetBranchStatus("D0phi",1); 
  fChain->SetBranchStatus("D0Lund",1);
  fChain->SetBranchStatus("D0d1Lund",1);
  fChain->SetBranchStatus("D0d1Idx",1);
  fChain->SetBranchStatus("D0d2Lund",1);
  fChain->SetBranchStatus("D0d2Idx",1);
  fChain->SetBranchStatus("nPi",1);
  fChain->SetBranchStatus("Picosth",1);
  fChain->SetBranchStatus("Pip3",1);
  fChain->SetBranchStatus("Piphi",1);
  fChain->SetBranchStatus("PiLund",1); 
  fChain->SetBranchStatus("nK",1);
  fChain->SetBranchStatus("Kcosth",1);
  fChain->SetBranchStatus("Kp3",1);
  fChain->SetBranchStatus("Kphi",1);
  fChain->SetBranchStatus("KLund",1);
 
  //UsrData
  fChain->SetBranchStatus("DstardPionnDch",1);
  fChain->SetBranchStatus("DstarpiSlownDch",1);
  fChain->SetBranchStatus("DstardKaonnDch",1);
  fChain->SetBranchStatus("DstardPionnSvt",1);
  fChain->SetBranchStatus("DstarpiSlownSvt",1);
  fChain->SetBranchStatus("DstardKaonnSvt",1);
  fChain->SetBranchStatus("DstarPiRPoca",1);
  fChain->SetBranchStatus("DstarSlowPiRPoca",1);
  fChain->SetBranchStatus("DstarPiZPoca",1);
  fChain->SetBranchStatus("DstarSlowPiZPoca",1);
  fChain->SetBranchStatus("DstarKRPoca",1);
  fChain->SetBranchStatus("DstarKZPoca",1);
  fChain->SetBranchStatus("DstarD0VtxProbab",1);
  fChain->SetBranchStatus("DstarpiSlowSVTConsis",1);

  //Histosgrams
  //
  Float_t DstarMassWindow=.090;  
  Float_t D0MassWindow=.090;
  Float_t PDGMassDiff=DstarPDGMass-D0PDGMass;
  Float_t MassDiffWindow=.015;
  Int_t NMassBins=182;

  ///////////////////cuts  
  ////Pre-Cuts
  //SlowPion,Pion,Kaon: RDocaMax=.5;ZDocaMax=4;
  //Pion,Kaon:LHTight
  //D0Masscut:1.7745,1.9545
  //SlowPi svt,dch,drc consis:>1e-15
  //DstarMassCut:2.01-.5,2.01+.5
  //DeltaM:.130,.160
  
  //////post-Cuts
  //Float_t D0MassCut=.02;
  Float_t DeltaMassCut=.0025;
  Float_t PocaRadiusCut=.35;
  //Float_t PocaZCut=2.2;
  Int_t SlowPinSvtCut=4;
  Int_t SlowPinDchCut=0; 
  Float_t D0VtxProbabCut=.005;
  Float_t D0p3CMCut=2.4;
  Float_t SlowPiSVTConsisCut=5e-3;
  //////////////
  

  /////D0 pion
  TH1F HD0PiMomentum;SetHistoXY(&HD0PiMomentum,"D0 Pion Momentum ",250,0,5,"#pi momentum (GeV)","Entries/20MeV");
  TH1F HD0PiRPoca;SetHistoXY(&HD0PiRPoca,"Pion RPoca",100,0,.5,"RPoca  (cm)","Entries/50#mu m");
  TH1F HD0PiZPoca;SetHistoXY(&HD0PiZPoca,"Pion ZPoca",100,-4.,4.,"ZPoca (cm)","Entries/1mm");
  // Kaon
  TH1F HD0KMomentum;SetHistoXY(&HD0KMomentum,"D0 K Momentum ",250,0,5,"K momentum (GeV)","Entries/20MeV");
  TH1F HD0KRPoca;SetHistoXY(&HD0KRPoca,"Kaon RPoca",100,0,.5,"RPoca  (cm)","Entries/50#mu m");
  TH1F HD0KZPoca;SetHistoXY(&HD0KZPoca,"Kaon ZPoca",100,-4.,4.,"ZPoca (cm)","Entries/1mm");
  /////D0 
  TH1F HD0p3;SetHistoXY(&HD0p3,"D0 Momentum ",300,0,6,"D0 momentum (GeV)","Entries/20MeV");
  TH1F HD0p3CM;SetHistoXY(&HD0p3CM,"D0 CM Momentum ",300,0,6,"D0 CM Momentum (GeV)","Entries/20MeV");
  TH1F HD0MassPreCuts;SetHistoXY(&HD0MassPreCuts,"D0 Mass ",NMassBins,D0PDGMass-D0MassWindow-.001,D0PDGMass+D0MassWindow+.001,"D0 Mass (GeV)","Entries/1MeV");
  TH1F HD0Mass;SetHistoXY(&HD0Mass,"D0 Mass ",NMassBins,D0PDGMass-D0MassWindow-.001,D0PDGMass+D0MassWindow+.001,"D0 Mass (GeV)","Entries/1MeV");
  TH1F HD0VtxProbab;SetHisto(&HD0VtxProbab,"D0 Vtx Probab ",1000,0,1,"D0 Vtx probability");
  //slow pion
  TH1F HSlowPiMomentum;SetHistoXY(&HSlowPiMomentum,"Slow Pion Momentum ",75,0,1.5,"#pi momentum (GeV)","Entries/20MeV");
  TH1F HSlowPiRPoca;SetHistoXY(&HSlowPiRPoca,"SlowPion RPoca",100,0,.5,"RPoca (cm)","Entries/50#mu m");
  TH1F HSlowPiZPoca;SetHistoXY(&HSlowPiZPoca,"SlowPion ZPoca",80,-4.,4.,"ZPoca (cm)","Entries/1mm");
  TH1F HSlowPinSvt;SetHistoXY(&HSlowPinSvt,"SlowPion nSvt hits",20,-.5,19.5,"n Svt hits","Entries");
  TH1F HSlowPinDch;SetHistoXY(&HSlowPinDch,"SlowPion nDch hits",20,-.5,19.5,"n Dch hits","Entries");
  TH1F HSlowPiSVTConsis;SetHisto(&HSlowPiSVTConsis,"SlowPion SVT Consis",1000,0,1,"consistancy");
  ////Dstar
  TH1F HDstarMassPreCuts;SetHistoXY(&HDstarMassPreCuts,"D* Mass ",NMassBins,DstarPDGMass-DstarMassWindow-.001,DstarPDGMass+DstarMassWindow+.001,"D* Mass (GeV)","Entries/1MeV");
  HDstarMassPreCuts.SetLineColor(1);
  TH1F HDstarMass;SetHistoXY(&HDstarMass,"D* Mass ",NMassBins,DstarPDGMass-DstarMassWindow-.001,DstarPDGMass+DstarMassWindow+.001,"D* Mass (GeV)","Entries/1MeV");
  HDstarMass.SetLineColor(3);
  TH1F HDstarMassR1;SetHistoXY(&HDstarMassR1,"D* Mass 1",NMassBins,DstarPDGMass-DstarMassWindow-.001,DstarPDGMass+DstarMassWindow+.001,"D* Mass (GeV)","Entries/1MeV");
  HDstarMassR1.SetLineColor(3);
  TH1F HDstarMassR2;SetHistoXY(&HDstarMassR2,"D* Mass 2",NMassBins,DstarPDGMass-DstarMassWindow-.001,DstarPDGMass+DstarMassWindow+.001,"D* Mass (GeV)","Entries/1MeV");
  HDstarMassR2.SetLineColor(3);
  TH1F HDstarMassBkgMassDiff;SetHistoXY(&HDstarMassBkgMassDiff,"D* Mass BkgMassDiff ",NMassBins,DstarPDGMass-DstarMassWindow-.001,DstarPDGMass+DstarMassWindow+.001,"D* Mass (GeV)","Entries/1MeV");
  HDstarMassBkgMassDiff.SetLineColor(4);
  TH1F HDstarMassBkgSlowPiR;SetHistoXY(&HDstarMassBkgSlowPiR,"D* Mass BkgSlowPiR",NMassBins,DstarPDGMass-DstarMassWindow-.001,DstarPDGMass+DstarMassWindow+.001,"D* Mass (GeV)","Entries/1MeV");
  HDstarMassBkgSlowPiR.SetLineColor(5);
  TH1F HDstarMassBkgSlowPinHits;SetHistoXY(&HDstarMassBkgSlowPinHits,"D* Mass BkgSlowPinHits",NMassBins,DstarPDGMass-DstarMassWindow-.001,DstarPDGMass+DstarMassWindow+.001,"D* Mass (GeV)","Entries/1MeV");
  HDstarMassBkgSlowPinHits.SetLineColor(6);
  TH1F HDstarMassBkgD0VtxProbab;SetHistoXY(&HDstarMassBkgD0VtxProbab,"D* Mass BkgD0VtxProbab",NMassBins,DstarPDGMass-DstarMassWindow-.001,DstarPDGMass+DstarMassWindow+.001,"D* Mass (GeV)","Entries/1MeV");
  HDstarMassBkgD0VtxProbab.SetLineColor(7);
  TH1F HDstarMassBkgD0p3CM;SetHistoXY(&HDstarMassBkgD0p3CM,"D* Mass BkgD0p3CM",NMassBins,DstarPDGMass-DstarMassWindow-.001,DstarPDGMass+DstarMassWindow+.001,"D* Mass (GeV)","Entries/1MeV");
  HDstarMassBkgD0p3CM.SetLineColor(8);
  TH1F HDstarMassBkgSlowPiSVTConsis;SetHistoXY(&HDstarMassBkgSlowPiSVTConsis,"D* Mass BkgSlowPiSVTConsis",NMassBins,DstarPDGMass-DstarMassWindow-.001,DstarPDGMass+DstarMassWindow+.001,"D* Mass (GeV)","Entries/1MeV");
  HDstarMassBkgSlowPiSVTConsis.SetLineColor(9);

  TH1F HMassDiff;SetHistoXY(&HMassDiff,"#Delta M",300,PDGMassDiff-MassDiffWindow,PDGMassDiff+MassDiffWindow,"D* Mass - D0 Mass (GeV)","Entries/.1MeV");  
  


  //check particle id 
  TH1F HLundCheck;SetHisto(&HLundCheck,"Lund Id Check",3,-1.5,1.5,"ParticleLund - LundExp");



  /*+++++++++Important
  // Dstard1=D0,Dstard2=Pi;D0d1=K;D0d2=Pi
  //Lunds:
  Int_t myD0Lund=421;
  Int_t myKLund=321;
  Int_t myPiLund=211;
  */
  

  //Start the event loop;
  Int_t eventid=0;
  while(fChain->GetEntry(eventid,0)>0){   

    if(nDstar>20)nDstar=20;
    
    for(Int_t dstar=0;dstar<nDstar;dstar++){
         
    
      

      ///Distributions
      HSlowPiRPoca.Fill(SlowPiRPoca[dstar]);
      HSlowPiZPoca.Fill(PiZPoca[Dstard2Idx[dstar]]);
      HSlowPinSvt.Fill(SlowPinSvt[dstar]);
      HSlowPinDch.Fill(SlowPinDch[dstar]);
      HSlowPiSVTConsis.Fill(SlowPiSVTConsis[dstar]);
      HD0PiRPoca.Fill(PiRPoca[dstar]);
      HD0PiZPoca.Fill(PiZPoca[D0d2Idx[Dstard1Idx[dstar]]]);

      HD0KRPoca.Fill(KRPoca[dstar]);
      HD0KZPoca.Fill(KZPoca[D0d1Idx[Dstard1Idx[dstar]]]);

      HD0p3.Fill(D0p3[Dstard1Idx[dstar]]);
      HD0p3CM.Fill(D0p3CM[Dstard1Idx[dstar]]);
      HD0VtxProbab.Fill(D0VtxProbab[dstar]);
      HD0MassPreCuts.Fill(D0Mass[Dstard1Idx[dstar]]);

      HDstarMassPreCuts.Fill(DstarMass[dstar]);   
      HMassDiff.Fill(DstarMass[dstar]-D0Mass[Dstard1Idx[dstar]]);   


      //Cuts
      if(fabs(DstarMass[dstar]-D0Mass[Dstard1Idx[dstar]]-PDGMassDiff)<DeltaMassCut
	 && D0VtxProbab[Dstard1Idx[dstar]] > D0VtxProbabCut
	 && D0p3CM[Dstard1Idx[dstar]] > D0p3CMCut
	 && SlowPiRPoca[dstar]<PocaRadiusCut
	 && SlowPinDch[dstar]>=SlowPinDchCut
	 && SlowPinSvt[dstar]>=SlowPinSvtCut
	 && SlowPiSVTConsis[dstar]>SlowPiSVTConsisCut
	 ){
	
	HDstarMass.Fill(DstarMass[dstar]);      
	
	HD0Mass.Fill(D0Mass[Dstard1Idx[dstar]]);    

	HSlowPiMomentum.Fill(Pip3[Dstard2Idx[dstar]]);
	HD0PiMomentum.Fill(Pip3[D0d2Idx[Dstard1Idx[dstar]]]);
	HD0KMomentum.Fill(Kp3[D0d1Idx[Dstard1Idx[dstar]]]);     
      }
      

    
      ////////Check What Im throwing away
      if(fabs(DstarMass[dstar]-D0Mass[Dstard1Idx[dstar]]-PDGMassDiff)>DeltaMassCut)      
	HDstarMassBkgMassDiff.Fill(DstarMass[dstar]);
      if(D0VtxProbab[dstar]<D0VtxProbabCut)
	HDstarMassBkgD0VtxProbab.Fill(DstarMass[dstar]);
      if(D0p3CM[Dstard1Idx[dstar]]<D0p3CMCut)
	HDstarMassBkgD0p3CM.Fill(DstarMass[Dstard1Idx[dstar]]);
      if(SlowPiRPoca[dstar]>PocaRadiusCut)
	HDstarMassBkgSlowPiR.Fill(DstarMass[dstar]); 
      if(SlowPinDch[dstar] < SlowPinDchCut ||  SlowPinSvt[dstar] < SlowPinSvtCut)
	HDstarMassBkgSlowPinHits.Fill(DstarMass[dstar]);
      if(SlowPiSVTConsis[dstar] < SlowPiSVTConsisCut /*||  SlowPinSvt[dstar] < SlowPinSvtCut*/)
	HDstarMassBkgSlowPiSVTConsis.Fill(DstarMass[dstar]);
      if(fabs(DstarMass[dstar]-D0Mass[Dstard1Idx[dstar]]-PDGMassDiff-.005)<DeltaMassCut)
	 HDstarMassR1.Fill(DstarMass[dstar]);    
      if(fabs(DstarMass[dstar]-D0Mass[Dstard1Idx[dstar]]-PDGMassDiff-.01)<DeltaMassCut)
	 HDstarMassR2.Fill(DstarMass[dstar]);    
    

      //Check that Im using proper indexes
      HLundCheck.Fill(abs(Dstard1Lund[dstar]-D0Lund[Dstard1Idx[dstar]])+
		      abs(Dstard2Lund[dstar]-PiLund[Dstard2Idx[dstar]])+
		      abs(D0d1Lund[Dstard1Idx[dstar]]-KLund[D0d1Idx[Dstard1Idx[dstar]]])+
		      abs(D0d2Lund[Dstard1Idx[dstar]]-PiLund[D0d2Idx[Dstard1Idx[dstar]]]));
    }
    
    eventid++;
    if(eventid%5000==0)cout<<eventid<<" Events done."<<endl;        
  }


  ////
  TCanvas C("C","DstarAnalysis");
  TString filename;
  if(DATAorMC==1)filename="DstarAnalysisDATA.ps";
  if(DATAorMC==2)filename="DstarAnalysisMC.ps";
  C.Print(filename+"[");


  TLine cutline;
  cutline.SetLineColor(2);



  ///The D0 Kaon
  C.Clear();
  HD0KZPoca.Draw();
  C.Print(filename);  
  C.Clear();
  HD0KMomentum.Draw();
  C.Print(filename);
  C.Clear();
  HD0KRPoca.Draw();
  C.Print(filename);

  //The D0 Pion
  C.Clear();
  HD0PiMomentum.Draw();
  C.Print(filename);
  C.Clear();
  HD0PiRPoca.Draw();
  C.Print(filename);
  C.Clear();
  HD0PiZPoca.Draw();
  C.Print(filename);
 
  //The D0 
  C.Clear();
  HD0p3.Draw();
  C.Print(filename);
  C.Clear();
  HD0p3CM.Draw();
  cutline.DrawLine(D0p3CMCut,0,D0p3CMCut, HD0p3CM.GetMaximum());
  C.Print(filename);
  C.Clear();
  C.SetLogy(1);
  C.SetLogx(1);
  HD0VtxProbab.Draw();
  cutline.DrawLine(D0VtxProbabCut,0,D0VtxProbabCut,HD0VtxProbab.GetMaximum());
  C.Print(filename);
  C.SetLogy(0);
  C.SetLogx(0);
  FitHistogram1gausPlusPolNFixRange(&HD0MassPreCuts,1.8,1.92,1); 
  //delete HD0MassPreCuts.GetListOfFunctions()->FindObject("stats");
  C.Clear();
  HD0MassPreCuts.Draw();
  C.Print(filename);
  FitHistogram1gausPlusPolNFixRange(&HD0Mass,1.8,1.92,1); 
  //delete HD0Mass.GetListOfFunctions()->FindObject("stats");
  C.Clear();
  HD0Mass.Draw();
  C.Print(filename);



  ///The Slow Pion
  C.Clear();
  HSlowPiMomentum.Draw();
  C.Print(filename);
  C.Clear();
  HSlowPiRPoca.Draw();
  cutline.DrawLine(PocaRadiusCut,0,PocaRadiusCut, HSlowPiRPoca.GetMaximum());
  C.Print(filename);
  C.Clear();
  HSlowPiZPoca.Draw();
  C.Print(filename); 
  C.Clear();
  HSlowPinSvt.Draw();
  cutline.DrawLine(SlowPinSvtCut,0,SlowPinSvtCut, HSlowPinSvt.GetMaximum());
  C.Print(filename);
  C.Clear();
  HSlowPinDch.Draw();
  C.Print(filename);
  C.Clear();
  C.SetLogy(1);
  C.SetLogx(1);
  HSlowPiSVTConsis.Draw();
  cutline.DrawLine(SlowPiSVTConsisCut,0,SlowPiSVTConsisCut, HSlowPiSVTConsis.GetMaximum());
  C.Print(filename);
  C.SetLogy(0);
  C.SetLogx(0);

  ///The Dstar
  FitHistogram1gausPlusPolNFixRange(&HDstarMassPreCuts,1.94,2.08,1); 
  delete HDstarMassPreCuts.GetListOfFunctions()->FindObject("stats");
  C.Clear();
  HDstarMassPreCuts.Draw();
  C.Print(filename); 
  //FitHistogram1gausPlusPolNFixRange(&HMassDiff,.142,.150,0); 
  C.Clear();
  HMassDiff.Draw();
  cutline.DrawLine(PDGMassDiff-DeltaMassCut,0,PDGMassDiff-DeltaMassCut,HMassDiff.GetMaximum());
  cutline.DrawLine(PDGMassDiff+DeltaMassCut,0,PDGMassDiff+DeltaMassCut,HMassDiff.GetMaximum());
  C.Print(filename);
  C.Clear();
  HDstarMassPreCuts.Draw();
  HDstarMass.Draw("same");
  HDstarMassBkgMassDiff.Draw("same");
  HDstarMassBkgSlowPiR.Draw("same");
  HDstarMassBkgSlowPinHits.Draw("same");
  HDstarMassBkgD0VtxProbab.Draw("same");
  HDstarMassBkgD0p3CM.Draw("same");
  HDstarMassBkgSlowPiSVTConsis.Draw("same");
  C.Print(filename);
  FitHistogram1gausPlusPolNFixRange(&HDstarMass,1.94,2.08,1); 
  //delete HDstarMass.GetListOfFunctions()->FindObject("stats");
  C.Clear();
  HDstarMass.Draw();
  C.Print(filename);
  C.Clear();
  TH1F* HBkgs[8];
  HBkgs[0]=&HDstarMassBkgMassDiff;
  HBkgs[1]=&HDstarMassBkgSlowPiR;
  HBkgs[2]=&HDstarMassBkgSlowPinHits;
  HBkgs[3]=&HDstarMassBkgD0VtxProbab;
  HBkgs[4]=&HDstarMassBkgD0p3CM;
  HBkgs[5]=&HDstarMassBkgSlowPiSVTConsis;
  HBkgs[6]=&HDstarMassR1;
  HBkgs[7]=&HDstarMassR2;
  C.Divide(2,4);
  for(Int_t pad=1;pad<=8;pad++){
    C.cd(pad);
    if(HBkgs[pad-1]!=NULL)
      HBkgs[pad-1]->Draw();
  }
  C.Print(filename);


  C.Clear();
  HLundCheck.Draw();
  C.Print(filename);


  C.Print(filename+"]");  
  return (TH1F*) HMassDiff.Clone(); 
}
