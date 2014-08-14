#include "DstPiAnalysis.h"
#include "RooBreitWigner.h"
#include "RooGaussian.h"
#include "RooChi2Var.h"
#include "../RooNLLVar1D.h"
#include "../RooChi2VarIntegral.h"
#include "RooMinuit.h"
#include "../RooRelBreitWigner.h"
#include "../RooSwaveBreitWigner.h"
#include "../RooDwaveBreitWigner.h"
#include "../RooPolynomialFixEdge.h"
#include "../RooConvolutionPdf.h"
#include "RooHistPdf.h"
#include "RooAbsOptTestStatistic.h"
#include <RooHistPdf.h>

#define BkgPolNpowers 10
#define NSIGNALS 10

Float_t HistoOffset=0;

TObject* DstPiAnalysis::RooFitMassPeak(Int_t WhichHisto){
  cout<<"Going to fit the D*Pi Mass Peak"<<endl;

  if(WhichHisto==5)HistoOffset=0;
  //--------------------------------------------------------
  ///
  //--------------------------------------------------------
  Float_t fitmin=.25;
  Float_t fitmax=.6;
  Int_t nfitbins=70;      
  
  Float_t x,xmin,xmax;Bool_t xfix; 
    
  //---------------------------
  //read the parameters from the file
  //----------------------------   
  ifstream ParameterInput;
  ParameterInput.open((const char*)(_OutputDir+"/"+roofitconfigfile));
  if(!ParameterInput.is_open()){cout<<"bad config filename"<<endl; return 0;}
  else cout<<"Opened config file :"<<roofitconfigfile<<endl;
  //remember to initialize integers with integer values otherwise they are not read in properly
  

  Int_t ncpu=0;
  Bool_t correff=0;

  ///---------------------
  //The variable to be fitted
  //----------------------
  ParameterInput>>fitmin>>fitmax>>nfitbins;
  cout<<fitmin<<" "<<fitmax<<" "<<nfitbins<<endl;
  mass.setRange(fitmin,fitmax);
  mass.setBins(nfitbins);

  ParameterInput>>x  >>xmin  >>xmax  >>xfix;  
  cout<<"Radius :"<<x<<" "<<xmin<<" "<<xmax<<" "<<xfix<<endl;
  radius.setVal(x);
  radius.setRange(xmin,xmax);
  radius.setConstant(xfix);

  ParameterInput>>ncpu>>correff;
  cout<<"ncpu "<<ncpu<<" correff "<<correff<<endl<<endl;;
  
  Float_t helimin=-1;
  Float_t helimax=1.;
  Int_t nhelibins=20;
  RooRealVar helicity("helicity","helicity",helimin,helimax);  
  helicity.setBins(nhelibins);

  //The signals 
  RooRealVar* sigYield[NSIGNALS];       
  RooRealVar* mean[NSIGNALS];
  RooRealVar* width[NSIGNALS]; 
  RooRealVar* heli[NSIGNALS]; 
  Bool_t applyps[NSIGNALS];
  Bool_t applyres[NSIGNALS];
  Int_t sigshape[NSIGNALS];
  Bool_t applypfctr[NSIGNALS];
  TString signame[NSIGNALS]; 

  Int_t component=-1;
  for(Int_t sig=0;sig<NSIGNALS;sig++){
    signame[sig]="0";    

    ParameterInput>>component;
    if(component!=sig){
      cout<<"No Signal: "<<sig<<endl;
    }else {
      ParameterInput>>signame[component];
      cout<<signame[component]<<endl;

      ParameterInput>>x  >>xmin  >>xmax  >>xfix;      
      if(WhichHisto==5&&component==9){x=nfitbins*HistoOffset;xmin=0;xmax=x;xfix=1;}
      cout<<"Yield:"<<x<<" "<<xmin<<" "<<xmax<<" "<<xfix<<endl;
      sigYield[component]=new RooRealVar(TString("sigYield")+(long)sig,TString("sigYield")+(long)sig,x,xmin,xmax);  
      sigYield[component]->setConstant(xfix);
 
      ParameterInput>>x  >>xmin  >>xmax  >>xfix;
      cout<<"mean:"<<x<<" "<<xmin<<" "<<xmax<<" "<<xfix<<endl;
      mean[component]=new RooRealVar(TString("mean")+(long)sig,TString("mean")+(long)sig,x,xmin,xmax); 
      mean[component]->setConstant(xfix); 
   
      ParameterInput>>x  >>xmin  >>xmax  >>xfix;
      cout<<"width:"<< x<<" "<<xmin<<" "<<xmax<<" "<<xfix<<endl;
      width[component]=new RooRealVar(TString("width")+(long)sig,TString("width")+(long)sig,x,xmin,xmax);
      width[component]->setConstant(xfix);

      ParameterInput>>x  >>xmin  >>xmax  >>xfix;
      cout<<"heli:"<< x<<" "<<xmin<<" "<<xmax<<" "<<xfix<<endl;
      heli[component]=new RooRealVar(TString("heli")+(long)sig,TString("heli")+(long)sig,x,xmin,xmax);
      heli[component]->setConstant(xfix);

      ParameterInput>>sigshape[component]>>applyps[component]>>applyres[component]>>applypfctr[component];
      cout<<"Shape: "<<sigshape[component]<<" "<<applyps[component]<<" "<<applyres[component]<<" "<<applypfctr[component]<<endl;
      cout<<endl;
    }
  }

  //the background
  TString bkgname="0";
  RooArgList coeflist;
  RooRealVar* BkgYield=NULL;
  RooRealVar* coef[BkgPolNpowers];//I get break seg. when I try create an object array of RooRealVars 
  RooRealVar* bkgheli[6];
  Int_t bkgtype=0;
  Bool_t applybkgps=0;
  ParameterInput>>component>>bkgname;
  if(component!=10)cout<<" No Background"<<endl;
  else {
    coeflist.add(mass);
    
    ParameterInput>>x  >>xmin  >>xmax  >>xfix;
    cout<<x<<" "<<xmin<<" "<<xmax<<" "<<xfix<<endl;   
    BkgYield=new RooRealVar("BkgYield","BkgYield",x,xmin,xmax);  BkgYield->setConstant(xfix);  
    coeflist.add(*BkgYield);//required for edge fix
    
    Int_t checkpar;
    for(Int_t i=0;i<BkgPolNpowers;i++){
      ParameterInput>>checkpar >>x  >>xmin  >>xmax  >>xfix;
      cout<<checkpar<<" "<<x<<" "<<xmin<<" "<<xmax<<" "<<xfix<<endl;
      if((checkpar-1)!=i){
	cout<<"polynomial pars not read right: "<<i<<endl;
	return 0;
      }
      coef[i]=new RooRealVar(TString("coef")+(long)(i+1),TString("coef")+(long)(i+1),x,xmin,xmax);    coef[i]->setConstant(xfix);
      coeflist.add(*coef[i]);
    }
  
    
    //read in helicity parameters
    for(Int_t i=0;i<6;i++){
      ParameterInput>>checkpar >>x  >>xmin  >>xmax  >>xfix;
      cout<<checkpar<<" "<<x<<" "<<xmin<<" "<<xmax<<" "<<xfix<<endl;
      if((checkpar-1)!=i){
	cout<<"polynomial pars not read right: "<<i<<endl;
	return 0;
      }
      bkgheli[i]=new RooRealVar(TString("bkgheli")+(long)(i+1),TString("bkgheli")+(long)(i+1),x,xmin,xmax); 
      bkgheli[i]->setConstant(xfix);
      coeflist.add(*bkgheli[i]);
    }
      


    ParameterInput>>bkgtype>>applybkgps;
    cout<<"Type: "<<bkgtype<<" "<<" ps="<<applybkgps<<endl;
   
  }


  ParameterInput.close();
  ParameterInput.clear();


  //ps file
  roofitfileid="";
  for(Int_t sig=0;sig<NSIGNALS;sig++){
    if(signame[sig]!="0")roofitfileid+=TString("_")+(long)sigshape[sig]+(long)applyps[sig]+(long)applyres[sig]+(long)applypfctr[sig];
  } 
  if(bkgname!="0")
    roofitfileid+=TString("_")+(long)bkgtype;
    

  filename=_OutputDir+"/DstPiRooFit"+roofitfileid+tempfittag+".ps";
  Canvas.Print(filename+"[");
   
  RooPlot* massframe=NULL;


  //-------------------------------------------------------- 
  //Get the data
  //--------------------------------------------------------
  TH1* HDstarPiFit=GetMassHistoForFit(WhichHisto,nfitbins,fitmin,fitmax);//dont change title, it is used for chi2 choice
  if(!HDstarPiFit){cout<<"No histo for fit"<<endl;return 0;}
  Canvas.Clear(); 
  if(HDstarPiFit->GetYaxis()->GetNbins()>1){
    HDstarPiFit->SetStats(0); 
    HDstarPiFit->Draw("colz");
  }
  else HDstarPiFit->Draw("pe");
  Canvas.Print(filename);  


  //Get the binned efficiency 
  TH1*HEfficiency=NULL;
  //RooHistPdf* effpdf=NULL;
  if(TString(HDstarPiFit->GetTitle())==TString("unweighted")&&correff){
    TH1*HEffCorr=GetMassHistoForFit(WhichHisto+1,nfitbins,fitmin,fitmax);//convenction: eff. corr. histo id = +1
    if(HEffCorr){  
      if(TString(HEffCorr->GetTitle())==TString("weighted")){
	HEfficiency=(TH1*)HDstarPiFit->Clone();
	HEfficiency->SetTitle("");
	HEfficiency->GetYaxis()->SetTitle("efficiency");
	HEfficiency->Divide(HEffCorr);
	cout<<"Efficiency Histo "<<HEfficiency->GetName()<<"    Max Eff="<<HEfficiency->GetMaximum()<<endl;
	Canvas.Clear();
	HEfficiency->SetStats(0); 
	HEfficiency->Draw("colz");
	Canvas.Print(filename);     

	///Multiply the total pdf by the efficiency for plotting
	//RooDataHist* effdat=new RooDataHist("effdat","effdat",RooArgList(mass,helicity),HEfficiency);
// 	if(HDstarPiFit->GetYaxis()->GetNbins()>1)
// 	  effpdf=new RooHistPdf("effpdf","effpdf",RooArgList(mass,helicity),*(new RooDataHist("effdat","effdat",RooArgList(mass,helicity),HEfficiency)),1);
// 	else   
// 	  effpdf=new RooHistPdf("effpdf","effpdf",RooArgList(mass),*(new RooDataHist("effdat","effdat",RooArgList(mass),HEfficiency)),1);
// 	cout<<" effPdf "<<effpdf<<" EffPdf Val="<<effpdf->getVal()<<endl;
// 	massframe=mass.frame();
// 	effpdf->plotOn(massframe);
// 	Canvas.Clear(); 
// 	massframe->Draw();  
// 	Canvas.Print(filename);  
// 	delete massframe;	
      }
      delete HEffCorr;
    }
  }
  
  


  //------------------------------------------------
  //The resolution 
  //------------------------------------------------  
  /////----------------------
  /////--Use single gaussian
  /////----------------------
  //   RooRealVar ResMean("ResMean","ResMean",0);
  //   RooRealVar ResSigma("ResSigma","ResSigma",.00174);
  //   RooGaussian respdf("respdf","respdf",mass,ResMean,ResSigma); 

  ////-------------------------------------------
  ////----Use the histogram as a resolution pdf
  ////-----------------------------------------
  //   TFile HistFile(_OutputDir+"/ResolutionEff.root","read");
  //   gROOT->cd();
  //   TH1F* HDstPiDMRes=(TH1F*)HistFile.Get("HDstPiDMRes");
  //   if(!HDstPiDMRes){cout<<" No resolution Histogram "<<endl;return 0; }
  //   Canvas.Clear();
  //   HDstPiDMRes->Draw();
  //   Canvas.Print(filename);
  //   mass.setRange(-.01,.01);
  //   mass.setBins(50); 
  //   RooDataHist resda("resda","resda",RooArgSet(mass),HDstPiDMRes);
  //   HistFile.Close();
  //   RooHistPdf respdf("respdf","respdf",RooArgSet(mass),resda,1);
  
  massframe=resVar.frame(-.05,.05);
  mass.setVal(2.422);
  respdf->plotOn(massframe,LineColor(2));
  mass.setVal(2.459);
  respdf->plotOn(massframe,LineColor(4));
  Canvas.Clear(); 
  massframe->Draw();  
  Canvas.Print(filename);  
  delete massframe;
  TH1*reshist=respdf->createHistogram("reshist",resVar);
  Canvas.Clear(); 
  reshist->Draw();
  Canvas.Print(filename); 
  delete reshist;
//   mass.setRange(fitmin,fitmax);
//   mass.setBins(nfitbins);
  
  

  //------------------
  //p* cut shape pdf
  //------------------
  sprintf(textlabel,"%.5f+%.5f*mass",.031,.25);//cut /total gen
  //sprintf(textlabel,"%.5f+%.5f*mass",.145,.204);//no shape eff corr /total gen
  if(_D0Mode==1){
    sprintf(textlabel,"%.3f+%.3f*mass",.14,.20);//fully eff corr/total gen
    if(WhichHisto==80) sprintf(textlabel,"%.3f+%.3f*mass",.151,.03);
    if(WhichHisto==81) sprintf(textlabel,"%.3f+%.3f*mass",-.076,.126);
    if(WhichHisto==82) sprintf(textlabel,"%.3f+%.3f*mass",-.105,.11);
    if(WhichHisto==83) sprintf(textlabel,"%.3f+%.3f*mass",.058,-.014);
  }
  if(_D0Mode==2){
    sprintf(textlabel,"%.3f+%.3f*mass",.095,.225);//fully eff corr/total gen
    if(WhichHisto==80) sprintf(textlabel,"%.3f+%.3f*mass",.0,.00);
    if(WhichHisto==81) sprintf(textlabel,"%.3f+%.3f*mass",.0,.00);
    if(WhichHisto==82) sprintf(textlabel,"%.3f+%.3f*mass",.0,.00);
    if(WhichHisto==83) sprintf(textlabel,"%.3f+%.3f*mass",.0,.00);
  }
  RooGenericPdfN* pstardf=new RooGenericPdfN("pstarpdf","pstarpdf",textlabel,RooArgSet(mass));  

  

  //------------------------
  //Helicity pdfs
  //---------------------
  RooGenericPdfN* helipdf[NSIGNALS];


  //-----------------
  //the signal pdfs
  //-----------------
  massframe=mass.frame();
  RooAbsPdf* SignalPdf[NSIGNALS];
  for(Int_t sig=0;sig<NSIGNALS;sig++){
    if( signame[sig]!="0"){
      ///Choose the correct line shape
      if(sigshape[sig]==0){
	SignalPdf[sig]=new RooBreitWigner(TString("SignalPdf")+(long)sig,TString("SignalPdf")+(long)sig,mass,*mean[sig],*width[sig]);     
      }
      if(sigshape[sig]==1){
	SignalPdf[sig]=new RooRelBreitWigner(TString("SignalPdf")+(long)sig,TString("SignalPdf")+(long)sig,mass,*mean[sig],*width[sig]);     
      }
      if(sigshape[sig]==2){
	SignalPdf[sig]=new RooSwaveBreitWigner(TString("SignalPdf")+(long)sig,TString("SignalPdf")+(long)sig,&mass,*mean[sig],*width[sig],&TwoBodyPS);     
      }
      if(sigshape[sig]==3){
	SignalPdf[sig]=new RooDwaveBreitWigner(TString("SignalPdf")+(long)sig,TString("SignalPdf")+(long)sig,&mass,*mean[sig],*width[sig],&TwoBodyPS,radius);
     
      }    
      if(sigshape[sig]==4){
	SignalPdf[sig]=new RooGaussian(TString("SignalPdf")+(long)sig,TString("SignalPdf")+(long)sig,mass,*mean[sig],*width[sig]);
     
      }    
      if(!SignalPdf[sig])return 0;	     

      //apply the resolution function
      if(applyres[sig]){
// 	RooNumConvPdf* tempconvpdf=new RooNumConvPdf(TString(SignalPdf[sig]->GetName())+"_Res",TString(SignalPdf[sig]->GetName())+"_Res",mass,*SignalPdf[sig],respdf);
//   	tempconvpdf->setConvolutionWindow(ResMean,ResSigma,3); 
//  	SignalPdf[sig]=tempconvpdf;

        SignalPdf[sig]=new RooConvolutionPdf(TString(SignalPdf[sig]->GetName())+"_R",TString(SignalPdf[sig]->GetName())+"_R",&mass,SignalPdf[sig],&resVar,respdf);	
      }

      
      //apply phase space factor
      if(applyps[sig])
	SignalPdf[sig]=new RooProdPdfN(TString(SignalPdf[sig]->GetName())+"_PS",TString(SignalPdf[sig]->GetName())+"_PS",TwoBodyPS,*SignalPdf[sig]);
      
      //
      if(applypfctr[sig])
	SignalPdf[sig]=new RooProdPdfN(TString(SignalPdf[sig]->GetName())+"_p",TString(SignalPdf[sig]->GetName())+"_p",*pstardf,*SignalPdf[sig]); 

      if(HDstarPiFit->GetYaxis()->GetNbins()>1){
	helipdf[sig]=new RooGenericPdfN(TString("helipdf")+(long)sig,TString("helipdf")+(long)sig,
					TString("1+heli")+(long)sig+"*helicity*helicity",RooArgSet(helicity,*heli[sig])); 
	SignalPdf[sig]=new RooProdPdfN(TString(SignalPdf[sig]->GetName())+"_h",TString(SignalPdf[sig]->GetName())+"_h",*helipdf[sig],*SignalPdf[sig]); 
      }


      SignalPdf[sig]->plotOn(massframe,LineColor(2));
     
    }
  }
  Canvas.Clear(); 
  massframe->Draw();  
  Canvas.SetLogy(0);
  Canvas.Print(filename);  
  Canvas.SetLogy(0);
  delete massframe;






  //---------------------------
  //The Background: 
  //---------------------------
  cout<<setprecision(6); 

  RooAbsPdf* BkgPdf=NULL;
  if(bkgname!="0"){
 
    if(bkgtype==5){  
      Float_t X0=0,X1=0,Y0=0,Y1=0;
      if(HDstarPiFit->GetYaxis()->GetNbins()>1){//doesnt work, need to fix 2D boundary
	X0=HDstarPiFit->GetXaxis()->GetBinCenter(2);
	X1=HDstarPiFit->GetXaxis()->GetBinCenter(HDstarPiFit->GetNbinsX()-1);	
	for(Int_t i=1;i<HDstarPiFit->GetYaxis()->GetNbins();i++){
	  Y0+=(HDstarPiFit->GetBinContent(1,i)+HDstarPiFit->GetBinContent(2,i)+HDstarPiFit->GetBinContent(3,i))/(3.*HDstarPiFit->GetXaxis()->GetBinWidth(1)*HDstarPiFit->GetYaxis()->GetNbins()*HDstarPiFit->GetYaxis()->GetBinWidth(1));
	  Y1+=(HDstarPiFit->GetBinContent(HDstarPiFit->GetNbinsX(),i)+HDstarPiFit->GetBinContent(HDstarPiFit->GetNbinsX()-1,i)+HDstarPiFit->GetBinContent(HDstarPiFit->GetNbinsX()-2,i))/(3.*HDstarPiFit->GetXaxis()->GetBinWidth(1)*HDstarPiFit->GetYaxis()->GetNbins()*HDstarPiFit->GetYaxis()->GetBinWidth(1));
	}
      }else {
	X0=HDstarPiFit->GetBinCenter(2);
	Y0=(HDstarPiFit->GetBinContent(1)+HDstarPiFit->GetBinContent(2)+HDstarPiFit->GetBinContent(3))/(3.*HDstarPiFit->GetBinWidth(1));        X1=HDstarPiFit->GetBinCenter(HDstarPiFit->GetNbinsX()-1);
	Y1=(HDstarPiFit->GetBinContent(HDstarPiFit->GetNbinsX())+HDstarPiFit->GetBinContent(HDstarPiFit->GetNbinsX()-1)+HDstarPiFit->GetBinContent(HDstarPiFit->GetNbinsX()-2))/(3.*HDstarPiFit->GetBinWidth(1));
      }
      
      RooRealVar* x0=new RooRealVar("x0","x0",X0);
      RooRealVar* y0=new RooRealVar("y0","y0",Y0);
      RooRealVar* x1=new RooRealVar("x1","x1",X1);
      RooRealVar* y1=new RooRealVar("y1","y1",Y1);
      
      BkgPdf=new RooPolynomialFixEdge("BkgPdf","BkgPdf",mass,*x0,*y0,*x1,*y1,*BkgYield,*coef[2],*coef[3],*coef[4],*coef[5],*coef[6],*coef[7],*coef[8],*coef[9]);

    }else{
      TString BkgWS2=GetBackgroundFormula(bkgtype);
      if(BkgWS2=="0"){cout<<"No Background formula"<<endl;     delete HDstarPiFit; return 0;}
      BkgPdf=((RooAbsPdf*)new RooGenericPdfN("BkgPdf",BkgWS2,coeflist));
    }
    
    //multiply helicity
    if(HDstarPiFit->GetYaxis()->GetNbins()>1){   
      if(bkgtype==5){    	
	RooAbsPdf*bkghelipdf=new RooGenericPdfN("bkghelipdf","bkghelipdf",
						"1+(bkgheli1+bkgheli2*mass+bkgheli3*mass*mass)*helicity+(bkgheli4+bkgheli5*mass+bkgheli6*mass*mass)*helicity*helicity",RooArgSet(helicity,mass,*bkgheli[0],*bkgheli[1],*bkgheli[2],*bkgheli[3],*bkgheli[4],*bkgheli[5])); 
	BkgPdf=new RooProdPdfN(TString(BkgPdf->GetName())+"_Hel",TString(BkgPdf->GetName())+"_Hel",*bkghelipdf,*BkgPdf);
      }if(bkgtype==4){   	
	RooAbsPdf*bkghelipdf=new RooGenericPdfN("bkghelipdf","bkghelipdf","(1+coef6*mass+coef7*mass*mass+coef8*mass*mass*mass+coef9*mass*mass*mass*mass)*helicity*helicity",RooArgSet(helicity,mass,*coef[5],*coef[6],*coef[7],*coef[8])); 

	BkgPdf=new RooAddPdf(TString(BkgPdf->GetName())+"_Hel",TString(BkgPdf->GetName())+"_Hel",*bkghelipdf,*BkgPdf,*coef[4]);
 	
      } else {
	RooAbsPdf*bkghelipdf=new RooGenericPdfN("bkghelipdf","bkghelipdf",
						"1+bkgheli1*helicity+bkgheli2*helicity*helicity",
						RooArgSet(helicity,*bkgheli[0],*bkgheli[1])); 
	BkgPdf=new RooProdPdfN(TString(BkgPdf->GetName())+"_Hel",TString(BkgPdf->GetName())+"_Hel",*bkghelipdf,*BkgPdf);
	
      }	
    }
    //multiply phase space
    if(applybkgps)
      BkgPdf=new RooProdPdfN(TString(BkgPdf->GetName())+"_PS",TString(BkgPdf->GetName())+"_PS",TwoBodyPS,*BkgPdf);


  
    cout<<"Values from pdf:"<<endl;
    mass.setVal(HDstarPiFit->GetBinCenter(2));      
    cout<<" "<<mass.getVal()<<" PDF "<<BkgPdf->getVal()*HDstarPiFit->GetBinWidth(1)<<" Histo "<<HDstarPiFit->GetBinContent(2)<<endl;
    mass.setVal(HDstarPiFit->GetBinCenter(HDstarPiFit->GetNbinsX()-1));
    cout<<" "<<mass.getVal()<<" PDF "<<BkgPdf->getVal()*HDstarPiFit->GetBinWidth(1)<<" Histo "<<HDstarPiFit->GetBinContent(HDstarPiFit->GetNbinsX()-1)<<endl;
  
  }
//   Canvas.Clear();
//   massframe=mass.frame(fitmin,fitmax);
//   BkgPdf->plotOn(massframe,RooFit::Normalization(BkgYield->getVal()));
//   massframe->Draw();  
//   Canvas.Print(filename);  
  // Canvas.Print(filename+"]");  
  //return 0;


  //--------------------------------------------------------
  ////construct the total  pdf: signal plus background
  //-------------------------------------------------------
  RooArgList pdflist;
  RooArgList yieldlist;
 
  for(Int_t sig=0;sig<NSIGNALS;sig++){
    if( signame[sig]!="0"){
      pdflist.add(*SignalPdf[sig]);
      yieldlist.add(*sigYield[sig]);
    }
  }

  if(bkgname!="0")pdflist.add(*BkgPdf); 
  if(bkgname!="0")yieldlist.add(*BkgYield);

  RooAbsPdf* totalpdf=new RooAddPdfN("totalpdf","totalpdf",pdflist,yieldlist);


  //--------------------------------------------------------
  //create binned dataset  //
  //--------------------------------------------------------
  RooArgList datavars;
  datavars.add(mass);
  if(HDstarPiFit->GetYaxis()->GetNbins()>1)
    datavars.add(helicity);  

  RooDataHist binneddata("binneddata","binneddata",datavars,HDstarPiFit);
  
  Canvas.Clear();  
  massframe=mass.frame();
  binneddata.plotOn(massframe,RooFit::DataError(RooAbsData::SumW2));
  massframe->Draw();
  Canvas.Print(filename);
  delete massframe;



  //---------------------
  //************Make the fit********************
  //---------------------
  //RooFitResult* res=totalpdf->fitTo(binneddata,"mhert");//m=MIGRAD,h=HESSE,e=EXTENTED,r=Results,t=time: "mhert"   
  RooAbsOptTestStatistic* chi2=NULL;  
  if(TString(HDstarPiFit->GetTitle())==TString("unweighted")){//
    ///Gives correct result for small statistics, Needs to be used only with unweighted data.
    //Note calling after chi2 because chi2 is more robust
    cout<<" calling RooNLLVar1D for unweighted histogram------------------------------"<<endl<<endl;
    //(const char *name, const char *title, RooAbsPdf& pdf, RooAbsData& data,const TH1* HEfficiency,Bool_t extended, const char* rangeName, const char* addCoefRangeName,Int_t nCPU, Bool_t verbose, Bool_t splitRange)
    chi2=new RooNLLVar1D("nll","nll",*totalpdf,binneddata,HEfficiency,0,0,0,ncpu,0,0);
    //RooNLLVar1D nll("nll","nll",*totalpdf,binneddata,effpdf,0,0,0,2,0,0);
    //nll.InitEff();
  }else{
    //  if(TString(HDstarPiFit->GetTitle())==TString("weighted")){//
    if(HDstarPiFit->GetXaxis()->GetBinWidth(1)<.005)
      chi2=new RooChi2Var("chi2","chi2",*totalpdf,binneddata,RooFit::DataError(RooAbsData::SumW2),RooFit::NumCPU(ncpu));
    else 
      chi2=new RooChi2VarIntegral("chi2","chi2",*totalpdf,binneddata,RooFit::DataError(RooAbsData::SumW2),RooFit::NumCPU(ncpu));
  }

  RooMinuit minuit(*chi2);
  minuit.migrad(); 
  
  //minuit.seek();//takes forever
  //minuit.improve();
  //minuit.hesse();
  //minuit.minos();	
  RooFitResult* res=minuit.save(); 
  if(!res){
    cout<<"no results"<<endl;
    delete HDstarPiFit;
    delete chi2;
    
    return 0;
  }
//   if(res->status()!=0){//sometimes creates nan results 
//     cout<<" Warning: Calling Migrad 2 times status()!=0"<<endl; 
//     minuit.migrad();//call migrad again to improve   
//   }
 
  if(res->status()!=0){
    cout<<"-----------------Fit Status!=0------------------------"<<endl; 
    cout<<" status="<<res->status()<<endl<<endl<<endl;  
  }
  
 
  //----------------------------------
  //extract the floated parameters
  //----------------------------------
  RooArgList* floatedParameters= (RooArgList*)&res->floatParsFinal();    
  if(!floatedParameters){
    cout<<"no floated pars"<<endl;
    delete HDstarPiFit;
    delete res;
    delete chi2;
    
    return 0;
  }
 
   
//   if(effpdf){//screws up pdf
//     totalpdf=new RooProdPdf(TString(totalpdf->GetName())+"_Eff",TString(totalpdf->GetName())+"_Eff",*effpdf,*totalpdf);
//     totalpdf->attachDataSet(binneddata);
//   }


  HDstarPiFit->SetTitle("");
  //----------------------------
  //the Fit plot
  //----------------------------
  massframe=mass.frame(fitmin,fitmax);
  massframe->SetTitle("");
  massframe->GetXaxis()->SetTitle("m(D*#pi)   GeV/c^{2}");
  sprintf(textlabel,"Entries / %.1f MeV/c^{2}",1000*(fitmax-fitmin)/nfitbins);
  massframe->GetYaxis()->SetTitle(textlabel);
  binneddata.plotOn(massframe,RooFit::DataError(RooAbsData::SumW2));
  totalpdf->plotOn(massframe,LineColor(4));
  if(WhichHisto==5){
    RooPlot* othermassframe=mass.frame(fitmin,fitmax);
    othermassframe->SetTitle("");
    othermassframe->GetXaxis()->SetTitle("m(D*#pi)   GeV/c^{2}");
    for(Int_t sig=0;sig<NSIGNALS;sig++){
      if( signame[sig]!="0"){
	//totalpdf->plotOn(othermassframe,Components(*SignalPdf[sig]),LineStyle(kSolid),LineColor(1),Normalization(fabs(sigYield[sig]->getVal())));
	SignalPdf[sig]->plotOn(othermassframe,LineStyle(kSolid),LineColor(1),Normalization(sigYield[sig]->getVal()));
      }
    }
    Canvas.Clear();
    othermassframe->Draw();
    Canvas.Print(filename);  
  }else {
    if(bkgname!="0")totalpdf->plotOn(massframe,Components(*BkgPdf),LineStyle(kDashed),LineColor(1));
    for(Int_t sig=0;sig<NSIGNALS;sig++){
      if( signame[sig]!="0")
	totalpdf->plotOn(massframe,Components(*SignalPdf[sig]),LineStyle(kSolid),LineColor(1));
    }
  }
  Canvas.Clear();
  massframe->Draw();
  Canvas.Print(filename);
 
  if(HDstarPiFit->GetYaxis()->GetNbins()>1){
    RooPlot*helicityframe=helicity.frame();
    helicityframe->SetTitle("");
    helicityframe->GetXaxis()->SetTitle("cos(#theta_{H})");
    sprintf(textlabel,"Entries / %.2f",(helimax-helimin)/nhelibins);
    helicityframe->GetYaxis()->SetTitle(textlabel);
    binneddata.plotOn(helicityframe,RooFit::DataError(RooAbsData::SumW2));
    totalpdf->plotOn(helicityframe,LineColor(4));
    if(bkgname!="0")totalpdf->plotOn(helicityframe,Components(*BkgPdf),LineStyle(kDashed),LineColor(1));
    for(Int_t sig=0;sig<NSIGNALS;sig++){
      if( signame[sig]!="0")
	totalpdf->plotOn(helicityframe,Components(*SignalPdf[sig]),LineStyle(kSolid),LineColor(1));
    }
    Canvas.Clear();
    helicityframe->Draw();
    Canvas.Print(filename);
    delete helicityframe;
  }


  //----------------------------
  //now make the residual plot
  //----------------------------
  Float_t chisquare=0; 
  Int_t ndof=1;
  TH1* HFitDstPiDM=NULL;
  if(HDstarPiFit->GetYaxis()->GetNbins()>1){
    TH2F*H2FitDstPi=(TH2F*)HDstarPiFit->Clone();
    H2FitDstPi->Reset();
    totalpdf->fillHistogram(H2FitDstPi,RooArgList(mass,helicity),HDstarPiFit->Integral(),0);
    H2FitDstPi->Add(HDstarPiFit,-1);
    for(Int_t i=1;i<=H2FitDstPi->GetXaxis()->GetNbins();i++)
      for(Int_t j=1;j<=H2FitDstPi->GetYaxis()->GetNbins();j++)
	H2FitDstPi->SetBinContent(i,j,H2FitDstPi->GetBinContent(i,j)/H2FitDstPi->GetBinError(i,j));
    Canvas.Clear();
    H2FitDstPi->Draw("colz");
    Canvas.Print(filename);
    
    TH1F HResProjec("HResProjec","",100,-5,5);
    for(Int_t i=1;i<=H2FitDstPi->GetXaxis()->GetNbins();i++)
      for(Int_t j=1;j<=H2FitDstPi->GetYaxis()->GetNbins();j++)
	HResProjec.Fill(H2FitDstPi->GetBinContent(i,j));
    Canvas.Clear();
    HResProjec.Fit("gauss");
    HResProjec.Draw("pe");
    Canvas.Print(filename);

    HDstarPiFit=(TH1*)((TH2F*)HDstarPiFit)->ProjectionX();//make mass projection for  1D residuals
    HFitDstPiDM=(TH1*)HDstarPiFit->Clone();//for 1D fits and projections
    HFitDstPiDM->Reset();
    totalpdf->fillHistogram(HFitDstPiDM,RooArgList(mass),HDstarPiFit->Integral(),new RooArgSet(helicity));
  }else {
    HFitDstPiDM=totalpdf->createHistogram("HFitDstPiDM",mass);
    HFitDstPiDM->Scale(HDstarPiFit->Integral());
  }

  if(HFitDstPiDM==NULL||HFitDstPiDM->Integral()<1){
    cout<<"No HDstPiDM  returned."<<endl;
    Canvas.Print(filename+"]");
    delete HDstarPiFit;
    delete res;
    delete chi2;
    
    delete massframe;
    return 0;
  }
 
  JResiduals roofitresiduals("res",HDstarPiFit,HFitDstPiDM);


  Canvas.Clear();
  HDstarPiFit->SetStats(0);
  roofitresiduals.MakeFitPlotWithResiduals(&Canvas,massframe);
  Canvas.Print(filename); 


  TH1F* HChiVsBin=roofitresiduals.GetChisquareHisto();    
  Canvas.Clear();
  HChiVsBin->Draw("p");
  Canvas.Print(filename); 
  delete HChiVsBin;  

  TH1F* HResProj=roofitresiduals.GetNormResidualProjection();    
  Canvas.Clear();
  FitHistogram1gaus(HResProj,-5,5);
  HResProj->Draw("pe");
  Canvas.Print(filename);
  delete HResProj;

  chisquare=roofitresiduals.GetChisquare(); 
  ndof=HDstarPiFit->GetXaxis()->GetNbins()- floatedParameters->getSize();

  cout<<"Chisquare per DF= "<<setprecision(5)<<chisquare<<"/"<<ndof<<"="<<setprecision(5)<<chisquare/ndof<<endl;

  Bool_t badfit=0;
  if(chisquare/ndof>5.0){
    cout<<"-----------------Fit Chisquare > 3.0------------------------"<<endl<<endl<<endl; 
    badfit=1;
  }

  //---------------------------------
  //save the parameters to a file 
  //----------------------------------
  ofstream ParameterOutput;
  ParameterOutput.open((const char*)(_OutputDir+"/FitPars"+roofitfileid+".txt"));   
  ParameterOutput<<"Range"<<" "<<"min"<<" "<<fitmin<<endl;
  ParameterOutput<<"Range"<<" "<<"max"<<" "<<fitmax<<endl;
  ParameterOutput<<"Range"<<" "<<"nbins"<<" "<<nfitbins<<endl;
  ParameterOutput<<"Range"<<" "<<"chisquare"<<" "<<chisquare<<endl;
  ParameterOutput<<"Range"<<" "<<"ndf"<<" "<<ndof<<endl;
  ParameterOutput<<"Range"<<" "<<"status"<<" "<<res->status()<<endl;

  const RooArgSet massset(mass);

  for(Int_t sig=0;sig<NSIGNALS;sig++){
    if(signame[sig]!="0"){
      ParameterOutput<<" "<<endl;
      ParameterOutput<<signame[sig]<<" "<<"yield"<<setprecision(5)<<" "<<(int)sigYield[sig]->getVal()<<endl;
      ParameterOutput<<signame[sig]<<" "<<"yielderror"<<setprecision(5)<<" "<<(int)sigYield[sig]->getError()<<endl;  
      // ParameterOutput<<(int)sigYield[sig]->getAsymErrorHi()<<" "<<(int)sigYield[sig]->getAsymErrorLo()<<endl;    
      
      ParameterOutput<<signame[sig]<<" "<<"mean"<<setprecision(6)<<" "<<mean[sig]->getVal()<<endl;
      ParameterOutput<<signame[sig]<<" "<<"meanerror"<<setprecision(3)<<" "<<mean[sig]->getError()<<endl; 
   
      ParameterOutput<<signame[sig]<<" "<<"width"<<setprecision(6)<<" "<<width[sig]->getVal()<<endl;
      ParameterOutput<<signame[sig]<<" "<<"widtherror"<<setprecision(3)<<" "<<width[sig]->getError()<<endl; 
      
      mass.setVal(mean[sig]->getVal());
      ParameterOutput<<signame[sig]<<" "<<"height"<<setprecision(3)<<" "<<sigYield[sig]->getVal()*SignalPdf[sig]->getVal(&massset)*HDstarPiFit->GetXaxis()->GetBinWidth(1)<<endl;
      
      mass.setVal((fitmax-fitmin)/2);
      if(signame[sig]=="D2420")mass.setVal(2.460);
      if(signame[sig]=="D2460")mass.setVal(2.420);
      ParameterOutput<<signame[sig]<<" "<<"height2"<<setprecision(3)<<" "<<sigYield[sig]->getVal()*SignalPdf[sig]->getVal(&massset)*HDstarPiFit->GetXaxis()->GetBinWidth(1)<<endl;

      cout<<signame[sig]<<setfill(' ')<<setprecision(5)<<" "<<(int)sigYield[sig]->getVal()<<" "<<setprecision(5)<<" "<<(int)sigYield[sig]->getError()<<endl;
      cout<<setprecision(6)<<" "<<mean[sig]->getVal()*1000<<" "<<setprecision(2)<<"+-"<<mean[sig]->getError()*1000<<endl;
      cout<<setprecision(4)<<" "<<width[sig]->getVal()*1000<<" "<<setprecision(2)<<"+-"<<width[sig]->getError()*1000<<endl;  
      
    }
  }

  if(bkgname!="0"){
    ParameterOutput<<" "<<endl;
    ParameterOutput<<bkgname<<" "<<"yield"<<setfill(' ')<<setprecision(5)<<" "<<(int)BkgYield->getVal()<<endl;
    ParameterOutput<<bkgname<<" "<<"yielderror"<<setprecision(5)<<" "<<(int)BkgYield->getError()<<endl;
    for(Int_t i=0;i<BkgPolNpowers;i++){
      ParameterOutput<<bkgname<<" "<<"coef"<<i<<setprecision(5)<<" "<<coef[i]->getVal()<<endl;
      ParameterOutput<<bkgname<<" "<<"coef"<<i<<"error"<<setprecision(5)<<" "<<coef[i]->getError()<<endl;
    }
    
    mass.setVal((fitmax+fitmin)/2);
    ParameterOutput<<bkgname<<" "<<"height"<<setprecision(3)<<" "<<BkgYield->getVal()*BkgPdf->getVal(&massset)*HDstarPiFit->GetXaxis()->GetBinWidth(1)<<endl;

  }

  ParameterOutput<<"END"<<endl;
  ParameterOutput.close();
  ParameterOutput.clear();

  
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

  text.SetTextColor(1);
  for(Int_t sig=0;sig<NSIGNALS;sig++){
    if( signame[sig]!="0"){
      sprintf(textlabel,"%s: Yield= %d +- %d",(const char*)signame[sig],(int)sigYield[sig]->getVal(),(int)sigYield[sig]->getError());
      text.DrawTextNDC(.001,.85-sig*.05,TString(textlabel));

      sprintf(textlabel,"Mean= %.2f +- %.2f",mean[sig]->getVal()*1000,mean[sig]->getError()*1000);
      text.DrawTextNDC(.265,.85-sig*.05,TString(textlabel));

      sprintf(textlabel,"Width= %.2f +- %.2f",width[sig]->getVal()*1000,width[sig]->getError()*1000);
      text.DrawTextNDC(.5,.85-sig*.05,TString(textlabel));

      text.DrawTextNDC(.7,.85-sig*.05,TString("")
		       +" shape="+(long)sigshape[sig]
		       +" phase="+(long)applyps[sig]
		       +" res="+(long)applyres[sig]
	               +" pstar="+(long)applypfctr[sig]);

    }
  }

  if(bkgname!="0"){
    sprintf(textlabel,"Background Yield= %d +- %d ",(int)BkgYield->getVal(),(int)BkgYield->getError());
    text.DrawTextNDC(.01,.35,textlabel);
    
    Float_t textstep=.03;
    Int_t nfl=0;
    for(Int_t i=0;i<BkgPolNpowers;i++){
      sprintf(textlabel,TString(coef[i]->GetName())+"= %.2e +- %.2e",coef[i]->getVal(),coef[i]->getError());
      text.DrawTextNDC(.01,.35-(nfl+1)*textstep,textlabel);
      nfl++;
      
    }
  }    

  sprintf(textlabel,"Chisqure/ndof = %.1f/%d ",chisquare,ndof);
  text.DrawTextNDC(.4,.0,textlabel);
  Canvas.Print(filename);

 

  Canvas.Print(filename+"]");

 
  delete HDstarPiFit;
  delete HFitDstPiDM;
  delete res;
  delete chi2;
  
  
 
  if(badfit){  delete massframe; return 0;}
  return (TObject*)massframe;

}

