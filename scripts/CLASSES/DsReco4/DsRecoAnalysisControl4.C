#include "../../MACROS/TreeChain.C"
#include "DsRecoAnalysis4.h"



DsRecoAnalysis4* analyzeDsReco4(Int_t WhichDATASet=-1,Bool_t ReduceNtuple=0){
  
  ////read in preferences
  TChain* chain=NULL;
  if(ReduceNtuple)chain=new TChain("ntp1"); 
  Int_t MatterOrAntiMatter=0; 
  TString OutputDir;
  Int_t TruthMatch=0;
  Bool_t MCorDATA=0;
  TString myntupledir="DsTolnu";  
  DsRecoAnalysis4* a=NULL;

  if(WhichDATASet==-1){
    cout<<"Which Data Set?"<<endl;
    cout<<"Enter id>";    

    cin>>WhichDATASet;
    cout<<endl;
  }

  if(WhichDATASet==1){
    OutputDir=myntupledir+"/SP1005Sig/DsReco"; 
    MatterOrAntiMatter=0;   
    TruthMatch=0;
    MCorDATA=0;
    a=new DsRecoAnalysis4(MCorDATA,TruthMatch,MatterOrAntiMatter,OutputDir);

    if(ReduceNtuple){   
      TreeChain(chain,1,187,myntupledir+"/SP1005Sig/DsReco/NtuplesSmall/Run3On");    
      a->SetChain(chain);
      a->SetReduceCuts(0,1,1,1,1,1,1,1,1);
    }   
  }

  if(WhichDATASet>=20&&WhichDATASet<30){
    OutputDir=myntupledir+"/SP1005Sig/DsReco";
    if(WhichDATASet==20)//all cuts
      OutputDir+="Final"; 
    if(WhichDATASet==21)//no Extra gamma energy cut
      OutputDir+="NoExGamCut"; 
    if(WhichDATASet==22)//no X gams number cut
      OutputDir+="NoXNGamCut"; 
    if(WhichDATASet==23)//no vtx prob  cut
      OutputDir+="NoVtxCut"; 
    MatterOrAntiMatter=0;   
    TruthMatch=0;
    MCorDATA=0;
 
    a=new DsRecoAnalysis4(MCorDATA,TruthMatch,MatterOrAntiMatter,OutputDir);
    if(ReduceNtuple){
      TreeChain(chain,1,12,myntupledir+"/SP1005Sig/DsReco/Ntuples/Run1On");    
      TreeChain(chain,1,1,myntupledir+"/SP1005Sig/DsReco/Ntuples/Run1Off");    
      TreeChain(chain,1,34,myntupledir+"/SP1005Sig/DsReco/Ntuples/Run2On");    
      TreeChain(chain,1,3,myntupledir+"/SP1005Sig/DsReco/Ntuples/Run2Off");    
      TreeChain(chain,1,17,myntupledir+"/SP1005Sig/DsReco/Ntuples/Run3On");    
      TreeChain(chain,1,1,myntupledir+"/SP1005Sig/DsReco/Ntuples/Run3Off");    
      TreeChain(chain,1,50,myntupledir+"/SP1005Sig/DsReco/Ntuples/Run4On");    
      TreeChain(chain,1,3,myntupledir+"/SP1005Sig/DsReco/Ntuples/Run4Off");  
      TreeChain(chain,1,74,myntupledir+"/SP1005Sig/DsReco/Ntuples/Run5On");    
      TreeChain(chain,1,7,myntupledir+"/SP1005Sig/DsReco/Ntuples/Run5Off");    
      TreeChain(chain,1,21,myntupledir+"/SP1005Sig/DsReco/Ntuples/Run6On");    
      TreeChain(chain,1,5,myntupledir+"/SP1005Sig/DsReco/Ntuples/Run6Off");      
      a->SetChain(chain);
    }   

    if(WhichDATASet==20)//all cuts
      a->SetReduceCuts(0,1,1,1,1,1,1,1,1);
    if(WhichDATASet==21)//all cuts
      a->SetReduceCuts(0,1,1,1,1,1,1,1,0);
    if(WhichDATASet==22)//all cuts
      a->SetReduceCuts(0,1,1,1,1,1,1,0,0);
    if(WhichDATASet==23)//all cuts
      a->SetReduceCuts(0,1,1,1,1,0,1,0,0);
    
  }


  if(WhichDATASet==30){
    OutputDir=myntupledir+"/SP1005Skim/DsReco"; 
    MatterOrAntiMatter=0;   
    TruthMatch=0;
    MCorDATA=0;
    a=new DsRecoAnalysis4(MCorDATA,TruthMatch,MatterOrAntiMatter,OutputDir);

    if(ReduceNtuple){
      TreeChain(chain,1,200,myntupledir+"/SP1005Skim/DsReco/NtuplesSmall");    
      a->SetChain(chain);
      a->SetReduceCuts(0,1,1,1,1,1,1,1,1);
    }   

  }
  if(WhichDATASet>=40&&WhichDATASet<50){
    OutputDir=myntupledir+"/SP1005Skim/DsReco/"; 
    if(WhichDATASet==40)//all cuts
      OutputDir+="Final"; 
    if(WhichDATASet==41)//no Extra gamma energy cut
      OutputDir+="NoExGamCut"; 
    if(WhichDATASet==42)//no X gams number cut
      OutputDir+="NoXNGamCut"; 
    if(WhichDATASet==43)//no vtx prob  cut
      OutputDir+="NoVtxCut";    
    MatterOrAntiMatter=0;   
    TruthMatch=0;
    MCorDATA=0;
    a=new DsRecoAnalysis4(MCorDATA,TruthMatch,MatterOrAntiMatter,OutputDir);

    if(ReduceNtuple){
      TreeChain(chain,1,600,myntupledir+"/SP1005Skim/DsReco/Ntuples");    
      a->SetChain(chain);
    }   
    if(WhichDATASet==40)//
      a->SetReduceCuts(0,1,1,1,1,1,1,1,1);
    if(WhichDATASet==41)//
      a->SetReduceCuts(0,1,1,1,1,1,1,1,0);
    if(WhichDATASet==42)//
      a->SetReduceCuts(0,1,1,1,1,1,1,0,0);
    if(WhichDATASet==43)//
      a->SetReduceCuts(0,1,1,1,1,0,1,0,0);
  }




  cout<<"The following data-set will be analyzed: "<<OutputDir<<endl;   
  
  return a;
}

