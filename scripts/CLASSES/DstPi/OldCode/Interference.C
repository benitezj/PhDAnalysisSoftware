void plotType1Bkgs(){
  TFile File("DstarCharged/DstarPi/Kpi/Reduced.root");
  gROOT->cd();
  TTree*ntuple=(TTree*)File.Get("CleanNtuple");

  TCanvas C("C","C");
  TString filename="Interference.ps";
  C.Print(filename+"[");

  //2-particle masses
  C.Clear();
  ntuple->Draw("massKpislow>>h21(300,.6,.9)");
  C.Print(filename);
  C.Clear();
  ntuple->Draw("masspipi1>>h22(1000,.2,2.2)");
  C.Print(filename);
  C.Clear();
  ntuple->Draw("masspislowpi1>>h23(350,.2,.9)");
  C.Print(filename);

  ///3-particle masses
  C.Clear();
  ntuple->Draw("masspipislowpi1>>h31(1000,.6,2.6)");
  C.Print(filename);
  C.Clear();
  ntuple->Draw("massKpislowpi1>>h32(1000,.8,2.8)");
  C.Print(filename);
  C.Clear();
  ntuple->Draw("massKpipi1>>h33(1000,1.9,3.9)");
  C.Print(filename);


  C.Print(filename+"]");

  return;
}


void plotInterference(){
  
  //2-D plots of SlowPi-Pi1 mass
  TLine cutline;
  C.Clear();
  ntuple->Draw("masspislowpi1:dstpideltam+2.01>>h2full(500,2.1,3.1,500,.2,.8)","","colz");
  cutline.DrawLine(2.420,.2,2.420,.8);
  cutline.DrawLine(2.460,.2,2.460,.8);
  C.Print(filename);  
  C.Clear();
  ntuple->Draw("masspislowpi1:dstpideltam+2.01>>h2(150,2.3,2.6,250,.30,.55)","","colz");
  cutline.DrawLine(2.420,.3,2.420,.55);
  cutline.DrawLine(2.460,.3,2.460,.55);
  C.Print(filename);
  C.Clear();
  ntuple->Draw("masspislowpi1:dstpideltam+2.01>>h2coarse(30,2.3,2.6,125,.30,.55)","","colz");
  cutline.DrawLine(2.420,.3,2.420,.55);
  cutline.DrawLine(2.460,.3,2.460,.55);
  C.Print(filename);
  C.Clear();
  ntuple->Draw("masspislowpi1:dstpideltam+2.01>>h2neg(150,2.3,2.6,250,.30,.55)","dstpicharge==-1","colz");
  cutline.DrawLine(2.420,.3,2.420,.55);
  cutline.DrawLine(2.460,.3,2.460,.55);
  C.Print(filename);
  C.Clear();
  ntuple->Draw("masspislowpi1:dstpideltam+2.01>>h2pos(150,2.3,2.6,250,.30,.55)","dstpicharge==1","colz");
  cutline.DrawLine(2.420,.3,2.420,.55);
  cutline.DrawLine(2.460,.3,2.460,.55);
  C.Print(filename);

  TF1 FitFun("FitFun","[0]+[1]*x",2.3,2.6);
  h2->Fit("FitFun");
  C.Clear();
  h2->Draw("colz");
  C.Print(filename);

  TH1F Hlow("Hlow","Hlow",150,2.3,2.6);
  TH1F Hhigh("Hhigh","Hhigh",150,2.3,2.6);
  
  for(Int_t x=1;x<=h2->GetNbinsX();x++){
    for(Int_t y=1;y<=h2->GetNbinsY();y++){
      if(FitFun.Eval(h2->GetXaxis()->GetBinCenter(x))>h2->GetYaxis()->GetBinCenter(y))
	Hlow.SetBinContent(x,Hlow.GetBinContent(x)+h2->GetBinContent(x,y));
      else
	Hhigh.SetBinContent(x,Hhigh.GetBinContent(x)+h2->GetBinContent(x,y));      
    }        
  }

  Hlow.SetLineColor(4);
  Hlow.GetYaxis()->SetRangeUser(0,Hlow.GetMaximum());
  C.Clear();
  Hlow.Draw();
  Hhigh.Draw("same");
  C.Print(filename);
  
  C.Clear();
  ntuple->Draw("masspislowpi1>>h23clean(350,.2,.9)","abs(dstpideltam+2.01-2.420)<.060");
  C.Print(filename);

  //higher mass peaks
  C.Clear();
  ntuple->Draw("masspislowpi1:dstpideltam+2.01>>h2other(30,2.55,2.85,30,.35,.65)","","colz");
  C.Print(filename);

  //"Dalitz plot
  C.Clear();
  ntuple->Draw("masspislowpi1:massKpipi1>>h2dalitz(100,1.9,2.9,100,.2,.8)","","colz");
  C.Print(filename);  
  C.Clear();
  ntuple->Draw("masspislowpi1*masspislowpi1:massKpipi1*massKpipi1>>h2dalitz2(100,3.6,9,100,.04,.64)","","colz");
  C.Print(filename);  
  
  return;
}
