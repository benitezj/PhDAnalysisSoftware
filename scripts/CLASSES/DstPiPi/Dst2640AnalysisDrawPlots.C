Int_t Dst2640Analysis::InitHistos(){
  cout<<"Setting Histograms"<<endl;
 

  //Initialize the histograms
  PiMomentum.Init(&pip3,"Pi Momentum",Dst2640p3CMNbins,Dst2640p3CMmin,Dst2640p3CMmax,"p (GeV/c)","Entries/100MeV");
  PiMomentum.AddCutsByList(&Dst2640Cuts);
 
  Pi1Momentum.Init(&pi1p3,"Pi1 Momentum",40,0,2, "p (GeV/c)","Entries/50MeV");
  Pi1Momentum.AddCutsByList(&Dst2640Cuts);

  Pi2Momentum.Init(&pi2p3,"Pi2 Momentum",40,0,2,"p (GeV/c)","Entries/50MeV");
  Pi2Momentum.AddCutsByList(&Dst2640Cuts);

  SlowPiMomentum.Init(&slowpip3,"Slow Pi Momentum",100,0,1,"p (GeV/c)","Entries/10MeV");
  SlowPiMomentum.AddCutsByList(&Dst2640Cuts);

  KMomentum.Init(&kp3,"K Momentum",Dst2640p3CMNbins,Dst2640p3CMmin,Dst2640p3CMmax,"p (GeV/c)","Entries/100MeV");
  KMomentum.AddCutsByList(&Dst2640Cuts);
 
  PiLH.InitInt(&pilh,"Pi Likelihood",4,1.5,5.5,"LH Selector","Entries");
  PiLH.AddCutsByList(&Dst2640Cuts);

  KLH.InitInt(&klh,"K Likelihood",5,9.5,14.5,"LH Selector","Entries");
  KLH.AddCutsByList(&Dst2640Cuts);
 
  D0PstarCuts.Init(&d0pstar,"D0 CM Momentum ",Dst2640p3CMNbins*4,Dst2640p3CMmin,Dst2640p3CMmax,"p* (GeV/c)","Entries/100MeV");
  D0PstarCuts.AddCutsByList(&Dst2640Cuts);

  D0MassCuts.Init(&d0mass,"D0 Mass ",100,database.GetD0MassRangeMin(),database.GetD0MassRangeMax(),"D0 Cand. Mass","Entries/1MeV");
  D0MassCuts.AddCutsByList(&Dst2640Cuts);
  
  D0ProbCuts.Init(&d0logvtxprob,"D0 Vertex Probability ",100,-5,0.000001,"log(probability) ","Entries/.05");
  D0ProbCuts.AddCutsByList(&Dst2640Cuts);
    
  DstarDeltaMCuts.Init(&dstardeltam,"D* DeltaM ",100,database.GetDstarDeltaMRangeMin(),database.GetDstarDeltaMRangeMax(),"D* Mass - D0 Mass ","Entries/.05MeV");
  DstarDeltaMCuts.AddCutsByList(&Dst2640Cuts);
  DstarDeltaMCuts.RemoveCut(&DstarDeltaMCut);

  DstarCosThetaCuts.Init(&dstarcostheta,"D*#pi#pi Helicity",20,-1.00001,1.00001,"cos(#theata_{H})","Entries/.1");
  DstarCosThetaCuts.AddCutsByList(&Dst2640Cuts);

  Dst2640ProbCuts.Init(&dst2640logvtxprob,"D*(2640) Vertex Probability ",100,-5,0.000001,"log(probability) ","Entries/.05");
  Dst2640ProbCuts.AddCutsByList(&Dst2640Cuts);


  Dst2640MassCuts.Init(&dst2640mass,"Dst2640 Mass ",100,database.GetDst2640MassRangeMin(),database.GetDst2640MassRangeMax(),"Dst2640 Cand. Mass","Entries/1MeV");
  Dst2640MassCuts.AddCutsByList(&Dst2640Cuts);

  Dst2640DeltaMCuts.Init(&dst2640deltam,"D*(2640) delta M",database.GetDst2640DeltaMNbins(),database.GetDst2640DeltaMRangeMin(),database.GetDst2640DeltaMRangeMax(),"Dst2640 Cand. DeltaM","");
  Dst2640DeltaMCuts.AddCutsByList(&Dst2640Cuts);

  Dst2640pstarCuts.Init(&dst2640pstar,"Dst2640 p*",Dst2640p3CMNbins,Dst2640p3CMmin,Dst2640p3CMmax,"p* (GeV/c)","Entries/100MeV");
  Dst2640pstarCuts.AddCutsByList(&Dst2640Cuts);
  
  Dst2640cosstarCuts.Init(&dst2640cosstar,"Dst2640 cos(#theta*)",Dst2640CosstarNbins,Dst2640Cosstarmin,Dst2640Cosstarmax,"cos(#theta*)","Entries/.02");
  Dst2640cosstarCuts.AddCutsByList(&Dst2640Cuts);

  H2Dst2640CMPvsTheta=new TH2F();
  SetHisto2D(H2Dst2640CMPvsTheta,"#theta* vs p*",Dst2640p3CMNbins,Dst2640p3CMmin,Dst2640p3CMmax,"p* (GeV/c)",Dst2640CosstarNbins,Dst2640Cosstarmin,Dst2640Cosstarmax,"cos(#theta*)","");

  H2DstarDeltaMVsD0Mass=new TH2F();
  SetHisto2D(H2DstarDeltaMVsD0Mass,"D* deltaM vs D0 Mass",200,database.GetD0MassRangeMin(),database.GetD0MassRangeMax(),"m(D^{0}) (GeV/c^{2})",200,database.GetDstarDeltaMRangeMin(),database.GetDstarDeltaMRangeMax(),"m(D*)-m(D^{0}) (GeV/c^{2})","");
  H2DstarDeltaMVsD0Mass->SetStats(0);
  
  
  H2D0pstarVsDst2640pstar=new TH2F();
  SetHisto2D(H2D0pstarVsDst2640pstar,"D^{0} p* vs. D*(2640) p*",Dst2640p3CMNbins,Dst2640p3CMmin,Dst2640p3CMmax,"p* (GeV/c)",Dst2640p3CMNbins*4,Dst2640p3CMmin,Dst2640p3CMmax,"p* (GeV/c)","");

  SetHistoXY(&HRecoNDst2640,"HRecoNDst2640",11,-.5,10.5,"nDst2640/event","Counts");

  if(!InitModeHistos())return 0 ;

  cout<<"Done initializing histograms."<<endl;
  return 1;
}
Int_t Dst2640Analysis::InitModeHistos(){

  if(_Mode=="D0ToK3Pi"){}
  return 1;
}

