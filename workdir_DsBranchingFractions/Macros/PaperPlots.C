
//Ds mass cuts
#define DsMassMin 1.934
#define DsMassMax 2.012
 
#define marksize 1
#define linewidth 2 
#define markstyle 8


#define marksizetau 1
#define linewidthtau 3
#define markstyletau 9


void PaperSetDsDefaults(TH1*h,TString xlabel,TString ylabel){
  h->SetStats(0);
  h->SetTitle("");
  h->GetYaxis()->SetRangeUser(0,1.1*h->GetMaximum());
 
  h->GetXaxis()->SetNdivisions(6);
  h->GetXaxis()->SetLabelSize(.08);
  h->GetXaxis()->SetTitleOffset(.85);
  h->GetXaxis()->SetTitle(xlabel+"      (GeV/c^{2})");
  h->GetXaxis()->SetTitleSize(.09);
 
  h->GetYaxis()->SetNdivisions(5);
  h->GetYaxis()->SetLabelSize(.08);
  h->GetYaxis()->SetTitleOffset(.71);
  h->GetYaxis()->SetTitle(TString("Events / ")+ylabel);
  h->GetYaxis()->SetTitleSize(.09);
 
}
 
void PaperSetDsMargins(TVirtualPad* p){
  p->SetLeftMargin(0.15);
  p->SetRightMargin(0.00);
  p->SetTopMargin(0.095);
  p->SetBottomMargin(0.18);
}


