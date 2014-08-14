#include "../Globals.h"
#include <TChain.h>
#include <TString.h>
#include <iostream>
using namespace std;


void countCands(TString filename,TString particle){
  TChain chain("ntp1");
  chain.Add(filename);
  
  Int_t nCands=0;
  chain.SetBranchAddress(particle,&nCands);

  
  Int_t total=0;
  Int_t eventid=0;
  while(chain.GetEntry(eventid,0)>0){          
    eventid++;	
    //cout<<eventid<<" "<<nCands<<endl;
    total+=nCands;    

  }
  
  cout<<"Total="<<total<<endl;
  
  return;
}


void printCands(TString filename,TString particle,TString d1name,TString d2name){
  TChain chain("ntp1");
  chain.Add(filename);
  chain.SetBranchStatus("*",0);

  Int_t nCands=0;
  chain.SetBranchAddress(TString("n")+particle,&nCands);

  Float_t candMass[1000];
  chain.SetBranchAddress(particle+"Mass",candMass);
  chain.SetBranchStatus(particle+"Mass",1);
  Int_t candd1Idx[1000];
  chain.SetBranchAddress(particle+"d1Idx",candd1Idx);
  chain.SetBranchStatus(particle+"d1Idx",1);
  Int_t candd2Idx[1000];
  chain.SetBranchAddress(particle+"d2Idx",candd2Idx);
  chain.SetBranchStatus(particle+"d2Idx",1);

  Float_t d1Mass[1000];
  chain.SetBranchAddress(d1name+"Mass",d1Mass);
  chain.SetBranchStatus(d1name+"Mass",1);
  Float_t d2Mass[1000];
  chain.SetBranchAddress(d2name+"Mass",d2Mass);
  chain.SetBranchStatus(d2name+"Mass",1);
   
   
  Float_t dm;
  
  Int_t total=0;
  Int_t eventid=0;
  while(chain.GetEntry(eventid,0)>0){          
    eventid++;	
    if(nCands<1) continue;

    cout<<"Event: "<<eventid<<" nParticles: "<<nCands<<endl;
    
    Int_t Idx=-1;
    while( Idx < nCands-1){
      Idx++;      
      
      dm=candMass[Idx]-d1Mass[candd1Idx[Idx]]-d2Mass[candd2Idx[Idx]];
      //if(dm<.015||.045<dm){      
      printf("    %d, %.5f      %d, %.5f      %d, %.5f   %.5f\n",
	     Idx,candMass[Idx]
	     ,candd1Idx[Idx],d1Mass[candd1Idx[Idx]]
	     ,candd2Idx[Idx],d2Mass[candd2Idx[Idx]]
	     ,dm);

	// }
      
    }
    total+=nCands;    

  }
  
  cout<<"Total="<<total<<endl;
  
  return;
}

