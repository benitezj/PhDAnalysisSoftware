#include "DstPiAnalysis.h"

//////////
//#include "DstPiAnalysisFitMassPeak.C"
#include "DstPiAnalysisRooFitMassPeak.C"
#include "DstPiAnalysisResolution.C"


ClassImp(DstPiAnalysis);
DstPiAnalysis::DstPiAnalysis(TChain* fChain,Bool_t TruthMatch,Int_t MatterOrAntiMatter,TString OutputDir):
  _fChain(fChain),
  _MatterOrAntiMatter(MatterOrAntiMatter),							   
  _OutputDir(OutputDir),
  _TruthMatch(TruthMatch),
  OutPutFile(NULL),
  ReducedNtuple(NULL),
  MCDstPiCounterPerEvent(0),
  MCDstPiCounterTotal(0),
  RecoDstPiCounterTotal(0)
{
  if(_fChain==NULL){cout<<"No Chain"<<endl;}

  //init the database 
  database.SetTruthMatch(_TruthMatch);
   
  //the phase space and q factor for the threshold
  phasefactor="((1/(2*(dstpideltam+2.01)**2))*sqrt(((dstpideltam+2.01)*(dstpideltam+2.01) - 4.618)*((dstpideltam+2.01)*(dstpideltam+2.01)-3.500)))";
  qfactor="((1/(2*(dstpideltam+2.01)))*sqrt(((dstpideltam+2.01)*(dstpideltam+2.01) - 4.618)*((dstpideltam+2.01)*(dstpideltam+2.01)-3.500)))";

  cout<<"Done Initializing the analysis"<<endl;
}

DstPiAnalysis::~DstPiAnalysis(){  
  
}


