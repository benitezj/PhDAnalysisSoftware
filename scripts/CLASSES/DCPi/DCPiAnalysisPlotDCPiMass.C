
Int_t DCPiAnalysis::MakeRawDCPiMass(){
 
  //RS
  if(OpenReducedNtuple()!=1) return 0;
  TH1F HDCPiMass("HDCPiMass","",2000,1.9,3.9);
  ReducedNtuple->Draw("xmdm>>HDCPiMass");
  if(CloseReducedNtuple()!=1) return 0;
  
  TFile file(_OutputDir+"/RawMassPlots.root","recreate");
  HDCPiMass.Write();
//   if(!_TruthMatch){
//     ///correct from WS ratio
//     HDCPiMass.Sumw2();
//     HDCPiMass.Divide(FWSRatio);
  HDCPiMass.SetName("BkgHisto");
  HDCPiMass.Write();    
//   }
  file.Close();

  return 1;
}

Int_t DCPiAnalysis::PlotRawDCPiMass(){
    

  filename=_OutputDir+"/RawDCPiMassPlots.ps";
  Canvas.Print(filename+"["); 

  TFile file(_OutputDir+"/RawMassPlots.root","read");
  gROOT->cd();
 
  if(_TruthMatch){
    HMCMass=(TH1F*)GetMCHisto("HMCMass");   
    if(HMCMass)SetHistoXYLabels(HMCMass,"m(D#pi)","GeV/c^{2}");
  } 
    
  ///
  Int_t nbins=200;
  Float_t min=2.0;
  Float_t max=3.0;
  if(HMCMass){
    nbins=HMCMass->GetXaxis()->GetNbins();
    min=HMCMass->GetXaxis()->GetXmin();
    max=HMCMass->GetXaxis()->GetXmax(); 
  }   


  //right sign
  TH1F*HDCPiMass=(TH1F*)file.Get("HDCPiMass");
  if(HMCMass){
    if(!(HDCPiMass=(TH1F*)RebinHistoX(HDCPiMass,nbins,min,max)))return 0; 
  }else 
    if(!(HDCPiMass=(TH1F*)RebinHistoX(HDCPiMass,nbins,min,max)))return 0; 
  SetHistoXYLabels(HDCPiMass,"m(D#pi)","GeV/c^{2}");
  HDCPiMass->GetYaxis()->SetRangeUser(0,HDCPiMass->GetMaximum()*1.1);

  ///
  Canvas.Clear();
  if(HMCMass)HMCMass->Draw();
  HDCPiMass->Draw(HMCMass ? "same":"");
  Canvas.Print(filename);
 

  //wrong sign
  if(!_TruthMatch){
    TFile fileWS(_OutputDir+"/../KPiPiWS/RawMassPlots.root","read");
    gROOT->cd();
    TH1F*HDCPiMassWS=(TH1F*)fileWS.Get("HDCPiMass");
    if(!(HDCPiMassWS=(TH1F*)RebinHistoX(HDCPiMassWS,nbins,min,max)))return 0;


    //side band
    TFile fileSB(_OutputDir+"/../KPiPiSB/RawMassPlots.root","read");
    gROOT->cd();
    TH1F*HDCPiMassSB=(TH1F*)fileSB.Get("HDCPiMass");
    if(!(HDCPiMassSB=(TH1F*)RebinHistoX(HDCPiMassSB,nbins,min,max)))return 0;

    ///
    Canvas.Clear();
    HDCPiMass->Draw("pe");
    if(HDCPiMassWS)HDCPiMassWS->Draw("pesame");
    if(HDCPiMassSB)HDCPiMassSB->Draw("pesame");
    cout<<" Fake D+ fraction determined from he D+ sideband="<<HDCPiMassSB->Integral()/HDCPiMass->Integral()<<endl;
    Canvas.Print(filename);



    //Compare with not-truth matched MC
    TFile fileNT("DCharged/DChargedPi/MC/SP-1005/KPiPiNT/RawMassPlots.root","read");
    gROOT->cd();
    TH1F*HDCPiMassNT=(TH1F*)fileNT.Get("HDCPiMass");
    if(!(HDCPiMassNT=(TH1F*)RebinHistoX(HDCPiMassNT,nbins,min,max)))return 0; 
    ///scale by lumi
    Canvas.Clear();
    HDCPiMass->Draw("pe");
    if(_MCorDATA==1)HDCPiMassNT->Scale(scalefactorccbar);
    HDCPiMassNT->Draw("pesame");
    Canvas.Print(filename);

    //scale to first 10 bins
    if(_MCorDATA==1){
      Canvas.Clear();
      HDCPiMass->Draw("pe");
      HDCPiMassNT->Scale(HDCPiMass->Integral(1,10)/HDCPiMassNT->Integral(1,10));
      HDCPiMassNT->Draw("pesame");
      Canvas.Print(filename);
    }


 
    if(_MCorDATA==0){
    
      //Show the different signals in the Generic truth
      Canvas.Clear();
      HDCPiMass->Draw("pe");
      if(OpenReducedNtuple()!=1) return 0;
      ReducedNtuple->Draw("xmdm","xlund==10423","same");
      ReducedNtuple->Draw("xmdm","xlund==425","same");
      ReducedNtuple->Draw("xmdm","xlund==20423","same");
      ReducedNtuple->Draw("xmdm","xlund==10421","same");
      Canvas.Print(filename);
      if(CloseReducedNtuple()!=1) return 0;
     
 
      //for NT generic show the corrected histo
      Canvas.Clear();    
      TH1F*BkgHisto=(TH1F*)file.Get("BkgHisto");
      if(!(BkgHisto=(TH1F*)RebinHistoX(BkgHisto,nbins,min,max)))return 0; 
      BkgHisto->Draw("pe");
      HDCPiMass->Draw("pesame");
      Canvas.Print(filename);
    }


    fileWS.Close();
    fileSB.Close();
    fileNT.Close();
  }

  Canvas.Print(filename+"]");


  file.Close();

  return 1;
}

