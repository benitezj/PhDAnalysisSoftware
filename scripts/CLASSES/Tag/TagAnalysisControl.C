#include "../../MACROS/TreeChain.C"
#include "TagAnalysis.h"

TagAnalysis* analyzeTag(Int_t WhichDATASet=-1,Bool_t ReduceNtuple=0){
  
  ////read in preferences
  TChain* chain=NULL;
  if(ReduceNtuple)chain=new TChain("ntp1"); 
  Int_t MatterOrAntiMatter=0; 
  TString OutputDir;
  Int_t TruthMatch=0;
  Bool_t MCorDATA=0;
  TString myntupledir="DsTolnu";  
  TagAnalysis* a=NULL;

  if(WhichDATASet==-1){
    cout<<"Which Data Set?"<<endl;
    cout<<"Enter id>";    

    cin>>WhichDATASet;
    cout<<endl;
  }
  
  ///---------------Small Test Sample
  if(WhichDATASet==1){
    OutputDir=myntupledir+"/SP1005Skim/TagOpt"; 
    MatterOrAntiMatter=0;   
    TruthMatch=0;
    MCorDATA=0;
    a=new TagAnalysis(MCorDATA,TruthMatch,MatterOrAntiMatter,OutputDir);

    if(ReduceNtuple){   
      TreeChain(chain,1,10,myntupledir+"/SP1005Skim/TagOpt/NtuplesData/Run3"); 
      a->SetChain(chain);
      a->SetReduceCuts(0,0,0,0,3);
    }   
  }


  ///large sample for Tag- Optimization
  if(WhichDATASet>=20&&WhichDATASet<30){
    OutputDir=myntupledir+"/SP1005Skim/TagOpt"; 
    if(WhichDATASet==20)
      OutputDir+="/Final"; 
    MatterOrAntiMatter=0;   
    TruthMatch=0;
    MCorDATA=0;
    a=new TagAnalysis(MCorDATA,TruthMatch,MatterOrAntiMatter,OutputDir);

    if(ReduceNtuple){
      TreeChain(chain,1,100,myntupledir+"/SP1005Skim/TagOpt/NtuplesData/Run3");//total 1213
      a->SetChain(chain);
      if(WhichDATASet==20)
	a->SetReduceCuts(0,0,0,0,3);
    }   

  }



  cout<<"The following data-set will be analyzed: "<<OutputDir<<endl;   
  
  return a;
}

