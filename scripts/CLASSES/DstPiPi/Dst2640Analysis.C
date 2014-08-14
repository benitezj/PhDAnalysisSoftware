#include "Dst2640Analysis.h"

//these kind files  might be strange:
//They contain several methods of the class in order to keep size of files small. 
#include "Dst2640AnalysisDrawPlots.C"
#include "Dst2640AnalysisDrawCutSummary.C"
#include "Dst2640AnalysisDrawTruthRatios.C"
//#include "Dst2640AnalysisFitResolution.C"
//#include "Dst2640AnalysisFitMassPeak.C"
//#include "Dst2640AnalysisDrawBkgPlot.C"
#include "Dst2640AnalysisDrawDalitzPlots.C"
#include "Dst2640AnalysisDrawMassPlots.C"
//#include "Dst2640AnalysisDrawBkgSubMass.C"
#include "Dst2640AnalysisDrawDstKs.C"


ClassImp(Dst2640Analysis);
Dst2640Analysis::Dst2640Analysis(TChain* fChain,Bool_t truthMatch,Int_t MatterOrAntiMatter, TString Mode,TString OutputDir,Int_t WhichCuts,Bool_t RunInTestMode):
  _fChain(fChain),
  _MatterOrAntiMatter(MatterOrAntiMatter),
  _Mode(Mode),							   
  _OutputDir(OutputDir),
  TruthMatch(truthMatch),
  _WhichCuts(WhichCuts),
  OutPutFile(NULL),
  ReducedNtuple(NULL),
  MCDst2640CounterPerEvent(0),
  MCDst2640CounterTotal(0),
  RecoDst2640CounterTotal(0),
  Dst2640p3CMmax(5.),
  Dst2640p3CMmin(0),
  Dst2640p3CMNbins(50),
  Dst2640MassWindow(.03),
  Dst2640PDGDeltaM(.630),
  Dst2640DeltaMNbins(100),
  Dst2640Cosstarmin(-1.0001),
  Dst2640Cosstarmax(1.0001),
  Dst2640CosstarNbins(20),
  NDalitzPlotBins(80),
  DalitzMin(4.5),
  DalitzMax(6.5),
  _RunInTestMode(RunInTestMode)
{
  if(_fChain==NULL){cout<<"No Chain"<<endl;}

  //
  ModeDir="/u2/benitezj/physics/NTUPLES/Dst2640ToDstPiPi/"+_Mode;

  //init the database
  database.SetMode(_Mode);
  database.SetTruthMatch(truthMatch);
  database.SetCuts(_WhichCuts);

 
  ////Initialize the cuts
  Dst2640MCTrueCut.Init("Dst2640MCTrueCut",&dst2640mctrue,1,1,1);
  Dst2640DeltaMCut.Init("Dst2640DeltaMCut",&dst2640deltam,database.GetDst2640DeltaMMin(),database.GetDst2640DeltaMMax(),1);
  D0pstarCut.Init("D0pstarCut",&d0pstar,database.GetD0pstarMin(),100,1);
  DstarDeltaMCut.Init("DstarDeltaMCut",&dstardeltam,database.GetDstarDeltaMMin(),database.GetDstarDeltaMMax(),1);
  KLHCut.Init("KLHCut",&klh,database.GetKLHBit(),100,1);
  PiLHCut.Init("PiLHCut",&pilh,database.GetPiLHBit(),100,1);  
  D0MassCut.Init("D0MassCut",&d0mass,database.GetD0MassMin(),database.GetD0MassMax(),1);
  D0ProbCut.Init("D0ProbCut",&d0logvtxprob,database.GetD0MinLogProbab(),.000001,1);
  Dst2640ProbCut.Init("Dst2640ProbCut",&dst2640logvtxprob,database.GetDst2640MinLogProbab(),.00001,1);
  Pi1LHCut.Init("Pi1LHCut",&pi1lh,2,100,1);//very loose LH cuts
  Pi2LHCut.Init("Pi2LHCut",&pi2lh,2,100,1);
  Pi1Pi2ChargeCut.Init("Pi1Pi2ChargeCut",&pi1pluspi2charge,0,0,1);
  D0Pi2LHCut.Init("D0Pi2LHCut",&pi1lh,2,100,1);
  D0Pi3LHCut.Init("D0Pi3LHCut",&pi2lh,2,100,1);
  Dst2640pstarCut.Init("Dst2640pstarLoCut",&dst2640pstar,database.GetDst2640pstarMin(),database.GetDst2640pstarMax(),1);
  HelicityCut.Init("HelicityCut",&dstarcostheta,-1.00001,database.GetDstarCosineMax(),1);



  //list of all cuts applied 
  Dst2640Cuts.AddCut(&Dst2640MCTrueCut);
  Dst2640Cuts.AddCut(&Pi1Pi2ChargeCut);
  Dst2640Cuts.AddCut(&Dst2640DeltaMCut);
  Dst2640Cuts.AddCut(&D0pstarCut);
  Dst2640Cuts.AddCut(&DstarDeltaMCut); 
  Dst2640Cuts.AddCut(&KLHCut);
  Dst2640Cuts.AddCut(&PiLHCut);
  Dst2640Cuts.AddCut(&D0MassCut);
  Dst2640Cuts.AddCut(&D0ProbCut);
  Dst2640Cuts.AddCut(&Dst2640ProbCut);
  Dst2640Cuts.AddCut(&Pi1LHCut);
  Dst2640Cuts.AddCut(&Pi2LHCut);
  Dst2640Cuts.AddCut(&Dst2640pstarCut);  
  Dst2640Cuts.AddCut(&HelicityCut);
  if(_Mode=="D0ToK3Pi"){
    Dst2640Cuts.AddCut(&D0Pi2LHCut);
    Dst2640Cuts.AddCut(&D0Pi3LHCut);
  } 

 

  cutline.SetLineColor(2);
  cutline.SetLineWidth(1);
  
  cutbox.SetLineColor(2);
  cutbox.SetLineWidth(2);
  cutbox.SetFillStyle(0);
 
  sprintf(DstarPiPiXLabel,"m(D*#pi#pi)-m(D*)+m_{D*} (GeV/c^{2})");
  sprintf(DstarPiPiYLabel,"Entries / 5 MeV");

   
  cout<<"Done Initializing the analysis"<<endl;
}

Dst2640Analysis::~Dst2640Analysis(){  
  delete Fccbar;
}


