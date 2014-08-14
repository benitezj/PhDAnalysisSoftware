//#include <stdio.h>
#include <iomanip>
#include <TROOT.h>
#include <TString.h>
#include <TH2F.h>
#include <TFile.h>
#include "RooFitResult.h"
#include "RooRealVar.h"
#include "DsInclusiveAnalysisInit.C"

//#include "../tmp/Linux26SL5_i386_gcc412/DRecoilToolsUser/NtupleCode/DRecoDsInclusiveAnalysis.hh"
using std::ostream;
using namespace std;
#define NSys 20

Float_t Max2(Float_t N1,Float_t N2){
  if(fabs(N1)>fabs(N2))return fabs(N1);
  else return fabs(N2);
}
Float_t Neg2(Float_t N1,Float_t N2){
  if(N1<0 && N2<0 )return (fabs(N1)>fabs(N2)? fabs(N1) : fabs(N2));
  if(N1<0)return fabs(N1);
  if(N2<0)return fabs(N2);
  return 0.;
}
Float_t Pos2(Float_t N1,Float_t N2){
  if(N1>0 && N2>0 )return (fabs(N1)>fabs(N2)? fabs(N1) : fabs(N2));
  if(N1>0)return fabs(N1);
  if(N2>0)return fabs(N2);
  return 0.;
}



