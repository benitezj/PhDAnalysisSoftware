


Int_t DCPiAnalysis::MakeOptimizeCuts(){

  if(OpenReducedNtuple()!=1) return 0;
  ReducedNtuple->SetBranchStatus("*",0);
  ReducedNtuple->SetBranchStatus("xmdm",1);
  ReducedNtuple->SetBranchStatus("xpstar",1);
  ReducedNtuple->SetBranchStatus("dflights",1);
  ReducedNtuple->SetBranchStatus("dmass",1);
  ReducedNtuple->SetBranchStatus("dcostheta",1);
   
  TTree*sigt=ReducedNtuple->CopyTree(topdatabase->GetCut("xmdmcutopt")+TCut("abs(dmass-1.8693)<.030"));
  TTree*bkgt=ReducedNtuple->CopyTree(topdatabase->GetCut("xmdmsbcutopt1")+TCut("abs(dmass-1.8693)<.030"));
  TTree*bkgt2=ReducedNtuple->CopyTree(topdatabase->GetCut("xmdmsbcutopt2")+TCut("abs(dmass-1.8693)<.030"));
  TTree*bkgt3=ReducedNtuple->CopyTree(topdatabase->GetCut("xmdmsbcutopt3")+TCut("abs(dmass-1.8693)<.030"));
  
  //cuts to be fixed
  RooRealVar pmax("pmax","xpstar",5.,5.5);pmax.setVal(5.5);pmax.setConstant(1);
  RooRealVar fmax("fmax","dflights",50,50);fmax.setVal(50.);fmax.setConstant(1);
  RooRealVar mmin("mmin","abs(dmass-1.8693)",0.,0.);mmin.setVal(0.);mmin.setConstant(1);
  RooRealVar cmin("cmin","dcostheta",-1.,-1.);cmin.setVal(-1.);cmin.setConstant(1);

  ///Variables for Minuit Optimization 
  ///cuts to be varied, Minuit step=.1*err
  RooRealVar pmin("pmin","xpstar",2.2,5.);pmin.setError(.1);pmin.setVal(3.0);//pmin.setConstant(1);  
  RooRealVar fmin("fmin","dflights",-3.,9.);fmin.setError(1.);fmin.setVal(0.);//fmin.setConstant(1);
  RooRealVar mmax("mmax","abs(dmass-1.8693)",.001,.030);mmax.setError(.001);mmax.setVal(.03);//mmax.setConstant(1);
  RooRealVar cmax("cmax","dcostheta",-.9,1.);cmax.setError(.1);cmax.setVal(1.);//cmax.setConstant(1);

  RooArgList cutvars;//order matters here,   
  cutvars.add(pmin); cutvars.add(pmax);
  cutvars.add(fmin); cutvars.add(fmax);
  cutvars.add(mmin); cutvars.add(mmax);
  cutvars.add(cmin); cutvars.add(cmax);

  //the dataset defines the variable (mass) to be excluded from the optimization, it has no other purpose
  mass.setRange(1.9,2.9);
  mass.setBins(1000);
  TH1F Histo("Histo","",1000,1.9,2.9);
  RooDataHist dataset("dataset","dataset",RooArgList(mass),&Histo); 
  //the pdf needs the mass variable to go with the dataset, cut variables need to be included here. (order doesnt matter)
  RooArgSet pdfvars(cutvars);
  pdfvars.add(mass);
  RooGenericPdf pdf("pdf","pow((pmin+pmax+fmin+fmax+mmin+mmax+cmin+cmax),2)*mass",pdfvars);
  RooCutOptimizer optimizer("optim","optim",pdf,dataset,sigt,bkgt,bkgt2,bkgt3,&cutvars,RooFit::DataError(RooAbsData::SumW2),RooFit::NumCPU(1));  

  
  ///Minuit Optimization does not work!! Because there are local minima and plateaus
  //RooMinuit minuit(optimizer);
  //minuit.setVerbose(1);//prints prevFCN
  //minuit.hesse();//important otherwise minuit gets stuck at initial values
  //minuit.setStrategy(2);//better derivative
  //minuit.setEps(1e-6);
  //minuit.migrad();
  

  ///Make Visual optimization. By looking at Significance Vs. each cut variable 
  //must not be run in batch mode so one can see the cuts graphs, also if one covers the canvas the plot will clear:
  TFile FScanCuts(_OutputDir+"/ScanCuts.root","recreate");
  gROOT->cd();
  Canvas.Print(filename+"[");
  TIterator*iter=cutvars.createIterator();
  for(Int_t i=1;i<=cutvars.getSize();i++){  
    RooRealVar* scanvar=(RooRealVar*)iter->Next();
    if(!scanvar->isConstant()){
      TGraph*GScan=optimizer.ScanVariable(scanvar);      

      Canvas.Clear();
      GScan->Draw("ap");
      Canvas.Print(filename);
      Canvas.Flush();//must Flush in order to be able to see it interactively
 
      cout<<" Enter new value for cut on "<<scanvar->GetTitle()<<endl;
      Float_t newcut;
      cin>>newcut;
      scanvar->setVal(newcut);

      FScanCuts.cd();
      GScan->SetName(scanvar->GetName());
      GScan->SetTitle(scanvar->GetTitle());
      GScan->Write();
      gROOT->cd();

      delete GScan;
    }
  }
  Canvas.Print(filename+"]");
  FScanCuts.ls();
  FScanCuts.Close();

  ofstream ParameterOutput;
  ParameterOutput.open((const char*)(_OutputDir+"/OptimizeCuts.txt"));
  ParameterOutput<<"cut"<<" "<<pmin.GetName()<<" "<<pmin.getVal()<<endl;
  ParameterOutput<<"cut"<<" "<<pmax.GetName()<<" "<<pmax.getVal()<<endl;
  ParameterOutput<<"cut"<<" "<<fmin.GetName()<<" "<<fmin.getVal()<<endl;
  ParameterOutput<<"cut"<<" "<<fmax.GetName()<<" "<<fmax.getVal()<<endl;
  ParameterOutput<<"cut"<<" "<<mmin.GetName()<<" "<<mmin.getVal()<<endl;
  ParameterOutput<<"cut"<<" "<<mmax.GetName()<<" "<<mmax.getVal()<<endl;
  ParameterOutput<<"cut"<<" "<<cmin.GetName()<<" "<<cmin.getVal()<<endl;
  ParameterOutput<<"cut"<<" "<<cmax.GetName()<<" "<<cmax.getVal()<<endl;
     
  ParameterOutput<<"END"<<endl;
  ParameterOutput.close();
  ParameterOutput.clear();

  delete sigt;
  delete bkgt;
  delete bkgt2;
  delete bkgt3;

  if(CloseReducedNtuple()!=1) return 0;
   
  return 1;
}


