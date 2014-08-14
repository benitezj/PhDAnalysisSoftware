#include "../../MACROS/TreeChain.C"
#include "DstPiAnalysis.h"

DstPiAnalysis* analyzeDstPi(Int_t WhichDATASet=-1,Int_t ReduceNtuple=0){
  
  ////read in preferences
  TChain* chain=NULL;
  if(ReduceNtuple)chain=new TChain("ntp1"); 
  TString myntupledir="DstarCharged/DstarPi";  
  DstPiAnalysis* a=NULL;
 

  if(WhichDATASet==0){
    a=new DstPiAnalysis(2,myntupledir+"/Test");
    if(1==ReduceNtuple){
      TreeChain(chain,1,33,myntupledir+"/DATARun3On");
      a->SetChain(chain);a->SetReduceCuts(0,1,1,0,1,1,1);
    }    
  }

  /////////////////////////////////////////////////////////
  ///Data: Fully Reduced (8/5/2010)  after charge bug. Need to update BAD plots.
  //////////////////////////////////////////////////////
  if(WhichDATASet>=1&&WhichDATASet<20){
    if(1==ReduceNtuple){
      TreeChain(chain,1,19,myntupledir+"/Data/Ntuples/DATARun1On");
      TreeChain(chain,1,3,myntupledir+"/Data/Ntuples/DATARun1Off");
      TreeChain(chain,1,62,myntupledir+"/Data/Ntuples/DATARun2On");
      TreeChain(chain,1,7,myntupledir+"/Data/Ntuples/DATARun2Off"); 
      TreeChain(chain,1,33,myntupledir+"/Data/Ntuples/DATARun3On");
      TreeChain(chain,1,3,myntupledir+"/Data/Ntuples/DATARun3Off");
      TreeChain(chain,1,99,myntupledir+"/Data/Ntuples/DATARun4On");
      TreeChain(chain,1,10,myntupledir+"/Data/Ntuples/DATARun4Off");
      TreeChain(chain,1,131,myntupledir+"/Data/Ntuples/DATARun5On");
      TreeChain(chain,1,14,myntupledir+"/Data/Ntuples/DATARun5Off");
      TreeChain(chain,1,53,myntupledir+"/Data/Ntuples/DATARun6On");
      TreeChain(chain,1,4,myntupledir+"/Data/Ntuples/DATARun6Off");
    }
  }

  if(WhichDATASet==1){
    a=new DstPiAnalysis(2,myntupledir+"/Data/KpiAndK3pi");
    if(1==ReduceNtuple){     a->SetChain(chain);a->SetReduceCuts(0,1,1,0,1,1,1);}
  }
  if(WhichDATASet==2){
    a=new DstPiAnalysis(2,myntupledir+"/Data/KpiAndK3piWS");
    if(1==ReduceNtuple){     a->SetChain(chain);a->SetReduceCuts(0,2,1,0,1,1,1);}
  }
  if(WhichDATASet==3){
    a=new DstPiAnalysis(2,myntupledir+"/Data/KpiAndK3piSB");
    if(1==ReduceNtuple){     a->SetChain(chain);a->SetReduceCuts(0,1,1,0,2,1,1);}
  }
  if(WhichDATASet==4){
    a=new DstPiAnalysis(2,myntupledir+"/Data/KpiAndK3piD0");
    if(1==ReduceNtuple){     a->SetChain(chain);a->SetReduceCuts(0,1,0,0,1,1,1);}
  }
  if(WhichDATASet==5){
    a=new DstPiAnalysis(2,myntupledir+"/Data/KpiAndK3piDstar");
    if(1==ReduceNtuple){     a->SetChain(chain);a->SetReduceCuts(0,1,1,0,0,1,1);}
  }
  if(WhichDATASet==6){
    a=new DstPiAnalysis(2,myntupledir+"/Data/KpiD0");
    if(1==ReduceNtuple){     a->SetChain(chain);a->SetReduceCuts(0,1,0,1,1,1,1);}
  }
  if(WhichDATASet==7){
    a=new DstPiAnalysis(2,myntupledir+"/Data/K3piD0");
    if(1==ReduceNtuple){     a->SetChain(chain);a->SetReduceCuts(0,1,0,2,1,1,1);}
  }
  if(WhichDATASet==8){
    a=new DstPiAnalysis(2,myntupledir+"/Data/KpiDstar");
    if(1==ReduceNtuple){     a->SetChain(chain);a->SetReduceCuts(0,1,1,1,0,1,1);}
  }
  if(WhichDATASet==9){
    a=new DstPiAnalysis(2,myntupledir+"/Data/K3piDstar");
    if(1==ReduceNtuple){     a->SetChain(chain);a->SetReduceCuts(0,1,1,2,0,1,1);}
  }
  if(WhichDATASet==10){//Test for D2430 sensitivity by adding a wide BW on the WS sample
    a=new DstPiAnalysis(2,myntupledir+"/Data/KpiAndK3piWSD2430");
  }
  if(WhichDATASet==11){
    a=new DstPiAnalysis(2,myntupledir+"/Data/Kpi");
    if(1==ReduceNtuple){     a->SetChain(chain);a->SetReduceCuts(0,1,1,1,1,1,1);}
  }

  ///---------------------------------------------------------------------------------------------
  ///ccbar MC:: Need to Reduce to correct charge bug !!!!
  ///----------------------------------------------------------------------------------------
  if(20<WhichDATASet&&WhichDATASet<30){
    if(1==ReduceNtuple){
      TreeChain(chain,1,363,myntupledir+"/MC/SP-1005/Ntuples/On");
      TreeChain(chain,1,25,myntupledir+"/MC/SP-1005/Ntuples/Off");
    }
  }
  if(WhichDATASet==21){
    a=new DstPiAnalysis(1,myntupledir+"/MC/SP-1005/KpiAndK3pi");
    if(1==ReduceNtuple){ a->SetChain(chain);a->SetReduceCuts(0,1,1,0,1,1,1);}
  }
  if(WhichDATASet==22){
    a=new DstPiAnalysis(1,myntupledir+"/MC/SP-1005/KpiAndK3piWS");
    if(1==ReduceNtuple){ a->SetChain(chain);a->SetReduceCuts(0,2,1,0,1,1,1);}
  }
  if(WhichDATASet==23){
    a=new DstPiAnalysis(1,myntupledir+"/MC/SP-1005/KpiAndK3piBkg");
    //if(1==ReduceNtuple){ a->SetChain(chain);a->SetReduceCuts(2,1,1,0,1,1,1);}//not set properly
  }
  if(WhichDATASet==24){
    a=new DstPiAnalysis(1,myntupledir+"/MC/SP-1005/KpiAndK3piD0");
    if(1==ReduceNtuple){ a->SetChain(chain);a->SetReduceCuts(0,1,0,0,1,1,1);}
  }
  if(WhichDATASet==25){
    a=new DstPiAnalysis(1,myntupledir+"/MC/SP-1005/KpiAndK3piDstar");
    if(1==ReduceNtuple){ a->SetChain(chain);a->SetReduceCuts(0,1,1,0,0,1,1);}
  }
  if(WhichDATASet==26){
    a=new DstPiAnalysis(1,myntupledir+"/MC/SP-1005/KpiAndK3piCos");
    if(1==ReduceNtuple){ a->SetChain(chain);a->SetReduceCuts(0,1,1,0,1,1,0);}
  }
  if(WhichDATASet==27){
    a=new DstPiAnalysis(2,myntupledir+"/MC/SP-1005/KpiAndK3piSB");
    if(1==ReduceNtuple){ a->SetChain(chain);a->SetReduceCuts(0,1,1,0,2,1,1);}
  }


  ///---------------------------------------------------------------------------------------------
  ///Signal MC: 
  // -Fully reduced after charge cut bug (8/5/2010). 
  // -BAD plots have been updated (8/5/2010)
  ///----------------------------------------------------------------------------------------

  if(WhichDATASet==1001){
    a=new DstPiAnalysis(0,myntupledir+"/MC/Signal/D2420/Kpi");
    if(1==ReduceNtuple){
      TreeChain(chain,1,8,myntupledir+"/MC/Signal/D2420/Kpi/Ntuples");     
      a->SetChain(chain);a->SetReduceCuts(0,1,1,1,1,1,1);
    }
  }
  if(WhichDATASet==1002){
    a=new DstPiAnalysis(0,myntupledir+"/MC/Signal/D2460/Kpi");
    if(1==ReduceNtuple){
      TreeChain(chain,1,8,myntupledir+"/MC/Signal/D2460/Kpi/Ntuples");     
      a->SetChain(chain);a->SetReduceCuts(0,1,1,1,1,1,1);
    }
  }
  if(WhichDATASet==1003){//for Kpi efficiency
    a=new DstPiAnalysis(0,myntupledir+"/MC/Signal/D2420andD2460/Kpi");
    if(1==ReduceNtuple){
      TreeChain(chain,1,8,myntupledir+"/MC/Signal/D2420/Kpi/Ntuples"); 
      TreeChain(chain,1,8,myntupledir+"/MC/Signal/D2460/Kpi/Ntuples"); 
      a->SetChain(chain);a->SetReduceCuts(0,1,1,1,1,1,1);
    }
  }
  if(WhichDATASet==1011){
    a=new DstPiAnalysis(0,myntupledir+"/MC/Signal/D2420/K3pi");
    if(1==ReduceNtuple){
      TreeChain(chain,1,8,myntupledir+"/MC/Signal/D2420/K3pi/Ntuples");     
      a->SetChain(chain);a->SetReduceCuts(0,1,1,2,1,1,1);
    }
  }
  if(WhichDATASet==1012){
    a=new DstPiAnalysis(0,myntupledir+"/MC/Signal/D2460/K3pi");
    if(1==ReduceNtuple){
      TreeChain(chain,1,8,myntupledir+"/MC/Signal/D2460/K3pi/Ntuples");     
      a->SetChain(chain);a->SetReduceCuts(0,1,1,2,1,1,1);
    }
  }
  if(WhichDATASet==1013){//for K3pi efficiency
    a=new DstPiAnalysis(0,myntupledir+"/MC/Signal/D2420andD2460/K3pi");
    if(1==ReduceNtuple){
      TreeChain(chain,1,8,myntupledir+"/MC/Signal/D2420/K3pi/Ntuples"); 
      TreeChain(chain,1,8,myntupledir+"/MC/Signal/D2460/K3pi/Ntuples");     
      a->SetChain(chain);a->SetReduceCuts(0,1,1,2,1,1,1);
    }
  }


  if(WhichDATASet==1021){//for resolution
    a=new DstPiAnalysis(0,myntupledir+"/MC/Signal/D2420andD2460/KpiAndK3pi");
    if(1==ReduceNtuple){
      TreeChain(chain,1,8,myntupledir+"/MC/Signal/D2420/Kpi/Ntuples"); 
      TreeChain(chain,1,8,myntupledir+"/MC/Signal/D2460/Kpi/Ntuples"); 
      TreeChain(chain,1,8,myntupledir+"/MC/Signal/D2420/K3pi/Ntuples"); 
      TreeChain(chain,1,8,myntupledir+"/MC/Signal/D2460/K3pi/Ntuples");     
      a->SetChain(chain);a->SetReduceCuts(0,1,1,0,1,1,1);
    }
  }


    
  return a;
}

