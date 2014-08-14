#include "../tmp/Linux26SL5_i386_gcc412/DRecoilToolsUser/NtupleCode/TreeChain.hh"
#include "../tmp/Linux26SL5_i386_gcc412/DRecoilToolsUser/NtupleCode/DRecoDsInclusiveAnalysis.hh"

DRecoDsInclusiveAnalysis* analyzeDsInclusive(Int_t WhichDATASet=-1,Bool_t ReduceNtuple=0){
  
  TChain* chain=NULL;
  if(ReduceNtuple)chain=new TChain("ntp1"); 
  DRecoDsInclusiveAnalysis* a=NULL;

  ///---------------ccbar 
  if(WhichDATASet>=1&&WhichDATASet<4)
    if(ReduceNtuple)
      if(ReduceNtuple){   
	TreeChain(chain,1,47,"DsTolnu/SP1005Skim/DsReco/Ntuples/Run1On/Ntuple");    
 	TreeChain(chain,1,138,"DsTolnu/SP1005Skim/DsReco/Ntuples/Run2On/Ntuple");  	
 	TreeChain(chain,1,74,"DsTolnu/SP1005Skim/DsReco/Ntuples/Run3On/Ntuple");    
 	TreeChain(chain,1,223,"DsTolnu/SP1005Skim/DsReco/Ntuples/Run4On/Ntuple");    
 	TreeChain(chain,1,286,"DsTolnu/SP1005Skim/DsReco/Ntuples/Run5On/Ntuple");    
 	TreeChain(chain,1,174,"DsTolnu/SP1005Skim/DsReco/Ntuples/Run6On/Ntuple"); 	
 	TreeChain(chain,1,28,"DsTolnu/SP1005Skim/DsReco/Ntuples/RunY2SOn/Ntuple");    
 	TreeChain(chain,1,66,"DsTolnu/SP1005Skim/DsReco/Ntuples/RunY3SOn/Ntuple");
  
 	TreeChain(chain,1,5,"DsTolnu/SP1005Skim/DsReco/Ntuples/Run1Off/Ntuple");    
 	TreeChain(chain,1,15,"DsTolnu/SP1005Skim/DsReco/Ntuples/Run2Off/Ntuple");    
 	TreeChain(chain,1,6,"DsTolnu/SP1005Skim/DsReco/Ntuples/Run3Off/Ntuple");    
 	TreeChain(chain,1,18,"DsTolnu/SP1005Skim/DsReco/Ntuples/Run4Off/Ntuple");    
 	TreeChain(chain,1,32,"DsTolnu/SP1005Skim/DsReco/Ntuples/Run5Off/Ntuple");    
 	TreeChain(chain,1,18,"DsTolnu/SP1005Skim/DsReco/Ntuples/Run6Off/Ntuple");
 	TreeChain(chain,1,3,"DsTolnu/SP1005Skim/DsReco/Ntuples/RunY2SOff/Ntuple");    
 	TreeChain(chain,1,8,"DsTolnu/SP1005Skim/DsReco/Ntuples/RunY3SOff/Ntuple"); 
      }
  
  if(WhichDATASet==1){//loose cuts
    a=new DRecoDsInclusiveAnalysis(1,"DsTolnu/SP1005Skim/DsReco/NoPstarCut");
    if(ReduceNtuple){     
      a->SetChain(chain);
      a->SetEvtReduceCuts(0,0);
      a->SetTagReduceCuts(1,1,0,0);	    
      a->SetDsReduceCuts(1,0,1,1);
    }   
  }
  if(WhichDATASet==2){//full cuts
    a=new DRecoDsInclusiveAnalysis(1,"DsTolnu/SP1005Skim/DsReco/Final");
    if(ReduceNtuple){   
      a->SetChain(chain);
      a->SetEvtReduceCuts(0,0);
      a->SetTagReduceCuts(1,1,0,0);	    
      a->SetDsReduceCuts(1,1,1,1);
    }   
  }
  if(WhichDATASet==3){//region of Ds mass below 1.8 
    a=new DRecoDsInclusiveAnalysis(1,"DsTolnu/SP1005Skim/DsReco/SideBand");
    if(ReduceNtuple){   
      a->SetChain(chain);
      a->SetEvtReduceCuts(0,0);
      a->SetTagReduceCuts(1,1,0,0);	    
      a->SetDsReduceCuts(2,0,1,1);
    }   
  }

  //small sample of ccbar
  if(WhichDATASet==10001){//loose cuts
    a=new DRecoDsInclusiveAnalysis(1,"DsTolnu/SP1005Skim/DsReco/Run5");
    if(ReduceNtuple){      
      TreeChain(chain,1,286,"DsTolnu/SP1005Skim/DsReco/Ntuples/Run5On/Ntuple");    
      a->SetChain(chain);
      a->SetEvtReduceCuts(0,0);
      a->SetTagReduceCuts(1,1,0,0);	    
      a->SetDsReduceCuts(1,0,1,1);
    }   
  }



  ///////////////////ccbar WS
  if(WhichDATASet>=4&&WhichDATASet<6)
    if(ReduceNtuple)
      if(ReduceNtuple){ 
	TreeChain(chain,1,47,"DsTolnu/SP1005Skim/DsReco/NtuplesWS/Run1On/Ntuple");    
	TreeChain(chain,1,138,"DsTolnu/SP1005Skim/DsReco/NtuplesWS/Run2On/Ntuple");    
	TreeChain(chain,1,74,"DsTolnu/SP1005Skim/DsReco/NtuplesWS/Run3On/Ntuple");    
	TreeChain(chain,1,223,"DsTolnu/SP1005Skim/DsReco/NtuplesWS/Run4On/Ntuple");    
	TreeChain(chain,1,286,"DsTolnu/SP1005Skim/DsReco/NtuplesWS/Run5On/Ntuple");    
	TreeChain(chain,1,174,"DsTolnu/SP1005Skim/DsReco/NtuplesWS/Run6On/Ntuple"); 	
	TreeChain(chain,1,28,"DsTolnu/SP1005Skim/DsReco/NtuplesWS/RunY2SOn/Ntuple");    
	TreeChain(chain,1,66,"DsTolnu/SP1005Skim/DsReco/NtuplesWS/RunY3SOn/Ntuple");
    
	TreeChain(chain,1,5,"DsTolnu/SP1005Skim/DsReco/NtuplesWS/Run1Off/Ntuple");    
	TreeChain(chain,1,15,"DsTolnu/SP1005Skim/DsReco/NtuplesWS/Run2Off/Ntuple");    
	TreeChain(chain,1,6,"DsTolnu/SP1005Skim/DsReco/NtuplesWS/Run3Off/Ntuple");    
	TreeChain(chain,1,18,"DsTolnu/SP1005Skim/DsReco/NtuplesWS/Run4Off/Ntuple");    
	TreeChain(chain,1,32,"DsTolnu/SP1005Skim/DsReco/NtuplesWS/Run5Off/Ntuple");    
	TreeChain(chain,1,18,"DsTolnu/SP1005Skim/DsReco/NtuplesWS/Run6Off/Ntuple");
	TreeChain(chain,1,3,"DsTolnu/SP1005Skim/DsReco/NtuplesWS/RunY2SOff/Ntuple");    
	TreeChain(chain,1,8,"DsTolnu/SP1005Skim/DsReco/NtuplesWS/RunY3SOff/Ntuple"); 
    }   
  
  if(WhichDATASet==4){//WS loose cut
    a=new DRecoDsInclusiveAnalysis(1,"DsTolnu/SP1005Skim/DsReco/NoPstarCutWS");
    if(ReduceNtuple){   
      a->SetChain(chain);
      a->SetEvtReduceCuts(0,0);
      a->SetTagReduceCuts(1,1,0,0);	    
      a->SetDsReduceCuts(1,0,1,1);
    }   
  }
  if(WhichDATASet==5){//WS full cut
    a=new DRecoDsInclusiveAnalysis(1,"DsTolnu/SP1005Skim/DsReco/FinalWS");
    if(ReduceNtuple){   
      a->SetChain(chain);
      a->SetEvtReduceCuts(0,0);
      a->SetTagReduceCuts(1,1,0,0);	    
      a->SetDsReduceCuts(1,1,1,1);
    }   
  }


  ///---------------ccbar Run3 Photon Study   
  if(WhichDATASet==1001){
    a=new DRecoDsInclusiveAnalysis(1,"DsTolnu/SP1005Skim/DsReco/PhotonStudy");
    if(ReduceNtuple){     
      TreeChain(chain,1,74,"DsTolnu/SP1005Skim/DsReco/Ntuples/Run3On/Ntuple");   
      a->SetChain(chain);
      a->SetEvtReduceCuts(0,0);
      a->SetTagReduceCuts(1,1,0,0);	    
      a->SetDsReduceCuts(1,1,1,1);
    }   
  }


  /////////////////-------------------------BB, uds ,tau  
  if(WhichDATASet==21){
    a=new DRecoDsInclusiveAnalysis(1,"DsTolnu/SP1235");
    if(ReduceNtuple){ 
      //1.04
      TreeChain(chain,1,5,"DsTolnu/SP1235/Ntuples/Run1On/Ntuple");
      TreeChain(chain,1,16,"DsTolnu/SP1235/Ntuples/Run2On/Ntuple");
      TreeChain(chain,1,9,"DsTolnu/SP1235/Ntuples/Run3On/Ntuple");
      TreeChain(chain,1,106,"DsTolnu/SP1235/Ntuples/Run4On/Ntuple");
      TreeChain(chain,1,31,"DsTolnu/SP1235/Ntuples/Run5On/Ntuple");
      TreeChain(chain,1,19,"DsTolnu/SP1235/Ntuples/Run6On/Ntuple");
      a->SetChain(chain);
      a->SetEvtReduceCuts(0,0);
      a->SetTagReduceCuts(1,1,0,0);	    
      a->SetDsReduceCuts(1,1,1,1);
    }   
  }
  if(WhichDATASet==22){
    a=new DRecoDsInclusiveAnalysis(1,"DsTolnu/SP1237");
    if(ReduceNtuple){ 
      //1.05
      TreeChain(chain,1,5,"DsTolnu/SP1237/Ntuples/Run1On/Ntuple");
      TreeChain(chain,1,15,"DsTolnu/SP1237/Ntuples/Run2On/Ntuple");
      TreeChain(chain,1,9,"DsTolnu/SP1237/Ntuples/Run3On/Ntuple");
      TreeChain(chain,1,108,"DsTolnu/SP1237/Ntuples/Run4On/Ntuple");
      TreeChain(chain,1,31,"DsTolnu/SP1237/Ntuples/Run5On/Ntuple");
      TreeChain(chain,1,20,"DsTolnu/SP1237/Ntuples/Run6On/Ntuple");
      a->SetChain(chain);
      a->SetEvtReduceCuts(0,0);
      a->SetTagReduceCuts(1,1,0,0);	    
      a->SetDsReduceCuts(1,1,1,1);
    }   
  }

  if(WhichDATASet==23){
    a=new DRecoDsInclusiveAnalysis(1,"DsTolnu/SP998");
    if(ReduceNtuple){ 
      //.9
      TreeChain(chain,1,23,"DsTolnu/SP998/Ntuples/Run1On/Ntuple");
      TreeChain(chain,1,3,"DsTolnu/SP998/Ntuples/Run1Off/Ntuple");
      TreeChain(chain,1,93,"DsTolnu/SP998/Ntuples/Run2On/Ntuple");
      TreeChain(chain,1,11,"DsTolnu/SP998/Ntuples/Run2Off/Ntuple");
      TreeChain(chain,1,69,"DsTolnu/SP998/Ntuples/Run3On/Ntuple");
      TreeChain(chain,1,11,"DsTolnu/SP998/Ntuples/Run3Off/Ntuple");
      TreeChain(chain,1,211,"DsTolnu/SP998/Ntuples/Run4On/Ntuple");
      TreeChain(chain,1,17,"DsTolnu/SP998/Ntuples/Run4Off/Ntuple");
      TreeChain(chain,1,278,"DsTolnu/SP998/Ntuples/Run5On/Ntuple");
      TreeChain(chain,1,31,"DsTolnu/SP998/Ntuples/Run5Off/Ntuple");
      TreeChain(chain,1,164,"DsTolnu/SP998/Ntuples/Run6On/Ntuple");
      TreeChain(chain,1,17,"DsTolnu/SP998/Ntuples/Run6Off/Ntuple");
      TreeChain(chain,1,15,"DsTolnu/SP998/Ntuples/RunY2SOn/Ntuple");
      TreeChain(chain,1,2,"DsTolnu/SP998/Ntuples/RunY2SOff/Ntuple");
      TreeChain(chain,1,34,"DsTolnu/SP998/Ntuples/RunY3SOn/Ntuple");
      TreeChain(chain,1,5,"DsTolnu/SP998/Ntuples/RunY3SOff/Ntuple");
      a->SetChain(chain);
      a->SetEvtReduceCuts(0,0);
      a->SetTagReduceCuts(1,1,0,0);	    
      a->SetDsReduceCuts(1,1,1,1);
    }   
  }
  if(WhichDATASet==24){
    a=new DRecoDsInclusiveAnalysis(1,"DsTolnu/SP3429");
    if(ReduceNtuple){ 
      //1.63
      TreeChain(chain,1,1,"DsTolnu/SP3429/Ntuples/Run1On/Ntuple");
      TreeChain(chain,1,1,"DsTolnu/SP3429/Ntuples/Run1Off/Ntuple");
      TreeChain(chain,1,1,"DsTolnu/SP3429/Ntuples/Run2On/Ntuple");
      TreeChain(chain,1,1,"DsTolnu/SP3429/Ntuples/Run2Off/Ntuple");
      TreeChain(chain,1,1,"DsTolnu/SP3429/Ntuples/Run3On/Ntuple");
      TreeChain(chain,1,1,"DsTolnu/SP3429/Ntuples/Run3Off/Ntuple");
      TreeChain(chain,1,1,"DsTolnu/SP3429/Ntuples/Run4On/Ntuple");
      TreeChain(chain,1,1,"DsTolnu/SP3429/Ntuples/Run4Off/Ntuple");
      TreeChain(chain,1,2,"DsTolnu/SP3429/Ntuples/Run5On/Ntuple");
      TreeChain(chain,1,1,"DsTolnu/SP3429/Ntuples/Run5Off/Ntuple");
      TreeChain(chain,1,1,"DsTolnu/SP3429/Ntuples/Run6On/Ntuple");
      TreeChain(chain,1,1,"DsTolnu/SP3429/Ntuples/Run6Off/Ntuple");
      TreeChain(chain,1,0,"DsTolnu/SP3429/Ntuples/RunY2SOn/Ntuple");
      TreeChain(chain,1,0,"DsTolnu/SP3429/Ntuples/RunY2SOff/Ntuple");
      TreeChain(chain,1,1,"DsTolnu/SP3429/Ntuples/RunY3SOn/Ntuple");
      TreeChain(chain,1,1,"DsTolnu/SP3429/Ntuples/RunY3SOff/Ntuple");
      a->SetChain(chain);
      a->SetEvtReduceCuts(0,0);
      a->SetTagReduceCuts(1,1,0,0);	    
      a->SetDsReduceCuts(1,1,1,1);
    }   
  }
  ///---------------------------------------------WS
  if(WhichDATASet==31){
    a=new DRecoDsInclusiveAnalysis(1,"DsTolnu/SP1235/FinalWS");
    if(ReduceNtuple){ 
      //1.04
      TreeChain(chain,1,5,"DsTolnu/SP1235/NtuplesWS/Run1On/Ntuple");
      TreeChain(chain,1,16,"DsTolnu/SP1235/NtuplesWS/Run2On/Ntuple");
      TreeChain(chain,1,9,"DsTolnu/SP1235/NtuplesWS/Run3On/Ntuple");
      TreeChain(chain,1,106,"DsTolnu/SP1235/NtuplesWS/Run4On/Ntuple");
      TreeChain(chain,1,31,"DsTolnu/SP1235/NtuplesWS/Run5On/Ntuple");
      TreeChain(chain,1,19,"DsTolnu/SP1235/NtuplesWS/Run6On/Ntuple");
      a->SetChain(chain);
      a->SetEvtReduceCuts(0,0);
      a->SetTagReduceCuts(1,1,0,0);	    
      a->SetDsReduceCuts(1,1,1,1);
    }   
  }
  if(WhichDATASet==32){
    a=new DRecoDsInclusiveAnalysis(1,"DsTolnu/SP1237/FinalWS");
    if(ReduceNtuple){ 
      //1.05
      TreeChain(chain,1,5,"DsTolnu/SP1237/NtuplesWS/Run1On/Ntuple");
      TreeChain(chain,1,15,"DsTolnu/SP1237/NtuplesWS/Run2On/Ntuple");
      TreeChain(chain,1,9,"DsTolnu/SP1237/NtuplesWS/Run3On/Ntuple");
      TreeChain(chain,1,108,"DsTolnu/SP1237/NtuplesWS/Run4On/Ntuple");
      TreeChain(chain,1,31,"DsTolnu/SP1237/NtuplesWS/Run5On/Ntuple");
      TreeChain(chain,1,20,"DsTolnu/SP1237/NtuplesWS/Run6On/Ntuple");
      a->SetChain(chain);
      a->SetEvtReduceCuts(0,0);
      a->SetTagReduceCuts(1,1,0,0);	    
      a->SetDsReduceCuts(1,1,1,1);
    }   
  }

  if(WhichDATASet==33){
    a=new DRecoDsInclusiveAnalysis(1,"DsTolnu/SP998/FinalWS");
    if(ReduceNtuple){ 
      //.9
      TreeChain(chain,1,23,"DsTolnu/SP998/NtuplesWS/Run1On/Ntuple");
      TreeChain(chain,1,3,"DsTolnu/SP998/NtuplesWS/Run1Off/Ntuple");
      TreeChain(chain,1,93,"DsTolnu/SP998/NtuplesWS/Run2On/Ntuple");
      TreeChain(chain,1,11,"DsTolnu/SP998/NtuplesWS/Run2Off/Ntuple");
      TreeChain(chain,1,69,"DsTolnu/SP998/NtuplesWS/Run3On/Ntuple");
      TreeChain(chain,1,11,"DsTolnu/SP998/NtuplesWS/Run3Off/Ntuple");
      TreeChain(chain,1,211,"DsTolnu/SP998/NtuplesWS/Run4On/Ntuple");
      TreeChain(chain,1,17,"DsTolnu/SP998/NtuplesWS/Run4Off/Ntuple");
      TreeChain(chain,1,278,"DsTolnu/SP998/NtuplesWS/Run5On/Ntuple");
      TreeChain(chain,1,31,"DsTolnu/SP998/NtuplesWS/Run5Off/Ntuple");
      TreeChain(chain,1,164,"DsTolnu/SP998/NtuplesWS/Run6On/Ntuple");
      TreeChain(chain,1,17,"DsTolnu/SP998/NtuplesWS/Run6Off/Ntuple");
      TreeChain(chain,1,15,"DsTolnu/SP998/NtuplesWS/RunY2SOn/Ntuple");
      TreeChain(chain,1,2,"DsTolnu/SP998/NtuplesWS/RunY2SOff/Ntuple");
      TreeChain(chain,1,34,"DsTolnu/SP998/NtuplesWS/RunY3SOn/Ntuple");
      TreeChain(chain,1,5,"DsTolnu/SP998/NtuplesWS/RunY3SOff/Ntuple");
      a->SetChain(chain);
      a->SetEvtReduceCuts(0,0);
      a->SetTagReduceCuts(1,1,0,0);	    
      a->SetDsReduceCuts(1,1,1,1);
    }   
  }
  if(WhichDATASet==34){
    a=new DRecoDsInclusiveAnalysis(1,"DsTolnu/SP3429/FinalWS");
    if(ReduceNtuple){ 
      //1.63
      TreeChain(chain,1,1,"DsTolnu/SP3429/NtuplesWS/Run1On/Ntuple");
      TreeChain(chain,1,1,"DsTolnu/SP3429/NtuplesWS/Run1Off/Ntuple");
      TreeChain(chain,1,1,"DsTolnu/SP3429/NtuplesWS/Run2On/Ntuple");
      TreeChain(chain,1,1,"DsTolnu/SP3429/NtuplesWS/Run2Off/Ntuple");
      TreeChain(chain,1,1,"DsTolnu/SP3429/NtuplesWS/Run3On/Ntuple");
      TreeChain(chain,1,1,"DsTolnu/SP3429/NtuplesWS/Run3Off/Ntuple");
      TreeChain(chain,1,1,"DsTolnu/SP3429/NtuplesWS/Run4On/Ntuple");
      TreeChain(chain,1,1,"DsTolnu/SP3429/NtuplesWS/Run4Off/Ntuple");
      TreeChain(chain,1,2,"DsTolnu/SP3429/NtuplesWS/Run5On/Ntuple");
      TreeChain(chain,1,1,"DsTolnu/SP3429/NtuplesWS/Run5Off/Ntuple");
      TreeChain(chain,1,1,"DsTolnu/SP3429/NtuplesWS/Run6On/Ntuple");
      TreeChain(chain,1,1,"DsTolnu/SP3429/NtuplesWS/Run6Off/Ntuple");
      TreeChain(chain,1,0,"DsTolnu/SP3429/NtuplesWS/RunY2SOn/Ntuple");
      TreeChain(chain,1,0,"DsTolnu/SP3429/NtuplesWS/RunY2SOff/Ntuple");
      TreeChain(chain,1,1,"DsTolnu/SP3429/NtuplesWS/RunY3SOn/Ntuple");
      TreeChain(chain,1,1,"DsTolnu/SP3429/NtuplesWS/RunY3SOff/Ntuple");
      a->SetChain(chain);
      a->SetEvtReduceCuts(0,0);
      a->SetTagReduceCuts(1,1,0,0);	    
      a->SetDsReduceCuts(1,1,1,1);
    }   
  }





  ///////////////////////Data Denominator
  if(WhichDATASet>=11&&WhichDATASet<=13)    
    if(ReduceNtuple){   
      TreeChain(chain,1,65,"DsTolnu/Data/Ntuples/Run1On/Ntuple");
      TreeChain(chain,1,7,"DsTolnu/Data/Ntuples/Run1Off/Ntuple");
      TreeChain(chain,1,211,"DsTolnu/Data/Ntuples/Run2On/Ntuple");
      TreeChain(chain,1,17,"DsTolnu/Data/Ntuples/Run2Off/Ntuple");
      TreeChain(chain,1,111,"DsTolnu/Data/Ntuples/Run3On/Ntuple");
      TreeChain(chain,1,7,"DsTolnu/Data/Ntuples/Run3Off/Ntuple");
      TreeChain(chain,1,351,"DsTolnu/Data/Ntuples/Run4On/Ntuple");
      TreeChain(chain,1,25,"DsTolnu/Data/Ntuples/Run4Off/Ntuple");
      TreeChain(chain,1,463,"DsTolnu/Data/Ntuples/Run5On/Ntuple");
      TreeChain(chain,1,40,"DsTolnu/Data/Ntuples/Run5Off/Ntuple");
      TreeChain(chain,1,243,"DsTolnu/Data/Ntuples/Run6On/Ntuple");
      TreeChain(chain,1,20,"DsTolnu/Data/Ntuples/Run6Off/Ntuple");
      TreeChain(chain,1,52,"DsTolnu/Data/Ntuples/RunY2SOn/Ntuple");
      TreeChain(chain,1,3,"DsTolnu/Data/Ntuples/RunY2SOff/Ntuple");
      TreeChain(chain,1,107,"DsTolnu/Data/Ntuples/RunY3SOn/Ntuple");
      TreeChain(chain,1,5,"DsTolnu/Data/Ntuples/RunY3SOff/Ntuple");
    }
  if(WhichDATASet==11){//loose cuts
    a=new DRecoDsInclusiveAnalysis(2,"DsTolnu/Data/NoPstarCut");
    if(ReduceNtuple){   
      a->SetChain(chain);
      a->SetEvtReduceCuts(0,0);
      a->SetTagReduceCuts(1,1,0,0);	    
      a->SetDsReduceCuts(1,0,1,1);
    }   
  }
  if(WhichDATASet==12){//full cuts
    a=new DRecoDsInclusiveAnalysis(2,"DsTolnu/Data/Final");
    if(ReduceNtuple){   
      a->SetChain(chain);
      a->SetEvtReduceCuts(0,0);
      a->SetTagReduceCuts(1,1,0,0);	    
      a->SetDsReduceCuts(1,1,1,1);
    }   
  }
  if(WhichDATASet==13){
    a=new DRecoDsInclusiveAnalysis(2,"DsTolnu/Data/Sideband");
    if(ReduceNtuple){   
      a->SetChain(chain);
      a->SetEvtReduceCuts(0,0);
      a->SetTagReduceCuts(1,1,0,0);	    
      a->SetDsReduceCuts(2,0,1,1);
    }   
  }
  ///////////////////////Data WS
  if(WhichDATASet>=14&&WhichDATASet<=15)    
    if(ReduceNtuple){   
      TreeChain(chain,1,65,"DsTolnu/Data/NtuplesWS/Run1On/Ntuple");
      TreeChain(chain,1,7,"DsTolnu/Data/NtuplesWS/Run1Off/Ntuple");
      TreeChain(chain,1,211,"DsTolnu/Data/NtuplesWS/Run2On/Ntuple");
      TreeChain(chain,1,17,"DsTolnu/Data/NtuplesWS/Run2Off/Ntuple");
      TreeChain(chain,1,111,"DsTolnu/Data/NtuplesWS/Run3On/Ntuple");
      TreeChain(chain,1,7,"DsTolnu/Data/NtuplesWS/Run3Off/Ntuple");
      TreeChain(chain,1,351,"DsTolnu/Data/NtuplesWS/Run4On/Ntuple");
      TreeChain(chain,1,25,"DsTolnu/Data/NtuplesWS/Run4Off/Ntuple");
      TreeChain(chain,1,463,"DsTolnu/Data/NtuplesWS/Run5On/Ntuple");
      TreeChain(chain,1,40,"DsTolnu/Data/NtuplesWS/Run5Off/Ntuple");
      TreeChain(chain,1,243,"DsTolnu/Data/NtuplesWS/Run6On/Ntuple");
      TreeChain(chain,1,20,"DsTolnu/Data/NtuplesWS/Run6Off/Ntuple");
      TreeChain(chain,1,52,"DsTolnu/Data/NtuplesWS/RunY2SOn/Ntuple");
      TreeChain(chain,1,3,"DsTolnu/Data/NtuplesWS/RunY2SOff/Ntuple");
      TreeChain(chain,1,107,"DsTolnu/Data/NtuplesWS/RunY3SOn/Ntuple");
      TreeChain(chain,1,5,"DsTolnu/Data/NtuplesWS/RunY3SOff/Ntuple");
    }
  if(WhichDATASet==14){//WS loose cuts
    a=new DRecoDsInclusiveAnalysis(2,"DsTolnu/Data/NoPstarCutWS");
    if(ReduceNtuple){   
      a->SetChain(chain);
      a->SetEvtReduceCuts(0,0);
      a->SetTagReduceCuts(1,1,0,0);	    
      a->SetDsReduceCuts(1,0,1,1);
    }   
  }  
  if(WhichDATASet==15){//WS full cuts
    a=new DRecoDsInclusiveAnalysis(2,"DsTolnu/Data/FinalWS");
    if(ReduceNtuple){   
      a->SetChain(chain);
      a->SetEvtReduceCuts(0,0);
      a->SetTagReduceCuts(1,1,0,0);	    
      a->SetDsReduceCuts(1,1,1,1);
    }   
  }  




  ///--------------- efficiency 
  //Muon eff
  if(WhichDATASet==2001){//
    a=new DRecoDsInclusiveAnalysis(0,"DsTolnu/MuonEff");
    if(ReduceNtuple){  
      TreeChain(chain,1,1,"DsTolnu/MuonEff/Ntuples/SP3046/Run1On/Ntuple");
      TreeChain(chain,1,1,"DsTolnu/MuonEff/Ntuples/SP3046/Run1Off/Ntuple");
      TreeChain(chain,1,1,"DsTolnu/MuonEff/Ntuples/SP3047/Run1On/Ntuple");
      TreeChain(chain,1,1,"DsTolnu/MuonEff/Ntuples/SP3047/Run1Off/Ntuple");
      TreeChain(chain,1,3,"DsTolnu/MuonEff/Ntuples/SP3046/Run2On/Ntuple");
      TreeChain(chain,1,1,"DsTolnu/MuonEff/Ntuples/SP3046/Run2Off/Ntuple");
      TreeChain(chain,1,3,"DsTolnu/MuonEff/Ntuples/SP3047/Run2On/Ntuple");
      TreeChain(chain,1,1,"DsTolnu/MuonEff/Ntuples/SP3047/Run2Off/Ntuple");
      TreeChain(chain,1,2,"DsTolnu/MuonEff/Ntuples/SP3046/Run3On/Ntuple");
      TreeChain(chain,1,1,"DsTolnu/MuonEff/Ntuples/SP3046/Run3Off/Ntuple");
      TreeChain(chain,1,2,"DsTolnu/MuonEff/Ntuples/SP3047/Run3On/Ntuple");
      TreeChain(chain,1,1,"DsTolnu/MuonEff/Ntuples/SP3047/Run3Off/Ntuple");
      TreeChain(chain,1,6,"DsTolnu/MuonEff/Ntuples/SP3046/Run4On/Ntuple");
      TreeChain(chain,1,1,"DsTolnu/MuonEff/Ntuples/SP3046/Run4Off/Ntuple");
      TreeChain(chain,1,6,"DsTolnu/MuonEff/Ntuples/SP3047/Run4On/Ntuple");
      TreeChain(chain,1,1,"DsTolnu/MuonEff/Ntuples/SP3047/Run4Off/Ntuple");
      TreeChain(chain,1,7,"DsTolnu/MuonEff/Ntuples/SP3046/Run5On/Ntuple");
      TreeChain(chain,1,1,"DsTolnu/MuonEff/Ntuples/SP3046/Run5Off/Ntuple");
      TreeChain(chain,1,7,"DsTolnu/MuonEff/Ntuples/SP3047/Run5On/Ntuple");
      TreeChain(chain,1,1,"DsTolnu/MuonEff/Ntuples/SP3047/Run5Off/Ntuple");
      TreeChain(chain,1,4,"DsTolnu/MuonEff/Ntuples/SP3046/Run6On/Ntuple");
      TreeChain(chain,1,1,"DsTolnu/MuonEff/Ntuples/SP3046/Run6Off/Ntuple");
      TreeChain(chain,1,4,"DsTolnu/MuonEff/Ntuples/SP3047/Run6On/Ntuple");
      TreeChain(chain,1,1,"DsTolnu/MuonEff/Ntuples/SP3047/Run6Off/Ntuple");
      TreeChain(chain,1,2,"DsTolnu/MuonEff/Ntuples/SP3046/RunY2SOn/Ntuple");
      TreeChain(chain,1,1,"DsTolnu/MuonEff/Ntuples/SP3046/RunY2SOff/Ntuple");
      TreeChain(chain,1,2,"DsTolnu/MuonEff/Ntuples/SP3047/RunY2SOn/Ntuple");
      TreeChain(chain,1,1,"DsTolnu/MuonEff/Ntuples/SP3047/RunY2SOff/Ntuple");
      TreeChain(chain,1,2,"DsTolnu/MuonEff/Ntuples/SP3046/RunY3SOn/Ntuple");
      TreeChain(chain,1,1,"DsTolnu/MuonEff/Ntuples/SP3046/RunY3SOff/Ntuple");
      TreeChain(chain,1,2,"DsTolnu/MuonEff/Ntuples/SP3047/RunY3SOn/Ntuple");
      TreeChain(chain,1,1,"DsTolnu/MuonEff/Ntuples/SP3047/RunY3SOff/Ntuple");
      a->SetChain(chain);
      a->SetEvtReduceCuts(0,0);
      a->SetTagReduceCuts(1,1,0,0);	    
      a->SetDsReduceCuts(1,1,1,1);
    }   
  }

  if(WhichDATASet==20011){//
    a=new DRecoDsInclusiveAnalysis(0,"DsTolnu");
    if(ReduceNtuple){  
      TreeChain(chain,1,1,"DsTolnu/MuonEff/Ntuples/SP3046/RunY2SOff/Ntuple");
      a->SetChain(chain);
      a->SetEvtReduceCuts(0,0);
      a->SetTagReduceCuts(1,1,0,0);	    
      a->SetDsReduceCuts(1,1,1,1);
    }   
  }




  //KKpiefficiency denominator
  if(WhichDATASet==2002){//
    a=new DRecoDsInclusiveAnalysis(0,"DsTolnu/KKpiEff");
    if(ReduceNtuple){   

      TreeChain(chain,1,2,"DsTolnu/KKpiEff/Ntuples/SP9775/Run1On/Ntuple");     
      TreeChain(chain,1,1,"DsTolnu/KKpiEff/Ntuples/SP9775/Run1Off/Ntuple"); 
      TreeChain(chain,1,6,"DsTolnu/KKpiEff/Ntuples/SP9775/Run2On/Ntuple");     
      TreeChain(chain,1,1,"DsTolnu/KKpiEff/Ntuples/SP9775/Run2Off/Ntuple"); 
      TreeChain(chain,1,4,"DsTolnu/KKpiEff/Ntuples/SP9775/Run3On/Ntuple");     
      TreeChain(chain,1,1,"DsTolnu/KKpiEff/Ntuples/SP9775/Run3Off/Ntuple"); 
      TreeChain(chain,1,11,"DsTolnu/KKpiEff/Ntuples/SP9775/Run4On/Ntuple");     
      TreeChain(chain,1,1,"DsTolnu/KKpiEff/Ntuples/SP9775/Run4Off/Ntuple"); 
      TreeChain(chain,1,14,"DsTolnu/KKpiEff/Ntuples/SP9775/Run5On/Ntuple");     
      TreeChain(chain,1,1,"DsTolnu/KKpiEff/Ntuples/SP9775/Run5Off/Ntuple"); 
      TreeChain(chain,1,8,"DsTolnu/KKpiEff/Ntuples/SP9775/Run6On/Ntuple");     
      TreeChain(chain,1,1,"DsTolnu/KKpiEff/Ntuples/SP9775/Run6Off/Ntuple");     
      //TreeChain(chain,1,,"DsTolnu/KKpiEff/Ntuples/SP9775/RunY2SOn/Ntuple");//MC collections lost   
      TreeChain(chain,1,1,"DsTolnu/KKpiEff/Ntuples/SP9775/RunY2SOff/Ntuple"); 
      TreeChain(chain,1,3,"DsTolnu/KKpiEff/Ntuples/SP9775/RunY3SOn/Ntuple");     
      TreeChain(chain,1,1,"DsTolnu/KKpiEff/Ntuples/SP9775/RunY3SOff/Ntuple"); 

      TreeChain(chain,1,2,"DsTolnu/KKpiEff/Ntuples/SP9776/Run1On/Ntuple");     
      TreeChain(chain,1,1,"DsTolnu/KKpiEff/Ntuples/SP9776/Run1Off/Ntuple"); 
      TreeChain(chain,1,6,"DsTolnu/KKpiEff/Ntuples/SP9776/Run2On/Ntuple");     
      TreeChain(chain,1,1,"DsTolnu/KKpiEff/Ntuples/SP9776/Run2Off/Ntuple"); 
      TreeChain(chain,1,4,"DsTolnu/KKpiEff/Ntuples/SP9776/Run3On/Ntuple");     
      TreeChain(chain,1,1,"DsTolnu/KKpiEff/Ntuples/SP9776/Run3Off/Ntuple"); 
      TreeChain(chain,1,11,"DsTolnu/KKpiEff/Ntuples/SP9776/Run4On/Ntuple");     
      TreeChain(chain,1,1,"DsTolnu/KKpiEff/Ntuples/SP9776/Run4Off/Ntuple"); 
      TreeChain(chain,1,14,"DsTolnu/KKpiEff/Ntuples/SP9776/Run5On/Ntuple");     
      TreeChain(chain,1,2,"DsTolnu/KKpiEff/Ntuples/SP9776/Run5Off/Ntuple"); 
      TreeChain(chain,1,8,"DsTolnu/KKpiEff/Ntuples/SP9776/Run6On/Ntuple");     
      TreeChain(chain,1,1,"DsTolnu/KKpiEff/Ntuples/SP9776/Run6Off/Ntuple");     
      TreeChain(chain,1,1,"DsTolnu/KKpiEff/Ntuples/SP9776/RunY2SOn/Ntuple");     
      TreeChain(chain,1,1,"DsTolnu/KKpiEff/Ntuples/SP9776/RunY2SOff/Ntuple"); 
      TreeChain(chain,1,3,"DsTolnu/KKpiEff/Ntuples/SP9776/RunY3SOn/Ntuple");     
      TreeChain(chain,1,1,"DsTolnu/KKpiEff/Ntuples/SP9776/RunY3SOff/Ntuple");

      a->SetChain(chain);
      a->SetEvtReduceCuts(0,0);
      a->SetTagReduceCuts(1,1,0,0);	    
      a->SetDsReduceCuts(1,1,1,1);
    }   
  }

  ///---------------KsK efficiency 
  if(WhichDATASet==2003){//
    a=new DRecoDsInclusiveAnalysis(0,"DsTolnu/KsKEff");
    if(ReduceNtuple){  
      TreeChain(chain,1,2,"DsTolnu/KsKEff/Ntuples/Run1On/Ntuple");
      TreeChain(chain,1,1,"DsTolnu/KsKEff/Ntuples/Run1Off/Ntuple");
      TreeChain(chain,1,5,"DsTolnu/KsKEff/Ntuples/Run2On/Ntuple");
      TreeChain(chain,1,1,"DsTolnu/KsKEff/Ntuples/Run2Off/Ntuple");
      TreeChain(chain,1,3,"DsTolnu/KsKEff/Ntuples/Run3On/Ntuple");
      TreeChain(chain,1,1,"DsTolnu/KsKEff/Ntuples/Run3Off/Ntuple");
      TreeChain(chain,1,9,"DsTolnu/KsKEff/Ntuples/Run4On/Ntuple");
      TreeChain(chain,1,1,"DsTolnu/KsKEff/Ntuples/Run4Off/Ntuple");
      TreeChain(chain,1,11,"DsTolnu/KsKEff/Ntuples/Run5On/Ntuple");
      TreeChain(chain,1,2,"DsTolnu/KsKEff/Ntuples/Run5Off/Ntuple");
      TreeChain(chain,1,7,"DsTolnu/KsKEff/Ntuples/Run6On/Ntuple");
      TreeChain(chain,1,1,"DsTolnu/KsKEff/Ntuples/Run6Off/Ntuple");
      TreeChain(chain,1,1,"DsTolnu/KsKEff/Ntuples/RunY2SOn/Ntuple");
      TreeChain(chain,1,1,"DsTolnu/KsKEff/Ntuples/RunY2SOff/Ntuple");
      TreeChain(chain,1,4,"DsTolnu/KsKEff/Ntuples/RunY3SOn/Ntuple");
      TreeChain(chain,1,1,"DsTolnu/KsKEff/Ntuples/RunY3SOff/Ntuple");
      a->SetChain(chain);
      a->SetEvtReduceCuts(0,0);
      a->SetTagReduceCuts(1,1,0,0);	    
      a->SetDsReduceCuts(1,1,1,1);
    }   
  }





  ///---------------NumCheck
  if(WhichDATASet==100001)
    a=new DRecoDsInclusiveAnalysis(0,"DsTolnu/NumCheck/Signal");
  
  if(WhichDATASet==100002)
    a=new DRecoDsInclusiveAnalysis(1,"DsTolnu/NumCheck/SP1005");
  






  return a;
}

