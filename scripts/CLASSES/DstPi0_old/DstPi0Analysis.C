#include "DstPi0Analysis.h"

/////////
// #include "DstPi0AnalysisFitMassPeak.C"
// #include "DstPi0AnalysisRooFitMassPeak.C"
// #include "DstPi0AnalysisRooFitWS.C"
// #include "DstPi0AnalysisResolutionEff.C"
// #include "DstPi0AnalysisK3piDstarBkg.C"
#include "DstPi0AnalysisPlotDMVsPstarCosHel.C"
// #include "DstPi0AnalysisRooFitDMLowCosHel.C"

ClassImp(DstPi0Analysis);
DstPi0Analysis::DstPi0Analysis(TChain* fChain,Bool_t TruthMatch,Int_t MatterOrAntiMatter,Int_t DstPiLund,TString OutputDir,Int_t D0Mode):
  _fChain(fChain),
  _TruthMatch(TruthMatch),
  _MatterOrAntiMatter(MatterOrAntiMatter),							   
  _MyDstPiLund(DstPiLund),
  _OutputDir(OutputDir), 
  _D0Mode(D0Mode),
  OutPutFile(NULL),
  ReducedNtuple(NULL),
  MCXCounterPerEvent(0),
  MCXCounterTotal(0),
  RecoXCounterTotal(0)
{
  if(_fChain==NULL){cout<<"No Chain"<<endl;}
   
  //init the database 
  database.SetTruthMatch(_TruthMatch);
  database.SetD0Mode(_D0Mode);
   
  //the phase space and q factor for the threshold
  phasefactor="((1/(2*(dstpideltam+2.01)**2))*sqrt(((dstpideltam+2.01)*(dstpideltam+2.01) -  4.601)*((dstpideltam+2.01)*(dstpideltam+2.01)- 3.515)))";
  qfactor="((1/(2*(dstpideltam+2.01)))*sqrt(((dstpideltam+2.01)*(dstpideltam+2.01) - 4.601)*((dstpideltam+2.01)*(dstpideltam+2.01)-3.515)))";
  
  ///labels
  DstarPiXLabel="m(D*^{+}#pi^{0})  -  m(D*^{+})   (GeV/c^{2}) ";
  DstarPiYLabel="Entries/5 MeV"; 

  cout<<"Done Initializing the analysis"<<endl;
}

DstPi0Analysis::~DstPi0Analysis(){  
  
}


