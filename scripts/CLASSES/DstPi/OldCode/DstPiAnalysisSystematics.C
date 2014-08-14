#include "DstPiAnalysis.h"


Int_t DstPiAnalysis::Systematics(TString sigtitle,Bool_t fit){
  cout<<" Doing Systematics: Remove FitPars files if new fits wanted. "<<endl;
 
  Float_t genmean=0;
  Float_t genwidth=0;
  Int_t whichplot=2;
  if(_MCorDATA==0){
    if(sigtitle=="D2420"){
      genmean=2422.2;
      genwidth=18.9;
    }
    if(sigtitle=="D2460"){
      genmean=2458.9;
      genwidth=29.9;
    }
  }
  if(!_TruthMatch){
    if(sigtitle=="D2420"){
      whichplot=4;
    }
    if(sigtitle=="D2460"){
      whichplot=3;
    }
  }




  Float_t defaultmean;
  Float_t defaultwidth;
  Float_t defaultmeaner;
  Float_t defaultwidther;
  Float_t mean;
  Float_t width;
  
  Float_t meandelta;
  Float_t widthdelta;
  Float_t totalmeandelta=0;
  Float_t totalwidthdelta=0;



  TString ftag;
  Bool_t nofile=0;

  Int_t npts=0;  
  TGraph GChisquare;
  Float_t chi=0;
  Float_t ndf=0;  
  TGraphErrors GMean;
  TGraphErrors GWidth;


  //default : only for computing the resolution error; In order to save time use fits without resolution, only do one fit with resolution to determine default values and 
  ftag="Default";
  roofitconfigfile="Systematics/InitRooFitPars_"+sigtitle+"_"+ftag+".txt";
  nofile=!OpenFitPars("Systematics/FitPars_"+sigtitle+"_"+ftag+".txt");
  if(fit||nofile){
    if(!RooFitMassPeak(whichplot))return 0;
    system(TString("mv ")+_OutputDir+"/FitPars"+roofitfileid+".txt "+_OutputDir+"/Systematics/FitPars_"+sigtitle+"_"+ftag+".txt");
    system(TString("mv ")+_OutputDir+"/DstPiRooFit"+roofitfileid+".ps "+_OutputDir+"/Systematics/FitPars_"+sigtitle+"_"+ftag+".ps");
    if(!OpenFitPars("Systematics/FitPars_"+sigtitle+"_"+ftag+".txt"))return 0;
  }
  defaultmean=GetFitPar(sigtitle,"mean")*1000;
  defaultwidth=GetFitPar(sigtitle,"width")*1000;
  defaultmeaner=GetFitPar(sigtitle,"meanerror")*1000;
  defaultwidther=GetFitPar(sigtitle,"widtherror")*1000;
  CloseFitPars();  
  cout<<"Default Values: mean="<<setprecision(7)<<defaultmean<<"+-"<<defaultmeaner<<"   width="<<setprecision(7)<<defaultwidth<<"+-"<<defaultwidther<<endl;


  //vary Resolution::Turn off resolution and take .1 of the change as systematic error on width, assign .05 error on mean
  ftag="Resolution"; 
  roofitconfigfile="Systematics/InitRooFitPars_"+sigtitle+"_"+ftag+".txt";
  nofile=!OpenFitPars("Systematics/FitPars_"+sigtitle+"_"+ftag+".txt");
  if(fit||nofile){
    if(!RooFitMassPeak(whichplot))return 0;
    system(TString("mv ")+_OutputDir+"/FitPars"+roofitfileid+".txt "+_OutputDir+"/Systematics/FitPars_"+sigtitle+"_"+ftag+".txt");
    system(TString("mv ")+_OutputDir+"/DstPiRooFit"+roofitfileid+".ps "+_OutputDir+"/Systematics/FitPars_"+sigtitle+"_"+ftag+".ps");
    if(!OpenFitPars("Systematics/FitPars_"+sigtitle+"_"+ftag+".txt"))return 0;
  }
  width=GetFitPar(sigtitle,"width")*1000;
  mean=GetFitPar(sigtitle,"mean")*1000;
  meandelta=.05;
  widthdelta=(width-defaultwidth)*.1;
  totalmeandelta+=meandelta*meandelta;
  totalwidthdelta+=widthdelta*widthdelta;
  cout<<ftag+" Delta: mean="<<setprecision(2)<<meandelta<<"   width="<<setprecision(2)<<widthdelta<<endl;
  GMean.SetPoint(npts,npts+1,defaultmean+meandelta);
  GMean.SetPointError(npts,0,GetFitPar(sigtitle,"meanerror")*1000);
  GWidth.SetPoint(npts,npts+1,defaultwidth+widthdelta);
  GWidth.SetPointError(npts,0,GetFitPar(sigtitle,"widtherror")*1000);
  chi=GetFitPar("Range","chisquare");
  ndf=GetFitPar("Range","ndf");
  GChisquare.SetPoint(npts,npts+1,chi/ndf);
  npts++;  
  CloseFitPars();

  //from now on use no resolution and account using offsets
  Float_t meanoffset=0;
  Float_t widthoffset=0;
  meanoffset=mean-defaultmean;
  widthoffset=width-defaultwidth;


  //turn off the shoulder and assign .5 the change as a systematic
  if(_MCorDATA==1){
    ftag="Shoulder"; 
    roofitconfigfile="Systematics/InitRooFitPars_"+sigtitle+"_"+ftag+".txt";
    nofile=!OpenFitPars("Systematics/FitPars_"+sigtitle+"_"+ftag+".txt");
    if(fit||nofile){
      if(!RooFitMassPeak(whichplot))return 0;
      system(TString("mv ")+_OutputDir+"/FitPars"+roofitfileid+".txt "+_OutputDir+"/Systematics/FitPars_"+sigtitle+"_"+ftag+".txt");
      system(TString("mv ")+_OutputDir+"/DstPiRooFit"+roofitfileid+".ps "+_OutputDir+"/Systematics/FitPars_"+sigtitle+"_"+ftag+".ps");
      if(!OpenFitPars("Systematics/FitPars_"+sigtitle+"_"+ftag+".txt"))return 0;
    }  
    mean=GetFitPar(sigtitle,"mean")*1000-meanoffset;
    width=GetFitPar(sigtitle,"width")*1000-widthoffset;
    meandelta=(mean-defaultmean)*.5;//assign half of the change 
    widthdelta=(width-defaultwidth)*.5;//assign half of the change 
    totalmeandelta+=meandelta*meandelta;
    totalwidthdelta+=widthdelta*widthdelta;
    cout<<ftag+" Delta: mean="<<setprecision(2)<<meandelta<<"   width="<<setprecision(2)<<widthdelta<<endl;
    GMean.SetPoint(npts,npts+1,defaultmean+meandelta);
    GMean.SetPointError(npts,0,GetFitPar(sigtitle,"meanerror")*1000);
    GWidth.SetPoint(npts,npts+1,defaultwidth+widthdelta);
    GWidth.SetPointError(npts,0,GetFitPar(sigtitle,"widtherror")*1000);
    chi=GetFitPar("Range","chisquare");
    ndf=GetFitPar("Range","ndf");
    GChisquare.SetPoint(npts,npts+1,chi/ndf);
    npts++;  
    CloseFitPars();

    //from now on use no shoulder and account using offsets: only for D2420 because for D2460 always fit with shoulder
    if(sigtitle=="D2420"){
      meanoffset+=mean-defaultmean;
      widthoffset+=width-defaultwidth;
    }

  }


  
  //vary the bin size  
  ftag="BinSize"; 
  roofitconfigfile="Systematics/InitRooFitPars_"+sigtitle+"_"+ftag+".txt";
  nofile=!OpenFitPars("Systematics/FitPars_"+sigtitle+"_"+ftag+".txt");
  if(fit||nofile){
    if(!RooFitMassPeak(whichplot))return 0;
    system(TString("mv ")+_OutputDir+"/FitPars"+roofitfileid+".txt "+_OutputDir+"/Systematics/FitPars_"+sigtitle+"_"+ftag+".txt");
    system(TString("mv ")+_OutputDir+"/DstPiRooFit"+roofitfileid+".ps "+_OutputDir+"/Systematics/FitPars_"+sigtitle+"_"+ftag+".ps");
    if(!OpenFitPars("Systematics/FitPars_"+sigtitle+"_"+ftag+".txt"))return 0;
  }  
  mean=GetFitPar(sigtitle,"mean")*1000-meanoffset;
  width=GetFitPar(sigtitle,"width")*1000-widthoffset;
  meandelta=(mean-defaultmean);
  widthdelta=(width-defaultwidth);
  totalmeandelta+=meandelta*meandelta;
  totalwidthdelta+=widthdelta*widthdelta;
  cout<<ftag+" Delta: mean="<<setprecision(2)<<meandelta<<"   width="<<setprecision(2)<<widthdelta<<endl;
  GMean.SetPoint(npts,npts+1,defaultmean+meandelta);
  GMean.SetPointError(npts,0,GetFitPar(sigtitle,"meanerror")*1000);
  GWidth.SetPoint(npts,npts+1,defaultwidth+widthdelta);
  GWidth.SetPointError(npts,0,GetFitPar(sigtitle,"widtherror")*1000);
  chi=GetFitPar("Range","chisquare");
  ndf=GetFitPar("Range","ndf");
  GChisquare.SetPoint(npts,npts+1,chi/ndf);
  npts++;  
  CloseFitPars();



  //vary the fit range
  ftag="FitRange"; 
  roofitconfigfile="Systematics/InitRooFitPars_"+sigtitle+"_"+ftag+".txt";
  nofile=!OpenFitPars("Systematics/FitPars_"+sigtitle+"_"+ftag+".txt");
  if(fit||nofile){
    if(!RooFitMassPeak(whichplot))return 0;
    system(TString("mv ")+_OutputDir+"/FitPars"+roofitfileid+".txt "+_OutputDir+"/Systematics/FitPars_"+sigtitle+"_"+ftag+".txt");
    system(TString("mv ")+_OutputDir+"/DstPiRooFit"+roofitfileid+".ps "+_OutputDir+"/Systematics/FitPars_"+sigtitle+"_"+ftag+".ps");
    if(!OpenFitPars("Systematics/FitPars_"+sigtitle+"_"+ftag+".txt"))return 0;
  }  
  mean=GetFitPar(sigtitle,"mean")*1000-meanoffset;
  width=GetFitPar(sigtitle,"width")*1000-widthoffset;
  meandelta=(mean-defaultmean);
  widthdelta=(width-defaultwidth);
  totalmeandelta+=meandelta*meandelta;
  totalwidthdelta+=widthdelta*widthdelta;
  cout<<ftag+" Delta: mean="<<setprecision(2)<<meandelta<<"   width="<<setprecision(2)<<widthdelta<<endl;
  GMean.SetPoint(npts,npts+1,defaultmean+meandelta);
  GMean.SetPointError(npts,0,GetFitPar(sigtitle,"meanerror")*1000);
  GWidth.SetPoint(npts,npts+1,defaultwidth+widthdelta);
  GWidth.SetPointError(npts,0,GetFitPar(sigtitle,"widtherror")*1000);
  chi=GetFitPar("Range","chisquare");
  ndf=GetFitPar("Range","ndf");
  GChisquare.SetPoint(npts,npts+1,chi/ndf);
  npts++;  
  CloseFitPars();


  //vary efficiency :: Turn off p* factor and take 1/2 of the change as systematic error
  ftag="Efficiency"; 
  roofitconfigfile="Systematics/InitRooFitPars_"+sigtitle+"_"+ftag+".txt";
  nofile=!OpenFitPars("Systematics/FitPars_"+sigtitle+"_"+ftag+".txt");
  if(fit||nofile){
    if(!RooFitMassPeak(whichplot))return 0;
    system(TString("mv ")+_OutputDir+"/FitPars"+roofitfileid+".txt "+_OutputDir+"/Systematics/FitPars_"+sigtitle+"_"+ftag+".txt");
    system(TString("mv ")+_OutputDir+"/DstPiRooFit"+roofitfileid+".ps "+_OutputDir+"/Systematics/FitPars_"+sigtitle+"_"+ftag+".ps");
    if(!OpenFitPars("Systematics/FitPars_"+sigtitle+"_"+ftag+".txt"))return 0;
  } 
  mean=GetFitPar(sigtitle,"mean")*1000-meanoffset;
  width=GetFitPar(sigtitle,"width")*1000-widthoffset;
  meandelta=(mean-defaultmean)/2;
  widthdelta=(width-defaultwidth)/2;
  totalmeandelta+=meandelta*meandelta;
  totalwidthdelta+=widthdelta*widthdelta;
  cout<<ftag+" Delta: mean="<<setprecision(2)<<meandelta<<"   width="<<setprecision(2)<<widthdelta<<endl;
  GMean.SetPoint(npts,npts+1,defaultmean+meandelta);
  GMean.SetPointError(npts,0,GetFitPar(sigtitle,"meanerror")*1000);
  GWidth.SetPoint(npts,npts+1,defaultwidth+widthdelta);
  GWidth.SetPointError(npts,0,GetFitPar(sigtitle,"widtherror")*1000);
  chi=GetFitPar("Range","chisquare");
  ndf=GetFitPar("Range","ndf");
  GChisquare.SetPoint(npts,npts+1,chi/ndf);
  npts++;  
  CloseFitPars();


 

  //For ccbar vary efficiency :: switch from 3rd order to 5th order  (no change observed at 4th order)
  if(!_TruthMatch){
    ftag="PolOrder"; 
    roofitconfigfile="Systematics/InitRooFitPars_"+sigtitle+"_"+ftag+".txt";
    nofile=!OpenFitPars("Systematics/FitPars_"+sigtitle+"_"+ftag+".txt");
    if(fit||nofile){
      if(!RooFitMassPeak(whichplot))return 0;
      system(TString("mv ")+_OutputDir+"/FitPars"+roofitfileid+".txt "+_OutputDir+"/Systematics/FitPars_"+sigtitle+"_"+ftag+".txt");
      system(TString("mv ")+_OutputDir+"/DstPiRooFit"+roofitfileid+".ps "+_OutputDir+"/Systematics/FitPars_"+sigtitle+"_"+ftag+".ps");
      if(!OpenFitPars("Systematics/FitPars_"+sigtitle+"_"+ftag+".txt"))return 0;
    }  
    mean=GetFitPar(sigtitle,"mean")*1000-meanoffset;
    width=GetFitPar(sigtitle,"width")*1000-widthoffset;
    meandelta=(mean-defaultmean);
    widthdelta=(width-defaultwidth);
    totalmeandelta+=meandelta*meandelta;
    totalwidthdelta+=widthdelta*widthdelta;
    cout<<ftag+" Delta: mean="<<setprecision(2)<<meandelta<<"   width="<<setprecision(2)<<widthdelta<<endl;
    GMean.SetPoint(npts,npts+1,defaultmean+meandelta);
    GMean.SetPointError(npts,0,GetFitPar(sigtitle,"meanerror")*1000);
    GWidth.SetPoint(npts,npts+1,defaultwidth+widthdelta);
    GWidth.SetPointError(npts,0,GetFitPar(sigtitle,"widtherror")*1000);
    chi=GetFitPar("Range","chisquare");
    ndf=GetFitPar("Range","ndf");
    GChisquare.SetPoint(npts,npts+1,chi/ndf);
    npts++;  
    CloseFitPars();

  }


  //---------------------------
  //---Kinematic variables
  //-------------------------   
  //save plots
  filename=_OutputDir+"/Systematics/Systematics_"+sigtitle+".ps";
  Canvas.Print(filename+"["); 

  ftag="Kinematics";//use different shoulder yield
  roofitconfigfile="Systematics/InitRooFitPars_"+sigtitle+"_"+ftag+".txt";
  cout<<" For the kinematic variables fit uncorrected distribution with config"<<roofitconfigfile<<endl;  

  //p* dependence
//   Double_t PX[4]={3.25,3.75,4.25,4.75};
//   Double_t PXE[4]={.25,.25,.25,.25};
   Double_t PMeanY[4];
//   Double_t PMeanYE[4];
   Double_t PWidthY[4];
//   Double_t PWidthYE[4]; 
  for(Int_t i=0;i<4;i++){
    nofile=!OpenFitPars("Systematics/FitPars_"+sigtitle+"_"+ftag+"_pstar"+(long)i+".txt");
    if(fit||nofile){
      if(!RooFitMassPeak(80+i))return 0;
      system(TString("mv ")+_OutputDir+"/FitPars"+roofitfileid+".txt "+_OutputDir+"/Systematics/FitPars_"+sigtitle+"_"+ftag+"_pstar"+(long)i+".txt");
      system(TString("mv ")+_OutputDir+"/DstPiRooFit"+roofitfileid+".ps "+_OutputDir+"/Systematics/FitPars_"+sigtitle+"_"+ftag+"_pstar"+(long)i+".ps");
      if(!OpenFitPars("Systematics/FitPars_"+sigtitle+"_"+ftag+"_pstar"+(long)i+".txt"))return 0;  
    }   
     PMeanY[i]=GetFitPar(sigtitle,"mean")*1000-meanoffset;
//     PMeanYE[i]=GetFitPar(sigtitle,"meanerror")*1000;
     PWidthY[i]=GetFitPar(sigtitle,"width")*1000-widthoffset; 
//     PWidthYE[i]=GetFitPar(sigtitle,"widtherror")*1000; 

    GMean.SetPoint(npts,npts+1,PMeanY[i]);
    GMean.SetPointError(npts,0,GetFitPar(sigtitle,"meanerror")*1000);
    GWidth.SetPoint(npts,npts+1,PWidthY[i]);
    GWidth.SetPointError(npts,0,GetFitPar(sigtitle,"widtherror")*1000);
    chi=GetFitPar("Range","chisquare");
    ndf=GetFitPar("Range","ndf");
    GChisquare.SetPoint(npts,npts+1,chi/ndf);
    npts++; 

    CloseFitPars();
  }

//    totalmeandelta+=pow(GraphXvsY(4,PX,PMeanY,PXE,PMeanYE,"p* (GeV)",sigtitle+" Mass (MeV)",defaultmean,defaultmeaner,genmean),2);
//    totalwidthdelta+=pow(GraphXvsY(4,PX,PWidthY,PXE,PWidthYE,"p* (GeV)",sigtitle+" Width (MeV)",defaultwidth,defaultwidther,genwidth),2);


 
  //phi* dependence
//   Double_t PhiX[4]={-180*3/4,-180*1/4,180*1/4,180*3/4};
//   Double_t PhiXE[4]={45,45,45,45};
   Double_t PhiMeanY[4];
//   Double_t PhiMeanYE[4];
   Double_t PhiWidthY[4];
//   Double_t PhiWidthYE[4];
  for(Int_t i=0;i<4;i++){
    nofile=!OpenFitPars("Systematics/FitPars_"+sigtitle+"_"+ftag+"_phi"+(long)i+".txt");
    if(fit||nofile){
      if(!RooFitMassPeak(90+i))return 0;
      system(TString("mv ")+_OutputDir+"/FitPars"+roofitfileid+".txt "+_OutputDir+"/Systematics/FitPars_"+sigtitle+"_"+ftag+"_phi"+(long)i+".txt");
      system(TString("mv ")+_OutputDir+"/DstPiRooFit"+roofitfileid+".ps "+_OutputDir+"/Systematics/FitPars_"+sigtitle+"_"+ftag+"_phi"+(long)i+".ps");
      if(!OpenFitPars("Systematics/FitPars_"+sigtitle+"_"+ftag+"_phi"+(long)i+".txt"))return 0; 
    }    
    PhiMeanY[i]=GetFitPar(sigtitle,"mean")*1000-meanoffset;
    //     PhiMeanYE[i]=GetFitPar(sigtitle,"meanerror")*1000;
    PhiWidthY[i]=GetFitPar(sigtitle,"width")*1000-widthoffset; 
    //     PhiWidthYE[i]=GetFitPar(sigtitle,"widtherror")*1000; 
    GMean.SetPoint(npts,npts+1,PhiMeanY[i]);
    GMean.SetPointError(npts,0,GetFitPar(sigtitle,"meanerror")*1000);
    GWidth.SetPoint(npts,npts+1,PhiWidthY[i]);
    GWidth.SetPointError(npts,0,GetFitPar(sigtitle,"widtherror")*1000);
    chi=GetFitPar("Range","chisquare");
    ndf=GetFitPar("Range","ndf");
    GChisquare.SetPoint(npts,npts+1,chi/ndf);
    npts++;
    CloseFitPars();
  }

//   totalmeandelta+=pow(GraphXvsY(4,PhiX,PhiMeanY,PhiXE,PhiMeanYE,"#phi (degrees)",sigtitle+" Mass (MeV)",defaultmean,defaultmeaner,genmean),2);
//   totalwidthdelta+=pow(GraphXvsY(4,PhiX,PhiWidthY,PhiXE,PhiWidthYE,"#phi (degrees)",sigtitle+" Width (MeV)",defaultwidth,defaultwidther,genwidth),2);


  //cos* dependence
//   Double_t CosX[4]={-.75,-.25,.25,.75};
//   Double_t CosXE[4]={.25,.25,.25,.25};
   Double_t CosMeanY[4];
//   Double_t CosMeanYE[4];
   Double_t CosWidthY[4];
//   Double_t CosWidthYE[4];
  for(Int_t i=0;i<4;i++){
    nofile=!OpenFitPars("Systematics/FitPars_"+sigtitle+"_"+ftag+"_cos"+(long)i+".txt");
    if(fit||nofile){
      if(!RooFitMassPeak(100+i))return 0;
      system(TString("mv ")+_OutputDir+"/FitPars"+roofitfileid+".txt "+_OutputDir+"/Systematics/FitPars_"+sigtitle+"_"+ftag+"_cos"+(long)i+".txt");
      system(TString("mv ")+_OutputDir+"/DstPiRooFit"+roofitfileid+".ps "+_OutputDir+"/Systematics/FitPars_"+sigtitle+"_"+ftag+"_cos"+(long)i+".ps");
      if(!OpenFitPars("Systematics/FitPars_"+sigtitle+"_"+ftag+"_cos"+(long)i+".txt"))return 0;
    }    
    CosMeanY[i]=GetFitPar(sigtitle,"mean")*1000-meanoffset;
//     CosMeanYE[i]=GetFitPar(sigtitle,"meanerror")*1000;
    CosWidthY[i]=GetFitPar(sigtitle,"width")*1000-widthoffset; 
//     CosWidthYE[i]=GetFitPar(sigtitle,"widtherror")*1000; 
    GMean.SetPoint(npts,npts+1,CosMeanY[i]);
    GMean.SetPointError(npts,0,GetFitPar(sigtitle,"meanerror")*1000);
    GWidth.SetPoint(npts,npts+1,CosWidthY[i]);
    GWidth.SetPointError(npts,0,GetFitPar(sigtitle,"widtherror")*1000);
    chi=GetFitPar("Range","chisquare");
    ndf=GetFitPar("Range","ndf");
    GChisquare.SetPoint(npts,npts+1,chi/ndf);
    npts++;
    CloseFitPars();
  }
 
  
//   totalmeandelta+=pow(GraphXvsY(4,CosX,CosMeanY,CosXE,CosMeanYE,"cos(#theta)",sigtitle+" Mass (MeV)",defaultmean,defaultmeaner,genmean),2);
//   totalwidthdelta+=pow(GraphXvsY(4,CosX,CosWidthY,CosXE,CosWidthYE,"cos(#theta)",sigtitle+" Width (MeV)",defaultwidth,defaultwidther,genwidth),2);


  TBox box;
  box.SetFillColor(kGreen-7);
  box.SetFillStyle(1001); 

  Canvas.Clear();
  GMean.Draw("ape");
  GMean.GetXaxis()->SetTitle("systematic id");
  GMean.GetYaxis()->SetTitle("Mass"); 
  Canvas.Update();
  box.DrawBox(1,defaultmean-defaultmeaner,npts+1,defaultmean+defaultmeaner);
  GMean.Draw("pe");
  Canvas.Print(filename);

  Canvas.Clear();
  GWidth.Draw("ape");
  GWidth.GetXaxis()->SetTitle("systematic id");
  GWidth.GetYaxis()->SetTitle("Width");
  box.DrawBox(1,defaultwidth-defaultwidther,npts+1,defaultwidth+defaultwidther);
  GWidth.Draw("pe");
  Canvas.Update();
  Canvas.Print(filename);
  
  Canvas.Clear();
  GChisquare.Draw("ap");
  GChisquare.GetXaxis()->SetTitle("systematic id");
  GChisquare.GetYaxis()->SetTitle("#chi^{2}/df");
  Canvas.Update();
  Canvas.Print(filename);

  Canvas.Print(filename+"]"); 


  if(_MCorDATA==1){
    //----------------------------
    //--other systematics
    //------------------------
    cout<<"----------------The following are the systematics obtained from Sig MC--------------------------"<<endl;
    Float_t BFieldmeandelta=0;
    Float_t BFieldwidthdelta=0;
    Float_t SVTmeandelta=0;
    Float_t SVTwidthdelta=0;
    if(_D0Mode==1){
      if(sigtitle=="D2420"){
	BFieldmeandelta=.31;
	BFieldwidthdelta=.11;
	SVTmeandelta=.13;
	SVTwidthdelta= .01;
      }
      if(sigtitle=="D2460"){
	BFieldmeandelta=.42;
	BFieldwidthdelta=.15;
	SVTmeandelta=.65;
	SVTwidthdelta=.08;
      }
    }
    if(_D0Mode==2){
      if(sigtitle=="D2420"){
	BFieldmeandelta=(-defaultmean);
	BFieldwidthdelta=(-defaultwidth);
	SVTmeandelta=(-defaultmean);
	SVTwidthdelta= (-defaultwidth);
      }
      if(sigtitle=="D2460"){
	BFieldmeandelta=(-defaultmean);
	BFieldwidthdelta=(-defaultwidth);
	SVTmeandelta=(-defaultmean);
	SVTwidthdelta= (-defaultwidth);
      }
    }


    ////----------------------------------------------
    // Bfield
 
    totalmeandelta+=BFieldmeandelta*BFieldmeandelta;
    totalwidthdelta+=BFieldwidthdelta*BFieldwidthdelta;

    cout<<"BFiled Solenoid=.9998 Delta: mean="<<setprecision(2)<<BFieldmeandelta<<"   width="<<setprecision(2)<<BFieldwidthdelta<<endl;     

    //SVT density 
 
    totalmeandelta+=SVTmeandelta*SVTmeandelta;
    totalwidthdelta+=SVTwidthdelta*SVTwidthdelta;

    cout<<"SVT Density=.8 Delta: mean="<<setprecision(2)<<SVTmeandelta<<"   width="<<setprecision(2)<<SVTwidthdelta<<endl;

    cout<<endl<<endl;
  }

  //---------------------------------
  ////print out the total
  //----------------------------------
  cout<<" Total Systematics:   Mean="<<setprecision(2)<<sqrt(totalmeandelta)<<"   Width="<<setprecision(2)<<sqrt(totalwidthdelta)<<endl;
  
  roofitconfigfile="InitRooFitPars.txt";
  return 1;
}



