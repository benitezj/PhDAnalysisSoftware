#include  "../Roo2BodyMassPhaseSpace.h"

void DsTolnuAnalysis::FitDsStDeltaM(){

  filename=_OutputDir+"/DsStDeltaMFit.ps";
  Canvas.Print(filename+"[");


  TH1F HDm("HDm","",500,0,.5);
  //TH1F HDm("HDm","",600,1.90,2.5);
  OpenReducedNtuple();
  ReducedNtuple->Draw("dsstdeltam>>HDm");
  CloseReducedNtuple();


  Canvas.Clear();
  HDm.Draw("pe");
  Canvas.Print(filename);

  mass.setRange(HDm.GetXaxis()->GetXmin(),HDm.GetXaxis()->GetXmax());
  mass.setBins(HDm.GetNbinsX());
  mass.SetTitle("deltaM (GeV/c^{2})");

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
