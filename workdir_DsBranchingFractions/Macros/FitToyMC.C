#define NXT 7
#include <TH1F.h>
#include <TH2F.h>
#include <TCanvas.h>
#include <TFile.h>
#include <RooRealVar.h>
#include <RooFitResult.h>
#include <TTree.h>
#include <TF1.h>
#include "DsInclusiveAnalysisInit.C"
//#include "../tmp/Linux24SL3_i386_gcc323/DRecoilToolsUser/NtupleCode/DRecoDsInclusiveAnalysis.hh"


void FitToyMC(Int_t id){
  DRecoDsInclusiveAnalysis*a=analyzeDsInclusive(2,0);
  
  TFile DataFile(TString("DsTolnu/SP1005Skim/DsReco/Final/ToyMC/ToyData")+(long)id+".root","read");
  gROOT->cd();
  TH2F* HDsMassDataWS=(TH2F*)DataFile.Get("HDsMassVsNXRWS");  
  TH2F* HDsMassData=(TH2F*)DataFile.Get("HDsMassVsNXR");
  if(!HDsMassDataWS||!HDsMassDataWS){
    cout<<"No histograms"<<endl;
    return;
  }

  a->RooFitDsMassvsnXRvsnXTSimulExpModel(HDsMassDataWS,HDsMassData,TString("ToyMC/FitResult")+(long)id);

  
  DataFile.Close();
}

void CreateToyMCNtuple(Int_t first,Int_t last){
  
  TFile NtupleFile("DsTolnu/SP1005Skim/DsReco/Final/ToyMC/ToyNtuple.root","recreate");
  gROOT->cd();
  TTree ToyNtuple("ToyNtuple","ToyNtuple");
  Int_t status;ToyNtuple.Branch("status",&status,"status/I");
  Int_t covqual;ToyNtuple.Branch("covqual",&covqual,"covqual/I");

  Float_t TotYieldGen;ToyNtuple.Branch("TotYieldGen",&TotYieldGen,"TotYieldGen/F");
  Float_t TotYieldReco;ToyNtuple.Branch("TotYieldReco",&TotYieldReco,"TotYieldReco/F");
  Float_t TotYieldEr;ToyNtuple.Branch("TotYieldEr",&TotYieldEr,"TotYieldEr/F");

  Float_t nXTCoefGen[NXT];
  Float_t nXTCoefReco[NXT];
  for(Int_t j=0;j<NXT;j++){
    ToyNtuple.Branch(TString("nXTCoefGen")+(long)j,&nXTCoefGen[j],TString("nXTCoefGen")+(long)j+"/F");
    ToyNtuple.Branch(TString("nXTCoefReco")+(long)j,&nXTCoefReco[j],TString("nXTCoefReco")+(long)j+"/F");
  }
  
  Float_t betaGen,gammaGen;
  ToyNtuple.Branch("betaGen",&betaGen,"betaGen/F");
  ToyNtuple.Branch("gammaGen",&gammaGen,"gammaGen/F");

  Float_t betaReco,gammaReco;
  ToyNtuple.Branch("betaReco",&betaReco,"betaReco/F");
  ToyNtuple.Branch("gammaReco",&gammaReco,"gammaReco/F");

  Float_t betaEr,gammaEr;
  ToyNtuple.Branch("betaEr",&betaEr,"betaEr/F");
  ToyNtuple.Branch("gammaEr",&gammaEr,"gammaEr/F");



  RooFitResult* result;
  for(Int_t e=first;e<=last;e++){
    //open the file with generated parameters
    TFile DataFile(TString("DsTolnu/SP1005Skim/DsReco/Final/ToyMC/ToyData")+(long)e+".root","read");
    if(DataFile.IsZombie())continue;
    //open the file with reconstructed parameters
    TFile ResultsFile(TString("DsTolnu/SP1005Skim/DsReco/Final/ToyMC/FitResult")+(long)e+".root","read");
    if(ResultsFile.IsZombie())continue;
    gROOT->cd();

    //get histo with gen pars 
    TH1F* HGenPars=(TH1F*)DataFile.Get("HPars");
    TotYieldGen=HGenPars->GetBinContent(1);
    betaGen=HGenPars->GetBinContent(11);
    gammaGen=HGenPars->GetBinContent(12);  
    for(Int_t j=0;j<NXT;j++){
      nXTCoefGen[j]=HGenPars->GetBinContent(30+j);    
    }
  

    //get results histograms
    result=(RooFitResult*)ResultsFile.Get("fitresult");
    status=result->status();
    covqual=result->covQual();
    
    TH1F* HDsMassData=(TH1F*)ResultsFile.Get("HDsMassData");    
    TH1F* TotSigPdfHistoFit=(TH1F*)ResultsFile.Get("TotSigPdfHistoFit");
    TotYieldReco=TotSigPdfHistoFit->GetBinContent(0)/HDsMassData->Integral();
    TotYieldEr=TotSigPdfHistoFit->GetBinError(0)/HDsMassData->Integral();

    TH1F* HnXTweights=(TH1F*)ResultsFile.Get("HnXTweights");
    for(Int_t j=0;j<NXT;j++){
      nXTCoefReco[j]=HnXTweights->GetBinContent(j+1);    
    }

    betaReco=((RooRealVar*)result->floatParsFinal().find("nXTb"))->getVal();
    betaEr=((RooRealVar*)result->floatParsFinal().find("nXTb"))->getError();
    gammaReco=((RooRealVar*)result->floatParsFinal().find("nXTg"))->getVal();
    gammaEr=((RooRealVar*)result->floatParsFinal().find("nXTg"))->getError();

    //
    ToyNtuple.Fill();

    DataFile.Close();
    ResultsFile.Close();
  }
  
  NtupleFile.cd();
  ToyNtuple.Write();
  NtupleFile.ls();
  NtupleFile.Close();
}


