
Int_t DCPiAnalysis::FitMassVsCos(){

  TFile file(_OutputDir+"/HistosForFit.root","read");
  gROOT->cd();
  
  TFile outfile(_OutputDir+"/CosFits/DCPiMassPlotsVsCos.root","update");
  gROOT->cd();


		
  for(Int_t i=0;i<20;i++){  
    
    Bool_t fit=0;
    inputpars.CloseFitPars();
    if(!inputpars.OpenFitPars(TString(_OutputDir+"/CosFits/FitPars_")+(long)i+".txt"))
      fit=1;
    else if(inputpars.GetFitPar("Range","status")!=0)
      fit=1;


    if(fit){
      TH1F*HDCPiMass=(TH1F*)file.Get(TString("HDCPi_Cos")+(long)i);
      if(!massfitter.Initialize(HDCPiMass,_OutputDir+"/CosFits/InitRooFitPars.txt"))return 0;
      massfitter.Fit();
      massfitter.SetFileTag(TString("_")+(long)i);
      massfitter.Save(_OutputDir+"/CosFits");
      TString tag=massfitter.GetFileTag();


      RooPlot* fitplot=massfitter.GetFitPlot(); 
      fitplot->SetName(TString("fitplot")+(long)i);
      outfile.Delete(TString("fitplot")+(long)i+";1");
      outfile.cd();
      fitplot->Write();
      gROOT->cd();
      delete fitplot;
      cout<<"Done with fit "<<i<<endl;
    }
  }
		
  outfile.ls();	
  outfile.Close();
  file.Close();

  return 1;
}

Int_t DCPiAnalysis::DrawMassVsCos(){

  TFile file(_OutputDir+"/CosFits/DCPiMassPlotsVsCos.root","read");
  file.ls();	
  gROOT->cd();


  filename=_OutputDir+"/CosFits/DCPiMassPlotsVsCos.ps";
  Canvas.Print(filename+"["); 


  TH1F HYVsCos("HYVsCos","",20,-1,1);
  SetHistoXYLabels(&HYVsCos,"cos(#theta*)","");
  TH1F HMVsCos("HMVsCos","",20,-1,1);
  SetHistoXYLabels(&HMVsCos,"cos(#theta*)","");
  TH1F HWVsCos("HWVsCos","",20,-1,1);
  SetHistoXYLabels(&HWVsCos,"cos(#theta*)","");
  TH1F HChisqVsCos("HChisqVsCos","",20,-1,1);
  SetHistoXYLabels(&HChisqVsCos,"cos(#theta*)","");
  
  Canvas.Clear();
  Canvas.Divide(4,5);
  text.SetTextSize(.2);
  for(Int_t i=0;i<20;i++){  
    
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
    text.DrawTextNDC(.15,.3,TString("")+(long)(i+1));
   

    inputpars.CloseFitPars();
    if(!inputpars.OpenFitPars(TString(_OutputDir+"/CosFits/FitPars_")+(long)i+".txt"))continue;
    if(inputpars.GetFitPar("Range","status")!=0){cout<<"did not converge"<<endl;continue;}


    HYVsCos.SetBinContent(i+1,inputpars.GetFitPar("D2460","yield"));
    HYVsCos.SetBinError(i+1,inputpars.GetFitPar("D2460","yielderror"));

    HMVsCos.SetBinContent(i+1,inputpars.GetFitPar("D2460","mean")*1000);
    HMVsCos.SetBinError(i+1,inputpars.GetFitPar("D2460","meanerror")*1000);

    HWVsCos.SetBinContent(i+1,inputpars.GetFitPar("D2460","width")*1000);
    HWVsCos.SetBinError(i+1,inputpars.GetFitPar("D2460","widtherror")*1000);


    HChisqVsCos.SetBinContent(i+1,inputpars.GetFitPar("Range","chisquare")/inputpars.GetFitPar("Range","ndf"));
    inputpars.CloseFitPars();     
  }
  Canvas.Print(filename);


  Canvas.Clear();
  HYVsCos.GetYaxis()->SetRangeUser(0,1.1*HYVsCos.GetMaximum());
  HYVsCos.SetStats(0);
  HYVsCos.Draw("pe");
  Canvas.Print(filename);

  Canvas.Clear();
  HMVsCos.SetStats(0);
  if(_MCorDATA==1)HMVsCos.GetYaxis()->SetRangeUser(2455,2465);
  if(_MCorDATA==0)HMVsCos.GetYaxis()->SetRangeUser(2450,2470);
  HMVsCos.Draw("pe");
  Canvas.Print(filename);

  Canvas.Clear();
  HWVsCos.SetStats(0);
  if(_MCorDATA==1)HWVsCos.GetYaxis()->SetRangeUser(40,60);
  if(_MCorDATA==0)HWVsCos.GetYaxis()->SetRangeUser(20,40);
  HWVsCos.Draw("pe");
  Canvas.Print(filename);

  Canvas.Clear();
  HChisqVsCos.GetYaxis()->SetRangeUser(0,1.1*HChisqVsCos.GetMaximum());
  HChisqVsCos.SetStats(0);
  HChisqVsCos.Draw("p");
  Canvas.Print(filename);


  Canvas.Print(filename+"]");
   		
  file.Close();

  return 1;
}