Int_t DCPiAnalysis::PlotOptimizeCuts(){
  

  if(OpenReducedNtuple()!=1) return 0;
  
  filename=_OutputDir+"/OptimizeCuts.ps";
  Canvas.Print(filename+"[");
 

  //validation histos
  TH1F HAll("HAll","",300,2.0,3.5);
  TH1F HSig("HSig","",300,2.0,3.5);
  TH1F HSB("HSB","",300,2.0,3.5);
  TH1F HSB2("HSB2","",300,2.0,3.5);
  TH1F HSB3("HSB3","",300,2.0,3.5);
  ReducedNtuple->Draw("xmdm>>HAll","","pe");
  ReducedNtuple->Draw("xmdm>>HSig",topdatabase->GetCut("xmdmcutopt"));
  ReducedNtuple->Draw("xmdm>>HSB",topdatabase->GetCut("xmdmsbcutopt1"));
  ReducedNtuple->Draw("xmdm>>HSB2",topdatabase->GetCut("xmdmsbcutopt2"));
  ReducedNtuple->Draw("xmdm>>HSB3",topdatabase->GetCut("xmdmsbcutopt3"));
  Canvas.Clear();
  SetHistoXYLabels(&HAll,"m(D#pi)","(GeV/c^{2})");
  HAll.Draw("pe");
  HSig.SetFillColor(2);
  HSig.Draw("same");
  HSB.SetFillColor(4);
  HSB.Draw("same");
  HSB2.SetFillColor(4);
  HSB2.Draw("same");
  HSB3.SetFillColor(4);
  HSB3.Draw("same");
  Canvas.Print(filename);




  inputpars.OpenFitPars((const char*)(_OutputDir+"/OptimizeCuts.txt"));
  
  TFile FScanCuts(_OutputDir+"/ScanCuts.root","read");
  FScanCuts.ls();
  TList* graphs=FScanCuts.GetListOfKeys();
  graphs->Print();
  TIterator*iter=graphs->MakeIterator();
  while(TObject*key=(TObject*)iter->Next()){
    TGraph*G=(TGraph*)FScanCuts.Get(key->GetName());    
    if(!G){cout<<"Graph: "<<key->GetName()<<"  Not found"<<endl;continue;}
    cout<<G->GetName()<<endl;
        
    Float_t cutval=inputpars.GetFitPar("cut",G->GetName());
    
    Canvas.Clear();
    G->GetYaxis()->SetRangeUser(0,G->GetYaxis()->GetXmax());
    G->SetTitle("");
    G->Draw("ap");
    Canvas.Update();
    Canvas.Flush();
    cutline.DrawLine(cutval,0,cutval,G->GetYaxis()->GetXmax());
    Canvas.Print(filename);
    
  }
  FScanCuts.Close();
  

  

  
  Float_t pmin=inputpars.GetFitPar("cut","pmin");
  Float_t fmin=inputpars.GetFitPar("cut","fmin");
  Float_t mmax=inputpars.GetFitPar("cut","mmax");
  Float_t cmax=inputpars.GetFitPar("cut","cmax");

  sprintf(textlabel,"%.3f<xpstar",pmin);
  TCut pcut=TCut(textlabel);
  sprintf(textlabel,"%.3f<dflights",fmin);
  TCut fcut=TCut(textlabel);
  sprintf(textlabel,"abs(dmass-%.6f)<%.4f",DPlusPDGMass,mmax);
  TCut mcut=TCut(textlabel);
  sprintf(textlabel,"dcostheta<%.3f",cmax);
  TCut ccut=TCut(textlabel);  

  Float_t DSBMin=.040;
  sprintf(textlabel,"abs(dmass-%.6f)<%.4f",DPlusPDGMass,.03);
  TCut mcutraw=TCut(textlabel);
  sprintf(textlabel,"%.3f<abs(dmass-%.6f)&&abs(dmass-%.6f)<%.3f",DSBMin,DPlusPDGMass,DPlusPDGMass,DSBMin+.03);
  TCut msbcutraw=TCut(textlabel);
  sprintf(textlabel,"%.3f<abs(dmass-%.6f)&&abs(dmass-%.6f)<%.3f",DSBMin,DPlusPDGMass,DPlusPDGMass,DSBMin+mmax);
  TCut msbcut=TCut(textlabel);


  TCut FlightPlotCut=pcut;
  TCut DMassPlotCut=pcut+fcut;
  TCut DCosPlotCut=pcut+fcut+mcut;
  TCut AllCuts=pcut+mcut+fcut+ccut;


  ////------------------------------------------
  //The D mass: Before the Flight cut
  ///-------------------------------------------
  TH1F HDMass("HDMass","",((DataBaseDC*)topdatabase)->GetDMassRangeNbins(),((DataBaseDC*)topdatabase)->GetDMassRangeMin(),((DataBaseDC*)topdatabase)->GetDMassRangeMax());
  ReducedNtuple->Draw("dmass>>HDMass",pcut);
 
  if(!massfitter.Initialize(&HDMass,_OutputDir+"/InitRooFitParsDMass.txt"))return 0;
  if(massfitter.Fit()!=0)return 0;
  RooAbsPdf*bkgraw=massfitter.GetBkgPdf();
  mass.setRange("sigregion",DPlusPDGMass-.03,DPlusPDGMass+.03);
  RooAbsReal* integralraw= bkgraw->createIntegral(mass,"sigregion");
  cout<<"SigYield="<<massfitter.GetSigYield()<<" totbkg="<<massfitter.GetBkgYield()<<" bkgundersig"<<integralraw->getVal()*massfitter.GetBkgYield()<<endl;
 
  Canvas.Clear();
  RooPlot*fitplot=massfitter.GetFitPlot();
  fitplot->GetXaxis()->SetTitle("m(K#pi#pi)  (GeV/c^{2})");
  fitplot->Draw();
  cutline.DrawLine(DPlusPDGMass-.03,0,DPlusPDGMass-.03,HDMass.GetMaximum());
  cutline.DrawLine(DPlusPDGMass+.03,0,DPlusPDGMass+.03,HDMass.GetMaximum());
  HDMass.SetFillColor(3);
  ReducedNtuple->Draw("dmass>>HDMass",pcut+msbcutraw,"same");
  Canvas.Print(filename);  
 
 

  //-----------------------------------------------
  //The D flight
  //------------------------------------------------
  TH1F HDFlight("HDFlight","",400,-10,30);
  ReducedNtuple->Draw("dflights>>HDFlight",FlightPlotCut+mcutraw);//does not have flight cut
 
  TH1F HDFlightSB("HDFlightSB","",400,-10,30);
  ReducedNtuple->Draw("dflights>>HDFlightSB",FlightPlotCut+msbcutraw);
  HDFlightSB.Scale(integralraw->getVal()*massfitter.GetBkgYield()/HDFlightSB.Integral());
  TH1F*HClone=(TH1F*)HDFlight.Clone();
  HClone->Add(&HDFlightSB,-1);
 
  Canvas.Clear();
  SetHistoXYLabels(&HDFlight,"D signed flight significance","");
  HDFlight.SetTitle("");
  HDFlight.SetStats(0);
  HDFlight.SetLineColor(4);
  HDFlight.Draw();
  HDFlightSB.SetLineColor(3);
  HDFlightSB.Draw("same");
  HClone->SetLineColor(2);
  HClone->Draw("same");
  cutline.DrawLine(fmin,0,fmin,HDFlight.GetMaximum());
  Canvas.Print(filename);  
  delete HClone;

  ////------------------------------------------
  //The D mass:: 
  ///-------------------------------------------
  TH1F HDMassPlot("HDMassPlot","",((DataBaseDC*)topdatabase)->GetDMassRangeNbins(),((DataBaseDC*)topdatabase)->GetDMassRangeMin(),((DataBaseDC*)topdatabase)->GetDMassRangeMax());
  ReducedNtuple->Draw("dmass>>HDMassPlot",DMassPlotCut);
    
  if(!massfitter.Initialize(&HDMassPlot,_OutputDir+"/InitRooFitParsDMass.txt"))return 0;
  if(massfitter.Fit()!=0)return 0;
  RooAbsPdf*dmassbkg=massfitter.GetBkgPdf();
  mass.setRange("sigregion",DPlusPDGMass-mmax,DPlusPDGMass+mmax);
  RooAbsReal* dmassintegral= dmassbkg->createIntegral(mass,"sigregion");
  cout<<"SigYield="<<massfitter.GetSigYield()<<" totbkg="<<massfitter.GetBkgYield()<<" bkgundersig"<<dmassintegral->getVal()*massfitter.GetBkgYield()<<endl;
  
  Canvas.Clear();
  fitplot=massfitter.GetFitPlot();
  fitplot->GetXaxis()->SetTitle("m(K#pi#pi)  (GeV/c^{2})");
  fitplot->Draw();
  cutline.DrawLine(DPlusPDGMass-mmax,0,DPlusPDGMass-mmax,HDMassPlot.GetMaximum());
  cutline.DrawLine(DPlusPDGMass+mmax,0,DPlusPDGMass+mmax,HDMassPlot.GetMaximum());
  Canvas.Print(filename);  
 

  
  ////////////////--------------------------
  ///Plot the  final DCos distribution
  ////--------------------------------------
  TH1F HDCosFinal("HDCosFinal","",40,-1,1);
  HDCosFinal.SetStats(0);
  ReducedNtuple->Draw("dcostheta>>HDCosFinal",pcut+fcut+mcut);
  Canvas.Clear();
  //HDCosFinal.SetLineColor(2);
  SetHistoXYLabels(&HDCosFinal,"cos(#theta_{D})","");
  HDCosFinal.Draw();
  cutline.DrawLine(cmax,0,cmax,HDCosFinal.GetMaximum());
  Canvas.Print(filename);  

  //-------------------------------------------------------------------------
  //The DPi mass with final cuts
  //-------------------------------------------------------------------------
  TH1F HDMassFinal("HDMassFinal","",((DataBaseDC*)topdatabase)->GetDMassRangeNbins(),((DataBaseDC*)topdatabase)->GetDMassRangeMin(),((DataBaseDC*)topdatabase)->GetDMassRangeMax());
  ReducedNtuple->Draw("dmass>>HDMassFinal",DMassPlotCut+ccut);
    
  if(!massfitter.Initialize(&HDMassFinal,_OutputDir+"/InitRooFitParsDMass.txt"))return 0;
  if(massfitter.Fit()!=0)return 0;
  RooAbsPdf*bkg=massfitter.GetBkgPdf();
  mass.setRange("sigregion",DPlusPDGMass-mmax,DPlusPDGMass+mmax);
  RooAbsReal* integral= bkg->createIntegral(mass,"sigregion");
  cout<<"SigYield="<<massfitter.GetSigYield()<<" totbkg="<<massfitter.GetBkgYield()<<" bkgundersig"<<integral->getVal()*massfitter.GetBkgYield()<<endl;
  
  Canvas.Clear();
  fitplot=massfitter.GetFitPlot();
  fitplot->GetXaxis()->SetTitle("m(K#pi#pi)  (GeV/c^{2})");
  fitplot->Draw();
  cutline.DrawLine(DPlusPDGMass-mmax,0,DPlusPDGMass-mmax,HDMassFinal.GetMaximum());
  cutline.DrawLine(DPlusPDGMass+mmax,0,DPlusPDGMass+mmax,HDMassFinal.GetMaximum());
  HDMassFinal.SetFillColor(3);
  ReducedNtuple->Draw("dmass>>HDMassFinal",DMassPlotCut+ccut+msbcut,"same");
  Canvas.Print(filename);

  TH1F HSigPCut("HSigPCut","",300,2.0,3.5);  
  TH1F HSigFCut("HSigFCut","",300,2.0,3.5);
  TH1F HSigMCut("HSigMCut","",300,2.0,3.5);
  TH1F HSigClean("HSigClean","",300,2.0,3.5); 
  TH1F HDSB("HDSB","",300,2.0,3.5);  
  ReducedNtuple->Draw("xmdm>>HSigPCut",pcut);
  ReducedNtuple->Draw("xmdm>>HSigFCut",pcut+fcut);
  ReducedNtuple->Draw("xmdm>>HSigMCut",pcut+fcut+mcut);
  ReducedNtuple->Draw("xmdm>>HSigClean",pcut+fcut+mcut+ccut);
  ReducedNtuple->Draw("xmdm>>HDSB",pcut+fcut+ccut+msbcut);//dmass sideband
  Canvas.Clear();
  HAll.GetYaxis()->SetRangeUser(HDSB.GetMaximum()*.1,HAll.GetMaximum()*1.1);  
  HAll.Draw("");
  HSigPCut.Draw("same");
  HSigFCut.Draw("same");
  HSigMCut.Draw("same");
  HSigClean.SetLineColor(2);HSigClean.Draw("same");
  HDSB.Scale(integral->getVal()*massfitter.GetBkgYield()/HDSB.Integral());
  HDSB.SetLineColor(3);
  HDSB.Draw("same");
  Canvas.SetLogy(1);
  Canvas.Print(filename);
  Canvas.SetLogy(0);


  

  Canvas.Print(filename+"]");
   
  inputpars.CloseFitPars();

  if(CloseReducedNtuple()!=1) return 0;

   
  return 1;
}


