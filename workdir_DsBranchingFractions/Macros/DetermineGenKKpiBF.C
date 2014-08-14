
#define myDsLund 431
#define myDsStLund 433
#define myKLund 321
#define myKS0Lund 310
#define myPiLund 211
#define myPi0Lund 111



void DetermineGenKKpiBF(){
  
  KanEventSource* mc = KanEventSource::miniMC();
  mc->Add("/store/SP/R24/001005/200303offres/24.3.3d/SP_001005_050419");
  mc->Add("/store/SP/R24/001005/200303offres/24.3.3d/SP_001005_050570");
  mc->Add("/store/SP/R24/001005/200303offres/24.3.3d/SP_001005_050637");

  KanHandleVect<GTrackI> mcHandle(KanCompMap::tru,"G4_Tracks");
  mc->setBranchToRead(mcHandle);

  Int_t nDsTotal=0;
  Int_t nKKpi=0;
  Int_t nKKpiFSR=0;
  
  TH1F Hdiff("Hdiff","",200,-.001,.001);


  //event loop
  Int_t e=0;
  //for(e=1;e<100000;e++){//mc->nEvt()
  //for(e=100000;e<200000;e++){//mc->nEvt()
  for(e=1;e<mc->nEvt();e++){//mc->nEvt()
    if(e%10000==0)cout<<e<<" events done"<<endl;
    
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
    TLorentzVector FSRGammap4;

    //loop to find the Ds 
    for(Int_t t=0;t<mcHandle.size();t++){
      GTrackI* mcCand = mcHandle[t];
      if(abs(mcCand->pdtEntry())==myDsLund){
	DsCand=mcCand;
	Dsp4=TLorentzVector(TVector3(DsCand->px(),DsCand->py(),DsCand->pz()),DsCand->e());
      }
    }

    if(DsCand){
      nDsTotal++;

      //find the K+
      for(Int_t t=0;t<mcHandle.size();t++){
	GTrackI* mcCand = mcHandle[t];
	if(mcCand->pdtEntry()==myKLund)
	  if(mcCand->parentTrack()->pdtEntry()==DsCand->pdtEntry()
	     ||mcCand->parentTrack()->parentTrack()->pdtEntry()==DsCand->pdtEntry()){
	    
	    KPlusp4=TLorentzVector(TVector3(mcCand->px(),mcCand->py(),mcCand->pz()),mcCand->e());	    
	    KPlusCand=mcCand;

	    
	    //find the K-
	    for(Int_t t=0;t<mcHandle.size();t++){
	      GTrackI* mcCand = mcHandle[t];
	      if(mcCand->pdtEntry()==-myKLund)
		if(mcCand->parentTrack()->pdtEntry()==DsCand->pdtEntry()
		   ||mcCand->parentTrack()->parentTrack()->pdtEntry()==DsCand->pdtEntry()){
	    
		  KMinusp4=TLorentzVector(TVector3(mcCand->px(),mcCand->py(),mcCand->pz()),mcCand->e());
		  KMinusCand=mcCand;

		  //find the pi
		  for(Int_t t=0;t<mcHandle.size();t++){
		    GTrackI* mcCand = mcHandle[t];
		    if(mcCand->pdtEntry()==(DsCand->pdtEntry()/abs(DsCand->pdtEntry()))*myPiLund)
		      if(mcCand->parentTrack()->pdtEntry()==DsCand->pdtEntry()
			 ||mcCand->parentTrack()->parentTrack()->pdtEntry()==DsCand->pdtEntry()){
			
			Pionp4.SetPxPyPzE(mcCand->px(),mcCand->py(),mcCand->pz(),mcCand->e());
			PiCand=mcCand;
			
			if(fabs((Dsp4-(KPlusp4+KMinusp4+Pionp4)).Mag2())<.0001)
			  nKKpi++;

			//find the FSR gamma
			for(Int_t t=0;t<mcHandle.size();t++){
			  GTrackI* mcCand = mcHandle[t];
			  if(mcCand->pdtEntry()==22)
			    if(mcCand->parentTrack()==KPlusCand
			       ||mcCand->parentTrack()==KMinusCand
			       ||mcCand->parentTrack()==PiCand){
			      FSRGammap4.SetPxPyPzE(mcCand->px(),mcCand->py(),mcCand->pz(),mcCand->e());
			    }
			}

			Hdiff.Fill((Dsp4-(KPlusp4+KMinusp4+Pionp4+FSRGammap4)).Mag2());			
			//KKpi must add up to Ds 4-vector
			if(fabs((Dsp4-(KPlusp4+KMinusp4+Pionp4+FSRGammap4)).Mag2())<.0005)
			  nKKpiFSR++;

			
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
      <<"  BF="<<((float)nKKpi)/nDsTotal
      <<"  nKKpiFSR="<<nKKpiFSR
      <<"  BFFSR="<<((float)nKKpiFSR)/nDsTotal
      <<endl;


  TCanvas C("C");
  C.SetLogy(1);
  Hdiff.Draw();
  C.Print("DetermineGenKKpiBF.ps");

  delete mc;

  //Hdiff->Draw();
}



// void DetermineGenKKpiBF(){

//   KanEventSource* mc = KanEventSource::miniMC();

//   //  mc->Add("CleanTagSkim/SP1005/Run3On/MyDRecoCleanTagSkim_1");
//   mc->Add("/store/SP/R24/001005/200303offres/24.3.3d/SP_001005_050419");
//   //mc->getCnd()->Print();
//   //mc->getTru();

//   //BtaCandHandleBranch TagD(KanCompMap::cnd ,"Cnd_DRecoTagList");
//   BtaCandHandleList Tag(KanCompMap::cnd ,"Cnd_DRecoTagList");
//   mc->setBranchToRead(Tag);

//   //event loop
//   for(Int_t e=1;e<20;e++){//mc->nEvt()
//     mc->LoadTree(e);
//     Tag(*mc);

//     //loop over the list of Ds cands
//     for(Int_t t=0;t<Tag.size();t++){
//       BtaCandIdI* tagCand = Tag.cand(t);
//       cout<<tagCand->lundID()<<endl;
      
//       if(abs(tagCand->lundID())==myDsLund){//loop over the Ds daus
// 	for(Int_t d=0;d<tagCand->nDau();d++){
// 	  BtaCandIdI*dauCand=tagCand->dau(d);
// 	  cout<<" dau "<<d<<"  lundId "<<dauCand->lundID()<<endl;
// 	}

//       }
//     }
//   }


//   delete mc;

// }