Int_t Dst2640Analysis::ReduceNtuple(){
  cout<<"Going to reduce the ntuple"<<endl;

  if(_fChain==NULL){
    cout<<"No chain."<<endl;
    return 0;
  } 


  ///set the branches to be read on tree 
  SetBranches();


  //the file must be created here so the 'new' histos and Tree is created in hard drive 
  OutPutFile=new TFile(_OutputDir+"/Dst2640Reduced.root","recreate",_OutputDir+"/Dst2640Reduced.root",4);
  if(OutPutFile==NULL){cout<<"Failed to create file"<<endl;return 0;}


  //create new reduced ntuple
  ReducedNtuple=new TTree("Dst2640Ntuple","Dst2640 Reduced Ntuple",99);        
 
  //create the branches in the ReducedNtuple
  MakeReducedNtupleBranches();


  if(TruthMatch){
    //create the MC Histos
    HMCDst2640Mass=new TH1F();
    SetHistoXY(HMCDst2640Mass,"HMCDst2640Mass",100,Dst2640PDGMass-Dst2640MassWindow,Dst2640PDGMass+Dst2640MassWindow,
	       "Dst2640 Cand. Mass (GeV/c^{2})","Entries/1MeV");
  
    HMCDst2640DeltaM=new TH1F();
    SetHistoXY(HMCDst2640DeltaM,"HMCDst2640DeltaM",database.GetDst2640DeltaMNbins(),database.GetDst2640DeltaMMin(),database.GetDst2640DeltaMMax(),"D*(2640) Mass - D*(2010) (GeV/c^{2})","");

    HMCDst2640p3CM=new TH1F();
    SetHistoXY(HMCDst2640p3CM,"HMCDst2640p3CM",Dst2640p3CMNbins,Dst2640p3CMmin,Dst2640p3CMmax,
	       "p* (GeV/c)","Entries/100MeV");
  
    HMCDst2640costhCM=new TH1F();
    SetHistoXY(HMCDst2640costhCM,"HMCDst2640costhCM",Dst2640CosstarNbins,Dst2640Cosstarmin,Dst2640Cosstarmax,
	       "cos(#theta)*","Entries/.04");
  
    H2MCDst2640CMPvsTheta=new TH2F();
    SetHisto2D(H2MCDst2640CMPvsTheta,"H2MCDst2640CMPvsTheta",Dst2640p3CMNbins,Dst2640p3CMmin,Dst2640p3CMmax,
	       "p* (GeV/c)",Dst2640CosstarNbins,Dst2640Cosstarmin,Dst2640Cosstarmax,"cos(#theta*)","entries/(.04x100MeV)");
  
    H2MCDst2640DalitzPlot=new TH2F();
    SetHisto2D(H2MCDst2640DalitzPlot,"H2MCDst2640DalitzPlot",NDalitzPlotBins,DalitzMin,DalitzMax,"m^{2}(D*#pi^{-})  (GeV^{2}/c^{4})",NDalitzPlotBins,DalitzMin,DalitzMax,"m^{2}(D*#pi^{+})  (GeV^{2}/c^{4})","");
    

    HMCNDst2640=new TH1F();
    SetHistoXY(HMCNDst2640,"HMCNDst2640",11,-.5,10.5,
	       "nDst2640/event","Counts");

  
    HLundCheck=new TH1F();
    SetHisto(HLundCheck,"HLundCheck",15,-.5,14.5,
	     "ParticleLund - LundExp");
  }
  //////


  //Start the event loop;
  Long_t MaxNumberEvents=1000000000;
  if(_RunInTestMode)MaxNumberEvents=5000;
  
  Int_t RemovedCands=0;
  Int_t InitialdCands=0;

  Int_t eventid=0;
  while(_fChain->GetEntry(eventid,0)>0&&eventid<MaxNumberEvents){   
    eventid++;
     
    if(eventid%5000==0)cout<<eventid<<" Events done.   With "<<RecoDst2640CounterTotal<<" Candidates saved."<<endl;    
           
    InitialdCands+=nDst2640;
    if(nDst2640>MAXNCANDS){
      cout<<"Too many cands at event "<<eventid<<" Only first "<<MAXNCANDS<<" of "<<nDst2640<<" will be used"<<endl;
      RemovedCands+=nDst2640-MAXNCANDS; 
      nDst2640=MAXNCANDS;      
    }

    ///Loop over the reconstructed
    RecoDst2640CounterPerEvent=0;      
    
    //------beam quantities
    TVector3 beamVtx(primVtxX,primVtxY,primVtxZ);


    for(Dst2640Idx=0;Dst2640Idx<nDst2640;Dst2640Idx++){

      ////////////cut out the unconverged fits
      if(Dst2640VtxStatus[Dst2640Idx]!=0)continue;               


      DstarIdx=Dst2640d1Idx[Dst2640Idx];
      SlowPiIdx=Dstard2Idx[DstarIdx];
      D0Idx=Dstard1Idx[DstarIdx];
      PiIdx=D0d2Idx[D0Idx];
      KIdx=D0d1Idx[D0Idx];
      Pi1Idx=Dst2640d2Idx[Dst2640Idx];
      Pi2Idx=Dst2640d3Idx[Dst2640Idx]; 
      if(_Mode=="D0ToK3Pi"){
	D0Pi2Idx=D0d3Idx[D0Idx];
	D0Pi3Idx=D0d4Idx[D0Idx];
      }
      
      PitrkIdx=PiTrkIdx[PiIdx];
      KtrkIdx=KTrkIdx[KIdx];
      SlowPitrkIdx=PiTrkIdx[SlowPiIdx];
      Pi1trkIdx=PiTrkIdx[Pi1Idx];
      Pi2trkIdx=PiTrkIdx[Pi2Idx];
      if(_Mode=="D0ToK3Pi"){
	D0Pi2trkIdx=PiTrkIdx[D0Pi2Idx];
	D0Pi3trkIdx=PiTrkIdx[D0Pi3Idx];
      }

 

      //Check that Im using proper indexes
      if(TruthMatch){
	if(Dstard1Lund[DstarIdx]!=D0Lund[D0Idx])HLundCheck->Fill(1);
	if(Dstard2Lund[DstarIdx]!=PiLund[SlowPiIdx])HLundCheck->Fill(2);
	if(D0d1Lund[D0Idx]!=KLund[KIdx])HLundCheck->Fill(3);
	if(D0d2Lund[D0Idx]!=PiLund[PiIdx])HLundCheck->Fill(4);
	if(TRKLund[SlowPitrkIdx]!=PiLund[SlowPiIdx])HLundCheck->Fill(5);
	if(TRKLund[PitrkIdx]!=PiLund[PiIdx])HLundCheck->Fill(6);
	if(Dst2640d1Lund[Dst2640Idx]!=DstarLund[DstarIdx])HLundCheck->Fill(7);
	if(TRKLund[Pi1trkIdx]!=PiLund[Pi1Idx])HLundCheck->Fill(9);
	if(TRKLund[Pi2trkIdx]!=PiLund[Pi2Idx])HLundCheck->Fill(10);
	if(Dst2640d2Lund[Dst2640Idx]!=PiLund[Pi1Idx])HLundCheck->Fill(11);
	if(Dst2640d3Lund[Dst2640Idx]!=PiLund[Pi2Idx])HLundCheck->Fill(12);
	//if(TRKLund[KtrkIdx]!=KLund[KIdx])HLundCheck->Fill(13);//TRKLund is always Pion

	if(_Mode=="D0ToK3Pi"){
	  if(D0d3Lund[D0Idx]!=PiLund[D0Pi2Idx])HLundCheck->Fill(13);
	  if(D0d4Lund[D0Idx]!=PiLund[D0Pi3Idx])HLundCheck->Fill(14);
	  if(TRKLund[D0Pi2trkIdx]!=PiLund[D0Pi2Idx])HLundCheck->Fill(15);
	  if(TRKLund[D0Pi3trkIdx]!=PiLund[D0Pi3Idx])HLundCheck->Fill(16);
	}
      }

      //////fill reduced ntuple variables    
      dst2640mass=Dst2640Mass[Dst2640Idx];
      dst2640pstar=Dst2640p3CM[Dst2640Idx];
      dst2640cosstar=Dst2640costhCM[Dst2640Idx];
      dst2640deltam=Dst2640Mass[Dst2640Idx]-DstarMass[DstarIdx];  
      if(TMath::Prob(Dst2640Chi2[Dst2640Idx],Dst2640nDof[Dst2640Idx])>0)
	dst2640logvtxprob=log10(TMath::Prob(Dst2640Chi2[Dst2640Idx],Dst2640nDof[Dst2640Idx]));
      else dst2640logvtxprob=-100;
      dst2640charge=Dst2640Lund[Dst2640Idx]/abs(Dst2640Lund[Dst2640Idx]);
      dst2640nDst2640=nDst2640;     

      //-------Dstar quantities
      dstarmass=DstarMass[DstarIdx];
      dstardeltam=DstarMass[DstarIdx]-D0Mass[D0Idx];
      if(DstarVtxStatus[DstarIdx]==0) dstarlogvtxprob=log10(TMath::Prob(DstarChi2[DstarIdx],DstarnDof[DstarIdx]));   
      else dstarlogvtxprob=0;
      dstarcostheta= ComputeDstarAngle();
      dstarpstar=Dstarp3CM[DstarIdx];

      //-------D0 quantities
      d0mass=D0Mass[D0Idx];
      //if(D0VtxStatus[D0Idx]==0)
      if(TMath::Prob(D0Chi2[D0Idx],D0nDof[D0Idx])>0) d0logvtxprob = log10(TMath::Prob(D0Chi2[D0Idx],D0nDof[D0Idx]));
      else d0logvtxprob=0;
      d0pstar=D0p3CM[D0Idx];
      d0mode=0;
      if(D0Pi2Idx==-1&&D0Pi3Idx==-1) d0mode=1;
      if(D0Pi2Idx>=0&&D0Pi3Idx>=0) d0mode=2;
      if(d0mode==0)cout<<"d0mode=0"<<endl;

      //track quantities
      kp3=Kp3[KIdx];
      pip3=Pip3[PiIdx];
      slowpip3=Pip3[SlowPiIdx];
      pi1p3=Pip3[Pi1Idx];
      pi2p3=Pip3[Pi2Idx];
      pi1charge=PiLund[Pi1Idx]/abs(PiLund[Pi1Idx]);
      pi2charge=PiLund[Pi2Idx]/abs(PiLund[Pi2Idx]);
      pi1pluspi2charge=pi1charge+pi2charge;
      
      //other
      msqdstarpiplus=ComputeMsqDstarPi(Pi1Idx);
      msqdstarpiminus=ComputeMsqDstarPi(Pi2Idx);
      pipimass=ComputePiPiMass();
      dstarpipicosine=ComputeDstarPiPiCosine();      
      pipicosine=ComputePiPiCosine();
      ComputeDstarPiMass(&mdstarpirs,&mdstarpiws);
      dstarpideltam=mdstarpirs-dstarmass;
      dstarpideltamws=mdstarpiws-dstarmass;
      if(nDst2640==2&&Dst2640Idx==0)ComputeDstarPiPiMass(&mdstarpipirs,&mdstarpipiws);
      else{mdstarpipirs=0;mdstarpipiws=0;}
      ComputeDstarPiCosHel(&dstarpicoshelrs,&dstarpicoshelws);
      ComputeDstarPipstar(&dstarpipstarrs,&dstarpipstarws);
	
      klh=14;  
   //    for(Int_t i=10;i<=14;i++)//10=notpion,11=veryloose,12=loose,13=tight,14=verytight
// 	if((KSelectorsMap[KtrkIdx] & (1<<i)) == (1<<i))	
// 	  klh=i;  
      
  
      pilh=5;pi1lh=5;pi2lh=5;  d0pi2lh=5; d0pi3lh=5; 
 //      for(Int_t i=2;i<=5;i++){//2=veryloose,3=loose,4=tight,5=verytight
// 	if((PiSelectorsMap[PitrkIdx] & (1<<i) ) == (1<<i))	
// 	  pilh=i;
// 	if((PiSelectorsMap[Pi1trkIdx] & (1<<i) ) == (1<<i))	
// 	  pi1lh=i;
// 	if((PiSelectorsMap[Pi2trkIdx] & (1<<i) ) == (1<<i))	
// 	  pi2lh=i;  

// 	if(_Mode=="D0ToK3Pi"){
// 	  if((PiSelectorsMap[D0Pi2trkIdx] & (1<<i) ) == (1<<i))	
// 	    d0pi2lh=i;  
// 	  if((PiSelectorsMap[D0Pi3trkIdx] & (1<<i) ) == (1<<i))	
// 	    d0pi3lh=i;  
// 	}

//       }
	

      dst2640mctrue=1;
      dstarmctrue=1;
      d0mctrue=1;
      pimctrue=1;
      slowpimctrue=1;
      kmctrue=1;
      pi1mctrue=1;
      pi2mctrue=1;     
      d0pi2mctrue=1;  
      d0pi3mctrue=1;      
      if(TruthMatch){
	dst2640mctrue=(Dst2640MCIdx[Dst2640Idx]>0);
	dstarmctrue=(DstarMCIdx[DstarIdx]>0);
	d0mctrue=(D0MCIdx[D0Idx]>0);
	pimctrue=(PiMCIdx[PiIdx]>0);
	slowpimctrue=(PiMCIdx[SlowPiIdx]>0);
	kmctrue=(KMCIdx[KIdx]>0);
	pi1mctrue=(PiMCIdx[Pi1Idx]>0);
	pi2mctrue=(PiMCIdx[Pi2Idx]>0);
	if(_Mode=="D0ToK3Pi"){
	  d0pi2mctrue=(PiMCIdx[D0Pi2Idx]>0);
	  d0pi3mctrue=(PiMCIdx[D0Pi3Idx]>0);
	}
      }

      if(TruthMatch&&dst2640mctrue)
	dst2640dmres=dst2640deltam-(mcmass[Dst2640MCIdx[Dst2640Idx]]-mcmass[DstarMCIdx[DstarIdx]]);
      else dst2640dmres=100000;
      
      eventnumber=eventid;
  
      
      ///Make some cuts due to much background  
      //if(!Dst2640Cuts.EvalCuts(1,100))continue;

      if(!(
	   //      KLHCut.EvalCut()&&
	   // 	   PiLHCut.EvalCut()&&
	   // 	   D0ProbCut.EvalCut()&&
	   D0MassCut.EvalCut()&&
	   // 	   D0pstarCut.EvalCut()&&   
	   DstarDeltaMCut.EvalCut()&&
	   // 	   Pi1LHCut.EvalCut()&&
	   // 	   Pi2LHCut.EvalCut()&&
	   Pi1Pi2ChargeCut.EvalCut()&&
	   Dst2640pstarCut.EvalCut()
	   ))continue;
 //      if(_Mode=="D0ToK3pi")
// 	if(!(
// 	     //D0Pi2LHCut.EvalCut()&&
// 	     //D0Pi3LHCut.EvalCut()
// 	))continue;
  

      ReducedNtuple->Fill();	              
     
          
      RecoDst2640CounterTotal++;
           
   
    }//Dst2640 loop    
   
    
    ///-------------------------------
    ///Fill the Generated Quantities
    ///-----------------------------
    if(TruthMatch){
      Float_t msqplus=0;
      Float_t msqminus=0;
      //now loop over the Generated MC
      MCDst2640CounterPerEvent=0; 
      Int_t mcid=-1;
      while(mcid<mcLen){
	mcid++;

	if(mcLund[mcid]==_MatterOrAntiMatter*myDst2640Lund){
	  MCDst2640CounterPerEvent++; 
	  MCDst2640CounterTotal++;
		
	  HMCDst2640Mass->Fill(mcmass[mcid]);
	  HMCDst2640p3CM->Fill(mcp3CM[mcid]); 
	  HMCDst2640costhCM->Fill(mccosthCM[mcid]); 	
	  H2MCDst2640CMPvsTheta->Fill(mcp3CM[mcid],mccosthCM[mcid]);

	  //calculate deltaM   //mcmass[dauIdx[mcid]]=first daughter; mcmass[dauIdx[mcid] + 1] = second daughter ...	    
	  HMCDst2640DeltaM->Fill(mcmass[mcid]-mcmass[dauIdx[mcid]]);


	  //calculate Dalitz plot:	  
	  TVector3 Dstarp3vec;Dstarp3vec.SetMagThetaPhi(mcp3CM[dauIdx[mcid]],acos(mccosthCM[dauIdx[mcid]]),mcphiCM[dauIdx[mcid]]);     
	  TLorentzVector Dstarp4(Dstarp3vec,sqrt(Dstarp3vec*Dstarp3vec+mcmass[dauIdx[mcid]]*mcmass[dauIdx[mcid]]));
	  
	  TVector3 Pi1p3vec;Pi1p3vec.SetMagThetaPhi(mcp3CM[dauIdx[mcid]+1],acos(mccosthCM[dauIdx[mcid]+1]),mcphiCM[dauIdx[mcid]+1]);
	  TLorentzVector Pi1p4(Pi1p3vec,sqrt(Pi1p3vec*Pi1p3vec+mcmass[dauIdx[mcid]+1]*mcmass[dauIdx[mcid]+1]));

	  TVector3 Pi2p3vec;Pi2p3vec.SetMagThetaPhi(mcp3CM[dauIdx[mcid]+2],acos(mccosthCM[dauIdx[mcid]+2]),mcphiCM[dauIdx[mcid]+2]);
	  TLorentzVector Pi2p4(Pi2p3vec,sqrt(Pi2p3vec*Pi2p3vec+mcmass[dauIdx[mcid]+2]*mcmass[dauIdx[mcid]+2]));
	  
	  msqplus=(Dstarp4+Pi1p4).Mag2();
	  msqminus=(Dstarp4+Pi2p4).Mag2();	  	  
	  if((msqplus-4.5) > (1.5 - (msqminus-4.5))) H2MCDst2640DalitzPlot->Fill(msqminus,msqplus);

	}
      }
      HMCNDst2640->Fill(MCDst2640CounterPerEvent);
    }

  }

  //print summary
  cout<<"--------Summary-------"<<endl;
  cout<<"Total events="<<eventid<<endl;
  cout<<"Total Generated="<<MCDst2640CounterTotal<<" Reconstructed="<<RecoDst2640CounterTotal<<endl;
  cout<<"Total candidates removed "<<RemovedCands<<" ( "<<(int)(100*RemovedCands/InitialdCands + .5)<<"%)"<<endl;
  cout<<"--------End Summary---"<<endl;

  ////Save the Ntuple and histograms
  cout<<"Going to save the ntuple and histograms"<<endl; 

  
  if(ReducedNtuple->Write()){ cout<<"Ntuple written."<<endl;}
  else{ cout<<"Failed to write ntuple"<<endl;return 0;}
  delete  ReducedNtuple;   

  if(TruthMatch){
    
    if(HMCDst2640Mass->Write()&&
       HMCDst2640DeltaM->Write()&&
       HMCDst2640p3CM->Write()&&
       HMCDst2640costhCM->Write()&&     
       H2MCDst2640CMPvsTheta->Write()&&
       H2MCDst2640DalitzPlot->Write()&&
       HMCNDst2640->Write()&&     
       HLundCheck->Write()
       ){ 
      cout<<" Histograms have been written"<<endl;
    }
    else{
      cout<<"Failed to write Histos"<<endl;return 0;
    }

    delete  HMCDst2640Mass;
    delete  HMCDst2640p3CM;
    delete  HMCDst2640costhCM;     
    delete  H2MCDst2640CMPvsTheta;
    delete  H2MCDst2640DalitzPlot;
    delete  HMCNDst2640;     
    delete  HLundCheck;   
    delete  HMCDst2640DeltaM;
    
    cout<<"histos deleted."<<endl;

  }

  OutPutFile->Write(); //necesary otherwise file is not closed 
  OutPutFile->Close();  
  cout<<OutPutFile->GetName()<<" has been closed."<<endl;
  delete OutPutFile;

  return 1;
}


