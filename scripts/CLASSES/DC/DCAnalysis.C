#include "DCAnalysis.h"
#include "RooHist.h"

ClassImp(DCAnalysis);
DCAnalysis::DCAnalysis(Int_t MCorDATA,TString OutputDir):
  AbsAnalysis(MCorDATA,OutputDir),
  LowMassHist(1.77),
  HighMassHist(1.97),
  MassHistBins(200),
  MeanMass(1.8693),
  LowMassCut(MeanMass-.02),
  HighMassCut(MeanMass+.02),
  LowSideLowMassCut(MeanMass-.02*3),
  LowSideHighMassCut(MeanMass-.02*2),
  HighSideLowMassCut(MeanMass+.02*2),
  HighSideHighMassCut(MeanMass+.02*3),
  DFlightCut(0),
  mass("mass","mass",2,3)  
{   
  
  cout<<"Done Initializing the analysis"<<endl;
}

DCAnalysis::~DCAnalysis(){  
}

Int_t DCAnalysis::ReduceNtuple(){
  cout<<"Going to reduce the ntuple"<<endl;

  if(_fChain==NULL){
    cout<<"No chain."<<endl;
    return 0;
  } 

  ///link the branches in the input chain
  SetDCBranches();

  //the file must be created here so the 'new' histos and Tree is created in hard drive //OutFile=new 
  TFile OutFile(_OutputDir+"/Reduced_New.root","recreate",_OutputDir+"/Reduced_New.root",1);//1=minimal compression
  if(OutFile.IsZombie()){cout<<"Failed to create file"<<endl;return 0;}


  //create new reduced ntuple
  TTree Ntuple("Ntuple","Ntuple",0);
  Long64_t maxsize=1500000000;
  cout<<" file size "<<maxsize<<endl;
  Ntuple.SetMaxTreeSize(maxsize);//split files in 1.5Gb

  //create the branches in the Ntuple
  MakeDCReducedNtupleBranches(&Ntuple);

  cout<<"Total Entries in Chain="<<_fChain->GetEntries()<<endl;
  cout<<"Going to start the event loop"<<endl;
  eventid=0;
  while(_fChain->GetEntry(eventid,0)>0){   
    eventid++;
     
    if(eventid%50000==0)cout<<eventid<<" Events done."<<endl;
      
    if(nD>MAXNCANDS){
      cout<<"Too many cands at event "<<eventid<<" Only first "<<MAXNCANDS<<" of "<<nD<<" will be used"<<endl;
      nD=MAXNCANDS;      
    }


    ///Event vars
    FillEventVars();
   

    //now do real loop
    for(DIdx=0;DIdx<nD;DIdx++){

      ///determine the indexes for each particle must be done at beginnig of every loop
      KIdx=Dd1Idx[DIdx];
      PiIdx=Dd2Idx[DIdx];
      DPi2Idx=Dd3Idx[DIdx];
      
//       PitrkIdx=PiTrkIdx[PiIdx];
//       KtrkIdx=KTrkIdx[KIdx];    
//       DPi2trkIdx=PiTrkIdx[DPi2Idx];      
      

      //should not fill any vars outside this method otherwise not saved into DCPi
      FillDCVars();

      //save
      Ntuple.Fill();	              
   
    }//D loop    
    
    
  }
  
  //print summary
  cout<<"--------Summary-------"<<endl;
  cout<<"Total events = "<<eventid<<endl;


  ////Save the Ntuple and histograms
  cout<<"Going to save the ntuple and histograms"<<endl; 


  if(Ntuple.AutoSave("Overwrite")){ cout<<"Ntuple written."<<endl;}
  else{ cout<<"Failed to write ntuple"<<endl;return 0;}

  cout<<"Objects in the current file"<<endl;
  Ntuple.GetCurrentFile()->ls();

  OutFile.Close(); //this will delete all objects created inside the file if they were not written  
  cout<<OutFile.GetName()<<" has been closed."<<endl;


  return 1;
}

