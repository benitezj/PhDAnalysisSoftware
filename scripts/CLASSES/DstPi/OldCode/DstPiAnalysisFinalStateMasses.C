#include "DstPiAnalysis.h"
 
Int_t DstPiAnalysis::FinalStateMasses(){ 
  cout<<"Going to plot final state masses"<<endl;
  //Use the same histograms that are fitted

  //ps file
  filename=_OutputDir+"/FinalStateMasses.ps";
  Canvas.Print(filename+"[");
  
  ///print the cuts into the file
  database.PrintCutList(&Canvas,filename);  

  if(!OpenReducedNtuple(2)) return 0;


  //2-particle masses
  Canvas.Clear();
  ReducedNtuple->Draw("massKpislow>>h21(300,.6,.9)");
  Canvas.Print(filename);
  Canvas.Clear();
  ReducedNtuple->Draw("masspipi1>>h22(1000,.2,2.2)");
  Canvas.Print(filename);
  Canvas.Clear();
  ReducedNtuple->Draw("masspislowpi1>>h23(350,.2,.9)");
  Canvas.Print(filename);

  ///3-particle masses
  Canvas.Clear();
  ReducedNtuple->Draw("masspipislowpi1>>h31(1000,.6,2.6)");
  Canvas.Print(filename);
  Canvas.Clear();
  ReducedNtuple->Draw("massKpislowpi1>>h32(1000,.8,2.8)");
  Canvas.Print(filename);
  Canvas.Clear();
  ReducedNtuple->Draw("massKpipi1>>h33(1000,1.9,3.9)");
  Canvas.Print(filename);


  Canvas.Print(filename+"]");

  if(CloseReducedNtuple()!=1) return 0;
  return 1;

}

Int_t DstPiAnalysis::PlotSlowPiPi1Interference(){ 

  //ps file
  TString filename=_OutputDir+"/PiSlowPi1Interference.ps";
  TCanvas Canvas(filename,filename);
  Canvas.Print(filename+"[");

  if(!OpenReducedNtuple(2)) return 0;

  //2-D plots of SlowPi-Pi1 mass
  Canvas.Clear();
  ReducedNtuple->Draw("masspislowpi1:dstpideltam+2.01>>h2full(500,2.1,3.1,500,.2,.8)","","colz");
  cutline.DrawLine(2.420,.2,2.420,.8);
  cutline.DrawLine(2.460,.2,2.460,.8);
  Canvas.Print(filename);  
  Canvas.Clear();
  TH2F h2("h2","h2",150,2.3,2.6,250,.30,.55);
  ReducedNtuple->Draw("masspislowpi1:dstpideltam+2.01>>h2","","colz");
  cutline.DrawLine(2.420,.3,2.420,.55);
  cutline.DrawLine(2.460,.3,2.460,.55);
  Canvas.Print(filename);
  Canvas.Clear();
  ReducedNtuple->Draw("masspislowpi1:dstpideltam+2.01>>h2coarse(30,2.3,2.6,125,.30,.55)","","colz");
  cutline.DrawLine(2.420,.3,2.420,.55);
  cutline.DrawLine(2.460,.3,2.460,.55);
  Canvas.Print(filename);
  Canvas.Clear();
  ReducedNtuple->Draw("masspislowpi1:dstpideltam+2.01>>h2neg(150,2.3,2.6,250,.30,.55)","dstpicharge==-1","colz");
  cutline.DrawLine(2.420,.3,2.420,.55);
  cutline.DrawLine(2.460,.3,2.460,.55);
  Canvas.Print(filename);
  Canvas.Clear();
  ReducedNtuple->Draw("masspislowpi1:dstpideltam+2.01>>h2pos(150,2.3,2.6,250,.30,.55)","dstpicharge==1","colz");
  cutline.DrawLine(2.420,.3,2.420,.55);
  cutline.DrawLine(2.460,.3,2.460,.55);
  Canvas.Print(filename);

  TF1 FitFun("FitFun","[0]+[1]*x",2.3,2.6);
  h2.Fit("FitFun");
  Canvas.Clear();
  h2.Draw("colz");
  Canvas.Print(filename);

  TH1F Hlow("Hlow","Hlow",150,2.3,2.6);
  TH1F Hhigh("Hhigh","Hhigh",150,2.3,2.6);
  
  for(Int_t x=1;x<=h2.GetNbinsX();x++){
    for(Int_t y=1;y<=h2.GetNbinsY();y++){
      if(FitFun.Eval(h2.GetXaxis()->GetBinCenter(x))>h2.GetYaxis()->GetBinCenter(y))
	Hlow.SetBinContent(x,Hlow.GetBinContent(x)+h2.GetBinContent(x,y));
      else
	Hhigh.SetBinContent(x,Hhigh.GetBinContent(x)+h2.GetBinContent(x,y));      
    }        
  }

  Hlow.SetLineColor(4);
  Hlow.GetYaxis()->SetRangeUser(0,Hlow.GetMaximum());
  Canvas.Clear();
  Hlow.Draw();
  Hhigh.Draw("same");
  Canvas.Print(filename);
  
  Canvas.Clear();
  ReducedNtuple->Draw("masspislowpi1>>h23clean(350,.2,.9)","abs(dstpideltam+2.01-2.420)<.060");
  Canvas.Print(filename);

  //higher mass peaks
  Canvas.Clear();
  ReducedNtuple->Draw("masspislowpi1:dstpideltam+2.01>>h2other(30,2.55,2.85,30,.35,.65)","","colz");
  Canvas.Print(filename);

  //"Dalitz plot
  Canvas.Clear();
  ReducedNtuple->Draw("masspislowpi1:massKpipi1>>h2dalitz(100,1.9,2.9,100,.2,.8)","","colz");
  Canvas.Print(filename);  
  Canvas.Clear();
  ReducedNtuple->Draw("masspislowpi1*masspislowpi1:massKpipi1*massKpipi1>>h2dalitz2(100,3.6,9,100,.04,.64)","","colz");
  Canvas.Print(filename);  
  
  Canvas.Print(filename+"]");

  if(CloseReducedNtuple()!=1) return 0;
  return 1;
}