TH1F* DstPiAnalysis::GetResolutionHisto(TString name){       

  TString fname="0";
  if(_D0Mode==1){
    if(name=="D2420"){
      fname="DstarCharged/DstarPi/MC/D2420/Kpi/ResolutionEff.root";
     }
    if(name=="D2460"){
      fname="DstarCharged/DstarPi/MC/D2460/Kpi/ResolutionEff.root";
     }
  }

  if(_D0Mode==2){
    if(name=="D2420"){
      fname="DstarCharged/DstarPi/MC/D2420/K3pi/ResolutionEff.root";
     }
    if(name=="D2460"){
      fname="DstarCharged/DstarPi/MC/D2460/K3pi/ResolutionEff.root";
     }
  }   
  if(fname=="0")return 0;
  
  TFile File(fname,"read");
  gROOT->cd();
  return (TH1F*)(File.Get("HDstPiDMRes")->Clone());
}

TH1F* CleanAndRebinHisto(TH1F* oldh,Int_t nfitbins,Float_t fitmin,Float_t fitmax){
    
    TH1F* newh=new TH1F(oldh->GetName(),oldh->GetName(),nfitbins,fitmin,fitmax);
    if(fabs(newh->GetXaxis()->GetBinWidth(1)-oldh->GetXaxis()->GetBinWidth(1))>.00001){
      cout<<"Error in bin width: new"<<newh->GetXaxis()->GetBinWidth(1)<<"  old"<<oldh->GetXaxis()->GetBinWidth(1)<<endl; return 0;
    }
    Int_t j=1;
    for(Int_t i=1;i<=oldh->GetNbinsX();i++){
      if(fabs(oldh->GetXaxis()->GetBinCenter(i)-newh->GetXaxis()->GetBinCenter(j))<.00001&&j<=nfitbins){
	newh->SetBinContent(j,(int)(oldh->GetBinContent(i)));
	newh->SetBinError(j,(int)(oldh->GetBinError(i)));
	j++;
      }
    }
    if((j-1)!=nfitbins){
      cout<<"Error in rebinning"<< (j-1)<<"!= nfitbins="<<nfitbins<<endl;
      return 0;
    }


  return newh;
}

