#include "DsRecoAnalysis2.h"

#include "DsRecoAnalysis2PlotGenTagSide.C"


ClassImp(DsRecoAnalysis2);
DsRecoAnalysis2::DsRecoAnalysis2(Bool_t MCorDATA,Bool_t TruthMatch,Int_t MatterOrAntiMatter,TString OutputDir):
  TagAnalysis(MCorDATA,TruthMatch,MatterOrAntiMatter,OutputDir),
  cuttruth(0),
  cutcharge(0),
  cutgamangle(0),
  cutKangle(0)
{
  cout<<"Done Initializing the analysis"<<endl;
}

DsRecoAnalysis2::~DsRecoAnalysis2(){ 
}

Int_t DsRecoAnalysis2::ReduceNtuple(Bool_t RunInTestMode){
  cout<<"Going to reduce the ntuple"<<endl;

  if(_fChain==NULL){
    cout<<"No chain."<<endl;
    return 0;
  } 

  ///link the branches in the input chain
  SetDsRecoBranches();



  //the file must be created here so the 'new' histos and Tree is created in hard drive //OutFile=new 
  TFile OutFile(_OutputDir+"/Ntuple_New.root","recreate",_OutputDir+"/Ntuple_New.root",1);//1=minimal compression
  if(OutFile.IsZombie()){cout<<"Failed to create file"<<endl;return 0;}


  //create new reduced ntuple
  TTree Ntuple("Ntuple","Ntuple",0);
  Long64_t maxsize=10000000000;//10Gb
  cout<<" file size "<<maxsize<<endl;
  Ntuple.SetMaxTreeSize(maxsize);//split files

  //disable autosaving 
  Long64_t fautosave=20000000000;//50Gb !!
  cout<<" autosave "<<fautosave<<endl;
  Ntuple.SetAutoSave(fautosave);

  //create the branches in the Ntuple
  MakeDsRecoReducedNtupleBranches(&Ntuple);


  //Start the event loop;
  Long_t MaxNumberEvents=1000000000;
  if(RunInTestMode)MaxNumberEvents=10000;


  //Counters
  Int_t MCBCounterTotal=0;
  Int_t RecoBCounterTotal=0;
  


  cout<<"cuts:"
      <<"  cuttruth="<<cuttruth
      <<"  cutcharge="<<cutcharge
      <<"  cuttagmass="<<cuttagmass
      <<"  cutgamangle="<<cutgamangle
      <<"  cutKangle="<<cutKangle
      <<endl;


  cout<<"Total Entries in Chain="<<_fChain->GetEntries()<<endl;
  cout<<"Going to start the event loop"<<endl;
  eventid=0;
  while(_fChain->GetEntry(eventid,0)>0&&eventid<MaxNumberEvents){   
    eventid++;
    
    if(eventid%100000==0){
      cout<<eventid<<" Events done.   With "<<RecoBCounterTotal<<" Candidates saved."<<endl;    
    }


    DsGenMCIdx=-1;
    //find the MC gen. Nu Idx
    if(_TruthMatch){
      for(Int_t mcc=0;mcc<mcLen;mcc++){
	if(abs(mcLund[mcc])==myDsLund){
	  DsGenMCIdx=mcc;
	}
      }
    }

    ///Event vars
    FillEventVars();
   

    //determine best candidate
    //DetermineBestCand();
    
    
    //now do real loop
    for(EvtIdx=0;EvtIdx<nEvt;EvtIdx++){
     
      //
      FillDsRecoVars();          
      
      //Selection cuts:
      if(cuttruth==1)if(evtmctrue!=1)continue;
      if(cutgamangle==1)if(gamangle>-.1)continue;
      if(cutKangle==1)if(fabs(Kangle)<.1)continue;
      if(cuttagmass==1)if(fabs(tagmassdiff)>TagMassCut)continue;
      if(tagpstar<2.5)continue;

      //save
      RecoBCounterTotal++;
      Ntuple.Fill();
   
    }// loop    

    
    
      
 

  }

  //print summary
  cout<<"--------Summary-------"<<endl;
  cout<<"Total events = "<<eventid<<endl;
  cout<<"Total reco ="<<RecoBCounterTotal<<endl;
  cout<<"Total mc ="<<MCBCounterTotal<<endl;
  cout<<"--------End Summary---"<<endl;

  ////Save the Ntuple and histograms
  cout<<"Going to save the ntuple and histograms"<<endl; 

  //---
  if(Ntuple.AutoSave("Overwrite")){ cout<<"Ntuple written."<<endl;}
  else{ cout<<"Failed to write ntuple"<<endl;return 0;}


  cout<<"Objects in the current file"<<endl;
  Ntuple.GetCurrentFile()->ls();

  OutFile.Close(); //this will delete all objects created inside the file if they were not written  
  cout<<OutFile.GetName()<<" has been closed."<<endl;


  return 1;
}

