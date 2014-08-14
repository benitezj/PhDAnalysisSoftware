#include "BToDstEtaAnalysis.h"


Int_t BToDstEtaAnalysis::MakeEtaMass(){

  if(!OpenReducedNtuple())return 0;

  TFile file(_OutputDir+"/EtaMassPlot.root","recreate");

  TH1F HEtaMass("HEtaMass","",100,1.8,1.9);
  ReducedNtuple->Draw("dmass>>HEtaMass");
  HEtaMass.Write();

  file.ls();
  file.Close();

  CloseReducedNtuple();

  return 1;
}

Int_t BToDstEtaAnalysis::PlotEtaMass(){

  TFile file(_OutputDir+"/EtaMassPlot.root","read");
  TH1F* HEtaMass=(TH1F*)file.Get("HEtaMass");
  gROOT->cd();


  if(!massfitter.Initialize(HEtaMass,_OutputDir+"/InitRooFitParsMass.txt"))return 0;
  if(massfitter.Fit()!=0)return 0;
  massfitter.Save(_OutputDir);
  delete HEtaMass;

  file.Close();
  return 1;
}