Int_t PrintSystematics(Bool_t remake=0){
  
  TString SysName[NSys];
  SysName[0]="WS $n_X^T$ weights";//one sided because Data prefers lower nX
  SysName[1]="RS $n_X^T$ weights";//alpha parameter varied between -10 and -.5
  SysName[2]="D$_s$ Signal Shape";//resolution as worsened and same difference is used for better resolution
  SysName[3]="Peaking Backgrounds";//currently this systematic accounts for both wrong fraction of WS/RS and D*0-->D0gamma
  SysName[4]="Background Model";//multiply and divide by RS/WS bkg ratio
  SysName[5]="$n_X$ Resolution";//worse n_X resolution
  SysName[6]="Signal Photon Backgrounds";//worse photon background

  TString SysFolder[NSys];
  SysFolder[0]="WSWeights";
  SysFolder[1]="RSWeights";
  SysFolder[2]="DsSigShape";
  SysFolder[3]="WSSigYield";
  SysFolder[4]="BackgroundModel";
  SysFolder[5]="nXResolution";
  SysFolder[6]="PhotonPi0Bkg";

  Float_t NominalYield;
  Float_t NominalYieldFull;
  //Float_t NominalChisq;
  Float_t SysPlusYield[NSys];  
  Float_t SysMinusYield[NSys];
  Float_t SysPlusYieldFull[NSys];  
  Float_t SysMinusYieldFull[NSys];
  //Float_t SysPlusChisq[NSys]; 
  //Float_t SysMinusChisq[NSys];
  Float_t SysScaleFactor[NSys];

  //rerun all the fits
  if(remake){
    for(Int_t i=0;i<NSys;i++){
      if(SysName[i]!=""){
	system(TString("rm -f DsTolnu/Data/Final/Systematics/")+SysFolder[i]+"/*");

	//positive variation
	DRecoDsInclusiveAnalysis* ap=new DRecoDsInclusiveAnalysis(2,"DsTolnu/Data/Final",i+1);
	ap->RooFitData();
	ap->PlotFitDsMassvsnXRvsnXTSimul();
	delete ap;
	gROOT->ProcessLine(TString(".cp DsTolnu/Data/Final/FitResult.root DsTolnu/Data/Final/Systematics/")+SysFolder[i]+"/FitResult_Plus.root");
	gROOT->ProcessLine(TString(".cp DsTolnu/Data/Final/PlotFitDsMassvsnXRvsnXTSimul.ps DsTolnu/Data/Final/Systematics/")+SysFolder[i]+"/PlotFitDsMassvsnXRvsnXTSimul_Plus.ps");
	gROOT->Reset();
	gROOT->Clear();	


	//negative variation
	DRecoDsInclusiveAnalysis* am=new DRecoDsInclusiveAnalysis(2,"DsTolnu/Data/Final",-(i+1));
	am->RooFitData();
	am->PlotFitDsMassvsnXRvsnXTSimul();
	delete am;
	gROOT->ProcessLine(TString(".cp DsTolnu/Data/Final/FitResult.root DsTolnu/Data/Final/Systematics/")+SysFolder[i]+"/FitResult_Minus.root");
	gROOT->ProcessLine(TString(".cp DsTolnu/Data/Final/PlotFitDsMassvsnXRvsnXTSimul.ps DsTolnu/Data/Final/Systematics/")+SysFolder[i]+"/PlotFitDsMassvsnXRvsnXTSimul_Minus.ps");
	gROOT->Reset();
	gROOT->Clear();


      }
    }
  }
  
  //Read in the yields
  TFile FDsFit("DsTolnu/Data/Final/Systematics/Nominal/FitResult.root","read");
  RooFitResult* DsFitResult=(RooFitResult*)FDsFit.Get("fitresult");
  if(!DsFitResult){cout<<"No DsFitResult"<<endl;return 0;}
  TH2F*HDsMassData=(TH2F*)FDsFit.Get("HDsMassData");
  if(!HDsMassData){cout<<"No HDsMassData "<<endl;return 0;}
  RooRealVar*DsYield=(RooRealVar*)DsFitResult->floatParsFinal().find("TotYield");
  if(!DsYield){cout<<"DsYield not found"<<endl;return 0;}
  TH2F* TotSigPdfHistoFit=(TH2F*)FDsFit.Get("TotSigPdfHistoFit");
  if(!TotSigPdfHistoFit){cout<<"No TotSigPdfHistoFit"<<endl;return 0;}
  NominalYield=HDsMassData->Integral()*DsYield->getVal()*TotSigPdfHistoFit->Integral(1,4,DenominatorDsMassBinMin,DenominatorDsMassBinMax)/TotSigPdfHistoFit->Integral()/1000.;
  NominalYieldFull=HDsMassData->Integral()*DsYield->getVal()/1000.;
    
  for(Int_t i=0;i<NSys;i++){
    if(SysName[i]!=""){
      //positive variations 
      TFile FDsFitPlus(TString("DsTolnu/Data/Final/Systematics/")+SysFolder[i]+"/FitResult_Plus.root","read");
      DsFitResult=(RooFitResult*)FDsFitPlus.Get("fitresult");
      if(!DsFitResult){cout<<"No DsFitResult"<<endl;return 0;}
      HDsMassData=(TH2F*)FDsFitPlus.Get("HDsMassData");
      if(!HDsMassData){cout<<"No HDsMassData "<<endl;return 0;}
      DsYield=(RooRealVar*)DsFitResult->floatParsFinal().find("TotYield");
      if(!DsYield){cout<<"DsYield not found"<<endl;return 0;}
      TotSigPdfHistoFit=(TH2F*)FDsFitPlus.Get("TotSigPdfHistoFit");
      if(!TotSigPdfHistoFit){cout<<"No TotSigPdfHistoFit"<<endl;return 0;}
      SysPlusYield[i]=HDsMassData->Integral()*DsYield->getVal()*TotSigPdfHistoFit->Integral(1,4,DenominatorDsMassBinMin,DenominatorDsMassBinMax)/TotSigPdfHistoFit->Integral()/1000.;
      SysPlusYieldFull[i]=HDsMassData->Integral()*DsYield->getVal()/1000.;

      //negative variations
      TFile FDsFitMinus(TString("DsTolnu/Data/Final/Systematics/")+SysFolder[i]+"/FitResult_Minus.root","read");
      DsFitResult=(RooFitResult*)FDsFitMinus.Get("fitresult");
      if(!DsFitResult){cout<<"No DsFitResult"<<endl;return 0;}
      HDsMassData=(TH2F*)FDsFitMinus.Get("HDsMassData");
      if(!HDsMassData){cout<<"No HDsMassData "<<endl;return 0;}
      DsYield=(RooRealVar*)DsFitResult->floatParsFinal().find("TotYield");
      if(!DsYield){cout<<"DsYield not found"<<endl;return 0;}
      TotSigPdfHistoFit=(TH2F*)FDsFitMinus.Get("TotSigPdfHistoFit");
      if(!TotSigPdfHistoFit){cout<<"No TotSigPdfHistoFit"<<endl;return 0;}
      SysMinusYield[i]=HDsMassData->Integral()*DsYield->getVal()*TotSigPdfHistoFit->Integral(1,4,DenominatorDsMassBinMin,DenominatorDsMassBinMax)/TotSigPdfHistoFit->Integral()/1000.;
      SysMinusYieldFull[i]=HDsMassData->Integral()*DsYield->getVal()/1000.;
      
    }
  }


  //modify the Ds Signal shape systematic the minus variation should be equal to the plus variation
  SysPlusYield[2]=NominalYield-(SysMinusYield[2]-NominalYield);//for better resolution assumme same change
  SysPlusYieldFull[2]=NominalYieldFull-(SysMinusYieldFull[2]-NominalYieldFull);
  cout<<"modified the Ds Sig Shap systematic"<<endl;



  ///Print table
  Float_t totaldiffm=0.;
  Float_t totaldiffp=0.;
  cout<<"    Systematic    Difference   % Difference "<<endl;
  cout<<setprecision(2)<<fixed;
  for(Int_t i=0;i<NSys;i++){
    if(SysName[i]!=""){
      SysScaleFactor[i]=1;
      
      cout<<SysName[i]
	  <<" & (-"<<Neg2(SysPlusYield[i] - NominalYield,SysMinusYield[i] - NominalYield)*SysScaleFactor[i] 
	  <<",+"<<Pos2(SysPlusYield[i] - NominalYield,SysMinusYield[i] - NominalYield)*SysScaleFactor[i] 
	  <<") & (-"<<100*Neg2(SysPlusYield[i] - NominalYield,SysMinusYield[i] - NominalYield)*SysScaleFactor[i]/NominalYield
	  <<",+"<<100*Pos2(SysPlusYield[i] - NominalYield,SysMinusYield[i] - NominalYield)*SysScaleFactor[i]/NominalYield
	  <<")\\% \\\\"<<endl; 
      totaldiffm+=pow(Neg2(SysPlusYield[i] - NominalYield,SysMinusYield[i] - NominalYield)*SysScaleFactor[i],2); 
      totaldiffp+=pow(Pos2(SysPlusYield[i] - NominalYield,SysMinusYield[i] - NominalYield)*SysScaleFactor[i],2); 
    }
  }    
  cout<<"\\hline"<<endl
      <<"{\\bf Total}"
      <<" & {\\bf (-"<<sqrt(totaldiffm)
      <<",+"<<sqrt(totaldiffp)
      <<")} & {\\bf (-"<<100*sqrt(totaldiffm)/NominalYield
      <<",+"<<100*sqrt(totaldiffp)/NominalYield
      <<")\\%} \\\\"<<endl;

  

  ///Print Full range systematics
  cout<<endl<<endl<<endl;
  Float_t totaldiffmfull=0.;
  Float_t totaldiffpfull=0.;
  for(Int_t i=0;i<NSys;i++){
    if(SysName[i]!=""){

      cout<<SysName[i]
	  <<" & (-"<<Neg2(SysPlusYieldFull[i] - NominalYieldFull,SysMinusYieldFull[i] - NominalYieldFull)*SysScaleFactor[i] 
	  <<",+"<<Pos2(SysPlusYieldFull[i] - NominalYieldFull,SysMinusYieldFull[i] - NominalYieldFull)*SysScaleFactor[i] 
	  <<") & (-"<<100*Neg2(SysPlusYieldFull[i] - NominalYieldFull,SysMinusYieldFull[i] - NominalYieldFull)*SysScaleFactor[i]/NominalYieldFull
	  <<",+"<<100*Pos2(SysPlusYieldFull[i] - NominalYieldFull,SysMinusYieldFull[i] - NominalYieldFull)*SysScaleFactor[i]/NominalYieldFull
	  <<")\\% \\\\"<<endl; 
      totaldiffmfull+=pow(Neg2(SysPlusYieldFull[i] - NominalYieldFull,SysMinusYieldFull[i] - NominalYieldFull)*SysScaleFactor[i],2); 
      totaldiffpfull+=pow(Pos2(SysPlusYieldFull[i] - NominalYieldFull,SysMinusYieldFull[i] - NominalYieldFull)*SysScaleFactor[i],2); 
    }
  }    
  cout<<"\\hline"<<endl
      <<"{\\bf Total}"
      <<" & {\\bf (-"<<sqrt(totaldiffmfull)
      <<",+"<<sqrt(totaldiffpfull)
      <<")} & {\\bf (-"<<100*sqrt(totaldiffmfull)/NominalYieldFull
      <<",+"<<100*sqrt(totaldiffpfull)/NominalYieldFull
      <<")\\%} \\\\"<<endl;



  return 1;  
}