Int_t DstPiAnalysis::FitMassVsCosDstarMW(TString sigtitle,Bool_t fit){

  Int_t  NBINS=8;  
  Int_t firstid=0;
  if(sigtitle=="All")firstid=180;
  if(sigtitle=="D2420")firstid=200;
  if(sigtitle=="D2460")firstid=210;

 
  tempfittag=sigtitle+"CosDstarFits";
  roofitconfigfile=TString("CosDstarFits/InitRooFitPars_")+sigtitle+".txt";
  TFile file(_OutputDir+"/CosDstarFits/FitMassVsCosDstar_"+sigtitle+".root","update");
  RooPlot* plot=NULL;
  Bool_t nofile=0;
  for(Int_t binindex=0;binindex<NBINS;binindex++){
    nofile=!OpenFitPars(TString("CosDstarFits/FitPars_")+sigtitle+"_"+(long)(binindex)+".txt");
    CloseFitPars();
    if(fit||nofile){      

      system(TString("rm -f ")+_OutputDir+"/CosDstarFits/FitPars_"+sigtitle+"_"+(long)(binindex)+".txt");
      system(TString("rm -f ")+_OutputDir+"/CosDstarFits/FitPars_"+sigtitle+"_"+(long)(binindex)+".ps");
      file.Delete(TString("plot")+(long)binindex+";1"); 


      plot=(RooPlot*)RooFitMassPeak(binindex+firstid);
      if(plot){
	system(TString("mv ")+_OutputDir+"/FitPars"+roofitfileid+".txt "+_OutputDir+"/CosDstarFits/FitPars_"+sigtitle+"_"+(long)(binindex)+".txt");
	system(TString("mv ")+_OutputDir+"/DstPiRooFit"+roofitfileid+".ps "+_OutputDir+"/CosDstarFits/FitPars_"+sigtitle+"_"+(long)(binindex)+".ps");
            
	file.cd();
	plot->SetName(TString("plot")+(long)binindex);
	file.Delete(TString("plot")+(long)binindex+";1");
	plot->Write();
	delete plot;
      }
    }   
    
    cout<<"fit done "<<binindex<<endl;
    
  }
  file.ls();
  file.Close();


  roofitconfigfile="InitRooFitPars.txt";
  return 1;
}

