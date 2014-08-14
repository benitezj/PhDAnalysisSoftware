#include "DsRecoAnalysis.h"

//#include "DsRecoAnalysisFitDsStDeltaM.C"

ClassImp(DsRecoAnalysis);
DsRecoAnalysis::DsRecoAnalysis(Bool_t MCorDATA,Bool_t TruthMatch,Int_t MatterOrAntiMatter,TString OutputDir):
  TagAnalysis(MCorDATA,TruthMatch,MatterOrAntiMatter,OutputDir),
  cuttruth(0),
  cutcharge(0),
  cutgamangle(0),
  cutKangle(0)
{
  cout<<"Done Initializing the analysis"<<endl;
}

DsRecoAnalysis::~DsRecoAnalysis(){ 
}

Int_t DsRecoAnalysis::ReduceNtuple(Bool_t RunInTestMode){
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
      <<"  cuttagmass="<<cuttagmass
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


    DsGenMCIdx=-1;
    //find the MC gen. Nu Idx
    if(_TruthMatch){
      for(Int_t mcc=0;mcc<mcLen;mcc++){
	if(abs(mcLund[mcc])==myDsLund){
	  DsGenMCIdx=mcc;
	}
      }
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
      if(cutgamangle==1)if(gamangle>-.1)continue;
      if(cutKangle==1)if(fabs(Kangle)<.1)continue;
      if(cuttagmass==1)if(fabs(tagmassdiff)>TagMassCut)continue;
      if(tagpstar<2.5)continue;

      //save
      RecoBCounterTotal++;
      Ntuple.Fill();
   
    }// loop    

    
    
      
 

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


void DsRecoAnalysis::FillDsTolnuVars(){

  if(EvtIdx>=MAXNCANDS)return;
  
  ///determine the indexes for each particle before used by other methods
  TagIdx=Evtd1Idx[EvtIdx];  
  TagKIdx=Evtd2Idx[EvtIdx];
  FragIdx=Evtd3Idx[EvtIdx];
  DsStIdx=Evtd4Idx[EvtIdx];
  DsIdx=DsStd1Idx[DsStIdx];
  GamIdx=DsStd2Idx[DsStIdx];

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
  evtdsstdeltam=evtdsstmass-evtdsmass;
  if(_MCorDATA==1) evtmctrue=1; else evtmctrue=(EvtMyDsMCIdx[EvtIdx]>=0);  
  if(_MCorDATA==1) tagmctrue=1; else tagmctrue=(EvtMyTagMCIdx[EvtIdx]>=0);  
  if(_MCorDATA==1) Kmctrue=1; else Kmctrue=(EvtMyKMCIdx[EvtIdx]>=0);  
  if(_MCorDATA==1) fragmctrue=1; else fragmctrue=(EvtMyFragMCIdx>=0);


  dsmass=DsMass[DsIdx];
  dspstar=Dsp3CM[DsIdx];
  dspx=Dsp3CM[DsIdx]*sin(acos(DscosthCM[DsIdx]))*cos(DsphiCM[DsIdx]);
  dspy=Dsp3CM[DsIdx]*sin(acos(DscosthCM[DsIdx]))*sin(DsphiCM[DsIdx]);
  dspz=Dsp3CM[DsIdx]*DscosthCM[DsIdx];  
  if(DsGenMCIdx>0){
    dspxres=dspx-mcp3CM[DsGenMCIdx]*sin(acos(mccosthCM[DsGenMCIdx]))*cos(mcphiCM[DsGenMCIdx]);
    dspyres=dspy-mcp3CM[DsGenMCIdx]*sin(acos(mccosthCM[DsGenMCIdx]))*sin(mcphiCM[DsGenMCIdx]);
    dspzres=dspz-mcp3CM[DsGenMCIdx]*mccosthCM[DsGenMCIdx];
  }else{ dspxres=100;  dspyres=100;  dspzres=100;}


  dsstmass=DsStMass[DsStIdx];
  dsstdeltam=DsStMass[DsStIdx]-DsMass[DsIdx];
  dsstpstar=DsStp3CM[DsStIdx];
 


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


Float_t DsRecoAnalysis::ComputeEvtMisMass(){
  

  TVector3 Evtp3vec(Evtp3[EvtIdx]*sin(acos(Evtcosth[EvtIdx]))*cos(Evtphi[EvtIdx]),
		    Evtp3[EvtIdx]*sin(acos(Evtcosth[EvtIdx]))*sin(Evtphi[EvtIdx]),
		    Evtp3[EvtIdx]*Evtcosth[EvtIdx]);
  TLorentzVector Evtp4(Evtp3vec,Evtenergy[EvtIdx]);
  
   
  //TVector3 Beamp3vec(0.,0,.0);
  TVector3 Beamp3vec(eePx,eePy,eePz); 
  TLorentzVector Beamp4(Beamp3vec,eeE);
   

  return (Beamp4-Evtp4).Mag();
  
}

Float_t DsRecoAnalysis::ComputeGamAngle(){
  TVector3 Tagp3vec(Tagp3CM[TagIdx]*sin(acos(TagcosthCM[TagIdx]))*cos(TagphiCM[TagIdx]),
		    Tagp3CM[TagIdx]*sin(acos(TagcosthCM[TagIdx]))*sin(TagphiCM[TagIdx]),
		    Tagp3CM[TagIdx]*TagcosthCM[TagIdx]);

  TVector3 Gamp3vec(Gamp3CM[GamIdx]*sin(acos(GamcosthCM[GamIdx]))*cos(GamphiCM[GamIdx]),
		    Gamp3CM[GamIdx]*sin(acos(GamcosthCM[GamIdx]))*sin(GamphiCM[GamIdx]),
		    Gamp3CM[GamIdx]*GamcosthCM[GamIdx]);


  return (Tagp3vec.Unit())*(Gamp3vec.Unit());
}


Float_t DsRecoAnalysis::ComputeKAngle(){
  TVector3 Tagp3vec(Tagp3CM[TagIdx]*sin(acos(TagcosthCM[TagIdx]))*cos(TagphiCM[TagIdx]),
		    Tagp3CM[TagIdx]*sin(acos(TagcosthCM[TagIdx]))*sin(TagphiCM[TagIdx]),
		    Tagp3CM[TagIdx]*TagcosthCM[TagIdx]);

  TVector3 Kp3vec(Kp3CM[TagKIdx]*sin(acos(KcosthCM[TagKIdx]))*cos(KphiCM[TagKIdx]),
		  Kp3CM[TagKIdx]*sin(acos(KcosthCM[TagKIdx]))*sin(KphiCM[TagKIdx]),
		  Kp3CM[TagKIdx]*KcosthCM[TagKIdx]);
  

  return (Tagp3vec.Unit())*(Kp3vec.Unit());
}




Int_t DsRecoAnalysis::SetDsTolnuBranches(){
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
  _fChain->SetBranchAddress("Fragd6Lund",Fragd6Lund);
  _fChain->SetBranchAddress("Fragd6Idx",Fragd6Idx);
  _fChain->SetBranchAddress("Fragd7Lund",Fragd7Lund);
  _fChain->SetBranchAddress("Fragd7Idx",Fragd7Idx);
  _fChain->SetBranchAddress("Fragd8Lund",Fragd8Lund);
  _fChain->SetBranchAddress("Fragd8Idx",Fragd8Idx);


  _fChain->SetBranchAddress("nDsSt",&nDsSt);
  _fChain->SetBranchAddress("DsStMass",DsStMass);
  _fChain->SetBranchAddress("DsStp3",DsStp3);
  _fChain->SetBranchAddress("DsStcosth",DsStcosth);
  _fChain->SetBranchAddress("DsStphi",DsStphi);
  _fChain->SetBranchAddress("DsStenergy",DsStenergy);
  _fChain->SetBranchAddress("DsStp3CM",DsStp3CM);
  _fChain->SetBranchAddress("DsStcosthCM",DsStcosthCM);
  _fChain->SetBranchAddress("DsStphiCM",DsStphiCM);
  _fChain->SetBranchAddress("DsStenergyCM",DsStenergyCM);
  _fChain->SetBranchAddress("DsStLund",DsStLund); 
  _fChain->SetBranchAddress("DsStd1Lund",DsStd1Lund);
  _fChain->SetBranchAddress("DsStd1Idx",DsStd1Idx);
  _fChain->SetBranchAddress("DsStd2Lund",DsStd2Lund);
  _fChain->SetBranchAddress("DsStd2Idx",DsStd2Idx);

  _fChain->SetBranchAddress("nDs",&nDs);
  _fChain->SetBranchAddress("DsMass",DsMass);
  _fChain->SetBranchAddress("Dsp3",Dsp3);
  _fChain->SetBranchAddress("Dscosth",Dscosth);
  _fChain->SetBranchAddress("Dsphi",Dsphi);
  _fChain->SetBranchAddress("Dsenergy",Dsenergy);
  _fChain->SetBranchAddress("Dsp3CM",Dsp3CM);
  _fChain->SetBranchAddress("DscosthCM",DscosthCM);
  _fChain->SetBranchAddress("DsphiCM",DsphiCM);
  _fChain->SetBranchAddress("DsenergyCM",DsenergyCM);
  _fChain->SetBranchAddress("DsLund",DsLund); 

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
 //    _fChain->SetBranchAddress("EvtMyDsMCIdx",EvtMyDsMCIdx);
//     _fChain->SetBranchAddress("EvtMyKMCIdx",EvtMyKMCIdx);
//     _fChain->SetBranchAddress("EvtMyTagMCIdx",EvtMyTagMCIdx);
//     _fChain->SetBranchAddress("EvtMyFragMCIdx",EvtMyFragMCIdx);
  } 

  SetTagBranches();
  cout<<"Done linking DsTolnu Chain Branches"<<endl;
  return 1;
}



Int_t DsRecoAnalysis::MakeDsTolnuReducedNtupleBranches(TTree* Ntuple){
 
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
  
  Ntuple->Branch("dspstar",&dspstar,"dspstar/F"); 
  Ntuple->Branch("dsmass",&dsmass,"dsmass/F"); 
  Ntuple->Branch("dspx",&dspx,"dspx/F"); 
  Ntuple->Branch("dspy",&dspy,"dspy/F"); 
  Ntuple->Branch("dspz",&dspz,"dspz/F"); 
  Ntuple->Branch("dspxres",&dspxres,"dspxres/F"); 
  Ntuple->Branch("dspyres",&dspyres,"dspyres/F"); 
  Ntuple->Branch("dspzres",&dspzres,"dspzres/F"); 
  

  Ntuple->Branch("dsstpstar",&dsstpstar,"dsstpstar/F"); 
  Ntuple->Branch("dsstmass",&dsstmass,"dsstmass/F"); 
  Ntuple->Branch("dsstdeltam",&dsstdeltam,"dssdeltam/F"); 

  Ntuple->Branch("evtmism",&evtmism,"evtmism/F");
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



Int_t DsRecoAnalysis::SetDsTolnuReducedNtupleBranches(){
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
  ReducedNtuple->SetBranchAddress("gamenergy",&gamenergy);
  ReducedNtuple->SetBranchAddress("gamangle",&gamangle);
  ReducedNtuple->SetBranchAddress("Kangle",&Kangle);


  SetTagReducedNtupleBranches();
  cout<<"Done linking DsTolnu ReducedNtuple Branches"<<endl;
  return 1;
}
