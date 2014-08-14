#include "AbsAnalysis.h"

/////////

ClassImp(AbsAnalysis);
AbsAnalysis::AbsAnalysis(Int_t MCorDATA,TString OutputDir):
  TNamed((const char*)(OutputDir+"Analysis"),"AbsAnalysis"),
  _fChain(NULL),
  _MCorDATA(MCorDATA),
  _OutputDir(OutputDir),   
  OutPutFile(NULL),
  ReducedFile(NULL),
  ReducedNtuple(NULL),
  //  topdatabase(0),
  Canvas("Canvas","Canvas"),
  filename("test.ps")
{
  cout<<"Done Initializing the analysis"<<endl;
}

AbsAnalysis::~AbsAnalysis(){  
}

Int_t AbsAnalysis::ReduceNtuple(){
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
  Long64_t maxsize=1500000000;//1.5Gb
  cout<<" file size "<<maxsize<<endl;
  Ntuple.SetMaxTreeSize(maxsize);

  //create the branches in the Ntuple
  MakeEventReducedNtupleBranches(&Ntuple);

  cout<<"Total Entries in Chain="<<_fChain->GetEntries()<<endl;
  cout<<"Going to start the event loop"<<endl;
  eventid=0;
  while(_fChain->GetEntry(eventid,0)>0){   
    eventid++;     
    if(eventid%50000==0)cout<<eventid<<" Events done."<<endl;

    FillEventVars();
      
    Ntuple.Fill();	               
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
  if(Ntuple.AutoSave("Overwrite")){ cout<<"Ntuple written."<<endl;}
  else{ cout<<"Failed to write ntuple"<<endl;return 0;}

  cout<<"Objects in the current file"<<endl;
  Ntuple.GetCurrentFile()->ls();
  OutFile.Close(); //this will delete all objects created inside the file if they were not written  
  cout<<OutFile.GetName()<<" has been closed."<<endl;


  return 1;
}
void AbsAnalysis::FillEventVars(){
  runnumber=runNumber;
  beamenergy=eeE;//

  //Boost beams into CM frame; Also should be sqrt(eeE*eeE-eePx*eePx-eePy*eePy-eePz*eePz);//CM beam energy= eeE/gamma
  TVector3 Beamsp3vec(eePx,eePy,eePz); 
  TLorentzVector Beamsp4(Beamsp3vec,eeE);
  beamcmenergy=Beamsp4.Mag();//equal to the mass of e+e-

}




Int_t AbsAnalysis::OpenReducedNtuple(){
  
  cout<<"Going to open reduced ntuple."<<endl; 


  
  if((OutPutFile=new TFile(_OutputDir+"/Ntuple.root","read")))
    if(!OutPutFile->IsZombie()){
      ReducedNtuple=new TChain("Ntuple","Ntuple");
      ReducedNtuple->Add(_OutputDir+"/Ntuple.root");
      cout<<"added "<<_OutputDir+"/Ntuple.root"<<endl;
    }
  delete OutPutFile;

  
  if(!ReducedNtuple){cout<<" No ReducedNtuple"<<endl; return 0;}
  else
    cout<<"Ntuple Extracted: "<<ReducedNtuple<<" "<<ReducedNtuple->GetName()<<"  With "<<ReducedNtuple->GetEntries()<<" Entries"<<endl;
  
  gROOT->cd();//******REMEMBER TO DO THIS WHEN OPENING A FILE otherwise new objects are created in that file******//
  
  return 1;
}
 
Int_t AbsAnalysis::CloseReducedNtuple(){
  cout<<"Going to close the file."<<endl;
  delete ReducedNtuple;   
  //delete OutPutFile;
  return 1;
}
 
Int_t AbsAnalysis::OpenReducedFile(){
  
  cout<<"Going to open Ntuple.root "<<endl; 
  
  if(!(ReducedFile=new TFile(_OutputDir+"/Ntuple.root","read"))){
    cout<<"Unable to open reduced file"<<endl;
    return 0;
    
  }
  gROOT->cd(); 

  return 1;
}

Int_t AbsAnalysis::CloseReducedFile(){
  cout<<"Going to close the file."<<endl;
 
  ReducedFile->Close();

  delete ReducedFile;
  
  return 1;
}


Double_t AbsAnalysis::ComputexyzMass(Double_t xp3,Double_t xcosth,Double_t xphi,Double_t xmass,
				    Double_t yp3,Double_t ycosth,Double_t yphi,Double_t ymass,
				    Double_t zp3,Double_t zcosth,Double_t zphi,Double_t zmass){
   
  TVector3 xp3vec(xp3*sin(acos(xcosth))*cos(xphi),
                  xp3*sin(acos(xcosth))*sin(xphi),
                  xp3*xcosth);
                   
  TLorentzVector xp4(xp3vec,sqrt(xp3vec*xp3vec+xmass*xmass));
   
  TVector3 yp3vec(yp3*sin(acos(ycosth))*cos(yphi),
                  yp3*sin(acos(ycosth))*sin(yphi),
                  yp3*ycosth);
   
  TLorentzVector yp4(yp3vec,sqrt(yp3vec*yp3vec+ymass*ymass));
 
  TVector3 zp3vec(zp3*sin(acos(zcosth))*cos(zphi),
                  zp3*sin(acos(zcosth))*sin(zphi),
                  zp3*zcosth);
   
  TLorentzVector zp4(zp3vec,sqrt(zp3vec*zp3vec+zmass*zmass));
   
                                                                                                                                          
  return (xp4+yp4+zp4).Mag();
}


Double_t AbsAnalysis::ComputexyMass(Double_t xp3,Double_t xcosth,Double_t xphi,Double_t xmass,
				   Double_t yp3,Double_t ycosth,Double_t yphi,Double_t ymass){
                                                                                                                                          
  TVector3 xp3vec(xp3*sin(acos(xcosth))*cos(xphi),
                  xp3*sin(acos(xcosth))*sin(xphi),
                  xp3*xcosth);
                                                                                                                                          
  TLorentzVector xp4(xp3vec,sqrt(xp3vec*xp3vec+xmass*xmass));
                                                                                                                                          
  TVector3 yp3vec(yp3*sin(acos(ycosth))*cos(yphi),
                  yp3*sin(acos(ycosth))*sin(yphi),
                  yp3*ycosth);
                                                                                                                                          
  TLorentzVector yp4(yp3vec,sqrt(yp3vec*yp3vec+ymass*ymass));
                                                                                                                                          
  //return the mass
  return (xp4+yp4).Mag();
                                                                                                                                          
}


Int_t AbsAnalysis::SetEventBranches(){
  if(_fChain==NULL){cout<<"No Chain"<<endl;return 0;} 
  

  _fChain->SetBranchAddress("runNumber",&runNumber);
  _fChain->SetBranchAddress("eePx",&eePx);
  _fChain->SetBranchAddress("eePy",&eePy);
  _fChain->SetBranchAddress("eePz",&eePz);
  _fChain->SetBranchAddress("eeE",&eeE);
  _fChain->SetBranchAddress("beamSX",&beamSX);
  _fChain->SetBranchAddress("beamSY",&beamSY);
  _fChain->SetBranchAddress("beamSZ",&beamSZ);

  if(_MCorDATA==0){
    _fChain->SetBranchAddress("mcLund",mcLund);
    _fChain->SetBranchAddress("mothIdx",mothIdx);
    _fChain->SetBranchAddress("dauLen",dauLen);
    _fChain->SetBranchAddress("dauIdx",dauIdx);
    _fChain->SetBranchAddress("mcLen",&mcLen);
    _fChain->SetBranchAddress("mcmass",mcmass);
    _fChain->SetBranchAddress("mcp3",mcp3);
    _fChain->SetBranchAddress("mccosth",mccosth);
    _fChain->SetBranchAddress("mcphi",mcphi);
    _fChain->SetBranchAddress("mcenergy",mcenergy);
    _fChain->SetBranchAddress("mcp3CM",mcp3CM);
    _fChain->SetBranchAddress("mccosthCM",mccosthCM); 
    _fChain->SetBranchAddress("mcphiCM",mcphiCM);
    _fChain->SetBranchAddress("mcenergyCM",mcenergyCM);
    _fChain->SetBranchAddress("mcVtxx",mcVtxx);
    _fChain->SetBranchAddress("mcVtxy",mcVtxy);
    _fChain->SetBranchAddress("mcVtxz",mcVtxz);
  }


  cout<<"Done linking Event Chain Branches"<<endl;
  return 1;
}



Int_t AbsAnalysis::MakeEventReducedNtupleBranches(TTree* Ntuple){
  
  Ntuple->Branch("eventid",&eventid,"eventid/I");
  Ntuple->Branch("runnumber",&runnumber,"runnumber/I");
  Ntuple->Branch("beamenergy",&beamenergy,"beamenergy/F");
  Ntuple->Branch("beamcmenergy",&beamcmenergy,"beamcmenergy/F");
  Ntuple->Branch("ngoodthisevent",&ngoodthisevent,"ngoodthisevent/I");
  Ntuple->Branch("goodeventid",&goodeventid,"goodeventid/I");  
  Ntuple->Branch("eePx",&eePx,"eePx/F");
  Ntuple->Branch("eePy",&eePy,"eePy/F");
  Ntuple->Branch("eePz",&eePz,"eePz/F");
  Ntuple->Branch("beamSX",&beamSX,"beamSX/F");
  Ntuple->Branch("beamSY",&beamSY,"beamSY/F");
  Ntuple->Branch("beamSZ",&beamSZ,"beamSZ/F");


  cout<<"Done Making Event Reduced Ntuple Branches"<<endl;
  return 1;
}



Int_t AbsAnalysis::SetEventReducedNtupleBranches(){
  
  ReducedNtuple->SetBranchAddress("eventid",&eventid);
  ReducedNtuple->SetBranchAddress("runnumber",&runnumber);
  ReducedNtuple->SetBranchAddress("beamenergy",&beamenergy);
  ReducedNtuple->SetBranchAddress("beamcmenergy",&beamcmenergy);

  cout<<"Done linking Event ReducedNtuple Branches"<<endl;
  return 1;
}

