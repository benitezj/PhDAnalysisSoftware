#include "DsInclusiveAnalysis.h"

#include "DsInclusiveAnalysisFitDsM.C"

ClassImp(DsInclusiveAnalysis);
DsInclusiveAnalysis::DsInclusiveAnalysis(Bool_t MCorDATA,Bool_t TruthMatch,Int_t MatterOrAntiMatter,TString OutputDir):
  TagAnalysis(MCorDATA,TruthMatch,MatterOrAntiMatter,OutputDir)
  ,cutcharge(0)
  ,cuttagmass(0)
  ,cuttagpstar(0)
  ,cutdsstmass(0)
  ,cutevtvtxprob(0)
  ,cutfragngams(0)
  ,cutfragexgame(0)
{
  cout<<"Done Initializing the analysis"<<endl;
}

DsInclusiveAnalysis::~DsInclusiveAnalysis(){ 
}

Int_t DsInclusiveAnalysis::ReduceNtuple(){
  cout<<"Going to reduce the ntuple"<<endl;

  if(_fChain==NULL){
    cout<<"No chain."<<endl;
    return 0;
  } 

  ///link the branches in the input chain
  SetDsInclusiveBranches();



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
  MakeDsInclusiveReducedNtupleBranches(&Ntuple);
 

  //Counters
  Int_t MCBCounterTotal=0;
  Int_t RecoBCounterTotal=0;
  


  cout<<"cuts:"
      <<"  cutcharge="<<cutcharge
      <<"  cuttagmass="<<cuttagmass
      <<"  cuttagpstar="<<cuttagpstar
      <<"  cutdsstmass="<<cutdsstmass
      <<"  cutevtvtxprob="<<cutevtvtxprob
      <<"  cutfragngams="<<cutfragngams
      <<"  cutfragexgame="<<cutfragexgame
      <<endl;


  cout<<"Total Entries in Chain="<<_fChain->GetEntries()<<endl;
  cout<<"Going to start the event loop"<<endl;
  eventid=0;
  goodeventid=0;
  while(_fChain->GetEntry(eventid,0)>0){   
    eventid++;
    
    if(eventid%10000==0){
      cout<<eventid<<" Events done.   With "<<RecoBCounterTotal<<" Candidates saved."<<endl;    
    }
 
        
    if(_TruthMatch){
      //find gen Ds* Index 
      DsStGenMCIdx=-1;
      for(Int_t mcc=0;mcc<mcLen;mcc++){
	if(abs(mcLund[mcc])==myDsStLund){
	  DsStGenMCIdx=mcc;
	}
      }
   
      //Get the Ds Index
      if(DsStGenMCIdx!=-1){
	if(dauLen[DsStGenMCIdx]>=1)if(abs(mcLund[dauIdx[DsStGenMCIdx]])==myDsLund)DsGenMCIdx=dauIdx[DsStGenMCIdx];
	if(dauLen[DsStGenMCIdx]>=2)if(abs(mcLund[dauIdx[DsStGenMCIdx]+1])==myDsLund)DsGenMCIdx=dauIdx[DsStGenMCIdx]+1;
	if(dauLen[DsStGenMCIdx]>=3)if(abs(mcLund[dauIdx[DsStGenMCIdx]+2])==myDsLund)DsGenMCIdx=dauIdx[DsStGenMCIdx]+2;
      } 
      
      
      //------------------------------------------------
      //Count the number of fragmentation particles
      //-------------------------------------------------
      //This method doesnt work because it doesnt take into account D*'s
      //fragnpartgen=dauLen[2];
      //for(Int_t i=dauIdx[2];i<dauIdx[2]+dauLen[2];i++)
      //if(mcLund[i]==myGammaLund&&mcp3[i]<.05)fragnpartgen--;

      fragnpartgen=0;
      fragnpigen=0;
      fragnpi0gen=0;
      fragngamgen=0;
      for(Int_t mcc=2;mcc<mcLen;mcc++){
	if(abs(mcLund[mcc])==myPiLund)
	  if(abs(mcLund[mothIdx[mcc]])!=myD0Lund
	     &&abs(mcLund[mothIdx[mothIdx[mcc]]])!=myD0Lund
	     &&abs(mcLund[mothIdx[mothIdx[mothIdx[mcc]]]])!=myD0Lund
	     &&abs(mcLund[mothIdx[mcc]])!=myDPlusLund
	     &&abs(mcLund[mothIdx[mothIdx[mcc]]])!=myDPlusLund
	     &&abs(mcLund[mothIdx[mothIdx[mothIdx[mcc]]]])!=myDPlusLund
	     &&abs(mcLund[mothIdx[mcc]])!=myDsLund
	     &&abs(mcLund[mothIdx[mothIdx[mcc]]])!=myDsLund
	     &&abs(mcLund[mothIdx[mothIdx[mothIdx[mcc]]]])!=myDsLund
	     ){
	    fragnpartgen++;      
	    fragnpigen++;
	  }

	if(mcLund[mcc]==myPi0Lund)
	  if(abs(mcLund[mothIdx[mcc]])!=myD0Lund
	     &&abs(mcLund[mothIdx[mothIdx[mcc]]])!=myD0Lund
	     &&abs(mcLund[mothIdx[mothIdx[mothIdx[mcc]]]])!=myD0Lund
	     &&abs(mcLund[mothIdx[mcc]])!=myDPlusLund
	     &&abs(mcLund[mothIdx[mothIdx[mcc]]])!=myDPlusLund
	     &&abs(mcLund[mothIdx[mothIdx[mothIdx[mcc]]]])!=myDPlusLund
	     &&abs(mcLund[mothIdx[mcc]])!=myDsLund
	     &&abs(mcLund[mothIdx[mothIdx[mcc]]])!=myDsLund
	     &&abs(mcLund[mothIdx[mothIdx[mothIdx[mcc]]]])!=myDsLund
	     ){
	    fragnpartgen++;      
	    fragnpi0gen++;
	  }

	if(mcLund[mcc]==myGammaLund&&mcp3[mcc]>.05)
	  if(mothIdx[mcc]==2){
	    fragnpartgen++;      
	    fragngamgen++;
	  }
      }      


      //--------------------------------
      //Find the generated Tag-D Idx
      //--------------------------------
      GenTagIdx=-1;
      for(Int_t mcc=0;mcc<mcLen;mcc++){
	if(abs(mcLund[mcc])==myD0Lund
	   ||abs(mcLund[mcc])==myDPlusLund){
	  GenTagIdx=mcc;
	}
      }


      ////Remove events which are not signal events: D-K-Ds*
      if(GenTagIdx==-1||DsStGenMCIdx==-1)continue;

    }

        
    
    ///Event vars
    FillEventVars();
   
    ngoodthisevent=0; 
    
    if(nEvt>MAXNCANDS)nEvt=MAXNCANDS;
    //now do real loop
    for(EvtIdx=0;EvtIdx<nEvt;EvtIdx++){
     
      //
      FillDsInclusiveVars();          
      
      //Selection cuts
      if(cuttagmass==1)if(fabs(tagmassdiff)>TagMassCut)continue;
      if(cuttagpstar==1)if(tagpstar<2.)continue;
      //if(cutdsstmass==1)if(fabs(dsstuncmass-2.112)>.150)continue;
      if(cutdsstmass==1)if(fabs(dsstuncmass-2.112)>.150&&fabs(dsuncmass-1.9693)>.150)continue;
      if(cutevtvtxprob==1)if(evtvtxprob<.01)continue;
      if(cutdspstar==1)if(dspstar<3.)continue;
      if(cutfragngams==1)if(fragngams>1.)continue;
      if(cutfragexgame==1)if(fragexgame*fragexgamn>1.)continue;
      
      if(tagksflights<2.)continue;
      if(dspdiff>3.)continue;
      


      //save
      ngoodthisevent++;
      RecoBCounterTotal++;
      Ntuple.Fill();
   
    }// loop    
    if(ngoodthisevent>0)goodeventid++;
	

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

void DsInclusiveAnalysis::FillDsInclusiveVars(){

  if(EvtIdx>=MAXNCANDS)return;
  
  ///determine the indexes for each particle before used by other methods
  TagIdx =Evtd1Idx[EvtIdx];
  TagKIdx=Evtd2Idx[EvtIdx];
  DsStIdx=Evtd3Idx[EvtIdx];
  FragIdx=Evtd4Idx[EvtIdx];  

  DsIdx=DsStd1Idx[DsStIdx];
  GamIdx=DsStd2Idx[DsStIdx];

  
  //////fill reduced ntuple variables    
  evtmass=EvtMass[EvtIdx];
  evtpstar=Evtp3CM[EvtIdx];
  evtcosstar=EvtcosthCM[EvtIdx];
  evtphistar=EvtphiCM[EvtIdx];
  evtcmenergy=EvtenergyCM[EvtIdx];
  if(abs(EvtLund[EvtIdx])==513)evtcharge=0;
  else evtcharge=EvtLund[EvtIdx]/abs(EvtLund[EvtIdx]);
  evtvtxprob=TMath::Prob(EvtChi2[EvtIdx],EvtnDof[EvtIdx]);
  evtvtxx=EvtVtxx[EvtIdx];
  evtvtxy=EvtVtxy[EvtIdx];
  evtvtxz=EvtVtxz[EvtIdx];



  //Upsilon
  TVector3 eeVelVec(eePx/eeE,eePy/eeE,eePz/eeE);
  TVector3 Evtp3VCM(Evtp3CM[EvtIdx]*sin(acos(EvtcosthCM[EvtIdx]))*cos(EvtphiCM[EvtIdx]),
		  Evtp3CM[EvtIdx]*sin(acos(EvtcosthCM[EvtIdx]))*sin(EvtphiCM[EvtIdx]),
		  Evtp3CM[EvtIdx]*EvtcosthCM[EvtIdx]);
  TLorentzVector Evtp4(Evtp3VCM,EvtenergyCM[EvtIdx]);
  evtpx=Evtp3VCM.X();
  evtpy=Evtp3VCM.Y();
  evtpz=Evtp3VCM.Z(); 

  //Tag
  TVector3 Tagp3vecCM(Tagp3CM[TagIdx]*sin(acos(TagcosthCM[TagIdx]))*cos(TagphiCM[TagIdx]),
		    Tagp3CM[TagIdx]*sin(acos(TagcosthCM[TagIdx]))*sin(TagphiCM[TagIdx]),
		    Tagp3CM[TagIdx]*TagcosthCM[TagIdx]);
  TLorentzVector Tagp4CM(Tagp3vecCM,TagenergyCM[TagIdx]);
  tagqual=EvtqualD[EvtIdx];  
  tagmctrue=EvtMyTagMCIdx[EvtIdx];
  
  //K
  TVector3 Kp3VCM;
  if(abs(Evtd2Lund[EvtIdx])==myKLund){
    Kp3VCM.SetXYZ(Kp3CM[TagKIdx]*sin(acos(KcosthCM[TagKIdx]))*cos(KphiCM[TagKIdx]),
		  Kp3CM[TagKIdx]*sin(acos(KcosthCM[TagKIdx]))*sin(KphiCM[TagKIdx]),
		  Kp3CM[TagKIdx]*KcosthCM[TagKIdx]);
    Kecm=KenergyCM[TagKIdx];
  }
  if(abs(Evtd2Lund[EvtIdx])==myKS0Lund){
    Kp3VCM.SetXYZ(Ksp3CM[TagKIdx]*sin(acos(KscosthCM[TagKIdx]))*cos(KsphiCM[TagKIdx]),
		  Ksp3CM[TagKIdx]*sin(acos(KscosthCM[TagKIdx]))*sin(KsphiCM[TagKIdx]),
		  Ksp3CM[TagKIdx]*KscosthCM[TagKIdx]);
    Kecm=KsenergyCM[TagKIdx];
  }
  TLorentzVector Kp4CM(Kp3VCM,Kecm);
  Kangle=ComputeKAngle(); 
  Klund=Evtd2Lund[EvtIdx];
  Kmctrue=EvtMyKMCIdx[EvtIdx];
  Kqual=EvtqualK[EvtIdx];

  //the Ds
  TVector3 Dsp3VCM(Dsp3CM[DsIdx]*sin(acos(DscosthCM[DsIdx]))*cos(DsphiCM[DsIdx]),
		     Dsp3CM[DsIdx]*sin(acos(DscosthCM[DsIdx]))*sin(DsphiCM[DsIdx]),
		     Dsp3CM[DsIdx]*DscosthCM[DsIdx]);
  TLorentzVector Dsp4CM(Dsp3VCM,DsenergyCM[DsIdx]);
  dsmass=Dsp4CM.Mag();
  //dsmass=DsMass[DsIdx];//The mass of particle is filled as: cand->mass(); no check is made for consistency with sqrt(E^2-p^2)!!
  dsuncmass=EvtDsUncMass[EvtIdx];
  dspstar=Dsp3CM[DsIdx];
  dsenergycm=Dsp4CM.T();
  dscharge=DsLund[DsIdx]/abs(DsLund[DsIdx]);
  dsangle=Dsp4CM.Vect().Unit()*Tagp3vecCM.Unit();
  if(DsGenMCIdx>0){
    TVector3 DsGenPVect(mcp3CM[DsGenMCIdx]*sin(acos(mccosthCM[DsGenMCIdx]))*cos(mcphiCM[DsGenMCIdx]),
			mcp3CM[DsGenMCIdx]*sin(acos(mccosthCM[DsGenMCIdx]))*sin(mcphiCM[DsGenMCIdx]),
			mcp3CM[DsGenMCIdx]*mccosthCM[DsGenMCIdx]);
    dspxdiff=Dsp4CM.Vect().X()-DsGenPVect.X();
    dspydiff=Dsp4CM.Vect().Y()-DsGenPVect.Y();
    dspzdiff=Dsp4CM.Vect().Z()-DsGenPVect.Z();
    dspdiff=sqrt(dspxdiff*dspxdiff+dspydiff*dspydiff+dspzdiff*dspzdiff);
  }  
  dsunfitmass=EvtDsMassUnFit[EvtIdx];

  //the Ds*  
  TVector3 DsStp3VCM(DsStp3CM[DsStIdx]*sin(acos(DsStcosthCM[DsStIdx]))*cos(DsStphiCM[DsStIdx]),
		     DsStp3CM[DsStIdx]*sin(acos(DsStcosthCM[DsStIdx]))*sin(DsStphiCM[DsStIdx]),
		     DsStp3CM[DsStIdx]*DsStcosthCM[DsStIdx]);
  TLorentzVector DsStp4CM(DsStp3VCM,DsStenergyCM[DsStIdx]);
  dsstuncmass=EvtDsStUncMass[EvtIdx];
  dsstmass=DsStp4CM.Mag();
  dsstdeltam=dsstmass-dsmass;
  dsstpstar=DsStp4CM.Vect().Mag();
  dsstenergycm=DsStp4CM.T();
  dsstangle=DsStp3VCM.Unit()*Tagp3vecCM.Unit();
  TVector3 DsStp3VecRaw(EvtDsStPxRaw[EvtIdx],EvtDsStPyRaw[EvtIdx],EvtDsStPzRaw[EvtIdx]);
  TLorentzVector DsStp4Raw(DsStp3VecRaw,EvtDsStERaw[EvtIdx]);
  DsStp4Raw.Boost(-eeVelVec);//boost into cm.
  TVector3 DsStp3VecCorr(EvtDsStPxCorr[EvtIdx],EvtDsStPyCorr[EvtIdx],EvtDsStPzCorr[EvtIdx]);
  TLorentzVector DsStp4Corr(DsStp3VecCorr,EvtDsStECorr[EvtIdx]);
  DsStp4Corr.Boost(-eeVelVec);//boost into cm.
  dsstcorrpdiff=(DsStp4Corr.Vect()-DsStp4Raw.Vect()).Mag();
  dsstfitpdiff=(DsStp3VCM-DsStp4Corr.Vect()).Mag();

  //gamma 
  TVector3 Gamp3V(Gamp3CM[GamIdx]*sin(acos(GamcosthCM[GamIdx]))*cos(GamphiCM[GamIdx]),
		  Gamp3CM[GamIdx]*sin(acos(GamcosthCM[GamIdx]))*sin(GamphiCM[GamIdx]),
		  Gamp3CM[GamIdx]*GamcosthCM[GamIdx]);
  TLorentzVector Gamp4CM(Gamp3V,Gamp3V.Mag());
  TLorentzVector Gamp4Lab=Gamp4CM;
  Gamp4Lab.Boost(eeVelVec);
  gamenergy=Gamp4Lab.T();
  gamenergycm=Gamp4CM.T();
  gamangle=Gamp4CM.Vect().Unit()*Tagp3vecCM.Unit();
  gamdsstangle=Gamp4CM.Vect().Unit()*DsStp4CM.Vect().Unit();
  gammctrue=EvtMyGamMCIdx[EvtIdx];
  //boost the gamma into the rest frame of Ds* then compute angle to original direction of Ds*
  TLorentzVector Gamp4Dst=Gamp4CM;
  TVector3 DsStVelVec(DsStp3VCM*(1./DsStenergyCM[DsStIdx]));
  Gamp4Dst.Boost(-DsStVelVec);
  dsstdecayangle=Gamp4Dst.Vect().Unit()*DsStp4CM.Vect().Unit();



  ///Fragmentation
  TVector3 Fragp3V; 
  if(FragIdx>=0){
    Fragp3V.SetXYZ(Fragp3CM[FragIdx]*sin(acos(FragcosthCM[FragIdx]))*cos(FragphiCM[FragIdx]),
		   Fragp3CM[FragIdx]*sin(acos(FragcosthCM[FragIdx]))*sin(FragphiCM[FragIdx]),
		   Fragp3CM[FragIdx]*FragcosthCM[FragIdx]);    
    fragenergy=FragenergyCM[FragIdx];
  }else fragenergy=0.;
  TLorentzVector Fragp4(Fragp3V,fragenergy);
  fragndaus=0;
  fragnpis=0;
  fragnpi0s=0;
  fragngams=0;
  fragpienergy=0.;
  fraggamenergy=0.;
  if(FragIdx>=0){
    fragcharge=FragLund[FragIdx]/abs(FragLund[FragIdx]);
    fragangle=ComputeFragAngle();  
    if(abs(Evtd4Lund[EvtIdx])!=myPiLund
       &&abs(Evtd4Lund[EvtIdx])!=myPi0Lund
       &&abs(Evtd4Lund[EvtIdx])!=myGammaLund)
      fragndaus=FragnDaus[FragIdx];
    else fragndaus=0;
    if(fragndaus==0){//X is a single pion or gamma
      if(abs(Evtd4Lund[EvtIdx])==myPiLund){fragnpis=1;fragpienergy=PienergyCM[FragIdx];}
      if(abs(Evtd4Lund[EvtIdx])==myPi0Lund){fragnpi0s=1;fragpienergy=Pi0energyCM[FragIdx];}
      if(abs(Evtd4Lund[EvtIdx])==myGammaLund){fragngams=1;fraggamenergy=GamenergyCM[FragIdx];}
      fragndaus=1;
    }else {
      for(Int_t i=0;i<fragndaus;i++){  
	if(abs(FragdauLund[i][FragIdx])==myPiLund){fragnpis++;fragpienergy+=PienergyCM[FragdauIdx[i][FragIdx]];}
	if(abs(FragdauLund[i][FragIdx])==myPi0Lund){fragnpi0s++;fragpienergy+=Pi0energyCM[FragdauIdx[i][FragIdx]];}
	if(abs(FragdauLund[i][FragIdx])==myGammaLund){fragngams++;fraggamenergy+=GamenergyCM[FragdauIdx[i][FragIdx]];}
      }
    }
  
  } else {
    fragcharge=0;
    fragndaus=0;
    fragangle=0.;
  } 
  
  fragexgame=Evtexgame[EvtIdx];
  fragexgamn=Evtexgamn[EvtIdx];
  fragmctrue=EvtMyXMCIdx[EvtIdx];
  fragqual=EvtqualX[EvtIdx];



  //separate fragmentation as tag side and sig side
  TVector3 TagSideFragp3VCM(0.,0.,0.);
  TVector3 SigSideFragp3VCM(0.,0.,0.);
  Float_t TagSideFragECM=0.;
  Float_t SigSideFragECM=0.;
  if(NFragDaus>0){
    for(Int_t i=0;i<NFragDaus;i++){
      TVector3 FragMemberp3VCM;
      Float_t FragMemberECM=0.;
      if(abs(FragdauLund[i][FragIdx])==myPiLund){
	FragMemberp3VCM.SetXYZ(Pip3CM[FragdauIdx[i][FragIdx]]
			       *sin(acos(PicosthCM[FragdauIdx[i][FragIdx]]))
			       *cos(PiphiCM[FragdauIdx[i][FragIdx]]),
			       Pip3CM[FragdauIdx[i][FragIdx]]
			       *sin(acos(PicosthCM[FragdauIdx[i][FragIdx]]))
			       *sin(PiphiCM[FragdauIdx[i][FragIdx]]),
			       Pip3CM[FragdauIdx[i][FragIdx]]
			       *PicosthCM[FragdauIdx[i][FragIdx]]);
	FragMemberECM=PienergyCM[FragdauIdx[i][FragIdx]];
      }
    
      if(abs(FragdauLund[i][FragIdx])==myPi0Lund){
	FragMemberp3VCM.SetXYZ(Pi0p3CM[FragdauIdx[i][FragIdx]]
			       *sin(acos(Pi0costhCM[FragdauIdx[i][FragIdx]]))
			       *cos(Pi0phiCM[FragdauIdx[i][FragIdx]]),
			       Pi0p3CM[FragdauIdx[i][FragIdx]]
			       *sin(acos(Pi0costhCM[FragdauIdx[i][FragIdx]]))
			       *sin(Pi0phiCM[FragdauIdx[i][FragIdx]]),
			       Pi0p3CM[FragdauIdx[i][FragIdx]]
			       *Pi0costhCM[FragdauIdx[i][FragIdx]]);
	FragMemberECM=Pi0energyCM[FragdauIdx[i][FragIdx]];
      }

      if(abs(FragdauLund[i][FragIdx])==myGammaLund){
	FragMemberp3VCM.SetXYZ(Gamp3CM[FragdauIdx[i][FragIdx]]
			       *sin(acos(GamcosthCM[FragdauIdx[i][FragIdx]]))
			       *cos(GamphiCM[FragdauIdx[i][FragIdx]]),
			       Gamp3CM[FragdauIdx[i][FragIdx]]
			       *sin(acos(GamcosthCM[FragdauIdx[i][FragIdx]]))
			       *sin(GamphiCM[FragdauIdx[i][FragIdx]]),
			       Gamp3CM[FragdauIdx[i][FragIdx]]
			       *GamcosthCM[FragdauIdx[i][FragIdx]]);
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
  }else if(NFragDaus==0){
    TVector3 FragMemberp3VCM;
    Float_t FragMemberECM=0.;
    if(abs(FragLund[FragIdx])==myPiLund){
	FragMemberp3VCM.SetXYZ(Pip3CM[FragIdx]
			       *sin(acos(PicosthCM[FragIdx]))
			       *cos(PiphiCM[FragIdx]),
			       Pip3CM[FragIdx]
			       *sin(acos(PicosthCM[FragIdx]))
			       *sin(PiphiCM[FragIdx]),
			       Pip3CM[FragIdx]
			       *PicosthCM[FragIdx]);
	FragMemberECM=PienergyCM[FragIdx];    
    }
    if(abs(FragLund[FragIdx])==myPi0Lund){
      FragMemberp3VCM.SetXYZ(Pi0p3CM[FragIdx]
			     *sin(acos(Pi0costhCM[FragIdx]))
			     *cos(Pi0phiCM[FragIdx]),
			     Pi0p3CM[FragIdx]
			     *sin(acos(Pi0costhCM[FragIdx]))
			     *sin(Pi0phiCM[FragIdx]),
			     Pi0p3CM[FragIdx]
			     *Pi0costhCM[FragIdx]);
      FragMemberECM=Pi0energyCM[FragIdx];
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

 
  //
  FillTagVars();
  //redefine tagmass
  tagmass=EvtTagUncMass[EvtIdx];
}


Float_t DsInclusiveAnalysis::ComputeGamAngle(){
//   //must be in CM frame//currently does not work
//   TVector3 Tagp3vec(Tagp3[TagIdx]*sin(acos(Tagcosth[TagIdx]))*cos(Tagphi[TagIdx]),
// 		    Tagp3[TagIdx]*sin(acos(Tagcosth[TagIdx]))*sin(Tagphi[TagIdx]),
// 		    Tagp3[TagIdx]*Tagcosth[TagIdx]);

  
//   TVector3 Gamp3V(Evtgampx[EvtIdx],Evtgampy[EvtIdx],Evtgampz[EvtIdx]);



  //return (Tagp3vec.Unit())*(Gamp3V.Unit());
  return -1.;
}


Float_t DsInclusiveAnalysis::ComputeKAngle(){
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

Float_t DsInclusiveAnalysis::ComputeFragAngle(){
  //must be in CM frame
  TVector3 Tagp3vec(Tagp3CM[TagIdx]*sin(acos(TagcosthCM[TagIdx]))*cos(TagphiCM[TagIdx]),
		    Tagp3CM[TagIdx]*sin(acos(TagcosthCM[TagIdx]))*sin(TagphiCM[TagIdx]),
		    Tagp3CM[TagIdx]*TagcosthCM[TagIdx]);

  TVector3 Fragp3vec(Fragp3CM[FragIdx]*sin(acos(FragcosthCM[FragIdx]))*cos(FragphiCM[FragIdx]),
		  Fragp3CM[FragIdx]*sin(acos(FragcosthCM[FragIdx]))*sin(FragphiCM[FragIdx]),
		  Fragp3CM[FragIdx]*FragcosthCM[FragIdx]);
  

  return (Tagp3vec.Unit())*(Fragp3vec.Unit());
}





Int_t DsInclusiveAnalysis::SetDsInclusiveBranches(){
  if(_fChain==NULL){cout<<"No Chain"<<endl;return 0;} 
  
  if(_fChain->GetBranch("nEvt"))_fChain->SetBranchAddress("nEvt",&nEvt);
  if(_fChain->GetBranch("EvtnDaus"))_fChain->SetBranchAddress("EvtnDaus",EvtnDaus);
  if(_fChain->GetBranch("EvtChi2"))_fChain->SetBranchAddress("EvtChi2",EvtChi2);
  if(_fChain->GetBranch("EvtnDof"))_fChain->SetBranchAddress("EvtnDof",EvtnDof);
  if(_fChain->GetBranch("EvtMass"))_fChain->SetBranchAddress("EvtMass",EvtMass);
  if(_fChain->GetBranch("Evtp3CM"))_fChain->SetBranchAddress("Evtp3CM",Evtp3CM);
  if(_fChain->GetBranch("EvtcosthCM"))_fChain->SetBranchAddress("EvtcosthCM",EvtcosthCM);
  if(_fChain->GetBranch("EvtphiCM"))_fChain->SetBranchAddress("EvtphiCM",EvtphiCM);
  if(_fChain->GetBranch("EvtenergyCM"))_fChain->SetBranchAddress("EvtenergyCM",EvtenergyCM);
  if(_fChain->GetBranch("EvtLund"))_fChain->SetBranchAddress("EvtLund",EvtLund); 
  if(_fChain->GetBranch("Evtd1Lund"))_fChain->SetBranchAddress("Evtd1Lund",Evtd1Lund);//tag
  if(_fChain->GetBranch("Evtd1Idx"))_fChain->SetBranchAddress("Evtd1Idx",Evtd1Idx);
  if(_fChain->GetBranch("Evtd2Lund"))_fChain->SetBranchAddress("Evtd2Lund",Evtd2Lund);//K
  if(_fChain->GetBranch("Evtd2Idx"))_fChain->SetBranchAddress("Evtd2Idx",Evtd2Idx);
  if(_fChain->GetBranch("Evtd3Lund"))_fChain->SetBranchAddress("Evtd3Lund",Evtd3Lund);//X
  if(_fChain->GetBranch("Evtd3Idx"))_fChain->SetBranchAddress("Evtd3Idx",Evtd3Idx);
  if(_fChain->GetBranch("Evtd4Lund"))_fChain->SetBranchAddress("Evtd4Lund",Evtd4Lund);//Ds*
  if(_fChain->GetBranch("Evtd4Idx"))_fChain->SetBranchAddress("Evtd4Idx",Evtd4Idx);
  if(_fChain->GetBranch("EvtVtxx"))_fChain->SetBranchAddress("EvtVtxx",EvtVtxx);
  if(_fChain->GetBranch("EvtVtxy"))_fChain->SetBranchAddress("EvtVtxy",EvtVtxy);
  if(_fChain->GetBranch("EvtVtxz"))_fChain->SetBranchAddress("EvtVtxz",EvtVtxz);
  if(_fChain->GetBranch("EvtDsStUncMass"))_fChain->SetBranchAddress("EvtDsStUncMass",EvtDsStUncMass);
  if(_fChain->GetBranch("EvtDsUncMass"))_fChain->SetBranchAddress("EvtDsUncMass",EvtDsUncMass);
  if(_fChain->GetBranch("EvtTagUncMass"))_fChain->SetBranchAddress("EvtTagUncMass",EvtTagUncMass);
  if(_fChain->GetBranch("Evtexgampx"))_fChain->SetBranchAddress("Evtexgampx",Evtexgampx);
  if(_fChain->GetBranch("Evtexgampy"))_fChain->SetBranchAddress("Evtexgampy",Evtexgampy);
  if(_fChain->GetBranch("Evtexgampz"))_fChain->SetBranchAddress("Evtexgampz",Evtexgampz);
  if(_fChain->GetBranch("Evtexgame"))_fChain->SetBranchAddress("Evtexgame",Evtexgame);
  if(_fChain->GetBranch("Evtexgamn"))_fChain->SetBranchAddress("Evtexgamn",Evtexgamn);
  if(_fChain->GetBranch("EvtqualD"))_fChain->SetBranchAddress("EvtqualD",EvtqualD);
  if(_fChain->GetBranch("EvtqualK"))_fChain->SetBranchAddress("EvtqualK",EvtqualK);
  if(_fChain->GetBranch("EvtqualX"))_fChain->SetBranchAddress("EvtqualX",EvtqualX);
  if(_fChain->GetBranch("EvtMyGamMCIdx"))_fChain->SetBranchAddress("EvtMyGamMCIdx",EvtMyGamMCIdx);  
  if(_fChain->GetBranch("EvtMyKMCIdx"))_fChain->SetBranchAddress("EvtMyKMCIdx",EvtMyKMCIdx);  
  if(_fChain->GetBranch("EvtMyTagMCIdx"))_fChain->SetBranchAddress("EvtMyTagMCIdx",EvtMyTagMCIdx);  
  if(_fChain->GetBranch("EvtMyXMCIdx"))_fChain->SetBranchAddress("EvtMyXMCIdx",EvtMyXMCIdx); 
  if(_fChain->GetBranch("EvtDsStPxRaw"))_fChain->SetBranchAddress("EvtDsStPxRaw",EvtDsStPxRaw); 
  if(_fChain->GetBranch("EvtDsStPyRaw"))_fChain->SetBranchAddress("EvtDsStPyRaw",EvtDsStPyRaw); 
  if(_fChain->GetBranch("EvtDsStPzRaw"))_fChain->SetBranchAddress("EvtDsStPzRaw",EvtDsStPzRaw); 
  if(_fChain->GetBranch("EvtDsStERaw"))_fChain->SetBranchAddress("EvtDsStERaw",EvtDsStERaw); 
  if(_fChain->GetBranch("EvtDsStPxCorr"))_fChain->SetBranchAddress("EvtDsStPxCorr",EvtDsStPxCorr); 
  if(_fChain->GetBranch("EvtDsStPyCorr"))_fChain->SetBranchAddress("EvtDsStPyCorr",EvtDsStPyCorr); 
  if(_fChain->GetBranch("EvtDsStPzCorr"))_fChain->SetBranchAddress("EvtDsStPzCorr",EvtDsStPzCorr); 
  if(_fChain->GetBranch("EvtDsStECorr"))_fChain->SetBranchAddress("EvtDsStECorr",EvtDsStECorr); 
  if(_fChain->GetBranch("EvtDsMassUnFit"))_fChain->SetBranchAddress("EvtDsMassUnFit",EvtDsMassUnFit);


  if(_fChain->GetBranch("nFrag"))_fChain->SetBranchAddress("nFrag",&nFrag);
  if(_fChain->GetBranch("FragnDaus"))_fChain->SetBranchAddress("FragnDaus",FragnDaus);
  if(_fChain->GetBranch("FragMass"))_fChain->SetBranchAddress("FragMass",FragMass);
  if(_fChain->GetBranch("Fragp3CM"))_fChain->SetBranchAddress("Fragp3CM",Fragp3CM);
  if(_fChain->GetBranch("FragcosthCM"))_fChain->SetBranchAddress("FragcosthCM",FragcosthCM);
  if(_fChain->GetBranch("FragphiCM"))_fChain->SetBranchAddress("FragphiCM",FragphiCM);
  if(_fChain->GetBranch("FragenergyCM"))_fChain->SetBranchAddress("FragenergyCM",FragenergyCM);
  if(_fChain->GetBranch("FragLund"))_fChain->SetBranchAddress("FragLund",FragLund); 
  if(_fChain->GetBranch("Fragd1Lund"))_fChain->SetBranchAddress("Fragd1Lund",FragdauLund[0]);
  if(_fChain->GetBranch("Fragd1Idx"))_fChain->SetBranchAddress("Fragd1Idx",FragdauIdx[0]);
  if(_fChain->GetBranch("Fragd2Lund"))_fChain->SetBranchAddress("Fragd2Lund",FragdauLund[1]);
  if(_fChain->GetBranch("Fragd2Idx"))_fChain->SetBranchAddress("Fragd2Idx",FragdauIdx[1]);
  if(_fChain->GetBranch("Fragd3Lund"))_fChain->SetBranchAddress("Fragd3Lund",FragdauLund[2]);
  if(_fChain->GetBranch("Fragd3Idx"))_fChain->SetBranchAddress("Fragd3Idx",FragdauIdx[2]);
  if(_fChain->GetBranch("Fragd4Lund"))_fChain->SetBranchAddress("Fragd4Lund",FragdauLund[3]);
  if(_fChain->GetBranch("Fragd4Idx"))_fChain->SetBranchAddress("Fragd4Idx",FragdauIdx[3]);
  if(_fChain->GetBranch("Fragd5Lund"))_fChain->SetBranchAddress("Fragd5Lund",FragdauLund[4]);
  if(_fChain->GetBranch("Fragd5Idx"))_fChain->SetBranchAddress("Fragd5Idx",FragdauIdx[4]);
  if(_fChain->GetBranch("Fragd6Lund"))_fChain->SetBranchAddress("Fragd6Lund",FragdauLund[5]);
  if(_fChain->GetBranch("Fragd6Idx"))_fChain->SetBranchAddress("Fragd6Idx",FragdauIdx[5]);
  if(_fChain->GetBranch("Fragd7Lund"))_fChain->SetBranchAddress("Fragd7Lund",FragdauLund[6]);
  if(_fChain->GetBranch("Fragd7Idx"))_fChain->SetBranchAddress("Fragd7Idx",FragdauIdx[6]);
  if(_fChain->GetBranch("Fragd8Lund"))_fChain->SetBranchAddress("Fragd8Lund",FragdauLund[7]);
  if(_fChain->GetBranch("Fragd8Idx"))_fChain->SetBranchAddress("Fragd8Idx",FragdauIdx[7]);
  if(_fChain->GetBranch("Fragd9Lund"))_fChain->SetBranchAddress("Fragd9Lund",FragdauLund[8]);
  if(_fChain->GetBranch("Fragd9Idx"))_fChain->SetBranchAddress("Fragd9Idx",FragdauIdx[8]);
  if(_fChain->GetBranch("Fragd10Lund"))_fChain->SetBranchAddress("Fragd10Lund",FragdauLund[9]);
  if(_fChain->GetBranch("Fragd10Idx"))_fChain->SetBranchAddress("Fragd10Idx",FragdauIdx[9]);
  if(_fChain->GetBranch("Fragd11Lund"))_fChain->SetBranchAddress("Fragd11Lund",FragdauLund[10]);
  if(_fChain->GetBranch("Fragd11Idx"))_fChain->SetBranchAddress("Fragd11Idx",FragdauIdx[10]);
  if(_fChain->GetBranch("Fragd12Lund"))_fChain->SetBranchAddress("Fragd12Lund",FragdauLund[11]);
  if(_fChain->GetBranch("Fragd12Idx"))_fChain->SetBranchAddress("Fragd12Idx",FragdauIdx[11]);
  if(_fChain->GetBranch("Fragd13Lund"))_fChain->SetBranchAddress("Fragd13Lund",FragdauLund[12]);
  if(_fChain->GetBranch("Fragd13Idx"))_fChain->SetBranchAddress("Fragd13Idx",FragdauIdx[12]);
  if(_fChain->GetBranch("Fragd14Lund"))_fChain->SetBranchAddress("Fragd14Lund",FragdauLund[13]);
  if(_fChain->GetBranch("Fragd14Idx"))_fChain->SetBranchAddress("Fragd14Idx",FragdauIdx[13]);
  if(_fChain->GetBranch("Fragd15Lund"))_fChain->SetBranchAddress("Fragd15Lund",FragdauLund[14]);
  if(_fChain->GetBranch("Fragd15Idx"))_fChain->SetBranchAddress("Fragd15Idx",FragdauIdx[14]);
  if(_fChain->GetBranch("Fragd16Lund"))_fChain->SetBranchAddress("Fragd16Lund",FragdauLund[15]);
  if(_fChain->GetBranch("Fragd16Idx"))_fChain->SetBranchAddress("Fragd16Idx",FragdauIdx[15]);
  if(_fChain->GetBranch("Fragd17Lund"))_fChain->SetBranchAddress("Fragd17Lund",FragdauLund[16]);
  if(_fChain->GetBranch("Fragd17Idx"))_fChain->SetBranchAddress("Fragd17Idx",FragdauIdx[16]);
  if(_fChain->GetBranch("Fragd18Lund"))_fChain->SetBranchAddress("Fragd18Lund",FragdauLund[17]);
  if(_fChain->GetBranch("Fragd18Idx"))_fChain->SetBranchAddress("Fragd18Idx",FragdauIdx[17]);
  if(_fChain->GetBranch("Fragd19Lund"))_fChain->SetBranchAddress("Fragd19Lund",FragdauLund[18]);
  if(_fChain->GetBranch("Fragd19Idx"))_fChain->SetBranchAddress("Fragd19Idx",FragdauIdx[18]);
  if(_fChain->GetBranch("Fragd20Lund"))_fChain->SetBranchAddress("Fragd20Lund",FragdauLund[19]);
  if(_fChain->GetBranch("Fragd20Idx"))_fChain->SetBranchAddress("Fragd20Idx",FragdauIdx[19]);


  if(_fChain->GetBranch("nDsSt"))_fChain->SetBranchAddress("nDsSt",&nDsSt);
  if(_fChain->GetBranch("DsStMass"))_fChain->SetBranchAddress("DsStMass",DsStMass);
  if(_fChain->GetBranch("DsStp3CM"))_fChain->SetBranchAddress("DsStp3CM",DsStp3CM);
  if(_fChain->GetBranch("DsStcosthCM"))_fChain->SetBranchAddress("DsStcosthCM",DsStcosthCM);
  if(_fChain->GetBranch("DsStphiCM"))_fChain->SetBranchAddress("DsStphiCM",DsStphiCM);
  if(_fChain->GetBranch("DsStenergyCM"))_fChain->SetBranchAddress("DsStenergyCM",DsStenergyCM);
  if(_fChain->GetBranch("DsStLund"))_fChain->SetBranchAddress("DsStLund",DsStLund); 
  if(_fChain->GetBranch("DsStd1Lund"))_fChain->SetBranchAddress("DsStd1Lund",DsStd1Lund);
  if(_fChain->GetBranch("DsStd1Idx"))_fChain->SetBranchAddress("DsStd1Idx",DsStd1Idx);
  if(_fChain->GetBranch("DsStd2Lund"))_fChain->SetBranchAddress("DsStd2Lund",DsStd2Lund);
  if(_fChain->GetBranch("DsStd2Idx"))_fChain->SetBranchAddress("DsStd2Idx",DsStd2Idx);

  if(_fChain->GetBranch("nDs"))_fChain->SetBranchAddress("nDs",&nDs);
  if(_fChain->GetBranch("DsMass"))_fChain->SetBranchAddress("DsMass",DsMass); 
  if(_fChain->GetBranch("Dsp3CM"))_fChain->SetBranchAddress("Dsp3CM",Dsp3CM);
  if(_fChain->GetBranch("DscosthCM"))_fChain->SetBranchAddress("DscosthCM",DscosthCM);
  if(_fChain->GetBranch("DsphiCM"))_fChain->SetBranchAddress("DsphiCM",DsphiCM);
  if(_fChain->GetBranch("DsenergyCM"))_fChain->SetBranchAddress("DsenergyCM",DsenergyCM);
  if(_fChain->GetBranch("DsLund"))_fChain->SetBranchAddress("DsLund",DsLund); 



  if(_fChain->GetBranch("nGam"))_fChain->SetBranchAddress("nGam",&nGam); 
  if(_fChain->GetBranch("Gamp3CM"))_fChain->SetBranchAddress("Gamp3CM",Gamp3CM);
  if(_fChain->GetBranch("GamcosthCM"))_fChain->SetBranchAddress("GamcosthCM",GamcosthCM);
  if(_fChain->GetBranch("GamphiCM"))_fChain->SetBranchAddress("GamphiCM",GamphiCM);
  if(_fChain->GetBranch("GamenergyCM"))_fChain->SetBranchAddress("GamenergyCM",GamenergyCM);
  if(_fChain->GetBranch("GamLund"))_fChain->SetBranchAddress("GamLund",GamLund);


  cout<<"Done linking DsInclusive Chain Branches"<<endl;
 
  return SetTagBranches();
}



Int_t DsInclusiveAnalysis::MakeDsInclusiveReducedNtupleBranches(TTree* Ntuple){
 
  Ntuple->Branch("evtmass",&evtmass,"evtmass/F");
  Ntuple->Branch("evtpstar",&evtpstar,"evtpstar/F");
  Ntuple->Branch("evtenergy",&evtenergy,"evtenergy/F");
  Ntuple->Branch("evtpx",&evtpx,"evtpx/F");
  Ntuple->Branch("evtpy",&evtpy,"evtpy/F");
  Ntuple->Branch("evtpz",&evtpz,"evtpz/F");
  Ntuple->Branch("evtcmenergy",&evtcmenergy,"evtcmenergy/F");
  Ntuple->Branch("evtcosstar",&evtcosstar,"evtcosstar/F");
  Ntuple->Branch("evtphistar",&evtphistar,"evtphistar/F");
  Ntuple->Branch("evtcharge",&evtcharge,"evtcharge/I");
  Ntuple->Branch("nEvt",&nEvt,"nEvt/I");
  Ntuple->Branch("EvtIdx",&EvtIdx,"EvtIdx/I");
  Ntuple->Branch("evtbestcand",&evtbestcand,"evtbestcand/I");
  Ntuple->Branch("evtmctrue",&evtmctrue,"evtmctrue/I"); 
  Ntuple->Branch("evtvtxprob",&evtvtxprob,"evtvtxprob/F");
  Ntuple->Branch("evtvtxx",&evtvtxx,"evtvtxx/F");
  Ntuple->Branch("evtvtxy",&evtvtxy,"evtvtxy/F");
  Ntuple->Branch("evtvtxz",&evtvtxz,"evtvtxz/F");

  Ntuple->Branch("tagsidemass",&tagsidemass,"tagsidemass/F");  
  Ntuple->Branch("tagsidepstar",&tagsidepstar,"tagsidepstar/F"); 
  Ntuple->Branch("tagsidep",&tagsidep,"tagsidep/F");  
  Ntuple->Branch("tagsideenergy",&tagsideenergy,"tagsideenergy/F");
  Ntuple->Branch("tagsideenergycm",&tagsideenergycm,"tagsideenergycm/F"); 
  Ntuple->Branch("tagsidedeltae",&tagsidedeltae,"tagsidedeltae/F");  
  Ntuple->Branch("tagsidecosthcm",&tagsidecosthcm,"tagsidecosthcm/F");
  Ntuple->Branch("tagqual",&tagqual,"tagqual/I");
 
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
  Ntuple->Branch("dsangle",&dsangle,"dsangle/F"); 
  Ntuple->Branch("dspxdiff",&dspxdiff,"dspxdiff/F");
  Ntuple->Branch("dspydiff",&dspydiff,"dspydiff/F");
  Ntuple->Branch("dspzdiff",&dspzdiff,"dspzdiff/F");
  Ntuple->Branch("dspdiff",&dspdiff,"dspdiff/F");
  Ntuple->Branch("dsuncmass",&dsuncmass,"dsuncmass/F");
  Ntuple->Branch("dsunfitmass",&dsunfitmass,"dsunfitmass/F");
  Ntuple->Branch("dscharge",&dscharge,"dscharge/I");

  Ntuple->Branch("dsstp",&dsstp,"dsstp/F"); 
  Ntuple->Branch("dsstpstar",&dsstpstar,"dsstpstar/F"); 
  Ntuple->Branch("dsstenergycm",&dsstenergycm,"dsstenergycm/F"); 
  Ntuple->Branch("dsstmass",&dsstmass,"dsstmass/F"); 
  Ntuple->Branch("dsstuncmass",&dsstuncmass,"dsstuncmass/F"); 
  Ntuple->Branch("dsstdeltam",&dsstdeltam,"dssdeltam/F"); 
  Ntuple->Branch("dsstangle",&dsstangle,"dsstangle/F"); 
  Ntuple->Branch("dsstdecayangle",&dsstdecayangle,"dsstdecayangle/F");
  Ntuple->Branch("dsstcorrpdiff",&dsstcorrpdiff,"dsstcorrpdiff/F");
  Ntuple->Branch("dsstfitpdiff",&dsstfitpdiff,"dsstfitpdiff/F");

  Ntuple->Branch("gamenergy",&gamenergy,"gamenergy/F");
  Ntuple->Branch("gamenergycm",&gamenergycm,"gamenergycm/F");
  Ntuple->Branch("gamangle",&gamangle,"gamangle/F");
  Ntuple->Branch("gamdsstangle",&gamdsstangle,"gamdsstangle/F");
  Ntuple->Branch("gammctrue",&gammctrue,"gammctrue/I");

  Ntuple->Branch("Kecm",&Kecm,"Kecm/F");
  Ntuple->Branch("Kangle",&Kangle,"Kangle/F");
  Ntuple->Branch("Kmctrue",&Kmctrue,"Kmctrue/I"); 
  Ntuple->Branch("Klund",&Klund,"Klund/I");
  Ntuple->Branch("Kqual",&Kqual,"Kqual/I");

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
  Ntuple->Branch("fragexgame",&fragexgame,"fragexgame/F");
  Ntuple->Branch("fragexgamn",&fragexgamn,"fragexgamn/I");
  Ntuple->Branch("fragqual",&fragqual,"fragqual/I");
  Ntuple->Branch("fragnpartgen",&fragnpartgen,"fragnpartgen/I");
  Ntuple->Branch("fragnpigen",&fragnpigen,"fragnpigen/I");
  Ntuple->Branch("fragnpi0gen",&fragnpi0gen,"fragnpi0gen/I");
  Ntuple->Branch("fragngamgen",&fragngamgen,"fragngamgen/I");
    
  MakeTagReducedNtupleBranches(Ntuple);
  cout<<"Done Making  DsInclusive  Reduced Ntuple Branches"<<endl;
  return 1;
}



Int_t DsInclusiveAnalysis::SetDsInclusiveReducedNtupleBranches(){
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
  cout<<"Done linking DsInclusive ReducedNtuple Branches"<<endl;
  return 1;
}


Int_t DsInclusiveAnalysis::ReduceNtupleGen(Long_t MaxNumberEvents){
  cout<<"Going to reduce the ntuple"<<endl;
  
  if(_fChain==NULL){
    cout<<"No chain."<<endl;
    return 0;
  } 

  ///link the branches in the input chain
  SetDsInclusiveBranches();


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
  Float_t taggenmass;Ntuple.Branch("taggenmass",&taggenmass,"taggenmass/F");
  Float_t taggenpstar;Ntuple.Branch("taggenpstar",&taggenpstar,"taggenpstar/F");
  Float_t gamangle;Ntuple.Branch("gamangle",&gamangle,"gamangle/F");
  Float_t dsgenpstar;Ntuple.Branch("dsgenpstar",&dsgenpstar,"dsgenpstar/F");
  Float_t dsangle;Ntuple.Branch("dsangle",&dsangle,"dsangle/F");
  Float_t dstrkangle;Ntuple.Branch("dstrkangle",&dstrkangle,"dstrkangle/F");
  Int_t dsntrks;Ntuple.Branch("dsntrks",&dsntrks,"dsntrks/I");
  Int_t fragnpartgen;Ntuple.Branch("fragnpartgen",&fragnpartgen,"fragnpartgen/I");
  Int_t fragnpigen;Ntuple.Branch("fragnpigen",&fragnpigen,"fragnpigen/I");
  Int_t fragnpi0gen;Ntuple.Branch("fragnpi0gen",&fragnpi0gen,"fragnpi0gen/I");
  Int_t fragngamgen;Ntuple.Branch("fragngamgen",&fragngamgen,"fragngamgen/I");
  //Counters
  Int_t MCBCounterTotal=0;


  cout<<"Total Entries in Chain="<<_fChain->GetEntries()<<endl;
  cout<<"Going to start the event loop for "<<MaxNumberEvents<<" events"<<endl;
  eventid=0;
  while(_fChain->GetEntry(eventid,0)>0&&eventid<MaxNumberEvents){   
    eventid++;
    
    if(eventid%1000==0){
      cout<<eventid<<" Events done.   With "<<MCBCounterTotal<<" Candidates saved."<<endl;    
    }

    if(mcLen>=MAXMCLEN)continue;

    
    //find the tag-D Idx
    GenTagIdx=-1;
    for(Int_t mcc=0;mcc<mcLen;mcc++){
      if(abs(mcLund[mcc])==myD0Lund
	 ||abs(mcLund[mcc])==myDPlusLund){
	GenTagIdx=mcc;break;
      }
    }
    if(GenTagIdx==-1||GenTagIdx>=MAXMCLEN)continue;
    taggenmass=mcmass[GenTagIdx];
    taggenpstar=mcp3CM[GenTagIdx];
    TVector3 Tagp3Vec(mcp3CM[GenTagIdx]*sin(acos(mccosthCM[GenTagIdx]))*cos(mcphiCM[GenTagIdx]),
		      mcp3CM[GenTagIdx]*sin(acos(mccosthCM[GenTagIdx]))*sin(mcphiCM[GenTagIdx]),
		      mcp3CM[GenTagIdx]*mccosthCM[GenTagIdx]);
    
    //find the Ds Idx    
    DsStGenMCIdx=-1;
    for(Int_t mcc=0;mcc<mcLen;mcc++){
      if(abs(mcLund[mcc])==myDsStLund){
	DsStGenMCIdx=mcc;break;
      }      
    }
    if(DsStGenMCIdx==-1||DsStGenMCIdx>=MAXMCLEN)continue;

    //compute angle of the gam to Tag
    GamGenMCIdx=dauIdx[DsStGenMCIdx]+1;
    if(GamGenMCIdx<0||GamGenMCIdx>=MAXMCLEN)continue;
    TVector3 Gamp3Vec(mcp3CM[GamGenMCIdx]*sin(acos(mccosthCM[GamGenMCIdx]))*cos(mcphiCM[GamGenMCIdx]),
		      mcp3CM[GamGenMCIdx]*sin(acos(mccosthCM[GamGenMCIdx]))*sin(mcphiCM[GamGenMCIdx]),
		      mcp3CM[GamGenMCIdx]*mccosthCM[GamGenMCIdx]);

    gamangle=Gamp3Vec.Unit()*Tagp3Vec.Unit();

    //compute angle of the Ds to Tag
    DsGenMCIdx=dauIdx[DsStGenMCIdx];
    if(DsGenMCIdx<0||DsGenMCIdx>=MAXMCLEN)continue;
    dsgenpstar=mcp3CM[DsGenMCIdx];
    TVector3 Dsp3Vec(mcp3CM[DsGenMCIdx]*sin(acos(mccosthCM[DsGenMCIdx]))*cos(mcphiCM[DsGenMCIdx]),
		     mcp3CM[DsGenMCIdx]*sin(acos(mccosthCM[DsGenMCIdx]))*sin(mcphiCM[DsGenMCIdx]),
		     mcp3CM[DsGenMCIdx]*mccosthCM[DsGenMCIdx]);

    dsangle=Dsp3Vec.Unit()*Tagp3Vec.Unit();
    dsntrks=dauLen[DsGenMCIdx];
    //find signal track closest to Tag
    dstrkangle=-1.;//set to opposite side of Tag
    for(Int_t dsdauid=dauIdx[DsGenMCIdx];dsdauid<=dauIdx[DsGenMCIdx]+dauLen[DsGenMCIdx]-1;dsdauid++){
      TVector3 Dsdaup3Vec(mcp3CM[dsdauid]*sin(acos(mccosthCM[dsdauid]))*cos(mcphiCM[dsdauid]),
			  mcp3CM[dsdauid]*sin(acos(mccosthCM[dsdauid]))*sin(mcphiCM[dsdauid]),
			  mcp3CM[dsdauid]*mccosthCM[dsdauid]);
      if(dauLen[dsdauid]>=2){
	for(Int_t dsdaudauid=dauIdx[dsdauid];dsdaudauid<=dauIdx[dsdauid]+dauLen[dsdauid]-1;dsdaudauid++){
	  TVector3 Dsdaudaup3Vec(mcp3CM[dsdaudauid]*sin(acos(mccosthCM[dsdaudauid]))*cos(mcphiCM[dsdaudauid]),
				 mcp3CM[dsdaudauid]*sin(acos(mccosthCM[dsdaudauid]))*sin(mcphiCM[dsdaudauid]),
				 mcp3CM[dsdaudauid]*mccosthCM[dsdaudauid]);
	  if(Dsdaudaup3Vec.Unit()*Tagp3Vec.Unit()>dstrkangle)
	    dstrkangle=Dsdaudaup3Vec.Unit()*Tagp3Vec.Unit();
	}
      }else{ 
	if(Dsdaup3Vec.Unit()*Tagp3Vec.Unit()>dstrkangle)
	  dstrkangle=Dsdaup3Vec.Unit()*Tagp3Vec.Unit();
      }
    }
    
    //find the beams 
    Int_t beameplusIdx=-1;
    Int_t beameminusIdx=-1;
    for(Int_t mcc=0;mcc<5;mcc++){
      if(mcLund[mcc]==11)beameplusIdx=mcc;
      if(mcLund[mcc]==-11)beameminusIdx=mcc;
    }
    if(beameplusIdx==-1||beameminusIdx==-1){cout<<"no beams"<<endl;continue;}

    fragnpartgen=0;
    fragnpigen=0;
    fragnpi0gen=0;
    fragngamgen=0;
    for(Int_t mcc=2;mcc<mcLen;mcc++){
      if(abs(mcLund[mcc])==myPiLund)
	if(// abs(mcLund[mothIdx[mcc]])!=myD0Lund
// 	   &&abs(mcLund[mothIdx[mothIdx[mcc]]])!=myD0Lund
// 	   &&abs(mcLund[mothIdx[mothIdx[mothIdx[mcc]]]])!=myD0Lund
// 	   &&abs(mcLund[mothIdx[mcc]])!=myDPlusLund
// 	   &&abs(mcLund[mothIdx[mothIdx[mcc]]])!=myDPlusLund
// 	   &&abs(mcLund[mothIdx[mothIdx[mothIdx[mcc]]]])!=myDPlusLund
// 	   &&abs(mcLund[mothIdx[mcc]])!=myDsLund
// 	   &&abs(mcLund[mothIdx[mothIdx[mcc]]])!=myDsLund
// 	   &&abs(mcLund[mothIdx[mothIdx[mothIdx[mcc]]]])!=myDsLund
	   mothIdx[mcc]==2
	   ){
	  fragnpartgen++;      
	  fragnpigen++;
	}

      if(mcLund[mcc]==myPi0Lund)
	if(// abs(mcLund[mothIdx[mcc]])!=myD0Lund
// 	   &&abs(mcLund[mothIdx[mothIdx[mcc]]])!=myD0Lund
// 	   &&abs(mcLund[mothIdx[mothIdx[mothIdx[mcc]]]])!=myD0Lund
// 	   &&abs(mcLund[mothIdx[mcc]])!=myDPlusLund
// 	   &&abs(mcLund[mothIdx[mothIdx[mcc]]])!=myDPlusLund
// 	   &&abs(mcLund[mothIdx[mothIdx[mothIdx[mcc]]]])!=myDPlusLund
// 	   &&abs(mcLund[mothIdx[mcc]])!=myDsLund
// 	   &&abs(mcLund[mothIdx[mothIdx[mcc]]])!=myDsLund
// 	   &&abs(mcLund[mothIdx[mothIdx[mothIdx[mcc]]]])!=myDsLund
	   mothIdx[mcc]==2
	   ){
	  fragnpartgen++;      
	  fragnpi0gen++;
	}

      if(mcLund[mcc]==myGammaLund&&mcp3[mcc]>.05)
	if(// mcLund[mothIdx[mcc]]==beameplusIdx
	   // 	   ||mcLund[mothIdx[mcc]]==beameminusIdx
	   mothIdx[mcc]==2
	   ){
	  fragnpartgen++;      
	  fragngamgen++;
	}
    }
	


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