void PlotToyMCStatistics(){
    
  TFile NtupleFile("DsTolnu/SP1005Skim/DsReco/Final/ToyMC/ToyNtuple.root","read");
  gROOT->cd();
  TTree* ToyNtuple=(TTree*)NtupleFile.Get("ToyNtuple");
  
  TCanvas C("C");
  C.Print("DsTolnu/SP1005Skim/DsReco/Final/ToyMC/ToyMCPlots.ps[");

  Float_t xmin=-4.;
  Float_t xmax=4.;
  Float_t nbins=20;
  
  C.Clear();
  TH2F HYield("HYield","",20,.15,.25,20,.1,.3);
  HYield.GetYaxis()->SetTitle("Reco. Yield");
  HYield.GetXaxis()->SetTitle("Gen. Yield");
  ToyNtuple->Draw("TotYieldReco:TotYieldGen>>HYield","status==0&&covqual==3","colz");
  C.Print("DsTolnu/SP1005Skim/DsReco/Final/ToyMC/ToyMCPlots.ps");


  TF1 FGauss("FGauss","[3]*[0]*exp(-.5*(x-[1])**2/[2]**2)/([2]*2.5066)",xmin,xmax);
  FGauss.SetParLimits(0,0,10000);
  FGauss.SetParLimits(1,xmin,xmax);
  FGauss.SetParLimits(2,.01,(xmax-xmin));
  FGauss.FixParameter(3,(xmax-xmin)/nbins);
  FGauss.SetParNames("Norm","Mean","Sigma");
  C.Clear();
  TH1F HTotYieldPull("HTotYieldPull","",nbins,xmin,xmax);
  ToyNtuple->Draw("(TotYieldReco-TotYieldGen)/TotYieldEr>>HTotYieldPull","status==0&&covqual==3");
  HTotYieldPull.Fit(&FGauss,"QILL","",xmin,xmax);
  HTotYieldPull.GetXaxis()->SetTitle("(Reco-Gen)/Error");
  C.Print("DsTolnu/SP1005Skim/DsReco/Final/ToyMC/ToyMCPlots.ps");


//   TH1F* HnXTCoefPull[NXT];
//   for(Int_t j=0;j<NXT;j++){

//     C.Clear();
//     ToyNtuple->Draw(TString("nXTCoefReco")+(long)j,"status==0&&covqual==3","p");
//     ToyNtuple->Draw(TString("nXTCoefGen")+(long)j,"status==0&&covqual==3","same");
//     C.Print("DsTolnu/SP1005Skim/DsReco/Final/ToyMC/ToyMCPlots.ps");    

//     C.Clear();
//     HnXTCoefPull[j]=new TH1F(TString("HnXTCoefPull")+(long)j,TString("nXT=")+(long)j+" Weight Pull",nbins,xmin,xmax);
//     ToyNtuple->Draw(TString("(nXTCoefReco")+(long)j+"-nXTCoefGen"+(long)j+")/nXTCoefEr"+(long)j+">>HnXTCoefPull"+(long)j,
// 		    "status==0&&covqual==3");
//     FGauss.SetParameter(1,HnXTCoefPull[j]->GetMean());
//     FGauss.SetParameter(2,HnXTCoefPull[j]->GetRMS());
//     HnXTCoefPull[j]->Fit(&FGauss,"QILL","",xmin,xmax);
// //     HnXTCoefPull[j]->GetXaxis()->SetLabelSize(.1);
// //     HnXTCoefPull[j]->GetXaxis()->SetNdivisions(5);
// //     HnXTCoefPull[j]->GetYaxis()->SetLabelSize(.08);
// //     HnXTCoefPull[j]->GetYaxis()->SetNdivisions(3);
//     HnXTCoefPull[j]->GetXaxis()->SetTitle("(Reco-Gen)/Error");
//     HnXTCoefPull[j]->Draw(); 
//     //    C.Update();
// //     TPaveStats*st=(TPaveStats*)(HnXTCoefPull[j]->FindObject("stats"));
// //     st->SetTextSize(.07);
// //     st->SetX1NDC(.45);
// //     st->SetX2NDC(.8);
// //     st->SetY1NDC(.2);
// //     st->SetY2NDC(.8);
// //    HnXTCoefPull[j]->Draw(); 
//     C.Print("DsTolnu/SP1005Skim/DsReco/Final/ToyMC/ToyMCPlots.ps");    
//     delete HnXTCoefPull[j];
//   }
   
 
  C.Clear();
  TH2F HBetaCorrel("HBetaCorrel","",20,0,5,20,-2,7);
  HBetaCorrel.GetYaxis()->SetTitle("Reco. #beta");
  HBetaCorrel.GetXaxis()->SetTitle("Gen. #beta");
  ToyNtuple->Draw("betaReco:betaGen>>HBetaCorrel","status==0&&covqual==3","colz");
  C.Print("DsTolnu/SP1005Skim/DsReco/Final/ToyMC/ToyMCPlots.ps");  
  C.Clear();
  TH1F HBetaPull("HBetaPull","",nbins,xmin,xmax);
  ToyNtuple->Draw("(betaReco-betaGen)/betaEr>>HBetaPull","status==0&&covqual==3");
  HBetaPull.Fit(&FGauss,"QILL","",xmin,xmax);
  HBetaPull.GetXaxis()->SetTitle("(Reco-Gen)/Error");
  C.Print("DsTolnu/SP1005Skim/DsReco/Final/ToyMC/ToyMCPlots.ps");    

  C.Clear();
  TH2F HGammaCorrel("HGammaCorrel","",20,0,2,20,-1,3);
  HGammaCorrel.GetYaxis()->SetTitle("Reco. #gamma");
  HGammaCorrel.GetXaxis()->SetTitle("Gen. #gamma");
  ToyNtuple->Draw("gammaReco:gammaGen>>HGammaCorrel","status==0&&covqual==3","colz");
  C.Print("DsTolnu/SP1005Skim/DsReco/Final/ToyMC/ToyMCPlots.ps");
  C.Clear();
  TH1F HGammaPull("HGammaPull","",nbins,xmin,xmax);
  ToyNtuple->Draw("(gammaReco-gammaGen)/gammaEr>>HGammaPull","status==0&&covqual==3");
  HGammaPull.Fit(&FGauss,"QILL","",xmin,xmax);
  HGammaPull.GetXaxis()->SetTitle("(Reco-Gen)/Error");
  C.Print("DsTolnu/SP1005Skim/DsReco/Final/ToyMC/ToyMCPlots.ps");    

  C.Print("DsTolnu/SP1005Skim/DsReco/Final/ToyMC/ToyMCPlots.ps]");

  NtupleFile.Close();
}



