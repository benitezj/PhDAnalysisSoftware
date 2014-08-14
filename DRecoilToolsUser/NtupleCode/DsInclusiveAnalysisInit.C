#include "../DRecoilToolsUser/NtupleCode/tmp/DRecoilToolsUser/NtupleCode/TreeChain.hh"
#include "../DRecoilToolsUser/NtupleCode/tmp/DRecoilToolsUser/NtupleCode/DRecoDsInclusiveAnalysis.hh"

DRecoDsInclusiveAnalysis* analyzeDsInclusive(Int_t WhichDATASet=-1,Bool_t ReduceNtuple=0){
  
  TChain* chain=NULL;
  if(ReduceNtuple)chain=new TChain("ntp1"); 
  TString OutputDir;
  Int_t DataType=0;
  TString myntupledir="DsTolnu";  
  DRecoDsInclusiveAnalysis* a=NULL;
  
  ///---------------Small Test Sample
  if(WhichDATASet==1){
    OutputDir=myntupledir+"/SP1005Sig/DsReco"; 
    DataType=0;
    a=new DRecoDsInclusiveAnalysis(DataType,OutputDir);

    if(ReduceNtuple){   
      TreeChain(chain,1,459,myntupledir+"/SP1005Sig/DsReco/NtuplesSmall/Run3On/Ntuple");    
      a->SetChain(chain);
      a->SetEvtReduceCuts(0,0);
      a->SetTagReduceCuts(1,1,0,0);	    
      a->SetDsReduceCuts(1,1,1,1,1);
    }   
  }


  cout<<"The following data-set will be analyzed: "<<OutputDir<<endl;   
  
  return a;
}

