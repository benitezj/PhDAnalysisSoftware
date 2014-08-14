

#include <TChain.h>
#include <TString.h>
#include <TROOT.h>
#include <TH1F.h>
#include <TVector3.h>
#include <TLorentzVector.h>
#include <iomanip.h>
#include <TCanvas.h>
using namespace std;
using std::ostream;

#define MAXMCLEN 100

void KKpiMCTest(){

  TChain chain("ntp1");
//   for(Int_t i=1;i<=4;i++)
//     chain.Add(TString("DsTolnu/KKpiEff/Ntuples/SP9775/Run3On/Ntuple")+(long)i+".root");

  for(Int_t i=1;i<=34;i++)
    chain.Add(TString("DsTolnu/KKpiEff/Ntuples/SP9775/Run3On/Ntuple")+(long)i+".root");

//   for(Int_t i=1;i<=14;i++)
//    chain.Add(TString("DsTolnu/KKpiEff/Ntuples/SP9775/Run5On/Ntuple")+(long)i+".root");
//   for(Int_t i=1;i<=4;i++)
//     chain.Add(TString("DsTolnu/KKpiEff/Ntuples/SP9776/Run3On/Ntuple")+(long)i+".root");
//   for(Int_t i=1;i<=14;i++)
//     chain.Add(TString("DsTolnu/KKpiEff/Ntuples/SP9776/Run5On/Ntuple")+(long)i+".root");


//   for(Int_t i=1;i<=4;i++)
//     chain.Add(TString("DsTolnu/KKpi/Signal/Ntuples/SP9775/Run3On/Ntuple")+(long)i+".root");
//   for(Int_t i=1;i<=14;i++)
//     chain.Add(TString("DsTolnu/KKpi/Signal/Ntuples/SP9775/Run5On/Ntuple")+(long)i+".root");
//   for(Int_t i=1;i<=4;i++)
//     chain.Add(TString("DsTolnu/KKpi/Signal/Ntuples/SP9776/Run3On/Ntuple")+(long)i+".root");
//   for(Int_t i=1;i<=14;i++)
//     chain.Add(TString("DsTolnu/KKpi/Signal/Ntuples/SP9776/Run5On/Ntuple")+(long)i+".root");

  

  Int_t   mcLen;
  Int_t   mcLund[MAXMCLEN];
  Int_t   mothIdx[MAXMCLEN];
  Int_t   dauLen[MAXMCLEN];
  Int_t   dauIdx[MAXMCLEN];
  Float_t mcmass[MAXMCLEN]; 
  Float_t mccosth[MAXMCLEN];
  Float_t mcphi[MAXMCLEN];
  Float_t mcp3[MAXMCLEN];
  Float_t mcenergy[MAXMCLEN];
  Float_t mccosthCM[MAXMCLEN];
  Float_t mcphiCM[MAXMCLEN];
  Float_t mcp3CM[MAXMCLEN];
  Float_t mcenergyCM[MAXMCLEN];
  
  if(chain.GetBranch("mcLund"))chain.SetBranchAddress("mcLund",mcLund);
  if(chain.GetBranch("mothIdx"))chain.SetBranchAddress("mothIdx",mothIdx);
  if(chain.GetBranch("dauLen"))chain.SetBranchAddress("dauLen",dauLen);
  if(chain.GetBranch("dauIdx"))chain.SetBranchAddress("dauIdx",dauIdx);
  if(chain.GetBranch("mcLen"))chain.SetBranchAddress("mcLen",&mcLen);
  if(chain.GetBranch("mcmass"))chain.SetBranchAddress("mcmass",mcmass);
  if(chain.GetBranch("mcp3"))chain.SetBranchAddress("mcp3",mcp3);
  if(chain.GetBranch("mccosth"))chain.SetBranchAddress("mccosth",mccosth);
  if(chain.GetBranch("mcphi"))chain.SetBranchAddress("mcphi",mcphi);
  if(chain.GetBranch("mcenergy"))chain.SetBranchAddress("mcenergy",mcenergy);
  if(chain.GetBranch("mcp3CM"))chain.SetBranchAddress("mcp3CM",mcp3CM);
  if(chain.GetBranch("mccosthCM"))chain.SetBranchAddress("mccosthCM",mccosthCM); 
  if(chain.GetBranch("mcphiCM"))chain.SetBranchAddress("mcphiCM",mcphiCM);
  if(chain.GetBranch("mcenergyCM"))chain.SetBranchAddress("mcenergyCM",mcenergyCM);
  
  TH1F*HKpiMass=new TH1F("HKpiMass","",2000,.2,2.2);
  TH1F*HKKMass=new TH1F("HKKMass","",300,.5,3.5);
  TH1F*HMCLen=new TH1F("HMCLen","",80,0,80);

  Int_t evtid=0;
  while(chain.GetEntry(evtid++)){
    
    for(Int_t m=0;m<mcLen;m++){
      //if(abs(mcLund[m])==433){//found Ds*
      if(mcLund[m]==433){//found Ds*
	Int_t DsMCidx=-1;
	if(dauLen[m]>0&&abs(mcLund[dauIdx[m]])==431)DsMCidx=dauIdx[m];
	if(dauLen[m]>1&&abs(mcLund[dauIdx[m]+1])==431)DsMCidx=dauIdx[m]+1;
	if(dauLen[m]>2&&abs(mcLund[dauIdx[m]+2])==431)DsMCidx=dauIdx[m]+2;
	if(DsMCidx<0){
	  cout<<"not a Ds "<<dauLen[m]<<" "<<mcLund[dauIdx[m]]<<" "<<mcLund[dauIdx[m]+1]<<" "<<mcLund[dauIdx[m]+2]<<endl;
	  continue;
	}
	
	Int_t nDsDaus=dauLen[DsMCidx];
	Int_t PiMCidx=-1;
	Int_t KPlusMCidx=-1;
	Int_t KMinusMCidx=-1;
	for(Int_t d=0;d<nDsDaus;d++){
	  if(abs(mcLund[dauIdx[DsMCidx]+d])==211)PiMCidx=dauIdx[DsMCidx]+d;
	  if(mcLund[dauIdx[DsMCidx]+d]==321)KPlusMCidx=dauIdx[DsMCidx]+d;
	  if(mcLund[dauIdx[DsMCidx]+d]==-321)KMinusMCidx=dauIdx[DsMCidx]+d;
	}
	if(PiMCidx<0||KPlusMCidx<0||KMinusMCidx<0){
// 	  cout<<evtid<<"     Indexes not right "<<PiMCidx<<" "<<KPlusMCidx<<" "<<KMinusMCidx<<"  nDsDaus="<<nDsDaus<<endl;
// 	  for(Int_t d=0;d<nDsDaus;d++)
// 	    cout<<" "<<mcLund[dauIdx[DsMCidx]+d];
// 	  cout<<endl;
	  continue;
	}

	Int_t KMCidx=mcLund[PiMCidx]*mcLund[KPlusMCidx] < 0 ? KPlusMCidx : KMinusMCidx ;

	//compute the Kpi mass
	TVector3 piP3;piP3.SetMagThetaPhi(mcp3CM[PiMCidx],acos(mccosthCM[PiMCidx]),mcphiCM[PiMCidx]);
	TLorentzVector piP4(piP3,mcenergyCM[PiMCidx]);

	TVector3 KP3;KP3.SetMagThetaPhi(mcp3CM[KMCidx],acos(mccosthCM[KMCidx]),mcphiCM[KMCidx]);
	TLorentzVector KP4(KP3,mcenergyCM[KMCidx]);

	TVector3 KPlusP3;KPlusP3.SetMagThetaPhi(mcp3CM[KPlusMCidx],acos(mccosthCM[KPlusMCidx]),mcphiCM[KPlusMCidx]);
	TLorentzVector KPlusP4(KPlusP3,mcenergyCM[KPlusMCidx]);

	TVector3 KMinusP3;KMinusP3.SetMagThetaPhi(mcp3CM[KMinusMCidx],acos(mccosthCM[KMinusMCidx]),mcphiCM[KMinusMCidx]);
	TLorentzVector KMinusP4(KMinusP3,mcenergyCM[KMinusMCidx]);
	
	HKpiMass->Fill((piP4+KP4)*(piP4+KP4));
	HKKMass->Fill((KPlusP4+KMinusP4)*(KPlusP4+KMinusP4));


// 	//for bad Kpi region print the decay tree
// 	if(.7<(piP4+KP4).Mag2()&&(piP4+KP4).Mag2()<.9){
// 	  cout<<evtid<<" "<<mcLen;
// 	  for(Int_t m=0;m<3;m++){
// 	    cout<<" "<<mcLund[m];
// 	  }
// 	  cout<<endl;
// 	  for(Int_t m2=0;m2<dauLen[2];m2++){
// 	    cout<<" "<<mcLund[dauIdx[2]+m2]<<": ";
// 	    for(Int_t m3=0;m3<dauLen[dauIdx[2]+m2];m3++){
// 	      cout<<" "<<mcLund[dauIdx[dauIdx[2]+m2]+m3]<<"(";
// 	      for(Int_t m4=0;m4<dauLen[dauIdx[dauIdx[2]+m2]+m3];m4++){
// 		cout<<" "<<mcLund[dauIdx[dauIdx[dauIdx[2]+m2]+m3]+m4];
// 	      }
// 	      cout<<")";
// 	    }
// 	    cout<<endl;
// 	  }
// 	  cout<<endl;
// 	}

      }

    }
  }
  cout<<evtid<<" events done"<<endl;

  TCanvas* C=new TCanvas("C");
  C->Divide(2,2);
  C->cd(1);
  HKpiMass->Draw();
  C->cd(2);
  HKKMass->Draw();
  C->cd(3);
  chain.Draw("mcLen>>HMCLen");
  HMCLen->Draw();
  
}




