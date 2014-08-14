

Int_t DCPiAnalysis::FitMassVsPolOrder(){

  TFile file(_OutputDir+"/HistosForFit.root","read");
  gROOT->cd();
    
  TFile outfile(_OutputDir+"/PolOrderFits/DCPiMassPlotsVsPolOrder.root","update");
  gROOT->cd();

  TH1F*HDCPiMass=(TH1F*)file.Get(TString("HDCPi"));

		
  for(Int_t i=2;i<=6;i++){  
    
    Bool_t fit=0;
    if(!inputpars.OpenFitPars(TString(_OutputDir+"/PolOrderFits/FitPars_")+(long)i+".txt"))
      fit=1;
    if(inputpars.GetFitPar("Range","status")!=0)
      fit=1;
    inputpars.CloseFitPars();

    if(fit){      
      if(!massfitter.Initialize(HDCPiMass,_OutputDir+"/PolOrderFits/InitRooFitPars"+(long)i+".txt"))return 0;
      massfitter.Fit();
      massfitter.SetFileTag(TString("_")+(long)i);
      massfitter.Save(_OutputDir+"/PolOrderFits");
      TString tag=massfitter.GetFileTag();

      outfile.cd();
      outfile.Delete(TString("fitplot")+(long)i+";1");
      //RooPlot* fitplot=massfitter.GetFitPlot(); 
      TH1F*fitplot=massfitter.GetNormResidualHisto();
      if(!fitplot){cout<<" no fitplot"<<endl;return 0;}
      fitplot->SetName(TString("fitplot")+(long)i);
      fitplot->Write();
      delete fitplot;
      gROOT->cd();

      cout<<"Done with fit "<<i<<endl;
    }
  }
		
  outfile.ls();	
  outfile.Close();
  file.Close();

  return 1;
}

Int_t DCPiAnalysis::DrawMassVsPolOrder(){

  TFile file(_OutputDir+"/PolOrderFits/DCPiMassPlotsVsPolOrder.root","read");
  file.ls();	
  gROOT->cd();


  filename=_OutputDir+"/PolOrderFits/DCPiMassPlotsVsPolOrder.ps";
  Canvas.Print(filename+"["); 


  TH1F HYVsPolOrder("HYVsPolOrder","",6,.5,6.5);
  SetHistoXYLabels(&HYVsPolOrder,"Polynomial Order","");
  TH1F HMVsPolOrder("HMVsPolOrder","",6,.5,6.5);
  SetHistoXYLabels(&HMVsPolOrder,"Polynomial Order","");
  TH1F HWVsPolOrder("HWVsPolOrder","",6,.5,6.5);
  SetHistoXYLabels(&HWVsPolOrder,"Polynomial Order","");
  TH1F HBVsPolOrder("HBVsPolOrder","",6,.5,6.5);
  SetHistoXYLabels(&HBVsPolOrder,"Polynomial Order","");
  TH1F HChisqVsPolOrder("HChisqVsPolOrder","",6,.5,6.5);
  SetHistoXYLabels(&HChisqVsPolOrder,"Polynomial Order","");
  
  Canvas.Clear();
  Canvas.Divide(2,3);
  text.SetTextSize(.2);
  for(Int_t i=2;i<=6;i++){

    //RooPlot* fitplot=(RooPlot*)file.Get(TString("fitplot")+(long)i);
    TH1F* fitplot=(TH1F*)file.Get(TString("fitplot")+(long)i);
    if(!fitplot){cout<<"no plot"<<endl;continue;}
    
    Canvas.cd(i);
    Canvas.GetPad(i)->SetTopMargin(.09);
    Canvas.GetPad(i)->SetBottomMargin(.2);
    Canvas.GetPad(i)->SetLeftMargin(.1);
    Canvas.GetPad(i)->SetRightMargin(.18);
   
    fitplot->SetTitle("");
    fitplot->GetYaxis()->SetRangeUser(-6,6);
    fitplot->GetYaxis()->SetTitle("");
    fitplot->GetXaxis()->SetTitle("");
    fitplot->GetXaxis()->SetTitleSize(.1);
    fitplot->GetYaxis()->SetTitleSize(.1);
    fitplot->GetYaxis()->SetTitleOffset(.5);
    fitplot->GetXaxis()->SetLabelSize(.1);
    fitplot->GetYaxis()->SetLabelSize(.1);
    fitplot->GetYaxis()->SetNdivisions(6);
    fitplot->GetXaxis()->SetNdivisions(5);

    fitplot->Draw("p");
    text.DrawTextNDC(.15,.3,TString("")+(long)(i));
    

    inputpars.CloseFitPars();
    if(!inputpars.OpenFitPars(TString(_OutputDir+"/PolOrderFits/FitPars_")+(long)i+".txt"))continue;
    if(inputpars.GetFitPar("Range","status")!=0){cout<<"did not converge"<<endl;continue;}
   

    HYVsPolOrder.SetBinContent(i,inputpars.GetFitPar("D2460","yield"));
    HYVsPolOrder.SetBinError(i,inputpars.GetFitPar("D2460","yielderror"));

    HMVsPolOrder.SetBinContent(i,inputpars.GetFitPar("D2460","mean")*1000);
    HMVsPolOrder.SetBinError(i,inputpars.GetFitPar("D2460","meanerror")*1000);

    HWVsPolOrder.SetBinContent(i,inputpars.GetFitPar("D2460","width")*1000);
    HWVsPolOrder.SetBinError(i,inputpars.GetFitPar("D2460","widtherror")*1000);

    HBVsPolOrder.SetBinContent(i,inputpars.GetFitPar("Background","yield"));
    //HBVsPolOrder.SetBinError(i,inputpars.GetFitPar("Background","yielderror"));

    HChisqVsPolOrder.SetBinContent(i,inputpars.GetFitPar("Range","chisquare")/inputpars.GetFitPar("Range","ndf"));
         
  }

  Canvas.Print(filename);

  Canvas.Clear();
  HYVsPolOrder.GetYaxis()->SetRangeUser(0,1.1*HYVsPolOrder.GetMaximum());
  HYVsPolOrder.SetStats(0);
  HYVsPolOrder.Draw("pe");
  Canvas.Print(filename);

  Canvas.Clear();
  HMVsPolOrder.SetStats(0);
  HMVsPolOrder.GetYaxis()->SetRangeUser(2450,2470);
  HMVsPolOrder.Draw("pe");
  Canvas.Print(filename);

  Canvas.Clear();
  HWVsPolOrder.SetStats(0);
  if(_MCorDATA==0)HWVsPolOrder.GetYaxis()->SetRangeUser(20,40);
  if(_MCorDATA==1)HWVsPolOrder.GetYaxis()->SetRangeUser(40,60);
  HWVsPolOrder.Draw("pe");
  Canvas.Print(filename);

  Canvas.Clear();
  HBVsPolOrder.SetStats(0);
  HBVsPolOrder.Draw("p");
  Canvas.Print(filename);


  Canvas.Clear();
  HChisqVsPolOrder.GetYaxis()->SetRangeUser(0,1.1*HChisqVsPolOrder.GetMaximum());
  HChisqVsPolOrder.SetStats(0);
  HChisqVsPolOrder.Draw("p");
  Canvas.Print(filename);


  Canvas.Print(filename+"]");
   		
  file.Close();

  return 1;
}


