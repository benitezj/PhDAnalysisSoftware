#include "DstPiAnalysis.h"
#include "RooBreitWigner.h"
#include "../RooRelBreitWigner.h"
#include "../RooSwaveBreitWigner.h"
#include "../RooDwaveBreitWigner.h"
#include "../Roo2BodyPhaseSpace.h"

#define BkgPolNpowers 20
TH1F* CreateRebinnedDstPiHisto(TH1*,Int_t nbins,Float_t min,Float_t max);


Int_t DstPiAnalysis::RooFitMassPeak(Bool_t fit,Int_t whichshape1,Int_t whichshape2,Bool_t applyPhaseFctr,Bool_t CorrRes,Bool_t CorrEff,Bool_t AddK3pi,Int_t fileid,Bool_t CreateNewHistos){
  cout<<"Going to fit the D*Pi Mass Peak"<<endl;

  //ps file
  TString filename=_OutputDir+"/DstPiRooFit_"+(long)fileid+".ps";
  TCanvas Canvas(filename,filename);
  Canvas.Print(filename+"[");
   

  ///print the cuts into the file
  database.PrintCutList(&Canvas,filename);  

  

  //--------------------------------------------------------
  ///parameters to be determined from a fit
  //--------------------------------------------------------
  Float_t fitmin=.25;
  Float_t fitmax=.6;
  Int_t nfitbins=70;    


  Int_t Nsig1 =0; Int_t Nsig1min =0; Int_t Nsig1max =0;
  Int_t Nsig1Error = 0;
  Bool_t Nsig1Fix=0;
  Float_t m01 = 0; Float_t m01min = 0; Float_t m01max = 0;
  Float_t m01Error = 0;
  Bool_t m01Fix=1;
  Float_t Gamma1  = 0;Float_t Gamma1min  = 0;Float_t Gamma1max  = 0;
  Float_t Gamma1Error =0;
  Bool_t Gamma1Fix=1;

  Int_t Nsig2 =0;Int_t Nsig2min =0;Int_t Nsig2max =0;
  Int_t Nsig2Error = 0;
  Bool_t Nsig2Fix=0;
  Float_t m02  = 0; Float_t m02min  = 0; Float_t m02max  = 0;
  Float_t m02Error = 0;
  Bool_t m02Fix=1;
  Float_t Gamma2  = 0; Float_t Gamma2min  = 0; Float_t Gamma2max  = 0;
  Float_t Gamma2Error =0;
  Bool_t Gamma2Fix=1;


  Int_t Nsig3 =0;Int_t Nsig3min =0;Int_t Nsig3max =0;
  Int_t Nsig3Error = 0;
  Bool_t Nsig3Fix=0;
  Float_t m03  = 0; Float_t m03min  = 0; Float_t m03max  = 0;
  Float_t m03Error = 0;
  Bool_t m03Fix=1;
  Float_t Gamma3  = 0; Float_t Gamma3min  = 0; Float_t Gamma3max  = 0;
  Float_t Gamma3Error =0;
  Bool_t Gamma3Fix=1;

  Int_t Nsig4 =0;Int_t Nsig4min =0;Int_t Nsig4max =0;
  Int_t Nsig4Error = 0;
  Bool_t Nsig4Fix=0;
  Float_t m04  = 0; Float_t m04min  = 0; Float_t m04max  = 0;
  Float_t m04Error = 0;
  Bool_t m04Fix=1;
  Float_t Gamma4  = 0; Float_t Gamma4min  = 0; Float_t Gamma4max  = 0;
  Float_t Gamma4Error =0;
  Bool_t Gamma4Fix=1;

  Int_t Nbkg = 0;Int_t Nbkgmin = 0;Int_t Nbkgmax = 0;
  Int_t NbkgError = 0;
  Bool_t NbkgFix=0;
  Double_t polpar[BkgPolNpowers];Double_t polparmin[BkgPolNpowers];Double_t polparmax[BkgPolNpowers];
  Float_t polparError[BkgPolNpowers];
  Bool_t polparFix[BkgPolNpowers];
  for(Int_t i=0;i<BkgPolNpowers;i++){   
    polpar[i]=0;
    polparError[i]=0;
    polparFix[i]=0;
  }
  

  //---------------------------
  //read the parameters from the file
  //----------------------------    
  ifstream ParameterInput;
  ParameterInput.open((const char*)(_OutputDir+"/InitRooFitPars.txt"));   
  //remember to initialize integers with integer values otherwise they are not read in properly
  ParameterInput>>fitmin >>fitmax    >>nfitbins;

  ParameterInput>>Nsig1  >>Nsig1min  >>Nsig1max  >>Nsig1Fix;
  ParameterInput>>m01    >>m01min    >>m01max    >>m01Fix;
  ParameterInput>>Gamma1 >>Gamma1min >>Gamma1max >>Gamma1Fix; 

  ParameterInput>>Nsig2  >>Nsig2min  >>Nsig2max  >>Nsig2Fix;
  ParameterInput>>m02    >>m02min    >>m02max    >>m02Fix;
  ParameterInput>>Gamma2 >>Gamma2min >>Gamma2max >>Gamma2Fix;

  ParameterInput>>Nsig3  >>Nsig3min  >>Nsig3max  >>Nsig3Fix;
  ParameterInput>>m03    >>m03min    >>m03max    >>m03Fix;
  ParameterInput>>Gamma3 >>Gamma3min >>Gamma3max >>Gamma3Fix;

  ParameterInput>>Nsig4  >>Nsig4min  >>Nsig4max  >>Nsig4Fix;
  ParameterInput>>m04    >>m04min    >>m04max    >>m04Fix;
  ParameterInput>>Gamma4 >>Gamma4min >>Gamma4max >>Gamma4Fix;

  ParameterInput>>Nbkg   >>Nbkgmin   >>Nbkgmax   >>NbkgFix;
  Int_t checkpar;
  for(Int_t i=0;i<BkgPolNpowers;i++){
    ParameterInput>>checkpar>>polpar[i]>>polparmin[i]>>polparmax[i]>>polparFix[i];
    if(checkpar!=i){
      cout<<"polynomial pars not read right: "<<i<<endl;
      return 0;
    }
  }

  ParameterInput.close();
  ParameterInput.clear();


  //print-out the parameters
  cout<<fitmin<<" "<<fitmax<<" "<<nfitbins<<endl;

  cout<<Nsig1<<" "<<Nsig1Fix<<endl;
  cout<<m01<<" "<<m01Fix<<endl;
  cout<<Gamma1<<" "<<Gamma1Fix<<endl; 

  cout<<Nsig2<<" "<<Nsig2Fix<<endl;
  cout<<m02<<" "<<m02Fix<<endl;
  cout<<Gamma2<<" "<<Gamma2Fix<<endl;

  cout<<Nsig3<<" "<<Nsig3Fix<<endl;
  cout<<m03<<" "<<m03Fix<<endl;
  cout<<Gamma3<<" "<<Gamma3Fix<<endl;

  cout<<Nsig4<<" "<<Nsig4Fix<<endl;
  cout<<m04<<" "<<m04Fix<<endl;
  cout<<Gamma4<<" "<<Gamma4Fix<<endl;

  cout<<Nbkg<<" "<<NbkgFix<<endl;
  for(Int_t i=0;i<BkgPolNpowers;i++)   
    cout<<i<<" "<<polpar[i]<<" "<<polparFix[i]<<endl;




  

  ///---------------------
  //The variable to be fitted
  //----------------------
  RooRealVar roodstpim("dstpim","dstpim",fitmin,fitmax);
  roodstpim.setBins(nfitbins); 
  roodstpim.Print();
  



  //-------------------
  //2-Body phase factor
  //-------------------
  RooGenericPdf PhaseFctrPdf("PhaseFctrPdf",phasefactor,RooArgSet(roodstpim));
  
  RooRealVar dau1m("dau1m","dau1m",DstarPDGMass);
  RooRealVar dau2m("dau2m","dau2m",PiPDGMass);
  Roo2BodyPhaseSpace TwoBodyPS("TwoBodyPS","TwoBodyPS",roodstpim,dau1m,dau2m);
  RooRealVar radius("radius","radius",.1);
  
  

  //-----------------
  //the signal pdfs
  //-----------------
  RooRealVar mean1("mean1","mean1",m01,m01min,m01max);//pdg=412+-1.3 MeV
  mean1.setConstant(m01Fix); 
  RooRealVar width1("width1","width1",Gamma1,Gamma1min,Gamma1max);//pdg=20.4 +- 6 MeV
  width1.setConstant(Gamma1Fix);

  TObject* temp=NULL;   
  
  //------final lineshape  
  RooAbsPdf* Signal1Pdf=NULL;
  TString shape1="BW";
  if(whichshape1==0){
    Signal1Pdf=new RooBreitWigner("Signal1BWPdf","Signal1BWPdf",roodstpim,mean1,width1);
    shape1="BW";  
  }
  if(whichshape1==1){
    Signal1Pdf=new RooRelBreitWigner("Signal1RelBWPdf","Signal1RelBWPdf",roodstpim,mean1,width1);
    shape1="RelBW";
  }
  if(whichshape1==2){
    Signal1Pdf=new RooSwaveBreitWigner("Signal1Spin1Pdf","Signal1Spin1Pdf",roodstpim,mean1,width1,&TwoBodyPS);
    shape1="Spin1";
  }
  if(whichshape1==3){
    Signal1Pdf=new RooDwaveBreitWigner("Signal1Spin2Pdf","Signal1Spin2Pdf",roodstpim,mean1,width1,&TwoBodyPS,radius);
    shape1="Spin2";
  }
  if(!Signal1Pdf)return 0;
  if(applyPhaseFctr){
    temp=Signal1Pdf;
    Signal1Pdf=new RooProdPdf("Signal1PSPdf","Signal1PSPdf",TwoBodyPS,*Signal1Pdf);
    //delete temp;//clean up
  }


  ///signal 2
  RooRealVar mean2("mean2","mean2",m02,m02min,m02max);//451+-1.6 MeV
  mean2.setConstant(m02Fix);
  RooRealVar width2("width2","width2",Gamma2,Gamma2min,Gamma2max);//pdg=43 +- 4 MeV   
  width2.setConstant(Gamma2Fix); 

  RooAbsPdf* Signal2Pdf=NULL;
  TString shape2="BW";
  if(whichshape2==0){
    Signal2Pdf=new RooBreitWigner("Signal2BWPdf","Signal2BWPdf",roodstpim,mean2,width2);
    shape2="BW";  
  }
  if(whichshape2==1){
    Signal2Pdf=new RooRelBreitWigner("Signal2RelBWPdf","Signal2RelBWPdf",roodstpim,mean2,width2);
    shape2="RelBW";
  }
  if(whichshape2==2){
    Signal2Pdf=new RooSwaveBreitWigner("Signal2Spin1Pdf","Signal2Spin1Pdf",roodstpim,mean2,width2,&TwoBodyPS);
    shape2="Spin1";
  }
  if(whichshape2==3){
    Signal2Pdf=new RooDwaveBreitWigner("Signal2Spin2Pdf","Signal2Spin2Pdf",roodstpim,mean2,width2,&TwoBodyPS,radius);
    shape2="Spin2";
  }
  if(!Signal2Pdf)return 0;
  if(applyPhaseFctr){
    temp=Signal2Pdf;
    Signal2Pdf=new RooProdPdf("Signal2PSPdf","Signal2PSPdf",TwoBodyPS,*Signal2Pdf);
    //delete temp;
  }




  
 


  RooRealVar mean3("mean3","mean3",m03,m03min,m03max);//451+-1.6 MeV
  mean3.setConstant(m03Fix);
  RooRealVar width3("width3","width3",Gamma3,Gamma3min,Gamma3max);//pdg=43 +- 4 MeV   
  width3.setConstant(Gamma3Fix); 
  //----------Rel-BW
  RooRelBreitWigner Signal3RelBWPdf("Signal3RelBWPdf","Signal3RelBWPdf",roodstpim,mean3,width3);
  RooProdPdf Signal3PSRelBWPdf("Signal3PSRelBWPdf","Signal3PSRelBWPdf",TwoBodyPS,Signal3RelBWPdf);
   //------final lineshape
  RooProdPdf* Signal3Pdf=NULL; 
  Signal3Pdf=&Signal3PSRelBWPdf;
  if(!Signal3Pdf)return 0;


  RooRealVar mean4("mean4","mean4",m04,m04min,m04max);//451+-1.6 MeV
  mean4.setConstant(m04Fix);
  RooRealVar width4("width4","width4",Gamma4,Gamma4min,Gamma4max);//pdg=43 +- 4 MeV   
  width4.setConstant(Gamma4Fix); 
  //----------Rel-BW
  RooRelBreitWigner Signal4RelBWPdf("Signal4RelBWPdf","Signal4RelBWPdf",roodstpim,mean4,width4);
  RooProdPdf Signal4PSRelBWPdf("Signal4PSRelBWPdf","Signal4PSRelBWPdf",TwoBodyPS,Signal4RelBWPdf);
   //------final lineshape
  RooProdPdf* Signal4Pdf=NULL; 
  Signal4Pdf=&Signal4PSRelBWPdf;
  if(!Signal4Pdf)return 0;



  //------------------------------------------------
  //
  //------------------------------------------------
  RooPlot* dstpimframe=roodstpim.frame(fitmin,fitmax);
  
  Canvas.Clear();    
  dstpimframe=roodstpim.frame(2.1,2.2);
  TwoBodyPS.plotOn(dstpimframe,LineColor(1));
  dstpimframe->Draw();
  Canvas.Print(filename);  
 


  //---------------------
  //The resolution functions: no cut on the cos(thetaD*) //needs update from deltaM to M
  //---------------------   
  RooRealVar ResMean("ResMean","ResMean",0);  
  RooRealVar ResSigma("ResSigma","ResSigma",.0017);//aproximately

  TString ressigma1="(.00019 + .0035*(dstpim-2.01))";//remember plots are scaled by 1000
  TString sigmaratio1="(3.5 - 1.9*(dstpim-2.01))";
  RooGenericPdf respdf1("respdf1","respdf1","exp(-.5*((dstpim-2.01)-(ResMean-2.01))**2/"+ressigma1+"**2)+.08*exp(-.5*((dstpim-2.01)-(ResMean-2.01))**2/("+sigmaratio1+"*"+ressigma1+")**2)",RooArgSet(roodstpim,ResMean)); 

  TString ressigma2="(.00007 + .0039*(dstpim-2.01))";//remember plots are scaled by 1000
  TString sigmaratio2="(4.0 - 3.6*(dstpim-2.01))";
  RooGenericPdf respdf2("respdf2","respdf2","exp(-.5*((dstpim-2.01)-(ResMean-2.01))**2/"+ressigma2+"**2)+.08*exp(-.5*((dstpim-2.01)-(ResMean-2.01))**2/("+sigmaratio2+"*"+ressigma2+")**2)",RooArgSet(roodstpim,ResMean)); 


  roodstpim.setRange(2.31,2.61);//this fixes some kind of precission problem in the plot related the with of the guassians
  roodstpim.setBins(nfitbins*100);
  dstpimframe=roodstpim.frame(fitmin,fitmax);
  Canvas.Clear();
  ResMean.setVal(2.360);
  respdf1.plotOn(dstpimframe);
  ResMean.setVal(2.410);
  respdf1.plotOn(dstpimframe);
  ResMean.setVal(2.460);
  respdf1.plotOn(dstpimframe);
  ResMean.setVal(2.510);
  respdf1.plotOn(dstpimframe);
  ResMean.setVal(2.560);
  respdf1.plotOn(dstpimframe);
  dstpimframe->Draw(); 
  Canvas.Print(filename); 
  roodstpim.setRange(fitmin,fitmax);  
  roodstpim.setBins(nfitbins);
  ResMean.setVal(0);


  //----------------------------------------------------
  // make convolution of signal with resolution function
  //---------------------------------------------------- 
  RooNumConvPdf Signal1PdfSmeared("Signal1PdfSmeared","Signal1PdfSmeared",roodstpim,*Signal1Pdf,respdf1);
  Signal1PdfSmeared.setConvolutionWindow(ResMean,ResSigma,5);
  RooNumConvPdf Signal2PdfSmeared("Signal2PdfSmeared","Signal2PdfSmeared",roodstpim,*Signal2Pdf,respdf2);
  Signal2PdfSmeared.setConvolutionWindow(ResMean,ResSigma,5);
  
 
  if(CorrRes){
    temp=Signal1Pdf;
    Signal1Pdf=new RooNumConvPdf("Signal1PdfSmeared","Signal1PdfSmeared",roodstpim,*Signal1Pdf,respdf1);
    //delete temp;

    temp=Signal2Pdf;
    Signal2Pdf=new RooNumConvPdf("Signal2PdfSmeared","Signal2PdfSmeared",roodstpim,*Signal2Pdf,respdf2);
    //delete temp;
  }
   

  ///--------------------------------------------
  ///The efficiency functions: no cut on the cos(thetaD*) //needs update from deltaM to M
  //----------------------------------------------
  RooGenericPdf sig1eff("sig1eff",".1265+.01395*(dstpim-2.01)",RooArgSet(roodstpim));
  RooGenericPdf sig2eff("sig2eff",".1305+.00639*(dstpim-2.01)",RooArgSet(roodstpim)); 
  //plot the efficiencies
  dstpimframe=roodstpim.frame(fitmin,fitmax);
  Canvas.Clear();
  sig1eff.plotOn(dstpimframe,LineColor(1));
  sig2eff.plotOn(dstpimframe,LineColor(2));
  dstpimframe->Draw();
  Canvas.Print(filename);    


  if(CorrEff){
    temp=Signal1Pdf;
    Signal1Pdf=new RooProdPdf("Signal1PdfSmearedEffCorr","Signal1PdfSmearedEffCorr",*Signal1Pdf,sig1eff);
    //delete temp;

    temp=Signal2Pdf;
    Signal2Pdf=new RooProdPdf("Signal2PdfSmearedEffCorr","Signal2PdfSmearedEffCorr",*Signal2Pdf,sig2eff);
    //delete temp;
  }


  //----------------------------------------------------
  //Multiply signal with efficiency function
  //---------------------------------------------------- 
  RooProdPdf Signal1PdfSmearedEffCorr("Signal1PdfSmearedEffCorr","Signal1PdfSmearedEffCorr",Signal1PdfSmeared,sig1eff);
  RooProdPdf Signal2PdfSmearedEffCorr("Signal2PdfSmearedEffCorr","Signal2PdfSmearedEffCorr",Signal2PdfSmeared,sig2eff);



  //---------------------------
  //The Background: 
  //---------------------------
  RooArgList coeflist;
  coeflist.add(roodstpim);
  RooRealVar* coef[BkgPolNpowers];//I get break seg. when I try create an object array of RooRealVars
  for(Int_t i=0;i<BkgPolNpowers;i++){
    coef[i]=new RooRealVar(TString("coef")+(long)i,TString("coef")+(long)i,polpar[i],polparmin[i],polparmax[i]);
    coef[i]->setConstant(polparFix[i]);
    coeflist.add(*coef[i]);
  }


  //------------verysimple: 2 exponential decays //needs update from deltaM to M
  TString BkgWS2="exp(-(coef0*(dstpim-2.01))**coef1)+coef2*exp(-coef3*(dstpim-2.01))";
  RooGenericPdf bkg("bkg",BkgWS2,coeflist);
  RooProdPdf BkgPdf("BkgPdf","BkgPdf",PhaseFctrPdf,bkg);




  //--------------------------------------------------------
  ////construct the total  pdf: signal plus background
  //--------------------------------------------------------  
  RooRealVar sig1Yield("sig1Yield","sig1Yield",Nsig1,Nsig1min,Nsig1max);  
  sig1Yield.setConstant(Nsig1Fix);          
  RooRealVar sig2Yield("sig2Yield","sig2Yield",Nsig2,Nsig2min,Nsig2max); 
  sig2Yield.setConstant(Nsig2Fix);      
  RooRealVar sig3Yield("sig3Yield","sig3Yield",Nsig3,Nsig3min,Nsig3max); 
  sig3Yield.setConstant(Nsig3Fix);      
  RooRealVar sig4Yield("sig4Yield","sig4Yield",Nsig4,Nsig4min,Nsig4max); 
  sig4Yield.setConstant(Nsig4Fix);       
  RooRealVar BkgYield("BkgYield","BkgYield",Nbkg,Nbkgmin,Nbkgmax);
  BkgYield.setConstant(NbkgFix);  


  RooAddPdf totalpdf("totalpdf","totalpdf",RooArgList(*Signal1Pdf,*Signal2Pdf,*Signal3Pdf,*Signal4Pdf,BkgPdf),RooArgList(sig1Yield,sig2Yield,sig3Yield,sig4Yield,BkgYield));

  //------------------------------------------------
  //plot pdfs
  //------------------------------------------------
  Canvas.Clear();    
  dstpimframe=roodstpim.frame(fitmin,fitmax);
  Signal1Pdf->plotOn(dstpimframe,LineColor(2));
  Signal2Pdf->plotOn(dstpimframe,LineColor(3));
  Signal3Pdf->plotOn(dstpimframe,LineColor(4));
  Signal4Pdf->plotOn(dstpimframe,LineColor(6));
  BkgPdf.plotOn(dstpimframe,LineColor(1));
  totalpdf.plotOn(dstpimframe,LineColor(4));
  dstpimframe->Draw();
  Canvas.Print(filename); 


  //-------------------------------------------------------- 
  //unbinned data
  //--------------------------------------------------------
  //RooDataSet unbinneddata("unbinneddata","unbinneddata",ReducedNtupleCopy,RooArgList(roodstpim));
  //create the final binned data
  //RooDataHist binneddata("binneddata","binneddata",RooArgList(roodstpim),HDstPiDMFinal);
  if(CreateNewHistos)
    if(!CreateDeltaMHistogramForFit())return 0;

  TFile HistFile(_OutputDir+"/HistosForFit.root","read");
  Canvas.Clear();
  TH1F* HDstar=(TH1F*)HistFile.Get("HDstar");
  if(!HDstar)return 0;
  SetHistoXYLabels(HDstar,"m(K^{+}#pi^{-}#pi^{+})-m(K^{+}#pi^{-})  (GeV/c^{2})","GeV");
  HDstar->Draw("pe");
  Canvas.Print(filename);

  Canvas.Clear();
  TH1F* HDstarPi=(TH1F*)HistFile.Get("HDstarPi");
  if(!HDstarPi)return 0;
  SetHistoXYLabels(HDstarPi,DstarPiXLabel,"GeV");
  HDstarPi->Draw("pe");
  Canvas.Print(filename);


  TH1F* HDstarPiFit=CreateRebinnedDstPiHisto((TH1*)HDstarPi,nfitbins,fitmin,fitmax);
  if(!HDstarPiFit)return 0;
  Canvas.Clear();
  SetHistoXYLabels(HDstarPiFit,DstarPiXLabel,"GeV");
  HDstarPiFit->Draw("pe");
  Canvas.Print(filename);    


  //--------------------------------------------------------
  //data for the binnned fit:
  //--------------------------------------------------------
  //the kpi data
  //RooDataHist kpibinneddata("kpibinneddata","kpibinneddata",RooArgList(roodstpim),unbinneddata);
  //TH1* HDstarPiFit=kpibinneddata.createHistogram("HDstarPiFit",roodstpim);
  TH1* HDstPiDMFinal=(TH1*)HDstarPiFit->Clone();  


  
  if(AddK3pi){
    HDstarPiFit->Sumw2();
    //now get the K3pi data
    TFile f("DstarCharged/DstarPi/K3pi/Reduced.root");
    gROOT->cd();
    TTree* t0=(TTree*)f.Get("CleanNtuple");
    TTree* k3pitree=t0->CopyTree(database.GetCut(1,100));
    if(!k3pitree)return 0;
    RooDataSet k3piunbinneddata("k3piunbinneddata","k3piunbinneddata",k3pitree,RooArgList(roodstpim));
    RooDataHist k3pibinneddata("k3pibinneddata","k3pibinneddata",RooArgList(roodstpim),k3piunbinneddata);    
    //get the uncorr k3pi histogram
    TH1* HDstPiDMK3pi=k3pibinneddata.createHistogram("HDstPiDMK3pi",roodstpim);
    //make a plot of the difference before
    Canvas.Clear();
    TH1*HDstPiDMK3piScale=(TH1*)HDstPiDMK3pi->Clone();
    HDstPiDMK3piScale->Scale(HDstarPiFit->Integral()/HDstPiDMK3piScale->Integral());
    TH1*HDstarPiFitDiffBefore=(TH1*)HDstarPiFit->Clone();
    HDstarPiFitDiffBefore->Add(HDstPiDMK3piScale,-1);
    HDstarPiFitDiffBefore->GetYaxis()->SetRangeUser(-1000,1000);
    HDstarPiFitDiffBefore->Draw("pe");
    Canvas.Print(filename);
    //get the bkg from D* bkg
    TH1* HDstPiDMK3piDstBkg=K3piDstarBkg(nfitbins,fitmin,fitmax); 
    if(!HDstPiDMK3piDstBkg)return 0;
    //subtract to correct k3pi
    TH1* HDstPiDMK3piDstBkgSub=(TH1*)HDstPiDMK3pi->Clone();
    HDstPiDMK3piDstBkgSub->Add(HDstPiDMK3piDstBkg,-1);  

    //make a plot of the difference after D* bkg correction
    Canvas.Clear();
    TH1*HDstPiDMK3piScaleAfter=(TH1*)HDstPiDMK3piDstBkgSub->Clone();
    HDstPiDMK3piScaleAfter->Scale(HDstarPiFit->Integral()/HDstPiDMK3piScaleAfter->Integral());
    TH1*HDstarPiFitDiffAfter=(TH1*)HDstarPiFit->Clone();
    HDstarPiFitDiffAfter->Add(HDstPiDMK3piScaleAfter,-1);
    HDstarPiFitDiffAfter->GetYaxis()->SetRangeUser(-1000,1000);
    HDstarPiFitDiffAfter->Draw("pe");
    Canvas.Print(filename);

    ///add the two modes
    HDstPiDMFinal->Add(HDstPiDMK3piDstBkgSub);

    delete k3pitree;
    delete HDstPiDMK3pi;
    delete HDstPiDMK3piScale;
    delete HDstarPiFitDiffBefore;
    delete HDstPiDMK3piDstBkg;
    delete HDstPiDMK3piDstBkgSub;
    delete HDstPiDMK3piScaleAfter;
    delete HDstarPiFitDiffAfter;
    
  }
  
  //create the final binned data
  RooDataHist binneddata("binneddata","binneddata",RooArgList(roodstpim),HDstPiDMFinal);
  
   

  //--------------------------------------------------------
  //the results from the Fit (if it is done)
  //--------------------------------------------------------
  RooFitResult* res=NULL;
  RooArgList* floatedParameters=NULL;

  //---------------------
  //Make the fit 
  //---------------------
  if(fit) res=totalpdf.fitTo(binneddata,"mhert");//"mhert"
  if(!res){
    cout<<"no results"<<endl;
    Canvas.Print(filename+"]");return 0;
  }
  //----------------------------------
  //extract the floated parameters
  //----------------------------------
  if(fit) floatedParameters = (RooArgList*)&res->floatParsFinal();
  
  

  //----------------------------
  //the Fit plot
  //----------------------------
  Canvas.Clear();  
  dstpimframe=roodstpim.frame(fitmin,fitmax);
  binneddata.plotOn(dstpimframe);
  totalpdf.plotOn(dstpimframe);
  totalpdf.plotOn(dstpimframe,Components(BkgPdf),LineStyle(kDashed),LineColor(1));
  totalpdf.plotOn(dstpimframe,Components(*Signal1Pdf),LineStyle(kSolid),LineColor(2));
  totalpdf.plotOn(dstpimframe,Components(*Signal2Pdf),LineStyle(kSolid),LineColor(2));
  totalpdf.plotOn(dstpimframe,Components(*Signal3Pdf),LineStyle(kSolid),LineColor(2));
  totalpdf.plotOn(dstpimframe,Components(*Signal4Pdf),LineStyle(kSolid),LineColor(2));
  dstpimframe->Draw();
  Canvas.Print(filename);


  //----------------------------
  //create a histogram of the data
  //----------------------------
  TH1* HDstPiDM=binneddata.createHistogram("HDstPiDM",roodstpim);
  if(HDstPiDM==NULL){
    cout<<"No HDstPiDM  returned."<<endl;
    Canvas.Print(filename+"]");return 0;
  }

 

  //----------------------------
  //now make the residual plot
  //----------------------------
  Float_t chisquare=0; 
  Int_t ndof=1;
  TH1* HFitDstPiDM=totalpdf.createHistogram("HFitDstPiDM",roodstpim);
  if(HFitDstPiDM==NULL){
    cout<<"No HDstPiDM  returned."<<endl;
     Canvas.Print(filename+"]");return 0;
  }
  HFitDstPiDM->Scale(HDstPiDM->Integral()); 

 
  //JResiduals roofitresiduals(HDstPiDM,&totalpdf,&roodstpim);//using the Pdf is tricky
  JResiduals roofitresiduals(HDstPiDM,HFitDstPiDM);
  chisquare=roofitresiduals.GetChisquare(); 
  if(fit) ndof=HDstPiDM->GetXaxis()->GetNbins() - floatedParameters->getSize();
  
  Canvas.Clear();
  roofitresiduals.MakeFitPlotWithResiduals(&Canvas);
  Canvas.Print(filename); 

  TH1F* ResidualHisto=roofitresiduals.GetResidualHisto();
  Canvas.Clear();
  ResidualHisto->Draw("pe");
  cutline.SetLineColor(1);
  cutline.DrawLine(fitmin,0,fitmax,0);
  Canvas.Print(filename); 
  
  TH1F* HChiVsBin=roofitresiduals.GetChisquareHisto();    
  Canvas.Clear();
  HChiVsBin->Draw("p");
  Canvas.Print(filename); 

  ///--------------------------------
  //extract the parameters
  //-------------------------------
  Nsig1  = (int)sig1Yield.getVal();
  Nsig1Error = (int)sig1Yield.getError();
  m01  = (float)mean1.getVal();
  m01Error = (float)mean1.getError();
  Gamma1  = (float)width1.getVal();
  Gamma1Error = (float)width1.getError();  

  Nsig2  = (int)sig2Yield.getVal();
  Nsig2Error = (int)sig2Yield.getError();
  m02  = (float)mean2.getVal();
  m02Error = (float)mean2.getError();
  Gamma2  = (float)width2.getVal();
  Gamma2Error = (float)width2.getError();   

  Nsig3  = (int)sig3Yield.getVal();
  Nsig3Error = (int)sig3Yield.getError();
  m03  = (float)mean3.getVal();
  m03Error = (float)mean3.getError();
  Gamma3  = (float)width3.getVal();
  Gamma3Error = (float)width3.getError();  

  Nsig4  = (int)sig4Yield.getVal();
  Nsig4Error = (int)sig4Yield.getError();
  m04  = (float)mean4.getVal();
  m04Error = (float)mean4.getError();
  Gamma4  = (float)width4.getVal();
  Gamma4Error = (float)width4.getError();    

  Nbkg  = (int)BkgYield.getVal();
  NbkgError = (int)BkgYield.getError();
  for(Int_t i=0;i<BkgPolNpowers;i++){
    polpar[i]=coef[i]->getVal();
    polparError[i]=coef[i]->getError();
  }

  //save the parameters to a file 
  ofstream ParameterOutput;
  ParameterOutput.open((const char*)(_OutputDir+"/FitPars_"+(long)fileid+".txt"));   
  ParameterOutput<<setprecision(5)<<setw(10)<<setfill(' ')<<chisquare<<" "<<setprecision(5)<<setw(10)<<setfill(' ')<<ndof<<endl;
  ParameterOutput<<setprecision(5)<<setw(10)<<setfill(' ')<<fitmin<<" "<<setprecision(5)<<setw(10)<<setfill(' ')<<fitmax<<" "<<setprecision(5)<<setw(10)<<setfill(' ')<<nfitbins<<endl;
  ParameterOutput<<setprecision(5)<<setw(10)<<setfill(' ')<<Nsig1<<" "<<setprecision(5)<<setw(10)<<setfill(' ')<<Nsig1Error<<" "<<setprecision(5)<<setw(10)<<setfill(' ')<<Nsig1Fix<<endl;
  ParameterOutput<<setprecision(5)<<setw(10)<<setfill(' ')<<m01<<" "<<setprecision(5)<<setw(10)<<setfill(' ')<<m01Error<<" "<<setprecision(5)<<setw(10)<<setfill(' ')<<m01Fix<<endl;
  ParameterOutput<<setprecision(5)<<setw(10)<<setfill(' ')<<Gamma1<<" "<<setprecision(5)<<setw(10)<<setfill(' ')<<Gamma1Error<<" "<<setprecision(5)<<setw(10)<<setfill(' ')<<Gamma1Fix<<endl; 
  ParameterOutput<<setprecision(5)<<setw(10)<<setfill(' ')<<Nsig2<<" "<<setprecision(5)<<setw(10)<<setfill(' ')<<Nsig2Error<<" "<<setprecision(5)<<setw(10)<<setfill(' ')<<Nsig2Fix<<endl;
  ParameterOutput<<setprecision(5)<<setw(10)<<setfill(' ')<<m02<<" "<<setprecision(5)<<setw(10)<<setfill(' ')<<m02Error<<" "<<setprecision(5)<<setw(10)<<setfill(' ')<<m02Fix<<endl;
  ParameterOutput<<setprecision(5)<<setw(10)<<setfill(' ')<<Gamma2<<" "<<setprecision(5)<<setw(10)<<setfill(' ')<<Gamma2Error<<" "<<setprecision(5)<<setw(10)<<setfill(' ')<<Gamma2Fix<<endl;
  ParameterOutput<<setprecision(5)<<setw(10)<<setfill(' ')<<Nsig3<<" "<<setprecision(5)<<setw(10)<<setfill(' ')<<Nsig3Error<<" "<<setprecision(5)<<setw(10)<<setfill(' ')<<Nsig3Fix<<endl;
  ParameterOutput<<setprecision(5)<<setw(10)<<setfill(' ')<<m03<<" "<<setprecision(5)<<setw(10)<<setfill(' ')<<m03Error<<" "<<setprecision(5)<<setw(10)<<setfill(' ')<<m03Fix<<endl;
  ParameterOutput<<setprecision(5)<<setw(10)<<setfill(' ')<<Gamma3<<" "<<setprecision(5)<<setw(10)<<setfill(' ')<<Gamma3Error<<" "<<setprecision(5)<<setw(10)<<setfill(' ')<<Gamma3Fix<<endl;
  ParameterOutput<<setprecision(5)<<setw(10)<<setfill(' ')<<Nsig4<<" "<<setprecision(5)<<setw(10)<<setfill(' ')<<Nsig4Error<<" "<<setprecision(5)<<setw(10)<<setfill(' ')<<Nsig4Fix<<endl;
  ParameterOutput<<setprecision(5)<<setw(10)<<setfill(' ')<<m04<<" "<<setprecision(5)<<setw(10)<<setfill(' ')<<m04Error<<" "<<setprecision(5)<<setw(10)<<setfill(' ')<<m04Fix<<endl;
  ParameterOutput<<setprecision(5)<<setw(10)<<setfill(' ')<<Gamma4<<" "<<setprecision(5)<<setw(10)<<setfill(' ')<<Gamma4Error<<" "<<setprecision(5)<<setw(10)<<setfill(' ')<<Gamma4Fix<<endl;
  ParameterOutput<<setprecision(5)<<setw(10)<<setfill(' ')<<Nbkg<<" "<<setprecision(5)<<setw(10)<<setfill(' ')<<NbkgError<<" "<<setprecision(5)<<setw(10)<<setfill(' ')<<NbkgFix<<endl;
  for(Int_t i=0;i<BkgPolNpowers;i++){
    ParameterOutput<<setprecision(5)<<setw(10)<<setfill(' ')<<polpar[i]<<" "<<setprecision(5)<<setw(10)<<setfill(' ')<<polparError[i]<<" "<<setprecision(5)<<setw(10)<<setfill(' ')<<polparFix[i]<<endl;
  }
   
  ParameterOutput.close();
  ParameterOutput.clear();

  
  //----------------------------
  //print the results
  //----------------------------
  if(fit) res->Print();

  //----------------------------
  //print results into file
  //----------------------------
  text.SetTextSize(.03);
  text.SetTextColor(1);

  Canvas.Clear();  
  sprintf(textlabel,"RESULTS:");
  text.DrawTextNDC(.05,.9,textlabel);

  text.SetTextColor(2);
  sprintf(textlabel,"Signal1 Yield= %d +- %d  Mean= %.2f +- %.2f MeV  Width= %.2f +- %.2f MeV",
	  Nsig1,Nsig1Error,m01*1000,m01Error*1000,Gamma1*1000,Gamma1Error*1000);
  text.DrawTextNDC(.07,.8,TString(textlabel)+" shape="+shape1);
  sprintf(textlabel,"Signal2 Yield= %d +- %d  Mean= %.2f +- %.2f MeV  Width= %.2f +- %.2f MeV",
	  Nsig2,Nsig2Error,m02*1000,m02Error*1000,Gamma2*1000,Gamma2Error*1000);
  text.DrawTextNDC(.07,.75,TString(textlabel)+" shape="+shape2);
  sprintf(textlabel,"Signal3 Yield= %d +- %d  Mean= %.2f +- %.2f MeV  Width= %.2f +- %.2f MeV",
	  Nsig3,Nsig3Error,m03*1000,m03Error*1000,Gamma3*1000,Gamma3Error*1000);
  text.DrawTextNDC(.07,.7,TString(textlabel));
  sprintf(textlabel,"Signal4 Yield= %d +- %d  Mean= %.2f +- %.2f MeV  Width= %.2f +- %.2f MeV",
	  Nsig4,Nsig4Error,m04*1000,m04Error*1000,Gamma4*1000,Gamma4Error*1000);
  text.DrawTextNDC(.07,.65,TString(textlabel));

  if(CorrRes){
    text.DrawTextNDC(.07,.6,"Resolution was corrected");
    if(CorrEff)text.DrawTextNDC(.3,.7,"Efficiency was corrected");
  }
  if(applyPhaseFctr)text.DrawTextNDC(.6,.6,"PhaseFactor applied");
     
  sprintf(textlabel,"Background Yield= %d +- %d ",Nbkg,NbkgError);
  text.DrawTextNDC(.07,.5,textlabel);
  Float_t textstep=.03;

  Int_t nfl=0;
  for(Int_t i=0;i<BkgPolNpowers;i++){
    if(!polparFix[i]){
      sprintf(textlabel,TString("coef")+(long)i+"= %.2e +- %.2e",polpar[i],polparError[i]);
      text.DrawTextNDC(.07+.5*(nfl%2),.5-(nfl/2+1)*textstep,textlabel);
      nfl++;
    }
  }
    

  sprintf(textlabel,"Chisqure/ndof = %.1f/%d ",chisquare,ndof);
  text.DrawTextNDC(.07,.0,textlabel);
  Canvas.Print(filename);



  Canvas.Print(filename+"]");


 
  //  delete ReducedNtupleCopy;
  for(Int_t i=0;i<BkgPolNpowers;i++)
    delete coef[i];
  delete HDstarPiFit;
  delete HDstPiDMFinal;
  delete HDstPiDM;
  delete HFitDstPiDM;

  return 1;

}