Int_t DstPi0Analysis::ReduceNtuple(Bool_t RunInTestMode){
  cout<<"Going to reduce the ntuple"<<endl;

  if(_fChain==NULL){
    cout<<"No chain."<<endl;
    return 0;
  } 


  ///link the branches in the input chain
  SetBranches();


  //the file must be created here so the 'new' histos and Tree is created in hard drive 
  OutPutFile=new TFile(_OutputDir+"/Reduced.root","recreate",_OutputDir+"/Reduced.root",4);
  if(OutPutFile==NULL){cout<<"Failed to create file"<<endl;return 0;}


  //create new reduced ntuple
  ReducedNtuple=new TTree("Ntuple","Reduced Ntuple",99);        
  ReducedNtuple->SetMaxTreeSize(9900000000);

  //create the branches in the ReducedNtuple
  MakeReducedNtupleBranches();


  if(_TruthMatch){
    //create the MC Histos
    cout<<"Going to init MC histograms"<<endl;
    HMCXMass=new TH1F();
    SetHistoXY(HMCXMass,"HMCXMass",database.GetDstPi0MassNbins(),database.GetDstPi0MassMin(),database.GetDstPi0MassMax(),"X Cand. Mass (GeV/c^{2})","");
  
    HMCXDeltaM=new TH1F();
    SetHistoXY(HMCXDeltaM,"HMCXDeltaM",database.GetDstPi0DeltaMNbins(),database.GetDstPi0DeltaMMin(),database.GetDstPi0DeltaMMax(),"D*(2640) Mass - D*(2010) (GeV/c^{2})","");


    HMCXp3CM=new TH1F();
    SetHistoXY(HMCXp3CM,"HMCXp3CM",50,0,5,
	       "p* (GeV/c)","Entries/100MeV");
  
    HMCXcosthCM=new TH1F();
    SetHistoXY(HMCXcosthCM,"HMCXcosthCM",20,-1.0001,1.0001,
	       "cos(#theta)*","");
  
    H2MCXCMPvsTheta=new TH2F();
    SetHisto2D(H2MCXCMPvsTheta,"H2MCXCMPvsTheta",50,0,5,
	       "p* (GeV/c)",20,-1.0001,1.0001,"cos(#theta*)","");  

    HMCNX=new TH1F();
    SetHistoXY(HMCNX,"HMCNX",11,-.5,10.5,
	       "nX/event","Counts");
  }
  //////


  ///check how many candidates are removed by array size cut
  Int_t RemovedCands=0;
  Int_t InitialdCands=0;

  //Start the event loop;
  Long_t MaxNumberEvents=1000000000;
  if(RunInTestMode)MaxNumberEvents=100000;

  cout<<"Going to start the event loop"<<endl;
  Int_t eventid=0;
  while(_fChain->GetEntry(eventid,0)>0&&eventid<MaxNumberEvents){   
    eventid++;
     
    if(eventid%50000==0)cout<<eventid<<" Events done.   With "<<RecoXCounterTotal<<" Candidates saved."<<endl;    
      
    eventnumber=eventid;
     
    InitialdCands+=nX;
    if(nX>MAXNCANDS){
      cout<<"Too many cands at event "<<eventid<<" Only first "<<MAXNCANDS<<" of "<<nX<<" will be used"<<endl;
      RemovedCands+=nX-MAXNCANDS; 
      nX=MAXNCANDS;      
    }

    ///Loop over the reconstructed
    RecoXCounterPerEvent=0;      
    
    //------beam quantities
    TVector3 beamVtx(primVtxX,primVtxY,primVtxZ);


    for(XIdx=0;XIdx<nX;XIdx++){

      ////////////cut out the unconverged fits
      if(XVtxStatus[XIdx]!=0)continue;               


      ///determine the indexes for each particle
      DstarIdx=Xd1Idx[XIdx];
      Pi0Idx=Xd2Idx[XIdx];
      Gam1Idx=Pi0d1Idx[Pi0Idx];
      Gam2Idx=Pi0d2Idx[Pi0Idx];
      SlowPiIdx=Dstard2Idx[DstarIdx];
      D0Idx=Dstard1Idx[DstarIdx];
      KIdx=D0d1Idx[D0Idx];
      PiIdx=D0d2Idx[D0Idx];
      D0Pi2Idx=D0d3Idx[D0Idx];
      D0Pi3Idx=D0d4Idx[D0Idx];
          
      PitrkIdx=PiTrkIdx[PiIdx];
      KtrkIdx=KTrkIdx[KIdx];
      SlowPitrkIdx=PiTrkIdx[SlowPiIdx];
      D0Pi2trkIdx=PiTrkIdx[D0Pi2Idx];
      D0Pi3trkIdx=PiTrkIdx[D0Pi3Idx];
      
     
      //////fill reduced ntuple variables    
      dstpimass=XMass[XIdx];      
      dstpipstar=Xp3CM[XIdx];
      dstpicosstar=XcosthCM[XIdx];
      dstpideltam=XMass[XIdx]-DstarMass[DstarIdx];  
      if(TMath::Prob(XChi2[XIdx],XnDof[XIdx])>0)
	dstpilogvtxprob=log10(TMath::Prob(XChi2[XIdx],XnDof[XIdx]));
      else dstpilogvtxprob=-100;
      //Determine D*pi combination: dstpicharge= -2=D*-pi- , -1=D*-pi+  , 1=D*+pi- , 2=D*+pi+
      dstpicharge=DstarLund[DstarIdx]/abs(DstarLund[DstarIdx]);
      if(dstpicharge==0)dstpicharge=DstarLund[DstarIdx]/abs(DstarLund[DstarIdx]);
      dstpinX=nX;  
      dstpiIdx=XIdx;

      //-------Dstar quantities
      dstarmass=DstarMass[DstarIdx];
      dstardeltam=DstarMass[DstarIdx]-D0Mass[D0Idx];
      if(DstarVtxStatus[DstarIdx]==0) dstarlogvtxprob=log10(TMath::Prob(DstarChi2[DstarIdx],DstarnDof[DstarIdx]));   
      else dstarlogvtxprob=0;
      dstarcostheta=ComputeDstarAngle();
      dstarpstar=Dstarp3CM[DstarIdx];
      dstarcharge=DstarLund[DstarIdx]/abs(DstarLund[DstarIdx]);

      //--Pi0 
      pi0mass=Pi0Mass[Pi0Idx];
      pi0pstar=Pi0p3CM[Pi0Idx];
      pi0energy=sqrt(pi0mass*pi0mass+Pi0p3[Pi0Idx]*Pi0p3[Pi0Idx]);
      pi0cosine=ComputePiDstarAngle();
      pi0costheta=ComputePiAngle();
      pi0helicity=ComputePiHelicityAngle();

      //--Gamma Quantities
      gam1energy=Gamp3[Gam1Idx];
      gam1cos=Gamcosth[Gam1Idx];
      gam2energy=Gamp3[Gam2Idx];
      gam2cos=Gamcosth[Gam2Idx];
  


      //-------D0 quantities
      d0mass=D0Mass[D0Idx];
      if(TMath::Prob(D0Chi2[D0Idx],D0nDof[D0Idx])>0) d0logvtxprob = log10(TMath::Prob(D0Chi2[D0Idx],D0nDof[D0Idx]));
      else d0logvtxprob=0;
      d0pstar=D0p3CM[D0Idx];
      d0mode=0;
      if(D0Pi2Idx==-1&&D0Pi3Idx==-1) d0mode=1;
      if(D0Pi2Idx>=0&&D0Pi3Idx>=0) d0mode=2;
      if(d0mode==0)cout<<"Error d0mode=0"<<endl;
      d0charge=DstarLund[DstarIdx]/abs(DstarLund[DstarIdx]);
      //d0costheta=ComputeD0Angle();

     
      //track quantities
      kp3=Kp3[KIdx];
      kcharge=KLund[KIdx]/abs(KLund[KIdx]);
      //kcostheta=ComputeKAngle();

      pip3=Pip3[PiIdx];
      picharge=PiLund[PiIdx]/abs(PiLund[PiIdx]);

      d0pi2p3=0;d0pi3p3=0;d0pi2charge=0;d0pi3charge=0;
      if(d0mode==2){
	d0pi2p3=Pip3[D0Pi2Idx];      
	d0pi2charge=PiLund[D0Pi2Idx]/abs(PiLund[D0Pi2Idx]);      
	d0pi3p3=Pip3[D0Pi3Idx];      
	d0pi3charge=PiLund[D0Pi3Idx]/abs(PiLund[D0Pi3Idx]);		
      }
          	      
     
      slowpip3=Pip3[SlowPiIdx];
      slowpicharge=PiLund[SlowPiIdx]/abs(PiLund[SlowPiIdx]);

      //---------Selectors
      klh=0;
      for(Int_t i=10;i<=14;i++){//10=notpion,11=veryloose,12=loose,13=tight,14=verytight
	if((KSelectorsMap[KtrkIdx] & (1<<i)) == (1<<i))
	  klh=i;

      }
            
      pilh=0;d0pi2lh=0;d0pi3lh=0;
      for(Int_t i=2;i<=5;i++){//2=veryloose,3=loose,4=tight,5=verytight
	if((PiSelectorsMap[PitrkIdx] & (1<<i) ) == (1<<i))
	  pilh=i;
	
	if(d0mode==2){
	  if((PiSelectorsMap[D0Pi2trkIdx] & (1<<i) ) == (1<<i))
	    d0pi2lh=i;
	  if((PiSelectorsMap[D0Pi3trkIdx] & (1<<i) ) == (1<<i))
	    d0pi3lh=i;
	}
      }
      
      
      //fore MC determine if candidate was truthmatched
      dstpimctrue=1;
      dstarmctrue=1;
      d0mctrue=1;
      pimctrue=1;
      slowpimctrue=1;
      kmctrue=1;
      pi0mctrue=1;
      d0pi2mctrue=1;  
      d0pi3mctrue=1;  
      dstpimclund=0;
      dstpidmres=100000;
      if(_TruthMatch){
	dstpimctrue=(XMCIdx[XIdx]>0);
	dstpimclund=mcLund[XMCIdx[XIdx]];
	dstarmctrue=(DstarMCIdx[DstarIdx]>0);
	d0mctrue=(D0MCIdx[D0Idx]>0);
	pimctrue=(PiMCIdx[PiIdx]>0);
	slowpimctrue=(PiMCIdx[SlowPiIdx]>0);
	kmctrue=(KMCIdx[KIdx]>0);
	pi0mctrue=(Pi0MCIdx[Pi0Idx]>0);
	if(d0mode==2){
	  d0pi2mctrue=(PiMCIdx[D0Pi2Idx]>0);
	  d0pi3mctrue=(PiMCIdx[D0Pi3Idx]>0);
	}
	if(dstpimctrue){      
	  dstpidmres=dstpideltam-(mcmass[XMCIdx[XIdx]]-mcmass[DstarMCIdx[DstarIdx]]);
	}

      }

                
      //apply some simple cuts to minimize file size
      if(dstardeltam>.180||d0mass<database.GetD0MassMin()||database.GetD0MassMax()<d0mass||d0mode!=_D0Mode) continue;
     
      ReducedNtuple->Fill();	              
               
      RecoXCounterTotal++;
           
   
    }//X loop    
      
    ///-------------------------------
    ///Fill the Generated Quantities
    ///-----------------------------
    if(_TruthMatch){    
      MCXCounterPerEvent=0; 
      Int_t mcid=-1;
      Bool_t goodcand;
      while(mcid<mcLen){
	mcid++;
       
	if(mcid>=MAXNCANDS||dauIdx[mcid]>=MAXNCANDS){cout<<"MC idx larger than array size."<<endl;continue;}

	goodcand=false;
	if(abs(_MatterOrAntiMatter)==1&&mcLund[mcid]==_MatterOrAntiMatter*_MyDstPiLund)goodcand=true;
	if(_MatterOrAntiMatter==0&&abs(mcLund[mcid])==_MyDstPiLund)goodcand=true;
	if(goodcand&&abs(mcLund[dauIdx[mcid]])!=MYDSTARLUND)goodcand=false;//D*2460 also decays to D0

	if(goodcand){
	  MCXCounterPerEvent++; 
	  MCXCounterTotal++;
	 
	  HMCXMass->Fill(mcmass[mcid]);
	  HMCXp3CM->Fill(mcp3CM[mcid]); 
	  HMCXcosthCM->Fill(mccosthCM[mcid]); 	
	  H2MCXCMPvsTheta->Fill(mcp3CM[mcid],mccosthCM[mcid]);

	  //calculate deltaM :  mcmass[dauIdx[mcid]]=first daughter; mcmass[dauIdx[mcid] + 1] = second daughter ...	    
	  HMCXDeltaM->Fill(mcmass[mcid]-mcmass[dauIdx[mcid]]);
	}
	
	//CalculateDstRandPiGenMassDist(mcid);      
      }
      HMCNX->Fill(MCXCounterPerEvent);
    }

  }

  //print summary
  cout<<"--------Summary-------"<<endl;
  cout<<"Total events="<<eventid<<endl;
  cout<<"Total Generated="<<MCXCounterTotal<<" Reconstructed="<<RecoXCounterTotal<<endl;
  cout<<"Total candidates removed "<<RemovedCands<<" ( "<<(int)(100*RemovedCands/InitialdCands + .5)<<"%)"<<endl;
  cout<<"--------End Summary---"<<endl;

  ////Save the Ntuple and histograms

  cout<<"Going to save the ntuple and histograms"<<endl; 

  ///////////Should not have to call TTree->Write(), TFile->Write() will doit
  if(ReducedNtuple->Write()){ cout<<"Ntuple written."<<endl;}
  else{ cout<<"Failed to write ntuple"<<endl;return 0;}
  delete  ReducedNtuple;   

  if(_TruthMatch){
    
    if(HMCXMass->Write()&&
       HMCXDeltaM->Write()&&
       HMCXp3CM->Write()&&
       HMCXcosthCM->Write()&&     
       H2MCXCMPvsTheta->Write()&&     
       HMCNX->Write()
       ){ 
      cout<<" Histograms have been written"<<endl;
    }
    else{
      cout<<"Failed to write Histos"<<endl;return 0;
    }
  
  }
  
  
  OutPutFile->ls();//turns out this is quite important for the objects to be written.
  OutPutFile->Close(); //this will delete all objects created inside the file if they were not written
  cout<<OutPutFile->GetName()<<" has been closed."<<endl;

  delete OutPutFile;

  return 1;
}


