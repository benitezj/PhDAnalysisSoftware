#include "BToDstPiPiAnalysis.h"

/////////
#include "BToDstPiPiAnalysisMES.C"
#include "BToDstPiPiAnalysisDalits.C"

ClassImp(BToDstPiPiAnalysis);
BToDstPiPiAnalysis::BToDstPiPiAnalysis(Bool_t MCorDATA,Bool_t TruthMatch,Int_t MatterOrAntiMatter,TString OutputDir):
  DstAnalysis(MCorDATA,TruthMatch,MatterOrAntiMatter,OutputDir),
  cuttruth(0),
  cutcharge(0),
  cutdmass(0),
  cutdstdeltam(0),
  cutthrust(0),
  cutmes(0),
  cutdeltae(0)
{
  FRandomizePions=new TF1("FRandomizePions","1",-1,1);

  cout<<"Done Initializing the analysis"<<endl;
}

BToDstPiPiAnalysis::~BToDstPiPiAnalysis(){ 
  delete FRandomizePions;
}

Int_t BToDstPiPiAnalysis::ReduceNtuple(Bool_t RunInTestMode){
  cout<<"Going to reduce the ntuple"<<endl;

  if(_fChain==NULL){
    cout<<"No chain."<<endl;
    return 0;
  } 

  ///link the branches in the input chain
  SetBToDstPiPiBranches();



  //the file must be created here so the 'new' histos and Tree is created in hard drive //OutFile=new 
  TFile OutFile(_OutputDir+"/Ntuple_New.root","recreate",_OutputDir+"/Ntuple_New.root",1);//1=minimal compression
  if(OutFile.IsZombie()){cout<<"Failed to create file"<<endl;return 0;}


  //create new reduced ntuple
  TTree Ntuple("Ntuple","Ntuple",0);
  Long64_t maxsize=10000000000;//10Gb
  cout<<" file size "<<maxsize<<endl;
  Ntuple.SetMaxTreeSize(maxsize);//split files

  //disable autosaving 
  Long64_t fautosave=20000000000;//50Gb !!
  cout<<" autosave "<<fautosave<<endl;
  Ntuple.SetAutoSave(fautosave);

  //create the branches in the Ntuple
  MakeBToDstPiPiReducedNtupleBranches(&Ntuple);


  //Start the event loop;
  Long_t MaxNumberEvents=1000000000;
  if(RunInTestMode)MaxNumberEvents=10000;


  //Counters
  Int_t MCBCounterTotal=0;
  Int_t RecoBCounterTotal=0;
  


  cout<<"cuts:"
      <<"  cuttruth="<<cuttruth
      <<"  cutcharge="<<cutcharge
      <<"  cutdmass="<<cutdmass
      <<"  cutdstdeltam="<<cutdstdeltam
      <<"  cutthrust="<<cutthrust
      <<"  cutmes="<<cutmes
      <<"  cutdeltae="<<cutdeltae
      <<endl;


  cout<<"Total Entries in Chain="<<_fChain->GetEntries()<<endl;
  cout<<"Going to start the event loop"<<endl;
  eventid=0;
  while(_fChain->GetEntry(eventid,0)>0&&eventid<MaxNumberEvents){   
    eventid++;
    
    if(eventid%100000==0){
      cout<<eventid<<" Events done.   With "<<RecoBCounterTotal<<" Candidates saved."<<endl;    
    }

    ///Event vars
    FillEventVars();
   

    //determine best candidate
    //DetermineBestCand();
    

    //now do real loop
    for(BIdx=0;BIdx<nB;BIdx++){
     
 
      //
      FillBToDstPiPiVars();     
      
      
      ///important for the normalization of the pdfs
      //cut wronly assigned massconstrains:
      if(fabs(bmass-BCMCMass)>.00001)continue;
      
      //Selection cuts:
      if(cuttruth==1)if(bmctrue!=1)continue;
      if(cutcharge==1)if(abs(bcharge)==2)continue;      
      if(cutcharge==2)if(abs(bcharge)==1)continue;      
      if(cutdmass==1)if(fabs(dmass-D0PDGMass)>.02) continue;
      if(cutdstdeltam==1)if(fabs(dstdeltam-.1454)>.001) continue;
      if(cutdstdeltam==2)if(fabs(dstdeltam-.1454)<.002||.003<fabs(dstdeltam-.1454)) continue;
      if(cutthrust==1)if(bthrust>.8)continue;
      if(cutmes==1)if(fabs(bmes-BCPDGMass)>.006)continue;//2 sigma cut
      if(cutmes==2)if(bmes<5.24||5.26<bmes)continue;
      if(cutdeltae==1)if(fabs(bdeltae)>.05)continue;

      //save
      RecoBCounterTotal++;
      Ntuple.Fill();
   
    }//B loop    
      
 

  }

  //print summary
  cout<<"--------Summary-------"<<endl;
  cout<<"Total events = "<<eventid<<endl;
  cout<<"Total reco ="<<RecoBCounterTotal<<endl;
  cout<<"Total mc ="<<MCBCounterTotal<<endl;
  cout<<"--------End Summary---"<<endl;

  ////Save the Ntuple and histograms
  cout<<"Going to save the ntuple and histograms"<<endl; 

  //---
  if(Ntuple.AutoSave("Overwrite")){ cout<<"Ntuple written."<<endl;}
  else{ cout<<"Failed to write ntuple"<<endl;return 0;}


  cout<<"Objects in the current file"<<endl;
  Ntuple.GetCurrentFile()->ls();

  OutFile.Close(); //this will delete all objects created inside the file if they were not written  
  cout<<OutFile.GetName()<<" has been closed."<<endl;


  return 1;
}


