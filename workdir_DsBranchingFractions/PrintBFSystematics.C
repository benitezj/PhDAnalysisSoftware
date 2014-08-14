//#include <stdio.h>
#include <iomanip>
#include <TROOT.h>
#include <TString.h>
#include <TH2F.h>
#include <TFile.h>
#include "RooFitResult.h"
#include "RooRealVar.h"
#include "../tmp/Linux24SL3_i386_gcc323/DRecoilToolsUser/NtupleCode/DRecoDsInclusiveAnalysis.hh"

//#include "../tmp/Linux24SL3_i386_gcc323/DRecoilToolsUser/NtupleCode/DRecoDsInclusiveAnalysis.hh"
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


void DetermineMuNuBFSystematics(){    

  
  //Nominal BF
  TFile FDenominatorFit("/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResults/DenominatorFitResult_Data.root","read");  
  TFile FNumeratorFit("/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResults/MuNuFitResult_Data.root","read");
  TFile FDenominatorEff("/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResults/DenominatorEff.root","read");
  TFile FNumeratorEff("/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResults/MuNuEffNumerator.root","read");
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
	//<<" & (-"<<100.*diffMinus[s]<<",+"<<100.*diffPlus[s]<<")"
	  <<" &  "
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
    //<<" & {\\bf (-"<<100*totaldiffMinus<<",+"<<100*totaldiffPlus<<")}"
      <<" &  "
      <<" & {\\bf (-"<<100*totaldiffMinus/BFNom<<",+"<<100*totaldiffPlus/BFNom<<")\\%}"
      <<"\\\\"<<endl;
  
}



