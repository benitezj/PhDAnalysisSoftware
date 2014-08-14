

Int_t DCPiAnalysis::FitMassVsEnergy(){

  TFile file(_OutputDir+"/HistosForFit.root","read");
  gROOT->cd();
  
  TFile outfile(_OutputDir+"/EnergyFits/DCPiMassPlotsVsEnergy.root","update");
  gROOT->cd();
		
  for(Int_t i=0;i<22;i++){  
    
    Bool_t fit=0;
    if(!inputpars.OpenFitPars(TString(_OutputDir+"/EnergyFits/FitPars_")+(long)i+".txt"))
      fit=1;
    if(inputpars.GetFitPar("Range","status")!=0)
      fit=1;
    inputpars.CloseFitPars();

    if(fit){
      TH1F*HDCPiMass=(TH1F*)file.Get(TString("HDCPi_Energy")+(long)i);
      if(!massfitter.Initialize(HDCPiMass,_OutputDir+"/EnergyFits/InitRooFitPars.txt"))return 0;
      massfitter.Fit();
      massfitter.SetFileTag(TString("_")+(long)i);
      massfitter.Save(_OutputDir+"/EnergyFits");
      TString tag=massfitter.GetFileTag();

      outfile.cd();
      outfile.Delete(TString("fitplot")+(long)i+";1");
      RooPlot* fitplot=massfitter.GetFitPlot(); 
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

Int_t DCPiAnalysis::DrawMassVsEnergy(){

  TFile file(_OutputDir+"/EnergyFits/DCPiMassPlotsVsEnergy.root","read");
  file.ls();	
  gROOT->cd();


  filename=_OutputDir+"/EnergyFits/DCPiMassPlotsVsEnergy.ps";
  Canvas.Print(filename+"["); 


  TH1F HYVsEnergy("HYVsEnergy","",22,3.4,5.6);
  SetHistoXYLabels(&HYVsEnergy,"E","GeV");
  TH1F HMVsEnergy("HMVsEnergy","",22,3.4,5.6);
  SetHistoXYLabels(&HMVsEnergy,"E","GeV");
  TH1F HWVsEnergy("HWVsEnergy","",22,3.4,5.6);
  SetHistoXYLabels(&HWVsEnergy,"E","GeV");
  TH1F HBVsEnergy("HBVsEnergy","",22,3.4,5.6);
  SetHistoXYLabels(&HBVsEnergy,"E","GeV");
  TH1F HChisqVsEnergy("HChisqVsEnergy","",22,3.4,5.6);
  SetHistoXYLabels(&HChisqVsEnergy,"E","GeV");
  
  for(Int_t i=0;i<22;i++){  
    RooPlot* fitplot=(RooPlot*)file.Get(TString("fitplot")+(long)i);
    if(!fitplot){cout<<"no plot"<<endl;continue;}
    inputpars.CloseFitPars();
    if(!inputpars.OpenFitPars(TString(_OutputDir+"/EnergyFits/FitPars_")+(long)i+".txt"))continue;
    //if(inputpars.GetFitPar("Range","status")!=0){cout<<"did not converge"<<endl;continue;}
    if(inputpars.GetFitPar("Range","chisquare")/inputpars.GetFitPar("Range","ndf")>4){cout<<"did not converge"<<endl;continue;}


    Canvas.Clear();
    fitplot->Draw();
    Canvas.Print(filename);
   

    HYVsEnergy.SetBinContent(i+1,inputpars.GetFitPar("D2460","yield"));
    HYVsEnergy.SetBinError(i+1,inputpars.GetFitPar("D2460","yielderror"));

    HMVsEnergy.SetBinContent(i+1,inputpars.GetFitPar("D2460","mean")*1000);
    HMVsEnergy.SetBinError(i+1,inputpars.GetFitPar("D2460","meanerror")*1000);

    HWVsEnergy.SetBinContent(i+1,inputpars.GetFitPar("D2460","width")*1000);
    HWVsEnergy.SetBinError(i+1,inputpars.GetFitPar("D2460","widtherror")*1000);

    HBVsEnergy.SetBinContent(i+1,inputpars.GetFitPar("Background","yield"));
    //HBVsEnergy.SetBinError(i+1,inputpars.GetFitPar("Background","yielderror"));

    HChisqVsEnergy.SetBinContent(i+1,inputpars.GetFitPar("Range","chisquare")/inputpars.GetFitPar("Range","ndf"));
         
  }



  Canvas.Clear();
  HYVsEnergy.GetYaxis()->SetRangeUser(0,1.1*HYVsEnergy.GetMaximum());
  HYVsEnergy.SetStats(0);
  HYVsEnergy.Draw("pe");
  Canvas.Print(filename);

  Canvas.Clear();
  HMVsEnergy.SetStats(0);
  HMVsEnergy.GetYaxis()->SetRangeUser(2450,2470);
  HMVsEnergy.Draw("pe");
  Canvas.Print(filename);

  Canvas.Clear();
  HWVsEnergy.SetStats(0);
  HWVsEnergy.GetYaxis()->SetRangeUser(40,60);
  HWVsEnergy.Draw("pe");
  Canvas.Print(filename);

  Canvas.Clear();
  HBVsEnergy.SetStats(0);
  HBVsEnergy.Draw("p");
  Canvas.Print(filename);


  Canvas.Clear();
  HChisqVsEnergy.GetYaxis()->SetRangeUser(0,1.1*HChisqVsEnergy.GetMaximum());
  HChisqVsEnergy.SetStats(0);
  HChisqVsEnergy.Draw("p");
  Canvas.Print(filename);


  Canvas.Print(filename+"]");
   		
  file.Close();

  return 1;
}



Int_t DCPiAnalysis::FitMassVsEnergyD2700(){

  TFile file(_OutputDir+"/HistosForFit.root","read");
  gROOT->cd();
  
  TFile outfile(_OutputDir+"/EnergyFits/DCPiMassPlotsVsEnergyD2700.root","update");
  gROOT->cd();


		
  for(Int_t i=0;i<11;i++){  
    Bool_t fit=0;
    if(!inputpars.OpenFitPars(TString(_OutputDir+"/EnergyFits/FitPars_")+(long)i+"_D2700.txt"))
      fit=1;
    if(inputpars.GetFitPar("Range","status")!=0)
      fit=1;
    inputpars.CloseFitPars();

    if(fit){
      TH1F*HDCPiMass=(TH1F*)file.Get(TString("HDCPi_EnergyCoarse")+(long)i);
      if(!massfitter.Initialize(HDCPiMass,_OutputDir+"/EnergyFits/InitRooFitParsD2700.txt"))return 0;
      massfitter.Fit();
      massfitter.SetFileTag(TString("_")+(long)i+"_D2700");
      massfitter.Save(_OutputDir+"/EnergyFits");
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

Int_t DCPiAnalysis::DrawMassVsEnergyD2700(){

  TFile file(_OutputDir+"/EnergyFits/DCPiMassPlotsVsEnergyD2700.root","read");
  file.ls();	
  gROOT->cd();


  filename=_OutputDir+"/EnergyFits/DCPiMassPlotsVsEnergyD2700.ps";
  Canvas.Print(filename+"["); 

  TH1F HYVsEnergy("HYVsEnergy","",11,3.4,5.6);
  SetHistoXYLabels(&HYVsEnergy,"E","GeV");
  TH1F HMVsEnergy("HMVsEnergy","",11,3.4,5.6);
  SetHistoXYLabels(&HMVsEnergy,"E","GeV");
  TH1F HWVsEnergy("HWVsEnergy","",11,3.4,5.6);
  SetHistoXYLabels(&HWVsEnergy,"E","GeV");
  TH1F HBVsEnergy("HBVsEnergy","",11,3.4,5.6);
  SetHistoXYLabels(&HBVsEnergy,"E","GeV");
  
  for(Int_t i=0;i<11;i++){  
    RooPlot* fitplot=(RooPlot*)file.Get(TString("fitplot")+(long)i);
    if(!fitplot){cout<<"no plot"<<endl;continue;}
    Canvas.Clear();
    fitplot->Draw();
    Canvas.Print(filename);

    inputpars.CloseFitPars();
    if(!inputpars.OpenFitPars(TString(_OutputDir+"/EnergyFits/FitPars_")+(long)i+"_D2700.txt"))continue;
    //if(inputpars.GetFitPar("Range","status")!=0){cout<<"did not converge"<<endl;continue;}
    if(inputpars.GetFitPar("Range","chisquare")/inputpars.GetFitPar("Range","ndf")>4){
      cout<<"did not converge"<<endl;
      continue;
    }  

   
    HYVsEnergy.SetBinContent(i+1,inputpars.GetFitPar("D2700","yield"));
    HYVsEnergy.SetBinError(i+1,inputpars.GetFitPar("D2700","yielderror"));

    HMVsEnergy.SetBinContent(i+1,inputpars.GetFitPar("D2700","mean")*1000);
    HMVsEnergy.SetBinError(i+1,inputpars.GetFitPar("D2700","meanerror")*1000);

    HWVsEnergy.SetBinContent(i+1,inputpars.GetFitPar("D2700","width")*1000);
    HWVsEnergy.SetBinError(i+1,inputpars.GetFitPar("D2700","widtherror")*1000);

    HBVsEnergy.SetBinContent(i+1,inputpars.GetFitPar("Background","yield"));
    //HBVsEnergy.SetBinError(i+1,inputpars.GetFitPar("Background","yielderror"));

      


  }



  Canvas.Clear();
  HYVsEnergy.SetStats(0);
  HYVsEnergy.Draw("pe");
  Canvas.Print(filename);

  Canvas.Clear();
  HMVsEnergy.SetStats(0);
  HMVsEnergy.GetYaxis()->SetRangeUser(2730,2790);
  HMVsEnergy.Draw("pe");
  Canvas.Print(filename);

  Canvas.Clear();
  HWVsEnergy.SetStats(0);
  HWVsEnergy.GetYaxis()->SetRangeUser(30,90);
  HWVsEnergy.Draw("pe");
  Canvas.Print(filename);

  Canvas.Clear();
  HBVsEnergy.SetStats(0);
  HBVsEnergy.Draw("pe");
  Canvas.Print(filename);


  Canvas.Print(filename+"]");
   		
  file.Close();

  return 1;
}
/*
Int_t DCPiAnalysis::FitMassVsEnergyGlobal(){

  TFile file(_OutputDir+"/HistosForFit.root","read");
  gROOT->cd();
  
  TFile outfile(_OutputDir+"/EnergyFits/DCPiMassPlotsVsEnergyGlobal.root","update");
  gROOT->cd();

		
  for(Int_t i=0;i<11;i++){  
    
    Bool_t fit=0;
    if(!inputpars.OpenFitPars(TString(_OutputDir+"/EnergyFits/FitPars_")+(long)i+"_Global.txt"))
      fit=1;
    if(inputpars.GetFitPar("Range","status")!=0)
      fit=1;
    inputpars.CloseFitPars();

    if(fit){
      TH1F*HDCPiMass=(TH1F*)file.Get(TString("HDCPi_EnergyCoarse")+(long)i);
      if(!massfitter.Initialize(HDCPiMass,_OutputDir+"/EnergyFits/InitRooFitParsGlobal.txt"))return 0;
      massfitter.Fit();
      massfitter.SetFileTag(TString("_")+(long)i+"_Global");
      massfitter.Save(_OutputDir+"/EnergyFits");
      TString tag=massfitter.GetFileTag();

      outfile.cd();
      outfile.Delete(TString("fitplot")+(long)i+";1");
      RooPlot* fitplot=massfitter.GetFitPlot(); 
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

Int_t DCPiAnalysis::DrawMassVsEnergyGlobal(){

  TFile file(_OutputDir+"/EnergyFits/DCPiMassPlotsVsEnergyGlobal.root","read");
  file.ls();	
  gROOT->cd();


  filename=_OutputDir+"/EnergyFits/DCPiMassPlotsVsEnergyGlobal.ps";
  Canvas.Print(filename+"["); 


  TH1F HYVsEnergy("HYVsEnergy","",11,3.4,5.6);
  SetHistoXYLabels(&HYVsEnergy,"E","GeV");
  TH1F HY2VsEnergy("HY2VsEnergy","",11,3.4,5.6);
  SetHistoXYLabels(&HY2VsEnergy,"E","GeV");
  TH1F HY3VsEnergy("HY3VsEnergy","",11,3.4,5.6);
  SetHistoXYLabels(&HY3VsEnergy,"E","GeV");
  TH1F HY4VsEnergy("HY4VsEnergy","",11,3.4,5.6);
  SetHistoXYLabels(&HY4VsEnergy,"E","GeV");
  TH1F HBVsEnergy("HBVsEnergy","",11,3.4,5.6);
  SetHistoXYLabels(&HBVsEnergy,"E","GeV");
  TH1F HChisqVsEnergy("HChisqVsEnergy","",11,3.4,5.6);
  SetHistoXYLabels(&HChisqVsEnergy,"E","GeV");
  
  for(Int_t i=0;i<11;i++){  
    RooPlot* fitplot=(RooPlot*)file.Get(TString("fitplot")+(long)i);
    if(!fitplot){cout<<"no plot"<<endl;continue;}

    Canvas.Clear();
    fitplot->Draw();
    Canvas.Print(filename);

    inputpars.CloseFitPars();
    if(!inputpars.OpenFitPars(TString(_OutputDir+"/EnergyFits/FitPars_")+(long)i+"_Global.txt"))continue;
    if(inputpars.GetFitPar("Range","status")!=0){cout<<"did not converge"<<endl;continue;}
//     if(inputpars.GetFitPar("Range","chisquare")/inputpars.GetFitPar("Range","ndf")>3){
//       cout<<"did not converge"<<endl;
//       continue;
//    }



    HYVsEnergy.SetBinContent(i+1,inputpars.GetFitPar("D2460","yield"));
    HYVsEnergy.SetBinError(i+1,inputpars.GetFitPar("D2460","yielderror"));

    HY2VsEnergy.SetBinContent(i+1,inputpars.GetFitPar("D2400","yield"));
    HY2VsEnergy.SetBinError(i+1,inputpars.GetFitPar("D2400","yielderror"));

    HY3VsEnergy.SetBinContent(i+1,inputpars.GetFitPar("D2700","yield"));
    HY3VsEnergy.SetBinError(i+1,inputpars.GetFitPar("D2700","yielderror"));

    HY4VsEnergy.SetBinContent(i+1,inputpars.GetFitPar("D2600","yield"));
    HY4VsEnergy.SetBinError(i+1,inputpars.GetFitPar("D2600","yielderror"));

    HBVsEnergy.SetBinContent(i+1,inputpars.GetFitPar("Background","yield"));
    //HBVsEnergy.SetBinError(i+1,inputpars.GetFitPar("Background","yielderror"));

    HChisqVsEnergy.SetBinContent(i+1,inputpars.GetFitPar("Range","chisquare")/inputpars.GetFitPar("Range","ndf"));
         
  }



  Canvas.Clear();
  HYVsEnergy.GetYaxis()->SetRangeUser(0,1.1*HYVsEnergy.GetMaximum());
  HYVsEnergy.SetStats(0);
  HYVsEnergy.Draw("pe");
  Canvas.Print(filename);

  Canvas.Clear();
  HY2VsEnergy.GetYaxis()->SetRangeUser(0,1.1*HY2VsEnergy.GetMaximum());
  HY2VsEnergy.SetStats(0);
  HY2VsEnergy.Draw("pe");
  Canvas.Print(filename);

  Canvas.Clear();
  HY3VsEnergy.GetYaxis()->SetRangeUser(0,1.1*HY3VsEnergy.GetMaximum());
  HY3VsEnergy.SetStats(0);
  HY3VsEnergy.Draw("pe");
  Canvas.Print(filename);

  Canvas.Clear();
  HY4VsEnergy.GetYaxis()->SetRangeUser(0,1.1*HY4VsEnergy.GetMaximum());
  HY4VsEnergy.SetStats(0);
  HY4VsEnergy.Draw("pe");
  Canvas.Print(filename);

  Canvas.Clear();
  HBVsEnergy.SetStats(0);
  HBVsEnergy.Draw("p");
  Canvas.Print(filename);


  Canvas.Clear();
  HChisqVsEnergy.GetYaxis()->SetRangeUser(0,1.1*HChisqVsEnergy.GetMaximum());
  HChisqVsEnergy.SetStats(0);
  HChisqVsEnergy.Draw("p");
  Canvas.Print(filename);


  Canvas.Print(filename+"]");
   		
  file.Close();

  return 1;
}



Int_t DCPiAnalysis::FitMassVsEnergyD2600(){

  TFile file(_OutputDir+"/HistosForFit.root","read");
  gROOT->cd();
  
  TFile outfile(_OutputDir+"/EnergyFits/DCPiMassPlotsVsEnergyD2600.root","update");
  gROOT->cd();

		
  for(Int_t i=0;i<11;i++){  
    
    Bool_t fit=0;
    if(!inputpars.OpenFitPars(TString(_OutputDir+"/EnergyFits/FitPars_")+(long)i+"_D2600.txt"))
      fit=1;
    if(inputpars.GetFitPar("Range","status")!=0)
      fit=1;
    inputpars.CloseFitPars();

    if(fit){
      TH1F*HDCPiMass=(TH1F*)file.Get(TString("HDCPi_EnergyCoarse")+(long)i);
      if(!massfitter.Initialize(HDCPiMass,_OutputDir+"/EnergyFits/InitRooFitParsD2600.txt"))return 0;
      massfitter.Fit();
      massfitter.SetFileTag(TString("_")+(long)i+"_D2600");
      massfitter.Save(_OutputDir+"/EnergyFits");
      TString tag=massfitter.GetFileTag();

      outfile.cd();
      outfile.Delete(TString("fitplot")+(long)i+";1");
      RooPlot* fitplot=massfitter.GetFitPlot(); 
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

Int_t DCPiAnalysis::DrawMassVsEnergyD2600(){

  TFile file(_OutputDir+"/EnergyFits/DCPiMassPlotsVsEnergyD2600.root","read");
  file.ls();	
  gROOT->cd();


  filename=_OutputDir+"/EnergyFits/DCPiMassPlotsVsEnergyD2600.ps";
  Canvas.Print(filename+"["); 


  TH1F HYVsEnergy("HYVsEnergy","",11,3.4,5.6);
  SetHistoXYLabels(&HYVsEnergy,"E","GeV");
  TH1F HBVsEnergy("HBVsEnergy","",11,3.4,5.6);
  SetHistoXYLabels(&HBVsEnergy,"E","GeV");
  TH1F HChisqVsEnergy("HChisqVsEnergy","",11,3.4,5.6);
  SetHistoXYLabels(&HChisqVsEnergy,"E","GeV");
  
  for(Int_t i=0;i<11;i++){  
    RooPlot* fitplot=(RooPlot*)file.Get(TString("fitplot")+(long)i);
    if(!fitplot){cout<<"no plot"<<endl;continue;}

    Canvas.Clear();
    fitplot->Draw();
    Canvas.Print(filename);

    inputpars.CloseFitPars();
    if(!inputpars.OpenFitPars(TString(_OutputDir+"/EnergyFits/FitPars_")+(long)i+"_D2600.txt"))continue;
    //if(inputpars.GetFitPar("Range","status")!=0){cout<<"did not converge"<<endl;continue;}
    if(inputpars.GetFitPar("Range","chisquare")/inputpars.GetFitPar("Range","ndf")>3){
      cout<<"did not converge"<<endl;
      continue;
    }



    HYVsEnergy.SetBinContent(i+1,inputpars.GetFitPar("D2600","yield"));
    HYVsEnergy.SetBinError(i+1,inputpars.GetFitPar("D2600","yielderror"));
 
    HBVsEnergy.SetBinContent(i+1,inputpars.GetFitPar("Background","yield"));
    //HBVsEnergy.SetBinError(i+1,inputpars.GetFitPar("Background","yielderror"));

    HChisqVsEnergy.SetBinContent(i+1,inputpars.GetFitPar("Range","chisquare")/inputpars.GetFitPar("Range","ndf"));
         
  }



  Canvas.Clear();
  HYVsEnergy.GetYaxis()->SetRangeUser(0,1.1*HYVsEnergy.GetMaximum());
  HYVsEnergy.SetStats(0);
  HYVsEnergy.Draw("pe");
  Canvas.Print(filename);

  Canvas.Clear();
  HBVsEnergy.SetStats(0);
  HBVsEnergy.Draw("p");
  Canvas.Print(filename);


  Canvas.Clear();
  HChisqVsEnergy.GetYaxis()->SetRangeUser(0,1.1*HChisqVsEnergy.GetMaximum());
  HChisqVsEnergy.SetStats(0);
  HChisqVsEnergy.Draw("p");
  Canvas.Print(filename);


  Canvas.Print(filename+"]");
   		
  file.Close();

  return 1;
}
*/


Int_t DCPiAnalysis::FitMassVsEnergyFeed(){

  TFile file(_OutputDir+"/HistosForFit.root","read");
  gROOT->cd();
  
  TFile outfile(_OutputDir+"/EnergyFits/DCPiMassPlotsVsEnergyFeed.root","update");
  gROOT->cd();

		
  for(Int_t i=0;i<11;i++){  
    
    Bool_t fit=0;
    if(!inputpars.OpenFitPars(TString(_OutputDir+"/EnergyFits/FitPars_")+(long)i+"_Feed.txt"))
      fit=1;
    if(inputpars.GetFitPar("Range","status")!=0)
      fit=1;
    inputpars.CloseFitPars();

    if(fit){
      TH1F*HDCPiMass=(TH1F*)file.Get(TString("HDCPi_EnergyCoarse")+(long)i);
      if(!massfitter.Initialize(HDCPiMass,_OutputDir+"/EnergyFits/InitRooFitParsFeed.txt"))return 0;
      massfitter.Fit();
      massfitter.SetFileTag(TString("_")+(long)i+"_Feed");
      massfitter.Save(_OutputDir+"/EnergyFits");
      TString tag=massfitter.GetFileTag();

      outfile.cd();
      outfile.Delete(TString("fitplot")+(long)i+";1");
      RooPlot* fitplot=massfitter.GetFitPlot(); 
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

Int_t DCPiAnalysis::DrawMassVsEnergyFeed(){

  TFile file(_OutputDir+"/EnergyFits/DCPiMassPlotsVsEnergyFeed.root","read");
  file.ls();	
  gROOT->cd();


  filename=_OutputDir+"/EnergyFits/DCPiMassPlotsVsEnergyFeed.ps";
  Canvas.Print(filename+"["); 


  TH1F HYVsEnergy("HYVsEnergy","",11,3.4,5.6);
  SetHistoXYLabels(&HYVsEnergy,"E","GeV");
  TH1F HBVsEnergy("HBVsEnergy","",11,3.4,5.6);
  SetHistoXYLabels(&HBVsEnergy,"E","GeV");
  TH1F HChisqVsEnergy("HChisqVsEnergy","",11,3.4,5.6);
  SetHistoXYLabels(&HChisqVsEnergy,"E","GeV");
  
  for(Int_t i=0;i<11;i++){  
    RooPlot* fitplot=(RooPlot*)file.Get(TString("fitplot")+(long)i);
    if(!fitplot){cout<<"no plot"<<endl;continue;}

    Canvas.Clear();
    fitplot->Draw();
    Canvas.Print(filename);

    inputpars.CloseFitPars();
    if(!inputpars.OpenFitPars(TString(_OutputDir+"/EnergyFits/FitPars_")+(long)i+"_Feed.txt"))continue;
    //if(inputpars.GetFitPar("Range","status")!=0){cout<<"did not converge"<<endl;continue;}
    if(inputpars.GetFitPar("Range","chisquare")/inputpars.GetFitPar("Range","ndf")>3){
      cout<<"did not converge"<<endl;
      continue;
    }


    HYVsEnergy.SetBinContent(i+1,inputpars.GetFitPar("F1","yield"));
    HYVsEnergy.SetBinError(i+1,inputpars.GetFitPar("F1","yielderror"));
 
    HBVsEnergy.SetBinContent(i+1,inputpars.GetFitPar("Background","yield"));
    //HBVsEnergy.SetBinError(i+1,inputpars.GetFitPar("Background","yielderror"));

    HChisqVsEnergy.SetBinContent(i+1,inputpars.GetFitPar("Range","chisquare")/inputpars.GetFitPar("Range","ndf"));
         
  }


  Canvas.Clear();
  HYVsEnergy.GetYaxis()->SetRangeUser(0,1.1*HYVsEnergy.GetMaximum());
  HYVsEnergy.SetStats(0);
  HYVsEnergy.Draw("pe");
  Canvas.Print(filename);

  Canvas.Clear();
  HBVsEnergy.SetStats(0);
  HBVsEnergy.Draw("p");
  Canvas.Print(filename);

  Canvas.Clear();
  HChisqVsEnergy.GetYaxis()->SetRangeUser(0,1.1*HChisqVsEnergy.GetMaximum());
  HChisqVsEnergy.SetStats(0);
  HChisqVsEnergy.Draw("p");
  Canvas.Print(filename);

  Canvas.Print(filename+"]");
   		
  file.Close();

  return 1;
}


