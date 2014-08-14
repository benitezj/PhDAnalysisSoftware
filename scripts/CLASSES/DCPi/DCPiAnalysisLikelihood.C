
Int_t DCPiAnalysis::MakeLikelihood(){

  TH1* Histo=NULL;

  TFile fileL(_OutputDir+"/Likelihood.root","recreate");
  

  TFile fileM(_OutputDir+"/DMassPlot.root","read");
  fileL.cd();
  Histo=(TH1*)fileM.Get("HDmassSignal");
  Histo->Scale(1./Histo->Integral());
  Histo->Write();
  Histo=(TH1*)fileM.Get("HDmassBkg");
  Histo->Scale(1./Histo->Integral());
  Histo->Write();
  cout<<fileM.GetName()<<" Done"<<endl;
  fileM.Close();

  TFile fileF(_OutputDir+"/DFlightPlots.root","read");
  fileL.cd();
  Histo=(TH1*)fileF.Get("HFlightSignal");
  Histo->Scale(1./Histo->Integral());
  Histo->Write();
  Histo=(TH1*)fileF.Get("HFlightSBAvg");
  Histo->Scale(1./Histo->Integral());
  Histo->Write();
  cout<<fileF.GetName()<<" Done"<<endl;
  fileF.Close();

  TFile fileD(_OutputDir+"/DalitzPlot.root","read");
  fileL.cd();
  Histo=(TH1*)fileD.Get("HDalitzSignal"); 
  Histo->Scale(1./Histo->Integral());
  Histo->Write();
  Histo=(TH1*)fileD.Get("HDalitzSBAvg");
  Histo->Scale(1./Histo->Integral());
  Histo->Write();
  cout<<fileD.GetName()<<" Done"<<endl;
  fileD.Close();

  TFile filePC(_OutputDir+"/DCosXPstarPlot.root","read");
  fileL.cd();
  Histo=(TH1*)filePC.Get("HDCosXPstarBkg");
  Histo->Scale(1./Histo->Integral());
  Histo->Write();
  cout<<filePC.GetName()<<" Done"<<endl;
  filePC.Close();

  TFile fileC(_OutputDir+"/DCosPlot.root","read");
  fileL.cd();
  Histo=(TH1*)fileC.Get("HDCosSignal");
  Histo=(TH1*)Histo->Clone();
  Histo->SetName("HDCosSignal");
  for(Int_t i=1;i<=Histo->GetXaxis()->GetNbins();i++)
    Histo->SetBinContent(i,FDCos->Eval(Histo->GetXaxis()->GetBinCenter(i)));
  Histo->Scale(1./Histo->Integral());
  Histo->Write();
//   Histo=(TH1*)fileC.Get("HDCosSBAvg");
//   Histo->Scale(1./Histo->Integral());
//   Histo->Write();
  cout<<fileC.GetName()<<" Done"<<endl;
  fileC.Close();


  TFile fileP(_OutputDir+"/PstarPlot.root","read");
  fileL.cd();
  Histo=(TH1*)fileP.Get("HPstarSignal");
  Histo->SetName("HPstarSignal");
  Histo=(TH1*)Histo->Clone();
  for(Int_t i=1;i<=Histo->GetXaxis()->GetNbins();i++)
    Histo->SetBinContent(i,fragfitter.GetModel("Collins")->Eval(Histo->GetXaxis()->GetBinCenter(i)));
  Histo->Scale(1./Histo->Integral());
  Histo->Write();
 //  Histo=(TH1*)fileP.Get("HPstarSBAvg");
//   Histo->Scale(1./Histo->Integral());
//   Histo->Write();
  cout<<fileP.GetName()<<" Done"<<endl;
  fileP.Close();

  fileL.ls();
  fileL.Close();


  return 1;
}


