#include "../../Globals.h"
#include "DstPiAnalysis.h"
#include "DstPiAnalysisControl.C"
#include <TStyle.h>

void FitMasVsHelicity(Bool_t fit){

  Float_t chisquare;
  Float_t ndof;
  Float_t D2420yield;
  Float_t D2460yield;
  Float_t D2420yieldEr;
  Float_t D2460yieldEr;
  Float_t D2420yieldTotal;
  Float_t D2460yieldTotal;
  Float_t D2420yieldErTotal;
  Float_t D2460yieldErTotal;
  char signame[100];
  TString SigName;
  Float_t dummy;
  TH1F HD2420VsHelicity("HD2420VsHelicity","HD2420VsHelicity",5,0,1);
  TH1F HD2460VsHelicity("HD2460VsHelicity","HD2460VsHelicity",5,0,1);
  TH1F HD2420Total("HD2420Total","HD2420Total",5,0,1);
  TH1F HD2460Total("HD2460Total","HD2460Total",5,0,1);

  DstPiAnalysis*a=analyzeDstPi(0,0);
  ifstream ParameterOutput;

  //make the inclusive fit   
  if(fit){
    a->RooFitMassPeak(4);     
    system(TString("mv ")+a->GetOutPutDir()+"/FitPars_0100_0100_5.txt "+a->GetOutPutDir()+"/FitPars_Helicity"+(long)(5)+".txt");
    system(TString("mv ")+a->GetOutPutDir()+"/DstPiRooFit_0100_0100_5.ps "+a->GetOutPutDir()+"/DstPiRooFit_Helicity"+(long)(5)+".ps");
  }
  ParameterOutput.open((const char*)(a->GetOutPutDir()+"/FitPars_Helicity"+(long)(5)+".txt"));    
  ParameterOutput>>chisquare>>ndof;
  ParameterOutput>>dummy>>dummy>>dummy;
  ParameterOutput>>signame>>D2420yieldTotal>>D2420yieldErTotal;//D2420yieldErTotal=sqrt(D2420yieldTotal);
  SigName=TString(signame);
  if(SigName!="D2420"){cout<<"Name does not match "<<signame<<endl;return;}   
  ParameterOutput>>dummy>>dummy;
  ParameterOutput>>dummy>>dummy;
  ParameterOutput>>signame>>D2460yieldTotal>>D2460yieldErTotal;//D2460yieldErTotal=sqrt(D2460yieldTotal);
  SigName=TString(signame);
  if(SigName!="D2460"){cout<<"Name does not match "<<signame<<endl;return;}
   
  ParameterOutput.close();  
  ParameterOutput.clear();
  cout<<"read file"<<10<<endl;
  

  //now make individual fits
  for(Int_t h=0;h<5;h++){
        
    if(fit){
      a->RooFitMassPeak(h+20);     
      system(TString("mv ")+a->GetOutPutDir()+"/FitPars_0100_0100_5.txt "+a->GetOutPutDir()+"/FitPars_Helicity"+(long)(h)+".txt");
      system(TString("mv ")+a->GetOutPutDir()+"/DstPiRooFit_0100_0100_5.ps "+a->GetOutPutDir()+"/DstPiRooFit_Helicity"+(long)(h)+".ps");
    }
  
    ParameterOutput.open((const char*)(a->GetOutPutDir()+"/FitPars_Helicity"+(long)(4-h)+".txt"));    
    ParameterOutput>>chisquare>>ndof;
    ParameterOutput>>dummy>>dummy>>dummy;
    ParameterOutput>>signame>>D2420yield>>D2420yieldEr;//D2420yieldEr=sqrt(D2420yield);
    SigName=TString(signame);
    if(SigName!="D2420"){cout<<"Name does not match "<<signame<<endl;return;}
    HD2420VsHelicity.SetBinContent(h+1,D2420yield+1);
    HD2420VsHelicity.SetBinError(h+1,D2420yieldEr);

    ParameterOutput>>dummy>>dummy;
    ParameterOutput>>dummy>>dummy;
    ParameterOutput>>signame>>D2460yield>>D2460yieldEr;//D2460yieldEr=sqrt(D2460yield);
    SigName=TString(signame);
    if(SigName!="D2460"){cout<<"Name does not match "<<signame<<endl;return;}
    HD2460VsHelicity.SetBinContent(h+1,D2460yield+1);
    HD2460VsHelicity.SetBinError(h+1,D2460yieldEr);
   

    HD2420Total.SetBinContent(h+1,D2420yieldTotal+1);
    HD2420Total.SetBinError(h+1,D2420yieldErTotal+1);
    HD2460Total.SetBinContent(h+1,D2460yieldTotal+1);
    HD2460Total.SetBinError(h+1,D2460yieldErTotal+1);

    ParameterOutput.close();  
    ParameterOutput.clear();
    cout<<"read file"<<h<<endl;
  }

  TString filename=a->GetOutPutDir()+"/FitVsHelicity.ps";
  TCanvas Canvas("Canvas","Canvas");
  Canvas.Print(filename+"[");

  Canvas.Clear();
  HD2420Total.Draw("pe");
  HD2460Total.Draw("pesame");
  Canvas.Print(filename);

  Canvas.Clear();
  HD2420VsHelicity.SetMarkerSize(1.5);
  HD2420VsHelicity.SetMarkerColor(1);
  HD2420VsHelicity.SetLineColor(1);
  HD2460VsHelicity.SetMarkerSize(1.5);
  HD2460VsHelicity.SetMarkerColor(4);
  HD2460VsHelicity.SetLineColor(4);   
  HD2420VsHelicity.GetYaxis()->SetRangeUser(0,HD2420VsHelicity.GetMaximum()*1.1);
  HD2420VsHelicity.Draw("pe");
  HD2460VsHelicity.Draw("pesame");
  Canvas.Print(filename);

  HD2420VsHelicity.Sumw2();
  HD2460VsHelicity.Sumw2();
  HD2420VsHelicity.Divide(&HD2420Total);//Scale(1./D2420yieldTotal);
  HD2460VsHelicity.Divide(&HD2460Total);//Scale(1./D2460yieldTotal);
  delete HD2420VsHelicity.FindObject("stats");
  delete HD2460VsHelicity.FindObject("stats");
  
  
  /////////////Fit the ratios
  TStyle* gStyle=gROOT->GetStyle("myStyle");
  gStyle->SetOptStat("i");
  
  TF1 FSinCos("FSinCos","[0]*(1-x**2)+[1]*x**2",-1,1);
  FSinCos.SetParameters(1,2);
  FSinCos.SetParNames("sin^{2}(#theta_{H})","cos^{2}(#theta_{H})");
  

  HD2420VsHelicity.Fit(&FSinCos,"I","pe",-1,1);
  TPaveStats*stats=(TPaveStats*)HD2420VsHelicity.FindObject("stats");
  if(!stats){cout<<"no stats D2420"<<endl;return;}
  stats->SetX1NDC(.7);
  stats->SetX2NDC(.95);
  stats->SetY1NDC(.4);
  stats->SetY2NDC(.6);
  stats->SetFillColor(0);
  stats->SetFillStyle(1001);
  stats->SetLineColor(1);
  stats->SetLineStyle(1001);
  stats->SetTextColor(1);
  stats->SetBorderSize(2);

  HD2460VsHelicity.Fit(&FSinCos,"I","pe",-1,1); 
  Canvas.Update();
  stats=(TPaveStats*)HD2460VsHelicity.FindObject("stats");
  if(!stats){cout<<"no stats D2460"<<endl;return;}
  stats->SetX1NDC(.7);
  stats->SetX2NDC(.95);
  stats->SetY1NDC(.2);
  stats->SetY2NDC(.4);
  stats->SetFillColor(0);
  stats->SetFillStyle(1001);
  stats->SetLineColor(4);
  stats->SetLineStyle(1001);
  stats->SetTextColor(4);
  stats->SetBorderSize(2);


  Canvas.Clear();
  HD2420VsHelicity.GetYaxis()->SetRangeUser(0,.5);
  HD2420VsHelicity.Draw("pe");
  HD2460VsHelicity.Draw("pesame");
  Canvas.Print(filename);
  

  gStyle->SetOptStat("imr");
  
  Canvas.Print(filename+"]");
 
}

void FoldMassVsHelicity(TString _OutputDir){

  
  TFile HistFile(_OutputDir+"/LegendreWeightedHistos.root","update");
 
  TH1F* HDstarPi1;
  TH1F* HDstarPi2;
  

  for(Int_t i=0;i<5;i++){
    HistFile.Delete(TString("HDstPiMVsHelicity")+(long)(i+10));
    
    HDstarPi1=(TH1F*)HistFile.Get(TString("HDstPiMVsHelicity")+(long)(i));
    if(!HDstarPi1)return;
    HDstarPi2=(TH1F*)HistFile.Get(TString("HDstPiMVsHelicity")+(long)(9-i));
    if(!HDstarPi2)return;
    
    HDstarPi1=(TH1F*)HDstarPi1->Clone();
    HDstarPi1->Sumw2();
    HDstarPi1->SetName(TString("HDstPiMVsHelicity")+(long)(i+10));
    HDstarPi1->SetTitle(TString("HDstPiMVsHelicity")+(long)(i+10));
    HDstarPi1->Add(HDstarPi2);
    HDstarPi1->Write();
  }
  
  HistFile.Close();
  
}