Int_t DstPi0Analysis::OpenReducedNtuple(TString filename){
  
  cout<<"Going to open reduced ntuple."<<endl;


  if(!(OutPutFile=new TFile(_OutputDir+"/"+filename,"read"))){
    cout<<"Unable to open reduced file"<<endl;
    return 0;
  }
  if(OutPutFile->IsZombie()){
    cout<<"Zombie reduced ntuple file"<<endl;return 0;
  }
  cout<<"File opened: "<<OutPutFile<<" "<<OutPutFile->GetName()<<endl;


  if(!(ReducedNtuple=(TTree*)OutPutFile->Get("Ntuple"))){
    cout<<"No reduced ntuple"<<endl;return 0;
  }
  

  
  //ReducedNtuple->SetDirectory(0);
  cout<<"Ntuple Extracted: "<<ReducedNtuple<<" "<<ReducedNtuple->GetName()<<"  With "<<ReducedNtuple->GetEntries()<<" Entries"<<endl;

  SetReducedNtupleBranches();
  
  gROOT->cd();//******REMEMBER TO DO THIS WHEN OPENING A FILE otherwise new objects are created in that file******//

  return 1;
}

Int_t DstPi0Analysis::CloseReducedNtuple(){
  cout<<"Going to close the file."<<endl;
 
  OutPutFile->Close();

  delete OutPutFile;
  
  return 1;
}