Int_t Dst2640Analysis::OpenReducedNtuple(){
  
  cout<<"Going to open reduced ntuple."<<endl;

  if(!(OutPutFile=new TFile(_OutputDir+"/Dst2640Reduced.root","read"))){
    cout<<"Unable to open reduced file"<<endl;
  }
  if(OutPutFile->IsZombie()){
    cout<<"Zombie reduced ntuple file"<<endl;return 0;
  }
  cout<<"File opened: "<<OutPutFile<<" "<<OutPutFile->GetName()<<endl;


  if(!(ReducedNtuple=(TTree*)OutPutFile->Get("Dst2640Ntuple"))){
    cout<<"No reduced ntuple"<<endl;return 0;
  }
  //ReducedNtuple->SetDirectory(0);
  cout<<"Ntuple Extracted: "<<ReducedNtuple<<" "<<ReducedNtuple->GetName()<<"  With "<<ReducedNtuple->GetEntries()<<" Entries"<<endl;

  SetReducedNtupleBranches();
  
  gROOT->cd();//******REMEMBER TO DO THIS WHEN OPENING A FILE otherwise new objects are created in that file******//

  return 1;
}

Int_t Dst2640Analysis::Close(){
  cout<<"Going to close the file."<<endl;
 
  OutPutFile->Close();

  delete OutPutFile;
  
  return 1;
}

