#include "DstAnalysis.h"

#include "DstAnalysisPlotDstMass.C"


ClassImp(DstAnalysis);
DstAnalysis::DstAnalysis(Bool_t MCorDATA,TString OutputDir):
  D0Analysis(MCorDATA,OutputDir)
{   
  cout<<"Done Initializing the analysis"<<endl;
}

DstAnalysis::~DstAnalysis(){  
}

Int_t DstAnalysis::ReduceNtuple(){
  cout<<"Going to reduce the ntuple"<<endl;

  if(_fChain==NULL){
    cout<<"No chain."<<endl;
    return 0;
  } 

  ///link the branches in the input chain
  SetDstBranches();

  //the file must be created here so the 'new' histos and Tree is created in hard drive //OutFile=new 
  TFile OutFile(_OutputDir+"/Reduced_New.root","recreate",_OutputDir+"/Reduced_New.root",1);//1=minimal compression
  if(OutFile.IsZombie()){cout<<"Failed to create file"<<endl;return 0;}

  
  //create new reduced ntuple
  TTree Ntuple("Ntuple","Ntuple",0);
  Ntuple.SetMaxTreeSize(900000000);//split files

  //disable autosaving 
  Ntuple.SetAutoSave(900000000);
 
  //create the branches in the Ntuple
  MakeDstReducedNtupleBranches(&Ntuple);


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
    for(DstIdx=0;DstIdx<nDst;DstIdx++){
      
      //should not fill any vars outside this method otherwise not saved into DstPi     
      FillDstVars();

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

void DstAnalysis::FillDstVars(){
    
  ///determine the indexes for each particle must be done at beginnig of every loop
  if(DstIdx>=MAXNCANDS)return;

  DIdx=Dstd1Idx[DstIdx];
  PiSlowIdx=Dstd2Idx[DstIdx];
  if(DIdx>=MAXNCANDS||PiSlowIdx>=MAXNCANDS)return;

  FillD0Vars();

  //-------Dst quantities
  dstmass=DstMass[DstIdx];
  dstdeltam=DstMass[DstIdx]-DMass[DIdx];
  dstp3=Dstp3[DstIdx];
  dstpstar=Dstp3CM[DstIdx];

  dstcharge=DstLund[DstIdx]/abs(DstLund[DstIdx]);

  if(TMath::Prob(DstChi2[DstIdx],DstnDof[DstIdx])>1e-300)
    dstlogvtxprob=log10(TMath::Prob(DstChi2[DstIdx],DstnDof[DstIdx]));
  else dstlogvtxprob=-100;


  //Pion
  pislowp3=Pip3[PiSlowIdx];
  pislowcharge=PiLund[PiSlowIdx]/abs(PiLund[PiSlowIdx]);
  
  
  
}







Int_t DstAnalysis::SetDstBranches(){
  if(_fChain==NULL){cout<<"No Chain"<<endl;return 0;} 

  SetD0Branches();

  _fChain->SetBranchAddress("nDst",&nDst);
  _fChain->SetBranchAddress("DstMass",DstMass);
  _fChain->SetBranchAddress("Dstp3CM",Dstp3CM); 
  _fChain->SetBranchAddress("DstcosthCM",DstcosthCM); 
  _fChain->SetBranchAddress("DstphiCM",DstphiCM);  
  _fChain->SetBranchAddress("Dstp3",Dstp3); 
  _fChain->SetBranchAddress("Dstcosth",Dstcosth); 
  _fChain->SetBranchAddress("Dstphi",Dstphi);  
  _fChain->SetBranchAddress("DstLund",DstLund);
  _fChain->SetBranchAddress("Dstd1Lund",Dstd1Lund);
  _fChain->SetBranchAddress("Dstd1Idx",Dstd1Idx);
  _fChain->SetBranchAddress("Dstd2Lund",Dstd2Lund);
  _fChain->SetBranchAddress("Dstd2Idx",Dstd2Idx);
  _fChain->SetBranchAddress("DstVtxx",DstVtxx);
  _fChain->SetBranchAddress("DstVtxy",DstVtxy);
  _fChain->SetBranchAddress("DstVtxz",DstVtxz);
  
  
  cout<<"Done linking Dst Chain Branches"<<endl;
  return 1;
}



Int_t DstAnalysis::MakeDstReducedNtupleBranches(TTree* Ntuple){

  Ntuple->Branch("dstmass",&dstmass,"dstmass/F"); 
  Ntuple->Branch("dstdeltam",&dstdeltam,"dstdeltam/F"); 
  Ntuple->Branch("dstp3",&dstp3,"dstp3/F");
  Ntuple->Branch("dstpstar",&dstpstar,"dstpstar/F");
  Ntuple->Branch("dstcharge",&dstcharge,"dstcharge/I");

  Ntuple->Branch("pislowp3",&pislowp3,"pislowp3/F");
  Ntuple->Branch("pislowcharge",&pislowcharge,"pislowcharge/I");

  MakeD0ReducedNtupleBranches(Ntuple);
  cout<<"Done Making Dst Reduced Ntuple Branches"<<endl;
  return 1;
}



Int_t DstAnalysis::SetDstReducedNtupleBranches(){

  ReducedNtuple->SetBranchAddress("dstmass",&dstmass);
  ReducedNtuple->SetBranchAddress("dstdeltam",&dstdeltam);
  ReducedNtuple->SetBranchAddress("dstp3",&dstp3);
  ReducedNtuple->SetBranchAddress("dstpstar",&dstpstar);
  ReducedNtuple->SetBranchAddress("dstcharge",&dstcharge);
  
  ReducedNtuple->SetBranchAddress("pislowcharge",&pislowcharge);
  ReducedNtuple->SetBranchAddress("pislowp3",&pislowp3);

  SetD0ReducedNtupleBranches();
  cout<<"Done linking Dst ReducedNtuple Branches"<<endl;
  return 1;
}
