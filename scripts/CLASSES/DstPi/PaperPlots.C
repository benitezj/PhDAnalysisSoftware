#include "../../Globals.h"
#include "../../AnalysisGlobals.h"
#include <TROOT.h>
#include <TAxis.h>
#include <TH1F.h>
#include <TFile.h>
#include <RooPlot.h>
#include <TGaxis.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TLatex.h>
#include <RooCurve.h>


//DPi mass cuts
#define MeanMass 1.86859

//D0Pi
#define DMassCutCenter 1.8645

//DstPi
#define D0MassMean D0PDGMass
#define D0MassCut .03
#define DstarDMMean .1454
#define DstarDMCut .002


void PaperSetDMassDefaults(TH1F*h,TString xlabel,TString ylabel){
  h->SetStats(0);
  h->GetYaxis()->SetRangeUser(0,1.1*h->GetMaximum());

  h->GetXaxis()->SetNdivisions(6);
  h->GetXaxis()->SetLabelSize(.08);
  h->GetXaxis()->SetTitleOffset(.80);
  h->GetXaxis()->SetTitle(xlabel+"      (GeV/c^{2})");
  h->GetXaxis()->SetTitleSize(.09);

  h->GetYaxis()->SetNdivisions(5);
  h->GetYaxis()->SetLabelSize(.08);
  h->GetYaxis()->SetTitleOffset(.71);
  h->GetYaxis()->SetTitle(TString("Entries / ")+ylabel);
  h->GetYaxis()->SetTitleSize(.09);

}

void PaperSetDMassMargins(TVirtualPad* p){
  p->SetLeftMargin(0.15);
  p->SetRightMargin(0.05);
  p->SetTopMargin(0.07);
  p->SetBottomMargin(0.15);
}