void DCAnalysis::FillDCVars(){
          
  //-------D quantities
  dmass=DMass[DIdx];
  dp3=Dp3[DIdx];
  dpstar=Dp3CM[DIdx];
  dcharge=DLund[DIdx]/abs(DLund[DIdx]);
  if(TMath::Prob(DChi2[DIdx],DnDof[DIdx])>1e-300)
    dlogvtxprob=log10(TMath::Prob(DChi2[DIdx],DnDof[DIdx]));
  else dlogvtxprob=-100;
  dflight=DFlightBSLen[DIdx];
  dflights=DFlightBSLen[DIdx]/DFlightBSErr[DIdx];

     
  //Kaon
  kp3=Kp3[KIdx];
  kcharge=KLund[KIdx]/abs(KLund[KIdx]);   
  kcostheta=ComputeKCosTheta();

  //Pion
  pip3=Pip3[PiIdx];
  picharge=PiLund[PiIdx]/abs(PiLund[PiIdx]);

  //other D pions
  dpi2p3=Pip3[DPi2Idx];      
  dpi2charge=PiLund[DPi2Idx]/abs(PiLund[DPi2Idx]);      

  //2-Body masses
  Kpimass=ComputexyMass(Kp3CM[KIdx],KcosthCM[KIdx],KphiCM[KIdx],KPDGMass,
			Pip3CM[PiIdx],PicosthCM[PiIdx],PiphiCM[PiIdx],PiPDGMass);
  Kpi2mass=ComputexyMass(Kp3CM[KIdx],KcosthCM[KIdx],KphiCM[KIdx],KPDGMass,
			 Pip3CM[DPi2Idx],PicosthCM[DPi2Idx],PiphiCM[DPi2Idx],PiPDGMass);

  pipi2mass=ComputexyMass(Pip3CM[PiIdx],PicosthCM[PiIdx],PiphiCM[PiIdx],PiPDGMass,
			  Pip3CM[DPi2Idx],PicosthCM[DPi2Idx],PiphiCM[DPi2Idx],PiPDGMass);


  // if(1.85<dmass&&dmass<1.89)//check convergence of D+ vertex
  //if(Kpimass>2&&Kpi2mass>2){
//   cout<<"DMass="<<dmass<<" Kpipi="<<ComputexyzMass(Kp3CM[KIdx],KcosthCM[KIdx],KphiCM[KIdx],KPDGMass,
// 						   Pip3CM[PiIdx],PicosthCM[PiIdx],PiphiCM[PiIdx],PiPDGMass,
// 						   Pip3CM[DPi2Idx],PicosthCM[DPi2Idx],PiphiCM[DPi2Idx],PiPDGMass)<<endl;
  //cout<<Kp3CM[KIdx]<<" "<<KcosthCM[KIdx]<<" "<<KphiCM[KIdx]<<" "<<KPDGMass<<" "<<Pip3CM[PiIdx]<<" "<<PicosthCM[PiIdx]<<" "<<PiphiCM[PiIdx]<<" "<<PiPDGMass<<" "<<Pip3CM[DPi2Idx]<<" "<<PicosthCM[DPi2Idx]<<" "<<PiphiCM[DPi2Idx]<<" "<<PiPDGMass<<endl;
  
  //}
  

//   //---------Selectors
//   klh=0;
//   for(Int_t i=10;i<=14;i++){//10=notpion,11=veryloose,12=loose,13=tight,14=verytight
//     if((KSelectorsMap[KtrkIdx] & (1<<i)) == (1<<i))
//       klh=i;
//   }
      
//   pilh=0;dpi2lh=0;
//   for(Int_t i=2;i<=5;i++){//2=veryloose,3=loose,4=tight,5=verytight
//     if((PiSelectorsMap[PitrkIdx] & (1<<i) ) == (1<<i))
//       pilh=i;
//     if((PiSelectorsMap[DPi2trkIdx] & (1<<i) ) == (1<<i))
//       dpi2lh=i;
	
//   }
      
}