Int_t DstPiAnalysis::ReduceNtuple(Bool_t RunInTestMode){
  cout<<"Going to reduce the ntuple"<<endl;

  if(_fChain==NULL){
    cout<<"No chain."<<endl;
    return 0;
  } 


  ///link the branches in the input chain
  SetBranches();


  //the file must be created here so the 'new' histos and Tree is created in hard drive 
  OutPutFile=new TFile(_OutputDir+"/DstPiReduced.root","recreate",_OutputDir+"/DstPiReduced.root",4);
  if(OutPutFile==NULL){cout<<"Failed to create file"<<endl;return 0;}


  //create new reduced ntuple
  ReducedNtuple=new TTree("DstPiNtuple","DstPi Reduced Ntuple",99);        
 
  //create the branches in the ReducedNtuple
  MakeReducedNtupleBranches();


  if(_TruthMatch){
    //create the MC Histos
    HMCDstPiMass=new TH1F();
    SetHistoXY(HMCDstPiMass,"HMCDstPiMass",database.GetDstPiMassNbins(),database.GetDstPiMassMin(),database.GetDstPiMassMax(),"DstPi Cand. Mass (GeV/c^{2})","");
  
    HMCDstPiDeltaM=new TH1F();
    SetHistoXY(HMCDstPiDeltaM,"HMCDstPiDeltaM",database.GetDstPiDeltaMNbins(),database.GetDstPiDeltaMMin(),database.GetDstPiDeltaMMax(),"D*(2640) Mass - D*(2010) (GeV/c^{2})","");

    HMCDstPip3CM=new TH1F();
    SetHistoXY(HMCDstPip3CM,"HMCDstPip3CM",50,0,5,
	       "p* (GeV/c)","Entries/100MeV");
  
    HMCDstPicosthCM=new TH1F();
    SetHistoXY(HMCDstPicosthCM,"HMCDstPicosthCM",20,-1.0001,1.0001,
	       "cos(#theta)*","");
  
    H2MCDstPiCMPvsTheta=new TH2F();
    SetHisto2D(H2MCDstPiCMPvsTheta,"H2MCDstPiCMPvsTheta",50,0,5,
	       "p* (GeV/c)",20,-1.0001,1.0001,"cos(#theta*)","");  

    HMCNDstPi=new TH1F();
    SetHistoXY(HMCNDstPi,"HMCNDstPi",11,-.5,10.5,
	       "nDstPi/event","Counts");
  }
  //////


  ///check how many candidates are removed by array size cut
  Int_t RemovedCands=0;
  Int_t InitialdCands=0;

  //Start the event loop;
  Long_t MaxNumberEvents=1000000000;
  if(RunInTestMode)MaxNumberEvents=10000;

  Int_t eventid=0;
  while(_fChain->GetEntry(eventid,0)>0&&eventid<MaxNumberEvents){   
    eventid++;
     
    if(eventid%5000==0)cout<<eventid<<" Events done.   With "<<RecoDstPiCounterTotal<<" Candidates saved."<<endl;    
      
    eventnumber=eventid;
     
    InitialdCands+=nDstPi;
    if(nDstPi>MAXNCANDS){
      cout<<"Too many cands at event "<<eventid<<" Only first "<<MAXNCANDS<<" of "<<nDstPi<<" will be used"<<endl;
      RemovedCands+=nDstPi-MAXNCANDS; 
      nDstPi=MAXNCANDS;      
    }

    ///Loop over the reconstructed
    RecoDstPiCounterPerEvent=0;      
    
    //------beam quantities
    TVector3 beamVtx(primVtxX,primVtxY,primVtxZ);


    for(DstPiIdx=0;DstPiIdx<nDstPi;DstPiIdx++){

      ////////////cut out the unconverged fits
      if(DstPiVtxStatus[DstPiIdx]!=0)continue;               


      ///determine the indexes for each particle
      DstarIdx=DstPid1Idx[DstPiIdx];
      SlowPiIdx=Dstard2Idx[DstarIdx];
      D0Idx=Dstard1Idx[DstarIdx];
      PiIdx=D0d2Idx[D0Idx];
      KIdx=D0d1Idx[D0Idx];
      Pi1Idx=DstPid2Idx[DstPiIdx];
      D0Pi2Idx=D0d3Idx[D0Idx];
      D0Pi3Idx=D0d4Idx[D0Idx];
          
      PitrkIdx=PiTrkIdx[PiIdx];
      KtrkIdx=KTrkIdx[KIdx];
      SlowPitrkIdx=PiTrkIdx[SlowPiIdx];
      Pi1trkIdx=PiTrkIdx[Pi1Idx];
      D0Pi2trkIdx=PiTrkIdx[D0Pi2Idx];
      D0Pi3trkIdx=PiTrkIdx[D0Pi3Idx];
       
      //////fill reduced ntuple variables    
      dstpimass=DstPiMass[DstPiIdx];
      dstpipstar=DstPip3CM[DstPiIdx];
      dstpicosstar=DstPicosthCM[DstPiIdx];
      dstpideltam=DstPiMass[DstPiIdx]-DstarMass[DstarIdx];  
      if(TMath::Prob(DstPiChi2[DstPiIdx],DstPinDof[DstPiIdx])>0)
	dstpilogvtxprob=log10(TMath::Prob(DstPiChi2[DstPiIdx],DstPinDof[DstPiIdx]));
      else dstpilogvtxprob=-100;
      //Determine D*pi combination: dstpicharge= -2=D*-pi- , -1=D*-pi+  , 1=D*+pi- , 2=D*+pi+
      dstpicharge=DstarLund[DstarIdx]/abs(DstarLund[DstarIdx])+PiLund[Pi1Idx]/abs(PiLund[Pi1Idx]);
      if(dstpicharge==0)dstpicharge=DstarLund[DstarIdx]/abs(DstarLund[DstarIdx]);
      dstpinDstPi=nDstPi;  
      dstpiIdx=DstPiIdx;

      //-------Dstar quantities
      dstarmass=DstarMass[DstarIdx];
      dstardeltam=DstarMass[DstarIdx]-D0Mass[D0Idx];
      if(DstarVtxStatus[DstarIdx]==0) dstarlogvtxprob=log10(TMath::Prob(DstarChi2[DstarIdx],DstarnDof[DstarIdx]));   
      else dstarlogvtxprob=0;
      dstarcostheta=ComputeDstarAngle();
      dstarpstar=Dstarp3CM[DstarIdx];
      dstarcharge=DstarLund[DstarIdx]/abs(DstarLund[DstarIdx]);

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

      pi1p3=Pip3[Pi1Idx];      
      pi1charge=PiLund[Pi1Idx]/abs(PiLund[Pi1Idx]);
 


      //fore MC determine if candidate was truthmatched
      dstpimctrue=1;
      dstarmctrue=1;
      d0mctrue=1;
      pimctrue=1;
      slowpimctrue=1;
      kmctrue=1;
      pi1mctrue=1;
      d0pi2mctrue=1;  
      d0pi3mctrue=1;      
      if(_TruthMatch){
	dstpimctrue=(DstPiMCIdx[DstPiIdx]>0);
	dstarmctrue=(DstarMCIdx[DstarIdx]>0);
	d0mctrue=(D0MCIdx[D0Idx]>0);
	pimctrue=(PiMCIdx[PiIdx]>0);
	slowpimctrue=(PiMCIdx[SlowPiIdx]>0);
	kmctrue=(KMCIdx[KIdx]>0);
	pi1mctrue=(PiMCIdx[Pi1Idx]>0);
	if(d0mode==2){
	  d0pi2mctrue=(PiMCIdx[D0Pi2Idx]>0);
	  d0pi3mctrue=(PiMCIdx[D0Pi3Idx]>0);
	}
      }

      //Determine the mass resolution for MC
      if(_TruthMatch&&dstpimctrue)      
	dstpidmres=dstpideltam-(mcmass[DstPiMCIdx[DstPiIdx]]-mcmass[DstarMCIdx[DstarIdx]]);
      else dstpidmres=100000;
      
      //Determine resolution from separate true D* and true pi 
      if(_TruthMatch)
	dstpigenmass=CalculateGenMass();
      else dstpigenmass=0;
          
      //apply some simple cuts to minimize file size
      if(dstarcostheta>.5||dstardeltam>.150||fabs(d0mass-1.865)>.02) continue;
     
      ReducedNtuple->Fill();	              
               
      RecoDstPiCounterTotal++;
           
   
    }//DstPi loop    
   
    
    ///-------------------------------
    ///Fill the Generated Quantities
    ///-----------------------------
    if(_TruthMatch){
      //now loop over the Generated MC
      MCDstPiCounterPerEvent=0; 
      Int_t mcid=-1;
      while(mcid<mcLen){
	mcid++;

	if(mcLund[mcid]==_MatterOrAntiMatter*MYDSTPILUND){
	  MCDstPiCounterPerEvent++; 
	  MCDstPiCounterTotal++;
		
	  HMCDstPiMass->Fill(mcmass[mcid]);
	  HMCDstPip3CM->Fill(mcp3CM[mcid]); 
	  HMCDstPicosthCM->Fill(mccosthCM[mcid]); 	
	  H2MCDstPiCMPvsTheta->Fill(mcp3CM[mcid],mccosthCM[mcid]);

	  //calculate deltaM   //mcmass[dauIdx[mcid]]=first daughter; mcmass[dauIdx[mcid] + 1] = second daughter ...	    
	  HMCDstPiDeltaM->Fill(mcmass[mcid]-mcmass[dauIdx[mcid]]);

	}
      }
      HMCNDstPi->Fill(MCDstPiCounterPerEvent);
    }

  }

  //print summary
  cout<<"--------Summary-------"<<endl;
  cout<<"Total events="<<eventid<<endl;
  cout<<"Total Generated="<<MCDstPiCounterTotal<<" Reconstructed="<<RecoDstPiCounterTotal<<endl;
  cout<<"Total candidates removed "<<RemovedCands<<" ( "<<(int)(100*RemovedCands/InitialdCands + .5)<<"%)"<<endl;
  cout<<"--------End Summary---"<<endl;

  ////Save the Ntuple and histograms
  cout<<"Going to save the ntuple and histograms"<<endl; 

  /*///////////Should not have to call TTree->Write(), TFile->Write() will doit
  if(ReducedNtuple->Write()){ cout<<"Ntuple written."<<endl;}
  else{ cout<<"Failed to write ntuple"<<endl;return 0;}
  delete  ReducedNtuple;   

  if(_TruthMatch){
    
    if(HMCDstPiMass->Write()&&
       HMCDstPiDeltaM->Write()&&
       HMCDstPip3CM->Write()&&
       HMCDstPicosthCM->Write()&&     
       H2MCDstPiCMPvsTheta->Write()&&     
       HMCNDstPi->Write()
       ){ 
      cout<<" Histograms have been written"<<endl;
    }
    else{
      cout<<"Failed to write Histos"<<endl;return 0;
    }

//////Should not have to do this, OutPutFile->Close() will do it.
//     delete  HMCDstPiMass;
//     delete  HMCDstPip3CM;
//     delete  HMCDstPicosthCM;     
//     delete  H2MCDstPiCMPvsTheta;   
//     delete  HMCNDstPi;        
//     delete  HMCDstPiDeltaM;
    
    cout<<"histos deleted."<<endl;

  }
  */
  
  OutPutFile->ls();
  OutPutFile->Write(); //necesary otherwise file is not closed, this will write all objects currently on memory
  cout<<OutPutFile->GetName()<<" has been written"<<endl;
  OutPutFile->ls();
  OutPutFile->Close(); //this will delete all objects created inside the file if they were not written
  cout<<OutPutFile->GetName()<<" has been closed."<<endl;

  delete OutPutFile;

  return 1;
}