Int_t  Dst2640Analysis::OpenBkgNtuple(){
  cout<<"Opening Background Ntuple"<<endl;

  //-------------For Background Studies need to know MC scale factors
  //Number of un-skimmed MC events for each type
  Long_t Nccbar=783276000;
  Long_t Nuds=775792000;
  Long_t Nbcharge=574668000;
  Long_t Nbzero=567264000;
  Long_t Ntau=325908000; 

  //Crossections for each type (in nb)
  Crossccbar=1.30;
  Float_t Crossuds=1.39+.35+.35;//u+d+s
  Float_t Crossbbbar=1.05;
  Float_t Crosstau=.000001;//still need to find this one

  //totallumi for each MC type
  Float_t lumiccbar=Nccbar/Crossccbar;
  Float_t lumiuds=Nuds/Crossuds;
  Float_t lumibbbar=(Nbcharge+Nbzero)/Crossbbbar;
  Float_t lumitau=Ntau/Crosstau;


  //Run 1 -5 luminosity (in nb^-1)
  lumidata=388e6; 

  //totalscalefactor for each MC type
  scalefactorccbar=lumidata/lumiccbar;
  scalefactoruds=lumidata/lumiuds;
  scalefactorbbbar=lumidata/lumibbbar;
  scalefactortau=lumidata/lumitau;
  
  //print out the scalefactor for each mode
  cout<<"ccbar MC scalefactor= "<<scalefactorccbar<<endl;
  cout<<"uds MC scalefactor= "<<scalefactoruds<<endl;
  cout<<"bbbar MC scalefactor= "<<scalefactorbbbar<<endl;
  cout<<"tau MC scalefactor= "<<scalefactortau<<endl;

  ///------------------------------

  
  //the background ntuple 
  Fccbar=TFile::Open(ModeDir+"/SP-1005/Dst2640Reduced.root","read");
  ccbartree=NULL;
  if(Fccbar)ccbartree=(TTree*)Fccbar->Get("Dst2640Ntuple");    
  else cout<<"Background File not opened"<<endl;
  gROOT->cd(); 
  if(ccbartree)cout<<"ccbar ntuple openened with "<<ccbartree->GetEntries()<<" entries"<<endl; 
  else cout<<"-------------ccbar ntuple not opened.---------------"<<endl;


  //--------
 
  return 1;
}
Int_t  Dst2640Analysis::CloseBkgNtuple(){
 
  Fccbar->Close();
  delete Fccbar;
  ccbartree=NULL;
 
  cout<<"Background Ntuple closed"<<endl;
  return 1;
}


Float_t Dst2640Analysis::ComputeDstarAngle(){
  TVector3 Dst2640p3vec(Dst2640p3CM[Dst2640Idx]*sin(acos(Dst2640costhCM[Dst2640Idx]))*cos(Dst2640phiCM[Dst2640Idx]),
			Dst2640p3CM[Dst2640Idx]*sin(acos(Dst2640costhCM[Dst2640Idx]))*sin(Dst2640phiCM[Dst2640Idx]),
			Dst2640p3CM[Dst2640Idx]*Dst2640costhCM[Dst2640Idx]);
  
  Float_t Dst2640Energy=sqrt(Dst2640p3vec*Dst2640p3vec + Dst2640Mass[Dst2640Idx]*Dst2640Mass[Dst2640Idx]);
  TVector3 Dst2640Velvec=(1/Dst2640Energy)*Dst2640p3vec;

  TVector3 Dstarp3vec(Dstarp3CM[DstarIdx]*sin(acos(DstarcosthCM[DstarIdx]))*cos(DstarphiCM[DstarIdx]),
		      Dstarp3CM[DstarIdx]*sin(acos(DstarcosthCM[DstarIdx]))*sin(DstarphiCM[DstarIdx]),
		      Dstarp3CM[DstarIdx]*DstarcosthCM[DstarIdx]); 
  
  TLorentzVector Dstarp4(Dstarp3vec,sqrt(Dstarp3vec*Dstarp3vec+DstarMass[DstarIdx]*DstarMass[DstarIdx]));

  //boost the dstar momentum into rest frame of Dst2640
  Dstarp4.Boost(-Dst2640Velvec);//watch the minus sign
  TVector3 DstarBoostedp3vec=Dstarp4.Vect();

  //dot product of boosted dstar direction with Dst2640 direction in CM frame
  Float_t cosine=DstarBoostedp3vec.Unit()*Dst2640Velvec.Unit();
  
  return cosine;
  
}

Float_t Dst2640Analysis::ComputeMsqDstarPi(Int_t WhichPiIdx){

  TVector3 Dstarp3vec;
  Dstarp3vec.SetMagThetaPhi(Dstarp3CM[DstarIdx],acos(DstarcosthCM[DstarIdx]),DstarphiCM[DstarIdx]);  
  TLorentzVector Dstarp4(Dstarp3vec,sqrt(Dstarp3vec*Dstarp3vec+DstarMass[DstarIdx]*DstarMass[DstarIdx]));

  TVector3 Pip3vec;
  Pip3vec.SetMagThetaPhi(Pip3CM[WhichPiIdx],acos(PicosthCM[WhichPiIdx]),PiphiCM[WhichPiIdx]);
  TLorentzVector Pip4(Pip3vec,sqrt(Pip3vec*Pip3vec+PiPDGMass*PiPDGMass));

  TLorentzVector DstarPip4(Dstarp4+Pip4);

  return DstarPip4.Mag2();
}