void BToDstPiPiAnalysis::FillBToDstPiPiVars(){

  if(BIdx>=MAXNCANDS)return;
  
  ///determine the indexes for each particle before used by other methods
  DstIdx=Bd1Idx[BIdx]; 
  Pi1Idx=Bd2Idx[BIdx];
  Pi2Idx=Bd3Idx[BIdx];
  if(FRandomizePions->GetRandom()>0){ //randomize the pions, make sure to use these indexes below, not predefined quantities
    Pi2Idx=Bd2Idx[BIdx];
    Pi1Idx=Bd3Idx[BIdx];
  }

  //////fill reduced ntuple variables    
  bmass=BMass[BIdx];
//   bmass=ComputexyzMass(Dstp3CM[DstIdx],DstcosthCM[DstIdx],DstphiCM[DstIdx],DstMass[DstIdx],			    
// 		       Pip3CM[Pi1Idx],PicosthCM[Pi1Idx],PiphiCM[Pi1Idx],PiPDGMass,
// 		       Pip3CM[Pi2Idx],PicosthCM[Pi2Idx],PiphiCM[Pi2Idx],PiPDGMass);
  bpstar=Bp3CM[BIdx];
  bcosstar=BcosthCM[BIdx];
  bphistar=BphiCM[BIdx];
  bmes=BmES[BIdx]; 
  bdeltae=BDeltaE[BIdx];  
  if(TMath::Prob(BChi2[BIdx],BnDof[BIdx])>1e-300)
    blogvtxprob=log10(TMath::Prob(BChi2[BIdx],BnDof[BIdx]));
  else blogvtxprob=-100;
  //Determine combination: xcharge=+1=D*-pi+pi+ , -1=D*+pi-pi-  , +2=D*-pi+pi- , -2=D*+pi+pi-
  if(PiLund[Pi1Idx]*PiLund[Pi2Idx]>0)bcharge=BLund[BIdx]/abs(BLund[BIdx]);
  if(PiLund[Pi1Idx]*PiLund[Pi2Idx]<0)bcharge=2*BLund[BIdx]/abs(BLund[BIdx]);
  if(_MCorDATA==1) bmctrue=1; else bmctrue=(BMyMCIdx[BIdx]>=0);  

  bdstpi1mass=ComputexyMass(Dstp3CM[DstIdx],DstcosthCM[DstIdx],DstphiCM[DstIdx],DstMass[DstIdx],			    
			    Pip3CM[Pi1Idx],PicosthCM[Pi1Idx],PiphiCM[Pi1Idx],PiPDGMass);
  bdstpi2mass=ComputexyMass(Dstp3CM[DstIdx],DstcosthCM[DstIdx],DstphiCM[DstIdx],DstMass[DstIdx],			    
			    Pip3CM[Pi2Idx],PicosthCM[Pi2Idx],PiphiCM[Pi2Idx],PiPDGMass);



  bdstpi1decayangle=ComputeDststDecayAngle(Pi2Idx,Pi1Idx);
  bdstpi2decayangle=ComputeDststDecayAngle(Pi1Idx,Pi2Idx);

  if(bdstpi2mass>bdstpi1mass){//switch such that m(D*pi1) is smaller and will be the D** 
    bdstpi1massfolded=bdstpi1mass;
    bdstpi2massfolded=bdstpi2mass;
    bdstpi1decayanglefolded=bdstpi1decayangle;
    bdstpi2decayanglefolded=bdstpi2decayangle;
  }else {
    bdstpi1massfolded=bdstpi2mass;
    bdstpi2massfolded=bdstpi1mass;
    bdstpi1decayanglefolded=bdstpi2decayangle;
    bdstpi2decayanglefolded=bdstpi1decayangle;
  }

  bflights=BFlightBSLen[BIdx]/BFlightBSErr[BIdx];
  bthrust=BROEthrust[BIdx];
  eventsph=Bsph[BIdx];
  eventr2=BR2[BIdx];
  eventthrust=Bthrust[BIdx];


  
  //
  FillDstVars();
 
}