// Int_t DCPiAnalysis::FitMassVsCosD2700(){

//   TFile file(_OutputDir+"/HistosForFit.root","read");
//   gROOT->cd();
  
//   TFile outfile(_OutputDir+"/CosFits/DCPiMassPlotsVsCosD2700.root","update");
//   gROOT->cd();


		
//   for(Int_t i=0;i<10;i++){  
//     Bool_t fit=0;
//     if(!inputpars.OpenFitPars(TString(_OutputDir+"/CosFits/FitPars_")+(long)i+"_D2700.txt"))
//       fit=1;
//     if(inputpars.GetFitPar("Range","status")!=0)
//       fit=1;
//     inputpars.CloseFitPars();

//     if(fit){
//       TH1F*HDCPiMass=(TH1F*)file.Get(TString("HDCPi_CosCoarse")+(long)i);
//       if(!massfitter.Initialize(HDCPiMass,_OutputDir+"/CosFits/InitRooFitParsD2700.txt"))return 0;
//       massfitter.Fit();
//       massfitter.SetFileTag(TString("_")+(long)i+"_D2700");
//       massfitter.Save(_OutputDir+"/CosFits");
//       TString tag=massfitter.GetFileTag();
      
//       outfile.cd();
//       outfile.Delete(TString("fitplot")+(long)i+";1");
//       RooPlot* fitplot=massfitter.GetFitPlot(); 
//       fitplot->SetName(TString("fitplot")+(long)i);
//       fitplot->Write();
//       delete fitplot;
//       gROOT->cd();
//     }
//   }
		
//   outfile.ls();	
//   outfile.Close();
//   file.Close();

//   return 1;
// }

// Int_t DCPiAnalysis::DrawMassVsCosD2700(){

//   TFile file(_OutputDir+"/CosFits/DCPiMassPlotsVsCosD2700.root","read");
//   file.ls();	
//   gROOT->cd();


//   filename=_OutputDir+"/CosFits/DCPiMassPlotsVsCosD2700.ps";
//   Canvas.Print(filename+"["); 

//   TH1F HYVsCos("HYVsCos","",10,-1,1);
//   SetHistoXYLabels(&HYVsCos,"cos(#theta*)","");
//   TH1F HMVsCos("HMVsCos","",10,-1,1);
//   SetHistoXYLabels(&HMVsCos,"cos(#theta*)","");
//   TH1F HWVsCos("HWVsCos","",10,-1,1);
//   SetHistoXYLabels(&HWVsCos,"cos(#theta*)","");
//   TH1F HBVsCos("HBVsCos","",10,-1,1);
//   SetHistoXYLabels(&HBVsCos,"cos(#theta*)","");
  
//   for(Int_t i=0;i<10;i++){  
//     RooPlot* fitplot=(RooPlot*)file.Get(TString("fitplot")+(long)i);
//     if(!fitplot){cout<<"no plot"<<endl;continue;}
//     Canvas.Clear();
//     fitplot->Draw();
//     Canvas.Print(filename);

//     inputpars.CloseFitPars();
//     if(!inputpars.OpenFitPars(TString(_OutputDir+"/CosFits/FitPars_")+(long)i+"_D2700.txt"))continue;
//     //if(inputpars.GetFitPar("Range","status")!=0){cout<<"did not converge"<<endl;continue;}
//     if(inputpars.GetFitPar("Range","chisquare")/inputpars.GetFitPar("Range","ndf")>4){
//       cout<<"did not converge"<<endl;
//       continue;
//     }  
 
   
//     HYVsCos.SetBinContent(i+1,inputpars.GetFitPar("D2700","yield"));
//     HYVsCos.SetBinError(i+1,inputpars.GetFitPar("D2700","yielderror"));

