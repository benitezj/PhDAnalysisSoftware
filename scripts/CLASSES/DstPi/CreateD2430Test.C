

Int_t CreateD2430Test(){
  TFile fin("DstarCharged/DstarPi/Data/KpiAndK3piWS/HistosForFit.root","read");
  gROOT->cd();
  TH1F* HWS=(TH1F*)fin.Get("HDstPi");
  if(!HWS)return 0;
  TH1F* HWSRebin=(TH1F*)HWS->Clone("HWSRebin");
  fin.Close();

  HWSRebin->Rebin(5);
  TH1F* HWSD2430=(TH1F*)HWSRebin->Clone("HWSD2430");

  TF1 fbw("fbw","1/((x-2.427)**2+(0.5*0.380)**2)",HWSD2430->GetXaxis()->GetXmin(),HWSD2430->GetXaxis()->GetXmax());
  TH1F HD2430("HD2430","",HWSD2430->GetXaxis()->GetNbins(),HWSD2430->GetXaxis()->GetXmin(),HWSD2430->GetXaxis()->GetXmax());
  HD2430.FillRandom("fbw",100000);//fix yield to D2420 Data yield
  HWSD2430->Add(&HD2430);

  TFile fout("DstarCharged/DstarPi/Data/KpiAndK3piWSD2430/HistosForFit.root","recreate");  
  HWSD2430->SetName("HDstPi");
  HWSD2430->Write();  
  HWSRebin->SetName("HDstPiBkg");
  HWSRebin->Write();
  fout.Close();


  return 1;
}
