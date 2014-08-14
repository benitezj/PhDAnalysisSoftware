#include "DRecoilToolsUser/NtupleCode/DRecoDsToKKsPlots.hh"
#include "DRecoilToolsUser/NtupleCode/DRecoResiduals.hh"
#include "DRecoilToolsUser/NtupleCode/DRecoHistPdfChi2Var.hh"

#include "RooChebychev.h"
#include "RooDataHist.h"
#include "RooGaussian.h"
#include "RooPolynomial.h"
#include "RooHistPdf.h"
#include "RooChi2Var.h"
#include "RooMinuit.h"
#include "RooFitResult.h"
#include "RooGenericPdf.h"
#include "RooProdPdf.h"
#include "RooPlot.h"
#include "RooAddPdf.h"
#include "RooGlobalFunc.h"
#include "RooCmdArg.h"
#include "TH2F.h"
#include "TList.h"
#include "TFile.h"
#include "TTree.h"
#include "THStack.h"
#include "TStyle.h"
#include "TPaveLabel.h"
#include <TLegend.h>
#include "RooRealVar.h"
#include "TCanvas.h"
#include "TROOT.h"

//#include "TH1F.h"
//#include "TH2F.h"
//#include "THStack.h"
//#include "TFile.h"
//#include "TLegend.h"
//#include "TTree.h"
//#include "TList.h"
//#include "TStyle.h"
//#include "stdio.h"
//#include <iostream>
//#include "Riostream.h"
//#include "RooPolynomial.h"
//#include "RooGenericPdf.h"
//#include "RooProdPdf.h"
//#include "RooPlot.h"
//#include "RooGlobalFunc.h"
//#include "RooCmdArg.h"
//#include "TPaveLabel.h"

using namespace RooFit ;

ClassImp(DRecoDsToKKsPlots) ;
DRecoDsToKKsPlots::DRecoDsToKKsPlots()
{
  gROOT->SetStyle("BABAR") ;
  cout << "Done Initializing the DRecoDsToKKsPlots analysis" << endl ;
  //gStyle->SetMarkerSize(1) ;
  gStyle->SetOptStat(0) ;

}

DRecoDsToKKsPlots::~DRecoDsToKKsPlots()
{ 
}

void DRecoDsToKKsPlots::backgroundID()
{
  TFile f("DsToKKs/SP1005/Run5On/Ntuple_New.root", "read");
  TTree* Ntuple = (TTree*) f.Get("Ntuple");
    
  Int_t truemothndaus;
  Int_t truegrandmothlund;
  Int_t truemothlund;
  Int_t truesib1lund;
  Int_t truesib2lund;
  Int_t truesib3lund;
  Int_t truesib4lund;
  Int_t truesib5lund;
  Int_t candklund;
  Float_t ksmassMMCFit2 ;  
  Float_t dsmassMMRaw ;
  
  Ntuple->SetBranchAddress("truemothndaus"    , &truemothndaus    );
  Ntuple->SetBranchAddress("truemothlund"     , &truemothlund     );
  Ntuple->SetBranchAddress("truesib1lund"     , &truesib1lund     );
  Ntuple->SetBranchAddress("truesib2lund"     , &truesib2lund     );
  Ntuple->SetBranchAddress("truesib3lund"     , &truesib3lund     ); 
  Ntuple->SetBranchAddress("truesib4lund"     , &truesib4lund     );
  Ntuple->SetBranchAddress("truesib5lund"     , &truesib5lund     );
  Ntuple->SetBranchAddress("candklund"        , &candklund        );
  Ntuple->SetBranchAddress("ksmassMMCFit2"    , &ksmassMMCFit2    );
  Ntuple->SetBranchAddress("dsmassMMRaw"      , &dsmassMMRaw      );
  Ntuple->SetBranchAddress("truegrandmothlund", &truegrandmothlund);

  Float_t Window[4];
  Window[0] = 0.100 ;
  Window[1] = 0.050 ;
  Window[2] = 0.025 ;
  Window[3] = 0.010 ;

  TString Cut[4] ; 
  Cut[0] = "VeryLoose" ;
  Cut[1] = "Loose"     ;
  Cut[2] = "Tight"     ;
  Cut[3] = "VeryTight" ; 

  TString prefix = "plots/DsMassWindow_" ; 

  TH1F* BkgType[4][2]; 
  
  for(Int_t i=0; i<4; i++){

    Int_t sig_ctr = 0 ;
    Int_t bkg_ctr = 0 ;
    TCanvas c5("c5", "", 0, 0, 500, 500) ;
    TString signal = "signal" ;
    TString background = "background" ;
    
    BkgType[i][0] = new TH1F(TString(signal + Cut[i]), "signal events", 80, 0., 0.8);
    BkgType[i][1] = new TH1F(TString(background + Cut[i]), "background events", 80, 0., 0.8);
    
    for (Int_t j=0;j<Ntuple->GetEntries();j++)
    {
      Ntuple->GetEntry(j) ;
      if(abs(truemothlund)==431 && truemothndaus==2 && abs(truesib1lund*truesib2lund)==311*321)
      {
	if( fabs(dsmassMMRaw-1.968) < Window[i] )
	{
	  BkgType[i][0]->Fill(sqrt(ksmassMMCFit2));
	  sig_ctr++ ;
	}
      }
      else
      {
	if( fabs(dsmassMMRaw-1.968) < Window[i] )
	{
	  BkgType[i][1]->Fill(sqrt(ksmassMMCFit2));
	  bkg_ctr++ ;
	}
      }
    }
    cout << "for the " << Cut[i] << "window, we get " << sig_ctr << " signal events and " << bkg_ctr << " background events" << endl ;

    BkgType[i][0]->SetFillColor(kRed);
    BkgType[i][1]->SetFillColor(kGreen);
    
    THStack stack;
    stack.Add(BkgType[i][1]);   //background
    stack.Add(BkgType[i][0]);   //signal
    stack.Draw() ;
    stack.GetXaxis()->SetTitle("K_s Missing Mass (GeV/c^{2})") ;
    stack.GetYaxis()->SetTitle("entries / 10 MeV/c^{2}") ;
    
    TLegend legend(0.8, 0.8, 0.9, 0.9);
    legend.AddEntry(BkgType[i][0], "Signal", "f");
    legend.AddEntry(BkgType[i][1], "Background", "f");
    legend.SetFillColor(kWhite);
    legend.Draw();
    
    c5.Print(TString(prefix + Cut[i] + ".eps") );
  }
}




