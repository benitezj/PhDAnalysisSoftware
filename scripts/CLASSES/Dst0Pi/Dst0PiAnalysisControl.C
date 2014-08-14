#include "../../MACROS/TreeChain.C"
#include "Dst0PiAnalysis.h"

Dst0PiAnalysis* analyzeDst0Pi(Int_t WhichDATASet,Bool_t ReduceNtuple){
  
  if(WhichDATASet==0){  
    cout<<"Which Data Set?"<<endl;
    cout<<"Enter id>";
    cin>>WhichDATASet;
    cout<<endl;
  }

  TChain* chain=new TChain("ntp1"); 
  Bool_t MCorData;
  Int_t MatterOrAntiMatter=0;
  TString OutputDir;
  Int_t TruthMatch=0;
  TString myntupledir="Dstar0/Dstar0Pi";
  Dst0PiAnalysis* a=0;

  ///Small Sample
  if(WhichDATASet==1){
    OutputDir=myntupledir+"/KpiAndK3pi";
    MCorData=1;
    TruthMatch=0;
    MatterOrAntiMatter=0;
    a=new Dst0PiAnalysis(MCorData,TruthMatch,MatterOrAntiMatter,OutputDir);
  
    if(ReduceNtuple){
      TreeChain(chain,1,47,myntupledir+"/DATARun3On");
      a->SetChain(chain);
    } 
  }

  //Full Dataset
  if(WhichDATASet==2){
    OutputDir=myntupledir+"/KpiAndK3pi";
    MCorData=1;
    TruthMatch=0;
    MatterOrAntiMatter=0;
    a=new Dst0PiAnalysis(MCorData,TruthMatch,MatterOrAntiMatter,OutputDir);
  
    if(ReduceNtuple){
      TreeChain(chain,1,27,myntupledir+"/DATARun1On");
      TreeChain(chain,1,4,myntupledir+"/DATARun1Off");
      TreeChain(chain,1,88,myntupledir+"/DATARun2On");
      TreeChain(chain,1,10,myntupledir+"/DATARun2Off"); 
      TreeChain(chain,1,47,myntupledir+"/DATARun3On");
      TreeChain(chain,1,4,myntupledir+"/DATARun3Off");
      TreeChain(chain,1,142,myntupledir+"/DATARun4On");
      TreeChain(chain,1,14,myntupledir+"/DATARun4Off");
      TreeChain(chain,1,188,myntupledir+"/DATARun5On");
      TreeChain(chain,1,19,myntupledir+"/DATARun5Off");
      TreeChain(chain,1,114,myntupledir+"/DATARun6On");
      TreeChain(chain,1,10,myntupledir+"/DATARun6Off");
      a->SetChain(chain);
    } 
  }


  //Plots from Antimo
  if(WhichDATASet==3){
    OutputDir=myntupledir+"/Antimo";
    MCorData=1;
    TruthMatch=0;
    MatterOrAntiMatter=0;
    a=new Dst0PiAnalysis(MCorData,TruthMatch,MatterOrAntiMatter,OutputDir);    
  } 




  return a;
}
