

Int_t DCPiAnalysis::MakePstar(){

  if(!OpenReducedNtuple())return 0;

  if(!topdatabase){cout<<" no database "<<endl; return 0;}

  TFile file(_OutputDir+"/PstarPlot.root","recreate");
  

  TH1F HDCPiMass("HDCPiMass","",2000,1.9,3.9);
  ReducedNtuple->Draw("xmdm>>HDCPiMass");
  HDCPiMass.Write();  
  
  TH1F HPstar("HPstar","",40,3.,5.0); 
  ReducedNtuple->Draw("xpstar>>HPstar",topdatabase->GetCut("xmdm"));
  HPstar.Write();

  TH1F HPstarLSB("HPstarLSB","",40,3.,5.0);
  ReducedNtuple->Draw("xpstar>>HPstarLSB",topdatabase->GetCut("xmdmsb")+TCut("xmdm<2.46"));
  HPstarLSB.Write();

  TH1F HPstarHSB("HPstarHSB","",40,3.,5.0);
  ReducedNtuple->Draw("xpstar>>HPstarHSB",topdatabase->GetCut("xmdmsb")+TCut("xmdm>2.46"));
  HPstarHSB.Write();


  file.ls();
  file.Close();

  CloseReducedNtuple();

  return 1;
}

Int_t DCPiAnalysis::PlotPstar(){


  TFile file(_OutputDir+"/PstarPlot.root","read");
  gROOT->cd();

  filename=_OutputDir+"/PstarPlots.ps";
  Canvas.Print(filename+"["); 


//   //get the bkg under the signal
//   TH1F* HDCPiMass=(TH1F*)file.Get("HDCPiMass");
//   if(!massfitter.Initialize(HDCPiMass,_OutputDir+"/InitRooFitPars.txt"))return 0;
//   if(massfitter.Fit()!=0)return 0;
//   RooAbsPdf*bkg=massfitter.GetBkgPdf();
//   mass.setRange("sigregion",((DataBaseDCPi*)topdatabase)->GetDCPiMassMin(),((DataBaseDCPi*)topdatabase)->GetDCPiMassMax());
//   RooAbsReal* integral= bkg->createIntegral(mass,"sigregion");
//   cout<<",  undersig="<<integral->getVal()<<endl;
//   RooPlot*fitplot=massfitter.GetFitPlot();
//   Canvas.Clear();
//   fitplot->Draw();
//   TH1F*HMassR=((RooHist*)fitplot->getObject(0))->GetHistogram();
//   ShowDCPiMassRegions(HMassR);
//   Canvas.Print(filename);
//   ///

  TH1F*HPstar=(TH1F*)file.Get("HPstar");
  Canvas.Clear();
  HPstar->Draw("pe");
  Canvas.Print(filename);



  TH1F*HPstarLSB=(TH1F*)file.Get("HPstarLSB");
  TH1F*HPstarHSB=(TH1F*)file.Get("HPstarHSB");
  Canvas.Clear();
  HPstarLSB->Draw("pe");
  HPstarHSB->Draw("pesame");
  Canvas.Print(filename);


//   TH1F*HPstarSBAvg=(TH1F*)HPstarHSB->Clone();
//   HPstarSBAvg->SetName("HPstarSBAvg");
//   HPstarSBAvg->Add(HPstarLSB);
//   HPstarSBAvg->Scale(integral->getVal()/HPstarSBAvg->Integral());
//   Canvas.Clear();
//   HPstarSBAvg->Draw("pe");
//   Canvas.Print(filename);
//   file.cd();
//   HPstarSBAvg->Write(); 

//   TH1F*HPstarSignal=(TH1F*)HPstar->Clone();
//   HPstarSignal->SetName("HPstarSignal");
//   HPstarSignal->Add(HPstarSBAvg,-1);
//   Canvas.Clear();
//   HPstar->Draw("pe");
//   HPstarSBAvg->Draw("pesame");
//   HPstarSignal->Draw("pesame");
//   Canvas.Print(filename);

  
//   file.cd();
//   HPstarSignal->Write();
  

  file.Close();



  Canvas.Print(filename+"]");


  
  return 1;
}


