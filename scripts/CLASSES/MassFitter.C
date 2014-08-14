
#include "MassFitter.h"
#include "../FUNCTIONS/Fits.h"
#include "RooNRBreitWigner.h"
#include "RooGaussian.h"
#include "RooProdPdf.h"
#include "RooGenericPdf.h"
#include "RooChebychev.h"
#include "RooGenericPdfN.h"
#include "RooProdPdfN.h"
#include "RooAddPdfN.h"
#include "RooRelBreitWigner.h"
#include "RooSwaveBreitWigner.h"
#include "RooDwaveBreitWigner.h"
#include "RooPolynomialFixEdgeNew.h"
#include "RooConvolutionPdf.h"
#include "RooNumConvPdf.h"
#include "RooPolyExpPdf.h"
#include "RooExpPiecePolyPdf.h"
#include "RooPolynomial.h"

ClassImp(MassFitter);
MassFitter::MassFitter(RooRealVar*MassVar):
  TNamed(TString(MassVar->GetName())+"Fitter",TString("Fitter")),
  mass(MassVar),
  filetag(""), 
  NBkgs(0),
  TwoBodyPS(0),
  dau1Mass(0),
  dau2Mass(0),
  radius("radius","radius",4),
  effPdf(0),
  respdf(0),
  resVar(0),
  totalpdfsig(0), 
  totalpdfprod(0),
  totalpdfcut(0),
  totalpdf(0),
  HistoNoCut(0),
  Histo(0),
  datasetNoCut(0),
  dataset(0),
  result(0),
  floatedParameters(0),
  massframe(0),   
  minuit(0),
  chi2(0),
  cutrangemin("cutrangemin","cutrangemin",0),
  cutrangemax("cutrangemax","cutrangemax",0),
  CutRangePdf("CutRangePdf","CutRangePdf","1-1*(cutrangemin<mass&&mass<cutrangemax)",RooArgSet(*mass,cutrangemin,cutrangemax)),
  RemoveCutRange(0),
  Eps(.001),
  NIntegbins(1),
  HFitDstPiDM(0)
{

  for(Int_t sig=0;sig<NSIGNALS;sig++){
    signame[sig]="0";
    SignalPdfres[sig]=0;
    SignalPdfprod[sig]=0;
    SignalPdfcut[sig]=0;
    CutOffPdf[sig]=0;
    SignalPdfraw[sig]=0;    
    yield[sig]=0;
    mean[sig]=0;
    width[sig]=0;
    cutoff[sig]=0;
    applyres[sig]=0;
    applyps[sig]=0;
  }

  for(Int_t bkg=0;bkg<NBACKGROUNDS;bkg++){  
    BkgPdfraw[bkg]=0;
    BkgPdfprod[bkg]=0;
    BkgPdf[bkg]=0;
    BkgHisto[bkg]=0;
    BkgDataSet[bkg]=0;
    BkgHistPdf[bkg]=0;
    BkgModifyPdf[bkg]=0;
    BkgYield[bkg]=0;
    bkgtype[bkg]="0";
    bkgname[bkg]="0";
    applyres[bkg]=0;
    bkgdraw[bkg]=0;
    for(Int_t i=0;i<BkgPolNpowers;i++)
      coef[i][bkg]=0;
  }
  totbkgpdf=0;


  histresdat=0;
  histrespdf=0;
  histresVar=0;

  reswasused=0;
  histreswasused=0;


  YaxisLabel="";

}

MassFitter::~MassFitter(){
  Clean();
}

void MassFitter::Clean(){
  if(HistoNoCut){ delete HistoNoCut;HistoNoCut=0;}
  if(Histo){ delete Histo;Histo=0;}
  if(datasetNoCut){ delete datasetNoCut;datasetNoCut=0;}
  if(dataset){ delete dataset;dataset=0;}
  if(result){ delete result;result=0;}     
  if(minuit){ delete minuit;minuit=0;}
  if(chi2){ delete chi2;chi2=0;}
  if(totalpdfcut){ delete totalpdfcut; totalpdfcut=0;}  
  if(totalpdfprod){ delete totalpdfprod; totalpdfprod=0;} 
  if(totalpdfsig){ delete totalpdfsig;totalpdfsig=0;}

 
  for(Int_t sig=0;sig<NSIGNALS;sig++){
    if(SignalPdfprod[sig]){ delete SignalPdfprod[sig];SignalPdfprod[sig]=0;}
    //if(SignalPdfeff[sig]){ delete SignalPdfeff[sig];SignalPdfeff[sig]=0;}
    if(SignalPdfcut[sig]){ delete SignalPdfcut[sig];SignalPdfcut[sig]=0;}
    if(CutOffPdf[sig]){ delete CutOffPdf[sig];CutOffPdf[sig]=0;}
    if(SignalPdfres[sig]){ delete SignalPdfres[sig];SignalPdfres[sig]=0;}
    if(SignalPdfraw[sig]){ delete SignalPdfraw[sig];SignalPdfraw[sig]=0;}
    if(yield[sig]){ delete yield[sig];yield[sig]=0;}
    if(mean[sig]){ delete mean[sig];mean[sig]=0;}
    if(width[sig]){ delete width[sig];width[sig]=0;}
    if(cutoff[sig]){ delete cutoff[sig];cutoff[sig]=0;}
    signame[sig]="0";  
  }  
 
  for(Int_t bkg=0;bkg<NBACKGROUNDS;bkg++){  
    if(BkgPdfprod[bkg]){ delete BkgPdfprod[bkg];BkgPdfprod[bkg]=0;}
    if(BkgPdfraw[bkg]){ delete BkgPdfraw[bkg];BkgPdfraw[bkg]=0;}
    if(BkgYield[bkg]){ delete BkgYield[bkg];BkgYield[bkg]=0;}
    for(Int_t i=0;i<BkgPolNpowers;i++)
      if(coef[i][bkg]){ delete coef[i][bkg]; coef[i][bkg]=0;}
    coeflist[bkg].removeAll();   
    if(BkgHisto[bkg]){delete BkgHisto[bkg];BkgHisto[bkg]=0;}
    if(BkgDataSet[bkg]){delete BkgDataSet[bkg];BkgDataSet[bkg]=0;}
    if(BkgHistPdf[bkg]){delete BkgHistPdf[bkg];BkgHistPdf[bkg]=0;}
    if(BkgModifyPdf[bkg]){delete BkgModifyPdf[bkg];BkgModifyPdf[bkg]=0;}
    bkgname[bkg]="0";  
  }
  

  if(totbkgpdf){delete totbkgpdf;totbkgpdf=0;}
  NBkgs=0;
  

  pdflist.removeAll();
  yieldlist.removeAll();

  flist.removeAll();
  ylist.removeAll();

  if(HFitDstPiDM){ delete HFitDstPiDM;HFitDstPiDM=0;}

  NIntegbins=1;
  sprintf(fitopt,"mh");//each digit is one command to minuit in that sequence   
  cutrangemin.setVal(0);
  cutrangemax.setVal(0);
  RemoveCutRange=0;


  if(histresdat){delete histresdat;histresdat=0;}
  if(histrespdf){delete histrespdf;histrespdf=0;}
  if(histresVar){delete histresVar;histresVar=0;}
   
  reswasused=0;
  histreswasused=0;

  YaxisLabel="";

  cout<<"After MassFitter:Clean "<<endl;gROOT->ls();
  
}


