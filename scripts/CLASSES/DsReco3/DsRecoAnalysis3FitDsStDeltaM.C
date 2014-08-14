#include  "../Roo2BodyMassPhaseSpace.h"

void DsRecoAnalysis3::FitDsStDeltaM(){

  filename=_OutputDir+"/DsStDeltaMFit.ps";
  Canvas.Print(filename+"[");


  TH1F HDm("HDm","",500,0,.5);
  //TH1F HDm("HDm","",600,1.90,2.5);
  OpenReducedNtuple();
  ReducedNtuple->Draw("dsstdeltam>>HDm","abs(dsmass-1.968)<.5");
  CloseReducedNtuple();


  Canvas.Clear();
  HDm.Draw("pe");
  Canvas.Print(filename);

  mass.setRange(HDm.GetXaxis()->GetXmin(),HDm.GetXaxis()->GetXmax());
  mass.setBins(HDm.GetNbinsX());
  mass.SetTitle("M_{rec}(DKX) (GeV/c^{2})");

  RooRealVar gamMass("gamMass","gamMass",0.);
  RooRealVar DsMass("DsMass","DsMass",1.9682);//use the mass diff. instead of D_s* mass
  //RooRealVar DsMass("DsMass","DsMass",0.);//use the mass diff. instead of D_s* mass
  Roo2BodyMassPhaseSpace TwoBPS("TwoBPS","TwoBPS",mass,gamMass,DsMass);  
  TwoBPS.setOffset(1.9682);
  massfitter.SetPhaseSpace(&TwoBPS);
  RooPlot*TwoBplot=mass.frame();
  TwoBPS.plotOn(TwoBplot);
  Canvas.Clear();
  TwoBplot->Draw();
  Canvas.Print(filename);

  massfitter.Initialize(&HDm,_OutputDir+"/InitRooFitPars.txt");
  massfitter.Fit();
  massfitter.Save(_OutputDir);

  RooPlot* Plot=massfitter.GetFitPlot();
  Canvas.Clear();
  Plot->Draw();
  Canvas.Print(filename);
  delete Plot;


  Canvas.Print(filename+"]");

}


void DsRecoAnalysis3::FitDsMass(){

  filename=_OutputDir+"/DsMassFit.ps";
  Canvas.Print(filename+"[");


  TH1F HMass("HMass","",5000,0,5);
  SetHistoXYLabels(&HMass,"M_{rec}(DKX#gamma)","GeV");
  OpenReducedNtuple();
  gROOT->cd();
  ReducedNtuple->Draw("dsmass>>HMass","abs(dsstdeltam-.144)<.03");
  //ReducedNtuple->Draw("dsmass>>HMass","abs(dsstmass-2.112)<.300");
  //ReducedNtuple->Draw("dsmass>>HMass","abs(dsstmass-2.112)<.300&&abs(dsstdeltam-.141)<.02");
  CloseReducedNtuple();


  Canvas.Clear();
  HMass.Draw("pe");
  Canvas.Print(filename);

  mass.setRange(HMass.GetXaxis()->GetXmin(),HMass.GetXaxis()->GetXmax());
  mass.setBins(HMass.GetNbinsX());

  RooRealVar gamMass("gamMass","gamMass",0.);
  RooRealVar DsMass("DsMass","DsMass",1.9682);//use the mass diff. instead of D_s* mass
  //RooRealVar DsMass("DsMass","DsMass",0.);//use the mass diff. instead of D_s* mass
  Roo2BodyMassPhaseSpace TwoBPS("TwoBPS","TwoBPS",mass,gamMass,DsMass);  
  TwoBPS.setOffset(1.9682);
  massfitter.SetPhaseSpace(&TwoBPS);
  RooPlot*TwoBplot=mass.frame();
  TwoBPS.plotOn(TwoBplot);
  Canvas.Clear();
  TwoBplot->Draw();
  Canvas.Print(filename);

  massfitter.Initialize(&HMass,_OutputDir+"/InitRooFitParsDs.txt");
  massfitter.Fit();
  massfitter.Save(_OutputDir);

  RooPlot* Plot=massfitter.GetFitPlot();
  Canvas.Clear();
  Plot->Draw();
  Canvas.Print(filename);
  delete Plot;


  Canvas.Print(filename+"]");

}