void PaperDMassPlot(){

  TFile DPiFile("DCharged/DChargedPi/KPiPiDMass/DMassPlot.root","read");
  TH1F* HDPiDMass=(TH1F*)DPiFile.Get("HDMass");
  TFile DzPiFile("Dzero/DzeroPi/AntimoData/KpiDMass/DMassPlot.root","read");
  TH1F* HDzPiDMass=(TH1F*)DzPiFile.Get("HDMassOne");
  TFile DstPiDFile("DstarCharged/DstarPi/Data/KpiAndK3piD0/HistosForFit.root","read");
  TH1F* HDstPiDMass=(TH1F*)DstPiDFile.Get("HDMass");
  TFile DstPiDstFile("DstarCharged/DstarPi/Data/KpiAndK3piDstar/HistosForFit.root","read");
  TH1F* HDstPiDM=(TH1F*)DstPiDstFile.Get("HDstDM");
  

  //fix up Antimos plot
  TH1F* HDzPiDMassNew=new TH1F("HDzPiDMass","",HDzPiDMass->GetXaxis()->GetNbins()+2,
			       HDzPiDMass->GetXaxis()->GetXmin()-.001,
			       HDzPiDMass->GetXaxis()->GetXmax()+.001);
  for(Int_t b=1;b<=HDzPiDMass->GetXaxis()->GetNbins();b++){
    HDzPiDMassNew->SetBinContent(b+1,HDzPiDMass->GetBinContent(b));
    HDzPiDMassNew->SetBinError(b+1,HDzPiDMass->GetBinError(b));
  }
  HDzPiDMassNew->SetBinContent(1,HDzPiDMassNew->GetBinContent(2));			       
  HDzPiDMassNew->SetBinContent(HDzPiDMassNew->GetXaxis()->GetNbins(),HDzPiDMassNew->GetBinContent(HDzPiDMassNew->GetXaxis()->GetNbins()-1));			       
  

  ////
  TCanvas C;
  C.Divide(2,2);
  TLine cutline;
  cutline.SetLineColor(2);
  TText text;
  text.SetTextSize(.1);

  TGaxis::SetMaxDigits(4);
  //TGaxis::SetDecimals(0);

  C.cd(1);
  PaperSetDMassMargins(C.GetPad(1));
  PaperSetDMassDefaults(HDPiDMass,"m(K^{-}#pi^{+}#pi^{+})","1 MeV/c^{2}");
  HDPiDMass->Draw("pe");
  Float_t LowMassCut=MeanMass-.0062*2.5;
  Float_t HighMassCut=MeanMass+.0062*2.5;
  Float_t LowSideLowMassCut=MeanMass-.0062*7.5;
  Float_t LowSideHighMassCut=MeanMass-.0062*5;
  Float_t HighSideLowMassCut=MeanMass+.0062*5;
  Float_t HighSideHighMassCut=MeanMass+.0062*7.5;
  cutline.DrawLine(LowMassCut,0,LowMassCut,HDPiDMass->GetMaximum());
  cutline.DrawLine(HighMassCut,0,HighMassCut,HDPiDMass->GetMaximum());
  cutline.DrawLine(LowSideLowMassCut,0,LowSideLowMassCut,.3*HDPiDMass->GetMaximum());
  cutline.DrawLine(LowSideHighMassCut,0,LowSideHighMassCut,.3*HDPiDMass->GetMaximum());
  cutline.DrawLine(HighSideLowMassCut,0,HighSideLowMassCut,.3*HDPiDMass->GetMaximum());
  cutline.DrawLine(HighSideHighMassCut,0,HighSideHighMassCut,.3*HDPiDMass->GetMaximum());
  text.DrawTextNDC(.25,.8,"a)");
    
  C.cd(2);
  PaperSetDMassMargins(C.GetPad(2));
  PaperSetDMassDefaults(HDzPiDMassNew,"m(K^{-}#pi^{+})","1 MeV/c^{2}");
  HDzPiDMassNew->GetYaxis()->SetDecimals(0);
  HDzPiDMassNew->Draw("pe");  
  Float_t DMassCutWindow=2.5*.0076;
  Float_t DMassCutSBMin=4*.0076;
  cutline.DrawLine(DMassCutCenter-DMassCutWindow,0,DMassCutCenter-DMassCutWindow,HDzPiDMassNew->GetMaximum());
  cutline.DrawLine(DMassCutCenter+DMassCutWindow,0,DMassCutCenter+DMassCutWindow,HDzPiDMassNew->GetMaximum());
  cutline.DrawLine(DMassCutCenter-DMassCutSBMin-DMassCutWindow,0,DMassCutCenter-DMassCutSBMin-DMassCutWindow,.2*HDzPiDMassNew->GetMaximum());
  cutline.DrawLine(DMassCutCenter-DMassCutSBMin,0,DMassCutCenter-DMassCutSBMin,.2*HDzPiDMassNew->GetMaximum());  
  cutline.DrawLine(DMassCutCenter+DMassCutSBMin,0,DMassCutCenter+DMassCutSBMin,.2*HDzPiDMassNew->GetMaximum());
  cutline.DrawLine(DMassCutCenter+DMassCutSBMin+DMassCutWindow,0,DMassCutCenter+DMassCutSBMin+DMassCutWindow,.2*HDzPiDMassNew->GetMaximum());
  text.DrawTextNDC(.25,.8,"b)");
  
  C.cd(3);
  PaperSetDMassMargins(C.GetPad(3));
  PaperSetDMassDefaults(HDstPiDMass,"D^{0} candidate mass","1 MeV/c^{2}");
  HDstPiDMass->Draw("pe");
  cutline.DrawLine(D0MassMean-D0MassCut,0,D0MassMean-D0MassCut,HDstPiDMass->GetMaximum());
  cutline.DrawLine(D0MassMean+D0MassCut,0,D0MassMean+D0MassCut,HDstPiDMass->GetMaximum());
  text.DrawTextNDC(.25,.8,"c)");
  
  C.cd(4);
  PaperSetDMassMargins(C.GetPad(4));
  PaperSetDMassDefaults(HDstPiDM,"m(D^{0}#pi^{+}) - m(D^{0})","0.1 MeV/c^{2}");
  HDstPiDM->Draw("pe");
  cutline.DrawLine(DstarDMMean-DstarDMCut,0,DstarDMMean-DstarDMCut,HDstPiDM->GetMaximum());
  cutline.DrawLine(DstarDMMean+DstarDMCut,0,DstarDMMean+DstarDMCut,HDstPiDM->GetMaximum());
  text.DrawTextNDC(.25,.8,"d)");
    
  C.Print("PaperDMassPlot.ps");
}