Int_t DCPiAnalysis::PlotRawDCPiMassRatio(){
  if(_TruthMatch)return 0;   

  filename=_OutputDir+"/RawDCPiMassRatioPlots.ps";
  Canvas.Print(filename+"["); 

  TFile file(_OutputDir+"/RawMassPlots.root","read");
  gROOT->cd();
 

  ///
  Int_t nbins=100;
  Float_t min=2.0;
  Float_t max=3.0;

  //right sign
  TH1F*HDCPiMass=(TH1F*)file.Get("HDCPiMass");
  if(!(HDCPiMass=(TH1F*)RebinHistoX(HDCPiMass,nbins,min,max)))return 0; 
  SetHistoXYLabels(HDCPiMass,"m(D#pi)","GeV/c^{2}");
  HDCPiMass->GetYaxis()->SetRangeUser(0,HDCPiMass->GetMaximum()*1.1);

  ///
  Canvas.Clear();
  HDCPiMass->Draw("");
  Canvas.Print(filename);
 

  //DATA
  TFile fileDATA("DCharged/DChargedPi/KPiPiWS/RawMassPlots.root","read");
  if(fileDATA.IsZombie())return 0;
  gROOT->cd();
  TH1F*HDCPiMassDATA=(TH1F*)fileDATA.Get("HDCPiMass");
  if(!(HDCPiMassDATA=(TH1F*)RebinHistoX(HDCPiMassDATA,nbins,min,max)))return 0;
  HDCPiMassDATA->SetMarkerColor(2);
  HDCPiMassDATA->SetLineColor(2);
    
  ///
  Canvas.Clear();
  HDCPiMass->Draw("pe");
  if(_MCorDATA==0)HDCPiMassDATA->Scale(1./scalefactorccbar);
  HDCPiMassDATA->Draw("pesame");
  Canvas.Print(filename);

  //scale to first 10 bins
  Canvas.Clear();
  HDCPiMassDATA->Scale(HDCPiMass->Integral(1,10)/HDCPiMassDATA->Integral(1,10));
  HDCPiMassDATA->Draw("pe");
  HDCPiMass->Draw("pesame");  
  Canvas.Print(filename);

  HDCPiMass->Sumw2();
  HDCPiMass->Divide(HDCPiMassDATA);
  Canvas.Clear();
  HDCPiMass->GetYaxis()->SetRangeUser(0,1.2);
  HDCPiMass->GetYaxis()->SetTitle("ratio");
  HDCPiMass->SetStats(0);
  HDCPiMass->Draw("pe");  
  Canvas.Print(filename);
  Canvas.Clear();
  HDCPiMass->Fit(FWSRatio);
  HDCPiMass->Draw("pe");  
  Canvas.Print(filename);
  for(Int_t i=0;i<FWSRatio->GetNpar();i++)    
    cout<<FWSRatio->GetParameter(i)<<",";
  cout<<endl;

  Canvas.Print(filename+"]");


  file.Close();
  fileDATA.Close();
  return 1;
}




