#include "../tmp/Linux26SL5_i386_gcc412/DRecoilToolsUser/NtupleCode/DRecoDsInclusiveAnalysis.hh"
#include "DsInclusiveAnalysisInit.C"
#include <TString.h>
#include <TFile.h>
//#include <stdio>
#include <iomanip>
using std::ostream;
using namespace std;
#define NSys 20

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


void DetermineKKpiBFSystematics(){
    
  DRecoDsInclusiveAnalysis a(2,"DsTolnu/Data/Final");
  
  //Nominal BF
  TFile FDenominatorFit("DsTolnu/Data/Final/Systematics/Nominal/FitResult.root","read");  
  TFile FNumeratorFit("DsTolnu/KKpi/Data/FitResults.root","read");
  TFile FDenominatorEff("DsTolnu/KKpiEff/HistosForFit.root","read");
  TFile FNumeratorEff("DsTolnu/KKpi/Signal/HistosForFit.root","read");
  TFile FDalitzWeight("DsTolnu/KKpi/NoDenom/FitResults.root","read");  
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
      
      //DsTolnu/Data/Final/Systematics/
      TFile FDenominatorFitPlus(TString("DsTolnu/Data/Final/Systematics/")+SDenominatorFolder[s]+"/FitResult_Plus.root","read");  
      BFPlus[s]= a.ComputeBFKKpiDalitz(&FDenominatorFitPlus,&FNumeratorFit,&FDenominatorEff,&FNumeratorEff,&FDalitzWeight);

      //DsTolnu/Data/Final/Systematics/
      TFile FDenominatorFitMinus(TString("DsTolnu/Data/Final/Systematics/")+SDenominatorFolder[s]+"/FitResult_Minus.root","read");  
      BFMinus[s]= a.ComputeBFKKpiDalitz(&FDenominatorFitMinus,&FNumeratorFit,&FDenominatorEff,&FNumeratorEff,&FDalitzWeight);
    }
  }


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
	//<<" & (-"<<100.*diffMinus[s]<<",+"<<100.*diffPlus[s]<<")"
	  <<" &  "
	  <<" & (-"<<100.*diffMinus[s]/BFNom<<",+"<<100.*diffPlus[s]/BFNom<<")\\% "
	  <<"\\\\"<<endl; 
      
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
    //<<" & {\\bf (-"<<100*totaldiffMinus<<",+"<<100*totaldiffPlus<<")}"
      <<" &  "
      <<" & {\\bf (-"<<100*totaldiffMinus/BFNom<<",+"<<100*totaldiffPlus/BFNom<<")\\%}"
      <<"\\\\"<<endl;
  
}
