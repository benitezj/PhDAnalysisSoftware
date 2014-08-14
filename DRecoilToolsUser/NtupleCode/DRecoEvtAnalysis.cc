#include "DRecoilToolsUser/NtupleCode/DRecoEvtAnalysis.hh"

ClassImp(DRecoEvtAnalysis);

DRecoEvtAnalysis::DRecoEvtAnalysis(Int_t DataType,TString OutputDir):
  TNamed((const char*)(OutputDir+"Analysis"),"Analysis"),
  _fChain(NULL),
  _DataType(DataType),
  _OutputDir(OutputDir),   
  OutPutFile(NULL),
  ReducedNtuple(NULL),
  cutrunnumber(0),
  cuteventid(0),
  filename("test.ps")
{
  Canvas=new TCanvas("Canvas","Canvas");
  cout<<"Done Initializing the DRecoEvtAnalysis analysis"<<endl;
}

DRecoEvtAnalysis::~DRecoEvtAnalysis(){
  delete Canvas;
}

Int_t DRecoEvtAnalysis::ReduceNtuple(){
  cout<<"Going to reduce the ntuple"<<endl;

  if(_fChain==NULL){
    cout<<"No chain."<<endl;
    return 0;
  } 

  ///link the branches in the input chain
  SetEventBranches();

  //the file must be created here so the 'new' histos and Tree is created in hard drive //OutFile=new 
  TFile OutFile(_OutputDir+"/Reduced_New.root","recreate",_OutputDir+"/Reduced_New.root",1);//1=minimal compression
  if(OutFile.IsZombie()){cout<<"Failed to create file"<<endl;return 0;}

  //create new reduced ntuple
  TTree Ntuple("Ntuple","Ntuple",0);
  ReducedNtuple=&Ntuple;
  //Long64_t maxsize=10000000000;//10Gb
  ReducedNtuple->SetMaxTreeSize(900000000);  
  //Long64_t fautosave=11000000000;//11Gb !! //disable autosaving 
  Ntuple.SetAutoSave(900000000);

  //create the branches in the Ntuple
  MakeEventReducedNtupleBranches();


//   cout<<"cuts:"<<endl
//       <<"cutrunnumber"<<cutrunnumber<<endl
//       <<"cuteventid"<<cuteventid<<endl;

 
  cout<<"Total Entries in Chain="<<_fChain->GetEntries()<<endl;
  cout<<"Going to start the event loop"<<endl;
  eventid=0;
  goodeventid=0;
  while(_fChain->GetEntry(eventid,0)>0){   
    eventid++;     
    if(eventid%50000==0)cout<<eventid<<" Events done."<<endl;

    if(!FillEventVars())continue;

    //some cuts go here
    if(!EvtPassesCuts())continue;

    goodeventid++;//id for good events
    ReducedNtuple->Fill();	               
  }

  //print summary
  cout<<"--------Summary-------"<<endl;
  cout<<"Total events = "<<eventid<<endl;
  cout<<"--------End Summary---"<<endl;

  ////Save the Ntuple and histograms
  cout<<"Going to save the ntuple and histograms"<<endl; 

  //---
  //TTree does AutoSave() automatically when tree size gets bigger than 10Mb (default). 
  //AutoSave() deletes the previous entry. If one calls  Write() it does not delete previous entry and one gets 2 ntuples in root file.
  //option "Overwrite" deletes then saves.
  //---
  if(ReducedNtuple->AutoSave("Overwrite")){ cout<<"Ntuple written."<<endl;}
  else{ cout<<"Failed to write ntuple"<<endl;return 0;}

  cout<<"Objects in the current file"<<endl;
  ReducedNtuple->GetCurrentFile()->ls();
  OutFile.Close(); //this will delete all objects created inside the file if they were not written  
  cout<<OutFile.GetName()<<" has been closed."<<endl;


  return 1;
}

Bool_t DRecoEvtAnalysis::FillEventVars(){
  runnumber=runNumber;
  beamenergy=eeE;
  eep3=TVector3(eePx,eePy,eePz); 
  eep4=TLorentzVector(eep3,eeE);
  eeVelVec=TVector3(eePx/eeE,eePy/eeE,eePz/eeE);
  beamSpot=TVector3(beamSX,beamSY,beamSZ);
  beamcmenergy=eep4.Mag();//equal to the mass of e+e-
  return 1;
}

Bool_t DRecoEvtAnalysis::EvtPassesCuts(){
  Bool_t pass=1;
  if(cutrunnumber>0)if(cutrunnumber!=runnumber)pass=0;
  if(cuteventid==1)if(eventid%2!=1)pass=0;//choose odd
  if(cuteventid==2)if(eventid%2!=0)pass=0;//choose even

  return pass;
}

Int_t DRecoEvtAnalysis::OpenReducedNtuple(){
  
  cout<<"Going to open reduced ntuple: "<<_OutputDir+"/Ntuple.root"<<endl; 
  
  if((OutPutFile=new TFile(_OutputDir+"/Ntuple.root","read")))
    if(!OutPutFile->IsZombie()){
      ReducedNtuple=(TTree*)OutPutFile->Get("Ntuple");
    }
  
  if(!ReducedNtuple){cout<<" No ReducedNtuple"<<endl; return 0;}
  else
    cout<<"Ntuple Extracted: "<<ReducedNtuple<<" "<<ReducedNtuple->GetName()<<"  With "<<ReducedNtuple->GetEntries()<<" Entries"<<endl;
  
  gROOT->cd();//******REMEMBER TO DO THIS WHEN OPENING A FILE otherwise new objects are created in that file******//
  
  return 1;
}
 
