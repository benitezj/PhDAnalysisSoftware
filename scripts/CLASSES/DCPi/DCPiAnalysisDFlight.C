

Int_t DCPiAnalysis::MakeDFlight(){

  TCut tightcuts=topdatabase->GetCut("xpstartight");

  OpenReducedNtuple();  

  TFile file(_OutputDir+"/DFlightPlots.root","recreate");
  
  //mass histo to go with this pdf plot
  TH1F HDMass("HDMass","",
	      ((DataBaseDC*)topdatabase)->GetDMassRangeNbins(),
	      ((DataBaseDC*)topdatabase)->GetDMassRangeMin(),
	      ((DataBaseDC*)topdatabase)->GetDMassRangeMax());
  topdatabase->DisableCut("dmass");  
  ReducedNtuple->Draw("dmass>>HDMass",tightcuts);
  topdatabase->EnableCut("dmass");
  file.cd();
  HDMass.Write();

  TH1F HFlight("HFlight","",400,-10,30);
  ReducedNtuple->Draw("dflights>>HFlight",topdatabase->GetCut("dmasssig")+tightcuts);
  file.cd();
  HFlight.Write();
  CloseReducedNtuple();


  OpenReducedNtuple();  
  TH1F HFlightLSB("HFlightLSB","",400,-10,30);
  ReducedNtuple->Draw("dflights>>HFlightLSB",tightcuts+topdatabase->GetCut("dmasslsb"));
  file.cd();
  HFlightLSB.Write();

  TH1F HFlightHSB("HFlightHSB","",400,-10,30);
  ReducedNtuple->Draw("dflights>>HFlightHSB",tightcuts+topdatabase->GetCut("dmasshsb"));
  file.cd();
  HFlightHSB.Write();

  CloseReducedNtuple(); 

  file.Close();

  return 1;
}



Int_t DCPiAnalysis::PlotDFlight(){


  TFile file(_OutputDir+"/DFlightPlots.root","update");
  gROOT->cd();

  filename=_OutputDir+"/DFlightPlots.ps";
  Canvas.Print(filename+"["); 


  //get the bkg under the signal
  TH1F* HDMass=(TH1F*)file.Get("HDMass");
  MassFitter massfit(&mass);
  if(!massfit.Initialize(HDMass,_OutputDir+"/InitRooFitParsDMass.txt"))return 0;
  if(massfit.Fit()!=0)return 0;
  RooAbsPdf*bkg=massfit.GetBkgPdf();
  mass.setRange("sigregion",((DataBaseDC*)topdatabase)->GetDMassMin(),((DataBaseDC*)topdatabase)->GetDMassMax());
  RooAbsReal* integral= bkg->createIntegral(mass,"sigregion");
  cout<<",  undersig="<<integral->getVal()<<endl;
  RooPlot*fitplot=massfit.GetFitPlot();
  Canvas.Clear();
  fitplot->Draw();
  TH1F*HMassR=((RooHist*)fitplot->getObject(0))->GetHistogram();
  ShowDMassRegions(HMassR);
  Canvas.Print(filename);
  ///



  TH1F*HFlight=(TH1F*)file.Get("HFlight");
  Canvas.Clear();
  HFlight->Draw("pe");
  Canvas.Print(filename);

  TH1F*HFlightLSB=(TH1F*)file.Get("HFlightLSB");
  TH1F*HFlightHSB=(TH1F*)file.Get("HFlightHSB");  
  Canvas.Clear();
  HFlightHSB->Draw("pe");
  HFlightLSB->Draw("pesame");
  Canvas.Print(filename);




  TH1F* HFlightSBAvg=(TH1F*)HFlightLSB->Clone();
  HFlightSBAvg->SetName("HFlightSBAvg");
  HFlightSBAvg->Add(HFlightHSB);
  HFlightSBAvg->Scale(integral->getVal()/HFlightSBAvg->Integral());
  file.cd();
  HFlightSBAvg->Write();

  TH1F* HFlightSignal=(TH1F*)HFlight->Clone();
  HFlightSignal->SetName("HFlightSignal");
  HFlightSignal->Add(HFlightSBAvg,-1);
  file.cd();
  HFlightSignal->Write(); 
 
  Canvas.Clear();
  HFlight->Draw("pe");
  HFlightSBAvg->Draw("pesame");
  HFlightSignal->Draw("pesame");
  Canvas.Print(filename);

  Canvas.Print(filename+"]"); 


  file.Close();

  return 1;
}





