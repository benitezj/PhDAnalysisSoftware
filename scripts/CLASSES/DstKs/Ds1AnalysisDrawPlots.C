
Int_t Ds1Analysis::InitHistos(){
  cout<<"Setting Histograms"<<endl;
 

  //Initialize the histograms
  PiMomentum.Init(&pip3,"Pi Momentum",Ds1pp3CMNbins,Ds1pp3CMmin,Ds1pp3CMmax,"p (GeV/c)","Entries/100MeV");
  PiMomentum.AddCutsByList(&Ds1pCuts);
 
  PiLH.InitInt(&pilh,"Pi Likelihood",4,1.5,5.5,
	       "LH Selector","Entries");
  PiLH.AddCut(&Ds1pMCTrueCut);
  PiLH.AddCut(&Ds1pDeltaM2Cut);
  PiLH.AddCut(&D0pstarCut);
  PiLH.AddCut(&DstarDeltaMCut);
  PiLH.AddCut(&KLHCut);
 
  KMomentum.Init(&kp3,"K Momentum",Ds1pp3CMNbins,Ds1pp3CMmin,Ds1pp3CMmax,"p (GeV/c)","Entries/100MeV");
  KMomentum.AddCutsByList(&Ds1pCuts);
  
  KLH.InitInt(&klh,"K Likelihood",5,9.5,14.5,"LH Selector","Entries");
  KLH.AddCut(&Ds1pMCTrueCut);
  KLH.AddCut(&Ds1pDeltaM2Cut);
  KLH.AddCut(&D0pstarCut);
  KLH.AddCut(&DstarDeltaMCut);

  Pi1Momentum.Init(&pi1p3,"Pi1 Momentum",40,0,2,"p (GeV/c)","Entries/50MeV");
  Pi1Momentum.AddCutsByList(&Ds1pCuts);

  Pi2Momentum.Init(&pi2p3,"Pi2 Momentum",40,0,2,"p (GeV/c)","Entries/50MeV");
  Pi2Momentum.AddCutsByList(&Ds1pCuts);
  

  SlowPiMomentum.Init(&slowpip3,"Slow Pi Momentum",100,0,1,
		      "p (GeV/c)","Entries/10MeV");
  SlowPiMomentum.AddCutsByList(&Ds1pCuts);
  
  D0PstarCuts.Init(&d0pstar,"D0 CM Momentum ",Ds1pp3CMNbins*4,Ds1pp3CMmin,Ds1pp3CMmax,
		   "p* (GeV/c)","Entries/100MeV");
  D0PstarCuts.AddCut(&Ds1pMCTrueCut);
  D0PstarCuts.AddCut(&Ds1pDeltaM2Cut);
 
  D0MassCuts.Init(&d0mass,"D0 Mass ",100,D0PDGMass-database.GetD0MassRangeWindow(),D0PDGMass+database.GetD0MassRangeWindow(),"D0 Cand. Mass","Entries/1MeV");
   D0MassCuts.AddCut(&Ds1pMCTrueCut);
  D0MassCuts.AddCut(&Ds1pDeltaM2Cut);
  D0MassCuts.AddCut(&D0pstarCut);
  D0MassCuts.AddCut(&DstarDeltaMCut);
  D0MassCuts.AddCut(&KLHCut);
  D0MassCuts.AddCut(&PiLHCut);

  
  D0ProbCuts.Init(&d0logvtxprob,"D0 Vertex Probability ",100,-5,0.000001,"log(probability)","Entries/.05");
  D0ProbCuts.AddCut(&Ds1pMCTrueCut);
  D0ProbCuts.AddCut(&Ds1pDeltaM2Cut);
  D0ProbCuts.AddCut(&D0pstarCut);
  D0ProbCuts.AddCut(&DstarDeltaMCut);
  D0ProbCuts.AddCut(&KLHCut);
  D0ProbCuts.AddCut(&PiLHCut);
  D0ProbCuts.AddCut(&D0MassCut);

  KsMassCuts.Init(&ksmass,"Ks Mass ",60,K0PDGMass-database.GetKsMassRangeWindow(),K0PDGMass+database.GetKsMassRangeWindow(),"Ks Cand. Mass","Entries/1MeV");
 
  KsMassCuts.AddCut(&Ds1pMCTrueCut);
  KsMassCuts.AddCut(&Ds1pDeltaM2Cut);
  KsMassCuts.AddCut(&D0pstarCut);
  KsMassCuts.AddCut(&DstarDeltaMCut);
  KsMassCuts.AddCut(&KLHCut);
  KsMassCuts.AddCut(&PiLHCut);
  KsMassCuts.AddCut(&D0MassCut);
  KsMassCuts.AddCut(&D0ProbCut);

  KsCosineCuts.Init(&kscosine,"Ks Direction",100,Ds1Cosstarmin,Ds1Cosstarmax,"Ks Cand. cosine","Entries/.02");
  KsCosineCuts.AddCut(&Ds1pMCTrueCut);
  KsCosineCuts.AddCut(&Ds1pDeltaM2Cut);
  KsCosineCuts.AddCut(&D0pstarCut);
  KsCosineCuts.AddCut(&DstarDeltaMCut);
  KsCosineCuts.AddCut(&KLHCut);
  KsCosineCuts.AddCut(&PiLHCut);
  KsCosineCuts.AddCut(&D0MassCut);
  KsCosineCuts.AddCut(&D0ProbCut);
  KsCosineCuts.AddCut(&KsMassCut);
 
  DstarDeltaMCuts.Init(&dstardeltam,"D* DeltaM ",DstarDeltaMNbins,DstarDeltaMmin,DstarDeltaMmax,
		       "D* Mass - D0 Mass ","Entries/.05MeV");
  DstarDeltaMCuts.AddCut(&Ds1pMCTrueCut);
  DstarDeltaMCuts.AddCut(&Ds1pDeltaM2Cut);
  DstarDeltaMCuts.AddCut(&D0pstarCut);
  DstarDeltaMCuts.AddCut(&KLHCut);
  DstarDeltaMCuts.AddCut(&PiLHCut);
  DstarDeltaMCuts.AddCut(&D0MassCut);
  DstarDeltaMCuts.AddCut(&D0ProbCut);
  //DstarDeltaMCuts.AddCut(&KsMassCut);
  //DstarDeltaMCuts.AddCut(&KsCosineCut);

  Ds1pMassCuts.Init(&ds1pmass,"Ds1 Mass ",100,Ds1pPDGMass-Ds1pMassWindow,Ds1pPDGMass+Ds1pMassWindow,
		    "Ds1 Cand. Mass","Entries/1MeV");
  Ds1pMassCuts.AddCutsByList(&Ds1pCuts);

  Ds1pDeltaM1Cuts.Init(&ds1pdeltam1,"Ds1 DeltaM1 ",100,Ds1pPDGDeltaM1-Ds1pMassWindow,Ds1pPDGDeltaM1+Ds1pMassWindow,
		       "Ds1 Mass - D* Mass","Entries/1MeV");
  Ds1pDeltaM1Cuts.AddCutsByList(&Ds1pCuts);
  
  Ds1pProbCuts.Init(&ds1plogvtxprob,"Ds1 Prob",100,-10,0.00001,"log(probability)","Entries/.1");
  Ds1pProbCuts.AddCut(&Ds1pMCTrueCut);
  Ds1pProbCuts.AddCut(&Ds1pDeltaM2Cut);//need this cut for background summary
  Ds1pProbCuts.AddCut(&D0pstarCut);
  Ds1pProbCuts.AddCut(&DstarDeltaMCut);
  Ds1pProbCuts.AddCut(&KLHCut);
  Ds1pProbCuts.AddCut(&PiLHCut);
  Ds1pProbCuts.AddCut(&D0MassCut);
  Ds1pProbCuts.AddCut(&D0ProbCut);
  Ds1pProbCuts.AddCut(&KsMassCut);
  Ds1pProbCuts.AddCut(&KsCosineCut);

  Ds1pDeltaM2Cuts.Init(&ds1pdeltam2,"Ds1 DeltaM2 ",Ds1DeltaMNbins,Ds1pPDGDeltaM2-Ds1pMassWindow,Ds1pPDGDeltaM2+Ds1pMassWindow,"Ds1 Mass - D* Mass - K_S Mass","Entries/.5MeV");
  Ds1pDeltaM2Cuts.AddCutsByList(&Ds1pCuts);

  Ds1ppstarCuts.Init(&ds1ppstar,"Ds1 p*",Ds1pp3CMNbins,Ds1pp3CMmin,Ds1pp3CMmax,"D'_{S1} p* (GeV/c)","Entries/100MeV");
  Ds1ppstarCuts.AddCutsByList(&Ds1pCuts);

  Ds1pcosstarCuts.Init(&ds1pcosstar,"Ds1 cos(#theta*)",Ds1CosstarNbins,Ds1Cosstarmin,Ds1Cosstarmax,
		       "D'_{S1} cos(#theta*)","Entries/.02");
  Ds1pcosstarCuts.AddCutsByList(&Ds1pCuts);

  H2Ds1pCMPvsTheta=new TH2F();
  SetHisto2D(H2Ds1pCMPvsTheta,"#theta* vs p*",Ds1pp3CMNbins,Ds1pp3CMmin,Ds1pp3CMmax,
	     "p* (GeV/c)",Ds1CosstarNbins,Ds1Cosstarmin,Ds1Cosstarmax,"cos(#theta*)","");

  H2DstarDeltaMVsD0Mass=new TH2F();
  SetHisto2D(H2DstarDeltaMVsD0Mass,"D* deltaM vs D0 Mass",100,D0PDGMass-database.GetD0MassRangeWindow(),D0PDGMass+database.GetD0MassRangeWindow(),"D0 Mass (GeV/c^{2})",DstarDeltaMNbins,DstarDeltaMmin,DstarDeltaMmax,"D* delta M (GeV/c^{2})","");
  H2DstarDeltaMVsD0Mass->SetStats(0);

  ///settings for number of reco cands
  SetHistoXY(&HRecoNDs1p,"HRecoNDs1p",11,-.5,10.5,"nDs1p/event","Counts");

  InitModeHistos();
  return 1;
}
Int_t Ds1Analysis::InitModeHistos(){

  if(_Mode=="D0ToK3Pi"){}
  return 1;
}