void DRecoDsToKKsPlots::Roofit()
{
  Float_t lower = 0.0 ;
  Float_t upper = 1.6 ;
  Int_t   nBins = 32 ;

  // Create RooRealVar for mm2
  RooRealVar KsMass("KsMass" , "m(K_{S})_{miss} (GeV/c^{2})" , lower, upper ) ;
  KsMass.setBins(nBins) ;

  RooArgSet KsMassArgSet(KsMass, "RASKsMass") ;
  KsMassArgSet.add(KsMass) ;

  const Int_t nType = 5 ;
  TString typeName[nType] ;
  typeName[0] = "signal"     ;
  typeName[1] = "DsToKstarK" ;
  typeName[2] = "DsToPhiPi"  ; // eta pi
  typeName[3] = "Other"      ; // K0 K
  typeName[4] = "Dataset"    ; // tau nu
  //typeName[5] = "trueSig"  ;

  TFile f_in("DsToKKs/SP1005/Run5On/Ntuple_New.root", "read");
  TTree* Ntuple = (TTree*) f_in.Get("Ntuple");
  
  Int_t truemothndaus;
  Int_t truegrandmothlund;
  Int_t truemothlund;
  Int_t truesib1lund;
  Int_t truesib2lund;
  Int_t truesib3lund;
  Int_t truesib4lund;
  Int_t truesib5lund;
  Int_t candklund;
  Float_t ksmassRaw; 
  Float_t ksmassMMCFit2 ;

  
  Ntuple->SetBranchAddress("truemothndaus", &truemothndaus);
  Ntuple->SetBranchAddress("truemothlund", &truemothlund);
  Ntuple->SetBranchAddress("truesib1lund", &truesib1lund);
  Ntuple->SetBranchAddress("truesib2lund", &truesib2lund);
  Ntuple->SetBranchAddress("truesib3lund", &truesib3lund); 
  Ntuple->SetBranchAddress("truesib4lund", &truesib4lund);
  Ntuple->SetBranchAddress("truesib5lund", &truesib5lund);
  Ntuple->SetBranchAddress("candklund", &candklund);
  Ntuple->SetBranchAddress("ksmassMMRaw2", &ksmassRaw);
  Ntuple->SetBranchAddress("ksmassMMCFit2", &ksmassMMCFit2);
  Ntuple->SetBranchAddress("truegrandmothlund", &truegrandmothlund);

  TCanvas c1("c1", "", 0, 0, 500, 500);
  TCanvas c2("c2", "", 0, 0, 500, 500);
  TCanvas c3("c3", "", 0, 0, 500, 500);
  TCanvas c4("c4", "", 0, 0, 500, 500);

  TH1F* BkgType[nType];
  BkgType[0] = new TH1F("signal", "signal events"         ,nBins, lower , upper);
  BkgType[1] = new TH1F("DsToKstarK", "DsToKstar0K events",nBins, lower , upper);
  BkgType[2] = new TH1F("PhiToKK", "Phi to K K events"    ,nBins, lower , upper);
  BkgType[3] = new TH1F("Other", "other events"           ,nBins, lower , upper);
  BkgType[4] = new TH1F("Dataset", "Dataset"              ,nBins, lower , upper);

  Float_t nTrueSig = 0 ;
  Int_t nEntries = Ntuple->GetEntries() ;
 for (Int_t i=0;i<nEntries;i++){
   Ntuple->GetEntry(i) ;
   if(i<nEntries/2){
     if((abs(truemothlund)==431 && truemothndaus==2 && abs(truesib1lund*truesib2lund)==311*321)  || (abs(truemothlund)==431 && truemothndaus==3 && abs(truesib1lund*truesib2lund*truesib3lund)==311*321*22)  ){
       BkgType[0]->Fill(sqrt(ksmassMMCFit2));
     }
     else if(abs(truemothlund)==431 && truemothndaus==2 && abs(truesib1lund*truesib2lund)==313*321){
       BkgType[1]->Fill(sqrt(ksmassMMCFit2));
     }
     else if(abs(truegrandmothlund)==431 && abs(truemothlund)==333 && truemothndaus==2 && (abs(truesib1lund*truesib2lund)==321*321 || abs(truesib1lund*truesib2lund)==311*311)){
       BkgType[2]->Fill(sqrt(ksmassMMCFit2));
     }
     else{
       BkgType[3]->Fill(sqrt(ksmassMMCFit2));
       if(truemothndaus == 3)
	 {
	   cout << "Siblings are " << truesib1lund << "    " << truesib2lund << "    " << truesib3lund << endl;
	   cout << "Truemotndaus for above is " << truemothndaus << endl ;
	 }
     }
   }
   else{
     if(abs(truemothlund)==431 && truemothndaus==2 && abs(truesib1lund*truesib2lund)==311*321){
       Float_t w = 1.0 ;
       BkgType[4]->Fill(sqrt(ksmassMMCFit2), w);
       nTrueSig += w ;
     }
     else if(abs(truemothlund)==431 && truemothndaus==2 && abs(truesib1lund*truesib2lund)==313*321){
       BkgType[4]->Fill(sqrt(ksmassMMCFit2), 1);
     }
     else if(abs(truegrandmothlund)==431 && abs(truemothlund)==333 && truemothndaus==2 && (abs(truesib1lund*truesib2lund)==321*321 || abs(truesib1lund*truesib2lund)==311*311)){
       BkgType[4]->Fill(sqrt(ksmassMMCFit2), 1);
     }
     else{
       BkgType[4]->Fill(sqrt(ksmassMMCFit2), 1);
     }
   }
}
  BkgType[0]->SetFillColor(kRed);    BkgType[0]->Draw() ;
  BkgType[1]->SetFillColor(kGreen);  BkgType[1]->Draw() ;
  BkgType[2]->SetFillColor(kYellow); BkgType[3]->Draw() ;
  BkgType[3]->SetFillColor(kMagenta);BkgType[3]->Draw() ;
  TCanvas c5("c5", "", 0, 0, 1500, 1000) ;
  c5.Divide(2,2) ;
  for(Int_t i=0 ; i<4 ; i++)
  {
    c5.cd(i+1) ; BkgType[i]->Draw();
  }
  c5.Print("plots/Ks_BkgComponents.png") ;
  TCanvas c100 ;
  BkgType[4]->SetFillColor(kMagenta);
  BkgType[4]->Draw() ;
  c100.Print("plots/allComponentsDataset.png") ;
}



void DRecoDsToKKsPlots::KsMassStudy()
{
  TFile* f = new TFile("DsToKKs/SP1005/Run5On/Ntuple_New.root", "READ") ;
  TTree* t = (TTree*) f->Get("Ntuple") ;

  const Int_t nVar = 4 ;
  TString variable[nVar] ;
  TString varName[nVar] ;
  Int_t nBins[nVar] ;
  Float_t lower[nVar] ;
  Float_t upper[nVar] ;
  variable[0] = "sqrt(ksmassListCFit2)" ; varName[0] = "ksmassListCFit" ; nBins[0] = 50 ; lower[0] = 0.46 ; upper[0] = 0.52 ;
  variable[1] = "sqrt(ksmassListRaw2)"  ; varName[1] = "ksmassListRaw"  ; nBins[1] = 50 ; lower[1] = 0.46 ; upper[1] = 0.52 ;
  variable[2] = "sqrt(ksmassListUFit2)" ; varName[2] = "ksmassListUFit" ; nBins[2] = 50 ; lower[2] = 0.46 ; upper[2] = 0.52 ;
  variable[3] = "sqrt(dsmassListRaw2)"  ; varName[3] = "dsmassListRaw"  ; nBins[3] = 50 ; lower[3] = 1.0  ; upper[3] = 2.5  ;

  TCanvas c1("c1","c1", 100, 100, 1000, 1000) ;
  TH2F* hVar[nVar][nVar] ;
  for(Int_t i=0 ; i<nVar-1 ; i++)
  {
    for(Int_t j=i+1 ; j<nVar ; j++)
    {
      TString histogramName = TString(TString("h")+varName[i]+varName[j]) ;
      hVar[i][j] = new TH2F(histogramName, histogramName, nBins[i], lower[i], upper[i], nBins[j], lower[j], upper[j]) ;
      hVar[i][j]->GetXaxis()->SetTitle(variable[i]) ;
      hVar[i][j]->GetYaxis()->SetTitle(variable[j]) ;
      t->Draw(TString(variable[j]+":"+variable[i]+">>"+histogramName), "FitStatusCFit==0 && abs(sqrt(dsmassListRaw2)-1.968)<0.15 && dsmassListRaw2>0") ;
      TString printName = TString(TString("plots/")+histogramName+".eps") ;
      c1.Print(printName) ;
      c1.Clear() ;
      hVar[i][j]->Draw("COLZ") ;
      printName = TString(TString("plots/")+histogramName+"colz.eps") ;
      c1.Print(printName) ;
      c1.Clear() ;
    }
  }
}

