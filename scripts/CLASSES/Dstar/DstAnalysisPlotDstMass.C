Int_t DstAnalysis::MakeDstMass(){
  if(!OpenReducedNtuple())return 0;

  TFile file(_OutputDir+"/DstMassPlot.root","recreate");

  TH1F HDstMass("HDstMass","",170,.138,.155);
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

  Canvas.Clear();
  HDstMass->Draw("pe");
  cutline.SetLineColor(2);
  cutline.DrawLine(DstMeanDeltaM-.001,0,DstMeanDeltaM-.001,HDstMass->GetMaximum());
  cutline.DrawLine(DstMeanDeltaM+.001,0,DstMeanDeltaM+.001,HDstMass->GetMaximum());
  Canvas.Print(filename);
  
  Canvas.Clear();
  HDstMass->Draw("pe");
  cutline.SetLineColor(2);
  cutline.DrawLine(DstMeanDeltaM-.001,0,DstMeanDeltaM-.001,HDstMass->GetMaximum());
  cutline.DrawLine(DstMeanDeltaM+.001,0,DstMeanDeltaM+.001,HDstMass->GetMaximum());
  cutline.SetLineColor(1);
  cutline.DrawLine(DstMeanDeltaM-.003,0,DstMeanDeltaM-.003,.3*HDstMass->GetMaximum());
  cutline.DrawLine(DstMeanDeltaM-.002,0,DstMeanDeltaM-.002,.3*HDstMass->GetMaximum());
  cutline.DrawLine(DstMeanDeltaM+.002,0,DstMeanDeltaM+.002,.3*HDstMass->GetMaximum());
  cutline.DrawLine(DstMeanDeltaM+.003,0,DstMeanDeltaM+.003,.3*HDstMass->GetMaximum());    
  Canvas.Print(filename);


  //fit the D* deltaM  
  delete HDstMass->FindObject("stats");
  Canvas.Clear();
  TF1 fsig;
  TF1 fbkg;
  TF1*fit=FitHistogramDstarDM(HDstMass,.1395,.153,&fsig,&fbkg);
  HDstMass->Draw("pe");
  fbkg.Draw("same");
  fit->Draw("same");
  cutline.SetLineColor(2);
  cutline.DrawLine(DstMeanDeltaM-.001,0,DstMeanDeltaM-.001,HDstMass->GetMaximum());
  cutline.DrawLine(DstMeanDeltaM+.001,0,DstMeanDeltaM+.001,HDstMass->GetMaximum());
  cutline.SetLineColor(1);
  cutline.DrawLine(DstMeanDeltaM-.003,0,DstMeanDeltaM-.003,.3*HDstMass->GetMaximum());
  cutline.DrawLine(DstMeanDeltaM-.002,0,DstMeanDeltaM-.002,.3*HDstMass->GetMaximum());
  cutline.DrawLine(DstMeanDeltaM+.002,0,DstMeanDeltaM+.002,.3*HDstMass->GetMaximum());
  cutline.DrawLine(DstMeanDeltaM+.003,0,DstMeanDeltaM+.003,.3*HDstMass->GetMaximum());    
  Canvas.Print(filename);

  file.Close();
  Canvas.Print(filename+"]");
  return 1;
}
