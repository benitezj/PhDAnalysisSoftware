#include "DsRecoAnalysis3.h"

#include "DsRecoAnalysis3FitDsStDeltaM.C"

ClassImp(DsRecoAnalysis3);
DsRecoAnalysis3::DsRecoAnalysis3(Bool_t MCorDATA,Bool_t TruthMatch,Int_t MatterOrAntiMatter,TString OutputDir):
  TagAnalysis(MCorDATA,TruthMatch,MatterOrAntiMatter,OutputDir),
  cuttruth(0),
  cutcharge(0),
  cutgamangle(0),
  cutKangle(0)
{
  cout<<"Done Initializing the analysis"<<endl;
}

DsRecoAnalysis3::~DsRecoAnalysis3(){ 
}

Int_t DsRecoAnalysis3::ReduceNtuple(Bool_t RunInTestMode){
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

    
    //find gen Ds 
    if(_TruthMatch){
      DsStGenMCIdx=-1;
      for(Int_t mcc=0;mcc<mcLen;mcc++){
	if(abs(mcLund[mcc])==myDsStLund){
	  DsStGenMCIdx=mcc;
	}
      }
   
      //Ds Idx
      if(DsStGenMCIdx!=-1){
	if(dauLen[DsStGenMCIdx]>=1)if(abs(mcLund[dauIdx[DsStGenMCIdx]])==myDsLund)DsGenMCIdx=dauIdx[DsStGenMCIdx];
	if(dauLen[DsStGenMCIdx]>=2)if(abs(mcLund[dauIdx[DsStGenMCIdx]+1])==myDsLund)DsGenMCIdx=dauIdx[DsStGenMCIdx]+1;
	if(dauLen[DsStGenMCIdx]>=3)if(abs(mcLund[dauIdx[DsStGenMCIdx]+2])==myDsLund)DsGenMCIdx=dauIdx[DsStGenMCIdx]+2;
      }
      //gam Idx


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

void DsRecoAnalysis3::FillDsRecoVars(){

  if(EvtIdx>=MAXNCANDS)return;
  
  ///determine the indexes for each particle before used by other methods
  TagIdx =Evtd1Idx[EvtIdx];
  TagKIdx=Evtd2Idx[EvtIdx];
  FragIdx=Evtd3Idx[EvtIdx];
  GamIdx =Evtd4Idx[EvtIdx];  

  
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
  if(_MCorDATA==1) Kmctrue=1;   else Kmctrue=(EvtMyKMCIdx[EvtIdx]>=0);  
  if(_MCorDATA==1) fragmctrue=1; else fragmctrue=(EvtMyFragMCIdx>=0);
  
  Kangle=ComputeKAngle(); 
  Klund=Evtd2Lund[EvtIdx];
 
  gamangle=ComputeGamAngle();
  gamenergy=Gamp3CM[GamIdx];

  //compute missing four-vector
  TLorentzVector eep4(TVector3(eePx,eePy,eePz),eeE);
  TVector3 eeVelVec(eePx/eeE,eePy/eeE,eePz/eeE);
  TVector3 Evtp3V(Evtp3[EvtIdx]*sin(acos(Evtcosth[EvtIdx]))*cos(Evtphi[EvtIdx]),
		  Evtp3[EvtIdx]*sin(acos(Evtcosth[EvtIdx]))*sin(Evtphi[EvtIdx]),
		  Evtp3[EvtIdx]*Evtcosth[EvtIdx]);
  TLorentzVector Evtp4(Evtp3V,Evtenergy[EvtIdx]);
  TVector3 Fragp3V(Fragp3[FragIdx]*sin(acos(Fragcosth[FragIdx]))*cos(Fragphi[FragIdx]),
		   Fragp3[FragIdx]*sin(acos(Fragcosth[FragIdx]))*sin(Fragphi[FragIdx]),
		   Fragp3[FragIdx]*Fragcosth[FragIdx]);
  TLorentzVector Fragp4(Fragp3V,Fragenergy[FragIdx]);
  TVector3 Gamp3V(Gamp3[GamIdx]*sin(acos(Gamcosth[GamIdx]))*cos(Gamphi[GamIdx]),
		  Gamp3[GamIdx]*sin(acos(Gamcosth[GamIdx]))*sin(Gamphi[GamIdx]),
		  Gamp3[GamIdx]*Gamcosth[GamIdx]);
  TLorentzVector Gamp4(Gamp3V,Gamenergy[GamIdx]);

  TVector3 Tagp3vec(Tagp3[TagIdx]*sin(acos(Tagcosth[TagIdx]))*cos(Tagphi[TagIdx]),
		    Tagp3[TagIdx]*sin(acos(Tagcosth[TagIdx]))*sin(Tagphi[TagIdx]),
		    Tagp3[TagIdx]*Tagcosth[TagIdx]);
  TLorentzVector Tagp4(Tagp3vec,Tagenergy[EvtIdx]);

  TVector3 Tagp3vecCM(Tagp3CM[TagIdx]*sin(acos(TagcosthCM[TagIdx]))*cos(TagphiCM[TagIdx]),
		    Tagp3CM[TagIdx]*sin(acos(TagcosthCM[TagIdx]))*sin(TagphiCM[TagIdx]),
		    Tagp3CM[TagIdx]*TagcosthCM[TagIdx]);
  TLorentzVector Tagp4CM(Tagp3vecCM,TagenergyCM[EvtIdx]);

  TVector3 Kp3VCM;
  if(abs(Evtd2Lund[EvtIdx])==myKLund){
    Kp3VCM.SetXYZ(Kp3CM[TagKIdx]*sin(acos(KcosthCM[TagKIdx]))*cos(KphiCM[TagKIdx]),
		  Kp3CM[TagKIdx]*sin(acos(KcosthCM[TagKIdx]))*sin(KphiCM[TagKIdx]),
		  Kp3CM[TagKIdx]*KcosthCM[TagKIdx]);
    Ke=Kenergy[TagKIdx];
    Kecm=KenergyCM[TagKIdx];
  }
  if(abs(Evtd2Lund[EvtIdx])==myKS0Lund){
    Kp3VCM.SetXYZ(Ksp3CM[TagKIdx]*sin(acos(KscosthCM[TagKIdx]))*cos(KsphiCM[TagKIdx]),
		  Ksp3CM[TagKIdx]*sin(acos(KscosthCM[TagKIdx]))*sin(KsphiCM[TagKIdx]),
		  Ksp3CM[TagKIdx]*KscosthCM[TagKIdx]);
    Ke=Ksenergy[TagKIdx];
    Kecm=KsenergyCM[TagKIdx];
  }
  TLorentzVector Kp4CM(Kp3VCM,Kecm);
 

  //the Ds
  TLorentzVector Dsp4(eep4-Evtp4);  
  TLorentzVector Dsp4CM=Dsp4;  Dsp4CM.Boost(-eeVelVec);
  dsmass=Dsp4.Mag();
  dsp=Dsp4.Vect().Mag();
  dspstar=Dsp4CM.Vect().Mag();
  dsenergycm=Dsp4CM.T();
  //the Ds*
  TLorentzVector DsStp4(Dsp4+Gamp4);
  TLorentzVector DsStp4CM=DsStp4;  DsStp4CM.Boost(-eeVelVec);
  dsstmass=DsStp4.Mag();
  dsstdeltam=dsstmass-dsmass;
  dsstp=DsStp4.Vect().Mag();
  dsstpstar=DsStp4CM.Vect().Mag();
  dsstenergycm=DsStp4CM.T();



  //separate fragmentation as tag side and sig side
  TVector3 TagSideFragp3VCM(0.,0.,0.);
  TVector3 SigSideFragp3VCM(0.,0.,0.);
  Float_t TagSideFragECM=0.;
  Float_t SigSideFragECM=0.; 
  for(Int_t i=0;i<NFragDaus;i++){
    TVector3 FragMemberp3VCM;
    Float_t FragMemberECM=0.;
    if(abs(FragdauLund[i][FragIdx])==myPiLund){
      FragMemberp3VCM.SetXYZ(Pip3CM[FragdauIdx[i][FragIdx]]*sin(acos(PicosthCM[FragdauIdx[i][FragIdx]]))*cos(PiphiCM[FragdauIdx[i][FragIdx]]),Pip3CM[FragdauIdx[i][FragIdx]]*sin(acos(PicosthCM[FragdauIdx[i][FragIdx]]))*sin(PiphiCM[FragdauIdx[i][FragIdx]]),Pip3CM[FragdauIdx[i][FragIdx]]*PicosthCM[FragdauIdx[i][FragIdx]]);
      FragMemberECM=PienergyCM[FragdauIdx[i][FragIdx]];
    }
    
    if(abs(FragdauLund[i][FragIdx])==myPi0Lund){
      FragMemberp3VCM.SetXYZ(Pi0p3CM[FragdauIdx[i][FragIdx]]*sin(acos(Pi0costhCM[FragdauIdx[i][FragIdx]]))*cos(Pi0phiCM[FragdauIdx[i][FragIdx]]),Pi0p3CM[FragdauIdx[i][FragIdx]]*sin(acos(Pi0costhCM[FragdauIdx[i][FragIdx]]))*sin(Pi0phiCM[FragdauIdx[i][FragIdx]]),Pi0p3CM[FragdauIdx[i][FragIdx]]*Pi0costhCM[FragdauIdx[i][FragIdx]]);
      FragMemberECM=Pi0energyCM[FragdauIdx[i][FragIdx]];
    }

    if(abs(FragdauLund[i][FragIdx])==myGammaLund){
      FragMemberp3VCM.SetXYZ(Gamp3CM[FragdauIdx[i][FragIdx]]*sin(acos(GamcosthCM[FragdauIdx[i][FragIdx]]))*cos(GamphiCM[FragdauIdx[i][FragIdx]]),Gamp3CM[FragdauIdx[i][FragIdx]]*sin(acos(GamcosthCM[FragdauIdx[i][FragIdx]]))*sin(GamphiCM[FragdauIdx[i][FragIdx]]),Gamp3CM[FragdauIdx[i][FragIdx]]*GamcosthCM[FragdauIdx[i][FragIdx]]);
      FragMemberECM=GamenergyCM[FragdauIdx[i][FragIdx]];
    }

    if(FragMemberp3VCM.Unit()*Tagp3vecCM.Unit()>0.){
      TagSideFragp3VCM+=FragMemberp3VCM;
      TagSideFragECM+=FragMemberECM;
    }

    if(FragMemberp3VCM.Unit()*Tagp3vecCM.Unit()<0.){
      SigSideFragp3VCM+=FragMemberp3VCM;
      SigSideFragECM+=FragMemberECM;
    }
    
  }
 
  //
  TLorentzVector TagSideFragp4CM=TLorentzVector(TagSideFragp3VCM,TagSideFragECM)+Tagp4CM;
  if(Kangle>0.)TagSideFragp4CM+=Kp4CM;
  TLorentzVector SigSideFragp4CM=TLorentzVector(SigSideFragp3VCM,SigSideFragECM)+DsStp4CM;
  if(Kangle<0.)SigSideFragp4CM+=Kp4CM;
  
  //compute tagside quantities
  tagsidemass=TagSideFragp4CM.Mag();
  tagsideenergycm=TagSideFragp4CM.T();
  tagsidepstar=TagSideFragp4CM.Vect().Mag();
  tagsidedeltae=tagsideenergycm-beamcmenergy/2.;
  //compute sigside quantities
  sigsidemass=SigSideFragp4CM.Mag();
  sigsideenergycm=SigSideFragp4CM.T();
  sigsidepstar=SigSideFragp4CM.Vect().Mag();
  sigsidedeltae=sigsideenergycm-beamcmenergy/2.;


  fragtagangle=TagSideFragp3VCM.Unit()*Tagp3vecCM.Unit();
  fragsigangle=SigSideFragp3VCM.Unit()*Tagp3vecCM.Unit();
   
  fragenergy=FragenergyCM[FragIdx];
  if(abs(FragLund[EvtIdx])==511)fragcharge=0;
  else fragcharge=FragLund[FragIdx]/abs(FragLund[FragIdx]);
  fragangle=ComputeFragAngle();
  fragndaus=FragnDaus[FragIdx];
  fragnpis=0;
  fragnpi0s=0;
  fragngams=0;
  fragpienergy=0.;
  fraggamenergy=0.;
  for(Int_t i=0;i<NFragDaus;i++){  
    if(abs(FragdauLund[i][FragIdx])==myPiLund){fragnpis++;fragpienergy+=Pienergy[FragdauIdx[i][FragIdx]];}
    if(abs(FragdauLund[i][FragIdx])==myPi0Lund){fragnpi0s++;fragpienergy+=Pi0energy[FragdauIdx[i][FragIdx]];}
    if(abs(FragdauLund[i][FragIdx])==myGammaLund){fragngams++;fraggamenergy+=Gamenergy[FragdauIdx[i][FragIdx]];}
  }
  
  //
  FillTagVars();
 
}


Float_t DsRecoAnalysis3::ComputeGamAngle(){
  //must be in CM frame
  TVector3 Tagp3vec(Tagp3CM[TagIdx]*sin(acos(TagcosthCM[TagIdx]))*cos(TagphiCM[TagIdx]),
		    Tagp3CM[TagIdx]*sin(acos(TagcosthCM[TagIdx]))*sin(TagphiCM[TagIdx]),
		    Tagp3CM[TagIdx]*TagcosthCM[TagIdx]);

  TVector3 Gamp3vec(Gamp3CM[GamIdx]*sin(acos(GamcosthCM[GamIdx]))*cos(GamphiCM[GamIdx]),
		    Gamp3CM[GamIdx]*sin(acos(GamcosthCM[GamIdx]))*sin(GamphiCM[GamIdx]),
		    Gamp3CM[GamIdx]*GamcosthCM[GamIdx]);


  return (Tagp3vec.Unit())*(Gamp3vec.Unit());
}


Float_t DsRecoAnalysis3::ComputeKAngle(){
  //must be in CM frame
  TVector3 Tagp3vec(Tagp3CM[TagIdx]*sin(acos(TagcosthCM[TagIdx]))*cos(TagphiCM[TagIdx]),
		    Tagp3CM[TagIdx]*sin(acos(TagcosthCM[TagIdx]))*sin(TagphiCM[TagIdx]),
		    Tagp3CM[TagIdx]*TagcosthCM[TagIdx]);

  TVector3 Kp3vec;
  if(abs(Evtd2Lund[EvtIdx])==myKLund){
    Kp3vec.SetXYZ(Kp3CM[TagKIdx]*sin(acos(KcosthCM[TagKIdx]))*cos(KphiCM[TagKIdx]),
		  Kp3CM[TagKIdx]*sin(acos(KcosthCM[TagKIdx]))*sin(KphiCM[TagKIdx]),
		  Kp3CM[TagKIdx]*KcosthCM[TagKIdx]);
  }
  if(abs(Evtd2Lund[EvtIdx])==myKS0Lund){
    Kp3vec.SetXYZ(Ksp3CM[TagKIdx]*sin(acos(KscosthCM[TagKIdx]))*cos(KsphiCM[TagKIdx]),
		  Ksp3CM[TagKIdx]*sin(acos(KscosthCM[TagKIdx]))*sin(KsphiCM[TagKIdx]),
		  Ksp3CM[TagKIdx]*KscosthCM[TagKIdx]);
  }
    

  return (Tagp3vec.Unit())*(Kp3vec.Unit());
}

Float_t DsRecoAnalysis3::ComputeFragAngle(){
  //must be in CM frame
  TVector3 Tagp3vec(Tagp3CM[TagIdx]*sin(acos(TagcosthCM[TagIdx]))*cos(TagphiCM[TagIdx]),
		    Tagp3CM[TagIdx]*sin(acos(TagcosthCM[TagIdx]))*sin(TagphiCM[TagIdx]),
		    Tagp3CM[TagIdx]*TagcosthCM[TagIdx]);

  TVector3 Fragp3vec(Fragp3CM[FragIdx]*sin(acos(FragcosthCM[FragIdx]))*cos(FragphiCM[FragIdx]),
		  Fragp3CM[FragIdx]*sin(acos(FragcosthCM[FragIdx]))*sin(FragphiCM[FragIdx]),
		  Fragp3CM[FragIdx]*FragcosthCM[FragIdx]);
  

  return (Tagp3vec.Unit())*(Fragp3vec.Unit());
}





Int_t DsRecoAnalysis3::SetDsRecoBranches(){
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
  _fChain->SetBranchAddress("Fragd1Lund",FragdauLund[0]);
  _fChain->SetBranchAddress("Fragd1Idx",FragdauIdx[0]);
  _fChain->SetBranchAddress("Fragd2Lund",FragdauLund[1]);
  _fChain->SetBranchAddress("Fragd2Idx",FragdauIdx[1]);
  _fChain->SetBranchAddress("Fragd3Lund",FragdauLund[2]);
  _fChain->SetBranchAddress("Fragd3Idx",FragdauIdx[2]);
  _fChain->SetBranchAddress("Fragd4Lund",FragdauLund[3]);
  _fChain->SetBranchAddress("Fragd4Idx",FragdauIdx[3]);
  _fChain->SetBranchAddress("Fragd5Lund",FragdauLund[4]);
  _fChain->SetBranchAddress("Fragd5Idx",FragdauIdx[4]);
  _fChain->SetBranchAddress("Fragd6Lund",FragdauLund[5]);
  _fChain->SetBranchAddress("Fragd6Idx",FragdauIdx[5]);
  _fChain->SetBranchAddress("Fragd7Lund",FragdauLund[6]);
  _fChain->SetBranchAddress("Fragd7Idx",FragdauIdx[6]);
  _fChain->SetBranchAddress("Fragd8Lund",FragdauLund[7]);
  _fChain->SetBranchAddress("Fragd8Idx",FragdauIdx[7]);
  _fChain->SetBranchAddress("Fragd9Lund",FragdauLund[8]);
  _fChain->SetBranchAddress("Fragd9Idx",FragdauIdx[8]);
  _fChain->SetBranchAddress("Fragd10Lund",FragdauLund[9]);
  _fChain->SetBranchAddress("Fragd10Idx",FragdauIdx[9]);
  _fChain->SetBranchAddress("Fragd11Lund",FragdauLund[10]);
  _fChain->SetBranchAddress("Fragd11Idx",FragdauIdx[10]);
  _fChain->SetBranchAddress("Fragd12Lund",FragdauLund[11]);
  _fChain->SetBranchAddress("Fragd12Idx",FragdauIdx[11]);
  _fChain->SetBranchAddress("Fragd13Lund",FragdauLund[12]);
  _fChain->SetBranchAddress("Fragd13Idx",FragdauIdx[12]);
  _fChain->SetBranchAddress("Fragd14Lund",FragdauLund[13]);
  _fChain->SetBranchAddress("Fragd14Idx",FragdauIdx[13]);
  _fChain->SetBranchAddress("Fragd15Lund",FragdauLund[14]);
  _fChain->SetBranchAddress("Fragd15Idx",FragdauIdx[14]);
  _fChain->SetBranchAddress("Fragd16Lund",FragdauLund[15]);
  _fChain->SetBranchAddress("Fragd16Idx",FragdauIdx[15]);
  _fChain->SetBranchAddress("Fragd17Lund",FragdauLund[16]);
  _fChain->SetBranchAddress("Fragd17Idx",FragdauIdx[16]);
  _fChain->SetBranchAddress("Fragd18Lund",FragdauLund[17]);
  _fChain->SetBranchAddress("Fragd18Idx",FragdauIdx[17]);
  _fChain->SetBranchAddress("Fragd19Lund",FragdauLund[18]);
  _fChain->SetBranchAddress("Fragd19Idx",FragdauIdx[18]);
  _fChain->SetBranchAddress("Fragd20Lund",FragdauLund[19]);
  _fChain->SetBranchAddress("Fragd20Idx",FragdauIdx[19]);


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



Int_t DsRecoAnalysis3::MakeDsRecoReducedNtupleBranches(TTree* Ntuple){
 
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

  Ntuple->Branch("tagsidemass",&tagsidemass,"tagsidemass/F");  
  Ntuple->Branch("tagsidepstar",&tagsidepstar,"tagsidepstar/F"); 
  Ntuple->Branch("tagsidep",&tagsidep,"tagsidep/F");  
  Ntuple->Branch("tagsideenergy",&tagsideenergy,"tagsideenergy/F");
  Ntuple->Branch("tagsideenergycm",&tagsideenergycm,"tagsideenergycm/F"); 
  Ntuple->Branch("tagsidedeltae",&tagsidedeltae,"tagsidedeltae/F");  
  Ntuple->Branch("tagsidecosthcm",&tagsidecosthcm,"tagsidecosthcm/F");

  Ntuple->Branch("sigsidemass",&sigsidemass,"sigsidemass/F");  
  Ntuple->Branch("sigsidepstar",&sigsidepstar,"sigsidepstar/F"); 
  Ntuple->Branch("sigsidep",&sigsidep,"sigsidep/F");  
  Ntuple->Branch("sigsideenergy",&sigsideenergy,"sigsideenergy/F");
  Ntuple->Branch("sigsideenergycm",&sigsideenergycm,"sigsideenergycm/F"); 
  Ntuple->Branch("sigsidedeltae",&sigsidedeltae,"sigsidedeltae/F");  
  Ntuple->Branch("sigsidecosthcm",&sigsidecosthcm,"sigsidecosthcm/F");

  Ntuple->Branch("dsp",&dsp,"dsp/F"); 
  Ntuple->Branch("dspstar",&dspstar,"dspstar/F"); 
  Ntuple->Branch("dsenergycm",&dsenergycm,"dsenergycm/F"); 
  Ntuple->Branch("dsmass",&dsmass,"dsmass/F"); 
  Ntuple->Branch("dsstp",&dsstp,"dsstp/F"); 
  Ntuple->Branch("dsstpstar",&dsstpstar,"dsstpstar/F"); 
  Ntuple->Branch("dsstenergycm",&dsstenergycm,"dsstenergycm/F"); 
  Ntuple->Branch("dsstmass",&dsstmass,"dsstmass/F"); 
  Ntuple->Branch("dsstdeltam",&dsstdeltam,"dssdeltam/F"); 

  Ntuple->Branch("gamenergy",&gamenergy,"gamenergy/F");
  Ntuple->Branch("gamangle",&gamangle,"gamangle/F");
  Ntuple->Branch("Ke",&Ke,"Ke/F");
  Ntuple->Branch("Kecm",&Kecm,"Kecm/F");
  Ntuple->Branch("Kangle",&Kangle,"Kangle/F");
  Ntuple->Branch("Kmctrue",&Kmctrue,"Kmctrue/I"); 
  Ntuple->Branch("Klund",&Klund,"Klund/I");
  Ntuple->Branch("fragenergy",&fragenergy,"fragenergy/F");
  Ntuple->Branch("fragcharge",&fragcharge,"fragcharge/I");
  Ntuple->Branch("fragndaus",&fragndaus,"fragndaus/I");
  Ntuple->Branch("fragpienergy",&fragpienergy,"fragpienergy/F");
  Ntuple->Branch("fragnpis",&fragnpis,"fragnpis/I");
  Ntuple->Branch("fragnpi0s",&fragnpi0s,"fragnpi0s/I");
  Ntuple->Branch("fraggamenergy",&fraggamenergy,"fraggamenergy/F");
  Ntuple->Branch("fragngams",&fragngams,"fragngams/I");
  Ntuple->Branch("fragmctrue",&fragmctrue,"fragmctrue/I");
  Ntuple->Branch("fragangle",&fragangle,"fragangle/F");
  Ntuple->Branch("fragtagangle",&fragtagangle,"fragtagangle/F");
  Ntuple->Branch("fragsigangle",&fragsigangle,"fragsigangle/F");


  MakeTagReducedNtupleBranches(Ntuple);
  cout<<"Done Making  DsReco  Reduced Ntuple Branches"<<endl;
  return 1;
}



Int_t DsRecoAnalysis3::SetDsRecoReducedNtupleBranches(){
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