void DRecoDsToKKsPlots::Dist_Roofit()
{
  const Int_t NFits = 4 ;
  Float_t lower = 0 ;
  Float_t upper = 0 ;
  Float_t lowers[2] ;
  Float_t uppers[2] ;
  lowers[0] = 0.0  ; uppers[0] = 0.6  ;
  lowers[1] = 0.16 ; uppers[1] = 0.36 ;
  Int_t nBins = 100  ;

  TString KSelectorString[6] ;
  KSelectorString[0] = "ChargedTracks" ;
  KSelectorString[1] = "KBDTNotAPion" ;
  KSelectorString[2] = "KBDTVeryLoose" ;
  KSelectorString[3] = "KBDTLoose" ;
  KSelectorString[4] = "KBDTTight" ;
  KSelectorString[5] = "KBDTVeryTight" ;

  TString FitType[NFits] ;
  FitType[0] = "MM_MC" ;
  FitType[1] = "List_MC" ;
  FitType[2] = "MM_Data" ;
  FitType[3] = "List_Data" ;

  const Int_t NSPModes = 7 ;
  TString SPModeName[NSPModes] ; 
  SPModeName[0] = "Data" ;
  SPModeName[1] = "SP998" ;
  SPModeName[2] = "SP1005" ;
  SPModeName[3] = "SP1235" ;
  SPModeName[4] = "SP1237" ;
  SPModeName[5] = "SP3429" ;
  SPModeName[6] = "AllMC" ;

  TFile* file[NSPModes] ;
  for(Int_t i=0 ; i<NSPModes-1 ; i++)
  {
    file[i] = new TFile(TString(TString("~cmmalone/BaBar/ana50/workdir/DsToKKs/")+ SPModeName[i] + "/Run5On/Ntuple_New.root"), "read");
  }
  file[6] = NULL ;

  TTree* ntuple[NSPModes] ;
  for(Int_t i=0 ; i<NSPModes-1 ; i++) ntuple[i] = (TTree*)file[i]->Get("Ntuple") ;
  //  for(Int_t i=NSPModes ; i<17 ; i++) ntuple[i] = (TTree*)file[i]->Get("Ntuple") ;  //new 8/27

  TString SPModeWeight[NSPModes] ;
  SPModeWeight[0] = "1.0" ;    //Data
  SPModeWeight[1] = "0.88" ;   //998
  SPModeWeight[2] = "0.4725" ; //1005
  SPModeWeight[3] = "1.0" ;    //1235
  SPModeWeight[4] = "1.0" ;    //1237
  SPModeWeight[5] = "1.0" ;    //3429

  RooDataHist*  r[NFits] ;
  RooRealVar*   f[NFits] ;
  RooRealVar*  f2[NFits] ;

  //TString WeightString[NSPModes] ;
  TH1F* histograms[NSPModes][NFits] ;

  TCanvas cStack("cStack", "", 0, 0, 500, 500); 
  THStack stack[NFits] ;

  TH1F* histToFit[NFits] ;
  TString plots_prefix = "plots/GausFits/" ;
  TString   web_prefix = "fDs2009/cmmalone/meetings/2009Jul28/plots/" ;

  TH1F* MCSum_Hist[2] ;
  TH1F* data_Hist[2] ;
  DRecoResiduals* residuals[NFits] ;
  TH1F* h_datafitOut[NFits] ;

  Float_t  meanValue[NFits] ;
  Float_t  meanError[NFits] ;
  Float_t sigmaValue[NFits] ;
  Float_t sigmaError[NFits] ;
  Int_t selector = 4 ;
  Float_t trueMCSignalYield        = -1 ;
  Float_t fittedMCSignalYield      = -1 ;
  Float_t fittedMCSignalYieldError = -1 ;
  Float_t fittedDataSignalYield      = -1 ;
  Float_t fittedDataSignalYieldError = -1 ;
  TCanvas c1("c1", "", 100, 100, 500, 500) ;

  for(Int_t i=0; i<NFits; i++)
  {
    lower = lowers[i%2] ;
    upper = uppers[i%2] ;
    
    RooRealVar KsMass("KsMass" , "m_{reco}^{2}(K_{S}) (GeV/c^{2})" , lower, upper ) ;
    KsMass.setBins(nBins) ;

    cout << lower << " " << upper << endl ;
    TString VarString = ((i%2)==0) ? "(ksmassMMCFit2)" : "(ksmassListCFit2)" ;
    TString CutString = ((i%2)==0) ? "((abs(dsmassMMRaw-1.968)<0.05)*(ksmassMMCFit2>0)*(probchi2MMCFit>0.00001))" 
      : TString(
		TString("(") + 
		TString("(abs(dsmassMMRaw-1.968)<0.1)")            + TString("*") +
		TString("(probchi2ListCFit>0.00001)")              + TString("*") + 
		TString("(ksmassListUFit2>0)")                     + TString("*") + 
		TString("(FitStatusUFit==0)")                      + TString("*") +
		TString("(abs(sqrt(dsmassListRaw2)-1.968)<0.01)")  + TString("*") +
		TString("(abs(sqrt(ksmassListRaw2)-0.498)<0.01)")  + 
		TString(")")
		);
    if(i<2)
    {
      //histogram and stack SP modes
      for(Int_t k=1; k<3; k++)
      {
	TString histogramName = "h" ;
	histogramName = TString(histogramName + SPModeName[k] + FitType[i]) ;
	histograms[k][i] = new TH1F(histogramName, histogramName, nBins, lower, upper) ; 
	TString weight = TString (SPModeWeight[k]+"*("+KSelectorString[3]+"==1)") ;
	weight = TString(weight + "*" + CutString) ;
	ntuple[k]->Draw(TString( VarString + ">>" + histogramName), weight, "goff");
	cout << weight << endl ;
	histograms[k][i]->SetFillColor(k+2);
	stack[i].Add(histograms[k][i]) ;
	
	histograms[k][i]->Draw() ;
	c1.Print(TString(histogramName+".eps")) ;
	c1.Clear() ;

	if(i==0)
	{
	  TString truthmatchString = TString(
					     TString("((abs(truemothlund)==431 && truemothndaus==2 && abs(truesib1lund*truesib2lund)==311*321)") +
					     TString("|| (abs(truemothlund)==431 && truemothndaus==3 && abs(truesib1lund*truesib2lund*truesib3lund)==311*321*22))")
					     ) ;
	  weight = TString(weight + "*" + truthmatchString) ;
	  TH1F hTemp("hTemp", "hTemp", nBins, lower, upper) ;
	  cout << endl << endl << endl << weight << endl << endl << endl ;
	  ntuple[2]->Draw(TString(VarString + ">>hTemp"), weight, "goff") ;
	  trueMCSignalYield = hTemp.GetSumOfWeights() ;
	}
      }
      
      //printing stuff for stacked SP modes histogram
      stack[i].Draw() ;
      stack[i].GetXaxis()->SetTitle("K_{s} " + TString(FitType[i]) + " Mass (GeV/c^{2})") ;
      stack[i].GetYaxis()->SetTitle("entries / 0.02 GeV/c^{2}") ; 	
      TLegend legend(0.8, 0.8, 0.9, 0.9);
      legend.AddEntry( histograms[2][i] , "ccbar", "f");
      legend.SetFillColor(kWhite);
      legend.Draw();
      cStack.Print(TString(FitType[i] + ".eps")) ;     
      
      //creates MC histogram we use for fitting  
      MCSum_Hist[i] = new TH1F("MCSum_Hist", "MCSum_Hist", nBins, lower, upper) ;
      for(Int_t j=0 ; j<nBins+1 ; j++)
      {
	MCSum_Hist[i]->SetBinContent(j,0) ;
	for(Int_t k=1 ; k<3 ; k++)
	{      
	  MCSum_Hist[i]->SetBinContent(j, MCSum_Hist[i]->GetBinContent(j)+histograms[k][i]->GetBinContent(j)) ;
	}
      }
      MCSum_Hist[i]->Draw() ;
      histToFit[i] = MCSum_Hist[i] ;
    }
    else
    {
      // Fill data histogram
      data_Hist[i-2] = new TH1F("data_Hist", "", nBins, lower, upper) ;
      TString weight = TString(CutString + "*(" + KSelectorString[selector]+"==1)") ;
      cout << endl << endl << weight << endl << endl ;
      ntuple[0]->Draw(TString( VarString + ">>" + "data_Hist"), weight, "goff"); 
      histToFit[i] = data_Hist[i-2] ;
      TCanvas canvasHistToFit("canvasHistToFit", "", 100, 100, 500, 500) ;
      histToFit[i]->Draw() ;
      canvasHistToFit.Print("histToFit.eps") ;
      Float_t integral = histToFit[i]->GetSumOfWeights() ;
    }
    
    TString rooDataHistName = "r" ;
    rooDataHistName = TString(rooDataHistName + FitType[i]) ;
    r[i] = new RooDataHist(rooDataHistName, rooDataHistName, KsMass, histToFit[i]) ;
    // r[i] is the RooDataHist we're going to fit to
    
    TString fName = "f" ;
    fName = TString(fName + FitType[i]) ;
    f[i] = new RooRealVar(fName, fName, 0.1, 0.0, 1.0) ;

    fName = TString(fName + FitType[i] +"2") ;
    f2[i] = new RooRealVar(fName, fName, 0.1, 0.0, 1.0) ;

    RooRealVar     mean("mean"   , "mean"   , 0.25, 0.1 , 0.5) ;
    RooRealVar    sigma("sigma"  , "sigma"  , 0.01, 0.0005 , 0.05 ) ;
    RooGaussian SigGaus("siggaus", "siggaus", KsMass, mean, sigma) ;

    RooRealVar     mean2("mean2"   , "mean2"   , 0.25, 0.1 , 0.5) ;
    RooRealVar    sigma2("sigma2"  , "sigma2"  , 0.01, 0.0005 , 0.05 ) ;
    RooGaussian SigGaus2("siggaus2", "siggaus2", KsMass, mean2, sigma2) ;

    RooRealVar cheb0("cheb0", "cheb0", 0 , -1, 1) ;
    RooRealVar cheb1("cheb1", "cheb1", 0 , -1, 1) ;
    RooRealVar cheb2("cheb2", "cheb2", 0 , -1, 1) ;
    RooChebychev BkgCheb("BkgCheb", "cheb", KsMass, RooArgList(cheb0, cheb1, cheb2));

    RooAddPdf* model ;
    if(i%2==0) model = new RooAddPdf("model", "model", RooArgList(SigGaus, BkgCheb )           , RooArgList(*f[i])          ) ;
    //if(i==1) model = new RooAddPdf("model", "model", RooArgList(SigGaus, SigGaus2, BkgCheb ) , RooArgList(*f[i] , *f2[i]) ) ;
    if(i==1) model = new RooAddPdf("model", "model", RooArgList(SigGaus, BkgCheb ) , RooArgList(*f[i] ) ) ;
    if(i==3) model = new RooAddPdf("model", "model", RooArgList(SigGaus, BkgCheb ) , RooArgList(*f[i] ) ) ;
    model->fitTo( *r[i] ) ;
      
    RooPlot* KsMass_frame = KsMass.frame() ;
    Float_t integral = histToFit[i]->GetSumOfWeights() ;
    h_datafitOut[i] = (TH1F*)model->createHistogram("dataHist", KsMass) ;  
    h_datafitOut[i]->Scale(integral) ;
    r[i]->plotOn(KsMass_frame, DataError(RooAbsData::SumW2)) ;
    model->plotOn(KsMass_frame) ;
    model->plotOn(KsMass_frame, Components(SigGaus) , LineStyle(kSolid) , LineColor(kRed)) ;

    cout << endl << endl <<  "--- Integral in peak is " << integral * f[i]->getVal() << endl ;
    cout << "--- Total events:  "      << integral  << endl << endl;
    
    TCanvas cResid("cResid", "", 0, 0, 500, 500);
    TString residualsName = "resids" ;
    residuals[i] = new DRecoResiduals(TString(residualsName+FitType[i]), histToFit[i], h_datafitOut[i]) ;
    residuals[i]->MakeFitPlotWithResiduals(&cResid, KsMass_frame) ;
    cResid.Print(TString(FitType[i]) + "_Resid.eps") ;
    cout << "Finished fit " << i << endl ;
    
    meanValue[i]  =  mean.getVal() ;
    meanError[i]  =  mean.getError() ;
    sigmaValue[i] = sigma.getVal() ;
    sigmaError[i] = sigma.getError() ;

    if(i==2)
    {
      fittedDataSignalYield      = ( f[i]->getVal()   )*integral ;
      fittedDataSignalYieldError = ( f[i]->getError() )*integral ;
    }
    if(i==0)
    {
      fittedMCSignalYield      = ( f[i]->getVal()   )*integral ;
      fittedMCSignalYieldError = ( f[i]->getError() )*integral ;
    }
    delete model ;
  }

  TH1F* hMean = new TH1F("hMean", "hMean", 4, 0, 4) ;
  hMean->GetXaxis()->SetTitle("Fit") ;
  hMean->GetYaxis()->SetTitle("Mean value of gaussian fit / MeV/c^{2}") ;
  hMean->SetLineColor(kRed) ;
  hMean->SetLineWidth(2) ;
  hMean->GetYaxis()->SetLabelSize(0.02) ;

  TH1F* hSigma = new TH1F("hSigma", "hSigma", 4, 0, 4) ;
  hSigma->GetXaxis()->SetTitle("Fit") ;
  hSigma->GetYaxis()->SetTitle("#sigma of gaussian fit / MeV/c^{2}") ;
  hSigma->SetLineColor(kBlue) ;
  hSigma->SetLineWidth(2) ;

  cout << endl << endl << "==========" << endl << " Results:" << endl << "==========" << endl ;
  for(Int_t i=0 ; i<NFits ; i++)
  {
    hMean->SetBinContent( i+1, (Double_t) meanValue[i]) ;
    hMean->SetBinError(   i+1, (Double_t) meanError[i]) ;
    hSigma->SetBinContent(i+1, sigmaValue[i]) ;
    hSigma->SetBinError(  i+1, sigmaError[i]) ;

    hMean->GetXaxis()->SetBinLabel(i+1, FitType[i]) ;
    hSigma->GetXaxis()->SetBinLabel(i+1, FitType[i]) ;

    cout << FitType[i] << ":" << endl ;
    cout << "       mean = " <<  meanValue[i] << endl ;
    cout << "  mean error= " <<  meanError[i] << endl ;
    cout << "      sigma = " << sigmaValue[i] << endl ;
    cout << " sigma error= " << sigmaError[i] << endl ;
    cout << endl ;
  }

  gStyle->SetOptStat(kFALSE) ;

  TCanvas summaryCanvas("summaryCanvas", "", 0, 0, 500, 500) ;
  hMean->Draw("p:e") ;
  summaryCanvas.Print(TString("KKSMeans.eps")) ;

  hSigma->Draw("p:e") ;
  summaryCanvas.Print(TString("KKSSigmas.eps")) ;

  cout << "  True MC signal yield = " <<   trueMCSignalYield*0.5  << endl ;
  cout << "Fitted MC signal yield = " << fittedMCSignalYield      << endl ;
  cout << "Fitted MC yield error  = " << fittedMCSignalYieldError << endl ;
  cout << "Fitted Data signal yield = " << fittedDataSignalYield      << endl ;
  cout << "Fitted Data yield error  = " << fittedDataSignalYieldError << endl ;

}