Float_t BToDstPiPiAnalysis::ComputeDststDecayAngle(Int_t BPiIdx,Int_t DPiIdx){
  ///////////////////////////////////////////////
  //Angle of the BPi w.r.t D**Pi in the D** frame

  //D* 
  TVector3 Dstp3vec(Dstp3CM[DstIdx]*sin(acos(DstcosthCM[DstIdx]))*cos(DstphiCM[DstIdx]),
                  Dstp3CM[DstIdx]*sin(acos(DstcosthCM[DstIdx]))*sin(DstphiCM[DstIdx]),
                  Dstp3CM[DstIdx]*DstcosthCM[DstIdx]); 
  TLorentzVector Dstp4(Dstp3vec,sqrt(Dstp3vec*Dstp3vec+DstMass[DstIdx]*DstMass[DstIdx]));                                          
  
  //pion from D**
  TVector3 DPip3vec(Pip3CM[DPiIdx]*sin(acos(PicosthCM[DPiIdx]))*cos(PiphiCM[DPiIdx]),
		    Pip3CM[DPiIdx]*sin(acos(PicosthCM[DPiIdx]))*sin(PiphiCM[DPiIdx]),
		    Pip3CM[DPiIdx]*PicosthCM[DPiIdx]);  
  TLorentzVector DPip4(DPip3vec,sqrt(DPip3vec*DPip3vec+PiPDGMass*PiPDGMass));                                                 

  //D** 4-vector
  TLorentzVector Dstst(Dstp4+DPip4);  
  Float_t DststEnergy=sqrt(Dstst.Vect()*Dstst.Vect() + Dstst.Mag()*Dstst.Mag());
  TVector3 DststVelvec=(1./DststEnergy)*Dstst.Vect();

  //pion from B
  TVector3 BPip3vec(Pip3CM[BPiIdx]*sin(acos(PicosthCM[BPiIdx]))*cos(PiphiCM[BPiIdx]),
		    Pip3CM[BPiIdx]*sin(acos(PicosthCM[BPiIdx]))*sin(PiphiCM[BPiIdx]),
		    Pip3CM[BPiIdx]*PicosthCM[BPiIdx]);
  
  TLorentzVector BPip4(BPip3vec,sqrt(BPip3vec*BPip3vec+PiPDGMass*PiPDGMass));    
  
  //boost into the D** frame
  DPip4.Boost(-DststVelvec);
  BPip4.Boost(-DststVelvec);
  
  //dot product 
  return (DPip4.Vect().Unit())*(BPip4.Vect().Unit());
}

Float_t BToDstPiPiAnalysis::ComputeDstHelicityAngle(Int_t DPiIdx){
  //////////////////////////////////////////////////////
  //Angle of the Slow Pion with respect to D**Pi in D* frame

  return DPiIdx;
}



