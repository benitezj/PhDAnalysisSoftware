

//
#include <TROOT.h>
#include <TMath.h>
#include <iostream>
#include <iomanip>
#include <TH1F.h>
#include <TGraphErrors.h>
#include <TBox.h>
#include <TCanvas.h>
#include <TLatex.h>
using namespace std;


//////Values to be be updated
#define BFMuNu 6.02e-3 //!!!!!!!!!!!!Update
#define BFMuNuErStat 0.38e-3 //!!!!!!!!!!!!Update
#define BFMuNuErSyst 0.34e-3 //!!!!!!!!!!!!Update

#define BFTauNuE 5.07e-2 //!!!!!!!!!!!!Update
#define BFTauNuEErStat 0.52e-2 //!!!!!!!!!!!!Update
#define BFTauNuEErSyst 0.68e-2 //!!!!!!!!!!!!Update

#define BFTauNuMu 4.91e-2 //!!!!!!!!!!!!Update
#define BFTauNuMuErStat 0.47e-2 //!!!!!!!!!!!!Update
#define BFTauNuMuErSyst 0.54e-2 //!!!!!!!!!!!!Update


//Values from other experiments
//BELLE 
#define BFMuNuBell 6.44e-3 
#define BFMuNuErStatBell 0.76e-3 
#define BFMuNuErSystBell 0.57e-3 

//CLEOc
#define BFMuNuCleo 5.65e-3 
#define BFMuNuErStatCleo 0.45e-3 
#define BFMuNuErSystCleo 0.17e-3 

//width=h/tau  
#define hbar 6.58211899e-25 // GeV s

// //PDG values
// #define tauDs 7.5963e+11 //=500e-15/hbar -->convert to GeV^-1
// #define massDs 1.96849 //GeV 
// #define massMu .105658367 //GeV
// #define massTau 1.77684 //GeV
// #define GF 1.16637e-5 //GeV^-2 
// //#define Vcs .97425 // set to Vud //(I.S. Towner and J.C. Hardy, Phys. Rev. C 79, 055502 (2009),  arXiv:0812.1202 [nucl-ex].) -->
// #define Vcs .97418 // set to Vud in PDG2008

//HFAG values
#define tauDs 7.5963e+11 //=500e-15/hbar -->convert to GeV^-1
#define massDs 1.96847 //GeV 
#define massMu 0.105658367 //GeV
#define massTau 1.77682 //GeV
#define GF 1.16637e-5 //GeV^-2 
#define Vcs 0.97345 



//errors
#define tauDsEr 1.06e+10 // =tauDs*sqrt(pow(0.00000010e-25/hbar,2)+pow(7e-15/500e-15,2)) 
#define massDsEr .00034 
#define massMuEr .000000004 
#define massTauEr .00017 
#define GFEr .000009e-5 
//#define VcsEr .00022 // set to Vud //(I.S. Towner and J.C. Hardy,
#define VcsEr .00027 // set to Vud in PDG2008


Float_t fDsStatTemp;
Float_t fDsSystTemp;


void ComputeErrorWeightAvg(Double_t X1,Double_t E1,
			   Double_t X2,Double_t E2,
			   Double_t X3=0,Double_t E3=0,
			   Double_t X4=0,Double_t E4=0,
			   Double_t X5=0,Double_t E5=0,
			   Double_t X6=0,Double_t E6=0
			   ){
  //compute error weighted average assuming uncorrelated errors

  Double_t W1=(E1>0)?1./(E1*E1):0;
  Double_t W2=(E2>0)?1./(E2*E2):0;
  Double_t W3=(E3>0)?1./(E3*E3):0;
  Double_t W4=(E4>0)?1./(E4*E4):0;
  Double_t W5=(E5>0)?1./(E5*E5):0;
  Double_t W6=(E6>0)?1./(E6*E6):0;
  Double_t WT=W1+W2+W3+W4+W5+W6;
 
  Double_t X=(X1*W1+X2*W2+X3*W3+X4*W4+X5*W5+X6*W6)/WT;

  Double_t E=sqrt(pow(E1*W1/WT,2) 
		 + pow(E2*W2/WT,2)		 
		 + pow(E3*W3/WT,2)		 
		 + pow(E4*W4/WT,2)		 
		 + pow(E5*W5/WT,2)		 
		 + pow(E6*W6/WT,2)		 
		 );


  if(E1>0)cout<<"X1="<<X1<<" E1="<<E1<<endl;
  if(E2>0)cout<<"X2="<<X2<<" E2="<<E2<<endl;
  if(E3>0)cout<<"X3="<<X3<<" E3="<<E3<<endl;
  if(E4>0)cout<<"X4="<<X4<<" E4="<<E4<<endl;
  if(E5>0)cout<<"X5="<<X5<<" E5="<<E5<<endl;
  if(E6>0)cout<<"X6="<<X6<<" E6="<<E6<<endl;

  cout<<X<<" $\\pm$ "<<E<<endl;
}


void ComputefDs(Double_t BF, Double_t BFSt, Double_t BFSy, Double_t massL, Double_t massLEr, Double_t * fDs, Double_t * fDsEr){
  //compute fDs using mu mass 
  
//   //Print out the PDG parameters:
//   cout<<scientific<<setprecision(5);
//   cout<<" GF="<<GF <<endl;
//   cout<<" m_l="<< massMu <<endl;
//   cout<<" m_Ds="<< massDs <<endl;
//   cout<<" tauDs="<< tauDs <<endl;
//   cout<<" Vcs="<<Vcs <<endl;



  //Compute the decay constant
  (*fDs)=sqrt((BF/tauDs)/(
			  (GF*GF*massDs*massDs*massDs/(8*TMath::Pi()))
			  *pow(massL/massDs,2)
			  *pow(1-massL*massL/(massDs*massDs),2)
			  *Vcs*Vcs
			  ));

  //Derivatives of (*fDs) wrt parameters
  Double_t dfdB=0.5*(*fDs)/BF;
  Double_t dfdtau=0.5*(*fDs)/tauDs;
  Double_t dfdGF=(*fDs)/GF;
  Double_t dfdMD=(sqrt(BF*8*TMath::Pi()/tauDs)/(GF*Vcs))*(1+3*massL*massL/(massDs*massDs))/(2*pow(massDs,(float)1.5)*pow(1-massL*massL/(massDs*massDs),(float)2.0));    
  Double_t dfdMMu=(sqrt(BF*8*TMath::Pi()/tauDs)/(GF*Vcs))*(1-3*massL*massL/(massDs*massDs))/(massL*massL*pow(1-massL*massL/(massDs*massDs),(float)2.0));
  Double_t dfdVcs=(*fDs)/Vcs;

  //errors Stat + Syst + PDG
  Double_t fDsStat=fabs(dfdB*BFSt);
  Double_t fDsSyst=fabs(dfdB*BFSy);
  Double_t fDsPDG=sqrt(pow(dfdtau*tauDsEr,(float)2)
		      +pow(dfdGF*GFEr,(float)2)
		      +pow(dfdMD*massDsEr,(float)2)
		      +pow(dfdMMu*massLEr,(float)2)
		      +pow(dfdVcs*VcsEr,(float)2));

  cout<<fixed<<setprecision(2);
  cout<<" fDs= "<<(*fDs)*1000<<" $\\pm$ "<<fDsStat*1000<<" $\\pm$ "<<fDsSyst*1000<<" $\\pm$ "<<fDsPDG*1000<<" \\mev"<<endl;  
  cout<<" fDs= "<<(*fDs)*1000<<" $\\pm$ "<<fDsStat*1000<<" $\\pm$ "<<sqrt(pow(fDsSyst,2)+pow(fDsPDG,2))*1000<<" \\mev"<<endl;

  (*fDsEr)=sqrt(fDsStat*fDsStat+fDsSyst*fDsSyst+fDsPDG*fDsPDG);

  fDsStatTemp=fDsStat;
  fDsSystTemp=sqrt(fDsSyst*fDsSyst+fDsPDG*fDsPDG);
}



