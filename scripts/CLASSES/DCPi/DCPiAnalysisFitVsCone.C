


Int_t DCPiAnalysis::FitMassVsCone(){

  TFile file(_OutputDir+"/HistosForFit.root","read");
  gROOT->cd();
  
  TFile outfile(_OutputDir+"/ConeFits/DCPiMassPlotsVsCone.root","update");
  gROOT->cd();

  Int_t Nfits=5;
		
  for(Int_t i=0;i<Nfits;i++){  
    Bool_t fit=0;
    if(!inputpars.OpenFitPars(TString(_OutputDir+"/ConeFits/FitPars_")+(long)i+".txt"))
      fit=1;
    else if(inputpars.GetFitPar("Range","status")!=0)
      fit=1;
    inputpars.CloseFitPars();

    if(fit){
      TH1F*HDCPiMass=NULL;
      HDCPiMass=(TH1F*)file.Get(TString("HDCPi_ConeCoarse")+(long)i);

      if(!massfitter.Initialize(HDCPiMass,_OutputDir+"/ConeFits/InitRooFitPars.txt"))continue;

      massfitter.Fit();
      massfitter.SetFileTag(TString("_")+(long)i);
      massfitter.Save(_OutputDir+"/ConeFits");
      TString tag=massfitter.GetFileTag();
      
      outfile.cd();
      outfile.Delete(TString("fitplot")+(long)i+";1");
      RooPlot* fitplot=massfitter.GetFitPlot(); 
      fitplot->SetName(TString("fitplot")+(long)i);
      fitplot->Write();
      delete fitplot;
      gROOT->cd();
    }
  }
	
	
  outfile.ls();	
  outfile.Close();
  file.Close();
  

  return 1;
}

Int_t DCPiAnalysis::DrawMassVsCone(){

  TFile file(_OutputDir+"/ConeFits/DCPiMassPlotsVsCone.root","read");
  file.ls();	
  gROOT->cd();
  

  filename=_OutputDir+"/ConeFits/DCPiMassPlotsVsCone.ps";
  Canvas.Print(filename+"["); 

  Int_t Nfits=5;
  Int_t Nbins=5;

  TH1F HYVsCone("HYVsCone","",Nbins,0.,1);
  SetHistoXYLabels(&HYVsCone,"cos(#theta)","");
  HYVsCone.GetYaxis()->SetTitle("Yield");
  TH1F HMVsCone("HMVsCone","",Nbins,0.,1);
  SetHistoXYLabels(&HMVsCone,"cos(#theta)","");
  HMVsCone.GetYaxis()->SetTitle("Mass (MeV)");
  TH1F HWVsCone("HWVsCone","",Nbins,0.,1);
  SetHistoXYLabels(&HWVsCone,"cos(#theta)","");
  HWVsCone.GetYaxis()->SetTitle("Width (MeV)");
  TH1F HChisqVsCone("HChisqVsCone","",Nbins,0.,1);
  SetHistoXYLabels(&HChisqVsCone,"cos(#theta)","");
  HChisqVsCone.GetYaxis()->SetTitle("#chi^{2}/ndf");
  
  Canvas.Clear();
  Canvas.Divide(2,3);
  text.SetTextSize(.2);
  for(Int_t i=0;i<Nfits;i++){  
    
    RooPlot* fitplot=(RooPlot*)file.Get(TString("fitplot")+(long)i);
    if(!fitplot)continue;   
       
    Canvas.cd(i+1);
    Canvas.GetPad(i+1)->SetTopMargin(.09);
    Canvas.GetPad(i+1)->SetBottomMargin(.2);
    Canvas.GetPad(i+1)->SetLeftMargin(.1);
    Canvas.GetPad(i+1)->SetRightMargin(.18);
   
    fitplot->SetTitle("");
    fitplot->GetYaxis()->SetTitle("");
    fitplot->GetXaxis()->SetTitle("");
    fitplot->GetXaxis()->SetTitleSize(.1);
    fitplot->GetYaxis()->SetTitleSize(.1);
    fitplot->GetYaxis()->SetTitleOffset(.5);
    fitplot->GetXaxis()->SetLabelSize(.1);
    fitplot->GetYaxis()->SetLabelSize(.1);
    fitplot->GetYaxis()->SetNdivisions(5);
    fitplot->GetXaxis()->SetNdivisions(5);

    fitplot->Draw();
    text.DrawTextNDC(.65,.7,TString("")+(long)(i+1));
   

    if(!inputpars.OpenFitPars(TString(_OutputDir+"/ConeFits/FitPars_")+(long)i+".txt"))continue;
    if(inputpars.GetFitPar("Range","status")!=0)continue;

    HYVsCone.SetBinContent(i+1,inputpars.GetFitPar("D2460","yield"));
    HYVsCone.SetBinError(i+1,inputpars.GetFitPar("D2460","yielderror"));

    HMVsCone.SetBinContent(i+1,inputpars.GetFitPar("D2460","mean")*1000);
    HMVsCone.SetBinError(i+1,inputpars.GetFitPar("D2460","meanerror")*1000);

    HWVsCone.SetBinContent(i+1,inputpars.GetFitPar("D2460","width")*1000);
    HWVsCone.SetBinError(i+1,inputpars.GetFitPar("D2460","widtherror")*1000);
    
    HChisqVsCone.SetBinContent(i+1,inputpars.GetFitPar("Range","chisquare")/inputpars.GetFitPar("Range","ndf"));
    

    inputpars.CloseFitPars();
  }
  Canvas.Print(filename);
  
  Canvas.Clear();
  HYVsCone.GetYaxis()->SetRangeUser(0,1.1*HYVsCone.GetMaximum());
  HYVsCone.SetStats(0);
  HYVsCone.Draw("pe");
  Canvas.Print(filename);
  Canvas.Clear();
  HYVsCone.SetStats(0);
  HYVsCone.Draw("pe");
  Canvas.Print(filename);


  Canvas.Clear();
  HMVsCone.SetStats(0);
  if(_MCorDATA==1)HMVsCone.GetYaxis()->SetRangeUser(2450,2475);
  if(_MCorDATA==0)HMVsCone.GetYaxis()->SetRangeUser(2450,2470);
  HMVsCone.Draw("pe");
  Canvas.Print(filename);

  Canvas.Clear();
  HMVsCone.Draw("pe");
  Canvas.Print(filename);

  Canvas.Clear();
  HWVsCone.GetYaxis()->SetTitle("Width (MeV)");
  HWVsCone.SetStats(0);
  if(_MCorDATA==1)HWVsCone.GetYaxis()->SetRangeUser(35,65);
  if(_MCorDATA==0)HWVsCone.GetYaxis()->SetRangeUser(25,45);
  HWVsCone.Draw("pe");
  Canvas.Print(filename);

  Canvas.Clear();
  HWVsCone.Draw("pe");
  Canvas.Print(filename);


  Canvas.Clear();
  HChisqVsCone.GetYaxis()->SetRangeUser(0,8);
  HChisqVsCone.SetStats(0);
  HChisqVsCone.Draw("p");
  Canvas.Print(filename);


  Canvas.Print(filename+"]");
   		
  file.Close();

  return 1;
}




