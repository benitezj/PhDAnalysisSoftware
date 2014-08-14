#include "Dst0Analysis.h"

//#include "Dst0AnalysisPlotDst0Mass.C"

ClassImp(Dst0Analysis);
Dst0Analysis::Dst0Analysis(Bool_t MCorDATA,TString OutputDir):
  D0Analysis(MCorDATA,OutputDir)
{   
  cout<<"Done Initializing the analysis"<<endl;
}

Dst0Analysis::~Dst0Analysis(){  
}

Int_t Dst0Analysis::ReduceNtuple(){
  cout<<"Going to reduce the ntuple"<<endl;

  if(_fChain==NULL){
    cout<<"No chain."<<endl;
    return 0;
  } 

  ///link the branches in the input chain
  SetDst0Branches();

  //the file must be created here so the 'new' histos and Tree is created in hard drive //OutFile=new 
  TFile OutFile(_OutputDir+"/Reduced_New.root","recreate",_OutputDir+"/Reduced_New.root",1);//1=minimal compression
  if(OutFile.IsZombie()){cout<<"Failed to create file"<<endl;return 0;}

  
  //create new reduced ntuple
  TTree Ntuple("Ntuple","Ntuple",0);
  Long64_t maxsize=10000000000;//10Gb
  cout<<" file size "<<maxsize<<endl;
  Ntuple.SetMaxTreeSize(maxsize);//split files

  //disable autosaving 
  Long64_t fautosave=20000000000;//20Gb !!
  cout<<" autosave "<<fautosave<<endl;
  Ntuple.SetAutoSave(fautosave);


  //create the branches in the Ntuple
  MakeDst0ReducedNtupleBranches(&Ntuple);


  cout<<"Total Entries in Chain="<<_fChain->GetEntries()<<endl;
  cout<<"Going to start the event loop"<<endl;
  eventid=0;
  while(_fChain->GetEntry(eventid,0)>0){   
    eventid++;
     
    if(eventid%50000==0)cout<<eventid<<" Events done."<<endl;
      
    if(nD>MAXNCANDS){
      cout<<"Too many cands at event "<<eventid<<" Only first "<<MAXNCANDS<<" of "<<nD<<" will be used"<<endl;
      nD=MAXNCANDS;      
    }


    ///Event vars
    FillEventVars();
   

    //now do real loop
    for(Dst0Idx=0;Dst0Idx<nDst0;Dst0Idx++){
      
      //should not fill any vars outside this method otherwise not saved into Dst0Pi     
      FillDst0Vars();

      //save
      Ntuple.Fill();	              
   
    }//D loop    
    
    
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

void Dst0Analysis::FillDst0Vars(){
    
  ///determine the indexes for each particle must be done at beginnig of every loop
  if(Dst0Idx>=MAXNCANDS)return;

  DIdx=Dst0d1Idx[Dst0Idx];
  PiSlowIdx=Dst0d2Idx[Dst0Idx];
  

  //-------Dst0 quantities
  dstmass=Dst0Mass[Dst0Idx];
  dstdeltam=Dst0Mass[Dst0Idx]-DMass[DIdx];
  dstp3=Dst0p3[Dst0Idx];
  dstpstar=Dst0p3CM[Dst0Idx];

  dstcharge=Dst0Lund[Dst0Idx]/abs(Dst0Lund[Dst0Idx]);

  if(TMath::Prob(Dst0Chi2[Dst0Idx],Dst0nDof[Dst0Idx])>1e-300)
    dstlogvtxprob=log10(TMath::Prob(Dst0Chi2[Dst0Idx],Dst0nDof[Dst0Idx]));
  else dstlogvtxprob=-100;


  //Pion
  pislowp3=Pip3[PiSlowIdx];
  pislowcharge=PiLund[PiSlowIdx]/abs(PiLund[PiSlowIdx]);
  
  
  FillD0Vars();
}







Int_t Dst0Analysis::SetDst0Branches(){
  if(_fChain==NULL){cout<<"No Chain"<<endl;return 0;} 

  _fChain->SetBranchAddress("nDst0",&nDst0);
  _fChain->SetBranchAddress("Dst0Mass",Dst0Mass);
  _fChain->SetBranchAddress("Dst0p3CM",Dst0p3CM); 
  _fChain->SetBranchAddress("Dst0costhCM",Dst0costhCM); 
  _fChain->SetBranchAddress("Dst0phiCM",Dst0phiCM);  
  _fChain->SetBranchAddress("Dst0p3",Dst0p3); 
  _fChain->SetBranchAddress("Dst0costh",Dst0costh); 
  _fChain->SetBranchAddress("Dst0phi",Dst0phi);  
  _fChain->SetBranchAddress("Dst0Lund",Dst0Lund);
  _fChain->SetBranchAddress("Dst0d1Lund",Dst0d1Lund);
  _fChain->SetBranchAddress("Dst0d1Idx",Dst0d1Idx);
  _fChain->SetBranchAddress("Dst0d2Lund",Dst0d2Lund);
  _fChain->SetBranchAddress("Dst0d2Idx",Dst0d2Idx);
  _fChain->SetBranchAddress("Dst0Vtxx",Dst0Vtxx);
  _fChain->SetBranchAddress("Dst0Vtxy",Dst0Vtxy);
  _fChain->SetBranchAddress("Dst0Vtxz",Dst0Vtxz);
  
  
  SetD0Branches();
  cout<<"Done linking Dst0 Chain Branches"<<endl;
  return 1;
}



Int_t Dst0Analysis::MakeDst0ReducedNtupleBranches(TTree* Ntuple){

  Ntuple->Branch("dstmass",&dstmass,"dstmass/F"); 
  Ntuple->Branch("dstdeltam",&dstdeltam,"dstdeltam/F"); 
  Ntuple->Branch("dstp3",&dstp3,"dstp3/F");
  Ntuple->Branch("dstpstar",&dstpstar,"dstpstar/F");
  Ntuple->Branch("dstcharge",&dstcharge,"dstcharge/I");

  Ntuple->Branch("pislowp3",&pislowp3,"pislowp3/F");
  Ntuple->Branch("pislowcharge",&pislowcharge,"pislowcharge/I");

  MakeD0ReducedNtupleBranches(Ntuple);
  cout<<"Done Making Dst0 Reduced Ntuple Branches"<<endl;
  return 1;
}



Int_t Dst0Analysis::SetDst0ReducedNtupleBranches(){

  ReducedNtuple->SetBranchAddress("dstmass",&dstmass);
  ReducedNtuple->SetBranchAddress("dstdeltam",&dstdeltam);
  ReducedNtuple->SetBranchAddress("dstp3",&dstp3);
  ReducedNtuple->SetBranchAddress("dstpstar",&dstpstar);
  ReducedNtuple->SetBranchAddress("dstcharge",&dstcharge);
  
  ReducedNtuple->SetBranchAddress("pislowcharge",&pislowcharge);
  ReducedNtuple->SetBranchAddress("pislowp3",&pislowp3);

  SetD0ReducedNtupleBranches();
  cout<<"Done linking Dst0 ReducedNtuple Branches"<<endl;
  return 1;
}