Int_t Ds1Analysis::ReadReducedNtuple(){

  cout<<"Looping over the reduced ntuple"<<endl;
  
  if(!OpenReducedNtuple()){
    cout<<"Failed to open ntuple"<<endl;
    return 0;
  }

  //initialize histograms
  if(!InitHistos())return 0;
 

  Int_t preveventid=1;
  Int_t ncands=0;
  ///Fill histograms
  Int_t cand=0;
  while(ReducedNtuple->GetEntry(cand,0)>0){   
    cand++;
    if(cand%10000==0)cout<<cand<<" Candidates done."<<endl;    

    //count the number of candidates per event   
    if(Ds1pCuts.EvalCuts(1,100)) {
      if(eventnumber==preveventid){
	ncands++;      
      }else{
	if(preveventid!=1)HRecoNDs1p.Fill(ncands);
	ncands=1;   
	preveventid=eventnumber;
      }
    }

    if(Ds1pCuts.EvalCuts(1,100)){    
      H2Ds1pCMPvsTheta->Fill(ds1ppstar,ds1pcosstar);
    }


    //if(Ds1pCuts.EvalCuts(1,3)&&Ds1pCuts.EvalCuts(5,6)&&Ds1pCuts.EvalCuts(8,100))   
    H2DstarDeltaMVsD0Mass->Fill(d0mass,dstardeltam);
  

    KsMassCuts.FillPlots();
    KsCosineCuts.FillPlots();
    D0PstarCuts.FillPlots();
    D0MassCuts.FillPlots();
    D0ProbCuts.FillPlots();
    DstarDeltaMCuts.FillPlots();
    Ds1pMassCuts.FillPlots();
    Ds1pDeltaM1Cuts.FillPlots();
    Ds1pProbCuts.FillPlots();
    Ds1pDeltaM2Cuts.FillPlots();
    Ds1ppstarCuts.FillPlots();
    Ds1pcosstarCuts.FillPlots(); 
    PiMomentum.FillPlots();
    KMomentum.FillPlots();
    Pi1Momentum.FillPlots();
    Pi2Momentum.FillPlots();
    SlowPiMomentum.FillPlots();
    KLH.FillPlots();
    PiLH.FillPlots();  

  }
  cout<<"Total Candidates="<<cand<<endl;


  if(!Close()){
    cout<<"Failed to close file"<<endl;
    return 0;
  }
  

  if(!PrintPlots())return 0; 
 

  return 1;
}


