#include "../../MACROS/TreeChain.C"
#include "DstPiAnalysis.h"

DstPiAnalysis* analyzeDstPi(Int_t WhichDATASet=-1,Int_t ReduceNtuple=0){
  
  ////read in preferences
  TChain* chain=NULL;
  if(ReduceNtuple)chain=new TChain("ntp1"); 
  Int_t MatterOrAntiMatter=0;
  Int_t DstPiLund=0;
  TString OutputDir;
  Int_t D0Mode=0;
  Int_t TruthMatch=0;
  Bool_t MCorDATA=0;
  TString myntupledir="DstarCharged/DstarPi";  


  if(WhichDATASet==-1){
    cout<<"Which Data Set?"<<endl;
    cout<<" "<<"0 =KPi OnPeak+OffPeak data"<<endl;
    cout<<" "<<"1 =K3pi OnPeak+OffPeak data"<<endl;
    cout<<" "<<"2 =Kpi from D2420"<<endl;
    cout<<" "<<"3 =Kpi from D2460"<<endl;
    cout<<" "<<"4 =K3pi from D2420"<<endl;
    cout<<" "<<"5 =K3pi from D2460"<<endl;
    cout<<" "<<"6 =Kpi ccbar no signal "<<endl;
    cout<<" "<<"7 =K3pi ccbar no signal"<<endl;
    cout<<" "<<"8 =KpiandK3pi data "<<endl;
    cout<<" "<<"9 =KpiandK3pi ccbar no signal"<<endl;
    cout<<" "<<"10 =K3pi B+B-"<<endl;
    cout<<" "<<"11 =K3pi B0B0"<<endl;
    cout<<" "<<"12 =K3pi uds"<<endl;
    cout<<" "<<"13 =Kpi ccbar with signal"<<endl;
    cout<<" "<<"14 =K3pi ccbar with signal"<<endl;
    cout<<" "<<"15 =Kpi D2420andD2460 "<<endl;
    cout<<" "<<"16 =K3pi D2420andD2460 "<<endl;
    cout<<" "<<"17 =KpiandK3pi ccbar with signal"<<endl;
    cout<<" "<<"18 =Kpi ccbar + sig embeded "<<endl;
    cout<<" "<<"21 =Kpi from D2420 BFieldChange .9998"<<endl;
    cout<<" "<<"22 =Kpi from D2420 BFieldChange 1.3"<<endl;
    cout<<" "<<"23 =Kpi from D2420 SvtChange 1.2"<<endl;
    cout<<" "<<"24 =Kpi from D2420 SvtChange 3.0"<<endl;
    cout<<" "<<"25 =Kpi from D2460 BFieldChange .9998"<<endl;
    cout<<" "<<"26 =Kpi from D2460 SvtChange 1.2"<<endl;
    cout<<" "<<"27 =K3pi from D2420 BFieldChange .9998"<<endl;
    cout<<" "<<"28 =K3pi from D2460 BFieldChange .9998"<<endl;
    cout<<" "<<"29 =K3pi from D2420 SvtChange 1.2"<<endl;
    cout<<" "<<"30 =K3pi from D2460 SvtChange 1.2"<<endl;
    cout<<"Enter id>";    

    cin>>WhichDATASet;
    cout<<endl;
  }


 
  if(WhichDATASet==0){
    OutputDir=myntupledir+"/Kpi";
    if(1==ReduceNtuple){
      TreeChain(chain,1,19,myntupledir+"/DATARun1On");
      TreeChain(chain,1,3,myntupledir+"/DATARun1Off");
      TreeChain(chain,1,62,myntupledir+"/DATARun2On");
      TreeChain(chain,1,7,myntupledir+"/DATARun2Off"); 
      TreeChain(chain,1,33,myntupledir+"/DATARun3On");
      TreeChain(chain,1,3,myntupledir+"/DATARun3Off");
      TreeChain(chain,1,99,myntupledir+"/DATARun4On");
      TreeChain(chain,1,10,myntupledir+"/DATARun4Off");
      TreeChain(chain,1,131,myntupledir+"/DATARun5On");
      TreeChain(chain,1,14,myntupledir+"/DATARun5Off");
      TreeChain(chain,1,53,myntupledir+"/DATARun6On");
      TreeChain(chain,1,4,myntupledir+"/DATARun6Off");
    }
    MatterOrAntiMatter=0;   
    TruthMatch=0;
    DstPiLund=myDstPiNeutralLund;
    D0Mode=1;
    MCorDATA=1;
  }

  if(WhichDATASet==1){
    OutputDir=myntupledir+"/K3pi";
    if(1==ReduceNtuple){
      TreeChain(chain,1,19,myntupledir+"/DATARun1On");
      TreeChain(chain,1,3,myntupledir+"/DATARun1Off");
      TreeChain(chain,1,62,myntupledir+"/DATARun2On");
      TreeChain(chain,1,7,myntupledir+"/DATARun2Off"); 
      TreeChain(chain,1,33,myntupledir+"/DATARun3On");
      TreeChain(chain,1,3,myntupledir+"/DATARun3Off");
      TreeChain(chain,1,99,myntupledir+"/DATARun4On");
      TreeChain(chain,1,10,myntupledir+"/DATARun4Off");
      TreeChain(chain,1,131,myntupledir+"/DATARun5On");
      TreeChain(chain,1,14,myntupledir+"/DATARun5Off");
      TreeChain(chain,1,53,myntupledir+"/DATARun6On");
      TreeChain(chain,1,4,myntupledir+"/DATARun6Off");
    }
    MatterOrAntiMatter=0;   
    TruthMatch=0;
    DstPiLund=myDstPiNeutralLund;
    D0Mode=2;
    MCorDATA=1;
  }


  if(WhichDATASet==2){
    OutputDir=myntupledir+"/MC/D2420/Kpi";
    if(1==ReduceNtuple){
      TreeChain(chain,1,8,OutputDir);
    }
    MatterOrAntiMatter=0;   
    TruthMatch=1;
    DstPiLund=myD2420NeutralLund;
    D0Mode=1;
    MCorDATA=0;
  }
  if(WhichDATASet==3){
    OutputDir=myntupledir+"/MC/D2460/Kpi";
    if(1==ReduceNtuple){
      TreeChain(chain,1,8,OutputDir);
    }
    MatterOrAntiMatter=0;   
    TruthMatch=1;
    DstPiLund=myD2460NeutralLund;
    D0Mode=1;
    MCorDATA=0;
  }
  if(WhichDATASet==4){
    OutputDir=myntupledir+"/MC/D2420/K3pi";
    if(1==ReduceNtuple){
      TreeChain(chain,1,8,OutputDir);
    }
    MatterOrAntiMatter=0;   
    TruthMatch=1;
    DstPiLund=myD2420NeutralLund;
    D0Mode=2;
    MCorDATA=0;
  }
  if(WhichDATASet==5){
    OutputDir=myntupledir+"/MC/D2460/K3pi";
    if(1==ReduceNtuple){
      TreeChain(chain,1,8,OutputDir);
    }
    MatterOrAntiMatter=0;   
    TruthMatch=1;
    DstPiLund=myD2460NeutralLund;
    D0Mode=2;
    MCorDATA=0;
  }
  if(WhichDATASet==6){
    OutputDir=myntupledir+"/MC/SP-1005/Kpi";
    if(1==ReduceNtuple){
      TreeChain(chain,1,363,myntupledir+"/MC/SP-1005/On");
      TreeChain(chain,1,25,myntupledir+"/MC/SP-1005/Off");
    }
    MatterOrAntiMatter=0;   
    TruthMatch=0;
    DstPiLund=myDstPiNeutralLund;
    D0Mode=1;
    MCorDATA=0;
  }
  if(WhichDATASet==7){
    OutputDir=myntupledir+"/MC/SP-1005/K3pi";
    if(1==ReduceNtuple){
      TreeChain(chain,1,363,myntupledir+"/MC/SP-1005/On");
      TreeChain(chain,1,25,myntupledir+"/MC/SP-1005/Off");
    }
    MatterOrAntiMatter=0;   
    TruthMatch=0;
    DstPiLund=myDstPiNeutralLund;
    D0Mode=2;
    MCorDATA=0;
  }

  if(WhichDATASet==8){
    OutputDir=myntupledir+"/KpiAndK3pi";
    MatterOrAntiMatter=0;   
    TruthMatch=0;
    DstPiLund=myDstPiNeutralLund;
    D0Mode=3;
    MCorDATA=1;    
  }
  if(WhichDATASet==9){
    OutputDir=myntupledir+"/MC/SP-1005/KpiAndK3pi";
    MatterOrAntiMatter=0;   
    TruthMatch=0;
    DstPiLund=myDstPiNeutralLund;
    D0Mode=3;
    MCorDATA=0;    
  }
  if(WhichDATASet==10){
    OutputDir=myntupledir+"/MC/SP-1235/K3pi";
    if(1==ReduceNtuple){
      TreeChain(chain,1,6,OutputDir);
    }
    MatterOrAntiMatter=0;   
    TruthMatch=0;
    DstPiLund=myDstPiNeutralLund;
    D0Mode=2;
    MCorDATA=0;    
  }
  if(WhichDATASet==11){
    OutputDir=myntupledir+"/MC/SP-1237/K3pi";
    if(1==ReduceNtuple){
      TreeChain(chain,1,8,OutputDir);
    }
    MatterOrAntiMatter=0;   
    TruthMatch=0;
    DstPiLund=myDstPiNeutralLund;
    D0Mode=2;
    MCorDATA=0;    
  }
  if(WhichDATASet==12){
    OutputDir=myntupledir+"/MC/SP-998/K3pi";
    if(1==ReduceNtuple){
      TreeChain(chain,1,4,OutputDir);
    }
    MatterOrAntiMatter=0;   
    TruthMatch=0;
    DstPiLund=myDstPiNeutralLund;
    D0Mode=2;
    MCorDATA=0;    
  }
  if(WhichDATASet==13){
    OutputDir=myntupledir+"/MC/SP-1005_Signal/Kpi";
    if(1==ReduceNtuple){
      TreeChain(chain,1,363,myntupledir+"/MC/SP-1005_Signal/On");
      TreeChain(chain,1,25,myntupledir+"/MC/SP-1005_Signal/Off");
    }
    MatterOrAntiMatter=0;   
    TruthMatch=0;
    DstPiLund=myDstPiNeutralLund;
    D0Mode=1;
    MCorDATA=0;    
  }
  if(WhichDATASet==14){
    OutputDir=myntupledir+"/MC/SP-1005_Signal/K3pi";
    if(1==ReduceNtuple){
      TreeChain(chain,1,363,myntupledir+"/MC/SP-1005_Signal/On");
      TreeChain(chain,1,25,myntupledir+"/MC/SP-1005_Signal/Off");
    }
    MatterOrAntiMatter=0;   
    TruthMatch=0;
    DstPiLund=myDstPiNeutralLund;
    D0Mode=2;
    MCorDATA=0;    
  }
  if(WhichDATASet==15){
    OutputDir=myntupledir+"/MC/D2420andD2460/Kpi";
    MatterOrAntiMatter=0;   
    TruthMatch=1;
    DstPiLund=myDstPiNeutralLund;
    D0Mode=1;
    MCorDATA=0;    
  }
  if(WhichDATASet==16){
    OutputDir=myntupledir+"/MC/D2420andD2460/K3pi";
    MatterOrAntiMatter=0;   
    TruthMatch=1;
    DstPiLund=myDstPiNeutralLund;
    D0Mode=2;
    MCorDATA=0;    
  }
  if(WhichDATASet==17){
    OutputDir=myntupledir+"/MC/SP-1005_Signal/KpiAndK3pi";
    MatterOrAntiMatter=0;   
    TruthMatch=0;
    DstPiLund=myDstPiNeutralLund;
    D0Mode=3;
    MCorDATA=0;
  }
  if(WhichDATASet==18){
    OutputDir=myntupledir+"/MC/SP-1005_SigEmbed/Kpi";
    MatterOrAntiMatter=0;   
    TruthMatch=0;
    DstPiLund=myDstPiNeutralLund;
    D0Mode=1;
    MCorDATA=0;
  }

  if(WhichDATASet==19){
    OutputDir=myntupledir+"/BFieldChange/Kpi";
    chain->SetName("ntp1002");
    if(1==ReduceNtuple){
//       chain->SetName("ntp1002");
//       TreeChain(chain,1,19,myntupledir+"/BFieldChange/DATARun1On");
//       TreeChain(chain,1,3,myntupledir+"/BFieldChange/DATARun1Off"); 
      return 0;
    }
    MatterOrAntiMatter=0;   
    TruthMatch=0;
    DstPiLund=myDstPiNeutralLund;
    D0Mode=1;
    MCorDATA=1;
  }
  if(WhichDATASet==20){
    OutputDir=myntupledir+"/BFieldChange2/Kpi";
    chain->SetName("ntp1002");
    if(1==ReduceNtuple){
//       chain->SetName("ntp1002");
//       TreeChain(chain,1,19,myntupledir+"/BFieldChange/DATARun1On");
//       TreeChain(chain,1,3,myntupledir+"/BFieldChange/DATARun1Off"); 
      return 0;
    }
    MatterOrAntiMatter=0;   
    TruthMatch=0;
    DstPiLund=myDstPiNeutralLund;
    D0Mode=1;
    MCorDATA=1;
  }


  if(WhichDATASet==21){
    OutputDir=myntupledir+"/MC/D2420/BFieldChange.9998/Kpi";
    if(1==ReduceNtuple){
      chain->SetName("ntp1002");
      TreeChain(chain,1,71,OutputDir);
    }
    MatterOrAntiMatter=0;   
    TruthMatch=1;
    DstPiLund=myD2420NeutralLund;
    D0Mode=1;
    MCorDATA=0;
  }
  if(WhichDATASet==22){
    OutputDir=myntupledir+"/MC/D2420/BFieldChange1.3/Kpi";
    if(1==ReduceNtuple){
      chain->SetName("ntp1002");
      TreeChain(chain,1,71,OutputDir);
    }
    MatterOrAntiMatter=0;   
    TruthMatch=1;
    DstPiLund=myD2420NeutralLund;
    D0Mode=1;
    MCorDATA=0;
  }
  if(WhichDATASet==23){
    OutputDir=myntupledir+"/MC/D2420/SvtChange/Kpi";
    if(1==ReduceNtuple){
      chain->SetName("ntp1002");
      TreeChain(chain,1,71,OutputDir);
    }
    MatterOrAntiMatter=0;   
    TruthMatch=1;
    DstPiLund=myD2420NeutralLund;
    D0Mode=1;
    MCorDATA=0;
  }
  if(WhichDATASet==24){    
    OutputDir=myntupledir+"/MC/D2420/SvtChange3.0/Kpi";
    if(1==ReduceNtuple){
      chain->SetName("ntp1002");
      TreeChain(chain,1,71,OutputDir);
    }
    MatterOrAntiMatter=0;   
    TruthMatch=1;
    DstPiLund=myD2420NeutralLund;
    D0Mode=1;
    MCorDATA=0;
  }
  if(WhichDATASet==25){
    OutputDir=myntupledir+"/MC/D2460/BFieldChange.9998/Kpi";
    if(1==ReduceNtuple){
      chain->SetName("ntp1002");
      TreeChain(chain,1,71,OutputDir);
    }
    MatterOrAntiMatter=0;   
    TruthMatch=1;
    DstPiLund=myD2460NeutralLund;
    D0Mode=1;
    MCorDATA=0;
  }
  if(WhichDATASet==26){
    OutputDir=myntupledir+"/MC/D2460/SvtChange/Kpi";
    if(1==ReduceNtuple){
      chain->SetName("ntp1002");
      TreeChain(chain,1,71,OutputDir);
    }
    MatterOrAntiMatter=0;   
    TruthMatch=1;
    DstPiLund=myD2460NeutralLund;
    D0Mode=1;
    MCorDATA=0;
  }
  if(WhichDATASet==27){
    OutputDir=myntupledir+"/MC/D2420/BFieldChange.9998/K3pi";
    if(1==ReduceNtuple){
      chain->SetName("ntp1002");
      TreeChain(chain,1,71,OutputDir);
    }
    MatterOrAntiMatter=0;   
    TruthMatch=1;
    DstPiLund=myD2420NeutralLund;
    D0Mode=2;
    MCorDATA=0;
  }
  if(WhichDATASet==28){
    OutputDir=myntupledir+"/MC/D2460/BFieldChange.9998/K3pi";
    if(1==ReduceNtuple){
      chain->SetName("ntp1002");
      TreeChain(chain,1,71,OutputDir);
    }
    MatterOrAntiMatter=0;   
    TruthMatch=1;
    DstPiLund=myD2460NeutralLund;
    D0Mode=2;
    MCorDATA=0;
  }
  if(WhichDATASet==29){
    OutputDir=myntupledir+"/MC/D2420/SvtChange/K3pi";
    if(1==ReduceNtuple){
      chain->SetName("ntp1002");
      TreeChain(chain,1,71,OutputDir);
    }
    MatterOrAntiMatter=0;   
    TruthMatch=1;
    DstPiLund=myD2420NeutralLund;
    D0Mode=2;
    MCorDATA=0;
  }
  if(WhichDATASet==30){
    OutputDir=myntupledir+"/MC/D2460/SvtChange/K3pi";
    if(1==ReduceNtuple){
      chain->SetName("ntp1002");
      TreeChain(chain,1,71,OutputDir);
    }
    MatterOrAntiMatter=0;   
    TruthMatch=1;
    DstPiLund=myD2460NeutralLund;
    D0Mode=2;
    MCorDATA=0;
  }
  //
  cout<<"The following data-set will be analyzed: "<<OutputDir<<endl;
  
  //create the analysis object
  DstPiAnalysis* a=new DstPiAnalysis(chain,MCorDATA,TruthMatch,MatterOrAntiMatter,DstPiLund,OutputDir,D0Mode);
  
  return a;
}