Bool_t MassFitter::Initialize(TH1* histo,TString configfile){
  
  if(!histo){cout<<"histo is NULL"<<endl;return 0;}
  if(histo->Integral()<=0){cout<<"Empty histogram"<<endl;return 0;}

  //clean if calling multiple times
  //Clean();

  Histo=histo;
  
  InputStream.open((const char*)(configfile));
  if(!InputStream.is_open()){cout<<"bad config filename"<<endl; return 0;}
  else cout<<"Opened config file :"<<configfile<<endl;
  //remember to initialize integers with integer values otherwise they are not read in properly

  
  TString command="";
  while((InputStream>>command)){       
    Int_t status=1;
    status=ProcessCommand(command);
    if(status==0){cout<<" Failed command "<<command<<endl;return 0;}
    if(status==-1)break;
  }

  InputStream.close();
  InputStream.clear();

 
  //-----------------
  //Initialize the signal pdfs
  //-----------------
  for(Int_t sig=0;sig<NSIGNALS;sig++){
    if(signame[sig]!="0"){
      ///Choose the correct line shape
      SignalPdfraw[sig]=GetPDF(sigshape[sig],sig);
      if(!SignalPdfraw[sig]){return 0;}	     
      SignalPdf[sig]=SignalPdfraw[sig];

      //apply cutoff 
      if(cutoff[sig]->getVal()>fitmin){
	CutOffPdf[sig]=new RooGenericPdf(TString("CutOffPdf")+(long)sig,TString("CutOffPdf")+(long)sig,TString("1.*(mass<")+signame[sig]+"cutoff"+")",RooArgSet(*mass,*cutoff[sig]));
	SignalPdfcut[sig]=new RooProdPdfN(TString(SignalPdf[sig]->GetName())+"_Cut",TString(SignalPdf[sig]->GetName())+"_Cut",*CutOffPdf[sig],*SignalPdf[sig]);
	SignalPdf[sig]=SignalPdfcut[sig];
      }


      //apply phase space factor///Note  that applyps[sig]==2 option is used below for efficiency
      if(applyps[sig]==1||applyps[sig]==3){
	if(TwoBodyPS){
	  SignalPdfprod[sig]=new RooProdPdfN(TString(SignalPdf[sig]->GetName())+"_PS",TString(SignalPdf[sig]->GetName())+"_PS",*TwoBodyPS,*SignalPdf[sig]);
	  SignalPdf[sig]=SignalPdfprod[sig];
	}
	else{cout<<" Phase space not initialized!!!"<<endl<<endl<<endl; return 0;}
      }

      //apply efficiency
      if(applyps[sig]==2||applyps[sig]==3){
	if(effPdf){
	  SignalPdfeff[sig]=new RooProdPdfN(TString(SignalPdf[sig]->GetName())+"_eff",TString(SignalPdf[sig]->GetName())+"_eff",*effPdf,*SignalPdf[sig]);
	  SignalPdf[sig]=SignalPdfeff[sig];
	  reswasused=1;
	}else {cout<<" efficiency not initialized!!!"<<endl<<endl<<endl; return 0;}
      }

      //apply the resolution function
      if(applyres[sig]==1)
	if(respdf){
	  SignalPdfres[sig]=new RooConvolutionPdf(TString(SignalPdf[sig]->GetName())+"_R",TString(SignalPdf[sig]->GetName())+"_R",mass,SignalPdf[sig],resVar,respdf); 
	  //((RooConvolutionPdf*)SignalPdfres[sig])->CacheResolution();
	  SignalPdf[sig]=SignalPdfres[sig];
	  reswasused=1;
	}else {cout<<" Resolution not initialized!!!"<<endl<<endl<<endl; return 0;}

   
      //use histogramed resolution for DCPi analysis feed-down
      if(applyres[sig]==10)
	if(histrespdf){
	  SignalPdfres[sig]=new RooConvolutionPdf(TString(SignalPdf[sig]->GetName())+"_R",TString(SignalPdf[sig]->GetName())+"_R",mass,SignalPdf[sig],histresVar,histrespdf); 
	  SignalPdf[sig]=SignalPdfres[sig];
	  histreswasused=1;
	}else {cout<<"Histogram Resolution not initialized!!!"<<endl<<endl<<endl; return 0;}
      
      
      pdflist.add(*SignalPdf[sig]);
      yieldlist.add(*yield[sig]);

      
      cout<<" Signal "<<signame[sig]<<" Initialized"<<endl;
    }
  }
  
   
  //-----------------
  //Initialize the bkg pdfs
  //-----------------  
  for(Int_t bkg=0;bkg<NBACKGROUNDS;bkg++) 
    if(bkgname[bkg]!="0"){

      if(bkgtype[bkg]=="RooPolyNomial"){ 
	cout<<"bkg=RooPolyNomial"<<endl;
	BkgPdfraw[bkg]=(RooAbsPdf*)(new RooPolynomial(bkgname[bkg]+"BkgPdf",bkgname[bkg]+"BkgPdf",*mass,coeflist[bkg]));
	//BkgPdfraw[bkg]=(RooAbsPdf*)(new RooChebychev(bkgname[bkg]+"BkgPdf",bkgname[bkg]+"BkgPdf",*mass,coeflist[bkg]));
      }
      if(bkgtype[bkg]=="ExpPol"){

	TString BkgWS2=TString("exp(")+bkgname[bkg]+"coef1*mass";
	for(Int_t i=1;i<BkgPolNpowers;i++)
	  BkgWS2+=TString("+")+bkgname[bkg]+"coef"+(long)(i+1)+"*mass**"+(long)(i+1);
	BkgWS2+=")";
      
	cout<<"bkg="<<BkgWS2<<endl;
	BkgPdfraw[bkg]=(RooAbsPdf*)(new RooGenericPdfN(bkgname[bkg]+"BkgPdf",BkgWS2,coeflist[bkg]));
      
      }

      if(bkgtype[bkg]=="PolyExp"){	
	BkgPdfraw[bkg]=(RooAbsPdf*)(new RooPolyExpPdf(bkgname[bkg]+"BkgPdf",bkgname[bkg]+"BkgPdf",*mass,coeflist[bkg]));
      }  

      if(bkgtype[bkg]=="ExpPolyPoly"){	
	BkgPdfraw[bkg]=(RooAbsPdf*)(new RooExpPiecePolyPdf(bkgname[bkg]+"BkgPdf",bkgname[bkg]+"BkgPdf",
							   *mass,*coef[0][bkg],*coef[1][bkg],*coef[2][bkg],*coef[3][bkg]));
	((RooExpPiecePolyPdf*)BkgPdfraw[bkg])->RotateCoefs(coef[4][bkg]->getVal());
      }
	
      if(!BkgPdfraw[bkg]){
      
	//attempt at opening a root file with background histo in it
	TFile BkgFile(bkgtype[bkg],"read");
	gROOT->cd();
	if(!BkgFile.IsZombie()){
	  BkgHisto[bkg]=(TH1*)BkgFile.Get("BkgHisto");
	  if(!BkgHisto[bkg]){cout<<" No BkgHisto in "<<bkgtype[bkg]<<"  file"<<endl;return 0;}
	  if(!(BkgHisto[bkg]=RebinHistoX(BkgHisto[bkg],nfitbins,fitmin,fitmax))){cout<<" Bad histo"<<endl;return 0;}	
	  if(applybkgps[bkg]){BkgHisto[bkg]->Smooth(2);applybkgps[bkg]=0;}//Use Phase-Space Option to smooth
	  BkgDataSet[bkg]=new RooDataHist(bkgname[bkg]+"BkgDataSet",bkgname[bkg]+"BkgDataSet",RooArgSet(*mass),BkgHisto[bkg]);
	  BkgHistPdf[bkg]=(RooAbsPdf*)(new RooHistPdf(bkgname[bkg]+"BkgHistPdf",bkgname[bkg]+"BkgHistPdf",RooArgSet(*mass),*BkgDataSet[bkg],1));

	  coef[0][bkg]->setRange(fitmin,fitmin);coef[0][bkg]->setVal(fitmin);coef[0][bkg]->setConstant(1);
	  TString BkgWS2=TString("1+")+bkgname[bkg]+"coef2*(mass-"+bkgname[bkg]+"coef1)";
	  for(Int_t i=2;i<5;i++)
	    BkgWS2+=TString("+")+bkgname[bkg]+"coef"+(long)(i+1)+"*(mass-"+bkgname[bkg]+"coef1)**"+(long)(i);
	  
	  BkgModifyPdf[bkg]=(RooAbsPdf*)(new RooGenericPdfN(bkgname[bkg]+"BkgModifyPdf",BkgWS2,RooArgList(*mass,*coef[0][bkg],*coef[1][bkg],*coef[2][bkg],*coef[3][bkg],*coef[4][bkg])));//coef1 must be set to fitmin
	  BkgPdfraw[bkg]=new RooProdPdfN(bkgname[bkg]+"BkgPdfraw",bkgname[bkg]+"BkgPdfraw",*BkgModifyPdf[bkg],*BkgHistPdf[bkg]);
	
	}

	BkgFile.Close();	
      }	                         
    
      if(!BkgPdfraw[bkg]){//use predefined formulas--------------------------	  	  
	TString BkgWS2=GetBackgroundFormula(bkgtype[bkg],bkgname[bkg]);
	if(BkgWS2!="0"){
	  BkgPdfraw[bkg]=((RooAbsPdf*)new RooGenericPdfN(bkgname[bkg]+"BkgPdfraw",BkgWS2,coeflist[bkg]));
	  cout<<"background type"<<BkgPdfraw[bkg]<< " initialized"<<endl; 
	}
      }
	  
      if(!BkgPdfraw[bkg]){cout<<"Bad background type"<<endl; return 0;}
      BkgPdf[bkg]=BkgPdfraw[bkg];

      //multiply phase space
      if(applybkgps[bkg]){
	if(TwoBodyPS){
	  BkgPdfprod[bkg]=new RooProdPdfN(TString(BkgPdfraw[bkg]->GetName())+"_PS",TString(BkgPdfraw[bkg]->GetName())+"_PS",*TwoBodyPS,*BkgPdfraw[bkg]);
	  BkgPdf[bkg]=BkgPdfprod[bkg];
	}else {cout<<" Phase space not initialized!!!"<<endl<<endl<<endl; return 0;}
      } 
    
 
      ///add to pdf list
      pdflist.add(*BkgPdf[bkg]); 
      flist.add(*BkgPdf[bkg]);//for independent totbkgpdf
      if(bkg<(NBkgs-1)){//bkg starts at 0
	yieldlist.add(*BkgYield[bkg]);            
	ylist.add(*BkgYield[bkg]);
      }
      if(bkg==NBkgs-1){
	BkgYield[bkg]->setRange(0,0);//must fix this last yield to zero otherwise problem with plotting below
	BkgYield[bkg]->setVal(0);
	BkgYield[bkg]->setConstant(1);
      }

      
      cout<<" Background "<<bkgname[bkg]<<" Initialized"<<endl;
    }


  ///construct total bkg pdf     
  if(NBkgs>0){
    totbkgpdf=new RooAddPdf("totbkgpdf","totbkgpdf",flist,ylist);
  }


  //--------------------------------------------------------
  ////construct the total  pdf
  //-------------------------------------------------------  
  totalpdf=new RooAddPdfN("totalpdf","totalpdf",pdflist,yieldlist);


  //totalpdf=totalpdfsig;    
//   //multiply by the range cut
//   if(cutrangemin.getVal()!=0.||cutrangemax.getVal()!=0.){
//     totalpdfcut=new RooProdPdfN(TString(totalpdf->GetName())+"_cut",TString(totalpdf->GetName())+"_cut",CutRangePdf,*totalpdf);
//     totalpdf=totalpdfcut; 
//   }
  


  //--------------------------------------------------------
  //create binned dataset  //
  //--------------------------------------------------------
  dataset=new RooDataHist("MassFitterdataset","MassFitterdataset",RooArgSet(*mass),Histo);
  

  //
  filetag="";
  for(Int_t sig=0;sig<NSIGNALS;sig++){
    if(signame[sig]!="0")filetag+=TString("_")+sigshape[sig]+(long)applyps[sig]+(long)applyres[sig];
  } 

  return 1;
}