TH1* DstPiAnalysis::GetMassHistoForFit(Int_t whichone,Int_t nfitbins,Float_t fitmin,Float_t fitmax){

  TFile* HistFile=NULL;
  TH1* HDstarPi=NULL;

  if(whichone==0){
    HDstarPi=HMassFit;  
    if(!HDstarPi)return 0;
    if(fabs(HDstarPi->Integral())<100){cout<<"Histo Integral<100"<<endl;return 0;}
    HDstarPi=CreateRebinnedDstPiHisto((TH1*)HDstarPi,nfitbins,fitmin,fitmax);
    if(!HDstarPi)return 0;
  }

  if(whichone==1){
    HistFile=new TFile(_OutputDir+"/LegendreWeightedHistos.root","read");
    if(!HistFile)return 0;
    gROOT->cd();

    HDstarPi=(TH1*)HistFile->Get("HDstPiMNoEff");    
    if(!HDstarPi)return 0;
    if(fabs(HDstarPi->Integral())<100){cout<<"Histo Integral<100"<<endl;return 0;}
    HDstarPi=CreateRebinnedDstPiHisto((TH1*)HDstarPi,nfitbins,fitmin,fitmax);
    if(!HDstarPi)return 0;
    HDstarPi->SetTitle("unweighted");
  }
  if(whichone==2){
    HistFile=new TFile(_OutputDir+"/LegendreWeightedHistos.root","read");
    if(!HistFile)return 0;
    gROOT->cd();

    HDstarPi=(TH1*)HistFile->Get("HDstPiMFine");
    if(!HDstarPi)return 0;   
    if(fabs(HDstarPi->Integral())<100){cout<<"Histo Integral<100"<<endl;return 0;}
    HDstarPi=CreateRebinnedDstPiHisto(HDstarPi,nfitbins,fitmin,fitmax);
    if(!HDstarPi)return 0;
    HDstarPi->SetTitle("weighted");
  }
  if(whichone==3){
    HistFile=new TFile(_OutputDir+"/LegendreWeightedHistos.root","read");
    if(!HistFile)return 0;
    gROOT->cd();

    HDstarPi=(TH1*)HistFile->Get("HDstPiMD2460Helicity");    
    if(!HDstarPi)return 0;
    if(fabs(HDstarPi->Integral())<100){cout<<"Histo Integral<100"<<endl;return 0;}
    HDstarPi=CreateRebinnedDstPiHisto((TH1*)HDstarPi,nfitbins,fitmin,fitmax);
    if(!HDstarPi)return 0;
    HDstarPi->SetTitle("weighted");
  }
  if(whichone==4){
    HistFile=new TFile(_OutputDir+"/LegendreWeightedHistos.root","read");
    if(!HistFile)return 0;
    gROOT->cd();

    HDstarPi=(TH1*)HistFile->Get("HDstPiMD2420Helicity");    
    if(!HDstarPi)return 0;
    if(fabs(HDstarPi->Integral())<100){cout<<"Histo Integral<100"<<endl;return 0;}
    HDstarPi=CreateRebinnedDstPiHisto((TH1*)HDstarPi,nfitbins,fitmin,fitmax);
    if(!HDstarPi)return 0;
    HDstarPi->SetTitle("weighted");
  }

  if(whichone==5){
    HistFile=new TFile(_OutputDir+"/LegendreWeightedHistos.root","read");
    if(!HistFile)return 0;
    gROOT->cd();

    HDstarPi=(TH1*)HistFile->Get("HDstPiMLegendre2");
    if(!HDstarPi)return 0;
    if(fabs(HDstarPi->Integral())<100){cout<<"Histo Integral<100"<<endl;return 0;}
    HDstarPi=CreateRebinnedDstPiHisto((TH1*)HDstarPi,nfitbins,fitmin,fitmax);
    if(!HDstarPi)return 0;
    HDstarPi->SetTitle("weighted");
  }
  if(whichone==6){//the original generated
    if(_MCorDATA!=0||!_TruthMatch)return 0;//generated only Truth-matched
    HistFile=new TFile(_OutputDir+"/Reduced.root","read");
    if(!HistFile)return 0;
    gROOT->cd();

    HDstarPi=(TH1*)HistFile->Get("HMCXMass");
    //if(!HDstarPi)   
    //HDstarPi->Smooth(20);//discretization is 
    //HDstarPi=(TH1*)HistFile->Get("HMCXMassDiff");
    if(!HDstarPi)return 0;   
    if(fabs(HDstarPi->Integral())<100){cout<<"Histo Integral<100"<<endl;return 0;}
    HDstarPi=CreateRebinnedDstPiHisto(HDstarPi,nfitbins,fitmin,fitmax);
    if(!HDstarPi)return 0;   
    HDstarPi->Smooth(1);
    HDstarPi->SetTitle("unweighted");
  }
  if(whichone==7){//MC with  resolution removed
    if(_MCorDATA!=0||!_TruthMatch)return 0;//resolution removed only in Truth-matched
    HistFile=new TFile(_OutputDir+"/LegendreWeightedHistos.root","read");
    if(!HistFile)return 0;
    gROOT->cd();

    HDstarPi=(TH1*)HistFile->Get("HDstPiMFineNoRes");
    if(!HDstarPi)return 0;   
    if(fabs(HDstarPi->Integral())<100){cout<<"Histo Integral<100"<<endl;return 0;}
    HDstarPi=CreateRebinnedDstPiHisto(HDstarPi,nfitbins,fitmin,fitmax);
    if(!HDstarPi)return 0;
    HDstarPi->SetTitle("unweighted");
  }
  if(whichone==8){
    if(_MCorDATA!=0||_TruthMatch)return 0;//allow to use only from ccbar
    if(_D0Mode==1)
      HistFile=new TFile("DstarCharged/DstarPi/MC/SP-1005/Kpi/CCBarBackground.root","read");
    if(_D0Mode==2)
      HistFile=new TFile("DstarCharged/DstarPi/MC/SP-1005/K3pi/CCBarBackground.root","read");
    if(!HistFile)return 0;
    gROOT->cd();
  
    HDstarPi=(TH1*)HistFile->Get("HDstPiMassNotTruthedFine");
    if(!HDstarPi)return 0;   
    if(fabs(HDstarPi->Integral())<100){cout<<"Histo Integral<100"<<endl;return 0;}
    HDstarPi=CreateRebinnedDstPiHisto(HDstarPi,nfitbins,fitmin,fitmax);
    if(!HDstarPi)return 0;
    HDstarPi->SetTitle("unweighted");
  }
   

  if(10<=whichone&&whichone<20){
    HistFile=new TFile(_OutputDir+"/LegendreWeightedHistos.root","read");
    if(!HistFile)return 0;
    gROOT->cd();

    HDstarPi=(TH1*)HistFile->Get(TString("HDstPiMVsHelicity")+(long)(whichone-10));
    if(!HDstarPi)return 0;
    if(fabs(HDstarPi->Integral())<100){cout<<"Histo Integral<100"<<endl;return 0;}
    HDstarPi=CreateRebinnedDstPiHisto(HDstarPi,nfitbins,fitmin,fitmax);
    //HDstarPi=CleanAndRebinHisto(HDstarPi,nfitbins,fitmin,fitmax); 
    if(!HDstarPi)return 0;
    HDstarPi->SetTitle("weighted");
  }

  if(20<=whichone&&whichone<25){
    HistFile=new TFile(_OutputDir+"/LegendreWeightedHistos.root","read");
    if(!HistFile)return 0;
    gROOT->cd();

    HDstarPi=(TH1*)HistFile->Get(TString("HDstPiMVsHelicityFolded")+(long)(whichone-20));
    if(!HDstarPi)return 0;
    if(fabs(HDstarPi->Integral())<100){cout<<"Histo Integral<100"<<endl;return 0;}
    HDstarPi=CreateRebinnedDstPiHisto(HDstarPi,nfitbins,fitmin,fitmax);
    //HDstarPi=CleanAndRebinHisto(HDstarPi,nfitbins,fitmin,fitmax);
    if(!HDstarPi)return 0;
    HDstarPi->SetTitle("weighted");
  }


  if(40<=whichone&&whichone<60){
    HistFile=new TFile(_OutputDir+"/LegendreWeightedHistos.root","read");
    if(!HistFile)return 0;
    gROOT->cd();

    HDstarPi=(TH1*)HistFile->Get(TString("HDstPiMVsHelicityFine")+(long)(whichone-40));
    if(!HDstarPi)return 0;
    if(fabs(HDstarPi->Integral())<100){cout<<"Histo Integral<100"<<endl;return 0;}
    HDstarPi=CreateRebinnedDstPiHisto(HDstarPi,nfitbins,fitmin,fitmax);
    //HDstarPi=CleanAndRebinHisto(HDstarPi,nfitbins,fitmin,fitmax);
    if(!HDstarPi)return 0;
    HDstarPi->SetTitle("weighted");
  }
 
  if(60<=whichone&&whichone<70){
    HistFile=new TFile(_OutputDir+"/LegendreWeightedHistos.root","read");
    if(!HistFile)return 0;
    gROOT->cd();

    HDstarPi=(TH1*)HistFile->Get(TString("HDstPiMVsHelicityFineFolded")+(long)(whichone-60));
    if(!HDstarPi)return 0;
    if(fabs(HDstarPi->Integral())<100){cout<<"Histo Integral<100"<<endl;return 0;}
    HDstarPi=CreateRebinnedDstPiHisto(HDstarPi,nfitbins,fitmin,fitmax);
    //HDstarPi=CleanAndRebinHisto(HDstarPi,nfitbins,fitmin,fitmax);
    if(!HDstarPi)return 0;
    HDstarPi->SetTitle("weighted");
  }
  if(70<=whichone&&whichone<80){
    HistFile=new TFile(_OutputDir+"/LegendreWeightedHistos.root","read");
    if(!HistFile)return 0;
    gROOT->cd();

    HDstarPi=(TH1*)HistFile->Get(TString("HDstPiMVsPstarFine")+(long)(whichone-70));
    if(!HDstarPi)return 0;
    if(fabs(HDstarPi->Integral())<100){cout<<"Histo Integral<100"<<endl;return 0;}
    HDstarPi=CreateRebinnedDstPiHisto(HDstarPi,nfitbins,fitmin,fitmax);
    //HDstarPi=CleanAndRebinHisto(HDstarPi,nfitbins,fitmin,fitmax);
    if(!HDstarPi)return 0;
    HDstarPi->SetTitle("weighted");
  }
  if(80<=whichone&&whichone<84){
    HistFile=new TFile(_OutputDir+"/LegendreWeightedHistos.root","read");
    if(!HistFile)return 0;
    gROOT->cd();

    HDstarPi=(TH1*)HistFile->Get(TString("HDstPiMVsPstarD2420")+(long)(whichone-80));
    if(!HDstarPi)return 0;
    if(fabs(HDstarPi->Integral())<100){cout<<"Histo Integral<100"<<endl;return 0;}
    HDstarPi=CreateRebinnedDstPiHisto(HDstarPi,nfitbins,fitmin,fitmax);
    if(!HDstarPi)return 0;
    HDstarPi->SetTitle("weighted");
  }
  if(84<=whichone&&whichone<88){
    HistFile=new TFile(_OutputDir+"/LegendreWeightedHistos.root","read");
    if(!HistFile)return 0;
    gROOT->cd();

    HDstarPi=(TH1*)HistFile->Get(TString("HDstPiMVsPstarD2460")+(long)(whichone-80));
    if(!HDstarPi)return 0;
    if(fabs(HDstarPi->Integral())<100){cout<<"Histo Integral<100"<<endl;return 0;}
    HDstarPi=CreateRebinnedDstPiHisto(HDstarPi,nfitbins,fitmin,fitmax);
    if(!HDstarPi)return 0;
    HDstarPi->SetTitle("weighted");
  }
//   if(85<=whichone&&whichone<89){//gen vs p
//     HistFile=new TFile(_OutputDir+"/LegendreWeightedHistos.root","read");
//     if(!HistFile)return 0;
//     gROOT->cd();

//     HDstarPi=(TH1*)HistFile->Get(TString("HMCMVsPstar")+(long)(whichone-85));
//     if(!HDstarPi)return 0;
//     if(fabs(HDstarPi->Integral())<100){cout<<"Histo Integral<100"<<endl;return 0;}
//     HDstarPi=CreateRebinnedDstPiHisto(HDstarPi,nfitbins,fitmin,fitmax);
//     if(!HDstarPi)return 0;
//     HDstarPi->SetTitle("unweighted");
//   }

  if(90<=whichone&&whichone<94){
    HistFile=new TFile(_OutputDir+"/LegendreWeightedHistos.root","read");
    if(!HistFile)return 0;
    gROOT->cd();

    HDstarPi=(TH1*)HistFile->Get(TString("HDstPiMVsPhiD2420")+(long)(whichone-90));
    if(!HDstarPi)return 0;
    if(fabs(HDstarPi->Integral())<100){cout<<"Histo Integral<100"<<endl;return 0;}
    HDstarPi=CreateRebinnedDstPiHisto(HDstarPi,nfitbins,fitmin,fitmax);
    if(!HDstarPi)return 0;
    HDstarPi->SetTitle("weighted");
  }
  if(94<=whichone&&whichone<98){
    HistFile=new TFile(_OutputDir+"/LegendreWeightedHistos.root","read");
    if(!HistFile)return 0;
    gROOT->cd();

    HDstarPi=(TH1*)HistFile->Get(TString("HDstPiMVsPhiD2460")+(long)(whichone-90));
    if(!HDstarPi)return 0;
    if(fabs(HDstarPi->Integral())<100){cout<<"Histo Integral<100"<<endl;return 0;}
    HDstarPi=CreateRebinnedDstPiHisto(HDstarPi,nfitbins,fitmin,fitmax);
    if(!HDstarPi)return 0;
    HDstarPi->SetTitle("weighted");
  }
  if(100<=whichone&&whichone<104){
    HistFile=new TFile(_OutputDir+"/LegendreWeightedHistos.root","read");
    if(!HistFile)return 0;
    gROOT->cd();

    HDstarPi=(TH1*)HistFile->Get(TString("HDstPiMVsCosD2420")+(long)(whichone-100));
    if(!HDstarPi)return 0;
    if(fabs(HDstarPi->Integral())<100){cout<<"Histo Integral<100"<<endl;return 0;}
    HDstarPi=CreateRebinnedDstPiHisto(HDstarPi,nfitbins,fitmin,fitmax);
    if(!HDstarPi)return 0;
    HDstarPi->SetTitle("weighted");
  }
  if(104<=whichone&&whichone<108){
    HistFile=new TFile(_OutputDir+"/LegendreWeightedHistos.root","read");
    if(!HistFile)return 0;
    gROOT->cd();

    HDstarPi=(TH1*)HistFile->Get(TString("HDstPiMVsCosD2460")+(long)(whichone-100));
    if(!HDstarPi)return 0;
    if(fabs(HDstarPi->Integral())<100){cout<<"Histo Integral<100"<<endl;return 0;}
    HDstarPi=CreateRebinnedDstPiHisto(HDstarPi,nfitbins,fitmin,fitmax);
    if(!HDstarPi)return 0;
    HDstarPi->SetTitle("weighted");
  }
  if(110<=whichone&&whichone<140){
    HistFile=new TFile(_OutputDir+"/LegendreWeightedHistos.root","read");
    if(!HistFile)return 0;
    gROOT->cd();

    HDstarPi=(TH1*)HistFile->Get(TString("HDstPiMVsEnergyFine")+(long)(whichone-110));
    if(!HDstarPi)return 0;
    if(fabs(HDstarPi->Integral())<100){cout<<"Histo Integral<100"<<endl;return 0;}
    HDstarPi=CreateRebinnedDstPiHisto(HDstarPi,nfitbins,fitmin,fitmax);
    if(!HDstarPi)return 0;
    HDstarPi->SetTitle("weighted");
  }
  if(140<=whichone&&whichone<160){
    HistFile=new TFile(_OutputDir+"/LegendreWeightedHistos.root","read");
    if(!HistFile)return 0;
    gROOT->cd();

    HDstarPi=(TH1*)HistFile->Get(TString("HDstPiMVsCosDstar")+(long)(whichone-140));
    if(!HDstarPi)return 0;
    if(fabs(HDstarPi->Integral())<100){cout<<"Histo Integral<100"<<endl;return 0;}
    HDstarPi=CreateRebinnedDstPiHisto(HDstarPi,nfitbins,fitmin,fitmax);
    if(!HDstarPi)return 0;
    HDstarPi->SetTitle("weighted");
  }

  if(160<=whichone&&whichone<170){
    HistFile=new TFile(_OutputDir+"/LegendreWeightedHistos.root","read");
    if(!HistFile)return 0;
    gROOT->cd();

    HDstarPi=(TH1*)HistFile->Get(TString("HDstPiMVsHelicityN5Cos")+(long)(whichone-160));
    if(!HDstarPi)return 0;
    if(fabs(HDstarPi->Integral())<100){cout<<"Histo Integral<100"<<endl;return 0;}
    HDstarPi=CreateRebinnedDstPiHisto(HDstarPi,nfitbins,fitmin,fitmax);
    if(!HDstarPi)return 0;
    HDstarPi->SetTitle("unweighted");
  }
  if(170<=whichone&&whichone<180){
    HistFile=new TFile(_OutputDir+"/LegendreWeightedHistos.root","read");
    if(!HistFile)return 0;
    gROOT->cd();

    HDstarPi=(TH1*)HistFile->Get(TString("HDstPiMVsHelicityN9Cos")+(long)(whichone-170));
    if(!HDstarPi)return 0;
    if(fabs(HDstarPi->Integral())<100){cout<<"Histo Integral<100"<<endl;return 0;}
    HDstarPi=CreateRebinnedDstPiHisto(HDstarPi,nfitbins,fitmin,fitmax);
    if(!HDstarPi)return 0;
    HDstarPi->SetTitle("unweighted");
  }

  if(180<=whichone&&whichone<188){
    HistFile=new TFile(_OutputDir+"/LegendreWeightedHistos.root","read");
    if(!HistFile)return 0;
    gROOT->cd();

    HDstarPi=(TH1*)HistFile->Get(TString("HDstPiMVsCosDstarMW")+(long)(whichone-180));
    if(!HDstarPi)return 0;
    if(fabs(HDstarPi->Integral())<100){cout<<"Histo Integral<100"<<endl;return 0;}
    HDstarPi=CreateRebinnedDstPiHisto(HDstarPi,nfitbins,fitmin,fitmax);
    if(!HDstarPi)return 0;
    HDstarPi->SetTitle("weighted");
  }

  if(190<=whichone&&whichone<198){
    HistFile=new TFile(_OutputDir+"/LegendreWeightedHistos.root","read");
    if(!HistFile)return 0;
    gROOT->cd();

    HDstarPi=(TH1*)HistFile->Get(TString("HDstPiMVsHelicityMW")+(long)(whichone-190));
    if(!HDstarPi)return 0;
    if(fabs(HDstarPi->Integral())<100){cout<<"Histo Integral<100"<<endl;return 0;}
    HDstarPi=CreateRebinnedDstPiHisto(HDstarPi,nfitbins,fitmin,fitmax);
    if(!HDstarPi)return 0;
    HDstarPi->SetTitle("weighted");
  }

  if(200<=whichone&&whichone<208){
    HistFile=new TFile(_OutputDir+"/LegendreWeightedHistos.root","read");
    if(!HistFile)return 0;
    gROOT->cd();

    HDstarPi=(TH1*)HistFile->Get(TString("HDstPiMVsCosDstarD2420MW")+(long)(whichone-200));
    if(!HDstarPi)return 0;
    if(fabs(HDstarPi->Integral())<100){cout<<"Histo Integral<100"<<endl;return 0;}
    HDstarPi=CreateRebinnedDstPiHisto(HDstarPi,nfitbins,fitmin,fitmax);
    if(!HDstarPi)return 0;
    HDstarPi->SetTitle("weighted");
  }
  if(210<=whichone&&whichone<218){
    HistFile=new TFile(_OutputDir+"/LegendreWeightedHistos.root","read");
    if(!HistFile)return 0;
    gROOT->cd();

    HDstarPi=(TH1*)HistFile->Get(TString("HDstPiMVsCosDstarD2460MW")+(long)(whichone-210));
    if(!HDstarPi)return 0;
    if(fabs(HDstarPi->Integral())<100){cout<<"Histo Integral<100"<<endl;return 0;}
    HDstarPi=CreateRebinnedDstPiHisto(HDstarPi,nfitbins,fitmin,fitmax);
    if(!HDstarPi)return 0;
    HDstarPi->SetTitle("weighted");
  }
  if(220<=whichone&&whichone<228){
    HistFile=new TFile(_OutputDir+"/LegendreWeightedHistos.root","read");
    if(!HistFile)return 0;
    gROOT->cd();

    HDstarPi=(TH1*)HistFile->Get(TString("HDstPiMP2VsCosDstar")+(long)(whichone-220));
    if(!HDstarPi)return 0;
    if(fabs(HDstarPi->Integral())<100){cout<<"Histo Integral<100"<<endl;return 0;}
    HDstarPi=CreateRebinnedDstPiHisto(HDstarPi,nfitbins,fitmin,fitmax);
    if(!HDstarPi)return 0;
    HDstarPi->SetTitle("weighted");
  }


  //-------------------------
  //2D fits
  //-------------------------  
  if(whichone==1000){
    HistFile=new TFile(_OutputDir+"/LegendreWeightedHistos.root","read");
    if(!HistFile)return 0;
    gROOT->cd();

    HDstarPi=(TH1*)HistFile->Get("H2DstPiMVsDstarHelNoEff");
    if(!HDstarPi)return 0;   
    if(fabs(HDstarPi->Integral())<100){cout<<"Histo Integral<100"<<endl;return 0;}
    HDstarPi=CreateRebinnedDstPiHisto(HDstarPi,nfitbins,fitmin,fitmax);
    if(!HDstarPi)return 0;
    HDstarPi->SetTitle("unweighted");
  }

  if(whichone==1001){
    HistFile=new TFile(_OutputDir+"/LegendreWeightedHistos.root","read");
    if(!HistFile)return 0;
    gROOT->cd();

    HDstarPi=(TH1*)HistFile->Get("H2DstPiMVsDstarHel");
    if(!HDstarPi)return 0;   
    if(fabs(HDstarPi->Integral())<100){cout<<"Histo Integral<100"<<endl;return 0;}
    HDstarPi=CreateRebinnedDstPiHisto(HDstarPi,nfitbins,fitmin,fitmax);
    if(!HDstarPi)return 0;
    HDstarPi->SetTitle("weighted");
  }




  if(!HDstarPi)return NULL;
  if(HistFile){cout<<"Histo  from file: "<<HistFile->GetName()<<endl; HistFile->Close();delete HistFile;}
  
  cout<<"Histogram Name: "<<HDstarPi->GetName()<<"  title="<<HDstarPi->GetTitle()<<endl;  
  
  return HDstarPi;
}