void plotfDs(){//Plot BaBar, Belle and Cleo MuNu results

  //////////Plot the  BFS
  TH1F HBF("HBF","",1,2,8.5);
  HBF.GetXaxis()->SetTitle("B(D_{s} #rightarrow #mu #nu)    (x10^{-3})");
  HBF.GetYaxis()->SetNdivisions(0);
  HBF.GetYaxis()->SetRangeUser(0,.5);
  HBF.SetStats(0);
  TGraphErrors GBF;
  GBF.SetMarkerSize(1.5);
  //GBF.SetMarkerStyle(3);


  Double_t BellBFMuNuEr=sqrt(BFMuNuErStatBell*BFMuNuErStatBell+BFMuNuErSystBell*BFMuNuErSystBell);
  Double_t CleoBFMuNuEr=sqrt(BFMuNuErStatCleo*BFMuNuErStatCleo+BFMuNuErSystCleo*BFMuNuErSystCleo);
  Double_t BFMuNuEr=sqrt(BFMuNuErStat*BFMuNuErStat+BFMuNuErSyst*BFMuNuErSyst);

  Double_t BFMuNuAvg=( BFMuNuBell/(BellBFMuNuEr*BellBFMuNuEr)
		      +BFMuNuCleo/(CleoBFMuNuEr*CleoBFMuNuEr)
		      +BFMuNu/(BFMuNuEr*BFMuNuEr))
                     /(1./(BellBFMuNuEr*BellBFMuNuEr)+1./(CleoBFMuNuEr*CleoBFMuNuEr)+1./(BFMuNuEr*BFMuNuEr));
  
  Double_t BFMuNuAvgEr=sqrt((pow(BellBFMuNuEr/(BellBFMuNuEr*BellBFMuNuEr),2)
			    +pow(CleoBFMuNuEr/(CleoBFMuNuEr*CleoBFMuNuEr),2)
			    +pow(BFMuNuEr/(BFMuNuEr*BFMuNuEr),2))
			   /pow(1./(BellBFMuNuEr*BellBFMuNuEr)+1./(CleoBFMuNuEr*CleoBFMuNuEr)+1./(BFMuNuEr*BFMuNuEr),2));


  GBF.SetPoint(0,BFMuNuBell*1000,.45);
  GBF.SetPointError(0,BellBFMuNuEr*1000,0);

  GBF.SetPoint(1,BFMuNuCleo*1000,.35);
  GBF.SetPointError(1,CleoBFMuNuEr*1000,0);

  GBF.SetPoint(2,BFMuNu*1000,.25);
  GBF.SetPointError(2,BFMuNuEr*1000,0);

  GBF.SetPoint(3,BFMuNuAvg*1000,.15);
  GBF.SetPointError(3,BFMuNuAvgEr*1000,0);


  TCanvas C("C");
  C.Print("PlotfDs.ps[");


  C.Clear();
  HBF.Draw();
 
  TBox boxBF;
  boxBF.SetLineColor(0);
  boxBF.SetFillColor(5);
  boxBF.SetFillStyle(1000);
  boxBF.DrawBox((BFMuNuAvg-BFMuNuAvgEr)*1000,.0,(BFMuNuAvg+BFMuNuAvgEr)*1000,.5);

  GBF.Draw("pe same");
 
  TLatex textBF;
  textBF.SetTextSize(.04);
  
  TLatex textBFn;
  textBFn.SetTextSize(.03);
  textBFn.SetTextColor(1);

  textBF.DrawLatex(2.4,.45,"BELLE");
  textBFn.DrawLatex(2.4,.43,TString("(")+(long)(BFMuNuBell*1000)+"."+(long)(BFMuNuBell*100000+.5)%100+"#pm"+(long)(BellBFMuNuEr*1000)+"."+(long)(BellBFMuNuEr*100000+.5)%100+")");
  textBF.DrawLatex(2.4,.35,"CLEOc");
  textBFn.DrawLatex(2.4,.33,TString("(")+(long)(BFMuNuCleo*1000)+"."+(long)(BFMuNuCleo*100000+.5)%100+"#pm"+(long)(CleoBFMuNuEr*1000)+"."+(long)(CleoBFMuNuEr*100000+.5)%100+")");
  textBF.SetTextColor(2);
  textBF.DrawLatex(2.4,.25,"BaBar");
  cout<<BFMuNu<<" "<<BFMuNuEr<<endl;
  textBFn.DrawLatex(2.4,.23,TString("(")+(long)(BFMuNu*1000)+"."+(long)(BFMuNu*10000)%10+(long)(BFMuNu*100000+.5)%100+"#pm"+(long)(BFMuNuEr*1000)+"."+(long)(BFMuNuEr*100000+.5)%100+")");
  textBF.SetTextColor(1);
  textBF.DrawLatex(2.4,.15,"Average");
  textBFn.DrawLatex(2.4,.13,TString("(")+(long)(BFMuNuAvg*1000)+"."+(long)(BFMuNuAvg*100000+.5)%100+"#pm"+(long)(BFMuNuAvgEr*1000)+"."+(long)(BFMuNuAvgEr*100000+.5)%100+")");

  C.Print("PlotfDs.ps");  


  //////Plot the  fDs values
  Double_t LQCDMuNu=.241;
  Double_t LQCDMuNuEr=.003;

  cout<<"Belle fDs="<<endl;
  Double_t BellMuNu;
  Double_t BellMuNuEr;
  ComputefDs(BFMuNuBell,BFMuNuErStatBell,BFMuNuErSystBell,massMu,massMuEr,&BellMuNu,&BellMuNuEr);  

  cout<<"CLEOc fDs="<<endl;
  Double_t CleoMuNu;
  Double_t CleoMuNuEr;
  ComputefDs(BFMuNuCleo,BFMuNuErStatCleo,BFMuNuErSystCleo,massMu,massMuEr,&CleoMuNu,&CleoMuNuEr);  

  cout<<"BABAR fDs="<<endl;
  Double_t MuNu;
  Double_t MuNuEr;
  ComputefDs(BFMuNu,BFMuNuErStat,BFMuNuErSyst,massMu,massMuEr,&MuNu,&MuNuEr);  
  

  Double_t MuNuAvg=( BellMuNu/(BellMuNuEr*BellMuNuEr)
		   +CleoMuNu/(CleoMuNuEr*CleoMuNuEr)
		   +MuNu/(MuNuEr*MuNuEr))
                    /(1./(BellMuNuEr*BellMuNuEr)+1./(CleoMuNuEr*CleoMuNuEr)+1./(MuNuEr*MuNuEr));

  Double_t MuNuAvgEr=sqrt((pow(BellMuNuEr/(BellMuNuEr*BellMuNuEr),2)
			 +pow(CleoMuNuEr/(CleoMuNuEr*CleoMuNuEr),2)
			  +pow(MuNuEr/(MuNuEr*MuNuEr),2))
			 /pow(1./(BellMuNuEr*BellMuNuEr)+1./(CleoMuNuEr*CleoMuNuEr)+1./(MuNuEr*MuNuEr),2));

  
  TH1F HPlot("HPlot","",1,100,320);
  HPlot.GetXaxis()->SetTitle("f_{Ds}   (MeV)");
  HPlot.GetYaxis()->SetNdivisions(0);
  HPlot.GetYaxis()->SetRangeUser(0,.5);
  HPlot.SetStats(0);
  TGraphErrors G;
  G.SetMarkerSize(1.5);
  //G.SetMarkerStyle(3);

  G.SetPoint(0,BellMuNu*1000,.45);
  G.SetPointError(0,BellMuNuEr*1000,0);

  G.SetPoint(1,CleoMuNu*1000,.35);
  G.SetPointError(1,CleoMuNuEr*1000,0);

  G.SetPoint(2,MuNu*1000,.25);
  G.SetPointError(2,MuNuEr*1000,0);

  G.SetPoint(3,MuNuAvg*1000,.15);
  G.SetPointError(3,MuNuAvgEr*1000,0);

  G.SetPoint(4,LQCDMuNu*1000,.05);
  G.SetPointError(4,LQCDMuNuEr*1000,0);

  C.Clear();
  HPlot.Draw();
 
  TBox box;
  box.SetLineColor(0);
  box.SetFillColor(5);
  box.SetFillStyle(1000);
  box.DrawBox((MuNuAvg-MuNuAvgEr)*1000,.0,(MuNuAvg+MuNuAvgEr)*1000,.5);

  G.Draw("pe same");
 
  TLatex text;
  text.SetTextSize(.04);
  
  TLatex textn;
  textn.SetTextSize(.03);
  textn.SetTextColor(1);

  text.DrawLatex(110,.45,"BELLE (#mu#nu)");
  textn.DrawLatex(110,.43,TString("(")+(long)(BellMuNu*1000.+.5)+"#pm"+(long)(BellMuNuEr*1000.+.5)+")");
  text.DrawLatex(110,.35,"CLEOc (#mu#nu)");
  textn.DrawLatex(110,.33,TString("(")+(long)(CleoMuNu*1000.+.5)+"#pm"+(long)(CleoMuNuEr*1000.+.5)+")");
  text.SetTextColor(2);
  text.DrawLatex(110,.25,"BaBar (#mu#nu)");
  textn.DrawLatex(110,.23,TString("(")+(long)(MuNu*1000.+.5)+"#pm"+(long)(MuNuEr*1000.+.5)+")");
  text.SetTextColor(1);
  text.DrawLatex(110,.15,"Average (#mu#nu)");
  textn.DrawLatex(110,.13,TString("(")+(long)(MuNuAvg*1000.+.5)+"."+(long)(MuNuAvg*10000+.5)%10+"#pm"+(long)(MuNuAvgEr*1000.+.5)+"."+(long)(MuNuAvgEr*10000+.5)%10+")");
  text.SetTextColor(1);
  text.DrawLatex(110,.05,"LQCD [HPQCD+UKQCD]");
  textn.DrawLatex(110,.03,TString("(")+(long)(LQCDMuNu*1000.+.5)+"#pm"+(long)(LQCDMuNuEr*1000.+.5)+")");

  C.Print("PlotfDs.ps");  

  C.Print("PlotfDs.ps]");
}



