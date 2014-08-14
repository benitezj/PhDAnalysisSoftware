#include "../tmp/Linux26SL5_i386_gcc412/DRecoilToolsUser/NtupleCode/TreeChain.hh"
#include "../tmp/Linux26SL5_i386_gcc412/DRecoilToolsUser/NtupleCode/DRecoTagAnalysis.hh"

DRecoTagAnalysis* analyzeTag(Int_t WhichDATASet=-1,Bool_t ReduceNtuple=0){
  
  ////read in preferences
  TChain* chain=NULL;
  if(ReduceNtuple)chain=new TChain("ntp1"); 
  TString OutputDir;
  Int_t DataType=0;
  TString myntupledir="DsTolnu";  
  DRecoTagAnalysis* a=NULL;

  if(WhichDATASet==-1){
    cout<<"Which Data Set?"<<endl;
    cout<<"Enter id>";    
    cin>>WhichDATASet;
    cout<<endl;
  }
  

  ///Tag- Optimization
  if(WhichDATASet==20){
    OutputDir=myntupledir+"/TagOptRaw"; 
    DataType=2;
    a=new DRecoTagAnalysis(DataType,OutputDir);
    if(ReduceNtuple){
      TreeChain(chain,1,300,myntupledir+"/SP1005Skim/TagOpt/NtuplesData/Run3/Ntuple");//1213 total
      a->SetChain(chain);
      a->SetEvtReduceCuts(0,0);
      a->SetTagReduceCuts(0,1,0,1);
    }
  } 
  ///Before Ds Reco
  if(WhichDATASet>=201&&WhichDATASet<=215){
    if(WhichDATASet==201)
      OutputDir=myntupledir+"/TagOptRaw/Mode1"; 
    if(WhichDATASet==202)
      OutputDir=myntupledir+"/TagOptRaw/Mode2"; 
    if(WhichDATASet==203)
      OutputDir=myntupledir+"/TagOptRaw/Mode3"; 
    if(WhichDATASet==204)
      OutputDir=myntupledir+"/TagOptRaw/Mode4"; 
    if(WhichDATASet==205)
      OutputDir=myntupledir+"/TagOptRaw/Mode5"; 
    if(WhichDATASet==206)
      OutputDir=myntupledir+"/TagOptRaw/Mode6"; 
    if(WhichDATASet==211)
      OutputDir=myntupledir+"/TagOptRaw/Mode11"; 
    if(WhichDATASet==212)
      OutputDir=myntupledir+"/TagOptRaw/Mode12"; 
    if(WhichDATASet==213)
      OutputDir=myntupledir+"/TagOptRaw/Mode13"; 
    if(WhichDATASet==214)
      OutputDir=myntupledir+"/TagOptRaw/Mode14"; 
    if(WhichDATASet==215)
      OutputDir=myntupledir+"/TagOptRaw/Mode15"; 
    DataType=2;
    a=new DRecoTagAnalysis(DataType,OutputDir);
    if(ReduceNtuple){
      TreeChain(chain,1,300,myntupledir+"/SP1005Skim/TagOpt/NtuplesData/Run3/Ntuple");
      a->SetChain(chain);
      a->SetEvtReduceCuts(0,0);
      if(WhichDATASet==201)
	a->SetTagReduceCuts(0,1,0,1);	
      if(WhichDATASet==202)
	a->SetTagReduceCuts(0,1,0,2);	
      if(WhichDATASet==203)
	a->SetTagReduceCuts(0,1,0,3);	
      if(WhichDATASet==204)
	a->SetTagReduceCuts(0,1,0,4);	   
      if(WhichDATASet==205)
	a->SetTagReduceCuts(0,1,0,5);	   
      if(WhichDATASet==206)
	a->SetTagReduceCuts(0,1,0,6);
      if(WhichDATASet==211)
	a->SetTagReduceCuts(0,1,0,11);	
      if(WhichDATASet==212)
	a->SetTagReduceCuts(0,1,0,12);	
      if(WhichDATASet==213)
	a->SetTagReduceCuts(0,1,0,13);	
      if(WhichDATASet==214)
	a->SetTagReduceCuts(0,1,0,14);	   
      if(WhichDATASet==215)	    
	a->SetTagReduceCuts(0,1,0,15);	   
    }
  }




  ///Skim Testing
  if(WhichDATASet==40){
    OutputDir=myntupledir+"/fDsSkimTest"; 
    DataType=2;
    a=new DRecoTagAnalysis(DataType,OutputDir);
    if(ReduceNtuple){
      TreeChain(chain,1,334,myntupledir+"/fDsSkimTest/Run3On/Ntuple");//1/3 of all Run3
      a->SetChain(chain);
      a->SetEvtReduceCuts(0,0);
      a->SetTagReduceCuts(0,1,1,0);
    }
  } 



  ///Dstar Tagging
  if(WhichDATASet==50){
    OutputDir=myntupledir+"/DstarTagOpt"; 
    DataType=2;
    a=new DRecoTagAnalysis(DataType,OutputDir);
    if(ReduceNtuple){
      TreeChain(chain,1,1213,myntupledir+"/DstarTagOpt/Run3/Ntuple");//1213 total
      a->SetChain(chain);
      a->SetEvtReduceCuts(0,0);
      a->SetTagReduceCuts(0,1,1,0);
    }
  } 


  cout<<"The following data-set will be analyzed: "<<OutputDir<<endl;   
  
  return a;
}