Int_t DCPiAnalysis::MakeFinalStateMasses(){
  if(_TruthMatch)return 0;

  if(OpenReducedNtuple()!=1) return 0;

  TFile file(_OutputDir+"/FinalStateMassPlots.root","recreate");

  TH1F HKPiMass("HKPiMass","",3000,0,3);
  ReducedNtuple->Draw("Kpimass>>HKPiMass");
  HKPiMass.Write(); 

  TH1F HKPi2Mass("HKPi2Mass","",3000,0,3);
  ReducedNtuple->Draw("Kpi2mass>>HKPi2Mass");
  HKPi2Mass.Write();

  TH1F HPi1KMass("HPi1KMass","",3000,0,3);
  ReducedNtuple->Draw("pi1Kmass>>HPi1KMass");
  HPi1KMass.Write();

  TH1F HPi1KPiMass("HPi1KPiMass","",3000,0,3);
  ReducedNtuple->Draw("pi1Kpimass>>HPi1KPiMass");
  HPi1KPiMass.Write();

  TH1F HPi1KPi2Mass("HPi1KPi2Mass","",3000,0,3);
  ReducedNtuple->Draw("pi1Kpi2mass>>HPi1KPi2Mass");
  HPi1KPi2Mass.Write();

  TH1F HPi1PiMass("HPi1PiMass","",3000,0,3);
  ReducedNtuple->Draw("pi1pimass>>HPi1PiMass");
  HPi1PiMass.Write();

  TH1F HPi1Pi2Mass("HPi1Pi2Mass","",3000,0,3);
  ReducedNtuple->Draw("pi1pi2mass>>HPi1Pi2Mass");
  HPi1Pi2Mass.Write();

 
  if(CloseReducedNtuple()!=1) return 0;
   
 
  file.Close();

  return 1;
}

Int_t DCPiAnalysis::PlotFinalStateMasses(){
  if(_TruthMatch)return 0;
  
 
  TFile file(_OutputDir+"/FinalStateMassPlots.root","read");
  gROOT->cd();
  
  TString name[10];
  name[0]="HKPiMass";
  name[1]="HKPi2Mass";
  name[2]="HPi1KMass";
  name[3]="HPi1KPiMass";
  name[4]="HPi1KPi2Mass";
  name[5]="HPi1PiMass";
  name[6]="HPi1Pi2Mass";
  name[7]="";
  name[8]="";
  name[9]="";

  filename=_OutputDir+"/FinalStateMassPlots.ps";
  Canvas.Print(filename+"["); 

  TH1F*Histo=NULL;
  Int_t id=0;
  while((Histo=(TH1F*)file.Get(name[id]))&&id<9){
    if(!(Histo=(TH1F*)RebinHistoX(Histo,300,0,3)))return 0;
    SetHistoXYLabels(Histo,name[id],"GeV/c^{2}");
    Canvas.Clear();
    Histo->Draw("pe");
    Canvas.Print(filename);
    delete Histo; 
    id++;
  }

  Canvas.Print(filename+"]");

  file.Close();
  return 1;
}