void PaperDPiFitPlots(){

  TFile* fDpi=new TFile("DCharged/DChargedPi/KPiPi/FitPars_SBW10_DBW10_NRBW10_NRBW10_NRBW110_NRBW110.root","read");
  gROOT->cd();
  RooPlot*DpiFit=(RooPlot*)fDpi->Get("fitframe");
  RooPlot*DpiFitSig=(RooPlot*)fDpi->Get("bkgsubframe");

  //names of curve
  //RooHist::h_MassFitterdataset
  //RooCurve::totalpdf_Norm[mass]_Comp[SignalPdf0_PS]
  //RooCurve::totalpdf_Norm[mass]_Comp[SignalPdf1_PS]
  //RooCurve::totalpdf_Norm[mass]_Comp[Bkg1BkgPdf_PS,SignalPdf2_PS]
  //RooCurve::totalpdf_Norm[mass]_Comp[Bkg1BkgPdf_PS,SignalPdf3_PS]
  //RooCurve::totalpdf_Norm[mass]_Comp[Bkg1BkgPdf_PS,SignalPdf4_PS_R]
  //RooCurve::totalpdf_Norm[mass]_Comp[Bkg1BkgPdf_PS,SignalPdf5_PS_R]
  //RooCurve::totalpdf_Norm[mass]_Comp[Bkg1BkgPdf_PS]
  //RooCurve::totalpdf_Norm[mass]
  

  TGaxis::SetMaxDigits(3);
  TCanvas* C=new TCanvas("C","",1000,600);
  TPad* inset=new TPad("inset","",.4,.4,.79,.84);
  inset->SetRightMargin(0);
  inset->SetLeftMargin(.08);
  inset->SetTopMargin(0);
  inset->SetBottomMargin(.08);
  inset->SetFillStyle(4000);
  inset->cd(); 
  DpiFitSig->GetYaxis()->SetTitle("");
  DpiFitSig->GetXaxis()->SetTitle("");
  DpiFitSig->GetXaxis()->SetRangeUser(2.4,2.89);
  DpiFitSig->GetXaxis()->SetNdivisions(6);
  DpiFitSig->GetXaxis()->SetLabelSize(.08);
  DpiFitSig->GetYaxis()->SetNdivisions(5);
  DpiFitSig->GetYaxis()->SetLabelSize(.08);
  DpiFitSig->GetYaxis()->SetRangeUser(-400,4500);
  RooCurve* Insetsig0=(RooCurve*)DpiFitSig->findObject("totalpdf_Norm[mass]_Comp[SignalPdf0_PS]");
  RooCurve* Insetsig1=(RooCurve*)DpiFitSig->findObject("totalpdf_Norm[mass]_Comp[SignalPdf1_PS]");
  RooCurve* Insetsig2=(RooCurve*)DpiFitSig->findObject("totalpdf_Norm[mass]_Comp[SignalPdf2_PS]");
  RooCurve* Insetsig3=(RooCurve*)DpiFitSig->findObject("totalpdf_Norm[mass]_Comp[SignalPdf3_PS]");
  Insetsig0->SetLineStyle(2);
  Insetsig1->SetLineStyle(2);
  Insetsig2->SetLineStyle(2);
  Insetsig3->SetLineStyle(2);
  DpiFitSig->Draw();

  C->cd();
  DpiFit->GetYaxis()->SetTitleOffset(.7);
  DpiFit->GetYaxis()->SetTitle("Events / (0.005 GeV/c^{2})");
  //DpiFit->GetXaxis()->SetTitle("M(D^{+}#pi^{-})      (GeV/c^{2})");
  DpiFit->GetXaxis()->SetTitle("");//join with DzPi Plot
  DpiFit->GetXaxis()->SetRangeUser(2.08,3.25);//set same as DzPi plot
  RooCurve* sig0=(RooCurve*)DpiFit->findObject("totalpdf_Norm[mass]_Comp[SignalPdf0_PS]");
  RooCurve* sig1=(RooCurve*)DpiFit->findObject("totalpdf_Norm[mass]_Comp[SignalPdf1_PS]");
  RooCurve* sig2=(RooCurve*)DpiFit->findObject("totalpdf_Norm[mass]_Comp[Bkg1BkgPdf_PS,SignalPdf2_PS]");
  RooCurve* sig3=(RooCurve*)DpiFit->findObject("totalpdf_Norm[mass]_Comp[Bkg1BkgPdf_PS,SignalPdf3_PS]");
  sig0->SetLineStyle(2);
  sig1->SetLineStyle(2);
  sig2->SetLineStyle(2);
  sig3->SetLineStyle(2);
  DpiFit->Draw();
  inset->Draw();
  TLatex text;
  text.SetTextSize(.05);
  text.DrawTextNDC(.15,.77,"Fit A");

 
  C->Print("PaperDPiFitPlots.ps");
}



