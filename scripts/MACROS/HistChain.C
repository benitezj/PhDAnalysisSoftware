#include "../Globals.h"

TChain* MyK0Chain(Int_t firstfile, Int_t lastfile,Int_t DATAorMC){
  TChain* chain=new TChain("ntp666");
 
  TFile* file;
  TString filename;

  Int_t ngoodfiles=0;
  for(Int_t n=firstfile;n<=lastfile;n++){
    
    if(DATAorMC==1)filename=TString("MyK0MiniAnalysis2_DATA")+(long)n+".root";   
    if(DATAorMC==2)filename=TString("MyK0MiniAnalysis2_MC")+(long)n+".root";
    

    file=new TFile(filename,"r");
    
    if(!file->IsZombie()){
      chain->Add(filename);
      ngoodfiles++;
    } else cout<<filename<<" is not good."<<endl;
    
    file->Close();
    if(file)delete file;
    
  }

  cout<<ngoodfiles<<" good files chained."<<endl;

  return chain;
}




TH1F* MyK0HChain(Int_t firstfile, Int_t lastfile,Int_t DATAorMC,Int_t histid){

  TH1F* h;  
  TFile* f[1000];

  if(DATAorMC==1)f[1]=new TFile(TString("MyK0MiniAnalysis2_DATA")+(long)firstfile+".root","r");
  if(DATAorMC==2)f[1]=new TFile(TString("MyK0MiniAnalysis2_MC")+(long)firstfile+".root","r");
  h=(TH1F*)f[1]->Get(TString("h1d")+(long)histid);

  for(Int_t n=firstfile+1;n<=lastfile;n++){
    if(DATAorMC==1)f[n-firstfile]=new TFile(TString("MyK0MiniAnalysis2_DATA")+(long)n+".root","r");
    if(DATAorMC==2)f[n-firstfile]=new TFile(TString("MyK0MiniAnalysis2_MC")+(long)n+".root","r");
    
    if(!f[n-firstfile]->IsZombie())h->Add((TH1F*)f[n-firstfile]->Get(TString("h1d")+(long)histid)); 

    f[n-firstfile]->Close();
    if( f[n-firstfile])delete  f[n-firstfile];
  }

  return (TH1F*)h->Clone();
}
