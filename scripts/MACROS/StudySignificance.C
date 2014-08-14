
#include <iostream>
#include <iomanip>
#include <TROOT.h>
#include <TMath.h>
#include <TString.h>
using namespace std;

Double_t ComputeSig(Double_t Chi2Nominal,
		Int_t NDFNominal,		
		Double_t Chi2NoSignal,
		Int_t NDFNoSignal){


  Double_t probNominal=TMath::Prob(Chi2Nominal,NDFNominal);
  Double_t probNoSignal=TMath::Prob(Chi2NoSignal,NDFNoSignal);

  cout<<" Chi2Nominal="<<Chi2Nominal<<endl;
  cout<<" Chi2NoSignal="<<Chi2NoSignal<<endl;
  cout<<" NDFNominal="<<NDFNominal<<endl;
  cout<<" NDFNoSignal="<<NDFNoSignal<<endl;
  cout<<" probNominal="<<probNominal<<endl;
  cout<<" probNoSignal="<<probNoSignal<<endl;



  //method 1
  //Double_t Sig=TMath::ChisquareQuantile(probNominal-probNoSignal,1);

  //method 2
  //Double_t Sig=sqrt(deltaChi2);
  
  //method 3
  //Double_t Sig=TMath::ChisquareQuantile(TMath::Prob(Chi2NoSignal-Chi2Nominal,1),1);

  //method 4
  Double_t Sig=sqrt(-2*log(probNoSignal/probNominal));


  cout<<"Statistical  Significance = "<<Sig<<endl;
  return Sig;
}


//generate the toys
#include <TF1.h>
#include <TH1F.h>
#include <TFile.h>
void StudySignificanceGenerate(){

  TF1 FBkg("FBkg","[0]+[1]*x",-10,10);
  FBkg.SetParameter(0,1);
  FBkg.SetParameter(1,0);


  TF1 FSig("FSig","[0]*exp(-0.5*(x-[1])**2/[2]**2)",-10,10);
  FSig.SetParameter(0,1);
  FSig.SetParameter(1,0);
  FSig.SetParameter(2,1);

  for(Int_t i=0;i<10;i++){

    TH1F HToy("HToy","",80,-10,10);  
    HToy.FillRandom("FBkg",3000000);
    HToy.FillRandom("FSig",1000*i);
    HToy.SetBinContent(0,1000*i);
    
    TFile File(TString("DstarCharged/DstarPi/StudySignificance/Toy")+(long)i+".root","recreate");
    HToy.Write();
    File.Close();
  }
}

//fit the distributions
#include <RooRealVar.h>
#include <RooGaussian.h>
#include <RooGenericPdf.h>
#include <RooAddPdf.h>
#include <RooPlot.h>
#include <RooChi2Var.h>
#include <RooMinuit.h>
#include <RooFitResult.h>
void StudySignificanceFit(){

//   TF1 FTot("FTot","[0]*([3]*exp(-0.5*(x-[1])**2/[2]**2)/([2]*sqrt(2*3.14159)) + (1-[3])/20)",-10,10);
//   FTot.FixParameter(1,0);
//   FTot.FixParameter(2,1);
//   FTot.SetParLimits(3,0,1);
 
  RooRealVar x("x","x",-10,10);
  RooRealVar mean("mean","mean",0);
  RooRealVar sigma("sigma","sigma",1);
  RooGaussian Gauss("Gauss","Gauss",x,mean,sigma);
  RooGenericPdf Linear("Linear","Linear","1",RooArgList(x));
  RooRealVar yield("yield","yield",0,1);
  RooAddPdf FTot("FTot","Ftot",Gauss,Linear,yield);
  
  TF1 FBkg("FBkg","[0]/20",-10,10);

  for(Int_t i=0;i<10;i++){
    TFile FileIn(TString("DstarCharged/DstarPi/StudySignificance/Toy")+(long)i+".root","read");
    TH1F*HToy=(TH1F*)FileIn.Get("HToy");
    
    //FTot.FixParameter(0,HToy->Integral()*HToy->GetXaxis()->GetBinWidth(1));
    //FBkg.FixParameter(0,HToy->Integral()*HToy->GetXaxis()->GetBinWidth(1));

    RooDataHist Data("Data","Data",RooArgList(x),HToy);
    RooChi2Var chi2("chi2","chi2",FTot,Data);
    RooMinuit minuit(chi2);
    minuit.migrad();
    RooFitResult* res=minuit.save();
    res->SetName("result");
    RooPlot*plot=x.frame();
    Data.plotOn(plot);
    FTot.plotOn(plot);
    plot->SetName("plot");
    
    TFile FileOut(TString("DstarCharged/DstarPi/StudySignificance/Fit")+(long)i+".root","recreate");
//     HToy->Fit(&FTot,"QI","",-10,10);      
//     FTot.SetName("FTot0");
//     FTot.Write();
//     //binwidth systematic
//     TH1*HToyR=HToy->Rebin(2,"rebin");
//     HToyR->Fit(&FTot,"QI","",-10,10);
//     FTot.SetName("FTot1");
//     FTot.Write();
//     //range systematic
//     HToy->Fit(&FTot,"QI","",-9,9);
//     FTot.SetName("FTot2");
//     FTot.Write();

    res->Write();
    plot->Write();

    HToy->Fit(&FBkg,"Q","",-10,10);
    FBkg.Write();
    FileOut.Close();
    delete res;
    delete plot;
  }


}

