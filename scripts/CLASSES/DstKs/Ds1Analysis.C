#include "Ds1Analysis.h"
ClassImp(Ds1Analysis);


//these kind files are might be strange:
//They contain several methods of the class in order to keep size of files small. 
#include "Ds1AnalysisDrawPlots.C"
#include "Ds1AnalysisDrawTruthRatios.C"
#include "Ds1AnalysisDrawCutSummary.C"
#include "Ds1AnalysisFitResolution.C"
#include "Ds1AnalysisFitMassPeak.C"
#include "Ds1AnalysisDrawPstarComparison.C"
#include "Ds1AnalysisDrawEfficiencyVsPstar.C"


//The constructor
Ds1Analysis::Ds1Analysis(TChain* fChain,Bool_t truthMatch,Int_t MatterOrAntiMatter, TString Mode,TString OutputDir,Int_t WhichCuts):
  _fChain(fChain),
  _MatterOrAntiMatter(MatterOrAntiMatter),
  _Mode(Mode),							   
  _OutputDir(OutputDir),
  TruthMatch(truthMatch),
  _WhichCuts(WhichCuts),
  OutPutFile(NULL),
  ReducedNtuple(NULL),
  MCDs1pCounterPerEvent(0),
  MCDs1pCounterTotal(0),
  RecoDs1pCounterTotal(0),
  DstarMassWindow(.050), 
  D0MassWindow(.060),
  KsMassWindow(.020),
  Ds1pp3CMmax(5),
  Ds1pp3CMmin(0),
  Ds1pp3CMNbins(50),
  Ds1pMassWindow(.03),
  Ds1pPDGDeltaM1(Ds1pPDGMass-DstarPDGMass),
  Ds1pPDGDeltaM2(Ds1pPDGMass-DstarPDGMass-K0PDGMass),
  Ds1DeltaMNbins(100),
  DstarDeltaMmin(.140),
  DstarDeltaMmax(.150),
  DstarDeltaMNbins(300),
  Ds1Cosstarmin(-1.0001),
  Ds1Cosstarmax(1.0001),
  Ds1CosstarNbins(20)

{
  if(_fChain==NULL){cout<<"No Chain"<<endl;}
  

  //init the database
  database.SetMode(Mode);
  database.SetTruthMatch(truthMatch);
  database.SetCuts(WhichCuts);//has to be called last
  ////Initialize the cuts

  Ds1pMCTrueCut.Init("Ds1pMCTrueCut",&ds1pmctrue,1,1,1);
  Ds1pDeltaM2Cut.Init("Ds1pDeltaM2Cut",&ds1pdeltam2,database.GetDs1pDeltaMMin(),database.GetDs1pDeltaMMax(),1);
  D0pstarCut.Init("D0pstarCut",&d0pstar,database.GetD0pstarMin(),100,1);
  DstarDeltaMCut.Init("DstarDeltaMCut",&dstardeltam,database.GetDstarDeltaMMin(),database.GetDstarDeltaMMax(),1);
  KLHCut.Init("KLHCut",&klh,database.GetKLHBit(),100,1);
  PiLHCut.Init("PiLHCut",&pilh,database.GetPiLHBit(),100,1);  
  D0MassCut.Init("D0MassCut",&d0mass,database.GetD0MassMin(),database.GetD0MassMax(),1);
  D0ProbCut.Init("D0ProbCut",&d0logvtxprob,database.GetD0MinLogProbab(),.00001,1);
  KsMassCut.Init("KsMassCut",&ksmass,database.GetKsMassMin(),database.GetKsMassMax(),1);
  KsCosineCut.Init("KsCosineCut",&kscosine,database.GetKsCosineMin(),1.000001,1);
  Ds1pProbCut.Init("Ds1pProbCut",&ds1plogvtxprob,database.GetDs1pMinLogProbab(),0.000001,1);
  Ds1ppstarCut.Init("Ds1ppstarCut",&ds1ppstar,database.GetDs1ppstarMin(),100,1);
  Ds1pBestVtxProbCut.Init("Ds1pBestVtxProbCut",&ds1pbestvtxprob,1,1,1);
  Pi1LHCut.Init("Pi1LHCut",&pi1lh,database.GetPiLHBit(),100,1);
  Pi2LHCut.Init("Pi2LHCut",&pi2lh,database.GetPiLHBit(),100,1);
  D0Pi2LHCut.Init("D0Pi2LHCut",&pi1lh,2,100,1);
  D0Pi3LHCut.Init("D0Pi3LHCut",&pi2lh,2,100,1);




  //list of all cuts applied 
  Ds1pCuts.AddCut(&Ds1pMCTrueCut);
  Ds1pCuts.AddCut(&Ds1pDeltaM2Cut);
  Ds1pCuts.AddCut(&D0pstarCut);
  Ds1pCuts.AddCut(&DstarDeltaMCut); 
  Ds1pCuts.AddCut(&KLHCut);
  Ds1pCuts.AddCut(&PiLHCut);
  Ds1pCuts.AddCut(&D0MassCut);
  Ds1pCuts.AddCut(&D0ProbCut);
  Ds1pCuts.AddCut(&KsMassCut);
  Ds1pCuts.AddCut(&KsCosineCut);
  Ds1pCuts.AddCut(&Ds1pProbCut);
  Ds1pCuts.AddCut(&Ds1ppstarCut);
  Ds1pCuts.AddCut(&Pi1LHCut);
  Ds1pCuts.AddCut(&Pi2LHCut);
  if(_Mode=="D0ToK3Pi"){
    Ds1pCuts.AddCut(&D0Pi2LHCut);
    Ds1pCuts.AddCut(&D0Pi3LHCut);
  }
  //
  cutline.SetLineColor(2);
  cutline.SetLineWidth(1);

  cutbox.SetLineColor(2);
  cutbox.SetLineWidth(1);
  //cutbox.SetLineStyle(1);
  cutbox.SetFillStyle(0);
  cout<<"Done Initializing the analysis"<<endl;
}

