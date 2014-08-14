void FakePhotonSystematic(){
  
  TFile FMCRSSig("DsTolnu/SP1005Skim/DsReco/Final/HistosForFit.root");
  TFile FMCWSSig("DsTolnu/SP1005Skim/DsReco/FinalWS/HistosForFit.root");

  //TFile FMCRS("DsTolnu/MC/Final/HistosForFit.root");
  //TFile FMCWS("DsTolnu/MC/FinalWS/HistosForFit.root");
  TFile FMCRS("DsTolnu/SP1005Skim/DsReco/Final/HistosForFit.root");
  TFile FMCWS("DsTolnu/SP1005Skim/DsReco/FinalWS/HistosForFit.root");

  TFile FDataRS("DsTolnu/Data/Final/HistosForFit.root");
  TFile FDataWS("DsTolnu/Data/FinalWS/HistosForFit.root");

  

  TCanvas C;
  C.Clear();
  C.Print("FakePhotonSystematic.ps[");

  C.Clear();
  C.Divide(2,2);
  C.cd(1);  
//   TH1F* HMCRS=(TH1F*)FMCRS.Get("HEvtMultiplicity");
//   HMCRS->SetTitle(TString("MC RS =")+(long)HMCRS->GetMean()+"."+(long)(1000*(HMCRS->GetMean()-1.)));
//   HMCRS->Draw();
//   C.cd(2);
//   TH1F* HMCWS=(TH1F*)FMCWS.Get("HEvtMultiplicity");
//   HMCWS->SetTitle(TString("MC WS =")+(long)HMCWS->GetMean()+"."+(long)(1000*(HMCWS->GetMean()-1.)));
//   HMCWS->Draw();
  C.cd(3);  
  TH1F* HDataRS=(TH1F*)FDataRS.Get("HEvtMultiplicity");
  HDataRS->SetTitle(TString("Data RS =")+(long)HDataRS->GetMean()+"."+(long)(1000*(HDataRS->GetMean()-1.)));
  HDataRS->Draw();
  C.cd(4);
  TH1F* HDataWS=(TH1F*)FDataWS.Get("HEvtMultiplicity");
  HDataWS->SetTitle(TString("Data WS =")+(long)HDataWS->GetMean()+"."+(long)(1000*(HDataWS->GetMean()-1.)));
  HDataWS->Draw();
  C.Print("FakePhotonSystematic.ps");



  C.Clear();
  C.Divide(2,2);
  C.cd(1);  
  TH1F* HSigMCRS=(TH1F*)FMCRSSig.Get("HEvtMultiplicitySig");
  HSigMCRS->SetTitle(TString("MC Sig RS =")+(long)HSigMCRS->GetMean()+"."+(long)(1000*(HSigMCRS->GetMean()-1.)));
  HSigMCRS->Draw();
  C.cd(2);
  TH1F* HSigMCWS=(TH1F*)FMCWSSig.Get("HEvtMultiplicitySig");
  HSigMCWS->SetTitle(TString("MC Sig WS =")+(long)HSigMCWS->GetMean()+"."+(long)(1000*(HSigMCWS->GetMean()-1.)));
  HSigMCWS->Draw();
  C.cd(3);  
  TH1F* HBkgMCRS=(TH1F*)FMCRS.Get("HEvtMultiplicityBkg");
  HBkgMCRS->SetTitle(TString("MC Bkg RS =")+(long)HBkgMCRS->GetMean()+"."+(long)(1000*(HBkgMCRS->GetMean()-1.)));
  HBkgMCRS->Draw();
  C.cd(4);
  TH1F* HBkgMCWS=(TH1F*)FMCWS.Get("HEvtMultiplicityBkg");
  HBkgMCWS->SetTitle(TString("MC Bkg WS =")+(long)HBkgMCWS->GetMean()+"."+(long)(1000*(HBkgMCWS->GetMean()-1.)));
  HBkgMCWS->Draw();
  C.Print("FakePhotonSystematic.ps");
  
  C.Clear();
  THStack HMCStack("HMCStack","");
  HBkgMCRS->SetFillColor(1);HMCStack.Add(HBkgMCRS);
  HSigMCRS->SetFillColor(2);HSigMCRS->SetLineColor(2);HMCStack.Add(HSigMCRS);
  HMCStack.Draw();
  C.Print("FakePhotonSystematic.ps");

  //---------------------------------------------
  //subtract background from Data
  //---------------------------------------------
  TH1F*HDataRSBkg=(TH1F*)HDataWS->Clone("HDataRSBkg");
  TH1F*HBkgMCWSCl=(TH1F*)HBkgMCWS->Clone("HBkgMCWSCl");
  //need to fix the WS first by subtracting the signal component
  HSigMCWS->Scale(11350./HSigMCWS->Integral());//scale to number of WS signal events in Data
  HDataRSBkg->Add(HSigMCWS,-1);
  //construct correction ratio from MC to correct the WS distribution
  TH1F*HBkgMCRSCl=(TH1F*)HBkgMCRS->Clone("HBkgMCRSCl");
  HBkgMCWSCl->Scale(1./HBkgMCWSCl->Integral());
  HBkgMCRSCl->Scale(1./HBkgMCRSCl->Integral());
  TH1F*HBkgMCRatio=(TH1F*)HBkgMCRSCl->Clone("HBkgMCRatio");
  HBkgMCRatio->Divide(HBkgMCWSCl);
  C.Clear();
  delete HBkgMCRatio->FindObject("stats");
  HBkgMCRatio->SetFillColor(0);HBkgMCRatio->SetLineColor(1);
  HBkgMCRatio->SetStats(0);
  HBkgMCRatio->GetYaxis()->SetRangeUser(0,1.5);
  HBkgMCRatio->Draw("hist");
  C.Print("FakePhotonSystematic.ps");
  //correct the WS distribution
  HDataRSBkg->Multiply(HBkgMCRatio);
  C.Clear();
  HDataWS->Scale(1./HDataWS->Integral());
  HDataRSBkg->Scale(1./HDataRSBkg->Integral());
  delete HDataWS->FindObject("stats");
  HDataWS->SetTitle("");
  HDataWS->GetXaxis()->SetTitle("Reconstruction Multiplicity");
  HDataWS->Draw("p");
  HDataRSBkg->SetLineColor(0);
  HDataRSBkg->SetFillColor(4);
  HDataRSBkg->Draw("hist same");
  HDataWS->Draw("psame");
  C.Print("FakePhotonSystematic.ps");
  //scale Data WS up to Data RS bkg
  HDataRSBkg->Scale((6.9952e+05)/HDataRSBkg->Integral());//scale to number of Bkg events in data //+(106000*.06)modify bkg by initial guess on error of Signal .06 comes from a 10% increase in the fake photon yield
  C.Clear();
  HDataRS->GetYaxis()->SetRangeUser(0,HDataRS->GetMaximum()*1.3);
  HDataRS->GetXaxis()->SetTitle("Reconstruction Multiplicity");
  HDataRS->SetTitle("");
  HDataRS->Draw("pe");
  HDataRSBkg->SetFillColor(4);HDataRSBkg->SetLineColor(0);
  delete HDataRSBkg->FindObject("stats");
  HDataRSBkg->Draw("same");
  C.Print("FakePhotonSystematic.ps");

  //Perform subtraction
  TH1F*HDataRSCl=new TH1F("HDataRSCl","",HDataRS->GetXaxis()->GetNbins(),
			  HDataRS->GetXaxis()->GetXmin(),HDataRS->GetXaxis()->GetXmax());
  HDataRSCl->Add(HDataRS);
  Float_t Ntot=0,Nw=0;
  for(Int_t b=1;b<=HDataRSCl->GetXaxis()->GetNbins();b++){
    Nw+=HDataRSCl->GetBinContent(b)*HDataRSCl->GetBinCenter(b);
    Ntot+=HDataRSCl->GetBinContent(b);
  } 
  cout<<"before "<<HDataRSCl->GetMean()<<" "<<Nw/Ntot<<endl;
  delete HDataRSCl->FindObject("stats");
  HDataRSCl->Add(HDataRSBkg,-1);
  Ntot=0,Nw=0;
  for(Int_t b=1;b<=HDataRSCl->GetXaxis()->GetNbins();b++){
    Nw+=HDataRSCl->GetBinContent(b)*HDataRSCl->GetBinCenter(b);
    Ntot+=HDataRSCl->GetBinContent(b);
  } 
  cout<<"after "<<HDataRSCl->GetMean()<<" "<<Nw/Ntot<<endl;
  //compare Data signal to MC signal
  TH1F*HSigMCRSCl=(TH1F*)HSigMCRS->Clone("HSigMCRSCl");
  HSigMCRSCl->Scale(HDataRSCl->Integral()/HSigMCRSCl->Integral());
  HSigMCRSCl->SetFillColor(0);HSigMCRSCl->SetLineColor(1);
  C.Clear();  
  HDataRSCl->GetYaxis()->SetRangeUser(0,HDataRSCl->GetMaximum()*1.3);
  //HDataRSCl->SetTitle(TString("Data Sig =")+(long)(Nw/Ntot)+"."+(long)(1000*(Nw/Ntot-1.)));
  HDataRSCl->SetTitle("");
  HDataRSCl->GetXaxis()->SetTitle("Reconstruction Multiplicity");
  delete HDataRSCl->FindObject("stats");
  HDataRSCl->SetStats(0);
  HDataRSCl->Draw("pe");
  HSigMCRSCl->SetStats(0);
  HSigMCRSCl->Draw("same");
  C.Print("FakePhotonSystematic.ps");

  TH1F*HDataSigMCSigDiff=new TH1F("HDataSigMCSigDiff","",HDataRS->GetXaxis()->GetNbins(),
			  HDataRS->GetXaxis()->GetXmin(),HDataRS->GetXaxis()->GetXmax());
  HDataSigMCSigDiff->Add(HDataRSCl);
  HDataSigMCSigDiff->Add(HSigMCRSCl,-1);
  C.Clear();
  delete HDataSigMCSigDiff->FindObject("stats");
  HDataSigMCSigDiff->SetStats(0);
  HDataSigMCSigDiff->Draw("");
  C.Print("FakePhotonSystematic.ps");


  //Signal multiplicity for modified photon backgrounds
  C.Clear();
  HSigMCRSCl->SetFillColor(0);HSigMCRSCl->SetLineColor(1);
  HSigMCRSCl->GetYaxis()->SetRangeUser(0,HSigMCRSCl->GetMaximum()*1.1);
  HSigMCRSCl->GetXaxis()->SetRangeUser(.5,3.5);
  HSigMCRSCl->SetStats(0);
  HSigMCRSCl->Draw("l"); 
  TGraph GMult;
  Int_t npts=0;
  TH1F* HDiff[21];
  TH1F* HSigMCRSMod[21];
  for(Int_t m=0;m<11;m++){
    HSigMCRSMod[m]=(TH1F*)FMCRSSig.Get(TString("HEvtMultiplicitySigMod")+(long)m);    
    if(HSigMCRSMod[m]->Integral()>0){
      GMult.SetPoint(npts,1+(m-10)*.01,HSigMCRSMod[m]->GetMean()); 
      npts++;
      HSigMCRSMod[m]->Scale(HDataRSCl->Integral()/HSigMCRSMod[m]->Integral());
    }
    HSigMCRSMod[m]->SetLineColor(4);
    HSigMCRSMod[m]->SetStats(0);
    HSigMCRSMod[m]->Draw("lsame");
    HDiff[m]=(TH1F*)HSigMCRSMod[m]->Clone(TString("HEvtMultiplicitySigModCl")+(long)m);
    HDiff[m]->Add(HSigMCRSCl,-1);
  }
  HSigMCRSCl->Draw("lsame");
  HDataRSCl->SetLineColor(2);
  HDataRSCl->SetStats(0);
  HDataRSCl->Draw("lsame");
  C.Print("FakePhotonSystematic.ps");

  //fix the multiplicity on the plus side 
  GMult.SetPoint(npts,1.1,HSigMCRSMod[10]->GetMean()+(HSigMCRSMod[10]->GetMean()-HSigMCRSMod[0]->GetMean()));
    

  TH1F* HDataDiff=(TH1F*)HDataRSCl->Clone("HDataDiff");
  HDataDiff->Add(HSigMCRSCl,-1);
  TH1F* HMCDiff=(TH1F*)HSigMCRSCl->Clone("HMCDiff");
  HMCDiff->Add(HSigMCRSCl,-1);
  C.Clear();
  HMCDiff->GetYaxis()->SetRangeUser(-6000,3000);
  HMCDiff->Draw("l");
  for(Int_t m=0;m<11;m++) HDiff[m]->Draw("lsame");
  HMCDiff->Draw("lsame");
  HDataDiff->Draw("lsame");
  C.Print("FakePhotonSystematic.ps");
  
  GMult.GetXaxis()->SetTitle("Amount of Fake Photon Background");
  GMult.GetYaxis()->SetTitle("Avg. Reconstruction Multiplicity");

  C.Clear();  //just the map
  GMult.Draw("apl");
  C.Print("FakePhotonSystematic.ps");

  //no Data error
  C.Clear();
  GMult.Draw("apl");
  TLine line;
  line.SetLineColor(2);  line.SetLineWidth(2);
  line.DrawLine(.9,Nw/Ntot,1.1,Nw/Ntot);
  C.Print("FakePhotonSystematic.ps");

  //with Data error
  C.Clear();
  GMult.Draw("apl");
  TBox box;
  box.SetLineColor(0);
  box.SetFillColor(3);
  box.SetFillStyle(1001);
  box.DrawBox(.9,Nw/Ntot-(1.769-1.763),1.1,Nw/Ntot+(1.769-1.763));
  line.DrawLine(.9,Nw/Ntot,1.1,Nw/Ntot);
  GMult.Draw("plsame");
  C.Print("FakePhotonSystematic.ps");

  C.Print("FakePhotonSystematic.ps]");

}
