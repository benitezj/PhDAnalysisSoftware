#include "../tmp/Linux24SL3_i386_gcc323/DRecoilToolsUser/NtupleCode/DRecoDsInclusiveAnalysis.hh"
#include "DsInclusiveAnalysisInit.C"
#include <TString.h>
#include <TFile.h>
//#include <stdio>
#include <iomanip>
using std::ostream;
using namespace std;
#define NSys 20

void DetermineMuNuBFSystematics(){
    
  DRecoDsInclusiveAnalysis*a=analyzeDsInclusive(12,0);

  TString SysName[NSys+1];
  TString SDenominatorFit[NSys+1];
  TString SNumeratorFit[NSys+1];
  TString SDenominatorEff[NSys+1];
  TString SNumeratorEff[NSys+1];
  TString SDalitzWeight[NSys+1];
  Float_t SysScaleFactor[NSys+1];

  ///Nominal 
  SysName[0]="Nominal";
  SDenominatorFit[0]="/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResults/DenominatorFitResult_Data.root";
  SNumeratorFit[0]="/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResults/MuNuFitResult_Data.root";
  //SDenominatorEff[0]="/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResults/MuNuEffDenominator.root";
  SDenominatorEff[0]="/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResults/DenominatorEff.root";
  SNumeratorEff[0]="/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResults/MuNuEffNumerator.root";

  //set all to nominal
  for(Int_t s=1;s<=NSys;s++){
    SysName[s]="";
    SDenominatorFit[s]=SDenominatorFit[0];
    SNumeratorFit[s]=SNumeratorFit[0];
    SDenominatorEff[s]=SDenominatorEff[0];
    SNumeratorEff[s]=SNumeratorEff[0];
    SDalitzWeight[s]=SDalitzWeight[0];
    SysScaleFactor[s]=1.;
  }

  ////All The  Denominator Systematics
  SysName[1]="WS $n_X^T$ weights";
  SDenominatorFit[1]="DsTolnu/Data/Final/Systematics/WSWeights/FitResult.root";

  SysName[2]="RS $n_X^T$ weights";
  SDenominatorFit[2]="DsTolnu/Data/Final/Systematics/RSWeights/FitResult_MinusGradient.root";

  SysName[3]="D$_s$ Signal Shape";
  SDenominatorFit[3]="DsTolnu/Data/Final/Systematics/DsSigShape/FitResult.root";
  SysScaleFactor[3]=451.891/234.867;

  SysName[4]="Peaking Backgrounds";
  SDenominatorFit[4]="DsTolnu/Data/Final/Systematics/WSSigYield/FitResult.root";

  SysName[5]="Background Model";
  SDenominatorFit[5]="DsTolnu/Data/Final/Systematics/BackgroundModel/FitResult.root";
  SysScaleFactor[5]=829./234.867;

  SysName[6]="$n_X$ Resolution";
  SDenominatorFit[6]="DsTolnu/Data/Final/Systematics/nXResolution/FitResult.root";

  SysName[7]="Signal Photon Backgrounds";
  SDenominatorFit[7]="DsTolnu/Data/Final/Systematics/PhotonPi0Bkg/FitResult.root";


  //compute all the BF's
  Float_t BF[NSys];
  Int_t syscounter=0;//start at -1 because of nominal
  for(Int_t s=0;s<=NSys;s++){
    if(SysName[s]!=""){
     
      cout<<" ------------ Computation of BF for Systematic: "<<SysName[s]<<endl;
      
      TFile FDenominatorFit(SDenominatorFit[s],"read");  
      TFile FNumeratorFit(SNumeratorFit[s],"read");
      TFile FDenominatorEff(SDenominatorEff[s],"read");
      TFile FNumeratorEff(SNumeratorEff[s],"read");
      
      BF[s]= a->ComputeBF(&FDenominatorFit,&FNumeratorFit,&FDenominatorEff,&FNumeratorEff);

      //sanity check
      if(BF[s]<.001){
	cout<<"Bad BF for systematic : "<<SysName[s]<<endl;
	return;
      }

      syscounter++;
    }
  }


//   //------------------------------------------------------------------
//   //Now add the systematics on the MuNu yield
//   //------------------------------------------------------------------
//   //MuNu background model
//   SysName[8]="$\\mu\\nu$ Background Model";
//   BF[8]=BF[0]*(1.+.001);

//   //MuNu Signal model
//   SysName[9]="$\\mu\\nu$ Signal Model";
//   BF[9]=BF[0]*(1.+.001);


//   //------------------------------------------------------------------
//   ///Now add the Systematics on the MuNu effciency
//   //-------------------------------------------------------------
//   //tracking systematic .17% per track
//   SysName[10]="$\\mu$ Tracking Efficiency";
//   BF[10]=BF[0]*(1.+.001);

//   //PID systematic 
//   SysName[11]="$\\mu$ PID Efficiency";
//   BF[11]=BF[0]*(1.+.001);


  cout<<endl<<endl<<endl<<"------------------------------------Summary-------------------------"<<endl;

  ///Print differences 
  Float_t diff[NSys];
  Float_t totaldiff=0.;
  cout<<setprecision(2)<<fixed;
  for(Int_t s=1;s<=NSys;s++){
    if(SysName[s]!=""){
      diff[s]=fabs(BF[s]-BF[0])/SysScaleFactor[s];
      cout<<SysName[s]
	  <<" & "<<1000.*diff[s] 
	  <<" & "<<100.*diff[s]/BF[0]<<"\\% "
	  <<"\\\\"<<endl; 
      
      //sanity check
      if(diff[s]/BF[0]>.1){
	cout<<"Difference too large for systematic : "<<SysName[s]<<endl;
	return;
      }
      
      totaldiff+=pow(diff[s],2); 
    }
  }  
  totaldiff=sqrt(totaldiff);

  cout<<"\\hline"<<endl
      <<"{\\bf Total}"
      <<" & {\\bf "<<1000.*totaldiff
      <<"} & {\\bf "<<100*totaldiff/BF[0]<<"\\%} \\\\"<<endl;
  
}