void DCAnalysis::ShowDMassRegions(TH1* Histo){
  cutline.DrawLine(LowMassCut,0,LowMassCut,Histo->GetMaximum());
  cutline.DrawLine(HighMassCut,0,HighMassCut,Histo->GetMaximum());

  cutline.DrawLine(LowSideLowMassCut,0,LowSideLowMassCut,1.4*Histo->GetBinContent(1));//.2*Histo->GetMaximum());
  cutline.DrawLine(LowSideHighMassCut,0,LowSideHighMassCut,1.4*Histo->GetBinContent(1));//.2*Histo->GetMaximum());
  
  cutline.DrawLine(HighSideLowMassCut,0,HighSideLowMassCut,1.4*Histo->GetBinContent(1));//.2*Histo->GetMaximum());
  cutline.DrawLine(HighSideHighMassCut,0,HighSideHighMassCut,1.4*Histo->GetBinContent(1));//.2*Histo->GetMaximum());
  
}


Float_t DCAnalysis::ComputeKCosTheta(){

  //Angle of the Extra pion wrt the X direction in the X frame
  TVector3 Dp3vec(Dp3CM[DIdx]*sin(acos(DcosthCM[DIdx]))*cos(DphiCM[DIdx]),
		  Dp3CM[DIdx]*sin(acos(DcosthCM[DIdx]))*sin(DphiCM[DIdx]),
		  Dp3CM[DIdx]*DcosthCM[DIdx]);
                

  Float_t DEnergy=sqrt(Dp3vec*Dp3vec + DMass[DIdx]*DMass[DIdx]);
  TVector3 DVelvec=(1/DEnergy)*Dp3vec;
                                                              

  //boost the K into the D rest frame
  TVector3 Kp3vec(Kp3CM[KIdx]*sin(acos(KcosthCM[KIdx]))*cos(KphiCM[KIdx]),
                   Kp3CM[KIdx]*sin(acos(KcosthCM[KIdx]))*sin(KphiCM[KIdx]),
                   Kp3CM[KIdx]*KcosthCM[KIdx]);
  
  TLorentzVector Kp4(Kp3vec,sqrt(Kp3vec*Kp3vec+KPDGMass*KPDGMass));
  Kp4.Boost(-DVelvec);//watch the minus sign
  TVector3 KBoostedp3vec=Kp4.Vect();
  
  //
  return KBoostedp3vec.Unit()*Dp3vec.Unit(); 
}


Int_t DCAnalysis::SetDCBranches(){
  if(_fChain==NULL){cout<<"No Chain"<<endl;return 0;} 
  SetEventBranches();


  _fChain->SetBranchAddress("nD",&nD);
  _fChain->SetBranchAddress("DMass",DMass);
  _fChain->SetBranchAddress("Dp3CM",Dp3CM); 
  _fChain->SetBranchAddress("DcosthCM",DcosthCM); 
  _fChain->SetBranchAddress("DphiCM",DphiCM);  
  _fChain->SetBranchAddress("Dp3",Dp3); 
  _fChain->SetBranchAddress("Dcosth",Dcosth); 
  _fChain->SetBranchAddress("Dphi",Dphi);  
  _fChain->SetBranchAddress("DLund",DLund);
  _fChain->SetBranchAddress("Dd1Lund",Dd1Lund);
  _fChain->SetBranchAddress("Dd1Idx",Dd1Idx);
  _fChain->SetBranchAddress("Dd2Lund",Dd2Lund);
  _fChain->SetBranchAddress("Dd2Idx",Dd2Idx);
  _fChain->SetBranchAddress("Dd3Lund",Dd3Lund);
  _fChain->SetBranchAddress("Dd3Idx",Dd3Idx);
//   _fChain->SetBranchAddress("DChi2",DChi2);
//   _fChain->SetBranchAddress("DnDof",DnDof);
//  _fChain->SetBranchAddress("DVtxStatus",DVtxStatus);
  _fChain->SetBranchAddress("DVtxx",DVtxx);
  _fChain->SetBranchAddress("DVtxy",DVtxy);
  _fChain->SetBranchAddress("DVtxz",DVtxz);
  _fChain->SetBranchAddress("DFlightBSLen",DFlightBSLen);
  _fChain->SetBranchAddress("DFlightBSErr",DFlightBSErr);
  
  _fChain->SetBranchAddress("nPi",&nPi);
  _fChain->SetBranchAddress("Pip3",Pip3);
  _fChain->SetBranchAddress("Picosth",Picosth);
  _fChain->SetBranchAddress("Piphi",Piphi);
  _fChain->SetBranchAddress("Pip3CM",Pip3CM);
  _fChain->SetBranchAddress("PicosthCM",PicosthCM);
  _fChain->SetBranchAddress("PiphiCM",PiphiCM);
  _fChain->SetBranchAddress("PiLund",PiLund);
  //  _fChain->SetBranchAddress("PiTrkIdx",PiTrkIdx);

  _fChain->SetBranchAddress("nK",&nK);
  _fChain->SetBranchAddress("Kp3",Kp3);
  _fChain->SetBranchAddress("Kp3CM",Kp3CM);
  _fChain->SetBranchAddress("KcosthCM",KcosthCM);
  _fChain->SetBranchAddress("KphiCM",KphiCM);
  _fChain->SetBranchAddress("KLund",KLund);
  //_fChain->SetBranchAddress("KTrkIdx",KTrkIdx);


//   _fChain->SetBranchAddress("TRKnSvt",TRKnSvt);
//   _fChain->SetBranchAddress("TRKLund",TRKLund);
//   _fChain->SetBranchAddress("piSelectorsMap",PiSelectorsMap);
//   _fChain->SetBranchAddress("KSelectorsMap",KSelectorsMap);
//   _fChain->SetBranchAddress("TRKdedxdch",TRKdedxdch);
//   _fChain->SetBranchAddress("TRKDrcTh",TRKDrcTh);

   
  cout<<"Done linking DC Chain Branches"<<endl;
  return 1;
}