void DCPiAnalysis::MakeFeedResolution(){
  
  TFile File("DstarCharged/DstarPi/MC/D2420/Kpi/ReducedClean.root");
  gROOT->cd();
  TTree*t=(TTree*)File.Get("CleanNtuple");
  TH1F HResRaw("HResRaw","",200,-.2,-.1);
  t->Draw("massKpipi1-d0mass+1.8645-(dstpimdm-dstpidmres)+.0046>>HResRaw");//last number corrects for pi+ - pi0 mass difference
  Float_t Mean=HResRaw.GetMean();
  cout<<"The mean value is "<< Mean<<endl;
  TH1F HRes("HRes","",200,-.05,.05);
  sprintf(textlabel,"massKpipi1-d0mass+1.8645-(dstpimdm-dstpidmres)+.0046-%.6f>>HRes",Mean);
  t->Draw(textlabel);//4.6MeV corrects for pi+ - pi0 mass difference
  File.Close();

  //Fit Function
  TF1 fg("fg","[0]/((([1]*asin((x-[2])/.1))**2)**[3] + 1.) + [4]*exp(-.5*(x-[5])**2/[6]**2)",-.2,-.1);///(3.14159/2.)
  fg.SetParLimits(0,0,1e5);
  fg.SetParLimits(1,.1,100);
  fg.SetParLimits(2,-.150,-.135);
  fg.SetParLimits(3,1,5);
  fg.SetParLimits(4,0,1e5);
  fg.SetParLimits(5,-.16,-.12);
  fg.SetParLimits(6,.01,.2);
  HResRaw.Fit(&fg);
  cout<<" chisquare="<<fg.GetChisquare()/(HResRaw.GetNbinsX()-fg.GetNumberFreeParameters())<<endl;
  
  TH1F HResFit("HResFit","",200,-.2,-.1);
  for(Int_t i=1;i<=HResFit.GetNbinsX();i++)
    HResFit.SetBinContent(i,fg.Eval(HResFit.GetBinCenter(i)));
  HResFit.Scale(1./HResFit.Integral());

  TFile FRes(_OutputDir+"/FeedResolution.root","recreate");
  cout<<FRes.GetName()<<" has been created"<<endl;
  HResRaw.Write();
  HRes.Write();
  HResFit.Write();
  FRes.Close();

}

Int_t DCPiAnalysis::FitMass(TString name){

  TFile file(_OutputDir+"/HistosForFit.root","read");
  //file.ls();
  gROOT->cd();

  TH1F*HDCPiMass=(TH1F*)file.Get(name);

  TH1F*HDCPiMassEff=(TH1F*)file.Get(name+"Eff");
 
  if(!HDCPiMass){cout<<"no histo"<<endl;return 0;}
  SetHistoXYLabels(HDCPiMass,"m(D#pi)","GeV/c^{2}");
  HDCPiMass->SetStats(0);
  
  TFile fileside(_OutputDir+"/../KPiPiSB/HistosForFit.root","read");
  gROOT->cd();
  TH1F*HDCPiMassSB=(TH1F*)fileside.Get(name);
  if(HDCPiMassSB){
    cout<<"Integral before sideband subtraction= "<<HDCPiMass->Integral()<<endl;
    HDCPiMass->Sumw2();
    HDCPiMassSB->Sumw2();
    HDCPiMass->Add(HDCPiMassSB,-1);
    cout<<"Integral after sideband subtraction= "<<HDCPiMass->Integral()<<endl;
  }

  fileside.Close();

  massfitter.SetEfficiency(HDCPiMassEff); 
  if(!massfitter.Initialize(HDCPiMass,_OutputDir+"/InitRooFitPars.txt"))return 0;
  if(massfitter.Fit()<0)return 0;
  massfitter.Save(_OutputDir);
  
  filename=_OutputDir+"/DCPiMassPlots.ps";
  Canvas.Print(filename+"[");  

  Canvas.Clear();
  HDCPiMass->Draw("pe");
  Canvas.Print(filename); 

  RooPlot*fitplot=massfitter.GetFitPlot(); 
  Canvas.Clear();
  fitplot->Draw();
  Canvas.Print(filename);
  delete fitplot; 

//   TGraph*Scan= massfitter.ScanParameter("D2460","mean");
//   if(Scan){
//     Canvas.Clear();
//     Scan->Draw("ap");
//     Canvas.Print(filename);
//   }

 
//   RooPlot*H2Cont= massfitter.GetContour("D2460","mean","D2460","width");
//   if(H2Cont){
//     Canvas.Clear();
//     H2Cont->Draw();
//     Canvas.Print(filename);  
//     delete H2Cont;
//   }

//   TH2F H2scan("H2","",10,2.456,2.462,10,.025,.035);
//   massfitter.ScanParameter2D("D2460","mean","D2460","width",&H2scan);
//   Canvas.Clear();
//   H2scan.Draw("colz");
//   Canvas.Print(filename);


//    TH2F H2scan("H2","",50,-5,15,50,-4,0);
//    massfitter.ScanParameter2D("Bkg1","coef2","Bkg1","coef3",&H2scan);
//    Canvas.Clear();
//    H2scan.Draw("colz");
//    Canvas.Print(filename); 
  
//   TH2F H2scan("H2","",100,0,10,100,-4,0);
//   massfitter.ScanParameter2D("Bkg1","coef2","Bkg1","coef3",&H2scan);
//   Canvas.Clear();
//   H2scan.Draw("colz");
//   Canvas.Print(filename);  


  Canvas.Print(filename+"]");

 
  file.Close();
  return 1;
}


