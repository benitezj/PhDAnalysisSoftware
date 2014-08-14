#include "TagAnalysis.h"

//#include "TagAnalysisPlotDMass.C"
#include "TagAnalysisOptimize.C"


ClassImp(TagAnalysis);
TagAnalysis::TagAnalysis(Bool_t MCorDATA,Bool_t TruthMatch,Int_t MatterOrAntiMatter,TString OutputDir):
  AbsAnalysis(MCorDATA,TruthMatch,MatterOrAntiMatter,OutputDir),
  mass("mass","mass",0,6),
  massfitter(&mass)
{   
  cout<<"Done Initializing the analysis"<<endl;
}

TagAnalysis::~TagAnalysis(){  
}

Int_t TagAnalysis::ReduceNtuple(){
  cout<<"Going to reduce the ntuple"<<endl;

  if(_fChain==NULL){
    cout<<"No chain."<<endl;
    return 0;
  } 

  ///link the branches in the input chain
  SetTagBranches();

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
  MakeTagReducedNtupleBranches(&Ntuple);


  cout<<"Total Entries in Chain="<<_fChain->GetEntries()<<endl;
  cout<<"Going to start the event loop"<<endl;
  eventid=0;
  while(_fChain->GetEntry(eventid,0)>0){   
    eventid++;
     
    if(eventid%50000==0)cout<<eventid<<" Events done."<<endl;
     

    ///Event vars
    FillEventVars();
   

    //now do real loop
    for(TagIdx=0;TagIdx<nTag;TagIdx++){

      //should not fill any vars outside this method otherwise not saved
      FillTagVars();
      
      //Selection cuts
      if(cuttagmass==1)if(fabs(tagmassdiff)>TagMassCut)continue;
      if(cuttagpstar==1)if(tagpstar<2.)continue;
      if(cuttagvtxprob==1)if(taglogvtxprob<-3.)continue;
      if(cuttagmode>0)if(((taglund-1)*10+tagmode)!=cuttagmode)continue;
      //save
      Ntuple.Fill();	              
   
    }//Tag loop    
    
    
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

void TagAnalysis::FillTagVars(){
  
  if(TagIdx>=MAXNCANDS)return;

  ///Fill all necesary indexes used in this method
  d1Idx=Tagd1Idx[TagIdx];
  d2Idx=Tagd2Idx[TagIdx];   
  d3Idx=Tagd3Idx[TagIdx];
  d4Idx=Tagd4Idx[TagIdx];
  d5Idx=Tagd5Idx[TagIdx];
  
  //-------TAG quantities
  tagmass=TagMass[TagIdx]; 
  tagcharge=TagLund[TagIdx]/abs(TagLund[TagIdx]);
  if(TMath::Prob(TagChi2[TagIdx],TagnDof[TagIdx])>1e-300)
    taglogvtxprob=log10(TMath::Prob(TagChi2[TagIdx],TagnDof[TagIdx]));
  else taglogvtxprob=-100;
  tagflights=TagFlightBSLen[TagIdx];
  tagenergy=TagenergyCM[TagIdx];

  tagndaus=TagnDaus[TagIdx];
  tagmctruebad=TagMCIdx[TagIdx];


  Int_t tagnK=0;Int_t tagnPi=0;Int_t tagnPi0=0;Int_t tagnKs=0;
  if(abs(Tagd1Lund[TagIdx])==myKLund)tagnK++;
  if(abs(Tagd2Lund[TagIdx])==myKLund)tagnK++;
  if(abs(Tagd3Lund[TagIdx])==myKLund)tagnK++;
  if(abs(Tagd4Lund[TagIdx])==myKLund)tagnK++;
  if(abs(Tagd5Lund[TagIdx])==myKLund)tagnK++;
  
  if(abs(Tagd1Lund[TagIdx])==myPiLund)tagnPi++;
  if(abs(Tagd2Lund[TagIdx])==myPiLund)tagnPi++;
  if(abs(Tagd3Lund[TagIdx])==myPiLund)tagnPi++;
  if(abs(Tagd4Lund[TagIdx])==myPiLund)tagnPi++;
  if(abs(Tagd5Lund[TagIdx])==myPiLund)tagnPi++;
  
  if(abs(Tagd1Lund[TagIdx])==myPi0Lund)tagnPi0++;
  if(abs(Tagd2Lund[TagIdx])==myPi0Lund)tagnPi0++;
  if(abs(Tagd3Lund[TagIdx])==myPi0Lund)tagnPi0++;
  if(abs(Tagd4Lund[TagIdx])==myPi0Lund)tagnPi0++;
  if(abs(Tagd5Lund[TagIdx])==myPi0Lund)tagnPi0++;

  if(abs(Tagd1Lund[TagIdx])==myKS0Lund)tagnKs++;
  if(abs(Tagd2Lund[TagIdx])==myKS0Lund)tagnKs++;
  if(abs(Tagd3Lund[TagIdx])==myKS0Lund)tagnKs++;
  if(abs(Tagd4Lund[TagIdx])==myKS0Lund)tagnKs++;
  if(abs(Tagd5Lund[TagIdx])==myKS0Lund)tagnKs++;
  
  taglund=0;
  if(abs(TagLund[TagIdx])==myD0Lund){
    TagPDGMass=D0PDGMass;
    TagMassCut=.015;
    tagpstar=Tagp3CM[TagIdx]; 
    if(tagnK==1&&tagnPi==1)            tagmode=1;//3.8
    if(tagnK==1&&tagnPi==1&&tagnPi0==1)tagmode=2;//14.1
    if(tagnK==1&&tagnPi==3)            tagmode=3;//7.7
    if(tagnKs==1&&tagnPi==2)           tagmode=4;//3.
    if(tagnK==1&&tagnPi==3&&tagnPi0==1)tagmode=5;//4.1
    if(tagnKs==1&&tagnPi==2&&tagnPi0==1)tagmode=6;//5.3
    taglund=1*TagLund[TagIdx]/abs(TagLund[TagIdx]);
  }
  if(abs(TagLund[TagIdx])==myDPlusLund){
    TagPDGMass=DPlusPDGMass;
    TagMassCut=.015;
    tagpstar=Tagp3CM[TagIdx];
    if(tagnK==1&&tagnPi==2)             tagmode=1;//9.5
    if(tagnK==1&&tagnPi==2&&tagnPi0==1) tagmode=2;//5.5
    if(tagnKs==1&&tagnPi==1)            tagmode=3;//1.47
    if(tagnKs==1&&tagnPi==1&&tagnPi0==1)tagmode=4;//6.8
    if(tagnKs==1&&tagnPi==3)            tagmode=5;//3.1
    taglund=2*TagLund[TagIdx]/abs(TagLund[TagIdx]);
  }
  //check if this tag mode was generated
  if(_TruthMatch)tagmodewasgen=TagModeIsGen(taglund,tagmode);
  
  //keep flight significance of Ks in Tag-D  
  tagksflights=10.;
  if(abs(Tagd1Lund[TagIdx])==myKS0Lund)tagksflights=KsFlightBSLen[Tagd1Idx[TagIdx]]/KsFlightBSErr[Tagd1Idx[TagIdx]];
  if(abs(Tagd2Lund[TagIdx])==myKS0Lund)tagksflights=KsFlightBSLen[Tagd2Idx[TagIdx]]/KsFlightBSErr[Tagd2Idx[TagIdx]];
  if(abs(Tagd3Lund[TagIdx])==myKS0Lund)tagksflights=KsFlightBSLen[Tagd3Idx[TagIdx]]/KsFlightBSErr[Tagd3Idx[TagIdx]];
  if(abs(Tagd4Lund[TagIdx])==myKS0Lund)tagksflights=KsFlightBSLen[Tagd4Idx[TagIdx]]/KsFlightBSErr[Tagd4Idx[TagIdx]];

  tagmassdiff=tagmass-TagPDGMass;
  
  //define the K PID selector
  tagpid=5;
  if(tagnK>0){  //this tag has a K track
    tagpid=0;
    Int_t ktrkidx=-1;//find the track index for the kaon
    if(abs(Tagd1Lund[TagIdx])==myKLund)ktrkidx=KTrkIdx[Tagd1Idx[TagIdx]];
    if(abs(Tagd2Lund[TagIdx])==myKLund)ktrkidx=KTrkIdx[Tagd1Idx[TagIdx]];
    if(abs(Tagd3Lund[TagIdx])==myKLund)ktrkidx=KTrkIdx[Tagd1Idx[TagIdx]];
    if(abs(Tagd4Lund[TagIdx])==myKLund)ktrkidx=KTrkIdx[Tagd1Idx[TagIdx]];
    if(abs(Tagd5Lund[TagIdx])==myKLund)ktrkidx=KTrkIdx[Tagd1Idx[TagIdx]];
    for(Int_t i=10;i<=14;i++){//10=notpion,11=veryloose,12=loose,13=tight,14=verytight
      if((KSelectorsMap[ktrkidx] & (1<<i)) == (1<<i))
	tagpid++;//if passes selector increase 
    }
  }

}



Int_t TagAnalysis::SetTagBranches(){
  if(_fChain==NULL){cout<<"No Chain"<<endl;return 0;} 
  
  
  if(_fChain->GetBranch("nTag"))_fChain->SetBranchAddress("nTag",&nTag);
  if(_fChain->GetBranch("TagMass"))_fChain->SetBranchAddress("TagMass",TagMass);
  if(_fChain->GetBranch("Tagp3CM"))_fChain->SetBranchAddress("Tagp3CM",Tagp3CM); 
  if(_fChain->GetBranch("TagcosthCM"))_fChain->SetBranchAddress("TagcosthCM",TagcosthCM); 
  if(_fChain->GetBranch("TagphiCM"))_fChain->SetBranchAddress("TagphiCM",TagphiCM);  
  if(_fChain->GetBranch("TagenergyCM"))_fChain->SetBranchAddress("TagenergyCM",TagenergyCM);   
  if(_fChain->GetBranch("TagLund"))_fChain->SetBranchAddress("TagLund",TagLund);
  if(_fChain->GetBranch("TagnDaus"))_fChain->SetBranchAddress("TagnDaus",TagnDaus);
  if(_fChain->GetBranch("TagMCIdx"))_fChain->SetBranchAddress("TagMCIdx",TagMCIdx);
  if(_fChain->GetBranch("Tagd1Lund"))_fChain->SetBranchAddress("Tagd1Lund",Tagd1Lund);
  if(_fChain->GetBranch("Tagd1Idx"))_fChain->SetBranchAddress("Tagd1Idx",Tagd1Idx);
  if(_fChain->GetBranch("Tagd2Lund"))_fChain->SetBranchAddress("Tagd2Lund",Tagd2Lund);
  if(_fChain->GetBranch("Tagd2Idx"))_fChain->SetBranchAddress("Tagd2Idx",Tagd2Idx);
  if(_fChain->GetBranch("Tagd3Lund"))_fChain->SetBranchAddress("Tagd3Lund",Tagd3Lund);
  if(_fChain->GetBranch("Tagd3Idx"))_fChain->SetBranchAddress("Tagd3Idx",Tagd3Idx);
  if(_fChain->GetBranch("Tagd4Lund"))_fChain->SetBranchAddress("Tagd4Lund",Tagd4Lund);
  if(_fChain->GetBranch("Tagd4Idx"))_fChain->SetBranchAddress("Tagd4Idx",Tagd4Idx);
  if(_fChain->GetBranch("Tagd5Lund"))_fChain->SetBranchAddress("Tagd5Lund",Tagd5Lund);
  if(_fChain->GetBranch("Tagd5Idx"))_fChain->SetBranchAddress("Tagd5Idx",Tagd5Idx);
  if(_fChain->GetBranch("TagChi2"))_fChain->SetBranchAddress("TagChi2",TagChi2);
  if(_fChain->GetBranch("TagnDof"))_fChain->SetBranchAddress("TagnDof",TagnDof);
  if(_fChain->GetBranch("TagVtxStatus"))_fChain->SetBranchAddress("TagVtxStatus",TagVtxStatus);
  if(_fChain->GetBranch("TagVtxx"))_fChain->SetBranchAddress("TagVtxx",TagVtxx);
  if(_fChain->GetBranch("TagVtxy"))_fChain->SetBranchAddress("TagVtxy",TagVtxy);
  if(_fChain->GetBranch("TagVtxz"))_fChain->SetBranchAddress("TagVtxz",TagVtxz);
  if(_fChain->GetBranch("TagFlightBSLen"))_fChain->SetBranchAddress("TagFlightBSLen",TagFlightBSLen);
  

  if(_fChain->GetBranch("nPi"))_fChain->SetBranchAddress("nPi",&nPi);
  if(_fChain->GetBranch("PienergyCM"))_fChain->SetBranchAddress("PienergyCM",PienergyCM);
  if(_fChain->GetBranch("Pip3CM"))_fChain->SetBranchAddress("Pip3CM",Pip3CM);
  if(_fChain->GetBranch("PicosthCM"))_fChain->SetBranchAddress("PicosthCM",PicosthCM);
  if(_fChain->GetBranch("PiphiCM"))_fChain->SetBranchAddress("PiphiCM",PiphiCM);
  if(_fChain->GetBranch("PiLund"))_fChain->SetBranchAddress("PiLund",PiLund);
  if(_fChain->GetBranch("PiTrkIdx"))_fChain->SetBranchAddress("PiTrkIdx",PiTrkIdx);

  if(_fChain->GetBranch("nPi0"))_fChain->SetBranchAddress("nPi0",&nPi0);
  if(_fChain->GetBranch("Pi0Mass"))_fChain->SetBranchAddress("Pi0Mass",Pi0Mass);
  if(_fChain->GetBranch("Pi0energyCM"))_fChain->SetBranchAddress("Pi0energyCM",Pi0energyCM);
  if(_fChain->GetBranch("Pi0p3CM"))_fChain->SetBranchAddress("Pi0p3CM",Pi0p3CM);
  if(_fChain->GetBranch("Pi0costhCM"))_fChain->SetBranchAddress("Pi0costhCM",Pi0costhCM);
  if(_fChain->GetBranch("Pi0phiCM"))_fChain->SetBranchAddress("Pi0phiCM",Pi0phiCM);
  if(_fChain->GetBranch("Pi0Lund"))_fChain->SetBranchAddress("Pi0Lund",Pi0Lund);

  if(_fChain->GetBranch("nKs"))_fChain->SetBranchAddress("nKs",&nKs);
  if(_fChain->GetBranch("KsMass"))_fChain->SetBranchAddress("KsMass",KsMass);
  if(_fChain->GetBranch("KsenergyCM"))_fChain->SetBranchAddress("KsenergyCM",KsenergyCM);
  if(_fChain->GetBranch("Ksp3CM"))_fChain->SetBranchAddress("Ksp3CM",Ksp3CM);
  if(_fChain->GetBranch("KscosthCM"))_fChain->SetBranchAddress("KscosthCM",KscosthCM);
  if(_fChain->GetBranch("KsphiCM"))_fChain->SetBranchAddress("KsphiCM",KsphiCM);
  if(_fChain->GetBranch("KsLund"))_fChain->SetBranchAddress("KsLund",KsLund);
  if(_fChain->GetBranch("KsFlightBSLen"))_fChain->SetBranchAddress("KsFlightBSLen",KsFlightBSLen);
  if(_fChain->GetBranch("KsFlightBSErr"))_fChain->SetBranchAddress("KsFlightBSErr",KsFlightBSErr);

  if(_fChain->GetBranch("nK"))_fChain->SetBranchAddress("nK",&nK);
  if(_fChain->GetBranch("Kp3CM"))_fChain->SetBranchAddress("Kp3CM",Kp3CM);
  if(_fChain->GetBranch("KenergyCM"))_fChain->SetBranchAddress("KenergyCM",KenergyCM);
  if(_fChain->GetBranch("KcosthCM"))_fChain->SetBranchAddress("KcosthCM",KcosthCM);
  if(_fChain->GetBranch("KphiCM"))_fChain->SetBranchAddress("KphiCM",KphiCM);
  if(_fChain->GetBranch("KLund"))_fChain->SetBranchAddress("KLund",KLund);
  if(_fChain->GetBranch("KTrkIdx"))_fChain->SetBranchAddress("KTrkIdx",KTrkIdx);
  
  if(_fChain->GetBranch("PiSelectorsMap"))_fChain->SetBranchAddress("PiSelectorsMap",PiSelectorsMap);
  if(_fChain->GetBranch("KSelectorsMap"))_fChain->SetBranchAddress("KSelectorsMap",KSelectorsMap);

  cout<<"Done linking Tag Chain Branches"<<endl;    

  return SetEventBranches();
}

Int_t TagAnalysis::MakeTagReducedNtupleBranches(TTree* Ntuple){

  Ntuple->Branch("tagenergy",&tagenergy,"tagenergy/F"); 
  Ntuple->Branch("tagmass",&tagmass,"tagmass/F"); 
  Ntuple->Branch("tagpstar",&tagpstar,"tagpstar/F");
  Ntuple->Branch("tagcharge",&tagcharge,"tagcharge/I");
  Ntuple->Branch("taglogvtxprob",&taglogvtxprob,"taglogvtxprob/F");
  Ntuple->Branch("tagflights",&tagflights,"tagflights/F");
  Ntuple->Branch("tagmode",&tagmode,"tagmode/I");
  Ntuple->Branch("tagndaus",&tagndaus,"tagndaus/I");
  Ntuple->Branch("tagmctrue",&tagmctrue,"tagmctrue/I"); 
  Ntuple->Branch("tagmctruebad",&tagmctruebad,"tagmctruebad/I");
  Ntuple->Branch("tagmassdiff",&tagmassdiff,"tagmassdiff/F");
  Ntuple->Branch("taglund",&taglund,"taglund/I");
  Ntuple->Branch("tagmodewasgen",&tagmodewasgen,"tagmodewasgen/I");
  Ntuple->Branch("tagksflights",&tagksflights,"tagksflights/F");
  Ntuple->Branch("tagpid",&tagpid,"tagpid/I");

  MakeEventReducedNtupleBranches(Ntuple);
  cout<<"Done Making Tag Reduced Ntuple Branches"<<endl;
  return 1;
}



Int_t TagAnalysis::SetTagReducedNtupleBranches(){

  ReducedNtuple->SetBranchAddress("tagmass",&tagmass);
  ReducedNtuple->SetBranchAddress("tagpstar",&tagpstar);
  ReducedNtuple->SetBranchAddress("tagcharge",&tagcharge);
  ReducedNtuple->SetBranchAddress("taglogvtxprob",&taglogvtxprob);
  ReducedNtuple->SetBranchAddress("tagflights",&tagflights);
  ReducedNtuple->SetBranchAddress("tagmode",&tagmode);
  ReducedNtuple->SetBranchAddress("tagndaus",&tagndaus);
  ReducedNtuple->SetBranchAddress("tagmctrue",&tagmctrue); 


  SetEventReducedNtupleBranches();
  cout<<"Done linking Tag ReducedNtuple Branches"<<endl;
  return 1;
}


Bool_t TagAnalysis::TagModeIsGen(Int_t tagLund,Int_t tagMode){

  //find gen tag-D
  Int_t GenIdx=-1;    
  for(Int_t mcc=0;mcc<mcLen&&GenIdx==-1;mcc++){
    if(mcLund[mcc]==tagLund)GenIdx=mcc;
  }
  if(GenIdx<0||GenIdx>=MAXMCLEN)return 0;

  //find gen tag decay mode  
  Int_t tagnK=0;Int_t tagnPi=0;Int_t tagnPi0=0;Int_t tagnKs=0;
  Int_t NewGenIdx=GenIdx;
  if(abs(tagLund)==myDstarLund){
    //first find the D0Idx	
    if(dauLen[NewGenIdx]>=1)if(abs(mcLund[dauIdx[GenIdx]])==myD0Lund)NewGenIdx=dauIdx[GenIdx];
    if(dauLen[NewGenIdx]>=2)if(abs(mcLund[dauIdx[GenIdx]+1])==myD0Lund)NewGenIdx=dauIdx[GenIdx]+1;
    if(dauLen[NewGenIdx]>=3)if(abs(mcLund[dauIdx[GenIdx]+2])==myD0Lund)NewGenIdx=dauIdx[GenIdx]+2;
    if(dauLen[NewGenIdx]>=4)if(abs(mcLund[dauIdx[GenIdx]+3])==myD0Lund)NewGenIdx=dauIdx[GenIdx]+3;
  }
     
  if(dauLen[NewGenIdx]>=1){
    if(abs(mcLund[dauIdx[NewGenIdx]])==myKLund)tagnK++;
    if(abs(mcLund[dauIdx[NewGenIdx]])==myPiLund)tagnPi++;
    if(abs(mcLund[dauIdx[NewGenIdx]])==myPi0Lund)tagnPi0++;
    if(abs(mcLund[dauIdx[NewGenIdx]])==myKS0Lund)tagnKs++;
  }
  if(dauLen[NewGenIdx]>=2){
    if(abs(mcLund[dauIdx[NewGenIdx]+1])==myKLund)tagnK++;
    if(abs(mcLund[dauIdx[NewGenIdx]+1])==myPiLund)tagnPi++;
    if(abs(mcLund[dauIdx[NewGenIdx]+1])==myPi0Lund)tagnPi0++;
    if(abs(mcLund[dauIdx[NewGenIdx]+1])==myKS0Lund)tagnKs++;
  }
  if(dauLen[NewGenIdx]>=3){
    if(abs(mcLund[dauIdx[NewGenIdx]+2])==myKLund)tagnK++;
    if(abs(mcLund[dauIdx[NewGenIdx]+2])==myPiLund)tagnPi++;
    if(abs(mcLund[dauIdx[NewGenIdx]+2])==myPi0Lund)tagnPi0++;
    if(abs(mcLund[dauIdx[NewGenIdx]+2])==myKS0Lund)tagnKs++;
  }
  if(dauLen[NewGenIdx]>=4){
    if(abs(mcLund[dauIdx[NewGenIdx]+3])==myKLund)tagnK++;
    if(abs(mcLund[dauIdx[NewGenIdx]+3])==myPiLund)tagnPi++;
    if(abs(mcLund[dauIdx[NewGenIdx]+3])==myPi0Lund)tagnPi0++;
    if(abs(mcLund[dauIdx[NewGenIdx]+3])==myKS0Lund)tagnKs++;
  }
  if(dauLen[NewGenIdx]>=5){
    if(abs(mcLund[dauIdx[NewGenIdx]+4])==myKLund)tagnK++;
    if(abs(mcLund[dauIdx[NewGenIdx]+4])==myPiLund)tagnPi++;
    if(abs(mcLund[dauIdx[NewGenIdx]+4])==myPi0Lund)tagnPi0++;
    if(abs(mcLund[dauIdx[NewGenIdx]+4])==myKS0Lund)tagnKs++;
  }


  Int_t tagModeGen=0;
  //do the D0 modes 
  if(abs(tagLund)==myD0Lund){
    if(tagnK==1&&tagnPi==1)            tagModeGen=1;
    if(tagnK==1&&tagnPi==1&&tagnPi0==1)tagModeGen=2;
    if(tagnK==1&&tagnPi==3)            tagModeGen=3;
    if(tagnKs==1&&tagnPi==2)           tagModeGen=4;
    if(tagnK==1&&tagnPi==3&&tagnPi0==1)tagModeGen=5;
    if(tagnKs==1&&tagnPi==2&&tagnPi0==1)tagModeGen=6;
  }
  //D+modes
  if(abs(tagLund)==myDPlusLund){
    if(tagnK==1&&tagnPi==2)             tagModeGen=1;
    if(tagnK==1&&tagnPi==2&&tagnPi0==1) tagModeGen=2;
    if(tagnKs==1&&tagnPi==1)            tagModeGen=3;
    if(tagnKs==1&&tagnPi==1&&tagnPi0==1)tagModeGen=4;
    if(tagnKs==1&&tagnPi==3)            tagModeGen=5;
  }
  
  //do the D*+ modes 
  if(abs(tagLund)==myDstarLund){
    if(tagnK==1&&tagnPi==1)            tagModeGen=1;
    if(tagnK==1&&tagnPi==1&&tagnPi0==1)tagModeGen=2;
    if(tagnK==1&&tagnPi==3)            tagModeGen=3;
  }
  

  return tagMode==tagModeGen;
}          