Int_t DstPiAnalysis::PlotMassVsCosDstarMW(TString sigtitle){

  Int_t firstid=0;
  if(sigtitle=="D2420")firstid=200;
  if(sigtitle=="D2460")firstid=210;
  if(sigtitle=="P2")firstid=220;
  
  Int_t  NBINS=6;
  filename=_OutputDir+"/CosDstarFits/MassVsCosDstar_"+sigtitle+".ps";
  Canvas.Print(filename+"[");
  
  //show the full range plots before fitting
  Canvas.Clear();
  Canvas.Divide(2,3);
  for(Int_t binindex=0;binindex<NBINS;binindex++){
    TH1*H=GetMassHistoForFit(firstid+binindex,100,2.1,3.1);
    if(H){
      Canvas.GetPad(binindex+1)->SetRightMargin(.2);
      Canvas.GetPad(binindex+1)->SetLeftMargin(.1);
      Canvas.GetPad(binindex+1)->SetTopMargin(.1);
      Canvas.GetPad(binindex+1)->SetBottomMargin(.1);
      Canvas.cd(binindex+1);
      H->SetTitle("");
      H->SetTitleSize(.04);
      H->GetYaxis()->SetTitle("");
      H->GetYaxis()->SetLabelSize(.08);
      H->GetYaxis()->SetNdivisions(5);
      H->GetXaxis()->SetTitle("");
      H->GetXaxis()->SetLabelSize(.12);
      H->GetXaxis()->SetNdivisions(5); 
      H->SetStats(0);
      H->SetMarkerSize(.7);
      H->Draw("pe");
      text.SetTextSize(.15);
      text.DrawTextNDC(.7,.75,TString("")+(long)(binindex+1));
      cutline.DrawLine(2.1,0,3.1,0);

      cout<<"done plot "<<binindex<<endl;
    }

  }
 
  Canvas.Print(filename);
 
  Canvas.Print(filename+"]");

  return 1;
}