Int_t DCAnalysis::MakeDCReducedNtupleBranches(TTree* Ntuple){

  Ntuple->Branch("dmass",&dmass,"dmass/F"); 
  Ntuple->Branch("dp3",&dp3,"dp3/F");
  Ntuple->Branch("dpstar",&dpstar,"dpstar/F");
  Ntuple->Branch("dcharge",&dcharge,"dcharge/I");
  Ntuple->Branch("dlogvtxprob",&dlogvtxprob,"dlogvtxprob/F");
  Ntuple->Branch("dflights",&dflights,"dflighs/F");
  Ntuple->Branch("dflight",&dflight,"dfligh/F");
  Ntuple->Branch("dmctrue",&dmctrue,"dmctrue/I");

  Ntuple->Branch("kmctrue",&kmctrue,"kmctrue/I");
  Ntuple->Branch("kp3",&kp3,"kp3/F");
  Ntuple->Branch("kcostheta",&kcostheta,"kcostheta/F"); 
  Ntuple->Branch("kcharge",&kcharge,"kcharge/I");
  Ntuple->Branch("klh",&klh,"klh/I");

  Ntuple->Branch("pimctrue",&pimctrue,"pimctrue/I");
  Ntuple->Branch("pip3",&pip3,"pip3/F");
  Ntuple->Branch("picharge",&picharge,"picharge/I");
  Ntuple->Branch("pilh",&pilh,"pilh/I");

  Ntuple->Branch("dpi2p3",&dpi2p3,"dpi2p3/F");
  Ntuple->Branch("dpi2mctrue",&dpi2mctrue,"dpi2mctrue/I");
  Ntuple->Branch("dpi2charge",&dpi2charge,"dpi2charge/I");
  Ntuple->Branch("dpi2lh",&dpi2lh,"dpi2lh/I");

  Ntuple->Branch("Kpimass",&Kpimass,"Kpimass/F");
  Ntuple->Branch("Kpi2mass",&Kpi2mass,"Kpi2mass/F");
  Ntuple->Branch("pipi2mass",&pipi2mass,"pipi2mass/F");


  Ntuple->Branch("KpimassFold",&KpimassFold,"KpimassFold/F");
  Ntuple->Branch("Kpi2massFold",&Kpi2massFold,"Kpi2massFold/F");
  

  MakeEventReducedNtupleBranches(Ntuple);
  cout<<"Done Making DC Reduced Ntuple Branches"<<endl;
  return 1;
}