void DsRecoAnalysis2::FillDsRecoVars(){

  if(EvtIdx>=MAXNCANDS)return;
  
  ///determine the indexes for each particle before used by other methods
  BIdx   =Evtd1Idx[EvtIdx];
  TagIdx =Bd1Idx[BIdx];
  FragIdx=Bd2Idx[BIdx];
  TagKIdx=Bd3Idx[BIdx];
  GamIdx =Evtd2Idx[EvtIdx];  
  if(TagKIdx<0)TagKIdx=Evtd3Idx[EvtIdx];

  //////fill reduced ntuple variables    
  evtmass=EvtMass[EvtIdx];
  evtpstar=Evtp3CM[EvtIdx];
  evtcosstar=EvtcosthCM[EvtIdx];
  evtphistar=EvtphiCM[EvtIdx];
  evtenergy=Evtenergy[EvtIdx];
  evtcmenergy=EvtenergyCM[EvtIdx];
  if(abs(EvtLund[EvtIdx])==513)evtcharge=0;
  else evtcharge=EvtLund[EvtIdx]/abs(EvtLund[EvtIdx]);
  if(_MCorDATA==1) evtmctrue=1; else evtmctrue=(EvtMyDsMCIdx[EvtIdx]>=0);  
  if(_MCorDATA==1) tagmctrue=1; else tagmctrue=(EvtMyTagMCIdx[EvtIdx]>=0);  
  if(_MCorDATA==1) Kmctrue=1; else Kmctrue=(EvtMyKMCIdx[EvtIdx]>=0);  
  if(_MCorDATA==1) fragmctrue=1; else fragmctrue=(EvtMyFragMCIdx>=0);
  
  Kangle=ComputeKAngle(); 
  gamangle=ComputeGamAngle();
  gamenergy=Gamp3CM[GamIdx];

  bmass=BMass[BIdx];
  benergy=BenergyCM[BIdx];
  bpstar=Bp3CM[BIdx];
  bdeltae=benergy-eeE/2.;

  //compute missing four-vector
  TLorentzVector eep4(TVector3(eePx,eePy,eePz),eeE);
  TVector3 Evtp3V(Evtp3[EvtIdx]*sin(acos(Evtcosth[EvtIdx]))*cos(Evtphi[EvtIdx]),
		  Evtp3[EvtIdx]*sin(acos(Evtcosth[EvtIdx]))*sin(Evtphi[EvtIdx]),
		  Evtp3[EvtIdx]*Evtcosth[EvtIdx]);
  TLorentzVector Evtp4(Evtp3V,Evtenergy[EvtIdx]);
  TVector3 Bp3V(Bp3[BIdx]*sin(acos(Bcosth[BIdx]))*cos(Bphi[BIdx]),
		Bp3[BIdx]*sin(acos(Bcosth[BIdx]))*sin(Bphi[BIdx]),
		Bp3[BIdx]*Bcosth[BIdx]);
  TLorentzVector Bp4(Bp3V,Benergy[BIdx]);
  TVector3 Fragp3V(Fragp3[FragIdx]*sin(acos(Fragcosth[FragIdx]))*cos(Fragphi[FragIdx]),
		   Fragp3[FragIdx]*sin(acos(Fragcosth[FragIdx]))*sin(Fragphi[FragIdx]),
		   Fragp3[FragIdx]*Fragcosth[FragIdx]);
  TLorentzVector Fragp4(Fragp3V,Fragenergy[FragIdx]);
  TVector3 Gamp3V(Gamp3[GamIdx]*sin(acos(Gamcosth[GamIdx]))*cos(Gamphi[GamIdx]),
		  Gamp3[GamIdx]*sin(acos(Gamcosth[GamIdx]))*sin(Gamphi[GamIdx]),
		  Gamp3[GamIdx]*Gamcosth[GamIdx]);
  TLorentzVector Gamp4(Gamp3V,Gamenergy[GamIdx]);
//   TVector3 Kp3V(Kp3[TagKIdx]*sin(acos(Kcosth[TagKIdx]))*cos(Kphi[TagKIdx]),
// 		Kp3[TagKIdx]*sin(acos(Kcosth[TagKIdx]))*sin(Kphi[TagKIdx]),
// 		Kp3[TagKIdx]*Kcosth[TagKIdx]);
//   TLorentzVector Kp4(Kp3V,Kenergy[TagKIdx]);
  TLorentzVector Misp4(eep4-Evtp4);  
  mismass=Misp4.Mag();
  misp=Misp4.Vect().Mag();

  //the "Ds"
  TLorentzVector Frag2p4(-Fragp4.Vect(),Fragp4.T());
  TLorentzVector Dsp4(Misp4-Frag2p4);
  TLorentzVector DsStp4(Dsp4+Gamp4);
  
  dsmass=Dsp4.Mag();
  dsp=Dsp4.Vect().Mag();
  dsstmass=DsStp4.Mag();
  dsstdeltam=dsstmass-dsmass;
  dsstp=DsStp4.Vect().Mag();


  fragenergy=FragenergyCM[FragIdx];
  if(abs(FragLund[EvtIdx])==511)fragcharge=0;
  else fragcharge=FragLund[FragIdx]/abs(FragLund[FragIdx]);
  fragangle=ComputeFragAngle();
  fragndaus=FragnDaus[FragIdx];
  fragnpis=0;
  if(abs(Fragd1Lund[FragIdx])==myPiLund)fragnpis++;
  if(abs(Fragd2Lund[FragIdx])==myPiLund)fragnpis++;
  if(abs(Fragd3Lund[FragIdx])==myPiLund)fragnpis++;
  if(abs(Fragd4Lund[FragIdx])==myPiLund)fragnpis++;
  if(abs(Fragd5Lund[FragIdx])==myPiLund)fragnpis++;
  if(abs(Fragd6Lund[FragIdx])==myPiLund)fragnpis++;
  if(abs(Fragd7Lund[FragIdx])==myPiLund)fragnpis++;
  if(abs(Fragd8Lund[FragIdx])==myPiLund)fragnpis++;
  if(abs(Fragd9Lund[FragIdx])==myPiLund)fragnpis++;

  fragnpi0s=0;
  if(abs(Fragd1Lund[FragIdx])==myPi0Lund)fragnpi0s++;
  if(abs(Fragd2Lund[FragIdx])==myPi0Lund)fragnpi0s++;
  if(abs(Fragd3Lund[FragIdx])==myPi0Lund)fragnpi0s++;
  if(abs(Fragd4Lund[FragIdx])==myPi0Lund)fragnpi0s++;
  if(abs(Fragd5Lund[FragIdx])==myPi0Lund)fragnpi0s++;
  if(abs(Fragd6Lund[FragIdx])==myPi0Lund)fragnpi0s++;
  if(abs(Fragd7Lund[FragIdx])==myPi0Lund)fragnpi0s++;
  if(abs(Fragd8Lund[FragIdx])==myPi0Lund)fragnpi0s++;
  if(abs(Fragd9Lund[FragIdx])==myPi0Lund)fragnpi0s++;

  fragngams=0;
  if(abs(Fragd1Lund[FragIdx])==myGammaLund)fragngams++;
  if(abs(Fragd2Lund[FragIdx])==myGammaLund)fragngams++;
  if(abs(Fragd3Lund[FragIdx])==myGammaLund)fragngams++;
  if(abs(Fragd4Lund[FragIdx])==myGammaLund)fragngams++;
  if(abs(Fragd5Lund[FragIdx])==myGammaLund)fragngams++;
  if(abs(Fragd6Lund[FragIdx])==myGammaLund)fragngams++;
  if(abs(Fragd7Lund[FragIdx])==myGammaLund)fragngams++;
  if(abs(Fragd8Lund[FragIdx])==myGammaLund)fragngams++;
  if(abs(Fragd9Lund[FragIdx])==myGammaLund)fragngams++;

  
  //
  FillTagVars();
 
}


