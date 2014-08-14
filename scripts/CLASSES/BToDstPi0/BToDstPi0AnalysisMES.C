
#include "RooCBShape.h"
#include "RooArgusBG.h"
#include "RooGaussian.h"

Int_t BToDstPi0Analysis::FitMES(){

  //define cuts
  Float_t mesmin=BCPDGMass-.009;
  Float_t mesmax=BCPDGMass+.009;
    
  if(!OpenReducedNtuple())return 0;
  
  TH1F HmES("HmES","",40,5.20,5.30);
  ReducedNtuple->Draw("bmes>>HmES");


  TH1F HmESN("HmESN","",40,5.20,5.30);
  ReducedNtuple->Draw("bmes>>HmESN","bcharge==-1");
  
  TH1F HmESP("HmESP","",40,5.20,5.30);
  ReducedNtuple->Draw("bmes>>HmESP","bcharge==1");


  filename=_OutputDir+"/MES.ps";
  Canvas.Print(filename+"[");


  Canvas.Clear();  
  HmES.Draw("pe");
  Canvas.Print(filename);
  Canvas.Clear();  
  HmESN.Draw("pe");
  Canvas.Print(filename);
  Canvas.Clear();  
  HmESP.Draw("pe");
  Canvas.Print(filename);

  
  mass.setRange(HmES.GetXaxis()->GetXmin(),HmES.GetXaxis()->GetXmax());
  mass.setBins(HmES.GetXaxis()->GetNbins());

  RooRealVar bm0("bm0","bm0",5.29);
  RooRealVar bc("bc","bc",-100,-10);
  RooRealVar bp("bp","bp",.1,1);
  RooArgusBG Bkg("Bkg","ArgusP",mass,bm0,bc,bp);




  ///-------------------------------Fit The RS
  RooDataHist dataset("dataset","dataset",RooArgList(mass),&HmES,1);  
  RooDataHist datasetn("datasetn","datasetn",RooArgList(mass),&HmESN,1);  
  RooDataHist datasetp("datasetp","datasetp",RooArgList(mass),&HmESP,1);  


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
  //Fit RS data Bkg + CBShape
  //--------------------------------------------------------------------
//   RooHistPdf HBkg("HBkg","HBP",RooArgSet(mass),HB,0);
//   RooRealVar Bsl0("Bsl0","Bsl0",HmES.GetXaxis()->GetXmin());
//   RooRealVar Bsl("Bsl","Bsl",-100,100);
//   RooGenericPdf BslP("BslP","BslP","1+Bsl*(mass-Bsl0)",RooArgList(Bsl0,Bsl,mass));
//   RooProdPdf BkgProd("BkgProd","BkgProd",BslP,HBkg);

  RooRealVar sm0("sm0","sm0",5.2794);
  RooRealVar sigma("sigma","sigma",.003);
  RooRealVar alpha("alpha","alpha",3.);
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
  //Calculate signal yield in cut   
  mass.setRange("sigregion",mesmin,mesmax);
  RooArgSet nset(mass);
  RooAbsReal* sintegral=CBP.createIntegral(nset,nset,"sigregion");
  Int_t syieldtot=(int)(HmES.Integral()*sintegral->getVal()*SYield.getVal());
  RooAbsReal* bintegral=Bkg.createIntegral(nset,nset,"sigregion");
  Int_t byieldtot=(int)(HmES.Integral()*bintegral->getVal()*(1-SYield.getVal()));

  cout<<"Yield Results in sig region  TotSig="<<syieldtot<<endl;
  cout<<"Yield Results in sig region  TotBkg="<<byieldtot<<endl;

  /*  
  ///Now fix the shape parameters
  sm0.setConstant(1);
  sigma.setConstant(1);
  alpha.setConstant(1);
  n.setConstant(1);
  

  
  ////Fit the B0
  FitPdf.fitTo(datasetn,"m");
  RooPlot*plotn=mass.frame();  
  datasetn.plotOn(plotn,MarkerColor(0),LineColor(0));
  //FitPdf.plotOn(plotn,LineStyle(1),LineColor(2)); 
  FitPdf.plotOn(plotn,Components(BkgProd),LineColor(kBlack));
  //Calculate total signal yield
  Int_t Syieldn=(int)(SYield.getVal()*HmESN.Integral());
  Int_t Syieldne=(int)(SYield.getError()*HmESN.Integral());
  Int_t Byieldn=(int)((1-SYield.getVal())*HmESN.Integral());
  Int_t Byieldne=(int)((SYield.getError())*HmESN.Integral());
  //Calculate signal yield in cut 
  Int_t syieldn=(int)(HmESN.Integral()*sintegral->getVal()*SYield.getVal());
  Int_t byieldn=(int)(HmESN.Integral()*bintegral->getVal()*(1-SYield.getVal()));
  


  ////Fit the B0bar
  FitPdf.fitTo(datasetp,"m");
  RooPlot*plotp=mass.frame();  
  datasetp.plotOn(plotp,MarkerColor(0),LineColor(0));
  //FitPdf.plotOn(plotp,LineStyle(1),LineColor(3)); 
  FitPdf.plotOn(plotp,Components(BkgProd),LineColor(kBlack));
  //Calculate total signal yield
  //Calculate total signal yield
  Int_t Syieldp=(int)(SYield.getVal()*HmESP.Integral());
  Int_t Syieldpe=(int)(SYield.getError()*HmESP.Integral());
  Int_t Byieldp=(int)((1-SYield.getVal())*HmESP.Integral());
  Int_t Byieldpe=(int)((SYield.getError())*HmESP.Integral());
  //Calculate signal yield in cut 
  Int_t syieldp=(int)(HmESP.Integral()*sintegral->getVal()*SYield.getVal());
  Int_t byieldp=(int)(HmESP.Integral()*bintegral->getVal()*(1-SYield.getVal()));


  Canvas.Clear();  
  plotn->Draw();

  HmESN.SetLineColor(2);HmESN.SetStats(0);
  HmESN.Draw("same");
  plotp->Draw("same");
  HmESP.SetLineColor(3);HmESP.SetStats(0);
  HmESP.Draw("same");
  Canvas.Print(filename);
  delete plotn;
  delete plotp;


  Float_t diff=Syieldn-Syieldp;
  Float_t diffe=sqrt((float)Syieldne*Syieldne+(float)Syieldpe*Syieldpe);
  Float_t asym=diff/Syieldtot;
  Float_t asyme=asym*sqrt(diffe*diffe/((float)diff*diff)+Syieldtote*Syieldtote/((float)Syieldtot*Syieldtot));
  cout<<"Yield Results  TotSig="<<Syieldtot<<"+-"<<Syieldtote
      <<",  SigN="<<Syieldn<<"+-"<<Syieldne
      <<",   SigP="<<Syieldp<<"+-"<<Syieldpe
      <<"    Asym=(SigN-SigP)/(SigN+SigP)="<<asym<<"+-"<<asyme
      <<endl<<endl;
    
  cout<<"Yield Results  TotBkg="<<Byieldtot<<"+-"<<Byieldtote<<",  BkgN="<<Byieldn<<"+-"<<Byieldne<<",   BkgP="<<Byieldp<<"+-"<<Byieldpe<<endl<<endl;


  cout<<"Yield Results in sig region  TotSig="<<syieldtot<<",  SigN="<<syieldn<<",   SigP="<<syieldp<<endl;
  cout<<"Yield Results in sig region  TotBkg="<<byieldtot<<",  BkgN="<<byieldn<<",   BkgP="<<byieldp<<endl;

  ///---------------------------------------------------------------
  


  ////Plot Difference of the histograms:
  TH1F*HDiff=(TH1F*)HmESN.Clone();
  HDiff->SetName("HDiff");HDiff->SetTitle("Difference");
  HDiff->Sumw2();
  HDiff->Add(&HmESP,-1);
  Canvas.Clear(); 
  HDiff->SetStats(1);
  HDiff->SetLineColor(1); 
  SetHistoXYLabels(HDiff,"m_{ES}","(GeV^{2}/c^{4})");
  HDiff->Draw("pe");
  Canvas.Update();
  MoveStatsBox(HDiff,-1,1);
  cutline.DrawLine(HmESN.GetXaxis()->GetXmin(),0,HmESN.GetXaxis()->GetXmax(),0);
  Canvas.Update();
  Canvas.Print(filename);
  cout<<" Asym="<<(float)(HDiff->Integral())/HmES.Integral(28,36)<<endl;
  delete HDiff;


  */

  Canvas.Print(filename+"]");

  return 1;
}