void plotBaBarfDsValues(){


  TCanvas C("C");
  C.Print("PlotBaBarfDsValues.ps[");


  //////Plot the  fDs values
  Double_t LQCDMuNu=.241;
  Double_t LQCDMuNuEr=.003;


  cout<<"MuNu fDs="<<endl;
  Double_t MuNu;
  Double_t MuNuEr;
  ComputefDs(BFMuNu,BFMuNuErStat,BFMuNuErSyst,massMu,massMuEr,&MuNu,&MuNuEr);  
 
  cout<<"TauNuE fDs="<<endl;
  Double_t TauNuE;
  Double_t TauNuEEr;
  ComputefDs(BFTauNuE,BFTauNuEErStat,BFTauNuEErSyst,massTau,massTauEr,&TauNuE,&TauNuEEr);  

  cout<<"TauNuMu fDs="<<endl;
  Double_t TauNuMu;
  Double_t TauNuMuEr;
  ComputefDs(BFTauNuMu,BFTauNuMuErStat,BFTauNuMuErSyst,massTau,massTauEr,&TauNuMu,&TauNuMuEr);  

 

  Double_t MuNuAvg=( TauNuE/(TauNuEEr*TauNuEEr)
		   +TauNuMu/(TauNuMuEr*TauNuMuEr)
		   +MuNu/(MuNuEr*MuNuEr))
                    /(1./(TauNuEEr*TauNuEEr)+1./(TauNuMuEr*TauNuMuEr)+1./(MuNuEr*MuNuEr));

  Double_t MuNuAvgEr=sqrt((pow(TauNuEEr/(TauNuEEr*TauNuEEr),2)
			 +pow(TauNuMuEr/(TauNuMuEr*TauNuMuEr),2)
			  +pow(MuNuEr/(MuNuEr*MuNuEr),2))
			 /pow(1./(TauNuEEr*TauNuEEr)+1./(TauNuMuEr*TauNuMuEr)+1./(MuNuEr*MuNuEr),2));

  
  TH1F HPlot("HPlot","",1,100,320);
  HPlot.GetXaxis()->SetTitle("f_{Ds}   (MeV)");
  HPlot.GetYaxis()->SetNdivisions(0);
  HPlot.GetYaxis()->SetRangeUser(0,.5);
  HPlot.SetStats(0);
  TGraphErrors G;
  G.SetMarkerSize(1.5);
  //G.SetMarkerStyle(3);


  G.SetPoint(2,MuNu*1000,.45);
  G.SetPointError(2,MuNuEr*1000,0);

  G.SetPoint(0,TauNuE*1000,.35);
  G.SetPointError(0,TauNuEEr*1000,0);

  G.SetPoint(1,TauNuMu*1000,.25);
  G.SetPointError(1,TauNuMuEr*1000,0);

  G.SetPoint(3,MuNuAvg*1000,.15);
  G.SetPointError(3,MuNuAvgEr*1000,0);

  G.SetPoint(4,LQCDMuNu*1000,.05);
  G.SetPointError(4,LQCDMuNuEr*1000,0);

  C.Clear();
  HPlot.Draw();
 
  TBox box;
  box.SetLineColor(0);
  box.SetFillColor(5);
  box.SetFillStyle(1000);
  box.DrawBox((MuNuAvg-MuNuAvgEr)*1000,.0,(MuNuAvg+MuNuAvgEr)*1000,.5);

  G.Draw("pe same");
 
  TLatex text;
  text.SetTextSize(.04);
  
  TLatex textn;
  textn.SetTextSize(.03);
  textn.SetTextColor(1);

  text.DrawLatex(110,.45,"D^{+}_{s}#rightarrow#mu^{+}#nu");
  textn.DrawLatex(110,.41,TString("(")+(long)(MuNu*1000.+.5)+"#pm"+(long)(MuNuEr*1000.+.5)+")");

  text.DrawLatex(110,.35,"D^{+}_{s}#rightarrow#tau^{+}_{e#nu#nu}#nu");
  textn.DrawLatex(110,.31,TString("(")+(long)(TauNuE*1000.+.5)+"#pm"+(long)(TauNuEEr*1000.+.5)+")");

  text.DrawLatex(110,.25,"D^{+}_{s}#rightarrow#tau^{+}_{#mu#nu#nu}#nu");
  textn.DrawLatex(110,.21,TString("(")+(long)(TauNuMu*1000.+.5)+"#pm"+(long)(TauNuMuEr*1000.+.5)+")");

  text.SetTextColor(2);
  text.DrawLatex(110,.15,"Average");
  textn.DrawLatex(110,.13,TString("(")+(long)(MuNuAvg*1000.+.5)+"."+(long)(MuNuAvg*10000+.5)%10+"#pm"+(long)(MuNuAvgEr*1000.+.5)+"."+(long)(MuNuAvgEr*10000+.5)%10+")");

  text.SetTextColor(1);
  text.DrawLatex(110,.05,"Lattice QCD");
  textn.DrawLatex(110,.03,TString("(")+(long)(LQCDMuNu*1000.+.5)+"#pm"+(long)(LQCDMuNuEr*1000.+.5)+")");

  C.Print("PlotBaBarfDsValues.ps");  

  C.Print("PlotBaBarfDsValues.ps]");
}



void plotfDsBaBarBelleCleo(){
  //plot the Final values of fDs (hard coded here) for BaBar, Belle and Cleo

  TCanvas C("C");
  C.Print("PlotfDsBaBarBelleCleo.ps[");


  //////Plot the  fDs values
  Double_t LQCD=241;
  Double_t LQCDEr=3;

  Double_t Bell=275;
  Double_t BellEr=sqrt(pow(16,2)+pow(12,2));

  Double_t Cleo=259.5;
  Double_t CleoEr=sqrt(pow(6.6,2)+pow(3.1,2));

  Double_t BABAR=265.7;
  Double_t BABAREr=sqrt(pow(8.4,2)+pow(7.7,2));
  

  Double_t Avg=( Bell/(BellEr*BellEr)
		+Cleo/(CleoEr*CleoEr)
		+BABAR/(BABAREr*BABAREr))
               /(1./(BellEr*BellEr)+1./(CleoEr*CleoEr)+1./(BABAREr*BABAREr));
  
  Double_t AvgEr=sqrt((pow(BellEr/(BellEr*BellEr),2)
		      +pow(CleoEr/(CleoEr*CleoEr),2)
		      +pow(BABAREr/(BABAREr*BABAREr),2))
		     /pow(1./(BellEr*BellEr)+1./(CleoEr*CleoEr)+1./(BABAREr*BABAREr),2));

  
  TH1F HPlot("HPlot","",1,100,320);
  HPlot.GetXaxis()->SetTitle("f_{Ds}   (MeV)");
  HPlot.GetYaxis()->SetNdivisions(0);
  HPlot.GetYaxis()->SetRangeUser(0,.5);
  HPlot.SetStats(0);
  TGraphErrors G;
  G.SetMarkerSize(1.5);
  //G.SetMarkerStyle(3);

  G.SetPoint(0,Bell,.45);
  G.SetPointError(0,BellEr,0);

  G.SetPoint(1,Cleo,.35);
  G.SetPointError(1,CleoEr,0);

  G.SetPoint(2,BABAR,.25);
  G.SetPointError(2,BABAREr,0);

  G.SetPoint(3,Avg,.15);
  G.SetPointError(3,AvgEr,0);

  G.SetPoint(4,LQCD,.05);
  G.SetPointError(4,LQCDEr,0);

  C.Clear();
  HPlot.Draw();
 
  TBox box;
  box.SetLineColor(0);
  box.SetFillColor(5);
  box.SetFillStyle(1000);
  box.DrawBox((Avg-AvgEr),.0,(Avg+AvgEr),.5);

  G.Draw("pe same");
 
  TLatex text;
  text.SetTextSize(.04);
  
  TLatex textn;
  textn.SetTextSize(.03);
  textn.SetTextColor(1);

  text.DrawLatex(110,.45,"BELLE");
  textn.DrawLatex(110,.43,TString("(")+(long)(Bell+.5)+"#pm"+(long)(BellEr+.5)+")");
  text.DrawLatex(110,.35,"CLEOc");
  textn.DrawLatex(110,.33,TString("(")+(long)(Cleo)+"."+(long)(Cleo*10)%10+"#pm"+(long)(CleoEr)+"."+(long)(CleoEr*10)%10+")");
  text.SetTextColor(2);
  text.DrawLatex(110,.25,"BaBar");
  textn.DrawLatex(110,.23,TString("(")+(long)(BABAR+.5)+"#pm"+(long)(BABAREr+.5)+")");
  text.SetTextColor(1);
  text.DrawLatex(110,.15,"Average");
  textn.DrawLatex(110,.13,TString("(")+(long)(Avg)+"."+(long)(Avg*10)%10+"#pm"+(long)(AvgEr)+"."+(long)(AvgEr*10)%10+")");
  text.SetTextColor(1);
  text.DrawLatex(110,.05,"LatticeQCD");
  textn.DrawLatex(110,.03,TString("(")+(long)(LQCD)+"#pm"+(long)(LQCDEr)+")");

  C.Print("PlotfDsBaBarBelleCleo.ps");  

  C.Print("PlotfDsBaBarBelleCleo.ps]");
}



