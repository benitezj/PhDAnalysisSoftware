
#include "RooCBShape.h"
#include "RooArgusBG.h"

Int_t BToDstPiPiAnalysis::FitMES(){

  //define cuts
  Float_t mesmin=BCPDGMass-.006;
  Float_t mesmax=BCPDGMass+.006;
  


  
  if(!OpenReducedNtuple())return 0;
  
 
  TFile WSmESFile(_OutputDir+"/../KPiWSmES/Ntuple.root","read");
  gROOT->cd();
  TTree*WStree=(TTree*)WSmESFile.Get("Ntuple");
  if(!WStree)return 0;
  

  TH1F HmES("HmES","",90,5.20,5.29);
  ReducedNtuple->Draw("bmes>>HmES");

  TH1F HWSmES("HWSmES","",90,5.20,5.29);
  WStree->Draw("bmes>>HWSmES");
  HWSmES.Smooth(2);

  filename=_OutputDir+"/MES.ps";
  Canvas.Print(filename+"[");


  Canvas.Clear();  
  HmES.Draw("pe");
  HWSmES.Draw("same");
  Canvas.Print(filename);

  //Fit mES
  mass.setRange(HmES.GetXaxis()->GetXmin(),HmES.GetXaxis()->GetXmax());
  mass.setBins(HmES.GetXaxis()->GetNbins());

  RooRealVar bm0("bm0","bm0",5.29);
  RooRealVar bc("bc","bc",-100,-10);
  RooRealVar bp("bp","bp",.1,1);
  RooArgusBG Bkg("Bkg","ArgusP",mass,bm0,bc,bp);
  RooDataHist HB("HB","HB",RooArgList(mass),&HWSmES,1);
  
  //Fit ws
  Bkg.fitTo(HB,"m");
  RooPlot*plot=mass.frame();
  HB.plotOn(plot);
  Bkg.plotOn(plot);
  Canvas.Clear();  
  plot->Draw();
  Canvas.Print(filename);
  delete plot;



  RooHistPdf HBkg("HBkg","HBP",RooArgSet(mass),HB,0);
  RooRealVar Bsl0("Bsl0","Bsl0",HmES.GetXaxis()->GetXmin());
  RooRealVar Bsl("Bsl","Bsl",1,10);
  RooGenericPdf BslP("BslP","BslP","1+Bsl*(mass-Bsl0)",RooArgList(Bsl0,Bsl,mass));
  RooProdPdf BkgProd("BkgProd","BkgProd",BslP,HBkg);

  RooRealVar sm0("sm0","sm0",5.27,5.29);
  RooRealVar sigma("sigma","sigma",.001,.005);
  RooRealVar alpha("alpha","alpha",.5,5);
  RooRealVar n("n","n",.001,4);
  RooCBShape CBP("CBP","CBP",mass,sm0,sigma,alpha,n);
  RooRealVar SYield("SYield","SYield",.01,.8);

  RooAddPdf FitPdf("FitPdf","FitPdf",RooArgList(CBP,BkgProd),RooArgList(SYield));
  RooDataHist dataset("dataset","dataset",RooArgList(mass),&HmES,1);  
  FitPdf.fitTo(dataset,"m");
  plot=mass.frame();  
  dataset.plotOn(plot);
  FitPdf.plotOn(plot); 
  FitPdf.plotOn(plot,Components(BkgProd),LineColor(kRed));
  Canvas.Clear();  
  plot->Draw();
  HWSmES.Draw("pesame");
  cutline.DrawLine(mesmin,0,mesmin,HmES.GetMaximum());
  cutline.DrawLine(mesmax,0,mesmax,HmES.GetMaximum());
  Canvas.Print(filename);
  delete plot;


  //Calculate total signal yield
  cout<<"Total Signal yield="<<SYield.getVal()*HmES.Integral()<<endl;
  

  //Calculate signal yield in +-6MeV cut   
  mass.setRange("sigregion",mesmin,mesmax);
  RooArgSet nset(mass);

  RooAbsReal* sintegral=CBP.createIntegral(nset,nset,"sigregion");
  cout<<"Sig in Sig region="<<HmES.Integral()*sintegral->getVal()*SYield.getVal()<<endl;

  RooAbsReal* bintegral=BkgProd.createIntegral(nset,nset,"sigregion");
  cout<<"Bkg in Sig region="<<HmES.Integral()*bintegral->getVal()*(1-SYield.getVal())<<endl;

  
  Canvas.Print(filename+"]");

  return 1;
}
