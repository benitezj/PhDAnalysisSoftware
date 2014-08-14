  ReducedNtuple->SetBranchStatus("*",0);
  ReducedNtuple->SetBranchStatus("dsmass",1);ReducedNtuple->SetBranchAddress("dsmass",&dsmass);
  ReducedNtuple->SetBranchStatus("goodeventid",1);ReducedNtuple->SetBranchAddress("goodeventid",&goodeventid);


  TH1F HDsMass("HDsMass","",NDsMassBins,DsMassMin,DsMassMax);
  HDsMass.Sumw2();
  
  Float_t priordsmass[NMAXDSCANDS];
  Int_t nDsCands=0;
  Int_t prioreventid=1;
  goodeventid=0;
  eventid=0;
  while(ReducedNtuple->GetEntry(eventid++,0)){

    if(goodeventid!=prioreventid){//Fill histogram with masses from previous event
      for(Int_t i=0;i<nDsCands;i++){
	HDsMass.Fill(priordsmass[i],1./nDsCands);
      }
      
      nDsCands=0;
      prioreventid=goodeventid;
    }
    
    //collect the masses from all Ds cands in this event    
    priordsmass[nDsCands]=dsmass;
    nDsCands++;       
  }
  
