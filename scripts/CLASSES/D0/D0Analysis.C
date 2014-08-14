#include "D0Analysis.h"

ClassImp(D0Analysis);
D0Analysis::D0Analysis(Bool_t MCorDATA,TString OutputDir):
  AbsAnalysis(MCorDATA,OutputDir),
  DMassCutCenter(D0PDGMass),
  DMassCutWindow(.02),
  DMassCutSBMin(.05),
  mass("mass","mass",0,6),
  massfitter(&mass),
  DNtupleTag("D")
{   
  cout<<"Done Initializing the analysis"<<endl;
}

D0Analysis::~D0Analysis(){  
}

Int_t D0Analysis::ReduceNtuple(){
  cout<<"Going to reduce the ntuple"<<endl;

  if(_fChain==NULL){
    cout<<"No chain."<<endl;
    return 0;
  } 

  ///link the branches in the input chain
  SetD0Branches();

  //the file must be created here so the 'new' histos and Tree is created in hard drive //OutFile=new 
  TFile OutFile(_OutputDir+"/Reduced_New.root","recreate",_OutputDir+"/Reduced_New.root",1);//1=minimal compression
  if(OutFile.IsZombie()){cout<<"Failed to create file"<<endl;return 0;}

  
  //create new reduced ntuple
  TTree Ntuple("Ntuple","Ntuple",0);
  Ntuple.SetMaxTreeSize(900000000);//split files

  //disable autosaving 
  Ntuple.SetAutoSave(900000000);


  //create the branches in the Ntuple
  MakeD0ReducedNtupleBranches(&Ntuple);


  cout<<"Total Entries in Chain="<<_fChain->GetEntries()<<endl;
  cout<<"Going to start the event loop"<<endl;
  eventid=0;
  while(_fChain->GetEntry(eventid,0)>0){   
    eventid++;
     
    if(eventid%50000==0)cout<<eventid<<" Events done."<<endl;
     

    ///Event vars
    FillEventVars();
   

    //now do real loop
    for(DIdx=0;DIdx<nD;DIdx++){

      //should not fill any vars outside this method otherwise not saved into D0Pi
      FillD0Vars();

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

void D0Analysis::FillD0Vars(){
  
  if(DIdx>=MAXNCANDS)return;

  ///Fill all necesary indexes used in this method
  KIdx=Dd1Idx[DIdx];
  PiIdx=Dd2Idx[DIdx];   
  DPi2Idx=Dd3Idx[DIdx];
  DPi3Idx=Dd4Idx[DIdx];
  
  if(KIdx>=MAXNCANDS||PiIdx>=MAXNCANDS||DPi2Idx>=MAXNCANDS||DPi3Idx>=MAXNCANDS)return;
  
  //       PitrkIdx=PiTrkIdx[PiIdx];
  //       KtrkIdx=KTrkIdx[KIdx];    
  //       DPi2trkIdx=PiTrkIdx[DPi2Idx];      
     
  //-------D quantities
  dmass=DMass[DIdx];
  dp3=Dp3[DIdx];
  dpstar=Dp3CM[DIdx];
  dcharge=DLund[DIdx]/abs(DLund[DIdx]);
  if(TMath::Prob(DChi2[DIdx],DnDof[DIdx])>1e-300)
    dlogvtxprob=log10(TMath::Prob(DChi2[DIdx],DnDof[DIdx]));
  else dlogvtxprob=-100;
  dflightbs=DFlightBSLen[DIdx];
  dmode=1;//Kpi
  if(DPi2Idx>=0&&DPi3Idx>=0)dmode=2;//Kpipipi
  
  //Kaon
  kp3=Kp3[KIdx];
  kcharge=KLund[KIdx]/abs(KLund[KIdx]);   
 
  //Pion
  pip3=Pip3[PiIdx];
  picharge=PiLund[PiIdx]/abs(PiLund[PiIdx]);
  

  //Truth-match the D0
  dmctrue=0;
  if(_MCorDATA==0){
    Int_t TrueCandIdx[4]={KMCIdx[KIdx],
			  PiMCIdx[PiIdx],
			  PiMCIdx[DPi2Idx],
			  PiMCIdx[DPi3Idx]
                          };
    for(Int_t m=1;m<mcLen;m++)
      if(abs(mcLund[m])==myD0Lund){		
	Int_t nfound=0;
	for(Int_t c=0;c<(dmode==1?2:4);c++){
	  Int_t ParentIdx=mothIdx[TrueCandIdx[c]];
	  while(ParentIdx>2){
	    if(ParentIdx==m)nfound++;
	    ParentIdx=mothIdx[ParentIdx];
	  }
	}
	if(nfound==(dmode==1?2:4)){
	  dmctrue=1;
	}
      }

  } 

  

}



Int_t D0Analysis::SetD0Branches(){
  if(_fChain==NULL){cout<<"No Chain"<<endl;return 0;} 
  
  SetEventBranches();

  _fChain->SetBranchAddress(TString("n")+DNtupleTag,&nD);
  _fChain->SetBranchAddress(DNtupleTag+"Mass",DMass);
  _fChain->SetBranchAddress(DNtupleTag+"p3CM",Dp3CM); 
  _fChain->SetBranchAddress(DNtupleTag+"costhCM",DcosthCM); 
  _fChain->SetBranchAddress(DNtupleTag+"phiCM",DphiCM);  
  _fChain->SetBranchAddress(DNtupleTag+"p3",Dp3); 
  _fChain->SetBranchAddress(DNtupleTag+"costh",Dcosth); 
  _fChain->SetBranchAddress(DNtupleTag+"phi",Dphi);  
  _fChain->SetBranchAddress(DNtupleTag+"Lund",DLund);
  _fChain->SetBranchAddress(DNtupleTag+"d1Lund",Dd1Lund);
  _fChain->SetBranchAddress(DNtupleTag+"d1Idx",Dd1Idx);
  _fChain->SetBranchAddress(DNtupleTag+"d2Lund",Dd2Lund);
  _fChain->SetBranchAddress(DNtupleTag+"d2Idx",Dd2Idx);
  _fChain->SetBranchAddress(DNtupleTag+"d3Lund",Dd3Lund);
  _fChain->SetBranchAddress(DNtupleTag+"d3Idx",Dd3Idx);
  _fChain->SetBranchAddress(DNtupleTag+"d4Lund",Dd4Lund);
  _fChain->SetBranchAddress(DNtupleTag+"d4Idx",Dd4Idx);
//   _fChain->SetBranchAddress(DNtupleTag+"Chi2",DChi2);
//   _fChain->SetBranchAddress(DNtupleTag+"nDof",DnDof);
//  _fChain->SetBranchAddress(DNtupleTag+"VtxStatus",DVtxStatus);
  _fChain->SetBranchAddress(DNtupleTag+"Vtxx",DVtxx);
  _fChain->SetBranchAddress(DNtupleTag+"Vtxy",DVtxy);
  _fChain->SetBranchAddress(DNtupleTag+"Vtxz",DVtxz);
  //_fChain->SetBranchAddress(DNtupleTag+"FlightBSLen",DFlightBSLen);
  
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
  _fChain->SetBranchAddress("Kcosth",Kcosth);
  _fChain->SetBranchAddress("Kphi",Kphi);
  _fChain->SetBranchAddress("Kp3CM",Kp3CM);
  _fChain->SetBranchAddress("KcosthCM",KcosthCM);
  _fChain->SetBranchAddress("KphiCM",KphiCM);
  _fChain->SetBranchAddress("KLund",KLund);
  //_fChain->SetBranchAddress("KTrkIdx",KTrkIdx);


  if(_MCorDATA==0){
    _fChain->SetBranchAddress("KMCIdx",KMCIdx);
    _fChain->SetBranchAddress("PiMCIdx",PiMCIdx);
  }


//   _fChain->SetBranchAddress("TRKnSvt",TRKnSvt);
//   _fChain->SetBranchAddress("TRKLund",TRKLund);
//   _fChain->SetBranchAddress("piSelectorsMap",PiSelectorsMap);
//   _fChain->SetBranchAddress("KSelectorsMap",KSelectorsMap);
//   _fChain->SetBranchAddress("TRKdedxdch",TRKdedxdch);
//   _fChain->SetBranchAddress("TRKDrcTh",TRKDrcTh);

  
 
  cout<<"Done linking D0 Chain Branches"<<endl;
  return 1;
}



Int_t D0Analysis::MakeD0ReducedNtupleBranches(TTree* Ntuple){
  if(!Ntuple){cout<<"D0Analysis::MakeD0ReducedNtupleBranches : No Ntuple"<<endl;return 0;}

  MakeEventReducedNtupleBranches(Ntuple);
 
  Ntuple->Branch("dmass",&dmass,"dmass/F"); 
  Ntuple->Branch("dp3",&dp3,"dp3/F");
  Ntuple->Branch("dpstar",&dpstar,"dpstar/F");
  Ntuple->Branch("dcharge",&dcharge,"dcharge/I");
  Ntuple->Branch("dlogvtxprob",&dlogvtxprob,"dlogvtxprob/F");
  Ntuple->Branch("dflightbs",&dflightbs,"dflightbs/F");
  Ntuple->Branch("dmode",&dmode,"dmode/I");
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
  

  
  cout<<"Done Making D0 Reduced Ntuple Branches"<<endl;
  return 1;
}



Int_t D0Analysis::SetD0ReducedNtupleBranches(){

  ReducedNtuple->SetBranchAddress("dmass",&dmass);
  ReducedNtuple->SetBranchAddress("dp3",&dp3);
  ReducedNtuple->SetBranchAddress("dpstar",&dpstar);
  ReducedNtuple->SetBranchAddress("dcharge",&dcharge);
  ReducedNtuple->SetBranchAddress("dlogvtxprob",&dlogvtxprob);
  ReducedNtuple->SetBranchAddress("dflightbs",&dflightbs);
  ReducedNtuple->SetBranchAddress("dmode",&dmode);
  ReducedNtuple->SetBranchAddress("dmctrue",&dmctrue);

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
  cout<<"Done linking D0 ReducedNtuple Branches"<<endl;
  return 1;
}





Int_t D0Analysis::MakeDMass(){

  if(!OpenReducedNtuple())return 0;

  TFile file(_OutputDir+"/DMassPlot.root","recreate");

  TH1F HDMass("HDMass","",200,1.76,1.96);
  ReducedNtuple->Draw("dmass>>HDMass");
  HDMass.Write();

  sprintf(textlabel,"abs(dmass-%.6f)>%.6f&&abs(dmass-%.6f)<%.6f",
	  DMassCutCenter,DMassCutSBMin,DMassCutCenter,DMassCutSBMin+DMassCutWindow);
  TH1F HDMassSB("HDMassSB","",200,1.76,1.96);
  ReducedNtuple->Draw("dmass>>HDMassSB",textlabel);
  HDMassSB.Write();

  file.ls();
  file.Close();

  CloseReducedNtuple();

  return 1;
}


void D0Analysis::ShowDMassRegions(TH1* Histo){
  cutline.DrawLine(DMassCutCenter-DMassCutWindow,0,DMassCutCenter-DMassCutWindow,Histo->GetMaximum());
  cutline.DrawLine(DMassCutCenter+DMassCutWindow,0,DMassCutCenter+DMassCutWindow,Histo->GetMaximum());

  cutline.DrawLine(DMassCutCenter-DMassCutSBMin-DMassCutWindow,0,DMassCutCenter-DMassCutSBMin-DMassCutWindow,1.6*Histo->GetBinContent(2));//.2*Histo->GetMaximum());
  cutline.DrawLine(DMassCutCenter-DMassCutSBMin,0,DMassCutCenter-DMassCutSBMin,1.6*Histo->GetBinContent(2));//.2*Histo->GetMaximum());
  
  cutline.DrawLine(DMassCutCenter+DMassCutSBMin,0,DMassCutCenter+DMassCutSBMin,1.6*Histo->GetBinContent(2));//.2*Histo->GetMaximum());
  cutline.DrawLine(DMassCutCenter+DMassCutSBMin+DMassCutWindow,0,DMassCutCenter+DMassCutSBMin+DMassCutWindow,1.6*Histo->GetBinContent(2));//.2*Histo->GetMaximum());
  
}


Int_t D0Analysis::PlotDMass(){

  TFile file(_OutputDir+"/DMassPlot.root","read");
  TH1F* HDMass=(TH1F*)file.Get("HDMass");
  //TH1F* HDMassSB=(TH1F*)file.Get("HDMassSB");
  gROOT->cd();
  if(!HDMass){//||!HDMassSB
    cout<<"HDMass not found"<<endl;
    return 0;
  }

  //TH1F* HDMassR=(TH1F*)RebinHistoX(HDMass,150,1.79,1.94);
  TH1F* HDMassR=(TH1F*)RebinHistoX(HDMass,100,1.81,1.92);
  if(!HDMassR){
    cout<<"HDMassR null"<<endl;
    return 0;
  }
  HDMassR->GetYaxis()->SetTitle(" Entries / 1.1 MeV/c^{2} ");
  HDMassR->GetXaxis()->SetTitle(" m(K^{-}#pi^{+})   GeV/c^{2} ");

  Canvas.Print(_OutputDir+"/DMassPlot.ps[");



  Canvas.Clear();
  HDMassR->SetStats(0);
  HDMassR->Draw("pe");
  ShowDMassRegions(HDMassR);
  Canvas.Print(_OutputDir+"/DMassPlot.ps");
   
  //Float_t x[9]={1e4,1.8645,.002,1e4,1.86,.001,100,20,30};
  if(HDMassR->FindObject("stats")) delete HDMassR->FindObject("stats");
  TF1* FitF=FitHistogram2gausPlusPolN(HDMassR,HDMassR->GetXaxis()->GetXmin(),HDMassR->GetXaxis()->GetXmax(),1,0);  
  Canvas.Clear();
  HDMassR->Draw("pe");
  Draw2gaus(FitF);
  ShowDMassRegions(HDMassR);
  Canvas.Print(_OutputDir+"/DMassPlot.ps");

  Canvas.Print(_OutputDir+"/DMassPlot.ps]");


  file.Close();

  return 1;
}


Int_t D0Analysis::FitDMass(){

  TFile file(_OutputDir+"/DMassPlot.root","read");
  TH1F* HDMass=(TH1F*)file.Get("HDMass");
  gROOT->cd();
  if(!HDMass)return 0;

  //TH1F* HDMassR=(TH1F*)RebinHistoX(HDMass,150,1.79,1.94);
  TH1F* HDMassR=HDMass;
  //SetHistoXYLabels(HDMassR,"m(D^{0})","GeV/c^{2}");
  HDMassR->GetYaxis()->SetTitle("Entries/1 MeV");
  HDMassR->GetXaxis()->SetTitle("m(K^{-}#pi^{+})     (GeV/c^{2})");
  HDMassR->SetMarkerStyle(10);
  HDMassR->SetMarkerSize(1);
  HDMassR->GetYaxis()->SetTitleOffset(1.2);

  Canvas.Print(_OutputDir+"/FitDMassPlot.ps[");
 
  //Float_t x[9]={1e4,1.8645,.002,1e4,1.86,.001,100,20,30};
  if(HDMassR->FindObject("stats")) delete HDMassR->FindObject("stats");
  //TF1* FitF=FitHistogram2gausPlusPolN(HDMassR,HDMassR->GetXaxis()->GetXmin(),HDMassR->GetXaxis()->GetXmax(),1,0);  
  TF1* FitF=FitHistogram1gausPlusPol1(HDMassR,HDMassR->GetXaxis()->GetXmin(),HDMassR->GetXaxis()->GetXmax());  
  Canvas.Clear();
  HDMassR->Draw("hist pe");
  FitF->Draw("same");
  //Draw2gaus(FitF);
  Canvas.Print(_OutputDir+"/FitDMassPlot.ps");

  if(HDMassR->FindObject("stats")) delete HDMassR->FindObject("stats");
  HDMassR->SetStats(0);
  Canvas.Clear();
  HDMassR->Draw("hist pe");
  cutline.DrawLine(DMassCutCenter-DMassCutWindow,0,DMassCutCenter-DMassCutWindow,HDMassR->GetMaximum());
  cutline.DrawLine(DMassCutCenter+DMassCutWindow,0,DMassCutCenter+DMassCutWindow,HDMassR->GetMaximum());


  cutline.DrawLine(DMassCutCenter-DMassCutSBMin,0,DMassCutCenter-DMassCutSBMin,.3*HDMassR->GetMaximum());
  cutline.DrawLine(DMassCutCenter-(DMassCutSBMin+DMassCutWindow),0,DMassCutCenter-(DMassCutSBMin+DMassCutWindow),.3*HDMassR->GetMaximum());

  cutline.DrawLine(DMassCutCenter+DMassCutSBMin,0,DMassCutCenter+DMassCutSBMin,.3*HDMassR->GetMaximum());
  cutline.DrawLine(DMassCutCenter+(DMassCutSBMin+DMassCutWindow),0,DMassCutCenter+(DMassCutSBMin+DMassCutWindow),.3*HDMassR->GetMaximum());



  Canvas.Print(_OutputDir+"/FitDMassPlot.ps");
 


  Canvas.Print(_OutputDir+"/FitDMassPlot.ps]");

  file.Close();

  return 1;
}