Float_t DstPi0Analysis::ComputeDstarAngle(){
  TVector3 Xp3vec(Xp3CM[XIdx]*sin(acos(XcosthCM[XIdx]))*cos(XphiCM[XIdx]),
		      Xp3CM[XIdx]*sin(acos(XcosthCM[XIdx]))*sin(XphiCM[XIdx]),
		      Xp3CM[XIdx]*XcosthCM[XIdx]);
  
  Float_t XEnergy=sqrt(Xp3vec*Xp3vec + XMass[XIdx]*XMass[XIdx]);
  TVector3 XVelvec=(1/XEnergy)*Xp3vec;

  TVector3 Dstarp3vec(Dstarp3CM[DstarIdx]*sin(acos(DstarcosthCM[DstarIdx]))*cos(DstarphiCM[DstarIdx]),
		      Dstarp3CM[DstarIdx]*sin(acos(DstarcosthCM[DstarIdx]))*sin(DstarphiCM[DstarIdx]),
		      Dstarp3CM[DstarIdx]*DstarcosthCM[DstarIdx]); 
  
  TLorentzVector Dstarp4(Dstarp3vec,sqrt(Dstarp3vec*Dstarp3vec+DstarMass[DstarIdx]*DstarMass[DstarIdx]));

  //boost the dstar momentum into rest frame of X
  Dstarp4.Boost(-XVelvec);//watch the minus sign
  TVector3 DstarBoostedp3vec=Dstarp4.Vect();

  //dot product of boosted dstar direction with X direction in CM frame
  Float_t cosine=DstarBoostedp3vec.Unit()*XVelvec.Unit();
  
  return cosine;
  
}

Float_t DstPi0Analysis::ComputePiAngle(){
  TVector3 Xp3vec(Xp3CM[XIdx]*sin(acos(XcosthCM[XIdx]))*cos(XphiCM[XIdx]),
		  Xp3CM[XIdx]*sin(acos(XcosthCM[XIdx]))*sin(XphiCM[XIdx]),
		  Xp3CM[XIdx]*XcosthCM[XIdx]);
  
  Float_t XEnergy=sqrt(Xp3vec*Xp3vec + XMass[XIdx]*XMass[XIdx]);
  TVector3 XVelvec=(1/XEnergy)*Xp3vec;

  TVector3 Pi0p3vec(Pi0p3CM[Pi0Idx]*sin(acos(Pi0costhCM[Pi0Idx]))*cos(Pi0phiCM[Pi0Idx]),
		    Pi0p3CM[Pi0Idx]*sin(acos(Pi0costhCM[Pi0Idx]))*sin(Pi0phiCM[Pi0Idx]),
		    Pi0p3CM[Pi0Idx]*Pi0costhCM[Pi0Idx]); 
  
  TLorentzVector Pi0p4(Pi0p3vec,sqrt(Pi0p3vec*Pi0p3vec+Pi0PDGMass*Pi0PDGMass));
  
  //boost the dstar momentum into rest frame of X
  Pi0p4.Boost(-XVelvec);//watch the minus sign
  TVector3 Pi0Boostedp3vec=Pi0p4.Vect();

  //dot product of boosted dstar direction with X direction 
  return Pi0Boostedp3vec.Unit()*Xp3vec.Unit();
  
}

Float_t DstPi0Analysis::ComputePiHelicityAngle(){
  TVector3 Dstarp3vec(Dstarp3CM[DstarIdx]*sin(acos(DstarcosthCM[DstarIdx]))*cos(DstarphiCM[DstarIdx]),
		  Dstarp3CM[DstarIdx]*sin(acos(DstarcosthCM[DstarIdx]))*sin(DstarphiCM[DstarIdx]),
		  Dstarp3CM[DstarIdx]*DstarcosthCM[DstarIdx]);
  
  Float_t DstarEnergy=sqrt(Dstarp3vec*Dstarp3vec + DstarMass[DstarIdx]*DstarMass[DstarIdx]);
  TVector3 DstarVelvec=(1/DstarEnergy)*Dstarp3vec;

  TVector3 Pi0p3vec(Pi0p3CM[Pi0Idx]*sin(acos(Pi0costhCM[Pi0Idx]))*cos(Pi0phiCM[Pi0Idx]),
		    Pi0p3CM[Pi0Idx]*sin(acos(Pi0costhCM[Pi0Idx]))*sin(Pi0phiCM[Pi0Idx]),
		    Pi0p3CM[Pi0Idx]*Pi0costhCM[Pi0Idx]); 
  
  TLorentzVector Pi0p4(Pi0p3vec,sqrt(Pi0p3vec*Pi0p3vec+Pi0PDGMass*Pi0PDGMass));
  
  //boost the dstar momentum into rest frame of Dstar
  Pi0p4.Boost(-DstarVelvec);//watch the minus sign
  TVector3 Pi0Boostedp3vec=Pi0p4.Vect();

  //dot product of boosted dstar direction with Dstar direction 
  return Pi0Boostedp3vec.Unit()*Dstarp3vec.Unit();
  
}


