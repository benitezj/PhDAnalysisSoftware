#include <RooDataHist.h>
#include "../Roo2BodyMassPhaseSpace.h"
#include <RooPolynomial.h>
#include "../RooChi2VarSimultaneous.h"

void DsInclusiveAnalysis::FitDsMass(){

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
  TH1F HBelleDsSigPdf("HBelleDsSigPdf","",50,1.82,2.12);
  TH1F HBelleDsBkgPdf("HBelleDsBkgPdf","",50,1.82,2.12);
  while(!InputStream.eof()&&ithpoint<50){
    ithpoint++;
    InputStream>>_x>>_y;
    InputStreamBkg>>_x>>_yb;
    HBelleDsSigPdf.SetBinContent(ithpoint,_y-_yb);   
    HBelleDsSigPdf.SetBinError(ithpoint,sqrt(_y+_yb));//sqrt(sum errors squared)
    HBelleDsBkgPdf.SetBinContent(ithpoint,_yb);    
    HBelleDsBkgPdf.SetBinError(ithpoint,sqrt(_yb));    
  }
  InputStream.close();
  InputStreamBkg.close();
  

  Canvas.Clear();
  HBelleDsSigPdf.GetYaxis()->SetRangeUser(0,HBelleDsSigPdf.GetMaximum()*1.2);
  HBelleDsSigPdf.Draw("pe");
  Canvas.Print(filename);

  Canvas.Clear();
  HBelleDsBkgPdf.GetYaxis()->SetRangeUser(0,HBelleDsBkgPdf.GetMaximum()*1.2);
  HBelleDsBkgPdf.Draw("pe");
  Canvas.Print(filename);



  TH1F HMass("HMass","",50,1.82,2.12);
  SetHistoXYLabels(&HMass,"M_{rec}(DKX#gamma)","GeV");
  TH1F HMass2("HMass2","",100,1.6,2.1);
  SetHistoXYLabels(&HMass2,"M_{rec}(DKX#gamma)","GeV");

  OpenReducedNtuple();
  gROOT->cd();
  ReducedNtuple->Draw("dsmass>>HMass");
  ReducedNtuple->Draw("dsmass>>HMass2");



  Canvas.Clear();
  HMass.Draw("pe");
  Canvas.Print(filename);

  mass.SetTitle("D_{s} mass");
  mass.setUnit("GeV");
  mass.setRange(HMass.GetXaxis()->GetXmin(),HMass.GetXaxis()->GetXmax());
  mass.setBins(HMass.GetNbinsX());

  //signal pdf
  RooDataHist BelleDsSigDat("BelleDsSigDat","BelleDsSigDat",RooArgList(mass),&HBelleDsSigPdf);  
  RooHistPdf  BelleDsSigPdf("BelleDsSigPdf","BelleDsSigPdf",RooArgSet(mass),BelleDsSigDat);

  //background pdf
  RooDataHist BelleDsBkgDat("BelleDsBkgDat","BelleDsBkgDat",RooArgList(mass),&HBelleDsBkgPdf);  
  RooHistPdf  BelleDsBkgPdf("BelleDsBkgPdf","BelleDsBkgPdf",RooArgSet(mass),BelleDsBkgDat);

  
  RooDataHist DsData("DsData","DsData",RooArgList(mass),&HMass);  
 


  //Compare with the signal pdf
  RooPlot* Plot=mass.frame();
  DsData.plotOn(Plot);
  BelleDsSigPdf.plotOn(Plot);
  Canvas.Clear();
  Plot->Draw();
  Canvas.Print(filename);
  delete Plot;

  //compare with the bkg pdf
  Plot=mass.frame();
  DsData.plotOn(Plot);
  BelleDsBkgPdf.plotOn(Plot);
  Canvas.Clear();
  Plot->Draw();
  Canvas.Print(filename);
  delete Plot;


 
  ///Fit with exponential and Breit-wigner
  RooRealVar Treshold("Treshold","Treshold",2.08,2.1); 
  RooRealVar gamMass("gamMass","gamMass",0.);
  Roo2BodyMassPhaseSpace TwoBPS("TwoBPS","TwoBPS",mass,gamMass,Treshold);  
  TwoBPS.setInverse(1);

  RooRealVar DsMass("DsMass","DsMass",1.97);//DsMass.setVal(1.9682);//DsMass.setConstant(1);
  RooRealVar Width("Width","Width",.005,.09);//Width.setVal(.02);
  //RooBreitWigner BW("BW","BW",mass,DsMass,Width);
  RooGenericPdf BW("BW","BW","1./((mass-DsMass)**2+(Width)**2)",RooArgList(mass,DsMass,Width));
 
  RooProdPdf sigPdf("sigPdf","sigPdf",TwoBPS,BW);
  

  RooRealVar coef1("coef1","coef1",.001,10.);
  RooRealVar coef2("coef2","coef2",.001,10.);
  //  RooGenericPdf ExpPdf("ExpPdf","ExpPdf","exp(-(Treshold-mass)/coef1-((Treshold-mass)/coef2)**2)",RooArgList(mass,coef1,coef2,Treshold));
  RooGenericPdf ExpPdf("ExpPdf","ExpPdf","exp(-(Treshold-mass)/coef1)",RooArgList(mass,coef1,Treshold));
  RooProdPdf bkgPdf("bkgPdf","bkgPdf",ExpPdf,TwoBPS);  
  RooRealVar Sigyield("Sigyield","Sigyield",.0,1.);
  RooAddPdf TotalPdf("TotalPdf","TotalPdf",sigPdf,bkgPdf,Sigyield);



  RooChi2VarIntegral Bellechi("Bellechi","Bellechi",TotalPdf,BelleDsSigDat,RooFit::DataError(RooAbsData::SumW2),RooFit::NumCPU(1));
  RooMinuit Bellemin(Bellechi);
  Bellemin.migrad();
  Plot=mass.frame();
  BelleDsSigDat.plotOn(Plot,RooFit::DataError(RooAbsData::SumW2));
  //BelleDsSigDat.statOn(Plot);
  TotalPdf.plotOn(Plot);
  TotalPdf.plotOn(Plot,Components(bkgPdf),LineColor(3));
  BW.paramOn(Plot,&BelleDsSigDat,"",1,"NE",.6,1.,1.);
  Canvas.Clear();
  Plot->SetTitle("");
  Plot->Draw();
  Canvas.Print(filename);
  delete Plot;

  RooChi2VarIntegral chi("chi","chi",TotalPdf,DsData);
  RooMinuit min(chi);
  min.migrad();
  Plot=mass.frame();
  DsData.plotOn(Plot);
  //DsData.statOn(Plot);
  TotalPdf.plotOn(Plot);
  TotalPdf.plotOn(Plot,Components(bkgPdf),LineColor(3));
  BW.paramOn(Plot,&DsData,"",1,"NE",.6,1.,1.);
  Canvas.Clear();
  Plot->SetTitle("");
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



  ///Plot the Ds mass  vs the n_X^R
  ///-------------------------
  //Total allowed n_X^R: 
  //Total energy not in X= E_tag + E_K + E_DsSt + E_gam = sqrt(1.87^2+2.^2) + sqrt(.5^2+.1^2) + sqrt(2.112^2+3.^2.)=

  TH2F HMassVsNX("HMassVsNX","",10,0,10,50,1.82,2.12);
  HMassVsNX.GetYaxis()->SetTitle("M_{rec}(DKX#gamma)");
  HMassVsNX.GetXaxis()->SetTitle("n_{X}^{R}");
  ReducedNtuple->Draw("dsmass:fragndaus>>HMassVsNX");
  Canvas.Clear();
  HMassVsNX.Draw("lego");
  Canvas.Print(filename);
  Canvas.Clear();
  Canvas.Divide(3,3);
  TH1* H[9];
  for(Int_t i=1;i<=9;i++){
    Canvas.cd(i);
    H[i-1]=HMassVsNX.ProjectionY(TString("_")+(long)i,i,i);
    H[i-1]->Draw("pe");
  }
  Canvas.Print(filename);
  for(Int_t i=1;i<=9;i++)delete H[i-1];

  Canvas.Clear();
  Canvas.Divide(2,2);		      
  TH1F HNReco("HNReco","",10,0.,10.);
  Canvas.cd(1);
  ReducedNtuple->Draw("fragndaus>>HNReco");  
  Canvas.cd(2);
  ReducedNtuple->Draw("fragnpis>>HNRecoPi(10,0.,10.)");
  Canvas.cd(3);
  ReducedNtuple->Draw("fragnpi0s>>HNRecoPi0(10,0.,10.)");
  Canvas.cd(4);
  ReducedNtuple->Draw("fragngams>>HNRecoGam(10,0.,10.)");
  Canvas.Print(filename);


  //Plot Mass as a function of n_X^T
  TH1* HGen[9];
  RooPlot* PlotTmp[9];
  Canvas.Clear();
  Canvas.Divide(3,3);
  for(Int_t i=0;i<9;i++){
    Canvas.cd(i+1);
    HGen[i]=new TH1F(TString("HGen")+(long)i,"",50,1.82,2.12);
    ReducedNtuple->Draw(TString("dsmass>>HGen")+(long)i,TString("fragnpartgen==")+(long)i,"pe");

    //fit to get resolution
    RooDataHist DataTmp("DataTmp","DataTmp",RooArgList(mass),HGen[i]);  
    RooChi2VarIntegral chi2Tmp("chi2Tmp","chi2Tmp",TotalPdf,DataTmp);
    RooMinuit minTmp(chi2Tmp);
    minTmp.migrad();
    PlotTmp[i]=mass.frame();
    DataTmp.plotOn(PlotTmp[i]);
    TotalPdf.plotOn(PlotTmp[i]);
    TotalPdf.plotOn(PlotTmp[i],Components(bkgPdf),LineColor(3));
    BW.paramOn(PlotTmp[i],&DsData,"",1,"NE",.65,1.,1.);
    PlotTmp[i]->SetTitle("");
    PlotTmp[i]->Draw();
  }
  Canvas.Print(filename);
  for(Int_t i=0;i<9;i++){delete HGen[i];  delete PlotTmp[i];}

  Canvas.Clear();
  Canvas.Divide(2,2);	
  TH1F HNGen("HNGen","",10,0.,10.);
  Canvas.cd(1);
  ReducedNtuple->Draw("fragnpartgen>>HNGen");  
  Canvas.cd(2);
  ReducedNtuple->Draw("fragnpigen>>HNGenPi(10,0.,10.)");
  Canvas.cd(3);
  ReducedNtuple->Draw("fragnpi0gen>>HNGenPi0(10,0.,10.)");
  Canvas.cd(4);
  ReducedNtuple->Draw("fragngamgen>>HNGenGam(10,0.,10.)");
  Canvas.Print(filename);

  //Compare number of Reco particles with number of Generated 
  Canvas.Clear();
  HNReco.Draw("pe");
  HNGen.Draw("same");
  Canvas.Print(filename);
  
  //Plot the number reco particles for each number of generated particles
  TH1F* HRecoVsGen[9];
  TH1F* HGenBin[9];
  Canvas.Clear();
  Canvas.Divide(3,3);
  for(Int_t i=0;i<9;i++){
    Canvas.cd(i+1);
    HRecoVsGen[i]=new TH1F(TString("HRecoVsGen")+(long)i,"",10,0,10.);
    HGenBin[i]=new TH1F(TString("HGenBin")+(long)i,"",10,0,10.);
    ReducedNtuple->Draw(TString("fragndaus>>HRecoVsGen")+(long)i,TString("fragnpartgen==")+(long)i);
    HGenBin[i]->SetBinContent(i+1,HRecoVsGen[i]->Integral());
    HGenBin[i]->Draw();
    HRecoVsGen[i]->Draw("pesame");
  }
  Canvas.Print(filename);
  for(Int_t i=0;i<9;i++){delete HRecoVsGen[i];delete HGenBin[i];}
  


  //plot the Ds diff vectors
  TH1F HDsPdiff("HDsPdiff","",200,0.,10.);
  ReducedNtuple->Draw("dspdiff>>HDsPdiff");  
  Canvas.Clear();		      
  HDsPdiff.Draw();
  Canvas.Print(filename);


  //plot the tag modes
  TH1F HTagMode("HTagMode","",20,0,20);
  ReducedNtuple->Draw("tagmode+(abs(taglund)-1)*10>>HTagMode");  
  Canvas.Clear();		      
  HTagMode.Draw();
  ReducedNtuple->Draw("tagmode+(abs(taglund)-1)*10","tagmctrue>0","same");
  ReducedNtuple->Draw("tagmode+(abs(taglund)-1)*10","tagmctruebad>0","psame");
  Canvas.Print(filename);


  CloseReducedNtuple();
  Canvas.Print(filename+"]");

}



