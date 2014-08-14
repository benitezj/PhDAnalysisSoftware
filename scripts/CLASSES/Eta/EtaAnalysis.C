#include "EtaAnalysis.h"

#include "EtaAnalysisPlotDMass.C"


ClassImp(EtaAnalysis);
EtaAnalysis::EtaAnalysis(Bool_t MCorDATA,Bool_t TruthMatch,Int_t MatterOrAntiMatter,TString OutputDir):
  AbsAnalysis(MCorDATA,TruthMatch,MatterOrAntiMatter,OutputDir),
  mass("mass","mass",0,6),
  massfitter(&mass)  
{   
  cout<<"Done Initializing the analysis"<<endl;
}

EtaAnalysis::~EtaAnalysis(){  
}

Int_t EtaAnalysis::ReduceNtuple(Bool_t RunInTestMode){
  cout<<"Going to reduce the ntuple"<<endl;

  if(_fChain==NULL){
    cout<<"No chain."<<endl;
    return 0;
  } 

  ///link the branches in the input chain
  SetEtaBranches();

  //the file must be created here so the 'new' histos and Tree is created in hard drive //OutFile=new 
  TFile OutFile(_OutputDir+"/Reduced_New.root","recreate",_OutputDir+"/Reduced_New.root",1);//1=minimal compression
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


  //Start the event loop;
  Long_t MaxNumberEvents=1000000000;
  if(RunInTestMode)MaxNumberEvents=10000;

  //create the branches in the Ntuple
  MakeEtaReducedNtupleBranches(&Ntuple);


  cout<<"Total Entries in Chain="<<_fChain->GetEntries()<<endl;
  cout<<"Going to start the event loop"<<endl;
  eventid=0;
  while(_fChain->GetEntry(eventid,0)>0&&eventid<MaxNumberEvents){   
    eventid++;
     
    if(eventid%50000==0)cout<<eventid<<" Events done."<<endl;
     

    ///Event vars
    FillEventVars();
   

    //now do real loop
    for(EtaIdx=0;EtaIdx<nEta;EtaIdx++){

      //should not fill any vars outside this method otherwise not saved into EtaPi
      FillEtaVars();

      //save
      Ntuple.Fill();	              
   
    }//Eta loop    
    
    
  }
  
  //print summary
  cout<<"--------Summary-------"<<endl;
  cout<<"Total events = "<<eventid<<endl;


  ////Save the Ntuple and histograms
  cout<<"Going to save the ntuple and histograms"<<endl; 


  if(Ntuple.AutoSave("Overwrite")){ cout<<"Ntuple written."<<endl;}
  else{ cout<<"Failed to write ntuple"<<endl;return 0;}

  cout<<"Objects in the current file"<<endl;
  Ntuple.GetCurrentFile()->ls();

  OutFile.Close(); //this will delete all objects created inside the file if they were not written  
  cout<<OutFile.GetName()<<" has been closed."<<endl;


  return 1;
}

void EtaAnalysis::FillEtaVars(){
  
  if(EtaIdx>=MAXNCANDS)return;

  ///Fill all necesary indexes used in this method
  Gam1Idx=Etad1Idx[EtaIdx];
  Gam2Idx=Etad2Idx[EtaIdx];   

     
  //-------Eta quantities
  etamass=EtaMass[EtaIdx];
  etap3=Etap3[EtaIdx];
  etapstar=Etap3CM[EtaIdx];
  if(TMath::Prob(EtaChi2[EtaIdx],EtanDof[EtaIdx])>1e-300)
    etalogvtxprob=log10(TMath::Prob(EtaChi2[EtaIdx],EtanDof[EtaIdx]));
  else etalogvtxprob=-100;


  gam1p3=Gamp3[Gam1Idx];
  gam2p3=Gamp3[Gam2Idx];

  
}







Int_t EtaAnalysis::SetEtaBranches(){
  if(_fChain==NULL){cout<<"No Chain"<<endl;return 0;} 

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

  
  SetEventBranches();
  cout<<"Done linking Eta Chain Branches"<<endl;
  return 1;
}



Int_t EtaAnalysis::MakeEtaReducedNtupleBranches(TTree* Ntuple){

  Ntuple->Branch("etamass",&etamass,"etamass/F"); 
  Ntuple->Branch("etap3",&etap3,"etap3/F");
  Ntuple->Branch("etapstar",&etapstar,"etapstar/F");
  Ntuple->Branch("etalogvtxprob",&etalogvtxprob,"etalogvtxprob/F");
   
  Ntuple->Branch("gam1mctrue",&gam1mctrue,"gam1mctrue/I");
  Ntuple->Branch("gam1p3",&gam1p3,"gam1p3/F");


  MakeEventReducedNtupleBranches(Ntuple);
  cout<<"Done Making Eta Reduced Ntuple Branches"<<endl;
  return 1;
}



Int_t EtaAnalysis::SetEtaReducedNtupleBranches(){

  ReducedNtuple->SetBranchAddress("etamass",&etamass);
  ReducedNtuple->SetBranchAddress("etap3",&etap3);
  ReducedNtuple->SetBranchAddress("etapstar",&etapstar);
  ReducedNtuple->SetBranchAddress("etalogvtxprob",&etalogvtxprob);

  ReducedNtuple->SetBranchAddress("gam1p3",&gam1p3);
  ReducedNtuple->SetBranchAddress("gam1mctrue",&gam1mctrue);
 
  ReducedNtuple->SetBranchAddress("gam2p3",&gam2p3);
  ReducedNtuple->SetBranchAddress("gam2mctrue",&gam2mctrue);
 

  SetEventReducedNtupleBranches();
  cout<<"Done linking Eta ReducedNtuple Branches"<<endl;
  return 1;
}
