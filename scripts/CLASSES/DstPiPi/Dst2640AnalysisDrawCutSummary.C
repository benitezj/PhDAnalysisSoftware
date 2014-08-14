
void Dst2640Analysis::DrawCutSummary(TCanvas* Canvas,TString filename){
  
  Float_t cutheight=0;

  Canvas->Clear();
  //Dst2640DeltaMCuts.GetPlotByName("Dst2640MCTrueCut")->Draw();
  Dst2640DeltaMCuts.GetPlot(0)->Draw();
  cutheight=Dst2640DeltaMCuts.GetPlot(0)->GetMaximum();
  cutline.DrawLine(database.GetDst2640DeltaMMin(),0,database.GetDst2640DeltaMMin(),cutheight);
  cutline.DrawLine(database.GetDst2640DeltaMMax(),0,database.GetDst2640DeltaMMax(),cutheight);
  Canvas->Print(filename);

  Canvas->Clear();
  //D0ProbCuts.GetPlotByName("D0MassCut")->Draw();
  D0ProbCuts.GetPlot(0)->Draw();
  cutheight=D0ProbCuts.GetPlot(0)->GetMaximum();
  cutline.DrawLine(database.GetD0MinLogProbab(),0,database.GetD0MinLogProbab(),cutheight);
  Canvas->Print(filename);
  
  Canvas->Clear();
  //D0MassCuts.GetPlotByName("PiLHCut")->Draw();
  D0MassCuts.GetPlot(0)->Draw();
  cutheight=D0MassCuts.GetPlot(0)->GetMaximum();
  cutline.DrawLine(database.GetD0MassMin(),0,database.GetD0MassMin(),cutheight);
  cutline.DrawLine(database.GetD0MassMax(),0,database.GetD0MassMax(),cutheight);
  Canvas->Print(filename); 

  Canvas->Clear();
  //D0PstarCuts.GetPlotByName("Dst2640DeltaMCut")->Draw();
  D0PstarCuts.GetPlot(0)->Draw();
  cutheight=D0PstarCuts.GetPlot(0)->GetMaximum();
  cutline.DrawLine(database.GetD0pstarMin(),0,database.GetD0pstarMin(),cutheight);
  Canvas->Print(filename);

  Canvas->Clear();
  Canvas->SetLogy(1);
  //KLH.GetPlotByName("DstarDeltaMCut")->Draw();
  KLH.GetPlot(0)->Draw();
  cutheight=KLH.GetPlot(0)->GetMaximum();
  cutline.DrawLine(database.GetKLHBit()-.5,0,database.GetKLHBit()-.5,cutheight);
  Canvas->Print(filename);

  Canvas->Clear();
  //PiLH.GetPlotByName("KLHCut")->Draw();
  PiLH.GetPlot(0)->Draw();
  cutheight=PiLH.GetPlot(0)->GetMaximum();
  cutline.DrawLine(database.GetPiLHBit()-.5,0,database.GetPiLHBit()-.5,cutheight);
  Canvas->SetLogy(1);
  Canvas->Print(filename);
  Canvas->SetLogy(0);

  Canvas->Clear();
  //DstarDeltaMCuts.GetPlotByName("D0pstarCut")->Draw();
  DstarDeltaMCuts.GetPlot(0)->Draw();
  cutheight=DstarDeltaMCuts.GetPlot(0)->GetMaximum();
  cutline.DrawLine(database.GetDstarDeltaMMin(),0,database.GetDstarDeltaMMin(),cutheight);
  cutline.DrawLine(database.GetDstarDeltaMMax(),0,database.GetDstarDeltaMMax(),cutheight);
  Canvas->Print(filename);

  Canvas->Clear();
  //Dst2640ProbCuts.GetPlotByName("D0ProbCut")->Draw();
  Dst2640ProbCuts.GetPlot(0)->Draw();
  cutheight=Dst2640ProbCuts.GetPlot(0)->GetMaximum();
  cutline.DrawLine(database.GetDst2640MinLogProbab(),0,database.GetDst2640MinLogProbab(),cutheight);
  Canvas->Print(filename);
  


  //////now draw the backgrounds

  TH1F* Htemp;

  Canvas->Clear();
  Canvas->Divide(4,4);
  for(Int_t b=1;b<=Dst2640DeltaMCuts.GetNCuts();b++){
    Canvas->cd(b);
    Htemp=Dst2640DeltaMCuts.GetBackground(b);
    Htemp->Draw();
  }
  Canvas->Print(filename);

  //now print a page with a list of cuts and names
  Canvas->Clear();
  text.SetTextSize(.04);
  for(Int_t cutnumber=1;cutnumber<=Dst2640DeltaMCuts.GetNCuts();cutnumber++){
    text.DrawTextNDC(.05,1-.05*cutnumber,TString(" ")+(long)cutnumber+". "+Dst2640DeltaMCuts.GetCut(cutnumber)->GetCutName());
  }
  Canvas->Print(filename);
  
  //now print the same cut list from database
  database.PrintCutList(Canvas,filename);

}

