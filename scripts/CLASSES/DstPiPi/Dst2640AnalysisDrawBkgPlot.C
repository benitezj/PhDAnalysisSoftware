


Int_t Dst2640Analysis::DrawBkgPlot(Bool_t signal,Bool_t ccbar,Bool_t uds,Bool_t bbbar,Bool_t tau){
//Int_t Dst2640Analysis::DrawBkgPlot(){
  cout<<"Going to Draw the background in deltaM"<<endl;


  //Number of expected ccbar events in Data
  Int_t NDataccbarEvents=(int)(lumidata*Crossccbar);
  Float_t BrccbrToDst2640BrDst2640ToDstpipi=.041;//Estimate From Delphi:  Br(ccbar->D*(2640)X) x Br(D*(2640)->D*pipi)
  //Float_t BrccbrToDst2640BrDst2640ToDstpipiError=.007;
  //Float_t BrccbrToDst2640BrDst2640ToDstpipiWorse=(BrccbrToDst2640BrDst2640ToDstpipi-2*BrccbrToDst2640BrDst2640ToDstpipiError)/10.;
  Float_t BrDstToD0=.677;
  Float_t BrD0Kpi=.038;
  Float_t BrD0K3pi=.077;
  Float_t EffKpi=.1;
  Float_t EffK3pi=.077;
  Int_t NDst2640ExpKpi=(int)(NDataccbarEvents*BrccbrToDst2640BrDst2640ToDstpipi*BrDstToD0*BrD0Kpi*EffKpi);
  Int_t NDst2640ExpK3pi=(int)(NDataccbarEvents*BrccbrToDst2640BrDst2640ToDstpipi*BrDstToD0*BrD0K3pi*EffK3pi);

//   Int_t NDst2640ExpKpiWorse=(int)(NDataccbarEvents*BrccbrToDst2640BrDst2640ToDstpipiWorse*BrDstToD0*BrD0Kpi*EffKpi);
//   Int_t NDst2640ExpK3piWorse=(int)(NDataccbarEvents*BrccbrToDst2640BrDst2640ToDstpipiWorse*BrDstToD0*BrD0K3pi*EffK3pi);

  if(_Mode=="D0ToKPi")cout<<"N exp Kpi Dst2640: "<<NDst2640ExpKpi<<endl;
  if(_Mode=="D0ToK3Pi")cout<<"N exp K3pi Dst2640: "<<NDst2640ExpK3pi<<endl;

//   if(_Mode=="D0ToKPi")cout<<"N exp Kpi Dst2640 Worse: "<<NDst2640ExpKpiWorse<<endl;
//   if(_Mode=="D0ToK3Pi")cout<<"N exp K3pi Dst2640 Worse: "<<NDst2640ExpK3piWorse<<endl;



  //-------------------
  //create a canvas
  //-------------------
  TCanvas Canvas("Canvas","Canvas");
  TString fname;
  fname=_OutputDir+"/"+"Dst2640Bkg"+(long)_WhichCuts+".ps";
  Canvas.Print(fname+"[");


  //-------------------
  //histogram ranges
  //-------------------
  database.SetTruthMatch(0);
  database.SetCuts(_WhichCuts);

//   Int_t NDelphiBins=65;
//   Float_t DelphiMin=2.350;
//   Float_t DelphiMax=3.;
  //Float_t DelphiPeakSigma=.007;



   Float_t deltammin=database.GetDst2640DeltaMMin()+DstarPDGMass;
   Float_t deltammax=database.GetDst2640DeltaMMax()+DstarPDGMass;
   Int_t ndeltambins=database.GetDst2640DeltaMNbins();

//   Float_t deltammin=DelphiMin;
//   Float_t deltammax=DelphiMax;
//   Int_t ndeltambins=NDelphiBins;



  //Create a signal histogram
  TH1F Hsignal("Hsignal","Signal MC",ndeltambins,deltammin,deltammax);
 //  TH1F HsignalWorse("HsignalWorse","Signal MC Worse Scenario",ndeltambins,deltammin,deltammax);
  // ReducedNtuple->Draw("dst2640deltam>>Hsignal",database.GetCut(1,100));

  Float_t NaturalFWHM=.001;
  //   Float_t NaturalFWHMWorse=.015;
  Float_t ResolutionFWHM=.0045;
  Float_t conversionfactor=2.35;//FWHW=2.35*sigma    
  Float_t MeanMass=2.637;
  Float_t PeakSigma=sqrt(pow(NaturalFWHM/conversionfactor,2)+pow(ResolutionFWHM/conversionfactor,2));
//   Float_t PeakSigmaWorse=sqrt(pow(NaturalFWHMWorse/conversionfactor,2)+pow(ResolutionFWHM/conversionfactor,2));
  cout<<"Signal drawn with a sigma="<<PeakSigma<<" and a mean="<<MeanMass<<endl;
  TF1 Fsignal("Fsignal","[0]*exp(-.5*(x-[1])**2/[2]**2)",deltammin,deltammax);
  Fsignal.SetParameter(0,1);
  Fsignal.SetParameter(1,MeanMass);
  Fsignal.SetParameter(2,PeakSigma);
   
  //avg scenario
  if(_Mode=="D0ToKPi")Hsignal.FillRandom("Fsignal",NDst2640ExpKpi);
  if(_Mode=="D0ToK3Pi")Hsignal.FillRandom("Fsignal",NDst2640ExpK3pi);
  Hsignal.SetStats(1);
  Hsignal.SetFillColor(3);
  Hsignal.SetLineColor(3);
  Hsignal.SetMarkerSize(.8);

//   Canvas.Clear(); 
//   Hsignal.Draw();
//   Canvas.Print(fname);

//   //worse scenario
//   Fsignal.SetParameter(2,PeakSigmaWorse);
//   if(_Mode=="D0ToKPi")HsignalWorse.FillRandom("Fsignal",NDst2640ExpKpiWorse);//number of Ds1's
//   if(_Mode=="D0ToK3Pi")HsignalWorse.FillRandom("Fsignal",NDst2640ExpK3piWorse);//number of Ds1's
//   HsignalWorse.SetStats(1);
//   HsignalWorse.SetFillColor(3);
//   HsignalWorse.SetLineColor(3);
//   //HsignalWorse.SetStats(0); 
//   HsignalWorse.SetMarkerSize(.8);

//   Canvas.Clear(); 
//   HsignalWorse.Draw();
//   Canvas.Print(fname);
//   //Close();  



  TString ModeDir="/u2/benitezj/physics/NTUPLES/Dst2640ToDstPiPi/"+_Mode;
  TTree* tree=NULL;
 

 
  //make ccbar background histo
  TH1F Hccbarbkg("Hccbarbkg","ccbar background",ndeltambins,deltammin,deltammax);
  if(ccbar){
    TFile Fccbar(ModeDir+"/SP-1005"+"/Dst2640Reduced.root","read");
    tree=(TTree*)Fccbar.Get("Dst2640Ntuple");    
    gROOT->cd();
    cout<<"ccbar ntuple openened with "<<tree->GetEntries()<<" entries"<<endl; 
    tree->Draw("dst2640deltam+2.010>>Hccbarbkg",database.GetCut(1,100));
    Hccbarbkg.Scale(scalefactorccbar);
    Hccbarbkg.SetFillColor(2);
    Hccbarbkg.SetFillStyle(1001);
    Hccbarbkg.SetLineColor(2);
    Hccbarbkg.SetStats(0);

    cout<<"ccbar scaled bkg: "<<Hccbarbkg.Integral()<<endl;
  }
  
  //uds background
  TH1F Hudsbkg("Hudsbkg","uds background",ndeltambins,deltammin,deltammax);
  if(uds){
    TFile Fuds(ModeDir+"/SP-998"+"/Dst2640Reduced.root");
    tree=(TTree*)Fuds.Get("Dst2640Ntuple");
    gROOT->cd();
    cout<<"uds ntuple openened with "<<tree->GetEntries()<<" entries"<<endl; 
    tree->Draw("dst2640deltam+2.010>>Hudsbkg",database.GetCut(1,100));
    Hudsbkg.Scale(scalefactoruds);   
    cout<<"uds scaled bkg: "<<Hudsbkg.Integral()<<endl;
  }
  
  //bbbar background
  //B+ B- background
  TH1F Hbbbarbkg("Hbbbarbkg","bbbar background",ndeltambins,deltammin,deltammax);
  if(bbbar){
    TFile Fbch(ModeDir+"/SP-1235"+"/Dst2640Reduced.root");
    tree=(TTree*)Fbch.Get("Dst2640Ntuple"); 
    gROOT->cd();
    cout<<"Bcharge ntuple openened with "<<tree->GetEntries()<<" entries"<<endl; 
    TH1F HBchargedbkg("HBchargebkg","B+ B- background",ndeltambins,deltammin,deltammax);  
    tree->Draw("dst2640deltam+2.010>>HBchargebkg",database.GetCut(1,100));
    //B0 B0bar background
    TFile Fbzero(ModeDir+"/SP-1237"+"/Dst2640Reduced.root");
    tree=(TTree*)Fbzero.Get("Dst2640Ntuple");
    gROOT->cd();
    cout<<"Bzero ntuple openened with "<<tree->GetEntries()<<" entries"<<endl; 
    TH1F HBzerobkg("Hzerobkg","B0 B0bar background",ndeltambins,deltammin,deltammax);
    tree->Draw("dst2640deltam+2.010>>HBzerobkg",database.GetCut(1,100));
    //now add the 2
    Hbbbarbkg.Add(&HBchargedbkg,1);
    Hbbbarbkg.Add(&HBzerobkg,1);
    Hbbbarbkg.Scale(scalefactorbbbar);
    cout<<"bbbar scaled bkg: "<<Hbbbarbkg.Integral()<<endl;
  }

 //  //tau background
  TH1F Htaubkg("Htaubkg","Tau background",ndeltambins,deltammin,deltammax);
  if(tau){
    TFile Ftau(ModeDir+"/SP-3429"+"/Dst2640Reduced.root");
    tree=(TTree*)Ftau.Get("Dst2640Ntuple");
    gROOT->cd();
    cout<<"tau ntuple openened with "<<tree->GetEntries()<<" entries"<<endl; 
    tree->Draw("dst2640deltam+2.010>>Htaubkg",database.GetCut(1,100));
    Htaubkg.Scale(scalefactortau);
    cout<<"tau scaled bkg: "<<Htaubkg.Integral()<<endl;
  }

  //make plot of ccbar plus uds
  TH1F Hccbarplusuds("Hccbarplusuds","ccbar plus uds Background",ndeltambins,deltammin,deltammax);
  if(ccbar)Hccbarplusuds.Add(&Hccbarbkg,1);
  if(uds)Hccbarplusuds.Add(&Hudsbkg,1);
  Hccbarplusuds.SetFillColor(3);
  Hccbarplusuds.SetFillStyle(1001);
  Hccbarplusuds.SetLineColor(3);
  Hccbarplusuds.SetStats(0); 


//   //make plot of ccbar plus uds plus bbbar
  TH1F Hccbarplusudsplusbbbar("Hccbarplusudsplusbbbar","ccbar plus uds plus bbbar Background",ndeltambins,deltammin,deltammax);
  if(ccbar)Hccbarplusudsplusbbbar.Add(&Hccbarbkg,1);
  if(uds)Hccbarplusudsplusbbbar.Add(&Hudsbkg,1);
  if(bbbar)Hccbarplusudsplusbbbar.Add(&Hbbbarbkg,1);
  Hccbarplusudsplusbbbar.SetFillColor(6);
  Hccbarplusudsplusbbbar.SetFillStyle(1001);
  Hccbarplusudsplusbbbar.SetLineColor(6);
  Hccbarplusudsplusbbbar.SetStats(0); 
  

  //Add all the bkgs
  TH1F HTotalBkg("HTotalBkg","Total Background",ndeltambins,deltammin,deltammax);
  if(ccbar)HTotalBkg.Add(&Hccbarbkg,1);
  if(uds)HTotalBkg.Add(&Hudsbkg,1);
  if(bbbar)HTotalBkg.Add(&Hbbbarbkg,1);
  if(tau)HTotalBkg.Add(&Htaubkg,1);
  HTotalBkg.SetFillColor(4);
  HTotalBkg.SetFillStyle(1001);
  HTotalBkg.SetLineColor(4);
  HTotalBkg.SetStats(0); 
  HTotalBkg.GetXaxis()->SetTitle("m(D*#pi#pi)-m(D*)+ m_{D*}");
  Float_t binw=(deltammax-deltammin)/ndeltambins;
  HTotalBkg.GetYaxis()->SetTitle(TString("Entries/")+((long)(1e3*binw)%1000)+"."+((long)(1e5*binw)%100)+" MeV");
  

  //Make plot with signal plus background
  TH1F HBkgPlusSignal("HBkgPlusSignal","Signal Plus Bkg",ndeltambins,deltammin,deltammax);
  HBkgPlusSignal.Add(&HTotalBkg);
  if(signal)HBkgPlusSignal.Add(&Hsignal);
  HBkgPlusSignal.SetFillColor(3);
  HBkgPlusSignal.SetLineColor(3);
  HBkgPlusSignal.SetStats(0); 
  HBkgPlusSignal.SetMarkerSize(.8);

//   //Make plot with signal plus background in Worse Scenario
//   TH1F HBkgPlusSignalWorse("HBkgPlusSignalWorse","SignalWorse Plus Bkg",ndeltambins,deltammin,deltammax);
//   HBkgPlusSignalWorse.Add(&HTotalBkg);
//   if(signal)HBkgPlusSignalWorse.Add(&HsignalWorse);
//   HBkgPlusSignalWorse.SetFillColor(3);
//   HBkgPlusSignalWorse.SetLineColor(3);
//   HBkgPlusSignalWorse.SetStats(0); 
//   HBkgPlusSignalWorse.SetMarkerSize(.8);

  

  Canvas.Clear(); 
  HTotalBkg.Draw();
  HTotalBkg.Draw("axis same");
  Canvas.Print(fname);
 
  Canvas.Clear();   
  HTotalBkg.Draw();
  Hccbarbkg.Draw("same");
  HTotalBkg.Draw("axis same");
  Canvas.Print(fname);


  if(signal){
    Canvas.Clear();   
    HBkgPlusSignal.Draw(); 
    HTotalBkg.Draw("same"); 
    HBkgPlusSignal.Draw("axis same");
    Canvas.Print(fname);
  }

//   if(signal){
//     Canvas.Clear();   
//     HBkgPlusSignalWorse.Draw(); 
//     HTotalBkg.Draw("same"); 
//     HBkgPlusSignalWorse.Draw("axis same");
//     Canvas.Print(fname);
//   }



  //Now we need to create a plot of the pstar cut on the Dst2640
  cout<<"Going to draw pstar distributions"<<endl;

  Float_t pstarcut=database.GetDst2640pstarMin();
  sprintf(textlabel,"dst2640pstar>%.5f",pstarcut);
  TCut pCut=textlabel;

  TH1F HccbarbkgSignal("HccbarbkgSignal","ccbar background",20,2.600,2.700);
  TH1F Hccbarbkgpstar("Hccbarbkgpstar","ccbar background",Dst2640p3CMNbins,Dst2640p3CMmin,Dst2640p3CMmax);
  if(ccbar){
    TFile Fccbar(ModeDir+"/SP-1005"+"/Dst2640Reduced.root","read");
    tree=(TTree*)Fccbar.Get("Dst2640Ntuple");    
    gROOT->cd();
    cout<<"ccbar ntuple openened with "<<tree->GetEntries()<<" entries"<<endl; 
    tree->Draw("dst2640pstar>>Hccbarbkgpstar",database.GetCut(1,100));
    Hccbarbkgpstar.Scale(scalefactorccbar);
    Hccbarbkgpstar.SetStats(0);
    
    //
    tree->Draw("dst2640deltam+2.010>>Hccbarbkg",database.GetCut(1,100)+pCut);
    Hccbarbkg.Scale(scalefactorccbar);
    Hccbarbkg.SetStats(0);
    
    //signal region
    tree->Draw("dst2640deltam+2.010>>HccbarbkgSignal",database.GetCut(1,100)+pCut);
    HccbarbkgSignal.Scale(scalefactorccbar);
//     HccbarbkgSignal.SetFillColor(2);
//     HccbarbkgSignal.SetFillStyle(1001);
//     HccbarbkgSignal.SetLineColor(2);
    HccbarbkgSignal.SetStats(0);
    HccbarbkgSignal.GetYaxis()->SetTitle("Entries/5.0 MeV");


    cout<<"ccbar scaled bkg momentum: "<<Hccbarbkgpstar.Integral()<<endl;
  }

  Canvas.Clear(); 
  Hccbarbkgpstar.Draw();  
  cutline.DrawLine(pstarcut,0,pstarcut,Hccbarbkgpstar.GetMaximum());
  Canvas.Print(fname);

 
  //return to proper config
  database.SetTruthMatch(TruthMatch);
  database.SetCuts(_WhichCuts);
  
 
  //open the Signal MC Ntuple to get the momentum distribution 
  if(!OpenReducedNtuple()){
    cout<<"Failed to open the file"<<endl;
    return 0;
  }
  if(ReducedNtuple==NULL){
    cout<<"No ntuple"<<endl;
    return 0;
  }

  TH1F Hsignalpstar("Hsignalpstar","Signal MC",Dst2640p3CMNbins,Dst2640p3CMmin,Dst2640p3CMmax);
  ReducedNtuple->Draw("dst2640pstar>>Hsignalpstar",database.GetCut(1,100));
  Hsignalpstar.SetStats(0);

  Canvas.Clear(); 
  Hsignalpstar.Draw();
  cutline.DrawLine(pstarcut,0,pstarcut,Hsignalpstar.GetMaximum());
  Canvas.Print(fname);
  

  Canvas.Clear();   
  HTotalBkg.Draw();
  Hccbarbkg.Draw("same");
  HTotalBkg.Draw("axis same");
  Canvas.Print(fname);


  Canvas.Clear();   
  HccbarbkgSignal.Draw("pe");
  Canvas.Print(fname);
    


//   Canvas.Clear(); 
//   TH1F*hdm=(TH1F*)Hccbarbkg.Clone();
//   hdm->GetYaxis()->SetNdivisions(4);
//   hdm->GetXaxis()->SetNdivisions(4);
//   hdm->GetXaxis()->SetLabelSize(.08);
//   hdm->GetYaxis()->SetLabelSize(.08);
//   hdm->SetStats(0);
//   hdm->SetLineColor(1);
   
//   //    Float_t  xmin=database.GetDst2640DeltaMMin();
//   //     Float_t deltax=(database.GetDst2640DeltaMMax()-database.GetDst2640DeltaMMin())/5;
//   Int_t deltabins=hdm->GetNbinsX()/5;
  
//   Canvas.Divide(2,3);
//   Canvas.cd(1);
//   TH1F*hdm1=(TH1F*)hdm->DrawClone("pe");
//   hdm1->GetXaxis()->SetRange(1,1*deltabins);
//   hdm1->GetYaxis()->SetRangeUser(hdm1->GetBinContent(1),hdm1->GetBinContent(1*deltabins));
//   Canvas.cd(2);
//   TH1F*hdm2=(TH1F*)hdm->DrawClone("pe");
//   hdm2->GetXaxis()->SetRange(1*deltabins,2*deltabins);
//   hdm2->GetYaxis()->SetRangeUser(hdm2->GetBinContent(1*deltabins),hdm2->GetBinContent(2*deltabins));
//   Canvas.cd(3);
//   TH1F*hdm3=(TH1F*)hdm->DrawClone("pe");
//   hdm3->GetXaxis()->SetRange(2*deltabins,3*deltabins);
//   hdm3->GetYaxis()->SetRangeUser(hdm3->GetBinContent(2*deltabins),hdm3->GetBinContent(3*deltabins));
//   cutline.DrawLine(database.GetDst2640DeltaMSignalMin(),hdm3->GetBinContent(2*deltabins),database.GetDst2640DeltaMSignalMin(),hdm3->GetBinContent(3*deltabins));
//   cutline.DrawLine(database.GetDst2640DeltaMSignalMax(),hdm3->GetBinContent(2*deltabins),database.GetDst2640DeltaMSignalMax(),hdm3->GetBinContent(3*deltabins));
//   Canvas.cd(5);
//   TH1F*hdm4=(TH1F*)hdm->DrawClone("pe");
//   hdm4->GetXaxis()->SetRange(3*deltabins,4*deltabins);
//   hdm4->GetYaxis()->SetRangeUser(hdm4->GetBinContent(3*deltabins),hdm4->GetBinContent(4*deltabins));
//   Canvas.cd(6);
//   TH1F*hdm5=(TH1F*)hdm->DrawClone("pe");
//   hdm5->GetXaxis()->SetRange(4*deltabins,5*deltabins);
//   hdm5->GetYaxis()->SetRangeUser(hdm5->GetBinContent(4*deltabins),hdm5->GetBinContent(hdm->GetNbinsX()));
//   Canvas.Update();
//   Canvas.Print(fname);
//   delete hdm1;
//   delete hdm2;
//   delete hdm3;
//   delete hdm4;
//   delete hdm5;





  Canvas.Print(fname+"]");  
  ////////done saving plots
 
  
 
  return 1;
}