Float_t DstPi0Analysis::ComputePiDstarAngle(){
  
  TVector3 Dstarp3vec(Dstarp3CM[DstarIdx]*sin(acos(DstarcosthCM[DstarIdx]))*cos(DstarphiCM[DstarIdx]),
		      Dstarp3CM[DstarIdx]*sin(acos(DstarcosthCM[DstarIdx]))*sin(DstarphiCM[DstarIdx]),
		      Dstarp3CM[DstarIdx]*DstarcosthCM[DstarIdx]); 

  TVector3 Pi0p3vec(Pi0p3CM[Pi0Idx]*sin(acos(Pi0costhCM[Pi0Idx]))*cos(Pi0phiCM[Pi0Idx]),
		   Pi0p3CM[Pi0Idx]*sin(acos(Pi0costhCM[Pi0Idx]))*sin(Pi0phiCM[Pi0Idx]),
		   Pi0p3CM[Pi0Idx]*Pi0costhCM[Pi0Idx]); 
  
  return Dstarp3vec.Unit()*Pi0p3vec.Unit();
  
}


Int_t DstPi0Analysis::SetBranches(){
  if(_fChain==NULL){cout<<"No Chain"<<endl;return 0;} 
  cout<<"Setting Chain Branch Addresses"<<endl;

  _fChain->SetBranchAddress("nX",&nX);
  _fChain->SetBranchAddress("XMass",XMass);
  _fChain->SetBranchAddress("Xp3",Xp3);
  _fChain->SetBranchAddress("Xcosth",Xcosth);
  _fChain->SetBranchAddress("Xphi",Xphi);
  _fChain->SetBranchAddress("Xp3CM",Xp3CM);
  _fChain->SetBranchAddress("XcosthCM",XcosthCM);
  _fChain->SetBranchAddress("XphiCM",XphiCM);
  _fChain->SetBranchAddress("XLund",XLund); 
  _fChain->SetBranchAddress("Xd1Lund",Xd1Lund);
  _fChain->SetBranchAddress("Xd1Idx",Xd1Idx);
  _fChain->SetBranchAddress("Xd2Lund",Xd2Lund);
  _fChain->SetBranchAddress("Xd2Idx",Xd2Idx);
  _fChain->SetBranchAddress("XVtxx",XVtxx);
  _fChain->SetBranchAddress("XVtxy",XVtxy);
  _fChain->SetBranchAddress("XVtxz",XVtxz);
  _fChain->SetBranchAddress("XChi2",XChi2);
  _fChain->SetBranchAddress("XnDof",XnDof);
  _fChain->SetBranchAddress("XVtxStatus",XVtxStatus);
 
  _fChain->SetBranchAddress("nDstar",&nDstar);
  _fChain->SetBranchAddress("DstarMass",DstarMass);
  _fChain->SetBranchAddress("Dstarp3",Dstarp3);
  _fChain->SetBranchAddress("Dstarcosth",Dstarcosth);
  _fChain->SetBranchAddress("Dstarphi",Dstarphi);
  _fChain->SetBranchAddress("Dstarp3CM",Dstarp3CM);
  _fChain->SetBranchAddress("DstarcosthCM",DstarcosthCM);
  _fChain->SetBranchAddress("DstarphiCM",DstarphiCM);
  _fChain->SetBranchAddress("DstarLund",DstarLund);
  _fChain->SetBranchAddress("Dstard1Lund",Dstard1Lund);
  _fChain->SetBranchAddress("Dstard1Idx",Dstard1Idx);
  _fChain->SetBranchAddress("Dstard2Lund",Dstard2Lund);
  _fChain->SetBranchAddress("Dstard2Idx",Dstard2Idx);
  _fChain->SetBranchAddress("DstarChi2",DstarChi2);
  _fChain->SetBranchAddress("DstarnDof",DstarnDof);
  _fChain->SetBranchAddress("DstarVtxStatus",DstarVtxStatus);

  _fChain->SetBranchAddress("nPi0",&nPi0);
  _fChain->SetBranchAddress("Pi0Mass",Pi0Mass);
  _fChain->SetBranchAddress("Pi0p3",Pi0p3);
  _fChain->SetBranchAddress("Pi0costh",Pi0costh);
  _fChain->SetBranchAddress("Pi0phi",Pi0phi);
  _fChain->SetBranchAddress("Pi0p3CM",Pi0p3CM);
  _fChain->SetBranchAddress("Pi0costhCM",Pi0costhCM);
  _fChain->SetBranchAddress("Pi0phiCM",Pi0phiCM);
  _fChain->SetBranchAddress("Pi0Lund",Pi0Lund);
  _fChain->SetBranchAddress("Pi0d1Lund",Pi0d1Lund);
  _fChain->SetBranchAddress("Pi0d1Idx",Pi0d1Idx);
  _fChain->SetBranchAddress("Pi0d2Lund",Pi0d2Lund);
  _fChain->SetBranchAddress("Pi0d2Idx",Pi0d2Idx);
  _fChain->SetBranchAddress("Pi0Chi2",Pi0Chi2);
  _fChain->SetBranchAddress("Pi0nDof",Pi0nDof);
  _fChain->SetBranchAddress("Pi0VtxStatus",Pi0VtxStatus);

  _fChain->SetBranchAddress("nGam",&nGam);
  _fChain->SetBranchAddress("Gamp3",Gamp3);
  _fChain->SetBranchAddress("Gamcosth",Gamcosth);
  _fChain->SetBranchAddress("Gamphi",Gamphi);
  _fChain->SetBranchAddress("Gamp3CM",Gamp3CM);
  _fChain->SetBranchAddress("GamcosthCM",GamcosthCM);
  _fChain->SetBranchAddress("GamphiCM",GamphiCM);
  _fChain->SetBranchAddress("GamLund",GamLund);

  _fChain->SetBranchAddress("nD0",&nD0);
  _fChain->SetBranchAddress("D0Mass",D0Mass);
  _fChain->SetBranchAddress("D0p3CM",D0p3CM); 
  _fChain->SetBranchAddress("D0costhCM",D0costhCM); 
  _fChain->SetBranchAddress("D0phiCM",D0phiCM);  
  _fChain->SetBranchAddress("D0Lund",D0Lund);
  _fChain->SetBranchAddress("D0d1Lund",D0d1Lund);
  _fChain->SetBranchAddress("D0d1Idx",D0d1Idx);
  _fChain->SetBranchAddress("D0d2Lund",D0d2Lund);
  _fChain->SetBranchAddress("D0d2Idx",D0d2Idx);
  _fChain->SetBranchAddress("D0Chi2",D0Chi2);
  _fChain->SetBranchAddress("D0nDof",D0nDof);
  _fChain->SetBranchAddress("D0VtxStatus",D0VtxStatus);
  //_fChain->SetBranchAddress("D0nDaus",D0nDaus);
  _fChain->SetBranchAddress("D0d3Lund",D0d3Lund);
  _fChain->SetBranchAddress("D0d3Idx",D0d3Idx);
  _fChain->SetBranchAddress("D0d4Lund",D0d4Lund);
  _fChain->SetBranchAddress("D0d4Idx",D0d4Idx);
  
  _fChain->SetBranchAddress("nPi",&nPi);
  _fChain->SetBranchAddress("Pip3",Pip3);
  _fChain->SetBranchAddress("Picosth",Picosth);
  _fChain->SetBranchAddress("Piphi",Piphi);
  _fChain->SetBranchAddress("Pip3CM",Pip3CM);
  _fChain->SetBranchAddress("PicosthCM",PicosthCM);
  _fChain->SetBranchAddress("PiphiCM",PiphiCM);
  _fChain->SetBranchAddress("PiLund",PiLund);
  _fChain->SetBranchAddress("PiTrkIdx",PiTrkIdx);

  _fChain->SetBranchAddress("nK",&nK);
  _fChain->SetBranchAddress("Kp3",Kp3);
  _fChain->SetBranchAddress("Kp3CM",Kp3CM);
  _fChain->SetBranchAddress("KcosthCM",KcosthCM);
  _fChain->SetBranchAddress("KphiCM",KphiCM);
  _fChain->SetBranchAddress("KLund",KLund);
  _fChain->SetBranchAddress("KTrkIdx",KTrkIdx);

  _fChain->SetBranchAddress("TRKnSvt",TRKnSvt);
  _fChain->SetBranchAddress("TRKLund",TRKLund); 
  _fChain->SetBranchAddress("piSelectorsMap",PiSelectorsMap);
  _fChain->SetBranchAddress("KSelectorsMap",KSelectorsMap);


  if(_TruthMatch){
    _fChain->SetBranchAddress("XMCIdx",XMCIdx);
    _fChain->SetBranchAddress("DstarMCIdx",DstarMCIdx);
    _fChain->SetBranchAddress("D0MCIdx",D0MCIdx);
    _fChain->SetBranchAddress("KMCIdx",KMCIdx);
    _fChain->SetBranchAddress("PiMCIdx",PiMCIdx);
    _fChain->SetBranchAddress("Pi0MCIdx",Pi0MCIdx);
    _fChain->SetBranchAddress("GamMCIdx",GamMCIdx);
    _fChain->SetBranchAddress("mcLund",mcLund);
    _fChain->SetBranchAddress("mothIdx",mothIdx);
    _fChain->SetBranchAddress("dauIdx",dauIdx);
    _fChain->SetBranchAddress("mcLen",&mcLen);
    _fChain->SetBranchAddress("mcmass",mcmass);
    _fChain->SetBranchAddress("mcp3",mcp3);
    _fChain->SetBranchAddress("mccosth",mccosth);
    _fChain->SetBranchAddress("mcphi",mcphi);
    _fChain->SetBranchAddress("mcp3CM",mcp3CM);
    _fChain->SetBranchAddress("mccosthCM",mccosthCM); 
    _fChain->SetBranchAddress("mcphiCM",mcphiCM);
    _fChain->SetBranchAddress("mcVtxx",mcVtxx);
    _fChain->SetBranchAddress("mcVtxy",mcVtxy);
    _fChain->SetBranchAddress("mcVtxz",mcVtxz);
  }


  cout<<"Done linking Chain Branches"<<endl;
  return 1;
}



