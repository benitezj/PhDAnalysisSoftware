Float_t lumiScale[5]={(2.04+1.93*.1)/(1+.1),//for cc do a weighted average because On-Off have been combined.
		      1.04,
		      1.05,
		      .9,
		      1.63};


void CombineMC(){

  TFile* FGenericMC[5];
  FGenericMC[0]=new TFile("DsTolnu/SP1005Skim/DsReco/Final/HistosForFit.root");
  FGenericMC[1]=new TFile("DsTolnu/SP1235/HistosForFit.root");
  FGenericMC[2]=new TFile("DsTolnu/SP1237/HistosForFit.root");
  FGenericMC[3]=new TFile("DsTolnu/SP998/HistosForFit.root");
  FGenericMC[4]=new TFile("DsTolnu/SP3429/HistosForFit.root");
  gROOT->cd();
  
  TString MCTitle[5]={"cc","B^{0}B^{0}","B^{+}B^{-}","uds","tau"};

  TCanvas C("C");
  C.Print("CombineMC.ps[");
  
  ///-------------------Total MC 
  TH1F* HDsMass[5];
  C.Clear();//Unweighted
  C.Divide(2,2);
  for(Int_t i=0;i<4;i++){
    C.cd(i+1);
    HDsMass[i]=(TH1F*)FGenericMC[i]->Get("HDsMass");
    HDsMass[i]->GetXaxis()->SetTitle("m_{recoil}(DKX#gamma) (GeV/c^{2})");
    HDsMass[i]->SetTitle(MCTitle[i]);
    HDsMass[i]->Draw();
  }
  C.Print("CombineMC.ps");

  TH1F* HTotal=(TH1F*)HDsMass[0]->Clone("HDsMass");
  HTotal->Reset();
  C.Clear();//lumi weighted
  C.Divide(2,2);
  for(Int_t i=0;i<4;i++){
    C.cd(i+1);
    delete HDsMass[i]->FindObject("stats");
    HDsMass[i]->Scale(1./lumiScale[i]);
    HDsMass[i]->Draw();
    HTotal->Add(HDsMass[i]);
  }
  C.Print("CombineMC.ps");

  C.Clear();
  delete HTotal->FindObject("stats");
  HTotal->Draw();
  C.Print("CombineMC.ps");


  ///-------------------Bkg MC 
  TH1F* HDsMassBkg[5];
  C.Clear();//Unweighted
  C.Divide(2,2);
  for(Int_t i=0;i<4;i++){
    C.cd(i+1);
    HDsMassBkg[i]=(TH1F*)FGenericMC[i]->Get("HDsMassNoSig");
    HDsMassBkg[i]->Draw();
  }
  C.Print("CombineMC.ps");

  TH1F* HTotalBkg=(TH1F*)HDsMassBkg[0]->Clone("HDsMassNoSig");
  HTotalBkg->Reset();
  C.Clear();//lumi weighted
  C.Divide(2,2);
  for(Int_t i=0;i<4;i++){
    C.cd(i+1);
    delete HDsMassBkg[i]->FindObject("stats");
    HDsMassBkg[i]->Scale(1./lumiScale[i]);
    HDsMassBkg[i]->Draw();
    HTotalBkg->Add(HDsMassBkg[i]);
  }
  C.Print("CombineMC.ps");

  C.Clear();
  delete HTotalBkg->FindObject("stats");
  HTotalBkg->Draw();
  C.Print("CombineMC.ps");


  /////////////-----------Bkg Multiplicity
  TH1F* HEvtMultiplicity[5];
  C.Clear();//Unweighted
  C.Divide(2,2);
  for(Int_t i=0;i<4;i++){
    C.cd(i+1);
    HEvtMultiplicity[i]=(TH1F*)FGenericMC[i]->Get("HEvtMultiplicityBkg");
    HEvtMultiplicity[i]->GetXaxis()->SetTitle("m_{recoil}(DKX#gamma) (GeV/c^{2})");
    HEvtMultiplicity[i]->SetTitle(MCTitle[i]);
    HEvtMultiplicity[i]->Draw();
  }
  C.Print("CombineMC.ps");

  TH1F* HTotalMultiplicity=(TH1F*)HEvtMultiplicity[0]->Clone("HEvtMultiplicityBkg");
  HTotalMultiplicity->Reset();
  C.Clear();//lumi weighted
  C.Divide(2,2);
  for(Int_t i=0;i<4;i++){
    C.cd(i+1);
    delete HEvtMultiplicity[i]->FindObject("stats");
    HEvtMultiplicity[i]->Scale(1./lumiScale[i]);
    HEvtMultiplicity[i]->Draw();
    HTotalMultiplicity->Add(HEvtMultiplicity[i]);
  }
  C.Print("CombineMC.ps");

  C.Clear();
  delete HTotalMultiplicity->FindObject("stats");
  HTotalMultiplicity->Draw();
  C.Print("CombineMC.ps");




  TFile FTotMC("DsTolnu/MC/Final/HistosForFit.root","recreate");
  HTotal->Write();
  HTotalBkg->Write();
  HTotalMultiplicity->Write();
  FTotMC.ls();
  FTotMC.Close();

  
  C.Print("CombineMC.ps]");
}