// void PlotToyMCStatistics(){
    
//   TFile NtupleFile("DsTolnu/SP1005Skim/DsReco/Final/ToyMC/ToyNtuple.root","read");
//   gROOT->cd();
//   TTree* ToyNtuple=(TTree*)NtupleFile.Get("ToyNtuple");
  
//   TCanvas C("C");
//   C.Print("DsTolnu/SP1005Skim/DsReco/Final/ToyMC/ToyMCPlots.ps[");

//   Float_t xmin=-4.;
//   Float_t xmax=4.;
//   Float_t nbins=20;
  
//   C.Clear();
//   ToyNtuple->Draw("TotYieldReco","status==0&&covqual==3","p");
//   ToyNtuple->Draw("TotYieldGen","status==0&&covqual==3","same");
//   C.Print("DsTolnu/SP1005Skim/DsReco/Final/ToyMC/ToyMCPlots.ps");


//   TF1 FGauss("FGauss","[3]*[0]*exp(-.5*(x-[1])**2/[2]**2)/([2]*2.5066)",xmin,xmax);
//   FGauss.SetParLimits(0,0,10000);
//   FGauss.SetParLimits(1,xmin,xmax);
//   FGauss.SetParLimits(2,.01,(xmax-xmin));
//   FGauss.FixParameter(3,(xmax-xmin)/nbins);
//   FGauss.SetParNames("Norm","Mean","Sigma");
//   C.Clear();
//   TH1F HTotYield("HTotYield","Total Yield Pull",nbins,xmin,xmax);
//   ToyNtuple->Draw("(TotYieldReco-TotYieldGen)/TotYieldEr>>HTotYield","status==0&&covqual==3");
//   HTotYield.Fit(&FGauss,"QILL","",xmin,xmax);
//   HTotYield.GetXaxis()->SetTitle("(Reco-Gen)/Error");
//   C.Print("DsTolnu/SP1005Skim/DsReco/Final/ToyMC/ToyMCPlots.ps");