Int_t DCAnalysis::SetDCReducedNtupleBranches(){

  ReducedNtuple->SetBranchAddress("dmass",&dmass);
  ReducedNtuple->SetBranchAddress("dp3",&dp3);
  ReducedNtuple->SetBranchAddress("dpstar",&dpstar);
  ReducedNtuple->SetBranchAddress("dcharge",&dcharge);
  ReducedNtuple->SetBranchAddress("dlogvtxprob",&dlogvtxprob);
  ReducedNtuple->SetBranchAddress("dflights",&dflights);
  ReducedNtuple->SetBranchAddress("dflight",&dflight);

  ReducedNtuple->SetBranchAddress("kcharge",&kcharge);
  ReducedNtuple->SetBranchAddress("kp3",&kp3);
  ReducedNtuple->SetBranchAddress("kcostheta",&kcostheta);
  ReducedNtuple->SetBranchAddress("kmctrue",&kmctrue);
  ReducedNtuple->SetBranchAddress("klh",&klh);

  ReducedNtuple->SetBranchAddress("picharge",&picharge);
  ReducedNtuple->SetBranchAddress("pip3",&pip3);
  ReducedNtuple->SetBranchAddress("pimctrue",&pimctrue);
  ReducedNtuple->SetBranchAddress("pilh",&pilh);
 
  ReducedNtuple->SetBranchAddress("dpi2p3",&dpi2p3);
  ReducedNtuple->SetBranchAddress("dpi2charge",&dpi2charge);
  ReducedNtuple->SetBranchAddress("dpi2mctrue",&dpi2mctrue);
  ReducedNtuple->SetBranchAddress("dpi2lh",&dpi2lh);

  
  ReducedNtuple->SetBranchAddress("Kpimass",&Kpimass);
  ReducedNtuple->SetBranchAddress("Kpi2mass",&Kpi2mass);
  ReducedNtuple->SetBranchAddress("pipi2mass",&pipi2mass);

  ReducedNtuple->SetBranchAddress("KpimassFold",&KpimassFold);
  ReducedNtuple->SetBranchAddress("Kpi2massFold",&Kpi2massFold);


  

  SetEventReducedNtupleBranches();
  cout<<"Done linking DC ReducedNtuple Branches"<<endl;
  return 1;
}


Int_t DCAnalysis::MakeDMass(){

  if(!OpenReducedNtuple())return 0;

  TFile file(_OutputDir+"/DMassPlot.root","recreate");

  TH1F HDMass("HDMass","",MassHistBins,LowMassHist,HighMassHist);
  ReducedNtuple->Draw("dmass>>HDMass");
  HDMass.Write();

  file.ls();
  file.Close();

  CloseReducedNtuple();

  return 1;
}