Float_t DsRecoAnalysis2::ComputeGamAngle(){
  //must be in CM frame
  TVector3 Tagp3vec(Tagp3CM[TagIdx]*sin(acos(TagcosthCM[TagIdx]))*cos(TagphiCM[TagIdx]),
		    Tagp3CM[TagIdx]*sin(acos(TagcosthCM[TagIdx]))*sin(TagphiCM[TagIdx]),
		    Tagp3CM[TagIdx]*TagcosthCM[TagIdx]);

  TVector3 Gamp3vec(Gamp3CM[GamIdx]*sin(acos(GamcosthCM[GamIdx]))*cos(GamphiCM[GamIdx]),
		    Gamp3CM[GamIdx]*sin(acos(GamcosthCM[GamIdx]))*sin(GamphiCM[GamIdx]),
		    Gamp3CM[GamIdx]*GamcosthCM[GamIdx]);


  return (Tagp3vec.Unit())*(Gamp3vec.Unit());
}


Float_t DsRecoAnalysis2::ComputeKAngle(){
  //must be in CM frame
  TVector3 Tagp3vec(Tagp3CM[TagIdx]*sin(acos(TagcosthCM[TagIdx]))*cos(TagphiCM[TagIdx]),
		    Tagp3CM[TagIdx]*sin(acos(TagcosthCM[TagIdx]))*sin(TagphiCM[TagIdx]),
		    Tagp3CM[TagIdx]*TagcosthCM[TagIdx]);

  TVector3 Kp3vec(Kp3CM[TagKIdx]*sin(acos(KcosthCM[TagKIdx]))*cos(KphiCM[TagKIdx]),
		  Kp3CM[TagKIdx]*sin(acos(KcosthCM[TagKIdx]))*sin(KphiCM[TagKIdx]),
		  Kp3CM[TagKIdx]*KcosthCM[TagKIdx]);
  

  return (Tagp3vec.Unit())*(Kp3vec.Unit());
}

Float_t DsRecoAnalysis2::ComputeFragAngle(){
  //must be in CM frame
  TVector3 Tagp3vec(Tagp3CM[TagIdx]*sin(acos(TagcosthCM[TagIdx]))*cos(TagphiCM[TagIdx]),
		    Tagp3CM[TagIdx]*sin(acos(TagcosthCM[TagIdx]))*sin(TagphiCM[TagIdx]),
		    Tagp3CM[TagIdx]*TagcosthCM[TagIdx]);

  TVector3 Fragp3vec(Fragp3CM[FragIdx]*sin(acos(FragcosthCM[FragIdx]))*cos(FragphiCM[FragIdx]),
		  Fragp3CM[FragIdx]*sin(acos(FragcosthCM[FragIdx]))*sin(FragphiCM[FragIdx]),
		  Fragp3CM[FragIdx]*FragcosthCM[FragIdx]);
  

  return (Tagp3vec.Unit())*(Fragp3vec.Unit());
}