//   TH1F* HnXTCoefPull[NXT];
//   for(Int_t j=0;j<NXT;j++){

//     C.Clear();
//     ToyNtuple->Draw(TString("nXTCoefReco")+(long)j,"status==0&&covqual==3","p");
//     ToyNtuple->Draw(TString("nXTCoefGen")+(long)j,"status==0&&covqual==3","same");
//     C.Print("DsTolnu/SP1005Skim/DsReco/Final/ToyMC/ToyMCPlots.ps");    

//     C.Clear();
//     HnXTCoefPull[j]=new TH1F(TString("HnXTCoefPull")+(long)j,TString("nXT=")+(long)j+" Weight Pull",nbins,xmin,xmax);
//     ToyNtuple->Draw(TString("(nXTCoefReco")+(long)j+"-nXTCoefGen"+(long)j+")/nXTCoefEr"+(long)j+">>HnXTCoefPull"+(long)j,
// 		    "status==0&&covqual==3");
//     FGauss.SetParameter(1,HnXTCoefPull[j]->GetMean());
//     FGauss.SetParameter(2,HnXTCoefPull[j]->GetRMS());
//     HnXTCoefPull[j]->Fit(&FGauss,"QILL","",xmin,xmax);
// //     HnXTCoefPull[j]->GetXaxis()->SetLabelSize(.1);
// //     HnXTCoefPull[j]->GetXaxis()->SetNdivisions(5);
// //     HnXTCoefPull[j]->GetYaxis()->SetLabelSize(.08);
// //     HnXTCoefPull[j]->GetYaxis()->SetNdivisions(3);
//     HnXTCoefPull[j]->GetXaxis()->SetTitle("(Reco-Gen)/Error");
//     HnXTCoefPull[j]->Draw(); 
//     //    C.Update();
// //     TPaveStats*st=(TPaveStats*)(HnXTCoefPull[j]->FindObject("stats"));
// //     st->SetTextSize(.07);
// //     st->SetX1NDC(.45);
// //     st->SetX2NDC(.8);
// //     st->SetY1NDC(.2);
// //     st->SetY2NDC(.8);
// //    HnXTCoefPull[j]->Draw(); 
//     C.Print("DsTolnu/SP1005Skim/DsReco/Final/ToyMC/ToyMCPlots.ps");    
//     delete HnXTCoefPull[j];
//   }
   


