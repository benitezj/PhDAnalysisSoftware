
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
#include "RooArgList.h"
#include "RooDataHist.h"
#include "RooAbsReal.h"
#include "RooNumConvPdf.h"
#include "RooAbsData.h"

#include "JResiduals.h"



Int_t Ds1Analysis::FitDs1DM(Int_t FitType,TString DMResParsDir,TString PstarComparisonFile){
  cout<<"Going to fit the Ds1 deltaM distribution"<<endl;
   
  if(!OpenReducedNtuple()){
    cout<<"Failed to open the file"<<endl;
    return 0;
  }


  //--------------------------------
  ///define the data to be fitted
  //------------------------------
  TTree* ReducedNtupleCopy=ReducedNtuple->CopyTree(database.GetCut(1,100));
  //ReducedNtupleCopy->SetBranchStatus("*",0);
  Close();

  //--------------------
  //create a canvas
  //-------------------
  TCanvas Canvas("Canvas","Canvas");
  TString fname;
  fname=_OutputDir+"/"+"Ds1DMRooFit"+(long)_WhichCuts+".ps";
  Canvas.Print(fname+"[");


  //--------------------
  //Fit variables and Range
  //-------------------
  Float_t fitmin=database.GetDs1pDeltaMMin();
  Float_t fitmax=database.GetDs1pDeltaMMax();
  Int_t nfitbins=database.GetDs1pDeltaMNbins();

  RooRealVar roods1pdeltam2("ds1pdeltam2","DeltaM",fitmin,fitmax);
  roods1pdeltam2.setBins(nfitbins);
  ReducedNtupleCopy->SetBranchStatus("ds1pdeltam2",1); 
  roods1pdeltam2.Print();

  RooArgList vars;
  vars.add(roods1pdeltam2);

  //----------------------------
  //the signal pdf
  //----------------------------
  RooGenericPdf* signalpdf;
  RooRealVar width("width","width",.001,.0005,.0015);
  RooRealVar mean("mean","mean",.027,.022,.032);

        
  ////////////MC generated with a nonrelativistic bw
  if(TruthMatch)
    signalpdf=new RooGenericPdf("signalpdf","width/((ds1pdeltam2-mean)**2+.25*(width**2))",RooArgSet(roods1pdeltam2,mean,width));
  ///DATA should follow relativistic bw
  if(!TruthMatch)
    signalpdf=new RooGenericPdf("signalpdf","ds1pdeltam2*mean*width/((ds1pdeltam2**2-mean**2)**2+(mean**2)*(width**2))",RooArgSet(roods1pdeltam2,mean,width));
  
  
  //---------------------------
  //The Background
  //---------------------------
  RooRealVar coef0("coef0","coef0",-1000,1000);
  RooRealVar coef1("coef1","coef1",-10000,10000);
  RooRealVar coef2("coef2","coef2",-100000,100000);
  RooGenericPdf* bkg=new RooGenericPdf("mybkg","coef0+coef1*ds1pdeltam2+coef2*ds1pdeltam2**2",RooArgList(roods1pdeltam2,coef0,coef1,coef2)); 

  //-------------------------
  //The Resolution function
  //-------------------------
  Float_t resolutionpar[7];
  if(!database.GetParameters(DMResParsDir+"/"+"Ds1pDMResPars"+(long)_WhichCuts+".txt",resolutionpar,7))
    return 0;
  RooRealVar N1("N1","N1",resolutionpar[0]);
  RooRealVar Mean("Mean","Mean",resolutionpar[1]);  
  RooRealVar S1("S1","S1",resolutionpar[2]);
  RooRealVar N2("N2","N2",resolutionpar[3]);
  RooRealVar S2("S2","S2",resolutionpar[4]);
  RooRealVar N3("N3","N3",resolutionpar[5]);
  RooRealVar S3("S3","S3",resolutionpar[6]);
  RooGenericPdf respdf("respdf","N1*exp(-.5*(ds1pdeltam2-Mean)**2/S1**2)+N2*exp(-.5*(ds1pdeltam2-Mean)**2/S2**2)+N3*exp(-.5*(ds1pdeltam2-Mean)**2/S3**2)",RooArgSet(roods1pdeltam2,N1,Mean,S1,N2,S2,N3,S3));


 
  //----------------------------------------------------
  // make convolution of signal with resolution function
  //----------------------------------------------------
  RooNumConvPdf signalsmeared("signalsmeared","signalsmeared",roods1pdeltam2,*signalpdf,respdf);
  RooRealVar windowMean("windowMean","windowMean",0);
  RooRealVar Sig("Sig","Sig",.001);//resolution is about 1MeV
  signalsmeared.setConvolutionWindow(windowMean,Sig,6);
  
  //------------------------------------------------
  //plot the smeared signal together with the unsmeared signal
  //------------------------------------------------
  RooPlot* ds1pdeltam2frame=roods1pdeltam2.frame(fitmin,fitmax);
  Canvas.Clear();  
  signalpdf->plotOn(ds1pdeltam2frame);
  signalsmeared.plotOn(ds1pdeltam2frame);
  ds1pdeltam2frame->Draw();
  Canvas.Print(fname);  

  //--------------------------------------------------------
  //construct the total  pdf: signal plus background
  //--------------------------------------------------------  
  RooRealVar BkgNb("BkgNb","BkgNb",0,0,10000);
  RooRealVar sigPiNb("sigPiNb","sigPiNb",0,0,1000000);  //Signal Yield
  RooAddPdf* totalpdf=new RooAddPdf("SignalPlusBkgPdf","SignalPlusBkgPdf",RooArgList(signalsmeared,*bkg),RooArgList(sigPiNb,BkgNb));

  

  //--------------------------------------------------------  
  //data from the ntuple
  //--------------------------------------------------------
  RooDataSet* unbinneddata=new RooDataSet("unbinneddata","unbinneddata",ReducedNtupleCopy,vars);

  //--------------------------------------------------------
  //data for the binnned fit:
  //--------------------------------------------------------
  RooDataHist* binneddata=new RooDataHist("binneddata","binneddata",RooArgList(roods1pdeltam2),*unbinneddata);
 
    
  //--------------------------------------------------------
  ///parameters to be determined from a fit
  //--------------------------------------------------------
  Int_t Nsig =0;
  Int_t NsigError = 0;
  Float_t m0  = 0;
  Float_t m0Error = 0;
  Float_t Gamma  = 0;
  Float_t GammaError =0;
  Int_t Nbkg  = 0;
  Int_t NbkgError = 0;
  Int_t Npowers=3;
  Float_t polpar[10];
  for(Int_t i=0;i<Npowers;i++){   
    polpar[i]=0;
  }
  
  //--------------------------------------------------------
  //the results from the Fit (if it is done)
  //--------------------------------------------------------
  RooFitResult* res=NULL;
  Float_t FitParameter[11];
  RooArgList* floatedParameters=NULL;
  

  //-----------------------------------------
  //do the binned fit before the unbinned fit
  //-----------------------------------------
  Int_t maxfittype=0;
  if(FitType==0)maxfittype=0;//no fit just read the parameters already stored  
  if(FitType==1)maxfittype=1;//binned  
  if(FitType==2)maxfittype=2;//unbinned

  for(Int_t whichfittype=0;whichfittype<=maxfittype;whichfittype++){

    //---------------------------
    //read the parameters from the file
    //-----------------------------
    if(!database.GetParameters(_OutputDir+"/FitPars"+(long)_WhichCuts+".txt",FitParameter,11))
      return 0;
        
    Nsig  = (int)FitParameter[0];
    NsigError = (int)FitParameter[1];
    m0  =(float) FitParameter[2];
    m0Error =(float) FitParameter[3];
    Gamma  = (float)FitParameter[4];
    GammaError = (float)FitParameter[5];
    Nbkg  = (int)FitParameter[6];
    NbkgError =(int) FitParameter[7];
    for(Int_t i=0;i<Npowers;i++){   
      polpar[i]=(float)FitParameter[8+i];
    }
 
    //---------------------
    //put parameters into the RooVariables
    //----------------------
    sigPiNb.setVal(Nsig);
    sigPiNb.setError(NsigError);
    mean.setVal(m0);
    mean.setError(m0Error);
    width.setVal(Gamma);
    width.setError(GammaError);
    BkgNb.setVal(Nbkg);
    BkgNb.setError(NbkgError);
    coef0.setVal(polpar[0]);
    coef1.setVal(polpar[1]);
    coef2.setVal(polpar[2]);
    
    //---------------------
    //make variables constant or not 
    //----------------------
    width.setConstant(kFALSE);  
    mean.setConstant(kFALSE);
    sigPiNb.setConstant(kFALSE);
    if(TruthMatch){
      BkgNb.setConstant(kTRUE);
      coef0.setConstant(kTRUE);
      coef1.setConstant(kTRUE);
      coef2.setConstant(kTRUE);
    }
    if(!TruthMatch){
      BkgNb.setConstant(kFALSE);
      coef0.setConstant(kFALSE);
      coef1.setConstant(kFALSE);
      coef2.setConstant(kFALSE);
    }


    
    //---------------------
    //Make the fit 
    //---------------------
    if(whichfittype==1){
      cout<<"Going to do the binned fit."<<endl;
      res=totalpdf->fitTo(*binneddata,"mhert");
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

    //----------------------------
    //print the results
    //----------------------------
    if(res)res->Print();
    

    //----------------------------------------
    //the following commands must be called otherwise pdf may not be well normalized in residuals. ??????????
    //----------------------------------------
    RooAbsReal* integral;
    roods1pdeltam2.setRange("binrange",fitmin,fitmax);
    integral= totalpdf->createIntegral(roods1pdeltam2,"binrange");
    cout<<"check pdf norm: "<<integral->getVal()<<" "<<totalpdf->selfNormalized()<<" "<<totalpdf->getNorm(roods1pdeltam2)<<endl;
  

    //----------------------------------
    //extract the floated parameters
    //----------------------------------
    if(res)floatedParameters = (RooArgList*)&res->floatParsFinal();
 
    Nsig  = (int)sigPiNb.getVal();
    NsigError = (int)sigPiNb.getError();
    m0  = (float)mean.getVal();
    m0Error = (float)mean.getError();
    Gamma  = (float)width.getVal();
    GammaError = (float)width.getError();

    Nbkg  = (int)BkgNb.getVal();
    NbkgError = (int)BkgNb.getError();
    polpar[0]=coef0.getVal();
    polpar[1]=coef1.getVal();
    polpar[2]=coef2.getVal();
  
    //------------------------------------
    //save the parameters in a text file
    //----------------------------------
    TString fitresultsname=_OutputDir+"/"+"FitPars"+(long)_WhichCuts+".txt";
    FILE* fitresults=fopen((const char*)fitresultsname,"w");
    cout<<"Fit parameters File:"<<fitresultsname<<"  has been created."<<endl;
    fprintf(fitresults,"%d %d %.3e %.3e %.3e %.3e %d %d %.3e %.3e %.3e\n"
	    ,Nsig
	    ,NsigError
	    ,m0
	    ,m0Error
	    ,Gamma
	    ,GammaError
	    ,Nbkg
	    ,NbkgError
	    ,polpar[0]
	    ,polpar[1]
	    ,polpar[2]
	    );
    fclose(fitresults);
  
  
  }
  
  //----------------------------
  //The background function 
  //----------------------------
  TF1* FPDFBkg=NULL;
  FPDFBkg=new TF1("FPDFBkg","[3]*([0]+[1]*x+[2]*x**2)",fitmin,fitmax);      
  //////////calculate normalization of bkg
  Float_t polnorm=0;
  for(Int_t i=0;i<Npowers;i++){  
    polnorm+=polpar[i]* ( pow(fitmax,(float)(i+1.)) - pow(fitmin,(float)(i+1.)) )/(i+1.);    
  }
  if(polnorm<0)cout<<"bkg norm not right:"<<endl;
  for(Int_t i=0;i<Npowers;i++){   
    FPDFBkg->SetParameter(i,polpar[i]);
  }
  if(!TruthMatch)FPDFBkg->SetParameter(3,1/polnorm);//set integral to 1
  if(TruthMatch)FPDFBkg->SetParameter(3,0);//set integral to 0
  cout<<"check Bkg norm integral= "<<FPDFBkg->Integral(fitmin,fitmax)<<endl;
  ////////////////////////////////


  
  ////////////////////////////////////Make Some plots
  
  //----------------------------
  //create a histogram of the data
  //----------------------------
  TH1* HDs1pDM=binneddata->createHistogram("HDs1pDM",roods1pdeltam2);
  if(HDs1pDM==NULL){
    cout<<"No HDs1pDM  returned."<<endl;
    return 0;
  }

  //----------------------------
  //now make the residual plot
  //----------------------------
  //  JResiduals roofitresiduals(HDs1pDM,totalpdf,&roods1pdeltam2);
  Float_t chisquare=0;//roofitresiduals.GetChisquare(); 
  Int_t nfloated=0;
  if(!TruthMatch)nfloated=3+Npowers;
  if(TruthMatch)nfloated=3;
  Int_t ndof=HDs1pDM->GetXaxis()->GetNbins() - nfloated;


  //----------------------------
  //the Fit plot
  //----------------------------
  Canvas.Clear();  
  ds1pdeltam2frame=roods1pdeltam2.frame(fitmin,fitmax);
  unbinneddata->plotOn(ds1pdeltam2frame);
  totalpdf->plotOn(ds1pdeltam2frame);
  ds1pdeltam2frame->Draw();
  if(!TruthMatch){
    FPDFBkg->SetParameter(3,(1/polnorm)*Nbkg*HDs1pDM->GetBinWidth(1));
    FPDFBkg->SetLineStyle(3);
    FPDFBkg->SetLineColor(1);
    FPDFBkg->Draw("same");
  }


   //---------------------------
  //print results into file
  //----------------------------
  text.SetTextSize(.03);

  text.SetTextColor(2);
  sprintf(textlabel,"Signal Yield= %d +- %d ",Nsig,NsigError);
  text.DrawTextNDC(.6,.8,textlabel);
  sprintf(textlabel,"Mean= %.4f +- %.4f MeV",m0*1000,m0Error*1000);
  text.DrawTextNDC(.6,.76,textlabel);
  sprintf(textlabel,"Width= %.4f +- %.4f MeV",Gamma*1000,GammaError*1000);
  text.DrawTextNDC(.6,.72,textlabel);  
  sprintf(textlabel,"Background Yield= %d +- %d ",Nbkg,NbkgError);
  text.DrawTextNDC(.6,.66,textlabel);
  sprintf(textlabel,"Coef0= %.2e ",polpar[0]);
  text.DrawTextNDC(.6,.62,textlabel);
  sprintf(textlabel,"Coef1= %.2e ",polpar[1]);
  text.DrawTextNDC(.6,.58,textlabel);
  sprintf(textlabel,"Coef2= %.2e ",polpar[2]);
  text.DrawTextNDC(.6,.54,textlabel);
  sprintf(textlabel,"Chisqure/ndof = %.1f/%d ",chisquare,ndof);
  text.DrawTextNDC(.6,.48,textlabel);

  Canvas.Print(fname);


  /*  

  TH1F* RooResidualHisto=roofitresiduals.GetResidualHisto();
  Canvas.Clear();
  RooResidualHisto->Draw("pe");
  cutline.SetLineColor(1);
  cutline.DrawLine(fitmin,0,fitmax,0);
  Canvas.Print(fname);
  
  */

  ////Compare the momentum distributions

  if(DrawPstarComparison(&Canvas,fname,PstarComparisonFile,Nsig)==0){
    cout<<"p* comparison failed"<<endl;
    return 0;
  }


  Canvas.Print(fname+"]");  
  ////////done saving plots


  //////
  if(!TruthMatch){
    if(DetermineEfficiencyVspstar(Nsig)==0){
      cout<<"Eff. Vs p* failed"<<endl;
      return 0;
    }
  }




 
  //-------
  //delete 
  //------
  delete ds1pdeltam2frame;
  delete HDs1pDM;
  delete FPDFBkg;
  delete res; 
  delete totalpdf;
  delete unbinneddata;  
  delete binneddata;
  delete ReducedNtupleCopy;

  return 1;
}

