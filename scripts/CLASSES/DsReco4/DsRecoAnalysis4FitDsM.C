#include <RooDataHist.h>
#include "../Roo2BodyMassPhaseSpace.h"
#include <RooPolynomial.h>

void DsRecoAnalysis4::FitDsMass(){

  filename=_OutputDir+"/DsMassFit.ps";
  Canvas.Print(filename+"[");


  TString BelleData="/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/BelleDsRecoData.dat";
  TString BelleBkg="/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/BelleDsRecoBkg.dat";

  ifstream InputStream;
  ifstream InputStreamBkg;
  InputStream.open((const char*)(BelleData));
  InputStreamBkg.open((const char*)(BelleBkg));
  if(!InputStream.is_open()){cout<<"bad fit file name"<<endl; return;}
  if(!InputStreamBkg.is_open()){cout<<"bad fit file name"<<endl; return;}
  Int_t ithpoint=0;
  Float_t _x,_y,_yb;
  TH1F HDsSigPdf("HDsSigPdf","",50,1.82,2.12);
  TH1F HDsBkgPdf("HDsBkgPdf","",50,1.82,2.12);
  while(!InputStream.eof()&&ithpoint<50){
    ithpoint++;
    InputStream>>_x>>_y;
    InputStreamBkg>>_x>>_yb;
    HDsSigPdf.SetBinContent(ithpoint,_y-_yb);    
    HDsBkgPdf.SetBinContent(ithpoint,_yb);    
  }
  InputStream.close();
  InputStreamBkg.close();
  
  Canvas.Clear();
  HDsSigPdf.GetYaxis()->SetRangeUser(0,HDsSigPdf.GetMaximum()*1.2);
  HDsSigPdf.Draw("pe");
  Canvas.Print(filename);

  Canvas.Clear();
  HDsBkgPdf.GetYaxis()->SetRangeUser(0,HDsBkgPdf.GetMaximum()*1.2);
  HDsBkgPdf.Draw("pe");
  Canvas.Print(filename);



  TH1F HMass("HMass","",50,1.82,2.12);
  SetHistoXYLabels(&HMass,"M_{rec}(DKX#gamma)","GeV");
  TH1F HMass2("HMass2","",100,1.6,2.1);
  SetHistoXYLabels(&HMass2,"M_{rec}(DKX#gamma)","GeV");
  OpenReducedNtuple();
  gROOT->cd();
  ReducedNtuple->Draw("dsmass>>HMass");
  ReducedNtuple->Draw("dsmass>>HMass2");
  CloseReducedNtuple();


  Canvas.Clear();
  HMass.Draw("pe");
  Canvas.Print(filename);

  mass.setRange(HMass.GetXaxis()->GetXmin(),HMass.GetXaxis()->GetXmax());
  mass.setBins(HMass.GetNbinsX());

  //signal pdf
  RooDataHist DsSigDat("DsSigDat","DsSigDat",RooArgList(mass),&HDsSigPdf);  
  RooHistPdf  DsSigPdf("DsSigPdf","DsSigPdf",RooArgSet(mass),DsSigDat);

  //background pdf
  RooDataHist DsBkgDat("DsBkgDat","DsBkgDat",RooArgList(mass),&HDsBkgPdf);  
  RooHistPdf  DsBkgPdf("DsBkgPdf","DsBkgPdf",RooArgSet(mass),DsBkgDat);

  
  RooDataHist DsData("DsData","DsData",RooArgList(mass),&HMass);  
 


  //Compare with the signal pdf
  RooPlot* Plot=mass.frame();
  DsData.plotOn(Plot);
  DsSigPdf.plotOn(Plot);
  Canvas.Clear();
  Plot->Draw();
  Canvas.Print(filename);
  delete Plot;

  //compare with the bkg pdf
  Plot=mass.frame();
  DsData.plotOn(Plot);
  DsBkgPdf.plotOn(Plot);
  Canvas.Clear();
  Plot->Draw();
  Canvas.Print(filename);
  delete Plot;


 
  
  RooRealVar Treshold("Treshold","Treshold",2.09); 
  RooRealVar gamMass("gamMass","gamMass",0.);
  Roo2BodyMassPhaseSpace TwoBPS("TwoBPS","TwoBPS",mass,gamMass,Treshold);  
  TwoBPS.setInverse(1);

  RooRealVar DsMass("DsMass","DsMass",1.93,2.0);//DsMass.setVal(1.9682);//DsMass.setConstant(1);
  RooRealVar Width("Width","Width",.005,.09);//Width.setVal(.02);
  RooBreitWigner BW("BW","BW",mass,DsMass,Width);
 
  RooProdPdf sigPdf("sigPdf","sigPdf",TwoBPS,BW);
  

  RooRealVar coef1("coef1","coef1",.001,10.);
  RooGenericPdf ExpPdf("ExpPdf","ExpPdf","exp(-(Treshold-mass)/coef1)",RooArgList(mass,coef1,Treshold));
  RooProdPdf bkgPdf("bkgPdf","bkgPdf",ExpPdf,TwoBPS);  
  RooRealVar Sigyield("Sigyield","Sigyield",.0,1.);
  RooAddPdf TotalPdf("TotalPdf","TotalPdf",sigPdf,bkgPdf,Sigyield);

  RooChi2VarIntegral chi("chi","chi",TotalPdf,DsData);
  RooMinuit min(chi);
  min.simplex();
  min.migrad();
  Plot=mass.frame();
  DsData.plotOn(Plot);
  TotalPdf.plotOn(Plot);
  TotalPdf.plotOn(Plot,Components(bkgPdf),LineColor(3));
  Canvas.Clear();
  Plot->Draw();
  Canvas.Print(filename);
  delete Plot;



  ///----------------------
  ///Fit the full mass range
  //------------------------
  mass.setRange(HMass2.GetXaxis()->GetXmin(),HMass2.GetXaxis()->GetXmax());
  mass.setBins(HMass2.GetNbinsX());
  RooDataHist DsData2("DsData2","DsData2",RooArgList(mass),&HMass2);  

  RooChi2VarIntegral chi2("chi2","chi2",TotalPdf,DsData2);
  RooMinuit min2(chi2);
  min2.simplex();
  min2.migrad();
  Plot=mass.frame();
  DsData2.plotOn(Plot);
  TotalPdf.plotOn(Plot);
  TotalPdf.plotOn(Plot,Components(bkgPdf),LineColor(3));
  Canvas.Clear();
  Plot->Draw();
  Canvas.Print(filename);
  delete Plot;


  Canvas.Print(filename+"]");

}
