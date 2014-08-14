
void CompareMCToData(){
  TFile FTotMC("DsTolnu/MC/Final/HistosForFit.root","read");
  TFile FData("DsTolnu/Data/Final/HistosForFit.root","read");
  gROOT->cd();
  TH1F* HDsMassMC=(TH1F*)FTotMC.Get("HDsMass");
  TH1F* HDsMassData=(TH1F*)FData.Get("HDsMass");
  
  TCanvas C("C");
  C.Print("CompareMCToData.ps[");
  
  C.Clear();//lumi scaled
  HDsMassData->Draw("pe");
  HDsMassMC->Draw("hist same");
  C.Print("CompareMCToData.ps");

  C.Clear();//scaled to low edge
  HDsMassData->GetYaxis()->SetRangeUser(0,HDsMassData->GetMaximum()*1.2);
  HDsMassData->GetXaxis()->SetTitle("m_{recoil}(DKX#gamma) (GeV/c^{2})");
  HDsMassData->Draw("pe");
  HDsMassMC->Scale(HDsMassData->Integral(1,5)/HDsMassMC->Integral(1,5));
  HDsMassMC->Draw("hist same");
  C.Print("CompareMCToData.ps");
  

  ///only Bkg
  TH1F* HDsMassNoSigMC=(TH1F*)FTotMC.Get("HDsMassNoSig");
  C.Clear();//lumi scaled
  HDsMassData->Draw("pe");
  HDsMassNoSigMC->Draw("hist same");
  C.Print("CompareMCToData.ps");

  C.Clear();//scaled to low edge
  HDsMassData->Draw("pe");
  HDsMassNoSigMC->Scale(HDsMassData->Integral(1,5)/HDsMassNoSigMC->Integral(1,5));
  HDsMassNoSigMC->Draw("hist same");
  C.Print("CompareMCToData.ps");
  


  C.Print("CompareMCToData.ps]");
}