// //   C.Clear();
// //   C.Divide(2,NXT/2);
// //   C.cd(1);ToyNtuple->Draw("mean1Reco","status==0&&covqual==3","p");ToyNtuple->Draw("mean1Gen","status==0&&covqual==3","same");
// //   C.cd(2);ToyNtuple->Draw("sigma1Reco","status==0&&covqual==3","p");ToyNtuple->Draw("sigma1Gen","status==0&&covqual==3","same");
// //   C.cd(3);ToyNtuple->Draw("fracReco","status==0&&covqual==3","p");ToyNtuple->Draw("fracGen","status==0&&covqual==3","same");
// //   C.cd(4);ToyNtuple->Draw("mean2Reco","status==0&&covqual==3","p");ToyNtuple->Draw("mean2Gen","status==0&&covqual==3","same");
// //   C.cd(5);ToyNtuple->Draw("sigma2Reco","status==0&&covqual==3","p");ToyNtuple->Draw("sigma2Gen","status==0&&covqual==3","same");
// //   C.Print("DsTolnu/SP1005Skim/DsReco/Final/ToyMC/ToyMCPlots.ps");
  
// //   TH1F* HWeightParPull[NXT]; 
// //   HWeightParPull[0]=new TH1F(TString("HWeightParPull")+(long)0,"mean1 Pull",nbins,xmin,xmax);
// //   ToyNtuple->Draw(TString("(mean1Reco-mean1Gen)/mean1Er>>HWeightParPull")+(long)0,"status==0&&covqual==3");
// //   HWeightParPull[1]=new TH1F(TString("HWeightParPull")+(long)1,"sigma1 Pull",nbins,xmin,xmax);
// //   ToyNtuple->Draw(TString("(sigma1Reco-sigma1Gen)/sigma1Er>>HWeightParPull")+(long)1,"status==0&&covqual==3");
// //   HWeightParPull[2]=new TH1F(TString("HWeightParPull")+(long)2,"frac Pull",nbins,xmin,xmax);
// //   ToyNtuple->Draw(TString("(fracReco-fracGen)/fracEr>>HWeightParPull")+(long)2,"status==0&&covqual==3");
// //   HWeightParPull[3]=new TH1F(TString("HWeightParPull")+(long)3,"mean2 Pull",nbins,xmin,xmax);
// //   ToyNtuple->Draw(TString("(mean2Reco-mean2Gen)/mean2Er>>HWeightParPull")+(long)3,"status==0&&covqual==3");
// //   HWeightParPull[4]=new TH1F(TString("HWeightParPull")+(long)4,"sigma2 Pull",nbins,xmin,xmax);
// //   ToyNtuple->Draw(TString("(sigma2Reco-sigma2Gen)/sigma2Er>>HWeightParPull")+(long)4,"status==0&&covqual==3");
// //   C.Clear();
// //   C.Divide(2,NXT/2);
// //   for(Int_t j=0;j<NXT-1;j++){
// //     C.cd(j+1);
// //     FGauss.SetParameter(1,HWeightParPull[j]->GetMean());
// //     FGauss.SetParameter(2,HWeightParPull[j]->GetRMS());
// //     HWeightParPull[j]->Fit(&FGauss,"QILL","",xmin,xmax);
// //     HWeightParPull[j]->GetXaxis()->SetLabelSize(.1);
// //     HWeightParPull[j]->GetXaxis()->SetNdivisions(5);
// //     HWeightParPull[j]->GetYaxis()->SetLabelSize(.08);
// //     HWeightParPull[j]->GetYaxis()->SetNdivisions(3);
// //     HWeightParPull[j]->GetXaxis()->SetTitle("(Reco-Gen)/Error");
// //     HWeightParPull[j]->Draw(); 
// //     C.Update();
// //     TPaveStats*st=(TPaveStats*)(HWeightParPull[j]->FindObject("stats"));
// //     st->SetTextSize(.07);
// //     st->SetX1NDC(.45);
// //     st->SetX2NDC(.8);
// //     st->SetY1NDC(.2);
// //     st->SetY2NDC(.8);
// //     HWeightParPull[j]->Draw(); 
// //   }
// //   C.Print("DsTolnu/SP1005Skim/DsReco/Final/ToyMC/ToyMCPlots.ps");