Int_t DsRecoAnalysis2::ReduceNtupleGen(Bool_t RunInTestMode){
  cout<<"Going to reduce the ntuple"<<endl;

  if(_fChain==NULL){
    cout<<"No chain."<<endl;
    return 0;
  } 

  ///link the branches in the input chain
  SetDsRecoBranches();



  //the file must be created here so the 'new' histos and Tree is created in hard drive //OutFile=new 
  TFile OutFile(_OutputDir+"/NtupleGen_New.root","recreate",_OutputDir+"/NtupleGen_New.root",1);//1=minimal compression
  if(OutFile.IsZombie()){cout<<"Failed to create file"<<endl;return 0;}


  //create new reduced ntuple
  TTree Ntuple("Ntuple","Ntuple",0);
  Long64_t maxsize=10000000000;//10Gb
  cout<<" file size "<<maxsize<<endl;
  Ntuple.SetMaxTreeSize(maxsize);//split files

  //disable autosaving 
  Long64_t fautosave=20000000000;//50Gb !!
  cout<<" autosave "<<fautosave<<endl;
  Ntuple.SetAutoSave(fautosave);

  //create the branches in the Ntuple
  MakeDsRecoReducedNtupleBranches(&Ntuple);
  //Ntuple->Branch("tagmass",&tagmass,"tagmass/F");

  //Start the event loop;
  Long_t MaxNumberEvents=1000000000;
  if(RunInTestMode)MaxNumberEvents=10000;


  //Counters
  Int_t MCBCounterTotal=0;


  cout<<"Total Entries in Chain="<<_fChain->GetEntries()<<endl;
  cout<<"Going to start the event loop"<<endl;
  eventid=0;
  while(_fChain->GetEntry(eventid,0)>0&&eventid<MaxNumberEvents){   
    eventid++;
    
    if(eventid%10000==0){
      cout<<eventid<<" Events done.   With "<<MCBCounterTotal<<" Candidates saved."<<endl;    
    }

    if(mcLen>=MAXMCLEN)continue;

    ///Event vars
    FillEventVars();

    //find the Ds Idx    
    DsStGenMCIdx=-1;
    for(Int_t mcc=0;mcc<mcLen;mcc++){
      if(abs(mcLund[mcc])==myDsStLund){
	  DsStGenMCIdx=mcc;break;
	}      
    }
    if(DsStGenMCIdx==-1)continue;
    dsstmass=mcmass[DsStGenMCIdx];
    DsGenMCIdx=dauIdx[DsStGenMCIdx];
    dsmass=mcmass[DsGenMCIdx];
    GamIdx=dauIdx[DsStGenMCIdx]+1;
       
    
    //find the tag-D Idx
    TagIdx=-1;
    for(Int_t mcc=0;mcc<mcLen;mcc++){
      if(abs(mcLund[mcc])==myDstarLund
	 ||abs(mcLund[mcc])==myD0Lund
	 ||abs(mcLund[mcc])==myDPlusLund){
	TagIdx=mcc;break;
      }
    }
    if(TagIdx==-1)continue;
    tagmass=mcmass[TagIdx];
    tagenergy=mcenergyCM[TagIdx];
    TVector3 Tagp3Vec(mcp3CM[TagIdx]*sin(acos(mccosthCM[TagIdx]))*cos(mcphiCM[TagIdx]),
		      mcp3CM[TagIdx]*sin(acos(mccosthCM[TagIdx]))*sin(mcphiCM[TagIdx]),
		      mcp3CM[TagIdx]*mccosthCM[TagIdx]);
    


    //analyze fragmentation in tag side
    TVector3 TagFragp3Vec(0.,0.,0.);
    Float_t TagFragE=0.;
    fragnpis=0;
    fragnpi0s=0;
    fragngams=0;
    fragnKs=0;
    fragnKss=0;
    fragnEs=0;
    fragnMus=0;
    fragnENus=0; 
    fragnMuNus=0; 
    fragndaus=0;
    fragenergypi=0;
    fragenergypi0=0;
    fragenergyK=0;
    fragenergyKs=0;
    fragenergygam=0;
    fragenergyE=0;
    fragenergyMu=0;
    fragenergyENu=0;
    fragenergyMuNu=0;
    for(Int_t mcc=3;mcc<mcLen;mcc++){   

      //select only final state particles
      if(abs(mcLund[mcc])!=myPiLund
	 &&abs(mcLund[mcc])!=myPi0Lund
	 &&abs(mcLund[mcc])!=myKLund
	 &&abs(mcLund[mcc])!=myKS0Lund
	 &&abs(mcLund[mcc])!=myGammaLund
	 &&abs(mcLund[mcc])!=myMuLund
	 &&(abs(mcLund[mcc])!=myELund||mcc<2)
	 &&abs(mcLund[mcc])!=myNuMuLund
	 &&abs(mcLund[mcc])!=myNuELund
	 )continue;
	
  
      //check that this particle is not part of the Tag-D
      Bool_t overlaps=0;
      Int_t dauidx=dauIdx[TagIdx]-1;
      while(dauidx<dauIdx[TagIdx]+dauLen[TagIdx]&&dauidx<MAXMCLEN){
	dauidx++;
	if(dauidx==mcc){
	  overlaps=1;break;
	}

	//check 2 layers
	Int_t daudauidx=dauIdx[dauidx]-1;
	while(daudauidx<dauIdx[dauidx]+dauLen[dauidx]&&daudauidx<MAXMCLEN){
	  daudauidx++;
	  if(daudauidx==mcc){
	    overlaps=1;break;
	  }
	  
	  //check 3 layers
	  Int_t daudaudauidx=dauIdx[daudauidx]-1;
	  while(daudaudauidx<dauIdx[daudauidx]+dauLen[daudauidx]&&daudaudauidx<MAXMCLEN){
	    //cout<<daudaudauidx<<"  "<<dauIdx[daudauidx]<<" "<<dauLen[daudauidx]<<endl;
	    daudaudauidx++;
	    if(daudaudauidx==mcc){
	      overlaps=1;break;
	    }	    

	    //check 4 layers
	    Int_t daudaudaudauidx=dauIdx[daudaudauidx]-1;
	    while(daudaudaudauidx<dauIdx[daudaudauidx]+dauLen[daudaudauidx]&&daudaudaudauidx<MAXMCLEN){
	      //cout<<daudaudauidx<<"  "<<dauIdx[daudauidx]<<" "<<dauLen[daudauidx]<<endl;
	      daudaudaudauidx++;
	      if(daudaudaudauidx==mcc){
		overlaps=1;break;
	      }	    
	    }
	    if(overlaps)break;	    	    
	  }
	  if(overlaps)break;	    	 
	}
	if(overlaps)break;
      }
      if(overlaps)continue;
      
     

      //remove double counting
      if(mothIdx[mcc]>=0){
	if(abs(mcLund[mcc])==myGammaLund&&abs(mcLund[mothIdx[mcc]])==myPi0Lund)continue;  //for gammas check parent is not a pi0;
	if(abs(mcLund[mcc])==myPiLund&&abs(mcLund[mothIdx[mcc]])==myKS0Lund)continue;     //for pi check parent is not a Ks;Ks->pi pi
	if(abs(mcLund[mcc])==myPi0Lund&&abs(mcLund[mothIdx[mcc]])==myKS0Lund)continue;    //for pi0 check parent is not a Ks;Ks->pi pi
	if(abs(mcLund[mcc])==myMuLund&&abs(mcLund[mothIdx[mcc]])==myPiLund)continue;      //for mu check parent is not pion;pi->mu nu
	if(abs(mcLund[mcc])==myNuMuLund&&abs(mcLund[mothIdx[mcc]])==myPiLund)continue;    //for nu check parent is not pion;pi->mu nu
	if(abs(mcLund[mcc])==myMuLund&&abs(mcLund[mothIdx[mcc]])==myKLund)continue;       //for mu check parent is not pion;K->mu nu
	if(abs(mcLund[mcc])==myNuMuLund&&abs(mcLund[mothIdx[mcc]])==myKLund)continue;     //for mu check parent is not pion;K->mu nu
	if(abs(mcLund[mcc])==myELund&&abs(mcLund[mothIdx[mcc]])==myMuLund)continue;       //for e check parent is not muon;mu->e nu nu
	if(abs(mcLund[mcc])==myNuMuLund&&abs(mcLund[mothIdx[mcc]])==myMuLund)continue;    //for nu check parent is not muon;mu->e nu nu
	if(abs(mcLund[mcc])==myNuELund&&abs(mcLund[mothIdx[mcc]])==myMuLund)continue;     //for nu check parent is not muon;my->e nu nu
      }
      
      //check that is on the tag side
      TVector3 Fragp3Vec(mcp3CM[mcc]*sin(acos(mccosthCM[mcc]))*cos(mcphiCM[mcc]),
			 mcp3CM[mcc]*sin(acos(mccosthCM[mcc]))*sin(mcphiCM[mcc]),	
			 mcp3CM[mcc]*mccosthCM[mcc]);

      if(Fragp3Vec.Unit()*Tagp3Vec.Unit()<0.)continue;	


      //Sum up all the frag. particles;	
      TagFragp3Vec+=Fragp3Vec;
      TagFragE+=mcenergyCM[mcc];             
      
      if(abs(mcLund[mcc])==myPiLund){   fragnpis++;  fragenergypi+=  mcenergyCM[mcc];}
      if(abs(mcLund[mcc])==myPi0Lund){  fragnpi0s++; fragenergypi0+= mcenergyCM[mcc];}
      if(abs(mcLund[mcc])==myKLund){    fragnKs++;   fragenergyK+=   mcenergyCM[mcc];}
      if(abs(mcLund[mcc])==myKS0Lund){  fragnKss++;  fragenergyKs=   mcenergyCM[mcc];}
      if(abs(mcLund[mcc])==myGammaLund){fragngams++; fragenergygam+= mcenergyCM[mcc];}
      if(abs(mcLund[mcc])==myELund){    fragnEs++;   fragenergyE+=   mcenergyCM[mcc];}
      if(abs(mcLund[mcc])==myMuLund){   fragnMus++;  fragenergyMu+=  mcenergyCM[mcc];}
      if(abs(mcLund[mcc])==myNuELund){  fragnENus++; fragenergyENu+= mcenergyCM[mcc];}
      if(abs(mcLund[mcc])==myNuMuLund){ fragnMuNus++;fragenergyMuNu+=mcenergyCM[mcc];}
      
      fragndaus++;
      
    }// loop               
 
    //compute the angle from fragm. to tag-D
    fragangle=TagFragp3Vec.Unit()*Tagp3Vec.Unit();
    fragenergy=TagFragE;
       

    //Tag side energy
    TVector3 bp3Vec(TagFragp3Vec+Tagp3Vec);
    benergycm=tagenergy+fragenergy;
    bdeltae=benergycm-beamcmenergy/2;
    bpstar=bp3Vec.Mag();
    bmass=(benergycm*benergycm-bpstar*bpstar>0 ? sqrt((double)(benergycm*benergycm-bpstar*bpstar)) : 0.);
    bcosthcm=bp3Vec.Pz()/bp3Vec.Mag();

  

    //save
    MCBCounterTotal++;
    Ntuple.Fill();
      
  }

  //print summary
  cout<<"--------Summary-------"<<endl;
  cout<<"Total events = "<<eventid<<endl;
  cout<<"Total mc ="<<MCBCounterTotal<<endl;
  cout<<"--------End Summary---"<<endl;

  ////Save the Ntuple and histograms
  cout<<"Going to save the ntuple and histograms"<<endl; 

  //---
  if(Ntuple.AutoSave("Overwrite")){ cout<<"Ntuple written."<<endl;}
  else{ cout<<"Failed to write ntuple"<<endl;return 0;}


  cout<<"Objects in the current file"<<endl;
  Ntuple.GetCurrentFile()->ls();

  OutFile.Close(); //this will delete all objects created inside the file if they were not written  
  cout<<OutFile.GetName()<<" has been closed."<<endl;


  return 1;
}