Int_t DstPiAnalysis::CreateDeltaMHistogramForFit(){
  cout<<" Will create new histograms for later fitting"<<endl;
  
  ///Open the reduced Ntuple
  if(OpenReducedNtuple(1)!=1) return 0;
    
  //apply some cuts 
  TCut TotalCut=database.GetCut(1,100);

  //------------------------------
  //define the data to be fitted
  //------------------------------
  TTree* ReducedNtupleCopy=ReducedNtuple->CopyTree(TotalCut);
  ReducedNtupleCopy->SetBranchStatus("*",0);
  if(CloseReducedNtuple()!=1) return 0;


  //----------
  //Plot the D* deltaM
  //----------
  TH1F HDstar("HDstar","HDstar",database.GetDstarDeltaMRangeNbins(),database.GetDstarDeltaMRangeMin(),database.GetDstarDeltaMRangeMax()); 
  ReducedNtupleCopy->SetBranchStatus("dstardeltam",1); 
  ReducedNtupleCopy->Draw("dstardeltam>>HDstar");

  
  ////--------------Get plot for full range
  TH1F HDstarPi("HDstarPi","HDstarPi",1500,2.,3.5);
  sprintf(textlabel,"dstpim+%.5f",DstarPDGMass);
  ReducedNtupleCopy->SetBranchStatus("dstpim",1); 
  ReducedNtupleCopy->Draw(TString(textlabel)+">>HDstarPi");



  TFile File(_OutputDir+"/HistosForFit.root","recreate");
  HDstar.Write();
  HDstarPi.Write();
  File.ls();
  File.Close();

  return 1;
}