// Int_t DCPiAnalysis::NtupleFinal(){
  

//   if(OpenReducedNtuple("Ntuple")!=1) return 0;
  

//   inputpars.OpenFitPars((const char*)(_OutputDir+"/OptimizeCuts.txt"));
  
  
//   Float_t mmax=inputpars.GetFitPar("cut","mmax");
//   Float_t fmin=inputpars.GetFitPar("cut","fmin");
//   Float_t pmin=inputpars.GetFitPar("cut","pmin");
//   Float_t cmax=inputpars.GetFitPar("cut","cmax");
//   Float_t vmin=inputpars.GetFitPar("cut","vmin");

//   sprintf(textlabel,"abs(dmass-%.6f)<%.4f",DPlusPDGMass,mmax);
//   TCut mcut=TCut(textlabel);
//   sprintf(textlabel,"%.3f<dflights",fmin);
//   TCut fcut=TCut(textlabel);
//   sprintf(textlabel,"%.3f<xpstar",pmin);
//   TCut pcut=TCut(textlabel);
//   sprintf(textlabel,"dcostheta<%.3f",cmax);
//   TCut ccut=TCut(textlabel);  
//   sprintf(textlabel,"%.3f<xlogvtxprob",vmin);
//   TCut vcut=TCut(textlabel);