Int_t BToDstPiPiAnalysis::SetBToDstPiPiBranches(){
  if(_fChain==NULL){cout<<"No Chain"<<endl;return 0;} 

  _fChain->SetBranchAddress("nB",&nB);
  _fChain->SetBranchAddress("BMass",BMass);
  _fChain->SetBranchAddress("Bp3",Bp3);
  _fChain->SetBranchAddress("Bcosth",Bcosth);
  _fChain->SetBranchAddress("Bphi",Bphi);
  _fChain->SetBranchAddress("Bp3CM",Bp3CM);
  _fChain->SetBranchAddress("BcosthCM",BcosthCM);
  _fChain->SetBranchAddress("BphiCM",BphiCM);
  _fChain->SetBranchAddress("BLund",BLund); 
  _fChain->SetBranchAddress("Bd1Lund",Bd1Lund);
  _fChain->SetBranchAddress("Bd1Idx",Bd1Idx);
  _fChain->SetBranchAddress("Bd2Lund",Bd2Lund);
  _fChain->SetBranchAddress("Bd2Idx",Bd2Idx);
  _fChain->SetBranchAddress("Bd3Lund",Bd3Lund);
  _fChain->SetBranchAddress("Bd3Idx",Bd3Idx);
  _fChain->SetBranchAddress("BVtxx",BVtxx);
  _fChain->SetBranchAddress("BVtxy",BVtxy);
  _fChain->SetBranchAddress("BVtxz",BVtxz);
  _fChain->SetBranchAddress("BChi2",BChi2);
  _fChain->SetBranchAddress("BnDof",BnDof);
  _fChain->SetBranchAddress("BVtxStatus",BVtxStatus);
  _fChain->SetBranchAddress("BmES",BmES);
  _fChain->SetBranchAddress("BDeltaE",BDeltaE);
  _fChain->SetBranchAddress("BROEthrust",BROEthrust);
  _fChain->SetBranchAddress("Bsph",Bsph);
  _fChain->SetBranchAddress("BR2",BR2);
  _fChain->SetBranchAddress("Bthrust",Bthrust);
  _fChain->SetBranchAddress("BMsq12",BMsq12);
  _fChain->SetBranchAddress("BMsq31",BMsq31);
  _fChain->SetBranchAddress("BFlightBSCosA",BFlightBSCosA);
  _fChain->SetBranchAddress("BFlightBSLen",BFlightBSLen);
  _fChain->SetBranchAddress("BFlightBSErr",BFlightBSErr);



  if(_TruthMatch){
    _fChain->SetBranchAddress("BMCIdx",BMCIdx);
    _fChain->SetBranchAddress("BMyMCIdx",BMyMCIdx);
  } 

  SetDstBranches();
  cout<<"Done linking BToDstPiPi Chain Branches"<<endl;
  return 1;
}