Int_t DCPiAnalysis::FitMassVsPstar(){

  TFile file(_OutputDir+"/HistosForFit.root","read");
  gROOT->cd();
  
  TFile outfile(_OutputDir+"/PstarFits/DCPiMassPlotsVsPstar.root","update");
  gROOT->cd();


		
  for(Int_t i=0;i<16;i++){  
    
    Bool_t fit=0;
    if(!inputpars.OpenFitPars(TString(_OutputDir+"/PstarFits/FitPars_")+(long)i+".txt"))
      fit=1;
    if(inputpars.GetFitPar("Range","status")!=0)
      fit=1;
    inputpars.CloseFitPars();

    if(fit){
      TH1F*HDCPiMass=(TH1F*)file.Get(TString("HDCPi_Pstar")+(long)i);
      if(!HDCPiMass)continue;
      if(!massfitter.Initialize(HDCPiMass,_OutputDir+"/PstarFits/InitRooFitPars.txt"))continue;
      massfitter.Fit();
      massfitter.SetFileTag(TString("_")+(long)i);
      massfitter.Save(_OutputDir+"/PstarFits");
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

Int_t DCPiAnalysis::DrawMassVsPstar(){

  TFile file(_OutputDir+"/PstarFits/DCPiMassPlotsVsPstar.root","read");
  file.ls();	
  gROOT->cd();


  filename=_OutputDir+"/PstarFits/DCPiMassPlotsVsPstar.ps";
  Canvas.Print(filename+"["); 


  TH1F HYVsPstar("HYVsPstar","",(int)((5.0-databaseDCPi.GetDCPiPstarCut())/.1 + .5),databaseDCPi.GetDCPiPstarCut(),5.0);
  SetHistoXYLabels(&HYVsPstar,"p*","GeV/c");
  TH1F HMVsPstar("HMVsPstar","",(int)((5.0-databaseDCPi.GetDCPiPstarCut())/.1 + .5),databaseDCPi.GetDCPiPstarCut(),5.0);
  SetHistoXYLabels(&HMVsPstar,"p*","GeV/c");
  TH1F HWVsPstar("HWVsPstar","",(int)((5.0-databaseDCPi.GetDCPiPstarCut())/.1 + .5),databaseDCPi.GetDCPiPstarCut(),5.0);
  SetHistoXYLabels(&HWVsPstar,"p*","GeV/c");
//   TH1F HBVsPstar("HBVsPstar","",(int)((5.0-databaseDCPi.GetDCPiPstarCut())/.1 + .5),databaseDCPi.GetDCPiPstarCut(),5.0);
//   SetHistoXYLabels(&HBVsPstar,"p*","GeV/c");
  TH1F HChisqVsPstar("HChisqVsPstar","",(int)((5.0-databaseDCPi.GetDCPiPstarCut())/.1 + .5),databaseDCPi.GetDCPiPstarCut(),5.0);
  SetHistoXYLabels(&HChisqVsPstar,"p*","GeV/c");
  
  Canvas.Clear();
  Canvas.Divide(4,4);
  text.SetTextSize(.2);
  for(Int_t i=0;i<16;i++){//skip last point

    
    RooPlot* fitplot=(RooPlot*)file.Get(TString("fitplot")+(long)i);
    if(!fitplot){cout<<"no plot"<<endl;continue;}
    
    Canvas.cd(i+1);
    Canvas.GetPad(i+1)->SetTopMargin(.09);
    Canvas.GetPad(i+1)->SetBottomMargin(.2);
    Canvas.GetPad(i+1)->SetLeftMargin(.13);
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
    if(!inputpars.OpenFitPars(TString(_OutputDir+"/PstarFits/FitPars_")+(long)i+".txt"))continue;
    if(inputpars.GetFitPar("Range","status")!=0){cout<<"did not converge"<<endl;continue;}
    //if(inputpars.GetFitPar("Range","chisquare")/inputpars.GetFitPar("Range","ndf")>3){cout<<"did not converge"<<endl;continue;}
    
    //if(inputpars.GetFitPar("D2460","mean")<2.455||inputpars.GetFitPar("D2460","mean")>2.465)continue;//cut bad measurements


    HYVsPstar.SetBinContent(i+1,inputpars.GetFitPar("D2460","yield"));
    HYVsPstar.SetBinError(i+1,inputpars.GetFitPar("D2460","yielderror"));

    if(HYVsPstar.GetBinCenter(i+1)>4.5)continue;//remove badly measurede points
    
    HMVsPstar.SetBinContent(i+1,inputpars.GetFitPar("D2460","mean")*1000);
    HMVsPstar.SetBinError(i+1,inputpars.GetFitPar("D2460","meanerror")*1000);

    HWVsPstar.SetBinContent(i+1,inputpars.GetFitPar("D2460","width")*1000);
    HWVsPstar.SetBinError(i+1,inputpars.GetFitPar("D2460","widtherror")*1000);

    HChisqVsPstar.SetBinContent(i+1,inputpars.GetFitPar("Range","chisquare")/inputpars.GetFitPar("Range","ndf"));
         
  }

  Canvas.Print(filename);

  Canvas.Clear();
  HYVsPstar.GetYaxis()->SetRangeUser(0,1.1*HYVsPstar.GetMaximum());
  HYVsPstar.SetStats(0);
  HYVsPstar.Draw("pe");
  Canvas.Print(filename);

  Canvas.Clear();
  HMVsPstar.GetYaxis()->SetTitle("Mass (MeV)");
  HMVsPstar.SetStats(0);
  if(_MCorDATA==1)HMVsPstar.GetYaxis()->SetRangeUser(2455,2465);
  if(_MCorDATA==0)HMVsPstar.GetYaxis()->SetRangeUser(2450,2470);
  HMVsPstar.Draw("pe");
  Canvas.Print(filename);

  Canvas.Clear();
  HMVsPstar.GetYaxis()->SetTitle("Width (MeV)");
  HWVsPstar.SetStats(0);
  if(_MCorDATA==1)HWVsPstar.GetYaxis()->SetRangeUser(40,60);
  if(_MCorDATA==0)HWVsPstar.GetYaxis()->SetRangeUser(20,40);
  HWVsPstar.Draw("pe");
  Canvas.Print(filename);

//   Canvas.Clear();
//   HBVsPstar.SetStats(0);
//   HBVsPstar.Draw("p");
//   Canvas.Print(filename);


  Canvas.Clear();
  HChisqVsPstar.GetYaxis()->SetRangeUser(0,1.1*HChisqVsPstar.GetMaximum());
  HChisqVsPstar.SetStats(0);
  HChisqVsPstar.Draw("p");
  Canvas.Print(filename);


  Canvas.Print(filename+"]");
   		
  file.Close();

  return 1;
}



// Int_t DCPiAnalysis::DrawMassVsPstarSBR(){

//   TFile file(_OutputDir+"/PstarFits/DCPiMassPlotsVsPstar.root","read");
//   file.ls();	
//   gROOT->cd();


//   filename=_OutputDir+"/PstarFits/DCPiMassPlotsVsPstarSBR.ps";
//   Canvas.Print(filename+"["); 

  
//   TGraphErrors GMeanSB;
//   Int_t ngood=0;

//   Canvas.Clear();
//   Canvas.Divide(4,4);
//   text.SetTextSize(.2);
//   cutline.SetLineColor(2);
//   cutline.SetLineWidth(1);
  
//   Float_t minsb=1000;
//   Float_t maxsb=-1000;

//   for(Int_t i=0;i<16;i++){//skip last point

//     RooPlot* fitplot=(RooPlot*)file.Get(TString("fitplot")+(long)i);
//     if(!fitplot){cout<<"no plot"<<endl;continue;}
    
//     Canvas.cd(i+1);
//     Canvas.GetPad(i+1)->SetTopMargin(.09);
//     Canvas.GetPad(i+1)->SetBottomMargin(.2);
//     Canvas.GetPad(i+1)->SetLeftMargin(.1);
//     Canvas.GetPad(i+1)->SetRightMargin(.18);
   
//     fitplot->SetTitle("");
//     fitplot->GetYaxis()->SetTitle("");
//     fitplot->GetXaxis()->SetTitle("");
//     fitplot->GetXaxis()->SetTitleSize(.1);
//     fitplot->GetYaxis()->SetTitleSize(.1);
//     fitplot->GetYaxis()->SetTitleOffset(.5);
//     fitplot->GetXaxis()->SetLabelSize(.1);
//     fitplot->GetYaxis()->SetLabelSize(.1);
//     fitplot->GetYaxis()->SetNdivisions(5);
//     fitplot->GetXaxis()->SetNdivisions(5);

//     fitplot->Draw();
//     text.DrawTextNDC(.15,.3,TString("")+(long)(i+1));
   
//     inputpars.CloseFitPars();
//     if(!inputpars.OpenFitPars(TString(_OutputDir+"/PstarFits/FitPars_")+(long)i+".txt"))continue;
//     if(inputpars.GetFitPar("Range","status")!=0){cout<<"did not converge"<<endl;continue;}
    
//     if(inputpars.GetFitPar("D2460","mean")<2.462)continue;//cut bad measurements


//     ///Signal over Bkg
//     ///Signal/Bkg Id depends on how many curves there are.
//     Int_t NBins=((RooCurve*)fitplot->getObject(3))->GetN();
//     Float_t x1=((RooCurve*)fitplot->getObject(3))->GetX()[0];
//     Float_t xN=((RooCurve*)fitplot->getObject(3))->GetX()[NBins-1];
//     Float_t step=(xN-x1)/NBins;
//     Int_t SigBin=(int)((inputpars.GetFitPar("D2460","mean")-x1)/step +.5)+1;
//     Float_t S=((RooCurve*)fitplot->getObject(3))->GetY()[SigBin]; 
   

//     NBins=((RooCurve*)fitplot->getObject(6))->GetN();
//     x1=((RooCurve*)fitplot->getObject(6))->GetX()[0];
//     xN=((RooCurve*)fitplot->getObject(6))->GetX()[NBins-1];
//     step=(xN-x1)/NBins;
//     Int_t BkgBin=(int)((inputpars.GetFitPar("D2460","mean")-x1)/step +.5);
//     Float_t B=((RooCurve*)fitplot->getObject(6))->GetY()[BkgBin];//1=signal 2=bkg 3=total pdf 
   

//     Float_t SBR=S/B;
  
//     cutline.DrawLine(((RooCurve*)fitplot->getObject(3))->GetX()[SigBin],0,((RooCurve*)fitplot->getObject(3))->GetX()[SigBin],S+B);
//     cutline.DrawLine(((RooCurve*)fitplot->getObject(6))->GetX()[BkgBin],0,((RooCurve*)fitplot->getObject(6))->GetX()[BkgBin],S+B);


   
    
//     if(SBR<.1||SBR>5)continue;   
    
//     GMeanSB.SetPoint(ngood,SBR,inputpars.GetFitPar("D2460","mean"));
//     GMeanSB.SetPointError(ngood,0,inputpars.GetFitPar("D2460","meanerror"));

//     if(minsb>SBR)minsb=SBR;
//     if(maxsb<SBR)maxsb=SBR;

//     ngood++;
//   }

//   Canvas.Print(filename);

 
//   Canvas.Clear();
//   //TH1F HMeanFr("HMeanFr","",1,minsb-.5,maxsb+.5);
//   //HMeanFr.SetStats(0);
//   GMeanSB.GetYaxis()->SetTitle("Mean (GeV)");
//   GMeanSB.GetXaxis()->SetTitle("Signal/Background ratio");
//   GMeanSB.GetYaxis()->SetRangeUser(2.460,2.470);  
//   //HMeanFr.Draw();
//   GMeanSB.Draw("ape");  
//   Canvas.Print(filename);


//   Canvas.Print(filename+"]");
   		
//   file.Close();

//   return 1;
// }


// Int_t DCPiAnalysis::FitMassVsPstarDCos(){

//   TFile file(_OutputDir+"/HistosForFit.root","read");
//   gROOT->cd();
  
//   TFile outfile(_OutputDir+"/PstarFits/DCPiMassPlotsVsPstarDCos.root","update");
//   gROOT->cd();
		
//   for(Int_t i=0;i<8;i++){  
//     for(Int_t j=0;j<4;j++){  
  
//       Bool_t fit=0;
//       if(!inputpars.OpenFitPars(TString(_OutputDir+"/PstarFits/FitPars_")+(long)i+"_"+(long)j+".txt"))
// 	fit=1;
//       if(inputpars.GetFitPar("Range","status")!=0)
// 	fit=1;
//       inputpars.CloseFitPars();

//       if(fit){
// 	TH1F*HDCPiMass=(TH1F*)file.Get(TString("HDCPi_PstarCoarse")+(long)i+"_DCos"+(long)j);
// 	if(!massfitter.Initialize(HDCPiMass,_OutputDir+"/PstarFits/InitRooFitPars.txt"))return 0;
// 	massfitter.Fit();
// 	massfitter.SetFileTag(TString("_")+(long)i+"_"+(long)j);
// 	massfitter.Save(_OutputDir+"/PstarFits");
// 	TString tag=massfitter.GetFileTag();

// 	outfile.cd();
// 	outfile.Delete(TString("fitplot")+(long)i+"_"+(long)j+";1");
// 	RooPlot* fitplot=massfitter.GetFitPlot(); 
// 	fitplot->SetName(TString("fitplot")+(long)i+"_"+(long)j);
// 	fitplot->Write();
// 	delete fitplot;
// 	gROOT->cd();

// 	cout<<"Done with fit "<<i<<" "<<j<<endl;
//       }
//     }
//   }
		
//   outfile.ls();	
//   outfile.Close();
//   file.Close();

//   return 1;
// }

// Int_t DCPiAnalysis::DrawMassVsPstarDCos(){

//   TFile file(_OutputDir+"/PstarFits/DCPiMassPlotsVsPstarDCos.root","read");
//   file.ls();	
//   gROOT->cd();

//   filename=_OutputDir+"/PstarFits/DCPiMassPlotsVsPstarDCos.ps";
//   Canvas.Print(filename+"["); 

//   TH1F HYVsPstar("HYVsPstar","",(int)((5.0-databaseDCPi.GetDCPiPstarCut())/.2 + .5),databaseDCPi.GetDCPiPstarCut(),5.0);
//   SetHistoXYLabels(&HYVsPstar,"p*","GeV/c");
//   TH1F HMVsPstar("HMVsPstar","",(int)((5.0-databaseDCPi.GetDCPiPstarCut())/.2 + .5),databaseDCPi.GetDCPiPstarCut(),5.0);
//   SetHistoXYLabels(&HMVsPstar,"p*","GeV/c");
//   TH1F HWVsPstar("HWVsPstar","",(int)((5.0-databaseDCPi.GetDCPiPstarCut())/.2 + .5),databaseDCPi.GetDCPiPstarCut(),5.0);
//   SetHistoXYLabels(&HWVsPstar,"p*","GeV/c");
//   TH1F HBVsPstar("HBVsPstar","",(int)((5.0-databaseDCPi.GetDCPiPstarCut())/.2 + .5),databaseDCPi.GetDCPiPstarCut(),5.0);
//   SetHistoXYLabels(&HBVsPstar,"p*","GeV/c");
//   TH1F HChisqVsPstar("HChisqVsPstar","",(int)((5.0-databaseDCPi.GetDCPiPstarCut())/.2 + .5),databaseDCPi.GetDCPiPstarCut(),5.0);
//   SetHistoXYLabels(&HChisqVsPstar,"p*","GeV/c");
  

//   text.SetTextSize(.2);
//   for(Int_t j=0;j<4;j++){  

//     Canvas.Clear();
//     Canvas.Divide(3,4);

//     for(Int_t i=0;i<8;i++){//skip last point

//       RooPlot* fitplot=(RooPlot*)file.Get(TString("fitplot")+(long)i+"_"+(long)j);
//       if(!fitplot){cout<<"no plot"<<endl;continue;}
    
//       Canvas.cd(i+1);
//       Canvas.GetPad(i+1)->SetTopMargin(.09);
//       Canvas.GetPad(i+1)->SetBottomMargin(.2);
//       Canvas.GetPad(i+1)->SetLeftMargin(.1);
//       Canvas.GetPad(i+1)->SetRightMargin(.18);
   
//       fitplot->SetTitle("");
//       fitplot->GetYaxis()->SetTitle("");
//       fitplot->GetXaxis()->SetTitle("");
//       fitplot->GetXaxis()->SetTitleSize(.1);
//       fitplot->GetYaxis()->SetTitleSize(.1);
//       fitplot->GetYaxis()->SetTitleOffset(.5);
//       fitplot->GetXaxis()->SetLabelSize(.1);
//       fitplot->GetYaxis()->SetLabelSize(.1);
//       fitplot->GetYaxis()->SetNdivisions(5);
//       fitplot->GetXaxis()->SetNdivisions(5);

//       fitplot->Draw();
//       text.DrawTextNDC(.7,.75,TString("")+(long)(i+1));
    

//       inputpars.CloseFitPars();
//       if(!inputpars.OpenFitPars(TString(_OutputDir+"/PstarFits/FitPars_")+(long)i+"_"+(long)j+".txt"))continue;
//       if(inputpars.GetFitPar("Range","status")!=0){cout<<"did not converge"<<endl;continue;}

//       HYVsPstar.SetBinContent(i+1,inputpars.GetFitPar("D2460","yield"));
//       HYVsPstar.SetBinError(i+1,inputpars.GetFitPar("D2460","yielderror"));

//       HMVsPstar.SetBinContent(i+1,inputpars.GetFitPar("D2460","mean")*1000);
//       HMVsPstar.SetBinError(i+1,inputpars.GetFitPar("D2460","meanerror")*1000);

//       HWVsPstar.SetBinContent(i+1,inputpars.GetFitPar("D2460","width")*1000);
//       HWVsPstar.SetBinError(i+1,inputpars.GetFitPar("D2460","widtherror")*1000);

//       HBVsPstar.SetBinContent(i+1,inputpars.GetFitPar("Background","yield"));
//       //HBVsPstar.SetBinError(i+1,inputpars.GetFitPar("Background","yielderror"));

//       HChisqVsPstar.SetBinContent(i+1,inputpars.GetFitPar("Range","chisquare")/inputpars.GetFitPar("Range","ndf"));
      
//     }  


//     Canvas.Print(filename);

//     Canvas.Clear();
//     HYVsPstar.GetYaxis()->SetRangeUser(0,1.1*HYVsPstar.GetMaximum());
//     HYVsPstar.SetStats(0);
//     HYVsPstar.Draw("pe");
//     Canvas.Print(filename);


//     Canvas.Clear();
//     HMVsPstar.SetStats(0);
//     HMVsPstar.GetYaxis()->SetRangeUser(2450,2475);
//     HMVsPstar.Draw("pe");
//     Canvas.Print(filename);

//     Canvas.Clear();
//     HWVsPstar.SetStats(0);
//     if(_MCorDATA==0)HWVsPstar.GetYaxis()->SetRangeUser(20,40);
//     if(_MCorDATA==1)HWVsPstar.GetYaxis()->SetRangeUser(40,60);
//     HWVsPstar.Draw("pe");
//     Canvas.Print(filename);

//     Canvas.Clear();
//     HBVsPstar.SetStats(0);
//     HBVsPstar.Draw("p");
//     Canvas.Print(filename);

//     Canvas.Clear();
//     HChisqVsPstar.GetYaxis()->SetRangeUser(0,1.1*HChisqVsPstar.GetMaximum());
//     HChisqVsPstar.SetStats(0);
//     HChisqVsPstar.Draw("p");
//     Canvas.Print(filename); 
//   }




//   Canvas.Print(filename+"]");
   		
//   file.Close();

//   return 1;
// }



// Int_t DCPiAnalysis::FitMassVsPstarD2700(){

//   TFile file(_OutputDir+"/HistosForFit.root","read");
//   gROOT->cd();
  
//   TFile outfile(_OutputDir+"/PstarFits/DCPiMassPlotsVsPstarD2700.root","update");
//   gROOT->cd();


		
//   for(Int_t i=0;i<8;i++){  
//     Bool_t fit=0;
//     if(!inputpars.OpenFitPars(TString(_OutputDir+"/PstarFits/FitPars_")+(long)i+"_D2700.txt"))
//       fit=1;
//     if(inputpars.GetFitPar("Range","status")!=0)
//       fit=1;
//     inputpars.CloseFitPars();

//     if(fit){
//       TH1F*HDCPiMass=(TH1F*)file.Get(TString("HDCPi_PstarCoarse")+(long)i);
//       if(!massfitter.Initialize(HDCPiMass,_OutputDir+"/PstarFits/InitRooFitParsD2700.txt"))return 0;
//       massfitter.Fit();
//       massfitter.SetFileTag(TString("_")+(long)i+"_D2700");
//       massfitter.Save(_OutputDir+"/PstarFits");
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

// Int_t DCPiAnalysis::DrawMassVsPstarD2700(){

//   TFile file(_OutputDir+"/PstarFits/DCPiMassPlotsVsPstarD2700.root","read");
//   file.ls();	
//   gROOT->cd();


//   filename=_OutputDir+"/PstarFits/DCPiMassPlotsVsPstarD2700.ps";
//   Canvas.Print(filename+"["); 

//   TH1F HYVsPstar("HYVsPstar","",(int)((5.0-databaseDCPi.GetDCPiPstarCut())/.2 + .5),databaseDCPi.GetDCPiPstarCut(),5.0);
//   SetHistoXYLabels(&HYVsPstar,"p*","GeV/c");
//   TH1F HMVsPstar("HMVsPstar","",(int)((5.0-databaseDCPi.GetDCPiPstarCut())/.2 + .5),databaseDCPi.GetDCPiPstarCut(),5.0);
//   SetHistoXYLabels(&HMVsPstar,"p*","GeV/c");
//   TH1F HWVsPstar("HWVsPstar","",(int)((5.0-databaseDCPi.GetDCPiPstarCut())/.2 + .5),databaseDCPi.GetDCPiPstarCut(),5.0);
//   SetHistoXYLabels(&HWVsPstar,"p*","GeV/c");
//   TH1F HBVsPstar("HBVsPstar","",(int)((5.0-databaseDCPi.GetDCPiPstarCut())/.2 + .5),databaseDCPi.GetDCPiPstarCut(),5.0);
//   SetHistoXYLabels(&HBVsPstar,"p*","GeV/c");
  
//   for(Int_t i=0;i<8;i++){  
//     RooPlot* fitplot=(RooPlot*)file.Get(TString("fitplot")+(long)i);
//     if(!fitplot){cout<<"no plot"<<endl;continue;}
//     Canvas.Clear();
//     fitplot->Draw();
//     Canvas.Print(filename);

//     inputpars.CloseFitPars();
//     if(!inputpars.OpenFitPars(TString(_OutputDir+"/PstarFits/FitPars_")+(long)i+"_D2700.txt"))continue;
//     //if(inputpars.GetFitPar("Range","status")!=0){cout<<"did not converge"<<endl;continue;}
//     if(inputpars.GetFitPar("Range","chisquare")/inputpars.GetFitPar("Range","ndf")>4){
//       cout<<"did not converge"<<endl;
//       continue;
//     }  
 
   
//     HYVsPstar.SetBinContent(i+1,inputpars.GetFitPar("D2700","yield"));
//     HYVsPstar.SetBinError(i+1,inputpars.GetFitPar("D2700","yielderror"));

//     HMVsPstar.SetBinContent(i+1,inputpars.GetFitPar("D2700","mean")*1000);
//     HMVsPstar.SetBinError(i+1,inputpars.GetFitPar("D2700","meanerror")*1000);

//     HWVsPstar.SetBinContent(i+1,inputpars.GetFitPar("D2700","width")*1000);
//     HWVsPstar.SetBinError(i+1,inputpars.GetFitPar("D2700","widtherror")*1000);

//     HBVsPstar.SetBinContent(i+1,inputpars.GetFitPar("Background","yield"));
//     //HBVsPstar.SetBinError(i+1,inputpars.GetFitPar("Background","yielderror"));

      


//   }



//   Canvas.Clear();
//   HYVsPstar.SetStats(0);
//   HYVsPstar.Draw("pe");
//   Canvas.Print(filename);

//   fragfitter.Init(&HYVsPstar,_OutputDir+"/PstarFits");
//   fragfitter.Fit();
//   fragfitter.Save(); 

//   Canvas.Clear();
//   HMVsPstar.SetStats(0);
//   HMVsPstar.GetYaxis()->SetRangeUser(2730,2790);
//   HMVsPstar.Draw("pe");
//   Canvas.Print(filename);

//   Canvas.Clear();
//   HWVsPstar.SetStats(0);
//   HWVsPstar.GetYaxis()->SetRangeUser(30,90);
//   HWVsPstar.Draw("pe");
//   Canvas.Print(filename);

//   Canvas.Clear();
//   HBVsPstar.SetStats(0);
//   HBVsPstar.Draw("pe");
//   Canvas.Print(filename);


//   Canvas.Print(filename+"]");
   		
//   file.Close();

//   return 1;
// }

// Int_t DCPiAnalysis::FitMassVsPstarGlobal(){

//   TFile file(_OutputDir+"/HistosForFit.root","read");
//   gROOT->cd();
  
//   TFile outfile(_OutputDir+"/PstarFits/DCPiMassPlotsVsPstarGlobal.root","update");
//   gROOT->cd();

		
//   for(Int_t i=0;i<8;i++){  
    
//     Bool_t fit=0;
//     if(!inputpars.OpenFitPars(TString(_OutputDir+"/PstarFits/FitPars_")+(long)i+"_Global.txt"))
//       fit=1;
//     if(inputpars.GetFitPar("Range","status")!=0)
//       fit=1;
//     inputpars.CloseFitPars();

//     if(fit){
//       TH1F*HDCPiMass=(TH1F*)file.Get(TString("HDCPi_PstarCoarse")+(long)i);
//       if(!massfitter.Initialize(HDCPiMass,_OutputDir+"/PstarFits/InitRooFitParsGlobal.txt"))return 0;
//       massfitter.Fit();
//       massfitter.SetFileTag(TString("_")+(long)i+"_Global");
//       massfitter.Save(_OutputDir+"/PstarFits");
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

// Int_t DCPiAnalysis::DrawMassVsPstarGlobal(){

//   TFile file(_OutputDir+"/PstarFits/DCPiMassPlotsVsPstarGlobal.root","read");
//   file.ls();	
//   gROOT->cd();


//   filename=_OutputDir+"/PstarFits/DCPiMassPlotsVsPstarGlobal.ps";
//   Canvas.Print(filename+"["); 


//   TH1F HYVsPstar("HYVsPstar","",(int)((5.0-databaseDCPi.GetDCPiPstarCut())/.2 + .5),databaseDCPi.GetDCPiPstarCut(),5.0);
//   SetHistoXYLabels(&HYVsPstar,"p*","GeV/c");
//   TH1F HY2VsPstar("HY2VsPstar","",(int)((5.0-databaseDCPi.GetDCPiPstarCut())/.2 + .5),databaseDCPi.GetDCPiPstarCut(),5.0);
//   SetHistoXYLabels(&HY2VsPstar,"p*","GeV/c");
//   TH1F HY3VsPstar("HY3VsPstar","",(int)((5.0-databaseDCPi.GetDCPiPstarCut())/.2 + .5),databaseDCPi.GetDCPiPstarCut(),5.0);
//   SetHistoXYLabels(&HY3VsPstar,"p*","GeV/c");
//   TH1F HY4VsPstar("HY4VsPstar","",(int)((5.0-databaseDCPi.GetDCPiPstarCut())/.2 + .5),databaseDCPi.GetDCPiPstarCut(),5.0);
//   SetHistoXYLabels(&HY4VsPstar,"p*","GeV/c");
//   TH1F HBVsPstar("HBVsPstar","",(int)((5.0-databaseDCPi.GetDCPiPstarCut())/.2 + .5),databaseDCPi.GetDCPiPstarCut(),5.0);
//   SetHistoXYLabels(&HBVsPstar,"p*","GeV/c");
//   TH1F HChisqVsPstar("HChisqVsPstar","",(int)((5.0-databaseDCPi.GetDCPiPstarCut())/.2 + .5),databaseDCPi.GetDCPiPstarCut(),5.0);
//   SetHistoXYLabels(&HChisqVsPstar,"p*","GeV/c");
  
//   for(Int_t i=0;i<8;i++){  
//     RooPlot* fitplot=(RooPlot*)file.Get(TString("fitplot")+(long)i);
//     if(!fitplot){cout<<"no plot"<<endl;continue;}

//     Canvas.Clear();
//     fitplot->Draw();
//     Canvas.Print(filename);

//     inputpars.CloseFitPars();
//     if(!inputpars.OpenFitPars(TString(_OutputDir+"/PstarFits/FitPars_")+(long)i+"_Global.txt"))continue;
//     if(inputpars.GetFitPar("Range","status")!=0){cout<<"did not converge"<<endl;continue;}
// //     if(inputpars.GetFitPar("Range","chisquare")/inputpars.GetFitPar("Range","ndf")>3){
// //       cout<<"did not converge"<<endl;
// //       continue;
// //    }



//     HYVsPstar.SetBinContent(i+1,inputpars.GetFitPar("D2460","yield"));
//     HYVsPstar.SetBinError(i+1,inputpars.GetFitPar("D2460","yielderror"));

//     HY2VsPstar.SetBinContent(i+1,inputpars.GetFitPar("D2400","yield"));
//     HY2VsPstar.SetBinError(i+1,inputpars.GetFitPar("D2400","yielderror"));

//     HY3VsPstar.SetBinContent(i+1,inputpars.GetFitPar("D2700","yield"));
//     HY3VsPstar.SetBinError(i+1,inputpars.GetFitPar("D2700","yielderror"));

//     HY4VsPstar.SetBinContent(i+1,inputpars.GetFitPar("D2600","yield"));
//     HY4VsPstar.SetBinError(i+1,inputpars.GetFitPar("D2600","yielderror"));

//     HBVsPstar.SetBinContent(i+1,inputpars.GetFitPar("Background","yield"));
//     //HBVsPstar.SetBinError(i+1,inputpars.GetFitPar("Background","yielderror"));

//     HChisqVsPstar.SetBinContent(i+1,inputpars.GetFitPar("Range","chisquare")/inputpars.GetFitPar("Range","ndf"));
         
//   }



//   Canvas.Clear();
//   HYVsPstar.GetYaxis()->SetRangeUser(0,1.1*HYVsPstar.GetMaximum());
//   HYVsPstar.SetStats(0);
//   HYVsPstar.Draw("pe");
//   Canvas.Print(filename);

//   Canvas.Clear();
//   HY2VsPstar.GetYaxis()->SetRangeUser(0,1.1*HY2VsPstar.GetMaximum());
//   HY2VsPstar.SetStats(0);
//   HY2VsPstar.Draw("pe");
//   Canvas.Print(filename);

//   Canvas.Clear();
//   HY3VsPstar.GetYaxis()->SetRangeUser(0,1.1*HY3VsPstar.GetMaximum());
//   HY3VsPstar.SetStats(0);
//   HY3VsPstar.Draw("pe");
//   Canvas.Print(filename);

//   Canvas.Clear();
//   HY4VsPstar.GetYaxis()->SetRangeUser(0,1.1*HY4VsPstar.GetMaximum());
//   HY4VsPstar.SetStats(0);
//   HY4VsPstar.Draw("pe");
//   Canvas.Print(filename);

//   Canvas.Clear();
//   HBVsPstar.SetStats(0);
//   HBVsPstar.Draw("p");
//   Canvas.Print(filename);


//   Canvas.Clear();
//   HChisqVsPstar.GetYaxis()->SetRangeUser(0,1.1*HChisqVsPstar.GetMaximum());
//   HChisqVsPstar.SetStats(0);
//   HChisqVsPstar.Draw("p");
//   Canvas.Print(filename);


//   Canvas.Print(filename+"]");
   		
//   file.Close();

//   return 1;
// }



// Int_t DCPiAnalysis::FitMassVsPstarD2600(){

//   TFile file(_OutputDir+"/HistosForFit.root","read");
//   gROOT->cd();
  
//   TFile outfile(_OutputDir+"/PstarFits/DCPiMassPlotsVsPstarD2600.root","update");
//   gROOT->cd();

		
//   for(Int_t i=0;i<8;i++){  
    
//     Bool_t fit=0;
//     if(!inputpars.OpenFitPars(TString(_OutputDir+"/PstarFits/FitPars_")+(long)i+"_D2600.txt"))
//       fit=1;
//     if(inputpars.GetFitPar("Range","status")!=0)
//       fit=1;
//     inputpars.CloseFitPars();

//     if(fit){
//       TH1F*HDCPiMass=(TH1F*)file.Get(TString("HDCPi_PstarCoarse")+(long)i);
//       if(!massfitter.Initialize(HDCPiMass,_OutputDir+"/PstarFits/InitRooFitParsD2600.txt"))return 0;
//       massfitter.Fit();
//       massfitter.SetFileTag(TString("_")+(long)i+"_D2600");
//       massfitter.Save(_OutputDir+"/PstarFits");
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

// Int_t DCPiAnalysis::DrawMassVsPstarD2600(){

//   TFile file(_OutputDir+"/PstarFits/DCPiMassPlotsVsPstarD2600.root","read");
//   file.ls();	
//   gROOT->cd();


//   filename=_OutputDir+"/PstarFits/DCPiMassPlotsVsPstarD2600.ps";
//   Canvas.Print(filename+"["); 


//   TH1F HYVsPstar("HYVsPstar","",(int)((5.0-databaseDCPi.GetDCPiPstarCut())/.2 + .5),databaseDCPi.GetDCPiPstarCut(),5.0);
//   SetHistoXYLabels(&HYVsPstar,"p*","GeV/c");
//   TH1F HBVsPstar("HBVsPstar","",(int)((5.0-databaseDCPi.GetDCPiPstarCut())/.2 + .5),databaseDCPi.GetDCPiPstarCut(),5.0);
//   SetHistoXYLabels(&HBVsPstar,"p*","GeV/c");
//   TH1F HChisqVsPstar("HChisqVsPstar","",(int)((5.0-databaseDCPi.GetDCPiPstarCut())/.2 + .5),databaseDCPi.GetDCPiPstarCut(),5.0);
//   SetHistoXYLabels(&HChisqVsPstar,"p*","GeV/c");
  
//   for(Int_t i=0;i<8;i++){  
//     RooPlot* fitplot=(RooPlot*)file.Get(TString("fitplot")+(long)i);
//     if(!fitplot){cout<<"no plot"<<endl;continue;}

//     Canvas.Clear();
//     fitplot->Draw();
//     Canvas.Print(filename);

//     inputpars.CloseFitPars();
//     if(!inputpars.OpenFitPars(TString(_OutputDir+"/PstarFits/FitPars_")+(long)i+"_D2600.txt"))continue;
//     //if(inputpars.GetFitPar("Range","status")!=0){cout<<"did not converge"<<endl;continue;}
//     if(inputpars.GetFitPar("Range","chisquare")/inputpars.GetFitPar("Range","ndf")>3){
//       cout<<"did not converge"<<endl;
//       continue;
//     }



//     HYVsPstar.SetBinContent(i+1,inputpars.GetFitPar("D2600","yield"));
//     HYVsPstar.SetBinError(i+1,inputpars.GetFitPar("D2600","yielderror"));
 
//     HBVsPstar.SetBinContent(i+1,inputpars.GetFitPar("Background","yield"));
//     //HBVsPstar.SetBinError(i+1,inputpars.GetFitPar("Background","yielderror"));

//     HChisqVsPstar.SetBinContent(i+1,inputpars.GetFitPar("Range","chisquare")/inputpars.GetFitPar("Range","ndf"));
         
//   }



//   Canvas.Clear();
//   HYVsPstar.GetYaxis()->SetRangeUser(0,1.1*HYVsPstar.GetMaximum());
//   HYVsPstar.SetStats(0);
//   HYVsPstar.Draw("pe");
//   Canvas.Print(filename);

//   Canvas.Clear();
//   HBVsPstar.SetStats(0);
//   HBVsPstar.Draw("p");
//   Canvas.Print(filename);


//   Canvas.Clear();
//   HChisqVsPstar.GetYaxis()->SetRangeUser(0,1.1*HChisqVsPstar.GetMaximum());
//   HChisqVsPstar.SetStats(0);
//   HChisqVsPstar.Draw("p");
//   Canvas.Print(filename);


//   Canvas.Print(filename+"]");
   		
//   file.Close();

//   return 1;
// }



// Int_t DCPiAnalysis::FitMassVsPstarFeed(){

//   TFile file(_OutputDir+"/HistosForFit.root","read");
//   gROOT->cd();
  
//   TFile outfile(_OutputDir+"/PstarFits/DCPiMassPlotsVsPstarFeed.root","update");
//   gROOT->cd();

		
//   for(Int_t i=0;i<8;i++){  
    
//     Bool_t fit=0;
//     if(!inputpars.OpenFitPars(TString(_OutputDir+"/PstarFits/FitPars_")+(long)i+"_Feed.txt"))
//       fit=1;
//     if(inputpars.GetFitPar("Range","status")!=0)
//       fit=1;
//     inputpars.CloseFitPars();

//     if(fit){
//       TH1F*HDCPiMass=(TH1F*)file.Get(TString("HDCPi_PstarCoarse")+(long)i);
//       if(!massfitter.Initialize(HDCPiMass,_OutputDir+"/PstarFits/InitRooFitParsFeed.txt"))return 0;
//       massfitter.Fit();
//       massfitter.SetFileTag(TString("_")+(long)i+"_Feed");
//       massfitter.Save(_OutputDir+"/PstarFits");
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

// Int_t DCPiAnalysis::DrawMassVsPstarFeed(){

//   TFile file(_OutputDir+"/PstarFits/DCPiMassPlotsVsPstarFeed.root","read");
//   file.ls();	
//   gROOT->cd();


//   filename=_OutputDir+"/PstarFits/DCPiMassPlotsVsPstarFeed.ps";
//   Canvas.Print(filename+"["); 


//   TH1F HYVsPstar("HYVsPstar","",(int)((5.0-databaseDCPi.GetDCPiPstarCut())/.2 + .5),databaseDCPi.GetDCPiPstarCut(),5.0);
//   SetHistoXYLabels(&HYVsPstar,"p*","GeV/c");
//   TH1F HBVsPstar("HBVsPstar","",(int)((5.0-databaseDCPi.GetDCPiPstarCut())/.2 + .5),databaseDCPi.GetDCPiPstarCut(),5.0);
//   SetHistoXYLabels(&HBVsPstar,"p*","GeV/c");
//   TH1F HChisqVsPstar("HChisqVsPstar","",(int)((5.0-databaseDCPi.GetDCPiPstarCut())/.2 + .5),databaseDCPi.GetDCPiPstarCut(),5.0);
//   SetHistoXYLabels(&HChisqVsPstar,"p*","GeV/c");
  
//   for(Int_t i=0;i<8;i++){  
//     RooPlot* fitplot=(RooPlot*)file.Get(TString("fitplot")+(long)i);
//     if(!fitplot){cout<<"no plot"<<endl;continue;}

//     Canvas.Clear();
//     fitplot->Draw();
//     Canvas.Print(filename);

//     inputpars.CloseFitPars();
//     if(!inputpars.OpenFitPars(TString(_OutputDir+"/PstarFits/FitPars_")+(long)i+"_Feed.txt"))continue;
//     if(inputpars.GetFitPar("Range","status")!=0){cout<<"did not converge"<<endl;continue;}
// //     if(inputpars.GetFitPar("Range","chisquare")/inputpars.GetFitPar("Range","ndf")>3){
// //       cout<<"did not converge"<<endl;
// //       continue;
// //     }



//     HYVsPstar.SetBinContent(i+1,inputpars.GetFitPar("F1","yield"));
//     HYVsPstar.SetBinError(i+1,inputpars.GetFitPar("F1","yielderror"));
 
//     HBVsPstar.SetBinContent(i+1,inputpars.GetFitPar("Background","yield"));
//     //HBVsPstar.SetBinError(i+1,inputpars.GetFitPar("Background","yielderror"));

//     HChisqVsPstar.SetBinContent(i+1,inputpars.GetFitPar("Range","chisquare")/inputpars.GetFitPar("Range","ndf"));
         
//   }


//   Canvas.Clear();
//   HYVsPstar.GetYaxis()->SetRangeUser(0,1.1*HYVsPstar.GetMaximum());
//   HYVsPstar.SetStats(0);
//   HYVsPstar.Draw("pe");
//   Canvas.Print(filename);

//   Canvas.Clear();
//   HBVsPstar.SetStats(0);
//   HBVsPstar.Draw("p");
//   Canvas.Print(filename);

//   Canvas.Clear();
//   HChisqVsPstar.GetYaxis()->SetRangeUser(0,1.1*HChisqVsPstar.GetMaximum());
//   HChisqVsPstar.SetStats(0);
//   HChisqVsPstar.Draw("p");
//   Canvas.Print(filename);

//   Canvas.Print(filename+"]");
   		
//   file.Close();

//   return 1;
// }

