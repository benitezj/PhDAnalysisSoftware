#include "BToDstEtaAnalysis.h"

/////////
#include "BToDstEtaAnalysisMES.C"
#include "BToDstEtaAnalysisPlotEtaMass.C"

ClassImp(BToDstEtaAnalysis);
BToDstEtaAnalysis::BToDstEtaAnalysis(Bool_t MCorDATA,Bool_t TruthMatch,Int_t MatterOrAntiMatter,TString OutputDir):
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

BToDstEtaAnalysis::~BToDstEtaAnalysis(){ 
}

Int_t BToDstEtaAnalysis::ReduceNtuple(Bool_t RunInTestMode){
  cout<<"Going to reduce the ntuple"<<endl;

  if(_fChain==NULL){
    cout<<"No chain."<<endl;
    return 0;
  } 

  ///link the branches in the input chain
  SetBToDstEtaBranches();



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
  MakeBToDstEtaReducedNtupleBranches(&Ntuple);


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
    
    bselectindex=0;
    //now do real loop
    for(BIdx=0;BIdx<nB;BIdx++){
     
 
      //
      FillBToDstEtaVars();     
      
      
      //Selection cuts:
      if(cuttruth==1)if(bmctrue!=1)continue;
      if(cutcharge==-1)if(bcharge==1)continue;      
      if(cutcharge==1)if(bcharge==-1)continue;      
      if(cutdmass==1)if(fabs(dmass-D0PDGMass)>.02) continue;
      if(cutdstdeltam==1)if(fabs(dstdeltam-.1454)>.001) continue;
      if(cutdstdeltam==2)if(fabs(dstdeltam-.1454)<.002||.003<fabs(dstdeltam-.1454)) continue;
      if(cutthrust==1)if(bthrust>.8)continue;
      if(cutmes==1)if(fabs(bmes-BCPDGMass)>.006)continue;//2 sigma cut
      if(cutmes==2)if(bmes<5.24||5.26<bmes)continue;
      if(cutdeltae==1)if(fabs(bdeltae)>.05)continue;


      bselectindex++;

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


void BToDstEtaAnalysis::FillBToDstEtaVars(){

  if(BIdx>=MAXNCANDS)return;
  
  ///determine the indexes for each particle before used by other methods
  DstIdx=Bd1Idx[BIdx]; 
  EtaIdx=Bd2Idx[BIdx]; 
  Gam1Idx=Etad1Idx[EtaIdx];
  Gam2Idx=Etad2Idx[EtaIdx];   

  //////fill reduced ntuple variables    
  bmass=BMass[BIdx];
  bpstar=Bp3CM[BIdx];
  bcosstar=BcosthCM[BIdx];
  bphistar=BphiCM[BIdx];
  bmes=BmES[BIdx]; 
  bdeltae=BDeltaE[BIdx];  
  if(TMath::Prob(BChi2[BIdx],BnDof[BIdx])>1e-300)
    blogvtxprob=log10(TMath::Prob(BChi2[BIdx],BnDof[BIdx]));
  else blogvtxprob=-100;
  //Determine combination: xcharge=-1=D*-pi+ , +1=D*+pi-  , -2=D*-pi- , +2=D*+pi+
  bcharge=BLund[BIdx]/abs(BLund[BIdx]);
  if(_MCorDATA==1) bmctrue=1; else bmctrue=(BMyMCIdx[BIdx]>=0);  

  bflights=BFlightBSLen[BIdx]/BFlightBSErr[BIdx];
  bthrust=Bthrust[BIdx];
  
  
  //-------Eta quantities
  etamass=EtaMass[EtaIdx];
  etap3=Etap3[EtaIdx];
  etapstar=Etap3CM[EtaIdx];
  if(TMath::Prob(EtaChi2[EtaIdx],EtanDof[EtaIdx])>1e-300)
    etalogvtxprob=log10(TMath::Prob(EtaChi2[EtaIdx],EtanDof[EtaIdx]));
  else etalogvtxprob=-100;


  gam1p3=Gamp3[Gam1Idx];
  gam2p3=Gamp3[Gam2Idx];

  bdecayangle=ComputeEtaAngle();
  bdstjointangle=ComputeBDstJointAngle();


  //
  FillDstVars();
 
}


Float_t BToDstEtaAnalysis::ComputeEtaAngle(){
  //Angle of the Extra pion wrt the B direction in the B frame
  TVector3 Bp3vec(Bp3CM[BIdx]*sin(acos(BcosthCM[BIdx]))*cos(BphiCM[BIdx]),
                  Bp3CM[BIdx]*sin(acos(BcosthCM[BIdx]))*sin(BphiCM[BIdx]),
                  Bp3CM[BIdx]*BcosthCM[BIdx]);
                                                                                                                                          
  Float_t BEnergy=sqrt(Bp3vec*Bp3vec + BMass[BIdx]*BMass[BIdx]);
  TVector3 BVelvec=(1/BEnergy)*Bp3vec;
                                                                                                                                          
  TVector3 Etap3vec(Etap3CM[EtaIdx]*sin(acos(EtacosthCM[EtaIdx]))*cos(EtaphiCM[EtaIdx]),
                   Etap3CM[EtaIdx]*sin(acos(EtacosthCM[EtaIdx]))*sin(EtaphiCM[EtaIdx]),
                   Etap3CM[EtaIdx]*EtacosthCM[EtaIdx]);
                                                                                                                                          
  TLorentzVector Etap4(Etap3vec,sqrt(Etap3vec*Etap3vec+EtaMass[EtaIdx]*EtaMass[EtaIdx]));
                                                                                                                                          
  //boost the eta momentum into rest frame of B
  Etap4.Boost(-BVelvec);//watch the minus sign
  TVector3 EtaBoostedp3vec=Etap4.Vect();
                                                                                                                                          
  //dot product of boosted dstar direction with B direction
  return EtaBoostedp3vec.Unit()*Bp3vec.Unit();
                                                                                                                                          
}

Float_t BToDstEtaAnalysis::ComputeBDstJointAngle(){
  //This is the angle of the D* pion and Eta Both measured in the D* rest frame.
                                                                                                                                          
  //---------
  //Get boost direction
  //----------
  TVector3 Dstp3vec(Dstp3CM[DstIdx]*sin(acos(DstcosthCM[DstIdx]))*cos(DstphiCM[DstIdx]),
                  Dstp3CM[DstIdx]*sin(acos(DstcosthCM[DstIdx]))*sin(DstphiCM[DstIdx]),
                  Dstp3CM[DstIdx]*DstcosthCM[DstIdx]);
  Float_t DstEnergy=sqrt(Dstp3vec*Dstp3vec + DstMass[DstIdx]*DstMass[DstIdx]);
  TVector3 DstVelvec=(1/DstEnergy)*Dstp3vec;

  //------
  //Boost the D into the D* frame
  //------
  TVector3 Dp3vec(Dp3CM[DIdx]*sin(acos(DcosthCM[DIdx]))*cos(DphiCM[DIdx]),
		   Dp3CM[DIdx]*sin(acos(DcosthCM[DIdx]))*sin(DphiCM[DIdx]),
		   Dp3CM[DIdx]*DcosthCM[DIdx]);
  
  TLorentzVector Dp4(Dp3vec,sqrt(Dp3vec*Dp3vec+DMass[DIdx]*DMass[DIdx]));

  Dp4.Boost(-DstVelvec);


  //---------
  //Boost the B into the D* frame
  //---------
  TVector3 Bp3vec(Bp3CM[BIdx]*sin(acos(BcosthCM[BIdx]))*cos(BphiCM[BIdx]),
                    Bp3CM[BIdx]*sin(acos(BcosthCM[BIdx]))*sin(BphiCM[BIdx]),
                    Bp3CM[BIdx]*BcosthCM[BIdx]);
  TLorentzVector Bp4(Bp3vec,sqrt(Bp3vec*Bp3vec+BMass[BIdx]*BMass[BIdx]));
  Bp4.Boost(-DstVelvec);

  //dot product
  return ((Dp4.Vect()).Unit())*((Bp4.Vect()).Unit());
}



Int_t BToDstEtaAnalysis::SetBToDstEtaBranches(){
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
  _fChain->SetBranchAddress("Bthrust",Bthrust);
  _fChain->SetBranchAddress("BFlightBSCosA",BFlightBSCosA);
  _fChain->SetBranchAddress("BFlightBSLen",BFlightBSLen);
  _fChain->SetBranchAddress("BFlightBSErr",BFlightBSErr);


  
  _fChain->SetBranchAddress("nEta",&nEta);
  _fChain->SetBranchAddress("EtaMass",EtaMass);
  _fChain->SetBranchAddress("Etap3CM",Etap3CM); 
  _fChain->SetBranchAddress("EtacosthCM",EtacosthCM); 
  _fChain->SetBranchAddress("EtaphiCM",EtaphiCM);  
  _fChain->SetBranchAddress("Etap3",Etap3); 
  _fChain->SetBranchAddress("Etacosth",Etacosth); 
  _fChain->SetBranchAddress("Etaphi",Etaphi);  
  _fChain->SetBranchAddress("EtaLund",EtaLund);
  _fChain->SetBranchAddress("Etad1Lund",Etad1Lund);
  _fChain->SetBranchAddress("Etad1Idx",Etad1Idx);
  _fChain->SetBranchAddress("Etad2Lund",Etad2Lund);
  _fChain->SetBranchAddress("Etad2Idx",Etad2Idx);
  _fChain->SetBranchAddress("EtaChi2",EtaChi2);
  _fChain->SetBranchAddress("EtanDof",EtanDof);
  _fChain->SetBranchAddress("EtaVtxStatus",EtaVtxStatus);
  _fChain->SetBranchAddress("EtaVtxx",EtaVtxx);
  _fChain->SetBranchAddress("EtaVtxy",EtaVtxy);
  _fChain->SetBranchAddress("EtaVtxz",EtaVtxz);
  
  _fChain->SetBranchAddress("nGam",&nGam);
  _fChain->SetBranchAddress("Gamp3",Gamp3);
  _fChain->SetBranchAddress("Gamcosth",Gamcosth);
  _fChain->SetBranchAddress("Gamphi",Gamphi);
  _fChain->SetBranchAddress("Gamp3CM",Gamp3CM);
  _fChain->SetBranchAddress("GamcosthCM",GamcosthCM);
  _fChain->SetBranchAddress("GamphiCM",GamphiCM);
  _fChain->SetBranchAddress("GamLund",GamLund);


  if(_TruthMatch){
    _fChain->SetBranchAddress("BMCIdx",BMCIdx);
    _fChain->SetBranchAddress("BMyMCIdx",BMyMCIdx);
  } 

  SetDstBranches();
  cout<<"Done linking BToDstEta Chain Branches"<<endl;
  return 1;
}



Int_t BToDstEtaAnalysis::MakeBToDstEtaReducedNtupleBranches(TTree* Ntuple){
 
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
  Ntuple->Branch("bdecayangle",&bdecayangle,"bdecayangle/F");
  Ntuple->Branch("bdstjointangle",&bdstjointangle,"bdstjointangle/F");
  Ntuple->Branch("bselectindex",&bselectindex,"bselectindex/I");

  Ntuple->Branch("etamass",&etamass,"etamass/F"); 
  Ntuple->Branch("etap3",&etap3,"etap3/F");
  Ntuple->Branch("etapstar",&etapstar,"etapstar/F");
  Ntuple->Branch("etalogvtxprob",&etalogvtxprob,"etalogvtxprob/F");
   
  Ntuple->Branch("gam1mctrue",&gam1mctrue,"gam1mctrue/I");
  Ntuple->Branch("gam1p3",&gam1p3,"gam1p3/F");
    
  MakeDstReducedNtupleBranches(Ntuple);
  cout<<"Done Making  BToDstEta  Reduced Ntuple Branches"<<endl;
  return 1;
}



Int_t BToDstEtaAnalysis::SetBToDstEtaReducedNtupleBranches(){


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
  ReducedNtuple->SetBranchAddress("bdecayangle",&bdecayangle);
  ReducedNtuple->SetBranchAddress("bdstjointangle",&bdstjointangle);
  ReducedNtuple->SetBranchAddress("bselectindex",&bselectindex);


  ReducedNtuple->SetBranchAddress("etamass",&etamass);
  ReducedNtuple->SetBranchAddress("etap3",&etap3);
  ReducedNtuple->SetBranchAddress("etapstar",&etapstar);
  ReducedNtuple->SetBranchAddress("etalogvtxprob",&etalogvtxprob);

  ReducedNtuple->SetBranchAddress("gam1p3",&gam1p3);
  ReducedNtuple->SetBranchAddress("gam1mctrue",&gam1mctrue);
 
  ReducedNtuple->SetBranchAddress("gam2p3",&gam2p3);
  ReducedNtuple->SetBranchAddress("gam2mctrue",&gam2mctrue);
 

  SetDstReducedNtupleBranches();
  cout<<"Done linking BToDstEta ReducedNtuple Branches"<<endl;
  return 1;
}