//////////////---------------------------------------------WS Histos---------------------------------


void CombineMCWS(){

  TFile* FGenericMC[5];
  FGenericMC[0]=new TFile("DsTolnu/SP1005Skim/DsReco/FinalWS/HistosForFit.root");
  FGenericMC[1]=new TFile("DsTolnu/SP1235/FinalWS/HistosForFit.root");
  FGenericMC[2]=new TFile("DsTolnu/SP1237/FinalWS/HistosForFit.root");
  FGenericMC[3]=new TFile("DsTolnu/SP998/FinalWS/HistosForFit.root");
  FGenericMC[4]=new TFile("DsTolnu/SP3429/FinalWS/HistosForFit.root");
  gROOT->cd();
  
  TString MCTitle[5]={"cc","B^{0}B^{0}","B^{+}B^{-}","uds","tau"};

  TCanvas C("C");
  C.Print("CombineMCWS.ps[");
  
  ///-------------------Total MC 
  TH1F* HEvtMultiplicity[5];
  C.Clear();//Unweighted
  C.Divide(2,2);
  for(Int_t i=0;i<4;i++){
    C.cd(i+1);
    HEvtMultiplicity[i]=(TH1F*)FGenericMC[i]->Get("HEvtMultiplicityBkg");
    HEvtMultiplicity[i]->GetXaxis()->SetTitle("m_{recoil}(DKX#gamma) (GeV/c^{2})");
    HEvtMultiplicity[i]->SetTitle(MCTitle[i]);
    HEvtMultiplicity[i]->Draw();
  }
  C.Print("CombineMCWS.ps");

  TH1F* HTotalMultiplicity=(TH1F*)HEvtMultiplicity[0]->Clone("HEvtMultiplicityBkg");
  HTotalMultiplicity->Reset();
  C.Clear();//lumi weighted
  C.Divide(2,2);
  for(Int_t i=0;i<4;i++){
    C.cd(i+1);
    delete HEvtMultiplicity[i]->FindObject("stats");
    HEvtMultiplicity[i]->Scale(1./lumiScale[i]);
    HEvtMultiplicity[i]->Draw();
    HTotalMultiplicity->Add(HEvtMultiplicity[i]);
  }
  C.Print("CombineMCWS.ps");

  C.Clear();
  delete HTotalMultiplicity->FindObject("stats");
  HTotalMultiplicity->Draw();
  C.Print("CombineMCWS.ps");


 

  TFile FTotMC("DsTolnu/MC/FinalWS/HistosForFit.root","recreate");
  HTotalMultiplicity->Write();
  FTotMC.ls();
  FTotMC.Close();

  
  C.Print("CombineMCWS.ps]");
}