TH1* DstPiAnalysis::CreateRebinnedDstPiHisto(TH1* oldhisto,Int_t nbins,Float_t min,Float_t max){
  ///this assumues the oldhisto has 1MeV binning

  Float_t newbinsize=(max-min)/nbins;
  Int_t nnewrangebins=(int)floor(1000*(max-min)  +.999);
  cout<<"Histo Info: "<<oldhisto->GetNbinsX()<<" "<<oldhisto->GetXaxis()->GetXmin()<<" "<<oldhisto->GetXaxis()->GetXmax()<<" "<<(oldhisto->GetXaxis()->GetXmax()-oldhisto->GetXaxis()->GetXmin())/oldhisto->GetNbinsX()<<endl;
  cout<<"New Binning: "<<nbins<<" "<<min<<" "<<max<<" "<<newbinsize<<endl;
 
  if( floor(1000*newbinsize+.001)!=floor(1000*newbinsize+.999)){
    cout<<"newbinsize is not good"<<endl;
    return NULL;
  }
  if( floor(1000*min+.001)!=floor(1000*min+.999)){
    cout<<"min is not good"<<endl;
    return NULL;
  }
  if( floor(1000*max+.001)!=floor(1000*max+.999)){
    cout<<"max is not good"<<endl;
    return NULL;
  }  
  if(min>max){
    cout<<"min>max not good"<<endl;
    return NULL;
  }
  if((min+1e-6)<oldhisto->GetXaxis()->GetXmin()){
    cout<<"min<histomin not good"<<endl;
    return NULL;
  }    
  if((max-1e-6)>oldhisto->GetXaxis()->GetXmax()){
    cout<<"max>histomax not good"<<endl;
    return NULL;
  }


  TH1* rebinhisto=NULL;
  Int_t group=nnewrangebins/nbins;

  //first create a new histo with new range but same bin size
  TH1* newhisto=NULL;
  if(oldhisto->GetYaxis()->GetNbins()==1){
    newhisto=new TH1F(TString(oldhisto->GetName())+"rebin",TString(oldhisto->GetTitle()),nnewrangebins,min,max);//need retain title 

    Int_t bn=1;
    for(Int_t b=0;b<=oldhisto->GetNbinsX();b++){
      //cout<<nnewrangebins<<" "<<b<<" "<<bn<<" "<<oldhisto->GetBinCenter(b)<<" "<<newhisto->GetBinCenter(bn)<<" "<<fabs(oldhisto->GetBinCenter(b)-newhisto->GetBinCenter(bn))<<endl;
      if(fabs(oldhisto->GetXaxis()->GetBinCenter(b)-newhisto->GetXaxis()->GetBinCenter(bn))<oldhisto->GetXaxis()->GetBinWidth(1)/100 && bn<=nnewrangebins){
	newhisto->SetBinContent(bn,oldhisto->GetBinContent(b));
	if(oldhisto->GetBinError(b)<1)newhisto->SetBinError(bn,1);
	else newhisto->SetBinError(bn,oldhisto->GetBinError(b));
	bn++;
      }    
    }
    if((bn-1)!=nnewrangebins){
      cout<<"There was a problem in rebinning, bin numbers dont match."<<bn-1<<" "<<nnewrangebins<<endl;
      return NULL;
    }
  
    newhisto->Sumw2();    
    //now group bins
    rebinhisto=(TH1*)newhisto->Rebin(group,TString(oldhisto->GetName())+"rebinned"); 

  }

  if(oldhisto->GetYaxis()->GetNbins()>1){//2D histo
    newhisto=new TH2F(TString(oldhisto->GetName())+"rebin",TString(oldhisto->GetTitle()),nnewrangebins,min,max,oldhisto->GetYaxis()->GetNbins(),oldhisto->GetYaxis()->GetXmin(),oldhisto->GetYaxis()->GetXmax());
    
    for(Int_t by=1;by<=oldhisto->GetYaxis()->GetNbins();by++){
      
      Int_t bn=1;
      for(Int_t b=1;b<=oldhisto->GetNbinsX();b++){
	if(fabs(oldhisto->GetXaxis()->GetBinCenter(b)-newhisto->GetXaxis()->GetBinCenter(bn))<oldhisto->GetXaxis()->GetBinWidth(1)/100 && bn<=nnewrangebins){
	  newhisto->SetBinContent(bn,by,oldhisto->GetBinContent(b,by));
	  //if(oldhisto->GetBinError(b,)<1)newhisto->SetBinError(bn,by,1);
	  //else
	  newhisto->SetBinError(bn,by,oldhisto->GetBinError(b,by));
	  bn++;
	}    
      }
      
    }
    
    newhisto->Sumw2();    
    //now group bins
    rebinhisto=(TH1*)(((TH2F*)newhisto)->RebinX(group,TString(oldhisto->GetName())+"rebinned"));       
  
  }  
  
  delete newhisto;
  return rebinhisto;
}


