#include "DstPiAnalysis.h"

Int_t DstPiAnalysis::PlotExtraPionPID(){

  cout<<"Going to plot extra pion PID"<<endl;

  //ps file
  filename=_OutputDir+"/ExtraPionPID.ps";
  Canvas.Print(filename+"[");

  ///Open the reduced Ntuple
  if(OpenReducedNtuple(2)!=1) return 0;
  
 
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
  


//   //make plots of ThetaC and cos_hel for clean signal at high p*
//   //this includes the D2420 and above
//   TCut sigcut=".35<dstpideltam&&dstpipstar>4.5&&dstarcostheta<-.5";
//   Canvas.Clear();
//   ReducedNtuple->Draw("pi1thetac:pi1p3>>HthetaC",sigcut,"colz");
//   HthetaC.GetYaxis()->SetRangeUser(0,HthetaC.GetMaximum()*1.2);
//   Canvas.Update();
//   Canvas.Print(filename);
  
//   TH1F HDstarCosth("HDstarCosth","HDstarCosth",10,-1.0001,-.5);
//   Canvas.Clear();
//   ReducedNtuple->Draw("dstarcostheta>>HDstarCosth",sigcut);
//   HDstarCosth.GetYaxis()->SetRangeUser(0,HDstarCosth.GetMaximum()*1.2);
//   Canvas.Update();
//   Canvas.Print(filename);

//   //this includes only the new bump
//   TCut sigcut2=".5<dstpideltam&&dstpipstar>4.5&&dstarcostheta<-.5";
//   Canvas.Clear();
//   ReducedNtuple->Draw("pi1thetac:pi1p3>>HthetaC",sigcut2,"colz");
//   HthetaC.GetYaxis()->SetRangeUser(0,HthetaC.GetMaximum()*1.2);
//   Canvas.Update();
//   Canvas.Print(filename);
  
//   Canvas.Clear();
//   ReducedNtuple->Draw("dstarcostheta>>HDstarCosth",sigcut2);
//   HDstarCosth.GetYaxis()->SetRangeUser(0,HDstarCosth.GetMaximum()*1.2);
//   Canvas.Update();
//   Canvas.Print(filename);

  Canvas.Print(filename+"]");  

  return 1;
}
