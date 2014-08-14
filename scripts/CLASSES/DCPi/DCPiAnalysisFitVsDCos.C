

Int_t DCPiAnalysis::MakeDCos(){

  if(!OpenReducedNtuple())return 0;

  if(!topdatabase){cout<<" no database "<<endl; return 0;}

  TFile file(_OutputDir+"/DCosPlot.root","recreate");
 
  TH1F HDCPiMass("HDCPiMass","",2000,1.9,3.9);
  ReducedNtuple->Draw("xmdm>>HDCPiMass");
  HDCPiMass.Write();  

  TH1F HDCos("HDCos","",40,-1.,1.); 
  ReducedNtuple->Draw("dcostheta>>HDCos",TCut("fabs(xmdm-2.46)<.04"));
  HDCos.Write();

  TH1F HDCosLSB("HDCosLSB","",40,-1.,1.);
  ReducedNtuple->Draw("dcostheta>>HDCosLSB",TCut("xmdm>2.38&&xmdm<2.42"));
  HDCosLSB.Write();

  TH1F HDCosHSB("HDCosHSB","",40,-1.,1.);
  ReducedNtuple->Draw("dcostheta>>HDCosHSB",TCut("xmdm>2.50&&xmdm<2.54"));
  HDCosHSB.Write();


  file.ls();
  file.Close();

  CloseReducedNtuple();

  return 1;
}

Int_t DCPiAnalysis::PlotDCos(){


  TFile file(_OutputDir+"/DCosPlot.root","read");
  gROOT->cd();


  filename=_OutputDir+"/DCosPlots.ps";
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



  TH1F*HDCos=(TH1F*)file.Get("HDCos");
  Canvas.Clear();
  HDCos->Draw("pe");
  Canvas.Print(filename);


  TH1F*HDCosLSB=(TH1F*)file.Get("HDCosLSB");
  TH1F*HDCosHSB=(TH1F*)file.Get("HDCosHSB");
  Canvas.Clear();
  HDCosLSB->Draw("pe");
  HDCosHSB->Draw("pesame");
  Canvas.Print(filename);


//   TH1F*HDCosSBAvg=(TH1F*)HDCosHSB->Clone();
//   HDCosSBAvg->SetName("HDCosSBAvg");
//   HDCosSBAvg->Add(HDCosLSB);
//   //HDCosSBAvg->Scale(integral->getVal()/HDCosSBAvg->Integral());
//   Canvas.Clear();
//   HDCosSBAvg->Draw("pe");
//   Canvas.Print(filename);
// //   file.cd();
// //   HDCosSBAvg->Write(); 

//   TH1F*HDCosSignal=(TH1F*)HDCos->Clone();
//   HDCosSignal->SetName("HDCosSignal");
//   HDCosSignal->Add(HDCosSBAvg,-1);
//   Canvas.Clear();
//   HDCos->Draw("pe");
//   HDCosSBAvg->Draw("pesame");
//   HDCosSignal->Draw("pesame");
//   Canvas.Print(filename);

  
//   file.cd();
//   HDCosSignal->Write();


  file.Close();

  Canvas.Print(filename+"]");
  
  return 1;
}