//     HMVsCos.SetBinContent(i+1,inputpars.GetFitPar("D2700","mean")*1000);
//     HMVsCos.SetBinError(i+1,inputpars.GetFitPar("D2700","meanerror")*1000);

//     HWVsCos.SetBinContent(i+1,inputpars.GetFitPar("D2700","width")*1000);
//     HWVsCos.SetBinError(i+1,inputpars.GetFitPar("D2700","widtherror")*1000);

//     HBVsCos.SetBinContent(i+1,inputpars.GetFitPar("Background","yield"));
//     //HBVsCos.SetBinError(i+1,inputpars.GetFitPar("Background","yielderror"));

      


//   }



//   Canvas.Clear();
//   HYVsCos.SetStats(0);
//   HYVsCos.Draw("pe");
//   Canvas.Print(filename);

//   Canvas.Clear();
//   HMVsCos.SetStats(0);
//   HMVsCos.GetYaxis()->SetRangeUser(2730,2790);
//   HMVsCos.Draw("pe");
//   Canvas.Print(filename);

//   Canvas.Clear();
//   HWVsCos.SetStats(0);
//   HWVsCos.GetYaxis()->SetRangeUser(30,90);
//   HWVsCos.Draw("pe");
//   Canvas.Print(filename);

//   Canvas.Clear();
//   HBVsCos.SetStats(0);
//   HBVsCos.Draw("pe");
//   Canvas.Print(filename);


//   Canvas.Print(filename+"]");
   		
//   file.Close();

//   return 1;
// }


// Int_t DCPiAnalysis::FitMassVsCosD2600(){

//   TFile file(_OutputDir+"/HistosForFit.root","read");
//   gROOT->cd();
  
//   TFile outfile(_OutputDir+"/CosFits/DCPiMassPlotsVsCosD2600.root","update");
//   gROOT->cd();

		
//   for(Int_t i=0;i<10;i++){  
    
//     Bool_t fit=0;
//     if(!inputpars.OpenFitPars(TString(_OutputDir+"/CosFits/FitPars_")+(long)i+"_D2600.txt"))
//       fit=1;
//     if(inputpars.GetFitPar("Range","status")!=0)
//       fit=1;
//     inputpars.CloseFitPars();

//     if(fit){
//       TH1F*HDCPiMass=(TH1F*)file.Get(TString("HDCPi_CosCoarse")+(long)i);
//       if(!massfitter.Initialize(HDCPiMass,_OutputDir+"/CosFits/InitRooFitParsD2600.txt"))return 0;
//       massfitter.Fit();
//       massfitter.SetFileTag(TString("_")+(long)i+"_D2600");
//       massfitter.Save(_OutputDir+"/CosFits");
//       TString tag=massfitter.GetFileTag();

//       outfile.cd();
//       outfile.Delete(TString("fitplot")+(long)i+";1");
//       RooPlot* fitplot=massfitter.GetFitPlot(); 
//       fitplot->SetName(TString("fitplot")+(long)i);
//       fitplot->Write();
//       delete fitplot;
//       gROOT->cd();

//       cout<<"Done with fit "<<i<<endl;
//     }
//   }
		
//   outfile.ls();	
//   outfile.Close();
//   file.Close();

//   return 1;
// }

// Int_t DCPiAnalysis::DrawMassVsCosD2600(){

//   TFile file(_OutputDir+"/CosFits/DCPiMassPlotsVsCosD2600.root","read");
//   file.ls();	
//   gROOT->cd();


//   filename=_OutputDir+"/CosFits/DCPiMassPlotsVsCosD2600.ps";
//   Canvas.Print(filename+"["); 


//   TH1F HYVsCos("HYVsCos","",10,-1,1);
//   SetHistoXYLabels(&HYVsCos,"cos(#theta*)","");
//   TH1F HBVsCos("HBVsCos","",10,-1,1);
//   SetHistoXYLabels(&HBVsCos,"cos(#theta*)","");
//   TH1F HChisqVsCos("HChisqVsCos","",10,-1,1);
//   SetHistoXYLabels(&HChisqVsCos,"cos(#theta*)","");
  