Int_t DsRecoAnalysis2::SetDsRecoBranches(){
  if(_fChain==NULL){cout<<"No Chain"<<endl;return 0;} 

  _fChain->SetBranchAddress("nEvt",&nEvt);
  _fChain->SetBranchAddress("EvtnDaus",EvtnDaus);
  _fChain->SetBranchAddress("EvtMass",EvtMass);
  _fChain->SetBranchAddress("Evtp3",Evtp3);
  _fChain->SetBranchAddress("Evtcosth",Evtcosth);
  _fChain->SetBranchAddress("Evtphi",Evtphi);
  _fChain->SetBranchAddress("Evtenergy",Evtenergy);
  _fChain->SetBranchAddress("Evtp3CM",Evtp3CM);
  _fChain->SetBranchAddress("EvtcosthCM",EvtcosthCM);
  _fChain->SetBranchAddress("EvtphiCM",EvtphiCM);
  _fChain->SetBranchAddress("EvtenergyCM",EvtenergyCM);
  _fChain->SetBranchAddress("EvtLund",EvtLund); 
  _fChain->SetBranchAddress("Evtd1Lund",Evtd1Lund);
  _fChain->SetBranchAddress("Evtd1Idx",Evtd1Idx);
  _fChain->SetBranchAddress("Evtd2Lund",Evtd2Lund);
  _fChain->SetBranchAddress("Evtd2Idx",Evtd2Idx);
  _fChain->SetBranchAddress("Evtd3Lund",Evtd3Lund);
  _fChain->SetBranchAddress("Evtd3Idx",Evtd3Idx);



  _fChain->SetBranchAddress("nFrag",&nFrag);
  _fChain->SetBranchAddress("FragnDaus",FragnDaus);
  _fChain->SetBranchAddress("FragMass",FragMass);
  _fChain->SetBranchAddress("Fragp3",Fragp3);
  _fChain->SetBranchAddress("Fragcosth",Fragcosth);
  _fChain->SetBranchAddress("Fragphi",Fragphi);
  _fChain->SetBranchAddress("Fragenergy",Fragenergy);
  _fChain->SetBranchAddress("Fragp3CM",Fragp3CM);
  _fChain->SetBranchAddress("FragcosthCM",FragcosthCM);
  _fChain->SetBranchAddress("FragphiCM",FragphiCM);
  _fChain->SetBranchAddress("FragenergyCM",FragenergyCM);
  _fChain->SetBranchAddress("FragLund",FragLund); 
  _fChain->SetBranchAddress("Fragd1Lund",Fragd1Lund);
  _fChain->SetBranchAddress("Fragd1Idx",Fragd1Idx);
  _fChain->SetBranchAddress("Fragd2Lund",Fragd2Lund);
  _fChain->SetBranchAddress("Fragd2Idx",Fragd2Idx);
  _fChain->SetBranchAddress("Fragd3Lund",Fragd3Lund);
  _fChain->SetBranchAddress("Fragd3Idx",Fragd3Idx);
  _fChain->SetBranchAddress("Fragd4Lund",Fragd4Lund);
  _fChain->SetBranchAddress("Fragd4Idx",Fragd4Idx);
  _fChain->SetBranchAddress("Fragd5Lund",Fragd5Lund);
  _fChain->SetBranchAddress("Fragd5Idx",Fragd5Idx);
  _fChain->SetBranchAddress("Fragd6Lund",Fragd6Lund);
  _fChain->SetBranchAddress("Fragd6Idx",Fragd6Idx);
  _fChain->SetBranchAddress("Fragd7Lund",Fragd7Lund);
  _fChain->SetBranchAddress("Fragd7Idx",Fragd7Idx);
  _fChain->SetBranchAddress("Fragd8Lund",Fragd8Lund);
  _fChain->SetBranchAddress("Fragd8Idx",Fragd8Idx);
  _fChain->SetBranchAddress("Fragd9Lund",Fragd9Lund);
  _fChain->SetBranchAddress("Fragd9Idx",Fragd9Idx);


  _fChain->SetBranchAddress("nB",&nB);
  _fChain->SetBranchAddress("BMass",BMass);
  _fChain->SetBranchAddress("Bp3",Bp3);
  _fChain->SetBranchAddress("Bcosth",Bcosth);
  _fChain->SetBranchAddress("Bphi",Bphi);
  _fChain->SetBranchAddress("Benergy",Benergy);
  _fChain->SetBranchAddress("Bp3CM",Bp3CM);
  _fChain->SetBranchAddress("BcosthCM",BcosthCM);
  _fChain->SetBranchAddress("BphiCM",BphiCM);
  _fChain->SetBranchAddress("BenergyCM",BenergyCM);
  _fChain->SetBranchAddress("BLund",BLund); 
  _fChain->SetBranchAddress("Bd1Lund",Bd1Lund);
  _fChain->SetBranchAddress("Bd1Idx",Bd1Idx);
  _fChain->SetBranchAddress("Bd2Lund",Bd2Lund);
  _fChain->SetBranchAddress("Bd2Idx",Bd2Idx);
  _fChain->SetBranchAddress("Bd3Lund",Bd3Lund);
  _fChain->SetBranchAddress("Bd3Idx",Bd3Idx);

  _fChain->SetBranchAddress("nGam",&nGam);
  _fChain->SetBranchAddress("Gamp3",Gamp3);
  _fChain->SetBranchAddress("Gamcosth",Gamcosth);
  _fChain->SetBranchAddress("Gamphi",Gamphi);
  _fChain->SetBranchAddress("Gamenergy",Gamenergy);
  _fChain->SetBranchAddress("Gamp3CM",Gamp3CM);
  _fChain->SetBranchAddress("GamcosthCM",GamcosthCM);
  _fChain->SetBranchAddress("GamphiCM",GamphiCM);
  _fChain->SetBranchAddress("GamenergyCM",GamenergyCM);
  _fChain->SetBranchAddress("GamLund",GamLund);



  if(_TruthMatch){
 //    _fChain->SetBranchAddress("EvtMyDsMCIdx",EvtMyDsMCIdx);
//     _fChain->SetBranchAddress("EvtMyKMCIdx",EvtMyKMCIdx);
//     _fChain->SetBranchAddress("EvtMyTagMCIdx",EvtMyTagMCIdx);
//     _fChain->SetBranchAddress("EvtMyFragMCIdx",EvtMyFragMCIdx);
  } 

  SetTagBranches();
  cout<<"Done linking DsReco Chain Branches"<<endl;
  return 1;
}



