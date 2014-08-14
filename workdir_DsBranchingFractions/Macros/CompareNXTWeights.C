#include <TFile.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TH2F.h>

void CompareNXTWeights(){

  TFile FSigW("fDs2009/Denominator/NominalFitResults/MuNuEffDenominator.root");
  TFile FGenericW("DsTolnu/SP1005Skim/DsReco/Final/HistosForFit.root");

  TH1F HSigW("HSigW","",7,-.5,6.5);
  HSigW.Sumw2();
  TH1F HGenW("HGenW","",7,-.5,6.5);
  HGenW.Sumw2();

  for(Int_t i=0;i<7;i++){
    //TH1F* HSignXT=(TH1F*)FSigW.Get(TString("hNXT")+(long)i+"DsToMuNu_EExtraCut");
    TH1F* HSignXT=(TH1F*)FSigW.Get(TString("HDsMassTrue2D_nXT")+(long)i);
    HSigW.SetBinContent(i+1,HSignXT->Integral());
    HSigW.SetBinError(i+1,sqrt(HSignXT->Integral()));
    
    TH2F* HGennXT=(TH2F*)FGenericW.Get(TString("HDsMassTrue2D_nXT")+(long)i);
    HGenW.SetBinContent(i+1,HGennXT->Integral());
    HGenW.SetBinError(i+1,sqrt(HGennXT->Integral()));
  }

  TCanvas C("C");
  C.Print("CompareNXTWeights.ps[");

  C.Clear();
  HGenW.SetLineColor(2);
  HGenW.Draw("hist");
  C.Print("CompareNXTWeights.ps");

  C.Clear();
  HSigW.SetLineColor(1);
  HSigW.Draw("hist");
  C.Print("CompareNXTWeights.ps");

  HGenW.Scale(1./HGenW.Integral());
  HSigW.Scale(1./HSigW.Integral());

  C.Clear();
  HSigW.Draw("hist");
  HGenW.Draw("hist same");
  C.Print("CompareNXTWeights.ps");

  C.Print("CompareNXTWeights.ps]");

}




void CompareNXTWeightsKKpi(){

  TFile FSigW("DsTolnu/KKpiEff/HistosForFit.root");
  TFile FGenericW("fDs2009/Denominator/NominalFitResults/MuNuEffDenominator.root");

  TH1F HSigW("HSigW","",7,-.5,6.5);
  HSigW.Sumw2();
  TH1F HGenW("HGenW","",7,-.5,6.5);
  HGenW.Sumw2();

  for(Int_t i=0;i<7;i++){
    TH1F* HSignXT=(TH1F*)FSigW.Get(TString("HDsMassTrue2D_nXT")+(long)i);
    HSigW.SetBinContent(i+1,HSignXT->Integral());
    HSigW.SetBinError(i+1,sqrt(HSignXT->Integral()));
    
    TH2F* HGennXT=(TH2F*)FGenericW.Get(TString("HDsMassTrue2D_nXT")+(long)i);
    HGenW.SetBinContent(i+1,HGennXT->Integral());
    HGenW.SetBinError(i+1,sqrt(HGennXT->Integral()));
  }

  TCanvas C("C");
  C.Print("CompareNXTWeightsKKpi.ps[");

  C.Clear();
  HGenW.SetLineColor(2);
  HGenW.Draw("hist");
  C.Print("CompareNXTWeightsKKpi.ps");

  C.Clear();
  HSigW.SetLineColor(1);
  HSigW.Draw("hist");
  C.Print("CompareNXTWeightsKKpi.ps");

  HGenW.Scale(1./HGenW.Integral());
  HSigW.Scale(1./HSigW.Integral());

  C.Clear();
  HSigW.Draw("hist");
  HGenW.Draw("hist same");
  C.Print("CompareNXTWeightsKKpi.ps");

  C.Print("CompareNXTWeightsKKpi.ps]");

}
