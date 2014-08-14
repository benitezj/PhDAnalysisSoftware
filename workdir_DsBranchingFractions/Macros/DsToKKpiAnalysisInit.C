#include "../tmp/Linux26SL5_i386_gcc412/DRecoilToolsUser/NtupleCode/TreeChain.hh"
#include "../tmp/Linux26SL5_i386_gcc412/DRecoilToolsUser/NtupleCode/DRecoDsToKKpiAnalysis.hh"

DRecoDsToKKpiAnalysis* analyzeDsToKKpi(Int_t WhichDATASet=-1,Bool_t ReduceNtuple=0){
  
  TChain* chain=NULL;
  if(ReduceNtuple)chain=new TChain("ntp1"); 
  DRecoDsToKKpiAnalysis* a=NULL;

  ///Signal SP9775+9776
  if(WhichDATASet==0){//
    a=new DRecoDsToKKpiAnalysis(0,"DsTolnu/KKpi/Signal");
    if(ReduceNtuple){   

      TreeChain(chain,1,2,"DsTolnu/KKpi/Signal/Ntuples/SP9775/Run1On/Ntuple");     
      TreeChain(chain,1,1,"DsTolnu/KKpi/Signal/Ntuples/SP9775/Run1Off/Ntuple"); 
      TreeChain(chain,1,6,"DsTolnu/KKpi/Signal/Ntuples/SP9775/Run2On/Ntuple");     
      TreeChain(chain,1,1,"DsTolnu/KKpi/Signal/Ntuples/SP9775/Run2Off/Ntuple"); 
      TreeChain(chain,1,4,"DsTolnu/KKpi/Signal/Ntuples/SP9775/Run3On/Ntuple");     
      TreeChain(chain,1,1,"DsTolnu/KKpi/Signal/Ntuples/SP9775/Run3Off/Ntuple"); 
      TreeChain(chain,1,11,"DsTolnu/KKpi/Signal/Ntuples/SP9775/Run4On/Ntuple");     
      TreeChain(chain,1,1,"DsTolnu/KKpi/Signal/Ntuples/SP9775/Run4Off/Ntuple"); 
      TreeChain(chain,1,14,"DsTolnu/KKpi/Signal/Ntuples/SP9775/Run5On/Ntuple");     
      TreeChain(chain,1,2,"DsTolnu/KKpi/Signal/Ntuples/SP9775/Run5Off/Ntuple"); 
      TreeChain(chain,1,8,"DsTolnu/KKpi/Signal/Ntuples/SP9775/Run6On/Ntuple");     
      TreeChain(chain,1,1,"DsTolnu/KKpi/Signal/Ntuples/SP9775/Run6Off/Ntuple");     
      //TreeChain(chain,1,,"DsTolnu/KKpi/Signal/Ntuples/SP9775/RunY2SOn/Ntuple");//SP collections lost   
      TreeChain(chain,1,1,"DsTolnu/KKpi/Signal/Ntuples/SP9775/RunY2SOff/Ntuple"); 
      TreeChain(chain,1,3,"DsTolnu/KKpi/Signal/Ntuples/SP9775/RunY3SOn/Ntuple");     
      TreeChain(chain,1,1,"DsTolnu/KKpi/Signal/Ntuples/SP9775/RunY3SOff/Ntuple"); 

      TreeChain(chain,1,2,"DsTolnu/KKpi/Signal/Ntuples/SP9776/Run1On/Ntuple");     
      TreeChain(chain,1,1,"DsTolnu/KKpi/Signal/Ntuples/SP9776/Run1Off/Ntuple"); 
      TreeChain(chain,1,6,"DsTolnu/KKpi/Signal/Ntuples/SP9776/Run2On/Ntuple");     
      TreeChain(chain,1,1,"DsTolnu/KKpi/Signal/Ntuples/SP9776/Run2Off/Ntuple"); 
      TreeChain(chain,1,4,"DsTolnu/KKpi/Signal/Ntuples/SP9776/Run3On/Ntuple");     
      TreeChain(chain,1,1,"DsTolnu/KKpi/Signal/Ntuples/SP9776/Run3Off/Ntuple"); 
      TreeChain(chain,1,11,"DsTolnu/KKpi/Signal/Ntuples/SP9776/Run4On/Ntuple");     
      TreeChain(chain,1,1,"DsTolnu/KKpi/Signal/Ntuples/SP9776/Run4Off/Ntuple"); 
      TreeChain(chain,1,14,"DsTolnu/KKpi/Signal/Ntuples/SP9776/Run5On/Ntuple");     
      TreeChain(chain,1,2,"DsTolnu/KKpi/Signal/Ntuples/SP9776/Run5Off/Ntuple"); 
      TreeChain(chain,1,8,"DsTolnu/KKpi/Signal/Ntuples/SP9776/Run6On/Ntuple");     
      TreeChain(chain,1,1,"DsTolnu/KKpi/Signal/Ntuples/SP9776/Run6Off/Ntuple");     
      TreeChain(chain,1,1,"DsTolnu/KKpi/Signal/Ntuples/SP9776/RunY2SOn/Ntuple");     
      TreeChain(chain,1,1,"DsTolnu/KKpi/Signal/Ntuples/SP9776/RunY2SOff/Ntuple"); 
      TreeChain(chain,1,3,"DsTolnu/KKpi/Signal/Ntuples/SP9776/RunY3SOn/Ntuple");     
      TreeChain(chain,1,1,"DsTolnu/KKpi/Signal/Ntuples/SP9776/RunY3SOff/Ntuple");

      a->SetChain(chain);
      a->SetEvtReduceCuts(0,0);
      a->SetTagReduceCuts(1,1,0,0);	    
      a->SetDsReduceCuts(1,1,1,1);
    }   
  }
  



  ///////////////ccbar
  if(WhichDATASet==1){//
    a=new DRecoDsToKKpiAnalysis(1,"DsTolnu/KKpi/SP1005");
    if(ReduceNtuple){   
      TreeChain(chain,1,47,"DsTolnu/KKpi/SP1005/Ntuples/Run1On/Ntuple");    
      TreeChain(chain,1,138,"DsTolnu/KKpi/SP1005/Ntuples/Run2On/Ntuple");    
      TreeChain(chain,1,74,"DsTolnu/KKpi/SP1005/Ntuples/Run3On/Ntuple");    
      TreeChain(chain,1,223,"DsTolnu/KKpi/SP1005/Ntuples/Run4On/Ntuple");    
      TreeChain(chain,1,286,"DsTolnu/KKpi/SP1005/Ntuples/Run5On/Ntuple");    
      TreeChain(chain,1,174,"DsTolnu/KKpi/SP1005/Ntuples/Run6On/Ntuple");    
      TreeChain(chain,1,28,"DsTolnu/KKpi/SP1005/Ntuples/RunY2SOn/Ntuple");    
      TreeChain(chain,1,66,"DsTolnu/KKpi/SP1005/Ntuples/RunY3SOn/Ntuple");    
      TreeChain(chain,1,5,"DsTolnu/KKpi/SP1005/Ntuples/Run1Off/Ntuple");    
      TreeChain(chain,1,15,"DsTolnu/KKpi/SP1005/Ntuples/Run2Off/Ntuple");    
      TreeChain(chain,1,6,"DsTolnu/KKpi/SP1005/Ntuples/Run3Off/Ntuple");    
      TreeChain(chain,1,18,"DsTolnu/KKpi/SP1005/Ntuples/Run4Off/Ntuple");    
      TreeChain(chain,1,32,"DsTolnu/KKpi/SP1005/Ntuples/Run5Off/Ntuple");    
      TreeChain(chain,1,18,"DsTolnu/KKpi/SP1005/Ntuples/Run6Off/Ntuple");
      TreeChain(chain,1,3,"DsTolnu/KKpi/SP1005/Ntuples/RunY2SOff/Ntuple");    
      TreeChain(chain,1,8,"DsTolnu/KKpi/SP1005/Ntuples/RunY3SOff/Ntuple");  
      a->SetChain(chain);
      a->SetEvtReduceCuts(0,0);
      a->SetTagReduceCuts(1,1,0,0);	    
      a->SetDsReduceCuts(1,1,1,1);
    }   
  }

  ///////////////Data
  if(WhichDATASet==2){//
    a=new DRecoDsToKKpiAnalysis(2,"DsTolnu/KKpi/Data");
    if(ReduceNtuple){  

      TreeChain(chain,1,65,"DsTolnu/KKpi/Data/Ntuples/Run1On/Ntuple");
      TreeChain(chain,1,7,"DsTolnu/KKpi/Data/Ntuples/Run1Off/Ntuple");
      TreeChain(chain,1,211,"DsTolnu/KKpi/Data/Ntuples/Run2On/Ntuple");
      TreeChain(chain,1,17,"DsTolnu/KKpi/Data/Ntuples/Run2Off/Ntuple");
      TreeChain(chain,1,111,"DsTolnu/KKpi/Data/Ntuples/Run3On/Ntuple");
      TreeChain(chain,1,7,"DsTolnu/KKpi/Data/Ntuples/Run3Off/Ntuple");
      TreeChain(chain,1,351,"DsTolnu/KKpi/Data/Ntuples/Run4On/Ntuple");
      TreeChain(chain,1,25,"DsTolnu/KKpi/Data/Ntuples/Run4Off/Ntuple");
      TreeChain(chain,1,463,"DsTolnu/KKpi/Data/Ntuples/Run5On/Ntuple");
      TreeChain(chain,1,40,"DsTolnu/KKpi/Data/Ntuples/Run5Off/Ntuple");
      TreeChain(chain,1,243,"DsTolnu/KKpi/Data/Ntuples/Run6On/Ntuple");
      TreeChain(chain,1,20,"DsTolnu/KKpi/Data/Ntuples/Run6Off/Ntuple");
      TreeChain(chain,1,52,"DsTolnu/KKpi/Data/Ntuples/RunY2SOn/Ntuple");
      TreeChain(chain,1,3,"DsTolnu/KKpi/Data/Ntuples/RunY2SOff/Ntuple");
      TreeChain(chain,1,107,"DsTolnu/KKpi/Data/Ntuples/RunY3SOn/Ntuple");
      TreeChain(chain,1,5,"DsTolnu/KKpi/Data/Ntuples/RunY3SOff/Ntuple");
 
      a->SetChain(chain);
      a->SetEvtReduceCuts(0,0);
      a->SetTagReduceCuts(1,1,0,0);	    
      a->SetDsReduceCuts(1,1,1,1);
    }   
  }

  if(WhichDATASet==21){//Unblinded Data
    a=new DRecoDsToKKpiAnalysis(2,"DsTolnu/KKpi/DataUnblind");
    if(ReduceNtuple){  
      TreeChain(chain,1,65,"DsTolnu/KKpi/Data/Ntuples/Run1On/Ntuple");
      TreeChain(chain,1,7,"DsTolnu/KKpi/Data/Ntuples/Run1Off/Ntuple");
      TreeChain(chain,1,211,"DsTolnu/KKpi/Data/Ntuples/Run2On/Ntuple");
      TreeChain(chain,1,17,"DsTolnu/KKpi/Data/Ntuples/Run2Off/Ntuple");
      TreeChain(chain,1,111,"DsTolnu/KKpi/Data/Ntuples/Run3On/Ntuple");
      TreeChain(chain,1,7,"DsTolnu/KKpi/Data/Ntuples/Run3Off/Ntuple");
      TreeChain(chain,1,351,"DsTolnu/KKpi/Data/Ntuples/Run4On/Ntuple");
      TreeChain(chain,1,25,"DsTolnu/KKpi/Data/Ntuples/Run4Off/Ntuple");
      TreeChain(chain,1,463,"DsTolnu/KKpi/Data/Ntuples/Run5On/Ntuple");
      TreeChain(chain,1,40,"DsTolnu/KKpi/Data/Ntuples/Run5Off/Ntuple");
      TreeChain(chain,1,243,"DsTolnu/KKpi/Data/Ntuples/Run6On/Ntuple");
      TreeChain(chain,1,20,"DsTolnu/KKpi/Data/Ntuples/Run6Off/Ntuple");
      TreeChain(chain,1,52,"DsTolnu/KKpi/Data/Ntuples/RunY2SOn/Ntuple");
      TreeChain(chain,1,3,"DsTolnu/KKpi/Data/Ntuples/RunY2SOff/Ntuple");
      TreeChain(chain,1,107,"DsTolnu/KKpi/Data/Ntuples/RunY3SOn/Ntuple");
      TreeChain(chain,1,5,"DsTolnu/KKpi/Data/Ntuples/RunY3SOff/Ntuple");
 
      a->SetChain(chain);
      a->SetEvtReduceCuts(0,0);
      a->SetTagReduceCuts(1,1,0,0);	    
      a->SetDsReduceCuts(1,1,1,1);      
    }   
    a->SetBlind(0);
  }


  ///////////////Check on KKpi efficiency
  if(WhichDATASet==1000){//uses Run5OnPeak skimmed for generated Ds-->KKpi events
    a=new DRecoDsToKKpiAnalysis(0,"KKpiBFccbarSkim/Run5OnNtuplesKKpi");
    if(ReduceNtuple){  
      TreeChain(chain,1,7,"KKpiBFccbarSkim/Run5OnNtuplesKKpi/Ntuple");
 
      a->SetChain(chain);
      a->SetEvtReduceCuts(0,0);
      a->SetTagReduceCuts(1,1,0,0);	    
      a->SetDsReduceCuts(1,1,1,1);
    }   
  }






  return a;
}