Int_t DCPiAnalysis::PlotLikelihood(){


  filename=_OutputDir+"/Likelihood.ps";
  Canvas.Print(filename+"["); 


  TH1* Histo=NULL;

  TFile fileL(_OutputDir+"/Likelihood.root","read");
  fileL.ls();


  Canvas.Clear();
  Histo=(TH1*)fileL.Get("HDmassSignal");
  Histo->SetLineColor(2);
  Histo->Draw();
  Histo=(TH1*)fileL.Get("HDmassBkg");
  Histo->SetLineColor(4);
  Histo->Draw("same");
  Canvas.Print(filename);

  Canvas.Clear();
  Histo=(TH1*)fileL.Get("HFlightSBAvg");
  Histo->SetLineColor(4);
  Histo->Draw();
  Histo=(TH1*)fileL.Get("HFlightSignal");
  Histo->SetLineColor(2);
  Histo->Draw("same");
  Canvas.Print(filename);

  Canvas.Clear();
  Histo=(TH1*)fileL.Get("HDalitzSignal");
  Histo->Draw("colz"); 
  Canvas.Print(filename);
  Canvas.Clear();
  Histo=(TH1*)fileL.Get("HDalitzSBAvg");
  Histo->Draw("colz");
  Canvas.Print(filename);


  Canvas.Clear();
  Histo=(TH1*)fileL.Get("HDCosSignal");
  Histo->SetLineColor(2);
  Histo->Draw("l");
  Canvas.Print(filename);

  Canvas.Clear();
  Histo=(TH1*)fileL.Get("HPstarSignal");
  Histo->SetLineColor(2);
  Histo->Draw("l");
  Canvas.Print(filename);


  Canvas.Clear();
  Histo=(TH1*)fileL.Get("HDCosXPstarBkg");
  Histo->Draw("colz");
  Canvas.Print(filename);

  
  if(!OpenReducedNtuple())return 0;
  TH1F HLog("HLog","",300,-6,3);
  TH1F HLogLLSB("HLogLLSB","",300,-6,3);
  TH1F HLogHLSB("HLogHLSB","",300,-6,3);
//   TH1F HLogLHSB("HLogLHSB","",300,-6,3);
//   TH1F HLogHHSB("HLogHHSB","",300,-6,3);
  ReducedNtuple->Draw("xlogL>>HLog",topdatabase->GetCut("xmdm")/*+topdatabase->GetCut("dmasssig")*/);
  ReducedNtuple->Draw("xlogL>>HLogLLSB",topdatabase->GetCut("xmdmlsb")/*+topdatabase->GetCut("dmasslsb")*/);
  ReducedNtuple->Draw("xlogL>>HLogHLSB",topdatabase->GetCut("xmdmhsb")/*+topdatabase->GetCut("dmasslsb")*/);
//   ReducedNtuple->Draw("xlogL>>HLogLHSB",topdatabase->GetCut("xmdmlsb")+topdatabase->GetCut("dmasshsb"));
//   ReducedNtuple->Draw("xlogL>>HLogHHSB",topdatabase->GetCut("xmdmhsb")+topdatabase->GetCut("dmasshsb"));
  Canvas.Clear();
  HLogLLSB.Draw();
  HLogHLSB.Draw("same");  
//   HLogLHSB.Draw("same");  
//   HLogHHSB.Draw("same"); 
  
  Canvas.Print(filename); 

  HLogLLSB.Add(&HLogHLSB);
//   HLogLLSB.Add(&HLogLHSB);
//   HLogLLSB.Add(&HLogHHSB);
  Canvas.Clear();
  HLog.SetStats(0);
  HLog.Draw();
  HLogLLSB.Draw("same");
  Canvas.Print(filename);

  HLog.Add(&HLogLLSB,-1);
  Canvas.Clear();
  HLog.SetStats(1);
  HLog.Draw();
  cutline.DrawLine(HLog.GetXaxis()->GetXmin(),0,HLog.GetXaxis()->GetXmax(),0);
  Canvas.Print(filename);

  CloseReducedNtuple();
  

  Canvas.Print(filename+"]"); 
  fileL.Close();

  return 1;
}

