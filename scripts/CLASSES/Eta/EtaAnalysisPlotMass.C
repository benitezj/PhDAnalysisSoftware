#include "EtaAnalysis.h"


Int_t EtaAnalysis::MakeMass(){

  if(!OpenReducedNtuple("Ntuple"))return 0;

  TFile file(_OutputDir+"/MassPlot.root","recreate");

  TH1F HMass("HMass","",100,1.8,1.9);
  ReducedNtuple->Draw("dmass>>HMass");
  HMass.Write();

  file.ls();
  file.Close();

  CloseReducedNtuple();

  return 1;
}

Int_t EtaAnalysis::PlotMass(){

  TFile file(_OutputDir+"/MassPlot.root","read");
  TH1F* HMass=(TH1F*)file.Get("HMass");
  gROOT->cd();


  if(!massfitter.Initialize(HMass,_OutputDir+"/InitRooFitParsMass.txt"))return 0;
  if(massfitter.Fit()!=0)return 0;
  massfitter.Save(_OutputDir);
  delete HMass;

  file.Close();
  return 1;
}
