#include <TTree.h>
#include <TCut.h>
#include <TChain.h>
#include <TString.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TF1.h>
#include <TFile.h>
#include "../DRecoilToolsUser/NtupleCode/TreeChain.rdl"
#include "../DRecoilToolsUser/NtupleCode/Fits.rdl"

Float_t xmin=1.87;
Float_t xmax=2.07;
Int_t nbins=50;

#define  ndsmbins 20
Float_t dsmmin=1.7;
Float_t dsmmax=2.1;
Float_t dsmstep=.02; 

Float_t dsmean=1.968;
Float_t dssigma=.0082;


///////////Notes for later:
/*
- Need to redefine the input Ds lists as they have different mass windows.
- Fit up to Ds* with gamma (Ds background should not peak in mmass ?)
- Clean out dirty modes with p* cuts
- Weight events by multiplicity?
- Study the p*(Ds) distribution.
- Study resolution as a function of M_Recoil(DKXg)
- Fit in deltaM instead of Ds mass? or cut on deltaM
- 
*/



void MakeDsToHadronic(Int_t dataset){
  TChain chain("ntp1");
  
  TString path="DsTolnu/SP1005SigHadronic/Ntuples";
  if(dataset==1)path="DsTolnu/SP1005SigHadronic/Ntuples";
  if(dataset==2)path="DsTolnu/SP1005SigHadronic/NtuplesWS";
  if(dataset==1||dataset==2){
    TreeChain(&chain,1,24,path+"/Run1On/Ntuple");
    TreeChain(&chain,1,2,path+"/Run1Off/Ntuple");
    TreeChain(&chain,1,68,path+"/Run2On/Ntuple");
    TreeChain(&chain,1,6,path+"/Run2Off/Ntuple");
    TreeChain(&chain,1,34,path+"/Run3On/Ntuple");
    TreeChain(&chain,1,2,path+"/Run3Off/Ntuple");
    TreeChain(&chain,1,100,path+"/Run4On/Ntuple");
    TreeChain(&chain,1,6,path+"/Run4Off/Ntuple");
    TreeChain(&chain,1,148,path+"/Run5On/Ntuple");
    TreeChain(&chain,1,14,path+"/Run5Off/Ntuple");
    TreeChain(&chain,1,42,path+"/Run6On/Ntuple");
    TreeChain(&chain,1,10,path+"/Run6Off/Ntuple");
  }

  if(dataset==11)path="DsTolnu/SP1005SkimHadronic/Ntuples";
  if(dataset==12)path="DsTolnu/SP1005SkimHadronic/NtuplesWS";
  if(dataset==11||dataset==12){
    TreeChain(&chain,1,99,path+"/Run1/Ntuple");
    TreeChain(&chain,1,282,path+"/Run2/Ntuple");
    TreeChain(&chain,1,140,path+"/Run3/Ntuple");
    TreeChain(&chain,1,422,path+"/Run4/Ntuple");
    TreeChain(&chain,1,612,path+"/Run5/Ntuple");
    TreeChain(&chain,1,175,path+"/Run6/Ntuple");
  }
  


  TCut modecut="!(DsnDaus==4&&Dsd1Lund==310)";//Ds->KsKpipi has narrow mass window with no signal
  TCut pstarcut="Dsp3CM>2.5";
  TCut masscut="1.87<DsMass&&DsMass<2.07";
  TCut masssigcut="abs(DsMass-1.968)<.024";
  TCut masssidecut=".032<abs(DsMass-1.968)&&abs(DsMass-1.968)<.056";
  TCut totcut=modecut+pstarcut+masscut;
  
  //TTree* ntuple=chain->CopyTree(modecut+pstarcut+masscut);
  
  TFile HistoFile(path+"/HistosFile.root","recreate");
 
  
  //Ds mass from Hadronic reco
  TH1F HDsH("HDsH","",nbins,xmin,xmax);
  chain.Draw("DsMass>>HDsH",totcut);
  HDsH.Write();

  //Ds mass from Hadronic reco truth-matched
  TH1F HDsHTrue("HDsHTrue","",nbins,xmin,xmax);
  chain.Draw("DsMass>>HDsHTrue",totcut+TCut("DsMCIdx>0"));
  HDsHTrue.Write();

//Ds* deltam from Hadronic reco
//   C.Clear();
//   TH1F HDsStH("HDsStH","",nbins,xmin,xmax);
//   //chain.Draw("sqrt(Ds*DsDsE-(DsDsPx*DsDsPx+DsDsPy*DsDsPy+DsDsPz*DsPz))>>HDsH");
//   C.Print(filename);

  //Ds mass from Recoil
  TH1F HDsR("HDsR","",ndsmbins,dsmmin,dsmmax);
  chain.Draw("sqrt(DsDsE*DsDsE-(DsDsPx*DsDsPx+DsDsPy*DsDsPy+DsDsPz*DsDsPz))>>HDsR",totcut+masssigcut);
  HDsR.Write();

  //plot of the signal-MC 
//   TH1F HDsR("HDsR","",nbinsrec,xminrec,xmaxrec);
//   chain.Draw("sqrt(DsDsE*DsDsE-(DsDsPx*DsDsPx+DsDsPy*DsDsPy+DsDsPz*DsDsPz))>>HDsR",totcut+masssigcut);
//   HDsR.Write();

  TH1F HDsRSide("HDsRSide","",ndsmbins,dsmmin,dsmmax);
  chain.Draw("sqrt(DsDsE*DsDsE-(DsDsPx*DsDsPx+DsDsPy*DsDsPy+DsDsPz*DsDsPz))>>HDsRSide",totcut+masssidecut);
  HDsRSide.Write();

  //Hadronic mass vs Recoil mass
  Int_t nbinscoarse=25;
  
  for(Int_t b=0;b<ndsmbins;b++){
    TString hname=TString("HDsHVsReco")+(long)b;
    char text[1000];
    sprintf(text,"%.6f<sqrt(DsDsE*DsDsE-(DsDsPx*DsDsPx+DsDsPy*DsDsPy+DsDsPz*DsDsPz))&&sqrt(DsDsE*DsDsE-(DsDsPx*DsDsPx+DsDsPy*DsDsPy+DsDsPz*DsDsPz))<%.6f",dsmmin+b*dsmstep,dsmmin+(b+1)*dsmstep);
    
    TH1F HDsHVsRec(hname,"",nbinscoarse,xmin,xmax);
    TCut mslicecut=text;
    chain.Draw(TString("DsMass>>")+hname,totcut+mslicecut);
    HDsHVsRec.Write();

  }
  HistoFile.Close();

}