Int_t Ds1Analysis::PrintPlots(){
  
  cout<<"Going to create ps file with plots."<<endl;

  if(!OpenReducedNtuple()){
    cout<<"Failed to open the file"<<endl;
    return 0;
  }



  /////MC stuff get from file
  if(!(HMCDs1pMass=(TH1F*)OutPutFile->Get("HMCDs1pMass"))){
    cout<<"No saved histogram HMCDs1pMass "<<endl;return 0;}
  HMCDs1pMass->SetDirectory(0);
  if(!(HMCDs1pp3CM=(TH1F*)OutPutFile->Get("HMCDs1pp3CM"))){
    cout<<"No saved histogram HMCDs1pp3CM"<<endl;return 0;}
  HMCDs1pp3CM->SetDirectory(0);
  if(!(HMCDs1pcosthCM=(TH1F*)OutPutFile->Get("HMCDs1pcosthCM"))){
    cout<<"No saved histogramHMCDs1pcosthCM "<<endl;return 0;}
  HMCDs1pcosthCM->SetDirectory(0);
  if(!(H2MCDs1pCMPvsTheta=(TH2F*)OutPutFile->Get("H2MCDs1pCMPvsTheta"))){
    cout<<"No saved histogramH2MCDs1pCMPvsTheta "<<endl;return 0;}
  H2MCDs1pCMPvsTheta->SetDirectory(0);
  if(!(HMCNDs1p=(TH1F*)OutPutFile->Get("HMCNDs1p"))){
    cout<<"No saved histogram HMCNDs1p"<<endl;return 0;}
  HMCNDs1p->SetDirectory(0);
  if(!(HLundCheck=(TH1F*)OutPutFile->Get("HLundCheck"))){
    cout<<"No saved histogram HLundCheck "<<endl;return 0;}    
  HLundCheck->SetDirectory(0);

  cout<<"MC Histograms extracted."<<endl;



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



  ////////Draw all cuts in one page
  DrawCutSummary(&Canvas,filename);  

  Canvas.Clear();
  DstarDeltaMCuts.DrawPlots(0,0,"");
  Canvas.Print(filename);
  Canvas.Clear();
  DstarDeltaMCuts.DrawPlots(100,100,"");
  Canvas.Print(filename);
  //
  Canvas.Clear(); 
  H2DstarDeltaMVsD0Mass->Draw("colz");
  cutbox.DrawBox(database.GetD0MassMin(),database.GetDstarDeltaMMin(),database.GetD0MassMax(),database.GetDstarDeltaMMax());
  Canvas.Print(filename); 
 

  Canvas.Clear();
  Ds1pMassCuts.DrawPlots(100,100,"");
  Canvas.Print(filename);


  Canvas.Clear();
  Ds1pDeltaM1Cuts.DrawPlots(100,100,"");
  Canvas.SetLogy(0);
  Canvas.Print(filename);
  Canvas.SetLogy(0); 

  Canvas.Clear();
  Ds1pDeltaM2Cuts.DrawPlots(100,100,"");
  Canvas.SetLogy(0);
  Canvas.Print(filename);
  Canvas.SetLogy(0);

  
  if(TruthMatch){
    ///Make plot of TruthMatched vs particle: 
    DrawTruthRatios(&Canvas,filename);
  }

  ///make a simple fit
  Canvas.Clear(); 
  TH1F HDs1pDM("HDs1pDM","Ds1 DeltaM",Ds1DeltaMNbins,database.GetDs1pDeltaMMin(),database.GetDs1pDeltaMMax());
  ReducedNtuple->Draw("ds1pdeltam2>>HDs1pDM",database.GetCut(1,100));
  Float_t initpar[10]={5,.027,.001,1,1,1};
  FitHistogramRelBreitWignerPolN(&HDs1pDM,database.GetDs1pDeltaMMin(),database.GetDs1pDeltaMMax(),2,initpar);
  cutline.DrawLine(database.GetDs1pDeltaMSignalMin(),0,database.GetDs1pDeltaMSignalMin(),HDs1pDM.GetMaximum());
  cutline.DrawLine(database.GetDs1pDeltaMSignalMax(),0,database.GetDs1pDeltaMSignalMax(),HDs1pDM.GetMaximum());
  Canvas.Print(filename);

 
  ////charge dependence
  Canvas.Clear();
  Canvas.Divide(1,2);
  Canvas.cd(1);
  TCut PosChargeCut="ds1pcharge==1";
  TH1F HDs1pDMPos("HDs1pDMPos","Positive",Ds1DeltaMNbins,database.GetDs1pDeltaMMin(),database.GetDs1pDeltaMMax());
  ReducedNtuple->Draw("ds1pdeltam2>>HDs1pDMPos",database.GetCut(1,100)+PosChargeCut);
  FitHistogramRelBreitWignerPolN(&HDs1pDMPos,database.GetDs1pDeltaMMin(),database.GetDs1pDeltaMMax(),2,initpar);
  Canvas.cd(2);
  TCut NegChargeCut="ds1pcharge==-1";
  TH1F HDs1pDMNeg("HDs1pDMNeg","Negative",Ds1DeltaMNbins,database.GetDs1pDeltaMMin(),database.GetDs1pDeltaMMax());
  ReducedNtuple->Draw("ds1pdeltam2>>HDs1pDMNeg",database.GetCut(1,100)+NegChargeCut);
  FitHistogramRelBreitWignerPolN(&HDs1pDMNeg,database.GetDs1pDeltaMMin(),database.GetDs1pDeltaMMax(),2,initpar);  
  Canvas.Print(filename);



  ///pstar Distribution
  Canvas.Clear();
  if(TruthMatch){
    HMCDs1pp3CM->GetYaxis()->SetRangeUser(.1,1.05*HMCDs1pp3CM->GetMaximum());
    HMCDs1pp3CM->Draw();
    Ds1ppstarCuts.DrawPlots(100,100,"same");
  }else Ds1ppstarCuts.DrawPlots(100,100,"");
  Canvas.Print(filename);
 

  ///Theta Distribution
  Canvas.Clear();
  if(TruthMatch){
    HMCDs1pcosthCM->GetYaxis()->SetRangeUser(.1,1.05*HMCDs1pcosthCM->GetMaximum());
    HMCDs1pcosthCM->Draw();
    Ds1pcosstarCuts.DrawPlots(100,100,"same");
  }else Ds1pcosstarCuts.DrawPlots(100,100,"");
  Canvas.Print(filename);
 


 
  if(TruthMatch){
    ///MC  Theta Vs Momentum
    Canvas.Clear();
    H2MCDs1pCMPvsTheta->Draw("colz");
    Canvas.Print(filename);
  }

  //DATA Theta Vs Momentum
  Canvas.Clear();
  H2Ds1pCMPvsTheta->Draw("colz");
  Canvas.Print(filename);
    
  
  //Efficiencies
  if(TruthMatch){
    
    //a histogram showing the efficiencies as function of cut
    TH1F HEfficiencies;
    Int_t ncuts=Ds1ppstarCuts.GetNCuts();
    SetHistoXY(&HEfficiencies,"HEfficiencies",ncuts,.5,(float)ncuts+.5,"Cut id","Efficiency");
    Float_t eff=1;
    for(Int_t id=1;id<=ncuts;id++){
      eff=Ds1ppstarCuts.GetPlot(id)->Integral()/HMCDs1pp3CM->Integral();
      HEfficiencies.SetBinContent(id,eff);
    }
    Canvas.Clear();
    HEfficiencies.SetMinimum(0);
    HEfficiencies.Draw("bar");
    text.SetTextSize(.03);
    for(Int_t id=1;id<=ncuts;id++){
      sprintf(textlabel,"%.3f",(float)HEfficiencies.GetBinContent(id));
      text.DrawText(HEfficiencies.GetBinLowEdge(id),.8*HEfficiencies.GetBinContent(id),textlabel);
    }
    Canvas.Print(filename);
    
 
    Canvas.Clear();
    Ds1ppstarCuts.DrawRatios(100,100,HMCDs1pp3CM,"");//start from 2, 0=nocuts,1=mctrue
    Canvas.Print(filename);
        
    Canvas.Clear();
    Ds1pcosstarCuts.DrawRatios(100,100,HMCDs1pcosthCM,"");
    Canvas.Print(filename);
    
    Canvas.Clear();
    TH2F* H2Ds1pCMPvsThetaEff=(TH2F*)H2Ds1pCMPvsTheta->Clone();
    Int_t bincontent=0; 
    for(Int_t binx=0;binx<=H2MCDs1pCMPvsTheta->GetNbinsX();binx++)
      for(Int_t biny=0;biny<=H2MCDs1pCMPvsTheta->GetNbinsY();biny++){
	bincontent=(int)H2MCDs1pCMPvsTheta->GetBinContent(binx,biny);
	if(bincontent>100){	  	  
	  H2Ds1pCMPvsThetaEff->SetBinContent(binx,biny,H2Ds1pCMPvsThetaEff->GetBinContent(binx,biny)/bincontent);
	}else H2Ds1pCMPvsThetaEff->SetBinContent(binx,biny,0);
      }
    H2Ds1pCMPvsThetaEff->Draw("colz");
    Canvas.Print(filename);
        
  }
  
  


  if(TruthMatch){
    //number of generated cands
    Canvas.Clear();
    HMCNDs1p->Draw();
    Canvas.SetLogy(1);     
    Canvas.Print(filename);
    Canvas.SetLogy(0);
  }

  //number of reconstructed cands
  Canvas.Clear(); 
  HRecoNDs1p.Draw();
  Canvas.SetLogy(1);    
  Canvas.Print(filename);
  Canvas.SetLogy(0);

  //check on the Idx 
  Canvas.Clear(); 
  HLundCheck->SetBarOffset(0);
  HLundCheck->SetBarWidth(.05);
  HLundCheck->Draw();
  Canvas.Print(filename);
  


  PrintModePlots(&Canvas,filename);


  Canvas.Print(filename+"]");


  //delete pointers
  delete  HMCDs1pMass;
  delete  HMCDs1pp3CM;
  delete  HMCDs1pcosthCM;     
  delete  H2MCDs1pCMPvsTheta;
  delete  HMCNDs1p;     
  delete  HLundCheck;

  delete H2DstarDeltaMVsD0Mass;
 
  if(!Close()){
    cout<<"Failed to close file"<<endl;
    return 0;
  }
  


  return 1;
}

Int_t Ds1Analysis::PrintModePlots(TCanvas* Canvas,TString filename){
 
  if(_Mode=="D0ToK3Pi"){
    cout<<Canvas<<" "<<filename<<endl;

  }

  return 1; 
}