Int_t DstPi0Analysis::MakeReducedNtupleBranches(){
  cout<<"Creating Reduced Ntuple Branches"<<endl;

  ReducedNtuple->Branch("eventnumber",&eventnumber,"eventnumber/I");

  ReducedNtuple->Branch("dstpimass",&dstpimass,"dstpimass/F");
  ReducedNtuple->Branch("dstpideltam",&dstpideltam,"dstpideltam/F");
  ReducedNtuple->Branch("dstpipstar",&dstpipstar,"dstpipstar/F");
  ReducedNtuple->Branch("dstpicosstar",&dstpicosstar,"dstpicosstar/F");
  ReducedNtuple->Branch("dstpicharge",&dstpicharge,"dstpicharge/I");
  ReducedNtuple->Branch("dstpinX",&dstpinX,"dstpinX/I");
  ReducedNtuple->Branch("dstpilogvtxprob",&dstpilogvtxprob,"dstpilogvtxprob/F");
  ReducedNtuple->Branch("dstpiIdx",&dstpiIdx,"dstpiIdx/I");
  ReducedNtuple->Branch("dstpimctrue",&dstpimctrue,"dstpimctrue/I");
  ReducedNtuple->Branch("dstpidmres",&dstpidmres,"dstpidmres/F");
  ReducedNtuple->Branch("dstpigenmass",&dstpigenmass,"dstpigenmass/F");
  ReducedNtuple->Branch("dstpigendist",&dstpigendist,"dstpigendist/F");
  ReducedNtuple->Branch("dstpimclund",&dstpimclund,"dstpimclund/I");

  ReducedNtuple->Branch("dstarmass",&dstarmass,"dstarmass/F");
  ReducedNtuple->Branch("dstarpstar",&dstarpstar,"dstarpstar/F");
  ReducedNtuple->Branch("dstardeltam",&dstardeltam,"dstardeltam/F");
  ReducedNtuple->Branch("dstarlogvtxprob",&dstarlogvtxprob,"dstarlogvtxprob/F");
  ReducedNtuple->Branch("dstarcostheta",&dstarcostheta,"dstarcostheta/F");
  ReducedNtuple->Branch("dstarcharge",&dstarcharge,"dstarcharge/I");
  ReducedNtuple->Branch("dstarmctrue",&dstarmctrue,"dstarmctrue/I");

  ReducedNtuple->Branch("pi0mass",&pi0mass,"pi0mass/F");
  ReducedNtuple->Branch("pi0pstar",&pi0pstar,"pi0pstar/F");
  ReducedNtuple->Branch("pi0energy",&pi0energy,"pi0energy/F");
  ReducedNtuple->Branch("pi0mctrue",&pi0mctrue,"pi0mctrue/I");
  ReducedNtuple->Branch("pi0cosine",&pi0cosine,"pi0cosine/F");
  ReducedNtuple->Branch("pi0costheta",&pi0costheta,"pi0costheta/F");
  ReducedNtuple->Branch("pi0helicity",&pi0helicity,"pi0helicity/F");
 

  ReducedNtuple->Branch("gam1energy",&gam1energy,"gam1energy/F");
  ReducedNtuple->Branch("gam1cos",&gam1cos,"gam1cos/F");
  ReducedNtuple->Branch("gam2energy",&gam2energy,"gam2energy/F");
  ReducedNtuple->Branch("gam2cos",&gam2cos,"gam2cos/F");


  ReducedNtuple->Branch("d0pstar",&d0pstar,"d0pstar/F");
  ReducedNtuple->Branch("d0logvtxprob",&d0logvtxprob,"d0logvtxprob/F");
  ReducedNtuple->Branch("d0charge",&d0charge,"d0charge/I");
  ReducedNtuple->Branch("d0mass",&d0mass,"d0mass/F"); 
  ReducedNtuple->Branch("d0costheta",&d0costheta,"d0costheta/F"); 
  ReducedNtuple->Branch("d0mode",&d0mode,"d0mode/I");
  ReducedNtuple->Branch("d0mctrue",&d0mctrue,"d0mctrue/I");

  ReducedNtuple->Branch("kmctrue",&kmctrue,"kmctrue/I");
  ReducedNtuple->Branch("kp3",&kp3,"kp3/F");
  ReducedNtuple->Branch("kcostheta",&kcostheta,"kcostheta/F"); 
  ReducedNtuple->Branch("kcharge",&kcharge,"kcharge/I");
  ReducedNtuple->Branch("klh",&klh,"klh/I");

  ReducedNtuple->Branch("pimctrue",&pimctrue,"pimctrue/I");
  ReducedNtuple->Branch("pip3",&pip3,"pip3/F");
  ReducedNtuple->Branch("picharge",&picharge,"picharge/I");
  ReducedNtuple->Branch("pilh",&pilh,"pilh/I");

  ReducedNtuple->Branch("slowpip3",&slowpip3,"slowpip3/F");
  ReducedNtuple->Branch("slowpimctrue",&slowpimctrue,"slowpimctrue/I");
  ReducedNtuple->Branch("slowpicharge",&slowpicharge,"slowpicharge/I");
  ReducedNtuple->Branch("slowpilh",&slowpilh,"slowpilh/I");

  ReducedNtuple->Branch("d0pi2p3",&d0pi2p3,"d0pi2p3/F");
  ReducedNtuple->Branch("d0pi2mctrue",&d0pi2mctrue,"d0pi2mctrue/I");
  ReducedNtuple->Branch("d0pi2charge",&d0pi2charge,"d0pi2charge/I");
  ReducedNtuple->Branch("d0pi2lh",&d0pi2lh,"d0pi2lh/I");

  ReducedNtuple->Branch("d0pi3p3",&d0pi3p3,"d0pi3p3/F");
  ReducedNtuple->Branch("d0pi3mctrue",&d0pi3mctrue,"d0pi3mctrue/I");
  ReducedNtuple->Branch("d0pi3charge",&d0pi3charge,"d0pi3charge/I");
  ReducedNtuple->Branch("d0pi3lh",&d0pi3lh,"d0pi3lh/I");
  
  cout<<"Done Making Reduced Ntuple Branches"<<endl;
  return 1;
}