Int_t DstPiAnalysis::OpenReducedNtuple(Int_t WhichOne){
  
  cout<<"Going to open reduced ntuple."<<endl;
  if(WhichOne==1)cout<<"Will return Full data Ntuple"<<endl;
  if(WhichOne==2)cout<<"Will return Clean Ntuple"<<endl;

  if(!(OutPutFile=new TFile(_OutputDir+"/DstPiReduced.root","read"))){
    cout<<"Unable to open reduced file"<<endl;
    return 0;
  }
  if(OutPutFile->IsZombie()){
    cout<<"Zombie reduced ntuple file"<<endl;return 0;
  }
  cout<<"File opened: "<<OutPutFile<<" "<<OutPutFile->GetName()<<endl;

  if(WhichOne==1){//Full Data
    if(!(ReducedNtuple=(TTree*)OutPutFile->Get("DstPiNtuple"))){
      cout<<"No reduced ntuple"<<endl;return 0;
    }
  }
  if(WhichOne==2){//Clean Ntuple
    if(!(ReducedNtuple=(TTree*)OutPutFile->Get("CleanNtuple"))){
      cout<<"No reduced ntuple"<<endl;return 0;
    }
  }
  
  //ReducedNtuple->SetDirectory(0);
  cout<<"Ntuple Extracted: "<<ReducedNtuple<<" "<<ReducedNtuple->GetName()<<"  With "<<ReducedNtuple->GetEntries()<<" Entries"<<endl;

  SetReducedNtupleBranches();
  
  gROOT->cd();//******REMEMBER TO DO THIS WHEN OPENING A FILE otherwise new objects are created in that file******//

  return 1;
}