//Plot the significance
#include <TCanvas.h>
#include <TGraph.h>
void StudySignificancePlot(){
  
  TCanvas C("C");
  C.Print("DstarCharged/DstarPi/StudySignificance/StudySignificance.ps[");
  
  TH1F HChi2("HChi2","",10,-.5,9.5);
  TH1F HYield("HYield","",10,-.5,9.5);
  TH1F HYieldTrue("HYieldTrue","",10,-.5,9.5);
  TH1F HYieldStat("HYieldStat","",10,-.5,9.5);
  TH1F HYieldSyst("HYieldSyst","",10,-.5,9.5);
  TGraph GStatSig;
  TGraph GMySig;

  for(Int_t i=0;i<10;i++){
    TFile FileH(TString("DstarCharged/DstarPi/StudySignificance/Toy")+(long)i+".root","read");
    TH1F*HToy=(TH1F*)FileH.Get("HToy");
    TFile FileF(TString("DstarCharged/DstarPi/StudySignificance/Fit")+(long)i+".root","read");
    TF1*FTot=(TF1*)FileF.Get("FTot0");
    TF1*FBkg=(TF1*)FileF.Get("FBkg");
    RooFitResult*res=(RooFitResult*)FileF.Get("result");
    RooPlot*plot=(RooPlot*)FileF.Get("plot");
    
    C.Clear();
//     HToy->GetYaxis()->SetRangeUser(0,HToy->GetMaximum()*1.1);
//     HToy->SetTitle(TString("Toy")+(long)i);
//     HToy->Draw("pe");
//     FTot->Draw("same");
    plot->Draw();
    FBkg->Draw("same");
    C.Print("DstarCharged/DstarPi/StudySignificance/StudySignificance.ps");
    
    GStatSig.SetPoint(i,i,ComputeSig(res->minNll(),//FTot->GetChisquare(),
				     HToy->GetXaxis()->GetNbins()-res->floatParsFinal().getSize(),//FTot->GetNumberFreeParameters(),
				     FBkg->GetChisquare(),
				     HToy->GetXaxis()->GetNbins()-FBkg->GetNumberFreeParameters()));

    //add systematics
    //TF1*FTot1=(TF1*)FileF.Get("FTot1");
    //TF1*FTot2=(TF1*)FileF.Get("FTot2");
    Float_t yield=((RooRealVar*)res->floatParsFinal().first())->getVal()*HToy->Integral();//FTot->GetParameter(3)*HToy->Integral();
    Float_t staterr=((RooRealVar*)res->floatParsFinal().first())->getError()*HToy->Integral();//FTot->GetParError(3)*HToy->Integral();
    Float_t systerr=0;//sqrt(pow(FTot->GetParameter(3)-FTot1->GetParameter(3),2)+pow(FTot->GetParameter(3)-FTot2->GetParameter(3),2))*HToy->Integral();
    Float_t err=sqrt(pow(staterr,2)+pow(systerr,2));
    

    HChi2.SetBinContent(i+1,res->minNll()/(HToy->GetXaxis()->GetNbins()-res->floatParsFinal().getSize()));//FTot->GetChisquare()/FTot->GetNDF());
    HYield.SetBinContent(i+1,yield);
    HYield.SetBinError(i+1,err);
    HYieldTrue.SetBinContent(i+1,HToy->GetBinContent(0));
    HYieldStat.SetBinContent(i+1,staterr);
    HYieldSyst.SetBinContent(i+1,systerr);
      
    cout<<"My Significnace = "<<yield<<"/"<<err
	<<"="<<yield/err<<endl;
    GMySig.SetPoint(i,i,yield/err);
    


    FileH.Close();FileF.Close();
  }


  C.Clear();
  HChi2.SetStats(0);
  HChi2.GetYaxis()->SetRangeUser(0,1.5);
  HChi2.GetXaxis()->SetTitle("Toy id");
  HChi2.GetYaxis()->SetTitle("chi2/ndf");
  HChi2.Draw("p");
  C.Print("DstarCharged/DstarPi/StudySignificance/StudySignificance.ps");

  C.Clear();
  HYield.SetStats(0);
  HYield.GetYaxis()->SetRangeUser(0,HYieldTrue.GetMaximum()*1.2);
  HYield.GetXaxis()->SetTitle("Toy id");
  HYield.GetYaxis()->SetTitle("Fit Yield");
  HYield.Draw("pe");
  HYieldTrue.Draw("hist same");
  C.Print("DstarCharged/DstarPi/StudySignificance/StudySignificance.ps");

  
  C.Clear();
  HYieldStat.SetStats(0);
  HYieldStat.GetYaxis()->SetRangeUser(0,HYieldStat.GetMaximum()*1.2);
  HYieldStat.GetXaxis()->SetTitle("Toy id");
  HYieldStat.GetYaxis()->SetTitle("Stat Error");
  HYieldStat.Draw("p");
  HYieldSyst.Draw("hist same");
  C.Print("DstarCharged/DstarPi/StudySignificance/StudySignificance.ps");

  C.Clear();
  GMySig.GetYaxis()->SetRangeUser(0,10);
  GMySig.GetXaxis()->SetTitle("Toy id");
  GMySig.GetYaxis()->SetTitle("Significance");
  GMySig.SetLineColor(1);
  GMySig.SetLineWidth(2);
  GMySig.Draw("al");
  GStatSig.SetLineColor(1);
  GStatSig.SetLineWidth(2);
  GStatSig.Draw("pl same");
  C.Print("DstarCharged/DstarPi/StudySignificance/StudySignificance.ps");

  C.Print("DstarCharged/DstarPi/StudySignificance/StudySignificance.ps]");

}