Int_t Dst2640Analysis::ReadReducedNtuple(){

  if(!OpenReducedNtuple()){
    cout<<"Failed to open ntuple"<<endl;
    return 0;
  }

  //initialize histograms
  if(!InitHistos())return 0;
 


  cout<<"Looping over the reduced ntuple"<<endl;
  
  Int_t preveventid=1;
  Int_t ncands=0;
  ///Fill histograms
  Long_t MaxNumberCands=1000000000;
  if(_RunInTestMode)MaxNumberCands=10000;
  
  Int_t cand=0;
  while(ReducedNtuple->GetEntry(cand,0)>0&&cand<MaxNumberCands){   
    cand++;
    if(cand%10000==0)cout<<cand<<" Candidates done."<<endl;    

    //count the number of candidates per event   
    if(Dst2640Cuts.EvalCuts(1,100)){
      if(eventnumber==preveventid){
	ncands++;      
      }else{
	if(preveventid!=1)HRecoNDst2640.Fill(ncands);
	ncands=1;   
	preveventid=eventnumber;
      }
    }

    if(Dst2640Cuts.EvalCuts(1,100)){     
      H2Dst2640CMPvsTheta->Fill(dst2640pstar,dst2640cosstar);
      H2D0pstarVsDst2640pstar->Fill(dst2640pstar,d0pstar);
    }

    //if(Dst2640Cuts.EvalCuts(1,3)&&Dst2640Cuts.EvalCuts(5,6)&&Dst2640Cuts.EvalCuts(8,100))   
    //if(Dst2640Cuts.EvalCuts(1,100)) 
    H2DstarDeltaMVsD0Mass->Fill(d0mass,dstardeltam);

    D0PstarCuts.FillPlots();
    D0MassCuts.FillPlots();
    D0ProbCuts.FillPlots();
    Dst2640ProbCuts.FillPlots();
    KLH.FillPlots();
    PiLH.FillPlots();
    Dst2640pstarCuts.FillPlots();
    DstarCosThetaCuts.FillPlots();

    DstarDeltaMCuts.FillPlots();
    Dst2640MassCuts.FillPlots();
    Dst2640DeltaMCuts.FillPlots();

    Dst2640cosstarCuts.FillPlots(); 
    PiMomentum.FillPlots();
    KMomentum.FillPlots();
    Pi1Momentum.FillPlots();
    Pi2Momentum.FillPlots();
    SlowPiMomentum.FillPlots();


  }
  cout<<"Total Candidates="<<cand<<endl;


 

  if(!PrintPlots())return 0; 
 

  if(!Close()){
    cout<<"Failed to close file"<<endl;
    return 0;
  }
  

  return 1;
}