Ds1Analysis::~Ds1Analysis(){  
}


Int_t Ds1Analysis::ReduceNtuple(){
  cout<<"Going to reduce the ntuple"<<endl;
  if(_fChain==NULL){
    cout<<"No chain."<<endl;
    return 0;
  } 


  ///set the branches to be read on tree 
  SetBranches();


  //create new reduced ntuple
  ReducedNtuple=new TTree("Ds1pNtuple","Ds1 Reduced Ntuple",99);        
  
  //create the branches in the ReducedNtuple
  MakeReducedNtupleBranches();

  //create the MC Histos
  HMCDs1pMass=new TH1F();
  SetHistoXY(HMCDs1pMass,"HMCDs1pMass",100,Ds1pPDGMass-Ds1pMassWindow,Ds1pPDGMass+Ds1pMassWindow,
	     "Ds1p Cand. Mass (GeV/c^{2})","");

  HMCDs1pp3CM=new TH1F();
  SetHistoXY(HMCDs1pp3CM,"HMCDs1pp3CM",Ds1pp3CMNbins,Ds1pp3CMmin,Ds1pp3CMmax,
	     "D'_{S1} p* (GeV/c)","");

  HMCDs1pp3CMTorsten=new TH1F();
  SetHistoXY(HMCDs1pp3CMTorsten,"HMCDs1pp3CMTorsten",10,2.7,4.6,
	     "D'_{S1} p* (GeV/c)","");


  HMCDs1pcosthCM=new TH1F();
  SetHistoXY(HMCDs1pcosthCM,"HMCDs1pcosthCM",Ds1CosstarNbins,Ds1Cosstarmin,Ds1Cosstarmax,
	     "D'_{S1} cos(#theta)*","");

  H2MCDs1pCMPvsTheta=new TH2F();
  SetHisto2D(H2MCDs1pCMPvsTheta,"H2MCDs1pCMPvsTheta",Ds1pp3CMNbins,Ds1pp3CMmin,Ds1pp3CMmax,
	     "D'_{S1} p* (GeV/c)",Ds1CosstarNbins,Ds1Cosstarmin,Ds1Cosstarmax,"D'_{S1} cos(#theta*)","");

  HMCNDs1p=new TH1F();
  SetHistoXY(HMCNDs1p,"HMCNDs1p",11,-.5,10.5,
	     "nDs1p/event","Counts");

  HLundCheck=new TH1F();
  SetHisto(HLundCheck,"HLundCheck",15,-.5,14.5,
	   "ParticleLund - LundExp");


  Double_t bestvtxchi2=0;
  Int_t bestDs1pIdx=0;
  //Start the event loop;
  Int_t eventid=0;
  while(_fChain->GetEntry(eventid,0)>0){   
    eventid++;
     
    if(eventid%5000==0)cout<<eventid<<" Events done."<<endl;    
           
    if(nDs1p>400){cout<<"Too many cands at event"<<eventid<<endl;continue;}


    RecoDs1pCounterPerEvent=0;   
    
    //flag the candidate with best chisquare
    bestvtxchi2=1e30;
    bestDs1pIdx=0;
    for(Ds1pIdx=0;Ds1pIdx<nDs1p;Ds1pIdx++){
       if(Ds1pChi2[Ds1pIdx]<bestvtxchi2){
	 bestvtxchi2=Ds1pChi2[Ds1pIdx];
	 bestDs1pIdx=Ds1pIdx;
       }
    } 
    //
    
    //------beam quantities
    TVector3 beamVtx(primVtxX,primVtxY,primVtxZ);

    for(Ds1pIdx=0;Ds1pIdx<nDs1p;Ds1pIdx++){

      ////////////cut out the unconverged fits
      if(Ds1pVtxStatus[Ds1pIdx]!=0)continue;

    
      KsIdx=Ds1pd2Idx[Ds1pIdx];  
      KsPi1Idx=Ksd1Idx[KsIdx];
      KsPi2Idx=Ksd2Idx[KsIdx];    
      DstarIdx=Ds1pd1Idx[Ds1pIdx];
      SlowPiIdx=Dstard2Idx[DstarIdx];
      D0Idx=Dstard1Idx[DstarIdx];
      PiIdx=D0d2Idx[D0Idx];
      KIdx=D0d1Idx[D0Idx];   
      if(_Mode=="D0ToK3Pi"){
	D0Pi2Idx=D0d3Idx[D0Idx];
	D0Pi3Idx=D0d4Idx[D0Idx];
      }   

      PitrkIdx=PiTrkIdx[PiIdx];
      KtrkIdx=KTrkIdx[KIdx];
      SlowPitrkIdx=PiTrkIdx[SlowPiIdx];
      KsPi1trkIdx=PiTrkIdx[KsPi1Idx];
      KsPi2trkIdx=PiTrkIdx[KsPi2Idx];
      if(_Mode=="D0ToK3Pi"){
	D0Pi2trkIdx=PiTrkIdx[D0Pi2Idx];
	D0Pi3trkIdx=PiTrkIdx[D0Pi3Idx];
      }
      //Check that Im using proper indexe 
      if(Dstard1Lund[DstarIdx]!=D0Lund[D0Idx])HLundCheck->Fill(1);
      if(Dstard2Lund[DstarIdx]!=PiLund[SlowPiIdx])HLundCheck->Fill(2);
      if(D0d1Lund[D0Idx]!=KLund[KIdx])HLundCheck->Fill(3);
      if(D0d2Lund[D0Idx]!=PiLund[PiIdx])HLundCheck->Fill(4);
      if(TRKLund[SlowPitrkIdx]!=PiLund[SlowPiIdx])HLundCheck->Fill(5);
      if(TRKLund[PitrkIdx]!=PiLund[PiIdx])HLundCheck->Fill(6);
      if(Ds1pd1Lund[Ds1pIdx]!=DstarLund[DstarIdx])HLundCheck->Fill(7);
      if(Ds1pd2Lund[Ds1pIdx]!=KsLund[KsIdx])HLundCheck->Fill(8);
      if(TRKLund[KsPi1trkIdx]!=PiLund[KsPi1Idx])HLundCheck->Fill(9);
      if(TRKLund[KsPi2trkIdx]!=PiLund[KsPi2Idx])HLundCheck->Fill(10);
      if(Ksd1Lund[KsIdx]!=PiLund[KsPi1Idx])HLundCheck->Fill(11);
      if(Ksd2Lund[KsIdx]!=PiLund[KsPi2Idx])HLundCheck->Fill(12);
      //if(TRKLund[KtrkIdx]!=KLund[KIdx])HLundCheck->Fill(13);//TRKLund is always Pion ??

      if(_Mode=="D0ToK3Pi"){
	if(D0d3Lund[D0Idx]!=PiLund[D0Pi2Idx])HLundCheck->Fill(13);
	if(D0d4Lund[D0Idx]!=PiLund[D0Pi3Idx])HLundCheck->Fill(14);
	if(TRKLund[D0Pi2trkIdx]!=PiLund[D0Pi2Idx])HLundCheck->Fill(15);
	if(TRKLund[D0Pi3trkIdx]!=PiLund[D0Pi3Idx])HLundCheck->Fill(16);
      }



      //////fill reduced ntuple variables    
      //----------Ds1 quantities
      ds1pmass=Ds1pMass[Ds1pIdx];
      ds1ppstar=Ds1pp3CM[Ds1pIdx];
      ds1pcosstar=Ds1pcosthCM[Ds1pIdx];
      ds1pdeltam1=Ds1pMass[Ds1pIdx]-DstarMass[DstarIdx];
      ds1pdeltam2=Ds1pMass[Ds1pIdx]-DstarMass[DstarIdx]-KsMass[KsIdx]; 
      ds1pvtxstatus=Ds1pVtxStatus[Ds1pIdx];       
      if(TMath::Prob(Ds1pChi2[Ds1pIdx],Ds1pnDof[Ds1pIdx])>0) ds1plogvtxprob=log10(TMath::Prob(Ds1pChi2[Ds1pIdx],Ds1pnDof[Ds1pIdx])); 
      else ds1plogvtxprob=-100;        
      if(Ds1pLund[Ds1pIdx]>0)ds1pcharge=1;
      if(Ds1pLund[Ds1pIdx]<0)ds1pcharge=-1;
      if(bestDs1pIdx==Ds1pIdx) ds1pbestvtxprob=1; else ds1pbestvtxprob=0;        
      ds1pnDs1p=nDs1p;      
      TVector3 Ds1pVtx(Ds1pVtxx[Ds1pIdx],Ds1pVtxy[Ds1pIdx],Ds1pVtxz[Ds1pIdx]);   
      TVector3 Ds1pDecayVtxToBeamVtxDirection(Ds1pVtx-beamVtx);
      Ds1pDecayVtxToBeamVtxDirection=Ds1pDecayVtxToBeamVtxDirection.Unit();  
      
      //-------Dstar quantities
      dstarmass=DstarMass[DstarIdx];
      dstardeltam=DstarMass[DstarIdx]-D0Mass[D0Idx];
      dstarvtxprob=TMath::Prob(DstarChi2[DstarIdx],DstarnDof[DstarIdx]);   
      TVector3 DstarMomemtumDirection(sin(acos(Dstarcosth[DstarIdx]))*cos(Dstarphi[DstarIdx]),
				      sin(acos(Dstarcosth[DstarIdx]))*sin(Dstarphi[DstarIdx]),
				      Dstarcosth[DstarIdx]); 
      TVector3 DstarVtx(DstarVtxx[DstarIdx],DstarVtxy[DstarIdx],DstarVtxz[DstarIdx]);
      
      TVector3 DstarDecayVtxToDs1pDecayVtxDirection(DstarVtx-Ds1pVtx);
      DstarDecayVtxToDs1pDecayVtxDirection=DstarDecayVtxToDs1pDecayVtxDirection.Unit();

      TVector3 DstarDecayVtxToBeamVtxDirection(DstarVtx-beamVtx);
      DstarDecayVtxToBeamVtxDirection=DstarDecayVtxToBeamVtxDirection.Unit();      

      //------D0 quantities
      d0mass=D0Mass[D0Idx];
      //d0logvtxprob=log10(DstarD0VtxProbUsr[DstarIdx]);
      if(D0VtxStatus[D0Idx]==0) d0logvtxprob=log10(TMath::Prob(D0Chi2[D0Idx],D0nDof[D0Idx]));
      else d0logvtxprob=0;//since I only make one fit at Ds1p vtx-> There is only one prob
      d0pstar=D0p3CM[D0Idx];    
      TVector3 D0Vtx(D0Vtxx[D0Idx],D0Vtxy[D0Idx],D0Vtxz[D0Idx]);
      TVector3 D0MomentumDirection(sin(acos(D0costh[D0Idx]))*cos(D0phi[D0Idx]),
				   sin(acos(D0costh[D0Idx]))*sin(D0phi[D0Idx]),
				   D0costh[D0Idx]); 
 
      
      TVector3 D0DecayVtxToDstarDecayVtxDirection(D0Vtx-DstarVtx);
      D0DecayVtxToDstarDecayVtxDirection=D0DecayVtxToDstarDecayVtxDirection.Unit();
      
      TVector3 D0DecayVtxToDs1pDecayVtxDirection(D0Vtx-Ds1pVtx);
      D0DecayVtxToDs1pDecayVtxDirection=D0DecayVtxToDs1pDecayVtxDirection.Unit();

      TVector3 D0DecayVtxToBeamVtxDirection(D0Vtx-beamVtx);
      D0DecayVtxToBeamVtxDirection=D0DecayVtxToBeamVtxDirection.Unit();
     
      d0cosine=D0MomentumDirection*D0DecayVtxToBeamVtxDirection;
      d0cosine1=D0MomentumDirection*D0DecayVtxToDstarDecayVtxDirection;
 
      
      //Ks quantities
      ksmass=Ds1pKsMassUsr[Ds1pIdx];//KsMass[KsIdx];//using Usr Data
      ksvtxprob=TMath::Prob(KsChi2[KsIdx],KsnDof[KsIdx]); 
      TVector3 KsVtx(KsVtxx[KsIdx],KsVtxy[KsIdx],KsVtxz[KsIdx]);
      TVector3 KsMomentumDirection(sin(acos(Kscosth[KsIdx]))*cos(Ksphi[KsIdx]),
				   sin(acos(Kscosth[KsIdx]))*sin(Ksphi[KsIdx]),
				   Kscosth[KsIdx]);
 
      TVector3 KsDecayVtxToDs1pDecayVtxDirection(KsVtx-Ds1pVtx);
      KsDecayVtxToDs1pDecayVtxDirection=KsDecayVtxToDs1pDecayVtxDirection.Unit();
      
      TVector3 KsDecayVtxToBeamVtxDirection(KsVtx-beamVtx);
      KsDecayVtxToBeamVtxDirection=KsDecayVtxToBeamVtxDirection.Unit();

      kscosine1=KsFlightBSCosA[KsIdx];
      kscosine=KsMomentumDirection*KsDecayVtxToBeamVtxDirection;
      kscosine2=KsMomentumDirection*KsDecayVtxToDs1pDecayVtxDirection;
      ksflightbslen=KsFlightBSLen[KsIdx];
      ksflightbscosa=KsFlightBSCosA[KsIdx];

  
      

      kp3=Kp3[KIdx];
      pip3=Pip3[PiIdx];
      slowpip3=Pip3[SlowPiIdx];
      pi1p3=Pip3[KsPi1Idx];
      pi2p3=Pip3[KsPi2Idx];

      klh=0;  
      for(Int_t i=10;i<=14;i++)//10=notpion,11=veryloose,12=loose,13=tight,14=verytight
	if((KSelectorsMap[KtrkIdx] & (1<<i)) == (1<<i))	
	  klh=i;  
      
      
      pilh=0;pi1lh=0;pi2lh=0; d0pi2lh=0; d0pi3lh=0; 
      for(Int_t i=2;i<=5;i++){//2=veryloose,3=loose,4=tight,5=verytight
	if((PiSelectorsMap[PitrkIdx] & (1<<i) ) == (1<<i))	
	  pilh=i;
	if((PiSelectorsMap[KsPi1trkIdx] & (1<<i) ) == (1<<i))	
	  pi1lh=i;
	if((PiSelectorsMap[KsPi2trkIdx] & (1<<i) ) == (1<<i))	
	  pi2lh=i;  

	if(_Mode=="D0ToK3Pi"){
	  if((PiSelectorsMap[D0Pi2trkIdx] & (1<<i) ) == (1<<i))	
	    d0pi2lh=i;  
	  if((PiSelectorsMap[D0Pi3trkIdx] & (1<<i) ) == (1<<i))	
	    d0pi3lh=i;  
	}
      }
	

      ds1pmctrue=1;
      dstarmctrue=1;
      d0mctrue=1;
      ksmctrue=1;
      pimctrue=1;
      slowpimctrue=1;
      kmctrue=1;
      pi1mctrue=1;
      pi2mctrue=1;
      
      if(TruthMatch){
	ds1pmctrue=(Ds1pMCIdx[Ds1pIdx]>0);
	dstarmctrue=(DstarMCIdx[DstarIdx]>0);
	d0mctrue=(D0MCIdx[D0Idx]>0);
	ksmctrue=(KsMCIdx[KsIdx]>0);
	pimctrue=(PiMCIdx[PiIdx]>0);
	slowpimctrue=(PiMCIdx[SlowPiIdx]>0);
	kmctrue=(KMCIdx[KIdx]>0);
	pi1mctrue=(PiMCIdx[KsPi1Idx]>0);
	pi2mctrue=(PiMCIdx[KsPi2Idx]>0);
      }

      if(TruthMatch&&ds1pmctrue)
	ds1pdmres=ds1pdeltam2-(mcmass[Ds1pMCIdx[Ds1pIdx]]-mcmass[DstarMCIdx[DstarIdx]]-mcmass[KsMCIdx[KsIdx]]);
      else ds1pdmres=100000;
      
      eventnumber=eventid;
  
      ReducedNtuple->Fill();	              
               
      RecoDs1pCounterTotal++;
    }//Ds1p loop    
   


    if(TruthMatch){
      //now loop over the MC
      MCDs1pCounterPerEvent=0; 
      Int_t mcid=-1;
      while(mcid<mcLen){
	mcid++;

	if(mcLund[mcid]==_MatterOrAntiMatter*myDs1pLund){
	  MCDs1pCounterPerEvent++; 
	  MCDs1pCounterTotal++;
		
	  HMCDs1pMass->Fill(mcmass[mcid]);
	  HMCDs1pp3CM->Fill(mcp3CM[mcid]); 
	  HMCDs1pp3CMTorsten->Fill(mcp3CM[mcid]); 
	  HMCDs1pcosthCM->Fill(mccosthCM[mcid]); 	
	  H2MCDs1pCMPvsTheta->Fill(mcp3CM[mcid],mccosthCM[mcid]);
	}
      }
      HMCNDs1p->Fill(MCDs1pCounterPerEvent);
    }


  }

  //print summary
  cout<<"--------Summary-------"<<endl;
  cout<<"Total events="<<eventid<<endl;
  cout<<"Total Generated="<<MCDs1pCounterTotal<<" Reconstructed="<<RecoDs1pCounterTotal<<endl;
  cout<<"--------End Summary---"<<endl;


  ///Save the new ntuple
  if(!SaveReducedNtuple())
    return 0;

  return 1;
}