Bool_t DCPiAnalysis::OpenLikelihood(){

  LikeFile=new TFile(_OutputDir+"/Likelihood.root","read");  
  gROOT->cd();

  HMLike=(TH1F*)LikeFile->Get("HDmassSignal");if(!HMLike)return 0;
  HFLike=(TH1F*)LikeFile->Get("HFlightSignal");if(!HFLike)return 0;
  HDLike=(TH2F*)LikeFile->Get("HDalitzSignal");if(!HDLike)return 0;
  HPLike=(TH1F*)LikeFile->Get("HPstarSignal");if(!HPLike)return 0;  
  HCLike=(TH1F*)LikeFile->Get("HDCosSignal");if(!HCLike)return 0;  

  HMLikeB=(TH1F*)LikeFile->Get("HDmassBkg");if(!HMLikeB)return 0;
  HFLikeB=(TH1F*)LikeFile->Get("HFlightSBAvg");if(!HFLikeB)return 0;
  HDLikeB=(TH2F*)LikeFile->Get("HDalitzSBAvg");if(!HDLikeB)return 0;
  HPCLikeB=(TH2F*)LikeFile->Get("HDCosXPstarBkg");if(!HPCLikeB)return 0;
  HPLikeB=(TH1F*)HPCLikeB->ProjectionX();
  HCLikeB=(TH1F*)HPCLikeB->ProjectionY();
  

  mlikemin=HMLike->GetXaxis()->GetXmin();
  mlikestep=HMLike->GetXaxis()->GetBinWidth(1);
 
  flikemin=HFLike->GetXaxis()->GetXmin();
  flikestep=HFLike->GetXaxis()->GetBinWidth(1);
    
  dlikemin=HDLike->GetXaxis()->GetXmin();
  dlikestep=HDLike->GetXaxis()->GetBinWidth(1);

  plikemin=HPLike->GetXaxis()->GetXmin();
  plikestep=HPLike->GetXaxis()->GetBinWidth(1);

  clikemin=HCLike->GetXaxis()->GetXmin();
  clikestep=HCLike->GetXaxis()->GetBinWidth(1);


  return 1;
}

Float_t DCPiAnalysis::GetLikelihood(Bool_t DMass,Bool_t Flight,Bool_t Dalitz,Bool_t Pstar,Bool_t Cos){
  
  Float_t BkgP=1;
  Float_t SigP=1;
  if(DMass&&HMLike)
    SigP*=HMLike->GetBinContent(1+(int)((dmass-mlikemin)/mlikestep));
  
  if(Flight&&HFLike)
    SigP*=HFLike->GetBinContent(1+(int)((dflights-flikemin)/flikestep));
  
  if(Dalitz&&HDLike)
    SigP*=HDLike->GetBinContent(1+(int)((Kpi2massFold-dlikemin)/dlikestep),1+(int)((KpimassFold-dlikemin)/dlikestep));
  
  if(Pstar&&HPLike)
    SigP*=HPLike->GetBinContent(1+(int)((xpstar-plikemin)/plikestep));
  
  if(Cos&&HCLike)
    SigP*=HCLike->GetBinContent(1+(int)((dcostheta-clikemin)/clikestep));



  if(DMass&&HMLikeB)
    BkgP*=HMLikeB->GetBinContent(1+(int)((dmass-mlikemin)/mlikestep));
  
  if(Flight&&HFLikeB)  
    BkgP*=HFLikeB->GetBinContent(1+(int)((dflights-flikemin)/flikestep));
  
  if(Dalitz&&HDLikeB)
    BkgP*=HDLikeB->GetBinContent(1+(int)((Kpi2mass-dlikemin)/dlikestep),1+(int)((Kpimass-dlikemin)/dlikestep));
  
  if(Cos&&Pstar&&HPCLikeB)
    BkgP*=HPCLikeB->GetBinContent(1+(int)((xpstar-plikemin)/plikestep),1+(int)((dcostheta-clikemin)/clikestep));
  else if(Pstar&&HPLikeB)
    BkgP*=HPLikeB->GetBinContent(1+(int)((xpstar-plikemin)/plikestep));  
  else if(Cos&&HCLikeB)
    BkgP*=HCLikeB->GetBinContent(1+(int)((dcostheta-clikemin)/clikestep));

  
  if(BkgP>1e-200)
    return log10(SigP/BkgP);
  
  return -300;
}
void DCPiAnalysis::CloseLikelihood(){

  LikeFile->Close();
  delete LikeFile;
}