//   for(Int_t i=0;i<10;i++){  
//     RooPlot* fitplot=(RooPlot*)file.Get(TString("fitplot")+(long)i);
//     if(!fitplot){cout<<"no plot"<<endl;continue;}

//     Canvas.Clear();
//     fitplot->Draw();
//     Canvas.Print(filename);

//     inputpars.CloseFitPars();
//     if(!inputpars.OpenFitPars(TString(_OutputDir+"/CosFits/FitPars_")+(long)i+"_D2600.txt"))continue;
//     //if(inputpars.GetFitPar("Range","status")!=0){cout<<"did not converge"<<endl;continue;}
//     if(inputpars.GetFitPar("Range","chisquare")/inputpars.GetFitPar("Range","ndf")>3){
//       cout<<"did not converge"<<endl;
//       continue;
//     }



//     HYVsCos.SetBinContent(i+1,inputpars.GetFitPar("D2600","yield"));
//     HYVsCos.SetBinError(i+1,inputpars.GetFitPar("D2600","yielderror"));
 
//     HBVsCos.SetBinContent(i+1,inputpars.GetFitPar("Background","yield"));
//     //HBVsCos.SetBinError(i+1,inputpars.GetFitPar("Background","yielderror"));

//     HChisqVsCos.SetBinContent(i+1,inputpars.GetFitPar("Range","chisquare")/inputpars.GetFitPar("Range","ndf"));
         
//   }



//   Canvas.Clear();
//   HYVsCos.GetYaxis()->SetRangeUser(0,1.1*HYVsCos.GetMaximum());
//   HYVsCos.SetStats(0);
//   HYVsCos.Draw("pe");
//   Canvas.Print(filename);

//   Canvas.Clear();
//   HBVsCos.SetStats(0);
//   HBVsCos.Draw("p");
//   Canvas.Print(filename);


//   Canvas.Clear();
//   HChisqVsCos.GetYaxis()->SetRangeUser(0,1.1*HChisqVsCos.GetMaximum());
//   HChisqVsCos.SetStats(0);
//   HChisqVsCos.Draw("p");
//   Canvas.Print(filename);


//   Canvas.Print(filename+"]");
   		
//   file.Close();

//   return 1;
// }



// Int_t DCPiAnalysis::FitMassVsCosFeed(){

//   TFile file(_OutputDir+"/HistosForFit.root","read");
//   gROOT->cd();
  
//   TFile outfile(_OutputDir+"/CosFits/DCPiMassPlotsVsCosFeed.root","update");
//   gROOT->cd();

		
//   for(Int_t i=0;i<10;i++){  
    
//     Bool_t fit=0;
//     if(!inputpars.OpenFitPars(TString(_OutputDir+"/CosFits/FitPars_")+(long)i+"_Feed.txt"))
//       fit=1;
//     if(inputpars.GetFitPar("Range","status")!=0)
//       fit=1;
//     inputpars.CloseFitPars();

//     if(fit){
//       TH1F*HDCPiMass=(TH1F*)file.Get(TString("HDCPi_CosCoarse")+(long)i);
//       if(!massfitter.Initialize(HDCPiMass,_OutputDir+"/CosFits/InitRooFitParsFeed.txt"))return 0;
//       massfitter.Fit();
//       massfitter.SetFileTag(TString("_")+(long)i+"_Feed");
//       massfitter.Save(_OutputDir+"/CosFits");
//       TString tag=massfitter.GetFileTag();

//       outfile.cd();
//       outfile.Delete(TString("fitplot")+(long)i+";1");
//       RooPlot* fitplot=massfitter.GetFitPlot(); 
//       fitplot->SetName(TString("fitplot")+(long)i);
//       fitplot->Write();
//       delete fitplot;
//       gROOT->cd();

//       cout<<"Done with fit "<<i<<endl;
//     }
//   }
		
//   outfile.ls();	
//   outfile.Close();
//   file.Close();

//   return 1;
// }

// Int_t DCPiAnalysis::DrawMassVsCosFeed(){

//   TFile file(_OutputDir+"/CosFits/DCPiMassPlotsVsCosFeed.root","read");
//   file.ls();	
//   gROOT->cd();


//   filename=_OutputDir+"/CosFits/DCPiMassPlotsVsCosFeed.ps";
//   Canvas.Print(filename+"["); 