void ComputeBaBarfDsAverage(){


  ///Average of the Tau BFs
  cout<<" Average BF for Taus: "<<endl;
  cout<<" Stat error only "<<endl;
  ComputeErrorWeightAvg(BFTauNuE*100,BFTauNuEErStat*100,
			BFTauNuMu*100,BFTauNuMuErStat*100);
  cout<<" Syst error only "<<endl;
  ComputeErrorWeightAvg(BFTauNuE*100,BFTauNuEErSyst*100,
			BFTauNuMu*100,BFTauNuMuErSyst*100);
  cout<<" Total error "<<endl;
  ComputeErrorWeightAvg(BFTauNuE*100,sqrt(BFTauNuEErStat*BFTauNuEErStat+BFTauNuEErSyst*BFTauNuEErSyst)*100,
			BFTauNuMu*100,sqrt(BFTauNuMuErStat*BFTauNuMuErStat+BFTauNuMuErSyst*BFTauNuMuErSyst)*100);


  Double_t fDs[3];
  Double_t fDsErStat[3];
  Double_t fDsErSyst[3];
  Double_t fDsEr[3];
  
  //MuNu
  ComputefDs(BFMuNu,BFMuNuErStat,BFMuNuErSyst,massMu,massMuEr,&fDs[0],&fDsEr[0]);
  fDsErStat[0]=fDsStatTemp;
  fDsErSyst[0]=fDsSystTemp;
  //TauNuE
  ComputefDs(BFTauNuE,BFTauNuEErStat,BFTauNuEErSyst,massTau,massTauEr,&fDs[1],&fDsEr[1]);  
  fDsErStat[1]=fDsStatTemp;
  fDsErSyst[1]=fDsSystTemp;
  //TauNuMu
  ComputefDs(BFTauNuMu,BFTauNuMuErStat,BFTauNuMuErSyst,massTau,massTauEr,&fDs[2],&fDsEr[2]);  
  fDsErStat[2]=fDsStatTemp;
  fDsErSyst[2]=fDsSystTemp;


//   ///Average of the Taus
//   cout<<" Average fDs for Taus: "<<endl;
//   ComputeErrorWeightAvg(fDs[1]*1000,fDsEr[1]*1000,
//			fDs[2]*1000,fDsEr[2]*1000); 

  ///Average of MuNu and the Taus
  //first do errors separate to get individual errors
  cout<<" Average fDs for Mu and Taus With Stat Error: "<<endl;
  ComputeErrorWeightAvg(fDs[0]*1000,fDsErStat[0]*1000,
			fDs[1]*1000,fDsErStat[1]*1000,
			fDs[2]*1000,fDsErStat[2]*1000);
  cout<<" Average fDs for Mu and Taus With Syst Error: "<<endl;
  ComputeErrorWeightAvg(fDs[0]*1000,fDsErSyst[0]*1000,
			fDs[1]*1000,fDsErSyst[1]*1000,
			fDs[2]*1000,fDsErSyst[2]*1000);
  //this last average gives the true fDs value
  cout<<" Average fDs for Mu and Taus With Total Error: "<<endl;
  ComputeErrorWeightAvg(fDs[0]*1000,fDsEr[0]*1000,
			fDs[1]*1000,fDsEr[1]*1000,
			fDs[2]*1000,fDsEr[2]*1000);
  
  
}



void ComputeLeptonUniversality(){
  //Ratio of BF(Ds-->MuNu)/ BF(Ds-->TauNu)

// #define BFMuNu 6.02e-3 //!!!!!!!!!!!!Update
// #define BFMuNuErStat 0.38e-3 //!!!!!!!!!!!!Update
// #define BFMuNuErSyst 0.34e-3 //!!!!!!!!!!!!Update

// #define BFTauNuE 5.07e-2 //!!!!!!!!!!!!Update
// #define BFTauNuEErStat 0.52e-2 //!!!!!!!!!!!!Update
// #define BFTauNuEErSyst 0.68e-2 //!!!!!!!!!!!!Update

// #define BFTauNuMu 4.91e-2 //!!!!!!!!!!!!Update
// #define BFTauNuMuErStat 0.47e-2 //!!!!!!!!!!!!Update
// #define BFTauNuMuErSyst 0.54e-2 //!!!!!!!!!!!!Update


  //Compute the ratio using the weights obtained from the Macro: ComputeAverage.C
  //Float_t MuNuWeight=.66;//not used here
  Float_t TauNuEWeight=0.31;//Jose (July 2014): corrected the weights to the weights from the Average of the Tau->e and Tau->mu 
  Float_t TauNuMuWeight=0.69;//Jose (July 2014): corrected the weights to the weights  from the Average of the Tau->e and Tau->mu 

  Float_t ratio=(BFTauNuE*TauNuEWeight+BFTauNuMu*TauNuMuWeight)/ BFMuNu ;
  

  //Compute the error
  //Derivatives 
  Float_t derivRatio[3];
  derivRatio[0]=-ratio/BFMuNu;
  derivRatio[1]=TauNuEWeight / BFMuNu;
  derivRatio[2]=TauNuMuWeight / BFMuNu;

  //determine the Stat Error matrix
  Float_t Vstat[3][3];
  Vstat[0][0]=BFMuNuErStat*BFMuNuErStat;
  Vstat[1][1]=BFTauNuEErStat*BFTauNuEErStat;
  Vstat[2][2]=BFTauNuMuErStat*BFTauNuMuErStat;
  
  Vstat[0][1]=(BFMuNu*0.0114) * (BFTauNuE*0.0114);//denominator statistical error
  Vstat[0][2]=(BFMuNu*0.0114) * (BFTauNuMu*0.0114);
  Vstat[1][2]=(BFTauNuE*0.0114) * (BFTauNuMu*0.0114);
  Vstat[1][0]=Vstat[0][1];
  Vstat[2][0]=Vstat[0][2];
  Vstat[2][1]=Vstat[1][2];
  

  Float_t ratioErStat=0;
  cout<<fixed<<setprecision(4)<<endl;
  cout<<" The stat covariance is matrix"<<endl;
  for(Int_t i=0;i<3;i++){
    for(Int_t j=0;j<3;j++){
      cout<<" "<<Vstat[i][j]*1e5;
      ratioErStat+= derivRatio[i]* derivRatio[j]*Vstat[i][j];
    }
    cout<<endl;
  }

  //determine the Syst Error matrix
  Float_t Vsyst[3][3];
  Vsyst[0][0]=BFMuNuErSyst*BFMuNuErSyst;
  Vsyst[1][1]=BFTauNuEErSyst*BFTauNuEErSyst;
  Vsyst[2][2]=BFTauNuMuErSyst*BFTauNuMuErSyst;
  
  Vsyst[0][1]=(BFMuNu*0.033) * (BFTauNuE*0.033)//denominator systematics
    +(BFMuNu*0.0083) * (BFTauNuE*0.0083)//Tracking
    +0;
  Vsyst[0][2]=(BFMuNu*0.033) * (BFTauNuMu*0.033)//denominator systematics
    +(BFMuNu*0.0083) * (BFTauNuMu*0.0083)//Tracking
    +(BFMuNu*0.018) * (BFTauNuMu*0.018)//PID
    +0;
  Vsyst[1][2]=(BFTauNuE*0.033) * (BFTauNuMu*0.033)//denominator systematics
    +(BFTauNuE*0.0083) * (BFTauNuMu*0.0083)//Tracking
    +(BFTauNuE*0.055) * (BFTauNuMu*0.018)//signal model (exponential variation)//Jose (July 2014): corrected to match tables in thesis
    +(BFTauNuE*0.116) * (BFTauNuMu*0.096)//background model (quadratic variation)//Jose (July 2014): corrected to match tables in thesis
    +0;
  Vsyst[1][0]=Vsyst[0][1];
  Vsyst[2][0]=Vsyst[0][2];
  Vsyst[2][1]=Vsyst[1][2];
  

  Float_t ratioErSyst=0;
  cout<<" The syst covariance is matrix"<<endl;
  for(Int_t i=0;i<3;i++){
    for(Int_t j=0;j<3;j++){
      cout<<" "<<Vsyst[i][j]*1e5;
      ratioErSyst+= derivRatio[i]*derivRatio[j]*Vsyst[i][j];
    }
    cout<<endl;
  }


  cout<<fixed<<setprecision(2)<<endl;
  cout<<" BF(Ds-->tau nu)/BF(Ds-->mu nu)  = "<<ratio<<" \\pm "<<sqrt(ratioErStat)<<" \\pm "<<sqrt(ratioErSyst)<<endl;

  Float_t smratio=pow(massTau/massMu,2) * pow((1-massTau*massTau/(massDs*massDs))/(1-massMu*massMu/(massDs*massDs)),2);
  cout<<" Compare to the Standard Model value : "<< smratio<<endl;
  
  cout<<" Difference = "<< (ratio - smratio)/sqrt(ratioErStat+ratioErSyst)<< "sigma"<<endl;

}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////     Computations of fDs for Thesis     ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ComputefDsCurrentMuNuCleoPlusBelle(){
  ///Compute the value corresponding to the Cleo+Belle MuNu Average
  
  Double_t BF=5.81e-3 ;
  Double_t BFSt=.43e-3 ; 
  
  //Compute the decay constant
  Double_t fDs=sqrt((BF/tauDs)/(
			  (GF*GF*massDs*massDs*massDs/(8*TMath::Pi()))
			  *pow(massMu/massDs,2)
			  *pow(1-massMu*massMu/(massDs*massDs),2)
			  *Vcs*Vcs
			  ));


  //Derivatives of (*fDs) wrt parameters
  Double_t dfdB=0.5*(fDs)/BF;
  Double_t dfdtau=0.5*(fDs)/tauDs;
  Double_t dfdGF=(fDs)/GF;
  Double_t dfdMD=(sqrt(BF*8*TMath::Pi()/tauDs)/(GF*Vcs))*(1+3*massMu*massMu/(massDs*massDs))/(2*pow(massDs,(float)1.5)*pow(1-massMu*massMu/(massDs*massDs),(float)2.0));    
  Double_t dfdMMu=(sqrt(BF*8*TMath::Pi()/tauDs)/(GF*Vcs))*(1-3*massMu*massMu/(massDs*massDs))/(massMu*massMu*pow(1-massMu*massMu/(massDs*massDs),(float)2.0));
  Double_t dfdVcs=(fDs)/Vcs;

  //errors Stat + Syst + PDG
  Double_t fDsEr=sqrt(pow(dfdB*BFSt,2)
		      +pow(dfdtau*tauDsEr,(float)2)
		      +pow(dfdGF*GFEr,(float)2)
		      +pow(dfdMD*massDsEr,(float)2)
		      +pow(dfdMMu*massMuEr,(float)2)
		      +pow(dfdVcs*VcsEr,(float)2));
  

  cout<<fixed<<setprecision(1)<<" fDs= "<<(fDs)*1000<<" $\\pm$ "<<fDsEr*1000<<" MeV"<<endl;  
}


