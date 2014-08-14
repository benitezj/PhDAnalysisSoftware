


#define myDsLund 431
#define myDsStLund 433
#define myKLund 321
#define myKS0Lund 310
#define myPiLund 211
#define myPi0Lund 111


////to Run this macro you need to use Kanga in proper release instead of root 
void KKpiMCTestKanga(){
  
  KanEventSource* mc = KanEventSource::miniMC();
  //pointer skims dont work
  //for(Int_t i=1;i<=1;i++)//134
  //mc->Add(TString("CleanTagSkim/SP9775/Run5On/MyDRecoCleanTagSkim_")+(long)i);
  
  mc->Add("/store/SP/R24/009775/run3/24.3.5d/SP_009775_122188");
  mc->Add("/store/SP/R24/009775/run3/24.3.5d/SP_009775_122197");
  mc->Add("/store/SP/R24/009775/run3/24.3.5d/SP_009775_122215");
  //mc->Add("");
  //mc->Add("");


  KanHandleVect<GTrackI> mcHandle(KanCompMap::tru,"G4_Tracks");
  mc->setBranchToRead(mcHandle);

  Int_t nDsTotal=0;
  Int_t nKKpi=0;
  
  TH1F*HKpiMass=new TH1F("HKpiMass","",200,.2,2.2);

  //event loop
  Int_t e=0;
  //for(e=1;e<100000;e++){//mc->nEvt()
  //for(e=100000;e<200000;e++){//mc->nEvt()
  for(e=1;e<mc->nEvt();e++){//mc->nEvt()
    if(e%1000==0)cout<<e<<" events done"<<endl;
    
    mc->LoadTree(e);
    mcHandle(*mc);

    GTrackI* DsCand=NULL;
    GTrackI* KPlusCand=NULL;
    GTrackI* KMinusCand=NULL;
    GTrackI* PiCand=NULL;
    
    TLorentzVector Dsp4;
    TLorentzVector KPlusp4;
    TLorentzVector KMinusp4;
    TLorentzVector Pionp4;

    //loop to find the Ds 
    for(Int_t t=0;t<mcHandle.size();t++){
      GTrackI* mcCand = mcHandle[t];
      //cout<<mcCand->pdtEntry()<<" ";
      if(abs(mcCand->pdtEntry())==431){
	DsCand=mcCand;
	Dsp4=TLorentzVector(TVector3(DsCand->px(),DsCand->py(),DsCand->pz()),DsCand->e());
      }
    }
    //cout<<endl;

    if(DsCand){
      nDsTotal++;

      //find the K+
      for(Int_t t=0;t<mcHandle.size();t++){
	GTrackI* mcCand = mcHandle[t];
	if(mcCand->pdtEntry()==myKLund&&mcCand->parentTrack()->pdtEntry()==DsCand->pdtEntry()){
	    
	  KPlusp4=TLorentzVector(TVector3(mcCand->px(),mcCand->py(),mcCand->pz()),mcCand->e());	    
	  KPlusCand=mcCand;
	    
	  //find the K-
	  for(Int_t t=0;t<mcHandle.size();t++){
	    GTrackI* mcCand = mcHandle[t];
	    if(mcCand->pdtEntry()==-myKLund&&mcCand->parentTrack()->pdtEntry()==DsCand->pdtEntry()){
	    
	      KMinusp4=TLorentzVector(TVector3(mcCand->px(),mcCand->py(),mcCand->pz()),mcCand->e());
	      KMinusCand=mcCand;

	      //find the pi
	      for(Int_t t=0;t<mcHandle.size();t++){
		GTrackI* mcCand = mcHandle[t];
		if(mcCand->pdtEntry()==(DsCand->pdtEntry()/abs(DsCand->pdtEntry()))*myPiLund
		   &&mcCand->parentTrack()->pdtEntry()==DsCand->pdtEntry()){
		      
		  Pionp4.SetPxPyPzE(mcCand->px(),mcCand->py(),mcCand->pz(),mcCand->e());
		  PiCand=mcCand;

		  nKKpi++;
		  
		  HKpiMass->Fill((PiCand->pdtEntry()*KMinusCand->pdtEntry()<0) ? (KMinusp4+Pionp4).Mag2() : (KPlusp4+Pionp4).Mag2());			
			
			
		}	
	      }
		  
	    }
	  }
       	    
	}	
      }
      
      
      
    }
    
  }
   
  cout<<"nEvt="<<e
      <<"  nDs="<<nDsTotal
      <<"  nKKpi="<<nKKpi
      <<endl;


  TCanvas*C=new TCanvas("C");
  HKpiMass->Draw();
  C->Print("HKpiMassKanga.ps");

  delete mc;

  //Hdiff->Draw();
}