Int_t MassFitter::Fit(){
  
  //RooFitResult* res=totalpdf->fitTo(*dataset,"mhert");//m=MIGRAD,h=HESSE,e=EXTENTED,r=Results,t=time: "mhert"   
   
  //if(TString(Histo->GetTitle())==TString("unweighted")){
  if(NIntegbins==0){/// use "nll"
    ///Gives correct result for small statistics, Needs to be used only with unweighted data.
    cout<<" calling RooNLLVar1D for unweighted histogram------------------------------"<<endl<<endl;

    //Options:(const char *name, const char *title, RooAbsPdf& pdf, RooAbsData& data,const TH1* HEffHisto,Bool_t extended, const char* rangeName, const char* addCoefRangeName,Int_t nCPU, Bool_t verbose, Bool_t splitRange)
    //chi2=new RooNLLVar1D("nll","nll",*totalpdf,*dataset,0,0,0,0,ncpu,0,0);
    chi2=new RooNLLVar1D("nll","nll",*totalpdf,*dataset,RooFit::DataError(RooAbsData::SumW2),RooFit::NumCPU(ncpu));//

    //RooNLLVar1D nll("nll","nll",*totalpdf,*dataset,effpdf,0,0,0,2,0,0);
    //nll.InitEff();
  }
  if(NIntegbins>0){
    // Chi2Var has trouble when using multiple cpus--> The SumW2 option is not transfered
    //chi2=new RooChi2Var("chi2","chi2",*totalpdf,*dataset,RooFit::DataError(RooAbsData::SumW2),RooFit::NumCPU(ncpu));//      
    
    //SumW2 option with multiple cpus fixed in RooChi2VarIntegral 
    chi2=new RooChi2VarIntegral("chi2Integral","chi2Integral",*totalpdf,*dataset,RooFit::DataError(RooAbsData::SumW2),RooFit::NumCPU(ncpu));//
    ((RooChi2VarIntegral*)chi2)->SetIntegralBins(NIntegbins);//

  }
  if(!chi2){
    cout<<"MassFitter: bad NIntegbins option"<<endl;
    return -1;
  }
  
  minuit=new RooMinuit(*chi2);

  //   minuit->setEps(Eps);


  ///Time the fit
  ULong_t starttime=(ULong_t)(gSystem->Now());

  Int_t f=0;
  while(fitopt[f]!=0){
    TString opt(fitopt[f]);
    if(opt=="k")minuit->seek();//takes forever
    if(opt=="s")minuit->simplex();
    if(opt=="m")minuit->migrad();
    if(opt=="i")minuit->improve();
    if(opt=="h")minuit->hesse();//must call if migrad converges too fast for good cov matrix
    if(opt=="n")minuit->minos();//should use with care as it will call migrad again-->screw cov matrix  

    time=(ULong_t)(gSystem->Now())-starttime;
    cout<<"The fit time is "<<time/1000<<" seconds"<<endl;

    f++;
  }



  

    

  result=minuit->save();
  
  //if(!(result=minuit->fit("mrt")))return -1; //produces a new DataSet in memory which cant be deleted  

  cout<<"The fit status is "<<result->status()<<endl;  
 

  //----------------------------
  //print the results
  //----------------------------
  result->Print();
  

  //----------------------------------
  //extract the floated parameters
  //----------------------------------
  if(!(floatedParameters= (RooArgList*)&result->floatParsFinal()))return -1;    

  //count degrees of freedom
  Int_t nbins=0;
  for(Int_t i=1;i<=Histo->GetXaxis()->GetNbins();i++)
    if(Histo->GetBinError(i)>0)nbins++;
  ndof=nbins - floatedParameters->getSize();
  

  
  //release the pdf cuts
  if(RemoveCutRange){
    cutrangemin.setVal(0);
    cutrangemax.setVal(0);

    delete dataset;
    dataset=datasetNoCut;
    datasetNoCut=0;
    
    //delete Histo;
    TH1* htemp=Histo;
    Histo=HistoNoCut;
    HistoNoCut=htemp;

  }

  //Clean ;  
  //  delete chi2;
  
  return result->status();
}

RooPlot* MassFitter::GetFitPlot(){
  //Returns ownership of RooPlot

  //if(massframe){ delete massframe;massframe=0;}//clean for multiple calls


  
  //----------------------------
  //the Fit plot
  //---------------------------- 
  massframe=mass->frame();    

  //plot the dataset
  dataset->plotOn(massframe,RooFit::DataError(RooAbsData::SumW2));




  ///plot the backgrounds
  for(Int_t bkg=0;bkg<NBACKGROUNDS;bkg++)
    if(bkgname[bkg]!="0"&&bkgdraw[bkg]==1)
      totalpdf->plotOn(massframe,Components(RooArgSet(*BkgPdf[bkg])),LineStyle(kSolid),LineColor(3));
  
  
  //plot the signals
  for(Int_t sig=0;sig<NSIGNALS;sig++)
    if(signame[sig]!="0"){
      if(sigdraw[sig]>1000&&sigdraw[sig]<2000){//draw alone
	if((sigdraw[sig]%1000)>100&&(sigdraw[sig]%1000)<200){//draw as line
	  if(((sigdraw[sig]%1000)%100)>10&&((sigdraw[sig]%1000)%100)<20)//draw as solid line
	    totalpdf->plotOn(massframe,Components(RooArgSet(*SignalPdf[sig])),LineStyle(kSolid),LineColor(((sigdraw[sig]%1000)%100)%10));	
	  if(((sigdraw[sig]%1000)%100)>20&&((sigdraw[sig]%1000)%100)<30)//draw as dashed line
	    totalpdf->plotOn(massframe,Components(RooArgSet(*SignalPdf[sig])),LineStyle(kDashed),LineColor(((sigdraw[sig]%1000)%100)%10));
	}
	if((sigdraw[sig]%1000)>200&&(sigdraw[sig]%1000)<300){//draw as fill
	  if(((sigdraw[sig]%1000)%100)>10&&((sigdraw[sig]%1000)%100)<20)//draw as solid fill
	    totalpdf->plotOn(massframe,Components(RooArgSet(*SignalPdf[sig])),DrawOption("F"),FillColor(((sigdraw[sig]%1000)%100)%10));
	  if(((sigdraw[sig]%1000)%100)>20&&((sigdraw[sig]%1000)%100)<30){//draw as diagonal fill
	    totalpdf->plotOn(massframe,Components(RooArgSet(*SignalPdf[sig])),DrawOption("LF"),FillStyle(3145),FillColor(((sigdraw[sig]%1000)%100)%10));
	    totalpdf->plotOn(massframe,Components(RooArgSet(*SignalPdf[sig])),LineStyle(kSolid),LineColor(((sigdraw[sig]%1000)%100)%10));	
	  }
	  if(((sigdraw[sig]%1000)%100)>30&&((sigdraw[sig]%1000)%100)<40){//draw as diagonal fill
	    totalpdf->plotOn(massframe,Components(RooArgSet(*SignalPdf[sig])),DrawOption("LF"),FillStyle(3154),FillColor(((sigdraw[sig]%1000)%100)%10));
	    totalpdf->plotOn(massframe,Components(RooArgSet(*SignalPdf[sig])),LineStyle(kSolid),LineColor(((sigdraw[sig]%1000)%100)%10));	
	  }
	}
	
      }else if(sigdraw[sig]>2000&&sigdraw[sig]<3000&&totbkgpdf){//draw on top of background
	RooArgSet compset;
	for(Int_t bkg=0;bkg<NBACKGROUNDS;bkg++)
	  if(bkgname[bkg]!="0")compset.add(*BkgPdf[bkg]);
	compset.add(*SignalPdf[sig]);
	totalpdf->plotOn(massframe,Components(compset),LineStyle(kSolid),LineColor(((sigdraw[sig]%1000)%100)%10));
      }else if(sigdraw[sig]!=0)
	cout<<"Plot option for signal="<<signame[sig]<<" Not found"<<endl;
    }
  
  
  

  //plot total background
  RooArgSet bkgset;
  for(Int_t bkg=0;bkg<NBACKGROUNDS;bkg++)
    if(bkgname[bkg]!="0")bkgset.add(*BkgPdf[bkg]);
  totalpdf->plotOn(massframe,Components(bkgset),LineStyle(kSolid),LineColor(1));
  
		
  //plot the total pdf
  totalpdf->plotOn(massframe,LineColor(kBlue));
  
  massframe->SetTitle("");
  massframe->GetXaxis()->SetTitle(Histo->GetXaxis()->GetTitle());  //do not transfer y title because bin width may have changed
  //massframe->GetYaxis()->SetTitle(YaxisLabel);


  return massframe;
}

TH1* MassFitter::GetBkgSubHist(){
  TH1*HistoClone=NULL;
  if(totbkgpdf){
    TH1*HBkg=totbkgpdf->createHistogram("HBkg",*mass);
    HBkg->Scale(GetBkgYield()/HBkg->Integral());
    
    HistoClone=(TH1*)Histo->Clone();
    HistoClone->Sumw2();
    HistoClone->Add(HBkg,-1);
    delete HBkg;
  }else cout<<"ERROR: MassFitter::GetBkgSubHist No totbkgpdf"<<endl;

  return  HistoClone;//returns ownership
}