//   TCut AllCuts=pcut+mcut+fcut+ccut;

//   ///-------------------------------------------
//   cout<<"Create Final Clean Ntuple with final cuts"<<endl;
//   ///------------------------------------------------
//   AllCuts.Print();

//   TFile temp(_OutputDir+"/NtupleFinal.root","recreate"); 
//   //TFile temp(_OutputDir+"/NtupleFinalFullDCos.root","recreate"); 
//   //TFile temp(_OutputDir+"/NtupleFinalFullPstar.root","recreate"); 
//   Long64_t maxsize=5000000000;
//   cout<<"file size "<<maxsize<<endl;
//   ReducedNtuple->SetMaxTreeSize(maxsize);
//   ReducedNtuple->SetAutoSave(maxsize);
//   TTree*Ntuple=ReducedNtuple->CopyTree((const char*)AllCuts,"",1000000000,0);   
//   Ntuple->SetName("Ntuple");
//   cout<<"Entries in clean ntuple "<<Ntuple->GetEntries()<<endl;
//   Ntuple->Write();
//   temp.ls();
//   temp.Close();


//   if(CloseReducedNtuple()!=1) return 0;

   
//   return 1;
// }




Int_t DCPiAnalysis::MakeHistosForFit(){

  if(OpenReducedNtuple()!=1) return 0;  
  SetDCPiReducedNtupleBranches();
  ///-------------------------------------------
  cout<<"Create root file with histos for fit."<<endl;
  ///------------------------------------------------

  if(!OpenEfficiency()) return 0;

  TFile temp(_OutputDir+"/HistosForFit.root","recreate"); 

  TH1F HDCPi("HDCPi","",2000,1.9,3.9);

  TH1F* HDCPiVsDCos[20];
  for(Int_t i=0;i<20;i++)
    HDCPiVsDCos[i]=new TH1F(TString("HDCPi_DCos")+(long)i,"",2000,1.9,3.9);

  TH1F* HDCPiVsDCosCoarse[10];
  for(Int_t i=0;i<10;i++)
    HDCPiVsDCosCoarse[i]=new TH1F(TString("HDCPi_DCosCoarse")+(long)i,"",2000,1.9,3.9);

  TH1F* HDCPiVsDCosCoarsePstar[10][4];
  for(Int_t i=0;i<10;i++)
    for(Int_t j=0;j<4;j++)
      HDCPiVsDCosCoarsePstar[i][j]=new TH1F(TString("HDCPi_DCosCoarse")+(long)i+"_Pstar"+(long)j,"",2000,1.9,3.9);

  TH1F* HDCPiVsCos[20];
  for(Int_t i=0;i<20;i++)
    HDCPiVsCos[i]=new TH1F(TString("HDCPi_Cos")+(long)i,"",2000,1.9,3.9);

  TH1F* HDCPiVsCosCoarse[10];
  for(Int_t i=0;i<10;i++)
    HDCPiVsCosCoarse[i]=new TH1F(TString("HDCPi_CosCoarse")+(long)i,"",2000,1.9,3.9);

  TH1F* HDCPiVsCone[10];
  for(Int_t i=0;i<10;i++)
    HDCPiVsCone[i]=new TH1F(TString("HDCPi_Cone")+(long)i,"",2000,1.9,3.9);

  TH1F* HDCPiVsConeCoarse[5];
  for(Int_t i=0;i<5;i++)
    HDCPiVsConeCoarse[i]=new TH1F(TString("HDCPi_ConeCoarse")+(long)i,"",2000,1.9,3.9);

  TH1F* HDCPiVsPstar[16];
  for(Int_t i=0;i<16;i++)
    HDCPiVsPstar[i]=new TH1F(TString("HDCPi_Pstar")+(long)i,"",2000,1.9,3.9);

  TH1F* HDCPiVsPstarCoarse[8];
  for(Int_t i=0;i<8;i++)
    HDCPiVsPstarCoarse[i]=new TH1F(TString("HDCPi_PstarCoarse")+(long)i,"",2000,1.9,3.9);

  TH1F* HDCPiVsPstarCoarseDCos[8][4];
  for(Int_t i=0;i<8;i++)
    for(Int_t j=0;j<4;j++)
      HDCPiVsPstarCoarseDCos[i][j]=new TH1F(TString("HDCPi_PstarCoarse")+(long)i+"_DCos"+(long)j,"",2000,1.9,3.9);


  TH1F* HDCPiVsPstarCut[16];
  for(Int_t i=0;i<16;i++)
    HDCPiVsPstarCut[i]=new TH1F(TString("HDCPi_PstarCut")+(long)i,"",2000,1.9,3.9);

  
  
  ///Efficiency corrected
  TH1F HDCPiEff("HDCPiEff","",2000,1.9,3.9);
  HDCPiEff.Sumw2();
  Float_t HDCPiEffNorm=0;

  TH1F* HDCPiVsDCosEff[20];
  Float_t HDCPiVsDCosEffNorm[20];
  for(Int_t i=0;i<20;i++){
    HDCPiVsDCosEff[i]=new TH1F(TString("HDCPiEff_DCos")+(long)i,"",2000,1.9,3.9);
    HDCPiVsDCosEff[i]->Sumw2();
    HDCPiVsDCosEffNorm[i]=0;
  }

  TH1F* HDCPiVsDCosCoarseEff[10];
  Float_t HDCPiVsDCosCoarseEffNorm[10];
  for(Int_t i=0;i<10;i++){
    HDCPiVsDCosCoarseEff[i]=new TH1F(TString("HDCPiEff_DCosCoarse")+(long)i,"",2000,1.9,3.9);
    HDCPiVsDCosCoarseEff[i]->Sumw2();
    HDCPiVsDCosCoarseEffNorm[i]=0;
  }
  
  TH1F* HDCPiVsDCosCoarsePstarEff[10][4];
  Float_t HDCPiVsDCosCoarsePstarEffNorm[10][4];
  for(Int_t i=0;i<10;i++)
    for(Int_t j=0;j<4;j++){
      HDCPiVsDCosCoarsePstarEff[i][j]=new TH1F(TString("HDCPiEff_DCosCoarse")+(long)i+"_Pstar"+(long)j,"",2000,1.9,3.9);
      HDCPiVsDCosCoarsePstarEff[i][j]->Sumw2();
      HDCPiVsDCosCoarsePstarEffNorm[i][j]=0;
    }

  TH1F* HDCPiVsPstarEff[16];
  Float_t HDCPiVsPstarEffNorm[16];        
  for(Int_t i=0;i<16;i++){
    HDCPiVsPstarEff[i]=new TH1F(TString("HDCPiEff_Pstar")+(long)i,"",2000,1.9,3.9);
    HDCPiVsPstarEff[i]->Sumw2();
    HDCPiVsPstarEffNorm[i]=0;
  }

  TH1F* HDCPiVsPstarCoarseEff[8];
  Float_t HDCPiVsPstarCoarseEffNorm[8];
  for(Int_t i=0;i<8;i++){
    HDCPiVsPstarCoarseEff[i]=new TH1F(TString("HDCPiEff_PstarCoarse")+(long)i,"",2000,1.9,3.9);
    HDCPiVsPstarCoarseEff[i]->Sumw2();
    HDCPiVsPstarCoarseEffNorm[i]=0;
  }


  TH1F* HDCPiVsPstarCoarseDCosEff[8][4];
  Float_t HDCPiVsPstarCoarseDCosEffNorm[8][4];
  for(Int_t i=0;i<8;i++)
    for(Int_t j=0;j<4;j++){
      HDCPiVsPstarCoarseDCosEff[i][j]=new TH1F(TString("HDCPiEff_PstarCoarse")+(long)i+"_DCos"+(long)j,"",2000,1.9,3.9);
      HDCPiVsPstarCoarseDCosEff[i][j]->Sumw2();
      HDCPiVsPstarCoarseDCosEffNorm[i][j]=0;
    }

  TH1F* HDCPiVsPstarCutEff[16];
  Float_t HDCPiVsPstarCutEffNorm[16];
  for(Int_t i=0;i<16;i++){
    HDCPiVsPstarCutEff[i]=new TH1F(TString("HDCPiEff_PstarCut")+(long)i,"",2000,1.9,3.9);
    HDCPiVsPstarCutEffNorm[i]=0;
  }


  ///
  Int_t e=0;
  while(ReducedNtuple->GetEntry(e)){
    e++;
    if(e%50000==0)cout<<e<<" cands done"<<endl;
    
    HDCPi.Fill(xmdm); 
        
    for(Int_t i=0;i<20;i++)
      if((-1.+i*.1)<dcostheta&&dcostheta<(-1.+(i+1)*.1))
	HDCPiVsDCos[i]->Fill(xmdm);

    for(Int_t i=0;i<10;i++)
      if((-1.+i*.2)<dcostheta&&dcostheta<(-1.+(i+1)*.2))
	HDCPiVsDCosCoarse[i]->Fill(xmdm);
    
    for(Int_t i=0;i<10;i++)
      for(Int_t j=0;j<4;j++)
	if((-1.+i*.2)<dcostheta&&dcostheta<(-1.+(i+1)*.2))
	  if((databaseDCPi.GetDCPiPstarCut()+j*.4)<xpstar&&xpstar<(databaseDCPi.GetDCPiPstarCut()+(j+1)*.4))
	    HDCPiVsDCosCoarsePstar[i][j]->Fill(xmdm);
	
    for(Int_t i=0;i<20;i++)
      if((-1.+i*.1)<xcosstar&&xcosstar<(-1.+(i+1)*.1))
	HDCPiVsCos[i]->Fill(xmdm);

    for(Int_t i=0;i<10;i++)
      if((-1.+i*.2)<xcosstar&&xcosstar<(-1.+(i+1)*.2))
	HDCPiVsCosCoarse[i]->Fill(xmdm);

   for(Int_t i=0;i<10;i++)
      if((0.+i*.1)<pi1mothcosine&&pi1mothcosine<(0.+(i+1)*.1))
	HDCPiVsCone[i]->Fill(xmdm);

    for(Int_t i=0;i<5;i++)
      if((0.+i*.2)<pi1mothcosine&&pi1mothcosine<(0.+(i+1)*.2))
	HDCPiVsConeCoarse[i]->Fill(xmdm);

    for(Int_t i=0;i<16;i++)
      if((databaseDCPi.GetDCPiPstarCut()+i*.1)<xpstar&&xpstar<(databaseDCPi.GetDCPiPstarCut()+(i+1)*.1))
	HDCPiVsPstar[i]->Fill(xmdm);

    for(Int_t i=0;i<8;i++)
      if((databaseDCPi.GetDCPiPstarCut()+i*.2)<xpstar&&xpstar<(databaseDCPi.GetDCPiPstarCut()+(i+1)*.2))
	HDCPiVsPstarCoarse[i]->Fill(xmdm);

    for(Int_t i=0;i<8;i++)
      for(Int_t j=0;j<4;j++)
	if((databaseDCPi.GetDCPiPstarCut()+i*.2)<xpstar&&xpstar<(databaseDCPi.GetDCPiPstarCut()+(i+1)*.2))
	  if((-1.+j*.5)<dcostheta&&dcostheta<(-1.+(j+1)*.5))
	    HDCPiVsPstarCoarseDCos[i][j]->Fill(xmdm);

    for(Int_t i=0;i<16;i++)
      if((databaseDCPi.GetDCPiPstarCut()+i*.1)<xpstar)
	HDCPiVsPstarCut[i]->Fill(xmdm);

//     for(Int_t i=0;i<16;i++)
//       if((databaseDCPi.GetDCPiPstarCut()+i*.1)<xenergy&&xenergy<(databaseDCPi.GetDCPiPstarCut()+(i+1)*.1))
// 	HDCPiVsEnergy[i]->Fill(xmdm);

//     for(Int_t i=0;i<8;i++)
//       if((databaseDCPi.GetDCPiPstarCut()+i*.2)<xenergy&&xenergy<(databaseDCPi.GetDCPiPstarCut()+(i+1)*.2))
// 	HDCPiVsEnergyCoarse[i]->Fill(xmdm);

    
    
    //Efficiency corrected histos
    Float_t eff=GetEfficiency();
    if(eff<=0)continue;
    HDCPiEff.Fill(xmdm,1./(eff*FPCutEff->Eval(xmdm,xpstar)));
    HDCPiEffNorm+=1./eff;
    
    for(Int_t i=0;i<20;i++)
      if((-1.+i*.1)<dcostheta&&dcostheta<(-1.+(i+1)*.1)){
	HDCPiVsDCosEff[i]->Fill(xmdm,1./(eff*FPCutEff->Eval(xmdm,xpstar)));
	HDCPiVsDCosEffNorm[i]+=1./eff;
      }

    for(Int_t i=0;i<10;i++)
      if((-1.+i*.2)<dcostheta&&dcostheta<(-1.+(i+1)*.2)){
	HDCPiVsDCosCoarseEff[i]->Fill(xmdm,1./(eff*FPCutEff->Eval(xmdm,xpstar)));
	HDCPiVsDCosCoarseEffNorm[i]+=1./eff;
      }

    for(Int_t i=0;i<10;i++)
      for(Int_t j=0;j<4;j++)
	if((-1.+i*.2)<dcostheta&&dcostheta<(-1.+(i+1)*.2))
	  if((databaseDCPi.GetDCPiPstarCut()+j*.4)<xpstar&&xpstar<(databaseDCPi.GetDCPiPstarCut()+(j+1)*.4)){
	    HDCPiVsDCosCoarsePstarEff[i][j]->Fill(xmdm,1./(eff*FPCutEff->Eval(xmdm,xpstar)));
	    HDCPiVsDCosCoarsePstarEffNorm[i][j]+=1./eff;
	  }

    for(Int_t i=0;i<16;i++)
      if((databaseDCPi.GetDCPiPstarCut()+i*.1)<xpstar&&xpstar<(databaseDCPi.GetDCPiPstarCut()+(i+1)*.1)){
	HDCPiVsPstarEff[i]->Fill(xmdm,1./(eff*FPCutEff->Eval(xmdm,xpstar)));
	HDCPiVsPstarEffNorm[i]+=1./eff;
      }

    for(Int_t i=0;i<8;i++)
      if((databaseDCPi.GetDCPiPstarCut()+i*.2)<xpstar&&xpstar<(databaseDCPi.GetDCPiPstarCut()+(i+1)*.2)){
	HDCPiVsPstarCoarseEff[i]->Fill(xmdm,1./(eff*FPCutEff->Eval(xmdm,xpstar)));
	HDCPiVsPstarCoarseEffNorm[i]+=1./eff;
      }

    for(Int_t i=0;i<8;i++)
      for(Int_t j=0;j<4;j++)
	if((databaseDCPi.GetDCPiPstarCut()+i*.2)<xpstar&&xpstar<(databaseDCPi.GetDCPiPstarCut()+(i+1)*.2))
	  if((-1.+j*.5)<dcostheta&&dcostheta<(-1.+(j+1)*.5)){
	    HDCPiVsPstarCoarseDCosEff[i][j]->Fill(xmdm,1./(eff*FPCutEff->Eval(xmdm,xpstar)));
	    HDCPiVsPstarCoarseDCosEffNorm[i][j]+=1./eff;
	  }


    for(Int_t i=0;i<16;i++)
      if((databaseDCPi.GetDCPiPstarCut()+i*.1)<xpstar){
	HDCPiVsPstarCutEff[i]->Fill(xmdm,1./(eff*FPCutEff->Eval(xmdm,xpstar)));
	HDCPiVsPstarCutEffNorm[i]+=1./eff;
      }
    
    
  }

  ///
  HDCPi.Write();
    
  for(Int_t i=0;i<20;i++)    
    HDCPiVsDCos[i]->Write();

  for(Int_t i=0;i<10;i++)    
    HDCPiVsDCosCoarse[i]->Write();
  
  for(Int_t i=0;i<10;i++)
    for(Int_t j=0;j<4;j++)
      HDCPiVsDCosCoarsePstar[i][j]->Write();

  for(Int_t i=0;i<20;i++)    
    HDCPiVsCos[i]->Write();

  for(Int_t i=0;i<10;i++)    
    HDCPiVsCosCoarse[i]->Write();

  for(Int_t i=0;i<10;i++)    
    HDCPiVsCone[i]->Write();

  for(Int_t i=0;i<5;i++)    
    HDCPiVsConeCoarse[i]->Write();

  for(Int_t i=0;i<16;i++)    
    HDCPiVsPstar[i]->Write();

  for(Int_t i=0;i<8;i++)    
    HDCPiVsPstarCoarse[i]->Write();

  for(Int_t i=0;i<8;i++)
    for(Int_t j=0;j<4;j++)
      HDCPiVsPstarCoarseDCos[i][j]->Write();
  
  for(Int_t i=0;i<16;i++)    
    HDCPiVsPstarCut[i]->Write();
  
//   for(Int_t i=0;i<16;i++)    
//     HDCPiVsEnergy[i]->Write();

//   for(Int_t i=0;i<8;i++)    
//     HDCPiVsEnergyCoarse[i]->Write();


  ///Eff corrected 
  HDCPiEff.Scale(HDCPiEffNorm/HDCPiEff.Integral());
  HDCPiEff.Write();  
  
  for(Int_t i=0;i<20;i++){   
    HDCPiVsDCosEff[i]->Scale(HDCPiVsDCosEffNorm[i]/HDCPiVsDCosEff[i]->Integral());
    HDCPiVsDCosEff[i]->Write();
  }
  
  for(Int_t i=0;i<10;i++){         
    HDCPiVsDCosCoarseEff[i]->Scale(HDCPiVsDCosCoarseEffNorm[i]/HDCPiVsDCosCoarseEff[i]->Integral());    
    HDCPiVsDCosCoarseEff[i]->Write();
  }
  
  for(Int_t i=0;i<10;i++)
    for(Int_t j=0;j<4;j++){                                                       
      HDCPiVsDCosCoarsePstarEff[i][j]->Scale(HDCPiVsDCosCoarsePstarEffNorm[i][j]/HDCPiVsDCosCoarsePstarEff[i][j]->Integral());
      HDCPiVsDCosCoarsePstarEff[i][j]->Write();
    }

  for(Int_t i=0;i<16;i++){                             
    HDCPiVsPstarEff[i]->Scale(HDCPiVsPstarEffNorm[i]/HDCPiVsPstarEff[i]->Integral());    
    HDCPiVsPstarEff[i]->Write();
  }

  for(Int_t i=0;i<8;i++){                                       
    HDCPiVsPstarCoarseEff[i]->Scale(HDCPiVsPstarCoarseEffNorm[i]/HDCPiVsPstarCoarseEff[i]->Integral()); 
    HDCPiVsPstarCoarseEff[i]->Write();
  }

  for(Int_t i=0;i<8;i++)
    for(Int_t j=0;j<4;j++){                                                      
      HDCPiVsPstarCoarseDCosEff[i][j]->Scale(HDCPiVsPstarCoarseDCosEffNorm[i][j]/HDCPiVsPstarCoarseDCosEff[i][j]->Integral());
      HDCPiVsPstarCoarseDCosEff[i][j]->Write();
    }
  
  for(Int_t i=0;i<16;i++){                                 
    HDCPiVsPstarCutEff[i]->Scale(HDCPiVsPstarCutEffNorm[i]/HDCPiVsPstarCutEff[i]->Integral());
    HDCPiVsPstarCutEff[i]->Write();
  }
  
  
  temp.ls();
  temp.Close();

  if(CloseReducedNtuple()!=1) return 0;
  return 1;
}

