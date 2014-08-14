
#include "RooCBShape.h"
#include "RooArgusBG.h"
#include "RooGaussian.h"
#include "../../FUNCTIONS/Project2DHistoLegendre.h"

Int_t BToDstEtaAnalysis::FitMES(){

  //define cuts
  Float_t mesmin=BCPDGMass-.009;
  Float_t mesmax=BCPDGMass+.009;
    
  if(!OpenReducedNtuple())return 0;
  
  

  TH1F HmES("HmES","",40,5.20,5.30);
  ReducedNtuple->Draw("bmes>>HmES");

  TH2F HmESVsDecAngle("HmESVsDecAngle","",40,5.20,5.30,20,-1,1);
  ReducedNtuple->Draw("bdecayangle:bmes>>HmESVsDecAngle");

  TH2F HmESVsJointAngle("HmESVsJointAngle","",40,5.20,5.30,20,-1,1);
  ReducedNtuple->Draw("bdstjointangle:bmes>>HmESVsJointAngle");



  filename=_OutputDir+"/MES.ps";
  Canvas.Print(filename+"[");


  Canvas.Clear();  
  HmES.Draw("pe");
  Canvas.Print(filename);


  
  mass.setRange(HmES.GetXaxis()->GetXmin(),HmES.GetXaxis()->GetXmax());
  mass.setBins(HmES.GetXaxis()->GetNbins());

  RooRealVar bm0("bm0","bm0",5.29);
  RooRealVar bc("bc","bc",-100,-10);
  RooRealVar bp("bp","bp",.1,1);
  RooArgusBG Bkg("Bkg","ArgusP",mass,bm0,bc,bp);
  

  ///-------------------------------Fit The RS
  RooDataHist dataset("dataset","dataset",RooArgList(mass),&HmES,1);  


  //--------------------------------------------------------------------
  //Fit RS data with Argus function
  //--------------------------------------------------------------------
  Bkg.fitTo(dataset,"m");
  RooPlot*plot=mass.frame();
  dataset.plotOn(plot);
  Bkg.plotOn(plot);
  Canvas.Clear();  
  plot->Draw();
  Canvas.Print(filename);
  delete plot;
  
  //--------------------------------------------------------------------
  RooRealVar sm0("sm0","sm0",5.2794);
  RooRealVar sigma("sigma","sigma",.003);
  RooRealVar alpha("alpha","alpha",1,5);//3.);
  RooRealVar n("n","n",1,10);//1);
  //RooCBShape CBP("CBP","CBP",mass,sm0,sigma,alpha,n);
  RooGaussian CBP("CBP","CBP",mass,sm0,sigma);
  RooRealVar SYield("SYield","SYield",.01,.9);

  RooAddPdf FitPdf("FitPdf","FitPdf",RooArgList(CBP,Bkg),RooArgList(SYield));

  mass.SetTitle("m_{ES}");
  mass.setUnit("GeV^{2}/c^{4}");

  ///Fit The Total
  FitPdf.fitTo(dataset,"m");
  plot=mass.frame();  
  dataset.plotOn(plot,MarkerColor(0),LineColor(0));
  FitPdf.plotOn(plot); 
  FitPdf.plotOn(plot,Components(Bkg),LineColor(kRed));
  Canvas.Clear();  
  plot->Draw();
  HmES.SetLineColor(1);HmES.SetStats(0);
  HmES.Draw("same");
  cutline.DrawLine(mesmin,0,mesmin,HmES.GetMaximum());
  cutline.DrawLine(mesmax,0,mesmax,HmES.GetMaximum());
  Canvas.Print(filename);
  delete plot;
//   //Calculate total signal yield
//   Int_t Syieldtot=(int)(SYield.getVal()*HmES.Integral());
//   Int_t Syieldtote=(int)(SYield.getError()*HmES.Integral());
//   Int_t Byieldtot=(int)((1-SYield.getVal())*HmES.Integral());
//   Int_t Byieldtote=(int)((SYield.getError())*HmES.Integral());
//   //Calculate signal yield in cut   
//   mass.setRange("sigregion",mesmin,mesmax);
//   RooArgSet nset(mass);
//   RooAbsReal* sintegral=CBP.createIntegral(nset,nset,"sigregion");
//   Int_t syieldtot=(int)(HmES.Integral()*sintegral->getVal()*SYield.getVal());
//   RooAbsReal* bintegral=Bkg.createIntegral(nset,nset,"sigregion");
//   Int_t byieldtot=(int)(HmES.Integral()*bintegral->getVal()*(1-SYield.getVal()));





//   Canvas.Clear();  
//   HmESVsDecAngle.Draw("colz");
//   Canvas.Print(filename);
//   TH1* HmESDecP2=project2DHistoLegendre(&HmESVsDecAngle,2);
//   Canvas.Clear();  
//   HmESDecP2->Draw("pe");
//   Canvas.Print(filename);


  Canvas.Clear();  
  HmESVsJointAngle.Draw("colz");
  Canvas.Print(filename);
  TH1* HmESJointP2=project2DHistoLegendre(&HmESVsJointAngle,2);
  Canvas.Clear();  
  HmESJointP2->Draw("pe");
  Canvas.Print(filename);

  TH1*HJointA=HmESVsJointAngle.ProjectionY("sig",30,40);
  TH1*HJointASB=HmESVsJointAngle.ProjectionY("sb",20,30);
  Canvas.Clear();  
  HJointA->Draw("pe");
  HJointASB->Draw("same");
  Canvas.Print(filename);

  Canvas.Clear();  
  RooDataHist datasetp2("datasetp2","datasetp2",RooArgList(mass),HmESJointP2,1);  
  //CBP.fitTo(datasetp2,"m");
  plot=mass.frame();  
  datasetp2.plotOn(plot,MarkerColor(0),LineColor(0),RooFit::DataError(RooAbsData::SumW2));
  CBP.plotOn(plot); 
  Canvas.Clear();  
  plot->Draw();
  HmESJointP2->SetLineColor(1);HmES.SetStats(0);
  HmESJointP2->Draw("same");
  Canvas.Print(filename);
  delete plot;

  Canvas.Print(filename+"]");



  CloseReducedNtuple();

  return 1;
}