Int_t DstPiAnalysis::PlotFitMassVsCosDstarMW(TString sigtitle){

  Int_t firstid=0;
  if(sigtitle=="D2420")firstid=200;
  if(sigtitle=="D2460")firstid=210;
  Int_t  NBINS=6;
  filename=_OutputDir+"/CosDstarFits/FitVsCosDstar_"+sigtitle+".ps";
  Canvas.Print(filename+"[");
  


 
  Float_t min=-1.;
  Float_t max=1;
  Float_t D2420width;
  Float_t D2460width;
  Float_t D2420widthEr;
  Float_t D2460widthEr;
  TH1F HD2420VsCosDstarWidth("HD2420VsCosDstarWidth","HD2420VsCosDstarWidth",8,min,max);
  TH1F HD2460VsCosDstarWidth("HD2460VsCosDstarWidth","HD2460VsCosDstarWidth",8,min,max); 

  Float_t D2420mean;
  Float_t D2460mean;
  Float_t D2420meanEr;
  Float_t D2460meanEr;
  TH1F HD2420VsCosDstarMean("HD2420VsCosDstarMean","HD2420VsCosDstarMean",8,min,max);
  TH1F HD2460VsCosDstarMean("HD2460VsCosDstarMean","HD2460VsCosDstarMean",8,min,max); 


  Float_t D2420yield;
  Float_t D2460yield;
  Float_t D2420yieldEr;
  Float_t D2460yieldEr;
  TH1F HD2420VsCosDstarYield("HD2420VsCosDstarYield","HD2420VsCosDstarYield",8,min,max);
  TH1F HD2460VsCosDstarYield("HD2460VsCosDstarYield","HD2460VsCosDstarYield",8,min,max);   
  TH1F HTotalYieldVsCosDstar("HTotalYieldVsCosDstar","HTotalYieldVsCosDstar",8,min,max); 
 

  TH1F HChiPerDF("HChiPerDF","HChiPerDF",8,min,max); 
 
  Float_t MeanD2420[8];


  for(Int_t binindex=0;binindex<NBINS;binindex++){
    
    if(!OpenFitPars(TString("CosDstarFits/FitPars_")+sigtitle+"_"+(long)(binindex)+".txt"))continue;
    
    //Width
    D2420width=GetFitPar("D2420","width")*1000;
    D2420widthEr=GetFitPar("D2420","widtherror")*1000; 
    HD2420VsCosDstarWidth.SetBinContent(binindex+1,D2420width);//round to nearest integer
    HD2420VsCosDstarWidth.SetBinError(binindex+1,D2420widthEr);
    if(D2420widthEr/D2420width>.9){
      HD2420VsCosDstarWidth.SetBinContent(binindex+1,0);
      HD2420VsCosDstarWidth.SetBinError(binindex+1,0);
    }

    D2460width=GetFitPar("D2460","width")*1000;
    D2460widthEr=GetFitPar("D2460","widtherror")*1000;
    HD2460VsCosDstarWidth.SetBinContent(binindex+1,D2460width);
    HD2460VsCosDstarWidth.SetBinError(binindex+1,D2460widthEr);
    if(D2460widthEr/D2460width>.9){
      HD2460VsCosDstarWidth.SetBinContent(binindex+1,0);
      HD2460VsCosDstarWidth.SetBinError(binindex+1,0);
    }

    //Mean
    D2420mean=GetFitPar("D2420","mean")*1000;
    D2420meanEr=GetFitPar("D2420","meanerror")*1000; 
    HD2420VsCosDstarMean.SetBinContent(binindex+1,D2420mean);//round to nearest integer
    HD2420VsCosDstarMean.SetBinError(binindex+1,D2420meanEr);
    if(D2420meanEr/D2420mean>.9){
      HD2420VsCosDstarMean.SetBinContent(binindex+1,0);
      HD2420VsCosDstarMean.SetBinError(binindex+1,0);
    }

    D2460mean=GetFitPar("D2460","mean")*1000;
    D2460meanEr=GetFitPar("D2460","meanerror")*1000;
    HD2460VsCosDstarMean.SetBinContent(binindex+1,D2460mean);
    HD2460VsCosDstarMean.SetBinError(binindex+1,D2460meanEr);
    if(D2460meanEr/D2460mean>.9){
      HD2460VsCosDstarMean.SetBinContent(binindex+1,0);
      HD2460VsCosDstarMean.SetBinError(binindex+1,0);
    }
    
    //Yield
    D2420yield=GetFitPar("D2420","yield");
    D2420yieldEr=GetFitPar("D2420","yielderror"); 
    HD2420VsCosDstarYield.SetBinContent(binindex+1,D2420yield);//round to nearest integer
    HD2420VsCosDstarYield.SetBinError(binindex+1,D2420yieldEr);
    if(D2420yieldEr/D2420yield>.9){
      HD2420VsCosDstarYield.SetBinContent(binindex+1,0);
      HD2420VsCosDstarYield.SetBinError(binindex+1,0);
    }

    D2460yield=GetFitPar("D2460","yield");
    D2460yieldEr=GetFitPar("D2460","yielderror");
    HD2460VsCosDstarYield.SetBinContent(binindex+1,D2460yield);
    HD2460VsCosDstarYield.SetBinError(binindex+1,D2460yieldEr);
    if(D2460yieldEr/D2460yield>.9){
      HD2460VsCosDstarYield.SetBinContent(binindex+1,0);
      HD2460VsCosDstarYield.SetBinError(binindex+1,0);
    }
    
    
    HTotalYieldVsCosDstar.SetBinContent(binindex+1,D2420yield+D2460yield);
    HTotalYieldVsCosDstar.SetBinError(binindex+1,sqrt(D2420yieldEr*D2420yieldEr+D2460yieldEr*D2460yieldEr));


    HChiPerDF.SetBinContent(binindex+1,GetFitPar("Range","chisquare")/GetFitPar("Range","ndf"));
    
    MeanD2420[binindex]=GetFitPar("D2420","mean");

    CloseFitPars();

    cout<<"read file"<<binindex<<endl;

  }


  TFile file(_OutputDir+"/CosDstarFits/FitMassVsCosDstar_"+sigtitle+".root","update");
  if(file.IsZombie()){Canvas.Print(filename+"]");return 0;}

  file.Delete("HD2420VsCosDstarWidth;1");
  HD2420VsCosDstarWidth.SetName("HD2420VsCosDstarWidth");
  HD2420VsCosDstarWidth.Write();
  file.Delete("HD2460VsCosDstarWidth;1");
  HD2460VsCosDstarWidth.SetName("HD2460VsCosDstarWidth");
  HD2460VsCosDstarWidth.Write();

  file.Delete("HD2420VsCosDstarMean;1");
  HD2420VsCosDstarMean.SetName("HD2420VsCosDstarMean");
  HD2420VsCosDstarMean.Write();
  file.Delete("HD2460VsCosDstarMean;1");
  HD2460VsCosDstarMean.SetName("HD2460VsCosDstarMean");
  HD2460VsCosDstarMean.Write();


  file.Delete("HD2420VsCosDstarYield;1");
  HD2420VsCosDstarYield.SetName("HD2420VsCosDstarYield");
  HD2420VsCosDstarYield.Write();
  file.Delete("HD2460VsCosDstarYield;1");
  HD2460VsCosDstarYield.SetName("HD2460VsCosDstarYield");
  HD2460VsCosDstarYield.Write();

  file.Delete("HTotalYieldVsCosDstar;1");
  HTotalYieldVsCosDstar.SetName("HTotalYieldVsCosDstar");
  HTotalYieldVsCosDstar.Write();

  file.ls();
  
  gROOT->cd();

  cutline.SetLineColor(2);
  cutline.SetLineWidth(1);

  RooPlot* plot=NULL;
  Canvas.Clear();
  Canvas.Divide(2,3);
  Int_t counter=0;
  for(Int_t binindex=0;binindex<NBINS;binindex++){
    plot=(RooPlot*)file.Get(TString("plot")+(long)(binindex));
    if(plot&&counter<6){
      counter++;
      //draw plots
      Canvas.GetPad(counter)->SetRightMargin(.2);
      Canvas.GetPad(counter)->SetLeftMargin(.1);
      Canvas.GetPad(counter)->SetTopMargin(.1);
      Canvas.GetPad(counter)->SetBottomMargin(.1);
      Canvas.cd(counter);
      plot->SetTitle("");
      plot->SetTitleSize(.04);
      plot->GetYaxis()->SetTitle("");
      plot->GetYaxis()->SetLabelSize(.08);
      plot->GetYaxis()->SetNdivisions(5);
      plot->GetXaxis()->SetTitle("");
      plot->GetXaxis()->SetLabelSize(.12);
      plot->GetXaxis()->SetNdivisions(5); 
      if(((RooHist*)plot->getObject(0)))
	((RooHist*)plot->getObject(0))->SetMarkerSize(.01);
      plot->Draw();
      text.SetTextSize(.15);
      text.DrawTextNDC(.7,.75,TString("")+(long)(binindex+1));
      cutline.DrawLine(MeanD2420[binindex],0,MeanD2420[binindex],((RooHist*)plot->getObject(0))->GetYaxis()->GetXmax());
      //cutline.DrawLine(MeanD2420[binindex],0,MeanD2420[binindex],plot->GetYaxis()->GetXmax());
      
      cout<<"done plot "<<binindex<<endl;
    }

  }
 
  Canvas.Print(filename);
  file.Close();



  Canvas.Clear();
  HChiPerDF.Draw("p");
  Canvas.Print(filename);

  Canvas.Clear();
  HD2420VsCosDstarWidth.SetMarkerSize(1.5);
  HD2420VsCosDstarWidth.SetMarkerColor(1);
  HD2420VsCosDstarWidth.SetLineColor(1);
  HD2420VsCosDstarWidth.SetStats(0);
  HD2420VsCosDstarWidth.GetYaxis()->SetRangeUser(25,HD2420VsCosDstarWidth.GetMaximum()+1);
  HD2420VsCosDstarWidth.GetYaxis()->SetTitle("D_{1}(2420) Width");
  HD2420VsCosDstarWidth.GetXaxis()->SetTitle("cos(#theta_{D*})");
  HD2420VsCosDstarWidth.SetTitle("");
  HD2420VsCosDstarWidth.Draw("pe");
  Canvas.Print(filename);

  Canvas.Clear();
  HD2460VsCosDstarWidth.SetMarkerSize(1.5);
  HD2460VsCosDstarWidth.SetMarkerColor(1);
  HD2460VsCosDstarWidth.SetLineColor(1);
  HD2460VsCosDstarWidth.SetStats(0);
  //HD2460VsCosDstarWidth.GetYaxis()->SetRangeUser(10,HD2420VsCosDstarWidth.GetMaximum()+1);
  HD2460VsCosDstarWidth.GetYaxis()->SetTitle("D_{2}(2460) Width");
  HD2460VsCosDstarWidth.GetXaxis()->SetTitle("cos(#theta_{D*})");
  HD2460VsCosDstarWidth.SetTitle("");
  HD2460VsCosDstarWidth.Draw("pe");
  Canvas.Print(filename);


  Canvas.Clear();
  HD2420VsCosDstarMean.SetMarkerSize(1.5);
  HD2420VsCosDstarMean.SetMarkerColor(1);
  HD2420VsCosDstarMean.SetLineColor(1);
  HD2420VsCosDstarMean.SetStats(0);
  HD2420VsCosDstarMean.GetYaxis()->SetRangeUser(2412,2426);
  HD2420VsCosDstarMean.GetYaxis()->SetTitle("D_{1}(2420) Mean");
  HD2420VsCosDstarMean.GetXaxis()->SetTitle("cos(#theta_{D*})");
  HD2420VsCosDstarMean.SetTitle("");
  HD2420VsCosDstarMean.Draw("pe");
  Canvas.Print(filename);

  Canvas.Clear();
  HD2460VsCosDstarMean.SetMarkerSize(1.5);
  HD2460VsCosDstarMean.SetMarkerColor(1);
  HD2460VsCosDstarMean.SetLineColor(1);
  HD2460VsCosDstarMean.SetStats(0);
  HD2460VsCosDstarMean.GetYaxis()->SetRangeUser(2450,2470);
  HD2460VsCosDstarMean.GetYaxis()->SetTitle("D_{2}(2460) Mean");
  HD2460VsCosDstarMean.GetXaxis()->SetTitle("cos(#theta_{D*})");
  HD2460VsCosDstarMean.SetTitle("");
  HD2460VsCosDstarMean.Draw("pe");
  Canvas.Print(filename);


  Canvas.Clear();
  HD2420VsCosDstarYield.SetMarkerSize(1.5);
  HD2420VsCosDstarYield.SetMarkerColor(1);
  HD2420VsCosDstarYield.SetLineColor(1);
  HD2420VsCosDstarYield.SetStats(0);
  HD2420VsCosDstarYield.GetYaxis()->SetTitle("D_{1}(2420) Yield");
  HD2420VsCosDstarYield.GetXaxis()->SetTitle("cos(#theta_{D*})");
  HD2420VsCosDstarYield.SetTitle("");
  HD2420VsCosDstarYield.Draw("pe");
  Canvas.Print(filename);

  Canvas.Clear();
  HD2460VsCosDstarYield.SetMarkerSize(1.5);
  HD2460VsCosDstarYield.SetMarkerColor(1);
  HD2460VsCosDstarYield.SetLineColor(1);
  HD2460VsCosDstarYield.SetStats(0);
  HD2460VsCosDstarYield.GetYaxis()->SetTitle("D_{2}(2460) Yield");
  HD2460VsCosDstarYield.GetXaxis()->SetTitle("cos(#theta_{D*})");
  HD2460VsCosDstarYield.SetTitle("");
  HD2460VsCosDstarYield.Draw("pe");
  Canvas.Print(filename);



  Canvas.Clear();
  HTotalYieldVsCosDstar.SetMarkerSize(1.5);
  HTotalYieldVsCosDstar.SetMarkerColor(1);
  HTotalYieldVsCosDstar.SetLineColor(1);
  HTotalYieldVsCosDstar.SetStats(0);
  HTotalYieldVsCosDstar.GetYaxis()->SetTitle("Total Yield");
  HTotalYieldVsCosDstar.GetXaxis()->SetTitle("cos(#theta_{D*})");
  HTotalYieldVsCosDstar.SetTitle("");
  HTotalYieldVsCosDstar.Draw("pe");
  Canvas.Print(filename);



  Canvas.Print(filename+"]");

  return 1;
}