void DsInclusiveAnalysis::CreateSigPdfs(){
  OpenReducedNtuple();  
  TFile PdfsFile(_OutputDir+"/SignalPdfsFile.root","recreate"); 
  for(Int_t r=0;r<NXR;r++){
    for(Int_t t=0;t<NXT;t++){
      TString hname=TString("HDsMass_R")+(long)r+TString("_T")+(long)t;
      TH1F HDsMassR(hname,hname,NDsMassBins,DsMassMin,DsMassMax);
      ReducedNtuple->Draw(TString("dsmass>>")+hname,TString("fragndaus==")+(long)r+TString("&&fragnpartgen==")+(long)t);
      HDsMassR.Write();
    }    
  }

  for(Int_t r=0;r<NXR;r++){
    TString hname=TString("HDsMass_R")+(long)r;
    TH1F HDsMass(hname,hname,NDsMassBins,DsMassMin,DsMassMax);
    ReducedNtuple->Draw(TString("dsmass>>")+hname,TString("fragndaus==")+(long)r);
    HDsMass.Write();
  }


  TH1F HDs("HDsMass","HDsMass",NDsMassBins,DsMassMin,DsMassMax);
  ReducedNtuple->Draw(TString("dsmass>>HDsMass"));
  HDs.Write();


  PdfsFile.Close();  
  CloseReducedNtuple();
}