void DRecoDsToKKsPlots::Sig_Yields()
{

  TFile* file = new TFile("~cmmalone/BaBar/ana50/workdir/DsToKKs/SP9756/Run5On/Ntuple_New.root", "read");
  TTree* Tree = (TTree*) file->Get("Ntuple") ;

  Double_t nNXT0 = Tree->Draw("sqrt(ksmassMMCFit2)" , "((abs(dsmassMMRaw-1.968)<0.1)*(ksmassMMCFit2>0)*(probchi2MMCFit>0.00001)*(fragnpartgen==0))") ;
  Double_t nNXT1 = Tree->Draw("sqrt(ksmassMMCFit2)" , "((abs(dsmassMMRaw-1.968)<0.1)*(ksmassMMCFit2>0)*(probchi2MMCFit>0.00001)*(fragnpartgen==1))") ;
  Double_t nNXT2 = Tree->Draw("sqrt(ksmassMMCFit2)" , "((abs(dsmassMMRaw-1.968)<0.1)*(ksmassMMCFit2>0)*(probchi2MMCFit>0.00001)*(fragnpartgen==2))") ;
  Double_t nNXT3 = Tree->Draw("sqrt(ksmassMMCFit2)" , "((abs(dsmassMMRaw-1.968)<0.1)*(ksmassMMCFit2>0)*(probchi2MMCFit>0.00001)*(fragnpartgen==3))") ;
  Double_t nNXT4 = Tree->Draw("sqrt(ksmassMMCFit2)" , "((abs(dsmassMMRaw-1.968)<0.1)*(ksmassMMCFit2>0)*(probchi2MMCFit>0.00001)*(fragnpartgen==4))") ;
  Double_t nNXT5 = Tree->Draw("sqrt(ksmassMMCFit2)" , "((abs(dsmassMMRaw-1.968)<0.1)*(ksmassMMCFit2>0)*(probchi2MMCFit>0.00001)*(fragnpartgen==5))") ;

  cout << "NXT==0 : " << nNXT0 << " Error = " << 1/sqrt(nNXT0) << endl ;
  cout << "NXT==1 : " << nNXT1 << " Error = " << 1/sqrt(nNXT1) << endl ;
  cout << "NXT==2 : " << nNXT2 << " Error = " << 1/sqrt(nNXT2) << endl ;
  cout << "NXT==3 : " << nNXT3 << " Error = " << 1/sqrt(nNXT3) << endl ;
  cout << "NXT==4 : " << nNXT4 << " Error = " << 1/sqrt(nNXT4) << endl ;
  cout << "NXT==5 : " << nNXT5 << " Error = " << 1/sqrt(nNXT5) << endl ;
  cout << "Steve Holt!" << endl ;
}



