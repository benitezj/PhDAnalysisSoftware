#include "../../MACROS/TreeChain.C"
#include "DsRecoAnalysis3.h"



DsRecoAnalysis3* analyzeDsReco3(Int_t WhichDATASet=-1,Bool_t ReduceNtuple=0){
  
  ////read in preferences
  TChain* chain=NULL;
  if(ReduceNtuple)chain=new TChain("ntp1"); 
  Int_t MatterOrAntiMatter=0; 
  TString OutputDir;
  Int_t TruthMatch=0;
  Bool_t MCorDATA=0;
  TString myntupledir="DsTolnu";  
  DsRecoAnalysis3* a=NULL;

  if(WhichDATASet==-1){
    cout<<"Which Data Set?"<<endl;
    cout<<"Enter id>";    

    cin>>WhichDATASet;
    cout<<endl;
  }



  if(WhichDATASet==1){
    OutputDir=myntupledir+"/SP5315/DsReco3"; 
    MatterOrAntiMatter=0;   
    TruthMatch=1;
    MCorDATA=0;
    a=new DsRecoAnalysis3(MCorDATA,TruthMatch,MatterOrAntiMatter,OutputDir);

    if(ReduceNtuple){
      TreeChain(chain,1,176,myntupledir+"/SP5315/DsReco3/Ntuples");    
      a->SetChain(chain);
      a->SetReduceCuts(0,0,1,1,1);
    }
   
  }
  if(WhichDATASet==2){
    OutputDir=myntupledir+"/SP5315/DsReco3"; 
    MatterOrAntiMatter=0;   
    TruthMatch=1;
    MCorDATA=0;
    a=new DsRecoAnalysis3(MCorDATA,TruthMatch,MatterOrAntiMatter,OutputDir);

    if(ReduceNtuple){
      TreeChain(chain,1,49,myntupledir+"/SP5315/DsReco3/Ntuples1");    
      a->SetChain(chain);
      a->SetReduceCuts(0,0,1,1,1);
    }
   
  }


  cout<<"The following data-set will be analyzed: "<<OutputDir<<endl;   
  
  return a;
}

