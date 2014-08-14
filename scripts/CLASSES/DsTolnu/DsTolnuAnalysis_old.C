#include "DsTolnuAnalysis.h"


ClassImp(DsTolnuAnalysis);
DsTolnuAnalysis::DsTolnuAnalysis(Bool_t MCorDATA,Bool_t TruthMatch,Int_t MatterOrAntiMatter,TString OutputDir):
  TagAnalysis(MCorDATA,TruthMatch,MatterOrAntiMatter,OutputDir),
  cuttruth(0),
  cutcharge(0),
  cutlepangle(0),
  cutgamangle(0),
  cutKangle(0)
{
  cout<<"Done Initializing the analysis"<<endl;
}

DsTolnuAnalysis::~DsTolnuAnalysis(){ 
}

Int_t DsTolnuAnalysis::ReduceNtuple(Bool_t RunInTestMode){
  cout<<"Going to reduce the ntuple"<<endl;

  if(_fChain==NULL){
    cout<<"No chain."<<endl;
    return 0;
  } 

  ///link the branches in the input chain
  SetDsTolnuBranches();



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
  MakeDsTolnuReducedNtupleBranches(&Ntuple);


  //Start the event loop;
  Long_t MaxNumberEvents=1000000000;
  if(RunInTestMode)MaxNumberEvents=10000;


  //Counters
  Int_t MCBCounterTotal=0;
  Int_t RecoBCounterTotal=0;
  


  cout<<"cuts:"
      <<"  cuttruth="<<cuttruth
      <<"  cutcharge="<<cutcharge
      <<"  cutlepangle="<<cutlepangle
      <<"  cutgamangle="<<cutgamangle
      <<"  cutKangle="<<cutKangle
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
    if(nEvt>0)FillEventVars();
   

    //determine best candidate
    //DetermineBestCand();
    
    
    //now do real loop
    for(EvtIdx=0;EvtIdx<nEvt;EvtIdx++){
     
      //
      FillDsTolnuVars();          
      
      //Selection cuts:
      if(cuttruth==1)if(evtmctrue!=1)continue;
      //if(abs(cutcharge)==1)if(evtcharge!=cutcharge)continue;
      if(cutlepangle==1)if(lepangle>-.1)continue;
      if(cutgamangle==1)if(gamangle>-.1)continue;
      if(cutKangle==1)if(fabs(Kangle)<.1)continue;
      

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


void DsTolnuAnalysis::FillDsTolnuVars(){

  if(EvtIdx>=MAXNCANDS)return;
  
  ///determine the indexes for each particle before used by other methods
  TagIdx=Evtd1Idx[EvtIdx];  
  TagKIdx=Evtd2Idx[EvtIdx];
  LepIdx=Evtd3Idx[EvtIdx];
  FragIdx=Evtd4Idx[EvtIdx];
  GamIdx=Evtd5Idx[EvtIdx];


  //////fill reduced ntuple variables    
  evtmass=EvtMass[EvtIdx];
  evtpstar=Evtp3CM[EvtIdx];
  evtcosstar=EvtcosthCM[EvtIdx];
  evtphistar=EvtphiCM[EvtIdx];
  evtenergy=Evtenergy[EvtIdx];
  evtcmenergy=EvtenergyCM[EvtIdx];
  if(abs(EvtLund[EvtIdx])==513)evtcharge=0;
  else evtcharge=EvtLund[EvtIdx]/abs(EvtLund[EvtIdx]);
  evtmism=ComputeEvtMisMass();
  lepangle=ComputeLepAngle();
  evtdsmass=ComputeDsRecoilMass();
  evtdsstmass=ComputeDsStRecoilMass();
  evtdsstdeltam=evtdsstmass-evtdsmass;
  if(_MCorDATA==1) evtmctrue=1; else evtmctrue=(EvtMyDsMCIdx[EvtIdx]>=0);  
  if(_MCorDATA==1) tagmctrue=1; else tagmctrue=(EvtMyTagMCIdx[EvtIdx]>=0);  
  if(_MCorDATA==1) Kmctrue=1; else Kmctrue=(EvtMyKMCIdx[EvtIdx]>=0);  
  if(_MCorDATA==1) fragmctrue=1; else fragmctrue=(EvtMyFragMCIdx>=0);

  lepenergy=MuenergyCM[LepIdx];
  gamangle=ComputeGamAngle();
  gamenergy=Gamp3CM[GamIdx];
  Kangle=ComputeKAngle();


  fragndaus=FragnDaus[FragIdx];
  fragenergy=FragenergyCM[FragIdx];
  if(abs(FragLund[EvtIdx])==511)fragcharge=0;
  else fragcharge=FragLund[FragIdx]/abs(FragLund[FragIdx]);
 
  


  //
  FillTagVars();
 
}


Float_t DsTolnuAnalysis::ComputeEvtMisMass(){
  

  TVector3 Evtp3vec(Evtp3[EvtIdx]*sin(acos(Evtcosth[EvtIdx]))*cos(Evtphi[EvtIdx]),
		    Evtp3[EvtIdx]*sin(acos(Evtcosth[EvtIdx]))*sin(Evtphi[EvtIdx]),
		    Evtp3[EvtIdx]*Evtcosth[EvtIdx]);
  TLorentzVector Evtp4(Evtp3vec,Evtenergy[EvtIdx]);
  
   
  //TVector3 Beamp3vec(0.,0,.0);
  TVector3 Beamp3vec(eePx,eePy,eePz); 
  TLorentzVector Beamp4(Beamp3vec,eeE);
   

  return (Beamp4-Evtp4).Mag();
  
}

Float_t DsTolnuAnalysis::ComputeLepAngle(){
  TVector3 Tagp3vec(Tagp3CM[TagIdx]*sin(acos(TagcosthCM[TagIdx]))*cos(TagphiCM[TagIdx]),
		    Tagp3CM[TagIdx]*sin(acos(TagcosthCM[TagIdx]))*sin(TagphiCM[TagIdx]),
		    Tagp3CM[TagIdx]*TagcosthCM[TagIdx]);

  TVector3 Mup3vec(Mup3CM[LepIdx]*sin(acos(MucosthCM[LepIdx]))*cos(MuphiCM[LepIdx]),
		   Mup3CM[LepIdx]*sin(acos(MucosthCM[LepIdx]))*sin(MuphiCM[LepIdx]),
		   Mup3CM[LepIdx]*MucosthCM[LepIdx]);


  return (Tagp3vec.Unit())*(Mup3vec.Unit());
}
Float_t DsTolnuAnalysis::ComputeGamAngle(){
  TVector3 Tagp3vec(Tagp3CM[TagIdx]*sin(acos(TagcosthCM[TagIdx]))*cos(TagphiCM[TagIdx]),
		    Tagp3CM[TagIdx]*sin(acos(TagcosthCM[TagIdx]))*sin(TagphiCM[TagIdx]),
		    Tagp3CM[TagIdx]*TagcosthCM[TagIdx]);

  TVector3 Gamp3vec(Gamp3CM[GamIdx]*sin(acos(GamcosthCM[GamIdx]))*cos(GamphiCM[GamIdx]),
		    Gamp3CM[GamIdx]*sin(acos(GamcosthCM[GamIdx]))*sin(GamphiCM[GamIdx]),
		    Gamp3CM[GamIdx]*GamcosthCM[GamIdx]);


  return (Tagp3vec.Unit())*(Gamp3vec.Unit());
}


Float_t DsTolnuAnalysis::ComputeKAngle(){
  TVector3 Tagp3vec(Tagp3CM[TagIdx]*sin(acos(TagcosthCM[TagIdx]))*cos(TagphiCM[TagIdx]),
		    Tagp3CM[TagIdx]*sin(acos(TagcosthCM[TagIdx]))*sin(TagphiCM[TagIdx]),
		    Tagp3CM[TagIdx]*TagcosthCM[TagIdx]);

  TVector3 Kp3vec(Kp3CM[TagKIdx]*sin(acos(KcosthCM[TagKIdx]))*cos(KphiCM[TagKIdx]),
		  Kp3CM[TagKIdx]*sin(acos(KcosthCM[TagKIdx]))*sin(KphiCM[TagKIdx]),
		  Kp3CM[TagKIdx]*KcosthCM[TagKIdx]);
  

  return (Tagp3vec.Unit())*(Kp3vec.Unit());
}

Float_t DsTolnuAnalysis::ComputeDsRecoilMass(){
  
  //beams
  TVector3 Beamp3vec(eePx,eePy,eePz); 
  TLorentzVector Beamp4(Beamp3vec,eeE);

  
  //tag
  TVector3 Evtp3vec(Evtp3[EvtIdx]*sin(acos(Evtcosth[EvtIdx]))*cos(Evtphi[EvtIdx]),
		    Evtp3[EvtIdx]*sin(acos(Evtcosth[EvtIdx]))*sin(Evtphi[EvtIdx]),
		    Evtp3[EvtIdx]*Evtcosth[EvtIdx]);
  TLorentzVector Evtp4(Evtp3vec,Evtenergy[EvtIdx]);

  //lepton
  TVector3 Lepp3vec(Mup3[LepIdx]*sin(acos(Mucosth[LepIdx]))*cos(Muphi[LepIdx]),
		    Mup3[LepIdx]*sin(acos(Mucosth[LepIdx]))*sin(Muphi[LepIdx]),
		    Mup3[LepIdx]*Mucosth[LepIdx]);
  TLorentzVector Lepp4(Lepp3vec,Muenergy[LepIdx]);

  //gam
  TVector3 Gamp3vec(Gamp3[GamIdx]*sin(acos(Gamcosth[GamIdx]))*cos(Gamphi[GamIdx]),
		    Gamp3[GamIdx]*sin(acos(Gamcosth[GamIdx]))*sin(Gamphi[GamIdx]),
		    Gamp3[GamIdx]*Gamcosth[GamIdx]);
  TLorentzVector Gamp4(Gamp3vec,Gamenergy[GamIdx]);

  return (Beamp4-(Evtp4-Lepp4-Gamp4)).Mag();    
}

Float_t DsTolnuAnalysis::ComputeDsStRecoilMass(){

  //beams
  TVector3 Beamp3vec(eePx,eePy,eePz); 
  TLorentzVector Beamp4(Beamp3vec,eeE);
  
  //tag
  TVector3 Evtp3vec(Evtp3[EvtIdx]*sin(acos(Evtcosth[EvtIdx]))*cos(Evtphi[EvtIdx]),
		    Evtp3[EvtIdx]*sin(acos(Evtcosth[EvtIdx]))*sin(Evtphi[EvtIdx]),
		    Evtp3[EvtIdx]*Evtcosth[EvtIdx]);
  TLorentzVector Evtp4(Evtp3vec,Evtenergy[EvtIdx]);

  //lepton
  TVector3 Lepp3vec(Mup3[LepIdx]*sin(acos(Mucosth[LepIdx]))*cos(Muphi[LepIdx]),
		    Mup3[LepIdx]*sin(acos(Mucosth[LepIdx]))*sin(Muphi[LepIdx]),
		    Mup3[LepIdx]*Mucosth[LepIdx]);
  TLorentzVector Lepp4(Lepp3vec,Muenergy[LepIdx]);


  return (Beamp4-(Evtp4-Lepp4)).Mag();  
}


Int_t DsTolnuAnalysis::SetDsTolnuBranches(){
  if(_fChain==NULL){cout<<"No Chain"<<endl;return 0;} 

  _fChain->SetBranchAddress("nEvt",&nEvt);
  _fChain->SetBranchAddress("EvtnDaus",EvtnDaus);
  _fChain->SetBranchAddress("EvtMass",EvtMass);
  _fChain->SetBranchAddress("Evtp3",Evtp3);
  _fChain->SetBranchAddress("Evtcosth",Evtcosth);
  _fChain->SetBranchAddress("Evtphi",Evtphi);
  _fChain->SetBranchAddress("Evtenergy",Evtenergy);
  _fChain->SetBranchAddress("Evtp3CM",Evtp3CM);
  _fChain->SetBranchAddress("EvtcosthCM",EvtcosthCM);
  _fChain->SetBranchAddress("EvtphiCM",EvtphiCM);
  _fChain->SetBranchAddress("EvtenergyCM",EvtenergyCM);
  _fChain->SetBranchAddress("EvtLund",EvtLund); 
  _fChain->SetBranchAddress("Evtd1Lund",Evtd1Lund);
  _fChain->SetBranchAddress("Evtd1Idx",Evtd1Idx);
  _fChain->SetBranchAddress("Evtd2Lund",Evtd2Lund);
  _fChain->SetBranchAddress("Evtd2Idx",Evtd2Idx);
  _fChain->SetBranchAddress("Evtd3Lund",Evtd3Lund);
  _fChain->SetBranchAddress("Evtd3Idx",Evtd3Idx);
  _fChain->SetBranchAddress("Evtd4Lund",Evtd4Lund);
  _fChain->SetBranchAddress("Evtd4Idx",Evtd4Idx);
  _fChain->SetBranchAddress("Evtd5Lund",Evtd5Lund);
  _fChain->SetBranchAddress("Evtd5Idx",Evtd5Idx);


  _fChain->SetBranchAddress("nFrag",&nFrag);
  _fChain->SetBranchAddress("FragnDaus",FragnDaus);
  _fChain->SetBranchAddress("FragMass",FragMass);
  _fChain->SetBranchAddress("Fragp3",Fragp3);
  _fChain->SetBranchAddress("Fragcosth",Fragcosth);
  _fChain->SetBranchAddress("Fragphi",Fragphi);
  _fChain->SetBranchAddress("Fragenergy",Fragenergy);
  _fChain->SetBranchAddress("Fragp3CM",Fragp3CM);
  _fChain->SetBranchAddress("FragcosthCM",FragcosthCM);
  _fChain->SetBranchAddress("FragphiCM",FragphiCM);
  _fChain->SetBranchAddress("FragenergyCM",FragenergyCM);
  _fChain->SetBranchAddress("FragLund",FragLund); 
  _fChain->SetBranchAddress("Fragd1Lund",Fragd1Lund);
  _fChain->SetBranchAddress("Fragd1Idx",Fragd1Idx);
  _fChain->SetBranchAddress("Fragd2Lund",Fragd2Lund);
  _fChain->SetBranchAddress("Fragd2Idx",Fragd2Idx);
  _fChain->SetBranchAddress("Fragd3Lund",Fragd3Lund);
  _fChain->SetBranchAddress("Fragd3Idx",Fragd3Idx);
  _fChain->SetBranchAddress("Fragd4Lund",Fragd4Lund);
  _fChain->SetBranchAddress("Fragd4Idx",Fragd4Idx);
  _fChain->SetBranchAddress("Fragd5Lund",Fragd5Lund);
  _fChain->SetBranchAddress("Fragd5Idx",Fragd5Idx);



  _fChain->SetBranchAddress("nMu",&nMu);
  _fChain->SetBranchAddress("Mup3",Mup3);
  _fChain->SetBranchAddress("Mucosth",Mucosth);
  _fChain->SetBranchAddress("Muphi",Muphi);
  _fChain->SetBranchAddress("Muenergy",Muenergy);
  _fChain->SetBranchAddress("Mup3CM",Mup3CM);
  _fChain->SetBranchAddress("MucosthCM",MucosthCM);
  _fChain->SetBranchAddress("MuphiCM",MuphiCM);
  _fChain->SetBranchAddress("MuenergyCM",MuenergyCM);
  _fChain->SetBranchAddress("MuLund",MuLund); 


  _fChain->SetBranchAddress("nGam",&nGam);
  _fChain->SetBranchAddress("Gamp3",Gamp3);
  _fChain->SetBranchAddress("Gamcosth",Gamcosth);
  _fChain->SetBranchAddress("Gamphi",Gamphi);
  _fChain->SetBranchAddress("Gamenergy",Gamenergy);
  _fChain->SetBranchAddress("Gamp3CM",Gamp3CM);
  _fChain->SetBranchAddress("GamcosthCM",GamcosthCM);
  _fChain->SetBranchAddress("GamphiCM",GamphiCM);
  _fChain->SetBranchAddress("GamenergyCM",GamenergyCM);
  _fChain->SetBranchAddress("GamLund",GamLund);



  if(_TruthMatch){
    _fChain->SetBranchAddress("EvtMyDsMCIdx",EvtMyDsMCIdx);
    _fChain->SetBranchAddress("EvtMyKMCIdx",EvtMyKMCIdx);
    _fChain->SetBranchAddress("EvtMyTagMCIdx",EvtMyTagMCIdx);
    _fChain->SetBranchAddress("EvtMyFragMCIdx",EvtMyFragMCIdx);
  } 

  SetTagBranches();
  cout<<"Done linking DsTolnu Chain Branches"<<endl;
  return 1;
}



Int_t DsTolnuAnalysis::MakeDsTolnuReducedNtupleBranches(TTree* Ntuple){
 
  Ntuple->Branch("evtmass",&evtmass,"evtmass/F");
  Ntuple->Branch("evtpstar",&evtpstar,"evtpstar/F");
  Ntuple->Branch("evtenergy",&evtenergy,"evtenergy/F");
  Ntuple->Branch("evtcmenergy",&evtcmenergy,"evtcmenergy/F");
  Ntuple->Branch("evtcosstar",&evtcosstar,"evtcosstar/F");
  Ntuple->Branch("evtphistar",&evtphistar,"evtphistar/F");
  Ntuple->Branch("evtcharge",&evtcharge,"evtcharge/I");
  Ntuple->Branch("nEvt",&nEvt,"nEvt/I");
  Ntuple->Branch("EvtIdx",&EvtIdx,"EvtIdx/I");
  Ntuple->Branch("evtbestcand",&evtbestcand,"evtbestcand/I");
  Ntuple->Branch("evtmctrue",&evtmctrue,"evtmctrue/I"); 
  Ntuple->Branch("evtdsmass",&evtdsmass,"evtdsmass/F"); 
  Ntuple->Branch("evtdsstmass",&evtdsstmass,"evtdsstmass/F"); 
  Ntuple->Branch("evtdsstdeltam",&evtdsstdeltam,"evtdssdeltam/F"); 
  

  Ntuple->Branch("evtmism",&evtmism,"evtmism/F");
  Ntuple->Branch("lepenergy",&lepenergy,"lepenergy/F");
  Ntuple->Branch("lepangle",&lepangle,"lepangle/F");
  Ntuple->Branch("gamenergy",&gamenergy,"gamenergy/F");
  Ntuple->Branch("gamangle",&gamangle,"gamangle/F");
  Ntuple->Branch("Kangle",&Kangle,"Kangle/F");
  Ntuple->Branch("Kmctrue",&Kmctrue,"Kmctrue/I"); 

  Ntuple->Branch("fragenergy",&fragenergy,"fragenergy/F");
  Ntuple->Branch("fragcharge",&fragcharge,"fragcharge/I");
  Ntuple->Branch("fragndaus",&fragndaus,"fragndaus/I");
  Ntuple->Branch("fragmctrue",&fragmctrue,"fragmctrue/I");

  MakeTagReducedNtupleBranches(Ntuple);
  cout<<"Done Making  DsTolnu  Reduced Ntuple Branches"<<endl;
  return 1;
}



Int_t DsTolnuAnalysis::SetDsTolnuReducedNtupleBranches(){
  ///only necesary if looping over the reduced ntuple

  ReducedNtuple->SetBranchAddress("evtmass",&evtmass);
  ReducedNtuple->SetBranchAddress("evtenergy",&evtenergy);
  ReducedNtuple->SetBranchAddress("evtcmenergy",&evtcmenergy);
  ReducedNtuple->SetBranchAddress("evtpstar",&evtpstar);
  ReducedNtuple->SetBranchAddress("evtcosstar",&evtcosstar);
  ReducedNtuple->SetBranchAddress("evtphistar",&evtphistar);
  ReducedNtuple->SetBranchAddress("EvtIdx",&EvtIdx);
  ReducedNtuple->SetBranchAddress("evtcharge",&evtcharge);
  ReducedNtuple->SetBranchAddress("nEvt",&nEvt);
  ReducedNtuple->SetBranchAddress("evtbestcand",&evtbestcand);
  ReducedNtuple->SetBranchAddress("evtmctrue",&evtmctrue);

  ReducedNtuple->SetBranchAddress("evtmism",&evtmism);
  ReducedNtuple->SetBranchAddress("lepangle",&lepangle);
  ReducedNtuple->SetBranchAddress("gamenergy",&gamenergy);
  ReducedNtuple->SetBranchAddress("gamangle",&gamangle);
  ReducedNtuple->SetBranchAddress("Kangle",&Kangle);


  SetTagReducedNtupleBranches();
  cout<<"Done linking DsTolnu ReducedNtuple Branches"<<endl;
  return 1;
}