void DetermineMuNuBFSystematics(){    

  
  //Nominal BF
  TFile FDenominatorFit("/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResults/DenominatorFitResult_Data.root","read");  
  TFile FNumeratorFit("/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResults/NumeratorFitResults_Data.root","read");
  TFile FDenominatorEff("/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResults/DenominatorEff.root","read");
  TFile FNumeratorEff("/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResults/NumeratorEfficiencies.root","read");
  gROOT->cd();


  DRecoDsInclusiveAnalysis a(2,"DsTolnu/Data/Final");
  Float_t BFNom= a.ComputeBF(&FDenominatorFit,&FNumeratorFit,&FDenominatorEff,&FNumeratorEff);


  //Systematics
  TString SysName[NSys];
  TString SDenominatorFolder[NSys];

  //set all to empty
  for(Int_t s=0;s<NSys;s++){
    SysName[s]="";
    SDenominatorFolder[s]="";
  }

  ////All The  Denominator Systematics
  SysName[0]="WS $n_X^T$ weights";
  SDenominatorFolder[0]="WSWeights";

  SysName[1]="RS $n_X^T$ weights";
  SDenominatorFolder[1]="RSWeights";

  SysName[2]="D$_s$ Signal Shape";
  SDenominatorFolder[2]="DsSigShape";

  SysName[3]="Peaking Backgrounds";
  SDenominatorFolder[3]="WSSigYield";

  SysName[4]="Background Model";
  SDenominatorFolder[4]="BackgroundModel";

  SysName[5]="$n_X$ Resolution";
  SDenominatorFolder[5]="nXResolution";

  SysName[6]="Signal Photon Backgrounds";
  SDenominatorFolder[6]="PhotonPi0Bkg";


  //compute all the BF's
  Float_t BFPlus[NSys];
  Float_t BFMinus[NSys];
  for(Int_t s=0;s<NSys;s++){
    if(SysName[s]!=""){
      cout<<" ------------ Computation of BF for Systematic: "<<SysName[s]<<endl;
      DRecoDsInclusiveAnalysis a(2,"DsTolnu/Data/Final");//renew each time so parameters get reset (nXTa)
      //
      TFile FDenominatorFitPlus(TString("DsTolnu/Data/Final/Systematics/")+SDenominatorFolder[s]+"/FitResult_Plus.root","read");  
      FDenominatorFitPlus.ls();
      gROOT->cd();
      BFPlus[s]= a.ComputeBF(&FDenominatorFitPlus,&FNumeratorFit,&FDenominatorEff,&FNumeratorEff);

      //
      TFile FDenominatorFitMinus(TString("DsTolnu/Data/Final/Systematics/")+SDenominatorFolder[s]+"/FitResult_Minus.root","read");  
      FDenominatorFitMinus.ls();
      gROOT->cd();
      BFMinus[s]= a.ComputeBF(&FDenominatorFitMinus,&FNumeratorFit,&FDenominatorEff,&FNumeratorEff);
    }
  }

  //fix the Ds resolution systematic which is assigned symetric error using worse resolution
  BFMinus[2]=BFNom+fabs(BFNom-BFPlus[2]);


//   //------------------------------------------------------------------
//   //Now add the systematics on the MuNu yield
//   //------------------------------------------------------------------
//   //Background model : 3.49%
// // Tracking: 0.23%
// // PID: 0.84%
// // Cuts: 1.95%

//   SysName[7]="$\\mu\\nu$ Background Model";
//   BFPlus[7]=BFNom*(1.+.0349);
//   BFMinus[7]=BFNom*(1.-.0349);

// //   //
// //   SysName[8]="$\\mu\\nu$ Signal Model";
// //   BFPlus[8]=BFNom*(1.+.0001);
// //   BFMinus[8]=BFNom*(1.-.0001);


//   //------------------------------------------------------------------
//   ///Now add the Systematics on the effciency
//   //-------------------------------------------------------------
//   //
//   SysName[9]="$\\mu$ Tracking Efficiency";
//   BFPlus[9]=BFNom*(1.+.0023);
//   BFMinus[9]=BFNom*(1.-.0023);

//   //
//   SysName[10]="$\\mu$ PID Efficiency";
//   BFPlus[10]=BFNom*(1.+.0084);
//   BFMinus[10]=BFNom*(1.-.0084);

//   //
//   SysName[11]="$\\mu\\nu$ Selections";
//   BFPlus[11]=BFNom*(1.+.0195);
//   BFMinus[11]=BFNom*(1.-.0195);



  cout<<setprecision(5)<<fixed;
  cout<<"Nominal  --> "<<BFNom<<endl;
  for(Int_t s=0;s<NSys;s++){
    if(SysName[s]!=""){
      cout<< SysName[s]<<"  --> "<<BFPlus[s]<<" "<< BFMinus[s]<<endl;
    }
  }
  cout<<endl<<endl<<endl<<"------------------------------------Summary-------------------------"<<endl;





  ///Print differences 
  Float_t diffPlus[NSys];
  Float_t totaldiffPlus=0.;
  Float_t diffMinus[NSys];
  Float_t totaldiffMinus=0.;
  cout<<setprecision(2)<<fixed;
  for(Int_t s=0;s<NSys;s++){
    if(SysName[s]!=""){
      diffPlus[s]=fabs(Pos2(BFPlus[s]-BFNom,BFMinus[s]-BFNom));
      diffMinus[s]=fabs(Neg2(BFPlus[s]-BFNom,BFMinus[s]-BFNom));

      cout<<SysName[s]
	  <<" & (-"<<1000.*diffMinus[s]<<",+"<<1000.*diffPlus[s]<<")"
	  <<" & (-"<<100.*diffMinus[s]/BFNom<<",+"<<100.*diffPlus[s]/BFNom<<")\\% "
	  <<"\\\\"<<endl; 
      
      //sanity check
      if(diffPlus[s]/BFNom>.1||diffMinus[s]/BFNom>.1){
	cout<<"!!!!!!Difference too large for systematic : "<<SysName[s]<<endl;
	//return;
      }
      
      totaldiffPlus+=pow(diffPlus[s],2); 
      totaldiffMinus+=pow(diffMinus[s],2); 
    }
  }  
  totaldiffPlus=sqrt(totaldiffPlus);
  totaldiffMinus=sqrt(totaldiffMinus);

  cout<<"\\hline"<<endl
      <<"{\\bf Total}"
      <<" & {\\bf (-"<<100*totaldiffMinus<<",+"<<100*totaldiffPlus<<")}"
      <<" &  "
      <<" & {\\bf (-"<<100*totaldiffMinus/BFNom<<",+"<<100*totaldiffPlus/BFNom<<")\\%}"
      <<"\\\\"<<endl;
  
}




