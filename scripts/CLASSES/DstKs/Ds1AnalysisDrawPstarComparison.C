
Int_t Ds1Analysis::DrawPstarComparison(TCanvas* Canvas,TString filename,TString PstarComparisonFile,Int_t Nsignal){


  if(!OpenReducedNtuple()){
    cout<<"Failed to open the file"<<endl;
    return 0;
  }


  TFile* temp;
  temp=new TFile(PstarComparisonFile,"read");
  if(temp->IsZombie()){
    cout<<"could not open file for p* comparison"<<endl;
    return 0;   
  }
  gROOT->cd();
  cout<<"Going to compare the p* distributions with file:"<<temp->GetName()<<endl;
  

  TTree* ComarisonNtuple=(TTree*)temp->Get("Ds1pNtuple");
  if(ComarisonNtuple==NULL){
    cout<<"No ntuple."<<endl;
    return 0;
  }

  sprintf(textlabel,"%.3f<ds1pdeltam2&&ds1pdeltam2<%.3f",database.GetDs1pDeltaMSignalMin(),database.GetDs1pDeltaMSignalMax());
  TCut tightsignalcut=textlabel;

  TH1F HpstarDATA("HpstarDATA","HpstarDATA",25,2.5,5);
  SetHistoXY(&HpstarDATA,"HpstarDATA",25,2.5,5,"Ds1p p* [GeV]","Entries/.1GeV");
  ReducedNtuple->Draw("ds1ppstar>>HpstarDATA",database.GetCut(1,100)+tightsignalcut);
  
  TH1F HSignalDATA("HSignalDATA","HSignalDATA",database.GetDs1pDeltaMNbins(),database.GetDs1pDeltaMMin(),database.GetDs1pDeltaMMax());
  SetHistoXY(&HSignalDATA,"HSignalDATA",database.GetDs1pDeltaMNbins(),database.GetDs1pDeltaMMin(),database.GetDs1pDeltaMMax(),"deltaM","");
  ReducedNtuple->Draw("ds1pdeltam2>>HSignalDATA",database.GetCut(1,100)+tightsignalcut);

  TH1F HSignalMC("HSignalMC","HSignalMC",database.GetDs1pDeltaMNbins(),database.GetDs1pDeltaMMin(),database.GetDs1pDeltaMMax());
  SetHistoXY(&HSignalMC,"HSignalMC",database.GetDs1pDeltaMNbins(),database.GetDs1pDeltaMMin(),database.GetDs1pDeltaMMax(),"deltaM","");
  ComarisonNtuple->Draw("ds1pdeltam2>>HSignalMC",database.GetCut(1,100)+tightsignalcut);
  HSignalMC.SetNormFactor(HSignalDATA.Integral());

  Canvas->Clear();
  HSignalDATA.Draw("pe");
  HSignalMC.Draw("same");  
  Canvas->Print(filename);
  

  //-------------------------------------------
  //need to subtract the  background out
  //-------------------------------------------
  Int_t NbkgCands= (int)HSignalDATA.Integral() - Nsignal; 
  if(TruthMatch) NbkgCands=0;
  cout<<"N bkg cands="<<NbkgCands<<endl;

  
  //get background momentum distribution
  sprintf(textlabel,"%.3f>ds1pdeltam2||ds1pdeltam2>%.3f",database.GetDs1pDeltaMSignalMin(),database.GetDs1pDeltaMSignalMax());
  TCut backgroundcut=textlabel;

  TH1F HpstarBkg("HpstarBkg","HpstarBkg",25,2.5,5);
  SetHistoXY(&HpstarBkg,"HpstarBkg",25,2.5,5,"Ds1p p* [GeV]","Entries/.1GeV");
  ReducedNtuple->Draw("ds1ppstar>>HpstarBkg",database.GetCut(1,100)+backgroundcut);
  HpstarBkg.SetNormFactor(NbkgCands);

  //now make the subtraction
  HpstarDATA.Add(&HpstarBkg,-1);
  

  TH1F HpstarMC("HpstarMC","HpstarMC",25,2.5,5);
  SetHistoXY(&HpstarMC,"HpstarMC",25,2.5,5,"Ds1p p* [GeV]","Entries/.1GeV");
  ComarisonNtuple->Draw("ds1ppstar>>HpstarMC",database.GetCut(1,100)+tightsignalcut);

  //Scale the MC
  //HpstarMC.Scale(HpstarDATA.Integral()/HpstarMC.Integral());
  HpstarMC.SetNormFactor(HpstarDATA.Integral());
  
  Canvas->Clear();
  HpstarDATA.Draw("pe");
  HpstarMC.Draw("same");  
  Canvas->Print(filename);

  temp->Close();
  delete temp;

  cout<<"Done with p* comparison"<<endl;


  Close();

  return 1;
}