Int_t DCPiAnalysis::FitMassVsDCos(TString Dir){

  TFile file(_OutputDir+"/HistosForFit.root","read");
  gROOT->cd();
  
  TFile outfile(_OutputDir+"/DCosFits/"+Dir+"/DCPiMassPlotsVsDCos.root","update");
  gROOT->cd();

  Int_t Nfits=9;
  if(Dir=="AllP") Nfits=18;
		
  for(Int_t i=0;i<Nfits;i++){  
    Bool_t fit=0;
    if(!inputpars.OpenFitPars(TString(_OutputDir+"/DCosFits/"+Dir+"/FitPars_")+(long)i+".txt"))
      fit=1;
    if(inputpars.GetFitPar("Range","status")!=0)
      fit=1;
    inputpars.CloseFitPars();

    if(fit){
      TH1F*HDCPiMass=NULL;
      if(Dir=="AllP")HDCPiMass=(TH1F*)file.Get(TString("HDCPi_DCos")+(long)i);
      if(Dir=="P1")HDCPiMass=(TH1F*)file.Get(TString("HDCPi_DCosCoarse")+(long)i+"_Pstar"+(long)0);
      if(Dir=="P2")HDCPiMass=(TH1F*)file.Get(TString("HDCPi_DCosCoarse")+(long)i+"_Pstar"+(long)1);
      if(Dir=="P3")HDCPiMass=(TH1F*)file.Get(TString("HDCPi_DCosCoarse")+(long)i+"_Pstar"+(long)2);
      if(Dir=="P4")HDCPiMass=(TH1F*)file.Get(TString("HDCPi_DCosCoarse")+(long)i+"_Pstar"+(long)3);

      if(!massfitter.Initialize(HDCPiMass,_OutputDir+"/DCosFits/InitRooFitPars.txt"))continue;
//       if(_MCorDATA==1&&_TruthMatch==0){
// 	massfitter.FixParameter("D2460","mean",.001*FDCosM->Eval(-1.+(i+.5)*.1));
// 	massfitter.FixParameter("D2460","width",.001*FDCosW->Eval(-1.+(i+.5)*.1));
//       }

      massfitter.Fit();
      massfitter.SetFileTag(TString("_")+(long)i);
      massfitter.Save(_OutputDir+"/DCosFits/"+Dir);
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

Int_t DCPiAnalysis::DrawMassVsDCos(TString Dir){

  TFile file(_OutputDir+"/DCosFits/"+Dir+"/DCPiMassPlotsVsDCos.root","read");
  file.ls();	
  gROOT->cd();
  

  filename=_OutputDir+"/DCosFits/"+Dir+"/DCPiMassPlotsVsDCos.ps";
  Canvas.Print(filename+"["); 

  Int_t Nfits=9;
  if(Dir=="AllP") Nfits=18;
  Int_t Nbins=10;
  if(Dir=="AllP") Nbins=20;

  TH1F HYVsDCos("HYVsDCos","",Nbins,-1,1);
  SetHistoXYLabels(&HYVsDCos,"cos(#theta_{D})","");
  HYVsDCos.GetYaxis()->SetTitle("Yield");
  TH1F HMVsDCos("HMVsDCos","",Nbins,-1,1);
  SetHistoXYLabels(&HMVsDCos,"cos(#theta_{D})","");
  HMVsDCos.GetYaxis()->SetTitle("Mass (MeV)");
  TH1F HWVsDCos("HWVsDCos","",Nbins,-1,1);
  SetHistoXYLabels(&HWVsDCos,"cos(#theta_{D})","");
  HWVsDCos.GetYaxis()->SetTitle("Width (MeV)");
//   TH1F HBVsDCos("HBVsDCos","",Nbins,-1,1);
//   SetHistoXYLabels(&HBVsDCos,"cos(#theta_{D})","");
  TH1F HChisqVsDCos("HChisqVsDCos","",Nbins,-1,1);
  SetHistoXYLabels(&HChisqVsDCos,"cos(#theta_{D})","");
  HChisqVsDCos.GetYaxis()->SetTitle("#chi^{2}/ndf");
  
  Canvas.Clear();
  if(Nbins==20)Canvas.Divide(4,5);
  if(Nbins==10)Canvas.Divide(2,5);
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
   

    if(!inputpars.OpenFitPars(TString(_OutputDir+"/DCosFits/"+Dir+"/FitPars_")+(long)i+".txt"))continue;
    if(inputpars.GetFitPar("Range","status")!=0)continue;

    HYVsDCos.SetBinContent(i+1,inputpars.GetFitPar("D2460","yield"));
    HYVsDCos.SetBinError(i+1,inputpars.GetFitPar("D2460","yielderror"));

    HMVsDCos.SetBinContent(i+1,inputpars.GetFitPar("D2460","mean")*1000);
    HMVsDCos.SetBinError(i+1,inputpars.GetFitPar("D2460","meanerror")*1000);

    HWVsDCos.SetBinContent(i+1,inputpars.GetFitPar("D2460","width")*1000);
    HWVsDCos.SetBinError(i+1,inputpars.GetFitPar("D2460","widtherror")*1000);

    //    HBVsDCos.SetBinContent(i+1,inputpars.GetFitPar("Background","yield"));
    
    HChisqVsDCos.SetBinContent(i+1,inputpars.GetFitPar("Range","chisquare")/inputpars.GetFitPar("Range","ndf"));
    

    inputpars.CloseFitPars();
  }
  Canvas.Print(filename);
  
  Canvas.Clear();
  HYVsDCos.GetYaxis()->SetRangeUser(0,1.1*HYVsDCos.GetMaximum());
  HYVsDCos.SetStats(0);
  HYVsDCos.Draw("pe");
  Canvas.Print(filename);
  HYVsDCos.Fit(FDCos,"I","",-1,1);
  Canvas.Clear();
  HYVsDCos.SetStats(0);
  HYVsDCos.Draw("pe");
  Canvas.Print(filename);


  Canvas.Clear();
  HMVsDCos.SetStats(0);
  if(_MCorDATA==1)HMVsDCos.GetYaxis()->SetRangeUser(2450,2475);
  if(_MCorDATA==0)HMVsDCos.GetYaxis()->SetRangeUser(2450,2470);
  HMVsDCos.Draw("pe");
  Canvas.Print(filename);

  HMVsDCos.Fit(FDCosM,"I","",-1,1);
  Canvas.Clear();
  HMVsDCos.Draw("pe");
  Canvas.Print(filename);

  Canvas.Clear();
  HMVsDCos.GetYaxis()->SetTitle("Width (MeV)");
  HWVsDCos.SetStats(0);
  if(_MCorDATA==1)HWVsDCos.GetYaxis()->SetRangeUser(35,65);
  if(_MCorDATA==0)HWVsDCos.GetYaxis()->SetRangeUser(25,45);
  HWVsDCos.Draw("pe");
  Canvas.Print(filename);

  HWVsDCos.Fit(FDCosW,"I","",-1,1);
  Canvas.Clear();
  HWVsDCos.Draw("pe");
  Canvas.Print(filename);


  Canvas.Clear();
  HChisqVsDCos.GetYaxis()->SetRangeUser(0,8);
  HChisqVsDCos.SetStats(0);
  HChisqVsDCos.Draw("p");
  Canvas.Print(filename);


  Canvas.Print(filename+"]");
   		
  file.Close();

  return 1;
}


Int_t DCPiAnalysis::PlotMassPeakVsDCos(TString Dir){
  
  TFile file(_OutputDir+"/HistosForFit.root","read");
  gROOT->cd();
  
  filename=_OutputDir+"/DCosFits/"+Dir+"/DCPiMassPeakVsDCos.ps";
  Canvas.Print(filename+"["); 
  
  cutline.SetLineColor(2);
  cutline.SetLineWidth(2);

  Canvas.Clear();
  TH1F* HAdd[4];
  TF1* F[4];
  
  TH1*HDCPiMass=(TH1*)file.Get(TString("HDCPi_DCos")+(long)0);
  HDCPiMass=RebinHistoX((TH1*)HDCPiMass,60,2.3,2.6);
  if(!HDCPiMass)return 0;
  HAdd[0]=(TH1F*)HDCPiMass->Clone();
  HAdd[1]=(TH1F*)HDCPiMass->Clone();
  HAdd[2]=(TH1F*)HDCPiMass->Clone();
  HAdd[3]=(TH1F*)HDCPiMass->Clone();

  Float_t par[7]={1,2.459,.06,1,2,3,4};
  
  for(Int_t i=1;i<5;i++){  
    TH1*HDCPiMass=(TH1*)file.Get(TString("HDCPi_DCos")+(long)i);
    HDCPiMass=RebinHistoX((TH1*)HDCPiMass,60,2.3,2.6);
    if(!HDCPiMass)continue;
    HAdd[0]->Add(HDCPiMass);
     
  }
  Float_t Bin1Content=HAdd[0]->GetBinContent(1);
  //HAdd[0]->Scale(1./HAdd[0]->Integral());
  HAdd[0]->Scale(Bin1Content/HAdd[0]->GetBinContent(1));
  F[0]=FitHistogramBreitWignerPolN(HAdd[0],2.3,2.6,2,par);
  F[0]->SetLineColor(1);
  HAdd[0]->SetStats(0);
  HAdd[0]->SetMarkerColor(1);
 
 
  HAdd[1]->Clear();HAdd[1]->Reset();cout<<HAdd[1]->Integral()<<endl;
  for(Int_t i=5;i<10;i++){  
    TH1*HDCPiMass=(TH1*)file.Get(TString("HDCPi_DCos")+(long)i);
    HDCPiMass=RebinHistoX((TH1*)HDCPiMass,60,2.3,2.6);
    if(!HDCPiMass)continue;
    HAdd[1]->Add(HDCPiMass);
     
  }
  //HAdd[1]->Scale(1./HAdd[1]->Integral());
  HAdd[1]->Scale(Bin1Content/HAdd[1]->GetBinContent(1));
  F[1]=FitHistogramBreitWignerPolN(HAdd[1],2.3,2.6,2,par);
  F[1]->SetLineColor(2);
  HAdd[1]->SetStats(0);
  HAdd[1]->SetMarkerColor(2);
 
  HAdd[2]->Clear();HAdd[2]->Reset();cout<<HAdd[2]->Integral()<<endl;
  for(Int_t i=10;i<15;i++){  
    TH1*HDCPiMass=(TH1*)file.Get(TString("HDCPi_DCos")+(long)i);
    HDCPiMass=RebinHistoX((TH1*)HDCPiMass,60,2.3,2.6);
    if(!HDCPiMass)continue;
    HAdd[2]->Add(HDCPiMass);
     
  }
  //HAdd[2]->Scale(1./HAdd[2]->Integral());
  HAdd[2]->Scale(Bin1Content/HAdd[2]->GetBinContent(1));
  F[2]=FitHistogramBreitWignerPolN(HAdd[2],2.3,2.6,2,par);
  F[2]->SetLineColor(3);
  HAdd[2]->SetStats(0);
  HAdd[2]->SetMarkerColor(3);


  HAdd[3]->Clear();HAdd[3]->Reset();cout<<HAdd[3]->Integral()<<endl;
  for(Int_t i=15;i<19;i++){  
    TH1*HDCPiMass=(TH1*)file.Get(TString("HDCPi_DCos")+(long)i);
    HDCPiMass=RebinHistoX((TH1*)HDCPiMass,60,2.3,2.6);
    if(!HDCPiMass)continue;
    HAdd[3]->Add(HDCPiMass);
     
  }
  //HAdd[3]->Scale(1./HAdd[3]->Integral());
  HAdd[3]->Scale(Bin1Content/HAdd[3]->GetBinContent(1));
  F[3]=FitHistogramBreitWignerPolN(HAdd[3],2.3,2.6,2,par);
  F[3]->SetLineColor(4);
  HAdd[3]->SetStats(0);
  HAdd[3]->SetMarkerColor(4);

  HAdd[0]->GetYaxis()->SetRangeUser(HAdd[0]->GetMinimum(),HAdd[0]->GetMaximum());
  HAdd[0]->GetXaxis()->SetTitle("m(D#pi)");
  HAdd[0]->GetYaxis()->SetTitle("normalized to unit area");
  HAdd[0]->Draw("l");

  cutline.SetLineColor(7);
  if(_MCorDATA==0)cutline.DrawLine(2.459,HAdd[0]->GetMinimum(),2.459,HAdd[0]->GetMaximum());
  if(_MCorDATA==1)cutline.DrawLine(2.46125,HAdd[0]->GetMinimum(),2.46125,HAdd[0]->GetMaximum());
  F[0]->Draw("lsame"); cutline.SetLineColor(1);
  cutline.DrawLine(F[0]->GetParameter(1),HAdd[0]->GetMinimum(),F[0]->GetParameter(1),HAdd[0]->GetMinimum()+(HAdd[0]->GetMaximum()-HAdd[0]->GetMinimum())/4.);
  
  HAdd[1]->Draw("lsame");  F[1]->Draw("lsame");cutline.SetLineColor(2);
  cutline.DrawLine(F[1]->GetParameter(1),HAdd[0]->GetMinimum(),F[1]->GetParameter(1),HAdd[0]->GetMinimum()+(HAdd[0]->GetMaximum()-HAdd[0]->GetMinimum())/4.);
  
  HAdd[2]->Draw("lsame");  F[2]->Draw("lsame");cutline.SetLineColor(3);
  cutline.DrawLine(F[2]->GetParameter(1),HAdd[0]->GetMinimum(),F[2]->GetParameter(1),HAdd[0]->GetMinimum()+(HAdd[0]->GetMaximum()-HAdd[0]->GetMinimum())/4.);
  
  HAdd[3]->Draw("lsame");  F[3]->Draw("lsame");cutline.SetLineColor(4);
  cutline.DrawLine(F[3]->GetParameter(1),HAdd[0]->GetMinimum(),F[3]->GetParameter(1),HAdd[0]->GetMinimum()+(HAdd[0]->GetMaximum()-HAdd[0]->GetMinimum())/4.);

 

  Canvas.Print(filename);

  Canvas.Print(filename+"]");
  file.Close();

  return 1;
}



// Int_t DCPiAnalysis::FitMassVsDCosPstar(){

//   TFile file(_OutputDir+"/HistosForFit.root","read");
//   file.ls();
//   gROOT->cd();
  
//   TFile outfile(_OutputDir+"/DCosFits/DCPiMassPlotsVsDCosPstar.root","update");
//   gROOT->cd();
		
//   for(Int_t i=0;i<10;i++){  
//     for(Int_t j=0;j<4;j++){  
  
//       Bool_t fit=0;
//       if(!inputpars.OpenFitPars(TString(_OutputDir+"/DCosFits/FitPars_")+(long)i+"_"+(long)j+".txt"))
// 	fit=1;
//       if(inputpars.GetFitPar("Range","status")!=0)
// 	fit=1;
//       inputpars.CloseFitPars();

//       if(fit){
// 	TH1F*HDCPiMass=(TH1F*)file.Get(TString("HDCPi_DCosCoarse")+(long)i+"_Pstar"+(long)j);
// 	if(!massfitter.Initialize(HDCPiMass,_OutputDir+"/DCosFits/InitRooFitPars.txt"))return 0;
// 	massfitter.Fit();
// 	massfitter.SetFileTag(TString("_")+(long)i+"_"+(long)j);
// 	massfitter.Save(_OutputDir+"/DCosFits");
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

// Int_t DCPiAnalysis::DrawMassVsDCosPstar(){

//   TFile file(_OutputDir+"/DCosFits/DCPiMassPlotsVsDCosPstar.root","read");
//   file.ls();	
//   gROOT->cd();

//   filename=_OutputDir+"/DCosFits/DCPiMassPlotsVsDCosPstar.ps";
//   Canvas.Print(filename+"["); 

//   TH1F HYVsDCos("HYVsDCos","",10,-1,1);
//   SetHistoXYLabels(&HYVsDCos,"cos(#theta_{D})","");
//   TH1F HMVsDCos("HMVsDCos","",10,-1,1);
//   SetHistoXYLabels(&HMVsDCos,"cos(#theta_{D})","");
//   HMVsDCos.GetYaxis()->SetTitle("Mean (GeV)");
//   TH1F HWVsDCos("HWVsDCos","",10,-1,1);
//   SetHistoXYLabels(&HWVsDCos,"cos(#theta_{D})","");
//   TH1F HBVsDCos("HBVsDCos","",10,-1,1);
//   SetHistoXYLabels(&HBVsDCos,"cos(#theta_{D})","");
//   TH1F HChisqVsDCos("HChisqVsDCos","",10,-1,1);
//   SetHistoXYLabels(&HChisqVsDCos,"cos(#theta_{D})","");
  

//   text.SetTextSize(.2);
//   for(Int_t j=0;j<4;j++){  

//     Canvas.Clear();
//     Canvas.Divide(3,4);

//     for(Int_t i=0;i<10;i++){
      
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
//       if(!inputpars.OpenFitPars(TString(_OutputDir+"/DCosFits/FitPars_")+(long)i+"_"+(long)j+".txt"))continue;
//       if(inputpars.GetFitPar("Range","status")!=0){cout<<"did not converge"<<endl;continue;}

//       HYVsDCos.SetBinContent(i+1,inputpars.GetFitPar("D2460","yield"));
//       HYVsDCos.SetBinError(i+1,inputpars.GetFitPar("D2460","yielderror"));

//       HMVsDCos.SetBinContent(i+1,inputpars.GetFitPar("D2460","mean"));
//       HMVsDCos.SetBinError(i+1,inputpars.GetFitPar("D2460","meanerror"));

//       HWVsDCos.SetBinContent(i+1,inputpars.GetFitPar("D2460","width"));
//       HWVsDCos.SetBinError(i+1,inputpars.GetFitPar("D2460","widtherror"));

//       HBVsDCos.SetBinContent(i+1,inputpars.GetFitPar("Background","yield"));
//       //HBVsDCos.SetBinError(i+1,inputpars.GetFitPar("Background","yielderror"));

//       HChisqVsDCos.SetBinContent(i+1,inputpars.GetFitPar("Range","chisquare")/inputpars.GetFitPar("Range","ndf"));
      
//     }  


//     Canvas.Print(filename);

//     Canvas.Clear();
//     HYVsDCos.GetYaxis()->SetRangeUser(0,1.1*HYVsDCos.GetMaximum());
//     HYVsDCos.SetStats(0);
//     HYVsDCos.Draw("pe");
//     Canvas.Print(filename);


//     Canvas.Clear();
//     HMVsDCos.SetStats(0);
//     HMVsDCos.GetYaxis()->SetRangeUser(2.450,2.475);
//     HMVsDCos.Draw("pe");
//     Canvas.Print(filename);

//     Canvas.Clear();
//     HWVsDCos.SetStats(0);
//     if(_MCorDATA==0)HWVsDCos.GetYaxis()->SetRangeUser(.020,.040);
//     if(_MCorDATA==1)HWVsDCos.GetYaxis()->SetRangeUser(.040,.060);
//     HWVsDCos.Draw("pe");
//     Canvas.Print(filename);

//     Canvas.Clear();
//     HBVsDCos.SetStats(0);
//     HBVsDCos.Draw("p");
//     Canvas.Print(filename);

//     Canvas.Clear();
//     HChisqVsDCos.GetYaxis()->SetRangeUser(0,1.1*HChisqVsDCos.GetMaximum());
//     HChisqVsDCos.SetStats(0);
//     HChisqVsDCos.Draw("p");
//     Canvas.Print(filename); 
//   }




//   Canvas.Print(filename+"]");
   		
//   file.Close();

//   return 1;
// }






// Int_t DCPiAnalysis::MakeDCosXPstar(){//for background pdf

//   if(!OpenReducedNtuple("CleanNtuple"))return 0;

//   if(!topdatabase){cout<<" no database "<<endl; return 0;}
 
//   TH1F HDCPiMass("HDCPiMass","",2000,1.9,3.9);
//   ReducedNtuple->Draw("xmdm>>HDCPiMass");
   
//   TH2F HDCosXPstar("HDCosXPstar","",40,3.,5.0,40,-1.,1.0); 
//   ReducedNtuple->Draw("dcostheta:xpstar>>HDCosXPstar",topdatabase->GetCut("xmdm"));
    
//   TH2F HDCosXPstarLSB("HDCosXPstarLSB","",40,3.,5.0,40,-1.,1.0);
//   ReducedNtuple->Draw("dcostheta:xpstar>>HDCosXPstarLSB",topdatabase->GetCut("xmdmsb")+TCut("xmdm<2.46"));

//   TH2F HDCosXPstarHSB("HDCosXPstarHSB","",40,3.,5.0,40,-1.,1.0);
//   ReducedNtuple->Draw("dcostheta:xpstar>>HDCosXPstarHSB",topdatabase->GetCut("xmdmsb")+TCut("xmdm>2.46"));
    
 
//   TFile file(_OutputDir+"/DCosXPstarPlot.root","recreate");
//   HDCPiMass.Write();    
//   HDCosXPstar.Write();
//   HDCosXPstarLSB.Write();
//   HDCosXPstarHSB.Write();

//   file.ls();
//   file.Close();

//   CloseReducedNtuple();

//   return 1;
// }

// Int_t DCPiAnalysis::PlotDCosXPstar(){

//   TFile file(_OutputDir+"/DCosXPstarPlot.root","update");
//   gROOT->cd();

//   filename=_OutputDir+"/DCosXPstarPlots.ps";
//   Canvas.Print(filename+"["); 


//   //get the signal on top of the signal
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


//   Canvas.SetLogz(1);
//   TH2F*HDCosXPstar=(TH2F*)file.Get("HDCosXPstar");
//   Canvas.Clear();
//   HDCosXPstar->Draw("colz");
//   Canvas.Print(filename);


//   TH2F*HDCosXPstarLSB=(TH2F*)file.Get("HDCosXPstarLSB");
//   Canvas.Clear();
//   HDCosXPstarLSB->Draw("colz");
//   Canvas.Print(filename);
 

//   TH2F*HDCosXPstarHSB=(TH2F*)file.Get("HDCosXPstarHSB");
//   Canvas.Clear();
//   HDCosXPstarHSB->Draw("colz");
//   Canvas.Print(filename);
  

//   TH2F*HDCosXPstarBkg=(TH2F*)HDCosXPstar->Clone();
//   HDCosXPstarBkg->SetName("HDCosXPstarBkg");
//   TH2F*HDCosXPstarSignal=(TH2F*)HDCosXPstar->Clone();
//   for(Int_t i=1;i<=HDCosXPstarSignal->GetXaxis()->GetNbins();i++)
//     for(Int_t j=1;j<=HDCosXPstarSignal->GetYaxis()->GetNbins();j++)
//       HDCosXPstarSignal->SetBinContent(i,j,
// 				       FDCos->Eval(HDCosXPstarSignal->GetYaxis()->GetBinCenter(j))
// 				       *fragfitter.GetModel("Peterson")->Eval(HDCosXPstarSignal->GetXaxis()->GetBinCenter(i))
// 				       );

//   Int_t sigyield=(int)(HDCosXPstar->Integral()-integral->getVal());
//   cout<<"sig yield in sig region "<<sigyield<<endl;
//   HDCosXPstarSignal->Scale(sigyield/HDCosXPstarSignal->Integral());

  
  
//   HDCosXPstarBkg->Add(HDCosXPstarSignal,-1);
//   Canvas.Clear();
//   HDCosXPstarBkg->Draw("colz");
//   Canvas.Print(filename);
//   Canvas.SetLogz(0);
  
//   Canvas.Clear();
//   HDCosXPstarSignal->Draw("colz");
//   Canvas.Print(filename);

// //   TH2F*HDCosXPstarSignal=(TH2F*)HDCosXPstar->Clone();
// //   HDCosXPstarSignal->SetName("HDCosXPstarSignal");
// //   HDCosXPstarSB->Scale(bkgyield/HDCosXPstarSB->Integral());
// //   HDCosXPstarSignal->Add(HDCosXPstarSB,-1);
// //   Canvas.Clear();
// //   HDCosXPstarSignal->Draw("colz");
// //   Canvas.Print(filename);

// //   for(Int_t x=1;x<=HDCosXPstarSignal->GetXaxis()->GetNbins();x++)
// //     for(Int_t y=1;y<=HDCosXPstarSignal->GetYaxis()->GetNbins();y++)
// //       if(HDCosXPstarSignal->GetBinContent(x,y)<0)
// // 	HDCosXPstarSignal->SetBinContent(x,y,0);

// //   Canvas.Clear();
// //   HDCosXPstarSignal->Draw("colz");
// //   Canvas.Print(filename);


//   file.cd();
//   HDCosXPstarBkg->Write();
//   file.Close();

//   Canvas.Print(filename+"]");
  
//   return 1;
// }



// Int_t DCPiAnalysis::MakeDCosHistos(){


//   if(!OpenReducedNtuple("CleanNtupleFullDCos"))
//     if(!OpenReducedNtuple("CleanNtupleFinal"))return 0;
//   SetDCPiReducedNtupleBranches();
  
//   TFile file(_OutputDir+"/HistosForDCosFit.root","recreate");


//   TH1F* HDCPiVsDCos[20];
//   for(Int_t i=0;i<20;i++)
//     HDCPiVsDCos[i]=new TH1F(TString("HDCPi_DCos")+(long)i,"",2000,1.9,3.9);

//   TH1F* HDCPiVsDCosCoarse[10];
//   for(Int_t i=0;i<10;i++)
//     HDCPiVsDCosCoarse[i]=new TH1F(TString("HDCPi_DCosCoarse")+(long)i,"",2000,1.9,3.9);
  
//   Int_t e=0;
//   while(ReducedNtuple->GetEntry(e)){
//     e++;
//     if(e%50000==0)cout<<e<<" cands done"<<endl;
      
//     for(Int_t i=0;i<20;i++)
//       if((-1.+i*.1)<dcostheta&&dcostheta<(-1.+(i+1)*.1))
// 	HDCPiVsDCos[i]->Fill(xmdm);

//     for(Int_t i=0;i<10;i++)
//       if((-1.+i*.2)<dcostheta&&dcostheta<(-1.+(i+1)*.2))
// 	HDCPiVsDCosCoarse[i]->Fill(xmdm);

//   }

//   for(Int_t i=0;i<20;i++)    
//     HDCPiVsDCos[i]->Write();

//   for(Int_t i=0;i<10;i++)    
//     HDCPiVsDCosCoarse[i]->Write();


//   file.ls();
//   file.Close();

//   if(CloseReducedNtuple()!=1) return 0;


//   return 1;
// }




// Int_t DCPiAnalysis::FitMassVsDCosD2700(){

//   TFile file(_OutputDir+"/HistosForFit.root","read");
//   gROOT->cd();
  
//   TFile outfile(_OutputDir+"/DCosFits/DCPiMassPlotsVsDCosD2700.root","update");
//   gROOT->cd();

//   for(Int_t i=0;i<10;i++){  
//     Bool_t fit=0;
//     if(!inputpars.OpenFitPars(TString(_OutputDir+"/DCosFits/FitPars_")+(long)i+"_D2700.txt"))
//       fit=1;
//     if(inputpars.GetFitPar("Range","status")!=0)
//       fit=1;
//     inputpars.CloseFitPars();

//     if(fit){
//       TH1F*HDCPiMass=(TH1F*)file.Get(TString("HDCPi_DCosCoarse")+(long)i);
//       if(!massfitter.Initialize(HDCPiMass,_OutputDir+"/DCosFits/InitRooFitParsD2700.txt"))continue;
//       massfitter.Fit();
//       massfitter.SetFileTag(TString("_")+(long)i+"_D2700");
//       massfitter.Save(_OutputDir+"/DCosFits");
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

// Int_t DCPiAnalysis::DrawMassVsDCosD2700(){

//   TFile file(_OutputDir+"/DCosFits/DCPiMassPlotsVsDCosD2700.root","read");
//   file.ls();	
//   gROOT->cd();
  

//   filename=_OutputDir+"/DCosFits/DCPiMassPlotsVsDCosD2700.ps";
//   Canvas.Print(filename+"["); 

//   TH1F HYVsDCos("HYVsDCos","",10,-1,1);
//   SetHistoXYLabels(&HYVsDCos,"cos(#theta_{D})","");  
//   TH1F HMVsDCos("HMVsDCos","",10,-1,1);
//   SetHistoXYLabels(&HMVsDCos,"cos(#theta_{D})","");
//   HMVsDCos.GetYaxis()->SetTitle("Mass (GeV)");
//   TH1F HWVsDCos("HWVsDCos","",10,-1,1);
//   SetHistoXYLabels(&HWVsDCos,"cos(#theta_{D})","");
//   TH1F HBVsDCos("HBVsDCos","",10,-1,1);
//   SetHistoXYLabels(&HBVsDCos,"cos(#theta_{D})","");
  
//   for(Int_t i=0;i<10;i++){  
//     RooPlot* fitplot=(RooPlot*)file.Get(TString("fitplot")+(long)i);
//     if(!fitplot){cout<<"no plot"<<endl;continue;}

//     Canvas.Clear();
//     fitplot->Draw();
//     Canvas.Print(filename);

//     inputpars.CloseFitPars();
//     if(!inputpars.OpenFitPars(TString(_OutputDir+"/DCosFits/FitPars_")+(long)i+"_D2700.txt"))continue;
//     if(inputpars.GetFitPar("Range","status")!=0){cout<<"did not converge"<<endl;continue;}
// //     if(inputpars.GetFitPar("Range","chisquare")/inputpars.GetFitPar("Range","ndf")>4){
// //       cout<<"did not converge"<<endl;
// //       continue;
// //     }  
    

   
    
//     HYVsDCos.SetBinContent(i+1,inputpars.GetFitPar("D2700","yield"));
//     HYVsDCos.SetBinError(i+1,inputpars.GetFitPar("D2700","yielderror"));

//     HMVsDCos.SetBinContent(i+1,inputpars.GetFitPar("D2700","mean"));
//     HMVsDCos.SetBinError(i+1,inputpars.GetFitPar("D2700","meanerror"));

//     HWVsDCos.SetBinContent(i+1,inputpars.GetFitPar("D2700","width"));
//     HWVsDCos.SetBinError(i+1,inputpars.GetFitPar("D2700","widtherror"));

//     HBVsDCos.SetBinContent(i+1,inputpars.GetFitPar("Background","yield"));
//     //HBVsDCos.SetBinError(i+1,inputpars.GetFitPar("Background","yielderror"));

      

   
//   }
  

  
//   Canvas.Clear();
//   HYVsDCos.GetYaxis()->SetRangeUser(0,1.1*HYVsDCos.GetMaximum());
//   HYVsDCos.SetStats(0);
//   HYVsDCos.Draw("pe");
//   Canvas.Print(filename);
//   HYVsDCos.Fit(FDCos,"I","",-1,1);
//   Canvas.Clear();
//   HYVsDCos.SetStats(0);
//   HYVsDCos.Draw("pe");
//   Canvas.Print(filename);


//   Canvas.Clear();
//   HMVsDCos.SetStats(0);
//   HMVsDCos.GetYaxis()->SetRangeUser(2.730,2.790);
//   HMVsDCos.Draw("pe");
//   Canvas.Print(filename);

//   Canvas.Clear();
//   HWVsDCos.SetStats(0);
//   HWVsDCos.GetYaxis()->SetRangeUser(.030,.090);
//   HWVsDCos.Draw("pe");
//   Canvas.Print(filename);

//   Canvas.Clear();
//   HBVsDCos.SetStats(0);
//   HBVsDCos.Draw("pe");
//   Canvas.Print(filename);


//   Canvas.Print(filename+"]");
   		
//   file.Close();
  
//   return 1;
// }




// Int_t DCPiAnalysis::FitMassVsDCosD2600(){

//   TFile file(_OutputDir+"/HistosForFit.root","read");
//   gROOT->cd();
  
//   TFile outfile(_OutputDir+"/DCosFits/DCPiMassPlotsVsDCosD2600.root","update");
//   gROOT->cd();

		
//   for(Int_t i=0;i<10;i++){  
//     Bool_t fit=0;
//     if(!inputpars.OpenFitPars(TString(_OutputDir+"/DCosFits/FitPars_")+(long)i+"_D2600.txt"))
//       fit=1;
//     if(inputpars.GetFitPar("Range","status")!=0)
//       fit=1;
//     inputpars.CloseFitPars();

//     if(fit){
//       TH1F*HDCPiMass=(TH1F*)file.Get(TString("HDCPi_DCosCoarse")+(long)i);
//       if(!massfitter.Initialize(HDCPiMass,_OutputDir+"/DCosFits/InitRooFitParsD2600.txt"))continue;
//       massfitter.Fit();
//       massfitter.SetFileTag(TString("_")+(long)i+"_D2600");
//       massfitter.Save(_OutputDir+"/DCosFits");
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

// Int_t DCPiAnalysis::DrawMassVsDCosD2600(){

//   TFile file(_OutputDir+"/DCosFits/DCPiMassPlotsVsDCosD2600.root","read");
//   file.ls();	
//   gROOT->cd();
  

//   filename=_OutputDir+"/DCosFits/DCPiMassPlotsVsDCosD2600.ps";
//   Canvas.Print(filename+"["); 

//   TH1F HYVsDCos("HYVsDCos","",10,-1,1);
//   SetHistoXYLabels(&HYVsDCos,"cos(#theta_{D})","");
//   TH1F HMVsDCos("HMVsDCos","",10,-1,1);
//   SetHistoXYLabels(&HMVsDCos,"cos(#theta_{D})","");
//   TH1F HWVsDCos("HWVsDCos","",10,-1,1);
//   SetHistoXYLabels(&HWVsDCos,"cos(#theta_{D})","");
//   TH1F HBVsDCos("HBVsDCos","",10,-1,1);
//   SetHistoXYLabels(&HBVsDCos,"cos(#theta_{D})","");
//   TH1F HChisqVsDCos("HChisqVsDCos","",10,-1,1);
//   SetHistoXYLabels(&HChisqVsDCos,"cos(#theta_{D})","");  

//   for(Int_t i=0;i<10;i++){  
//     RooPlot* fitplot=(RooPlot*)file.Get(TString("fitplot")+(long)i);
//     if(!fitplot){cout<<"no plot"<<endl;continue;}
//     Canvas.Clear();
//     fitplot->Draw();
//     Canvas.Print(filename);
   
    
//     if(!inputpars.OpenFitPars(TString(_OutputDir+"/DCosFits/FitPars_")+(long)i+"_D2600.txt"))continue;
//     //if(inputpars.GetFitPar("Range","status")!=0){cout<<"did not converge"<<endl;continue;}
//     if(inputpars.GetFitPar("Range","chisquare")/inputpars.GetFitPar("Range","ndf")>4){
//       cout<<"did not converge"<<endl;
//       continue;
//     }  
 
//     HYVsDCos.SetBinContent(i+1,inputpars.GetFitPar("D2600","yield"));
//     HYVsDCos.SetBinError(i+1,inputpars.GetFitPar("D2600","yielderror"));

//     HMVsDCos.SetBinContent(i+1,inputpars.GetFitPar("D2600","mean")*1000);
//     HMVsDCos.SetBinError(i+1,inputpars.GetFitPar("D2600","meanerror")*1000);

//     HWVsDCos.SetBinContent(i+1,inputpars.GetFitPar("D2600","width")*1000);
//     HWVsDCos.SetBinError(i+1,inputpars.GetFitPar("D2600","widtherror")*1000);

//     HBVsDCos.SetBinContent(i+1,inputpars.GetFitPar("Background","yield"));
//     //HBVsDCos.SetBinError(i+1,inputpars.GetFitPar("Background","yielderror"));

//     HChisqVsDCos.SetBinContent(i+1,inputpars.GetFitPar("Range","chisquare")/inputpars.GetFitPar("Range","ndf"));
    

//     inputpars.CloseFitPars();
//   }
  

  
//   Canvas.Clear();
//   HYVsDCos.GetYaxis()->SetRangeUser(0,1.1*HYVsDCos.GetMaximum());
//   HYVsDCos.SetStats(0);
//   HYVsDCos.Draw("pe");
//   Canvas.Print(filename);
//   HYVsDCos.Fit(FDCos,"I","",-1,1);
//   Canvas.Clear();
//   HYVsDCos.SetStats(0);
//   HYVsDCos.Draw("pe");
//   Canvas.Print(filename);


//   Canvas.Clear();
//   HMVsDCos.SetStats(0);
//   HMVsDCos.GetYaxis()->SetRangeUser(2550,2650);
//   HMVsDCos.Draw("pe");
//   Canvas.Print(filename);

//   Canvas.Clear();
//   HWVsDCos.SetStats(0);
//   HWVsDCos.GetYaxis()->SetRangeUser(50,150);
//   HWVsDCos.Draw("pe");
//   Canvas.Print(filename);

//   Canvas.Clear();
//   HBVsDCos.SetStats(0);
//   HBVsDCos.Draw("pe");
//   Canvas.Print(filename);


//   Canvas.Clear();
//   HChisqVsDCos.GetYaxis()->SetRangeUser(0,1.1*HChisqVsDCos.GetMaximum());
//   HChisqVsDCos.SetStats(0);
//   HChisqVsDCos.Draw("p");
//   Canvas.Print(filename);


//   Canvas.Print(filename+"]");
   		
//   file.Close();
  
//   return 1;
// }

// Int_t DCPiAnalysis::FitMassVsDCosFeed(){

//   TFile file(_OutputDir+"/HistosForFit.root","read");
//   gROOT->cd();
  
//   TFile outfile(_OutputDir+"/DCosFits/DCPiMassPlotsVsDCosFeed.root","update");
//   gROOT->cd();

		
//   for(Int_t i=0;i<10;i++){  
//     Bool_t fit=0;
//     if(!inputpars.OpenFitPars(TString(_OutputDir+"/DCosFits/FitPars_")+(long)i+"_Feed.txt"))
//       fit=1;
//     if(inputpars.GetFitPar("Range","status")!=0)
//       fit=1;
//     inputpars.CloseFitPars();

//     if(fit){
//       TH1F*HDCPiMass=(TH1F*)file.Get(TString("HDCPi_DCosCoarse")+(long)i);
//       if(!massfitter.Initialize(HDCPiMass,_OutputDir+"/DCosFits/InitRooFitParsFeed.txt"))continue;
//       massfitter.Fit();
//       massfitter.SetFileTag(TString("_")+(long)i+"_Feed");
//       massfitter.Save(_OutputDir+"/DCosFits");
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

// Int_t DCPiAnalysis::DrawMassVsDCosFeed(){

//   TFile file(_OutputDir+"/DCosFits/DCPiMassPlotsVsDCosFeed.root","read");
//   file.ls();	
//   gROOT->cd();
  

//   filename=_OutputDir+"/DCosFits/DCPiMassPlotsVsDCosFeed.ps";
//   Canvas.Print(filename+"["); 

//   TH1F HYVsDCos("HYVsDCos","",10,-1,1);
//   SetHistoXYLabels(&HYVsDCos,"cos(#theta_{D})","");
//   TH1F HMVsDCos("HMVsDCos","",10,-1,1);
//   SetHistoXYLabels(&HMVsDCos,"cos(#theta_{D})","");
//   TH1F HWVsDCos("HWVsDCos","",10,-1,1);
//   SetHistoXYLabels(&HWVsDCos,"cos(#theta_{D})","");
//   TH1F HBVsDCos("HBVsDCos","",10,-1,1);
//   SetHistoXYLabels(&HBVsDCos,"cos(#theta_{D})","");
//   TH1F HChisqVsDCos("HChisqVsDCos","",20,-1,1);
//   SetHistoXYLabels(&HChisqVsDCos,"cos(#theta_{D})","");  

//   for(Int_t i=0;i<10;i++){  
//     RooPlot* fitplot=(RooPlot*)file.Get(TString("fitplot")+(long)i);
//     if(!fitplot){cout<<"no plot"<<endl;continue;}
//     Canvas.Clear();
//     fitplot->Draw();
//     Canvas.Print(filename);
   
    
//     if(!inputpars.OpenFitPars(TString(_OutputDir+"/DCosFits/FitPars_")+(long)i+"_Feed.txt"))continue;
//     //if(inputpars.GetFitPar("Range","status")!=0){cout<<"did not converge"<<endl;continue;}
//     if(inputpars.GetFitPar("Range","chisquare")/inputpars.GetFitPar("Range","ndf")>4){
//       cout<<"did not converge"<<endl;
//       continue;
//     }  
 
//     HYVsDCos.SetBinContent(i+1,inputpars.GetFitPar("F1","yield"));
//     HYVsDCos.SetBinError(i+1,inputpars.GetFitPar("F1","yielderror"));

//     HMVsDCos.SetBinContent(i+1,inputpars.GetFitPar("F1","mean")*1000);
//     HMVsDCos.SetBinError(i+1,inputpars.GetFitPar("F1","meanerror")*1000);

//     HWVsDCos.SetBinContent(i+1,inputpars.GetFitPar("F1","width")*1000);
//     HWVsDCos.SetBinError(i+1,inputpars.GetFitPar("F1","widtherror")*1000);

//     HBVsDCos.SetBinContent(i+1,inputpars.GetFitPar("Background","yield"));
//     //HBVsDCos.SetBinError(i+1,inputpars.GetFitPar("Background","yielderror"));

//     HChisqVsDCos.SetBinContent(i+1,inputpars.GetFitPar("Range","chisquare")/inputpars.GetFitPar("Range","ndf"));
    

//     inputpars.CloseFitPars();
//   }
  

  
//   Canvas.Clear();
//   HYVsDCos.GetYaxis()->SetRangeUser(0,1.1*HYVsDCos.GetMaximum());
//   HYVsDCos.SetStats(0);
//   HYVsDCos.Draw("pe");
//   Canvas.Print(filename);
//   HYVsDCos.Fit(FDCos,"I","",-1,1);
//   Canvas.Clear();
//   HYVsDCos.SetStats(0);
//   HYVsDCos.Draw("pe");
//   Canvas.Print(filename);


//   Canvas.Clear();
//   HMVsDCos.SetStats(0);
//   HMVsDCos.GetYaxis()->SetRangeUser(2250,2300);
//   HMVsDCos.Draw("pe");
//   Canvas.Print(filename);

//   Canvas.Clear();
//   HWVsDCos.SetStats(0);
//   HWVsDCos.GetYaxis()->SetRangeUser(10,50);
//   HWVsDCos.Draw("pe");
//   Canvas.Print(filename);

//   Canvas.Clear();
//   HBVsDCos.SetStats(0);
//   HBVsDCos.Draw("pe");
//   Canvas.Print(filename);


//   Canvas.Clear();
//   HChisqVsDCos.GetYaxis()->SetRangeUser(0,1.1*HChisqVsDCos.GetMaximum());
//   HChisqVsDCos.SetStats(0);
//   HChisqVsDCos.Draw("p");
//   Canvas.Print(filename);


//   Canvas.Print(filename+"]");
   		
//   file.Close();
  
//   return 1;
// }

