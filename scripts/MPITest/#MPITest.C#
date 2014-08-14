#include "RooRealVar.h"
#include "RooBreitWigner.h"
#include "RooPlot.h"
#include "RooArgSet.h"
#include "RooFit.h"
#include "RooGlobalFunc.h"
#include "RooDataSet.h"
#include "RooNLLVar.h"
#include "RooMinuit.h"
#include <TCanvas.h>
#include <TFile.h>


RooRealVar x("x","x",-.1,.1);
RooRealVar m("m","m",-.1,.1);
RooRealVar w("w","w",.0001,.1);
RooBreitWigner bw("bw","bw",x,m,w);


void MPITestGenerate(){
  TFile File("SOFTWARE/MPITest/MPITest.root","recreate");   
  m.setVal(0);
  w.setVal(.01);
  RooDataSet* dataset=bw.generate(RooArgSet(x),100000,kFALSE);
  dataset->SetName("DataSet");
  dataset->Write();
  File.ls();
  File.Close();  
}


void MPITestFit(){
  
  TFile File("SOFTWARE/MPITest/MPITest.root","read");  
  RooDataSet* dataset=(RooDataSet*)File.Get("DataSet");
  
  RooNLLVar nll("nll","nll",bw,*dataset,RooFit::NumCPU(2));  
  RooMinuit minuit(nll);
  minuit.migrad();


  TCanvas C("C","C");

  C.Clear();
  RooPlot*pl=x.frame();
  dataset->plotOn(pl);
  bw.plotOn(pl);
  pl->Draw();

  C.Print("SOFTWARE/MPITest/MPITest.ps");

  delete pl;

  File.Close();

}
