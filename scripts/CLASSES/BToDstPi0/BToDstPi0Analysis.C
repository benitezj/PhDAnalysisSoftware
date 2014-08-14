#include "BToDstPi0Analysis.h"

/////////
#include "BToDstPi0AnalysisMES.C"

ClassImp(BToDstPi0Analysis);
BToDstPi0Analysis::BToDstPi0Analysis(Bool_t MCorDATA,Bool_t TruthMatch,Int_t MatterOrAntiMatter,TString OutputDir):
  DstAnalysis(MCorDATA,TruthMatch,MatterOrAntiMatter,OutputDir),
  cuttruth(0),
  cutcharge(0),
  cutdmass(0),
  cutdstdeltam(0),
  cutthrust(0),
  cutmes(0),
  cutdeltae(0)
{

  cout<<"Done Initializing the analysis"<<endl;
}

BToDstPi0Analysis::~BToDstPi0Analysis(){ 
}

Int_t BToDstPi0Analysis::ReduceNtuple(Bool_t RunInTestMode){
  cout<<"Going to reduce the ntuple"<<endl;

  if(_fChain==NULL){
    cout<<"No chain."<<endl;
    return 0;
  } 

  ///link the branches in the input chain
  SetBToDstPi0Branches();



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
  MakeBToDstPi0ReducedNtupleBranches(&Ntuple);


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
      FillBToDstPi0Vars();     
      
      
      //Selection cuts:
      if(cuttruth==1)if(bmctrue!=1)continue;
      if(abs(cutcharge)==1)if(bcharge!=cutcharge)continue;
      if(cutdmass==1)if(fabs(dmass-D0PDGMass)>.02)continue;
      if(cutdstdeltam==1)if(fabs(dstdeltam-.1454)>.001) continue;
      if(cutdstdeltam==2)if(fabs(dstdeltam-.1454)<.002||.003<fabs(dstdeltam-.1454))continue;
      if(cutthrust==1)if(bthrust>.8)continue;
      if(cutmes==1)if(fabs(bmes-BCPDGMass)>.006)continue;//2 sigma cut
      if(cutmes==2)if(bmes<5.24||5.26<bmes)continue;
      if(cutdeltae==1)if(fabs(bdeltae)>.05)continue;
      
      if(blogvtxprob<-2)continue;//otherwise B+->D*-pi+pi+pi0 peaks a lot near threshold. Should do a fit without B mass constrain then cut on deltaE.

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


void BToDstPi0Analysis::FillBToDstPi0Vars(){

  if(BIdx>=MAXNCANDS)return;
  
  ///determine the indexes for each particle before used by other methods
  DstIdx=Bd1Idx[BIdx]; 
  Pi0Idx=Bd2Idx[BIdx];

  //////fill reduced ntuple variables    
  bmass=BMass[BIdx];
  bpstar=Bp3CM[BIdx];
  bcosstar=BcosthCM[BIdx];
  bphistar=BphiCM[BIdx];
  bmes=BmES[BIdx]; 
  bdeltae=BDeltaE[BIdx];  
  benergy=sqrt(Dstp3CM[DstIdx]*Dstp3CM[DstIdx]+DstMass[DstIdx]*DstMass[DstIdx])+sqrt(Pi0p3CM[Pi0Idx]*Pi0p3CM[Pi0Idx]+Pi0PDGMass*Pi0PDGMass);
  if(TMath::Prob(BChi2[BIdx],BnDof[BIdx])>1e-300)
    blogvtxprob=log10(TMath::Prob(BChi2[BIdx],BnDof[BIdx]));
  else blogvtxprob=-100;
  
  bcharge=BLund[BIdx]/abs(BLund[BIdx]);
 
  if(_MCorDATA==1) bmctrue=1; else bmctrue=(BMyMCIdx[BIdx]>=0);  

  bflights=BFlightBSLen[BIdx]/BFlightBSErr[BIdx];
  bthrust=Bthrust[BIdx];
  eventr2=BR2[BIdx];
  
  pi0pstar=Pi0p3CM[Pi0Idx];
  pi0angle=ComputePi0Angle();
  bdecayangle=ComputeBDecayAngle();


  //
  FillDstVars();
 
}


Float_t BToDstPi0Analysis::ComputePi0Angle(){
  //angle of pi0 w.r.t D*+ in D*+ frame

  TVector3 Dstp3vec(Dstp3CM[DstIdx]*sin(acos(DstcosthCM[DstIdx]))*cos(DstphiCM[DstIdx]),
                  Dstp3CM[DstIdx]*sin(acos(DstcosthCM[DstIdx]))*sin(DstphiCM[DstIdx]),
                  Dstp3CM[DstIdx]*DstcosthCM[DstIdx]);
   
  Float_t DstEnergy=sqrt(Dstp3vec*Dstp3vec + DstMass[DstIdx]*DstMass[DstIdx]);
  TVector3 DstVelvec=(1/DstEnergy)*Dstp3vec;
 
  TVector3 Pi0p3vec(Pi0p3CM[Pi0Idx]*sin(acos(Pi0costhCM[Pi0Idx]))*cos(Pi0phiCM[Pi0Idx]),
                      Pi0p3CM[Pi0Idx]*sin(acos(Pi0costhCM[Pi0Idx]))*sin(Pi0phiCM[Pi0Idx]),
                      Pi0p3CM[Pi0Idx]*Pi0costhCM[Pi0Idx]);
   
  TLorentzVector Pi0p4(Pi0p3vec,sqrt(Pi0p3vec*Pi0p3vec+Pi0PDGMass*Pi0PDGMass));
   
 
  Pi0p4.Boost(-DstVelvec);//watch the minus sign
  TVector3 Pi0Boostedp3vec=Pi0p4.Vect();

  
  return Pi0Boostedp3vec.Unit()*Dstp3vec.Unit();
}
  
Float_t BToDstPi0Analysis::ComputeBDecayAngle(){
  

  TVector3 Bp3vec(Bp3CM[BIdx]*sin(acos(BcosthCM[BIdx]))*cos(BphiCM[BIdx]),
                  Bp3CM[BIdx]*sin(acos(BcosthCM[BIdx]))*sin(BphiCM[BIdx]),
                  Bp3CM[BIdx]*BcosthCM[BIdx]);
   
  Float_t BEnergy=sqrt(Bp3vec*Bp3vec + BCPDGMass*BCPDGMass);
  TVector3 BVelvec=(1/BEnergy)*Bp3vec;
 
  TVector3 Pi0p3vec(Pi0p3CM[Pi0Idx]*sin(acos(Pi0costhCM[Pi0Idx]))*cos(Pi0phiCM[Pi0Idx]),
                      Pi0p3CM[Pi0Idx]*sin(acos(Pi0costhCM[Pi0Idx]))*sin(Pi0phiCM[Pi0Idx]),
                      Pi0p3CM[Pi0Idx]*Pi0costhCM[Pi0Idx]);
   
  TLorentzVector Pi0p4(Pi0p3vec,sqrt(Pi0p3vec*Pi0p3vec+Pi0PDGMass*Pi0PDGMass));
   
 
  Pi0p4.Boost(-BVelvec);//watch the minus sign
  TVector3 Pi0Boostedp3vec=Pi0p4.Vect();

  return Pi0Boostedp3vec.Unit()*Bp3vec.Unit();
  
}

Int_t BToDstPi0Analysis::SetBToDstPi0Branches(){
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
  _fChain->SetBranchAddress("BVtxx",BVtxx);
  _fChain->SetBranchAddress("BVtxy",BVtxy);
  _fChain->SetBranchAddress("BVtxz",BVtxz);
  _fChain->SetBranchAddress("BChi2",BChi2);
  _fChain->SetBranchAddress("BnDof",BnDof);
  _fChain->SetBranchAddress("BVtxStatus",BVtxStatus);
  _fChain->SetBranchAddress("BmES",BmES);
  _fChain->SetBranchAddress("BDeltaE",BDeltaE);
  _fChain->SetBranchAddress("BR2",BR2);
  _fChain->SetBranchAddress("Bthrust",Bthrust);
  _fChain->SetBranchAddress("BFlightBSCosA",BFlightBSCosA);
  _fChain->SetBranchAddress("BFlightBSLen",BFlightBSLen);
  _fChain->SetBranchAddress("BFlightBSErr",BFlightBSErr);


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



  if(_TruthMatch){
    _fChain->SetBranchAddress("BMCIdx",BMCIdx);
    _fChain->SetBranchAddress("BMyMCIdx",BMyMCIdx);
  } 

  SetDstBranches();
  cout<<"Done linking BToDstPi0 Chain Branches"<<endl;
  return 1;
}



Int_t BToDstPi0Analysis::MakeBToDstPi0ReducedNtupleBranches(TTree* Ntuple){
 
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
  Ntuple->Branch("bflights",&bflights,"bflights/F");
  Ntuple->Branch("bthrust",&bthrust,"bthrust/F");
  Ntuple->Branch("eventr2",&eventr2,"eventr2/F");
   
  Ntuple->Branch("pi0pstar",&pi0pstar,"pi0pstar/F");
  Ntuple->Branch("pi0angle",&pi0angle,"pi0angle/F");
  Ntuple->Branch("bdecayangle",&bdecayangle,"bdecayangle/F");

 
  MakeDstReducedNtupleBranches(Ntuple);
  cout<<"Done Making  BToDstPi0  Reduced Ntuple Branches"<<endl;
  return 1;
}



Int_t BToDstPi0Analysis::SetBToDstPi0ReducedNtupleBranches(){


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
  ReducedNtuple->SetBranchAddress("bflights",&bflights);
  ReducedNtuple->SetBranchAddress("bthrust",&bthrust);
  ReducedNtuple->SetBranchAddress("eventr2",&eventr2);

  ReducedNtuple->SetBranchAddress("pi0pstar",&pi0pstar);
  ReducedNtuple->SetBranchAddress("pi0angle",&pi0angle);
  ReducedNtuple->SetBranchAddress("bdecayangle",&bdecayangle);


  SetDstReducedNtupleBranches();
  cout<<"Done linking BToDstPi0 ReducedNtuple Branches"<<endl;
  return 1;
}
