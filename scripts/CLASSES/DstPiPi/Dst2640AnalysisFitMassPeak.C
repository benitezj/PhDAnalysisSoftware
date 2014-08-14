

//////needed for RooFit
#include <TLine.h>
#include <TGraphErrors.h>
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
//#include "RooCmdArg.h"
#include "RooArgList.h"
#include "RooDataHist.h"
#include "RooAbsReal.h"
#include "RooNumConvPdf.h"
#include "RooAbsData.h"
#include "RooGlobalFunc.h"
using namespace RooFit ;
////////////////////////
#include <fstream>
using std::streambuf;
using std::ostream;


Int_t Dst2640Analysis::FitDst2640DM(Int_t FitType,TString DMResParsDir,Float_t helcut,Float_t pcut){
  cout<<"Going to fit the Dst2640 deltaM distribution"<<endl;
  if(TruthMatch){cout<<" This operation only for DATA."<<endl;return 0;}
  cout<<DMResParsDir<<endl;
  
  if(!OpenReducedNtuple()){
    cout<<"Failed to open the file"<<endl;
    return 0;
  }  


  //--------------------------------------------------------
  ///parameters to be determined from a fit
  //--------------------------------------------------------
  Float_t binsize=.016;
  Float_t plotmin=.28;
  Float_t plotmax=.68;
  Float_t fitmin=.28;
  Float_t fitmax=.68;

  Float_t ressigma=.0017;
  Float_t resmean=0;

  Int_t Nsig1 =0;
  Int_t Nsig1Error = 0;
  Bool_t Nsig1Fix=0;
  Float_t m01  = 0;
  Float_t m01Error = 0;
  Bool_t m01Fix=1;
  Float_t Gamma1  = 0;
  Float_t Gamma1Error =0;
  Bool_t Gamma1Fix=1;

  Int_t Nsig2 =0;
  Int_t Nsig2Error = 0;
  Bool_t Nsig2Fix=0;
  Float_t m02  = 0;
  Float_t m02Error = 0;
  Bool_t m02Fix=1;
  Float_t Gamma2  = 0;
  Float_t Gamma2Error =0;
  Bool_t Gamma2Fix=1;

  Int_t Nbkg = 0;
  Int_t NbkgError = 0;
  Bool_t NbkgFix=0;
  Int_t Npowers=6;//order of the polynomial
  Double_t polpar[10];
  Float_t polparError[10];
  Bool_t polparFix[10];
  for(Int_t i=0;i<=Npowers;i++){   
    polpar[i]=0;
    polparError[i]=0;
    polparFix[i]=0;
  }
  

  //---------------------------
  //read the parameters from the file
  //----------------------------    
  ifstream ParameterInput;
  ParameterInput.open((const char*)(_OutputDir+"/InitFitPars"+(long)_WhichCuts+".txt"));   
  ParameterInput>>plotmin>>plotmax>>binsize;
  ParameterInput>>fitmin>>fitmax;
  ParameterInput>>ressigma>>resmean;

  ParameterInput>>Nsig1>>Nsig1Fix;
  ParameterInput>>m01>>m01Fix;
  ParameterInput>>Gamma1>>Gamma1Fix;
 
  ParameterInput>>Nsig2>>Nsig2Fix;
  ParameterInput>>m02>>m02Fix;
  ParameterInput>>Gamma2>>Gamma2Fix;


  ParameterInput>>Nbkg>>NbkgFix;
  for(Int_t i=0;i<=Npowers;i++)   
    ParameterInput>>polpar[i]>>polparFix[i];
  ParameterInput.close();
  ParameterInput.clear();



  //------------------------------
  ///define the data to be fitted
  //------------------------------
  sprintf(textlabel,"dstarcostheta<%.2f",helcut);
  TCut heliCut=textlabel;
  sprintf(textlabel,"dst2640pstar>%.2f",pcut); 
  TCut pstarCut=textlabel;
  sprintf(textlabel,"dst2640logvtxprob>-2."); 
  TCut probCut=textlabel;
  sprintf(textlabel,"d0mode==1"); 
  TCut modeCut=textlabel;
  TTree* ReducedNtupleCopy=ReducedNtuple->CopyTree(pstarCut+heliCut+probCut+modeCut);
  //TTree* ReducedNtupleCopy=ReducedNtuple->CopyTree(database.GetCut(1,100)+pstarCut+heliCut);
  ReducedNtupleCopy->SetBranchStatus("*",0);
  Close();

  //-------------------
  //create a canvas
  //-------------------
  TCanvas Canvas("Canvas","Canvas");
  TString fname;
  fname=_OutputDir+"/"+"Dst2640DMRooFit"+(long)_WhichCuts+".ps";
  Canvas.Print(fname+"[");


  //--------------------
  //Fit variables and Range
  //-------------------
  //Float_t fitmin=database.GetDst2640DeltaMMin();
  //Float_t fitmax=database.GetDst2640DeltaMMax();
  //Int_t nfitbins=databae.GetDst2640DeltaMNbins();
  Int_t nfitbins=(int)((fitmax-fitmin)/binsize +.5);
    
  RooRealVar roodst2640deltam("dst2640deltam","DeltaM",fitmin,fitmax);
  roodst2640deltam.setBins(nfitbins);
  ReducedNtupleCopy->SetBranchStatus("*",0); 
  ReducedNtupleCopy->SetBranchStatus("dst2640deltam",1); 
  roodst2640deltam.Print();

  RooArgList vars;
  vars.add(roodst2640deltam);

  //3-Body phase-space factor over M this is from a fit to a plot
  TString phasespace="((-4.076+12.28*(dst2640deltam+2.01)-8.392*pow((dst2640deltam+2.01),2)+1.662*pow((dst2640deltam+2.01),3))/(dst2640deltam+2.01))*";


  //-------------------------
  //The Resolution function 
  //-------------------------
//   Float_t resolutionpar[7];
//   if(!database.GetParameters(DMResParsDir+"/"+"Dst2640DMResPars"+(long)_WhichCuts+".txt",resolutionpar,7))
//   return 0;
//   RooRealVar N1("N1","N1",resolutionpar[0]);
//   RooRealVar Mean("Mean","Mean",resolutionpar[1]);  
//   RooRealVar S1("S1","S1",resolutionpar[2]);
//   RooRealVar N2("N2","N2",resolutionpar[3]);
//   RooRealVar S2("S2","S2",resolutionpar[4]);
//   RooRealVar N3("N3","N3",resolutionpar[5]);
//   RooRealVar S3("S3","S3",resolutionpar[6]);
//  RooGenericPdf respdf("respdf","N1*exp(-.5*(dst2640deltam-Mean)**2/S1**2)+N2*exp(-.5*(dst2640deltam-Mean)**2/S2**2)+N3*exp(-.5*(dst2640deltam-Mean)**2/S3**2)",RooArgSet(roodst2640deltam,N1,Mean,S1,N2,S2,N3,S3));
  RooRealVar ResMean("ResMean","ResMean",resmean);  
  RooRealVar ResSigma("ResSigma","ResSigma",ressigma);//FWHM=4MeV
  RooGenericPdf respdf("respdf","exp(-.5*(dst2640deltam-ResMean)**2/ResSigma**2)",RooArgSet(roodst2640deltam,ResMean,ResSigma));

  //-----------------
  //the signal pdfs
  //-----------------
  RooRealVar width1("width1","width1",.032,.01,1);
  width1.setVal(Gamma1);   
  width1.setConstant(Gamma1Fix); 
  RooRealVar mean1("mean1","mean1",.413,plotmin,plotmax);
  mean1.setVal(m01);
  mean1.setConstant(m01Fix);   
  RooGenericPdf* signal1pdf=new RooGenericPdf("signal1pdf",phasespace+"dst2640deltam/(pow(dst2640deltam*dst2640deltam-mean1*mean1,2)+pow(mean1*width1,2))",RooArgSet(roodst2640deltam,mean1,width1)); 
  RooGenericPdf* signal1pdfNoPS=new RooGenericPdf("signal1pdfNoPS","dst2640deltam/(pow(dst2640deltam*dst2640deltam-mean1*mean1,2)+pow(mean1*width1,2))",RooArgSet(roodst2640deltam,mean1,width1)); 


  RooGenericPdf* signal2pdf=NULL;
  RooRealVar width2("width2","width2",.046,.01,1);
  width2.setVal(Gamma2);   
  width2.setConstant(Gamma2Fix); 
  RooRealVar mean2("mean2","mean2",.450,fitmin,fitmax);
  mean2.setVal(m02);
  mean2.setConstant(m02Fix); 
  signal2pdf=new RooGenericPdf("signal2pdf",phasespace+"dst2640deltam/(pow(dst2640deltam*dst2640deltam-mean2*mean2,2)+pow(mean2*width2,2))",RooArgSet(roodst2640deltam,mean2,width2));


  //----------------------------------------------------
  // make convolution of signal with resolution function
  //----------------------------------------------------
  RooRealVar windowMean("windowMean","windowMean",0);
  RooRealVar Sig("Sig","Sig",ResSigma.getVal());

  RooNumConvPdf signal1pdfsmeared("signal1pdfsmeared","signal1pdfsmeared",roodst2640deltam,*signal1pdf,respdf);
  signal1pdfsmeared.setConvolutionWindow(windowMean,Sig,5);

  RooNumConvPdf signal2pdfsmeared("signal2pdfsmeared","signal2pdfsmeared",roodst2640deltam,*signal2pdf,respdf);
  signal2pdfsmeared.setConvolutionWindow(windowMean,Sig,5);
         

  //------------------------------------------------
  //plot the smeared signal
  //------------------------------------------------
  RooPlot* dst2640deltamframe=roodst2640deltam.frame(plotmin,plotmax);
  Canvas.Clear();  
  signal1pdfNoPS->plotOn(dst2640deltamframe,LineColor(2));
  signal1pdf->plotOn(dst2640deltamframe,LineColor(3));
  signal1pdfsmeared.plotOn(dst2640deltamframe,LineStyle(kDashed));
  dst2640deltamframe->Draw();
  Canvas.Print(fname);  
  


  //---------------------------
  //The Background
  //---------------------------
  RooRealVar coef0("coef0","coef0",1,-2e5,1e5);//,1,-2e10,1e10
  coef0.setVal(polpar[0]);
  coef0.setConstant(polparFix[0]);
  RooRealVar coef1("coef1","coef1",1,-2e5,1e5);
  coef1.setVal(polpar[1]);
  coef1.setConstant(polparFix[1]);
  RooRealVar coef2("coef2","coef2",1,-2e5,1e5);
  coef2.setVal(polpar[2]);
  coef2.setConstant(polparFix[2]);
  RooRealVar coef3("coef3","coef3",1,-2e5,1e5);
  coef3.setVal(polpar[3]);
  coef3.setConstant(polparFix[3]);
  RooRealVar coef4("coef4","coef4",1,0,1e6);//norm
  coef4.setVal(polpar[4]);
  coef4.setConstant(polparFix[4]);
  RooRealVar coef5("coef5","coef5",(fitmax+fitmin)/2,fitmin,fitmax);//mean
  coef5.setVal(polpar[5]);
  coef5.setConstant(polparFix[5]);
  RooRealVar coef6("coef6","coef6",1,0,10);//sigma
  coef6.setVal(polpar[6]);
  coef6.setConstant(polparFix[6]);
  //  RooGenericPdf* bkg=new RooGenericPdf("mybkg","(coef0+coef1*dst2640deltam+coef2*pow(dst2640deltam,2)+coef3*pow(dst2640deltam,3)+coef4*pow(dst2640deltam,4)+coef5*pow(dst2640deltam,5)+coef6*pow(dst2640deltam,6))",RooArgList(roodst2640deltam,coef0,coef1,coef2,coef3,coef4,coef5,coef6)); 
  RooGenericPdf* bkg=new RooGenericPdf("mybkg","((dst2640deltam-.27912)**1.5)*(coef4*exp(-.5*(dst2640deltam-coef5)**2/coef6**2)+coef0+coef1*dst2640deltam+coef2*pow(dst2640deltam,2)+coef3*pow(dst2640deltam,3))",RooArgList(roodst2640deltam,coef0,coef1,coef2,coef3,coef4,coef5,coef6));
  //


  //--------------------------------------------------------
  //construct the total  pdf: signal plus background
  //--------------------------------------------------------  
  RooRealVar sig1PiNb("sig1PiNb","sig1PiNb",1,0,1e4);  //Signal Yield
  sig1PiNb.setVal(Nsig1);
  sig1PiNb.setConstant(Nsig1Fix);          

  RooRealVar sig2PiNb("sig2PiNb","sig2PiNb",1,0,1e4); 
  sig2PiNb.setVal(Nsig2);
  sig2PiNb.setConstant(Nsig2Fix);       


  RooRealVar BkgNb("BkgNb","BkgNb",1,0,1e7);
  BkgNb.setVal(Nbkg);
  BkgNb.setConstant(NbkgFix);

//   RooRealVar totalsigPiNb("totalsigPiNb","totalsigPiNb",1,0,1e10); 
//   totalsigPiNb.setVal(Nsig3);
//   totalsigPiNb.setConstant(Nsig3Fix);     

  //Make the total pdf.
  RooAddPdf* totalpdf=new RooAddPdf("SignalPlusBkgPdf","SignalPlusBkgPdf",RooArgList(signal1pdfsmeared,signal2pdfsmeared,*bkg),RooArgList(sig1PiNb,sig2PiNb,BkgNb));

 
  //--------------------------------------------------------  
  //--------------------------------------------------------
  RooDataSet* unbinneddata=new RooDataSet("unbinneddata","unbinneddata",ReducedNtupleCopy,vars);

  //--------------------------------------------------------
  //data for the binnned fit:
  //--------------------------------------------------------
  RooDataHist* binneddata=new RooDataHist("binneddata","binneddata",RooArgList(roodst2640deltam),*unbinneddata);
 

  //--------------------------------------------------------
  //the results from the Fit (if it is done)
  //--------------------------------------------------------
  RooFitResult* res=NULL;
  RooArgList* floatedParameters=NULL;
  


  //-----------------------------------------
  //do the binned fit before the unbinned fit
  //-----------------------------------------
  Int_t maxfittype=0;
  if(FitType==1)maxfittype=1;//binned  
  if(FitType==2)maxfittype=2;//unbinned
  for(Int_t whichfittype=1;whichfittype<=maxfittype;whichfittype++){


    //---------------------
    //Make the fit 
    //---------------------
    sprintf(textlabel,"%.3f:%.3f",fitmin,fitmax);
    if(whichfittype==1){
      cout<<"Going to do the binned fit."<<endl;    
      res=totalpdf->fitTo(*binneddata,"mhert");//,"c",textlabel
      if(res==NULL){
	cout<<"No fit results returned."<<endl;
	return 0;
      }
    }
    if(whichfittype==2){
      cout<<"Going to do the unbinned fit."<<endl;
      res=totalpdf->fitTo(*unbinneddata,"mhert");
      if(res==NULL){
	cout<<"No fit results returned."<<endl;
	return 0;
      }
    }

    //----------------------------------------
    //the following commands must be called otherwise pdf may not be well normalized in residuals. ??????????
    //----------------------------------------
    RooAbsReal* integral;
    roodst2640deltam.setRange("binrange",fitmin,fitmax);
    integral= totalpdf->createIntegral(roodst2640deltam,"binrange");
    cout<<"check pdf norm: "<<integral->getVal()<<" "<<totalpdf->selfNormalized()<<" "<<totalpdf->getNorm(roodst2640deltam)<<endl;
  

    //----------------------------------
    //extract the floated parameters
    //----------------------------------
    floatedParameters = (RooArgList*)&res->floatParsFinal();
 
    Nsig1  = (int)sig1PiNb.getVal();
    Nsig1Error = (int)sig1PiNb.getError();
    m01  = (float)mean1.getVal();
    m01Error = (float)mean1.getError();
    Gamma1  = (float)width1.getVal();
    Gamma1Error = (float)width1.getError();
  
    Nsig2  = (int)sig2PiNb.getVal();
    Nsig2Error = (int)sig2PiNb.getError();
    m02  = (float)mean2.getVal();
    m02Error = (float)mean2.getError();
    Gamma2  = (float)width2.getVal();
    Gamma2Error = (float)width2.getError();
    

    Nbkg  = (int)BkgNb.getVal();
    NbkgError = (int)BkgNb.getError();
    polpar[0]=coef0.getVal();
    polparError[0]=coef0.getError();
    polpar[1]=coef1.getVal();
    polparError[1]=coef1.getError();
    polpar[2]=coef2.getVal();
    polparError[2]=coef2.getError();
    polpar[3]=coef3.getVal();
    polparError[3]=coef3.getError();
    polpar[4]=coef4.getVal();
    polparError[4]=coef4.getError();
    polpar[5]=coef5.getVal();
    polparError[5]=coef5.getError();
    polpar[6]=coef6.getVal();
    polparError[6]=coef6.getError();
  
    //----------------------------------
    //save the parameters in a text file
    //----------------------------------
    TString fitresultsname=_OutputDir+"/"+"FitPars"+(long)_WhichCuts+".txt";
    FILE* fitresults=fopen((const char*)fitresultsname,"w");
    cout<<"Fit parameters File:"<<fitresultsname<<"  has been created."<<endl;
    fprintf(fitresults,"%d %d \n%.3e %.3e \n%.3e %.3e\n",Nsig1,Nsig1Error,m01,m01Error,Gamma1,Gamma1Error);
    fprintf(fitresults,"%d %d \n%.3e %.3e \n%.3e %.3e\n",Nsig2,Nsig2Error,m02,m02Error,Gamma2,Gamma2Error);
    fprintf(fitresults,"%d %d \n%.3e %.3e \n%.3e %.3e \n%.3e %.3e \n%.3e %.3e \n%.3e %.3e \n%.3e %.3e \n%.3e %.3e \n"
	    ,Nbkg,NbkgError
	    ,polpar[0],polparError[0]
	    ,polpar[1],polparError[1]
	    ,polpar[2],polparError[2]
	    ,polpar[3],polparError[3]
	    ,polpar[4],polparError[4]
	    ,polpar[5],polparError[5]
	    ,polpar[6],polparError[6]
	    );
    fclose(fitresults);
    
  }
  if(floatedParameters==NULL)return 0;
  

  
  ////////////////////////////////////Make Some plots  
  //----------------------------
  //create a histogram of the data
  //----------------------------
  TH1* HDst2640DM=binneddata->createHistogram("HDst2640DM",roodst2640deltam);
  if(HDst2640DM==NULL){
    cout<<"No HDst2640DM  returned."<<endl;
    return 0;
  }

  //----------------------------
  //the Fit plot
  //----------------------------
  Canvas.Clear();  
  dst2640deltamframe=roodst2640deltam.frame(plotmin,plotmax);
  unbinneddata->plotOn(dst2640deltamframe);
  totalpdf->plotOn(dst2640deltamframe);
  totalpdf->plotOn(dst2640deltamframe,Components(*bkg),LineStyle(kDashed),LineColor(1));
  totalpdf->plotOn(dst2640deltamframe,Components(signal1pdfsmeared),LineStyle(kSolid),LineColor(2));
  totalpdf->plotOn(dst2640deltamframe,Components(signal2pdfsmeared),LineStyle(kSolid),LineColor(3));
  dst2640deltamframe->Draw();
  Canvas.Print(fname);

  //----------------------------
  //now make the residual plot
  //----------------------------
  JResiduals roofitresiduals(HDst2640DM,totalpdf,&roodst2640deltam);
  Float_t chisquare=roofitresiduals.GetChisquare(); 
  Int_t ndof=HDst2640DM->GetXaxis()->GetNbins() - floatedParameters->getSize();

  TH1F* RooResidualHisto=roofitresiduals.GetResidualHisto();
  Canvas.Clear();
  RooResidualHisto->Draw("pe");
  cutline.SetLineColor(1);
  cutline.DrawLine(fitmin,0,fitmax,0);
  Canvas.Print(fname); 
    

  //----------------------------
  //print the results
  //----------------------------
  res->Print();

  //----------------------------
  //print results into file
  //----------------------------
  text.SetTextSize(.03);
  text.SetTextColor(1);

  Canvas.Clear();  
  sprintf(textlabel,"RESULTS:");
  text.DrawTextNDC(.05,.9,textlabel);

  text.SetTextColor(2);
  sprintf(textlabel,"Signal1 Yield= %d +- %d  Mean= %.2f +- %.2f MeV  Width= %.2f +- %.2f MeV",Nsig1,Nsig1Error,m01*1000,m01Error*1000,Gamma1*1000,Gamma1Error*1000);
  text.DrawTextNDC(.07,.8,textlabel);
  sprintf(textlabel,"Signal2 Yield= %d +- %d  Mean= %.2f +- %.2f MeV  Width= %.2f +- %.2f MeV",Nsig2,Nsig2Error,m02*1000,m02Error*1000,Gamma2*1000,Gamma2Error*1000);
  text.DrawTextNDC(.07,.75,textlabel);
   
  sprintf(textlabel,"Background Yield= %d +- %d ",Nbkg,NbkgError);
  text.DrawTextNDC(.07,.53,textlabel);
  sprintf(textlabel,"Coef0= %.2e +- %.2e",polpar[0],polparError[0]);
  text.DrawTextNDC(.07,.50,textlabel);
  sprintf(textlabel,"Coef1= %.2e +- %.2e",polpar[1],polparError[1]);
  text.DrawTextNDC(.07,.47,textlabel);
  sprintf(textlabel,"Coef2= %.2e +- %.2e",polpar[2],polparError[2]);
  text.DrawTextNDC(.07,.44,textlabel);
  sprintf(textlabel,"Coef3= %.2e +- %.2e",polpar[3],polparError[3]);
  text.DrawTextNDC(.07,.41,textlabel);
  sprintf(textlabel,"Coef4= %.2e +- %.2e",polpar[4],polparError[4]);
  text.DrawTextNDC(.07,.38,textlabel);
  sprintf(textlabel,"Coef5= %.2e +- %.2e",polpar[5],polparError[5]);
  text.DrawTextNDC(.07,.35,textlabel);
  sprintf(textlabel,"Coef6= %.2e +- %.2e",polpar[6],polparError[6]);
  text.DrawTextNDC(.07,.32,textlabel);
  sprintf(textlabel,"Chisqure/ndof = %.1f/%d ",chisquare,ndof);
  text.DrawTextNDC(.07,.0,textlabel);
  Canvas.Print(fname);

  
  Canvas.Print(fname+"]");  
  ////////done saving plots
 

  delete dst2640deltamframe;
  delete HDst2640DM;
  delete signal1pdf;
  delete signal2pdf;
  delete bkg;
  delete res;
  delete totalpdf;
  delete unbinneddata;
  delete binneddata;
  delete ReducedNtupleCopy;


  return 1;
}

