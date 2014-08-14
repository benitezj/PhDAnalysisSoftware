
#include "RooCBShape.h"
#include "RooArgusBG.h"

Int_t BToDstGamAnalysis::FitMES(){

  //define cuts
  Float_t mesmin=BCPDGMass-.006;
  Float_t mesmax=BCPDGMass+.006;
  
  if(!OpenReducedNtuple())return 0;

  TH1F HmES("HmES","",20,5.20,5.30);
  ReducedNtuple->Draw("bmes>>HmES");

  filename=_OutputDir+"/MES.ps";
  Canvas.Print(filename+"[");


  Canvas.Clear();  
  HmES.Draw("pe");
  Canvas.Print(filename);


  //Fit mES
  mass.setRange(HmES.GetXaxis()->GetXmin(),HmES.GetXaxis()->GetXmax());
  mass.setBins(HmES.GetXaxis()->GetNbins());

  RooRealVar bm0("bm0","bm0",5.29);
  RooRealVar bc("bc","bc",-100,-10);
  RooRealVar bp("bp","bp",.1,1);
  RooArgusBG Bkg("Bkg","ArgusP",mass,bm0,bc,bp);

  RooRealVar sm0("sm0","sm0",5.27,5.29);sm0.setVal(5.279);sm0.setConstant(1);
  RooRealVar sigma("sigma","sigma",.001,.005);sigma.setVal(.002);sigma.setConstant(1);
  RooRealVar alpha("alpha","alpha",.5,5);alpha.setVal(2.8);alpha.setConstant(1);
  RooRealVar n("n","n",.1,4);n.setVal(.5);n.setConstant(1);
  RooCBShape CBP("CBP","CBP",mass,sm0,sigma,alpha,n);
  RooRealVar SYield("SYield","SYield",.01,.8);

  RooAddPdf FitPdf("FitPdf","FitPdf",RooArgList(CBP,Bkg),RooArgList(SYield));
  RooDataHist dataset("dataset","dataset",RooArgList(mass),&HmES,1);  
  FitPdf.fitTo(dataset,"m");
  RooPlot*plot=mass.frame();  
  dataset.plotOn(plot);
  FitPdf.plotOn(plot); 
  FitPdf.plotOn(plot,Components(Bkg),LineColor(kRed));
  Canvas.Clear();  
  plot->Draw();
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

  RooAbsReal* bintegral=Bkg.createIntegral(nset,nset,"sigregion");
  cout<<"Bkg in Sig region="<<HmES.Integral()*bintegral->getVal()*(1-SYield.getVal())<<endl;

  
  Canvas.Print(filename+"]");

  return 1;
}