Int_t DstPiAnalysis::CloseReducedNtuple(){
  cout<<"Going to close the file."<<endl;
 
  OutPutFile->Close();

  delete OutPutFile;
  
  return 1;
}


Float_t DstPiAnalysis::ComputeDstarAngle(){
  TVector3 DstPip3vec(DstPip3CM[DstPiIdx]*sin(acos(DstPicosthCM[DstPiIdx]))*cos(DstPiphiCM[DstPiIdx]),
		      DstPip3CM[DstPiIdx]*sin(acos(DstPicosthCM[DstPiIdx]))*sin(DstPiphiCM[DstPiIdx]),
		      DstPip3CM[DstPiIdx]*DstPicosthCM[DstPiIdx]);
  
  Float_t DstPiEnergy=sqrt(DstPip3vec*DstPip3vec + DstPiMass[DstPiIdx]*DstPiMass[DstPiIdx]);
  TVector3 DstPiVelvec=(1/DstPiEnergy)*DstPip3vec;

  TVector3 Dstarp3vec(Dstarp3CM[DstarIdx]*sin(acos(DstarcosthCM[DstarIdx]))*cos(DstarphiCM[DstarIdx]),
		      Dstarp3CM[DstarIdx]*sin(acos(DstarcosthCM[DstarIdx]))*sin(DstarphiCM[DstarIdx]),
		      Dstarp3CM[DstarIdx]*DstarcosthCM[DstarIdx]); 
  
  TLorentzVector Dstarp4(Dstarp3vec,sqrt(Dstarp3vec*Dstarp3vec+DstarMass[DstarIdx]*DstarMass[DstarIdx]));

  //boost the dstar momentum into rest frame of DstPi
  Dstarp4.Boost(-DstPiVelvec);//watch the minus sign
  TVector3 DstarBoostedp3vec=Dstarp4.Vect();

  //dot product of boosted dstar direction with DstPi direction in CM frame
  Float_t cosine=DstarBoostedp3vec.Unit()*DstPiVelvec.Unit();
  
  return cosine;
  
}
Float_t DstPiAnalysis::CalculateGenMass(){
  if(DstarIdx<0||Pi1Idx<0)return 0;

  //determine 4-vector of true-D* 
  TVector3 Dstarp3vec;
  Dstarp3vec.SetMagThetaPhi(mcp3CM[DstarMCIdx[DstarIdx]],acos(mccosthCM[DstarMCIdx[DstarIdx]]),mcphiCM[DstarMCIdx[DstarIdx]]);
  TLorentzVector Dstarp4(Dstarp3vec,sqrt(Dstarp3vec*Dstarp3vec+pow(mcmass[DstarMCIdx[DstarIdx]],2)));

  //determine 4-vector of true-pi
  TVector3 Pi1p3vec;
  Pi1p3vec.SetMagThetaPhi(mcp3CM[PiMCIdx[Pi1Idx]],acos(mccosthCM[PiMCIdx[Pi1Idx]]),mcphiCM[PiMCIdx[Pi1Idx]]);
  TLorentzVector Pi1p4(Pi1p3vec,sqrt(Pi1p3vec*Pi1p3vec+pow(mcmass[PiMCIdx[Pi1Idx]],2)));

  //return the mass
  return (Dstarp4+Pi1p4).Mag();
}