void DRecoDsToKKsPlots::Signal_Roofit()
{

  Float_t lower = 0.2 ;
  Float_t upper = 1.0 ;
  Int_t nBins   = 40  ;

  Int_t NSPModes = 11 ;
  TFile* file[NSPModes] ;
  file[0] = new TFile("~cmmalone/BaBar/ana50/workdir/DsToKKs/SP9756/Run2On/Ntuple_New.root", "read");
  file[1] = new TFile("~cmmalone/BaBar/ana50/workdir/DsToKKs/SP9756/Run2Off/Ntuple_New.root", "read");
  file[2] = new TFile("~cmmalone/BaBar/ana50/workdir/DsToKKs/SP9756/Run3On/Ntuple_New.root", "read");
  file[3] = new TFile("~cmmalone/BaBar/ana50/workdir/DsToKKs/SP9756/Run3Off/Ntuple_New.root", "read");
  file[4] = new TFile("~cmmalone/BaBar/ana50/workdir/DsToKKs/SP9756/Run4On/Ntuple_New.root", "read");
  file[5] = new TFile("~cmmalone/BaBar/ana50/workdir/DsToKKs/SP9756/Run4Off/Ntuple_New.root", "read");
  file[6] = new TFile("~cmmalone/BaBar/ana50/workdir/DsToKKs/SP9756/Run5On/Ntuple_New.root", "read");
  file[7] = new TFile("~cmmalone/BaBar/ana50/workdir/DsToKKs/SP9756/Run5Off/Ntuple_New.root", "read");
  file[8] = new TFile("~cmmalone/BaBar/ana50/workdir/DsToKKs/SP9756/Run6On/Ntuple_New.root", "read");
  file[9] = new TFile("~cmmalone/BaBar/ana50/workdir/DsToKKs/SP9756/Run6Off/Ntuple_New.root", "read");
  file[10] = NULL ;

  TString SPModeName[NSPModes] ;
  SPModeName[0] = "Run2On" ;
  SPModeName[1] = "Run2Off" ;
  SPModeName[2] = "Run3On" ;
  SPModeName[3] = "Run3Off" ;
  SPModeName[4] = "Run4On" ;
  SPModeName[5] = "Run4Off" ;
  SPModeName[6] = "Run5On" ;
  SPModeName[7] = "Run5Off" ;
  SPModeName[8] = "Run6On" ;
  SPModeName[9] = "Run6Off" ;
  

  TTree* ntuple[NSPModes] ;
  for(Int_t i=0 ; i<NSPModes-1 ; i++) ntuple[i] = (TTree*)file[i]->Get("Ntuple") ;
  TH1F* histograms[NSPModes] ;

  //TString WeightString[NSPModes] ;
  for(Int_t i=0; i<NSPModes-1; i++) {
    TString histogramName = "h" ;
    histogramName = TString(histogramName + SPModeName[i]) ;
    histograms[i] = new TH1F(histogramName, histogramName, nBins, lower, upper) ; 
    ntuple[i]->Draw(TString(TString("sqrt(ksmassMMCFit2)>>")+histogramName) );
  }

  TCanvas cStack("cStack", "", 0, 0, 500, 500); 
  THStack stack ;

  TH1F* histToFit ;
  TString plots_prefix = "plots/GausFits/" ;
  TString web_prefix = "fDs2009/cmmalone/meetings/2009Jul28/plots/" ;

  RooRealVar KsMass("KsMass" , "m(K_{S}) (GeV/c^{2})" , lower, upper ) ;
  KsMass.setBins(nBins) ;
  RooArgSet KsMassArgSet(KsMass, "RASKsMass") ;
  KsMassArgSet.add(KsMass) ;

  //creates MC histogram we use for fitting 
  TH1F* MC9756_Hist = new TH1F("MC9756_Hist", "MC9756_Hist", nBins, lower, upper) ;
  for(Int_t j=0 ; j<nBins+1 ; j++){
    MC9756_Hist->SetBinContent(j,0) ;
    for(Int_t k=1 ; k<NSPModes-1 ; k++){      
      MC9756_Hist->SetBinContent(j, MC9756_Hist->GetBinContent(j)+histograms[k]->GetBinContent(j)) ;
    }
  }
  MC9756_Hist->Draw() ;
  histToFit = MC9756_Hist ;

  TString rooDataHistName = "r" ;
  rooDataHistName = TString(rooDataHistName) ;
  RooDataHist* r = new RooDataHist(rooDataHistName, rooDataHistName, KsMass, histToFit) ;
  // r[i] is the RooDataHist we're going to fit to
  
  TString fName = "f" ;
  fName = TString(fName) ;
  RooRealVar* f = new RooRealVar(fName, fName, 0.1, 0.0, 1.0) ;
  
  RooRealVar mean("mean", "mean", 0.47, 0.53) ;
  RooRealVar sigma("sigma", "sigma", 0.0, 0.08) ;
  RooGaussian SigGaus("siggaus", "siggaus", KsMass, mean, sigma) ;
  
  RooRealVar cheb0("cheb0", "cheb0",0,-1,1) ;
  RooRealVar cheb1("cheb1", "cheb1",0,-1,1) ;
  RooRealVar cheb2("cheb2", "cheb2",0,-1,1) ;
  RooChebychev BkgCheb("BkgCheb", "cheb", KsMass, RooArgList(cheb0, cheb1, cheb2));
  
  RooAddPdf model("model", "model", RooArgList(SigGaus, BkgCheb ) , RooArgList(*f)) ;
  model.fitTo( *r ) ;
  
  RooPlot* KsMass_frame = KsMass.frame() ;
  Float_t integral = histToFit->GetSumOfWeights() ;
  TH1F* h_datafitOut = (TH1F*)model.createHistogram("dataHist", KsMass) ;  
  h_datafitOut->Scale(integral) ;
  r->plotOn(KsMass_frame, DataError(RooAbsData::SumW2)) ;
  model.plotOn(KsMass_frame) ;
  model.plotOn(KsMass_frame, Components(SigGaus) , LineStyle(kSolid) , LineColor(kRed)) ;


  TCanvas cResid("cResid", "", 0, 0, 500, 500);
  TString residualsName = "resids" ;
  DRecoResiduals* residuals = new DRecoResiduals( TString(residualsName), histToFit, h_datafitOut) ;
  residuals->MakeFitPlotWithResiduals(&cResid, KsMass_frame) ;
  cResid.Print( "plots/SP9756_Resid.eps") ;
  cout << "Mean value is " <<  mean.getVal() << endl ;
  cout << "Mean error is " <<   mean.getError() << endl ;
  cout << "Sigma is " << sigma.getVal() << endl ;
  cout << "Sigma error is " << sigma.getError() << endl ;
}