TH1F* CreateRebinnedDstPiHisto(TH1* oldhisto,Int_t nbins,Float_t min,Float_t max){

  Float_t newbinsize=(max-min)/nbins;
  if( fabs(1000*newbinsize - floor(1000*newbinsize + .5)) >.01
      ||fabs(1000*min - floor(1000*min + .5)) >.01 
      ||fabs(1000*max - floor(1000*max + .5)) >.01  
      ||min>max){
    cout<<"Rebinning of DstPi histo is not good, must use binsize and boundaries of multiples of 1MeV"<<endl;
    cout<<"                  "<<nbins<<" "<<min<<" "<<max<<endl;
    cout<<floor(1000*newbinsize + .5)<<" "<<floor(1000*min + .5)<<" "<<floor(1000*max + .5)<<endl;
    return NULL;
  }


  Int_t nnewrangebins=(int)floor(1000*(max-min)  +.5);
  TH1F* newhisto=new TH1F(TString(oldhisto->GetName())+"rebin",TString(oldhisto->GetName())+"rebin",nnewrangebins,min,max);


  Int_t bn=1;
  for(Int_t b=1;b<=oldhisto->GetNbinsX();b++){
    //cout<<nnewrangebins<<" "<<b<<" "<<bn<<" "<<oldhisto->GetBinCenter(b)<<" "<<newhisto->GetBinCenter(bn)<<" "<<fabs(oldhisto->GetBinCenter(b)-newhisto->GetBinCenter(bn))<<endl;
    if(fabs(oldhisto->GetBinCenter(b)-newhisto->GetBinCenter(bn))<oldhisto->GetBinWidth(1)/100 && bn<=nnewrangebins){
      newhisto->SetBinContent(bn,oldhisto->GetBinContent(b));
      bn++;
    }    
  }
  if((bn-1)!=nnewrangebins){
    cout<<"There was a problem in rebinning, bin numbers dont match."<<endl;
    return NULL;
  }


  Int_t group=nnewrangebins/nbins;
  TH1F* rebinhisto=(TH1F*)newhisto->Rebin(group,TString(oldhisto->GetName())+"rebinned"); 
  
  if(rebinhisto)
    cout<<"Rebinned histo created successfully."<<endl;

  delete newhisto;
  return rebinhisto;
}
