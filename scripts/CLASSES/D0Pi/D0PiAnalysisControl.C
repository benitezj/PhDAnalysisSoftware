#include "../../MACROS/TreeChain.C"
#include "D0PiAnalysis.h"

D0PiAnalysis* analyzeD0Pi(Int_t WhichDATASet=-1,Bool_t ReduceNtuple=0){
  
  ////read in preferences
  TChain* chain=NULL;
  if(ReduceNtuple)chain=new TChain("ntp1"); 
  TString myntupledir="Dzero/DzeroPi";  
  D0PiAnalysis* a=NULL;

  if(WhichDATASet==-1){
    cout<<"Which Data Set?"<<endl;
    cout<<" "<<"2 =OnPeak+OffPeak SP1005"<<endl;
    cout<<"Enter id>";    
    cin>>WhichDATASet;
    cout<<endl;
  }


  //
  if(WhichDATASet==0){
    a=new D0PiAnalysis(0,myntupledir+"/MC/SP1005/Test");

    if(ReduceNtuple){
      TreeChain(chain,1,14,myntupledir+"/MC/SP1005/NtuplesOff");
      a->SetChain(chain);
      a->SetReduceCuts(0,0,0,0);
    }   
  }

  //--------------------Signal--------------------------
  if(WhichDATASet==1){
    a=new D0PiAnalysis(0,myntupledir+"/Signal");

    if(ReduceNtuple){
      TreeChain(chain,1,35,myntupledir+"/Signal/Ntuples/On");
      TreeChain(chain,1,4,myntupledir+"/Signal/Ntuples/Off");
      a->SetChain(chain);
      a->SetReduceCuts(1,1,1,1);
    }   
  }


  //--------------------Generics--------------------------
  if(WhichDATASet==2){
    a=new D0PiAnalysis(0,myntupledir+"/MC/SP1005/Kpi");

    if(ReduceNtuple){
      TreeChain(chain,1,218,myntupledir+"/MC/SP1005/NtuplesOn");
      TreeChain(chain,1,14,myntupledir+"/MC/SP1005/NtuplesOff");
      a->SetChain(chain);
      a->SetReduceCuts(1,1,1,1);
    }   
  }

  if(WhichDATASet==21){
    a=new D0PiAnalysis(0,myntupledir+"/MC/SP1005/KpiDMass");

    if(ReduceNtuple){
      TreeChain(chain,1,218,myntupledir+"/MC/SP1005/NtuplesOn");
      TreeChain(chain,1,14,myntupledir+"/MC/SP1005/NtuplesOff");
      a->SetChain(chain);
      a->SetReduceCuts(1,1,0,1);
    }   
  }
  if(WhichDATASet==22){
    a=new D0PiAnalysis(0,myntupledir+"/MC/SP1005/KpiWS");

    if(ReduceNtuple){
      TreeChain(chain,1,218,myntupledir+"/MC/SP1005/NtuplesOn");
      TreeChain(chain,1,14,myntupledir+"/MC/SP1005/NtuplesOff");
      a->SetChain(chain);
      a->SetReduceCuts(2,1,1,1);
    }   
  }
  if(WhichDATASet==24){
    a=new D0PiAnalysis(0,myntupledir+"/MC/SP1005/KpiSB");

    if(ReduceNtuple){
      TreeChain(chain,1,218,myntupledir+"/MC/SP1005/NtuplesOn");
      TreeChain(chain,1,14,myntupledir+"/MC/SP1005/NtuplesOff");
      a->SetChain(chain);
      a->SetReduceCuts(1,1,2,1);
    }   
  }
  if(WhichDATASet==26){
    a=new D0PiAnalysis(0,myntupledir+"/MC/SP1005/KpiSBWS");

    if(ReduceNtuple){
      TreeChain(chain,1,218,myntupledir+"/MC/SP1005/NtuplesOn");
      TreeChain(chain,1,14,myntupledir+"/MC/SP1005/NtuplesOff");
      a->SetChain(chain);
      a->SetReduceCuts(2,1,2,1);
    }   
  }
  if(WhichDATASet==27){
    a=new D0PiAnalysis(0,myntupledir+"/MC/SP1005/KpiDCos");

    if(ReduceNtuple){
      TreeChain(chain,1,218,myntupledir+"/MC/SP1005/NtuplesOn");
      TreeChain(chain,1,14,myntupledir+"/MC/SP1005/NtuplesOff");
      a->SetChain(chain);
      a->SetReduceCuts(1,1,1,0);
    }   
  }


  //--------------------Antimo Histos--------------------------
  //MC
  if(WhichDATASet==51){
    a=new D0PiAnalysis(1,myntupledir+"/AntimoMC/Kpi");
  }
  if(WhichDATASet==52){
    a=new D0PiAnalysis(1,myntupledir+"/AntimoMC/KpiSB");
  }
  if(WhichDATASet==53){
    a=new D0PiAnalysis(1,myntupledir+"/AntimoMC/KpiWS");
  }
  if(WhichDATASet==54){
    a=new D0PiAnalysis(1,myntupledir+"/AntimoMC/KpiWSSB");
  }
  if(WhichDATASet==55){
    a=new D0PiAnalysis(1,myntupledir+"/AntimoMC/KpiDMass");
  }

  //Weighted MC
  if(WhichDATASet==61){
    a=new D0PiAnalysis(1,myntupledir+"/AntimoMCWeight/Kpi");
  }

  //Data
  if(WhichDATASet==71){
    a=new D0PiAnalysis(1,myntupledir+"/AntimoData/Kpi");
  }
  if(WhichDATASet==72){
    a=new D0PiAnalysis(1,myntupledir+"/AntimoData/KpiSB");
  }
  if(WhichDATASet==73){
    a=new D0PiAnalysis(1,myntupledir+"/AntimoData/KpiWS");
  }
  if(WhichDATASet==74){
    a=new D0PiAnalysis(1,myntupledir+"/AntimoData/KpiWSSB");
  }
  if(WhichDATASet==75){
    a=new D0PiAnalysis(1,myntupledir+"/AntimoData/KpiDMass");
  }


  return a;
}