void DRecoDsToKKsPlots::EExtra_Plots(Int_t nBins , Float_t lower , Float_t upper)
{
  if(nBins==0)
  {
    lower = 0.0 ;
    upper = 2.0 ;
    nBins = 20  ;
  }

  Int_t ModeNumber = 6 ;
  TString SPModes[ModeNumber] ;
  SPModes[5] = "Data" ;
  SPModes[0] = "SP998" ;
  SPModes[1] = "SP1005" ;
  SPModes[2] = "SP1235" ;
  SPModes[3] = "SP1237" ;
  SPModes[4] = "SP3429" ;
  
  Float_t weight[7] ;
  weight[6] = 1.0 ;
  weight[0] = 0.877 ;  
  weight[1] = 0.472 ;
  weight[2] = 1.0 ;
  weight[3] = 1.0 ;
  weight[4] = 1.0 ;
  weight[5] = 1.0 ;

  const Int_t NScaling = 11 ;
  Float_t Scaling[NScaling] ;
  Float_t ScalingLower = 1.0 ;
  Float_t ScalingUpper = 1.22 ;
  for(Int_t i=0 ; i<NScaling ; i++)
  {
    Scaling[i] = ScalingLower + (ScalingUpper-ScalingLower)*((Float_t)i)/((Float_t)NScaling) ;
  }

  Int_t KSelector         ;
  Int_t goodeventid       ;
  Float_t eextra          ;
  Float_t ksmassMMCFit2   ;
  Int_t truemothndaus     ;
  Int_t truegrandmothlund ;
  Int_t truemothlund      ;
  Int_t truesib1lund      ;
  Int_t truesib2lund      ;
  Int_t truesib3lund      ;
  Int_t truesib4lund      ;
  Int_t truesib5lund      ;
  Int_t candklund         ;
  
  TFile* file[ModeNumber] ;
  TTree* Tree[ModeNumber] ;
  for(Int_t i=0 ; i<ModeNumber ; i++)
  {
    file[i] = new TFile(TString(TString("DsToKKs/") + SPModes[i] + "/Run5On/Ntuple_New.root"), "read");
    Tree[i] = (TTree*) file[i]->Get("Ntuple") ;
    Tree[i]->SetBranchAddress("goodeventid", &goodeventid) ;
    Tree[i]->SetBranchAddress("KBDTTight",   &KSelector  ) ;
    Tree[i]->SetBranchAddress("eextra",      &eextra     ) ;
    Tree[i]->SetBranchAddress("ksmassMMCFit2",      &ksmassMMCFit2     ) ;
    Tree[i]->SetBranchAddress("truemothndaus", &truemothndaus);
    Tree[i]->SetBranchAddress("truemothlund", &truemothlund);
    Tree[i]->SetBranchAddress("truesib1lund", &truesib1lund);
    Tree[i]->SetBranchAddress("truesib2lund", &truesib2lund);
    Tree[i]->SetBranchAddress("truesib3lund", &truesib3lund); 
    Tree[i]->SetBranchAddress("truesib4lund", &truesib4lund);
    Tree[i]->SetBranchAddress("truesib5lund", &truesib5lund);
    Tree[i]->SetBranchAddress("candklund", &candklund);
    Tree[i]->SetBranchAddress("truegrandmothlund", &truegrandmothlund) ;
  }

  TH1F* histogram[ModeNumber][NScaling] ;
  TString HistPrefix = "N_" ; 
  TString plotsdir = "plots/EExtraPlots/" ;

  Int_t largestNEvents = -99 ;
  for(Int_t i=0; i<ModeNumber; i++){
    Tree[i]->GetEntry(Tree[i]->GetEntries()-1) ;
    if(goodeventid > largestNEvents){
      largestNEvents = goodeventid ;
    }
  }
  const Int_t NEvents = largestNEvents ;
  cout << "NEvents: " << NEvents << endl ;
  Int_t nRecons[NEvents+1] ;

  for(Int_t i=0; i<ModeNumber; i++){
    
    gStyle->SetOptStat(0) ;

    for(Int_t k=0; k<NScaling; k++)
    {
      TString histogramName = TString(TString("hEExtra") + SPModes[i] + "_" + (long)k) ;
      char buffer[100] ;
      sprintf(buffer, "%.2f", Scaling[k]);
      TString scalingString = TString(buffer) ;
      TString histogramTitle = TString(TString("E_{extra} (Scaling = ") + scalingString + ")")  ;
      histogram[i][k] = new TH1F(histogramName, histogramTitle, nBins, lower, upper) ;  
      histogram[i][k]->SetFillColor(i+2) ;
      histogram[i][k]->GetXaxis()->SetTitle("eextra (GeV)") ;
      histogram[i][k]->GetYaxis()->SetTitle("entries / 100 MeV") ;
    }	
    for(Int_t j=0 ; j<NEvents ; j++) nRecons[j] = 0 ;
    Int_t NEntries = Tree[i]->GetEntries() ;
    for(Int_t j=0 ; j<NEntries; j++){
      Tree[i]->GetEntry(j) ;
      if(KSelector== 0) continue ;
      if(sqrt(ksmassMMCFit2) >0.7 || sqrt(ksmassMMCFit2) < 0.2) continue ;
      nRecons[goodeventid]++ ;
      //cout << "Event number " << goodeventid << " has " << nRecons[goodeventid] << " reconstructions" << endl ; 
    }  

    Int_t ct_211_321 = 0 ; 
    Int_t ct_311_321 = 0 ;
    Int_t ct_other = 0 ;
    

    for(Int_t j=0; j<NEntries; j++)
    {
      Tree[i]->GetEntry(j) ;
      if(sqrt(ksmassMMCFit2)>0.7 || sqrt(ksmassMMCFit2) < 0.2) continue ;
      if(KSelector==0) continue ; 
      if(nRecons[goodeventid]==0) continue ;
      Float_t w = weight[i]/(Float_t)nRecons[goodeventid] ;
      for(Int_t k=0 ; k<NScaling ; k++)
      {
	if(i==5)
	{
	  histogram[i][k]->Fill(eextra, w) ;
	}
	else
	{
	  histogram[i][k]->Fill(eextra*Scaling[k], w) ;
	}
      }
    }
    
    cout << "In SP1005, the number of 211+321 evts are " << ct_211_321 << endl ;
    cout << "In SP1005, the number of 311+321 evts are " << ct_311_321 << endl ;
    cout << "In SP1005, the number of other evts are " << ct_other << endl ;

    for(Int_t k=0; k<NScaling; k++)
    {
      TCanvas histogramCanvas("c1", "", 100, 100, 800, 300) ;  
      histogram[i][k]->Draw() ;
      histogramCanvas.Print(TString(plotsdir + histogram[i][k]->GetName() + ".eps")) ;
    }
  }

  TH1F* hScaling[NScaling] ;
  THStack* stackScaling[NScaling] ;
  TLegend legend(0.7, 0.7, 0.9, 0.9) ;
  Float_t chi2[NScaling] ;
  Float_t error[NScaling] ;

  TCanvas canvas("canvas", "", 100, 100, 800, 300) ;
  for(Int_t k=0; k<NScaling; k++)
  {
    stackScaling[k] = new THStack ;
    for(Int_t j=0 ; j<5 ; j++)
    {
      stackScaling[k]->Add(histogram[j][k]) ;
      if(k==0) legend.AddEntry(histogram[j][k], SPModes[j], "f") ;
    }
    stackScaling[k]->Draw() ;
    stackScaling[k]->GetXaxis()->SetTitle(histogram[0][k]->GetXaxis()->GetTitle()) ;
    stackScaling[k]->GetYaxis()->SetTitle(histogram[0][k]->GetYaxis()->GetTitle()) ;
    canvas.Print(TString(plotsdir + "stack" + (long)k + ".eps")) ;

    TString histogramName = TString(TString("hScaling")+(long)Scaling[k]) ;
    char buffer[100] ;
    sprintf(buffer, "%.2f", Scaling[k]);
    TString scalingString = TString(buffer) ;
    TString histogramTitle = TString(TString("E_{extra} (Scaling = ") + scalingString + ")")  ;
    hScaling[k] = new TH1F (histogramName, histogramTitle, nBins, lower, upper) ; 
    for(Int_t i = 1; i<nBins+1; i++)
    {
      for(Int_t j=0 ; j<5 ; j++)
      {
	hScaling[k]->SetBinContent(i, hScaling[k]->GetBinContent(i) + histogram[j][k]->GetBinContent(i)) ;
      }
    }
    hScaling[k]->Scale( histogram[5][k]->GetSumOfWeights()/hScaling[k]->GetSumOfWeights() ) ;
    chi2[k] = 0.0 ;
    error[k] = 0.0 ;
    for(Int_t i = 1; i<(nBins+1); i++)
    {
      if(histogram[5][k]->GetBinContent(i)>0)
      {
	chi2[k] += ((hScaling[k]->GetBinContent(i)-histogram[5][k]->GetBinContent(i))*((hScaling[k]->GetBinContent(i)-histogram[5][k]->GetBinContent(i)))) / (histogram[5][k]->GetBinContent(i)) ;
	error[k] += 1.0/histogram[5][k]->GetBinContent(i) ;
      }
    }
    error[k] = sqrt(error[k]) ;
    
    cout << "Scaling = " << Scaling[k] << ";    Chi2 = " << chi2[k] << "     Error = " << error[k] << endl ;
    cout << "Kolmogorov statistic : " << histogram[5][k]->KolmogorovTest(hScaling[k]) << endl ;

    if(histogram[5][k]->GetMaximum()>hScaling[k]->GetMaximum())
    {
      
      histogram[5][k]->SetMinimum(0) ;
      histogram[5][k]->Draw("p:e") ;
      hScaling[k]->SetTitle("Testing") ;
      hScaling[k]->GetXaxis()->SetTitle("eextra (GeV)") ;
      hScaling[k]->GetYaxis()->SetTitle("entries / 50 MeV") ;
      hScaling[k]->Draw("Sames") ;
      histogram[5][k]->SetFillColor(kGreen) ;
      histogram[5][k]->Draw("p:e:Sames") ;
      //canvas.Print(TString(plotsdir + hScaling[k]->GetTitle() + "_compare.eps")) ;
      canvas.Print(TString(plotsdir + "hScale" + (long)k + "_compare.eps")) ;
      canvas.Clear() ;
    }
    else
    {
      hScaling[k]->SetMinimum(0) ;
      hScaling[k]->Draw() ;
      hScaling[k]->GetXaxis()->SetTitle("eextra (GeV)") ;
      hScaling[k]->GetYaxis()->SetTitle("entries / 50 MeV") ;
      histogram[5][k]->SetFillColor(kGreen) ;
      histogram[5][k]->Draw("p:e:Sames") ;  
      canvas.Print(TString(plotsdir + "hScale" + (long)k + "_compare.eps")) ;
      canvas.Clear() ;
    }
  }
}