Int_t DstPi0Analysis::SetReducedNtupleBranches(){
  cout<<"Setting Reduced Ntuple Branch Addresses"<<endl;

  ReducedNtuple->SetBranchAddress("eventnumber",&eventnumber);


  ReducedNtuple->SetBranchAddress("dstpimass",&dstpimass);
  ReducedNtuple->SetBranchAddress("dstpideltam",&dstpideltam);
  ReducedNtuple->SetBranchAddress("dstpipstar",&dstpipstar);
  ReducedNtuple->SetBranchAddress("dstpicosstar",&dstpicosstar);
  ReducedNtuple->SetBranchAddress("dstpiIdx",&dstpiIdx);
  ReducedNtuple->SetBranchAddress("dstpimctrue",&dstpimctrue);
  ReducedNtuple->SetBranchAddress("dstpicharge",&dstpicharge);
  ReducedNtuple->SetBranchAddress("dstpinX",&dstpinX);
  ReducedNtuple->SetBranchAddress("dstpilogvtxprob",&dstpilogvtxprob);
  ReducedNtuple->SetBranchAddress("dstpidmres",&dstpidmres);
  ReducedNtuple->SetBranchAddress("dstpigenmass",&dstpigenmass);
  ReducedNtuple->SetBranchAddress("dstpigendist",&dstpigendist);
  ReducedNtuple->SetBranchAddress("dstpimclund",&dstpimclund);

  ReducedNtuple->SetBranchAddress("dstarmass",&dstarmass);
  ReducedNtuple->SetBranchAddress("dstarpstar",&dstarpstar);
  ReducedNtuple->SetBranchAddress("dstardeltam",&dstardeltam);
  ReducedNtuple->SetBranchAddress("dstarcharge",&dstarcharge);
  ReducedNtuple->SetBranchAddress("dstarlogvtxprob",&dstarlogvtxprob);
  ReducedNtuple->SetBranchAddress("dstarcostheta",&dstarcostheta);
  ReducedNtuple->SetBranchAddress("dstarmctrue",&dstarmctrue);

  ReducedNtuple->SetBranchAddress("pi0mass",&pi0mass);
  ReducedNtuple->SetBranchAddress("pi0pstar",&pi0pstar);
  ReducedNtuple->SetBranchAddress("pi0energy",&pi0energy);
  ReducedNtuple->SetBranchAddress("pi0mctrue",&pi0mctrue);
  ReducedNtuple->SetBranchAddress("pi0cosine",&pi0cosine);
  ReducedNtuple->SetBranchAddress("pi0costheta",&pi0costheta);  
  ReducedNtuple->SetBranchAddress("pi0helicity",&pi0helicity);
  

  ReducedNtuple->SetBranchAddress("gam1energy",&gam1energy);
  ReducedNtuple->SetBranchAddress("gam1cos",&gam1cos);
  ReducedNtuple->SetBranchAddress("gam2energy",&gam2energy);
  ReducedNtuple->SetBranchAddress("gam2cos",&gam2cos);

  ReducedNtuple->SetBranchAddress("d0mass",&d0mass);
  ReducedNtuple->SetBranchAddress("d0pstar",&d0pstar);
  ReducedNtuple->SetBranchAddress("d0charge",&d0charge);
  ReducedNtuple->SetBranchAddress("d0logvtxprob",&d0logvtxprob);
  ReducedNtuple->SetBranchAddress("d0mode",&d0mode);
  ReducedNtuple->SetBranchAddress("d0mctrue",&d0mctrue);

  ReducedNtuple->SetBranchAddress("kcharge",&kcharge);
  ReducedNtuple->SetBranchAddress("kp3",&kp3);
  ReducedNtuple->SetBranchAddress("kcostheta",&kcostheta);
  ReducedNtuple->SetBranchAddress("kmctrue",&kmctrue);
  ReducedNtuple->SetBranchAddress("klh",&klh);

  ReducedNtuple->SetBranchAddress("picharge",&picharge);
  ReducedNtuple->SetBranchAddress("pip3",&pip3);
  ReducedNtuple->SetBranchAddress("pimctrue",&pimctrue);
  ReducedNtuple->SetBranchAddress("pilh",&pilh);

  ReducedNtuple->SetBranchAddress("slowpip3",&slowpip3);
  ReducedNtuple->SetBranchAddress("slowpicharge",&slowpicharge);
  ReducedNtuple->SetBranchAddress("slowpimctrue",&slowpimctrue);
  ReducedNtuple->SetBranchAddress("slowpilh",&slowpilh);
  
  ReducedNtuple->SetBranchAddress("d0pi2p3",&d0pi2p3);
  ReducedNtuple->SetBranchAddress("d0pi2charge",&d0pi2charge);
  ReducedNtuple->SetBranchAddress("d0pi2mctrue",&d0pi2mctrue);
  ReducedNtuple->SetBranchAddress("d0pi2lh",&d0pi2lh);

  ReducedNtuple->SetBranchAddress("d0pi3p3",&d0pi3p3);
  ReducedNtuple->SetBranchAddress("d0pi3charge",&d0pi3charge);
  ReducedNtuple->SetBranchAddress("d0pi3mctrue",&d0pi3mctrue);
  ReducedNtuple->SetBranchAddress("d0pi3lh",&d0pi3lh);
  
  cout<<"Done linking ReducedNtuple Branches"<<endl;
  return 1;
}
