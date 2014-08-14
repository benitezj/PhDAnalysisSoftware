#include "../../MACROS/TreeChain.C"
#include "BToDstPiAnalysis.h"



BToDstPiAnalysis* analyzeBToDstPi(Int_t WhichDATASet=-1,Bool_t ReduceNtuple=0){
  
  ////read in preferences
  TChain* chain=NULL;
  if(ReduceNtuple)chain=new TChain("ntp1"); 
  Int_t MatterOrAntiMatter=0; 
  TString OutputDir;
  Int_t TruthMatch=0;
  Bool_t MCorDATA=0;
  TString myntupledir="DstarCharged/BToDstarPi";  
  BToDstPiAnalysis* a=NULL;

  if(WhichDATASet==-1){
    cout<<"Which Data Set?"<<endl;
    cout<<" "<<"1 = Run3 OnPeak Kpi+K3pi data"<<endl;
    cout<<"Enter id>";    

    cin>>WhichDATASet;
    cout<<endl;
  }

  if(WhichDATASet>=101&&WhichDATASet<=106){
    if(WhichDATASet==101)OutputDir=myntupledir+"/mESRun1"; 
    if(WhichDATASet==102)OutputDir=myntupledir+"/mESRun2"; 
    if(WhichDATASet==103)OutputDir=myntupledir+"/mESRun3"; 
    if(WhichDATASet==104)OutputDir=myntupledir+"/mESRun4"; 
    if(WhichDATASet==105)OutputDir=myntupledir+"/mESRun5"; 
    if(WhichDATASet==106)OutputDir=myntupledir+"/mESRun6"; 
    MatterOrAntiMatter=0;   
    TruthMatch=0;
    MCorDATA=1;
    a=new BToDstPiAnalysis(MCorDATA,TruthMatch,MatterOrAntiMatter,OutputDir);

    if(ReduceNtuple){
      if(WhichDATASet==101)TreeChain(chain,1,19,myntupledir+"/DATARun1On");
      if(WhichDATASet==102)TreeChain(chain,1,62,myntupledir+"/DATARun2On");
      if(WhichDATASet==103)TreeChain(chain,1,33,myntupledir+"/DATARun3On");
      if(WhichDATASet==104)TreeChain(chain,1,99,myntupledir+"/DATARun4On");
      if(WhichDATASet==105)TreeChain(chain,1,131,myntupledir+"/DATARun5On");
      if(WhichDATASet==106)TreeChain(chain,1,53,myntupledir+"/DATARun6On");      
      a->SetChain(chain);
      a->SetReduceCuts(0,1,1,1,1,0,0);
    }   
  }

 
  if(WhichDATASet==1){
    OutputDir=myntupledir+"/mES"; 
    MatterOrAntiMatter=0;   
    TruthMatch=0;
    MCorDATA=1;
    a=new BToDstPiAnalysis(MCorDATA,TruthMatch,MatterOrAntiMatter,OutputDir);

    if(ReduceNtuple){
      TreeChain(chain,1,19,myntupledir+"/DATARun1On");
      TreeChain(chain,1,62,myntupledir+"/DATARun2On");
      TreeChain(chain,1,33,myntupledir+"/DATARun3On");
      TreeChain(chain,1,99,myntupledir+"/DATARun4On");
      TreeChain(chain,1,131,myntupledir+"/DATARun5On");
      TreeChain(chain,1,53,myntupledir+"/DATARun6On");      
      a->SetChain(chain);
      a->SetReduceCuts(0,1,1,1,1,0,0);

    }
   
  }

  if(WhichDATASet==12){
    OutputDir=myntupledir+"/mESWS"; 
    MatterOrAntiMatter=0;   
    TruthMatch=0;
    MCorDATA=1;
    a=new BToDstPiAnalysis(MCorDATA,TruthMatch,MatterOrAntiMatter,OutputDir);

    if(ReduceNtuple){
      TreeChain(chain,1,19,myntupledir+"/DATARun1On");
      TreeChain(chain,1,62,myntupledir+"/DATARun2On");
      TreeChain(chain,1,33,myntupledir+"/DATARun3On");
      TreeChain(chain,1,99,myntupledir+"/DATARun4On");
      TreeChain(chain,1,131,myntupledir+"/DATARun5On");
      TreeChain(chain,1,53,myntupledir+"/DATARun6On");      
      a->SetChain(chain);
      a->SetReduceCuts(0,2,1,1,1,0,0);

    }
   
  }

  if(WhichDATASet==13){
    OutputDir=myntupledir+"/Dst"; 
    MatterOrAntiMatter=0;   
    TruthMatch=0;
    MCorDATA=1;
    a=new BToDstPiAnalysis(MCorDATA,TruthMatch,MatterOrAntiMatter,OutputDir);

    if(ReduceNtuple){
      TreeChain(chain,1,19,myntupledir+"/DATARun1On");
      TreeChain(chain,1,62,myntupledir+"/DATARun2On");
      TreeChain(chain,1,33,myntupledir+"/DATARun3On");
      TreeChain(chain,1,99,myntupledir+"/DATARun4On");
      TreeChain(chain,1,131,myntupledir+"/DATARun5On");
      TreeChain(chain,1,53,myntupledir+"/DATARun6On");      
      a->SetChain(chain);
      a->SetReduceCuts(0,1,1,0,1,1,0);

    }
   
  }

  if(WhichDATASet==14){
    OutputDir=myntupledir+"/ContmES"; 
    MatterOrAntiMatter=0;   
    TruthMatch=0;
    MCorDATA=1;
    a=new BToDstPiAnalysis(MCorDATA,TruthMatch,MatterOrAntiMatter,OutputDir);

    if(ReduceNtuple){
      TreeChain(chain,1,19,myntupledir+"/DATARun1Off");
      TreeChain(chain,1,62,myntupledir+"/DATARun2Off");
      TreeChain(chain,1,33,myntupledir+"/DATARun3Off");
      TreeChain(chain,1,99,myntupledir+"/DATARun4Off");
      TreeChain(chain,1,131,myntupledir+"/DATARun5Off");
      TreeChain(chain,1,53,myntupledir+"/DATARun6Off");      
      a->SetChain(chain);
      a->SetReduceCuts(0,1,1,1,1,0,0);

    }
   
  }


  cout<<"The following data-set will be analyzed: "<<OutputDir<<endl;   
  
  return a;
}

