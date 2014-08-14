#define Npi 5
Float_t NDst0Bins=35;
Float_t Dst0MMin=1.8;
Float_t Dst0MMax=2.45;

void FitDst0MassVsnX(){

  TFile file("DsTolnu/SP1005Sig/DsReco/Final/Ntuple.root","read");
  gROOT->cd();
  TTree*Ntuple=(TTree*)file.Get("Ntuple");

  TCanvas C("C");
  TString fname="Dst0MassVsnX.ps";
  C.Print(fname+"[");

  C.Clear();
  TH1F HTot("HTot","",NDst0Bins,Dst0MMin,Dst0MMax);
  Ntuple->Draw("dsstuncmass>>HTot","abs(dsstuncmass-dsuncmass-.143)<.03&&fragnpi0s<=1&&fragngams==0&&fragnpis<=3");
  TF1*ftot=FitHistogram1gausPlusPolN(&HTot,Dst0MMin,Dst0MMax,2,0);
  HTot.GetYaxis()->SetRangeUser(0,HTot.GetMaximum()*1.1);
  HTot.Draw("pe");
  C.Print(fname);

  //Total with 0pi0
  C.Clear();
  TH1F HTot0pi0("HTot0pi0","",NDst0Bins,Dst0MMin,Dst0MMax);
  Ntuple->Draw("dsstuncmass>>HTot0pi0","abs(dsstuncmass-dsuncmass-.143)<.03&&fragnpi0s==0&&fragngams==0&&fragnpis<=3");
  TF1*ftot0pi0=FitHistogram1gausPlusPolN(&HTot0pi0,Dst0MMin,Dst0MMax,2,0);
  HTot0pi0.GetYaxis()->SetRangeUser(0,HTot0pi0.GetMaximum()*1.1);
  HTot0pi0.Draw("pe");
  C.Print(fname);

  TLine cutline;
  cutline.SetLineColor(4);

  //0 pi0 vs N_pi+
  C.Clear();
  C.Divide(2,2);  
  TH1F* H0pi0[Npi];
  for(Int_t i=0;i<Npi;i++){
    C.cd(i+1);
    TString hname=TString("H0pi0")+(long)i;
    H0pi0[i]=new TH1F(hname,"",NDst0Bins,Dst0MMin,Dst0MMax);
    Ntuple->Draw(TString("dsstuncmass>>")+hname,TString("abs(dsstuncmass-dsuncmass-.143)<.03&&fragnpi0s==0&&fragngams==0&&fragnpis==")+(long)i);
    TF1*f=FitHistogram1gausPlusPolN(H0pi0[i],Dst0MMin,Dst0MMax,2,0);
    f->FixParameter(1,ftot0pi0->GetParameter(1));
    H0pi0[i]->Fit(f);
    H0pi0[i]->GetYaxis()->SetRangeUser(0,H0pi0[i]->GetMaximum()*1.1);
    H0pi0[i]->Draw("pe");
    cutline.DrawLine(ftot0pi0->GetParameter(1)-2.5*f->GetParameter(2),0,ftot0pi0->GetParameter(1)-2.5*f->GetParameter(2),H0pi0[i]->GetMaximum());
    cutline.DrawLine(ftot0pi0->GetParameter(1)+2.5*f->GetParameter(2),0,ftot0pi0->GetParameter(1)+2.5*f->GetParameter(2),H0pi0[i]->GetMaximum());
  }
  C.Print(fname);


  //Total with 1pi0
  C.Clear();
  TH1F HTot1pi0("HTot1pi0","",NDst0Bins,Dst0MMin,Dst0MMax);
  Ntuple->Draw("dsstuncmass>>HTot1pi0","abs(dsstuncmass-dsuncmass-.143)<.03&&fragnpi0s==1&&fragngams==0&&fragnpis<=3&&fragpi0true>0");
  TF1*ftot1pi0=FitHistogram1gausPlusPolN(&HTot1pi0,Dst0MMin,Dst0MMax,2,0);
  //ftot1pi0->FixParameter(1,ftot0pi0->GetParameter(1));
  HTot1pi0.Fit(ftot1pi0);
  HTot1pi0.GetYaxis()->SetRangeUser(0,HTot1pi0.GetMaximum()*1.1);
  HTot1pi0.Draw("pe");
  cutline.DrawLine(ftot1pi0->GetParameter(1)-2.5*ftot1pi0->GetParameter(2),0,ftot1pi0->GetParameter(1)-2.5*ftot1pi0->GetParameter(2),HTot1pi0->GetMaximum());
  cutline.DrawLine(ftot1pi0->GetParameter(1)+2.5*ftot1pi0->GetParameter(2),0,ftot1pi0->GetParameter(1)+2.5*ftot1pi0->GetParameter(2),HTot1pi0->GetMaximum());
  C.Print(fname);
  
  //1 pi0 vs N_pi+
  C.Clear();
  C.Divide(2,2);
  TH1F* H1pi0[Npi];
  for(Int_t i=0;i<Npi;i++){
    C.cd(i+1);
    TString hname=TString("H1pi0")+(long)i;
    H1pi0[i]=new TH1F(hname,"",NDst0Bins,Dst0MMin,Dst0MMax);
    Ntuple->Draw(TString("dsstuncmass>>")+hname,TString("abs(dsstuncmass-dsuncmass-.143)<.03&&fragnpi0s==1&&fragpi0true>0&&fragngams==0&&fragnpis==")+(long)i);
    TF1*f=FitHistogram1gausPlusPolN(H1pi0[i],Dst0MMin,Dst0MMax,2,0);
    f->FixParameter(1,ftot1pi0->GetParameter(1));
    H1pi0[i]->Fit(f);
    H1pi0[i]->GetYaxis()->SetRangeUser(0,H1pi0[i]->GetMaximum()*1.1);
    H1pi0[i]->Draw("pe");
  }
  C.Print(fname);


  //Total with 1gamma
  C.Clear();
  TH1F HTot1Gam("HTot1Gam","",NDst0Bins,Dst0MMin,Dst0MMax);
  Ntuple->Draw("dsstuncmass>>HTot1Gam","abs(dsstuncmass-dsuncmass-.143)<.03&&fragnpi0s==0&&fragngams==1&&fragnpis<=3");
  TF1*ftot1Gam=FitHistogram1gausPlusPolN(&HTot1Gam,Dst0MMin,Dst0MMax,2,0);
  //ftot1Gam->FixParameter(1,ftot0pi0->GetParameter(1));
  HTot1Gam.Fit(ftot1Gam);
  HTot1Gam.GetYaxis()->SetRangeUser(0,HTot1Gam.GetMaximum()*1.1);
  HTot1Gam.Draw("pe");
  C.Print(fname);
  
  //1 gamma vs N_pi+
  C.Clear();
  C.Divide(2,2);
  TH1F* H1Gam[Npi];
  for(Int_t i=0;i<Npi;i++){
    C.cd(i+1);
    TString hname=TString("H1Gam")+(long)i;
    H1Gam[i]=new TH1F(hname,"",NDst0Bins,Dst0MMin,Dst0MMax);
    Ntuple->Draw(TString("dsstuncmass>>")+hname,TString("abs(dsstuncmass-dsuncmass-.143)<.03&&fragnpi0s==0&&fragngams==1&&fragnpis==")+(long)i);
    TF1*f=FitHistogram1gausPlusPolN(H1Gam[i],Dst0MMin,Dst0MMax,2,0);
    f->FixParameter(1,ftot1Gam->GetParameter(1));
    H1Gam[i]->Fit(f);
    H1Gam[i]->GetYaxis()->SetRangeUser(0,H1Gam[i]->GetMaximum()*1.1);
    H1Gam[i]->Draw("pe");
  }
  C.Print(fname);

  C.Print(fname+"]");

  file.Close();
}