// //   for(Int_t j=0;j<NXT-1;j++)
// //     delete HWeightParPull[j];
  


//   C.Print("DsTolnu/SP1005Skim/DsReco/Final/ToyMC/ToyMCPlots.ps]");

//   NtupleFile.Close();
// }



// void CreateToyMCNtuple(Int_t first,Int_t last){
  
//   TFile NtupleFile("DsTolnu/SP1005Skim/DsReco/Final/ToyMC/ToyNtuple.root","recreate");
//   gROOT->cd();
//   TTree ToyNtuple("ToyNtuple","ToyNtuple");
//   Int_t status;ToyNtuple.Branch("status",&status,"status/I");
//   Int_t covqual;ToyNtuple.Branch("covqual",&covqual,"covqual/I");

//   Float_t TotYieldGen;ToyNtuple.Branch("TotYieldGen",&TotYieldGen,"TotYieldGen/F");
//   Float_t TotYieldReco;ToyNtuple.Branch("TotYieldReco",&TotYieldReco,"TotYieldReco/F");
//   Float_t TotYieldEr;ToyNtuple.Branch("TotYieldEr",&TotYieldEr,"TotYieldEr/F");

//   Float_t nXTCoefGen[NXT];
//   Float_t nXTCoefReco[NXT];
//   Float_t nXTCoefEr[NXT];
//   for(Int_t j=0;j<NXT;j++){
//     ToyNtuple.Branch(TString("nXTCoefGen")+(long)j,&nXTCoefGen[j],TString("nXTCoefGen")+(long)j+"/F");
//     ToyNtuple.Branch(TString("nXTCoefReco")+(long)j,&nXTCoefReco[j],TString("nXTCoefReco")+(long)j+"/F");
//     ToyNtuple.Branch(TString("nXTCoefEr")+(long)j,&nXTCoefEr[j],TString("nXTCoefEr")+(long)j+"/F");
//   }
  
//   Float_t mean1Gen,mean2Gen;
//   ToyNtuple.Branch("mean1Gen",&mean1Gen,"mean1Gen/F");
//   ToyNtuple.Branch("mean2Gen",&mean2Gen,"mean2Gen/F");
//   Float_t sigma1Gen,sigma2Gen;
//   ToyNtuple.Branch("sigma1Gen",&sigma1Gen,"sigma1Gen/F");
//   ToyNtuple.Branch("sigma2Gen",&sigma2Gen,"sigma2Gen/F");
//   Float_t fracGen;
//   ToyNtuple.Branch("fracGen",&fracGen,"fracGen/F");

//   Float_t mean1Reco,mean2Reco;
//   ToyNtuple.Branch("mean1Reco",&mean1Reco,"mean1Reco/F");
//   ToyNtuple.Branch("mean2Reco",&mean2Reco,"mean2Reco/F");
//   Float_t sigma1Reco,sigma2Reco;
//   ToyNtuple.Branch("sigma1Reco",&sigma1Reco,"sigma1Reco/F");
//   ToyNtuple.Branch("sigma2Reco",&sigma2Reco,"sigma2Reco/F");
//   Float_t fracReco;
//   ToyNtuple.Branch("fracReco",&fracReco,"fracReco/F");

//   Float_t mean1Er,mean2Er;
//   ToyNtuple.Branch("mean1Er",&mean1Er,"mean1Er/F");
//   ToyNtuple.Branch("mean2Er",&mean2Er,"mean2Er/F");
//   Float_t sigma1Er,sigma2Er;
//   ToyNtuple.Branch("sigma1Er",&sigma1Er,"sigma1Er/F");
//   ToyNtuple.Branch("sigma2Er",&sigma2Er,"sigma2Er/F");
//   Float_t fracEr;
//   ToyNtuple.Branch("fracEr",&fracEr,"fracEr/F");