RooPlot* MassFitter::GetBkgSubPlot(){


  //----------------------------
  //the Fit plot
  //---------------------------- 
  massframe=mass->frame();    
  massframe->SetTitle("");
  massframe->GetXaxis()->SetTitle(Histo->GetXaxis()->GetTitle());

  TH1*HistoBkgSub=GetBkgSubHist();
  if(!HistoBkgSub)return 0;
  RooDataHist bkgsubdataset("bkgsubdataset","bkgsubdataset",RooArgSet(*mass),HistoBkgSub);
  bkgsubdataset.plotOn(massframe,RooFit::DataError(RooAbsData::SumW2));
  delete HistoBkgSub;

  //plot the signals
  for(Int_t sig=0;sig<NSIGNALS;sig++)
    if(signame[sig]!="0"){
      if(sigdraw[sig]>1000&&sigdraw[sig]<2000||(sigdraw[sig]>2000&&sigdraw[sig]<3000)){//draw alone
	if((sigdraw[sig]%1000)>100&&(sigdraw[sig]%1000)<200){//draw as line
	  if(((sigdraw[sig]%1000)%100)>10&&((sigdraw[sig]%1000)%100)<20)//draw as solid line
	    totalpdf->plotOn(massframe,Normalization(Histo->Integral()/GetSigYield()),Components(RooArgSet(*SignalPdf[sig])),LineStyle(kSolid),LineColor(((sigdraw[sig]%1000)%100)%10));	
	  if(((sigdraw[sig]%1000)%100)>20&&((sigdraw[sig]%1000)%100)<30)//draw as dashed line
	    totalpdf->plotOn(massframe,Normalization(Histo->Integral()/GetSigYield()),Components(RooArgSet(*SignalPdf[sig])),LineStyle(kDashed),LineColor(((sigdraw[sig]%1000)%100)%10));
	}
	if((sigdraw[sig]%1000)>200&&(sigdraw[sig]%1000)<300){//draw as fill
	  if(((sigdraw[sig]%1000)%100)>10&&((sigdraw[sig]%1000)%100)<20)//draw as solid fill
	    totalpdf->plotOn(massframe,Normalization(Histo->Integral()/GetSigYield()),Components(RooArgSet(*SignalPdf[sig])),DrawOption("F"),FillColor(((sigdraw[sig]%1000)%100)%10));
	  if(((sigdraw[sig]%1000)%100)>20&&((sigdraw[sig]%1000)%100)<30){//draw as diagonal fill
	    totalpdf->plotOn(massframe,Normalization(Histo->Integral()/GetSigYield()),Components(RooArgSet(*SignalPdf[sig])),DrawOption("LF"),FillStyle(3145),FillColor(((sigdraw[sig]%1000)%100)%10));
	    totalpdf->plotOn(massframe,Normalization(Histo->Integral()/GetSigYield()),Components(RooArgSet(*SignalPdf[sig])),LineStyle(kSolid),LineColor(((sigdraw[sig]%1000)%100)%10));	
	  }
	  if(((sigdraw[sig]%1000)%100)>30&&((sigdraw[sig]%1000)%100)<40){//draw as diagonal fill
	    totalpdf->plotOn(massframe,Normalization(Histo->Integral()/GetSigYield()),Components(RooArgSet(*SignalPdf[sig])),DrawOption("LF"),FillStyle(3154),FillColor(((sigdraw[sig]%1000)%100)%10));
	    totalpdf->plotOn(massframe,Normalization(Histo->Integral()/GetSigYield()),Components(RooArgSet(*SignalPdf[sig])),LineStyle(kSolid),LineColor(((sigdraw[sig]%1000)%100)%10));	
	  }
	}
	//Normalization((int)(yield[sig]->getVal()*Histo->Integral())),
      }else if(sigdraw[sig]!=0)
	cout<<"Plot option for signal="<<signame[sig]<<" Not found"<<endl;
    }
  
  
  

  //plot total signal
  RooArgSet sigset;
  for(Int_t sig=0;sig<NBACKGROUNDS;sig++)
    if(signame[sig]!="0")sigset.add(*SignalPdf[sig]);
  totalpdf->plotOn(massframe,Components(sigset),LineStyle(kSolid),LineColor(4),Normalization(Histo->Integral()/GetSigYield()));//)
 	       

  return massframe;
}