Int_t DCPiAnalysis::MakeDCPiVsLogL(){



  if(OpenReducedNtuple()!=1) return 0;
  
  Int_t N=20;

  TFile file(_OutputDir+"/DCPiVsLogL.root","recreate");
  TH1F* H;
  for(Int_t i=0;i<N;i++){
    sprintf(textlabel,"xlogL>%.3f",-2.+4.*((float)i)/N);
    H=new TH1F(TString("H")+(long)i,textlabel,1000,2,3);    
    ReducedNtuple->Draw(TString("xmdm>>H")+(long)i,textlabel);
    H->Write();
  }
  
  file.ls();
  file.Close();
  
  if(CloseReducedNtuple()!=1) return 0;
   
  return 1;
}



Int_t DCPiAnalysis::PlotDCPiVsLogL(){

  filename=_OutputDir+"DCPiVsLogL.ps";
  Canvas.Print(filename+"[");
  

  MassFitter massfit(&mass);
  massfit.SetPhaseSpace(&TwoBodyPS);
  massfit.SetResolution(respdf,&resVar);
  RooPlot*fitplot;


  //calculate the bkg yield under the signal;
  RooAbsPdf*bkg;
  //RooAbsPdf*sigpdf;
  RooAbsReal* integral;

 
  TGraph GSig;
  TGraph GBkg;
  TGraph GS;
  TGraph GR;

  TFile file(_OutputDir+"/DCPiVsLogL.root","read");
  file.ls();
  gROOT->cd();
  TH1F* H;
  Int_t i=0;
  while((H=(TH1F*)file.Get(TString("H")+(long)i))){
 
    
    if(!massfit.Initialize(H,_OutputDir+"/InitRooFitPars.txt"))return 0;
    Int_t status=massfit.Fit();
    if(status!=0)massfit.Fit();
    massfit.Save(_OutputDir);


    bkg=massfit.GetBkgPdf();
    mass.setRange("sigregion",((DataBaseDCPi*)topdatabase)->GetDCPiMassMin(),((DataBaseDCPi*)topdatabase)->GetDCPiMassMax());
    integral= bkg->createIntegral(mass,"sigregion");
    cout<<integral->getVal()<<endl;

    GBkg.SetPoint(i,-2+i*.2,massfit.GetSigYield());
    GSig.SetPoint(i,-2+i*.2,integral->getVal());
    GS.SetPoint(i,-2+i*.2,massfit.GetSigYield()/sqrt(massfit.GetSigYield()+integral->getVal()));
    GR.SetPoint(i,-2+i*.2,massfit.GetSigYield()/(integral->getVal()+.01));

    Canvas.Clear();
    fitplot=massfit.GetFitPlot();
    fitplot->SetTitle(H->GetTitle());
    fitplot->Draw();
    Canvas.Print(filename);

    i++;
  }
  
  Canvas.Clear();
  GSig.Draw("ape");
  Canvas.Print(filename);
  Canvas.Clear();
  GBkg.Draw("ape");
  Canvas.Print(filename);
  Canvas.Clear();
  GS.Draw("ape");
  Canvas.Print(filename);
  Canvas.Clear();
  GR.Draw("ape");
  Canvas.Print(filename);


  Canvas.Print(filename+"]");
 
  file.Close();

  return 1;
}