//   RooFitResult* result;
//   for(Int_t e=first;e<=last;e++){
//     //open the file with generated parameters
//     TFile DataFile(TString("DsTolnu/SP1005Skim/DsReco/Final/ToyMC/ToyData")+(long)e+".root","read");
//     if(DataFile.IsZombie())continue;
//     //open the file with reconstructed parameters
//     TFile ResultsFile(TString("DsTolnu/SP1005Skim/DsReco/Final/ToyMC/FitResult")+(long)e+".root","read");
//     if(ResultsFile.IsZombie())continue;
//     gROOT->cd();

//     //get histo with gen pars 
//     TH1F* HGenPars=(TH1F*)DataFile.Get("HPars");
//     TotYieldGen=HGenPars->GetBinContent(1);
//     nXTCoefGen[NXT-1]=1.;    
//     for(Int_t j=0;j<NXT-1;j++){
//       nXTCoefGen[j]=HGenPars->GetBinContent(30+j);    
//       nXTCoefGen[NXT-1]-=nXTCoefGen[j];
//     }

//     mean1Gen=HGenPars->GetBinContent(60+0);
//     sigma1Gen=HGenPars->GetBinContent(60+1);
//     fracGen=HGenPars->GetBinContent(60+2);
//     mean2Gen=HGenPars->GetBinContent(60+3);
//     sigma2Gen=HGenPars->GetBinContent(60+4);

//     //get results histograms
//     result=(RooFitResult*)ResultsFile.Get("fitresult");
//     status=result->status();
//     covqual=result->covQual();
    
//     TH1F* HDsMassData=(TH1F*)ResultsFile.Get("HDsMassData");    
//     TH1F* TotSigPdfHistoFit=(TH1F*)ResultsFile.Get("TotSigPdfHistoFit");
//     TotYieldReco=TotSigPdfHistoFit->GetBinContent(0)/HDsMassData->Integral();
//     TotYieldEr=TotSigPdfHistoFit->GetBinError(0)/HDsMassData->Integral();

//     TH1F* HnXTweights=(TH1F*)ResultsFile.Get("HnXTweights");
//     TH1F* HnXTweightsLoEr=(TH1F*)ResultsFile.Get("HnXTweightsLoEr");
//     TH1F* HnXTweightsHiEr=(TH1F*)ResultsFile.Get("HnXTweightsHiEr");    
//     for(Int_t j=0;j<NXT;j++){
//       nXTCoefReco[j]=HnXTweights->GetBinContent(j+1);    
//       nXTCoefEr[j]=HnXTweights->GetBinError(j+1);
//       if(HnXTweightsLoEr&&HnXTweightsHiEr){
// 	//nXTCoefEr[j]=(HnXTweightsLoEr->GetBinContent(j+1)+HnXTweightsHiEr->GetBinContent(j+1))/2.;
// 	//nXTCoefEr[j]=(HnXTweightsLoEr->GetBinContent(j+1) > HnXTweightsHiEr->GetBinContent(j+1)) ?
// 	//HnXTweightsLoEr->GetBinContent(j+1) : HnXTweightsHiEr->GetBinContent(j+1);
// 	nXTCoefEr[j]=( nXTCoefReco[j] - nXTCoefGen[j] >  0. ) ?
// 	  HnXTweightsLoEr->GetBinContent(j+1) : HnXTweightsHiEr->GetBinContent(j+1);
//       }
//     }

//     TH1F* HnXTModelPar=(TH1F*)ResultsFile.Get("HnXTModelPar");
//     if(HnXTModelPar){
//       mean1Reco=HnXTModelPar->GetBinContent(1);     mean1Er=HnXTModelPar->GetBinError(1);
//       sigma1Reco=HnXTModelPar->GetBinContent(2);     sigma1Er=HnXTModelPar->GetBinError(2);    
//       fracReco=HnXTModelPar->GetBinContent(3);      fracEr=HnXTModelPar->GetBinError(3);     
//       mean2Reco=HnXTModelPar->GetBinContent(4);     mean2Er=HnXTModelPar->GetBinError(4);
//       sigma2Reco=HnXTModelPar->GetBinContent(5);     sigma2Er=HnXTModelPar->GetBinError(5);    
//     }

//     //
//     ToyNtuple.Fill();

//     DataFile.Close();
//     ResultsFile.Close();
//   }
  
//   NtupleFile.cd();
//   ToyNtuple.Write();
//   NtupleFile.ls();
//   NtupleFile.Close();
// }