void MassFitter::Save(TString outputdir){
  


  //ps file 
  TCanvas Canvas("CanvasMassFitter","CanvasMassFitter");
  TString psfile;
  psfile=outputdir+"/FitPars"+filetag+".ps";
  Canvas.Print(psfile+"[");
 
  TFile RootFile(outputdir+"/FitPars"+filetag+".root","recreate");
  gROOT->cd();

  //   massframe=mass->frame();
  //   massframe->SetTitle("");
  //   massframe->GetXaxis()->SetTitle("m(D*#pi)   GeV/c^{2}");
  //   sprintf(textlabel,"Entries / %.1f MeV/c^{2}",1000*(mass->getMax()-mass->getMin())/mass->getBins());
  //   massframe->GetYaxis()->SetTitle(textlabel);
  //   dataset->plotOn(massframe,RooFit::DataError(RooAbsData::SumW2));
  Canvas.Clear();
  //massframe->Draw();
  Histo->SetStats(1);
  Histo->Draw("pe");
  Canvas.Print(psfile);
  //delete massframe;massframe=0;
  
  RootFile.cd();
  Histo->Write();gROOT->cd();


  // 
  massframe=GetFitPlot();



  //----------------------------
  //now make the residual plot
  //----------------------------
  Float_t chisquare=0; 
  if(!HFitDstPiDM){
    HFitDstPiDM=totalpdf->createHistogram("HFitDstPiDM",*mass);
    HFitDstPiDM->Scale(Histo->Integral());
  }
  if(!HFitDstPiDM){
    cout<<"No HDstPiDM  returned."<<endl;
    Canvas.Print(psfile+"]");
    return;
  }

    
  JResiduals roofitresiduals("res",Histo,HFitDstPiDM);


  Canvas.Clear();
  Histo->SetStats(0);
  roofitresiduals.MakeFitPlotWithResiduals(&Canvas,massframe);
  if(RemoveCutRange){
    Canvas.cd(1);
    HistoNoCut->SetMarkerColor(2);
    HistoNoCut->Draw("same");
  }
  Canvas.Print(psfile); 

  RootFile.cd();
  massframe->SetName("fitframe");massframe->Write();gROOT->cd();
  delete massframe;massframe=0;
 
  RootFile.cd();
  TH1*H=roofitresiduals.GetNormResidualHisto();
  H->SetName("HNormResid");H->Write(); delete H;
  H=roofitresiduals.GetResidualHisto();
  H->SetName("HResid");H->Write(); delete H;
  gROOT->cd(); 
  

  TH1F* HChiVsBin=roofitresiduals.GetChisquareHisto();    
  Canvas.Clear();
  HChiVsBin->Draw("p");
  Canvas.Print(psfile); 
  RootFile.cd();
  HChiVsBin->Write();gROOT->cd();
  delete HChiVsBin;  

  TH1F* HResProj=roofitresiduals.GetNormResidualProjection();    
  Canvas.Clear();
  //FitHistogram1gaus(HResProj,-5,5);
  HResProj->Fit("gaus");
  HResProj->Draw("pe");
  Canvas.Print(psfile);
  RootFile.cd();
  HResProj->Write();gROOT->cd();
  delete HResProj;
  


  //----------------------------
  //the Bkg Components Fit plot
  //---------------------------- 
  if(totbkgpdf){

    massframe=mass->frame();    
    massframe->SetTitle("");
    massframe->GetXaxis()->SetTitle(Histo->GetXaxis()->GetTitle());
    dataset->plotOn(massframe,RooFit::DataError(RooAbsData::SumW2));
    
    for(Int_t bkg=0;bkg<NBACKGROUNDS;bkg++)
      if(bkgname[bkg]!="0"){
	RooArgSet compset;
	Float_t norm=0;
	for(Int_t b=0;b<=bkg;b++){
	  compset.add(*BkgPdf[b]);
	  norm+=BkgYield[b]->getVal();
	}
		
	totalpdf->plotOn(massframe,
			 Components(compset),
			 LineStyle(kSolid),
			 LineColor(bkg<(NBkgs-1)? 3:4)
			 );
      }
    

  
    Canvas.Clear();
    massframe->Draw();
    Canvas.Print(psfile);

    RootFile.cd();
    massframe->SetName("bkgframe");massframe->Write();gROOT->cd();
    delete massframe;massframe=0;
  
  }

  ///Plot the bkg subtracted distribution
  if(totbkgpdf){
    if((massframe=GetBkgSubPlot())){
      Canvas.Clear();
      massframe->Draw();
      Canvas.Print(psfile);
      RootFile.cd();
      massframe->SetName("bkgsubframe");massframe->Write();gROOT->cd();
      delete massframe;massframe=0;
    }

   
    TH1*HistoBkgSub=GetBkgSubHist();
    if(HistoBkgSub){
      HistoBkgSub->SetName(TString(Histo->GetName())+"_BkgSub");
      RootFile.cd();
      HistoBkgSub->Write();
      gROOT->cd();
      delete HistoBkgSub;
    }

  }
 
    
 
  if(respdf&&reswasused){
    massframe=resVar->frame();    

    Canvas.Clear();
    mass->setVal(fitmin+(.5)*(fitmax-fitmin)/10.);
    respdf->plotOn(massframe); 
    massframe->SetTitle("Resolution PDF");
    massframe->Draw();        
    
    RooPlot* temp[9];
    for(Int_t i=0;i<9;i++){
      mass->setVal(fitmin+(i+1.5)*(fitmax-fitmin)/10.);
      temp[i]=resVar->frame();  
      respdf->plotOn(temp[i]); 
      temp[i]->Draw("same");      
    }
    Canvas.Print(psfile);

    RootFile.cd();
    massframe->SetName("resframe");massframe->Write();gROOT->cd();

    delete massframe;massframe=0;
    for(Int_t i=0;i<9;i++)
      delete temp[i];


  }

  if(histrespdf&&histreswasused){
    massframe=histresVar->frame();    

    Canvas.Clear();
    histrespdf->plotOn(massframe); 
    massframe->SetTitle("Histogramed Resolution PDF");
    massframe->Draw();  
    Canvas.Print(psfile);

    RootFile.cd();
    massframe->SetName("histresframe");massframe->Write();gROOT->cd();
    delete massframe;massframe=0;
  }
 

  if(TwoBodyPS){
    massframe=mass->frame();    
    Canvas.Clear();
    TwoBodyPS->plotOn(massframe); 
    massframe->SetTitle("Phase-Space PDF");
    massframe->Draw();
    Canvas.Print(psfile);
    RootFile.cd();
    massframe->SetName("psspaceframe");massframe->Write();gROOT->cd();
    delete massframe;massframe=0; 
  }

  if(effPdf){
    massframe=mass->frame();    
    Canvas.Clear();
    effPdf->plotOn(massframe); 
    massframe->SetTitle("Efficiency PDF");
    massframe->Draw();
    Canvas.Print(psfile);
    RootFile.cd();
    massframe->SetName("psspaceframe");massframe->Write();gROOT->cd();
    delete massframe;massframe=0; 
  }
 

        
  chisquare=roofitresiduals.GetChisquare(); 

  cout<<"Chisquare per DF= "<<chisquare<<"/"<<ndof<<"="<<chisquare/ndof<<endl;

  Bool_t badfit=0;
  if(chisquare/ndof>5.0){
    cout<<"-----------------Fit Chisquare > 3.0------------------------"<<endl<<endl<<endl; 
    badfit=1;
  }

 
  //----------------------------
  //print results into file
  //----------------------------
  TText text;
  text.SetTextSize(.03);
  text.SetTextColor(1);

  Canvas.Clear();  
  sprintf(textlabel,"RESULTS:");
  text.DrawTextNDC(.05,.9,textlabel);

  text.SetTextColor(1);
  for(Int_t sig=0;sig<NSIGNALS;sig++){
    if( signame[sig]!="0"){
      sprintf(textlabel,"%s: Yield= %d +- %d",(const char*)signame[sig],(int)(yield[sig]->getVal()*Histo->Integral()),
	      (int)(yield[sig]->getError()*Histo->Integral()));
      text.DrawTextNDC(.0001,.85-sig*.05,TString(textlabel));

      if(mean[sig]->isConstant()){
	sprintf(textlabel,"Mean= %.2f (fix)",mean[sig]->getVal()*1000);
	text.DrawTextNDC(.26,.85-sig*.05,TString(textlabel));
      }else{
	sprintf(textlabel,"Mean= %.2f +- %.2f",mean[sig]->getVal()*1000,mean[sig]->getError()*1000);
	text.DrawTextNDC(.26,.85-sig*.05,TString(textlabel));
      }

      if(width[sig]->isConstant()){
	sprintf(textlabel,"Width= %.2f (fix)",width[sig]->getVal()*1000);
	text.DrawTextNDC(.49,.85-sig*.05,TString(textlabel));
      }else{
	sprintf(textlabel,"Width= %.2f +- %.2f",width[sig]->getVal()*1000,width[sig]->getError()*1000);
	text.DrawTextNDC(.49,.85-sig*.05,TString(textlabel));
      }

      text.DrawTextNDC(.69,.85-sig*.05,TString("")
		       +" shp="+sigshape[sig]
		       +" ps="+(long)applyps[sig]
		       +" res="+(long)applyres[sig]
		       );

    }
  }

  
  for(Int_t bkg=0;bkg<NBACKGROUNDS;bkg++)
    if(bkgname[bkg]!="0"){

      if(bkg<(NBkgs-1))
	sprintf(textlabel,"%s= %d +- %d ",BkgYield[bkg]->GetName(),(int)(BkgYield[bkg]->getVal()*Histo->Integral()),(int)(BkgYield[bkg]->getError()*Histo->Integral()));
      else {
	
	Float_t sbyield=0;
	for(Int_t sig=0;sig<NSIGNALS;sig++)
	  if(signame[sig]!="0")
	    sbyield+=yield[sig]->getVal();
	
	for(Int_t bg=0;bg<NBACKGROUNDS;bg++)
	  if(bkgname[bg]!="0")
	    sbyield+=BkgYield[bg]->getVal();	

	sprintf(textlabel,"%s= %d +- %d ",BkgYield[bkg]->GetName(),(int)((1.-sbyield)*Histo->Integral()),0);
      }

      //(int)(BkgYield->getError()*Histo->Integral()));
      text.DrawTextNDC(.01+bkg*.25,.35,textlabel);
      
      for(Int_t i=0;i<BkgPolNpowers;i++){
	sprintf(textlabel,TString("")+(long)i+"= %.2e +- %.2e",coef[i][bkg]->getVal(),coef[i][bkg]->getError());
	text.DrawTextNDC(.01+bkg*.25,.35-(i+1)*.03,textlabel);
      }
    }    

  sprintf(textlabel,"Chisqure/ndof = %.1f/%d      Cov. Matrix Qual.=%d",chisquare,ndof,result->covQual());
  text.DrawTextNDC(.4,.0,textlabel);
  Canvas.Print(psfile);
  

  RootFile.cd();
  result->Write();
//   totalpdf->Write();
//   if(totbkgpdf)totbkgpdf->Write();

  gROOT->cd();
 
  RootFile.Close();
  
  Canvas.Print(psfile+"]");

  //---------------------------------
  //save the parameters to a file 
  //----------------------------------
  ofstream ParameterOutput; 
  ParameterOutput.open((const char*)(outputdir+"/FitPars"+filetag+".txt"));
  cout<<"Created File "<<outputdir+"/FitPars"+filetag+".txt"<<endl;
  ParameterOutput<<"Range"<<" "<<"min"<<" "<<mass->getMin()<<endl;
  ParameterOutput<<"Range"<<" "<<"max"<<" "<<mass->getMax()<<endl;
  ParameterOutput<<"Range"<<" "<<"nbins"<<" "<<mass->getBins()<<endl;
  ParameterOutput<<"Range"<<" "<<"integral"<<" "<<Histo->Integral()<<endl;
  ParameterOutput<<"Range"<<" "<<"chisquare"<<" "<<chisquare<<endl;
  ParameterOutput<<"Range"<<" "<<"ndf"<<" "<<ndof<<endl;
  ParameterOutput<<"Range"<<" "<<"status"<<" "<<result->status()<<endl;  
  ParameterOutput<<"Range"<<" "<<"covqual"<<" "<<result->covQual()<<endl;
  ParameterOutput<<"Range"<<" "<<"fittime"<<" "<<time<<endl;

  for(Int_t sig=0;sig<NSIGNALS;sig++){
    if(signame[sig]!="0"){
      ParameterOutput<<" "<<endl;
      ParameterOutput<<signame[sig]<<" "<<"yield"<<" "<<(int)(yield[sig]->getVal()*Histo->Integral())<<endl;
      ParameterOutput<<signame[sig]<<" "<<"yielderror"<<" "<<(int)(yield[sig]->getError()*Histo->Integral())<<endl;  
      // ParameterOutput<<(int)yield[sig]->getAsymErrorHi()<<" "<<(int)yield[sig]->getAsymErrorLo()<<endl;    
      ParameterOutput<<signame[sig]<<" "<<"yieldmin"<<" "<<yield[sig]->getMin()<<endl;   
      ParameterOutput<<signame[sig]<<" "<<"yieldmax"<<" "<<yield[sig]->getMax()<<endl;   
      

      ParameterOutput<<signame[sig]<<" "<<"mean"<<" "<<mean[sig]->getVal()<<endl;
      ParameterOutput<<signame[sig]<<" "<<"meanerror"<<" "<<mean[sig]->getError()<<endl; 
      ParameterOutput<<signame[sig]<<" "<<"meanmin"<<" "<<mean[sig]->getMin()<<endl;   
      ParameterOutput<<signame[sig]<<" "<<"meanmax"<<" "<<mean[sig]->getMax()<<endl;   


      ParameterOutput<<signame[sig]<<" "<<"width"<<" "<<width[sig]->getVal()<<endl;
      ParameterOutput<<signame[sig]<<" "<<"widtherror"<<" "<<width[sig]->getError()<<endl; 
      ParameterOutput<<signame[sig]<<" "<<"widthmin"<<" "<<width[sig]->getMin()<<endl;   
      ParameterOutput<<signame[sig]<<" "<<"widthmax"<<" "<<width[sig]->getMax()<<endl;   

      ParameterOutput<<signame[sig]<<" "<<"cutoff"<<" "<<cutoff[sig]->getVal()<<endl;


      //latex format      
      cout<<fixed
	  <<setprecision(1)<<" & "<<yield[sig]->getVal()*Histo->Integral()/1000.<<" $\\pm$ "<<yield[sig]->getError()*Histo->Integral()/1000.
	  <<setprecision(1)<<" & "<<mean[sig]->getVal()*1000<<" $\\pm$ "<<mean[sig]->getError()*1000
	  <<setprecision(1)<<" & "<<width[sig]->getVal()*1000<<" $\\pm$ "<<width[sig]->getError()*1000
	  <<" \\\\ "<<endl;
    }
  }

  for(Int_t bkg=0;bkg<NBACKGROUNDS;bkg++)
    if(bkgname[bkg]!="0"){
      
      ParameterOutput<<" "<<endl;
      ParameterOutput<<bkgname[bkg]<<" "<<"yield"<<setfill(' ')<<" "<<(int)(BkgYield[bkg]->getVal()*Histo->Integral())<<endl;
      ParameterOutput<<bkgname[bkg]<<" "<<"yielderror"<<" "<<(int)(BkgYield[bkg]->getError()*Histo->Integral())<<endl;
      ParameterOutput<<bkgname[bkg]<<" "<<"yieldmin"<<" "<<(int)(BkgYield[bkg]->getMin()*Histo->Integral())<<endl;   
      ParameterOutput<<bkgname[bkg]<<" "<<"yieldmax"<<" "<<(int)(BkgYield[bkg]->getMax()*Histo->Integral())<<endl;   

      for(Int_t i=0;i<BkgPolNpowers;i++){
	ParameterOutput<<bkgname[bkg]<<" "<<"coef"<<i<<" "<<coef[i][bkg]->getVal()<<endl;
	ParameterOutput<<bkgname[bkg]<<" "<<"coef"<<i<<"error"<<" "<<coef[i][bkg]->getError()<<endl;
	ParameterOutput<<bkgname[bkg]<<" "<<"coef"<<i<<"min"<<" "<<coef[i][bkg]->getMin()<<endl;   
	ParameterOutput<<bkgname[bkg]<<" "<<"coef"<<i<<"max"<<" "<<coef[i][bkg]->getMax()<<endl;   
      
      }
      
    }


  ParameterOutput<<"END"<<endl;
  ParameterOutput.close();
  ParameterOutput.clear();
  cout<<"Closed "<<outputdir+"/FitPars"+filetag+".txt"<<endl;


}