Int_t Ds1Analysis::SaveReducedNtuple(){
  cout<<"Going to save the ntuple and histograms"<<endl; 

  if(ReducedNtuple==NULL){cout<<"ReducedNtuple  is NULL."<<endl;return 0;}
  if(HMCDs1pMass==NULL){cout<<"HMCDs1pMass  is NULL."<<endl;return 0;}
  if(HMCDs1pp3CM==NULL){cout<<"HMCDs1pp3CM  is NULL."<<endl;return 0;}
  if(HMCDs1pp3CMTorsten==NULL){cout<<"HMCDs1pp3CMTorsten  is NULL."<<endl;return 0;}
  if(HMCDs1pcosthCM==NULL){cout<<"HMCDs1pcosthCM  is NULL."<<endl;return 0;}     
  if(H2MCDs1pCMPvsTheta==NULL){cout<<"H2MCDs1pCMPvsTheta  is NULL."<<endl;return 0;}
  if(HMCNDs1p==NULL){cout<<"HMCNDs1p  is NULL."<<endl;return 0;}     
  if(HLundCheck==NULL){cout<<"HLundCheck  is NULL."<<endl;return 0;}   

  OutPutFile=new TFile(_OutputDir+"/Ds1pReduced.root","recreate",_OutputDir+"/Ds1pReduced.root",4);
  if(OutPutFile==NULL){cout<<"Failed to create file"<<endl;return 0;}

  if(ReducedNtuple->Write()&&
     HMCDs1pMass->Write()&&
     HMCDs1pp3CM->Write()&&
     HMCDs1pp3CMTorsten->Write()&&
     HMCDs1pcosthCM->Write()&&     
     H2MCDs1pCMPvsTheta->Write()&&
     HMCNDs1p->Write()&&     
     HLundCheck->Write()){ 
    cout<<"Ntuple and Histograms have been written"<<endl;
  }
  else{
    cout<<"Failed to write objects"<<endl;return 0;
  }

 
  OutPutFile->Write(); //necesary otherwise file is not closed 
  OutPutFile->Close();  
  cout<<OutPutFile->GetName()<<" has been closed."<<endl;
  delete OutPutFile;

  delete  ReducedNtuple;
  delete  HMCDs1pMass;
  delete  HMCDs1pp3CM;
  delete  HMCDs1pcosthCM;     
  delete  H2MCDs1pCMPvsTheta;
  delete  HMCNDs1p;     
  delete  HLundCheck;   
  return 1;
}