void DCPiAnalysis::PlotRandFits(TString Dir,TString signame,TString parname,Int_t Nfiles){
  filename=_OutputDir+"/"+Dir+".ps";
  Canvas.Print(filename+"["); 
  

  TH1F HChis("HChis","",60,0,3);
  TGraphErrors GChiVsPar;
  TGraphErrors GCovVsPar;

  
  Float_t chi=0.;
  Float_t ndf=0.;
  Float_t chidf=0.;
  Int_t covq=0;
  Int_t ngood=0;
  for(Int_t i=1;i<=Nfiles;i++){
    inputpars.CloseFitPars();
    if(!inputpars.OpenFitPars(_OutputDir+"/"+Dir+"/FitPars_InitRooFitPars_"+(long)i+".txt"))continue;
    if(inputpars.GetFitPar("Range","status")!=0){cout<<" Status="<<inputpars.GetFitPar("Range","status")<<endl;continue;}
    chi=inputpars.GetFitPar("Range","chisquare");
    ndf=inputpars.GetFitPar("Range","ndf");
    chidf=chi/ndf;
    if(chidf>10.){cout<<"Bad Chi/DF="<<chidf<<"  chi="<<chi<<"  ndf="<<ndf<<endl;inputpars.CloseFitPars();continue;}
    covq=(int)inputpars.GetFitPar("Range","covqual");
    
    HChis.Fill(chidf);
    GChiVsPar.SetPoint(ngood,inputpars.GetFitPar(signame,parname),chidf);
    GCovVsPar.SetPoint(ngood,inputpars.GetFitPar(signame,parname),covq);
    ngood++;
 
  }

  Canvas.Clear();
  HChis.Draw("pe");
  Canvas.Print(filename); 

  Canvas.Clear();
  GChiVsPar.GetYaxis()->SetRangeUser(0,3);
  GChiVsPar.GetYaxis()->SetTitle("#chi^{2}/NDF");
  GChiVsPar.GetXaxis()->SetTitle(signame+" "+parname);
  GChiVsPar.Draw("ape");
  Canvas.Print(filename); 

  Canvas.Clear();
  GCovVsPar.GetYaxis()->SetTitle("Covariance Matrix Quality");
  GCovVsPar.GetXaxis()->SetTitle(signame+" "+parname);
  GCovVsPar.Draw("ape");
  Canvas.Print(filename); 


  Canvas.Print(filename+"]"); 
}


