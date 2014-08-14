#include "../../MACROS/TreeChain.C"
#include "DCPiAnalysis.h"

DCPiAnalysis* analyzeDCPi(Int_t WhichDATASet=-1,Bool_t ReduceNtuple=0){
  
  ////read in preferences
  TChain* chain=NULL;
  if(ReduceNtuple)chain=new TChain("ntp1"); 
  TString myntupledir="DCharged/DChargedPi";  
  DCPiAnalysis* a=NULL;


 //--------------------Signal--------------------------
  if(WhichDATASet==0){
    a=new DCPiAnalysis(0,myntupledir+"/MC/SP-9015/KPiPi");
    if(ReduceNtuple){ 
      TreeChain(chain,1,5,myntupledir+"/MC/SP-9015");
      a->SetChain(chain);
      a->SetReduceCuts(1,1,1,1,1,1,1);
    }   
  }



  //--------------------Generics--------------------------
  if(WhichDATASet>=10&&WhichDATASet<20&&ReduceNtuple){
    TreeChain(chain,1,545,myntupledir+"/MC/SP-1005/On");
    TreeChain(chain,1,37,myntupledir+"/MC/SP-1005/Off");
  }
  if(WhichDATASet==10){
    a=new DCPiAnalysis(1,myntupledir+"/MC/SP-1005/KPiPi");
    if(ReduceNtuple){
      a->SetChain(chain);
      a->SetReduceCuts(0,1,1,1,1,1,1);
    }   
  }
  if(WhichDATASet==11){
    a=new DCPiAnalysis(1,myntupledir+"/MC/SP-1005/KPiPiSB");
    if(ReduceNtuple){
      a->SetChain(chain);
      a->SetReduceCuts(0,1,1,1,2,1,1);
    }   
  }
  if(WhichDATASet==12){
    a=new DCPiAnalysis(1,myntupledir+"/MC/SP-1005/KPiPiWS");
    if(ReduceNtuple){
      a->SetChain(chain);
      a->SetReduceCuts(0,2,1,1,1,1,1);  
    }   
  }
  if(WhichDATASet==13){//needed to subtract sharp peak at threshold
    a=new DCPiAnalysis(1,myntupledir+"/MC/SP-1005/KPiPiWSSB");
    if(ReduceNtuple){
      a->SetChain(chain);
      a->SetReduceCuts(0,2,1,1,2,1,1);  
    }   
  }
  if(WhichDATASet==14){//
    a=new DCPiAnalysis(1,myntupledir+"/MC/SP-1005/KPiPiDMass");
    if(ReduceNtuple){
      a->SetChain(chain);
      a->SetReduceCuts(0,1,1,1,0,1,1);  
    }   
  }


  //--------------------DATA--------------------------
  if(WhichDATASet>=20&&WhichDATASet<30&&ReduceNtuple){
      TreeChain(chain,1,23,myntupledir+"/DATARun1On");
      TreeChain(chain,1,3,myntupledir+"/DATARun1Off");
      TreeChain(chain,1,74,myntupledir+"/DATARun2On");
      TreeChain(chain,1,7,myntupledir+"/DATARun2Off"); 
      TreeChain(chain,1,39,myntupledir+"/DATARun3On");
      TreeChain(chain,1,3,myntupledir+"/DATARun3Off");
      TreeChain(chain,1,119,myntupledir+"/DATARun4On");
      TreeChain(chain,1,10,myntupledir+"/DATARun4Off");
      TreeChain(chain,1,158,myntupledir+"/DATARun5On");
      TreeChain(chain,1,14,myntupledir+"/DATARun5Off");
      TreeChain(chain,1,83,myntupledir+"/DATARun6On");
      TreeChain(chain,1,5,myntupledir+"/DATARun6Off");
  }
  if(WhichDATASet==20){
    a=new DCPiAnalysis(2,myntupledir+"/KPiPi");
   if(ReduceNtuple){
      a->SetChain(chain);
      a->SetReduceCuts(0,1,1,1,1,1,1);
    }   
  }
  if(WhichDATASet==21){
    a=new DCPiAnalysis(2,myntupledir+"/KPiPiSB");
   if(ReduceNtuple){
   a->SetChain(chain);
      a->SetReduceCuts(0,1,1,1,2,1,1); 
    }   
  }
  if(WhichDATASet==22){
    a=new DCPiAnalysis(2,myntupledir+"/KPiPiWS");
   if(ReduceNtuple){
      a->SetChain(chain);
      a->SetReduceCuts(0,2,1,1,1,1,1);
    }  
  }
  if(WhichDATASet==23){
    a=new DCPiAnalysis(2,myntupledir+"/KPiPiWSSB");
   if(ReduceNtuple){
     a->SetChain(chain);
     a->SetReduceCuts(0,2,1,1,2,1,1);
   }   
  }
  if(WhichDATASet==24){
    a=new DCPiAnalysis(2,myntupledir+"/KPiPiDMass");
   if(ReduceNtuple){
     a->SetChain(chain);
     a->SetReduceCuts(0,1,1,1,0,1,1);
   }   
  }




  ////////////////for cut optimization
  if(100<WhichDATASet&&WhichDATASet<110){    
    if(WhichDATASet==101){
      a=new DCPiAnalysis(2,myntupledir+"/CutOpt");
      a->SetReduceCuts(0,1,0,0,0,0,0);
    }

    if(ReduceNtuple){
      TreeChain(chain,1,39,myntupledir+"/DATARun3On");
      a->SetChain(chain);
    }
  }


  if(WhichDATASet==111){
    a=new DCPiAnalysis(1,myntupledir+"/MC/SP-1005/CutOpt");
    if(ReduceNtuple){
      TreeChain(chain,1,37,myntupledir+"/MC/SP-1005/Off");
      a->SetChain(chain);
      a->SetReduceCuts(0,1,0,0,0,0,0);
    }   
  }


  

  return a;
}