Int_t DstPiAnalysis::SetBranches(){
  if(_fChain==NULL){cout<<"No Chain"<<endl;return 0;} 
  cout<<"Setting Chain Branch Addresses"<<endl;

  _fChain->SetBranchAddress("nDstPi",&nDstPi);
  _fChain->SetBranchAddress("DstPiMass",DstPiMass);
  _fChain->SetBranchAddress("DstPip3",DstPip3);
  _fChain->SetBranchAddress("DstPicosth",DstPicosth);
  _fChain->SetBranchAddress("DstPiphi",DstPiphi);
  _fChain->SetBranchAddress("DstPip3CM",DstPip3CM);
  _fChain->SetBranchAddress("DstPicosthCM",DstPicosthCM);
  _fChain->SetBranchAddress("DstPiphiCM",DstPiphiCM);
  _fChain->SetBranchAddress("DstPiLund",DstPiLund); 
  _fChain->SetBranchAddress("DstPid1Lund",DstPid1Lund);
  _fChain->SetBranchAddress("DstPid1Idx",DstPid1Idx);
  _fChain->SetBranchAddress("DstPid2Lund",DstPid2Lund);
  _fChain->SetBranchAddress("DstPid2Idx",DstPid2Idx);
  _fChain->SetBranchAddress("DstPid3Lund",DstPid3Lund);
  _fChain->SetBranchAddress("DstPid3Idx",DstPid3Idx);
  _fChain->SetBranchAddress("DstPiVtxx",DstPiVtxx);
  _fChain->SetBranchAddress("DstPiVtxy",DstPiVtxy);
  _fChain->SetBranchAddress("DstPiVtxz",DstPiVtxz);
  _fChain->SetBranchAddress("DstPiChi2",DstPiChi2);
  _fChain->SetBranchAddress("DstPinDof",DstPinDof);
  _fChain->SetBranchAddress("DstPiVtxStatus",DstPiVtxStatus);
 
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

  if(_TruthMatch){
    _fChain->SetBranchAddress("DstPiMCIdx",DstPiMCIdx);
    _fChain->SetBranchAddress("DstarMCIdx",DstarMCIdx);
    _fChain->SetBranchAddress("D0MCIdx",D0MCIdx);
    _fChain->SetBranchAddress("KMCIdx",KMCIdx);
    _fChain->SetBranchAddress("PiMCIdx",PiMCIdx);
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
  }


  cout<<"Done linking Chain Branches"<<endl;
  return 1;
}