Int_t DCPiAnalysis::FitD3000(){
  
  TFile file(_OutputDir+"/FitPars_NRBW100_NRBW100_NRBW100_NRBW100_NRBW100_NRBW100.root","read");
  if(file.IsZombie())return 0;
  file.ls();
  TH1F* H=(TH1F*)file.Get("HResid");
  if(!H)return 0;

  Float_t fitmin=2.85;
  Float_t fitmax=3.25;  
 
  for(Int_t i=1;i<=H->GetNbinsX();i++)
    if(H->GetXaxis()->GetBinCenter(i)<fitmin||H->GetXaxis()->GetBinCenter(i)>fitmax)
      H->SetBinContent(i,0);

  //Rebin
  H=(TH1F*)H->Rebin(2,"HR");


  Float_t ymax=0;
  Float_t ymin=0;
  for(Int_t i=1;i<=H->GetNbinsX();i++){
    if(ymax<H->GetBinContent(i))ymax=H->GetBinContent(i);
    if(ymin>H->GetBinContent(i))ymin=H->GetBinContent(i);
  }

  filename=_OutputDir+"/D3000ResFit.ps";
  Canvas.Print(filename+"[");   


  
//   TF1 FBW("FBW","([0]*[3]/(2*3.14159*[2]))/((x-[1])*(x-[1])+.25*[2]*[2])",fitmin,fitmax);
//   FBW.SetParLimits(0,1,10000);
//   FBW.SetParLimits(1,2.95,3.05);
//   FBW.SetParLimits(2,.005,.2);
//   FBW.FixParameter(3,H->GetBinWidth(1));
//   FBW.SetParNames("Yield","Mass","Width");


  //FitHistogramBreitWigner(H,fitmin,fitmax);
  //H->Fit(&FBW,"","",fitmin,fitmax);

//   Canvas.Clear();
//   H->GetXaxis()->SetRangeUser(fitmin,fitmax);
//   H->GetYaxis()->SetRangeUser(1.1*ymin,1.75*ymax);
//   H->SetStats(1);
//   H->Draw("pe");
//   cutline.DrawLine(fitmin,0,fitmax,0);
//   Canvas.Print(filename); 


  mass.setRange(fitmin,fitmax);
  RooRealVar Mean("Mean","Mean",fitmin,fitmax);Mean.setUnit("GeV");
  RooRealVar Width("Width","Width",.001,.1);Width.setUnit("GeV");
  RooGenericPdfN FBW("FBW","FBW","1./((mass-Mean)**2+.25*Width*Width)",RooArgList(mass,Mean,Width));// + Yield*(mass>5),Yield

  RooRealVar slope("slope","slope",-10000,10000);
  RooRealVar fmin("fmin","fmin",fitmin);
  RooRealVar fmax("fmax","fmax",fitmax);
  RooGenericPdfN FBkg("FBkg","FBkg","(1-.5*slope*(fmax-fmin)*(fmax-fmin))/(fmax-fmin)+slope*(mass-fmin)",RooArgList(mass,slope,fmin,fmax));
  
  RooRealVar Yield("Yield","Yield",0,10);
  RooAddPdfN totpdf("totpdf","totpdf",FBW,FBkg,Yield);

  RooDataHist dataset("dataset","dataset",mass,H);
  //FBW.fitTo(dataset,"mh",RooFit::DataError(RooAbsData::SumW2));

  RooChi2VarIntegral chi2("chi2","chi2",totpdf,dataset,RooFit::DataError(RooAbsData::SumW2));
  chi2.SetIntegralBins(4);
  RooMinuit minuit(chi2);
  minuit.migrad();
  RooFitResult* result=minuit.save();
  result->Print();


  Canvas.Clear();
  RooPlot*pl=mass.frame();
  dataset.plotOn(pl,RooFit::DataError(RooAbsData::SumW2));
  totpdf.plotOn(pl);
  //FBW.paramOn(pl,&dataset,"",1,"NELU",.45,.78,.83);
  //dataset.statOn(pl);
  totpdf.paramOn(pl,Parameters(RooArgSet(Mean,Width)),Layout(.45,.78,.83),Format("NELU",AutoPrecision(1)));
 
  Yield.setRange(0,(int)(H->Integral()*Yield.getVal()));
  Yield.setVal((int)(H->Integral()*Yield.getVal()));
  totpdf.paramOn(pl,Parameters(RooArgSet(Yield)),Layout(.45,.78,.7),Format("N"));
  pl->SetTitle("");
  pl->GetXaxis()->SetRangeUser(fitmin,fitmax);
  pl->GetYaxis()->SetRangeUser(1.1*ymin,1.75*ymax);
  pl->GetXaxis()->SetTitle("m(D#pi) (GeV)");
  pl->Draw();
  cutline.DrawLine(fitmin,0,fitmax,0);
  Canvas.Print(filename); 
  delete pl;

  Canvas.Print(filename+"]"); 
 
  return 1;
}


// Int_t DCPiAnalysis::FitMassD2700(){

//   TFile file(_OutputDir+"/HistosForFit.root","read");
//   gROOT->cd();

//   filename=_OutputDir+"/DCPiMassPlotsD2700.ps";
//   Canvas.Print(filename+"["); 


//   TH1F*HDCPiMass=(TH1F*)file.Get("HDCPi");  
//   SetHistoXYLabels(HDCPiMass,"m(D#pi)","GeV/c^{2}");
//   HDCPiMass->SetStats(0);

//   Canvas.Clear();
//   HDCPiMass->Draw("pe");
//   Canvas.Print(filename); 

  
//   if(!massfitter.Initialize(HDCPiMass,_OutputDir+"/InitRooFitParsD2700.txt"))return 0;
//   massfitter.Fit();
//   massfitter.Save(_OutputDir);