Float_t Dst2640Analysis::ComputePiPiMass(){
  
  TVector3 Pi1p3vec;
  Pi1p3vec.SetMagThetaPhi(Pip3CM[Pi1Idx],acos(PicosthCM[Pi1Idx]),PiphiCM[Pi1Idx]);
  TLorentzVector Pi1p4(Pi1p3vec,sqrt(Pi1p3vec*Pi1p3vec+PiPDGMass*PiPDGMass));

  TVector3 Pi2p3vec;
  Pi2p3vec.SetMagThetaPhi(Pip3CM[Pi2Idx],acos(PicosthCM[Pi2Idx]),PiphiCM[Pi2Idx]);
  TLorentzVector Pi2p4(Pi2p3vec,sqrt(Pi2p3vec*Pi2p3vec+PiPDGMass*PiPDGMass));

  TLorentzVector PiPip4(Pi1p4+Pi2p4);

  return PiPip4.Mag();
}

Float_t Dst2640Analysis::ComputeDstarPiPiCosine(){

  TVector3 Dstarp3vec;
  Dstarp3vec.SetMagThetaPhi(Dstarp3CM[DstarIdx],acos(DstarcosthCM[DstarIdx]),DstarphiCM[DstarIdx]);  
  
  TVector3 Pi1p3vec;
  Pi1p3vec.SetMagThetaPhi(Pip3CM[Pi1Idx],acos(PicosthCM[Pi1Idx]),PiphiCM[Pi1Idx]);

  TVector3 Pi2p3vec;
  Pi2p3vec.SetMagThetaPhi(Pip3CM[Pi2Idx],acos(PicosthCM[Pi2Idx]),PiphiCM[Pi2Idx]);

  TVector3 PiPip3vec(Pi1p3vec+Pi2p3vec);

  return Dstarp3vec.Unit()*PiPip3vec.Unit();
}

Float_t Dst2640Analysis::ComputePiPiCosine(){
  TVector3 Pi1p3vec;
  Pi1p3vec.SetMagThetaPhi(Pip3CM[Pi1Idx],acos(PicosthCM[Pi1Idx]),PiphiCM[Pi1Idx]);

  TVector3 Pi2p3vec;
  Pi2p3vec.SetMagThetaPhi(Pip3CM[Pi2Idx],acos(PicosthCM[Pi2Idx]),PiphiCM[Pi2Idx]);

  return Pi1p3vec.Unit()*Pi2p3vec.Unit();
}


void Dst2640Analysis::ComputeDstarPiMass(Float_t *mrs,Float_t *mws){

  TVector3 Dstarp3vec;
  Dstarp3vec.SetMagThetaPhi(Dstarp3CM[DstarIdx],acos(DstarcosthCM[DstarIdx]),DstarphiCM[DstarIdx]);  
  TLorentzVector Dstarp4(Dstarp3vec,sqrt(Dstarp3vec*Dstarp3vec+DstarMass[DstarIdx]*DstarMass[DstarIdx]));

  Int_t PiRSIdx=Pi1Idx;
  Int_t PiWSIdx=Pi2Idx;
 
  if(DstarLund[DstarIdx]*PiLund[Pi1Idx]>0){//same sign lunds ->Wrong sign combination
    PiRSIdx=Pi2Idx;
    PiWSIdx=Pi1Idx;
  }
 
  TVector3 PiRSp3vec;
  PiRSp3vec.SetMagThetaPhi(Pip3CM[PiRSIdx],acos(PicosthCM[PiRSIdx]),PiphiCM[PiRSIdx]);
  TLorentzVector PiRSp4(PiRSp3vec,sqrt(PiRSp3vec*PiRSp3vec+PiPDGMass*PiPDGMass));

  TVector3 PiWSp3vec;
  PiWSp3vec.SetMagThetaPhi(Pip3CM[PiWSIdx],acos(PicosthCM[PiWSIdx]),PiphiCM[PiWSIdx]);
  TLorentzVector PiWSp4(PiWSp3vec,sqrt(PiWSp3vec*PiWSp3vec+PiPDGMass*PiPDGMass));

  TLorentzVector DstarPiRSp4(Dstarp4+PiRSp4);
  TLorentzVector DstarPiWSp4(Dstarp4+PiWSp4);

  *mrs=DstarPiRSp4.Mag();
  *mws=DstarPiWSp4.Mag();

  return ;
}


void Dst2640Analysis::ComputeDstarPiPiMass(Float_t *mrs,Float_t *mws){

  TVector3 Dstarp3vec;
  Dstarp3vec.SetMagThetaPhi(Dstarp3CM[DstarIdx],acos(DstarcosthCM[DstarIdx]),DstarphiCM[DstarIdx]);  
  TLorentzVector Dstarp4(Dstarp3vec,sqrt(Dstarp3vec*Dstarp3vec+DstarMass[DstarIdx]*DstarMass[DstarIdx]));
  
  TVector3 Pi1p3vec;
  Pi1p3vec.SetMagThetaPhi(Pip3CM[Pi1Idx],acos(PicosthCM[Pi1Idx]),PiphiCM[Pi1Idx]);
  TLorentzVector Pi1p4(Pi1p3vec,sqrt(Pi1p3vec*Pi1p3vec+PiPDGMass*PiPDGMass));

  //Get Indexes for next D*(2640) in the list
  Int_t NextPi1Idx=Dst2640d2Idx[Dst2640Idx+1];
  Int_t NextPi2Idx=Dst2640d3Idx[Dst2640Idx+1];

  Int_t PiRSIdx=NextPi2Idx;//assume that pi2 of next D*(2640) has correct sign
  Int_t PiWSIdx=NextPi1Idx;
 
  if(PiLund[Pi1Idx]*PiLund[NextPi1Idx]<0){//actually next pi1 has the right sign ==> next pi2 has wrong sign 
    PiRSIdx=NextPi1Idx;
    PiWSIdx=NextPi2Idx;
  }
 
  TVector3 PiRSp3vec;
  PiRSp3vec.SetMagThetaPhi(Pip3CM[PiRSIdx],acos(PicosthCM[PiRSIdx]),PiphiCM[PiRSIdx]);
  TLorentzVector PiRSp4(PiRSp3vec,sqrt(PiRSp3vec*PiRSp3vec+PiPDGMass*PiPDGMass));

  TVector3 PiWSp3vec;
  PiWSp3vec.SetMagThetaPhi(Pip3CM[PiWSIdx],acos(PicosthCM[PiWSIdx]),PiphiCM[PiWSIdx]);
  TLorentzVector PiWSp4(PiWSp3vec,sqrt(PiWSp3vec*PiWSp3vec+PiPDGMass*PiPDGMass));

  TLorentzVector DstarPiRSp4(Dstarp4+Pi1p4+PiRSp4);
  TLorentzVector DstarPiWSp4(Dstarp4+Pi1p4+PiWSp4);

  *mrs=DstarPiRSp4.Mag();
  *mws=DstarPiWSp4.Mag();

  return ;
}

void Dst2640Analysis::ComputeDstarPiCosHel( Float_t* cosiners, Float_t* cosinews){
  //need momentum of D* pi in the CM
 
  TVector3 Dstarp3vec;
  Dstarp3vec.SetMagThetaPhi(Dstarp3CM[DstarIdx],acos(DstarcosthCM[DstarIdx]),DstarphiCM[DstarIdx]);  
  TLorentzVector Dstarp4(Dstarp3vec,sqrt(Dstarp3vec*Dstarp3vec+DstarMass[DstarIdx]*DstarMass[DstarIdx]));

  Int_t PiRSIdx=Pi1Idx;
  Int_t PiWSIdx=Pi2Idx;
 
  if(DstarLund[DstarIdx]*PiLund[Pi1Idx]>0){//same sign lunds ->Wrong sign combination
    PiRSIdx=Pi2Idx;
    PiWSIdx=Pi1Idx;
  }
 
  TVector3 PiRSp3vec;
  PiRSp3vec.SetMagThetaPhi(Pip3CM[PiRSIdx],acos(PicosthCM[PiRSIdx]),PiphiCM[PiRSIdx]);
  TLorentzVector PiRSp4(PiRSp3vec,sqrt(PiRSp3vec*PiRSp3vec+PiPDGMass*PiPDGMass));

  TVector3 PiWSp3vec;
  PiWSp3vec.SetMagThetaPhi(Pip3CM[PiWSIdx],acos(PicosthCM[PiWSIdx]),PiphiCM[PiWSIdx]);
  TLorentzVector PiWSp4(PiWSp3vec,sqrt(PiWSp3vec*PiWSp3vec+PiPDGMass*PiPDGMass));

  TLorentzVector DstarPiRSp4(Dstarp4+PiRSp4);
  TLorentzVector DstarPiWSp4(Dstarp4+PiWSp4);
  
  ///get momenta and energies
  TVector3 DstPiRSp3vec=DstarPiRSp4.Vect();
  TVector3 DstPiWSp3vec=DstarPiWSp4.Vect();
  
  Float_t DstPiRSEnergy=sqrt(DstPiRSp3vec*DstPiRSp3vec + DstarPiRSp4.Mag2());
  TVector3 DstPiRSVelvec=(1/DstPiRSEnergy)*DstPiRSp3vec;

  Float_t DstPiWSEnergy=sqrt(DstPiWSp3vec*DstPiWSp3vec + DstarPiWSp4.Mag2());
  TVector3 DstPiWSVelvec=(1/DstPiWSEnergy)*DstPiWSp3vec;

  //boost the dstar momentum into rest frame of DstPi
  Dstarp4.Boost(-DstPiRSVelvec);//watch the minus sign otherwise you are boosting away from the moving frame
  TVector3 DstarBoostedRSp3vec=Dstarp4.Vect();
  //undo boost
  Dstarp4.Boost(DstPiRSVelvec);
  //now boost into wrong sign
  Dstarp4.Boost(-DstPiWSVelvec);
  TVector3 DstarBoostedWSp3vec=Dstarp4.Vect();

  //dot product of boosted dstar direction with DstPi direction in CM frame
  *cosiners=DstarBoostedRSp3vec.Unit()*DstPiRSVelvec.Unit();
  *cosinews=DstarBoostedWSp3vec.Unit()*DstPiWSVelvec.Unit();
  
}