void PaperDzPiFitPlots(){
      
  TFile fDpi("Dzero/DzeroPi/AntimoData/Kpi/FitPars_NRBW31_DBW31_NRBW31_NRBW31_NRBW310_NRBW310.root","read");
  gROOT->cd();
  RooPlot*DpiFitO=(RooPlot*)fDpi.Get("fitframe");
  RooPlot*DpiFit=(RooPlot*)DpiFitO->Clone("DpiFit");
  cout<<DpiFit<<endl;
//   RooPlot*DpiFitSigO=(RooPlot*)fDpi.Get("bkgsubframe");
//   RooPlot*DpiFitSig=(RooPlot*)DpiFitSigO->Clone("DpiFitSig");
//   cout<<DpiFitSig<<endl;
  fDpi.Close();
 
  TGaxis::SetMaxDigits(3);
 
  //Draw the main plot
  TCanvas* C=new TCanvas("Frame","Frame",1000,600);
  cout<<C<<endl;
  C->cd();
  DpiFit->GetYaxis()->SetTitleOffset(.7);
  DpiFit->GetYaxis()->SetTitle("Events / (0.005 GeV/c^{2})");
  //DpiFit->GetXaxis()->SetTitle("M(D^{0}#pi^{+})      (GeV/c^{2})");
  DpiFit->GetXaxis()->SetTitle("M(D#pi)      (GeV/c^{2})");
  DpiFit->GetXaxis()->SetRangeUser(DpiFit->GetXaxis()->GetXmin(),3.25);
  DpiFit->GetYaxis()->SetRangeUser(0,26000);
  DpiFit->Draw();

  /////Draw the plot label
  TLatex text;
  text.SetTextSize(.05);
  text.DrawTextNDC(.15,.77,"Fit B");


  //cout the x axis range for use in the D+pi- plot
  cout<<"Xmin="<<DpiFit->GetXaxis()->GetXmin()<<endl;
  cout<<"Xmax="<<DpiFit->GetXaxis()->GetXmax()<<endl;

//   //Draw the background subtracted plot
//   TPad* inset=new TPad("inset","inset",.4,.4,.79,.84);
//   cout<<inset<<endl;
//   inset->SetRightMargin(0);
//   inset->SetLeftMargin(.08);
//   inset->SetTopMargin(0);
//   inset->SetBottomMargin(.08);
//   inset->SetFillStyle(4000);
//   inset->Draw();
//   inset->cd(); 
//   DpiFitSig->GetYaxis()->SetTitle("");
//   DpiFitSig->GetXaxis()->SetTitle("");
//   DpiFitSig->GetXaxis()->SetRangeUser(2.4,2.9);
//   DpiFitSig->GetYaxis()->SetRangeUser(-800,11000);
//   DpiFitSig->GetXaxis()->SetNdivisions(6);
//   DpiFitSig->GetXaxis()->SetLabelSize(.08);
//   DpiFitSig->GetYaxis()->SetNdivisions(6);
//   DpiFitSig->GetYaxis()->SetLabelSize(.08);
//   DpiFitSig->GetYaxis()->SetRangeUser(-200,2900);
//   DpiFitSig->Draw();
  

  cout<<"Need to delete the fill of the signals and change the line style manually"<<endl;
}