void ComputefDsCurrentTauNuCleo(){
  ///Compute the value corresponding to the Cleo TauNu result
  
  Double_t BF=6.42e-2 ;
  Double_t BFSt=.83e-2 ; 

  //Compute the decay constant
  Double_t fDs=sqrt((BF/tauDs)/(
			  (GF*GF*massDs*massDs*massDs/(8*TMath::Pi()))
			  *pow(massTau/massDs,2)
			  *pow(1-massTau*massTau/(massDs*massDs),2)
			  *Vcs*Vcs
			  ));


  //Derivatives of (*fDs) wrt parameters
  Double_t dfdB=0.5*(fDs)/BF;
  Double_t dfdtau=0.5*(fDs)/tauDs;
  Double_t dfdGF=(fDs)/GF;
  Double_t dfdMD=(sqrt(BF*8*TMath::Pi()/tauDs)/(GF*Vcs))*(1+3*massTau*massTau/(massDs*massDs))/(2*pow(massDs,(float)1.5)*pow(1-massTau*massTau/(massDs*massDs),(float)2.0));    
  Double_t dfdMMu=(sqrt(BF*8*TMath::Pi()/tauDs)/(GF*Vcs))*(1-3*massTau*massTau/(massDs*massDs))/(massTau*massTau*pow(1-massTau*massTau/(massDs*massDs),(float)2.0));
  Double_t dfdVcs=(fDs)/Vcs;

  //errors Stat + Syst + PDG
  Double_t fDsEr=sqrt(pow(dfdB*BFSt,2)
		      +pow(dfdtau*tauDsEr,(float)2)
		      +pow(dfdGF*GFEr,(float)2)
		      +pow(dfdMD*massDsEr,(float)2)
		      +pow(dfdMMu*massTauEr,(float)2)
		      +pow(dfdVcs*VcsEr,(float)2));
  

  cout<<fixed<<setprecision(1)<<" fDs= "<<(fDs)*1000<<" $\\pm$ "<<fDsEr*1000<<" MeV"<<endl;  
}





//////////////////////////For SLAC Seminar/////////////////////////////////