RooAbsPdf* MassFitter::GetPDF(TString type,Int_t id){
  if(type=="NRBW"){
    return new RooNRBreitWigner(TString("SignalPdf")+(long)id,TString("SignalPdf")+(long)id,mass,*mean[id],*width[id]);     
  }
  if(type=="RBW"){
    return new RooRelBreitWigner(TString("SignalPdf")+(long)id,TString("SignalPdf")+(long)id,*mass,*mean[id],*width[id]);     
  }
  if(type=="SBW"){
    //if(!TwoBodyPS)return 0;
    return new RooSwaveBreitWigner(TString("SignalPdf")+(long)id,TString("SignalPdf")+(long)id,mass,*mean[id],*width[id],*dau1Mass,*dau2Mass);     
  }
  if(type=="DBW"){
    //if(!TwoBodyPS)return 0;
    return new RooDwaveBreitWigner(TString("SignalPdf")+(long)id,TString("SignalPdf")+(long)id,mass,*mean[id],*width[id],*dau1Mass,*dau2Mass,radius);
     
  }    
  if(type=="Gauss"){
    return new RooGaussian(TString("SignalPdf")+(long)id,TString("SignalPdf")+(long)id,*mass,*mean[id],*width[id]);
     
  }    
  cout<<" Signal type "<<type<<" not available."<<endl; 
      
  return NULL;
}


TString MassFitter::GetBackgroundFormula(TString type,TString name){

  //choose background type
  TString formula="0";

  if(type=="4Exp")
    formula=TString("exp(")+name+"coef1*mass) + "+name+"coef2*exp("+name+"coef3*mass) + "+name+"coef4*exp("+name+"coef5*mass) + "+name+"coef6*exp("+name+"coef7*mass)";

  if(type=="GaussExp")
    formula=TString("exp(-.5*(mass-")+name+"coef1)**2/"+name+"coef2**2)+"+name+"coef3*exp("+name+"coef4*mass+"+name+"coef5*mass**2+"+name+"coef6*mass**3+"+name+"coef7*mass**4+"+name+"coef8*mass**5)";

  if(type=="powExp"){
    TString BkgWS2=TString("((mass-")+name+"coef1)**"+name+"coef2)*exp("+name+"coef3*mass";
    for(Int_t i=1;i<BkgPolNpowers;i++)
      BkgWS2+=TString("+")+name+"coef"+(long)(i+1)+"*mass**"+(long)(i+1);
    BkgWS2+=")";
    formula=BkgWS2;
  }

  if(type=="myPoly"){ 
    TString BkgWS2=TString("1+")+name+"coef1*mass";
    for(Int_t i=1;i<BkgPolNpowers;i++)
      BkgWS2+=TString("+")+name+"coef"+(long)(i+1)+"*mass**"+(long)(i+1);
    formula=BkgWS2;
  }
  
  cout<<" The background formula is: "<<formula<<endl;
 
  return formula;
}

TGraph* MassFitter::ScanParameter(TString Signal,TString ParName){


  RooRealVar* scannedpar=NULL;

  //find the parameter
  for(Int_t sig=0;sig<NSIGNALS;sig++){
    if(Signal==signame[sig]){
      if(Signal+ParName==yield[sig]->GetName()) scannedpar=yield[sig];
      if(Signal+ParName==mean[sig]->GetName()) scannedpar=mean[sig];
      if(Signal+ParName==width[sig]->GetName()) scannedpar=width[sig];
    }
  }
  
  //   if(Signal==bkgname)
  //     for(Int_t i=0;i<BkgPolNpowers;i++)
  //       if(ParName==coef[i]->GetName()) scannedpar=coef[i];
  
  if(!scannedpar){
    cout<<"Parameter:"<<Signal<<" "<<ParName<<"  not found."<<endl;
    return 0;
  }

  if(scannedpar->isConstant()){
    cout<<"Parameter:"<<Signal<<" "<<ParName<<"  is constant"<<endl;
    return 0;
  }
    
  

  //scan parameter from to max
  Int_t Npoints=100;
  TGraph G(Npoints);
  Float_t parmin=scannedpar->getVal()-(scannedpar->getVal()-scannedpar->getMin())/2.;
  Float_t parmax=scannedpar->getVal()+(scannedpar->getMax()-scannedpar->getVal())/2.;
  Float_t parstep=(parmax-parmin)/Npoints;
  Float_t parinit=scannedpar->getVal();

  for(Int_t i=0;i<Npoints;i++){
    
    scannedpar->setVal(parmin+i*parstep);

    TH1* HFitHist=totalpdf->createHistogram("HFitHist",*mass);
    if(!HFitHist){
      cout<<"No HDstPiDM  returned."<<endl;
      return 0;
    }

    HFitHist->Scale(Histo->Integral());    
    JResiduals roofitresiduals("res",Histo,HFitHist);  

    G.SetPoint(i,scannedpar->getVal(),roofitresiduals.GetChisquare()/ndof);
    delete HFitHist;

  }

  scannedpar->setVal(parinit);
  
  return (TGraph*)G.Clone();
}

void MassFitter::ScanParameter2D(TString sig1,TString par1,TString sig2,TString par2,TH2F* H2){
 
  if(!H2){cout<<" Not Histo provided H2"<<endl;return;}
  if(!chi2){cout<<" chi2 Not yet created!"<<endl;return;}

  RooRealVar* scannedpar1=NULL;
  RooRealVar* scannedpar2=NULL;

  //find the parameter in Signals
  for(Int_t sig=0;sig<NSIGNALS;sig++){
    if(sig1==signame[sig]){
      if(sig1+par1==yield[sig]->GetName()) scannedpar1=yield[sig];
      if(sig1+par1==mean[sig]->GetName()) scannedpar1=mean[sig];
      if(sig1+par1==width[sig]->GetName()) scannedpar1=width[sig];
    }

    if(sig2==signame[sig]){
      if(sig2+par2==yield[sig]->GetName()) scannedpar2=yield[sig];
      if(sig2+par2==mean[sig]->GetName()) scannedpar2=mean[sig];
      if(sig2+par2==width[sig]->GetName()) scannedpar2=width[sig];
    }

    
  }


  //find the parameter in Bkgs
  for(Int_t bkg=0;bkg<NBACKGROUNDS;bkg++)
    if(bkgname[bkg]!="0"){
      if(BkgYield[bkg]->GetName()==sig1+par1)scannedpar1=BkgYield[bkg];
      for(Int_t i=0;i<BkgPolNpowers;i++)
	if(coef[i][bkg]->GetName()==sig1+par1)scannedpar1=coef[i][bkg];
  
      if(BkgYield[bkg]->GetName()==sig2+par2)scannedpar2=BkgYield[bkg];
      for(Int_t i=0;i<BkgPolNpowers;i++)
	if(coef[i][bkg]->GetName()==sig2+par2)scannedpar2=coef[i][bkg];      

    }
    
    
  
  

  if(!scannedpar1||!scannedpar2){
    cout<<"Parameter(s) not found."<<endl;
    return;
  }

  if(scannedpar1->isConstant()||scannedpar2->isConstant()){
    cout<<"Parameter(s) is constant"<<endl;
    return;
  }
    
  Float_t par1init=scannedpar1->getVal();
  Float_t par2init=scannedpar2->getVal();  


  for(Int_t i=1;i<=H2->GetXaxis()->GetNbins();i++){    
    scannedpar1->setVal(H2->GetXaxis()->GetBinCenter(i));

    for(Int_t j=1;j<=H2->GetYaxis()->GetNbins();j++){      
      scannedpar2->setVal(H2->GetYaxis()->GetBinCenter(j));

      H2->SetBinContent(i,j,1./chi2->getVal());
    }
  }

  H2->GetXaxis()->SetTitle(scannedpar1->GetName());
  H2->GetYaxis()->SetTitle(scannedpar2->GetName());
  H2->GetZaxis()->SetTitle("1/#chi^{2}");

  scannedpar1->setVal(par1init);
  scannedpar2->setVal(par2init);
  
}

