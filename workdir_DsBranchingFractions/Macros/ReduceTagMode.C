#define NMAXTAG 300
#define PiPDGMass .139570 
#define KPDGMass .49368 
#define K0PDGMass .49767
#define DstarPDGMass 2.009936 //charged
#define Dstar0PDGMass 2.00662
#define D0PDGMass  1.864847 //1.8645=old mass
#define DPlusPDGMass 1.8693 
#define DsPDGMass 1.9693
#define DsStPDGMass 2.112
#define LambdaCPDGMass 2.2849 
#define Pi0PDGMass .134976
#define ElectronPDGMass .000510999
#define MuonPDGMass .1056584
#define ProtonPDGMass .9382723
#define EtaPDGMass .54784 

#define myDstarLund 413
#define myDstar0Lund 423
#define myD0Lund 421
#define myDPlusLund 411
#define myDsLund 431
#define myDsStLund 433
#define myLambdaCLund 4122
#define myKLund 321
#define myKS0Lund 310
#define myPiLund 211
#define myPi0Lund 111
#define myPhiLund 333
#define myRhoCLund 213
#define myBPlusLund 521

#define myEtaLund 221 
#define myEtaPLund 331

#define myPLund 2212
#define myLzLund 3122
#define mySzLund 3212
#define myGammaLund 22
#define myELund 11
#define myMuLund 13
#define myNuELund 12
#define myNuMuLund 14
#define myD2420ChargedLund 10413
#define myD2420NeutralLund 10423

#include "Riostream.h"
#include <fstream>
#include <iomanip>
#include <time.h>
#include <math.h>
#include "TROOT.h"
#include "TSystem.h"
#include "TMinuit.h"
#include "TDirectory.h"
#include "TChain.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TVector3.h"
#include "TLorentzRotation.h"
#include "TLorentzVector.h"
#include "TLine.h"
#include "TGraphErrors.h"
#include "TLatex.h"
 
using std::streambuf;
using std::ostream;
using namespace std;


#include "TChain.h"
#include "TTree.h"
#include "../tmp/Linux26SL5_i386_gcc412/DRecoilToolsUser/NtupleCode/TreeChain.hh"
#include "TFile.h"
#include "TROOT.h"
#include "TMath.h"
#include <stdio.h>
#include <iomanip>


