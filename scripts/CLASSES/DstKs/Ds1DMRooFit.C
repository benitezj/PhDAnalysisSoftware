#include "../Globals.h"
#include "../FUNCTIONS/Fits.h"
#include <TLine.h>
#include <TGraphErrors.h>
#include "../CLASSES/JDataBase.h"
#include <TLatex.h>

#include "RooRealVar.h"
#include "RooCategory.h"
#include "RooPlot.h"
#include "RooDataSet.h"
#include "RooDataHist.h"
#include "RooAddPdf.h"
#include "RooFitResult.h"
#include "RooBreitWigner.h"
#include "RooGenericPdf.h"
#include "RooArgSet.h"
#include "RooArgList.h"
#include "RooDataHist.h"
#include "RooAbsReal.h"
#include "RooNumConvPdf.h"


Int_t fitDs1DMWithRooFit(TString rootfile,Bool_t TruthMatch,Int_t Nevents){

  TFile* file=new TFile(rootfile,"read");  
  if(file==NULL){
    cout<<"Bad file"<<endl;
    return 0;
  }
  TTree* tree=(TTree*)file->Get("Ds1pNtuple");
  if(tree!=NULL)cout<<"Ntuple: "<<rootfile<<" extracted with"<<tree->GetEntries()<<" entries"<<endl;
  tree->SetDirectory(0);
  file->Close();
  tree->SetBranchStatus("*",0);
  

  JDataBase database;
  database.SetCuts(1);

  ///Plot the results
  TCanvas Canvas("Canvas","Canvas");
  TString fname;


  /////
  Float_t fitmin=database.GetDs1pDeltaMMin();
  Float_t fitmax=database.GetDs1pDeltaMMax();


  ///Ntuple variables
  RooRealVar ds1pdeltam2("ds1pdeltam2","DeltaM",fitmin,fitmax);
  ds1pdeltam2.setBins(100);
  tree->SetBranchStatus("ds1pdeltam2",1); 
  ds1pdeltam2.Print();
  RooRealVar d0pstar("d0pstar","d0pstar",database.GetD0pstarMin(),5);
  tree->SetBranchStatus("d0pstar",1);
  d0pstar.Print();
  RooRealVar dstardeltam("dstardeltam","dstardeltam",database.GetDstarDeltaMMin(),database.GetDstarDeltaMMax());
  tree->SetBranchStatus("dstardeltam",1);
  dstardeltam.Print();
  RooRealVar klh("klh","klh",database.GetKLHBit(),15);
  tree->SetBranchStatus("klh",1);
  klh.Print();
  RooRealVar pilh("pilh","pilh",database.GetPiLHBit(),5);
  tree->SetBranchStatus("pilh",1);
  pilh.Print();
  RooRealVar d0vtxprob("d0vtxprob","d0vtxprob",database.GetD0MinProbab(),1.001);
  tree->SetBranchStatus("d0vtxprob",1);
  d0vtxprob.Print();
  RooRealVar ksmass("ksmass","ksmass",K0PDGMass-database.GetKsMassWindow(),K0PDGMass+database.GetKsMassWindow());
  tree->SetBranchStatus("ksmass",1);
  ksmass.Print();
  RooRealVar kscosine("kscosine","kscosine",database.GetKsCosineMin(),1.01);
  tree->SetBranchStatus("kscosine",1);
  kscosine.Print();
  RooRealVar d0mass("d0mass","d0mass",D0PDGMass-database.GetD0MassWindow(),D0PDGMass+database.GetD0MassWindow());
  tree->SetBranchStatus("d0mass",1);
  d0mass.Print();
  RooRealVar ds1pmctrue("ds1pmctrue","ds1pmctrue",.99,1.01);
  tree->SetBranchStatus("ds1pmctrue",1);
  ds1pmctrue.Print();
  RooRealVar eventnumber("eventnumber","eventnumber",1,Nevents);
  tree->SetBranchStatus("eventnumber",1);
  eventnumber.Print();

 
  RooArgList vars;
  vars.add(ds1pdeltam2);
  vars.add(d0pstar);  
  vars.add(dstardeltam);
  vars.add(pilh);
  vars.add(klh);
  vars.add(d0vtxprob);
  vars.add(ksmass);
  vars.add(kscosine);
  vars.add(d0mass);
  vars.add(eventnumber);


  //The Signal
  RooRealVar width("width","width",.001);
  // width.setRange(database.GetDs1pDeltaMMin(),database.GetDs1pDeltaMMax());
  width.setRange(.0005,.0015);
  width.setConstant(kFALSE);
  
  RooRealVar mean("mean","mean",.027);
  //mean.setRange(database.GetDs1pDeltaMMin(),database.GetDs1pDeltaMMax());
  mean.setRange(.022,.032);
  mean.setConstant(kFALSE);

 
  //the signal pdf
  RooGenericPdf* signalpdf;

  ////use 3 gaussians with (same mean) as the resolution
  RooRealVar Mean("Mean","Mean",1.6e-5);  
  RooRealVar N1("N1","N1",9.538);
  RooRealVar S1("S1","S1",.0105);
  RooRealVar N2("N2","N2",244.4);
  RooRealVar S2("S2","S2",.0009727);
  RooRealVar N3("N3","N3",581.8);
  RooRealVar S3("S3","S3",.0004849);
  RooGenericPdf respdf("respdf","N1*exp(-.5*(ds1pdeltam2-Mean)**2/S1**2)+N2*exp(-.5*(ds1pdeltam2-Mean)**2/S2**2)+N3*exp(-.5*(ds1pdeltam2-Mean)**2/S3**2)",RooArgSet(ds1pdeltam2,N1,Mean,S1,N2,S2,N3,S3));


 
  ////The Background
  RooRealVar coef0("coef0","coef0",0,100);
  RooRealVar coef1("coef1","coef1",-1000,1000);
  RooRealVar coef2("coef2","coef2",-10000,10000);
  ///
  coef0.setConstant(kFALSE);
  coef1.setConstant(kFALSE);
  coef2.setConstant(kFALSE);
  
  RooGenericPdf* bkg=NULL;
  bkg=new RooGenericPdf("mybkg","coef0+coef1*ds1pdeltam2+coef2*ds1pdeltam2**2",RooArgList(ds1pdeltam2,coef0,coef1,coef2));  
     
  /////////
 
  if(TruthMatch){

    vars.add(ds1pmctrue);

    ///MC generated with a nonrelativistic bw
    //signalpdf=new RooGenericPdf("sinalpdf","width/((ds1pdeltam2-mean)**2+.25*(width**2))",
                                  //RooArgSet(ds1pdeltam2,mean,width));
    signalpdf=new RooGenericPdf("signalpdf","ds1pdeltam2*mean*width/((ds1pdeltam2**2-mean**2)**2+(mean**2)*(width**2))",
				RooArgSet(ds1pdeltam2,mean,width));

    fname="Ds1RooFitMC.ps";
  }
  if(!TruthMatch){
 
    ///DATA should follow relativistic bw
    signalpdf=new RooGenericPdf("signalpdf","ds1pdeltam2*mean*width/((ds1pdeltam2**2-mean**2)**2+(mean**2)*(width**2))",
				RooArgSet(ds1pdeltam2,mean,width));

    //plots name
    fname="Ds1RooFitDATA.ps";
  }
  

  /////////The Smeared signal
  RooNumConvPdf signalsmeared("signalsmeared","signalsmeared",ds1pdeltam2,*signalpdf,respdf);
  signalsmeared.setConvolutionWindow(Mean,S2,10);
  

  Canvas.Print(fname+"[");

  ///////////////////Lets check the smeared distribution
  RooPlot* ds1pdeltam2frame=ds1pdeltam2.frame(fitmin,fitmax);

  Canvas.Clear();  
  signalpdf->plotOn(ds1pdeltam2frame);
  signalsmeared.plotOn(ds1pdeltam2frame);
  ds1pdeltam2frame->Draw();
  Canvas.Print(fname);  
  ///////////////////////////////
  



  ///
  RooRealVar BkgNb("BkgNb","BkgNb",0,100000);
  //BkgNb.setError(1000);
  BkgNb.setConstant(kFALSE);
  RooRealVar sigPiNb("sigPiNb","sigPiNb",0,100000);  //Signal Yield
  //sigPiNb.setError(100);
  sigPiNb.setConstant(kFALSE);

  ///construct the total  pdf: signal plus background
  if(bkg==NULL) return 0;
  RooAddPdf* pdf=new RooAddPdf("SignalPlusBkgPdf","SignalPlusBkgPdf",RooArgList(signalsmeared,*bkg),RooArgList(sigPiNb,BkgNb));
  


  ////time the fit 
  
  time_t* timer=NULL;
  Int_t starttime=0;
  Int_t endtime=0;
  starttime=time(timer);

  
  ///get the data
  RooDataSet* data=new RooDataSet("data","data",tree,vars);


  //Make the fit 
  RooFitResult* res=pdf->fitTo(*data,"mher");
  if(res==NULL){
    cout<<"No fit results returned."<<endl;
    return 0;
  }

  endtime=time(timer);
  cout<<"-----------------Time To Fit:"<<(endtime-starttime)/60<<" minutes"<<endl;

  ////////////////////////////////////Make Some plots
  TLine line;
 
  ////////the Fit plot
  Canvas.Clear();  
  ds1pdeltam2frame=ds1pdeltam2.frame(fitmin,fitmax);
  data->plotOn(ds1pdeltam2frame);
  pdf->plotOn(ds1pdeltam2frame);
  ds1pdeltam2frame->Draw();
  Canvas.Print(fname);
  //if(ds1pdeltam2frame!=NULL)delete ds1pdeltam2frame;
 

  RooDataHist dmhist("dmhist","Ds1 Delta M",RooArgList(ds1pdeltam2),*data);  
  TH1* HDs1pDM=dmhist.createHistogram("HDs1pDM",ds1pdeltam2);
  if(HDs1pDM==NULL){
    cout<<"No HDs1pDM  returned."<<endl;
    return 0;
  }

  ///////////////residuals 

  RooArgList LastParList = res->floatParsFinal();
  RooRealVar* yield = (RooRealVar*)LastParList.find("sigPiNb");
  if(yield==NULL){
    cout<<"No yield  returned."<<endl;
    return 0;
  }
  Int_t Nsig  = (int)yield->getVal();
  Int_t ENsig = (int)yield->getError();
  RooRealVar* parmean = (RooRealVar*)LastParList.find("mean");
  if(parmean==NULL){
    cout<<"No parmean  returned."<<endl;
    return 0;
  }
  Float_t m0  = (float)parmean->getVal();
  Float_t Em0 = (float)parmean->getError();
  RooRealVar* parwidth = (RooRealVar*)LastParList.find("width");
  if(parwidth==NULL){
    cout<<"No parwidth  returned."<<endl;
    return 0;
  }
  Float_t Gamma  = (float)parwidth->getVal();
  Float_t EGamma = (float)parwidth->getError();


  Float_t integral=0;
  Int_t residual=0;
  Float_t residualerror=0;
  Int_t Nbins=HDs1pDM->GetXaxis()->GetNbins();
  Float_t binwidth=HDs1pDM->GetXaxis()->GetBinWidth(1);

  TGraphErrors* GRes=new TGraphErrors();


  /////The background
  RooRealVar* yieldbkg = (RooRealVar*)LastParList.find("BkgNb");
  if(yieldbkg==NULL){
    cout<<"No yieldbkg  returned."<<endl;
    return 0;
  }
  Int_t Nbkg  = (int)yieldbkg->getVal();
  Int_t ENbkg = (int)yieldbkg->getError();

  
  TF1* FPDFBkg=NULL;
  FPDFBkg=new TF1("FPDFBkg","[3]*([0]+[1]*x+[2]*x**2)",fitmin,fitmax);

  Int_t Npowers=3;
  Float_t polpar[7];
  for(Int_t i=0;i<Npowers;i++){   
    polpar[i]=(float)((RooRealVar*)LastParList.find(TString("coef")+(long)i))->getVal();
    FPDFBkg->SetParameter(i,polpar[i]);
  }
  FPDFBkg->SetParameter(Npowers,1);
      
  //////////calculate normalization of bkg
  Float_t polnorm=0;
  for(Int_t i=0;i<Npowers;i++){  
    polnorm+=polpar[i]*(pow(fitmax,(float)(i+1))-pow(fitmin,(float)(i+1)))/(i+1.0);    
  }

  if(polnorm<=0)cout<<"bkg norm not right:"<<polnorm<<endl;
  cout<<"bkg norm"<<polnorm<<endl;
  // FPDFBkg->SetParameter(3,1/polnorm);//normalize integral to 1


  Canvas.Clear();
  Float_t initpar[10]={5,.027,.001,1,1,1};
  FitHistogramRelBreitWignerPolN((TH1F*)HDs1pDM,database.GetDs1pDeltaMMin(),database.GetDs1pDeltaMMax(),2,initpar);
  HDs1pDM->Draw();
  Canvas.Print(fname);  

 

  Canvas.Clear();  
  HDs1pDM->Draw("l");
  ds1pdeltam2frame=ds1pdeltam2.frame(fitmin,fitmax);
  data->plotOn(ds1pdeltam2frame);
  pdf->plotOn(ds1pdeltam2frame);
  ds1pdeltam2frame->Draw("same");

  FPDFBkg->SetParameter(3,Nbkg/polnorm);//normalize integral to 1
  FPDFBkg->Draw("same");
  Canvas.Print(fname);  


  ///Also compute  the chisquare
  Float_t chisquare=0; 
  RooAbsReal* inte;

  for(Int_t bin=1;bin<=Nbins;bin++){
 
    //method 4
    ds1pdeltam2.setRange(TString("subrange")+(long)bin,HDs1pDM->GetBinLowEdge(bin),HDs1pDM->GetBinLowEdge(bin)+binwidth) ;
    inte= pdf->createIntegral(ds1pdeltam2,ds1pdeltam2,TString("subrange")+(long)bin);
    integral=(float)inte->getVal()*Nsig;
    ///////////////

    residual=(int)(HDs1pDM->GetBinContent(bin)-integral);
    //cout<<bin<<" "<<HDs1pDM->GetBinContent(bin)<<" "<<integral<<" "<<residual<<endl;

    residualerror=sqrt(HDs1pDM->GetBinContent(bin));
    if(residualerror<1)residualerror=1;
    
    GRes->SetPoint(bin-1,HDs1pDM->GetBinLowEdge(bin)+binwidth/2,residual);
    GRes->SetPointError(bin-1,0,residualerror);
  
    chisquare+=(residual*residual)/(residualerror*residualerror);
    
  }
  Int_t ndof=Nbins-LastParList.getSize();

  Canvas.Clear();  
  GRes->GetYaxis()->SetRangeUser(-30.,30.);
  GRes->GetYaxis()->SetTitle("Residual");
  GRes->GetXaxis()->SetTitle("DeltaM(D_{s1})");
  GRes->Draw("ape");
  Canvas.Print(fname);  


  ////////done saving plots
  
  ///////////////////////////////////////////////////////////////////
  
  ////////////////print the results
  res->Print();

  
  cout<<"+++++++++++++++++RESULTS+++++++++++++"<<endl;


  

  Float_t corrmw=(float)res->correlation("mean","width");
  Float_t corrmm=(float)res->correlation("mean","mean");
  Float_t corrww=(float)res->correlation("width","width");



  cout<<"Correlation matrix"<<endl;
  cout<<corrmm<<" "<<corrmw<<endl;
  cout<<corrmw<<" "<<corrww<<endl;
  cout<<endl;
  cout<<"min log(l)"<<endl;
  cout<<res->minNll()<<endl;
  cout<<endl;

  cout<<endl;
  cout<<"Chisquare/ndof ="<<chisquare<<"/"<<ndof<<endl;

  cout<<"+++++++++++++++++RESULTS+++++++++++++"<<endl;


  ///print results into file

  char text[1000];
  TLatex sometext;
  sometext.SetTextSize(.05);
  sometext.SetTextColor(2);

  Canvas.Clear();  
  sprintf(text,"RESULTS:");
  sometext.DrawTextNDC(.05,.95,text);
  sprintf(text,"Mean= %.2f +- %.2f MeV",m0*1000,Em0*1000);
  sometext.DrawTextNDC(.05,.9,text);
  sprintf(text,"Width= %.2f +- %.2f MeV",Gamma*1000,EGamma*1000);
  sometext.DrawTextNDC(.05,.85,text);
  sprintf(text,"Yield= %d +- %d ",Nsig,ENsig);
  sometext.DrawTextNDC(.05,.8,text);
  sprintf(text,"Chisqure/ndof = %.1f/%d ",chisquare,ndof);
  sometext.DrawTextNDC(.05,.75,text);  
  sprintf(text,"Nbkg= %d +- %d ",Nbkg,ENbkg);
  sometext.DrawTextNDC(.05,.6,text);
  sprintf(text,"Coef0= %.2e ",polpar[0]);
  sometext.DrawTextNDC(.05,.55,text);
  sprintf(text,"Coef1= %.2e ",polpar[1]);
  sometext.DrawTextNDC(.05,.5,text);
  sprintf(text,"Coef2= %.2e ",polpar[2]);
  sometext.DrawTextNDC(.05,.45,text);
  Canvas.Print(fname);


  Canvas.Print(fname+"]");


  if(ds1pdeltam2frame!=NULL)delete ds1pdeltam2frame;
  if(HDs1pDM!=NULL)delete HDs1pDM;
  if(FPDFBkg!=NULL)delete FPDFBkg;
  if(res!=NULL)delete res;
  if(pdf!=NULL)delete pdf;
  if(data!=NULL)delete data;
  if(tree!=NULL)delete tree;

  
  return 1;
  
}