void PaperDzPiFitPlots2(){
  //Draw the background subtracted plot as an inset  
  //This must be done using a separate postscript file because otherwise the program gets stuck. 
  //The plots need be merged by editing the postscript files.

  TFile fDpi("Dzero/DzeroPi/AntimoData/Kpi/FitPars_NRBW31_DBW31_NRBW31_NRBW31_NRBW310_NRBW310.root","read");
  gROOT->cd();
  RooPlot*DpiFitSigO=(RooPlot*)fDpi.Get("bkgsubframe");
  RooPlot*DpiFitSig=(RooPlot*)DpiFitSigO->Clone("DpiFitSig");
  cout<<DpiFitSig<<endl;
  fDpi.Close();
 
  TGaxis::SetMaxDigits(3);
  TCanvas* C2=new TCanvas("Frame2","Frame2",1000,600);
  cout<<C2<<endl;
  C2->cd();
  TPad* inset=new TPad("inset","inset",.4,.4,.79,.84);
  cout<<inset<<endl;
  inset->SetRightMargin(0);
  inset->SetLeftMargin(.08);
  inset->SetTopMargin(0);
  inset->SetBottomMargin(.08);
  inset->SetFillStyle(4000);
  inset->Draw();
  inset->cd(); 
  DpiFitSig->GetYaxis()->SetTitle("");
  DpiFitSig->GetXaxis()->SetTitle("");
  DpiFitSig->GetXaxis()->SetRangeUser(2.4,2.89);
  DpiFitSig->GetYaxis()->SetRangeUser(-800,11000);
  DpiFitSig->GetXaxis()->SetNdivisions(6);
  DpiFitSig->GetXaxis()->SetLabelSize(.08);
  DpiFitSig->GetYaxis()->SetNdivisions(6);
  DpiFitSig->GetYaxis()->SetLabelSize(.08);
  DpiFitSig->GetYaxis()->SetRangeUser(-200,2900);
  DpiFitSig->Draw();
  
  cout<<"Need to delete the fill of the signals and change the line style manually"<<endl;
}



void PaperDstPiFitPlotsD2420(){

  TFile* fDpi=new TFile("DstarCharged/DstarPi/Data/KpiAndK3pi/FitPars_D2420Helicity.root","read");
  gROOT->cd();
  RooPlot*DpiFit=(RooPlot*)fDpi->Get("fitframe");
  RooPlot*DpiFitSig=(RooPlot*)fDpi->Get("bkgsubframe");

  TGaxis::SetMaxDigits(3);
  TCanvas* C=new TCanvas("C","",1000,600);
  TPad* inset=new TPad("inset","",.4,.4,.79,.84);
  inset->SetRightMargin(0);
  inset->SetLeftMargin(.08);
  inset->SetTopMargin(.0);
  inset->SetBottomMargin(.08);
  inset->SetFillStyle(4000);
  inset->cd(); 
  DpiFitSig->GetYaxis()->SetTitle("");
  DpiFitSig->GetXaxis()->SetTitle("");
  DpiFitSig->GetXaxis()->SetRangeUser(2.4,2.89);
  DpiFitSig->GetXaxis()->SetNdivisions(8);
  DpiFitSig->GetXaxis()->SetLabelSize(.08);
  DpiFitSig->GetYaxis()->SetNdivisions(6);
  DpiFitSig->GetYaxis()->SetLabelSize(.08);
  DpiFitSig->GetYaxis()->SetRangeUser(-100,2600);
  DpiFitSig->Draw();
  C->cd();
  DpiFit->GetXaxis()->SetTitle("");
  DpiFit->GetXaxis()->SetLabelColor(0);
  DpiFit->GetYaxis()->SetTitle(" Events / ( 0.005 GeV/c^{2} )");
  DpiFit->GetYaxis()->SetTitleOffset(.7);
  DpiFit->GetYaxis()->SetRangeUser(1.0,18000);
  DpiFit->Draw();
  inset->Draw();
  TLatex text;
  text.SetTextSize(.05);
  text.DrawTextNDC(.15,.77,"Fit C");

  C->Print("PaperDstPiFitPlotsD2420.ps");

}