void DsInclusiveAnalysis::PlotSigPdfs(){
  Canvas.Clear();
  filename=_OutputDir+"/SignalPdfs.ps";
  Canvas.Print(filename+"[");
  
  TFile PdfsFile(_OutputDir+"/SignalPdfsFile.root","read"); 
  TH1F* HDsMassR;
  Canvas.Divide(NXT,NXR);
  for(Int_t r=0;r<NXR;r++){
    for(Int_t t=0;t<NXT;t++){
      Int_t pad=NXT*(NXR-1-r)+t+1;
      Canvas.cd(pad);
      TString hname=TString("HDsMass_R")+(long)r+TString("_T")+(long)t;
      HDsMassR=(TH1F*)PdfsFile.Get(hname);
      if(!HDsMassR){cout<<hname<<" Not found"<<endl;return;}
      HDsMassR->SetTitle(TString("R")+(long)r+TString("_T")+(long)t);
      HDsMassR->Draw("pe");
    }    
  }
  Canvas.Print(filename);

  Canvas.Print(filename+"]");
  PdfsFile.Close();
}


void DsInclusiveAnalysis::CreateBkgPdfs(){
  OpenReducedNtuple();  
  TFile PdfsFile(_OutputDir+"/BkgPdfsFile.root","recreate"); 
  for(Int_t r=0;r<NXR;r++){
    TString hname=TString("HDsMass_R")+(long)r;
    TH1F HDsMassR(hname,hname,NDsMassBins,DsMassMin,DsMassMax);
    //ReducedNtuple->Draw(TString("dsmass>>")+hname,TString("fragndaus==")+(long)r);
    ReducedNtuple->Draw(TString("dsmass>>")+hname,TString("fragndaus==")+(long)r+"&&gammctrue<0");
    HDsMassR.Write();
  }

  TH1F HDs("HDsMass","HDsMass",NDsMassBins,DsMassMin,DsMassMax);
  //ReducedNtuple->Draw(TString("dsmass>>HDsMass"));
  ReducedNtuple->Draw(TString("dsmass>>HDsMass"),"gammctrue<0");
  HDs.Write();

  PdfsFile.Close();  
  CloseReducedNtuple();
}