Float_t DstPiAnalysis::GraphXvsY(Int_t N,Double_t* X,Double_t*Y,Double_t* XE,Double_t*YE,TString xtitle,TString ytitle,Float_t finalres,Float_t finalresEr,Float_t gen){
  Canvas.Clear();
  TGraphErrors Graph(N,X,Y,XE,YE);
  Graph.SetMarkerSize(2);
  Graph.Draw("ape");
  Graph.SetTitle("");
  Graph.GetYaxis()->SetTitle(ytitle);
  Graph.GetXaxis()->SetTitle(xtitle);
  Graph.GetYaxis()->SetNdivisions(4);
  TBox box(Graph.GetXaxis()->GetXmin(),finalres-finalresEr,Graph.GetXaxis()->GetXmax(),finalres+finalresEr);
  box.SetFillColor(kGreen-7);
  box.SetFillStyle(1001);  
  if(finalres!=0&&finalresEr!=0){     
    box.Draw();
  }
  Graph.Draw("pesame");
  cutline.SetLineColor(2);
  cutline.SetLineWidth(2);
  if(gen!=0)
    cutline.DrawLine(Graph.GetXaxis()->GetXmin(),gen,Graph.GetXaxis()->GetXmax(),gen);
  Canvas.Update();
  Canvas.Print(filename);
  cout<<setprecision(5);
  Float_t avgdiff=0;
  if(Y&&YE){    
    Float_t diff=0;
    Float_t differ=1;
    for(Int_t i=0;i<N;i++){     
      diff=fabs(Y[i]-finalres);
      differ=sqrt(YE[i]*YE[i]+finalresEr*finalresEr);
      Float_t nsig=diff/differ;
      cout<<i<<" "<<Y[i]<<" "<<YE[i]<<" "<<finalres<<" "<<finalresEr<<" "<<diff<<" "<<differ<<" "<<nsig<<" add="<<(nsig/(nsig +1.)) * diff / N<<endl;
      avgdiff+=(nsig/(nsig +1.)) * diff / N;
    }
    
    cout<<" Weight Avg Delta: "<<xtitle<<" "<<ytitle; printf("        %.3f\n",avgdiff);
  }

  return avgdiff;
}