Int_t Dst2640Analysis::PrintPlots(){
  
  cout<<"Going to create ps file with plots."<<endl;

  
  if(TruthMatch){
    /////MC stuff get from file
    if(!(HMCDst2640Mass=(TH1F*)OutPutFile->Get("HMCDst2640Mass"))){
      cout<<"No saved histogram HMCDst2640Mass "<<endl;return 0;}
    HMCDst2640Mass->SetDirectory(0);
    if(!(HMCDst2640DeltaM=(TH1F*)OutPutFile->Get("HMCDst2640DeltaM"))){
      cout<<"No saved histogram HMCDst2640DeltaM "<<endl;return 0;}
    HMCDst2640DeltaM->SetDirectory(0); 
    if(!(HMCDst2640p3CM=(TH1F*)OutPutFile->Get("HMCDst2640p3CM"))){
      cout<<"No saved histogram HMCDst2640p3CM"<<endl;return 0;}
    HMCDst2640p3CM->SetDirectory(0);
    if(!(HMCDst2640costhCM=(TH1F*)OutPutFile->Get("HMCDst2640costhCM"))){
      cout<<"No saved histogramHMCDst2640costhCM "<<endl;return 0;}
    HMCDst2640costhCM->SetDirectory(0);
    if(!(H2MCDst2640CMPvsTheta=(TH2F*)OutPutFile->Get("H2MCDst2640CMPvsTheta"))){
      cout<<"No saved histogramH2MCDst2640CMPvsTheta "<<endl;return 0;}
    H2MCDst2640CMPvsTheta->SetDirectory(0);
    
    if(!(HMCNDst2640=(TH1F*)OutPutFile->Get("HMCNDst2640"))){
      cout<<"No saved histogram HMCNDst2640"<<endl;return 0;}
    HMCNDst2640->SetDirectory(0);  

    if(!(HLundCheck=(TH1F*)OutPutFile->Get("HLundCheck"))){
      cout<<"No saved histogram HLundCheck "<<endl;return 0;}  
    HLundCheck->SetDirectory(0);
    
    cout<<"Histograms extracted."<<endl;
  }

  //////// PS File.
  TString filename;
  filename=_OutputDir+"/"+"Plots"+(long)_WhichCuts+".ps";
  TCanvas Canvas(filename,filename);
  Canvas.Print(filename+"[");

  ////////

  Canvas.Clear();
  PiMomentum.DrawPlots(100,100,"");
  Canvas.SetLogy(0);
  Canvas.Print(filename); 
  Canvas.SetLogy(0);

  Canvas.Clear();
  KMomentum.DrawPlots(100,100,"");
  Canvas.SetLogy(0);
  Canvas.Print(filename); 
  Canvas.SetLogy(0);

  Canvas.Clear();
  SlowPiMomentum.DrawPlots(100,100,"");
  Canvas.SetLogy(0);
  Canvas.Print(filename); 
  Canvas.SetLogy(0);

  Canvas.Clear();
  Pi1Momentum.DrawPlots(100,100,"");
  Canvas.SetLogy(0);
  Canvas.Print(filename); 
  Canvas.SetLogy(0);

  Canvas.Clear(); 
  Pi2Momentum.DrawPlots(100,100,"");
  Canvas.SetLogy(0);
  Canvas.Print(filename); 
  Canvas.SetLogy(0);


  Canvas.Clear(); 
  DstarCosThetaCuts.DrawPlots(100,100,"");
  Canvas.Print(filename); 
  

  ////////Draw all cuts in one page
  DrawCutSummary(&Canvas,filename);  

  //
  Canvas.Clear(); 
  H2DstarDeltaMVsD0Mass->Draw("colz");
  cutbox.DrawBox(database.GetD0MassMin(),database.GetDstarDeltaMMin(),database.GetD0MassMax(),database.GetDstarDeltaMMax());
  //D0MassAndDstarDMCut.DrawCut();
  Canvas.Print(filename); 


  Canvas.Clear(); 
  DstarDeltaMCuts.DrawPlots(100,100,"");
  Float_t cutheight=DstarDeltaMCuts.GetPlot(100)->GetMaximum();
  cutline.DrawLine(database.GetDstarDeltaMMin(),0,database.GetDstarDeltaMMin(),cutheight);
  cutline.DrawLine(database.GetDstarDeltaMMax(),0,database.GetDstarDeltaMMax(),cutheight);
  Canvas.Print(filename); 

  Canvas.Clear(); 
  H2D0pstarVsDst2640pstar->Draw("colz");
  Canvas.Print(filename); 

  Canvas.Clear();
  Dst2640MassCuts.DrawPlots(100,100,"");
  Canvas.Print(filename);

  if(TruthMatch){
    Canvas.Clear();
    HMCDst2640Mass->Draw();
    Canvas.Print(filename);
  }
  
  Canvas.Clear();
  Dst2640DeltaMCuts.DrawPlots(100,100,"pe");
  cutline.DrawLine(database.GetDst2640DeltaMSignalMin(),0,
		   database.GetDst2640DeltaMSignalMin(),Dst2640DeltaMCuts.GetPlot(100)->GetMaximum());
  cutline.DrawLine(database.GetDst2640DeltaMSignalMax(),0,
		   database.GetDst2640DeltaMSignalMax(),Dst2640DeltaMCuts.GetPlot(100)->GetMaximum());
  Canvas.Print(filename);

 
  if(TruthMatch){
    Canvas.Clear();
    HMCDst2640DeltaM->Draw();
    Canvas.Print(filename);
  }


  /*
  ///make a simple fit
  Canvas.Clear(); 
  TH1F HDst2640DM("HDst2640DM","Dst2640 DeltaM",database.GetDst2640DeltaMNbins(),database.GetDst2640DeltaMMin()+DstarPDGMass,database.GetDst2640DeltaMMax()+DstarPDGMass);
  HDst2640DM.GetXaxis()->SetTitle(DstarPiPiXLabel);//"m(D*#pi#pi) - m(D*) + m_{D*}  (GeV/c^{2})");
  HDst2640DM.GetYaxis()->SetTitle(DstarPiPiYLabel);
  HDst2640DM.SetStats(0);
  HDst2640DM.SetMarkerSize(.8);
  HDst2640DM.SetLineWidth(3);
  ReducedNtuple->Draw("dst2640deltam+2.010>>HDst2640DM",database.GetCut(1,100),"pe");
  HDst2640DM.GetYaxis()->SetRangeUser(.1,HDst2640DM.GetMaximum()*1.05); 
  cutline.DrawLine(database.GetDst2640DeltaMSignalMin()+DstarPDGMass,0,database.GetDst2640DeltaMSignalMin()+DstarPDGMass,HDst2640DM.GetMaximum());
  cutline.DrawLine(database.GetDst2640DeltaMSignalMax()+DstarPDGMass,0,database.GetDst2640DeltaMSignalMax()+DstarPDGMass,HDst2640DM.GetMaximum());
  Canvas.Print(filename);
  */

  ////charge dependence
  Canvas.Clear();
  Canvas.Divide(1,2);
  Canvas.cd(1);
  TCut PosChargeCut="dst2640charge==1";
  TH1F HDst2640DMPos("HDst2640DMPos","Positive",Dst2640DeltaMNbins,database.GetDst2640DeltaMMin(),database.GetDst2640DeltaMMax());
  ReducedNtuple->Draw("dst2640deltam>>HDst2640DMPos",database.GetCut(1,100)+PosChargeCut);
  HDst2640DMPos.GetYaxis()->SetRangeUser(0,HDst2640DMPos.GetMaximum()*1.05);
  Canvas.cd(2);
  TCut NegChargeCut="dst2640charge==-1";
  TH1F HDst2640DMNeg("HDst2640DMNeg","Negative",Dst2640DeltaMNbins,database.GetDst2640DeltaMMin(),database.GetDst2640DeltaMMax());
  ReducedNtuple->Draw("dst2640deltam>>HDst2640DMNeg",database.GetCut(1,100)+NegChargeCut);
  HDst2640DMNeg.GetYaxis()->SetRangeUser(0,HDst2640DMNeg.GetMaximum()*1.05);
  Canvas.Print(filename);



  //Draw the mass spectrum as a function of p*
  //Int_t DrawBackground=0;
  //if(!TruthMatch)DrawBackground=1;
  //DrawMassPlots(&Canvas,filename,0,DrawBackground);
  //DrawMassPlots(&Canvas,filename,1,DrawBackground);



  Canvas.Clear();
  if(TruthMatch){
    HMCDst2640p3CM->GetYaxis()->SetRangeUser(.1,1.05*HMCDst2640p3CM->GetMaximum());
    HMCDst2640p3CM->Draw();
    Dst2640pstarCuts.DrawPlots(100,100,"same");
  }else Dst2640pstarCuts.DrawPlots(100,100,"");
  Canvas.Print(filename);


  /*
  Canvas.Clear();
  TH1F Hccbarbkgpstar("Hccbarbkgpstar","ccbar background",Dst2640p3CMNbins,Dst2640p3CMmin,Dst2640p3CMmax);    
  if(!TruthMatch){
    ccbartree->Draw("dst2640pstar>>Hccbarbkgpstar",database.GetCut(1,100));
    Hccbarbkgpstar.Scale(scalefactorccbar);
    Hccbarbkgpstar.Draw();
    MoveStatsBox(&Hccbarbkgpstar,1,0);
  }
  Dst2640pstarCuts.DrawPlots(100,100,"pe"); 
  Hccbarbkgpstar.Draw("same");  
  Canvas.Print(filename);
  */

  Canvas.Clear();
  if(TruthMatch){
    HMCDst2640costhCM->GetYaxis()->SetRangeUser(.1,1.05*HMCDst2640costhCM->GetMaximum());
    HMCDst2640costhCM->Draw();
    Dst2640cosstarCuts.DrawPlots(100,100,"same");
  }else Dst2640cosstarCuts.DrawPlots(100,100,"");
  Canvas.Print(filename);

  if(TruthMatch){
    Canvas.Clear();
    H2MCDst2640CMPvsTheta->Draw("colz");
    Canvas.Print(filename);
  }

  Canvas.Clear();
  H2Dst2640CMPvsTheta->Draw("colz");
  Canvas.Print(filename);
    

  if(TruthMatch){
    //a histogram showing the efficiencies
    TH1F HEfficiencies;
    Int_t ncuts=Dst2640pstarCuts.GetNCuts();
    SetHistoXY(&HEfficiencies,"HEfficiencies",ncuts,.5,(float)ncuts+.5,"Cut id","Efficiency");
    Float_t eff=1;
    for(Int_t id=1;id<=ncuts;id++){
      eff=Dst2640pstarCuts.GetPlot(id)->Integral()/HMCDst2640p3CM->Integral();
      HEfficiencies.SetBinContent(id,eff);
    }
    Canvas.Clear();
    HEfficiencies.SetFillStyle(1);
    HEfficiencies.SetMinimum(0);
    HEfficiencies.Draw("bar");
    HEfficiencies.SetStats(kFALSE);
    HEfficiencies.Draw("bar");
    text.SetTextSize(.03);
    for(Int_t id=1;id<=ncuts;id++){
      sprintf(textlabel,"%.3f",(float)HEfficiencies.GetBinContent(id));
      text.DrawText(HEfficiencies.GetBinLowEdge(id),.8*HEfficiencies.GetBinContent(id),textlabel);
    }
    Canvas.Print(filename);
    
 
    Canvas.Clear();
    Dst2640pstarCuts.DrawRatios(100,100,HMCDst2640p3CM,"");//start from 2, 0=nocuts,1=mctrue
    Canvas.Print(filename);
        
    Canvas.Clear();
    Dst2640cosstarCuts.DrawRatios(100,100,HMCDst2640costhCM,"");
    Canvas.Print(filename);
    
    Canvas.Clear();
    TH2F* H2Dst2640CMPvsThetaEff=(TH2F*)H2Dst2640CMPvsTheta->Clone();
    Int_t bincontent=0; 
    for(Int_t binx=0;binx<=H2MCDst2640CMPvsTheta->GetNbinsX();binx++)
      for(Int_t biny=0;biny<=H2MCDst2640CMPvsTheta->GetNbinsY();biny++){
	bincontent=(int)H2MCDst2640CMPvsTheta->GetBinContent(binx,biny);
	if(bincontent>100){	  	  
	  H2Dst2640CMPvsThetaEff->SetBinContent(binx,biny,H2Dst2640CMPvsThetaEff->GetBinContent(binx,biny)/bincontent);
	}else H2Dst2640CMPvsThetaEff->SetBinContent(binx,biny,0);
      }
    H2Dst2640CMPvsThetaEff->Draw("colz");
    Canvas.Print(filename);
    
    ///Make plot of TruthMatched vs particle: 
    DrawTruthRatios(&Canvas,filename);
    
    DrawDalitzPlots(&Canvas,filename);
      
    Canvas.Clear();
    HMCNDst2640->Draw();   
    Canvas.Print(filename);

    
  }
  
  Canvas.Clear();
  Canvas.SetLogy(1); 
  HRecoNDst2640.Draw();   
  Canvas.Print(filename);
  Canvas.SetLogy(0);


  if(TruthMatch){
    Canvas.Clear(); 
    HLundCheck->SetBarOffset(0);
    HLundCheck->SetBarWidth(.05);
    HLundCheck->Draw();
    Canvas.Print(filename);
  }


  PrintModePlots(&Canvas,filename);


  Canvas.Print(filename+"]");
  

  delete H2Dst2640CMPvsTheta;
  delete H2DstarDeltaMVsD0Mass;
  delete H2D0pstarVsDst2640pstar;

  return 1;
}

Int_t Dst2640Analysis::PrintModePlots(TCanvas* Canvas,TString filename){
 
  
  if(_Mode=="D0ToK3Pi"){
    //Canvas->Print(filename);
    Canvas=NULL;
    filename="";
  }

  return 1; 
}