Int_t DCAnalysis::PlotDMass(){
  
  filename=_OutputDir+"/DMassPlots.ps";
  Canvas.Print(filename+"["); 

  TH1F* HDMass=0;
  TFile file(_OutputDir+"/DMassPlot.root","read");
  HDMass=(TH1F*)file.Get("HDMass");
  HDMass->GetXaxis()->SetTitle("m(K^{-}#pi^{+}#pi^{+})   GeV/c^{2}");
  HDMass->GetYaxis()->SetTitle("Entries / 1 MeV/c^{2}");

  //simple
  Canvas.Clear();
  HDMass->SetStats(0);
  HDMass->GetYaxis()->SetRangeUser(0,HDMass->GetMaximum()*1.1);
  HDMass->Draw("pe");
  Canvas.Print(filename);
  ShowDMassRegions(HDMass);
  Canvas.Print(filename);

  //Fit
  Canvas.Clear();
  TF1 FGausPol("FGausPol",Form("[0]*exp(-.5*([1]-x)**2/[2]**2) + [3] + [4]*(x-%.5f)",LowMassHist),LowMassHist,HighMassHist);  
  FGausPol.SetParNames("Norm","#mu","#sigma");
  FGausPol.SetParLimits(0,.1*HDMass->GetMaximum(),HDMass->GetMaximum());
  FGausPol.SetParLimits(1,HDMass->GetMean()-HDMass->GetRMS(),HDMass->GetMean()+HDMass->GetRMS());
  FGausPol.SetParLimits(2,.001,HDMass->GetRMS());
  FGausPol.SetParLimits(3,0,HDMass->GetMaximum());FGausPol.SetParameter(3,HDMass->GetBinContent(1));
  FGausPol.SetParLimits(4,-HDMass->GetMaximum()/HDMass->GetRMS(),HDMass->GetMaximum()/HDMass->GetRMS());
  FGausPol.SetParameter(0,MeanMass);
  FGausPol.SetParameter(1,.01);
  FGausPol.SetParameter(4,(HDMass->GetBinContent(HDMass->GetXaxis()->GetNbins())-HDMass->GetBinContent(1))/(HighMassHist-LowMassHist));
  delete HDMass->GetListOfFunctions()->FindObject("stats");
  HDMass->Fit(&FGausPol,"LL","pe",LowMassHist,HighMassHist);
  HDMass->Draw("pe");
  Canvas.Print(filename);

  ///Compute the purity
  TF1 FSig("FSig",Form("[0]*exp(-.5*([1]-x)**2/[2]**2)",LowMassHist),LowMassHist,HighMassHist);
  FSig.SetParameter(0,FGausPol.GetParameter(0));
  FSig.SetParameter(1,FGausPol.GetParameter(1));
  FSig.SetParameter(2,FGausPol.GetParameter(2));  
//   TF1 FBkg("FBkg",Form("[0] + [1]*(x-%.5f)",LowMassHist),LowMassHist,HighMassHist); 
//   FBkg.SetParameter(0,FGausPol.GetParameter(3));
//   FBkg.SetParameter(1,FGausPol.GetParameter(4));

  cout<<"The purity is = "<<FSig.Integral(LowMassCut,HighMassCut)/FGausPol.Integral(LowMassCut,HighMassCut)<<endl;


  //regions
  Canvas.Clear();
  HDMass->Draw("pe");
  ShowDMassRegions(HDMass);
  Canvas.Print(filename);


  Canvas.Print(filename+"]");
  return 1;
}


Int_t DCAnalysis::MakeDFlight(){

  
  if(!OpenReducedNtuple())return 0;

  TFile file(_OutputDir+"/DFlightPlot.root","recreate");

  TH1F HDFlight("HDFlight","",250,-5,20);
  ReducedNtuple->Draw("dflights>>HDFlight",Form("%.5f<dmass&&dmass<%.5f",LowMassCut,HighMassCut));
  HDFlight.Write();

  cout<<"The side-bands need to be in the Ntuple"<<endl;
  TH1F HDFlightSB("HDFlightSB","",250,-5,20);
  ReducedNtuple->Draw("dflights>>HDFlightSB",Form("(%.5f<dmass&&dmass<%.5f)||(%.5f<dmass&&dmass<%.5f)",LowSideLowMassCut,LowSideHighMassCut,HighSideLowMassCut,HighSideHighMassCut));
  HDFlightSB.Write();

  file.ls();
  file.Close();

  CloseReducedNtuple();

  return 1;
}

Int_t DCAnalysis::PlotDFlight(){
  
  filename=_OutputDir+"/DFlightPlots.ps";
  Canvas.Print(filename+"["); 

  TFile file(_OutputDir+"/DFlightPlot.root","read");

  TH1F* HDFlight=(TH1F*)file.Get("HDFlight");
  HDFlight->GetXaxis()->SetTitle("flight significance");
  HDFlight->GetYaxis()->SetTitle("Entries/.1");

  
  TH1F* HDFlightSB=(TH1F*)file.Get("HDFlightSB");
  HDFlightSB->GetXaxis()->SetTitle("flight significance");
  HDFlightSB->GetYaxis()->SetTitle("Entries/.1");

  //simple total 
  Canvas.Clear();
  HDFlight->GetYaxis()->SetRangeUser(0,HDFlight->GetMaximum()*1.1);
  HDFlight->Draw();
  HDFlightSB->Draw("same");
  Canvas.Print(filename);

  //Compare Signal and SB
  HDFlight->Add(HDFlightSB,-1);
  Canvas.Clear();
  delete  HDFlightSB->GetListOfFunctions()->FindObject("stats");
  delete  HDFlight->GetListOfFunctions()->FindObject("stats");
  HDFlightSB->Sumw2();
  HDFlightSB->Scale(1./HDFlightSB->Integral());
  HDFlightSB->GetYaxis()->SetTitle("");
  //HDFlightSB->GetYaxis()->SetRangeUser(0,HDFlightSB->GetMaximum()*1.1);
  HDFlightSB->Draw("hist");
  HDFlight->Sumw2();
  HDFlight->Scale(1./HDFlight->Integral());
  HDFlight->Draw("pe same");
  cutline.DrawLine(DFlightCut,0,DFlightCut,HDFlightSB->GetMaximum());
  Canvas.Print(filename);


  Canvas.Print(filename+"]");
  return 1;
}