Int_t DstPiAnalysis::OpenFitPars(TString filename){
  readpars.open((const char*)(_OutputDir+"/"+filename));  
  if(!readpars.is_open()){cout<<"bad fit pars file:"<<filename<<endl; return 0;}  
  cout<<" opened fit pars file "<<_OutputDir+"/"+filename<<endl;
  return 1;
}

Int_t DstPiAnalysis::CloseFitPars(){
  readpars.close();
  readpars.clear();
  return 1;
}

Float_t DstPiAnalysis::GetFitPar(TString signalname,TString parametername){
  
  if(!readpars.is_open()){cout<<"fit pars file not open"<<endl; return 0;}
  readpars.clear();
  readpars.seekg(0,fstream::beg);

  Float_t fitpar=0;
  TString signame;  
  Int_t ntries=0;
  Bool_t foundpar=0;
  TString parname;
  Bool_t END=0;
  while(!foundpar&&!readpars.eof()&&ntries<1000&&!END){
    readpars>>signame>>parname>>fitpar;
    if(signame==signalname&&parname==parametername)
      foundpar=1;
    
    if(signame=="END")END=1;
    ntries++;
  }
  if(!foundpar){cout<<"fit par not found:"<<signalname<<" "<<parametername<<endl;return 0;}
  
  return fitpar;
}


