#include "DstPiAnalysis.h"

Int_t DstPiAnalysis::CompareWithK3pi(Bool_t getsideband){

  filename=_OutputDir+"/CompareWithK3pi.ps";
  Canvas.Print(filename+"[");

  Int_t nfitbins=200;
  Float_t fitmin=2.1;
  Float_t fitmax=3.1;

  TFile* HistFile=new TFile(_OutputDir+"/LegendreWeightedHistos.root","read");
  gROOT->cd();
  
  TH1F*HDstarPi=(TH1F*)HistFile->Get(TString("HDstPiM"));
  if(!HDstarPi)return 0;
  HDstarPi=CleanAndRebinHisto(HDstarPi,nfitbins,fitmin,fitmax);
  if(!HDstarPi)return 0;

  TFile* HistFile2=new TFile(_OutputDir+"/../K3pi/LegendreWeightedHistos.root","read");
  gROOT->cd();
  
  TH1F*HDstarPi2=(TH1F*)HistFile2->Get(TString("HDstPiM"));
  if(!HDstarPi2)return 0;
  HDstarPi2=CleanAndRebinHisto(HDstarPi2,nfitbins,fitmin,fitmax);
  if(!HDstarPi2)return 0;
  HDstarPi2->Sumw2();

  Canvas.Clear();
  HDstarPi2->Draw();
  HDstarPi->SetMarkerColor(4);
  HDstarPi->Draw("same");
  Canvas.Print(filename);

  TH1F*HDstarPi2Clone=(TH1F*)HDstarPi2->Clone();

  HDstarPi2->Scale(HDstarPi->Integral()/HDstarPi2->Integral());
  Canvas.Clear();
  HDstarPi2->Draw();
  HDstarPi->Draw("same");
  Canvas.Print(filename);

  HDstarPi2->Add(HDstarPi,-1);
  Canvas.Clear();
  HDstarPi2->Draw();
  cutline.DrawLine(fitmin,0,fitmax,0);
  Canvas.Print(filename);

  HDstarPi2->Divide(HDstarPi);
  delete HDstarPi2->FindObject("stats");
  HDstarPi2->Fit("pol1");  
  Canvas.Clear();
  HDstarPi2->GetYaxis()->SetRangeUser(-.2,.2);
  HDstarPi2->Draw();
  cutline.DrawLine(fitmin,0,fitmax,0);
  Canvas.Update();
  MoveStatsBox(HDstarPi2,-1,1);
  Canvas.Print(filename);

  delete HDstarPi2;

  ////
  if(getsideband){
    TH1F* Hbkg=K3piDstarBkgEffWeighted(nfitbins,fitmin,fitmax);
    if(!Hbkg){cout<<"No K3pi Background histo"<<endl;return 0;}
  
    Canvas.Clear();
    HDstarPi2Clone->Draw();
    Hbkg->Draw("same");
    Canvas.Print(filename);

    HDstarPi2Clone->Add(Hbkg,-1);
    HDstarPi2Clone->Scale(HDstarPi->Integral()/HDstarPi2Clone->Integral()); 


    Canvas.Clear();
    HDstarPi2Clone->Draw();
    HDstarPi->Draw("same");
    Canvas.Print(filename);

    HDstarPi2Clone->Add(HDstarPi,-1);
    Canvas.Clear();
    HDstarPi2Clone->Draw();
    Canvas.Print(filename);


    HDstarPi2Clone->Divide(HDstarPi);
    delete HDstarPi2Clone->FindObject("stats");
    HDstarPi2Clone->Fit("pol1");  
    Canvas.Clear();
    HDstarPi2Clone->GetYaxis()->SetRangeUser(-.2,.2);
    HDstarPi2Clone->Draw();
    cutline.DrawLine(fitmin,0,fitmax,0);
    Canvas.Update();
    MoveStatsBox(HDstarPi2Clone,-1,1);
    Canvas.Print(filename);
 
    
    delete Hbkg;
  }

 

  Canvas.Print(filename+"]");
  delete HDstarPi2Clone;
  delete HDstarPi;
  
  return 1;
}