void PaperDstPiFitPlotsD2460(){

  TFile* fDpi=new TFile("DstarCharged/DstarPi/Data/KpiAndK3pi/FitPars_D2460Helicity.root","read");
  gROOT->cd();
  RooPlot*DpiFit=(RooPlot*)fDpi->Get("fitframe");
  RooPlot*DpiFitSig=(RooPlot*)fDpi->Get("bkgsubframe");

  TGaxis::SetMaxDigits(3);
  TCanvas* C=new TCanvas("C","",1000,600);
  TPad* inset=new TPad("inset","",.4,.42,.79,.84);
  inset->SetRightMargin(0);
  inset->SetLeftMargin(.08);
  inset->SetTopMargin(0);
  inset->SetBottomMargin(.08);
  inset->SetFillStyle(4000);
  inset->cd(); 
  DpiFitSig->GetYaxis()->SetTitle("");
  DpiFitSig->GetXaxis()->SetTitle("");
  DpiFitSig->GetXaxis()->SetRangeUser(2.4,2.89);
  DpiFitSig->GetXaxis()->SetNdivisions(8);
  DpiFitSig->GetXaxis()->SetLabelSize(.08);
  DpiFitSig->GetYaxis()->SetNdivisions(6);
  DpiFitSig->GetYaxis()->SetLabelSize(.08);
  DpiFitSig->GetYaxis()->SetRangeUser(-100,5500);
  DpiFitSig->Draw();
  C->cd();
  DpiFit->GetXaxis()->SetTitle("");
  DpiFit->GetXaxis()->SetLabelColor(0);
  DpiFit->GetYaxis()->SetTitle(" Events / ( 0.005 GeV/c^{2} )");
  DpiFit->GetYaxis()->SetTitleOffset(.7);
  DpiFit->GetYaxis()->SetRangeUser(1,19000);
  DpiFit->Draw();
  inset->Draw();
  TLatex text;
  text.SetTextSize(.05);
  text.DrawTextNDC(.15,.77,"Fit D");


  C->Print("PaperDstPiFitPlotsD2460.ps");
}

void PaperDstPiFitPlotsFinal(){

  //TFile* fDpi=new TFile("DstarCharged/DstarPi/Data/KpiAndK3pi/FitPars_NRBW31_NRBW31_NRBW30_NRBW30_NRBW30.root","read");
  TFile* fDpi=new TFile("DstarCharged/DstarPi/Data/KpiAndK3pi/FitPars_Final.root","read");
  gROOT->cd();
  RooPlot*DpiFit=(RooPlot*)fDpi->Get("fitframe");
  RooPlot*DpiFitSig=(RooPlot*)fDpi->Get("bkgsubframe");

  TGaxis::SetMaxDigits(3);
  TCanvas* C=new TCanvas("C","",1000,600);
  TPad* inset=new TPad("inset","",.4,.4,.79,.84);
  inset->SetRightMargin(0);
  inset->SetLeftMargin(.08);
  inset->SetTopMargin(0);
  inset->SetBottomMargin(.08);
  inset->SetFillStyle(4000);
  inset->cd(); 
  DpiFitSig->GetYaxis()->SetTitle("");
  DpiFitSig->GetXaxis()->SetTitle("");
  DpiFitSig->GetXaxis()->SetRangeUser(2.4,2.89);
  DpiFitSig->GetXaxis()->SetNdivisions(8);
  DpiFitSig->GetXaxis()->SetLabelSize(.08);
  DpiFitSig->GetYaxis()->SetNdivisions(6);
  DpiFitSig->GetYaxis()->SetLabelSize(.08);
  DpiFitSig->GetYaxis()->SetRangeUser(-300,11500);
  DpiFitSig->Draw();
  C->cd();
  DpiFit->GetXaxis()->SetTitle("M(D^{*+}#pi^{-})      (GeV/c^{2})");
  DpiFit->GetYaxis()->SetTitle(" Events / ( 0.005 GeV/c^{2} )");
  DpiFit->GetYaxis()->SetTitleOffset(.7);
  DpiFit->GetYaxis()->SetRangeUser(1,48000);
  DpiFit->Draw();
  inset->Draw();
  TLatex text;
  text.SetTextSize(.05);
  text.DrawTextNDC(.15,.77,"Fit E");


  C->Print("PaperDstPiFitPlotsFinal.ps");
}