void PaperPlotDenominator(){

  TFile file("/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResults/DenominatorFitResult_Data.root","read");
  TH2F*HData=(TH2F*)file.Get("HDsMassData");
  TH2F*HFit=(TH2F*)file.Get("TotPdfHistoFit");
  TH2F*HFitBkg=(TH2F*)file.Get("TotBkgPdfHistoFit");

  TH2F*HDataWS=(TH2F*)file.Get("HDsMassDataWS");
  TH2F*HFitWS=(TH2F*)file.Get("TotPdfHistoFitWS");
  TH2F*HFitBkgWS=(TH2F*)file.Get("TotBkgPdfHistoFitWS");

  
  TH1F*HDataN[4];
  TH1F*HFitN[4];
  TH1F*HFitNAxis[4];
  TH1F*HFitBkgN[4];
  for(Int_t b=1;b<=4;b++){
    HDataN[b-1]=new TH1F(TString("HDataN")+(long)b,"",HData->GetYaxis()->GetNbins(),HData->GetYaxis()->GetXmin(),HData->GetYaxis()->GetXmax());
    HFitNAxis[b-1]=new TH1F(TString("HDataNAxis")+(long)b,"",HData->GetYaxis()->GetNbins(),HData->GetYaxis()->GetXmin(),HData->GetYaxis()->GetXmax());
    for(Int_t m=1;m<=HData->GetYaxis()->GetNbins();m++){
      HDataN[b-1]->SetBinContent(m,HData->GetBinContent(b,m));
      HDataN[b-1]->SetBinError(m,HData->GetBinError(b,m));
    }

    HFitN[b-1]=new TH1F(TString("HFitN")+(long)b,"",HFit->GetYaxis()->GetNbins(),HFit->GetYaxis()->GetXmin(),HFit->GetYaxis()->GetXmax());
    for(Int_t m=1;m<=HFit->GetYaxis()->GetNbins();m++){
      HFitN[b-1]->SetBinContent(m,HFit->GetBinContent(b,m));
      HFitN[b-1]->SetBinError(m,HFit->GetBinError(b,m));
    }

    HFitBkgN[b-1]=new TH1F(TString("HFitBkgN")+(long)b,"",HFitBkg->GetYaxis()->GetNbins(),HFitBkg->GetYaxis()->GetXmin(),HFitBkg->GetYaxis()->GetXmax());
    for(Int_t m=1;m<=HFitBkg->GetYaxis()->GetNbins();m++){
      HFitBkgN[b-1]->SetBinContent(m,HFitBkg->GetBinContent(b,m));
      HFitBkgN[b-1]->SetBinError(m,HFitBkg->GetBinError(b,m));
    }
    
  }
  

  TCanvas C("C");
  C.Print("PaperDsPlots.ps[");
  
  ////////////////Plot of Ds Mass vs nXR
  C.Divide(2,2);
  TLine cutline;
  cutline.SetLineColor(2);
  TLatex text;
  text.SetTextSize(.1);
  TGaxis::SetMaxDigits(3);

  for(Int_t b=1;b<=4;b++){
    C.cd(b);
    PaperSetDsMargins(C.GetPad(b));
    PaperSetDsDefaults(HFitBkgN[b-1],"m_{r}(DKX#gamma)","6 MeV/c^{2}");
    HFitBkgN[b-1]->GetYaxis()->SetRangeUser(0,HFitN[b-1]->GetMaximum()*1.1);
    HFitBkgN[b-1]->SetLineColor(0);
    HFitBkgN[b-1]->SetFillColor(3);
    HFitBkgN[b-1]->Draw("hist");
    HFitN[b-1]->SetLineColor(2);
    HFitN[b-1]->SetMarkerColor(2);
    HFitN[b-1]->SetLineWidth(linewidth);
    HFitN[b-1]->Draw("same hist");
    HDataN[b-1]->SetMarkerSize(marksize);
    HDataN[b-1]->SetMarkerStyle(markstyle);
    HDataN[b-1]->SetLineWidth(linewidth);
    HDataN[b-1]->Draw("pe same hist");
    HFitN[b-1]->Draw("same hist");
    //HFitNAxis[b-1]->Draw("same hist");
    cutline.DrawLine(DsMassMin,0,DsMassMin,HFitBkgN[b-1]->GetMaximum());
    cutline.DrawLine(DsMassMax,0,DsMassMax,HFitBkgN[b-1]->GetMaximum());
    text.DrawLatex(1.835,HFitN[b-1]->GetMaximum()*.8,TString("n^{R}_{X}=")+(long)(b-1));
  }
  
  C.Print("PaperDsPlots.ps");

  
  /////////////Plot of Total mass distributions
  TH1D*HDataRS=(TH1D*)HData->ProjectionY("HDataRS");
  TH1D*HFitRS=(TH1D*)HFit->ProjectionY("HFitRS");
  TH1D*HFitBkgRS=(TH1D*)HFitBkg->ProjectionY("HFitBkgRS");
  TH1D*HDataWS_=(TH1D*)HDataWS->ProjectionY("HDataWS");
  TH1D*HFitWS_=(TH1D*)HFitWS->ProjectionY("HFitWS");
  TH1D*HFitBkgWS_=(TH1D*)HFitBkgWS->ProjectionY("HFitBkgWS");

  C.Clear();
  C.Divide(2,2);//must divide by 4 so plots scale like the above ones 
  

  C.cd(1);
  PaperSetDsMargins(C.GetPad(1));
  PaperSetDsDefaults(HFitBkgWS_,"m_{r}(DKX#gamma)","6 MeV/c^{2}");
  HFitBkgWS_->GetYaxis()->SetRangeUser(0,HFitWS_->GetMaximum()*1.1);
  HFitBkgWS_->SetLineColor(0);
  HFitBkgWS_->SetFillColor(3);
  HFitBkgWS_->Draw("hist");
  HFitWS_->SetLineColor(2);
  HFitWS_->SetLineWidth(linewidth);
  HFitWS_->Draw("same hist");
  HDataWS_->SetMarkerSize(marksize);
  HDataWS_->SetMarkerStyle(markstyle);
  HDataWS_->SetLineWidth(linewidth);
  HDataWS_->Draw("pe same hist");
  HFitWS_->Draw("same hist");
  //cutline.DrawLine(DsMassMin,0,DsMassMin,HFitBkgWS_->GetMaximum());
  //cutline.DrawLine(DsMassMax,0,DsMassMax,HFitBkgWS_->GetMaximum());


  C.cd(2);
  PaperSetDsMargins(C.GetPad(2));

  PaperSetDsDefaults(HFitBkgRS,"m_{r}(DKX#gamma)","6 MeV/c^{2}");
  HFitBkgRS->GetYaxis()->SetRangeUser(0,HFitRS->GetMaximum()*1.1);
  HFitBkgRS->SetLineColor(0);
  HFitBkgRS->SetFillColor(3);
  HFitBkgRS->Draw("hist");
  HDataRS->SetMarkerSize(marksize);
  HDataRS->SetMarkerStyle(markstyle);
  HDataRS->SetLineWidth(linewidth);
  HDataRS->Draw("pe same hist");
  HFitRS->SetLineColor(2);
  HFitRS->SetMarkerColor(2);
  HFitRS->SetLineWidth(linewidth);
  HFitRS->Draw("same hist");
  cutline.DrawLine(DsMassMin,0,DsMassMin,HFitBkgRS->GetMaximum());
  cutline.DrawLine(DsMassMax,0,DsMassMax,HFitBkgRS->GetMaximum());
  C.Print("PaperDsPlots.ps");

  C.Print("PaperDsPlots.ps]");
}