void plotSummaryfDs(){

  ////////////////////////////////
  //BF values for all experiments:
  ////////////////////////////////

  //BABAR Old MuNu
  Float_t fDsBaBarOldMu=283;
  Float_t fDsBaBarOldMuEr=23;

  //BELLE: Phys. Rev.Lett. 100, 241801 (2008).
  Float_t BFBelleMu=6.44e-3;
  Float_t BFBelleMuEr=sqrt(pow(0.76e-3,2)+pow(0.52e-3,2));
  Float_t fDsBelleMu=275;
  Float_t fDsBelleMuEr=20;

  //CLEOc: Phys. Rev. D 79, 052001 (2009).
  Float_t BFCleoMu=5.65e-3;
  Float_t BFCleoMuEr=sqrt(pow(0.45e-3,2)+pow(0.17e-3,2));
  Float_t fDsCleoMu=257;
  Float_t fDsCleoMuEr=11;

  //BABAR:Phys. Rev. D 82, 091103(R) (2010).
  Float_t BFBaBarMu=6.02e-3;
  Float_t BFBaBarMuEr=sqrt(pow(0.38e-3,2)+pow(0.34e-3,2));
  Float_t fDsBaBarMu=266;
  Float_t fDsBaBarMuEr=11;

  //CLEOc: Phys. Rev. D 79, 052002 (2009).
  Float_t BFCleoTauToE=5.30e-2;
  Float_t BFCleoTauToEEr=sqrt(pow(0.47e-2,2)+pow(0.22e-2,2));
  Float_t fDsCleoTauToE=253;
  Float_t fDsCleoTauToEEr=sqrt(11*11+5*5);
  
  //CLEOc: Phys. Rev. D 79, 052001 (2009).
  Float_t BFCleoTauToPi=6.42e-2;
  Float_t BFCleoTauToPiEr=sqrt(pow(0.81e-2,2)+pow(0.18e-2,2));
  Float_t fDsCleoTauToPi=279;
  Float_t fDsCleoTauToPiEr=sqrt(17*17+4*4);

  //CLEOc: Phys. Rev. D 80, 112004 (2009).
  Float_t BFCleoTauToRho=5.52e-2;
  Float_t BFCleoTauToRhoEr=sqrt(pow(0.57e-2,2)+pow(0.21e-2,2));
  Float_t fDsCleoTauToRho=258;
  Float_t fDsCleoTauToRhoEr=sqrt(13*13+5*5);

  //BABAR:Phys. Rev. D 82, 091103(R) (2010).
  Float_t BFBaBarTauToE=5.07e-2;
  Float_t BFBaBarTauToEEr=sqrt(pow(0.52e-2,2)+pow(0.68e-2,2));
  Float_t fDsBaBarTauToE=247;
  Float_t fDsBaBarTauToEEr=sqrt(13*13 + 17*17);

  //BABAR:Phys. Rev. D 82, 091103(R) (2010).
  Float_t BFBaBarTauToMu=4.91e-2;
  Float_t BFBaBarTauToMuEr=sqrt(pow(0.47e-2,2)+pow(0.54e-2,2));
  Float_t fDsBaBarTauToMu=243;
  Float_t fDsBaBarTauToMuEr=sqrt(12*12 + 14*14);

  //BABAR: Shane arxiv
  Float_t BFBaBarTauToE2=4.54e-2;
  Float_t BFBaBarTauToE2Er=sqrt(pow(0.53e-2,2)+pow(0.49e-2,2));
  Float_t fDsBaBarTauToE2=233;
  Float_t fDsBaBarTauToE2Er=18;


  ///rerun code twice to get values from print out
  ///Average  of MuNu
  cout<<"Average of MuNu"<<endl;
  ComputeErrorWeightAvg(BFBelleMu,BFBelleMuEr,BFCleoMu,BFCleoMuEr,BFBaBarMu,BFBaBarMuEr);
  Float_t BFAvgMu=0.00590192;
  Float_t BFAvgMuEr=0.000327089;


  //compute the chi2 of the MuNu values:
  Float_t Muchi2=pow((BFBelleMu-BFAvgMu)/BFBelleMuEr,2)
    +pow((BFCleoMu-BFAvgMu)/BFCleoMuEr,2) 
    +pow((BFBaBarMu-BFAvgMu)/BFBaBarMuEr,2);
  cout<<" Ds-->MuNu chi2/(N-1) = "<<Muchi2<<"/2 = "<<Muchi2/2.0<<endl;



  ///Average  of TauNu
  cout<<"Average of TauNu"<<endl;
//   ComputeErrorWeightAvg(BFCleoTauToE,BFCleoTauToEEr,
//                         BFCleoTauToPi,BFCleoTauToPiEr,
//                         BFCleoTauToRho,BFCleoTauToRhoEr,
// 			   BFBaBarTauToE,BFBaBarTauToEEr,
//                         BFBaBarTauToMu,BFBaBarTauToMuEr);
//   Float_t BFAvgTau=0.0540258;
//   Float_t BFAvgTauEr=0.00298919;

  //to account for correlations
//   ComputeErrorWeightAvg(5.58e-2,sqrt(pow(0.33e-2,2)+pow(0.13e-2,2)), //avg of CLEO Ds-->TauNu from their TauToRho paper
// 			5.00e-2,sqrt(pow(0.35e-2,2)+pow(0.49e-2,2)),  //avg of BaBar Ds-->TauNu from BaBar paper
// 			);  
//   Float_t BFAvgTau=5.43061e-2;
//   Float_t BFAvgTauEr=0.305609e-2;

  //to account for correlations and Shanes value
  ComputeErrorWeightAvg(5.58e-2,sqrt(pow(0.33e-2,2)+pow(0.13e-2,2)), //avg of CLEO Ds-->TauNu from their TauToRho paper
			5.00e-2,sqrt(pow(0.35e-2,2)+pow(0.49e-2,2)),  //avg of BaBar Ds-->TauNu from BaBar paper
			BFBaBarTauToE2,BFBaBarTauToE2Er );  //Shanes TauToE
  Float_t BFAvgTau=0.0529522;
  Float_t BFAvgTauEr= 0.00281424;

  //compute the chi2 of the TauNu values:
  Float_t Tauchi2=pow((BFCleoTauToE-BFAvgTau)/BFCleoTauToEEr,2)
    +pow((BFCleoTauToPi-BFAvgTau)/BFCleoTauToPiEr,2)
    +pow((BFCleoTauToRho-BFAvgTau)/BFCleoTauToRhoEr,2)
    +pow((BFBaBarTauToE-BFAvgTau)/BFBaBarTauToEEr,2)
    +pow((BFBaBarTauToMu-BFAvgTau)/BFBaBarTauToMuEr,2)
    +pow((BFBaBarTauToE2-BFAvgTau)/BFBaBarTauToE2Er,2);
  cout<<" Ds-->TauNu chi2/(N-1) = "<<Tauchi2<<"/5 = "<<Tauchi2/5.0<<endl;



  ////////////////////////////////
  ///Plots
  /////
  TCanvas C;
  C.SetFillStyle(4000);  
  C.Print("SummaryfDs.ps[");

  TGraph GFrame;
  GFrame.SetMarkerSize(.001);

  TLatex text;
  text.SetTextSize(.05);

  TLatex valtext;
  valtext.SetTextSize(.04);

  TBox avg;
  avg.SetLineColor(0);
  avg.SetFillColor(kYellow);
  avg.SetFillStyle(1000);

  TPad pad("pad","pad",0,0,1,1);
  pad.SetFillStyle(4000);

  char value[100];

  ////////////////////
  //Plot of BF(Ds-->mu nu)
  ////////////////////
  TGraphErrors GBFMuNu;
  GBFMuNu.SetPoint(0,BFBelleMu*1000,.8);
  GBFMuNu.SetPointError(0,BFBelleMuEr*1000,0);
  GBFMuNu.SetPoint(1,BFCleoMu*1000,.6);
  GBFMuNu.SetPointError(1,BFCleoMuEr*1000,0);
  GBFMuNu.SetPoint(2,BFBaBarMu*1000,.4);
  GBFMuNu.SetPointError(2,BFBaBarMuEr*1000,0);


  C.Clear();
  TH1F hMuNuBF("hMuNuBF","",1,3.5,7.5);
  hMuNuBF.SetStats(0);
  hMuNuBF.GetYaxis()->SetRangeUser(0.1,0.9);
  hMuNuBF.GetYaxis()->SetNdivisions(0,0);
  hMuNuBF.SetFillStyle(4000);  
  hMuNuBF.GetXaxis()->SetTitle("B(D_{s}^{-} #rightarrow #mu^{-} #nu_{#mu})    (x10^{-3})");
  C.cd();
  hMuNuBF.Draw();
  avg.DrawBox((BFAvgMu-BFAvgMuEr)*1000,.1,(BFAvgMu+BFAvgMuEr)*1000,.9);
  pad.cd();
  hMuNuBF.Draw();
  GBFMuNu.Draw("pesame");
  C.cd();
  pad.Draw();  
  text.DrawLatex(3.8,.8,"BELLE");
  sprintf(value,"(%.2f#pm%.2f)",BFBelleMu*1000,BFBelleMuEr*1000);
  valtext.DrawLatex((BFBelleMu-BFBelleMuEr/2)*1000,.82,value);
  text.DrawLatex(3.8,.6,"CLEOc");
  sprintf(value,"(%.2f#pm%.2f)",BFCleoMu*1000,BFCleoMuEr*1000);
  valtext.DrawLatex((BFCleoMu-BFCleoMuEr/2)*1000,.62,value);
  text.DrawLatex(3.8,.4,"BABAR");
  sprintf(value,"(%.2f#pm%.2f)",BFBaBarMu*1000,BFBaBarMuEr*1000);
  valtext.DrawLatex((BFBaBarMu-BFBaBarMuEr/2)*1000,.42,value);
  text.DrawLatex(3.8,.2,"average");
  sprintf(value,"(%.2f#pm%.2f)",BFAvgMu*1000,BFAvgMuEr*1000);
  valtext.DrawLatex((BFAvgMu-BFAvgMuEr/2)*1000,.22,value);
  C.Print("SummaryfDs.ps");
  

  ///////////////////
  ///Plot B(Ds-->TauNu)
  //////////////////
  TGraphErrors GBFTauNu;
  GBFTauNu.SetPoint(0,BFCleoTauToE*100,1.4);
  GBFTauNu.SetPointError(0,BFCleoTauToEEr*100,0);
  GBFTauNu.SetPoint(1,BFCleoTauToPi*100,1.2);
  GBFTauNu.SetPointError(1,BFCleoTauToPiEr*100,0);
  GBFTauNu.SetPoint(2,BFCleoTauToRho*100,1.0);
  GBFTauNu.SetPointError(2,BFCleoTauToRhoEr*100,0);
  GBFTauNu.SetPoint(3,BFBaBarTauToE*100,0.8);
  GBFTauNu.SetPointError(3,BFBaBarTauToEEr*100,0);
  GBFTauNu.SetPoint(4,BFBaBarTauToMu*100,0.6);
  GBFTauNu.SetPointError(4,BFBaBarTauToMuEr*100,0);
  GBFTauNu.SetPoint(5,BFBaBarTauToE2*100,0.4);
  GBFTauNu.SetPointError(5,BFBaBarTauToE2Er*100,0);
  

  TLatex tautext;
  tautext.SetTextSize(.03);

  C.Clear();
  TH1F hTauNuBF("hTauNuBF","",1,2.75,7.5);
  hTauNuBF.SetStats(0);
  hTauNuBF.GetYaxis()->SetRangeUser(0.0,1.7);
  hTauNuBF.GetYaxis()->SetNdivisions(0,0);
  hTauNuBF.SetFillStyle(4000);  
  hTauNuBF.GetXaxis()->SetTitle("B(D_{s}^{-} #rightarrow #tau^{-} #nu_{#tau})    (x10^{-2})");
  C.cd();
  hTauNuBF.Draw();
  avg.DrawBox((BFAvgTau-BFAvgTauEr)*100,.0,(BFAvgTau+BFAvgTauEr)*100,1.7);
  pad.Clear();
  pad.cd();
  hTauNuBF.Draw();
  GBFTauNu.Draw("pesame");
  C.cd();
  pad.Draw();  
  text.DrawLatex(2.9,1.4,"CLEOc");
  sprintf(value,"(%.2f#pm%.2f)",BFCleoTauToE*100,BFCleoTauToEEr*100);
  valtext.DrawLatex((BFCleoTauToE-BFCleoTauToEEr/2)*100,1.42,value);
  tautext.DrawLatex(2.9,1.4-.055,"(#tau #rightarrow e#nu#nu)");

  text.DrawLatex(2.9,1.2,"CLEOc");
  sprintf(value,"(%.2f#pm%.2f)",BFCleoTauToPi*100,BFCleoTauToPiEr*100);
  valtext.DrawLatex((BFCleoTauToPi-BFCleoTauToPiEr/2)*100,1.2,value);
  tautext.DrawLatex(2.9,1.2-.055,"(#tau #rightarrow #pi#nu)");

  text.DrawLatex(2.9,1.0,"CLEOc");
  sprintf(value,"(%.2f#pm%.2f)",BFCleoTauToRho*100,BFCleoTauToRhoEr*100);
  valtext.DrawLatex((BFCleoTauToRho-BFCleoTauToRhoEr/2)*100,1.02,value);
  tautext.DrawLatex(2.9,1.0-.055,"(#tau #rightarrow #rho#nu)");

  text.DrawLatex(2.9,0.8,"BABAR");
  sprintf(value,"(%.2f#pm%.2f)",BFBaBarTauToE*100,BFBaBarTauToEEr*100);
  valtext.DrawLatex((BFBaBarTauToE-BFBaBarTauToEEr/2)*100,0.82,value);
  tautext.DrawLatex(2.9,.8-.055,"(#tau #rightarrow e#nu#nu)");

  text.DrawLatex(2.9,0.6,"BABAR");
  sprintf(value,"(%.2f#pm%.2f)",BFBaBarTauToMu*100,BFBaBarTauToMuEr*100);
  valtext.DrawLatex((BFBaBarTauToMu-BFBaBarTauToMuEr/2)*100,0.62,value);
  tautext.DrawLatex(2.9,0.6-.055,"(#tau #rightarrow #mu#nu#nu)");

  text.DrawLatex(2.9,0.4,"BABAR");
  sprintf(value,"(%.2f#pm%.2f)",BFBaBarTauToE2*100,BFBaBarTauToE2Er*100);
  valtext.DrawLatex((BFBaBarTauToE2-BFBaBarTauToE2Er/2)*100,0.42,value);
  tautext.DrawLatex(2.9,0.4-.055,"(#tau #rightarrow e#nu#nu)'"); ///(#tau #rightarrow K_{S}K)

  text.DrawLatex(2.9,0.2,"average");
  sprintf(value,"(%.2f#pm%.2f)",BFAvgTau*100,BFAvgTauEr*100);
  valtext.DrawLatex((BFAvgTau-BFAvgTauEr/2)*100,0.2-0.04,value);
  C.Print("SummaryfDs.ps");



  //Plot fDs values before  BaBar analysis
  Float_t fDsLQCD2008=241.0;
  Float_t fDsLQCD2008Er=3.0;
  Float_t fDsAvg2009=256.9;//HFAG avg 2009
  Float_t fDsAvg2009Er=6.8;
  TGraphErrors GfDs1;
  GfDs1.SetPoint(0,fDsBelleMu,2.0);
  GfDs1.SetPointError(0,fDsBelleMuEr,0);
  GfDs1.SetPoint(1,fDsCleoMu,1.8);
  GfDs1.SetPointError(1,fDsCleoMuEr,0);
  GfDs1.SetPoint(2,fDsBaBarOldMu,1.6);
  GfDs1.SetPointError(2,fDsBaBarOldMuEr,0);

  GfDs1.SetPoint(3,fDsCleoTauToE,1.4);
  GfDs1.SetPointError(3,fDsCleoTauToEEr,0);
  GfDs1.SetPoint(4,fDsCleoTauToPi,1.2);
  GfDs1.SetPointError(4,fDsCleoTauToPiEr,0);
  GfDs1.SetPoint(5,fDsCleoTauToRho,1.0);
  GfDs1.SetPointError(5,fDsCleoTauToRhoEr,0);
  // GfDs1.SetPoint(6,fDsBaBarTauToE2,0.8);
  //  GfDs1.SetPointError(6,fDsBaBarTauToE2Er,0);

  GfDs1.SetPoint(7,fDsLQCD2008,0.52);
  GfDs1.SetPointError(7,fDsLQCD2008Er,0);

  TLatex fds3text;
  fds3text.SetTextSize(.03);

  C.Clear();
  TH1F hfDs1("hfDs1","",1,200,300);
  hfDs1.SetStats(0);
  hfDs1.GetYaxis()->SetRangeUser(0.4,2.2);
  hfDs1.GetYaxis()->SetNdivisions(0,0);
  hfDs1.SetFillStyle(4000);  
  hfDs1.GetXaxis()->SetTitle("f_{D_{s}}   (MeV)");
  C.cd();
  hfDs1.Draw();
  avg.DrawBox((fDsAvg2009-fDsAvg2009Er),0.4,(fDsAvg2009+fDsAvg2009Er),2.2);
  pad.Clear();
  pad.cd();
  hfDs1.Draw();
  GfDs1.Draw("pesame");
  C.cd();
  pad.Draw();  

  text.DrawLatex(204,2.0,"BELLE");
  sprintf(value,"(%.0f#pm%.0f)",fDsBelleMu,fDsBelleMuEr);
 // valtext.DrawLatex((fDsBelleMu-fDsBelleMuEr/2),2.02,value);
  fds3text.DrawLatex(204,2.0-.055,"(#mu#nu)"); 

  text.DrawLatex(204,1.8,"CLEOc");
  sprintf(value,"(%.0f#pm%.0f)",fDsCleoMu,fDsCleoMuEr);
 // valtext.DrawLatex((fDsCleoMu-fDsCleoMuEr/2),1.82,value);
  fds3text.DrawLatex(204,1.8-.055,"(#mu#nu)"); 

  text.DrawLatex(204,1.6,"BABAR");
  sprintf(value,"(%.0f#pm%.0f)",fDsBaBarOldMu,fDsBaBarOldMuEr);
 // valtext.DrawLatex((fDsBaBarOldMu-fDsBaBarOldMuEr/2),1.62,value);
  fds3text.DrawLatex(204,1.6-.055,"(#mu#nu)"); 

  text.DrawLatex(204,1.4,"CLEOc");
  sprintf(value,"(%.0f#pm%.0f)",fDsCleoTauToE,fDsCleoTauToEEr);
 // valtext.DrawLatex((fDsCleoTauToE-fDsCleoTauToEEr/2),1.42,value);
  fds3text.DrawLatex(204,1.4-.055,"(#tau #rightarrow e#nu#nu)");

  text.DrawLatex(204,1.2,"CLEOc");
  sprintf(value,"(%.0f#pm%.0f)",fDsCleoTauToPi,fDsCleoTauToPiEr);
 // valtext.DrawLatex((fDsCleoTauToPi-fDsCleoTauToPiEr/2),1.2,value);
  fds3text.DrawLatex(204,1.2-.055,"(#tau #rightarrow #pi#nu)");

  text.DrawLatex(204,1.0,"CLEOc");
  sprintf(value,"(%.0f#pm%.0f)",fDsCleoTauToRho,fDsCleoTauToRhoEr);
 // valtext.DrawLatex((fDsCleoTauToRho-fDsCleoTauToRhoEr/2),1.02,value);
  fds3text.DrawLatex(204,1.0-.055,"(#tau #rightarrow #rho#nu)");

  // text.DrawLatex(204,0.8,"BABAR");
  //sprintf(value,"(%.0f#pm%.0f)",fDsBaBarTauToE2,fDsBaBarTauToE2Er);
 // valtext.DrawLatex((fDsBaBarTauToE2-fDsBaBarTauToE2Er/2),0.82,value);
  //  fds3text.DrawLatex(204,0.8-.055,"(#tau #rightarrow e#nu#nu)");

  text.DrawLatex(204,0.5,"LQCD");
  sprintf(value,"(%.1f#pm%.1f)",fDsLQCD2008,fDsLQCD2008Er);
 // valtext.DrawLatex((fDsLQCD2008-3*fDsLQCD2008Er),0.52-0.04,value);


//   text.DrawLatex(204,0.2,"average");
//   sprintf(value,"(%.0f#pm%.0f)",fDsAvg2011,fDsAvg2011Er);
//   valtext.DrawLatex((fDsAvg2011-fDsAvg2011Er/2),0.2-0.04,value);

  C.Print("SummaryfDs.ps");



  ///---------------------------------------
  //Plot the fDs values after BaBar analysis
  //-----------------------------------------
  Float_t fDsAvg2011=257.3;//HFAG avg Jan 2011
  Float_t fDsAvg2011Er=5.3;
  TGraphErrors GfDs2;
  GfDs2.SetPoint(0,fDsBelleMu,2.0);
  GfDs2.SetPointError(0,fDsBelleMuEr,0);
  GfDs2.SetPoint(1,fDsCleoMu,1.8);
  GfDs2.SetPointError(1,fDsCleoMuEr,0);
  GfDs2.SetPoint(2,fDsBaBarMu,1.6);
  GfDs2.SetPointError(2,fDsBaBarMuEr,0);

  GfDs2.SetPoint(3,fDsCleoTauToE,1.4);
  GfDs2.SetPointError(3,fDsCleoTauToEEr,0);
  GfDs2.SetPoint(4,fDsCleoTauToPi,1.2);
  GfDs2.SetPointError(4,fDsCleoTauToPiEr,0);
  GfDs2.SetPoint(5,fDsCleoTauToRho,1.0);
  GfDs2.SetPointError(5,fDsCleoTauToRhoEr,0);
  GfDs2.SetPoint(6,fDsBaBarTauToE2,0.8);
  GfDs2.SetPointError(6,fDsBaBarTauToE2Er,0);
  GfDs2.SetPoint(7,fDsBaBarTauToE,0.6);
  GfDs2.SetPointError(7,fDsBaBarTauToEEr,0);
  GfDs2.SetPoint(8,fDsBaBarTauToMu,0.4);
  GfDs2.SetPointError(8,fDsBaBarTauToMuEr,0);

  GfDs2.SetPoint(9,fDsLQCD2008,0.12);
  GfDs2.SetPointError(9,fDsLQCD2008Er,0);

  C.Clear();
  TH1F hfDs3("hfDs3","",1,200,300);
  hfDs3.SetStats(0);
  hfDs3.GetYaxis()->SetRangeUser(0.0,2.2);
  hfDs3.GetYaxis()->SetNdivisions(0,0);
  hfDs3.SetFillStyle(4000);  
  hfDs3.GetXaxis()->SetTitle("f_{D_{s}}   (MeV)");
  C.cd();
  hfDs3.Draw();
  avg.DrawBox((fDsAvg2011-fDsAvg2011Er),0.0,(fDsAvg2011+fDsAvg2011Er),2.2);
  pad.Clear();
  pad.cd();
  hfDs3.Draw();
  GfDs2.Draw("pesame");
  C.cd();
  pad.Draw();  

  text.DrawLatex(204,2.0,"BELLE");
  sprintf(value,"(%.0f#pm%.0f)",fDsBelleMu,fDsBelleMuEr);
 // valtext.DrawLatex((fDsBelleMu-fDsBelleMuEr/2),2.02,value);
  fds3text.DrawLatex(204,2.0-.055,"(#mu#nu)"); 

  text.DrawLatex(204,1.8,"CLEOc");
  sprintf(value,"(%.0f#pm%.0f)",fDsCleoMu,fDsCleoMuEr);
 // valtext.DrawLatex((fDsCleoMu-fDsCleoMuEr/2),1.82,value);
  fds3text.DrawLatex(204,1.8-.055,"(#mu#nu)"); 

  text.DrawLatex(204,1.6,"BABAR");
  sprintf(value,"(%.0f#pm%.0f)",fDsBaBarMu,fDsBaBarMuEr);
 // valtext.DrawLatex((fDsBaBarMu-fDsBaBarMuEr/2),1.62,value);
  fds3text.DrawLatex(204,1.6-.055,"(#mu#nu)"); 

  text.DrawLatex(204,1.4,"CLEOc");
  sprintf(value,"(%.0f#pm%.0f)",fDsCleoTauToE,fDsCleoTauToEEr);
 // valtext.DrawLatex((fDsCleoTauToE-fDsCleoTauToEEr/2),1.42,value);
  fds3text.DrawLatex(204,1.4-.055,"(#tau #rightarrow e#nu#nu)");

  text.DrawLatex(204,1.2,"CLEOc");
  sprintf(value,"(%.0f#pm%.0f)",fDsCleoTauToPi,fDsCleoTauToPiEr);
 // valtext.DrawLatex((fDsCleoTauToPi-fDsCleoTauToPiEr/2),1.2,value);
  fds3text.DrawLatex(204,1.2-.055,"(#tau #rightarrow #pi#nu)");

  text.DrawLatex(204,1.0,"CLEOc");
  sprintf(value,"(%.0f#pm%.0f)",fDsCleoTauToRho,fDsCleoTauToRhoEr);
 // valtext.DrawLatex((fDsCleoTauToRho-fDsCleoTauToRhoEr/2),1.02,value);
  fds3text.DrawLatex(204,1.0-.055,"(#tau #rightarrow #rho#nu)");

  text.DrawLatex(204,0.8,"BABAR");
  sprintf(value,"(%.0f#pm%.0f)",fDsBaBarTauToE2,fDsBaBarTauToE2Er);
 // valtext.DrawLatex((fDsBaBarTauToE2-fDsBaBarTauToE2Er/2),0.82,value);
  fds3text.DrawLatex(204,0.8-.055,"(#tau #rightarrow e#nu#nu)");

  text.DrawLatex(204,0.6,"BABAR");
  sprintf(value,"(%.0f#pm%.0f)",fDsBaBarTauToE,fDsBaBarTauToEEr);
 // valtext.DrawLatex((fDsBaBarTauToE-fDsBaBarTauToEEr/2),0.62,value);
  fds3text.DrawLatex(204,0.6-.055,"(#tau #rightarrow e#nu#nu)"); ///(#tau #rightarrow K_{S}K)

  text.DrawLatex(204,0.4,"BABAR");
  sprintf(value,"(%.0f#pm%.0f)",fDsBaBarTauToMu,fDsBaBarTauToMuEr);
 // valtext.DrawLatex((fDsBaBarTauToMu-fDsBaBarTauToMuEr/2),0.42,value);
  fds3text.DrawLatex(204,0.4-.055,"(#tau #rightarrow #mu#nu#nu)");

  text.DrawLatex(204,0.12,"LQCD");
  sprintf(value,"(%.1f#pm%.1f)",fDsLQCD2008,fDsLQCD2008Er);
 // valtext.DrawLatex((fDsLQCD2008-3*fDsLQCD2008Er),0.2-0.04,value);


//   text.DrawLatex(204,0.2,"average");
//   sprintf(value,"(%.0f#pm%.0f)",fDsAvg2011,fDsAvg2011Er);
//   valtext.DrawLatex((fDsAvg2011-fDsAvg2011Er/2),0.2-0.04,value);

  C.Print("SummaryfDs.ps");




  //Plot fDs values after LQCD new value
  Float_t fDsLQCD2010=248.0;
  Float_t fDsLQCD2010Er=2.5;
  TGraphErrors GfDs3;
  GfDs3.SetPoint(0,fDsBelleMu,2.0);
  GfDs3.SetPointError(0,fDsBelleMuEr,0);
  GfDs3.SetPoint(1,fDsCleoMu,1.8);
  GfDs3.SetPointError(1,fDsCleoMuEr,0);
  GfDs3.SetPoint(2,fDsBaBarMu,1.6);
  GfDs3.SetPointError(2,fDsBaBarMuEr,0);

  GfDs3.SetPoint(3,fDsCleoTauToE,1.4);
  GfDs3.SetPointError(3,fDsCleoTauToEEr,0);
  GfDs3.SetPoint(4,fDsCleoTauToPi,1.2);
  GfDs3.SetPointError(4,fDsCleoTauToPiEr,0);
  GfDs3.SetPoint(5,fDsCleoTauToRho,1.0);
  GfDs3.SetPointError(5,fDsCleoTauToRhoEr,0);
  GfDs3.SetPoint(6,fDsBaBarTauToE2,0.8);
  GfDs3.SetPointError(6,fDsBaBarTauToE2Er,0);
  GfDs3.SetPoint(7,fDsBaBarTauToE,0.6);
  GfDs3.SetPointError(7,fDsBaBarTauToEEr,0);
  GfDs3.SetPoint(8,fDsBaBarTauToMu,0.4);
  GfDs3.SetPointError(8,fDsBaBarTauToMuEr,0);

  GfDs3.SetPoint(9,fDsLQCD2010,0.12);
  GfDs3.SetPointError(9,fDsLQCD2010Er,0);


  C.Clear();
//   TH1F hfDs3("hfDs3","",1,200,300);
//   hfDs3.SetStats(0);
//   hfDs3.GetYaxis()->SetRangeUser(0.0,2.2);
//   hfDs3.GetYaxis()->SetNdivisions(0,0);
//   hfDs3.SetFillStyle(4000);  
//   hfDs3.GetXaxis()->SetTitle("f_{D_{s}}   (MeV)");
  C.cd();
  hfDs3.Draw();
  avg.DrawBox((fDsAvg2011-fDsAvg2011Er),0.0,(fDsAvg2011+fDsAvg2011Er),2.2);
  pad.Clear();
  pad.cd();
  hfDs3.Draw();
  GfDs3.Draw("pesame");
  C.cd();
  pad.Draw();  

  text.DrawLatex(204,2.0,"BELLE");
  sprintf(value,"(%.0f#pm%.0f)",fDsBelleMu,fDsBelleMuEr);
 // valtext.DrawLatex((fDsBelleMu-fDsBelleMuEr/2),2.02,value);
  fds3text.DrawLatex(204,2.0-.055,"(#mu#nu)"); 

  text.DrawLatex(204,1.8,"CLEOc");
  sprintf(value,"(%.0f#pm%.0f)",fDsCleoMu,fDsCleoMuEr);
 // valtext.DrawLatex((fDsCleoMu-fDsCleoMuEr/2),1.82,value);
  fds3text.DrawLatex(204,1.8-.055,"(#mu#nu)"); 

  text.DrawLatex(204,1.6,"BABAR");
  sprintf(value,"(%.0f#pm%.0f)",fDsBaBarMu,fDsBaBarMuEr);
 // valtext.DrawLatex((fDsBaBarMu-fDsBaBarMuEr/2),1.62,value);
  fds3text.DrawLatex(204,1.6-.055,"(#mu#nu)"); 

  text.DrawLatex(204,1.4,"CLEOc");
  sprintf(value,"(%.0f#pm%.0f)",fDsCleoTauToE,fDsCleoTauToEEr);
 // valtext.DrawLatex((fDsCleoTauToE-fDsCleoTauToEEr/2),1.42,value);
  fds3text.DrawLatex(204,1.4-.055,"(#tau #rightarrow e#nu#nu)");

  text.DrawLatex(204,1.2,"CLEOc");
  sprintf(value,"(%.0f#pm%.0f)",fDsCleoTauToPi,fDsCleoTauToPiEr);
 // valtext.DrawLatex((fDsCleoTauToPi-fDsCleoTauToPiEr/2),1.2,value);
  fds3text.DrawLatex(204,1.2-.055,"(#tau #rightarrow #pi#nu)");

  text.DrawLatex(204,1.0,"CLEOc");
  sprintf(value,"(%.0f#pm%.0f)",fDsCleoTauToRho,fDsCleoTauToRhoEr);
 // valtext.DrawLatex((fDsCleoTauToRho-fDsCleoTauToRhoEr/2),1.02,value);
  fds3text.DrawLatex(204,1.0-.055,"(#tau #rightarrow #rho#nu)");

  text.DrawLatex(204,0.8,"BABAR");
  sprintf(value,"(%.0f#pm%.0f)",fDsBaBarTauToE2,fDsBaBarTauToE2Er);
 // valtext.DrawLatex((fDsBaBarTauToE2-fDsBaBarTauToE2Er/2),0.82,value);
  fds3text.DrawLatex(204,0.8-.055,"(#tau #rightarrow e#nu#nu)");

  text.DrawLatex(204,0.6,"BABAR");
  sprintf(value,"(%.0f#pm%.0f)",fDsBaBarTauToE,fDsBaBarTauToEEr);
 // valtext.DrawLatex((fDsBaBarTauToE-fDsBaBarTauToEEr/2),0.62,value);
  fds3text.DrawLatex(204,0.6-.055,"(#tau #rightarrow e#nu#nu)"); ///(#tau #rightarrow K_{S}K)

  text.DrawLatex(204,0.4,"BABAR");
  sprintf(value,"(%.0f#pm%.0f)",fDsBaBarTauToMu,fDsBaBarTauToMuEr);
 // valtext.DrawLatex((fDsBaBarTauToMu-fDsBaBarTauToMuEr/2),0.42,value);
  fds3text.DrawLatex(204,0.4-.055,"(#tau #rightarrow #mu#nu#nu)");

  text.DrawLatex(204,0.12,"LQCD");
  sprintf(value,"(%.1f#pm%.1f)",fDsLQCD2010,fDsLQCD2010Er);
 // valtext.DrawLatex((fDsLQCD2010-3*fDsLQCD2010Er),0.2-0.04,value);


//   text.DrawLatex(204,0.2,"average");
//   sprintf(value,"(%.0f#pm%.0f)",fDsAvg2011,fDsAvg2011Er);
//   valtext.DrawLatex((fDsAvg2011-fDsAvg2011Er/2),0.2-0.04,value);

  C.Print("SummaryfDs.ps");



  C.Print("SummaryfDs.ps]");

}




void ComputefDsAverageMuTau(){


  Double_t fDs[2];
  Double_t fDsEr[2];
  
  //MuNu
  ComputefDs(5.90e-3,0.33e-3,0,massMu,massMuEr,&fDs[0],&fDsEr[0]);

  //TauNu
  //ComputefDs(5.43e-2,0.31e-2,0,massTau,massTauEr,&fDs[1],&fDsEr[1]);  
  ComputefDs(0.0528413,0.0028053,0,massTau,massTauEr,&fDs[1],&fDsEr[1]); //include Shanes TauToE


  ///Average of MuNu and the Taus
  ComputeErrorWeightAvg(fDs[0]*1000,fDsEr[0]*1000,
			fDs[1]*1000,fDsEr[1]*1000);
  
}