TH2F* DstPiAnalysis::CreateRebinnedDstPiHisto2D(TH2* oldhisto,Int_t nbins,Float_t min,Float_t max,Int_t prevBinning){
  if(!oldhisto)return NULL;
  cout<<"Creating rebinned histo of "<<oldhisto->GetName()<<endl;
  
  Float_t newbinsize=(max-min)/nbins;
 
  if( floor(1000*newbinsize+.001)!=floor(1000*newbinsize+.999)){
    cout<<"newbinsize is not good"<<endl;
    return NULL;
  }
  if( floor(1000*min+.001)!=floor(1000*min+.999)){
    cout<<"min is not good"<<endl;
    return NULL;
  }
  if( floor(1000*max+.001)!=floor(1000*max+.999)){
    cout<<"max is not good"<<endl;
    return NULL;
  }  
  if(min>max){
    cout<<"min>max not good"<<endl;
    return NULL;
  }
  if(min<oldhisto->GetXaxis()->GetXmin()){
    cout<<"min<histomin not good"<<endl;
    return NULL;
  }    
  if(max>oldhisto->GetXaxis()->GetXmax()+.0001){
    cout<<"max>histomax not good"<<endl;
    return NULL;
  }


  Int_t nnewrangebins=((int)floor(1000*(max-min)  +.5))/prevBinning;
  Int_t newybins=oldhisto->GetYaxis()->GetNbins();
  Float_t newymin=oldhisto->GetYaxis()->GetXmin();
  Float_t newymax=oldhisto->GetYaxis()->GetXmax();
  TH2F* newhisto=new TH2F(TString(oldhisto->GetName())+"rebin",TString(oldhisto->GetName())+"rebin",nnewrangebins,min,max,newybins,newymin,newymax);


  //first create a new histo with new range but same binning 
  Int_t newbx=1;
  for(Int_t by=1;by<=oldhisto->GetNbinsY();by++){
    newbx=1;
    for(Int_t bx=1;bx<=oldhisto->GetNbinsX();bx++){
      
      //cout<<nnewrangebins<<" "<<bx<<" "<<newbx<<" "<<oldhisto->GetXaxis()->GetBinCenter(bx)<<" "<<newhisto->GetXaxis()->GetBinCenter(newbx)<<" "<<fabs(oldhisto->GetXaxis()->GetBinCenter(bx)-newhisto->GetXaxis()->GetBinCenter(newbx))<<endl;

      if(fabs(oldhisto->GetXaxis()->GetBinCenter(bx)-newhisto->GetXaxis()->GetBinCenter(newbx))
	 <oldhisto->GetXaxis()->GetBinWidth(1)/100
	 && newbx<=nnewrangebins){
	newhisto->SetBinContent(newbx,by,oldhisto->GetBinContent(bx,by));
	newhisto->SetBinError(newbx,by,oldhisto->GetBinError(bx,by));
	newbx++;
	
      }    
    }
    
  }

  if((newbx-1)!=nnewrangebins){
    cout<<"There was a problem in rebinning, bin numbers dont match."<<endl;
    return NULL;
  }

  
  //now group bins
  Int_t group=nnewrangebins/nbins;
  TH2F* rebinhisto=(TH2F*)newhisto->Rebin2D(group,1,TString(oldhisto->GetName())+"rebinned"); 
  
  //if(rebinhisto)
    //cout<<"Rebinned histo created successfully."<<endl;

  delete newhisto;
  return rebinhisto;
}