//   RooPlot*fitplot=massfitter.GetFitPlot(); 

//   Canvas.Clear();
//   fitplot->Draw();
//   Canvas.Print(filename);
//   delete fitplot;

//   //TGraph*GScan=massfitter.ScanParameter("Background","coef3"); 
//   TGraph*GScan=massfitter.ScanParameter("D2700","yield"); 
//   if(GScan){
//     Canvas.Clear();
//     GScan->GetYaxis()->SetRangeUser(0,GScan->GetYaxis()->GetXmax());
//     GScan->Draw("ape");
//     Canvas.Print(filename);
//   }

//   Canvas.Print(filename+"]");

//   delete fitplot;
//   delete HDCPiMass;

//   file.Close();
//   return 1;
// }



// Int_t DCPiAnalysis::FitMassGlobal(){

//   TFile file(_OutputDir+"/HistosForFit.root","read");
//   gROOT->cd();


//   TH1F*HDCPiMass=(TH1F*)file.Get("HDCPi");
//   SetHistoXYLabels(HDCPiMass,"m(D#pi)","GeV/c^{2}");
//   HDCPiMass->SetStats(0);

//   if(!massfitter.Initialize(HDCPiMass,_OutputDir+"/InitRooFitParsGlobal.txt"))return 0;
//   massfitter.Fit();
//   massfitter.Save(_OutputDir);


//   filename=_OutputDir+"/DCPiMassPlotsGlobal.ps";
//   Canvas.Print(filename+"["); 

//   Canvas.Clear();
//   HDCPiMass->Draw("pe");
//   Canvas.Print(filename); 

//   RooPlot*fitplot=massfitter.GetFitPlot(); 
//   Canvas.Clear();
//   fitplot->Draw();
//   Canvas.Print(filename);
  

//   Canvas.Print(filename+"]");

//   delete fitplot;
//   delete HDCPiMass;

//   file.Close();
//   return 1;
// }


// Int_t DCPiAnalysis::FitMassD2600(){

//   TFile file(_OutputDir+"/HistosForFit.root","read");
//   gROOT->cd();


//   TH1F*HDCPiMass=(TH1F*)file.Get("HDCPi");
//   SetHistoXYLabels(HDCPiMass,"m(D#pi)","GeV/c^{2}");
//   HDCPiMass->SetStats(0);

//   if(!massfitter.Initialize(HDCPiMass,_OutputDir+"/InitRooFitParsD2600.txt"))return 0;
//   massfitter.Fit();
//   massfitter.Save(_OutputDir);


//   filename=_OutputDir+"/DCPiMassPlotsD2600.ps";
//   Canvas.Print(filename+"["); 

//   Canvas.Clear();
//   HDCPiMass->Draw("pe");
//   Canvas.Print(filename); 

//   RooPlot*fitplot=massfitter.GetFitPlot(); 
//   Canvas.Clear();
//   fitplot->Draw();
//   Canvas.Print(filename);
  

//   Canvas.Print(filename+"]");

//   delete fitplot;
//   delete HDCPiMass;

//   file.Close();
//   return 1;
// }


// Int_t DCPiAnalysis::FitMassFeed(){

//   TFile file(_OutputDir+"/HistosForFit.root","read");
//   gROOT->cd();


//   TH1F*HDCPiMass=(TH1F*)file.Get("HDCPi");
//   SetHistoXYLabels(HDCPiMass,"m(D#pi)","GeV/c^{2}");
//   HDCPiMass->SetStats(0);

//   if(!massfitter.Initialize(HDCPiMass,_OutputDir+"/InitRooFitParsFeed.txt"))return 0;
//   massfitter.Fit();
//   massfitter.Save(_OutputDir);


//   filename=_OutputDir+"/DCPiMassPlotsFeed.ps";
//   Canvas.Print(filename+"["); 

//   Canvas.Clear();
//   HDCPiMass->Draw("pe");
//   Canvas.Print(filename); 

//   RooPlot*fitplot=massfitter.GetFitPlot(); 
//   Canvas.Clear();
//   fitplot->Draw();
//   Canvas.Print(filename);
  

//   Canvas.Print(filename+"]");

//   delete fitplot;
//   delete HDCPiMass;

//   file.Close();
//   return 1;
// }