void Dst2640Analysis::ComputeDstarPipstar(Float_t* pstarrs, Float_t* pstarws){

  TVector3 Dstarp3vec;
  Dstarp3vec.SetMagThetaPhi(Dstarp3CM[DstarIdx],acos(DstarcosthCM[DstarIdx]),DstarphiCM[DstarIdx]);  
  TLorentzVector Dstarp4(Dstarp3vec,sqrt(Dstarp3vec*Dstarp3vec+DstarMass[DstarIdx]*DstarMass[DstarIdx]));

  Int_t PiRSIdx=Pi1Idx;
  Int_t PiWSIdx=Pi2Idx;
 
  if(DstarLund[DstarIdx]*PiLund[Pi1Idx]>0){//same sign lunds ->Wrong sign combination
    PiRSIdx=Pi2Idx;
    PiWSIdx=Pi1Idx;
  }
 
  TVector3 PiRSp3vec;
  PiRSp3vec.SetMagThetaPhi(Pip3CM[PiRSIdx],acos(PicosthCM[PiRSIdx]),PiphiCM[PiRSIdx]);
  TLorentzVector PiRSp4(PiRSp3vec,sqrt(PiRSp3vec*PiRSp3vec+PiPDGMass*PiPDGMass));

  TVector3 PiWSp3vec;
  PiWSp3vec.SetMagThetaPhi(Pip3CM[PiWSIdx],acos(PicosthCM[PiWSIdx]),PiphiCM[PiWSIdx]);
  TLorentzVector PiWSp4(PiWSp3vec,sqrt(PiWSp3vec*PiWSp3vec+PiPDGMass*PiPDGMass));

  TLorentzVector DstarPiRSp4(Dstarp4+PiRSp4);
  TLorentzVector DstarPiWSp4(Dstarp4+PiWSp4);

  *pstarrs=(DstarPiRSp4.Vect()).Mag();
  *pstarws=(DstarPiWSp4.Vect()).Mag();
  

}





Int_t Dst2640Analysis::SetBranches(){
  if(_fChain==NULL){cout<<"No Chain"<<endl;return 0;} 
  cout<<"Setting Chain Branch Addresses"<<endl;
  
//   _fChain->SetBranchAddress("FoxWolfMom0",&FoxWolfMom0);
//   _fChain->SetBranchAddress("FoxWolfMom1",&FoxWolfMom1);
//   _fChain->SetBranchAddress("FoxWolfMom2",&FoxWolfMom2);
//   _fChain->SetBranchAddress("FoxWolfMom3",&FoxWolfMom3);
//   _fChain->SetBranchAddress("Thrust",&Thrust);
//   _fChain->SetBranchAddress("Thrustx",&Thrustx);
//   _fChain->SetBranchAddress("Thrusty",&Thrusty);
//   _fChain->SetBranchAddress("Thrustz",&Thrustz);

  _fChain->SetBranchAddress("nDst2640",&nDst2640);
  _fChain->SetBranchAddress("Dst2640Mass",Dst2640Mass);
  _fChain->SetBranchAddress("Dst2640p3",Dst2640p3);
  _fChain->SetBranchAddress("Dst2640costh",Dst2640costh);
  _fChain->SetBranchAddress("Dst2640phi",Dst2640phi);
  _fChain->SetBranchAddress("Dst2640p3CM",Dst2640p3CM);
  _fChain->SetBranchAddress("Dst2640costhCM",Dst2640costhCM);
  _fChain->SetBranchAddress("Dst2640phiCM",Dst2640phiCM);
  _fChain->SetBranchAddress("Dst2640Lund",Dst2640Lund); 
  _fChain->SetBranchAddress("Dst2640d1Lund",Dst2640d1Lund);
  _fChain->SetBranchAddress("Dst2640d1Idx",Dst2640d1Idx);
  _fChain->SetBranchAddress("Dst2640d2Lund",Dst2640d2Lund);
  _fChain->SetBranchAddress("Dst2640d2Idx",Dst2640d2Idx);
  _fChain->SetBranchAddress("Dst2640d3Lund",Dst2640d3Lund);
  _fChain->SetBranchAddress("Dst2640d3Idx",Dst2640d3Idx);
  _fChain->SetBranchAddress("Dst2640Vtxx",Dst2640Vtxx);
  _fChain->SetBranchAddress("Dst2640Vtxy",Dst2640Vtxy);
  _fChain->SetBranchAddress("Dst2640Vtxz",Dst2640Vtxz);
  _fChain->SetBranchAddress("Dst2640Chi2",Dst2640Chi2);
  _fChain->SetBranchAddress("Dst2640nDof",Dst2640nDof);
  _fChain->SetBranchAddress("Dst2640VtxStatus",Dst2640VtxStatus);
 
  _fChain->SetBranchAddress("nDstar",&nDstar);
  _fChain->SetBranchAddress("DstarMass",DstarMass);
  _fChain->SetBranchAddress("Dstarp3",Dstarp3);
  _fChain->SetBranchAddress("Dstarcosth",Dstarcosth);
  _fChain->SetBranchAddress("Dstarphi",Dstarphi);
  _fChain->SetBranchAddress("Dstarp3CM",Dstarp3CM);
  _fChain->SetBranchAddress("DstarcosthCM",DstarcosthCM);
  _fChain->SetBranchAddress("DstarphiCM",DstarphiCM);
  _fChain->SetBranchAddress("DstarLund",DstarLund);
  _fChain->SetBranchAddress("Dstard1Lund",Dstard1Lund);
  _fChain->SetBranchAddress("Dstard1Idx",Dstard1Idx);
  _fChain->SetBranchAddress("Dstard2Lund",Dstard2Lund);
  _fChain->SetBranchAddress("Dstard2Idx",Dstard2Idx);
  _fChain->SetBranchAddress("DstarChi2",DstarChi2);
  _fChain->SetBranchAddress("DstarnDof",DstarnDof);
  //  _fChain->SetBranchAddress("DstarD0VtxProbUsr",DstarD0VtxProbUsr);
  _fChain->SetBranchAddress("DstarVtxStatus",DstarVtxStatus);


  _fChain->SetBranchAddress("nD0",&nD0);
  _fChain->SetBranchAddress("D0Mass",D0Mass);
  _fChain->SetBranchAddress("D0p3CM",D0p3CM); 
  _fChain->SetBranchAddress("D0costhCM",D0costhCM); 
  _fChain->SetBranchAddress("D0phiCM",D0phiCM);  
  _fChain->SetBranchAddress("D0Lund",D0Lund);
  _fChain->SetBranchAddress("D0d1Lund",D0d1Lund);
  _fChain->SetBranchAddress("D0d1Idx",D0d1Idx);
  _fChain->SetBranchAddress("D0d2Lund",D0d2Lund);
  _fChain->SetBranchAddress("D0d2Idx",D0d2Idx);
  _fChain->SetBranchAddress("D0Chi2",D0Chi2);
  _fChain->SetBranchAddress("D0nDof",D0nDof);
  _fChain->SetBranchAddress("D0VtxStatus",D0VtxStatus);


  _fChain->SetBranchAddress("nPi",&nPi);
  _fChain->SetBranchAddress("Pip3",Pip3);
  _fChain->SetBranchAddress("Picosth",Picosth);
  _fChain->SetBranchAddress("Piphi",Piphi);
  _fChain->SetBranchAddress("Pip3CM",Pip3CM);
  _fChain->SetBranchAddress("PicosthCM",PicosthCM);
  _fChain->SetBranchAddress("PiphiCM",PiphiCM);
  _fChain->SetBranchAddress("PiLund",PiLund);
  _fChain->SetBranchAddress("PiTrkIdx",PiTrkIdx);

  _fChain->SetBranchAddress("nK",&nK);
  _fChain->SetBranchAddress("Kp3",Kp3);
  _fChain->SetBranchAddress("Kp3CM",Kp3CM);
  _fChain->SetBranchAddress("KcosthCM",KcosthCM);
  _fChain->SetBranchAddress("KphiCM",KphiCM);
  _fChain->SetBranchAddress("KLund",KLund);
  _fChain->SetBranchAddress("KTrkIdx",KTrkIdx);

  _fChain->SetBranchAddress("TRKnSvt",TRKnSvt);
  _fChain->SetBranchAddress("TRKLund",TRKLund);
  //_fChain->SetBranchAddress("piSelectorsMap",PiSelectorsMap);
  //  _fChain->SetBranchAddress("KSelectorsMap",KSelectorsMap);


  if(TruthMatch){
    _fChain->SetBranchAddress("Dst2640MCIdx",Dst2640MCIdx);
    _fChain->SetBranchAddress("DstarMCIdx",DstarMCIdx);
    _fChain->SetBranchAddress("D0MCIdx",D0MCIdx);
    _fChain->SetBranchAddress("KMCIdx",KMCIdx);
    _fChain->SetBranchAddress("PiMCIdx",PiMCIdx);
    _fChain->SetBranchAddress("mcLund",mcLund);
    _fChain->SetBranchAddress("mothIdx",mothIdx);
    _fChain->SetBranchAddress("dauIdx",dauIdx);
    _fChain->SetBranchAddress("mcLen",&mcLen);
    _fChain->SetBranchAddress("mcmass",mcmass);
    _fChain->SetBranchAddress("mcp3",mcp3);
    _fChain->SetBranchAddress("mccosth",mccosth);
    _fChain->SetBranchAddress("mcphi",mcphi);
    _fChain->SetBranchAddress("mcp3CM",mcp3CM);
    _fChain->SetBranchAddress("mccosthCM",mccosthCM); 
    _fChain->SetBranchAddress("mcphiCM",mcphiCM);
   }
  

 
  SetModeBranches();

  return 1;
}