void PaperSetDsDefaultsNum(TH1*h,TString xlabel,TString ylabel){
  h->SetStats(0);
  h->SetTitle("");
 
  h->GetXaxis()->SetNdivisions(6);
  h->GetXaxis()->SetLabelSize(.08);
  h->GetXaxis()->SetTitleOffset(.9);
  h->GetXaxis()->SetTitle(xlabel);
  h->GetXaxis()->SetTitleSize(.085);
 
  h->GetYaxis()->SetNdivisions(5);
  h->GetYaxis()->SetLabelSize(.08);
  h->GetYaxis()->SetTitleOffset(.95);
  h->GetYaxis()->SetTitle(ylabel);
  h->GetYaxis()->SetTitleSize(.08);
 
}
 
void PaperSetDsMarginsNum(TVirtualPad* p){
  p->SetLeftMargin(0.15);
  p->SetRightMargin(0.01);
  p->SetTopMargin(0.06);
  p->SetBottomMargin(0.18);
}


void PaperPlotNumerator(){

  TFile fileKKpi("/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResultsKKpi/KKpiFitResult_Data.root","read");
  TH1F*HKKpiData=(TH1F*)fileKKpi.Get("HKKpiGamMass");
  TH1F*HKKpiFit=(TH1F*)fileKKpi.Get("HFit");
  TH1F*HKKpiBkg=(TH1F*)fileKKpi.Get("HFitBkg");

  TFile fileMuNu("/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResults/DsToMuNuNumeratorFitResults_Data.root","read");
  TH1F*HMuNuData=(TH1F*)fileMuNu.Get("DsToMuNu_Data_RunAll_EExtraCut_Data");
  TH1F*HMuNuFit=(TH1F*)fileMuNu.Get("DsToMuNu_Data_RunAll_EExtraCut_Total__mm20");
  TH1F*HMuNuBkg=(TH1F*)fileMuNu.Get("DsToMuNu_Data_RunAll_EExtraCut_Background");

  TFile fileENu("/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResults/DsToENuNumeratorFitResults_Data.root","read");
  TH1F*HENuData=(TH1F*)fileENu.Get("DsToENu_Data_RunAll_EExtraCut_Data");
  TH1F*HENuFit=(TH1F*)fileENu.Get("DsToENu_Data_RunAll_EExtraCut_Total__mm20");
  TH1F*HENuBkg=(TH1F*)fileENu.Get("DsToENu_Data_RunAll_EExtraCut_Background");
  TH1F*HENuSig=(TH1F*)fileENu.Get("DsToENu_Data_RunAll_EExtraCut_Signal__mm20");

  TCanvas C("C");
  
  ////////////////Plot of Ds Mass vs nXR
  C.Divide(3,3);
  TLatex text;
  text.SetTextSize(.1);
  TGaxis::SetMaxDigits(3);

  C.cd(1);
  PaperSetDsMarginsNum(C.GetPad(1));
  PaperSetDsDefaultsNum(HMuNuBkg,"m^{2}_{r}(DKX#gamma#mu)   (GeV^{2}/c^{4})","Events / 0.05 GeV^{2}/c^{4}");
  HMuNuBkg->GetYaxis()->SetRangeUser(0,HMuNuFit->GetMaximum()*1.15);
  HMuNuBkg->SetLineColor(0);
  HMuNuBkg->SetFillColor(3);
  HMuNuBkg->Draw("hist");
  HMuNuData->SetMarkerSize(marksize);
  HMuNuData->SetMarkerStyle(markstyle);
  HMuNuData->SetLineWidth(linewidth);
  HMuNuData->Draw("pe same hist");
  HMuNuFit->SetLineColor(2);
  HMuNuFit->SetMarkerColor(2);
  HMuNuFit->SetLineWidth(linewidth);
  HMuNuFit->Draw("same hist");
  text.DrawTextNDC(.24,.8,"a)");

  C.cd(2);
  PaperSetDsMarginsNum(C.GetPad(2));
  PaperSetDsDefaultsNum(HENuBkg,"m^{2}_{r}(DKX#gammae)   (GeV^{2}/c^{4})","Events / 0.05 GeV^{2}/c^{4}");
  HENuBkg->GetYaxis()->SetRangeUser(0,HENuFit->GetMaximum()*1.15);
  HENuBkg->SetLineColor(0);
  HENuBkg->SetFillColor(3);
  HENuBkg->Draw("hist");
  HENuData->SetMarkerSize(marksize);
  HENuData->SetMarkerStyle(markstyle);
  HENuData->SetLineWidth(linewidth);
  HENuData->Draw("pe same hist");
  HENuFit->SetLineColor(2);
  HENuFit->SetMarkerColor(2);
  HENuFit->SetLineWidth(linewidth);
  HENuFit->Draw("same hist");
  text.DrawTextNDC(.24,.8,"b)");
  
  C.cd(3);
  PaperSetDsMarginsNum(C.GetPad(3));
  PaperSetDsDefaultsNum(HKKpiBkg,"m(K^{+}K^{-}#pi^{+}#gamma)   (GeV/c^{2})","Events / 8 MeV/c^{2}");
  HKKpiBkg->GetYaxis()->SetRangeUser(0,HKKpiFit->GetMaximum()*1.);
  HKKpiBkg->SetLineColor(0);
  HKKpiBkg->SetFillColor(3);
  HKKpiBkg->Draw("hist");
  HKKpiData->SetMarkerSize(marksize);
  HKKpiData->SetMarkerStyle(markstyle);
  HKKpiData->SetLineWidth(linewidth);
  HKKpiData->Draw("pe same hist");
  HKKpiFit->SetLineColor(2);
  HKKpiFit->SetMarkerColor(2);
  HKKpiFit->SetLineWidth(linewidth);
  HKKpiFit->Draw("same hist");
  text.DrawTextNDC(.24,.8,"c)");

  C.Print("PaperNumPlots.ps");


}




