
void Ds1Analysis::DrawCutSummary(TCanvas* Canvas,TString filename){
  
  Bool_t SetLogY=0;

  Canvas->Clear();
  Ds1pDeltaM2Cuts.GetPlotByName("Ds1pMCTrueCut")->Draw();
  cutline.DrawLine(database.GetDs1pDeltaMMin(),0,
		   database.GetDs1pDeltaMMin(),Ds1pDeltaM2Cuts.GetPlotByName("Ds1pMCTrueCut")->GetMaximum());
  cutline.DrawLine(database.GetDs1pDeltaMMax(),0,
		   database.GetDs1pDeltaMMax(),Ds1pDeltaM2Cuts.GetPlotByName("Ds1pMCTrueCut")->GetMaximum());
  Canvas->Print(filename);
  Canvas->Clear();
  

  D0PstarCuts.GetPlotByName("Ds1pDeltaM2Cut")->Draw();
  cutline.DrawLine(database.GetD0pstarMin(),0,
		   database.GetD0pstarMin(),D0PstarCuts.GetPlotByName("Ds1pDeltaM2Cut")->GetMaximum());
  Canvas->Print(filename);
  Canvas->Clear();
 

  DstarDeltaMCuts.GetPlotByName("D0pstarCut")->Draw();
  cutline.DrawLine(database.GetDstarDeltaMMin(),0,database.GetDstarDeltaMMin(),
		   DstarDeltaMCuts.GetPlotByName("D0pstarCut")->GetMaximum());
  cutline.DrawLine(database.GetDstarDeltaMMax(),0,
		   database.GetDstarDeltaMMax(),DstarDeltaMCuts.GetPlotByName("D0pstarCut")->GetMaximum());
  Canvas->Print(filename);
  Canvas->Clear();

  KLH.GetPlotByName("DstarDeltaMCut")->Draw("bar");
  cutline.DrawLine(database.GetKLHBit()-.5,0,
		   database.GetKLHBit()-.5,KLH.GetPlotByName("DstarDeltaMCut")->GetMaximum());
  Canvas->SetLogy(1);
  Canvas->Print(filename);
  Canvas->SetLogy(0);

  Canvas->Clear();
  PiLH.GetPlotByName("KLHCut")->Draw("bar");
  cutline.DrawLine(database.GetPiLHBit()-.5,0,
		   database.GetPiLHBit()-.5,PiLH.GetPlotByName("KLHCut")->GetMaximum());
  Canvas->SetLogy(1);
  Canvas->Print(filename);
  Canvas->SetLogy(0);

  Canvas->Clear();
  D0MassCuts.GetPlotByName("PiLHCut")->Draw();
  cutline.DrawLine(database.GetD0MassMin(),0,
		   database.GetD0MassMin(),D0MassCuts.GetPlotByName("PiLHCut")->GetMaximum());
  cutline.DrawLine(database.GetD0MassMax(),0,
		   database.GetD0MassMax(),D0MassCuts.GetPlotByName("PiLHCut")->GetMaximum());
  Canvas->Print(filename);

  Canvas->Clear();
  D0ProbCuts.GetPlotByName("D0MassCut")->Draw();
  cutline.DrawLine(database.GetD0MinLogProbab(),0,
		   database.GetD0MinLogProbab(),D0ProbCuts.GetPlotByName("D0MassCut")->GetMaximum());
  Canvas->Print(filename);

  Canvas->Clear();
  KsMassCuts.GetPlotByName("D0ProbCut")->Draw();
  cutline.DrawLine(database.GetKsMassMin(),0,
		   database.GetKsMassMin(),KsMassCuts.GetPlotByName("D0ProbCut")->GetMaximum());
  cutline.DrawLine(database.GetKsMassMax(),0,
		   database.GetKsMassMax(),KsMassCuts.GetPlotByName("D0ProbCut")->GetMaximum());
  Canvas->Print(filename);

  Canvas->Clear();
  KsCosineCuts.GetPlotByName("KsMassCut")->Draw();
  cutline.DrawLine(database.GetKsCosineMin(),0,
		   database.GetKsCosineMin(),KsCosineCuts.GetPlotByName("KsMassCut")->GetMaximum());
  Canvas->SetLogy(1);
  Canvas->Print(filename);
  Canvas->SetLogy(0);


  Canvas->Clear();
  Ds1pProbCuts.GetPlotByName("KsCosineCut")->Draw();
  cutline.DrawLine(database.GetDs1pMinLogProbab(),0,
		   database.GetDs1pMinLogProbab(),Ds1pProbCuts.GetPlotByName("KsCosineCut")->GetMaximum());
  Canvas->Print(filename);

  Canvas->Clear();
  Ds1ppstarCuts.GetPlotByName("Ds1pProbCut")->Draw();
  cutline.DrawLine(database.GetDs1ppstarMin(),0,
		   database.GetDs1ppstarMin(),Ds1ppstarCuts.GetPlotByName("Ds1pProbCut")->GetMaximum());
  Canvas->Print(filename);
  


  //////now draw the backgrounds
  TH1F* Htemp;
  Canvas->Clear();
  Canvas->Divide(4,4);

  for(Int_t b=1;b<=Ds1pDeltaM2Cuts.GetNCuts();b++){
    Canvas->cd(b);
    Canvas->GetPad(b)->SetLogy(SetLogY);
    Htemp=Ds1pDeltaM2Cuts.GetBackground(b);
    Htemp->Draw();
   
  }
  Canvas->Print(filename);

}