void DsInclusiveAnalysis::PlotBkgPdfs(){
  Canvas.Clear();
  filename=_OutputDir+"/BkgPdfs.ps";
  Canvas.Print(filename+"[");
  
  TFile PdfsFile(_OutputDir+"/BkgPdfsFile.root","read"); 
  TH1F* HDsMassR;
  Canvas.Divide(3,2);

  for(Int_t r=0;r<NXR;r++){
      Canvas.cd(r+1);
      TString hname=TString("HDsMass_R")+(long)r;
      HDsMassR=(TH1F*)PdfsFile.Get(hname);
      if(!HDsMassR){cout<<hname<<" Not found"<<endl;return;}
      HDsMassR->SetTitle(TString("R")+(long)r);
      HDsMassR->Draw("pe");
  }
  Canvas.Print(filename);

  Canvas.Print(filename+"]");
  PdfsFile.Close();
}


void DsInclusiveAnalysis::RooFitDsMass(){


  mass.setRange(DsMassMin,DsMassMax);
  mass.setBins(NDsMassBins);
  RooArgSet paramset;
  paramset.add(mass);

  TString dummyformula=TString("mass");
//   //Declare the Weights for each n_X^T
//   RooRealVar* nXTWeight[NXT];
//   for(Int_t t=0;t<NXT;t++){
//     nXTWeight[t]=new RooRealVar(TString("nXTWeight")+(long)t,TString("nXTWeight")+(long)t,0,.1);
//     paramset.add(*nXTWeight[t]);
//     dummyformula+=TString("+nXTWeight")+(long)t;
//   }


  //Get the signal pdfs for each n_X^R
  RooAbsPdf* nXRPdf[NXR];
  TFile PdfsFile("DsTolnu/SigPdfs/SignalPdfsFile.root","read"); 
  gROOT->cd();
  TH1F* HDsMassR[NXR];
  RooDataHist* Hdatahist[NXR];
  //RooHistPdf* HPdf[NXR][NXT];
  for(Int_t r=0;r<NXR;r++){

//     RooArgList pdflist;
//     RooArgList weightlist;  
//     for(Int_t t=0;t<NXT;t++){
//       TString hid=TString("_R")+(long)r+TString("_T")+(long)t;
//       TString hname=TString("HDsMass")+hid;
//       HDsMassR[r][t]=(TH1F*)PdfsFile.Get(hname);
//       if(!HDsMassR[r][t]){cout<<hname<<" Not found"<<endl;return;}

//       Hdatahist[r][t]=new RooDataHist(TString("Hdata")+hid,TString("Hdata")+hid,RooArgSet(mass),HDsMassR[r][t]);
//       HPdf[r][t]=new RooHistPdf(TString("Hpdf")+hid,TString("Hpdf")+hid,RooArgSet(mass),*Hdatahist[r][t],0);
//       pdflist.add(*HPdf[r][t]);
//       weightlist.add(*nXTWeight[t]);      
//     }
//     pdflist.add(*BkgPdf[r]);
//     nXRPdf[r]=new RooAddPdf(TString("SigPdf_nXR")+(long)r,TString("SigPdf_nXR")+(long)r,pdflist,weightlist);

    TString hid=TString("_R")+(long)r;
    TString hname=TString("HDsMass")+hid;
    HDsMassR[r]=(TH1F*)PdfsFile.Get(hname);
    if(!HDsMassR[r]){cout<<hname<<" Not found"<<endl;return;}    
    Hdatahist[r]=new RooDataHist(TString("Hdata")+hid,TString("Hdata")+hid,RooArgSet(mass),HDsMassR[r]);
    nXRPdf[r]=new RooHistPdf(TString("Hpdf")+hid,TString("Hpdf")+hid,RooArgSet(mass),*Hdatahist[r],0);

  }
  PdfsFile.Close(); 



  //Get the bkg pdfs for each n_X^R
  TFile BkgPdfsFile("DsTolnu/BkgPdfs/BkgPdfsFile.root","read"); 
  gROOT->cd();
  TH1F* HDsMassRBkg[NXR];
  RooDataHist* HdatahistBkg[NXR];
  RooHistPdf* BkgPdf[NXR];
  RooRealVar* SigYield[NXR];
  RooAbsPdf*  TotalPdf[NXR];
  for(Int_t r=0;r<NXR;r++){

    TString hid=TString("_R")+(long)r;    
    TString hname=TString("HDsMass")+hid;
    HDsMassRBkg[r]=(TH1F*)BkgPdfsFile.Get(hname);
    if(!HDsMassRBkg[r]){cout<<hname<<" Not found"<<endl;return;}    
    HdatahistBkg[r]=new RooDataHist(TString("HdataBkg")+hid,TString("HdataBkg")+hid,RooArgSet(mass),HDsMassRBkg[r]);
    BkgPdf[r]=new RooHistPdf(TString("HpdfBkg")+hid,TString("HpdfBkg")+hid,RooArgSet(mass),*HdatahistBkg[r],0);
  
    SigYield[r]=new RooRealVar(TString("SigYieldnXR")+(long)r,TString("SigYieldnXR")+(long)r,0,1);
    dummyformula+=TString("+SigYieldnXR")+(long)r;
    paramset.add(*SigYield[r]);

    TotalPdf[r]=new RooAddPdf(TString("TotalPdf_nXR")+(long)r,TString("TotalPdf_nXR")+(long)r,*nXRPdf[r],*BkgPdf[r],*SigYield[r]);
  }
  BkgPdfsFile.Close(); 



  //Get the Data Histograms  for each n_X^R
  OpenReducedNtuple();
  gROOT->cd();
  TH2F HMassVsNX("HMassVsNX","",NXR,0,NXR,NDsMassBins,DsMassMin,DsMassMax);
  HMassVsNX.GetYaxis()->SetTitle("M_{rec}(DKX#gamma)");
  HMassVsNX.GetXaxis()->SetTitle("n_{X}^{R}");
  ReducedNtuple->Draw("dsmass:fragndaus>>HMassVsNX");
  TH1* HDsMassData[NXR];
  RooDataHist* DsMassDataSet[NXR];
  for(Int_t i=1;i<=NXR;i++){
    HDsMassData[i-1]=HMassVsNX.ProjectionY(TString("_")+(long)i,i,i);
    DsMassDataSet[i-1]=new RooDataHist(HDsMassData[i-1]->GetName(),HDsMassData[i-1]->GetName(),RooArgSet(mass),HDsMassData[i-1]);
  }
  CloseReducedNtuple();


  //Now we have a set of datasets and a set of pdfs (one for each n_X^R).. Perform a simultanous fit  
  RooDataHist dummydata("dummydata","dummydata",RooArgList(mass),HDsMassData[0]);
  RooGenericPdf dummypdf("dummypdf","dummypdf",dummyformula,paramset);
  //RooChi2VarSimultaneous Chi2("Chi2","Chi2",dummypdf,dummydata,HDsMassData,TotalPdf,NXR,0);
  RooRealVar* VarArray[10];
  RooChi2VarSimultaneous Chi2("Chi2","Chi2",dummypdf,dummydata,HDsMassData,TotalPdf,VarArray,NXR,0);
  
  RooMinuit Minuit(Chi2);
  Minuit.migrad();
   
  filename=_OutputDir+"/DsMassRooFit.ps";
  Canvas.Clear();
  Canvas.Print(filename+"[");
  Canvas.Divide(3,2);
  RooPlot*plots[NXR];
  for(Int_t i=1;i<=NXR;i++){
    Canvas.cd(i);
    plots[i-1]=mass.frame();
    DsMassDataSet[i-1]->plotOn(plots[i-1]);
    nXRPdf[i-1]->plotOn(plots[i-1]);
    plots[i-1]->Draw();
  }
  Canvas.Print(filename); 
  for(Int_t i=1;i<=NXR;i++)delete plots[i-1];

//   ///Plot one sample pdf for nXR=2
//   Canvas.Clear();
//   RooPlot*plot=mass.frame();
//   DsMassDataSet[2]->plotOn(plot);
//   RooArgSet comps;
//   for(Int_t t=0;t<NXT;t++){
//     comps.add(*HPdf[2][t]);
//     nXRPdf[2]->plotOn(plot,Components(comps),LineColor(t+1));
//   }
//   nXRPdf[2]->plotOn(plot);
//   plot->Draw();
//   Canvas.Print(filename);
//   delete plot;


  Canvas.Print(filename+"]");

}





