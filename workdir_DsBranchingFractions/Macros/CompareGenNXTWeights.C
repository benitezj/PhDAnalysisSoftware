#define NXT 7


void CompareGenNXTWeights(){
  TH1F HDs("HDs","",NXT,-.5,6.5);
  TH1F HNu("HNu","",NXT,-.5,6.5);
  
 
  //declare text files with produced Ds and Muon yields as a function of nXT
  ifstream DsProducedFile; 
  DsProducedFile.open("/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResults/DsProduced.txt");
  ifstream MuNuProducedFile; 
  MuNuProducedFile.open("/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResults/MuNuProduced.txt");
  
  Float_t TotDsProd=0;Float_t TotDsRec=0;

  cout<<" nXT efficiencies: "<<endl;
  for(Int_t j=0;j<NXT;j++){
    //Denominators
    Int_t indexcheck;
    Float_t NProdDs=0.;
    DsProducedFile>>indexcheck>>NProdDs; 
    if(indexcheck!=j){cout<<" NXT index does not match for Ds produced   j="<<j<<endl;exit(0);}
    Float_t NProdMuNu=0.;
    MuNuProducedFile>>indexcheck>>NProdMuNu;
    if(indexcheck!=j){cout<<" NXT index does not match for MuNu produced   j="<<j<<endl;exit(0);}

    HDs.SetBinContent(j+1,NProdDs);  
    HNu.SetBinContent(j+1,NProdMuNu);  
  }

  
  TCanvas C("C");
  C.Print("CompareGenNXTWeights.ps[");

  HDs.GetYaxis()->SetTitle("N_{Ds}");
  HNu.GetYaxis()->SetTitle("N_{Ds}");
  HDs.GetXaxis()->SetTitle("n_{X}^{T}");
  HNu.GetXaxis()->SetTitle("n_{X}^{T}");
  HNu.SetLineColor(4);
  C.Clear();
  HDs.GetYaxis()->SetRangeUser(0.,1.2*HDs.GetMaximum());
  HDs.Draw("hist");
  C.Print("CompareGenNXTWeights.ps");
  C.Clear();
  HNu.GetYaxis()->SetRangeUser(0.,1.2*HNu.GetMaximum());
  HNu.Draw("hist");
  C.Print("CompareGenNXTWeights.ps");


  HDs.Scale(1./HDs.Integral());
  HNu.Scale(1./HNu.Integral());
  HDs.GetYaxis()->SetTitle("weight");
  HNu.GetYaxis()->SetTitle("weight");
  C.Clear();
  HDs.GetYaxis()->SetRangeUser(0.,1.2*HDs.GetMaximum());
  HDs.Draw("hist");
  HNu.Draw("hist same");
  C.Print("CompareGenNXTWeights.ps");

  HNu.Divide(&HDs);
  HNu.GetYaxis()->SetTitle("ratio");
  C.Clear();
  delete HNu.FindObject("stats");
  HNu.SetStats(0);
  HNu.GetYaxis()->SetRangeUser(0.,1.2);
  HNu.Draw("hist");
  C.Print("CompareGenNXTWeights.ps");
  
  C.Print("CompareGenNXTWeights.ps]");


  //print out the correction values
  for(Int_t j=0;j<NXT;j++)
    printf(",%.3f",HNu.GetBinContent(j+1));
  printf("\n hello");
}


void CompareGenNXTWeightsKKpi(){
  TH1F HDs("HDs","",NXT,-.5,6.5);
  TH1F* HNu[3];
  for(Int_t d=0;d<3;d++)
    HNu[d]=new TH1F(TString("HNu")+(long)d,"",NXT,-.5,6.5);
 
  //declare text files with produced Ds and Muon yields as a function of nXT
  ifstream DsProducedFile; 
  DsProducedFile.open("/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResults/DsProduced.txt");
  ifstream MuNuProducedFile; 
  MuNuProducedFile.open("/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResultsKKpi/KKpiProduced.txt");
  
  Float_t TotDsProd=0;Float_t TotDsRec=0;

  cout<<"Ds efficiencies: "<<endl;
  for(Int_t j=0;j<NXT;j++){
    //Denominators
    Int_t indexcheck;
    Float_t NProdDs=0.;
    DsProducedFile>>indexcheck>>NProdDs; 
    if(indexcheck!=j){cout<<" NXT index does not match for Ds produced   j="<<j<<endl;exit(0);}
    HDs.SetBinContent(j+1,NProdDs);  
  }

  cout<<"KKpi efficiencies: "<<endl;

  for(Int_t j=0;j<NXT;j++){
    for(Int_t d=0;d<3;d++){
      //Denominators
      Int_t indexcheck;
      Int_t dalitzcheck;
      Float_t NProdMuNu=0.;
      MuNuProducedFile>>indexcheck>>dalitzcheck>>NProdMuNu;
      if(indexcheck!=j||dalitzcheck!=d+1){cout<<" NXT index does not match for MuNu produced   j="<<j<<" d="<<d<<endl;exit(0);}
      HNu[d]->SetBinContent(j+1,NProdMuNu);  
    }
  }


  
  TCanvas C("C");
  C.Print("CompareGenNXTWeightsKKpi.ps[");

  for(Int_t d=0;d<3;d++){
    HDs.GetYaxis()->SetTitle("N_{Ds}");
    HNu[d]->GetYaxis()->SetTitle("N_{Ds}");
    HDs.GetXaxis()->SetTitle("n_{X}^{T}");
    HNu[d]->GetXaxis()->SetTitle("n_{X}^{T}");
    HNu[d]->SetLineColor(4);
    C.Clear();
    HDs.GetYaxis()->SetRangeUser(0.,1.2*HDs.GetMaximum());
    HDs.Draw("hist");
    C.Print("CompareGenNXTWeightsKKpi.ps");
    C.Clear();
    HNu[d]->GetYaxis()->SetRangeUser(0.,1.2*HNu[d]->GetMaximum());
    HNu[d]->Draw("hist");
    C.Print("CompareGenNXTWeightsKKpi.ps");


    HDs.Scale(1./HDs.Integral());
    HNu[d]->Scale(1./HNu[d]->Integral());
    HDs.GetYaxis()->SetTitle("weight");
    HNu[d]->GetYaxis()->SetTitle("weight");
    C.Clear();
    HDs.GetYaxis()->SetRangeUser(0.,1.2*HDs.GetMaximum());
    HDs.Draw("hist");
    HNu[d]->Draw("hist same");
    C.Print("CompareGenNXTWeightsKKpi.ps");

    HNu[d]->Divide(&HDs);
    HNu[d]->GetYaxis()->SetTitle("ratio");
    C.Clear();
    delete HNu[d]->FindObject("stats");
    HNu[d]->SetStats(0);
    HNu[d]->GetYaxis()->SetRangeUser(0.,1.2);
    HNu[d]->Draw("hist");
    C.Print("CompareGenNXTWeightsKKpi.ps");
  

    //print out the correction values
    for(Int_t j=0;j<NXT;j++)
      printf(",%.3f",HNu[d]->GetBinContent(j+1));
    printf("\n ");

  }

   C.Print("CompareGenNXTWeightsKKpi.ps]");

}