TString DstPiAnalysis::GetBackgroundFormula(Int_t type){
  
  //choose background type
  TString formula="0";

  if(type==1)//exponential(polynomial)
    formula="exp(coef1*mass+coef2*mass**2+coef3*mass**3+coef4*mass**4)";
  if(type==2)//
    formula="exp(coef1*mass) + coef2*exp(coef3*mass) + coef4*exp(coef5*mass)";
  if(type==3)//exponential+exponential
    formula="exp(coef1*mass)+coef2*exp(coef3*mass)"; 
  if(type==4)//polynomial
    formula="1+coef1*mass+coef2*mass**2+coef3*mass**3+coef4*mass**4";//+coef5*mass**5+coef6*mass**6+coef7*mass**7+coef8*mass**8
  
  cout<<" The background formula is: "<<formula<<endl;

  return formula;
}


// Int_t DstPiAnalysis::CreateDeltaMHistogramForFit(){
//   cout<<" Will create new histograms for later fitting"<<endl;
      
//   //------------------------------
//   //define the data to be fitted
//   //------------------------------  
//   if(!OpenReducedNtuple()) return 0;

     
//   //----------
//   //Plot the D* deltaM
//   //----------
//   TH1F HDstar("HDstar","HDstar",database.GetDstarDeltaMRangeNbins(),database.GetDstarDeltaMRangeMin(),database.GetDstarDeltaMRangeMax()); 
//   ReducedNtuple->Draw("dstardeltam>>HDstar");