RooPlot* MassFitter::GetContour(TString sig1,TString par1,TString sig2,TString par2){

  if(!minuit){cout<<" minuit does not exist"<<endl;return 0;}
  
  //find the parameter
  RooRealVar* var1=0;
  RooRealVar* var2=0;
  for(Int_t sig=0;sig<NSIGNALS;sig++){
    if(sig1==signame[sig]){
      if(sig1+par1==yield[sig]->GetName()) var1=yield[sig];
      if(sig1+par1==mean[sig]->GetName()) var1=mean[sig];
      if(sig1+par1==width[sig]->GetName()) var1=width[sig];
    }
    if(sig2==signame[sig]){
      if(sig2+par2==yield[sig]->GetName()) var2=yield[sig];
      if(sig2+par2==mean[sig]->GetName()) var2=mean[sig];
      if(sig2+par2==width[sig]->GetName()) var2=width[sig];
    }
  }
  if(!var1){cout<<"MassFitter::GetContour(): "<<sig1+par1<<" not found "<<endl;return 0;}
  if(var1->isConstant()){cout<<"MassFitter::GetContour(): "<<sig1+par1<<" is constant"<<endl;return 0;}
  if(!var2){cout<<"MassFitter::GetContour(): "<<sig2+par2<<" not found "<<endl;return 0;}
  if(var2->isConstant()){cout<<"MassFitter::GetContour(): "<<sig2+par2<<" is constant"<<endl;return 0;}
  cout<<" Going to make contour of "<<sig1+par1<<" and "<<sig2+par2<<endl;
  
  //cout<<gMinuit<<" "<<gDirectory<<endl;
  //return minuit->contour(*var1,*var2,1.*(n>=1),1.*(n>=2),1.*(n>=3),1.*(n>=4),1.*(n>=5),1.*(n>=6));
  
  RooPlot* cont=new RooPlot(*var1,*var2,var1->getVal()-2*var1->getError(),var1->getVal()+2*var1->getError(),var2->getVal()-2*var2->getError(),var2->getVal()+2*var2->getError());
  result->plotOn(cont,*var1,*var2,"MEAV12");
  cont->GetYaxis()->SetNdivisions(4,1);
  cont->GetXaxis()->SetNdivisions(4,1);
  return cont; 
}


TH1F* MassFitter::GetNormResidualHisto(){
 
  //----------------------------
  //now make the residual plot
  //---------------------------- 
  if(!HFitDstPiDM){
    HFitDstPiDM=totalpdf->createHistogram("HFitDstPiDM",*mass);
    HFitDstPiDM->Scale(Histo->Integral());
  }
  if(!HFitDstPiDM){
    cout<<"No HDstPiDM  returned."<<endl;
    return 0;
  }

  
  JResiduals roofitresiduals("res",Histo,HFitDstPiDM);
  
  
  return roofitresiduals.GetNormResidualHisto();
}

Bool_t MassFitter::FixParameter(TString SigName,TString ParName,Float_t value){
  RooRealVar* par=0;
  for(Int_t sig=0;sig<NSIGNALS;sig++)
    if(signame[sig]==SigName){
      if(yield[sig]->GetName()==SigName+ParName)par=yield[sig];
      if(mean[sig]->GetName()==SigName+ParName)par=mean[sig];
      if(width[sig]->GetName()==SigName+ParName)par=width[sig];
    }
    
  for(Int_t bkg=0;bkg<NBACKGROUNDS;bkg++)
    if(bkgname[bkg]==SigName){
      if(BkgYield[bkg]->GetName()==SigName+ParName)par=BkgYield[bkg];
      for(Int_t i=0;i<BkgPolNpowers;i++)
	if(coef[i][bkg]->GetName()==SigName+ParName)par=coef[i][bkg];
    }
  
  
  if(!par){cout<<" Parameter "<<SigName+ParName<<" Not Found"<<endl;return 0;}
  
  par->setRange(value,value);
  par->setConstant(1);
  par->setVal(value);
  
  return 1;
}

void MassFitter::DifferencePars(TString oldfile,TString newfile,TString TableOption){

  ParameterRead oldread;
  if(!oldread.OpenFitPars(oldfile)){cout<<"bad oldfile filename"<<endl; return;}

  ParameterRead newread;

  Bool_t usecurrent=0;
  if(newfile=="0"){//compare to current parameters
    usecurrent=1;
  }else{//compare to written file
    if(!newread.OpenFitPars(newfile)){cout<<"bad newfile filename"<<endl; return;}
    
  }

  if(TableOption=="V"){//vertical table
    for(Int_t sig=0;sig<NSIGNALS;sig++)
      if(signame[sig]!="0"){	
	if(usecurrent){
	  cout<<signame[sig]
	      <<" "<<Histo->Integral()*(yield[sig]->getVal()-oldread.GetFitPar(signame[sig],"yield"))
	      <<" "<<(mean[sig]->getVal()-oldread.GetFitPar(signame[sig],"mean"))
	      <<" "<<(width[sig]->getVal()-oldread.GetFitPar(signame[sig],"width"))
	      <<endl;
	}else{
	  cout<<signame[sig]
	      <<" "<<Histo->Integral()*(newread.GetFitPar(signame[sig],"yield")-oldread.GetFitPar(signame[sig],"yield"))
	      <<" "<<(newread.GetFitPar(signame[sig],"mean")-oldread.GetFitPar(signame[sig],"mean"))
	      <<" "<<(newread.GetFitPar(signame[sig],"width")-oldread.GetFitPar(signame[sig],"width"))
	      <<endl;
	}
	
      }
    
  }
  if(TableOption=="H"){//horizontal table
    for(Int_t sig=0;sig<NSIGNALS;sig++)
      if(signame[sig]!="0")
	cout<<" "<<signame[sig];
    cout<<endl;
    
    if(usecurrent){
      for(Int_t sig=0;sig<NSIGNALS;sig++)
	if(signame[sig]!="0")cout<<" & "<<Histo->Integral()*(yield[sig]->getVal()-oldread.GetFitPar(signame[sig],"yield"));      
      cout<<endl;      
      for(Int_t sig=0;sig<NSIGNALS;sig++)
	if(signame[sig]!="0") cout<<" & "<<(mean[sig]->getVal()-oldread.GetFitPar(signame[sig],"mean"));
      cout<<endl;      
      for(Int_t sig=0;sig<NSIGNALS;sig++)
	if(signame[sig]!="0") cout<<" & "<<(width[sig]->getVal()-oldread.GetFitPar(signame[sig],"width"));
      cout<<endl;
	
    }else{
      for(Int_t sig=0;sig<NSIGNALS;sig++)
	if(signame[sig]!="0")cout<<" & "<<Histo->Integral()*(newread.GetFitPar(signame[sig],"yield")-oldread.GetFitPar(signame[sig],"yield"));
      cout<<endl;
      for(Int_t sig=0;sig<NSIGNALS;sig++)
	if(signame[sig]!="0")cout<<" & "<<(newread.GetFitPar(signame[sig],"mean")-oldread.GetFitPar(signame[sig],"mean"));
      cout<<endl;
      for(Int_t sig=0;sig<NSIGNALS;sig++)
	if(signame[sig]!="0")cout<<" & "<<(newread.GetFitPar(signame[sig],"width")-oldread.GetFitPar(signame[sig],"width"));
      cout<<endl;

    }
  }
  

 
  newread.CloseFitPars();
  oldread.CloseFitPars();

}


void MassFitter::MakeParTable(TString TableOption,Float_t scale){

  if(TableOption=="V"){//vertical table
    for(Int_t sig=0;sig<NSIGNALS;sig++)
      if(signame[sig]!="0"){	
	cout<<signame[sig]
	    <<"& "<<Histo->Integral()*(yield[sig]->getVal())<<" \\pm "<<Histo->Integral()*(yield[sig]->getError())
	    <<"& "<<scale*(mean[sig]->getVal())<<" \\pm "<<scale*mean[sig]->getError()
	    <<"& "<<scale*(width[sig]->getVal())<<" \\pm "<<scale*width[sig]->getError()
	    <<endl;
      }
    
  }
  if(TableOption=="H"){//horizontal table
    for(Int_t sig=0;sig<NSIGNALS;sig++)
      if(signame[sig]!="0")
	cout<<" "<<signame[sig];
    cout<<endl;

    for(Int_t sig=0;sig<NSIGNALS;sig++)
      if(signame[sig]!="0")cout<<" & "<<Histo->Integral()*(yield[sig]->getVal())<<" \\pm "<<Histo->Integral()*yield[sig]->getError(); 
    cout<<endl;      
    for(Int_t sig=0;sig<NSIGNALS;sig++)
      if(signame[sig]!="0") cout<<" & "<<scale*(mean[sig]->getVal())<<" \\pm "<<scale*mean[sig]->getError();
    cout<<endl;      
    for(Int_t sig=0;sig<NSIGNALS;sig++)
      if(signame[sig]!="0") cout<<" & "<<scale*(width[sig]->getVal())<<" \\pm "<<scale*width[sig]->getError();
    cout<<endl;
	
  }
  
}

