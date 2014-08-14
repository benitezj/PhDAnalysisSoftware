Int_t DstAnalysis::MakeDstMass(){
  if(!OpenReducedNtuple())return 0;

  TFile file(_OutputDir+"/DstMassPlot.root","recreate");

  TH1F HDstMass("HDstMass","",120,.130,.160);
  ReducedNtuple->Draw("dstdeltam>>HDstMass");
  HDstMass.Write();

  file.ls();
  file.Close();

  CloseReducedNtuple();

  return 1;

}

Int_t DstAnalysis::PlotDstMass(){

  TH1F* HDstMass=0;
  TFile file(_OutputDir+"/DstMassPlot.root","read");
  file.ls();
  gROOT->cd();
  if(!(HDstMass=(TH1F*)file.Get("HDstMass")))return 0;
  
  filename=_OutputDir+"/DstMassPlots.ps";
  Canvas.Print(filename+"["); 

  Canvas.Clear();
  HDstMass->Draw("pe");
  Canvas.Print(filename);


  file.Close();
  Canvas.Print(filename+"]");
  return 1;
}
