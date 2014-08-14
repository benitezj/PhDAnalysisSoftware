
Int_t Dst0PiAnalysis::MakeRawDst0PiMass(){
 
  if(OpenReducedNtuple()!=1) return 0;
  TH1F HDst0PiMass("HDst0PiMass","",2000,1.9,3.9);
  ReducedNtuple->Draw("xmdm>>HDst0PiMass");
  if(CloseReducedNtuple()!=1) return 0;
  
  TFile file(_OutputDir+"/RawMassPlots.root","recreate");
  HDst0PiMass.Write();
  file.Close();

  return 1;
}

Int_t Dst0PiAnalysis::PlotRawDst0PiMass(){
    

  filename=_OutputDir+"/RawDst0PiMassPlots.ps";
  Canvas.Print(filename+"["); 

  TFile file(_OutputDir+"/RawMassPlots.root","read");
  gROOT->cd();
 
  Int_t nbins=200;
  Float_t min=2.;
  Float_t max=3.;

  //right sign
  TH1F*HDst0PiMass=(TH1F*)file.Get("HDst0PiMass");
  if(!(HDst0PiMass=(TH1F*)RebinHistoX(HDst0PiMass,nbins,min,max)))return 0; 
  SetHistoXYLabels(HDst0PiMass,"m(D#pi)","GeV/c^{2}");
  HDst0PiMass->GetYaxis()->SetRangeUser(0,HDst0PiMass->GetMaximum()*1.1);

  ///
  Canvas.Clear();
  HDst0PiMass->Draw("pe");
  Canvas.Print(filename); 

  Canvas.Print(filename+"]");

  file.Close();

  return 1;
}

Int_t Dst0PiAnalysis::FitMass(TString name,TString subname){

  TFile file(_OutputDir+"/HistosForFit.root","read");
  gROOT->cd();

  TH1F*HDst0PiMass=(TH1F*)file.Get(name);
  TH1F*HDst0PiMassSB=(TH1F*)file.Get(subname);    
  if(HDst0PiMassSB){
    cout<<" Side band Histo found. Going to subtract sideband"<<endl;
    HDst0PiMass->Sumw2();
    HDst0PiMassSB->Sumw2();
    HDst0PiMassSB->Scale(.868); cout<<" Sideband scale factor is "<<.868<<endl;
    HDst0PiMass->Add(HDst0PiMassSB,-1);
  }
 
  if(!HDst0PiMass){cout<<"no histo"<<endl;return 0;}
  SetHistoXYLabels(HDst0PiMass,"m(D#pi)","GeV/c^{2}");
  HDst0PiMass->SetStats(0);
  
  //massfitter.SetEfficiency(HDst0PiMassEff); 
  if(!massfitter.Initialize(HDst0PiMass,_OutputDir+"/InitRooFitPars.txt"))return 0;
  if(massfitter.Fit()<0)return 0;
  massfitter.Save(_OutputDir);

  filename=_OutputDir+"/Dst0PiMassPlots.ps";
  Canvas.Print(filename+"[");  
  
  Canvas.Clear();
  HDst0PiMass->Draw("pe");
  Canvas.Print(filename); 

  RooPlot*fitplot=massfitter.GetFitPlot(); 
  Canvas.Clear();
  fitplot->Draw();
  Canvas.Print(filename);
  delete fitplot; 


  Canvas.Print(filename+"]");

 
  file.Close();
  return 1;
}