void FitDsToHadronic(Int_t dataset){

  TString path="DsTolnu/SP1005SigHadronic/Ntuples";
  if(dataset==1)path="DsTolnu/SP1005SigHadronic/Ntuples";
  if(dataset==2)path="DsTolnu/SP1005SigHadronic/NtuplesWS";
  if(dataset==11)path="DsTolnu/SP1005SkimHadronic/Ntuples";
  if(dataset==12)path="DsTolnu/SP1005SkimHadronic/NtuplesWS";


  TCanvas C("C");
  TString filename=path+"/DsToHadPlots.ps";
  C.Print(filename+"[");

  TFile HistoFile(path+"/HistosFile.root","read");
  
  //Ds mass from Hadronic reco
  TH1F* HDsH=(TH1F*)HistoFile.Get("HDsH");
  TF1* FitF=FitHistogram1gausPlusPol1(HDsH,HDsH->GetXaxis()->GetXmin(),HDsH->GetXaxis()->GetXmin());
  C.Clear();
  HDsH->GetYaxis()->SetRangeUser(0,1.1*HDsH->GetMaximum());
  HDsH->Draw("pe");
  C.Print(filename);

  //Ds mass from Recoil
  TH1F* HDsR=(TH1F*)HistoFile.Get("HDsR");
  C.Clear();
  HDsR->GetYaxis()->SetRangeUser(0,1.1*HDsR->GetMaximum());
  HDsR->Draw("pe");
  C.Print(filename);

  //Hadronic mass vs Recoil mass
  TH1F* HDsHVsRec[ndsmbins];  
  TH1F HDsRYield("HDsRYield","",ndsmbins,dsmmin,dsmmax);
  FitF->FixParameter(1,dsmean);
  FitF->FixParameter(2,dssigma);
  TF1 Fgaus("Fgaus","[0]*exp(-.5*(x-[1])**2/[2]**2)",xmin,xmax);
  Fgaus.FixParameter(1,dsmean);
  Fgaus.FixParameter(2,dssigma);
  C.Clear();
  C.Divide(ndsmbins/4,4);
  for(Int_t b=0;b<ndsmbins;b++){
    C.cd(b+1);
    
    HDsHVsRec[b]=(TH1F*)HistoFile.Get(TString("HDsHVsReco")+(long)b);
    Fgaus.SetParLimits(0,0,HDsHVsRec[b]->GetMaximum());
    HDsHVsRec[b]->Fit(FitF);
    HDsHVsRec[b]->Draw("pe");

    Fgaus.SetParameter(0,FitF->GetParameter(0));
    HDsRYield.SetBinContent(b+1,Fgaus.Integral(xmin,xmax)/dsmstep);
    HDsRYield.SetBinError(b+1,(Fgaus.Integral(xmin,xmax)/dsmstep)*(FitF->GetParError(0)/FitF->GetParameter(0)));
  }  
  C.Print(filename);
 

  C.Clear();
  HDsRYield.Draw("pe");
  C.Print(filename);
 

  TH1* HDsRYieldNorm=(TH1*)HDsRYield.Clone();
  HDsRYieldNorm->Scale(HDsR->Integral()/HDsRYieldNorm->Integral());
  HDsRYieldNorm->SetLineColor(2);
  HDsRYieldNorm->SetMarkerColor(2);
  HDsRYieldNorm->SetStats(0);
  C.Clear();
  HDsR->GetYaxis()->SetRangeUser(0,HDsR->GetMaximum()*1.2);
  HDsR->Draw("pe");
  HDsRYieldNorm->Draw("lsame");
  C.Print(filename);
  
  C.Print(filename+"]");


  HistoFile.Close();
}