Int_t Ds1Analysis::OpenReducedNtuple(){
  
  cout<<"Going to open reduced ntuple."<<endl;

  if(!(OutPutFile=new TFile(_OutputDir+"/Ds1pReduced.root","read"))){
    cout<<"Unable to open reduced file"<<endl;
  }
  if(OutPutFile->IsZombie()){
    cout<<"Zombie reduced ntuple file"<<endl;return 0;
  }
  gROOT->cd();
  cout<<"File opened: "<<OutPutFile->GetName()<<endl;


  if(!(ReducedNtuple=(TTree*)OutPutFile->Get("Ds1pNtuple"))){
    cout<<"No reduced ntuple"<<endl;return 0;
  }
  ReducedNtuple->SetDirectory(0);
  cout<<"Ntuple Extracted: "<<ReducedNtuple->GetName()<<"  With "<<ReducedNtuple->GetEntries()<<" Entries"<<endl;


  SetReducedNtupleBranches();
  

  return 1;
}

Int_t Ds1Analysis::Close(){
  cout<<"Going to close the reduced Ntuple file."<<endl;
 
  OutPutFile->Close();
  //if(OutPutFile!=NULL)
  delete OutPutFile;
  
  delete ReducedNtuple;


  return 1;
}

Int_t Ds1Analysis::SetBranches(){
  if(_fChain==NULL){cout<<"No Chain"<<endl;return 0;} 

  _fChain->SetBranchAddress("beamSX",&beamSX);
  _fChain->SetBranchAddress("beamSY",&beamSY);
  _fChain->SetBranchAddress("beamSZ",&beamSZ);
  _fChain->SetBranchAddress("primVtxX",&primVtxX);
  _fChain->SetBranchAddress("primVtxY",&primVtxY);
  _fChain->SetBranchAddress("primVtxZ",&primVtxZ);
    

  _fChain->SetBranchAddress("nDs1p",&nDs1p);
  _fChain->SetBranchAddress("Ds1pMass",Ds1pMass);
  _fChain->SetBranchAddress("Ds1pcosthCM",Ds1pcosthCM);
  _fChain->SetBranchAddress("Ds1pp3CM",Ds1pp3CM);
  _fChain->SetBranchAddress("Ds1pLund",Ds1pLund); 
  _fChain->SetBranchAddress("Ds1pd1Lund",Ds1pd1Lund);
  _fChain->SetBranchAddress("Ds1pd1Idx",Ds1pd1Idx);
  _fChain->SetBranchAddress("Ds1pd2Lund",Ds1pd2Lund);
  _fChain->SetBranchAddress("Ds1pd2Idx",Ds1pd2Idx);
  _fChain->SetBranchAddress("Ds1pVtxx",Ds1pVtxx);
  _fChain->SetBranchAddress("Ds1pVtxy",Ds1pVtxy);
  _fChain->SetBranchAddress("Ds1pVtxz",Ds1pVtxz);
  _fChain->SetBranchAddress("Ds1pVtxStatus",Ds1pVtxStatus);
  _fChain->SetBranchAddress("Ds1pChi2",Ds1pChi2);
  _fChain->SetBranchAddress("Ds1pnDof",Ds1pnDof);
  _fChain->SetBranchAddress("Ds1pKsMassUsr",Ds1pKsMassUsr);

  _fChain->SetBranchAddress("nKs",&nKs);
  _fChain->SetBranchAddress("KsMass",KsMass);
  _fChain->SetBranchAddress("Ksp3CM",Ksp3CM);  
  _fChain->SetBranchAddress("KsLund",KsLund);
  _fChain->SetBranchAddress("Ksd1Lund",Ksd1Lund);
  _fChain->SetBranchAddress("Ksd1Idx",Ksd1Idx);
  _fChain->SetBranchAddress("Ksd2Lund",Ksd2Lund);
  _fChain->SetBranchAddress("Ksd2Idx",Ksd2Idx);
  _fChain->SetBranchAddress("KsChi2",KsChi2);
  _fChain->SetBranchAddress("KsnDof",KsnDof);
  _fChain->SetBranchAddress("KsVtxx",KsVtxx);
  _fChain->SetBranchAddress("KsVtxy",KsVtxy);
  _fChain->SetBranchAddress("KsVtxz",KsVtxz);
  _fChain->SetBranchAddress("Kscosth",Kscosth);
  _fChain->SetBranchAddress("Ksphi",Ksphi);
  _fChain->SetBranchAddress("KsFlightBSLen",KsFlightBSLen);
  _fChain->SetBranchAddress("KsFlightBSCosA",KsFlightBSCosA);

  _fChain->SetBranchAddress("nDstar",&nDstar);
  _fChain->SetBranchAddress("DstarMass",DstarMass);
  _fChain->SetBranchAddress("Dstarp3",Dstarp3);
  _fChain->SetBranchAddress("Dstarcosth",Dstarcosth);
  _fChain->SetBranchAddress("Dstarphi",Dstarphi);
  _fChain->SetBranchAddress("DstarLund",DstarLund);
  _fChain->SetBranchAddress("Dstard1Lund",Dstard1Lund);
  _fChain->SetBranchAddress("Dstard1Idx",Dstard1Idx);
  _fChain->SetBranchAddress("Dstard2Lund",Dstard2Lund);
  _fChain->SetBranchAddress("Dstard2Idx",Dstard2Idx);
  _fChain->SetBranchAddress("DstarChi2",DstarChi2);
  _fChain->SetBranchAddress("DstarnDof",DstarnDof);
  _fChain->SetBranchAddress("DstarD0VtxProbUsr",DstarD0VtxProbUsr);
  _fChain->SetBranchAddress("DstarVtxx",DstarVtxx);
  _fChain->SetBranchAddress("DstarVtxy",DstarVtxy);
  _fChain->SetBranchAddress("DstarVtxz",DstarVtxz);

  _fChain->SetBranchAddress("nD0",&nD0);
  _fChain->SetBranchAddress("D0Mass",D0Mass);
  _fChain->SetBranchAddress("D0p3",D0p3);
  _fChain->SetBranchAddress("D0costh",D0costh);
  _fChain->SetBranchAddress("D0phi",D0phi);
  _fChain->SetBranchAddress("D0p3CM",D0p3CM);  
  _fChain->SetBranchAddress("D0Lund",D0Lund);
  _fChain->SetBranchAddress("D0d1Lund",D0d1Lund);
  _fChain->SetBranchAddress("D0d1Idx",D0d1Idx);
  _fChain->SetBranchAddress("D0d2Lund",D0d2Lund);
  _fChain->SetBranchAddress("D0d2Idx",D0d2Idx);
  _fChain->SetBranchAddress("D0Chi2",D0Chi2);
  _fChain->SetBranchAddress("D0nDof",D0nDof);
  _fChain->SetBranchAddress("D0Vtxx",D0Vtxx);
  _fChain->SetBranchAddress("D0Vtxy",D0Vtxy);
  _fChain->SetBranchAddress("D0Vtxz",D0Vtxz);

  _fChain->SetBranchAddress("nPi",&nPi);
  _fChain->SetBranchAddress("Pip3",Pip3);
  _fChain->SetBranchAddress("PiLund",PiLund);
  _fChain->SetBranchAddress("PiTrkIdx",PiTrkIdx);

  _fChain->SetBranchAddress("nK",&nK);
  _fChain->SetBranchAddress("Kp3",Kp3);
  _fChain->SetBranchAddress("KLund",KLund);
  _fChain->SetBranchAddress("KTrkIdx",KTrkIdx);


  _fChain->SetBranchAddress("TRKnSvt",TRKnSvt);
  _fChain->SetBranchAddress("TRKLund",TRKLund);
  _fChain->SetBranchAddress("piSelectorsMap",PiSelectorsMap);
  _fChain->SetBranchAddress("KSelectorsMap",KSelectorsMap);


  if(TruthMatch){
    _fChain->SetBranchAddress("Ds1pMCIdx",Ds1pMCIdx);
    _fChain->SetBranchAddress("DstarMCIdx",DstarMCIdx);
    _fChain->SetBranchAddress("D0MCIdx",D0MCIdx);
    _fChain->SetBranchAddress("KMCIdx",KMCIdx);
    _fChain->SetBranchAddress("PiMCIdx",PiMCIdx);
    _fChain->SetBranchAddress("KsMCIdx",KsMCIdx);
    _fChain->SetBranchAddress("mcLund",mcLund);
    _fChain->SetBranchAddress("mcLen",&mcLen);
    _fChain->SetBranchAddress("mcmass",mcmass);
    _fChain->SetBranchAddress("mcp3",mcp3);
    _fChain->SetBranchAddress("mccosth",mccosth);
    _fChain->SetBranchAddress("mcp3CM",mcp3CM);
    _fChain->SetBranchAddress("mccosthCM",mccosthCM); 
   }
  

 
  SetModeBranches();

  return 1;
}

