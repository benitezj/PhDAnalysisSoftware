#include "../../MACROS/TreeChain.C"
#include "DsRecoAnalysis.h"



DsRecoAnalysis* analyzeDsReco(Int_t WhichDATASet=-1,Bool_t ReduceNtuple=0){
  
  ////read in preferences
  TChain* chain=NULL;
  if(ReduceNtuple)chain=new TChain("ntp1"); 
  Int_t MatterOrAntiMatter=0; 
  TString OutputDir;
  Int_t TruthMatch=0;
  Bool_t MCorDATA=0;
  TString myntupledir="DsTolnu";  
  DsRecoAnalysis* a=NULL;

  if(WhichDATASet==-1){
    cout<<"Which Data Set?"<<endl;
    cout<<" "<<"0 = Run3 OffPeak"<<endl;
    cout<<"Enter id>";    

    cin>>WhichDATASet;
    cout<<endl;
  }



  if(WhichDATASet==0){
    OutputDir=myntupledir+"/SP5315/DsReco"; 
    MatterOrAntiMatter=0;   
    TruthMatch=1;
    MCorDATA=0;
    a=new DsRecoAnalysis(MCorDATA,TruthMatch,MatterOrAntiMatter,OutputDir);

    if(ReduceNtuple){
      TreeChain(chain,1,176,myntupledir+"/SP5315/DsReco/Ntuples");    
      a->SetChain(chain);
      a->SetReduceCuts(0,0,1,1,1);
    }
   
  }
  if(WhichDATASet==1){
    OutputDir=myntupledir+"/SP5315/DsReco"; 
    MatterOrAntiMatter=0;   
    TruthMatch=1;
    MCorDATA=0;
    a=new DsRecoAnalysis(MCorDATA,TruthMatch,MatterOrAntiMatter,OutputDir);

    if(ReduceNtuple){
      TreeChain(chain,1,49,myntupledir+"/SP5315/DsReco/Ntuples1");    
      a->SetChain(chain);
      a->SetReduceCuts(0,0,1,1,1);
    }
   
  }


  cout<<"The following data-set will be analyzed: "<<OutputDir<<endl;   
  
  return a;
}