Int_t DRecoEvtAnalysis::CloseReducedNtuple(){
  cout<<"Going to close the file."<<endl;
  delete OutPutFile;   
  return 1;
}
 
Int_t DRecoEvtAnalysis::SetEventBranches(){
  if(_fChain==NULL){cout<<"No Chain"<<endl;return 0;} 
  
  if(_fChain->GetBranch("runNumber"))_fChain->SetBranchAddress("runNumber",&runNumber);
  if(_fChain->GetBranch("date"))_fChain->SetBranchAddress("date",&date);
  if(_fChain->GetBranch("eePx"))_fChain->SetBranchAddress("eePx",&eePx);
  if(_fChain->GetBranch("eePy"))_fChain->SetBranchAddress("eePy",&eePy);
  if(_fChain->GetBranch("eePz"))_fChain->SetBranchAddress("eePz",&eePz);
  if(_fChain->GetBranch("eeE"))_fChain->SetBranchAddress("eeE",&eeE);
  if(_fChain->GetBranch("beamSX"))_fChain->SetBranchAddress("beamSX",&beamSX);
  if(_fChain->GetBranch("beamSY"))_fChain->SetBranchAddress("beamSY",&beamSY);
  if(_fChain->GetBranch("beamSZ"))_fChain->SetBranchAddress("beamSZ",&beamSZ);
  if(_fChain->GetBranch("primVtxX"))_fChain->SetBranchAddress("primVtxX",&primVtxX);
  if(_fChain->GetBranch("primVtxY"))_fChain->SetBranchAddress("primVtxY",&primVtxY);
  if(_fChain->GetBranch("primVtxZ"))_fChain->SetBranchAddress("primVtxZ",&primVtxZ);


  if(_DataType==0 || _DataType==1){
    if(_fChain->GetBranch("mcLund"))_fChain->SetBranchAddress("mcLund",mcLund);
    if(_fChain->GetBranch("mothIdx"))_fChain->SetBranchAddress("mothIdx",mothIdx);
    if(_fChain->GetBranch("dauLen"))_fChain->SetBranchAddress("dauLen",dauLen);
    if(_fChain->GetBranch("dauIdx"))_fChain->SetBranchAddress("dauIdx",dauIdx);
    if(_fChain->GetBranch("mcLen"))_fChain->SetBranchAddress("mcLen",&mcLen);
    if(_fChain->GetBranch("mcmass"))_fChain->SetBranchAddress("mcmass",mcmass);
    if(_fChain->GetBranch("mcp3"))_fChain->SetBranchAddress("mcp3",mcp3);
    if(_fChain->GetBranch("mccosth"))_fChain->SetBranchAddress("mccosth",mccosth);
    if(_fChain->GetBranch("mcphi"))_fChain->SetBranchAddress("mcphi",mcphi);
    if(_fChain->GetBranch("mcenergy"))_fChain->SetBranchAddress("mcenergy",mcenergy);
    if(_fChain->GetBranch("mcp3CM"))_fChain->SetBranchAddress("mcp3CM",mcp3CM);
    if(_fChain->GetBranch("mccosthCM"))_fChain->SetBranchAddress("mccosthCM",mccosthCM); 
    if(_fChain->GetBranch("mcphiCM"))_fChain->SetBranchAddress("mcphiCM",mcphiCM);
    if(_fChain->GetBranch("mcenergyCM"))_fChain->SetBranchAddress("mcenergyCM",mcenergyCM);
    if(_fChain->GetBranch("mcVtxx"))_fChain->SetBranchAddress("mcVtxx",mcVtxx);
    if(_fChain->GetBranch("mcVtxy"))_fChain->SetBranchAddress("mcVtxy",mcVtxy);
    if(_fChain->GetBranch("mcVtxz"))_fChain->SetBranchAddress("mcVtxz",mcVtxz);
  }


  cout<<"Done linking Event Chain Branches"<<endl;
  return 1;
}



Int_t DRecoEvtAnalysis::MakeEventReducedNtupleBranches(){
  ReducedNtuple->Branch("eventid",&eventid,"eventid/I");
  ReducedNtuple->Branch("date",&date,"date/I");
  ReducedNtuple->Branch("runnumber",&runnumber,"runnumber/I");
  ReducedNtuple->Branch("beamenergy",&beamenergy,"beamenergy/F");
  ReducedNtuple->Branch("beamcmenergy",&beamcmenergy,"beamcmenergy/F");
  ReducedNtuple->Branch("eePx",&eePx,"eePx/F");
  ReducedNtuple->Branch("eePy",&eePy,"eePy/F");
  ReducedNtuple->Branch("eePz",&eePz,"eePz/F");
  ReducedNtuple->Branch("beamSX",&beamSX,"beamSX/F");
  ReducedNtuple->Branch("beamSY",&beamSY,"beamSY/F");
  ReducedNtuple->Branch("beamSZ",&beamSZ,"beamSZ/F");
  ReducedNtuple->Branch("goodeventid",&goodeventid,"goodeventid/I");


  cout<<"Done Making Event Reduced Ntuple Branches"<<endl;
  return 1;
}