Int_t DsRecoAnalysis2::MakeDsRecoReducedNtupleBranches(TTree* Ntuple){
 
  Ntuple->Branch("evtmass",&evtmass,"evtmass/F");
  Ntuple->Branch("evtpstar",&evtpstar,"evtpstar/F");
  Ntuple->Branch("evtenergy",&evtenergy,"evtenergy/F");
  Ntuple->Branch("evtcmenergy",&evtcmenergy,"evtcmenergy/F");
  Ntuple->Branch("evtcosstar",&evtcosstar,"evtcosstar/F");
  Ntuple->Branch("evtphistar",&evtphistar,"evtphistar/F");
  Ntuple->Branch("evtcharge",&evtcharge,"evtcharge/I");
  Ntuple->Branch("nEvt",&nEvt,"nEvt/I");
  Ntuple->Branch("EvtIdx",&EvtIdx,"EvtIdx/I");
  Ntuple->Branch("evtbestcand",&evtbestcand,"evtbestcand/I");
  Ntuple->Branch("evtmctrue",&evtmctrue,"evtmctrue/I"); 

  Ntuple->Branch("bmass",&bmass,"bmass/F");  
  Ntuple->Branch("bpstar",&bpstar,"bpstar/F"); 
  Ntuple->Branch("bp",&bp,"bp/F");  
  Ntuple->Branch("benergy",&benergy,"benergy/F");
  Ntuple->Branch("benergycm",&benergycm,"benergycm/F"); 
  Ntuple->Branch("bdeltae",&bdeltae,"bdeltae/F");  
  Ntuple->Branch("bcosthcm",&bcosthcm,"bcosthcm/F");

  Ntuple->Branch("mismass",&mismass,"mismass/F");  
  Ntuple->Branch("misp",&misp,"misp/F"); 

  Ntuple->Branch("dsp",&dsp,"dsp/F"); 
  Ntuple->Branch("dsmass",&dsmass,"dsmass/F"); 
  Ntuple->Branch("dsstp",&dsstp,"dsstp/F"); 
  Ntuple->Branch("dsstmass",&dsstmass,"dsstmass/F"); 
  Ntuple->Branch("dsstdeltam",&dsstdeltam,"dssdeltam/F"); 


  Ntuple->Branch("gamenergy",&gamenergy,"gamenergy/F");
  Ntuple->Branch("gamangle",&gamangle,"gamangle/F");
  Ntuple->Branch("Kangle",&Kangle,"Kangle/F");
  Ntuple->Branch("Kmctrue",&Kmctrue,"Kmctrue/I"); 

  Ntuple->Branch("fragenergy",&fragenergy,"fragenergy/F");
  Ntuple->Branch("fragcharge",&fragcharge,"fragcharge/I");
  Ntuple->Branch("fragndaus",&fragndaus,"fragndaus/I");
  Ntuple->Branch("fragnpis",&fragnpis,"fragnpis/I");
  Ntuple->Branch("fragnpi0s",&fragnpi0s,"fragnpi0s/I");
  Ntuple->Branch("fragngams",&fragngams,"fragngams/I");
  Ntuple->Branch("fragnKs",&fragnKs,"fragnKs/I");
  Ntuple->Branch("fragnKss",&fragnKss,"fragnKss/I");
  Ntuple->Branch("fragnEs",&fragnEs,"fragnEs/I");
  Ntuple->Branch("fragnMus",&fragnMus,"fragnMus/I");
  Ntuple->Branch("fragnENus",&fragnENus,"fragnENus/I");
  Ntuple->Branch("fragnMuNus",&fragnMuNus,"fragnMuNus/I");
  Ntuple->Branch("fragmctrue",&fragmctrue,"fragmctrue/I");
  Ntuple->Branch("fragangle",&fragangle,"fragangle/F");
  Ntuple->Branch("fragenergyK",&fragenergyK,"fragenergyK/F");
  Ntuple->Branch("fragenergyKs",&fragenergyKs,"fragenergyKs/F");
  Ntuple->Branch("fragenergypi",&fragenergypi,"fragenergypi/F");
  Ntuple->Branch("fragenergypi0",&fragenergypi0,"fragenergypi0/F");
  Ntuple->Branch("fragenergygam",&fragenergygam,"fragenergygam/F");
  Ntuple->Branch("fragenergyE",&fragenergyE,"fragenergyE/F");
  Ntuple->Branch("fragenergyMu",&fragenergyMu,"fragenergyMu/F");
  Ntuple->Branch("fragenergyENu",&fragenergyENu,"fragenergyENu/F");
  Ntuple->Branch("fragenergyMuNu",&fragenergyMuNu,"fragenergyMuNu/F");
  


  MakeTagReducedNtupleBranches(Ntuple);
  cout<<"Done Making  DsReco  Reduced Ntuple Branches"<<endl;
  return 1;
}



