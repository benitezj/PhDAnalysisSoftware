#include "../Globals.h"
#include "../FUNCTIONS/Fits.h"
#include <TLine.h>
#include <TGraphErrors.h>

void fitDs1DM(TString rootfile){

  TFile*file=new TFile(rootfile,"read");

  TCanvas Canvas("Canvas","Canvas");
  TString fname="Ds1DMFit.ps";
  Canvas.Print(fname+"[");
  TLine line;

  TH1F*HDs1DM=(TH1F*)file->Get("Ds1 DeltaM2  Cut6");
  if(HDs1DM==NULL)HDs1DM=(TH1F*)file->Get("Ds1 DeltaM2  Cut7");
  if(HDs1DM==NULL){cout<<"No histo."<<endl;return;}
   
  TObject* stats=HDs1DM->FindObject("stats");
  if(stats!=NULL)delete stats;else cout<<"no stats found"<<endl;
  Float_t initpar[20]={5,.027,.001,1,1,1,1,1,1,1,1};
  TF1* f=FitHistogramRelBreitWignerPolN(HDs1DM,.005,.06,7,initpar);
  
  TF1* fbw=new TF1("fbw","[0]*x*[1]*[2]/((x**2-[1]**2)**2+([1]**2)*([2]**2))",0,.1);
  fbw->SetParameter(0,f->GetParameter(0));
  fbw->SetParameter(1,f->GetParameter(1));
  fbw->SetParameter(2,f->GetParameter(2));
  fbw->SetLineColor(3);
  fbw->SetLineWidth(1);
  fbw->Draw("same");

  TF1* fbkg=new TF1("fbkg","[0]+[1]*x+[2]*x**2+[3]*x**3+[4]*x**4+[5]*x**5+[6]*x**6+[7]*x**7",0,.1);
  fbkg->SetParameter(0,f->GetParameter(3));
  fbkg->SetParameter(1,f->GetParameter(4));
  fbkg->SetParameter(2,f->GetParameter(5));
  fbkg->SetParameter(3,f->GetParameter(6));
  fbkg->SetParameter(4,f->GetParameter(7));
  fbkg->SetParameter(5,f->GetParameter(8));
  fbkg->SetParameter(6,f->GetParameter(9));
  fbkg->SetParameter(7,f->GetParameter(10));

  fbkg->SetLineColor(4);
  fbkg->SetLineWidth(1);
  
  
  



  Canvas.Clear();
  //Canvas.Divide(1,2);
  //Canvas.cd(1);
  HDs1DM->Draw("pe");  
  fbw->Draw("same");
  fbkg->Draw("same");
  Canvas.Print(fname);



  //////////////Create Residuals
  Int_t Nbins=HDs1DM->GetXaxis()->GetNbins();
  TGraphErrors*GRes=new TGraphErrors();
  TGraphErrors*GResPercent=new TGraphErrors();
  
  Float_t integral=0;
  Float_t residual=0;
  Float_t bwidth=HDs1DM->GetBinWidth(1);
  for(Int_t b=1;b<=Nbins;b++){

    integral=f->Integral(HDs1DM->GetBinLowEdge(b),HDs1DM->GetBinLowEdge(b)+bwidth);
    residual=HDs1DM->GetBinContent(b)-integral/bwidth;
    //cout<<b<<" "<<integral<<" "<<residual<<endl;
   
    GRes->SetPoint(b,HDs1DM->GetBinLowEdge(b)+bwidth/2,residual);
    GRes->SetPointError(b,0,HDs1DM->GetBinError(b));

    GResPercent->SetPoint(b,HDs1DM->GetBinLowEdge(b)+bwidth/2,100*residual/HDs1DM->GetBinContent(b));
    GResPercent->SetPointError(b,0,100*HDs1DM->GetBinError(b)/HDs1DM->GetBinContent(b));
    //HRes->SetBinContent(b,res);
  }

  Canvas.Clear();
 
  //Canvas.cd(2);
  GRes->Draw("ap");
  GRes->GetYaxis()->SetRangeUser(-15.,15.);
  GRes->GetXaxis()->SetRangeUser(HDs1DM->GetBinLowEdge(1),HDs1DM->GetBinLowEdge(Nbins)+bwidth);
  GRes->GetYaxis()->SetTitle("Residual");
  GRes->GetXaxis()->SetTitle("DeltaM(D_{s1})");
  GRes->Draw("ape");
  line.DrawLine(0,.0,HDs1DM->GetBinLowEdge(Nbins)+bwidth,0);  
  Canvas.Print(fname);



  Canvas.Clear();
 
  //Canvas.cd(2);
  GResPercent->Draw("ap");
  GResPercent->GetYaxis()->SetRangeUser(-100.,100.);
  GResPercent->GetXaxis()->SetRangeUser(HDs1DM->GetBinLowEdge(1),HDs1DM->GetBinLowEdge(Nbins)+bwidth);
  GResPercent->GetYaxis()->SetTitle("Residual/Entries");
  GResPercent->GetXaxis()->SetTitle("DeltaM(D_{s1})");
  GResPercent->Draw("ape");
  line.DrawLine(0,.0,HDs1DM->GetBinLowEdge(Nbins)+bwidth,0);  
  Canvas.Print(fname);

  
  Canvas.Print(fname+"]");
  //file->Close();
  //delete file;
  
}