void PaperPlotNumeratorTau(){

  TFile fileKKpi("/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResultsKKpi/KKpiFitResult_Data.root","read");
  TH1F*HKKpiData=(TH1F*)fileKKpi.Get("HKKpiGamMass");
  TH1F*HKKpiFit=(TH1F*)fileKKpi.Get("HFit");
  TH1F*HKKpiBkg=(TH1F*)fileKKpi.Get("HFitBkg");

  TFile fileMuNu("/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResults/DsToMuNuNumeratorFitResults_Data.root","read");
  TH1F*HMuNuData=(TH1F*)fileMuNu.Get("DsToMuNu_Data_RunAll_EExtraCut_Data");
  TH1F*HMuNuFit=(TH1F*)fileMuNu.Get("DsToMuNu_Data_RunAll_EExtraCut_Total__mm20");
  TH1F*HMuNuBkg=(TH1F*)fileMuNu.Get("DsToMuNu_Data_RunAll_EExtraCut_Background");

  TFile fileENu("/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResults/DsToENuNumeratorFitResults_Data.root","read");
  TH1F*HENuData=(TH1F*)fileENu.Get("DsToENu_Data_RunAll_EExtraCut_Data");
  TH1F*HENuFit=(TH1F*)fileENu.Get("DsToENu_Data_RunAll_EExtraCut_Total__mm20");
  TH1F*HENuBkg=(TH1F*)fileENu.Get("DsToENu_Data_RunAll_EExtraCut_Background");
  TH1F*HENuSig=(TH1F*)fileENu.Get("DsToENu_Data_RunAll_EExtraCut_Signal__mm20");


  TFile fileTauNuE("/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResults/DsToTauNuTauToENuNuNumeratorFitResults_Data.root","read");
  TH1F*HTauNuEData=(TH1F*)fileTauNuE.Get("DsToTauNuTauToENuNu_Data_RunAll_EExtraCut_Data");
  TH1F*HTauNuEFit=(TH1F*)fileTauNuE.Get("DsToTauNuTauToENuNu_Data_RunAll_EExtraCut_Total__eextra2");
  TH1F*HTauNuEBkg=(TH1F*)fileTauNuE.Get("DsToTauNuTauToENuNu_Data_RunAll_EExtraCut_Background");
  TH1F*HTauNuESig=(TH1F*)fileTauNuE.Get("DsToTauNuTauToENuNu_Data_RunAll_EExtraCut_Signal__eextra2");


  TFile fileTauNuMu("/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResults/DsToTauNuTauToMuNuNuNumeratorFitResults_Data.root","read");
  TH1F*HTauNuMuData=(TH1F*)fileTauNuMu.Get("DsToTauNuTauToMuNuNu_Data_RunAll_EExtraCut_Data");
  TH1F*HTauNuMuFit=(TH1F*)fileTauNuMu.Get("DsToTauNuTauToMuNuNu_Data_RunAll_EExtraCut_Total__eextra2");
  TH1F*HTauNuMuBkg=(TH1F*)fileTauNuMu.Get("DsToTauNuTauToMuNuNu_Data_RunAll_EExtraCut_Background");
  TH1F*HTauNuMuSig=(TH1F*)fileTauNuMu.Get("DsToTauNuTauToMuNuNu_Data_RunAll_EExtraCut_Signal__eextra2");


  TCanvas C("C","C",850,1100);
  
  ////////////////Plot of Ds Mass vs nXR
  C.Divide(2,3);
  TLatex text;
  text.SetTextSize(.1);
  //TGaxis::SetMaxDigits(3);

  C.cd(1);
  PaperSetDsMarginsNum(C.GetPad(1));
  PaperSetDsDefaultsNum(HMuNuBkg,"m^{2}_{r}(DKX#gamma#mu)   (GeV^{2}/c^{4})","Events / 0.05 GeV^{2}/c^{4}   ");
  HMuNuBkg->GetYaxis()->SetRangeUser(1,HMuNuFit->GetMaximum()*1.15);
  HMuNuBkg->SetLineColor(0);
  HMuNuBkg->SetFillColor(3);
  HMuNuBkg->GetXaxis()->SetRangeUser(-0.5,2.);
  HMuNuBkg->Draw("hist");
  HMuNuData->SetMarkerSize(marksizetau);
  HMuNuData->SetMarkerStyle(markstyletau);
  HMuNuData->SetLineWidth(linewidthtau);
  HMuNuData->Draw("pe same hist");
  HMuNuFit->SetLineColor(2);
  HMuNuFit->SetMarkerColor(2);
  HMuNuFit->SetLineWidth(linewidthtau);
  HMuNuFit->Draw("same hist");
  text.DrawTextNDC(.85,.8,"a)");

  C.cd(2);
  PaperSetDsMarginsNum(C.GetPad(2));
  PaperSetDsDefaultsNum(HENuBkg,"m^{2}_{r}(DKX#gammae)   (GeV^{2}/c^{4})","Events / 0.05 GeV^{2}/c^{4}   ");
  HENuBkg->GetYaxis()->SetRangeUser(1,HENuFit->GetMaximum()*1.5);
  HENuBkg->SetLineColor(0);
  HENuBkg->SetFillColor(3);
  HENuBkg->GetXaxis()->SetRangeUser(-0.5,2.);
  HENuBkg->Draw("hist");
  HENuData->SetMarkerSize(marksizetau);
  HENuData->SetMarkerStyle(markstyletau);
  HENuData->SetLineWidth(linewidthtau);
  HENuData->Draw("pe same hist");
  HENuFit->SetLineColor(2);
  HENuFit->SetMarkerColor(2);
  HENuFit->SetLineWidth(linewidthtau);
  HENuFit->Draw("same hist");
  text.DrawTextNDC(.85,.8,"b)");
  


  C.cd(3);
  PaperSetDsMarginsNum(C.GetPad(3));
  PaperSetDsDefaultsNum(HTauNuEBkg,"E_{extra}  (GeV)    ","Events / 0.05 GeV");
  HTauNuEBkg->GetYaxis()->SetRangeUser(1,HTauNuEFit->GetMaximum()*1.1);
  HTauNuEBkg->SetLineColor(0);
  HTauNuEBkg->SetFillColor(3);
  HTauNuEBkg->GetXaxis()->SetRangeUser(0.,3.);
  HTauNuEBkg->Draw("hist");
  HTauNuEData->SetMarkerSize(marksizetau);
  HTauNuEData->SetMarkerStyle(markstyletau);
  HTauNuEData->SetLineWidth(linewidthtau);
  HTauNuEData->Draw("pe same hist");
  HTauNuEFit->SetLineColor(2);
  HTauNuEFit->SetMarkerColor(2);
  HTauNuEFit->SetLineWidth(linewidthtau);
  HTauNuEFit->Draw("same hist");
  text.DrawTextNDC(.85,.8,"c)");


  C.cd(4);
  PaperSetDsMarginsNum(C.GetPad(4));
  PaperSetDsDefaultsNum(HTauNuMuBkg,"E_{extra}  (GeV)    ","Events / 0.05 GeV");
  HTauNuMuBkg->GetYaxis()->SetRangeUser(1,HTauNuMuFit->GetMaximum()*1.1);
  HTauNuMuBkg->SetLineColor(0);
  HTauNuMuBkg->SetFillColor(3);
  HTauNuMuBkg->GetXaxis()->SetRangeUser(0.,3.);
  HTauNuMuBkg->Draw("hist");
  HTauNuMuData->SetMarkerSize(marksizetau);
  HTauNuMuData->SetMarkerStyle(markstyletau);
  HTauNuMuData->SetLineWidth(linewidthtau);
  HTauNuMuData->Draw("pe same hist");
  HTauNuMuFit->SetLineColor(2);
  HTauNuMuFit->SetMarkerColor(2);
  HTauNuMuFit->SetLineWidth(linewidthtau);
  HTauNuMuFit->Draw("same hist");
  text.DrawTextNDC(.85,.8,"d)");



  C.cd(5);
  PaperSetDsMarginsNum(C.GetPad(5));
  PaperSetDsDefaultsNum(HKKpiBkg,"m(K^{+}K^{-}#pi^{+}#gamma)   (GeV/c^{2})","Events / 8 MeV/c^{2}");
  HKKpiBkg->GetYaxis()->SetRangeUser(1,HKKpiFit->GetMaximum()*1.);
  HKKpiBkg->SetLineColor(0);
  HKKpiBkg->SetFillColor(3);
  HKKpiBkg->Draw("hist");
  HKKpiFit->SetLineColor(2);
  HKKpiFit->SetMarkerColor(2);
  HKKpiFit->SetLineWidth(linewidthtau);
  HKKpiFit->Draw("same hist");
  HKKpiData->SetMarkerSize(marksizetau);
  HKKpiData->SetMarkerStyle(markstyletau);
  HKKpiData->SetLineWidth(linewidthtau);
  HKKpiData->Draw("pe same hist");
  text.DrawTextNDC(.85,.8,"e)");

  C.Print("PaperNumPlots.ps");


}