//   TH1F HYVsCos("HYVsCos","",10,-1,1);
//   SetHistoXYLabels(&HYVsCos,"cos(#theta*)","");
//   TH1F HBVsCos("HBVsCos","",10,-1,1);
//   SetHistoXYLabels(&HBVsCos,"cos(#theta*)","");
//   TH1F HChisqVsCos("HChisqVsCos","",10,-1,1);
//   SetHistoXYLabels(&HChisqVsCos,"cos(#theta*)","");
  
//   for(Int_t i=0;i<10;i++){  
//     RooPlot* fitplot=(RooPlot*)file.Get(TString("fitplot")+(long)i);
//     if(!fitplot){cout<<"no plot"<<endl;continue;}

//     Canvas.Clear();
//     fitplot->Draw();
//     Canvas.Print(filename);

//     inputpars.CloseFitPars();
//     if(!inputpars.OpenFitPars(TString(_OutputDir+"/CosFits/FitPars_")+(long)i+"_Feed.txt"))continue;
//     //if(inputpars.GetFitPar("Range","status")!=0){cout<<"did not converge"<<endl;continue;}
//     if(inputpars.GetFitPar("Range","chisquare")/inputpars.GetFitPar("Range","ndf")>3){
//       cout<<"did not converge"<<endl;
//       continue;
//     }



//     HYVsCos.SetBinContent(i+1,inputpars.GetFitPar("F1","yield"));
//     HYVsCos.SetBinError(i+1,inputpars.GetFitPar("F1","yielderror"));
 
//     HBVsCos.SetBinContent(i+1,inputpars.GetFitPar("Background","yield"));
//     //HBVsCos.SetBinError(i+1,inputpars.GetFitPar("Background","yielderror"));

//     HChisqVsCos.SetBinContent(i+1,inputpars.GetFitPar("Range","chisquare")/inputpars.GetFitPar("Range","ndf"));
         
//   }


//   Canvas.Clear();
//   HYVsCos.GetYaxis()->SetRangeUser(0,1.1*HYVsCos.GetMaximum());
//   HYVsCos.SetStats(0);
//   HYVsCos.Draw("pe");
//   Canvas.Print(filename);

//   Canvas.Clear();
//   HBVsCos.SetStats(0);
//   HBVsCos.Draw("p");
//   Canvas.Print(filename);

//   Canvas.Clear();
//   HChisqVsCos.GetYaxis()->SetRangeUser(0,1.1*HChisqVsCos.GetMaximum());
//   HChisqVsCos.SetStats(0);
//   HChisqVsCos.Draw("p");
//   Canvas.Print(filename);

//   Canvas.Print(filename+"]");
   		
//   file.Close();

//   return 1;
// }

