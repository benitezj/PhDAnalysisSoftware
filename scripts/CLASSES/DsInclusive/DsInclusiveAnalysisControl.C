#include "../../MACROS/TreeChain.C"
#include "DsInclusiveAnalysis.h"

DsInclusiveAnalysis* analyzeDsInclusive(Int_t WhichDATASet=-1,Bool_t ReduceNtuple=0){
  
  ////read in preferences
  TChain* chain=NULL;
  if(ReduceNtuple)chain=new TChain("ntp1"); 
  Int_t MatterOrAntiMatter=0; 
  TString OutputDir;
  Int_t TruthMatch=0;
  Bool_t MCorDATA=0;
  TString myntupledir="DsTolnu";  
  DsInclusiveAnalysis* a=NULL;

  if(WhichDATASet==-1){
    cout<<"Which Data Set?"<<endl;
    cout<<"Enter id>";    

    cin>>WhichDATASet;
    cout<<endl;
  }
  
  ///---------------Small Test Sample
  if(WhichDATASet==1){
    OutputDir=myntupledir+"/SP1005Sig/DsReco"; 
    MatterOrAntiMatter=0;   
    TruthMatch=1;
    MCorDATA=0;
    a=new DsInclusiveAnalysis(MCorDATA,TruthMatch,MatterOrAntiMatter,OutputDir);

    if(ReduceNtuple){   
      TreeChain(chain,1,459,myntupledir+"/SP1005Sig/DsReco/NtuplesSmall/Run3On");    
      a->SetChain(chain);
      a->SetReduceCuts(1,1,1,1,1,1,1,1);
    }   
  }
  if(WhichDATASet==11){//Generated quantities
    OutputDir=myntupledir+"/SP1005Sig/DsReco"; 
    MatterOrAntiMatter=0;   
    TruthMatch=1;
    MCorDATA=0;
    a=new DsInclusiveAnalysis(MCorDATA,TruthMatch,MatterOrAntiMatter,OutputDir);

    if(ReduceNtuple){   
      TreeChain(chain,1,10,myntupledir+"/SP1005Sig/DsReco/NtuplesGen");    
      a->SetChain(chain);
      a->SetReduceCuts(1,1,1,1,1,1,1,1);
    }   
  }



  ///---------------Full Signal MC 
  if(WhichDATASet>=20&&WhichDATASet<30){
    OutputDir=myntupledir+"/SP1005Sig/DsReco";
    if(WhichDATASet==20)//all cuts
      OutputDir+="/Final"; 
    if(WhichDATASet==21)//no Extra gamma energy cut
      OutputDir+="/NoExGamCut"; 
    if(WhichDATASet==22)//no X gams number cut
      OutputDir+="/NoXNGamCut"; 
    if(WhichDATASet==23)//no vtx prob  cut
      OutputDir+="/NoVtxCut"; 
    MatterOrAntiMatter=0;   
    TruthMatch=1;
    MCorDATA=0;
 
    a=new DsInclusiveAnalysis(MCorDATA,TruthMatch,MatterOrAntiMatter,OutputDir);
    if(ReduceNtuple){
      TreeChain(chain,1,24,myntupledir+"/SP1005Sig/DsReco/Ntuples/Run1On");    
      TreeChain(chain,1,2,myntupledir+"/SP1005Sig/DsReco/Ntuples/Run1Off");    
      TreeChain(chain,1,68,myntupledir+"/SP1005Sig/DsReco/Ntuples/Run2On");    
      TreeChain(chain,1,6,myntupledir+"/SP1005Sig/DsReco/Ntuples/Run2Off");    
      TreeChain(chain,1,34,myntupledir+"/SP1005Sig/DsReco/Ntuples/Run3On");    
      TreeChain(chain,1,2,myntupledir+"/SP1005Sig/DsReco/Ntuples/Run3Off");    
      TreeChain(chain,1,100,myntupledir+"/SP1005Sig/DsReco/Ntuples/Run4On");    
      TreeChain(chain,1,6,myntupledir+"/SP1005Sig/DsReco/Ntuples/Run4Off");  
      TreeChain(chain,1,148,myntupledir+"/SP1005Sig/DsReco/Ntuples/Run5On");    
      TreeChain(chain,1,14,myntupledir+"/SP1005Sig/DsReco/Ntuples/Run5Off");    
      TreeChain(chain,1,42,myntupledir+"/SP1005Sig/DsReco/Ntuples/Run6On");    
      TreeChain(chain,1,10,myntupledir+"/SP1005Sig/DsReco/Ntuples/Run6Off");      
      a->SetChain(chain);
      if(WhichDATASet==20)
	a->SetReduceCuts(1,1,1,1,1,1,1,1);
      if(WhichDATASet==21)
	a->SetReduceCuts(1,1,1,1,1,1,1,0);
      if(WhichDATASet==22)
	a->SetReduceCuts(1,1,1,1,1,1,0,0);
      if(WhichDATASet==23)
	a->SetReduceCuts(1,1,1,1,0,1,0,0);
    }   
    
  }

  ///---------------Small Generic ccbar sample
  if(WhichDATASet>=30&&WhichDATASet<40){
    OutputDir=myntupledir+"/SP1005Skim/DsRecoSmall"; 
    if(WhichDATASet==30)//all cuts
      OutputDir+="/Final"; 
    if(WhichDATASet==31)//no Extra gamma energy cut
      OutputDir+="/NoExGamCut"; 
    if(WhichDATASet==32)//no X gams number cut
      OutputDir+="/NoXNGamCut"; 
    if(WhichDATASet==33)//no vtx prob  cut
      OutputDir+="/NoVtxCut"; 
    MatterOrAntiMatter=0;   
    TruthMatch=0;
    MCorDATA=0;
    a=new DsInclusiveAnalysis(MCorDATA,TruthMatch,MatterOrAntiMatter,OutputDir);

    if(ReduceNtuple){
      TreeChain(chain,1,840,myntupledir+"/SP1005Skim/DsRecoSmall/Ntuples");    
      a->SetChain(chain);
      if(WhichDATASet==30)
	a->SetReduceCuts(1,1,1,1,1,1,1,1);
      if(WhichDATASet==31)
	a->SetReduceCuts(1,1,1,1,1,1,1,0);
      if(WhichDATASet==32)
	a->SetReduceCuts(1,1,1,1,1,1,0,0);
      if(WhichDATASet==33)
	a->SetReduceCuts(1,1,1,1,0,1,0,0);
    }   
  }

  ///---------------Generic ccbar MC
  if(WhichDATASet>=40&&WhichDATASet<50){
    OutputDir=myntupledir+"/SP1005Skim/DsReco"; 
    if(WhichDATASet==40)//all cuts
      OutputDir+="/Final"; 
    if(WhichDATASet==41)//no Extra gamma energy cut
      OutputDir+="/NoExGamCut"; 
    if(WhichDATASet==42)//no X gams number cut
      OutputDir+="/NoXNGamCut"; 
    if(WhichDATASet==43)//no vtx prob  cut
      OutputDir+="/NoVtxCut";    
    MatterOrAntiMatter=0;   
    TruthMatch=0;
    MCorDATA=0;
    a=new DsInclusiveAnalysis(MCorDATA,TruthMatch,MatterOrAntiMatter,OutputDir);


    if(ReduceNtuple){
      TreeChain(chain,1,590,myntupledir+"/SP1005Skim/DsReco/Ntuples/Run1"); 
      TreeChain(chain,1,1690,myntupledir+"/SP1005Skim/DsReco/Ntuples/Run2"); 
      TreeChain(chain,1,840,myntupledir+"/SP1005Skim/DsReco/Ntuples/Run3"); 
      TreeChain(chain,1,2530,myntupledir+"/SP1005Skim/DsReco/Ntuples/Run4"); 
      //TreeChain(chain,1,3668,myntupledir+"/SP1005Skim/DsReco/Ntuples/Run5"); 
      TreeChain(chain,1,1048,myntupledir+"/SP1005Skim/DsReco/Ntuples/Run6");    
      a->SetChain(chain);
      if(WhichDATASet==40)
	a->SetReduceCuts(1,1,1,1,1,1,1,1);
      if(WhichDATASet==41)
	a->SetReduceCuts(1,1,1,1,1,1,1,0);
      if(WhichDATASet==42)
	a->SetReduceCuts(1,1,1,1,1,1,0,0);
      if(WhichDATASet==43)
	a->SetReduceCuts(1,1,1,1,0,1,0,0);
    }   

  }

 


  ///---------------Generic ccbar MC WS
  if(WhichDATASet>=50&&WhichDATASet<60){
    OutputDir=myntupledir+"/SP1005Skim/DsReco"; 
    if(WhichDATASet==50)//all cuts
      OutputDir+="/FinalWS"; 
    MatterOrAntiMatter=0;   
    TruthMatch=0;
    MCorDATA=0;
    a=new DsInclusiveAnalysis(MCorDATA,TruthMatch,MatterOrAntiMatter,OutputDir);

    if(ReduceNtuple){
      TreeChain(chain,1,840,myntupledir+"/SP1005Skim/DsReco/NtuplesWS/Run3"); 
      a->SetChain(chain);
      if(WhichDATASet==50)
	a->SetReduceCuts(1,1,1,1,1,1,1,1);
    }   

  }

  ///Tag- Optimization
  if(WhichDATASet>=60&&WhichDATASet<70){
    OutputDir=myntupledir+"/SP1005Skim/TagOpt"; 
    if(WhichDATASet==60)//all cuts
      OutputDir+="/Final"; 
    MatterOrAntiMatter=0;   
    TruthMatch=0;
    MCorDATA=0;
    a=new DsInclusiveAnalysis(MCorDATA,TruthMatch,MatterOrAntiMatter,OutputDir);


    if(ReduceNtuple){
      TreeChain(chain,1,590,myntupledir+"/SP1005Skim/TagOpt/Ntuples/Run1"); 
      //TreeChain(chain,1,1690,myntupledir+"/SP1005Skim/TagOpt/Ntuples/Run2"); 
      //TreeChain(chain,1,840,myntupledir+"/SP1005Skim/TagOpt/Ntuples/Run3");
      //TreeChain(chain,1,2530,myntupledir+"/SP1005Skim/TagOpt/Ntuples/Run4"); 
      //TreeChain(chain,1,3668,myntupledir+"/SP1005Skim/TagOpt/Ntuples/Run5"); 
      //TreeChain(chain,1,1048,myntupledir+"/SP1005Skim/TagOpt/Ntuples/Run6");    
      a->SetChain(chain);
      if(WhichDATASet==60)
	a->SetReduceCuts(1,0,1,1,1,1,1,1);
    }   

  }

  
  //-------------uds 
  if(WhichDATASet>=70&&WhichDATASet<80){
    OutputDir=myntupledir+"/SP998Skim/DsReco"; 
    if(WhichDATASet==70)
      OutputDir+=""; 
    MatterOrAntiMatter=0;   
    TruthMatch=0;
    MCorDATA=0;
    a=new DsInclusiveAnalysis(MCorDATA,TruthMatch,MatterOrAntiMatter,OutputDir);
    if(ReduceNtuple){
      TreeChain(chain,1,670,myntupledir+"/SP998Skim/DsReco/Run3"); 
      a->SetChain(chain);
      if(WhichDATASet==70)
	a->SetReduceCuts(1,1,1,1,1,1,1,1);
    }   

  }


  cout<<"The following data-set will be analyzed: "<<OutputDir<<endl;   
  
  return a;
}