void DsInclusiveAnalysis::RooFitDsMassSimple(){


  mass.setRange(DsMassMin,DsMassMax);
  mass.setBins(NDsMassBins);

  //Get the signal pdfs for each n_X^R
  RooAbsPdf* nXRPdf;
  TFile PdfsFile("DsTolnu/SigPdfs/SignalPdfsFile.root","read"); 
  gROOT->cd();
  TH1F* HDsMassR;
  RooDataHist* Hdatahist;
  HDsMassR=(TH1F*)PdfsFile.Get("HDsMass");
  if(!HDsMassR){cout<<" Not found"<<endl;return;}    
  Hdatahist=new RooDataHist(TString("Hdata"),TString("Hdata"),RooArgSet(mass),HDsMassR);
  nXRPdf=new RooHistPdf(TString("Hpdf"),TString("Hpdf"),RooArgSet(mass),*Hdatahist,0);
  PdfsFile.Close(); 



  //Get the bkg pdfs for each n_X^R
  TFile BkgPdfsFile("DsTolnu/BkgPdfs/BkgPdfsFile.root","read"); 
  gROOT->cd();
  TH1F* HDsMassRBkg;
  RooDataHist* HdatahistBkg;
  RooHistPdf* BkgPdf;
  RooRealVar* SigYield;
  RooAbsPdf*  TotalPdf; 
  HDsMassRBkg=(TH1F*)BkgPdfsFile.Get("HDsMass");
  if(!HDsMassRBkg){cout<<" Not found"<<endl;return;}    
  HdatahistBkg=new RooDataHist(TString("HdataBkg"),TString("HdataBkg"),RooArgSet(mass),HDsMassRBkg);
  BkgPdf=new RooHistPdf(TString("HpdfBkg"),TString("HpdfBkg"),RooArgSet(mass),*HdatahistBkg,0);
  
  SigYield=new RooRealVar(TString("SigYieldnXR"),TString("SigYieldnXR"),0,1);
  TotalPdf=new RooAddPdf(TString("TotalPdf_nXR"),TString("TotalPdf_nXR"),*nXRPdf,*BkgPdf,*SigYield);

  BkgPdfsFile.Close(); 



  //Get the Data Histograms  for each n_X^R
  OpenReducedNtuple();
  gROOT->cd();
  TH1F HDsMassData("HDsMassData","",NDsMassBins,DsMassMin,DsMassMax);
  ReducedNtuple->Draw("dsmass>>HDsMassData");
  RooDataHist* DsMassDataSet;
  DsMassDataSet=new RooDataHist(HDsMassData.GetName(),HDsMassData.GetName(),RooArgSet(mass),&HDsMassData);
  CloseReducedNtuple();


  TotalPdf->fitTo(*DsMassDataSet);
  filename=_OutputDir+"/DsMassRooFit.ps";
  Canvas.Clear();
  Canvas.Print(filename+"[");
  RooPlot*plots=mass.frame();
  DsMassDataSet->plotOn(plots);
  TotalPdf->plotOn(plots);
  TotalPdf->plotOn(plots,Components(RooArgSet(*BkgPdf)),LineColor(3));
  plots->Draw();
  Canvas.Print(filename); 


  Canvas.Print(filename+"]");

}
