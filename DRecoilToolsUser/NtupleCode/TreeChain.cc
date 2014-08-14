
#include "DRecoilToolsUser/NtupleCode/TreeChain.hh"
#include <TFile.h>
#include <TList.h>
#include <iostream>
#include <iomanip>

using std::ostream;

using namespace std;

void TreeChain(TChain* chain,Int_t firstfile, Int_t lastfile,TString NtupleName){

  TString ntuplename = "ntp1" ;
  TString ntuplename2 = "ntp1002" ;
  TFile* file = NULL ;
  TList* keys = NULL ;
  TTree* ntp1 = NULL ;
  TTree* ntp2 = NULL ;
  TString filename ;
  Int_t ngoodfiles = 0 ;
  for(Int_t n = firstfile ; n<=lastfile ; n++)
  {    
    filename=NtupleName+(long)n+".root" ;    
    file=new TFile(filename,"r") ;

    if(file && !file->IsZombie())
    {
      keys = file->GetListOfKeys() ;
      if(keys && keys->GetSize()>0)
      {
    	if(file->Get(ntuplename) || file->Get(ntuplename2))
    	{
	  ntp1 = (TTree*)file->Get(ntuplename) ;
	  if(!ntp1) ntp2 = (TTree*)file->Get(ntuplename2) ;
	  Int_t nEntries = 0 ;
	  if(ntp1) nEntries += ntp1->GetEntries() ;
	  if(ntp2) nEntries += ntp2->GetEntries() ;
	  if(nEntries>0) chain->Add(filename) ;
    	  ngoodfiles++ ;
	  delete ntp2 ;
	  delete ntp1 ;
    	}
	else cout<<filename<<" has no ntuple" << endl ;
      }
      else cout<<filename<<" has bad keys" << endl ;
      file->Close() ;
    }
    else cout<<filename<<" is Zombie" << endl ;
    delete file ;
  }
  cout << ngoodfiles<<"/"<< (lastfile+1-firstfile)<< " good files chained in " << NtupleName << endl ;

  //gROOT->cd() ;
  return ;
}