/*
Int_t DCPiAnalysis::FitMassVsCosGlobal(){

  TFile file(_OutputDir+"/HistosForFit.root","read");
  gROOT->cd();
  
  TFile outfile(_OutputDir+"/CosFits/DCPiMassPlotsVsCosGlobal.root","update");
  gROOT->cd();

		
  for(Int_t i=0;i<10;i++){  
    
    Bool_t fit=0;
    if(!inputpars.OpenFitPars(TString(_OutputDir+"/CosFits/FitPars_")+(long)i+"_Global.txt"))
      fit=1;
    if(inputpars.GetFitPar("Range","status")!=0)
      fit=1;
    inputpars.CloseFitPars();

    if(fit){
      TH1F*HDCPiMass=(TH1F*)file.Get(TString("HDCPi_CosCoarse")+(long)i);
      if(!massfitter.Initialize(HDCPiMass,_OutputDir+"/CosFits/InitRooFitParsGlobal.txt"))return 0;
      massfitter.Fit();
      massfitter.SetFileTag(TString("_")+(long)i+"_Global");
      massfitter.Save(_OutputDir+"/CosFits");
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

Int_t DCPiAnalysis::DrawMassVsCosGlobal(){

  TFile file(_OutputDir+"/CosFits/DCPiMassPlotsVsCosGlobal.root","read");
  file.ls();	
  gROOT->cd();


  filename=_OutputDir+"/CosFits/DCPiMassPlotsVsCosGlobal.ps";
  Canvas.Print(filename+"["); 


  TH1F HYVsCos("HYVsCos","",10,-1,1);
  SetHistoXYLabels(&HYVsCos,"cos(#theta*)","");
  TH1F HY2VsCos("HY2VsCos","",10,-1,1);
  SetHistoXYLabels(&HY2VsCos,"cos(#theta*)","");
  TH1F HY3VsCos("HY3VsCos","",10,-1,1);
  SetHistoXYLabels(&HY3VsCos,"cos(#theta*)","");
  TH1F HY4VsCos("HY4VsCos","",10,-1,1);
  SetHistoXYLabels(&HY4VsCos,"cos(#theta*)","");
  TH1F HBVsCos("HBVsCos","",10,-1,1);
  SetHistoXYLabels(&HBVsCos,"cos(#theta*)","");
  TH1F HChisqVsCos("HChisqVsCos","",10,-1,1);
  SetHistoXYLabels(&HChisqVsCos,"cos(#theta*)","");
  
  for(Int_t i=0;i<10;i++){  
    RooPlot* fitplot=(RooPlot*)file.Get(TString("fitplot")+(long)i);
    if(!fitplot){cout<<"no plot"<<endl;continue;}

    Canvas.Clear();
    fitplot->Draw();
    Canvas.Print(filename);

    inputpars.CloseFitPars();
    if(!inputpars.OpenFitPars(TString(_OutputDir+"/CosFits/FitPars_")+(long)i+"_Global.txt"))continue;
    if(inputpars.GetFitPar("Range","status")!=0){cout<<"did not converge"<<endl;continue;}
//     if(inputpars.GetFitPar("Range","chisquare")/inputpars.GetFitPar("Range","ndf")>3){
//       cout<<"did not converge"<<endl;
//       continue;
//    }



    HYVsCos.SetBinContent(i+1,inputpars.GetFitPar("D2460","yield"));
    HYVsCos.SetBinError(i+1,inputpars.GetFitPar("D2460","yielderror"));

    HY2VsCos.SetBinContent(i+1,inputpars.GetFitPar("D2400","yield"));
    HY2VsCos.SetBinError(i+1,inputpars.GetFitPar("D2400","yielderror"));

    HY3VsCos.SetBinContent(i+1,inputpars.GetFitPar("D2700","yield"));
    HY3VsCos.SetBinError(i+1,inputpars.GetFitPar("D2700","yielderror"));

    HY4VsCos.SetBinContent(i+1,inputpars.GetFitPar("D2600","yield"));
    HY4VsCos.SetBinError(i+1,inputpars.GetFitPar("D2600","yielderror"));

    HBVsCos.SetBinContent(i+1,inputpars.GetFitPar("Background","yield"));
    //HBVsCos.SetBinError(i+1,inputpars.GetFitPar("Background","yielderror"));

    HChisqVsCos.SetBinContent(i+1,inputpars.GetFitPar("Range","chisquare")/inputpars.GetFitPar("Range","ndf"));
         
  }



  Canvas.Clear();
  HYVsCos.GetYaxis()->SetRangeUser(0,1.1*HYVsCos.GetMaximum());
  HYVsCos.SetStats(0);
  HYVsCos.Draw("pe");
  Canvas.Print(filename);

  Canvas.Clear();
  HY2VsCos.GetYaxis()->SetRangeUser(0,1.1*HY2VsCos.GetMaximum());
  HY2VsCos.SetStats(0);
  HY2VsCos.Draw("pe");
  Canvas.Print(filename);

  Canvas.Clear();
  HY3VsCos.GetYaxis()->SetRangeUser(0,1.1*HY3VsCos.GetMaximum());
  HY3VsCos.SetStats(0);
  HY3VsCos.Draw("pe");
  Canvas.Print(filename);

  Canvas.Clear();
  HY4VsCos.GetYaxis()->SetRangeUser(0,1.1*HY4VsCos.GetMaximum());
  HY4VsCos.SetStats(0);
  HY4VsCos.Draw("pe");
  Canvas.Print(filename);

  Canvas.Clear();
  HBVsCos.SetStats(0);
  HBVsCos.Draw("p");
  Canvas.Print(filename);


  Canvas.Clear();
  HChisqVsCos.GetYaxis()->SetRangeUser(0,1.1*HChisqVsCos.GetMaximum());
  HChisqVsCos.SetStats(0);
  HChisqVsCos.Draw("p");
  Canvas.Print(filename);


  Canvas.Print(filename+"]");
   		
  file.Close();

  return 1;
}




*/