void DRecoDsToKKsPlots::EExtraNXT()
{
  TCanvas canvas("canvas", "", 100, 100, 800, 300) ;
  TH1F*   hEExtraMC = new TH1F("hEExtraMC"  , "hEExtraMC"  , 50, 0.0, 3.0 ) ;
  TH1F* hEExtraData = new TH1F("hEExtraData", "hEExtraData", 50, 0.0, 3.0 ) ;
  const Int_t NNXT = 12 ;
  TH1F* hEExtraNXT[NNXT] ;
  for(Int_t i=0 ; i<NNXT ; i++)
  {
    TString histogramName = TString(TString("hEExtraNNXT") + (long)i) ;
    hEExtraNXT[i] = new TH1F(histogramName, histogramName, 50, 0, 3) ;
    hEExtraNXT[i]->SetFillColor(3) ;
    hEExtraNXT[i]->SetMinimum(0) ;
    hEExtraNXT[i]->GetXaxis()->SetTitle("Extra energy / GeV") ;
    hEExtraNXT[i]->GetYaxis()->SetTitle("entries / 0.06 GeV") ;
  }

  Int_t   fragnpartgen   ;
  Int_t   SPMode ;
  Float_t eextra ;

  TFile* NXTfile[2] ;
  TTree* NXTTree[2] ;
  NXTfile[0] = new TFile("DsToKKs/SP1005/Run5On/Ntuple_New.root", "read");
  NXTfile[1] = new TFile("DsToKKs/Data/Run5On/Ntuple_New.root", "read");
  NXTTree[0] = (TTree*) NXTfile[0]->Get("Ntuple") ;
  NXTTree[1] = (TTree*) NXTfile[1]->Get("Ntuple") ;

  NXTTree[0]->SetBranchAddress( "fragnpartgen"   , &fragnpartgen  ) ;
  NXTTree[0]->SetBranchAddress( "eextra"         , &eextra        ) ;
  NXTTree[1]->SetBranchAddress( "fragnpartgen"   , &fragnpartgen  ) ;
  NXTTree[1]->SetBranchAddress( "eextra"         , &eextra        ) ;  

 // Int_t   currentGoodeventid = -1 ;
 // Int_t   currentRun         = -1 ;
 // Int_t   currentSPMode      = -1 ;

  for(Int_t i=0 ; i<NXTTree[0]->GetEntries() ; i++)
  {
    NXTTree[0]->GetEntry(i) ;
    hEExtraMC->Fill(eextra) ;
    if(fragnpartgen<NNXT) hEExtraNXT[fragnpartgen]->Fill(eextra) ;
  }
  for(Int_t i=0 ; i<NXTTree[1]->GetEntries() ; i++)
  {
    NXTTree[1]->GetEntry(i) ;
    hEExtraData->Fill(eextra) ;
  }

  hEExtraMC->SetFillColor(kGreen) ; 
  hEExtraMC->Draw() ;
  canvas.Print("plots/EExtraPlots/NXTPlots/SP1005_plot.eps") ;
  canvas.Clear() ;

  hEExtraData->SetFillColor(kBlue) ;
  hEExtraData->Draw() ;
  canvas.Print("plots/EExtraPlots/NXTPlots/Data_plot.eps") ;
  canvas.Clear() ;

  for(Int_t i=0 ; i<NNXT ; i++)
  {
    hEExtraNXT[i]->Draw() ;
    canvas.Print(TString(TString("plots/EExtraPlots/NXTPlots/MC_plotNXT") + (long)i + ".eps")) ;
    canvas.Clear() ;
  }
}

TH1F* DRecoDsToKKsPlots::EExtra_Diff(TH1F* HMC, TH1F* HData)
{
  TH1F* HDiff = NULL ;
  
  Float_t HMC_xmin   = HMC->GetXaxis()->GetXmin() ;
  Float_t HMC_xmax   = HMC->GetXaxis()->GetXmax() ;
  Float_t HData_xmin = HData->GetXaxis()->GetXmin() ;
  Float_t HData_xmax = HData->GetXaxis()->GetXmax() ;
  Int_t HMC_Nbins    = HMC->GetXaxis()->GetNbins() ;
  Int_t HData_Nbins  = HData->GetXaxis()->GetNbins() ;

  if( HMC_xmin != HData_xmin ) 
  {
    cout << "Minimum values different " << endl ;
    return HDiff ;
  }
  if( HMC_xmax != HData_xmax ) 
  {
    cout << "Maximum values different " << endl ;
    return HDiff ;
  }
  if( HMC_Nbins != HData_Nbins ) 
  {
    cout << "NBins different " << endl ;
    return HDiff ;
  }
  
  Float_t scale = HData->GetSumOfWeights() / HMC->GetSumOfWeights() ; 
  HMC->Scale(scale) ;
  TCanvas cHMC_Scale("cHMC_Scale", "", 0, 0, 400, 400 ) ;
  HMC->SetMarkerStyle(20) ;
  HMC->Draw() ;
  cHMC_Scale.Print("plots/EExtra_ScaledMCSum.eps") ;
  
  HDiff = new TH1F ( "Htemp", "", HMC_Nbins, HMC_xmin, HMC_xmax ) ;
  for(Int_t i = 1; i<HMC_Nbins+1; i++)
  {
    if(HData->GetBinError(i) == 0 )
    {
      HDiff->SetBinContent(i, 0) ;
    }
    else
    {
      HDiff->SetBinContent(i, (HData->GetBinContent(i)-HMC->GetBinContent(i))/HData->GetBinError(i)  ) ;
    }
  }
  
  TCanvas cHDiff("cHDiff", "", 0, 0, 400, 400 ) ;
  HDiff->SetMarkerStyle(20) ;
  HDiff->Draw("P") ;
  cHDiff.Print("plots/EExtra_HDiff.eps") ;
  
  return HDiff ;
}

