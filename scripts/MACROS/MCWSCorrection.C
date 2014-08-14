



void MCWSCorrection(){

  TFile FMCRSBkg("DstarCharged/DstarPi/MC/SP-1005/KpiAndK3piBkg/HistosForFit.root","read");
  TFile FMCWS("DstarCharged/DstarPi/MC/SP-1005/KpiAndK3piWS/HistosForFit.root","read");
  TFile FDataWS("DstarCharged/DstarPi/Data/KpiAndK3piWS/HistosForFit.root","read");
  TFile FDataRS("DstarCharged/DstarPi/Data/KpiAndK3pi/HistosForFit.root","read");
  

  TH1F* HMCRSBkg=(TH1F*)FMCRSBkg.Get("HDstPi");
  TH1F* HMCWS=(TH1F*)FMCWS.Get("HDstPi");
  TH1F* HDataWS=(TH1F*)FDataWS.Get("HDstPi");
  TH1F* HDataRS=(TH1F*)FDataRS.Get("HDstPi");
  HMCRSBkg=(TH1F*)RebinHistoX(HMCRSBkg,260,2.1,3.4);
  HMCWS=(TH1F*)RebinHistoX(HMCWS,260,2.1,3.4);
  HDataWS=(TH1F*)RebinHistoX(HDataWS,260,2.1,3.4);
  HDataRS=(TH1F*)RebinHistoX(HDataRS,260,2.1,3.4);


  TH1F* HMCRSBkgC=(TH1F*)HMCRSBkg->Clone("HMCRSBkgC");
  TH1F* HMCWSC=(TH1F*)HMCWS->Clone("HMCWSC");
  TH1F* HDataRSC=(TH1F*)HDataRS->Clone("HDataRSC");
  TH1F* HDataRSCC=(TH1F*)HDataRS->Clone("HDataRSCC");


  TCanvas C("C");

  C.Print("MCWSCorrection.ps[");

  C.Clear();
  HMCRSBkgC->Scale(HDataRS->Integral(HDataRS->GetXaxis()->GetNbins()-10,HDataRS->GetXaxis()->GetNbins())/HMCRSBkgC->Integral(HDataRS->GetXaxis()->GetNbins()-10,HDataRS->GetXaxis()->GetNbins()));
  HDataRS->Draw("hist pe");
  HMCRSBkgC->Draw("hist same");
  C.Print("MCWSCorrection.ps");
   
  HDataRSC->Add(HMCRSBkgC,-1);
  C.Clear();
  HDataRSC->Draw("hist pe");
  C.Print("MCWSCorrection.ps");

  C.Clear();
  HMCWSC->Scale(HDataWS->Integral(HDataRS->GetXaxis()->GetNbins()-10,HDataRS->GetXaxis()->GetNbins())/HMCWSC->Integral(HDataRS->GetXaxis()->GetNbins()-10,HDataRS->GetXaxis()->GetNbins()));  
  HDataWS->Draw("hist pe");
  HMCWSC->Draw("hist same");
  C.Print("MCWSCorrection.ps");
  
  TH1F* HMCWSCC=(TH1F*)HMCWSC->Clone("HMCWSCC");
  HMCWSCC->Divide(HDataWS);
  C.Clear();
  HMCWSCC->Draw("hist ");
  C.Print("MCWSCorrection.ps");

  TH1F* HMCRSBkgCC=(TH1F*)HMCRSBkgC->Clone("HMCRSBkgC");
  HMCRSBkgCC->Divide(HMCWSCC);
  C.Clear();
  HDataRS->Draw("hist pe");
  HMCRSBkgCC->Draw("hist same");
  C.Print("MCWSCorrection.ps");

  HDataRSCC->Add(HMCRSBkgCC,-1);
  C.Clear();
  HDataRSCC->Draw("hist pe");
  C.Print("MCWSCorrection.ps");

  C.Print("MCWSCorrection.ps]");  
}