Int_t Ds1Analysis::SetModeBranches(){
  if(_Mode=="D0ToK3Pi"){
    _fChain->SetBranchAddress("D0d3Lund",D0d3Lund);
    _fChain->SetBranchAddress("D0d3Idx",D0d3Idx);
    _fChain->SetBranchAddress("D0d4Lund",D0d4Lund);
    _fChain->SetBranchAddress("D0d4Idx",D0d4Idx);
  }


  return 1;
}

Int_t Ds1Analysis::MakeReducedNtupleBranches(){

  ReducedNtuple->Branch("eventnumber",&eventnumber,"eventnumber/I");
  ReducedNtuple->Branch("ds1pmass",&ds1pmass,"ds1pmass/F");
  ReducedNtuple->Branch("ds1pdeltam1",&ds1pdeltam1,"ds1pdeltam1/F");
  ReducedNtuple->Branch("ds1pdeltam2",&ds1pdeltam2,"ds1pdeltam2/F");
  ReducedNtuple->Branch("ds1ppstar",&ds1ppstar,"ds1ppstar/F");
  ReducedNtuple->Branch("ds1pcosstar",&ds1pcosstar,"ds1pcosstar/F");
  ReducedNtuple->Branch("ds1pmctrue",&ds1pmctrue,"ds1pmctrue/I");
  ReducedNtuple->Branch("ds1pcharge",&ds1pcharge,"ds1pcharge/I");
  ReducedNtuple->Branch("ds1pbestvtxprob",&ds1pbestvtxprob,"ds1pbestvtxprob/I");
  ReducedNtuple->Branch("Ds1pIdx",&Ds1pIdx,"Ds1pIdx/I");
  ReducedNtuple->Branch("ds1pnDs1p",&ds1pnDs1p,"ds1pnDs1p/I");
  ReducedNtuple->Branch("ds1pdmres",&ds1pdmres,"ds1pdmres/F");
  ReducedNtuple->Branch("ds1pvtxstatus",&ds1pvtxstatus,"ds1pvtxstatus/I");
  ReducedNtuple->Branch("ds1plogvtxprob",&ds1plogvtxprob,"ds1plogvtxprob/F");
  ReducedNtuple->Branch("dstarmass",&dstarmass,"dstarmass/F");
  ReducedNtuple->Branch("dstardeltam",&dstardeltam,"dstardeltam/F");
  ReducedNtuple->Branch("dstarmctrue",&dstarmctrue,"dstarmctrue/I");
  ReducedNtuple->Branch("dstarvtxprob",&dstarvtxprob,"dstarvtxprob/F");
  ReducedNtuple->Branch("d0mass",&d0mass,"d0mass/F");
  ReducedNtuple->Branch("d0pstar",&d0pstar,"d0pstar/F");
  ReducedNtuple->Branch("d0mctrue",&d0mctrue,"d0mctrue/I");
  ReducedNtuple->Branch("d0cosine",&d0cosine,"d0cosine/F");
  ReducedNtuple->Branch("d0cosine1",&d0cosine1,"d0cosine1/F");
  ReducedNtuple->Branch("d0logvtxprob",&d0logvtxprob,"d0logvtxprob/F");
  ReducedNtuple->Branch("ksmass",&ksmass,"ksmass/F");
  ReducedNtuple->Branch("kscosine",&kscosine,"kscosine/F");
  ReducedNtuple->Branch("kscosine1",&kscosine1,"kscosine1/F");
  ReducedNtuple->Branch("kscosine2",&kscosine2,"kscosine2/F");
  ReducedNtuple->Branch("ksmctrue",&ksmctrue,"ksmctrue/I");
  ReducedNtuple->Branch("ksvtxprob",&ksvtxprob,"ksvtxprob/F");
  ReducedNtuple->Branch("ksflightbslen",&ksflightbslen,"ksflightbslen/F");
  ReducedNtuple->Branch("ksflightbscosa",&ksflightbscosa,"ksflightbscosa/F");
  ReducedNtuple->Branch("klh",&klh,"klh/I");
  ReducedNtuple->Branch("kmctrue",&kmctrue,"kmctrue/I");
  ReducedNtuple->Branch("kp3",&kp3,"kp3/F");
  ReducedNtuple->Branch("pilh",&pilh,"pilh/I");
  ReducedNtuple->Branch("pimctrue",&pimctrue,"pimctrue/I");
  ReducedNtuple->Branch("pip3",&pip3,"pip3/F");
  ReducedNtuple->Branch("pi1lh",&pi1lh,"pi1lh/I");
  ReducedNtuple->Branch("pi1mctrue",&pi1mctrue,"pi1mctrue/I");
  ReducedNtuple->Branch("pi1p3",&pi1p3,"pi1p3/F");
  ReducedNtuple->Branch("pi2lh",&pi2lh,"pi2lh/I");
  ReducedNtuple->Branch("pi2mctrue",&pi2mctrue,"pi2mctrue/I");
  ReducedNtuple->Branch("pi2p3",&pi2p3,"pi2p3/F");
  ReducedNtuple->Branch("slowpip3",&slowpip3,"slowpip3/F");
  ReducedNtuple->Branch("slowpimctrue",&slowpimctrue,"slowpimctrue/I");


 
  MakeModeReducedNtupleBranches();

  return 1;
}

