#include "../../MACROS/TreeChain.C"
#include "BToDstPi0Analysis.h"



BToDstPi0Analysis* analyzeBToDstPi0(Int_t WhichDATASet=-1,Bool_t ReduceNtuple=0){
  
  ////read in preferences
  TChain* chain=NULL;
  if(ReduceNtuple)chain=new TChain("ntp1"); 
  Int_t MatterOrAntiMatter=0; 
  TString OutputDir;
  Int_t TruthMatch=0;
  Bool_t MCorDATA=0;
  TString myntupledir="DstarCharged/BToDstarPi0";  
  BToDstPi0Analysis* a=NULL;

  if(WhichDATASet==-1){
    cout<<"Which Data Set?"<<endl;
    cout<<" "<<"1 = Run3 OnPeak Kpi+K3pi data"<<endl;
    cout<<"Enter id>";    

    cin>>WhichDATASet;
    cout<<endl;
  }

  if(WhichDATASet==0){
    OutputDir=myntupledir+"/mESTest"; 
    MatterOrAntiMatter=0;   
    TruthMatch=0;
    MCorDATA=1;
    a=new BToDstPi0Analysis(MCorDATA,TruthMatch,MatterOrAntiMatter,OutputDir);

    if(ReduceNtuple){
      TreeChain(chain,1,33,myntupledir+"/DATARun3On");
      a->SetChain(chain);
      a->SetReduceCuts(0,0,1,1,1,0,0);

    }
   
  }
 
  if(WhichDATASet==1){
    OutputDir=myntupledir+"/mES"; 
    MatterOrAntiMatter=0;   
    TruthMatch=0;
    MCorDATA=1;
    a=new BToDstPi0Analysis(MCorDATA,TruthMatch,MatterOrAntiMatter,OutputDir);

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

  if(WhichDATASet==12){
    OutputDir=myntupledir+"/mESDst"; 
    MatterOrAntiMatter=0;   
    TruthMatch=0;
    MCorDATA=1;
    a=new BToDstPi0Analysis(MCorDATA,TruthMatch,MatterOrAntiMatter,OutputDir);

    if(ReduceNtuple){
      TreeChain(chain,1,19,myntupledir+"/DATARun1On");
      TreeChain(chain,1,62,myntupledir+"/DATARun2On");
      TreeChain(chain,1,33,myntupledir+"/DATARun3On");
      TreeChain(chain,1,99,myntupledir+"/DATARun4On");
      TreeChain(chain,1,131,myntupledir+"/DATARun5On");
      TreeChain(chain,1,53,myntupledir+"/DATARun6On");      
      a->SetChain(chain);
      a->SetReduceCuts(0,0,1,0,1,0,0);

    }
   
  }

  if(WhichDATASet==13){
    OutputDir=myntupledir+"/Dst"; 
    MatterOrAntiMatter=0;
    TruthMatch=0;
    MCorDATA=1;
    a=new BToDstPi0Analysis(MCorDATA,TruthMatch,MatterOrAntiMatter,OutputDir);

    if(ReduceNtuple){
      TreeChain(chain,1,19,myntupledir+"/DATARun1On");
      TreeChain(chain,1,62,myntupledir+"/DATARun2On");
      TreeChain(chain,1,33,myntupledir+"/DATARun3On");
      TreeChain(chain,1,99,myntupledir+"/DATARun4On");
      TreeChain(chain,1,131,myntupledir+"/DATARun5On");
      TreeChain(chain,1,53,myntupledir+"/DATARun6On");      
      a->SetChain(chain);
      a->SetReduceCuts(0,0,1,0,1,1,0);

    }
   
  }

  if(WhichDATASet==2){
    OutputDir=myntupledir+"/mESOff"; 
    MatterOrAntiMatter=0;
    TruthMatch=0;
    MCorDATA=1;
    a=new BToDstPi0Analysis(MCorDATA,TruthMatch,MatterOrAntiMatter,OutputDir);

    if(ReduceNtuple){

      TreeChain(chain,1,3,myntupledir+"/DATARun1Off");
      TreeChain(chain,1,7,myntupledir+"/DATARun2Off");
      TreeChain(chain,1,3,myntupledir+"/DATARun3Off");
      TreeChain(chain,1,10,myntupledir+"/DATARun4Off");
      TreeChain(chain,1,14,myntupledir+"/DATARun5Off");
      TreeChain(chain,1,4,myntupledir+"/DATARun6Off");

      a->SetChain(chain);
      a->SetReduceCuts(0,0,1,1,1,0,0);

    }
   
  }






  cout<<"The following data-set will be analyzed: "<<OutputDir<<endl;   
  
  return a;
}