void DRecoDsToKKsPlots::FindTrueDecays(Float_t lower, Float_t upper)
{
  Float_t windowLower = lower ;
  Float_t windowUpper = upper ;
  Int_t nBins = 10 ;

  Int_t truemothndaus;
  Int_t truegrandmothlund;
  Int_t truemothlund;
  Int_t truesib1lund;
  Int_t truesib2lund;
  Int_t truesib3lund;
  Int_t truesib4lund;
  Int_t truesib5lund;
  Int_t candklund;
  Float_t ksmassRaw; 
  Float_t ksmassMMCFit2 ;
  Int_t KSelector ;

  TFile f("DsToKKs/SP1005/Run5On/Ntuple_New.root", "read");
  TTree* Ntuple = (TTree*) f.Get("Ntuple");
  
  Ntuple->SetBranchAddress("truemothndaus"    , &truemothndaus);
  Ntuple->SetBranchAddress("truemothlund"     , &truemothlund);
  Ntuple->SetBranchAddress("truesib1lund"     , &truesib1lund);
  Ntuple->SetBranchAddress("truesib2lund"     , &truesib2lund);
  Ntuple->SetBranchAddress("truesib3lund"     , &truesib3lund); 
  Ntuple->SetBranchAddress("truesib4lund"     , &truesib4lund);
  Ntuple->SetBranchAddress("truesib5lund"     , &truesib5lund);
  Ntuple->SetBranchAddress("candklund"        , &candklund);
  Ntuple->SetBranchAddress("ksmassMMRaw2"     , &ksmassRaw);
  Ntuple->SetBranchAddress("ksmassMMCFit2"    , &ksmassMMCFit2);
  Ntuple->SetBranchAddress("truegrandmothlund", &truegrandmothlund);
  Ntuple->SetBranchAddress("KBDTTight"        , &KSelector);
  Int_t nBkgType = 11 ; 
  TH1I * hBkgType = new TH1I("hBkgType", "hBkgType", nBkgType, 1, nBkgType+1) ;

  TH1F* hBkg[nBkgType] ; 
  for(Int_t i=0; i<nBkgType; i++){
    TString HistogramName = "h" ;
    HistogramName = TString( HistogramName + (long) i ) ;
    hBkg[i] = new TH1F(HistogramName, HistogramName, nBins, lower, upper) ;
    hBkg[i]->SetFillColor(i+2) ;
  }

  for(Int_t i=0 ; i<Ntuple->GetEntries() ; i++)
  {
    Ntuple->GetEntry(i) ;
    if( KSelector==0     ) continue ;
    if( ksmassMMCFit2<0  ) continue ;
    if( sqrt(ksmassMMCFit2) > windowUpper ) continue ;
    if( sqrt(ksmassMMCFit2) < windowLower ) continue ;
    
    if( (abs(truemothlund)==431 && abs(truesib1lund*truesib2lund)==321*311) 
	|| (abs(truemothlund)==431 && abs(truesib1lund*truesib2lund*truesib3lund)== 321*311*22 ) )
    {
      hBkgType->Fill(2) ;
      hBkg[1]->Fill(sqrt(ksmassMMCFit2)) ;
    }
    else if( abs(truemothlund)==431 && abs(truesib1lund*truesib2lund)==211*333 )
    {
      hBkgType->Fill(3) ;
      hBkg[2]->Fill(sqrt(ksmassMMCFit2)) ;
    }
    else if( abs(truemothlund)==22 && abs(truesib1lund*truesib2lund)==11*11 )
    {
      hBkgType->Fill(4) ;
      hBkg[3]->Fill(sqrt(ksmassMMCFit2)) ;
    }
    else if( ((abs(truemothlund)==323 || abs(truemothlund)==325) && abs(truesib1lund*truesib2lund)==321*111) || ((abs(truemothlund)==313 || abs(truemothlund)==315) && abs(truesib1lund*truesib2lund)==321*211) )
    {
      hBkgType->Fill(5) ;
      hBkg[4]->Fill(sqrt(ksmassMMCFit2)) ;
    }
    else if( abs(truemothlund)==10022 )
    {
      hBkgType->Fill(6) ;
      hBkg[5]->Fill(sqrt(ksmassMMCFit2)) ;
    }
    else if( (abs(truemothlund)==211 || abs(truemothlund)==321 || abs(truemothlund)==130 ||abs(truemothlund)==310 || abs(truemothlund)==22) && (abs(truesib1lund)>1000 || abs(truesib2lund)>1000 || abs(truesib3lund)>1000 || abs(truesib4lund)>1000 || abs(truesib5lund)>1000 ) )
    {
      hBkgType->Fill(7) ;
      hBkg[6]->Fill(sqrt(ksmassMMCFit2)) ;
    }
    else if( abs(truemothlund)==11 && abs(truesib1lund)==0  )
    {
      hBkgType->Fill(8) ;
      hBkg[7]->Fill(sqrt(ksmassMMCFit2)) ;
    }
    else if( abs(truemothlund)==421 && (abs(truesib1lund)==11 || abs(truesib2lund)==11 || abs(truesib3lund)==11 || abs(truesib4lund)==11 || abs(truesib5lund)==11 || abs(truesib1lund)==13 || abs(truesib2lund)==13 || abs(truesib3lund)==13 || abs(truesib4lund)==13 || abs(truesib5lund)==13)  )
    {
      hBkgType->Fill(9) ;
      hBkg[8]->Fill(sqrt(ksmassMMCFit2)) ;
    }
    else if( abs(truemothlund)==421 && (abs(truesib1lund)!=11 && abs(truesib2lund)!=11 && abs(truesib3lund)!=11 && abs(truesib4lund)!=11 && abs(truesib5lund)!=11 && abs(truesib1lund)!=13 && abs(truesib2lund)!=13 && abs(truesib3lund)!=13 && abs(truesib4lund)!=13 && abs(truesib5lund)!=13)  )
    {
      hBkgType->Fill(10) ;
      hBkg[9]->Fill(sqrt(ksmassMMCFit2)) ;
    }

    else if( abs(truemothlund)==431 && (abs(truesib1lund*truesib2lund)==211*221||abs(truesib1lund*truesib2lund)==211*223)  )
    {
      hBkgType->Fill(11) ;
      hBkg[10]->Fill(sqrt(ksmassMMCFit2)) ;
    }
    else
    {
      hBkgType->Fill(1) ;
      hBkg[0]->Fill(sqrt(ksmassMMCFit2)) ;
      if(sqrt(ksmassMMCFit2)>0.49 && sqrt(ksmassMMCFit2)<0.52){
	cout << "ksmass Moth lund, sib lunds: " << sqrt(ksmassMMCFit2) << " " << truemothlund << " " << truesib1lund << " " << truesib2lund << " " << truesib3lund << " " << truesib4lund << " " << truesib5lund << endl ;
      }
    }
  }

  TString BkgTitle[nBkgType] ;
  BkgTitle[0] = "Other" ;
  BkgTitle[1] = "D_{s}#rightarrowKK_{S}" ;
  BkgTitle[2] = "D_{s}#rightarrow#phi#pi" ;
  BkgTitle[3] = "#gamma#rightarrowee" ;
  BkgTitle[4] = "K^{*} #rightarrow K^{+}#pi" ;
  BkgTitle[5] = "#gamma_{virtual} #rightarrow anything" ;
  BkgTitle[6] = "Machine interactions" ;
  BkgTitle[7] = "e mystery" ;
  BkgTitle[8] = "semileptonic D^{0}" ;
  BkgTitle[9] = "hadronic D^{0}" ;
  BkgTitle[10] = "D_{s}#rightarrow#eta^{(')}#pi" ;

  TLegend legend(0.7,0.6,0.9,0.9) ;

  THStack stack ;
  for(Int_t i=0; i<nBkgType; i++){
    hBkgType->GetXaxis()->SetBinLabel(i+1, BkgTitle[i]) ;
    //if(i==1) continue ;
    if(i!=0) continue ;
    stack.Add(hBkg[i]) ;
    legend.AddEntry(hBkg[nBkgType-1-i], BkgTitle[nBkgType-1-i], "f") ;
  }

  hBkgType->GetXaxis()->SetTitle("Background type") ;
  hBkgType->GetYaxis()->SetTitle("Number of events") ;

  TCanvas c1("c1", "", 100, 100, 500, 500) ;
  stack.Draw() ;
  stack.GetXaxis()->SetTitle("K mass (missing)") ;
  stack.GetYaxis()->SetTitle("Number of events") ;
  legend.SetFillColor(kWhite) ;
  legend.Draw() ;
  c1.Print("plots/bkgTypeStack.eps") ;

  hBkgType->SetFillColor(kBlue) ;
  hBkgType->Draw() ;
  c1.Print("plots/bkgType.eps") ;
}

void DRecoDsToKKsPlots::LogPlots()
{
  TFile f1005("DsToKKs/SP1005/Run5On/Ntuple_New.root", "read") ;
  TFile f998("DsToKKs/SP998/Run5On/Ntuple_New.root", "read") ;
  TFile fData("DsToKKs/Data/Run5On/Ntuple_New.root", "read") ;

  TTree *t1005=(TTree*) f1005.Get("Ntuple") ;
  TTree *t998=(TTree*) f998.Get("Ntuple") ;
  TTree *tData=(TTree*) fData.Get("Ntuple") ;
 

  TCanvas c1("c1", "", 100, 100, 500, 500) ;
  c1.SetLogy() ;
    
  TH1F* h1005=new TH1F("h1005", "h1005", 100, 0.0, 0.75);
  TH1F* h998=new TH1F("h998", "h998", 100, 0.0, 0.75);
  TH1F* hData=new TH1F("hData", "hData", 100, 0.0, 0.75);
  t1005->Draw("log(1+probchi2MMCFit)>>h1005");
  t998->Draw("log(1+probchi2MMCFit)>>h998");
  tData->Draw("log(1+probchi2MMCFit)>>hData");
  h1005->SetFillColor(kGreen);
  h998->SetFillColor(kYellow);
  hData->Sumw2();
  //h1005->Scale(0.472) ;
  h1005->Scale(0.504);
  h998->Scale(0.88);
  THStack stack;
  stack.Add(h998);
  stack.Add(h1005);
  stack.Draw();
  stack.GetXaxis()->SetTitle("log(p(#chi^{2}|n)+1)");
  stack.GetYaxis()->SetTitle("Number of Events");
  TLegend legend(0.7, 0.7, 0.9, 0.9);
  legend.SetFillColor(kWhite);
  legend.AddEntry(h1005, "ccbar", "f");
  legend.AddEntry(h998, "uds", "f");


  hData->SetMarkerStyle(24);
  hData->Draw("p:sames");
  gStyle->SetOptStat(kFALSE);
  hData->SetMarkerSize(1);
  hData->Draw("p:sames");
  legend.Draw(); 
  c1.Print("plots/Logprobchi2.eps") ;
}