Int_t DsRecoAnalysis2::SetDsRecoReducedNtupleBranches(){
  ///only necesary if looping over the reduced ntuple

  ReducedNtuple->SetBranchAddress("evtmass",&evtmass);
  ReducedNtuple->SetBranchAddress("evtenergy",&evtenergy);
  ReducedNtuple->SetBranchAddress("evtcmenergy",&evtcmenergy);
  ReducedNtuple->SetBranchAddress("evtpstar",&evtpstar);
  ReducedNtuple->SetBranchAddress("evtcosstar",&evtcosstar);
  ReducedNtuple->SetBranchAddress("evtphistar",&evtphistar);
  ReducedNtuple->SetBranchAddress("EvtIdx",&EvtIdx);
  ReducedNtuple->SetBranchAddress("evtcharge",&evtcharge);
  ReducedNtuple->SetBranchAddress("nEvt",&nEvt);
  ReducedNtuple->SetBranchAddress("evtbestcand",&evtbestcand);
  ReducedNtuple->SetBranchAddress("evtmctrue",&evtmctrue);


  ReducedNtuple->SetBranchAddress("gamenergy",&gamenergy);
  ReducedNtuple->SetBranchAddress("gamangle",&gamangle);
  ReducedNtuple->SetBranchAddress("Kangle",&Kangle);


  SetTagReducedNtupleBranches();
  cout<<"Done linking DsReco ReducedNtuple Branches"<<endl;
  return 1;
}
