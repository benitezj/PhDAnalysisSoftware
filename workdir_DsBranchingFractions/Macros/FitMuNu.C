#include "../tmp/Linux26SL5_i386_gcc412/DRecoilToolsUser/NtupleCode/DRecoHistPdf.hh"
#include "../tmp/Linux26SL5_i386_gcc412/DRecoilToolsUser/NtupleCode/DRecoAddPdf.hh"
#include "../tmp/Linux26SL5_i386_gcc412/DRecoilToolsUser/NtupleCode/DRecoHistPdfChi2Var.hh"
//#include "../SOFTWARE/FUNCTIONS/RebinHisto.h"
#include "RooPlot.h"
#include "RooMinuit.h"
#include "RooRealVar.h"
#include "RooArgList.h"
#include "RooArgSet.h"
#include "RooDataHist.h"
#include "RooGenericPdf.h"
#include <TFile.h>
#include <TH1F.h>
#include <TCanvas.h>

void FitMuNu(){


  Float_t xmin=-0.5;
  Float_t xmax=3.5;
  Int_t nbins=40;
  RooRealVar mass("mass","M_{recoil}(DKX#gamma#mu)",xmin,xmax);
  mass.setBins(nbins);
  mass.setUnit("GeV^{2}");
  RooArgSet norm(mass);

  TFile FData("$fdsdocdir/Denominator/NominalFitResults/NumeratorFitResults_MC.root","read");
  TH1F* HData=(TH1F*)((TH1F*)FData.Get("DsToMuNu_RunAll_EExtraCut"))->Clone("my");
  RooDataHist DHData("DHData","DHData",RooArgSet(mass),HData);

  TFile FSig("$fdsdocdir/Denominator/NominalFitResults/NumeratorEfficiencies.root","read");
  TH1F* HSigTmp=(TH1F*)((TH1F*)FSig.Get("DsToMuNuNXTAll"))->Clone("my"); 
  //if(!(HSig=(TH1F*)RebinHistoX(HSig,nbins,xmin,xmax))){cout<<" Bad histo for rebin"<<endl;return;}
  TH1F* HSig=new TH1F("HSig","HSig",nbins,xmin,xmax);
  for(Int_t b=1;b<=nbins;b++){ 
    HSig->SetBinContent(b,HSigTmp->GetBinContent(11+2*(b-1))+HSigTmp->GetBinContent(11+2*(b-1)+1));
    HSig->SetBinError(b,sqrt(pow(HSigTmp->GetBinError(11+2*(b-1)),2)+pow(HSigTmp->GetBinError(11+2*(b-1)+1),2)));
  }
  RooDataHist DHSig("DHSig","DHSig",RooArgSet(mass),HSig);
  DRecoHistPdf PdfSig("PdfSig","PdfSig",norm,DHSig,0);
  RooRealVar YieldSig("YieldSig","YieldSig",.001,.2);
  //pdflist.add(PdfSig);
  //coeflist.add(YieldSig);


  //read in the backgrounds
  TFile FBkg("$fdsdocdir/Denominator/NominalFitResults/NumeratorFitResults_MC.root","read");
  RooArgList bkgpdflist;
  RooArgList bkgcoeflist;

  //Background number 1
//   TH1F* HBkg1=(TH1F*)((TH1F*)FBkg.Get("DsToMuNu_RunAll_EExtraCut"))->Clone("my");
//   HSig->Sumw2();
//   HSig->Scale(321/HSig->Integral());
//   HBkg1->Add(HSig,-1);
//   RooDataHist DHBkg1("DHBkg1","DHBkg1",RooArgSet(mass),HBkg1);
//   DRecoHistPdf PdfBkg1("PdfBkg1","PdfBkg1",norm,DHBkg1,0);
//   RooRealVar YieldBkg1("YieldBkg1","YieldBkg1",.001,.2);
//   bkgpdflist.add(PdfBkg1);
//   //bkgcoeflist.add(YieldBkg1);

  RooRealVar c1("c1","c1",0,3.5);
  RooRealVar c2("c2","c2",.01,3.);
  RooRealVar c3("c3","c3",0,.95);
  RooRealVar c4("c4","c4",-.5,3.5);
  RooGenericPdf PdfBkg1("PdfBkg1","PdfBkg1","exp(-1./(mass+.5))*(3.5-mass)*(exp(-.5*(mass-c1)**2/c2**2)+c3*exp(-.5*(mass-c4)**2/c2**2))",RooArgList(mass,c1,c2,c3,c4));
  RooRealVar YieldBkg1("YieldBkg1","YieldBkg1",.001,.9);
  bkgpdflist.add(PdfBkg1);

  //Background number 2
//   TH1F* HBkg2=(TH1F*)((TH1F*)FBkg.Get("DsToMuNu_RunAll_EExtraCut"))->Clone("my");
//   HSigPDF->Scale(321/HSigPDF->Integral());
//   HBkg2->Add(HSigPDF,-1);
//   RooDataHist DHBkg2("DHBkg2","DHBkg2",RooArgSet(mass),HBkg2);
//   DRecoHistPdf PdfBkg2("PdfBkg2","PdfBkg2",norm,DHBkg2,0);
//   RooRealVar YieldBkg2("YieldBkg2","YieldBkg2",.001,.2);
//   bkgpdflist.add(PdfBkg2);
  //bkgcoeflist.add(YieldBkg2);//last coef not to be added
  

  //Construct total Bkg
  DRecoAddPdf PdfBkgTot("PDFBkgTot","PDFBkgTot",norm,bkgpdflist,bkgcoeflist);
  
  //Construct total PDF
  DRecoAddPdf PdfTot("PDFTot","PDFTot",norm,PdfSig,PdfBkgTot,YieldSig);
  
  //chi2Var
  DRecoHistPdfChi2Var chi2("chi2","chi2",PdfTot,DHData);
  RooMinuit minuit(chi2);
  minuit.migrad();
  
  
  TCanvas C("C");
  C.Print("MuNuFit.ps[");

  RooPlot* plot=mass.frame(); 
  plot->SetTitle(TString("yield= ")+(long)(HData->Integral()*YieldSig.getVal())+" #pm "+(long)(HData->Integral()*YieldSig.getError()));
  //plot->addTH1(HData,"hist pe");

  //create the total fit histogram 
  TH1F* HPdfTot=(TH1F*)PdfTot.CreateHistogram("HPdfTot");
  HPdfTot->Scale(HData->Integral()/HPdfTot->Integral());
  //HPdfTot->SetFillColor(2);
  //HPdfTot->SetMarkerSize(.001);
  HPdfTot->SetLineColor(2);
  HPdfTot->SetLineWidth(2);
  plot->addTH1(HPdfTot,"hist");
  
  //create total bkg fit histogram
  TH1F* HPdfBkgTot=(TH1F*)PdfBkgTot.CreateHistogram("HPdfBkgTot");
  HPdfBkgTot->Scale(HData->Integral()*(1-YieldSig.getVal())/HPdfBkgTot->Integral());
  HPdfBkgTot->SetLineColor(0);
  HPdfBkgTot->SetFillColor(4);
  plot->addTH1(HPdfBkgTot,"hist");

  //plot Data again on top
  plot->addTH1(HData,"hist pe");

  plot->Draw();
  C.Print("MuNuFit.ps");
  delete plot;

  C.Print("MuNuFit.ps]");
}
