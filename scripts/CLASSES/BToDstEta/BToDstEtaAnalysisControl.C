#include "../../MACROS/TreeChain.C"
#include "BToDstEtaAnalysis.h"



BToDstEtaAnalysis* analyzeBToDstEta(Int_t WhichDATASet=-1,Bool_t ReduceNtuple=0){
  
  ////read in preferences
  TChain* chain=NULL;
  if(ReduceNtuple)chain=new TChain("ntp1"); 
  Int_t MatterOrAntiMatter=0; 
  TString OutputDir;
  Int_t TruthMatch=0;
  Bool_t MCorDATA=0;
  TString myntupledir="DstarCharged/BToDstarEta";  
  BToDstEtaAnalysis* a=NULL;

  if(WhichDATASet==-1){
    cout<<"Which Data Set?"<<endl;
    cout<<" "<<"1 = Run3 OnPeak Kpi+K3pi data"<<endl;
    cout<<"Enter id>";    

    cin>>WhichDATASet;
    cout<<endl;
  }


  if(WhichDATASet==1){
    OutputDir=myntupledir+"/mES"; 
    MatterOrAntiMatter=0;   
    TruthMatch=0;
    MCorDATA=1;
    a=new BToDstEtaAnalysis(MCorDATA,TruthMatch,MatterOrAntiMatter,OutputDir);

    if(ReduceNtuple){
      TreeChain(chain,1,19,myntupledir+"/DATARun1On");
      TreeChain(chain,1,62,myntupledir+"/DATARun2On");
      TreeChain(chain,1,33,myntupledir+"/DATARun3On");
      TreeChain(chain,1,99,myntupledir+"/DATARun4On");   
      TreeChain(chain,1,131,myntupledir+"/DATARun5On");
      TreeChain(chain,1,53,myntupledir+"/DATARun6On");  
      
      a->SetChain(chain);
      a->SetReduceCuts(0,0,1,1,1,0,0);

    }
   
  }



  cout<<"The following data-set will be analyzed: "<<OutputDir<<endl;   
  
  return a;
}