//   TCut cut;
//   TCut lowcut;
  
//   TH1F* HDstarPi[8];
//   TH1F* HDstarPiSlice[8];
 
//   for(Int_t c=0;c<8;c++){   
//     sprintf(textlabel,"dstarcostheta<%.2f",1.-c*.25);
//     cut=TCut(textlabel);
//     sprintf(textlabel,"dstarcostheta>%.2f",1.-(c+1)*.25);
//     lowcut=TCut(textlabel);
    
//     HDstarPi[c]=new TH1F(TString("HDstarPi")+(long)c,TString("HDstarPi")+(long)c,1500,2.00,3.50);
//     sprintf(textlabel,"dstpideltam+%.5f",DstarPDGMass);
//     ReducedNtuple->Draw(TString(textlabel)+">>HDstarPi"+(long)c,cut);   

//     HDstarPiSlice[c]=new TH1F(TString("HDstarPiSlice")+(long)c,TString("HDstarPiSlice")+(long)c,1500,2.00,3.50);
//     ReducedNtuple->Draw(TString(textlabel)+">>HDstarPiSlice"+(long)c,cut+lowcut);    
   
//   }
//   //2-D Map
//   TH2F H2DstarPi("H2DstarPi","H2DstarPi",40,-1.001,1.001,1500,2.00,3.50);
//   ReducedNtuple->Draw("(dstpideltam+2.01):dstarcostheta>>H2DstarPi"); 