Int_t Dst2640Analysis::SetModeBranches(){
  if(_Mode=="D0ToK3Pi"){
    _fChain->SetBranchAddress("D0d3Lund",D0d3Lund);
    _fChain->SetBranchAddress("D0d3Idx",D0d3Idx);
    _fChain->SetBranchAddress("D0d4Lund",D0d4Lund);
    _fChain->SetBranchAddress("D0d4Idx",D0d4Idx);
  }


  return 1;
}

Int_t Dst2640Analysis::MakeReducedNtupleBranches(){
  cout<<"Creating Reduced Ntuple Branches"<<endl;

  ReducedNtuple->Branch("eventnumber",&eventnumber,"eventnumber/I");
  ReducedNtuple->Branch("eventfwm0",&eventfwm0,"eventfwm0/F");
  ReducedNtuple->Branch("eventfwm1",&eventfwm1,"eventfwm1/F");
  ReducedNtuple->Branch("eventfwm2",&eventfwm2,"eventfwm2/F");
  ReducedNtuple->Branch("eventfwm3",&eventfwm3,"eventfwm3/F");
  ReducedNtuple->Branch("eventthrust",&eventthrust,"eventthrust/F");
  ReducedNtuple->Branch("eventthrustx",&eventthrustx,"eventthrustx/F");
  ReducedNtuple->Branch("eventthrusty",&eventthrusty,"eventthrusty/F");
  ReducedNtuple->Branch("eventthrustz",&eventthrustz,"eventthrustz/F");

  ReducedNtuple->Branch("dst2640mass",&dst2640mass,"dst2640mass/F");
  ReducedNtuple->Branch("dst2640deltam",&dst2640deltam,"dst2640deltam/F");
  ReducedNtuple->Branch("dst2640pstar",&dst2640pstar,"dst2640pstar/F");
  ReducedNtuple->Branch("dst2640cosstar",&dst2640cosstar,"dst2640cosstar/F");
  ReducedNtuple->Branch("dst2640mctrue",&dst2640mctrue,"dst2640mctrue/I");
  ReducedNtuple->Branch("dst2640charge",&dst2640charge,"dst2640charge/I");
  ReducedNtuple->Branch("dst2640bestvtxprob",&dst2640bestvtxprob,"dst2640bestvtxprob/I");
  ReducedNtuple->Branch("Dst2640Idx",&Dst2640Idx,"Dst2640Idx/I");
  ReducedNtuple->Branch("dst2640nDst2640",&dst2640nDst2640,"dst2640nDst2640/I");
  ReducedNtuple->Branch("dst2640logvtxprob",&dst2640logvtxprob,"dst2640logvtxprob/F");
  ReducedNtuple->Branch("dst2640dmres",&dst2640dmres,"dst2640dmres/F");

  ReducedNtuple->Branch("dstarmass",&dstarmass,"dstarmass/F");
  ReducedNtuple->Branch("dstarpstar",&dstarpstar,"dstarpstar/F");
  ReducedNtuple->Branch("dstardeltam",&dstardeltam,"dstardeltam/F");
  ReducedNtuple->Branch("dstarmctrue",&dstarmctrue,"dstarmctrue/I");
  ReducedNtuple->Branch("dstarlogvtxprob",&dstarlogvtxprob,"dstarlogvtxprob/F");
  ReducedNtuple->Branch("dstarcostheta",&dstarcostheta,"dstarcostheta/F");

  ReducedNtuple->Branch("d0mass",&d0mass,"d0mass/F"); 
  ReducedNtuple->Branch("d0pstar",&d0pstar,"d0pstar/F");
  ReducedNtuple->Branch("d0mctrue",&d0mctrue,"d0mctrue/I");
  ReducedNtuple->Branch("d0logvtxprob",&d0logvtxprob,"d0logvtxprob/F");
  ReducedNtuple->Branch("d0mode",&d0mode,"d0mode/I");

  ReducedNtuple->Branch("klh",&klh,"klh/I");
  ReducedNtuple->Branch("kmctrue",&kmctrue,"kmctrue/I");
  ReducedNtuple->Branch("kp3",&kp3,"kp3/F");

  ReducedNtuple->Branch("pilh",&pilh,"pilh/I");
  ReducedNtuple->Branch("pimctrue",&pimctrue,"pimctrue/I");
  ReducedNtuple->Branch("pip3",&pip3,"pip3/F");

  ReducedNtuple->Branch("pi1lh",&pi1lh,"pi1lh/I");
  ReducedNtuple->Branch("pi1mctrue",&pi1mctrue,"pi1mctrue/I");
  ReducedNtuple->Branch("pi1p3",&pi1p3,"pi1p3/F");
  ReducedNtuple->Branch("pi1charge",&pi1charge,"pi1charge/I");

  ReducedNtuple->Branch("pi2lh",&pi2lh,"pi2lh/I");
  ReducedNtuple->Branch("pi2mctrue",&pi2mctrue,"pi2mctrue/I");
  ReducedNtuple->Branch("pi2p3",&pi2p3,"pi2p3/F");
  ReducedNtuple->Branch("pi2charge",&pi2charge,"pi2charge/I");

  ReducedNtuple->Branch("slowpip3",&slowpip3,"slowpip3/F");
  ReducedNtuple->Branch("slowpimctrue",&slowpimctrue,"slowpimctrue/I");

  ReducedNtuple->Branch("msqdstarpiplus",&msqdstarpiplus,"msqdstarpiplus/F");
  ReducedNtuple->Branch("msqdstarpiminus",&msqdstarpiminus,"msqdstarpiminus/F");
  ReducedNtuple->Branch("pipimass",&pipimass,"pipimass/F");  
  ReducedNtuple->Branch("dstarpipicosine",&dstarpipicosine,"dstarpipicosine/F");
  ReducedNtuple->Branch("pipicosine",&pipicosine,"pipicosine/F");
  ReducedNtuple->Branch("mdstarpirs",&mdstarpirs,"mdstarpirs/F");
  ReducedNtuple->Branch("mdstarpiws",&mdstarpiws,"mdstarpiws/F");
  ReducedNtuple->Branch("mdstarpipirs",&mdstarpipirs,"mdstarpipirs/F");
  ReducedNtuple->Branch("mdstarpipiws",&mdstarpipiws,"mdstarpipiws/F");
  ReducedNtuple->Branch("pi1pluspi2charge",&pi1pluspi2charge,"pi1pluspi2charge/I");
  ReducedNtuple->Branch("dstarpideltam",&dstarpideltam,"dstarpideltam/F");
  ReducedNtuple->Branch("dstarpideltamws",&dstarpideltamws,"dstarpideltamws/F");
  ReducedNtuple->Branch("dstarpicoshelrs",&dstarpicoshelrs,"dstarpicoshelrs/F");
  ReducedNtuple->Branch("dstarpicoshelws",&dstarpicoshelws,"dstarpicoshelws/F");
  ReducedNtuple->Branch("dstarpipstarrs",&dstarpipstarrs,"dstarpipstarrs/F");
  ReducedNtuple->Branch("dstarpipstarws",&dstarpipstarws,"dstarpipstarws/F");


  MakeModeReducedNtupleBranches();

  return 1;
}