Int_t DCAnalysis::MakeDalitzPlot(){

  TFile file(_OutputDir+"/DalitzPlot.root","recreate");

  //sprintf(textlabel,"Kpimass*Kpimass:Kpi2mass*Kpi2mass");
  sprintf(textlabel,"KpimassFold*KpimassFold:Kpi2massFold*Kpi2massFold");
  

  if(OpenReducedNtuple()!=1) return 0;
  gROOT->cd();

  //mass histo to go with this pdf plot
  TH1F HDMass("HDMass","",200,1.77,1.97);
  ReducedNtuple->Draw("dmass>>HDMass");
  file.cd();
  HDMass.Write();



  TH2F H2DMass("H2DMass","",3000,.25,3.25,3000,.25,3.25); 
  ReducedNtuple->Draw(TString(textlabel)+">>H2DMass",Form("%.5f<dmass&&dmass<%.5f",LowMassCut,HighMassCut));
  file.cd();
  H2DMass.Write(); 
  if(CloseReducedNtuple()!=1) return 0;
   

  if(OpenReducedNtuple()!=1) return 0;
  gROOT->cd();
  TH2F H2DMassSB("H2DMassSB","",3000,.25,3.25,3000,.25,3.25);
  ReducedNtuple->Draw(TString(textlabel)+">>H2DMassSB",Form("(%.5f<dmass&&dmass<%.5f)||(%.5f<dmass&&dmass<%.5f)",LowSideLowMassCut,LowSideHighMassCut,HighSideLowMassCut,HighSideHighMassCut));
  file.cd();
  H2DMassSB.Write(); 


  if(CloseReducedNtuple()!=1) return 0;
  
  file.Close();

  return 1;
}

Int_t DCAnalysis::PlotDalitzPlot(){
 
  TFile file(_OutputDir+"/DalitzPlot.root","read");
  gROOT->cd();
  

  filename=_OutputDir+"/DalitzPlot.ps";
  Canvas.Print(filename+"["); 


  //get the bkg under the signal
  TH1F* HDMass=(TH1F*)file.Get("HDMass");
  MassFitter massfit(&mass);
  if(!massfit.Initialize(HDMass,_OutputDir+"/InitRooFitParsDMass.txt"))return 0;
  if(massfit.Fit()!=0)return 0;
  RooAbsPdf*bkg=massfit.GetBkgPdf();
  mass.setRange("sigregion",LowMassCut,HighMassCut);
  RooAbsReal* integral= bkg->createIntegral(mass,"sigregion");
  cout<<",  undersig="<<integral->getVal()<<endl;
  Canvas.Clear();
  RooPlot*fitplot=massfit.GetFitPlot();
  fitplot->Draw();
  ShowDMassRegions(HDMass);
  Canvas.Print(filename);
  ///


  TH2F*H2DMass=(TH2F*)file.Get("H2DMass");
  H2DMass=(TH2F*)H2DMass->RebinX(20,"rX");
  H2DMass=(TH2F*)H2DMass->RebinY(20,"rY");
  Canvas.Clear();
  H2DMass->Draw("colz");
  Canvas.Print(filename);
 

  TH2F*HistoSB=(TH2F*)file.Get("H2DMassSB");
  HistoSB=(TH2F*)HistoSB->RebinX(20,"rsX");
  HistoSB=(TH2F*)HistoSB->RebinY(20,"rsY");
  Canvas.Clear();
  HistoSB->Draw("colz");
  Canvas.Print(filename);



  Canvas.Print(filename+"]");

  return 1;
}