Int_t MassFitter::ProcessCommand(TString command){

  if(command=="Range"){
    InputStream>>fitmin>>fitmax>>nfitbins>>YaxisLabel;
    cout<<command<<" "<<fitmin<<" "<<fitmax<<" "<<nfitbins<<" "<<YaxisLabel<<endl;

    mass->setRange(fitmin,fitmax);
    mass->setBins(nfitbins);
    //mass->setUnit(YaxisLabel);
    
    //Rebin the histogram accordingly
    if(!(Histo=RebinHistoX(Histo,nfitbins,fitmin,fitmax))){cout<<" Bad histo"<<endl;return 0;}
    
    return 1;
  }


  ///process commands
  if(command=="CutRange"){  
    Float_t min=0,max=0;
    InputStream>>min>>max;
    cout<<command<<" "<<min<<" "<<max<<endl;
    
    HistoNoCut=(TH1F*)Histo->Clone();
    HistoNoCut->SetName(TString(Histo->GetName())+"NoCut");
    datasetNoCut=new RooDataHist("MassFitterdatasetNoCut","MassFitterdatasetNoCut",RooArgSet(*mass),HistoNoCut);

    for(Int_t i=1;i<=Histo->GetNbinsX();i++)
      if(min<Histo->GetXaxis()->GetBinCenter(i)&&Histo->GetXaxis()->GetBinCenter(i)<max){//offset so pdf evaluates to zero where error is zero
	Histo->SetBinContent(i,0);
	Histo->SetBinError(i,0);
      }
    
    
    cutrangemin.setVal(min);
    cutrangemax.setVal(max);

    return 1;
  }

  if(command=="RemoveCutRange"){
    InputStream>>RemoveCutRange;
    cout<<command<<" "<<RemoveCutRange<<endl;     
    return 1;
  }

  if(command=="Ncpu"){
    InputStream>>ncpu;
    cout<<command<<" "<<ncpu<<endl;
    return 1;
  }
  if(command=="Nintegbins"){
    InputStream>>NIntegbins;
    cout<<command<<" "<<NIntegbins<<endl;
    return 1;
  }
  if(command=="FitOpt"){
    TString FitOpt;
    InputStream>>FitOpt;
    sprintf(fitopt,"%s",(const char*)FitOpt);
    cout<<command<<" "<<FitOpt<<endl;
    return 1;
  }
  if(command=="HistRes"){
    TString fname;
    TString hname;
    InputStream>>fname>>hname;
    cout<<command<<" "<<fname<<" "<<hname<<endl;
    TFile resfile(fname,"read");
    gROOT->cd();
    TH1*HRes=(TH1*)resfile.Get(hname);
    if(!HRes){cout<<"No Resolution Histo in"<<fname<<endl;return 0;}
    cout<<"Extracted "<<HRes->GetName()<<" Integral="<<HRes->Integral()<<"  Mean="<<HRes->GetMean()<<" RMS="<<HRes->GetRMS()<<endl;
    histresVar=new RooRealVar("histresVar","histresVar",HRes->GetXaxis()->GetXmin(),HRes->GetXaxis()->GetXmax());
    histresVar->setBins(HRes->GetXaxis()->GetNbins());
    histresdat=new RooDataHist("histresdat","histresdat",RooArgList(*histresVar),HRes,1);
    histrespdf=new RooHistPdf("histrespdf","histrespdf",RooArgSet(*histresVar),*histresdat,0);
    return 1;
  } 
  if(command=="Signal"){
    Int_t sig=-1;
    for(Int_t i=0;i<NSIGNALS&&sig==-1;i++)
      if(signame[i]=="0")sig=i;
    if(sig==-1){cout<<"No where to put signal"<<endl;return 0;}

    Float_t x=0,xmin=0,xmax=0;Bool_t xfix=0; 
    
    InputStream>>signame[sig]>>sigdraw[sig];
    cout<<"Signal "<<signame[sig]<<endl;

    InputStream>>x  >>xmin  >>xmax  >>xfix;      
    cout<<"yield:"<<x<<" "<<xmin<<" "<<xmax<<" "<<xfix<<endl;
    yield[sig]=new RooRealVar(signame[sig]+"yield",signame[sig]+"yield",x,xmin,xmax);//do not change name structure: needed later
    yield[sig]->setConstant(xfix);
 
    InputStream>>x  >>xmin  >>xmax  >>xfix;
    cout<<"mean:"<<x<<" "<<xmin<<" "<<xmax<<" "<<xfix<<endl;
    mean[sig]=new RooRealVar(signame[sig]+"mean",signame[sig]+"mean",x,xmin,xmax); 
    mean[sig]->setConstant(xfix); 
   
    InputStream>>x  >>xmin  >>xmax  >>xfix;
    cout<<"width:"<< x<<" "<<xmin<<" "<<xmax<<" "<<xfix<<endl;
    width[sig]=new RooRealVar(signame[sig]+"width",signame[sig]+"width",x,xmin,xmax);
    width[sig]->setConstant(xfix);

    InputStream>>sigshape[sig]>>applyps[sig]>>applyres[sig]>>x;
    cout<<"Shape: "<<sigshape[sig]<<" ps="<<applyps[sig]<<" res="<<applyres[sig]<<" cutoff="<<x<<endl;
    cout<<endl;
      
    cutoff[sig]=new RooRealVar(signame[sig]+"cutoff",signame[sig]+"cutoff",x);
      

    return 1;
  }


  if(command=="Background"){

    Int_t bkg=-1;
    for(Int_t i=0;i<NBACKGROUNDS&&bkg==-1;i++)
      if(bkgname[i]=="0")bkg=i;
    if(bkg==-1){cout<<"No where to put background"<<endl;return 0;}

    Float_t x=0,xmin=0,xmax=0;Bool_t xfix=0; 

    InputStream>>bkgname[bkg]>>bkgdraw[bkg];
    cout<<"Background"<<bkgname[bkg]<<endl;
    
    
    coeflist[bkg].add(*mass);
       
    InputStream>>x  >>xmin  >>xmax  >>xfix;      
    cout<<"yield:"<<x<<" "<<xmin<<" "<<xmax<<" "<<xfix<<endl;
    BkgYield[bkg]=new RooRealVar(bkgname[bkg]+"yield",bkgname[bkg]+"yield",x,xmin,xmax);//do not change name structure: needed later
    BkgYield[bkg]->setConstant(xfix);
    

    Int_t checkpar;
    for(Int_t i=0;i<BkgPolNpowers;i++){
      InputStream>>checkpar >>x  >>xmin  >>xmax  >>xfix;
      cout<<checkpar<<" "<<x<<" "<<xmin<<" "<<xmax<<" "<<xfix<<endl;
      if((checkpar-1)!=i){
	cout<<"polynomial pars not read right: "<<i<<endl;
	return 0;
      }
      
      //if(coef[i])delete coef[i];
      coef[i][bkg]=new RooRealVar(bkgname[bkg]+"coef"+(long)(i+1),bkgname[bkg]+"coef"+(long)(i+1),x,xmin,xmax);
      coef[i][bkg]->setConstant(xfix);
      coeflist[bkg].add(*coef[i][bkg]);
    }
      
      
    InputStream>>bkgtype[bkg]>>applybkgps[bkg];
    cout<<" Type: "<<bkgtype[bkg]<<" "<<" ps="<<applybkgps[bkg]<<endl;
       
    NBkgs++;

    return 1;
  }

  
  
  if(command=="Constrain"){
    TString parname;
    TString otherparname;
    InputStream>>parname>>otherparname;
    
    cout<<command<<" "<<parname<<" "<<otherparname<<endl;
    
    //now search all signal and bkg pars for paname
    RooRealVar** par=NULL;
    RooRealVar** otherpar=NULL;
    
    for(Int_t sig=0;sig<NSIGNALS;sig++){
      if(signame[sig]!="0"){
	if(yield[sig]->GetName()==parname){par=&yield[sig];}
	if(mean[sig]->GetName()==parname){par=&mean[sig];}
	if(width[sig]->GetName()==parname){par=&width[sig];}

	if(yield[sig]->GetName()==otherparname){otherpar=&yield[sig];}
	if(mean[sig]->GetName()==otherparname){otherpar=&mean[sig];}
	if(width[sig]->GetName()==otherparname){otherpar=&width[sig];}
      }
    }  
    
    for(Int_t bkg=0;bkg<NBACKGROUNDS;bkg++){  
      if(bkgname[bkg]!="0"){
	if(BkgYield[bkg]->GetName()==parname){par=&BkgYield[bkg];}
	for(Int_t i=0;i<BkgPolNpowers;i++)
	  if(coef[i][bkg]->GetName()==parname){par=&coef[i][bkg];}

	if(BkgYield[bkg]->GetName()==otherparname){otherpar=&BkgYield[bkg];}
	for(Int_t i=0;i<BkgPolNpowers;i++)
	  if(coef[i][bkg]->GetName()==otherparname){otherpar=&coef[i][bkg];}
      }
    }
    
    if(!*par){cout<<" Parameter for constrain:"<<parname<<" Not found"<<endl;}
    if(!*otherpar){cout<<" Parameter for constrain:"<<otherparname<<" Not found"<<endl;}
    
    //note that program will likely crash if cleaning in multiple fits
    delete *par;
    *par=*otherpar;
    

    return 1;
  }


  //Did not find this command
  return -1;

}