Int_t DstPiAnalysis::MakeReducedNtupleBranches(){
  cout<<"Creating Reduced Ntuple Branches"<<endl;

  ReducedNtuple->Branch("eventnumber",&eventnumber,"eventnumber/I");

  ReducedNtuple->Branch("dstpimass",&dstpimass,"dstpimass/F");
  ReducedNtuple->Branch("dstpideltam",&dstpideltam,"dstpideltam/F");
  ReducedNtuple->Branch("dstpipstar",&dstpipstar,"dstpipstar/F");
  ReducedNtuple->Branch("dstpicosstar",&dstpicosstar,"dstpicosstar/F");
  ReducedNtuple->Branch("dstpicharge",&dstpicharge,"dstpicharge/I");
  ReducedNtuple->Branch("dstpinDstPi",&dstpinDstPi,"dstpinDstPi/I");
  ReducedNtuple->Branch("dstpilogvtxprob",&dstpilogvtxprob,"dstpilogvtxprob/F");
  ReducedNtuple->Branch("dstpiIdx",&dstpiIdx,"dstpiIdx/I");
  ReducedNtuple->Branch("dstpimctrue",&dstpimctrue,"dstpimctrue/I");
  ReducedNtuple->Branch("dstpidmres",&dstpidmres,"dstpidmres/F");
  ReducedNtuple->Branch("dstpigenmass",&dstpigenmass,"dstpigenmass/F");

  ReducedNtuple->Branch("dstarmass",&dstarmass,"dstarmass/F");
  ReducedNtuple->Branch("dstarpstar",&dstarpstar,"dstarpstar/F");
  ReducedNtuple->Branch("dstardeltam",&dstardeltam,"dstardeltam/F");
  ReducedNtuple->Branch("dstarlogvtxprob",&dstarlogvtxprob,"dstarlogvtxprob/F");
  ReducedNtuple->Branch("dstarcostheta",&dstarcostheta,"dstarcostheta/F");
  ReducedNtuple->Branch("dstarcharge",&dstarcharge,"dstarcharge/I");
  ReducedNtuple->Branch("dstarmctrue",&dstarmctrue,"dstarmctrue/I");

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

  ReducedNtuple->Branch("pimctrue",&pimctrue,"pimctrue/I");
  ReducedNtuple->Branch("pip3",&pip3,"pip3/F");
  ReducedNtuple->Branch("picharge",&picharge,"picharge/I");

  ReducedNtuple->Branch("pi1mctrue",&pi1mctrue,"pi1mctrue/I");
  ReducedNtuple->Branch("pi1p3",&pi1p3,"pi1p3/F");
  ReducedNtuple->Branch("pi1charge",&pi1charge,"pi1charge/I");

  ReducedNtuple->Branch("slowpip3",&slowpip3,"slowpip3/F");
  ReducedNtuple->Branch("slowpimctrue",&slowpimctrue,"slowpimctrue/I");
  ReducedNtuple->Branch("slowpicharge",&slowpicharge,"slowpicharge/I");

  ReducedNtuple->Branch("d0pi2p3",&d0pi2p3,"d0pi2p3/F");
  ReducedNtuple->Branch("d0pi2mctrue",&d0pi2mctrue,"d0pi2mctrue/I");
  ReducedNtuple->Branch("d0pi2charge",&d0pi2charge,"d0pi2charge/I");
 
  ReducedNtuple->Branch("d0pi3p3",&d0pi3p3,"d0pi3p3/F");
  ReducedNtuple->Branch("d0pi3mctrue",&d0pi3mctrue,"d0pi3mctrue/I");
  ReducedNtuple->Branch("d0pi3charge",&d0pi3charge,"d0pi3charge/I");
  
  cout<<"Done Making Reduced Ntuple Branches"<<endl;
  return 1;
}



