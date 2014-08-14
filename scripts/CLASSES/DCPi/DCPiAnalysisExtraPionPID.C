#include "DCPiAnalysis.h"

Int_t DCPiAnalysis::PlotExtraPionPID(){

  cout<<"Going to plot extra pion PID"<<endl;

  //ps file
  filename=_OutputDir+"/ExtraPionPID.ps";
  Canvas.Print(filename+"[");

  ///Open the reduced Ntuple
  if(OpenReducedNtuple("CleanNtuple")!=1) return 0;
  
 
  TH2F HdEdx("HdEdx","",300,0,3,5000,200,10200);
  HdEdx.SetStats(0);
  HdEdx.GetYaxis()->SetTitle("dE/dx (arbitrary units)");
  HdEdx.GetXaxis()->SetTitle("lab momentum (GeV/c)");
  Canvas.Clear();
  Canvas.SetLogy(1);
  ReducedNtuple->Draw("pi1dedx:pi1p3>>HdEdx","","colz");
  Canvas.Print(filename);
  Canvas.SetLogy(0);
  
  TH2F HthetaC("HthetaC","",300,0,3,700,.15,.85);  
  HthetaC.SetStats(0);
  HthetaC.GetYaxis()->SetTitle("#theta_{C} (rad)");
  HthetaC.GetXaxis()->SetTitle("lab momentum (GeV/c)");
  Canvas.Clear();
  ReducedNtuple->Draw("pi1thetac:pi1p3>>HthetaC","","colz");
  Canvas.Print(filename);


  CloseReducedNtuple();


  Canvas.Print(filename+"]");  

  return 1;
}
