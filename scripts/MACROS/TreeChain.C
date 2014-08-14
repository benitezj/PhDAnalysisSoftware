#ifndef TreeChain_C
#define TreeChain_C

#include "../Globals.h"


void TreeChain(TChain* chain,Int_t firstfile, Int_t lastfile,TString NtupleDir){

  TString ntuplename="ntp1";
  TString ntuplename2="ntp1002";
  TFile* file=NULL;
  TList* keys=NULL;
  TString filename;
  Int_t ngoodfiles=0;
  for(Int_t n=firstfile;n<=lastfile;n++){    
    filename=NtupleDir+"/Ntuple"+(long)n+".root";          

    file=new TFile(filename,"r");
    if(file&&!file->IsZombie()){
      keys=file->GetListOfKeys();    
      if(keys&&keys->GetSize()>0){	
	if(file->Get(ntuplename)||file->Get(ntuplename2)){	
	  chain->Add(filename);
	  ngoodfiles++;
	}// else cout<<filename<<" is not good."<<endl;	
      }
      file->Close();           
    }
    delete file;      
  }
  cout<<ngoodfiles<<" good files chained in "<<NtupleDir<<endl;

  gROOT->cd();
  return;
}



#endif