Int_t BToDstPiPiAnalysis::MakeBToDstPiPiReducedNtupleBranches(TTree* Ntuple){
 
  Ntuple->Branch("bmass",&bmass,"bmass/F");
  Ntuple->Branch("bmes",&bmes,"bmes/F");
  Ntuple->Branch("benergy",&benergy,"benergy/F");
  Ntuple->Branch("bdeltae",&bdeltae,"bdeltae/F");
  Ntuple->Branch("bmesres",&bmesres,"bmesres/F");
  Ntuple->Branch("bpstar",&bpstar,"bpstar/F");
  Ntuple->Branch("bcosstar",&bcosstar,"bcosstar/F");
  Ntuple->Branch("bphistar",&bphistar,"bphistar/F");
  Ntuple->Branch("bcharge",&bcharge,"bcharge/I");
  Ntuple->Branch("nB",&nB,"nB/I");
  Ntuple->Branch("blogvtxprob",&blogvtxprob,"blogvtxprob/F");
  Ntuple->Branch("BIdx",&BIdx,"BIdx/I");
  Ntuple->Branch("bbestcand",&bbestcand,"bbestcand/I");
  Ntuple->Branch("bmctrue",&bmctrue,"bmctrue/I"); 
  Ntuple->Branch("bvtxstatus",&bvtxstatus,"bvtxstatus/I");
  Ntuple->Branch("bdstpi1mass",&bdstpi1mass,"bdstpi1mass/F");
  Ntuple->Branch("bdstpi2mass",&bdstpi2mass,"bdstpi2mass/F");
  Ntuple->Branch("bdstpi1massfolded",&bdstpi1massfolded,"bdstpi1massfolded/F");
  Ntuple->Branch("bdstpi2massfolded",&bdstpi2massfolded,"bdstpi2massfolded/F");
  Ntuple->Branch("bdstpi1decayangle",&bdstpi1decayangle,"bdstpi1decayangle/F");
  Ntuple->Branch("bdstpi2decayangle",&bdstpi2decayangle,"bdstpi2decayangle/F");
  Ntuple->Branch("bdstpi1decayanglefolded",&bdstpi1decayanglefolded,"bdstpi1decayanglefolded/F");
  Ntuple->Branch("bdstpi2decayanglefolded",&bdstpi2decayanglefolded,"bdstpi2decayanglefolded/F");
  Ntuple->Branch("bflights",&bflights,"bflights/F");
  Ntuple->Branch("bthrust",&bthrust,"bthrust/F");
  Ntuple->Branch("eventthrust",&eventthrust,"eventthrust/F");
  Ntuple->Branch("eventsph",&eventsph,"eventsph/F");
  Ntuple->Branch("eventr2",&eventr2,"eventr2/F");
    
  MakeDstReducedNtupleBranches(Ntuple);
  cout<<"Done Making  BToDstPiPi  Reduced Ntuple Branches"<<endl;
  return 1;
}



Int_t BToDstPiPiAnalysis::SetBToDstPiPiReducedNtupleBranches(){


  ReducedNtuple->SetBranchAddress("bmass",&bmass);
  ReducedNtuple->SetBranchAddress("bmes",&bmes);
  ReducedNtuple->SetBranchAddress("benergy",&benergy);
  ReducedNtuple->SetBranchAddress("bdeltae",&bdeltae);
  ReducedNtuple->SetBranchAddress("bmesres",&bmesres);
  ReducedNtuple->SetBranchAddress("bpstar",&bpstar);
  ReducedNtuple->SetBranchAddress("bcosstar",&bcosstar);
  ReducedNtuple->SetBranchAddress("bphistar",&bphistar);
  ReducedNtuple->SetBranchAddress("BIdx",&BIdx);
  ReducedNtuple->SetBranchAddress("bcharge",&bcharge);
  ReducedNtuple->SetBranchAddress("nB",&nB);
  ReducedNtuple->SetBranchAddress("blogvtxprob",&blogvtxprob);
  ReducedNtuple->SetBranchAddress("bbestcand",&bbestcand);
  ReducedNtuple->SetBranchAddress("bmctrue",&bmctrue);
  ReducedNtuple->SetBranchAddress("bvtxstatus",&bvtxstatus);
  ReducedNtuple->SetBranchAddress("bdstpi1mass",&bdstpi1mass);
  ReducedNtuple->SetBranchAddress("bdstpi2mass",&bdstpi2mass);
  ReducedNtuple->SetBranchAddress("bdstpi1massfolded",&bdstpi1massfolded);
  ReducedNtuple->SetBranchAddress("bdstpi2massfolded",&bdstpi2massfolded);
  ReducedNtuple->SetBranchAddress("bdstpi1decayangle",&bdstpi1decayangle);
  ReducedNtuple->SetBranchAddress("bdstpi2decayangle",&bdstpi2decayangle);
  ReducedNtuple->SetBranchAddress("bdstpi1decayanglefolded",&bdstpi1decayanglefolded);
  ReducedNtuple->SetBranchAddress("bdstpi2decayanglefolded",&bdstpi2decayanglefolded);
  ReducedNtuple->SetBranchAddress("bflights",&bflights);
  ReducedNtuple->SetBranchAddress("bthrust",&bthrust);
  ReducedNtuple->SetBranchAddress("eventthrust",&eventthrust);
  ReducedNtuple->SetBranchAddress("eventsph",&eventsph);
  ReducedNtuple->SetBranchAddress("eventr2",&eventr2);


  SetDstReducedNtupleBranches();
  cout<<"Done linking BToDstPiPi ReducedNtuple Branches"<<endl;
  return 1;
}