Int_t Ds1Analysis::MakeModeReducedNtupleBranches(){

  if(_Mode=="D0ToK3Pi"){
    ReducedNtuple->Branch("d0pi2lh",&d0pi2lh,"d0pi2lh/I");
    ReducedNtuple->Branch("d0pi3lh",&d0pi3lh,"d0pi3lh/I");
  }
  return 1;
}


Int_t Ds1Analysis::SetReducedNtupleBranches(){
  
  ReducedNtuple->SetBranchAddress("eventnumber",&eventnumber);
  ReducedNtuple->SetBranchAddress("ds1pmass",&ds1pmass);
  ReducedNtuple->SetBranchAddress("ds1pdeltam1",&ds1pdeltam1);
  ReducedNtuple->SetBranchAddress("ds1pdeltam2",&ds1pdeltam2);
  ReducedNtuple->SetBranchAddress("ds1ppstar",&ds1ppstar);
  ReducedNtuple->SetBranchAddress("ds1pcosstar",&ds1pcosstar);
  ReducedNtuple->SetBranchAddress("Ds1pIdx",&Ds1pIdx);
  ReducedNtuple->SetBranchAddress("ds1pmctrue",&ds1pmctrue);
  ReducedNtuple->SetBranchAddress("ds1pcharge",&ds1pcharge);
  ReducedNtuple->SetBranchAddress("ds1pbestvtxprob",&ds1pbestvtxprob);
  ReducedNtuple->SetBranchAddress("ds1pnDs1p",&ds1pnDs1p);
  ReducedNtuple->SetBranchAddress("ds1pdmres",&ds1pdmres);
  ReducedNtuple->SetBranchAddress("ds1plogvtxprob",&ds1plogvtxprob);
  ReducedNtuple->SetBranchAddress("ds1pvtxstatus",&ds1pvtxstatus);

  ReducedNtuple->SetBranchAddress("dstarmass",&dstarmass);
  ReducedNtuple->SetBranchAddress("dstardeltam",&dstardeltam);
  ReducedNtuple->SetBranchAddress("dstarmctrue",&dstarmctrue);
  ReducedNtuple->SetBranchAddress("dstarvtxprob",&dstarvtxprob);
 
  ReducedNtuple->SetBranchAddress("d0mass",&d0mass);
  ReducedNtuple->SetBranchAddress("d0pstar",&d0pstar);
  ReducedNtuple->SetBranchAddress("d0mctrue",&d0mctrue);
  ReducedNtuple->SetBranchAddress("d0logvtxprob",&d0logvtxprob);
  ReducedNtuple->SetBranchAddress("d0cosine",&d0cosine);
  ReducedNtuple->SetBranchAddress("d0cosine1",&d0cosine1);

  ReducedNtuple->SetBranchAddress("ksmass",&ksmass);
  ReducedNtuple->SetBranchAddress("kscosine",&kscosine);
  ReducedNtuple->SetBranchAddress("kscosine1",&kscosine1);
  ReducedNtuple->SetBranchAddress("kscosine2",&kscosine2);
  ReducedNtuple->SetBranchAddress("ksmctrue",&ksmctrue);
  ReducedNtuple->SetBranchAddress("ksvtxprob",&ksvtxprob);
  ReducedNtuple->SetBranchAddress("ksflightbslen",&ksflightbslen);
  ReducedNtuple->SetBranchAddress("ksflightbscosa",&ksflightbscosa);
  
  ReducedNtuple->SetBranchAddress("klh",&klh);
  ReducedNtuple->SetBranchAddress("kmctrue",&kmctrue);
  ReducedNtuple->SetBranchAddress("kp3",&kp3); 

  ReducedNtuple->SetBranchAddress("pilh",&pilh);
  ReducedNtuple->SetBranchAddress("pimctrue",&pimctrue);
  ReducedNtuple->SetBranchAddress("pip3",&pip3);

  ReducedNtuple->SetBranchAddress("pi1lh",&pi1lh);
  ReducedNtuple->SetBranchAddress("pi1mctrue",&pi1mctrue);
  ReducedNtuple->SetBranchAddress("pi1p3",&pi1p3);

  ReducedNtuple->SetBranchAddress("pi2lh",&pi2lh);
  ReducedNtuple->SetBranchAddress("pi2mctrue",&pi2mctrue);
  ReducedNtuple->SetBranchAddress("pi2p3",&pi2p3);

  ReducedNtuple->SetBranchAddress("slowpip3",&slowpip3);
  ReducedNtuple->SetBranchAddress("slowpimctrue",&slowpimctrue);

  SetModeReducedNtupleBranches();

  return 1;
}

Int_t Ds1Analysis::SetModeReducedNtupleBranches(){
  

  if(_Mode=="D0ToK3Pi"){
    ReducedNtuple->SetBranchAddress("d0pi2lh",&d0pi2lh);
    ReducedNtuple->SetBranchAddress("d0pi3lh",&d0pi3lh);
  
  }

  return 1;
}