/* 
Int_t fitDs1WithRooFit(TString rootfile,Bool_t TruthMatch,Int_t WhichBkg,Int_t Nevents){

  TFile* file=new TFile(rootfile,"read");  
  if(file==NULL){
    cout<<"Bad file"<<endl;
    return 0;
  }
  TTree* tree=(TTree*)file->Get("Ds1pNtuple");
  if(tree!=NULL)cout<<"Ntuple: "<<rootfile<<" extracted with"<<tree->GetEntries()<<" entries"<<endl;
  tree->SetDirectory(0);
  file->Close();
  tree->SetBranchStatus("*",0);
  

  JDataBase database;
  database.SetCuts(1);

  ///Plot the results
  TCanvas Canvas("Canvas","Canvas");
  TString fname;


  /////
  Float_t fitmin=.01;
  Float_t fitmax=.05;


  ///Ntuple variables
  RooRealVar ds1pdeltam2("ds1pdeltam2","DeltaM",fitmin,fitmax);
  ds1pdeltam2.setBins(100);
  tree->SetBranchStatus("ds1pdeltam2",1); 
  ds1pdeltam2.Print();
  RooRealVar d0pstar("d0pstar","d0pstar",database.GetD0pstarMin(),5);
  tree->SetBranchStatus("d0pstar",1);
  d0pstar.Print();
  RooRealVar dstardeltam("dstardeltam","dstardeltam",database.GetDstarDeltaMMin(),database.GetDstarDeltaMMax());
  tree->SetBranchStatus("dstardeltam",1);
  dstardeltam.Print();
  RooRealVar klh("klh","klh",database.GetKLHBit(),15);
  tree->SetBranchStatus("klh",1);
  klh.Print();
  RooRealVar pilh("pilh","pilh",database.GetPiLHBit(),5);
  tree->SetBranchStatus("pilh",1);
  pilh.Print();
  RooRealVar d0vtxprob("d0vtxprob","d0vtxprob",database.GetD0MinProbab(),1.001);
  tree->SetBranchStatus("d0vtxprob",1);
  d0vtxprob.Print();
  RooRealVar ksmass("ksmass","ksmass",K0PDGMass-database.GetKsMassWindow(),K0PDGMass+database.GetKsMassWindow());
  tree->SetBranchStatus("ksmass",1);
  ksmass.Print();
  RooRealVar kscosine("kscosine","kscosine",database.GetKsCosineMin(),1.01);
  tree->SetBranchStatus("kscosine",1);
  kscosine.Print();
  RooRealVar d0mass("d0mass","d0mass",D0PDGMass-database.GetD0MassWindow(),D0PDGMass+database.GetD0MassWindow());
  tree->SetBranchStatus("d0mass",1);
  d0mass.Print();
  RooRealVar ds1pmctrue("ds1pmctrue","ds1pmctrue",.99,1.01);
  tree->SetBranchStatus("ds1pmctrue",1);
  ds1pmctrue.Print();
  RooRealVar eventnumber("eventnumber","eventnumber",1,Nevents);
  tree->SetBranchStatus("eventnumber",1);
  eventnumber.Print();

 
  RooArgList vars;
  vars.add(ds1pdeltam2);
  vars.add(d0pstar);  
  vars.add(dstardeltam);
  vars.add(pilh);
  vars.add(klh);
  vars.add(d0vtxprob);
  vars.add(ksmass);
  vars.add(kscosine);
  vars.add(d0mass);
  vars.add(eventnumber);


/////////////////try to use RooCategory to read integers: ???
//   RooCategory ds1pmctrue("ds1pmctrue","ds1pmctrue") ;
//   ds1pmctrue.defineType("1.000") ;
//   ds1pmctrue.defineType("0.000") ;
//   ds1pmctrue.setRange("cut","1.000") ;
//   // ds1pmctrue.setRange("cut","fake") ;




  //The Signal
  RooRealVar width("width","width",.001);
  // width.setRange(database.GetDs1pDeltaMMin(),database.GetDs1pDeltaMMax());
  width.setRange(.0005,.002);
  width.setConstant(kFALSE);
  
  RooRealVar mean("mean","mean",.027);
  //mean.setRange(database.GetDs1pDeltaMMin(),database.GetDs1pDeltaMMax());
  mean.setRange(.017,.037);
  mean.setConstant(kFALSE);

  //RooBreitWigner bw("bw","bw",ds1pdeltam2,mean,width);

  //relativistic bw
  TString relbwformula="ds1pdeltam2*mean*width/((ds1pdeltam2**2-mean**2)**2+(mean**2)*(width**2))";
  //RooGenericPdf relbw("relbw",relbwformula,RooArgSet(ds1pdeltam2,mean,width));

  RooGenericPdf* signalpdf;


  /////////The Resolution Function
  //TF1 Fdmresolution("Fdmres","[0]+[1]*x+[2]*x*82",2.2,5);
  //TF1 Fdmresolution("Fdmres","[0]+[1]*x+[2]*x*82",2.2,5);
  
  ////simple gaussian gaussianx
  //RooRealVar dmresbais("dmresbais","dmresbais",0);
  //RooRealVar dmres("dmres","dmres",.0013);  
  //RooGaussian respdf("respdf","res pdf",ds1pdeltam2,dmresbais,dmres);  
  ////breight wigner resolution
  //RooBreitWigner respdf("respdf","bw",ds1pdeltam2,dmresbais,dmres);

  ////use 3 gaussians with (same mean) as the resolution
  RooRealVar Mean("Mean","Mean",1.6e-5);  
  RooRealVar N1("N1","N1",9.538);
  RooRealVar S1("S1","S1",.0105);
  RooRealVar N2("N2","N2",244.4);
  RooRealVar S2("S2","S2",.0009727);
  RooRealVar N3("N3","N3",581.8);
  RooRealVar S3("S3","S3",.0004849);
  RooGenericPdf respdf("respdf","N1*exp(-.5*(ds1pdeltam2-Mean)**2/S1**2)+N2*exp(-.5*(ds1pdeltam2-Mean)**2/S2**2)+N3*exp(-.5*(ds1pdeltam2-Mean)**2/S3**2)",RooArgSet(ds1pdeltam2,N1,Mean,S1,N2,S2,N3,S3));


 

 
  ////The Background
  RooRealVar coef0("coef0","coef0",0,10000);
  coef0.setConstant(kFALSE);
  RooRealVar coef1("coef1","coef1",0,10000);
  coef1.setConstant(kFALSE);
  RooRealVar coef2("coef2","coef2",0,10000);
  coef2.setConstant(kFALSE);
  RooRealVar coef3("coef3","coef3",0,10000);
  coef3.setConstant(kFALSE);
  RooRealVar coef4("coef4","coef4",0,10000);
  coef4.setConstant(kFALSE);
  RooRealVar coef5("coef5","coef5",0,10000);
  coef5.setConstant(kFALSE);
  RooRealVar coef6("coef6","coef6",0,10000);
  coef6.setConstant(kFALSE);

  RooGenericPdf* bkg=NULL;
  //RooPolynomial* bkg=NULL;
  /////////
 
  if(TruthMatch){

    //bkg=new RooPolynomial("bkg","bkg",ds1pdeltam2,RooArgList(coef0));
    bkg=new RooGenericPdf("mybkg","coef0",RooArgList(ds1pdeltam2,coef0));

    vars.add(ds1pmctrue);
    signalpdf=new RooGenericPdf("sinalpdf","width/((ds1pdeltam2-mean)**2+.25*(width**2))",RooArgSet(ds1pdeltam2,mean,width));
    

    fname="Ds1RooFitMC.ps";
  }
  if(!TruthMatch){
    //////background
    //bkg=new RooPolynomial("bkg","bkg",ds1pdeltam2,RooArgList(coef0,coef1,coef2,coef3,coef4,coef5,coef6));

    /////better background
    if(WhichBkg==1)bkg=new RooGenericPdf("mybkg","coef0+coef1*ds1pdeltam2+coef2*ds1pdeltam2**.5+coef3*ds1pdeltam2**.333+coef4*ds1pdeltam2**.25+coef5*ds1pdeltam2**.2+coef6*ds1pdeltam2**1.666",RooArgList(ds1pdeltam2,coef0,coef1,coef2,coef3,coef4,coef5,coef6));  
    if(WhichBkg==2)bkg=new RooGenericPdf("mybkg","coef0+coef1*ds1pdeltam2+coef2*ds1pdeltam2**2+coef3*ds1pdeltam2**3+coef4*ds1pdeltam2**4+coef5*ds1pdeltam2**5+coef6*ds1pdeltam2**6",RooArgList(ds1pdeltam2,coef0,coef1,coef2,coef3,coef4,coef5,coef6));  
    if(WhichBkg==3)bkg=new RooGenericPdf("mybkg","coef0+coef1*ds1pdeltam2+coef2*ds1pdeltam2**2",RooArgList(ds1pdeltam2,coef0,coef1,coef2));  
 

    signalpdf=new RooGenericPdf("signalpdf",relbwformula,RooArgSet(ds1pdeltam2,mean,width));


    //plots name
    fname="Ds1RooFitDATA.ps";
  }
  

  /////////The Smeared signal
  RooNumConvPdf relbwsmeared("relbwsmeared","relbwsmeared",ds1pdeltam2,*signalpdf,respdf);
  relbwsmeared.setConvolutionWindow(Mean,S2,10);
  

  Canvas.Print(fname+"[");

  ///////////////////Lets check the smeared distribution
  RooPlot* ds1pdeltam2frame=ds1pdeltam2.frame(fitmin,fitmax);

  Canvas.Clear();  
  signalpdf->plotOn(ds1pdeltam2frame);
  relbwsmeared.plotOn(ds1pdeltam2frame);
  ds1pdeltam2frame->Draw();
  Canvas.Print(fname);  
  //if(ds1pdeltam2frame!=NULL)delete ds1pdeltam2frame;

//   Canvas.Clear();  
//   ds1pdeltam2frame=ds1pdeltam2.frame(fitmin,fitmax);
//   relbwsmeared.plotOn(ds1pdeltam2frame);
//   ds1pdeltam2frame->Draw();
//   Canvas.Print(fname);  
//   ////if(ds1pdeltam2frame!=NULL)delete ds1pdeltam2frame;

  ///////////////////////////////
  

  
  //data=new RooDataSet("data","data",tree,RooArgSet(ds1pdeltam2,d0pstar,dstardeltam,pilh,klh,kscosine,ds1pmctrue));
  RooDataSet* data=new RooDataSet("data","data",tree,vars);

  ///
  RooRealVar BkgNb("BkgNb","BkgNb",0,100000);
  //BkgNb.setError(1000);
  BkgNb.setConstant(kFALSE);
  RooRealVar sigPiNb("sigPiNb","sigPiNb",0,100000);  //Signal Yield
  //sigPiNb.setError(100);
  sigPiNb.setConstant(kFALSE);

  ///construct the total  pdf: signal plus background
  ///Total PDF
  //RooRealVar F("F","F",0.,1.0);
  //F.setConstant(kFALSE);
  if(bkg==NULL) return 0;
  //RooAddPdf* pdf=new RooAddPdf("SignalPlusBkgPdf","SignalPlusBkgPdf",RooArgList(relbw,*bkg),RooArgList(F));
  //RooAddPdf* pdf=new RooAddPdf("SignalPlusBkgPdf","SignalPlusBkgPdf",RooArgList(relbw,*bkg),RooArgList(sigPiNb,BkgNb));
  RooAddPdf* pdf=new RooAddPdf("SignalPlusBkgPdf","SignalPlusBkgPdf",RooArgList(relbwsmeared,*bkg),RooArgList(sigPiNb,BkgNb));
 


  ////time the fit 
  //////////time the code
  time_t* timer=NULL;
  Int_t starttime=0;
  Int_t endtime=0;
  starttime=time(timer);

  //Make the fit 
  RooFitResult* res=pdf->fitTo(*data,"mher");
  if(res==NULL){
    cout<<"No fit results returned."<<endl;
    return 0;
  }

  endtime=time(timer);
  cout<<"-----------------Time To Fit:"<<(endtime-starttime)/60<<" minutes"<<endl;

  ////////////////////////////////////Make Some plots
  TLine line;
 
//   ////////the Fit plot
//   Canvas.Clear();
//   ds1pdeltam2frame=ds1pdeltam2.frame(fitmin,fitmax);
//   data->plotOn(ds1pdeltam2frame);
//   pdf->plotOn(ds1pdeltam2frame);
//   ds1pdeltam2frame->Draw();
//   Canvas.Print(fname);
//   //if(ds1pdeltam2frame!=NULL)delete ds1pdeltam2frame;
 

  RooDataHist dmhist("dmhist","Ds1 Delta M",RooArgList(ds1pdeltam2),*data);  
  TH1* HDs1DM=dmhist.createHistogram("HDs1DM",ds1pdeltam2);
  if(HDs1DM==NULL){
    cout<<"No HDs1DM  returned."<<endl;
    return 0;
  }

  ///////////////residuals 



  RooArgList LastParList = res->floatParsFinal();
  RooRealVar* yield = (RooRealVar*)LastParList.find("sigPiNb");
  if(yield==NULL){
    cout<<"No yield  returned."<<endl;
    return 0;
  }
  Int_t Nsig  = (int)yield->getVal();
  Int_t ENsig = (int)yield->getError();
  RooRealVar* parmean = (RooRealVar*)LastParList.find("mean");
  if(parmean==NULL){
    cout<<"No parmean  returned."<<endl;
    return 0;
  }
  Float_t m0  = (float)parmean->getVal();
  Float_t Em0 = (float)parmean->getError();
  RooRealVar* parwidth = (RooRealVar*)LastParList.find("width");
  if(parwidth==NULL){
    cout<<"No parwidth  returned."<<endl;
    return 0;
  }
  Float_t Gamma  = (float)parwidth->getVal();
  Float_t EGamma = (float)parwidth->getError();


  Float_t integral=0;
  Int_t residual=0;
  Float_t residualerror=0;
  Int_t Nbins=HDs1DM->GetXaxis()->GetNbins();
  Float_t binwidth=HDs1DM->GetXaxis()->GetBinWidth(1);

  TGraphErrors* GRes=new TGraphErrors();
  TGraphErrors* GResPercent=new TGraphErrors();

  RooRealVar intrange("intrange","intrange",fitmin,fitmax);
  //RooAbsReal* inte= pdf->createIntegral(RooArgSet(intrange));

  ///////Creat at TF1 to make residuals
  TF1 FPDFSig("FPDFSig","[0]*(2*x*[1]*[2]/3.14159265358979312)/((x**2-[1]**2)**2+([1]**2)*([2]**2))",0,100);
  FPDFSig.SetParameter(0,1);//normalize integral to 1
  FPDFSig.SetParameter(1,m0);
  FPDFSig.SetParameter(2,Gamma);
  //cout<<"sig integral "<<FPDFSig.Integral(fitmin,fitmax)<<endl;





  /////The background
  RooRealVar* yieldbkg = (RooRealVar*)LastParList.find("BkgNb");
  if(yieldbkg==NULL){
    cout<<"No yieldbkg  returned."<<endl;
    return 0;
  }
  Int_t Nbkg  = (int)yieldbkg->getVal();
  Int_t ENbkg = (int)yieldbkg->getError();


  Int_t Npowers=7;
  Float_t polpar[7];
  for(Int_t i=0;i<Npowers;i++){   
    if(!TruthMatch)polpar[i]=(float)((RooRealVar*)LastParList.find(TString("coef")+(long)i))->getVal();

  }
  

  
  TF1* FPDFBkg=NULL;
  if(WhichBkg==1)FPDFBkg=new TF1("FPDFBkg","[7]*([0]+[1]*x+[2]*x**.5+[3]*x**.333+[4]*x**.25+[5]*x**.2+[6]*x**1.666)",fitmin,fitmax);
  if(WhichBkg==2)FPDFBkg=new TF1("FPDFBkg","[7]*([0]+[1]*x+[2]*x**2+[3]*x**3 +[4]*x**4 +[5]*x**5 +[6]*x**6)",fitmin,fitmax);
  

  
  //////////calculate normalization of bkg
  Float_t polnorm=0;
  for(Int_t i=0;i<Npowers;i++){
    FPDFBkg->SetParameter(i,polpar[i]);

    if(WhichBkg==1){
      if(i<=1)polnorm+=polpar[i]*(pow(fitmax,(float)(i+1))-pow(fitmin,(float)(i+1)))/(i+1.0);
      else polnorm+=polpar[i]*(pow(fitmax,(float)(1.0/i+1))-pow(fitmin,(float)(1.0/i+1)))/(1.0/i+1);
    }
    if(WhichBkg==2)polnorm+=polpar[i]*(pow(fitmax,(float)(i+1))-pow(fitmin,(float)(i+1)))/(i+1.0);    
  }

  if(polnorm<=0)cout<<"bkg norm not right"<<endl;
  FPDFBkg->SetParameter(7,1/polnorm);//normalize integral to 1



  //////////plot the fit
  Canvas.Clear();
  HDs1DM->Draw("pe");
  Canvas.Print(fname);

//   Canvas.Clear();
//   FPDFSig.Draw();
//   //FPDFBkg->Draw();
//   Canvas.Print(fname);
  
  Canvas.Clear();
  HDs1DM->Draw("pe");
  FPDFSig.SetParameter(0,Nsig*binwidth);
  FPDFSig.Draw("same");
  //FPDFBkg->Draw("same");
  Canvas.Print(fname);
  FPDFSig.SetParameter(0,1);





 

  ///Also compute  the chisquare
  Float_t chisquare=0;
 
  RooAbsReal* inte;

  for(Int_t bin=1;bin<=Nbins;bin++){
    ////try integrating the RooPdf
    //method 1
    //intrange.setRange(TString("subrange")+(long)bin,HDs1DM->GetBinLowEdge(bin),HDs1DM->GetBinLowEdge(bin)+binwidth) ;
    //inte= pdf->createIntegral(intrange,intrange,TString("subrange")+(long)bin);
    
    //method 2
    //intrange.setFitMin(HDs1DM->GetBinLowEdge(bin));
    //intrange.setFitMax(HDs1DM->GetBinLowEdge(bin)+binwidth);
        
    ///method 3
    //intrange.setRange(TString("subrange"),HDs1DM->GetBinLowEdge(bin),HDs1DM->GetBinLowEdge(bin)+binwidth) ;     
 
    
    //method 4
    ds1pdeltam2.setRange(TString("subrange")+(long)bin,HDs1DM->GetBinLowEdge(bin),HDs1DM->GetBinLowEdge(bin)+binwidth) ;
    inte= pdf->createIntegral(ds1pdeltam2,ds1pdeltam2,TString("subrange")+(long)bin);
    integral=(float)inte->getVal()*Nsig;
    ///////////////


    //integral=Nsig*FPDFSig.Integral(HDs1DM->GetBinLowEdge(bin),HDs1DM->GetBinLowEdge(bin)+binwidth)
    //      + Nbkg*FPDFBkg->Integral(HDs1DM->GetBinLowEdge(bin),HDs1DM->GetBinLowEdge(bin)+binwidth);

    residual=(int)(HDs1DM->GetBinContent(bin)-integral);
    cout<<bin<<" "<<HDs1DM->GetBinContent(bin)<<" "<<integral<<" "<<residual<<endl;

    residualerror=HDs1DM->GetBinError(bin);

    GRes->SetPoint(bin,HDs1DM->GetBinLowEdge(bin)+binwidth/2,residual);
    GRes->SetPointError(bin,0,residualerror);

    GResPercent->SetPoint(bin,HDs1DM->GetBinLowEdge(bin)+binwidth/2,100*residual/HDs1DM->GetBinContent(bin));
    GResPercent->SetPointError(bin,0,100*residualerror/HDs1DM->GetBinContent(bin));
    
    chisquare+=(residual*residual)/(residualerror*residualerror);
  }
  Int_t ndof=Nbins-(6+2);

  Canvas.Clear();  
  // GRes->Draw("ap");
  GRes->GetYaxis()->SetRangeUser(-30.,30.);
  GRes->GetXaxis()->SetRangeUser(HDs1DM->GetBinLowEdge(1),HDs1DM->GetBinLowEdge(Nbins)+binwidth);
  GRes->GetYaxis()->SetTitle("Residual");
  GRes->GetXaxis()->SetTitle("DeltaM(D_{s1})");
  GRes->Draw("ape");
  line.DrawLine(0,.0,HDs1DM->GetBinLowEdge(Nbins)+binwidth,0); 
  Canvas.Print(fname);  


//   Canvas.Clear();
 
//   //Canvas.cd(2);
//   GResPercent->Draw("ap");
//   GResPercent->GetYaxis()->SetRangeUser(-100.,100.);
//   GResPercent->GetXaxis()->SetRangeUser(HDs1DM->GetBinLowEdge(1),HDs1DM->GetBinLowEdge(Nbins)+binwidth);
//   GResPercent->GetYaxis()->SetTitle("Residual/Entries");
//   GResPercent->GetXaxis()->SetTitle("DeltaM(D_{s1})");
//   GResPercent->Draw("ape");
//   line.DrawLine(0,.0,HDs1DM->GetBinLowEdge(Nbins)+binwidth,0);  
//   Canvas.Print(fname);


  ////////done saving plots
  Canvas.Print(fname+"]");
  ///////////////////////////////////////////////////////////////////
  
  ////////////////print the results
  res->Print();

  
  Float_t corrmw=(float)res->correlation("mean","width");
  Float_t corrmm=(float)res->correlation("mean","mean");
  Float_t corrww=(float)res->correlation("width","width");

  cout<<"+++++++++++++++++RESULTS+++++++++++++"<<endl;
  cout<<"Correlation matrix"<<endl;
  cout<<corrmm<<" "<<corrmw<<endl;
  cout<<corrmw<<" "<<corrww<<endl;
  cout<<endl;
  cout<<"min log(l)"<<endl;
  cout<<res->minNll()<<endl;
  cout<<endl;

  cout<<"# Signal Events="<<Nsig<<"+-"<<ENsig<<endl;
  cout<<"# Signal m0="<<m0<<"+-"<<Em0<<endl;
  cout<<"# Signal Width ="<<Gamma<<"+-"<<EGamma<<endl; 
  cout<<"sig integral "<<FPDFSig.Integral(0,10000)<<endl;///tails on the rel. bw are very long

  cout<<"# Bkg  Events="<<Nbkg<<"+-"<<ENbkg<<endl;
  cout<<"bkg ntegral "<<FPDFBkg->Integral(fitmin,fitmax)<<endl;

  

  cout<<endl;
  cout<<"Chisquare/ndof ="<<chisquare<<"/"<<ndof<<endl;

  cout<<"+++++++++++++++++RESULTS+++++++++++++"<<endl;






  if(ds1pdeltam2frame!=NULL)delete ds1pdeltam2frame;
  if(HDs1DM!=NULL)delete HDs1DM;
  if(FPDFBkg!=NULL)delete FPDFBkg;
  if(res!=NULL)delete res;
  if(pdf!=NULL)delete pdf;
  if(data!=NULL)delete data;
  if(tree!=NULL)delete tree;

  
  return 1;
  
}
*/