Int_t Dst2640Analysis::MakeModeReducedNtupleBranches(){

  if(_Mode=="D0ToK3Pi"){
    ReducedNtuple->Branch("d0pi2lh",&d0pi2lh,"d0pi2lh/I");
    ReducedNtuple->Branch("d0pi3lh",&d0pi3lh,"d0pi3lh/I");
    ReducedNtuple->Branch("d0pi2mctrue",&d0pi2mctrue,"d0pi2mctrue/I");
    ReducedNtuple->Branch("d0pi3mctrue",&d0pi3mctrue,"d0pi3mctrue/I");
  }
  return 1;
}


Int_t Dst2640Analysis::SetReducedNtupleBranches(){
  cout<<"Setting Reduced Ntuple Branch Addresses"<<endl;

  ReducedNtuple->SetBranchAddress("eventnumber",&eventnumber);
  ReducedNtuple->SetBranchAddress("eventfwm0",&eventfwm0);
  ReducedNtuple->SetBranchAddress("eventfwm1",&eventfwm1);
  ReducedNtuple->SetBranchAddress("eventfwm2",&eventfwm2);
  ReducedNtuple->SetBranchAddress("eventfwm3",&eventfwm3);
  ReducedNtuple->SetBranchAddress("eventthrust",&eventthrust);
  ReducedNtuple->SetBranchAddress("eventthrustx",&eventthrustx);
  ReducedNtuple->SetBranchAddress("eventthrusty",&eventthrusty);
  ReducedNtuple->SetBranchAddress("eventthrustz",&eventthrustz);


  ReducedNtuple->SetBranchAddress("dst2640mass",&dst2640mass);
  ReducedNtuple->SetBranchAddress("dst2640deltam",&dst2640deltam);
  ReducedNtuple->SetBranchAddress("dst2640pstar",&dst2640pstar);
  ReducedNtuple->SetBranchAddress("dst2640cosstar",&dst2640cosstar);
  ReducedNtuple->SetBranchAddress("Dst2640Idx",&Dst2640Idx);
  ReducedNtuple->SetBranchAddress("dst2640mctrue",&dst2640mctrue);
  ReducedNtuple->SetBranchAddress("dst2640charge",&dst2640charge);
  ReducedNtuple->SetBranchAddress("dst2640bestvtxprob",&dst2640bestvtxprob);
  ReducedNtuple->SetBranchAddress("dst2640nDst2640",&dst2640nDst2640);
  ReducedNtuple->SetBranchAddress("dst2640logvtxprob",&dst2640logvtxprob);
  ReducedNtuple->SetBranchAddress("dst2640dmres",&dst2640dmres);

  ReducedNtuple->SetBranchAddress("dstarmass",&dstarmass);
  ReducedNtuple->SetBranchAddress("dstarpstar",&dstarpstar);
  ReducedNtuple->SetBranchAddress("dstardeltam",&dstardeltam);
  ReducedNtuple->SetBranchAddress("dstarmctrue",&dstarmctrue);
  ReducedNtuple->SetBranchAddress("dstarlogvtxprob",&dstarlogvtxprob);
  ReducedNtuple->SetBranchAddress("dstarcostheta",&dstarcostheta);

  ReducedNtuple->SetBranchAddress("d0mass",&d0mass);
  ReducedNtuple->SetBranchAddress("d0pstar",&d0pstar);
  ReducedNtuple->SetBranchAddress("d0mctrue",&d0mctrue);
  ReducedNtuple->SetBranchAddress("d0logvtxprob",&d0logvtxprob);
  ReducedNtuple->SetBranchAddress("d0mode",&d0mode);

  ReducedNtuple->SetBranchAddress("klh",&klh);
  ReducedNtuple->SetBranchAddress("kmctrue",&kmctrue);
  ReducedNtuple->SetBranchAddress("kp3",&kp3);

  ReducedNtuple->SetBranchAddress("pilh",&pilh);
  ReducedNtuple->SetBranchAddress("pimctrue",&pimctrue);
  ReducedNtuple->SetBranchAddress("pip3",&pip3);

  ReducedNtuple->SetBranchAddress("pi1lh",&pi1lh);
  ReducedNtuple->SetBranchAddress("pi1mctrue",&pi1mctrue);
  ReducedNtuple->SetBranchAddress("pi1p3",&pi1p3);
  ReducedNtuple->SetBranchAddress("pi1charge",&pi1charge);
  
  ReducedNtuple->SetBranchAddress("pi2lh",&pi2lh);
  ReducedNtuple->SetBranchAddress("pi2mctrue",&pi2mctrue);
  ReducedNtuple->SetBranchAddress("pi2p3",&pi2p3);
  ReducedNtuple->SetBranchAddress("pi2charge",&pi2charge);

  ReducedNtuple->SetBranchAddress("slowpip3",&slowpip3);
  ReducedNtuple->SetBranchAddress("slowpimctrue",&slowpimctrue);
  
  ReducedNtuple->SetBranchAddress("msqdstarpiplus",&msqdstarpiplus);
  ReducedNtuple->SetBranchAddress("msqdstarpiminus",&msqdstarpiminus);
  ReducedNtuple->SetBranchAddress("pipimass",&pipimass);
  ReducedNtuple->SetBranchAddress("dstarpipicosine",&dstarpipicosine);
  ReducedNtuple->SetBranchAddress("pipicosine",&pipicosine);
  ReducedNtuple->SetBranchAddress("mdstarpirs",&mdstarpirs);
  ReducedNtuple->SetBranchAddress("mdstarpiws",&mdstarpiws);
  ReducedNtuple->SetBranchAddress("mdstarpipirs",&mdstarpipirs);
  ReducedNtuple->SetBranchAddress("mdstarpipiws",&mdstarpipiws);
  ReducedNtuple->SetBranchAddress("pi1pluspi2charge",&pi1pluspi2charge);
  ReducedNtuple->SetBranchAddress("dstarpideltam",&dstarpideltam);
  ReducedNtuple->SetBranchAddress("dstarpideltamws",&dstarpideltamws);
  ReducedNtuple->SetBranchAddress("dstarpicoshelrs",&dstarpicoshelrs);
  ReducedNtuple->SetBranchAddress("dstarpicoshelws",&dstarpicoshelws);
  ReducedNtuple->SetBranchAddress("dstarpipstarrs",&dstarpipstarrs);
  ReducedNtuple->SetBranchAddress("dstarpipstarws",&dstarpipstarws);


  return 1;
}

Int_t Dst2640Analysis::SetModeReducedNtupleBranches(){  
  if(_Mode=="D0ToK3Pi"){
    ReducedNtuple->SetBranchAddress("d0pi2lh",&d0pi2lh);
    ReducedNtuple->SetBranchAddress("d0pi3lh",&d0pi3lh);
    ReducedNtuple->SetBranchAddress("d0pi2mctrue",&d0pi2mctrue);
    ReducedNtuple->SetBranchAddress("d0pi3mctrue",&d0pi3mctrue);
  
  }

  return 1;
}
