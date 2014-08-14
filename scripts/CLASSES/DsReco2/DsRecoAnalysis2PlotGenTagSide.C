

void DsRecoAnalysis2::PlotGenTagSide(){

  
  TFile FGen(_OutputDir+"/NtupleGen.root","read");
  gROOT->cd();

  ReducedNtuple=(TChain*)FGen.Get("Ntuple");

  filename=_OutputDir+"/GenTagSide.ps";
  Canvas.Print(filename+"[");

  TH1F HTagE("HTagE","",200,0,8.);
  ReducedNtuple->Draw("tagenergy>>HTagE");  
  SetHistoXYLabels(&HTagE,"Tag-D Energy ","GeV");
  Canvas.Clear();
  HTagE.Draw();
  Canvas.Print(filename);

  TH1F HFragE("HFragE","",200,0,8.);
  ReducedNtuple->Draw("fragenergy>>HFragE");  
  SetHistoXYLabels(&HFragE," Total Frag. Energy ","GeV");
  Canvas.Clear();
  HFragE.Draw();
  Canvas.Print(filename);

  TH1F HFragAngle("HFragAngle","",200,0,1.);
  ReducedNtuple->Draw("fragangle>>HFragAngle");  
  SetHistoXYLabels(&HFragAngle,"cosine of Angle to Tag-D","");
  Canvas.Clear();
  HFragAngle.Draw();
  Canvas.Print(filename);


  TH1F HdE("HdE","",200,-5.,5.);
  ReducedNtuple->Draw("bdeltae>>HdE");  
  SetHistoXYLabels(&HdE,"Sum Particle Energy  -  (Beam Energy)/2 ","GeV");
  Canvas.Clear();
  HdE.Draw();
  Canvas.Print(filename);

  TH1F HKN("HKN","",10,-.5,9.5);
  ReducedNtuple->Draw("fragnKs>>HKN");  
  SetHistoXYLabels(&HKN,"# of K","");
  Canvas.Clear();
  HKN.Draw();
  Canvas.Print(filename);

  TH1F HKsN("HKsN","",10,-.5,9.5);
  ReducedNtuple->Draw("fragnKss>>HKsN");  
  SetHistoXYLabels(&HKsN,"# of K_{S}","");
  Canvas.Clear();
  HKsN.Draw();
  Canvas.Print(filename);

  TH1F HpiN("HpiN","",10,-.5,9.5);
  ReducedNtuple->Draw("fragnpis>>HpiN");  
  SetHistoXYLabels(&HpiN,"# of #pi","");
  Canvas.Clear();
  HpiN.Draw();
  Canvas.Print(filename);

  TH1F Hpi0N("Hpi0N","",10,-.5,9.5);
  ReducedNtuple->Draw("fragnpi0s>>Hpi0N");  
  SetHistoXYLabels(&Hpi0N,"# of #pi^{0}","");
  Canvas.Clear();
  Hpi0N.Draw();
  Canvas.Print(filename);

  TH1F HGamN("HGamN","",10,-.5,9.5);
  ReducedNtuple->Draw("fragngams>>HGamN");  
  SetHistoXYLabels(&HGamN,"# of #gamma","");
  Canvas.Clear();
  HGamN.Draw();
  Canvas.Print(filename);

  TH1F HMuN("HMuN","",10,-.5,9.5);
  ReducedNtuple->Draw("fragnMus>>HMuN");  
  SetHistoXYLabels(&HMuN,"# of #mu","");
  Canvas.Clear();
  HMuN.Draw();
  Canvas.Print(filename);

  TH1F HEN("HEN","",10,-.5,9.5);
  ReducedNtuple->Draw("fragnEs>>HEN");  
  SetHistoXYLabels(&HEN,"# of e","");
  Canvas.Clear();
  HEN.Draw();
  Canvas.Print(filename);

  TH1F HNuMuN("HNuMuN","",10,-.5,9.5);
  ReducedNtuple->Draw("fragnMuNus>>HNuMuN");  
  SetHistoXYLabels(&HNuMuN,"# of #nu_{#mu}","");
  Canvas.Clear();
  HNuMuN.Draw();
  Canvas.Print(filename);

  TH1F HNuEN("HNuEN","",10,-.5,9.5);
  ReducedNtuple->Draw("fragnENus>>HNuEN");  
  SetHistoXYLabels(&HNuEN,"# of #nu_{e}","");
  Canvas.Clear();
  HNuEN.Draw();
  Canvas.Print(filename);


  Canvas.SetLogy(1);

  TH1F HKE("HKE","",200,0,1.00001);
  ReducedNtuple->Draw("fragenergyK/fragenergy>>HKE","fragenergy>0");  
  SetHistoXYLabels(&HKE,"K Energy / Total Frag. Energy ","GeV");
  Canvas.Clear();
  HKE.Draw();
  Canvas.Print(filename);

  TH1F HKsE("HKsE","",200,0,1.00001);
  ReducedNtuple->Draw("fragenergyKs/fragenergy>>HKsE","fragenergy>0");  
  SetHistoXYLabels(&HKsE,"K_{S} Energy / Total Frag. Energy ","GeV");
  Canvas.Clear();
  HKsE.Draw();
  Canvas.Print(filename);

  TH1F HpiE("HpiE","",200,0,1.00001);
  ReducedNtuple->Draw("fragenergypi/fragenergy>>HpiE","fragenergy>0");  
  SetHistoXYLabels(&HpiE,"#pi Energy / Total Frag. Energy ","GeV");
  Canvas.Clear();
  HpiE.Draw();
  Canvas.Print(filename);

  TH1F Hpi0E("Hpi0E","",200,0,1.00001);
  ReducedNtuple->Draw("fragenergypi0/fragenergy>>Hpi0E","fragenergy>0");  
  SetHistoXYLabels(&Hpi0E,"#pi^{0} Energy / Total Frag. Energy ","GeV");
  Canvas.Clear();
  Hpi0E.Draw();
  Canvas.Print(filename);

  TH1F HGamE("HGamE","",200,0,1.00001);
  ReducedNtuple->Draw("fragenergygam/fragenergy>>HGamE","fragenergy>0");  
  SetHistoXYLabels(&HGamE,"#gamma Energy / Total Frag. Energy ","GeV");
  Canvas.Clear();
  HGamE.Draw();
  Canvas.Print(filename);

  TH1F HMuE("HMuE","",200,0,1.00001);
  ReducedNtuple->Draw("fragenergyMu/fragenergy>>HMuE","fragenergy>0");  
  SetHistoXYLabels(&HMuE,"#mu Energy / Total Frag. Energy ","GeV");
  Canvas.Clear();
  HMuE.Draw();
  Canvas.Print(filename);

  TH1F HEE("HEE","",200,0,1.00001);
  ReducedNtuple->Draw("fragenergyE/fragenergy>>HEE","fragenergy>0");  
  SetHistoXYLabels(&HEE,"e Energy / Total Frag. Energy ","GeV");
  Canvas.Clear();
  HEE.Draw();
  Canvas.Print(filename);


  TH1F HNuMuE("HNuMuE","",200,0,1.00001);
  ReducedNtuple->Draw("fragenergyMuNu/fragenergy>>HNuMuE","fragenergy>0");  
  SetHistoXYLabels(&HNuMuE,"#nu_{#mu} Energy / Total Frag. Energy ","GeV");
  Canvas.Clear();
  HNuMuE.Draw();
  Canvas.Print(filename);

  TH1F HNuEE("HNuEE","",200,0,1.00001);
  ReducedNtuple->Draw("fragenergyENu/fragenergy>>HNuEE","fragenergy>0");  
  SetHistoXYLabels(&HNuEE,"#nu_{e} Energy / Total Frag. Energy ","GeV");
  Canvas.Clear();
  HNuEE.Draw();
  Canvas.Print(filename);

  Canvas.SetLogy(0);


  FGen.Close();
  
  Canvas.Print(filename+"]");
}