//   TFile File(_OutputDir+"/HistosForFit.root","recreate");
//   File.mkdir("RawHistos");  
//   File.cd("RawHistos");  

//   HDstar.Write();    
//   for(Int_t c=0;c<8;c++){   
//     HDstarPi[c]->Write();
//     HDstarPiSlice[c]->Write();
//   }
//   H2DstarPi.Write();

//   File.Write();  
//   File.ls();
//   File.Close();
  
//   for(Int_t c=0;c<8;c++){   
//     delete HDstarPi[c];
//     delete HDstarPiSlice[c];
//   }
  
//   if(CloseReducedNtuple()!=1) return 0;
  
//   cout<<" Done creating histograms for later fitting"<<endl;
//   return 1;
// }


// Int_t DstPiAnalysis::CreateNotTruthCCbarHistogramForFit(){
//   cout<<" Will create histo for later fitting"<<endl;

//   //------------------------------
//   //define the data to be fitted
//   //------------------------------  
//   if(!OpenReducedNtuple()) return 0;
  

//   //save histo for later fitting
//   TH1F HDstPiMassNotTruthedFine("HDstPiMassNotTruthedFine","",1300,2.1,3.4);
//   ReducedNtuple->Draw("dstpimdm>>HDstPiMassNotTruthedFine");
//   TFile File(_OutputDir+"/CCBarBackground.root","recreate");
//   HDstPiMassNotTruthedFine.Write();
//   cout<<HDstPiMassNotTruthedFine.Integral()<<endl;
//   File.ls();
//   File.Close();

//   if(CloseReducedNtuple()!=1) return 0;
  
//   cout<<" Done creating histograms for later fitting"<<endl;
//   return 1;
// }

Int_t DstPiAnalysis::ShowLineShapes(){


  //RooRealVar mass("mass","mass",-.1,.1);mass.setBins(200);
  RooRealVar mean("mean","mean",2.420);
  RooRealVar width("width","width",.03); 

  RooSwaveBreitWigner bw3("bw3","bw3",&mass,mean,width,&TwoBodyPS);  
  RooDwaveBreitWigner bw4("bw4","bw4",&mass,mean,width,&TwoBodyPS,radius);


  filename="LineShapeComparison.ps";
  Canvas.Print(filename+"[");    

  
  RooPlot*pl=mass.frame(2.320,2.520);  
  bw3.plotOn(pl,LineColor(1));
  //radius.setVal(3);
  //bw4.plotOn(pl,LineStyle(kDashed),LineColor(1));   
  //radius.setVal(4);
  bw4.plotOn(pl,LineStyle(kDashed),LineColor(1));   
  //radius.setVal(5);
  //bw4.plotOn(pl,LineStyle(kDashed),LineColor(1));   
  //radius.setVal(10);
  //bw4.plotOn(pl,LineStyle(kDashed),LineColor(1));   
  Canvas.Clear();
  pl->SetTitle("");
  pl->GetXaxis()->SetTitle(" x ");
  pl->GetYaxis()->SetTitle("");
  pl->Draw();
  Canvas.Print(filename);    
  delete pl;



  Canvas.Print(filename+"]");    
  return 1;
}


// Int_t DstPiAnalysis::FitReflection(){
//   cout<<" Will Fit Reflection"<<endl;

// //   filename=_OutputDir+"/ReflectionFit.ps";
// //   Canvas.Print(filename+"["); 
  

//   //------------------------------
//   //define the data to be fitted
//   //------------------------------  
//   if(!OpenReducedNtuple()) return 0;
  
//   //save histo for later fitting
//   TH1F HReflection("HReflection","",1300,2.1,3.4);
//   //ReducedNtuple->Draw("massKpipi1>>HReflection","abs(dstarhelicity)<.5");
//   ReducedNtuple->Draw("massKpipi1-dstarmass+2.01>>HReflection");
//   if(!massfitter.Initialize(&HReflection,_OutputDir+"/InitRooFitParsReflection.txt"))return 0;
//   massfitter.Fit();
//   massfitter.Save(_OutputDir);

// //   TFile File(_OutputDir+"/CCBarBackground.root","recreate");
// //   HReflection.Write();
// //   cout<<HReflection.Integral()<<endl;
// //   File.ls();
// //   File.Close();

//   if(CloseReducedNtuple()!=1) return 0;
  
//   return 1;
// }
  

Int_t DstPiAnalysis::FitMass(TString HName){

  TFile file(_OutputDir+"/HistosForFit.root","read");
  gROOT->cd();

  TH1F*HDCPiMass=(TH1F*)file.Get(HName);
 
  if(!HDCPiMass){cout<<"no histo"<<endl;return 0;}
  SetHistoXYLabels(HDCPiMass,"m(D*#pi)","GeV/c^{2}");
  HDCPiMass->SetStats(0);
  
  if(!massfitter.Initialize(HDCPiMass,_OutputDir+"/InitRooFitPars.txt"))return 0;
  if(massfitter.Fit()<0)return 0;
  massfitter.Save(_OutputDir);
  
 
  file.Close();
  return 1;
}