void ReduceTagMode(){


  TChain* _fChain=new TChain("ntp1");
  TreeChain(_fChain,1,322,"DsTolnu_G/ntp/skim-AllEvents-Run3-OnPeak-R24-fDs-");
  //TreeChain(_fChain,1,32,"DsTolnu_G/ntp/skim-AllEvents-Run3-OnPeak-R24-fDs-");

  Float_t eePx;      
  Float_t eePy;   
  Float_t eePz;    
  Float_t eeE;     
  TVector3 eeVelVec;//velocity vector of e+e- in lab

  Int_t   nTag;
  Float_t TagMass[NMAXTAG];  
  Float_t Tagp3CM[NMAXTAG];  
  Float_t TagcosthCM[NMAXTAG];  
  Float_t TagphiCM[NMAXTAG];  
  Float_t TagenergyCM[NMAXTAG];  
  Float_t TagChi2[NMAXTAG];
  Int_t   TagnDof[NMAXTAG];  
  Int_t   TagVtxStatus[NMAXTAG];
  Float_t TagVtxx[NMAXTAG];  
  Float_t TagVtxy[NMAXTAG];  
  Float_t TagVtxz[NMAXTAG];  
  Int_t   TagLund[NMAXTAG];  
  Int_t   TagnDaus[NMAXTAG];
  Int_t   Tagd1Lund[NMAXTAG];  
  Int_t   Tagd1Idx[NMAXTAG];  
  Int_t   Tagd2Lund[NMAXTAG];  
  Int_t   Tagd2Idx[NMAXTAG];   
  Int_t   Tagd3Lund[NMAXTAG];  
  Int_t   Tagd3Idx[NMAXTAG];
  Int_t   Tagd4Lund[NMAXTAG];  
  Int_t   Tagd4Idx[NMAXTAG];
  Int_t   Tagd5Lund[NMAXTAG];  
  Int_t   Tagd5Idx[NMAXTAG];

  Int_t   nPi;
  Float_t PienergyCM[NMAXTAG]; 
  Float_t Pip3CM[NMAXTAG]; 
  Float_t PicosthCM[NMAXTAG]; 
  Float_t PiphiCM[NMAXTAG]; 
  Int_t   PiLund[NMAXTAG];   
  Int_t   PiMCIdx[NMAXTAG];
  Int_t   PiTrkIdx[NMAXTAG]; 

  Int_t   nK;
  Float_t Kp3CM[NMAXTAG]; 
  Float_t KcosthCM[NMAXTAG]; 
  Float_t KphiCM[NMAXTAG]; 
  Float_t KenergyCM[NMAXTAG]; 
  Int_t   KLund[NMAXTAG]; 
  Int_t   KMCIdx[NMAXTAG];
  Int_t   KTrkIdx[NMAXTAG];

  Int_t   nPi0;
  Float_t Pi0Mass[NMAXTAG];  
  Float_t Pi0energyCM[NMAXTAG]; 
  Float_t Pi0p3CM[NMAXTAG]; 
  Float_t Pi0costhCM[NMAXTAG]; 
  Float_t Pi0phiCM[NMAXTAG]; 
  Int_t   Pi0Lund[NMAXTAG];   
  Int_t   Pi0MCIdx[NMAXTAG];
  Int_t   Pi0d1Lund[NMAXTAG];  
  Int_t   Pi0d1Idx[NMAXTAG];  
  Int_t   Pi0d2Lund[NMAXTAG];  
  Int_t   Pi0d2Idx[NMAXTAG];  

  Int_t   nGam;
  Float_t GamenergyCM[NMAXTAG]; 
  Float_t Gamp3CM[NMAXTAG]; 
  Float_t GamcosthCM[NMAXTAG]; 
  Float_t GamphiCM[NMAXTAG]; 
  Int_t   GamLund[NMAXTAG];   
  Int_t   GamMCIdx[NMAXTAG];

  Int_t   piSelectorsMap[NMAXTAG];
  Int_t   KSelectorsMap[NMAXTAG];

  if(_fChain->GetBranch("eePx"))_fChain->SetBranchAddress("eePx",&eePx);
  if(_fChain->GetBranch("eePy"))_fChain->SetBranchAddress("eePy",&eePy);
  if(_fChain->GetBranch("eePz"))_fChain->SetBranchAddress("eePz",&eePz);
  if(_fChain->GetBranch("eeE"))_fChain->SetBranchAddress("eeE",&eeE);

  if(_fChain->GetBranch("nTag"))_fChain->SetBranchAddress("nTag",&nTag);
  if(_fChain->GetBranch("TagMass"))_fChain->SetBranchAddress("TagMass",TagMass);
  if(_fChain->GetBranch("Tagp3CM"))_fChain->SetBranchAddress("Tagp3CM",Tagp3CM); 
  if(_fChain->GetBranch("TagcosthCM"))_fChain->SetBranchAddress("TagcosthCM",TagcosthCM); 
  if(_fChain->GetBranch("TagphiCM"))_fChain->SetBranchAddress("TagphiCM",TagphiCM);  
  if(_fChain->GetBranch("TagenergyCM"))_fChain->SetBranchAddress("TagenergyCM",TagenergyCM);   
  if(_fChain->GetBranch("TagLund"))_fChain->SetBranchAddress("TagLund",TagLund);
  if(_fChain->GetBranch("TagnDaus"))_fChain->SetBranchAddress("TagnDaus",TagnDaus);
  if(_fChain->GetBranch("Tagd1Lund"))_fChain->SetBranchAddress("Tagd1Lund",Tagd1Lund);
  if(_fChain->GetBranch("Tagd1Idx"))_fChain->SetBranchAddress("Tagd1Idx",Tagd1Idx);
  if(_fChain->GetBranch("Tagd2Lund"))_fChain->SetBranchAddress("Tagd2Lund",Tagd2Lund);
  if(_fChain->GetBranch("Tagd2Idx"))_fChain->SetBranchAddress("Tagd2Idx",Tagd2Idx);
  if(_fChain->GetBranch("Tagd3Lund"))_fChain->SetBranchAddress("Tagd3Lund",Tagd3Lund);
  if(_fChain->GetBranch("Tagd3Idx"))_fChain->SetBranchAddress("Tagd3Idx",Tagd3Idx);
  if(_fChain->GetBranch("Tagd4Lund"))_fChain->SetBranchAddress("Tagd4Lund",Tagd4Lund);
  if(_fChain->GetBranch("Tagd4Idx"))_fChain->SetBranchAddress("Tagd4Idx",Tagd4Idx);
  if(_fChain->GetBranch("Tagd5Lund"))_fChain->SetBranchAddress("Tagd5Lund",Tagd5Lund);
  if(_fChain->GetBranch("Tagd5Idx"))_fChain->SetBranchAddress("Tagd5Idx",Tagd5Idx);
  if(_fChain->GetBranch("TagChi2"))_fChain->SetBranchAddress("TagChi2",TagChi2);
  if(_fChain->GetBranch("TagnDof"))_fChain->SetBranchAddress("TagnDof",TagnDof);
  if(_fChain->GetBranch("TagVtxStatus"))_fChain->SetBranchAddress("TagVtxStatus",TagVtxStatus);
  if(_fChain->GetBranch("TagVtxx"))_fChain->SetBranchAddress("TagVtxx",TagVtxx);
  if(_fChain->GetBranch("TagVtxy"))_fChain->SetBranchAddress("TagVtxy",TagVtxy);
  if(_fChain->GetBranch("TagVtxz"))_fChain->SetBranchAddress("TagVtxz",TagVtxz);

  if(_fChain->GetBranch("nPi"))_fChain->SetBranchAddress("nPi",&nPi);
  if(_fChain->GetBranch("PienergyCM"))_fChain->SetBranchAddress("PienergyCM",PienergyCM);
  if(_fChain->GetBranch("Pip3CM"))_fChain->SetBranchAddress("Pip3CM",Pip3CM);
  if(_fChain->GetBranch("PicosthCM"))_fChain->SetBranchAddress("PicosthCM",PicosthCM);
  if(_fChain->GetBranch("PiphiCM"))_fChain->SetBranchAddress("PiphiCM",PiphiCM);
  if(_fChain->GetBranch("PiLund"))_fChain->SetBranchAddress("PiLund",PiLund);
  if(_fChain->GetBranch("PiTrkIdx"))_fChain->SetBranchAddress("PiTrkIdx",PiTrkIdx);

  if(_fChain->GetBranch("nK"))_fChain->SetBranchAddress("nK",&nK);
  if(_fChain->GetBranch("Kp3CM"))_fChain->SetBranchAddress("Kp3CM",Kp3CM);
  if(_fChain->GetBranch("KenergyCM"))_fChain->SetBranchAddress("KenergyCM",KenergyCM);
  if(_fChain->GetBranch("KcosthCM"))_fChain->SetBranchAddress("KcosthCM",KcosthCM);
  if(_fChain->GetBranch("KphiCM"))_fChain->SetBranchAddress("KphiCM",KphiCM);
  if(_fChain->GetBranch("KLund"))_fChain->SetBranchAddress("KLund",KLund);
  if(_fChain->GetBranch("KTrkIdx"))_fChain->SetBranchAddress("KTrkIdx",KTrkIdx);
  if(_fChain->GetBranch("KMCIdx"))_fChain->SetBranchAddress("KMCIdx",KMCIdx);  

  if(_fChain->GetBranch("nPi0"))_fChain->SetBranchAddress("nPi0",&nPi0);
  if(_fChain->GetBranch("Pi0Mass"))_fChain->SetBranchAddress("Pi0Mass",Pi0Mass);
  if(_fChain->GetBranch("Pi0energyCM"))_fChain->SetBranchAddress("Pi0energyCM",Pi0energyCM);
  if(_fChain->GetBranch("Pi0p3CM"))_fChain->SetBranchAddress("Pi0p3CM",Pi0p3CM);
  if(_fChain->GetBranch("Pi0costhCM"))_fChain->SetBranchAddress("Pi0costhCM",Pi0costhCM);
  if(_fChain->GetBranch("Pi0phiCM"))_fChain->SetBranchAddress("Pi0phiCM",Pi0phiCM);
  if(_fChain->GetBranch("Pi0Lund"))_fChain->SetBranchAddress("Pi0Lund",Pi0Lund);
  if(_fChain->GetBranch("Pi0d1Lund"))_fChain->SetBranchAddress("Pi0d1Lund",Pi0d1Lund);
  if(_fChain->GetBranch("Pi0d1Idx"))_fChain->SetBranchAddress("Pi0d1Idx",Pi0d1Idx);
  if(_fChain->GetBranch("Pi0d2Lund"))_fChain->SetBranchAddress("Pi0d2Lund",Pi0d2Lund);
  if(_fChain->GetBranch("Pi0d2Idx"))_fChain->SetBranchAddress("Pi0d2Idx",Pi0d2Idx);

  if(_fChain->GetBranch("nGam"))_fChain->SetBranchAddress("nGam",&nGam);
  if(_fChain->GetBranch("GamenergyCM"))_fChain->SetBranchAddress("GamenergyCM",GamenergyCM);
  if(_fChain->GetBranch("Gamp3CM"))_fChain->SetBranchAddress("Gamp3CM",Gamp3CM);
  if(_fChain->GetBranch("GamcosthCM"))_fChain->SetBranchAddress("GamcosthCM",GamcosthCM);
  if(_fChain->GetBranch("GamphiCM"))_fChain->SetBranchAddress("GamphiCM",GamphiCM);
  if(_fChain->GetBranch("GamLund"))_fChain->SetBranchAddress("GamLund",GamLund);
  if(_fChain->GetBranch("GamMCIdx"))_fChain->SetBranchAddress("GamMCIdx",GamMCIdx);

  if(_fChain->GetBranch("KSelectorsMap"))_fChain->SetBranchAddress("KSelectorsMap",KSelectorsMap);
  if(_fChain->GetBranch("piSelectorsMap"))_fChain->SetBranchAddress("piSelectorsMap",piSelectorsMap);

  TFile File("DsTolnu/TagOptCheck/Mode12/Ntuple_New.root","recreate");

  //create new reduced ntuple
  TTree Ntuple("Ntuple","Ntuple",0);
  TTree*ReducedNtuple=&Ntuple;
  Long64_t maxsize=10000000000;//10Gb
  cout<<" file size "<<maxsize<<endl;
  ReducedNtuple->SetMaxTreeSize(maxsize);//split files
  Long64_t fautosave=11000000000;//11Gb !!//disable autosaving 
  cout<<" autosave "<<fautosave<<endl;
  ReducedNtuple->SetAutoSave(fautosave);

  //-------------------Derived quantities

  Int_t   TagIdx;//index of Tag in the Tag Array

  Float_t tagmass;//unconstrained tag mass
  Float_t tagpstar;//cm momentum of tag
  Float_t tagenergycm;//cm energy of tag
  Int_t   tagcharge;//+-1
  Float_t tagflights;//flight significance of tag from BeamSpot
  Int_t   tagmode;//tag decay mode
  Int_t   taglund;//D0,D+,...=+-1,+-2,..
  Int_t   tagdstarlund;//0=no D*, 1=D0pi+, 2=D0pi0, 4=D+pi0, 3=D0gamma
  Float_t tagdstarmass;//unconstrained D*  mass
  Int_t   tagndaus;//number tag daughters
  Float_t taglogvtxprob;//log10(tag fit prob)  from first tag fit
  Int_t   tagmctrue;//true tag mc index
  Int_t   tagmctruebad;//default truth-matching
  Int_t   tagmctrueparent;//default truth-matching
  Int_t   tagpid;//if tag has K+, pid selector (1->5)
  Int_t   tagpipid;//if tag has K+, pid selector (1->5)
  Float_t tagpi0gamenergy;//if tag has pi0, energy of lower energy gamma OR 



  ReducedNtuple->Branch("tagenergycm",&tagenergycm,"tagenergycm/F"); 
  ReducedNtuple->Branch("tagmass",&tagmass,"tagmass/F"); 
  ReducedNtuple->Branch("tagpstar",&tagpstar,"tagpstar/F");
  ReducedNtuple->Branch("tagcharge",&tagcharge,"tagcharge/I");
  ReducedNtuple->Branch("taglogvtxprob",&taglogvtxprob,"taglogvtxprob/F");
  ReducedNtuple->Branch("tagflights",&tagflights,"tagflights/F");
  ReducedNtuple->Branch("tagmode",&tagmode,"tagmode/I");
  ReducedNtuple->Branch("tagndaus",&tagndaus,"tagndaus/I");
  ReducedNtuple->Branch("tagmctrue",&tagmctrue,"tagmctrue/I"); 
  ReducedNtuple->Branch("tagmctruebad",&tagmctruebad,"tagmctruebad/I");
  ReducedNtuple->Branch("tagmctrueparent",&tagmctrueparent,"tagmctrueparent/I");
  ReducedNtuple->Branch("taglund",&taglund,"taglund/I");
  ReducedNtuple->Branch("tagdstarlund",&tagdstarlund,"tagdstarlund/I");
  ReducedNtuple->Branch("tagdstarmass",&tagdstarmass,"tagdstarmass/F");
  ReducedNtuple->Branch("tagpid",&tagpid,"tagpid/I");
  ReducedNtuple->Branch("tagpipid",&tagpipid,"tagpipid/I");
  ReducedNtuple->Branch("tagpi0gamenergy",&tagpi0gamenergy,"tagpi0gamenergy/F");


  //begin loop
  cout<<"Going to start the event loop"<<endl;
  cout<<"Entries="<<_fChain->GetEntries()<<endl;
  Int_t eventid=0;
  Int_t ngoodthisevent=0;
  Int_t goodeventid=0;
  while(_fChain->GetEntry(eventid,0)>0){   
    eventid++;
     
    if(eventid%5000==0)cout<<eventid<<" Events done."<<endl;

    eeVelVec=TVector3(eePx/eeE,eePy/eeE,eePz/eeE);
    
    //    FillTagEventVars();    
    //     //if(!TagEvtPassesCuts())continue;   
    
    if(nTag>NMAXTAG)continue;//cannot be fixed ROOT fills arrays out of bounds
    
    ngoodthisevent=0;
    for(TagIdx=0;TagIdx<nTag;TagIdx++){

      //should not fill any vars outside this method otherwise not saved
      // FillTagVars();


      //-------TAG quantities
      tagmass=TagMass[TagIdx]; 
      if(abs(TagLund[TagIdx])==myD0Lund)tagcharge=0;
      else tagcharge=TagLund[TagIdx]/abs(TagLund[TagIdx]);  
      if(TMath::Prob(TagChi2[TagIdx],TagnDof[TagIdx])>1e-300)
	taglogvtxprob=log10(TMath::Prob(TagChi2[TagIdx],TagnDof[TagIdx]));
      else taglogvtxprob=-100;
      tagenergycm=TagenergyCM[TagIdx];
      tagpstar=Tagp3CM[TagIdx];  
      tagndaus=TagnDaus[TagIdx];
      
      //count daughter types
      Int_t tagnK=0;Int_t tagnPi=0;Int_t tagnPi0=0;Int_t tagnKs=0;
      if(abs(Tagd1Lund[TagIdx])==myKLund) tagnK++;
      if(abs(Tagd2Lund[TagIdx])==myKLund) tagnK++;
      if(abs(Tagd3Lund[TagIdx])==myKLund) tagnK++;
      if(abs(Tagd4Lund[TagIdx])==myKLund) tagnK++;
      if(abs(Tagd5Lund[TagIdx])==myKLund) tagnK++;
  
      if(abs(Tagd1Lund[TagIdx])==myPiLund) tagnPi++;
      if(abs(Tagd2Lund[TagIdx])==myPiLund) tagnPi++;
      if(abs(Tagd3Lund[TagIdx])==myPiLund) tagnPi++;
      if(abs(Tagd4Lund[TagIdx])==myPiLund) tagnPi++;
      if(abs(Tagd5Lund[TagIdx])==myPiLund) tagnPi++;
  
      if(abs(Tagd1Lund[TagIdx])==myPi0Lund) tagnPi0++;
      if(abs(Tagd2Lund[TagIdx])==myPi0Lund) tagnPi0++;
      if(abs(Tagd3Lund[TagIdx])==myPi0Lund) tagnPi0++;
      if(abs(Tagd4Lund[TagIdx])==myPi0Lund) tagnPi0++;
      if(abs(Tagd5Lund[TagIdx])==myPi0Lund) tagnPi0++;


      taglund=0;
      tagmode=0;
      if(abs(TagLund[TagIdx])==myDPlusLund){
	if( tagnKs==0 && tagnK==1 && tagnPi==2 && tagnPi0==0 ) tagmode=1 ; //Kpipi      9.5
	if( tagnKs==0 && tagnK==1 && tagnPi==2 && tagnPi0==1 ) tagmode=2 ; //Kpipipi0   5.5
	if( tagnKs==1 && tagnK==0 && tagnPi==1 && tagnPi0==0 ) tagmode=3 ; //Kspi       1.47
	if( tagnKs==1 && tagnK==0 && tagnPi==1 && tagnPi0==1 ) tagmode=4 ; //Kspipi0    6.8
	if( tagnKs==1 && tagnK==0 && tagnPi==3 && tagnPi0==0 ) tagmode=5 ; //Ks3pi      3.1
	taglund=2*TagLund[TagIdx]/abs(TagLund[TagIdx]);
      }



      tagpid=5;
      if(tagnK>0){  //this tag has a K track    
	Int_t ktrkidx=KTrkIdx[Tagd1Idx[TagIdx]];
	if(0<=ktrkidx&&ktrkidx<NMAXTAG){
	  tagpid=0;
	  for(Int_t i=19;i<=23;i++){//NEW PID; 19=notpion,20=veryloose,21=loose,22=tight,23=verytight
	    if((KSelectorsMap[ktrkidx] & (1<<i)) == (1<<i))
	      tagpid++;//if passes selector increase 
	  }

	}
      }
      tagpipid=5;
      if(tagnPi>0){  //this tag has a K track    
	Int_t pitrkidx1=PiTrkIdx[Tagd2Idx[TagIdx]];
	Int_t pitrkidx2=PiTrkIdx[Tagd3Idx[TagIdx]];
	if(0<=pitrkidx1&&pitrkidx1<NMAXTAG&&0<=pitrkidx2&&pitrkidx2<NMAXTAG){
	  Int_t tagpipid1=0;
	  for(Int_t i=10;i<=15;i++){
	    if((piSelectorsMap[pitrkidx1] & (1<<i)) == (1<<i))
	      tagpipid1++;//if passes selector increase 
	  }
	  Int_t tagpipid2=0;
	  for(Int_t i=10;i<=15;i++){
	    if((piSelectorsMap[pitrkidx2] & (1<<i)) == (1<<i))
	      tagpipid2++;//if passes selector increase 
	  }	  
	  tagpipid=(tagpipid1<tagpipid2) ? tagpipid1:tagpipid2;
	}
      }

      //for tags with pi0's keep energy of lower gamma
      tagpi0gamenergy=100.;
      if(tagnPi0>0){
	Int_t tagpi0idx=Tagd4Idx[TagIdx];   
	if(0<=tagpi0idx&&tagpi0idx<NMAXTAG){
	  Int_t tagpi0gam1idx=Pi0d1Idx[tagpi0idx];
	  Int_t tagpi0gam2idx=Pi0d2Idx[tagpi0idx];
	  if(tagpi0gam1idx<NMAXTAG&&tagpi0gam2idx<NMAXTAG){
	    TVector3 Gam1p3VCM(Gamp3CM[tagpi0gam1idx]*sin(acos(GamcosthCM[tagpi0gam1idx]))*cos(GamphiCM[tagpi0gam1idx]),
			       Gamp3CM[tagpi0gam1idx]*sin(acos(GamcosthCM[tagpi0gam1idx]))*sin(GamphiCM[tagpi0gam1idx]),
			       Gamp3CM[tagpi0gam1idx]*GamcosthCM[tagpi0gam1idx]);
	    TLorentzVector Gam1p4(Gam1p3VCM,GamenergyCM[tagpi0gam1idx]);
	    Gam1p4.Boost(eeVelVec);
	    
	    TVector3 Gam2p3VCM(Gamp3CM[tagpi0gam2idx]*sin(acos(GamcosthCM[tagpi0gam2idx]))*cos(GamphiCM[tagpi0gam2idx]),
			       Gamp3CM[tagpi0gam2idx]*sin(acos(GamcosthCM[tagpi0gam2idx]))*sin(GamphiCM[tagpi0gam2idx]),
			       Gamp3CM[tagpi0gam2idx]*GamcosthCM[tagpi0gam2idx]);
	    TLorentzVector Gam2p4(Gam2p3VCM,GamenergyCM[tagpi0gam2idx]);
	    Gam2p4.Boost(eeVelVec);
	    tagpi0gamenergy=Gam1p4.T();
	    if(Gam2p4.T()<tagpi0gamenergy)tagpi0gamenergy=Gam2p4.T();//pick the smallest one
	  }
	}
    
      }



      //      //some cuts go here
      //       if(!TagPassesCuts())continue;   
  
      if(abs(taglund)!=2||tagmode!=2) continue;
  
      

      //save
      ngoodthisevent++;//increase each fill
      ReducedNtuple->Fill();	              
   
    }//Tag loop    
    if(ngoodthisevent>0)goodeventid++;
    
    
  }
  
  //print summary
  cout<<"--------Summary-------"<<endl;
  cout<<"Total events = "<<eventid<<endl;


  ////Save the Ntuple and histograms
  cout<<"Going to save the ntuple and histograms"<<endl; 


  ReducedNtuple->AutoSave("Overwrite");


  File.ls();
  File.Close();
}