Int_t DstPiAnalysis::SetReducedNtupleBranches(){
  cout<<"Setting Reduced Ntuple Branch Addresses"<<endl;

  ReducedNtuple->SetBranchAddress("eventnumber",&eventnumber);


  ReducedNtuple->SetBranchAddress("dstpimass",&dstpimass);
  ReducedNtuple->SetBranchAddress("dstpideltam",&dstpideltam);
  ReducedNtuple->SetBranchAddress("dstpipstar",&dstpipstar);
  ReducedNtuple->SetBranchAddress("dstpicosstar",&dstpicosstar);
  ReducedNtuple->SetBranchAddress("dstpiIdx",&dstpiIdx);
  ReducedNtuple->SetBranchAddress("dstpimctrue",&dstpimctrue);
  ReducedNtuple->SetBranchAddress("dstpicharge",&dstpicharge);
  ReducedNtuple->SetBranchAddress("dstpinDstPi",&dstpinDstPi);
  ReducedNtuple->SetBranchAddress("dstpilogvtxprob",&dstpilogvtxprob);
  ReducedNtuple->SetBranchAddress("dstpidmres",&dstpidmres);
  ReducedNtuple->SetBranchAddress("dstpigenmass",&dstpigenmass);

  ReducedNtuple->SetBranchAddress("dstarmass",&dstarmass);
  ReducedNtuple->SetBranchAddress("dstarpstar",&dstarpstar);
  ReducedNtuple->SetBranchAddress("dstardeltam",&dstardeltam);
  ReducedNtuple->SetBranchAddress("dstarcharge",&dstarcharge);
  ReducedNtuple->SetBranchAddress("dstarlogvtxprob",&dstarlogvtxprob);
  ReducedNtuple->SetBranchAddress("dstarcostheta",&dstarcostheta);
  ReducedNtuple->SetBranchAddress("dstarmctrue",&dstarmctrue);

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

  ReducedNtuple->SetBranchAddress("picharge",&picharge);
  ReducedNtuple->SetBranchAddress("pip3",&pip3);
  ReducedNtuple->SetBranchAddress("pimctrue",&pimctrue);

  ReducedNtuple->SetBranchAddress("pi1mctrue",&pi1mctrue);
  ReducedNtuple->SetBranchAddress("pi1p3",&pi1p3);
  ReducedNtuple->SetBranchAddress("pi1charge",&pi1charge);
  
  ReducedNtuple->SetBranchAddress("slowpip3",&slowpip3);
  ReducedNtuple->SetBranchAddress("slowpicharge",&slowpicharge);
  ReducedNtuple->SetBranchAddress("slowpimctrue",&slowpimctrue);
  
  ReducedNtuple->SetBranchAddress("d0pi2p3",&d0pi2p3);
  ReducedNtuple->SetBranchAddress("d0pi2charge",&d0pi2charge);
  ReducedNtuple->SetBranchAddress("d0pi2mctrue",&d0pi2mctrue);
  
  ReducedNtuple->SetBranchAddress("d0pi3p3",&d0pi3p3);
  ReducedNtuple->SetBranchAddress("d0pi3charge",&d0pi3charge);
  ReducedNtuple->SetBranchAddress("d0pi3mctrue",&d0pi3mctrue);
  
  cout<<"Done linking ReducedNtuple Branches"<<endl;
  return 1;
}
