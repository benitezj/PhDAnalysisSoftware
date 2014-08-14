
Int_t Dst2640Analysis::DrawPstarComparison(TCanvas* Canvas,TString filename,TString PstarComparisonFile){

  TFile* temp;
  temp=new TFile(PstarComparisonFile,"read");
  if(temp->IsZombie()){
    cout<<"could not open file for p* comparison"<<endl;
    return 0;   
  }
  gROOT->cd();
  cout<<"Going to compare the p* distributions with file:"<<temp->GetName()<<endl;
  

  TTree* tree=(TTree*)temp->Get("Dst2640Ntuple");
  if(tree==NULL){
    cout<<"No ntuple."<<endl;
    return 0;
  }

  TH1F HpstarMC("HpstarMC","HpstarMC",10,2.4,5);
  SetHistoXY(&HpstarMC,"HpstarMC",10,2.5,5,
	     "Dst2640 p* [GeV]","Entries/.25GeV");

  TH1F HpstarDATA("HpstarDATA","HpstarDATA",10,2.4,5);
  SetHistoXY(&HpstarDATA,"HpstarDATA",10,2.5,5,
	     "Dst2640 p* [GeV]","Entries/.25GeV");


  char str[1000];
  sprintf(str,"%.3f<dst2640deltam&&dst2640deltam<%.3f",database.GetDst2640DeltaMSignalMin(),database.GetDst2640DeltaMSignalMax());
  TCut cut=TCut(str);
  ReducedNtuple->Draw("dst2640pstar>>HpstarDATA",database.GetCut(1,100)+cut);
  tree->Draw("dst2640pstar>>HpstarMC",database.GetCut(1,100)+cut);

  HpstarMC.Scale(HpstarDATA.Integral()/HpstarMC.Integral());

  Canvas->Clear();
  if(HpstarDATA.GetMaximum()>HpstarMC.GetMaximum()){
    HpstarDATA.Draw("pe");
    HpstarMC.Draw("same");
  }else{
    HpstarMC.Draw();
    HpstarDATA.Draw("pesame");
  }
  Canvas->Print(filename);

  temp->Close();
  if(temp!=NULL)delete temp;

  cout<<"Done with p* comparison"<<endl;

  return 1;
}