void DetermineKKpiBFSystematics(){
    
  DRecoDsInclusiveAnalysis a(2,"DsTolnu/Data/Final");
  //DRecoDsInclusiveAnalysis*a=analyzeDsInclusive(12,0);
  //Nominal BF
//   TFile FDenominatorFit("DsTolnu/Data/Final/Systematics/Nominal/FitResult.root","read");  
//   TFile FNumeratorFit("DsTolnu/KKpi/Data/FitResults.root","read");
//   TFile FDenominatorEff("DsTolnu/KKpiEff/HistosForFit.root","read");
//   TFile FNumeratorEff("DsTolnu/KKpi/Signal/HistosForFit.root","read");
//   TFile FDalitzWeight("DsTolnu/KKpi/NoDenom/FitResults.root","read");  

  TFile FDenominatorFit("DsTolnu/Data/Final/Systematics/Nominal/FitResult.root","read");  
  //TFile FDenominatorFit("/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResults/DenominatorFitResult_Data.root","read");
  TFile FNumeratorFit("/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResultsKKpi/KKpiFitResult_Data.root","read");
  TFile FDenominatorEff("/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResults/DenominatorEff.root","read");
  TFile FNumeratorEff("/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResultsKKpi/KKpiEffNumerator.root","read");
  TFile FDalitzWeight("/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResultsKKpi/DalitzWeights.root","read");  
  
  Float_t BFNom= a.ComputeBFKKpiDalitz(&FDenominatorFit,&FNumeratorFit,&FDenominatorEff,&FNumeratorEff,&FDalitzWeight);

  //Systematics
  TString SysName[NSys];
  TString SDenominatorFolder[NSys];

  //set all to empty
  for(Int_t s=0;s<NSys;s++){
    SysName[s]="";
    SDenominatorFolder[s]="";
  }

  ////All The  Denominator Systematics
  SysName[0]="WS $n_X^T$ weights";
  SDenominatorFolder[0]="WSWeights";

  SysName[1]="RS $n_X^T$ weights";
  SDenominatorFolder[1]="RSWeights";

  SysName[2]="D$_s$ Signal Shape";
  SDenominatorFolder[2]="DsSigShape";

  SysName[3]="Peaking Backgrounds";
  SDenominatorFolder[3]="WSSigYield";

  SysName[4]="Background Model";
  SDenominatorFolder[4]="BackgroundModel";

  SysName[5]="$n_X$ Resolution";
  SDenominatorFolder[5]="nXResolution";

  SysName[6]="Signal Photon Backgrounds";
  SDenominatorFolder[6]="PhotonPi0Bkg";


  //compute all the BF's
  Float_t BFPlus[NSys];
  Float_t BFMinus[NSys];
  for(Int_t s=0;s<7;s++){
    if(SysName[s]!=""){
      cout<<" ------------ Computation of BF for Systematic: "<<SysName[s]<<endl;
      DRecoDsInclusiveAnalysis a(2,"DsTolnu/Data/Final");
      //
      TFile FDenominatorFitPlus(TString("DsTolnu/Data/Final/Systematics/")+SDenominatorFolder[s]+"/FitResult_Plus.root","read");  
      BFPlus[s]= a.ComputeBFKKpiDalitz(&FDenominatorFitPlus,&FNumeratorFit,&FDenominatorEff,&FNumeratorEff,&FDalitzWeight);

      //
      TFile FDenominatorFitMinus(TString("DsTolnu/Data/Final/Systematics/")+SDenominatorFolder[s]+"/FitResult_Minus.root","read");  
      BFMinus[s]= a.ComputeBFKKpiDalitz(&FDenominatorFitMinus,&FNumeratorFit,&FDenominatorEff,&FNumeratorEff,&FDalitzWeight);
    }
  }

  //fix the Ds resolution systematic which is assigned symetric error using worse resolution
  BFMinus[2]=BFNom+fabs(BFNom-BFPlus[2]);

  //------------------------------------------------------------------
  //Now add the systematics on the KKpi yield
  //------------------------------------------------------------------
  //KKpi background model
  SysName[7]="KK$\\pi$ Background Model";
  BFPlus[7]=BFNom*(1.+.007);
  BFMinus[7]=BFNom*(1.-.007);

  //KKpi Signal model
  SysName[8]="KK$\\pi$ Signal Model";
  BFPlus[8]=BFNom*(1.+.003);
  BFMinus[8]=BFNom*(1.-.003);


  //------------------------------------------------------------------
  ///Now add the Systematics on the KKpi effciency
  //-------------------------------------------------------------
  //tracking systematic .17% per track * 3 tracks (correlated)
  SysName[9]="KK$\\pi$ Tracking Efficiency";
  BFPlus[9]=BFNom*(1.+3*.0017);
  BFMinus[9]=BFNom*(1.-3*.0017);

  //PID systematic 1.11% per Kaon * 2 Kaons (correlated)
  SysName[10]="KK$\\pi$ PID Efficiency";
  BFPlus[10]=BFNom*(1.+2*.0111);
  BFMinus[10]=BFNom*(1.-2*.0111);


  cout<<endl<<endl<<endl<<"------------------------------------Summary-------------------------"<<endl;

  ///Print differences 
  Float_t diffPlus[NSys];
  Float_t totaldiffPlus=0.;
  Float_t diffMinus[NSys];
  Float_t totaldiffMinus=0.;
  cout<<setprecision(2)<<fixed;
  for(Int_t s=0;s<NSys;s++){
    if(SysName[s]!=""){
      diffPlus[s]=fabs(Pos2(BFPlus[s]-BFNom,BFMinus[s]-BFNom));
      diffMinus[s]=fabs(Neg2(BFPlus[s]-BFNom,BFMinus[s]-BFNom));

      cout<<SysName[s]
	  <<setprecision(3)<<" & (-"<<100.*diffMinus[s]<<",+"<<100.*diffPlus[s]<<")"
	//<<" &  "
	  <<" & (-"<<100.*diffMinus[s]/BFNom<<",+"<<100.*diffPlus[s]/BFNom<<")\\% "
	  <<setprecision(3)<<"\\\\"<<endl; 
      
      //sanity check
      if(diffPlus[s]/BFNom>.1||diffMinus[s]/BFNom>.1){
	cout<<"Difference too large for systematic : "<<SysName[s]<<endl;
	return;
      }
      
      totaldiffPlus+=pow(diffPlus[s],2); 
      totaldiffMinus+=pow(diffMinus[s],2); 
    }
  }  
  totaldiffPlus=sqrt(totaldiffPlus);
  totaldiffMinus=sqrt(totaldiffMinus);

  cout<<"\\hline"<<endl
      <<"{\\bf Total}"
      <<setprecision(2)<<" & {\\bf (-"<<100*totaldiffMinus<<",+"<<100*totaldiffPlus<<")}"
    //<<" &  "
      <<setprecision(2)<<" & {\\bf (-"<<100*totaldiffMinus/BFNom<<",+"<<100*totaldiffPlus/BFNom<<")\\%}"
      <<"\\\\"<<endl;
  
}


