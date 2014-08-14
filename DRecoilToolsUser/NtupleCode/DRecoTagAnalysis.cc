#include "DRecoilToolsUser/NtupleCode/DRecoTagAnalysis.hh"

#include "DRecoilToolsUser/NtupleCode/Fits.hh"

#include "RooDataHist.h"
#include "RooArgSet.h"
#include "RooGenericPdf.h"
#include "TH1F.h"
#include "RooFormulaVar.h"



ClassImp(DRecoTagAnalysis);
DRecoTagAnalysis::DRecoTagAnalysis(Int_t DataType,TString OutputDir):
  DRecoEvtAnalysis(DataType,OutputDir),
  cuttagmass(0),   
  cuttagpstar(0),  
  cuttagvtxprob(0),
  cuttagmode(0), 
  mass("mass","mass",0,6)
{   
  nTagModes[0]=6;
  nTagModes[1]=5;
  nTagModes[2]=7;
  nTagModes[3]=8;

//   //check if the file with the latest fitted means an sigmas exists
//   ifstream inputStats("/nfs/farm/g/eb/u17/Charm/gjackson/TagOptimization/DsTolnu/TagOptRaw/fittedTagMeansAndWidths.txt");
//   //
//   if (!inputStats.is_open()) {

    cout<<"Using default tag info."<<endl;

    TagNames[0][0]="D0ToKPi";
    TagNames[0][1]="D0ToKPiPi0";
    TagNames[0][2]="D0ToK3Pi";
    TagNames[0][3]="D0ToKsPiPi";
    TagNames[0][4]="D0ToK3PiPi0";
    TagNames[0][5]="D0ToKsPiPiPi0";
    
    TagNames[1][0]="DPlusToKPiPi";
    TagNames[1][1]="DPlusToKPiPiPi0";
    TagNames[1][2]="DPlusToKsPi";
    TagNames[1][3]="DPlusToKsPiPi0";
    TagNames[1][4]="DPlusToKs3Pi";
     
    TagNames[2][0]="DsToKsK";
    TagNames[2][1]="DsToPhiPi";
    TagNames[2][2]="DsToPhiRhoC";
    TagNames[2][3]="DsToKsKPiPi";
    TagNames[2][4]="DsToEtaPPi";
    TagNames[2][5]="DsToPhiRho0pi";
    TagNames[2][6]="DsToKKstar0";
    
    TagNames[3][0]="LambdaCTopKpi";
    TagNames[3][1]="LambdaCTopKpipi0";
    TagNames[3][2]="LambdaCTopKs";
    TagNames[3][3]="LambdaCTopKspipi";
    TagNames[3][4]="LambdaCToLzpi";
    TagNames[3][5]="LambdaCToLzpipi0";
    TagNames[3][6]="LambdaCToLzpipipi";
    TagNames[3][7]="LambdaCToSzpi";
    
    TagMean[0][0]=1.8638;
    TagMean[0][1]=1.86256;
    TagMean[0][2]=1.86369;
    TagMean[0][3]=1.86388;
    TagMean[0][4]=1.8631;
    TagMean[0][5]=1.86267;
    		  
    TagMean[1][0]=1.86854;
    TagMean[1][1]=1.86717;
    TagMean[1][2]=1.86895;
    TagMean[1][3]=1.86785;
    TagMean[1][4]=1.86867;
    		  
    TagMean[2][0]=1.96761; 
    TagMean[2][1]=1.96716; 
    TagMean[2][2]=1.96624; 
    TagMean[2][3]=1.96739; 
    TagMean[2][4]=1.96772; 
    TagMean[2][5]=1.96792; 
    TagMean[2][6]=1.96741; 
    		  
    TagMean[3][0]=2.2857; 
    TagMean[3][1]=2.28433; 
    TagMean[3][2]=2.28588; 
    TagMean[3][3]=2.28563; 
    TagMean[3][4]=2.2863; 
    TagMean[3][5]=2.28373; 
    TagMean[3][6]=2.28555; 
    TagMean[3][7]=2.28587; 
    
    //correspond to 3*sigma where sigma was rounded up
    TagWidth[0][0]=3*7.3617/1000;
    TagWidth[0][1]=3*12.865/1000;
    TagWidth[0][2]=3*5.91578/1000;
    TagWidth[0][3]=3*5.6642/1000;
    TagWidth[0][4]=3*9.22685/1000;
    TagWidth[0][5]=3*10.9308/1000;
    		     
    TagWidth[1][0]=3*5.99326/1000;
    TagWidth[1][1]=3*11.9255/1000;
    TagWidth[1][2]=3*6.48779/1000;
    TagWidth[1][3]=3*13.1797/1000;
    TagWidth[1][4]=3*5.49975/1000;
    		     
    TagWidth[2][0]=3*6.53709/1000;
    TagWidth[2][1]=3*7.0125/1000;
    TagWidth[2][2]=3*11.3125/1000; 
    TagWidth[2][3]=3*5.36424/1000;
    TagWidth[2][4]=3*7.33129/1000;
    TagWidth[2][5]=3*3.84237/1000;
    TagWidth[2][6]=3*6.20826/1000;
    		     
    TagWidth[3][0]=3*4.97632/1000;
    TagWidth[3][1]=3*9.4882/1000;
    TagWidth[3][2]=3*5.67084/1000;
    TagWidth[3][3]=3*4.13083/1000;
    TagWidth[3][4]=3*5.10548/1000;
    TagWidth[3][5]=3*17.0918/1000; 
    TagWidth[3][6]=3*4.95128/1000;
    TagWidth[3][7]=3*3.7372/1000;
//   }else{
//     //These mean values are 'iterative' in a sense, since they come from the latest 
//     //fitted tag mass plot values, so make sure these lastest fits look sensible
//     //The ps file with the graphs is DsTolnu/TagOptRaw/TagMassPlots.ps
//     //The text file with the fit values is DsTolnu/TagOptRaw/fittedTagMeansAndWidths.txt
//     //
//     cout<<endl<<"Reading input tag info from file."<<endl<<"WARNING - MAKE SURE YOUR TagMassPlots LOOK GOOD!!!"<<endl<<endl;;
//     //
//     Double_t tempfield;
//     for(Int_t Dindex=1;Dindex<=NTagTypes;Dindex++){
//       for(Int_t Modeindex=1;Modeindex<=nTagModes[Dindex-1];Modeindex++){
// 	inputStats>>TagNames[Dindex-1][Modeindex-1]; 
// 	inputStats>>TagMean[Dindex-1][Modeindex-1];
// 	inputStats>>tempfield;
// 	TagWidth[Dindex-1][Modeindex-1]=tempfield*3;
//       }
//     }
//   }

  cout<<"Done Initializing the DRecoTagAnalysis analysis"<<endl;
}

DRecoTagAnalysis::~DRecoTagAnalysis(){  
}

Int_t DRecoTagAnalysis::ReduceNtuple(){
  cout<<"Going to reduce the ntuple"<<endl;

  if(_fChain==NULL){
    cout<<"No chain."<<endl;
    return 0;
  } 

  ///link the branches in the input chain
  SetTagBranches();

  //the file must be created here so the 'new' histos and Tree is created in hard drive //OutFile=new 
  //TFile OutFile(_OutputDir+"/Ntuple_New.root","recreate",_OutputDir+"/Ntuple_New.root",1);//1=minimal compression
  TFile OutFile(_OutputDir+Form("/Ntuple_New_%d_to_%d.root",startfile,endfile),"recreate",_OutputDir+"/Ntuple_New.root",1);//1=minimal compression
  //TFile OutFile(_OutputDir+"/Ntuple_new.root","recreate",_OutputDir+"/Ntuple.root",1);//1=minimal compression
  if(OutFile.IsZombie()){cout<<"Failed to create file"<<endl;return 0;}

  
  //create new reduced ntuple
  TTree Ntuple("Ntuple","Ntuple",0);
  ReducedNtuple=&Ntuple;
  //Long64_t maxsize=10000000000;//10Gb
  //cout<<" file size "<<maxsize<<endl;
  ReducedNtuple->SetMaxTreeSize(900000000);//split files
  //Long64_t fautosave=11000000000;//11Gb !!//disable autosaving 
  //cout<<" autosave "<<fautosave<<endl;
  ReducedNtuple->SetAutoSave(900000000);
 
  //create the branches in the Ntuple
  MakeTagReducedNtupleBranches();

//   //print out cut settings
//   cout<<"cuttagmass    "<<cuttagmass<<endl;
//   cout<<"cuttagpstar   "<<cuttagpstar<<endl;
//   cout<<"cuttagvtxprob "<<cuttagvtxprob<<endl;
//   cout<<"cuttagmode    "<<cuttagmode<<endl;

  //begin loop
  cout<<"Total Entries in Chain="<<_fChain->GetEntries()<<endl;
  cout<<"Going to start the event loop"<<endl;
  eventid=0;
  Int_t ngoodthisevent=0;
  goodeventid=0;
  while(_fChain->GetEntry(eventid,0)>0){   
    eventid++;
     
    if(eventid%50000==0)cout<<eventid<<" Events done."<<endl;
    
    if(!FillTagEventVars())continue;
        
    if(!TagEvtPassesCuts())continue;   

    ngoodthisevent=0;
    goodrecoid=0;
    for(TagIdx=0;TagIdx<nTag;TagIdx++){

      //should not fill any vars outside this method otherwise not saved
      if(!FillTagVars())continue;
      
      //some cuts go here
      if(!TagPassesCuts())continue;   

      //save
      ngoodthisevent++;//increase each fill
      goodrecoid++;
      ReducedNtuple->Fill();	              
   
    }//Tag loop    
    if(ngoodthisevent>0)goodeventid++;
    
    
  }
  
  //print summary
  cout<<"--------Summary-------"<<endl;
  cout<<"Total events = "<<eventid<<endl;


  ////Save the Ntuple and histograms
  cout<<"Going to save the ntuple and histograms"<<endl; 


  if(ReducedNtuple->AutoSave("Overwrite")){ cout<<"Ntuple written."<<endl;}
  else{ cout<<"Failed to write ntuple"<<endl;return 0;}

  cout<<"Objects in the current file"<<endl;
  ReducedNtuple->GetCurrentFile()->ls();

  OutFile.Close(); //this will delete all objects created inside the file if they were not written  
  cout<<OutFile.GetName()<<" has been closed."<<endl;


  return 1;
}

Bool_t DRecoTagAnalysis::FillTagEventVars(){
  ///Event vars
  if(!FillEventVars()){cout<<"FillEventVars()=0"<<endl;return 0;}

  ////for MC fill generated quantities
  if(_DataType==0||_DataType==1){
    //--------------------------------
    //Find the generated Tag-D Idx
    //--------------------------------
    GenTagIdx=-1;
    for(Int_t mcc=2;mcc<mcLen;mcc++){
      if(abs(mcLund[mcc])==myD0Lund
	 ||abs(mcLund[mcc])==myDPlusLund
	 ||abs(mcLund[mcc])==myDsLund
	 ||abs(mcLund[mcc])==myLambdaCLund){
	GenTagIdx=mcc;
      }
    }
  }

  return 1;
}

Bool_t DRecoTagAnalysis::TagEvtPassesCuts(){
  Bool_t pass=1;
  if(!EvtPassesCuts())pass=0;
  //if(pass)cout<<"passEvt"<<endl;

  ////for Sig-MC remove events which do not have a tag
  if(_DataType==0)
    if(GenTagIdx==-1)pass=0;
  //if(pass)cout<<"pass GenTagIdx"<<endl;
 
  return pass;
}


Bool_t DRecoTagAnalysis::FillTagVars(){
  
  if(TagIdx<0||TagIdx>=NMAXTAG){cout<<" TagIdx="<<TagIdx<<endl;return 0;}

  //For D* tags switch to the D daughter
  tagdstarlund=0;
  tagdstargamenergy=0.;
  tagdstardauangle=0.;
  tagdstardecayangle=0.;
  tagdstarmass=0.;
  if(abs(TagLund[TagIdx])==myDstarLund || abs(TagLund[TagIdx])==myDstar0Lund){  
    tagdstarmass=TagMass[TagIdx];
    //Note that D*'s must have D as first daughter when creating them in beta code; 
    //determine the decay mode
    if(abs(Tagd1Lund[TagIdx])==myD0Lund&&abs(Tagd2Lund[TagIdx])==myPiLund)tagdstarlund=1;
    if(abs(Tagd1Lund[TagIdx])==myD0Lund&&abs(Tagd2Lund[TagIdx])==myPi0Lund)tagdstarlund=2;
    if(abs(Tagd1Lund[TagIdx])==myDPlusLund&&abs(Tagd2Lund[TagIdx])==myPi0Lund)tagdstarlund=3;
    if(abs(Tagd1Lund[TagIdx])==myD0Lund&&abs(Tagd2Lund[TagIdx])==myGammaLund)tagdstarlund=4;

    //angle of pi/gam with respect to D* in cm frame
    TLorentzVector DstarVectCM(Tagp3CM[TagIdx]*sin(acos(TagcosthCM[TagIdx]))*cos(TagphiCM[TagIdx]),
			       Tagp3CM[TagIdx]*sin(acos(TagcosthCM[TagIdx]))*sin(TagphiCM[TagIdx]),
			       Tagp3CM[TagIdx]*TagcosthCM[TagIdx],
			       TagenergyCM[TagIdx]);
    TLorentzVector DstarDauVectCM;
    if(abs(Tagd2Lund[TagIdx])==myPiLund)
      DstarDauVectCM=TLorentzVector(Pip3CM[Tagd2Idx[TagIdx]]*sin(acos(PicosthCM[Tagd2Idx[TagIdx]]))*cos(PiphiCM[Tagd2Idx[TagIdx]]),
				    Pip3CM[Tagd2Idx[TagIdx]]*sin(acos(PicosthCM[Tagd2Idx[TagIdx]]))*sin(PiphiCM[Tagd2Idx[TagIdx]]),
				    Pip3CM[Tagd2Idx[TagIdx]]*PicosthCM[Tagd2Idx[TagIdx]],
				    PienergyCM[Tagd2Idx[TagIdx]]);
    if(abs(Tagd2Lund[TagIdx])==myPi0Lund)
      DstarDauVectCM=TLorentzVector(Pi0p3CM[Tagd2Idx[TagIdx]]*sin(acos(Pi0costhCM[Tagd2Idx[TagIdx]]))*cos(Pi0phiCM[Tagd2Idx[TagIdx]]),
				    Pi0p3CM[Tagd2Idx[TagIdx]]*sin(acos(Pi0costhCM[Tagd2Idx[TagIdx]]))*sin(Pi0phiCM[Tagd2Idx[TagIdx]]),
				    Pi0p3CM[Tagd2Idx[TagIdx]]*Pi0costhCM[Tagd2Idx[TagIdx]],
				    Pi0energyCM[Tagd2Idx[TagIdx]]);
    if(abs(Tagd2Lund[TagIdx])==myGammaLund){
      DstarDauVectCM=TLorentzVector(Gamp3CM[Tagd2Idx[TagIdx]]*sin(acos(GamcosthCM[Tagd2Idx[TagIdx]]))*cos(GamphiCM[Tagd2Idx[TagIdx]]),
				    Gamp3CM[Tagd2Idx[TagIdx]]*sin(acos(GamcosthCM[Tagd2Idx[TagIdx]]))*sin(GamphiCM[Tagd2Idx[TagIdx]]),
				    Gamp3CM[Tagd2Idx[TagIdx]]*GamcosthCM[Tagd2Idx[TagIdx]],
				    GamenergyCM[Tagd2Idx[TagIdx]]);
      TLorentzVector DstarDauVectLab=DstarDauVectCM;DstarDauVectLab.Boost(eeVelVec);
      tagdstargamenergy=DstarDauVectLab.T();
    }
    tagdstardauangle=DstarDauVectCM.Vect().Unit()*DstarVectCM.Vect().Unit();
    
    //angle of pi/gam in D* frame
    DstarDauVectCM.Boost(-DstarVectCM.Vect()*(1./DstarVectCM.T()));
    tagdstardecayangle=DstarDauVectCM.Vect().Unit()*DstarVectCM.Vect().Unit();

    //switch index*******************
    TagIdx=Tagd1Idx[TagIdx]; 
    if(TagIdx<0||TagIdx>=NMAXTAG){cout<<" after D* switch TagIdx="<<TagIdx<<endl;return 0;}
  }




  ///Fill all necesary indexes used in this method
  d1Idx=Tagd1Idx[TagIdx];
  d2Idx=Tagd2Idx[TagIdx];   
  d3Idx=Tagd3Idx[TagIdx];
  d4Idx=Tagd4Idx[TagIdx];
  d5Idx=Tagd5Idx[TagIdx];
  if(d1Idx<0||d1Idx>=MAXNCANDS
     ||d2Idx<0||d2Idx>=MAXNCANDS
     ){cout<<"bad  Tag dau index d1="<<d1Idx<<" d2="<<d2Idx<<endl;return 0;}
     
  
  //-------TAG quantities
  Tagp3vecCM=TVector3(Tagp3CM[TagIdx]*sin(acos(TagcosthCM[TagIdx]))*cos(TagphiCM[TagIdx]),
		      Tagp3CM[TagIdx]*sin(acos(TagcosthCM[TagIdx]))*sin(TagphiCM[TagIdx]),
		      Tagp3CM[TagIdx]*TagcosthCM[TagIdx]);
  Tagp4CM=TLorentzVector(Tagp3vecCM,TagenergyCM[TagIdx]);
  Tagp4Lab=Tagp4CM;Tagp4Lab.Boost(eeVelVec);
  tagmass=TagMass[TagIdx];
  if(abs(TagLund[TagIdx])==myD0Lund)tagcharge=0;//add || Dst0 then do else below
  else tagcharge=TagLund[TagIdx]/abs(TagLund[TagIdx]);
  if(TMath::Prob(TagChi2[TagIdx],TagnDof[TagIdx])>1e-300)
    taglogvtxprob=log10(TMath::Prob(TagChi2[TagIdx],TagnDof[TagIdx]));
  else taglogvtxprob=-100;
  tagflights=TagFlightBSLen[TagIdx];
  tagenergycm=TagenergyCM[TagIdx];
  tagpstar=Tagp3CM[TagIdx];  
  tagndaus=TagnDaus[TagIdx];
  tagmctruebad=TagMCIdx[TagIdx];
  if(tagmctruebad>0)tagmctrueparent=mcLund[mothIdx[tagmctruebad]];else tagmctrueparent=0;
   
  //count daughter types
  Int_t tagnK=0;Int_t tagnPi=0;Int_t tagnPi0=0;Int_t tagnKs=0;
  Int_t tagnPhi=0, tagnRhoC=0, tagnRho0=0, tagnEta=0, tagnEtaP=0, tagnP=0, tagnLz=0, tagnSz=0, tagnKstar0=0;
  Int_t etaPidx=-2;
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

  if(abs(Tagd1Lund[TagIdx])==myKS0Lund) tagnKs++;
  if(abs(Tagd2Lund[TagIdx])==myKS0Lund) tagnKs++;
  if(abs(Tagd3Lund[TagIdx])==myKS0Lund) tagnKs++;
  if(abs(Tagd4Lund[TagIdx])==myKS0Lund) tagnKs++;
  if(abs(Tagd5Lund[TagIdx])==myKS0Lund) tagnKs++;
  
  if(abs(Tagd1Lund[TagIdx])==myPhiLund) tagnPhi++;
  if(abs(Tagd2Lund[TagIdx])==myPhiLund) tagnPhi++;
  if(abs(Tagd3Lund[TagIdx])==myPhiLund) tagnPhi++;
  if(abs(Tagd4Lund[TagIdx])==myPhiLund) tagnPhi++;
  if(abs(Tagd5Lund[TagIdx])==myPhiLund) tagnPhi++;
  
  if(abs(Tagd1Lund[TagIdx])==myRhoCLund) tagnRhoC++;
  if(abs(Tagd2Lund[TagIdx])==myRhoCLund) tagnRhoC++;
  if(abs(Tagd3Lund[TagIdx])==myRhoCLund) tagnRhoC++;
  if(abs(Tagd4Lund[TagIdx])==myRhoCLund) tagnRhoC++;
  if(abs(Tagd5Lund[TagIdx])==myRhoCLund) tagnRhoC++;

  if(abs(Tagd1Lund[TagIdx])==myRho0Lund) tagnRho0++;
  if(abs(Tagd2Lund[TagIdx])==myRho0Lund) tagnRho0++;
  if(abs(Tagd3Lund[TagIdx])==myRho0Lund) tagnRho0++;
  if(abs(Tagd4Lund[TagIdx])==myRho0Lund) tagnRho0++;
  if(abs(Tagd5Lund[TagIdx])==myRho0Lund) tagnRho0++;

  if(abs(Tagd1Lund[TagIdx])==myEtaLund) tagnEta++;
  if(abs(Tagd2Lund[TagIdx])==myEtaLund) tagnEta++;
  if(abs(Tagd3Lund[TagIdx])==myEtaLund) tagnEta++;
  if(abs(Tagd4Lund[TagIdx])==myEtaLund) tagnEta++;
  if(abs(Tagd5Lund[TagIdx])==myEtaLund) tagnEta++;

  if(abs(Tagd1Lund[TagIdx])==myEtaPLund) {tagnEtaP++;etaPidx=Tagd1Idx[TagIdx];}
  if(abs(Tagd2Lund[TagIdx])==myEtaPLund) {tagnEtaP++;etaPidx=Tagd2Idx[TagIdx];}
  if(abs(Tagd3Lund[TagIdx])==myEtaPLund) {tagnEtaP++;etaPidx=Tagd3Idx[TagIdx];}
  if(abs(Tagd4Lund[TagIdx])==myEtaPLund) {tagnEtaP++;etaPidx=Tagd4Idx[TagIdx];}
  if(abs(Tagd5Lund[TagIdx])==myEtaPLund) {tagnEtaP++;etaPidx=Tagd5Idx[TagIdx];}

  if(abs(Tagd1Lund[TagIdx])==myKstar0Lund) tagnKstar0++;
  if(abs(Tagd2Lund[TagIdx])==myKstar0Lund) tagnKstar0++;
  if(abs(Tagd3Lund[TagIdx])==myKstar0Lund) tagnKstar0++;
  if(abs(Tagd4Lund[TagIdx])==myKstar0Lund) tagnKstar0++;
  if(abs(Tagd5Lund[TagIdx])==myKstar0Lund) tagnKstar0++;

  if(abs(Tagd1Lund[TagIdx])==myPLund) tagnP++;
  if(abs(Tagd2Lund[TagIdx])==myPLund) tagnP++;
  if(abs(Tagd3Lund[TagIdx])==myPLund) tagnP++;
  if(abs(Tagd4Lund[TagIdx])==myPLund) tagnP++;
  if(abs(Tagd5Lund[TagIdx])==myPLund) tagnP++;

  if(abs(Tagd1Lund[TagIdx])==myLzLund) tagnLz++;
  if(abs(Tagd2Lund[TagIdx])==myLzLund) tagnLz++;
  if(abs(Tagd3Lund[TagIdx])==myLzLund) tagnLz++;
  if(abs(Tagd4Lund[TagIdx])==myLzLund) tagnLz++;
  if(abs(Tagd5Lund[TagIdx])==myLzLund) tagnLz++;

  if(abs(Tagd1Lund[TagIdx])==mySzLund) tagnSz++;
  if(abs(Tagd2Lund[TagIdx])==mySzLund) tagnSz++;
  if(abs(Tagd3Lund[TagIdx])==mySzLund) tagnSz++;
  if(abs(Tagd4Lund[TagIdx])==mySzLund) tagnSz++;
  if(abs(Tagd5Lund[TagIdx])==mySzLund) tagnSz++;
  taglund=0;
  tagmode=0;
  if(abs(TagLund[TagIdx])==myD0Lund){
    if( tagnKs==0 && tagnK==1 && tagnPi==1 && tagnPi0==0 ) tagmode=1 ; //Kpi        3.8
    if( tagnKs==0 && tagnK==1 && tagnPi==1 && tagnPi0==1 ) tagmode=2 ; //Kpipi0    14.1
    if( tagnKs==0 && tagnK==1 && tagnPi==3 && tagnPi0==0 ) tagmode=3 ; //K3pi       7.7
    if( tagnKs==1 && tagnK==0 && tagnPi==2 && tagnPi0==0 ) tagmode=4 ; //Kspipi     3.0
    if( tagnKs==0 && tagnK==1 && tagnPi==3 && tagnPi0==1 ) tagmode=5 ; //K3pipi0    4.1
    if( tagnKs==1 && tagnK==0 && tagnPi==2 && tagnPi0==1 ) tagmode=6 ; //Kspipipi0  5.3
    taglund=1*TagLund[TagIdx]/abs(TagLund[TagIdx]);
  }
  if(abs(TagLund[TagIdx])==myDPlusLund){
    if( tagnKs==0 && tagnK==1 && tagnPi==2 && tagnPi0==0 ) tagmode=1 ; //Kpipi      9.5
    if( tagnKs==0 && tagnK==1 && tagnPi==2 && tagnPi0==1 ) tagmode=2 ; //Kpipipi0   5.5
    if( tagnKs==1 && tagnK==0 && tagnPi==1 && tagnPi0==0 ) tagmode=3 ; //Kspi       1.47
    if( tagnKs==1 && tagnK==0 && tagnPi==1 && tagnPi0==1 ) tagmode=4 ; //Kspipi0    6.8
    if( tagnKs==1 && tagnK==0 && tagnPi==3 && tagnPi0==0 ) tagmode=5 ; //Ks3pi      3.1
    taglund=2*TagLund[TagIdx]/abs(TagLund[TagIdx]);
  }
  if(abs(TagLund[TagIdx])==myDsLund){
    if( tagnKs==1&&tagnK==1&&tagnPi==0&&tagnPhi==0&&tagnRhoC==0&&tagnRho0==0&&tagnEta==0&&tagnEtaP==0&&tagnKstar0==0) tagmode=1 ;//KsK
    if( tagnKs==0&&tagnK==0&&tagnPi==1&&tagnPhi==1&&tagnRhoC==0&&tagnRho0==0&&tagnEta==0&&tagnEtaP==0&&tagnKstar0==0) tagmode=2 ;//Phipi
    if( tagnKs==0&&tagnK==0&&tagnPi==0&&tagnPhi==1&&tagnRhoC==1&&tagnRho0==0&&tagnEta==0&&tagnEtaP==0&&tagnKstar0==0) tagmode=3 ;//PhiRhoC
    if( tagnKs==1&&tagnK==1&&tagnPi==2&&tagnPhi==0&&tagnRhoC==0&&tagnRho0==0&&tagnEta==0&&tagnEtaP==0&&tagnKstar0==0) tagmode=4 ; //KsKpipi
    if( tagnKs==0&&tagnK==0&&tagnPi==1&&tagnPhi==0&&tagnRhoC==0&&tagnRho0==0&&tagnEta==0&&tagnEtaP==1&&tagnKstar0==0) tagmode=5 ;//EtaPpi           
    if( tagnKs==0&&tagnK==0&&tagnPi==1&&tagnPhi==1&&tagnRhoC==0&&tagnRho0==1&&tagnEta==0&&tagnEtaP==0&&tagnKstar0==0) tagmode=6 ; //PhiRho0pi
    if( tagnKs==0&&tagnK==1&&tagnPi==0&&tagnPhi==0&&tagnRhoC==0&&tagnRho0==0&&tagnEta==0&&tagnEtaP==0&&tagnKstar0==1) tagmode=7 ; //KK*
    taglund=3*TagLund[TagIdx]/abs(TagLund[TagIdx]);
  }
  if(abs(TagLund[TagIdx])==myLambdaCLund){
    if( tagnKs==0&&tagnK==1&&tagnPi==1&&tagnPi0==0&&tagnP==1&&tagnLz==0&&tagnSz==0 ) tagmode=1 ; //pKpi
    if( tagnKs==0&&tagnK==1&&tagnPi==1&&tagnPi0==1&&tagnP==1&&tagnLz==0&&tagnSz==0 ) tagmode=2 ; //pKpipi (pKpipi0)
    if( tagnKs==1&&tagnK==0&&tagnPi==0&&tagnPi0==0&&tagnP==1&&tagnLz==0&&tagnSz==0 ) tagmode=3 ; //pKs
    if( tagnKs==1&&tagnK==0&&tagnPi==2&&tagnPi0==0&&tagnP==1&&tagnLz==0&&tagnSz==0 ) tagmode=4 ; //pKspipi
    if( tagnKs==0&&tagnK==0&&tagnPi==1&&tagnPi0==0&&tagnP==0&&tagnLz==1&&tagnSz==0 ) tagmode=5 ; //Lzpi
    if( tagnKs==0&&tagnK==0&&tagnPi==1&&tagnPi0==1&&tagnP==0&&tagnLz==1&&tagnSz==0 ) tagmode=6 ; //Lzpipi (Lzpipi0)
    if( tagnKs==0&&tagnK==0&&tagnPi==3&&tagnPi0==0&&tagnP==0&&tagnLz==1&&tagnSz==0 ) tagmode=7 ; //Lzpipipi
    if( tagnKs==0&&tagnK==0&&tagnPi==1&&tagnPi0==0&&tagnP==0&&tagnLz==0&&tagnSz==1 ) tagmode=8 ; //Szpi
    //if( tagnKs==0&&tagnK==0&&tagnPi==3&&tagnPi0==0&&tagnP==0&&tagnLz==0&&tagnSz==1 ) tagmode=9 ; //Szpipipi
    taglund=4*TagLund[TagIdx]/abs(TagLund[TagIdx]);
  }
  if(tagmode<=0||tagmode>10||abs(taglund)<=0||abs(taglund)>NTagTypes){
    cout<<"taglund "<<taglund<<" tagmode "<<tagmode<<" abs(TagLund[TagIdx]) "<<abs(TagLund[TagIdx])<<endl;
//     cout << "Invalid tagmode or taglund" << endl ;
//     cout << "nKs, nK, nK*0, nPi, nPi0, nPhi, nRhoC, nRho0, nEta, nEtaP, nP, nLz, nSz: " << tagnKs << " " << tagnK << " " << tagnKstar0 << " " << tagnPi << " " << tagnPi0 << " " << tagnPhi << " " << tagnRhoC << " " << tagnRho0 << " " << tagnEta << " " << tagnEtaP << " " << tagnP << " " << tagnLz << " " << tagnSz << endl ;
//     exit(0);
    return 0;
  }

  //keep flight significance of Ks in Tag-D  
  tagksflights=100.;
  tagksflightcos=1;
  tagksflightcosbeam=1;
  tagksgamconvmass=100.;
  if(tagnKs>0){
    Int_t KsIdx=-1;
    if(abs(Tagd1Lund[TagIdx])==myKS0Lund)KsIdx=Tagd1Idx[TagIdx];
    if(abs(Tagd2Lund[TagIdx])==myKS0Lund)KsIdx=Tagd2Idx[TagIdx];
    if(abs(Tagd3Lund[TagIdx])==myKS0Lund)KsIdx=Tagd3Idx[TagIdx];
    if(abs(Tagd4Lund[TagIdx])==myKS0Lund)KsIdx=Tagd4Idx[TagIdx];
    if(abs(Tagd5Lund[TagIdx])==myKS0Lund)KsIdx=Tagd5Idx[TagIdx];
    if(0<=KsIdx&&KsIdx<MAXNCANDS){
      //determine Ks flight significance
      tagksflights=KsFlightBSLen[KsIdx]/KsFlightBSErr[KsIdx]; 

      //determine the angle between Ks flight and Ks momentum
      TVector3 KsVertex(KsVtxx[KsIdx],KsVtxy[KsIdx],KsVtxz[KsIdx]);
      TVector3 TagVertex(TagVtxx[TagIdx],TagVtxy[TagIdx],TagVtxz[TagIdx]);
      TVector3 Ksp3vecCM(Ksp3CM[KsIdx]*sin(acos(KscosthCM[KsIdx]))*cos(KsphiCM[KsIdx]),
			 Ksp3CM[KsIdx]*sin(acos(KscosthCM[KsIdx]))*sin(KsphiCM[KsIdx]),
			 Ksp3CM[KsIdx]*KscosthCM[KsIdx]);
      TLorentzVector Ksp4(Ksp3vecCM,KsenergyCM[KsIdx]);
      Ksp4.Boost(eeVelVec);//boost into lab frame because vertexes are in lab    
      tagksflightcos=(KsVertex-TagVertex).Unit()*Ksp4.Vect().Unit();//origin defined by parent
      tagksflightcosbeam=(KsVertex-beamSpot).Unit()*Ksp4.Vect().Unit();//origin defined by beam spot
    
      //assign the pions the electron mass and check if they are conversions gamma->e+e-
      tagksgamconvmass=100.;
      if(Ksd1Idx[KsIdx]<MAXNCANDS&&Ksd2Idx[KsIdx]<MAXNCANDS){
	TVector3 Ksd1p3vecCM(Pip3CM[Ksd1Idx[KsIdx]]*sin(acos(PicosthCM[Ksd1Idx[KsIdx]]))*cos(PiphiCM[Ksd1Idx[KsIdx]]),
			     Pip3CM[Ksd1Idx[KsIdx]]*sin(acos(PicosthCM[Ksd1Idx[KsIdx]]))*sin(PiphiCM[Ksd1Idx[KsIdx]]),
			     Pip3CM[Ksd1Idx[KsIdx]]*PicosthCM[Ksd1Idx[KsIdx]]);
	TLorentzVector Ksd1p4(Ksd1p3vecCM,sqrt(ElectronPDGMass*ElectronPDGMass+Ksd1p3vecCM*Ksd1p3vecCM));
	TVector3 Ksd2p3vecCM(Pip3CM[Ksd2Idx[KsIdx]]*sin(acos(PicosthCM[Ksd2Idx[KsIdx]]))*cos(PiphiCM[Ksd2Idx[KsIdx]]),
			     Pip3CM[Ksd2Idx[KsIdx]]*sin(acos(PicosthCM[Ksd2Idx[KsIdx]]))*sin(PiphiCM[Ksd2Idx[KsIdx]]),
			     Pip3CM[Ksd2Idx[KsIdx]]*PicosthCM[Ksd2Idx[KsIdx]]);
	TLorentzVector Ksd2p4(Ksd2p3vecCM,sqrt(ElectronPDGMass*ElectronPDGMass+Ksd2p3vecCM*Ksd2p3vecCM));
	tagksgamconvmass=(Ksd1p4+Ksd2p4)*(Ksd1p4+Ksd2p4);
      }
    }

  }
  

  //define the K PID selector
  //NOTE that depending on whether KK is deemed to have gone through a phi or not ONLY ONE of
  //the following two if statements will be entered...
  
  tagpid=5;
  if(tagnK>0){  //this tag has a K track    
    Int_t ktrkidx=-1;//find the track index for the kaon
    if(abs(Tagd1Lund[TagIdx])==myKLund)ktrkidx=KTrkIdx[Tagd1Idx[TagIdx]];
    if(abs(Tagd2Lund[TagIdx])==myKLund)ktrkidx=KTrkIdx[Tagd2Idx[TagIdx]];
    if(abs(Tagd3Lund[TagIdx])==myKLund)ktrkidx=KTrkIdx[Tagd3Idx[TagIdx]];
    if(abs(Tagd4Lund[TagIdx])==myKLund)ktrkidx=KTrkIdx[Tagd4Idx[TagIdx]];
    if(abs(Tagd5Lund[TagIdx])==myKLund)ktrkidx=KTrkIdx[Tagd5Idx[TagIdx]];
    if(0<=ktrkidx&&ktrkidx<MAXNCANDS){
      tagpid=0;
      for(Int_t i=19;i<=23;i++){//NEW PID; 19=notpion,20=veryloose,21=loose,22=tight,23=verytight
	if((KSelectorsMap[ktrkidx] & (1<<i)) == (1<<i))
	  tagpid++;//if passes selector increase 
      }   
    }
  }
  //Or if we're looking at KKstar0
  if(tagnKstar0>0){  //this tag has a K*0 track    
    Int_t kstar0idx=-1;//find the track index for the K*0
    if(abs(Tagd1Lund[TagIdx])==myKstar0Lund)kstar0idx=Tagd1Idx[TagIdx];
    if(abs(Tagd2Lund[TagIdx])==myKstar0Lund)kstar0idx=Tagd2Idx[TagIdx];
    if(abs(Tagd3Lund[TagIdx])==myKstar0Lund)kstar0idx=Tagd3Idx[TagIdx];
    if(abs(Tagd4Lund[TagIdx])==myKstar0Lund)kstar0idx=Tagd4Idx[TagIdx];
    if(abs(Tagd5Lund[TagIdx])==myKstar0Lund)kstar0idx=Tagd5Idx[TagIdx];
    if(0<=kstar0idx&&kstar0idx<NMAXKSTAR0)
      if(0<=Kstar0d1Idx[kstar0idx]&&Kstar0d1Idx[kstar0idx]<NMAXK)
	if(0<=KTrkIdx[Kstar0d1Idx[kstar0idx]]&&KTrkIdx[Kstar0d1Idx[kstar0idx]]<MAXNCANDS){
	  Int_t dummy_tagkstar0pid=0;
	  for(Int_t i=19;i<=23;i++){//NEW PID; 19=notpion,20=veryloose,21=loose,22=tight,23=verytight
	    if((KSelectorsMap[KTrkIdx[Kstar0d1Idx[kstar0idx]]] & (1<<i)) == (1<<i))
	      dummy_tagkstar0pid++;//if passes selector increase 
	  }
	  if(dummy_tagkstar0pid<tagpid){tagpid=dummy_tagkstar0pid;}
	}
  }
  //
  //OR if the tag has a phi built from KK
  if(tagnPhi>0){  //this tag has a phi      
    Int_t phiidx=-1;//find the index for the phi
    if(abs(Tagd1Lund[TagIdx])==myPhiLund)phiidx=Tagd1Idx[TagIdx];
    if(abs(Tagd2Lund[TagIdx])==myPhiLund)phiidx=Tagd2Idx[TagIdx];
    if(abs(Tagd3Lund[TagIdx])==myPhiLund)phiidx=Tagd3Idx[TagIdx];
    if(abs(Tagd4Lund[TagIdx])==myPhiLund)phiidx=Tagd4Idx[TagIdx];
    if(abs(Tagd5Lund[TagIdx])==myPhiLund)phiidx=Tagd5Idx[TagIdx];
    if(0<=phiidx&&phiidx<NMAXPHI)
      if(Phid1Idx[phiidx]<NMAXK&&Phid2Idx[phiidx]<NMAXK)
	if(KTrkIdx[Phid1Idx[phiidx]]<MAXNCANDS&&KTrkIdx[Phid2Idx[phiidx]]<MAXNCANDS){
	  tagpid=0;
	  //initialize dummy tag counters
	  int dummy_tagpid_d1=0, dummy_tagpid_d2=0;
	  if(abs(taglund)==3&&tagmode==2){ //tagmode 2 is Ds->PhiPi, which uses NEW Kaon pid.
	    //find out what pid requirements the first daughter will pass
	    for(Int_t i=19;i<=23;i++){//NEW PID; 19=notpion,20=veryloose,21=loose,22=tight,23=verytight
	      if((KSelectorsMap[KTrkIdx[Phid1Idx[phiidx]]] & (1<<i)) == (1<<i))
		dummy_tagpid_d1++;//if passes selector increase 
	      if((KSelectorsMap[KTrkIdx[Phid2Idx[phiidx]]] & (1<<i)) == (1<<i))
		dummy_tagpid_d2++;//if passes selector increase 
	    } 
	    //check which of the phi's two kaon daughters passed the fewest pid selectors and give that to 'tagpid'
	    (dummy_tagpid_d1>dummy_tagpid_d2) ? tagpid=dummy_tagpid_d2 : tagpid=dummy_tagpid_d1 ;
	  }else if(abs(taglund)==3&&(tagmode==3||tagmode==6)){ //tagmode 3 is Ds->PhiRhoC, which uses OLD Kaon pid.
	    //find out what pid requirements the first daughter will pass
	    for(Int_t i=10;i<=14;i++){//OLD PID; 10=notpion,11=veryloose,12=loose,13=tight,14=verytight
	      if((KSelectorsMap[KTrkIdx[Phid1Idx[phiidx]]] & (1<<i)) == (1<<i))
		dummy_tagpid_d1++;//if passes selector increase 
	      if((KSelectorsMap[KTrkIdx[Phid2Idx[phiidx]]] & (1<<i)) == (1<<i))
		dummy_tagpid_d2++;//if passes selector increase 
	    } 
	    //check which of the phi's two kaon daughters passed the fewest pid selectors and give that to 'tagpid'
	    (dummy_tagpid_d1>dummy_tagpid_d2) ? tagpid=dummy_tagpid_d2 : tagpid=dummy_tagpid_d1 ;	
	  } else {
	    cout<<"ERROR - There's a tag mode containing a "<<tagnPhi<<" phi(s) that has not been accounted for!"<<endl;
	  }
	}
  }


  //pion selector ; only uses the top pion daughters of the Tag
  tagpipid=6;
  if(tagnPi>0){//this tag has a pi track    
    Int_t tagdidx[5]={Tagd1Idx[TagIdx],Tagd2Idx[TagIdx],Tagd3Idx[TagIdx],Tagd4Idx[TagIdx],Tagd5Idx[TagIdx]};
    Int_t tagdlund[5]={Tagd1Lund[TagIdx],Tagd2Lund[TagIdx],Tagd3Lund[TagIdx],Tagd4Lund[TagIdx],Tagd5Lund[TagIdx]};
    for(Int_t d=0;d<5;d++){
      if(abs(tagdlund[d])==myPiLund)
	if(tagdidx[d]<NMAXPI)
	  if(0<=PiTrkIdx[tagdidx[d]]&&PiTrkIdx[tagdidx[d]]<MAXNCANDS){
	    if((0<=tagdidx[d]&&tagdidx[d]<MAXNCANDS)&&(0<=PiTrkIdx[tagdidx[d]]&&PiTrkIdx[tagdidx[d]]<MAXNCANDS)){
	      Int_t tagpipidtemp=0;
	      for(Int_t i=10;i<=15;i++) {//piKM; 10=SuperLoose,11=VL,12=L,13=T,14=VT,15=SuperTight
		if((piSelectorsMap[PiTrkIdx[tagdidx[d]]] & (1<<i)) == (1<<i)){tagpipidtemp++;}//if passes selector increase
	      }
	      if(tagpipidtemp<tagpipid){tagpipid=tagpipidtemp;}
	    }
	  }
    }
  }


  //define the p PID selector
  tagppid=6;
  if(tagnP>0){  //this tag has a p track    
    Int_t ptrkidx=-1;//find the track index for the proton
    if(abs(Tagd1Lund[TagIdx])==myPLund)ptrkidx=PTrkIdx[Tagd1Idx[TagIdx]];
    if(abs(Tagd2Lund[TagIdx])==myPLund)ptrkidx=PTrkIdx[Tagd2Idx[TagIdx]];
    if(abs(Tagd3Lund[TagIdx])==myPLund)ptrkidx=PTrkIdx[Tagd3Idx[TagIdx]];
    if(abs(Tagd4Lund[TagIdx])==myPLund)ptrkidx=PTrkIdx[Tagd4Idx[TagIdx]];
    if(abs(Tagd5Lund[TagIdx])==myPLund)ptrkidx=PTrkIdx[Tagd5Idx[TagIdx]];
    if(0<=ptrkidx&&ptrkidx<MAXNCANDS){
      tagppid=0;
      for(Int_t i=12;i<=17;i++){//(NEW) PID; 12=superloose,13=veryloose,14=loose,15=tight,16=verytight,17=supertight
	if((pSelectorsMap[ptrkidx] & (1<<i)) == (1<<i))
	  tagppid++;//if passes selector increase 
      }
    }
  }
  //OR if the tag has a lambda0 built from p pi
  if(tagnLz>0){  //this tag has a lambda0
    Int_t lambda0idx=-1;//find the index for the lambda0
    if(abs(Tagd1Lund[TagIdx])==myLzLund)lambda0idx=Tagd1Idx[TagIdx];
    if(abs(Tagd2Lund[TagIdx])==myLzLund)lambda0idx=Tagd2Idx[TagIdx];
    if(abs(Tagd3Lund[TagIdx])==myLzLund)lambda0idx=Tagd3Idx[TagIdx];
    if(abs(Tagd4Lund[TagIdx])==myLzLund)lambda0idx=Tagd4Idx[TagIdx];
    if(abs(Tagd5Lund[TagIdx])==myLzLund)lambda0idx=Tagd5Idx[TagIdx];
    if(0<=lambda0idx&&lambda0idx<NMAXLAM0)
      if(Lam0d1Idx[lambda0idx]<NMAXP)
	if(PTrkIdx[Lam0d1Idx[lambda0idx]]<MAXNCANDS){
	  tagppid=0;
	  for(Int_t i=0;i<=3;i++){//(OLD) PID pLH; 1=VL, 0=L, 2=T, 3=VT;
	    //Note that the first daughter of the lambda0 is the proton
	    if((pSelectorsMap[PTrkIdx[Lam0d1Idx[lambda0idx]]] & (1<<i)) == (1<<i))
	      tagppid++;//if passes selector increase 
	  }
	}
  }


  //for tags with pi0's keep energy of lower gamma
  tagpi0gamenergy=100.;
  if(tagnPi0>0){
    Int_t tagpi0idx=-1;
    if(abs(Tagd1Lund[TagIdx])==myPi0Lund)tagpi0idx=Tagd1Idx[TagIdx];
    if(abs(Tagd2Lund[TagIdx])==myPi0Lund)tagpi0idx=Tagd2Idx[TagIdx];
    if(abs(Tagd3Lund[TagIdx])==myPi0Lund)tagpi0idx=Tagd3Idx[TagIdx];
    if(abs(Tagd4Lund[TagIdx])==myPi0Lund)tagpi0idx=Tagd4Idx[TagIdx];  
    if(abs(Tagd5Lund[TagIdx])==myPi0Lund)tagpi0idx=Tagd5Idx[TagIdx];   
    if(0<=tagpi0idx&&tagpi0idx<NMAXPI0){
      Int_t tagpi0gam1idx=Pi0d1Idx[tagpi0idx];
      Int_t tagpi0gam2idx=Pi0d2Idx[tagpi0idx];
      if(tagpi0gam1idx<NMAXGAM&&tagpi0gam2idx<NMAXGAM){
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



  //Jose(11/25/2009): commented out this code because it was crashing
  //for tags with rho+'s keep energy of lower gamma from it's pi0 daughter
  //tagpi0gamenergy=100.; //don't need to define this twice!
//   if(tagnRhoC>0){
//     Int_t tagrhoCidx=-1;
//     if(abs(Tagd1Lund[TagIdx])==myRhoCLund)tagrhoCidx=Tagd1Idx[TagIdx];
//     if(abs(Tagd2Lund[TagIdx])==myRhoCLund)tagrhoCidx=Tagd2Idx[TagIdx];
//     if(abs(Tagd3Lund[TagIdx])==myRhoCLund)tagrhoCidx=Tagd3Idx[TagIdx];
//     if(abs(Tagd4Lund[TagIdx])==myRhoCLund)tagrhoCidx=Tagd4Idx[TagIdx];  
//     if(abs(Tagd5Lund[TagIdx])==myRhoCLund)tagrhoCidx=Tagd5Idx[TagIdx];   
//     if(0<=tagrhoCidx&&tagrhoCidx<NMAXRHOC){
//       Int_t tagpi0idx=-1;
//       if(abs(RhoCd1Lund[tagrhoCidx])==myPi0Lund)tagpi0idx=RhoCd1Idx[tagrhoCidx];
//       if(abs(RhoCd2Lund[tagrhoCidx])==myPi0Lund)tagpi0idx=RhoCd2Idx[tagrhoCidx];
//       if(0<=tagpi0idx&&tagpi0idx<NMAXPI0){
// 	Int_t tagpi0gam1idx=Pi0d1Idx[tagpi0idx];
// 	Int_t tagpi0gam2idx=Pi0d1Idx[tagpi0idx];
// 	if(tagpi0gam1idx<NMAXGAM&&tagpi0gam2idx<NMAXGAM){
// 	  TVector3 Gam1p3VCM(Gamp3CM[tagpi0gam1idx]*sin(acos(GamcosthCM[tagpi0gam1idx]))*cos(GamphiCM[tagpi0gam1idx]),
// 			     Gamp3CM[tagpi0gam1idx]*sin(acos(GamcosthCM[tagpi0gam1idx]))*sin(GamphiCM[tagpi0gam1idx]),
// 			     Gamp3CM[tagpi0gam1idx]*GamcosthCM[tagpi0gam1idx]);
// 	  TLorentzVector Gam1p4(Gam1p3VCM,GamenergyCM[tagpi0gam1idx]);
// 	  Gam1p4.Boost(eeVelVec);
	  
// 	  TVector3 Gam2p3VCM(Gamp3CM[tagpi0gam2idx]*sin(acos(GamcosthCM[tagpi0gam2idx]))*cos(GamphiCM[tagpi0gam2idx]),
// 			     Gamp3CM[tagpi0gam2idx]*sin(acos(GamcosthCM[tagpi0gam2idx]))*sin(GamphiCM[tagpi0gam2idx]),
// 			     Gamp3CM[tagpi0gam2idx]*GamcosthCM[tagpi0gam2idx]);
// 	  TLorentzVector Gam2p4(Gam2p3VCM,GamenergyCM[tagpi0gam2idx]);
// 	  Gam2p4.Boost(eeVelVec);
// 	  tagpi0gamenergy=Gam1p4.T();
// 	  if(Gam2p4.T()<tagpi0gamenergy)tagpi0gamenergy=Gam2p4.T();//pick the smallest one
// 	}
//       }
//     }       
//   }
  
//   //for tags with an eta keep energy of lower gamma
//   tagetagamenergy=100.;
//   if(tagnEta>0){
//     Int_t tagetaidx=-1;
//     if(abs(Tagd1Lund[TagIdx])==myEtaLund)tagetaidx=Tagd1Idx[TagIdx];
//     if(abs(Tagd2Lund[TagIdx])==myEtaLund)tagetaidx=Tagd2Idx[TagIdx];
//     if(abs(Tagd3Lund[TagIdx])==myEtaLund)tagetaidx=Tagd3Idx[TagIdx];
//     if(abs(Tagd4Lund[TagIdx])==myEtaLund)tagetaidx=Tagd4Idx[TagIdx];  
//     if(abs(Tagd5Lund[TagIdx])==myEtaLund)tagetaidx=Tagd5Idx[TagIdx];   
//     if(0<=tagetaidx&&tagetaidx<NMAXETA){
//       Int_t tagetagam1idx=Etad1Idx[tagetaidx];
//       Int_t tagetagam2idx=Etad2Idx[tagetaidx];
//       if(tagetagam1idx<NMAXGAM&&tagetagam2idx<NMAXGAM){
// 	TVector3 Gam1p3VCM(Gamp3CM[tagetagam1idx]*sin(acos(GamcosthCM[tagetagam1idx]))*cos(GamphiCM[tagetagam1idx]),
// 			   Gamp3CM[tagetagam1idx]*sin(acos(GamcosthCM[tagetagam1idx]))*sin(GamphiCM[tagetagam1idx]),
// 			   Gamp3CM[tagetagam1idx]*GamcosthCM[tagetagam1idx]);
// 	TLorentzVector Gam1p4(Gam1p3VCM,GamenergyCM[tagetagam1idx]);
// 	Gam1p4.Boost(eeVelVec);

// 	TVector3 Gam2p3VCM(Gamp3CM[tagetagam2idx]*sin(acos(GamcosthCM[tagetagam2idx]))*cos(GamphiCM[tagetagam2idx]),
// 			   Gamp3CM[tagetagam2idx]*sin(acos(GamcosthCM[tagetagam2idx]))*sin(GamphiCM[tagetagam2idx]),
// 			   Gamp3CM[tagetagam2idx]*GamcosthCM[tagetagam2idx]);
// 	TLorentzVector Gam2p4(Gam2p3VCM,GamenergyCM[tagetagam2idx]);
// 	Gam2p4.Boost(eeVelVec);
// 	tagetagamenergy=Gam1p4.T();
// 	if(Gam2p4.T()<tagetagamenergy)tagetagamenergy=Gam2p4.T();//pick the smallest one
//       }
//     } 
//   }
  //OR for tags with an eta' keep energy of lower gamma from it's eta daughter
//   if(tagnEtaP>0){
//     Int_t tagetapidx=-1;
//     if(abs(Tagd1Lund[TagIdx])==myEtaPLund)tagetapidx=Tagd1Idx[TagIdx];
//     if(abs(Tagd2Lund[TagIdx])==myEtaPLund)tagetapidx=Tagd2Idx[TagIdx];
//     if(abs(Tagd3Lund[TagIdx])==myEtaPLund)tagetapidx=Tagd3Idx[TagIdx];
//     if(abs(Tagd4Lund[TagIdx])==myEtaPLund)tagetapidx=Tagd4Idx[TagIdx];  
//     if(abs(Tagd5Lund[TagIdx])==myEtaPLund)tagetapidx=Tagd5Idx[TagIdx];   
//     if(0<=tagetapidx&&tagetapidx<NMAXETAP){
//       Int_t tagetagam1idx=Etad1Idx[EtaPd1Idx[tagetapidx]];
//       Int_t tagetagam2idx=Etad2Idx[EtaPd1Idx[tagetapidx]];
//       if(tagetagam1idx<NMAXGAM&&tagetagam2idx<NMAXGAM){
// 	TVector3 Gam1p3VCM(Gamp3CM[tagetagam1idx]*sin(acos(GamcosthCM[tagetagam1idx]))*cos(GamphiCM[tagetagam1idx]),
// 			   Gamp3CM[tagetagam1idx]*sin(acos(GamcosthCM[tagetagam1idx]))*sin(GamphiCM[tagetagam1idx]),
// 			   Gamp3CM[tagetagam1idx]*GamcosthCM[tagetagam1idx]);
// 	TLorentzVector Gam1p4(Gam1p3VCM,GamenergyCM[tagetagam1idx]);
// 	Gam1p4.Boost(eeVelVec);

// 	TVector3 Gam2p3VCM(Gamp3CM[tagetagam2idx]*sin(acos(GamcosthCM[tagetagam2idx]))*cos(GamphiCM[tagetagam2idx]),
// 			   Gamp3CM[tagetagam2idx]*sin(acos(GamcosthCM[tagetagam2idx]))*sin(GamphiCM[tagetagam2idx]),
// 			   Gamp3CM[tagetagam2idx]*GamcosthCM[tagetagam2idx]);
// 	TLorentzVector Gam2p4(Gam2p3VCM,GamenergyCM[tagetagam2idx]);
// 	Gam2p4.Boost(eeVelVec);
// 	tagetagamenergy=Gam1p4.T();
// 	if(Gam2p4.T()<tagetagamenergy)tagetagamenergy=Gam2p4.T();//pick the smallest one
//       }
//     } 
//   }


  return 1;
}




Bool_t DRecoTagAnalysis::TagPassesCuts(){
  Bool_t pass=1;
  
  //Selection cuts
  //if(cuttagmass==1)if(fabs(tagmass-TagMean[abs(taglund)-1][tagmode-1])>TagWidth[abs(taglund)-1][tagmode-1])pass=0;//problem with D*'s
  if(cuttagpstar==1)if(tagpstar<2.)pass=0;
  if(cuttagvtxprob==1)if(taglogvtxprob<-5.)pass=0;
  if(cuttagmode>0)if(((abs(taglund)-1)*10+tagmode)!=cuttagmode)pass=0;
  //Ks default selection cuts
  //???if( tagksflights!=100 || tagksflightcos!=1  tagksflightcosbeam!=1 && tagksgamconvmass!=100.){
    if(tagksflightcos<0||tagksflightcosbeam<0||tagksflights<10.)pass=0;
    //}

  return pass;
}


Int_t DRecoTagAnalysis::SetTagBranches(){
  if(_fChain==NULL){cout<<"No Chain"<<endl;return 0;} 
  
  
  if(_fChain->GetBranch("nTag"))_fChain->SetBranchAddress("nTag",&nTag);
  if(_fChain->GetBranch("TagMass"))_fChain->SetBranchAddress("TagMass",TagMass);
  if(_fChain->GetBranch("Tagp3CM"))_fChain->SetBranchAddress("Tagp3CM",Tagp3CM); 
  if(_fChain->GetBranch("TagcosthCM"))_fChain->SetBranchAddress("TagcosthCM",TagcosthCM); 
  if(_fChain->GetBranch("TagphiCM"))_fChain->SetBranchAddress("TagphiCM",TagphiCM);  
  if(_fChain->GetBranch("TagenergyCM"))_fChain->SetBranchAddress("TagenergyCM",TagenergyCM);   
  if(_fChain->GetBranch("TagLund"))_fChain->SetBranchAddress("TagLund",TagLund);
  if(_fChain->GetBranch("TagnDaus"))_fChain->SetBranchAddress("TagnDaus",TagnDaus);
  if(_fChain->GetBranch("TagMCIdx"))_fChain->SetBranchAddress("TagMCIdx",TagMCIdx);
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
  if(_fChain->GetBranch("TagFlightBSLen"))_fChain->SetBranchAddress("TagFlightBSLen",TagFlightBSLen);
  
  if(_fChain->GetBranch("nPi"))_fChain->SetBranchAddress("nPi",&nPi);
  if(_fChain->GetBranch("PienergyCM"))_fChain->SetBranchAddress("PienergyCM",PienergyCM);
  if(_fChain->GetBranch("Pip3CM"))_fChain->SetBranchAddress("Pip3CM",Pip3CM);
  if(_fChain->GetBranch("PicosthCM"))_fChain->SetBranchAddress("PicosthCM",PicosthCM);
  if(_fChain->GetBranch("PiphiCM"))_fChain->SetBranchAddress("PiphiCM",PiphiCM);
  if(_fChain->GetBranch("PiLund"))_fChain->SetBranchAddress("PiLund",PiLund);
  if(_fChain->GetBranch("PiTrkIdx"))_fChain->SetBranchAddress("PiTrkIdx",PiTrkIdx);
  if(_fChain->GetBranch("PiMCIdx"))_fChain->SetBranchAddress("PiMCIdx",PiMCIdx);

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
  if(_fChain->GetBranch("GamLat"))_fChain->SetBranchAddress("GamLat",GamLat);
  if(_fChain->GetBranch("GamA42"))_fChain->SetBranchAddress("GamA42",GamA42);
  if(_fChain->GetBranch("GamnCrys"))_fChain->SetBranchAddress("GamnCrys",GamnCrys);
  if(_fChain->GetBranch("GamClosestBumpDist"))_fChain->SetBranchAddress("GamClosestBumpDist",GamClosestBumpDist);
  if(_fChain->GetBranch("GamClosestTrkDist"))_fChain->SetBranchAddress("GamClosestTrkDist",GamClosestTrkDist);

  if(_fChain->GetBranch("nKs"))_fChain->SetBranchAddress("nKs",&nKs);
  if(_fChain->GetBranch("KsMass"))_fChain->SetBranchAddress("KsMass",KsMass);
  if(_fChain->GetBranch("KsenergyCM"))_fChain->SetBranchAddress("KsenergyCM",KsenergyCM);
  if(_fChain->GetBranch("Ksp3CM"))_fChain->SetBranchAddress("Ksp3CM",Ksp3CM);
  if(_fChain->GetBranch("KscosthCM"))_fChain->SetBranchAddress("KscosthCM",KscosthCM);
  if(_fChain->GetBranch("KsphiCM"))_fChain->SetBranchAddress("KsphiCM",KsphiCM);
  if(_fChain->GetBranch("KsLund"))_fChain->SetBranchAddress("KsLund",KsLund);
  if(_fChain->GetBranch("KsVtxx"))_fChain->SetBranchAddress("KsVtxx",KsVtxx);
  if(_fChain->GetBranch("KsVtxy"))_fChain->SetBranchAddress("KsVtxy",KsVtxy);
  if(_fChain->GetBranch("KsVtxz"))_fChain->SetBranchAddress("KsVtxz",KsVtxz);
  if(_fChain->GetBranch("KsFlightBSLen"))_fChain->SetBranchAddress("KsFlightBSLen",KsFlightBSLen);
  if(_fChain->GetBranch("KsFlightBSErr"))_fChain->SetBranchAddress("KsFlightBSErr",KsFlightBSErr);
  if(_fChain->GetBranch("Ksd1Lund"))_fChain->SetBranchAddress("Ksd1Lund",Ksd1Lund);
  if(_fChain->GetBranch("Ksd1Idx"))_fChain->SetBranchAddress("Ksd1Idx",Ksd1Idx);
  if(_fChain->GetBranch("Ksd2Lund"))_fChain->SetBranchAddress("Ksd2Lund",Ksd2Lund);
  if(_fChain->GetBranch("Ksd2Idx"))_fChain->SetBranchAddress("Ksd2Idx",Ksd2Idx);
  if(_fChain->GetBranch("KsMCIdx"))_fChain->SetBranchAddress("KsMCIdx",KsMCIdx);

  if(_fChain->GetBranch("nK"))_fChain->SetBranchAddress("nK",&nK);
  if(_fChain->GetBranch("Kp3CM"))_fChain->SetBranchAddress("Kp3CM",Kp3CM);
  if(_fChain->GetBranch("KenergyCM"))_fChain->SetBranchAddress("KenergyCM",KenergyCM);
  if(_fChain->GetBranch("KcosthCM"))_fChain->SetBranchAddress("KcosthCM",KcosthCM);
  if(_fChain->GetBranch("KphiCM"))_fChain->SetBranchAddress("KphiCM",KphiCM);
  if(_fChain->GetBranch("KLund"))_fChain->SetBranchAddress("KLund",KLund);
  if(_fChain->GetBranch("KTrkIdx"))_fChain->SetBranchAddress("KTrkIdx",KTrkIdx);
  if(_fChain->GetBranch("KMCIdx"))_fChain->SetBranchAddress("KMCIdx",KMCIdx);  

  if(_fChain->GetBranch("nEta"))_fChain->SetBranchAddress("nEta",&nEta);  
  if(_fChain->GetBranch("EtaMass"))_fChain->SetBranchAddress("EtaMass",EtaMass);  
  if(_fChain->GetBranch("EtaenergyCM"))_fChain->SetBranchAddress("EtaenergyCM",EtaenergyCM);  
  if(_fChain->GetBranch("Etap3CM"))_fChain->SetBranchAddress("Etap3CM",Etap3CM);  
  if(_fChain->GetBranch("EtacosthCM"))_fChain->SetBranchAddress("EtacosthCM",EtacosthCM);  
  if(_fChain->GetBranch("EtaphiCM"))_fChain->SetBranchAddress("EtaphiCM",EtaphiCM);  
  if(_fChain->GetBranch("EtaLund"))_fChain->SetBranchAddress("EtaLund",EtaLund);  
  if(_fChain->GetBranch("EtaMCIdx"))_fChain->SetBranchAddress("EtaMCIdx",EtaMCIdx);  
  if(_fChain->GetBranch("Etad1Lund"))_fChain->SetBranchAddress("Etad1Lund",Etad1Lund);  
  if(_fChain->GetBranch("Etad1Idx"))_fChain->SetBranchAddress("Etad1Idx",Etad1Idx);  
  if(_fChain->GetBranch("Etad2Lund"))_fChain->SetBranchAddress("Etad2Lund",Etad2Lund);  
  if(_fChain->GetBranch("Etad2Idx"))_fChain->SetBranchAddress("Etad2Idx",Etad2Idx);  
  if(_fChain->GetBranch("Etad3Lund"))_fChain->SetBranchAddress("Etad3Lund",Etad3Lund);  
  if(_fChain->GetBranch("Etad3Idx"))_fChain->SetBranchAddress("Etad3Idx",Etad3Idx);  
  if(_fChain->GetBranch("EtanDaus"))_fChain->SetBranchAddress("EtanDaus",EtanDaus);  

  if(_fChain->GetBranch("nEtaP"))_fChain->SetBranchAddress("nEtaP",&nEtaP);  
  if(_fChain->GetBranch("EtaPMass"))_fChain->SetBranchAddress("EtaPMass",EtaPMass);  
  if(_fChain->GetBranch("EtaPenergyCM"))_fChain->SetBranchAddress("EtaPenergyCM",EtaPenergyCM);  
  if(_fChain->GetBranch("EtaPp3CM"))_fChain->SetBranchAddress("EtaPp3CM",EtaPp3CM);  
  if(_fChain->GetBranch("EtaPcosthCM"))_fChain->SetBranchAddress("EtaPcosthCM",EtaPcosthCM);  
  if(_fChain->GetBranch("EtaPphiCM"))_fChain->SetBranchAddress("EtaPphiCM",EtaPphiCM);  
  if(_fChain->GetBranch("EtaPLund"))_fChain->SetBranchAddress("EtaPLund",EtaPLund); 
  if(_fChain->GetBranch("EtaPMCIdx"))_fChain->SetBranchAddress("EtaPMCIdx",EtaPMCIdx);  
  if(_fChain->GetBranch("EtaPd1Lund"))_fChain->SetBranchAddress("EtaPd1Lund",EtaPd1Lund);  
  if(_fChain->GetBranch("EtaPd1Idx"))_fChain->SetBranchAddress("EtaPd1Idx",EtaPd1Idx);  
  if(_fChain->GetBranch("EtaPd2Lund"))_fChain->SetBranchAddress("EtaPd2Lund",EtaPd2Lund);  
  if(_fChain->GetBranch("EtaPd2Idx"))_fChain->SetBranchAddress("EtaPd2Idx",EtaPd2Idx);  
  if(_fChain->GetBranch("EtaPd3Lund"))_fChain->SetBranchAddress("EtaPd3Lund",EtaPd3Lund);  
  if(_fChain->GetBranch("EtaPd3Idx"))_fChain->SetBranchAddress("EtaPd3Idx",EtaPd3Idx);  
  if(_fChain->GetBranch("EtaPnDaus"))_fChain->SetBranchAddress("EtaPnDaus",EtaPnDaus);  
 
  if(_fChain->GetBranch("nRhoC"))_fChain->SetBranchAddress("nRhoC",&nRhoC); 
  if(_fChain->GetBranch("RhoCMass"))_fChain->SetBranchAddress("RhoCMass",RhoCMass); 
  if(_fChain->GetBranch("RhoCp3CM"))_fChain->SetBranchAddress("RhoCp3CM",RhoCp3CM); 
  if(_fChain->GetBranch("RhoCenergyCM"))_fChain->SetBranchAddress("RhoCenergyCM",RhoCenergyCM); 
  if(_fChain->GetBranch("RhoCcosthCM"))_fChain->SetBranchAddress("RhoCcosthCM",RhoCcosthCM); 
  if(_fChain->GetBranch("RhoCphiCM"))_fChain->SetBranchAddress("RhoCphiCM",RhoCphiCM); 
  if(_fChain->GetBranch("RhoCLund"))_fChain->SetBranchAddress("RhoCLund",RhoCLund);
  if(_fChain->GetBranch("RhoCMCIdx"))_fChain->SetBranchAddress("RhoCMCIdx",RhoCMCIdx);  
  if(_fChain->GetBranch("RhoCd1Lund"))_fChain->SetBranchAddress("RhoCd1Lund",RhoCd1Lund); 
  if(_fChain->GetBranch("RhoCd1Idx"))_fChain->SetBranchAddress("RhoCd1Idx",RhoCd1Idx); 
  if(_fChain->GetBranch("RhoCd2Lund"))_fChain->SetBranchAddress("RhoCd2Lund",RhoCd2Lund); 
  if(_fChain->GetBranch("RhoCd2Idx"))_fChain->SetBranchAddress("RhoCd2Idx",RhoCd2Idx); 

  if(_fChain->GetBranch("nRho0"))_fChain->SetBranchAddress("nRho0",&nRho0); 
  if(_fChain->GetBranch("Rho0Mass"))_fChain->SetBranchAddress("Rho0Mass",Rho0Mass); 
  if(_fChain->GetBranch("Rho0p3CM"))_fChain->SetBranchAddress("Rho0p3CM",Rho0p3CM); 
  if(_fChain->GetBranch("Rho0energyCM"))_fChain->SetBranchAddress("Rho0energyCM",Rho0energyCM); 
  if(_fChain->GetBranch("Rho0costhCM"))_fChain->SetBranchAddress("Rho0costhCM",Rho0costhCM); 
  if(_fChain->GetBranch("Rho0phiCM"))_fChain->SetBranchAddress("Rho0phiCM",Rho0phiCM); 
  if(_fChain->GetBranch("Rho0Lund"))_fChain->SetBranchAddress("Rho0Lund",Rho0Lund); 
  if(_fChain->GetBranch("Rho0MCIdx"))_fChain->SetBranchAddress("Rho0MCIdx",Rho0MCIdx);  
  if(_fChain->GetBranch("Rho0d1Lund"))_fChain->SetBranchAddress("Rho0d1Lund",Rho0d1Lund); 
  if(_fChain->GetBranch("Rho0d1Idx"))_fChain->SetBranchAddress("Rho0d1Idx",Rho0d1Idx); 
  if(_fChain->GetBranch("Rho0d2Lund"))_fChain->SetBranchAddress("Rho0d2Lund",Rho0d2Lund); 
  if(_fChain->GetBranch("Rho0d2Idx"))_fChain->SetBranchAddress("Rho0d2Idx",Rho0d2Idx); 

  if(_fChain->GetBranch("nPhi"))_fChain->SetBranchAddress("nPhi",&nPhi); 
  if(_fChain->GetBranch("PhiMass"))_fChain->SetBranchAddress("PhiMass",PhiMass); 
  if(_fChain->GetBranch("Phip3CM"))_fChain->SetBranchAddress("Phip3CM",Phip3CM); 
  if(_fChain->GetBranch("PhienergyCM"))_fChain->SetBranchAddress("PhienergyCM",PhienergyCM); 
  if(_fChain->GetBranch("PhicosthCM"))_fChain->SetBranchAddress("PhicosthCM",PhicosthCM); 
  if(_fChain->GetBranch("PhiphiCM"))_fChain->SetBranchAddress("PhiphiCM",PhiphiCM); 
  if(_fChain->GetBranch("PhiLund"))_fChain->SetBranchAddress("PhiLund",PhiLund); 
  if(_fChain->GetBranch("PhiMCIdx"))_fChain->SetBranchAddress("PhiMCIdx",PhiMCIdx);  
  if(_fChain->GetBranch("Phid1Lund"))_fChain->SetBranchAddress("Phid1Lund",Phid1Lund); 
  if(_fChain->GetBranch("Phid1Idx"))_fChain->SetBranchAddress("Phid1Idx",Phid1Idx); 
  if(_fChain->GetBranch("Phid2Lund"))_fChain->SetBranchAddress("Phid2Lund",Phid2Lund); 
  if(_fChain->GetBranch("Phid2Idx"))_fChain->SetBranchAddress("Phid2Idx",Phid2Idx); 

  if(_fChain->GetBranch("nKstar0"))_fChain->SetBranchAddress("nKstar0",&nKstar0); 
  if(_fChain->GetBranch("Kstar0Mass"))_fChain->SetBranchAddress("Kstar0Mass",Kstar0Mass); 
  if(_fChain->GetBranch("Kstar0p3CM"))_fChain->SetBranchAddress("Kstar0p3CM",Kstar0p3CM); 
  if(_fChain->GetBranch("Kstar0energyCM"))_fChain->SetBranchAddress("Kstar0energyCM",Kstar0energyCM); 
  if(_fChain->GetBranch("Kstar0costhCM"))_fChain->SetBranchAddress("Kstar0costhCM",Kstar0costhCM); 
  if(_fChain->GetBranch("Kstar0phiCM"))_fChain->SetBranchAddress("Kstar0phiCM",Kstar0phiCM); 
  if(_fChain->GetBranch("Kstar0Lund"))_fChain->SetBranchAddress("Kstar0Lund",Kstar0Lund); 
  if(_fChain->GetBranch("Kstar0MCIdx"))_fChain->SetBranchAddress("Kstar0MCIdx",Kstar0MCIdx); 
  if(_fChain->GetBranch("Kstar0d1Lund"))_fChain->SetBranchAddress("Kstar0d1Lund",Kstar0d1Lund); 
  if(_fChain->GetBranch("Kstar0d1Idx"))_fChain->SetBranchAddress("Kstar0d1Idx",Kstar0d1Idx); 
  if(_fChain->GetBranch("Kstar0d2Lund"))_fChain->SetBranchAddress("Kstar0d2Lund",Kstar0d2Lund); 
  if(_fChain->GetBranch("Kstar0d2Idx"))_fChain->SetBranchAddress("Kstar0d2Idx",Kstar0d2Idx); 

  if(_fChain->GetBranch("nP"))_fChain->SetBranchAddress("nP",&nP); 
  if(_fChain->GetBranch("PMass"))_fChain->SetBranchAddress("PMass",PMass); 
  if(_fChain->GetBranch("Pp3CM"))_fChain->SetBranchAddress("Pp3CM",Pp3CM); 
  if(_fChain->GetBranch("PcosthCM"))_fChain->SetBranchAddress("PcosthCM",PcosthCM); 
  if(_fChain->GetBranch("PphiCM"))_fChain->SetBranchAddress("PphiCM",PphiCM); 
  if(_fChain->GetBranch("PenergyCM"))_fChain->SetBranchAddress("PenergyCM",PenergyCM); 
  if(_fChain->GetBranch("PLund"))_fChain->SetBranchAddress("PLund",PLund); 
  if(_fChain->GetBranch("PMCIdx"))_fChain->SetBranchAddress("PMCIdx",PMCIdx); 
  if(_fChain->GetBranch("PTrkIdx"))_fChain->SetBranchAddress("PTrkIdx",PTrkIdx); 

  if(_fChain->GetBranch("nSig0"))_fChain->SetBranchAddress("nSig0",&nSig0); 
  if(_fChain->GetBranch("Sig0Mass"))_fChain->SetBranchAddress("Sig0Mass",Sig0Mass); 
  if(_fChain->GetBranch("Sig0p3CM"))_fChain->SetBranchAddress("Sig0p3CM",Sig0p3CM); 
  if(_fChain->GetBranch("Sig0energyCM"))_fChain->SetBranchAddress("Sig0energyCM",Sig0energyCM); 
  if(_fChain->GetBranch("Sig0costhCM"))_fChain->SetBranchAddress("Sig0costhCM",Sig0costhCM); 
  if(_fChain->GetBranch("Sig0phiCM"))_fChain->SetBranchAddress("Sig0phiCM",Sig0phiCM); 
  if(_fChain->GetBranch("Sig0Lund"))_fChain->SetBranchAddress("Sig0Lund",Sig0Lund); 
  if(_fChain->GetBranch("Sig0MCIdx"))_fChain->SetBranchAddress("Sig0MCIdx",Sig0MCIdx); 
  if(_fChain->GetBranch("Sig0d1Lund"))_fChain->SetBranchAddress("Sig0d1Lund",Sig0d1Lund); 
  if(_fChain->GetBranch("Sig0d1Idx"))_fChain->SetBranchAddress("Sig0d1Idx",Sig0d1Idx); 
  if(_fChain->GetBranch("Sig0d2Lund"))_fChain->SetBranchAddress("Sig0d2Lund",Sig0d2Lund); 
  if(_fChain->GetBranch("Sig0d2Idx"))_fChain->SetBranchAddress("Sig0d2Idx",Sig0d2Idx); 

  if(_fChain->GetBranch("nLam0"))_fChain->SetBranchAddress("nLam0",&nLam0); 
  if(_fChain->GetBranch("Lam0Mass"))_fChain->SetBranchAddress("Lam0Mass",Lam0Mass); 
  if(_fChain->GetBranch("Lam0p3CM"))_fChain->SetBranchAddress("Lam0p3CM",Lam0p3CM); 
  if(_fChain->GetBranch("Lam0energyCM"))_fChain->SetBranchAddress("Lam0energyCM",Lam0energyCM); 
  if(_fChain->GetBranch("Lam0costhCM"))_fChain->SetBranchAddress("Lam0costhCM",Lam0costhCM); 
  if(_fChain->GetBranch("Lam0phiCM"))_fChain->SetBranchAddress("Lam0phiCM",Lam0phiCM); 
  if(_fChain->GetBranch("Lam0Lund"))_fChain->SetBranchAddress("Lam0Lund",Lam0Lund); 
  if(_fChain->GetBranch("Lam0MCIdx"))_fChain->SetBranchAddress("Lam0MCIdx",Lam0MCIdx);
  if(_fChain->GetBranch("Lam0Vtxx"))_fChain->SetBranchAddress("Lam0Vtxx",Lam0Vtxx);
  if(_fChain->GetBranch("Lam0Vtxy"))_fChain->SetBranchAddress("Lam0Vtxy",Lam0Vtxy);
  if(_fChain->GetBranch("Lam0Vtxz"))_fChain->SetBranchAddress("Lam0Vtxz",Lam0Vtxz);
  if(_fChain->GetBranch("Lam0FlightBSLen"))_fChain->SetBranchAddress("Lam0FlightBSLen",Lam0FlightBSLen);
  if(_fChain->GetBranch("Lam0FlightBSErr"))_fChain->SetBranchAddress("Lam0FlightBSErr",Lam0FlightBSErr);
  if(_fChain->GetBranch("Lam0d1Lund"))_fChain->SetBranchAddress("Lam0d1Lund",Lam0d1Lund); 
  if(_fChain->GetBranch("Lam0d1Idx"))_fChain->SetBranchAddress("Lam0d1Idx",Lam0d1Idx); 
  if(_fChain->GetBranch("Lam0d2Lund"))_fChain->SetBranchAddress("Lam0d2Lund",Lam0d2Lund); 
  if(_fChain->GetBranch("Lam0d2Idx"))_fChain->SetBranchAddress("Lam0d2Idx",Lam0d2Idx); 

  if(_fChain->GetBranch("KSelectorsMap"))_fChain->SetBranchAddress("KSelectorsMap",KSelectorsMap);
  if(_fChain->GetBranch("piSelectorsMap"))_fChain->SetBranchAddress("piSelectorsMap",piSelectorsMap);
  if(_fChain->GetBranch("pSelectorsMap"))_fChain->SetBranchAddress("pSelectorsMap",pSelectorsMap);
 
  cout<<"Done linking Tag Chain Branches"<<endl;    

  return SetEventBranches();
}

Int_t DRecoTagAnalysis::MakeTagReducedNtupleBranches(){
  
  //
  ReducedNtuple->Branch("goodrecoid",&goodrecoid,"goodrecoid/I");

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
  ReducedNtuple->Branch("tagdstardauangle",&tagdstardauangle,"tagdstardauangle/F");
  ReducedNtuple->Branch("tagdstardecayangle",&tagdstardecayangle,"tagdstardecayangle/F");
  ReducedNtuple->Branch("tagdstargamenergy",&tagdstargamenergy,"tagdstargamenergy/F");
  ReducedNtuple->Branch("tagksflights",&tagksflights,"tagksflights/F");
  ReducedNtuple->Branch("tagpid",&tagpid,"tagpid/I");
  ReducedNtuple->Branch("tagpipid",&tagpipid,"tagpipid/I");
  ReducedNtuple->Branch("tagppid",&tagppid,"tagppid/I");
  ReducedNtuple->Branch("tagpi0gamenergy",&tagpi0gamenergy,"tagpi0gamenergy/F");
  ReducedNtuple->Branch("tagetagamenergy",&tagetagamenergy,"tagetagamenergy/F");
  ReducedNtuple->Branch("tagksflightcos",&tagksflightcos,"tagksflightcos/F");
  ReducedNtuple->Branch("tagksflightcosbeam",&tagksflightcosbeam,"tagksflightcosbeam/F");
  ReducedNtuple->Branch("tagksgamconvmass",&tagksgamconvmass,"tagksgamconvmass/F");

  MakeEventReducedNtupleBranches();
  cout<<"Done Making Tag Reduced Ntuple Branches"<<endl;
  return 1;
}

Int_t DRecoTagAnalysis::OptimizeCuts(Int_t mode,Float_t lowMassValue,Float_t lowPminValue,Float_t lowPidKMin){
  
  if(OpenReducedNtuple()!=1) return 0;

  //Determine mode indexes
  Int_t Dindex=mode/10;
  Int_t Modeindex=mode%10-1;  

  ///Define optimization vars for each mode
  TString TagOpCuts[4][10];
  //D0
  TagOpCuts[0][0]="mwin,pmin,probmin,pidkmin,pidpimin";//Kpi
  TagOpCuts[0][1]="mwin,pmin,probmin,pidkmin,pi0gamemin,pidpimin";//Kpipi0  
  TagOpCuts[0][2]="mwin,pmin,probmin,pidkmin,pidpimin";//K3pi
  TagOpCuts[0][3]="mwin,pmin,pidpimin";//Kspipi   ,kscosmin
  TagOpCuts[0][4]="mwin,pmin,probmin,pidkmin,pidpimin";//K3pipi0
  TagOpCuts[0][5]="mwin,pmin,probmin,pi0gamemin,pidpimin";//Kspipipi0 kscosmin,
  //Dc
  TagOpCuts[1][0]="mwin,pmin,probmin,pidkmin,pidpimin";//Kpipi
  TagOpCuts[1][1]="mwin,pmin,probmin,pidkmin,pi0gamemin,pidpimin";//Kpipipi0
  TagOpCuts[1][2]="mwin,pmin,probmin,pidpimin";//Kspi ,kscosmin
  TagOpCuts[1][3]="mwin,pmin,pi0gamemin,pidpimin";//Kspipi0 kscosmin
  TagOpCuts[1][4]="mwin,pmin,probmin,pidpimin";//Ks3pi ,kscosmin
  //Ds
  TagOpCuts[2][0]="mwin,pmin,pidkmin";//KsK
  TagOpCuts[2][1]="mwin,pmin,pidpimin";//PhiPi
  TagOpCuts[2][2]="mwin,pmin,pi0gamemin";//PhiRhoC
  TagOpCuts[2][3]="mwin,pmin,probmin,pidkmin,pidpimin";//KsKPiPi
  TagOpCuts[2][4]="mwin,pmin,probmin,etagamemin,pidpimin";//EtaPPi
  TagOpCuts[2][5]="mwin,pmin,probmin,pidpimin";//PhiRho0pi
  TagOpCuts[2][6]="mwin,pmin,probmin,pidkmin";//KKstar0
  //LambdaC
  TagOpCuts[3][0]="mwin,pmin,probmin,pidkmin,pidprmin,pidpimin";//pKpi
  TagOpCuts[3][1]="mwin,pmin,probmin,pidkmin,pidprmin,pi0gamemin,pidpimin";//pKpipi
  TagOpCuts[3][2]="mwin,pmin,probmin,pidprmin";//pKs
  TagOpCuts[3][3]="mwin,pmin,probmin,pidprmin,pidpimin";//pKspipi
  TagOpCuts[3][4]="mwin,pmin,probmin,pidprmin,pidpimin";//Lzpi
  TagOpCuts[3][5]="mwin,pmin,pi0gamemin,pidpimin";//Lzpipi
  TagOpCuts[3][6]="mwin,pmin,probmin,pidpimin";//Lzpipipi
  TagOpCuts[3][7]="mwin,pmin,probmin,pidpimin";//Szpi
  //  TagOpCuts[3][8]="";//Szpipipi

  ReducedNtuple->SetBranchStatus("*",0);
  ReducedNtuple->SetBranchStatus("tagmass",1);
  if(TagOpCuts[Dindex][Modeindex].Contains("pmin"))ReducedNtuple->SetBranchStatus("tagpstar",1);
  if(TagOpCuts[Dindex][Modeindex].Contains("probmin"))ReducedNtuple->SetBranchStatus("taglogvtxprob",1);
  if(TagOpCuts[Dindex][Modeindex].Contains("pidkmin"))ReducedNtuple->SetBranchStatus("tagpid",1);
  if(TagOpCuts[Dindex][Modeindex].Contains("pidpimin"))ReducedNtuple->SetBranchStatus("tagpipid",1);
  if(TagOpCuts[Dindex][Modeindex].Contains("pidprmin"))ReducedNtuple->SetBranchStatus("tagppid",1);
  if(TagOpCuts[Dindex][Modeindex].Contains("pi0gamemin"))ReducedNtuple->SetBranchStatus("tagpi0gamenergy",1);
  if(TagOpCuts[Dindex][Modeindex].Contains("etagamemin"))ReducedNtuple->SetBranchStatus("tagetagamenergy",1);
  if(TagOpCuts[Dindex][Modeindex].Contains("kscosmin"))ReducedNtuple->SetBranchStatus("tagksflightcosbeam",1);

  Float_t SideMinScale=1.5;
  //For this mode; Ds->PhiRhoC we found that the mass range wasn't quite large enough 
  //to optimize with, so this following if statement is used to 'squeeze it in' just for the optimization
  if(Dindex==2&&Modeindex==2){
    SideMinScale=1.;
    TagWidth[Dindex][Modeindex]=3*18.000/1000;
  }
  //Lc->Lpipipi was the only other mode whose mass window wasn't big enough.  Here though, all we need to do is not scale up where 
  //the sideband window starts.
  if(Dindex==3&&Modeindex==5){
    SideMinScale=1.;
  }
  TTree*sigt=new TTree("sigt","sigt");
  TTree*bkgt=new TTree("bkgt","bkgt");
  sigt=ReducedNtuple->CopyTree(TCut(Form("abs(tagmass-%.6f)<%.6f",TagMean[Dindex][Modeindex],TagWidth[Dindex][Modeindex])));
  bkgt=ReducedNtuple->CopyTree(TCut(Form("%.6f<abs(tagmass-%.6f)&&abs(tagmass-%.6f)<%.6f",SideMinScale*TagWidth[Dindex][Modeindex],TagMean[Dindex][Modeindex],TagMean[Dindex][Modeindex],(SideMinScale+1)*TagWidth[Dindex][Modeindex])));

  cout<<"Entries in signal tree= "<<sigt->GetEntriesFast()<<endl;
  cout<<"Entries in sideband tree= "<<bkgt->GetEntriesFast()<<endl;

  //Plots to be saved to root file
  TString rootFileName;
  if ( mode == 2 || mode == 3 || mode == 12 || mode == 32 ){
    rootFileName=Form("/OptimizeCuts_%f_%f_%f.root",lowMassValue,lowPminValue,lowPidKMin);
  }else{
    rootFileName=Form("/OptimizeCuts_%f_%f.root",lowMassValue,lowPminValue);
  }
  TFile Results(_OutputDir+rootFileName,"recreate");

  gROOT->cd();
  
  ///Variables for Minuit Optimization 
  //RooRealVar mwin("mwin",Form("abs(tagmass-%.6f)<%s",TagMean[Dindex][Modeindex],"%.6f"),TagWidth[Dindex][Modeindex]/3.,TagWidth[Dindex][Modeindex]);
  RooRealVar mwin("mwin",Form("abs(tagmass-%.6f)<%s",TagMean[Dindex][Modeindex],"%.6f"),lowMassValue,lowMassValue);
  mwin.setUnit("mass window");mwin.setVal(lowMassValue);mwin.setError(.001);//this name cannot be changed because is hard coded in DRecoTagOptimizer.
  RooRealVar pmin("pmin","tagpstar>%6f",lowPminValue,lowPminValue);
  pmin.setUnit("p*");pmin.setError(.1);pmin.setVal(lowPminValue);//pmin.setConstant(1);
  RooRealVar probmin("probmin","taglogvtxprob>%.6f",-5.0,-1.0);probmin.setUnit("vtx fit prob.");probmin.setVal(-5.0);probmin.setError(.2);
  //
  //Set pidkmin dummy variable
  RooRealVar pidkmin("pidkmin","tagpid>=%.1f",-1.,-1.);
  pidkmin.setUnit("K PID selector");pidkmin.setError(1.); 
  //define pidkmin based on which mode we have
  if ( mode == 2 || mode == 3 || mode == 12 || mode == 32 ){
    pidkmin.setRange(lowPidKMin,lowPidKMin);pidkmin.setVal(lowPidKMin);
  }else{
    pidkmin.setRange(0.,5.);pidkmin.setVal(0.);
  } 
  //
  RooRealVar pidpimin("pidpimin","tagpipid>=%.1f",0.,6.);pidpimin.setUnit("pi PID selector");pidpimin.setVal(0.);pidpimin.setError(1.); 
  RooRealVar pidprmin("pidprmin","tagppid>=%.1f",0.,6.);pidprmin.setUnit("p PID selector");pidprmin.setVal(0.);pidprmin.setError(1.); 
  RooRealVar pi0gamemin("pi0gamemin","tagpi0gamenergy>%.6f",.1,.2);pi0gamemin.setUnit("pi0 gamma E");pi0gamemin.setVal(.1);pi0gamemin.setError(.01);
  RooRealVar etagamemin("etagamemin","tagetagamenergy>%.6f",.1,.2);etagamemin.setUnit("eta gamma E");etagamemin.setVal(.1);etagamemin.setError(.01);
  RooRealVar kscosmin("kscosmin","tagksflightcosbeam>%.6f",-.5,.5);kscosmin.setUnit("Ks Flight cosine");kscosmin.setVal(-.5);kscosmin.setError(.1);

  RooArgList cutvars;
  cutvars.add(mwin);
  if(TagOpCuts[Dindex][Modeindex].Contains("pmin"))cutvars.add(pmin);
  if(TagOpCuts[Dindex][Modeindex].Contains("probmin"))cutvars.add(probmin);
  if(TagOpCuts[Dindex][Modeindex].Contains("pidkmin"))cutvars.add(pidkmin);
  if(TagOpCuts[Dindex][Modeindex].Contains("pidpimin"))cutvars.add(pidpimin);
  if(TagOpCuts[Dindex][Modeindex].Contains("pidprmin"))cutvars.add(pidprmin);
  if(TagOpCuts[Dindex][Modeindex].Contains("pi0gamemin"))cutvars.add(pi0gamemin);
  if(TagOpCuts[Dindex][Modeindex].Contains("etagamemin"))cutvars.add(etagamemin);
  if(TagOpCuts[Dindex][Modeindex].Contains("kscosmin"))cutvars.add(kscosmin);


  //make plot with initial cuts and sideband definition
  Int_t nbins=200;
  Float_t xmin=0;Float_t xmax=0;   
  if(Dindex==0){xmin=1.77;xmax=1.97;}
  if(Dindex==1){xmin=1.77;xmax=1.97;}
  if(Dindex==2){xmin=1.87;xmax=2.07;}
  if(Dindex==3){xmin=2.18;xmax=2.38;}
  //cout<<"Dindex "<<Dindex<<" xmin "<<xmin<<" xmax "<<xmax<<endl;
  TH1F HMass("HMass","",nbins,xmin,xmax);
  HMass.SetTitle(TagNames[Dindex][Modeindex]);
  TH1F HMassSig("HMassSig","",nbins,xmin,xmax);HMassSig.SetFillColor(2);
  TH1F HMassSide("HMassSide","",nbins,xmin,xmax);HMassSide.SetFillColor(4);
  //compare plot with initial cuts: do this because p* distribution is tailing down below skim cut
  TCut initcutnomass;
  TIterator*cutvarIter=cutvars.createIterator();
  char textl[100];
  while(RooRealVar* var=(RooRealVar*)cutvarIter->Next()){
    sprintf(textl,var->GetTitle(),var->getVal());
    if(TString(var->GetName())!="mwin")initcutnomass+=TCut(textl); 
  }  

  ReducedNtuple->Draw("tagmass>>HMass",initcutnomass);
  sigt->Draw("tagmass>>HMassSig",initcutnomass,"same");
  bkgt->Draw("tagmass>>HMassSide",initcutnomass,"same");
  Results.cd();
  HMass.Write();
  HMassSig.Write();
  HMassSide.Write();
  gROOT->cd();


  
  //the dataset defines the variable (mass) to be excluded from the optimization, it has no other purpose
  //mass has to do with the range of a dummy pdf.  It is not something you'd want to vary as part of optimization.
  mass.setRange(xmin,xmax);
  mass.setBins(nbins);
  TH1F Histo("Histo","",nbins,xmin,xmax);
  RooDataHist dataset("dataset","dataset",RooArgList(mass),&Histo); 
  //the pdf needs the mass variable to go with the dataset, cut variables need to be included here.
  RooArgSet pdfvars(cutvars);
  pdfvars.add(mass);

  
  TString pdfformula="mass";
  pdfformula+="+mwin";
  if(TagOpCuts[Dindex][Modeindex].Contains("pmin"))pdfformula+="+pmin";
  if(TagOpCuts[Dindex][Modeindex].Contains("probmin"))pdfformula+="+probmin";
  if(TagOpCuts[Dindex][Modeindex].Contains("pidkmin"))pdfformula+="+pidkmin";
  if(TagOpCuts[Dindex][Modeindex].Contains("pidpimin"))pdfformula+="+pidpimin";
  if(TagOpCuts[Dindex][Modeindex].Contains("pidprmin"))pdfformula+="+pidprmin";
  if(TagOpCuts[Dindex][Modeindex].Contains("pi0gamemin"))pdfformula+="+pi0gamemin";
  if(TagOpCuts[Dindex][Modeindex].Contains("etagamemin"))pdfformula+="+etagamemin";
  if(TagOpCuts[Dindex][Modeindex].Contains("kscosmin"))pdfformula+="+kscosmin";
  RooGenericPdf pdf("pdf",pdfformula,pdfvars);
  //The 'optimizer' will hold the significance of the dummy fit.
  DRecoTagOptimizer optimizer("optimizer","optimizer",pdf,dataset,sigt,bkgt,&cutvars,0);
  optimizer.SetSigType(1);
  optimizer.SetMinSideBandLimit(SideMinScale*TagWidth[Dindex][Modeindex]);
  //optimizer.getVal() will return 1./significance for current cut varibles.



  //print the intial significance;
  Float_t initsig=optimizer.GetSignificance();
  cout<<"Initial Significance= "<<initsig<<endl;
  ULong_t starttime=(ULong_t)(gSystem->Now());
  SearchOptimizationGrid(&cutvars,&optimizer,0.);
  ULong_t tottime=((ULong_t)(gSystem->Now())-starttime)/1000;
  cout<<"Search time= "<<tottime<<" seconds"<<endl;
  Float_t finalsig=optimizer.GetSignificance();
  //print the final significance;
  cout<<"Final Significance= "<<finalsig<<endl;
 
  ofstream outfile;
  
  TString resultsFileName;
  if ( mode == 2 || mode == 3 || mode == 12 || mode == 32 ){
    resultsFileName=Form("/results_%f_%f_%f.txt",lowMassValue,lowPminValue,lowPidKMin);
  }else{
    resultsFileName=Form("/results_%f_%f.txt",lowMassValue,lowPminValue); 
  }

  outfile.open(_OutputDir+resultsFileName); 

  outfile<<"initsig"<<" "<<initsig<<endl;
  //compare plot with final cuts:
  TCut finalcut;
  TCut finalcutnomass;
  cutvarIter->Reset();
  while(RooRealVar* var=(RooRealVar*)cutvarIter->Next()){
    sprintf(textl,var->GetTitle(),var->getVal());
    finalcut+=TCut(textl); 
    if(TString(var->GetName())!="mwin")finalcutnomass+=TCut(textl); 
    
    TGraph*Scan=optimizer.ScanVariable(var);   
    Scan->SetName(TString(var->GetName())+"_Scan");
    Results.cd(); 
    Scan->Write();
    gROOT->cd();
    
    outfile<<var->GetName()<<" "<<var->getVal()<<endl;
  }  
  outfile<<"finalsig"<<" "<<finalsig<<endl;
  outfile<<"tottime"<<" "<<tottime<<endl;
  outfile.close();

  Results.cd(); 
  TH1F HMassFinalNoMass("HMassFinalNoMass","",nbins,xmin,xmax);  
  ReducedNtuple->Draw("tagmass>>HMassFinalNoMass",finalcutnomass);
  HMassFinalNoMass.Write();
  TH1F HMassFinal("HMassFinal","",nbins,xmin,xmax);  
  ReducedNtuple->Draw("tagmass>>HMassFinal",finalcut);
  HMassFinal.Write();
  Results.Close();

  delete sigt;delete bkgt;

  return 1;
  
}


Int_t DRecoTagAnalysis::DrawOptimizeCuts(){

  //Plots to be saved to root file
  TFile Results(_OutputDir+"/OptimizeCuts.root","read");
  gROOT->cd();
  
  filename=_OutputDir+"/OptimizeCuts.ps";
  Canvas->Print(filename+"[");
  
  TH1F* HMass=(TH1F*)Results.Get("HMass");
  TH1F* HMassSig=(TH1F*)Results.Get("HMassSig");
  TH1F* HMassSide=(TH1F*)Results.Get("HMassSide");

  Canvas->Clear();
  HMass->GetYaxis()->SetRangeUser(0,HMass->GetMaximum());
  HMass->Draw();
  HMassSig->Draw("same");
  HMassSide->Draw("same");
  Canvas->Print(filename);

  TH1F* HMassFinalNoMass=(TH1F*)Results.Get("HMassFinalNoMass");
  TH1F* HMassFinal=(TH1F*)Results.Get("HMassFinal");
  Canvas->Clear();
  HMass->Draw();
  HMassFinalNoMass->Draw("same");
  HMassFinal->Draw("same");
  Canvas->Print(filename);

  TGraph*mwinScan=(TGraph*)Results.Get("mwin_Scan");
  Canvas->Clear();
  mwinScan->Draw("ap");
  Canvas->Print(filename);

  TGraph*pScan=(TGraph*)Results.Get("pmin_Scan");
  if(pScan){
    Canvas->Clear();
    pScan->Draw("ap");
    Canvas->Print(filename);
  }
  TGraph*vtxScan=(TGraph*)Results.Get("probmin_Scan");
  if(vtxScan){
    Canvas->Clear();
    vtxScan->Draw("ap");
    Canvas->Print(filename);
  }
  TGraph*pidkScan=(TGraph*)Results.Get("pidkmin_Scan");
  if(pidkScan){
    Canvas->Clear();
    pidkScan->Draw("ap");
    Canvas->Print(filename);
  }
  TGraph*pidpiScan=(TGraph*)Results.Get("pidpimin_Scan");
  if(pidpiScan){
    Canvas->Clear();
    pidpiScan->Draw("ap");
    Canvas->Print(filename);
  }
  TGraph*pidprScan=(TGraph*)Results.Get("pidprmin_Scan");
  if(pidprScan){
    Canvas->Clear();
    pidprScan->Draw("ap");
    Canvas->Print(filename);
  }
  TGraph*pi0Scan=(TGraph*)Results.Get("pi0gamemin_Scan");
  if(pi0Scan){
    Canvas->Clear();
    pi0Scan->Draw("ap");
    Canvas->Print(filename);
  }
  TGraph*etaScan=(TGraph*)Results.Get("etagamemin_Scan");
  if(etaScan){
    Canvas->Clear();
    etaScan->Draw("ap");
    Canvas->Print(filename);
  }
  TGraph*ksScan=(TGraph*)Results.Get("kscosmin_Scan");
  if(ksScan){
    Canvas->Clear();
    ksScan->Draw("ap");
    Canvas->Print(filename);
  }

  Canvas->Print(filename+"]");
  Results.Close();


  return 1;
}

Int_t DRecoTagAnalysis::DrawOptimizeCutsAll(){
  filename=_OutputDir+"/OptimizeCutsAll.ps";
  Canvas->Print(filename+"[");
    
  
  for(Int_t Dindex=1;Dindex<=NTagTypes;Dindex++){  
    Canvas->Clear();
    Canvas->Divide(2,(nTagModes[Dindex-1]%2==0) ? nTagModes[Dindex-1]/2 : nTagModes[Dindex-1]/2 +1);
    for(Int_t Modeindex=1;Modeindex<=nTagModes[Dindex-1];Modeindex++){//
      Canvas->cd(Modeindex);
      TH1F* HMassC=NULL;
      TH1F* HMassFinalNoMassC=NULL;
      TH1F* HMassFinalC=NULL;

      TFile Results(_OutputDir+"/Mode"+(long)(10*(Dindex-1)+Modeindex)+"/OptimizeCuts.root","read");

      if(Results.IsZombie())continue;
      gROOT->cd();
      TH1F* HMass=(TH1F*)Results.Get("HMass");     
      TH1F* HMassFinalNoMass=(TH1F*)Results.Get("HMassFinalNoMass");
      TH1F* HMassFinal=(TH1F*)Results.Get("HMassFinal"); 
      if(!HMass||!HMassFinalNoMass||!HMassFinal)continue;
      HMass->SetTitle(TagNames[Dindex-1][Modeindex-1]);
      HMass->GetYaxis()->SetRangeUser(0,HMass->GetMaximum());
      HMass->Draw();   
      //Make clones for drawing
      HMassC=(TH1F*)HMass->Clone(Form("HMassC%d_%d",Dindex,Modeindex));
      HMassFinalNoMassC=(TH1F*)HMassFinalNoMass->Clone(Form("HMassC%d_%d",Dindex,Modeindex));
      HMassFinalC=(TH1F*)HMassFinal->Clone(Form("HMassC%d_%d",Dindex,Modeindex));
      Results.Close(); 
      HMassC->Draw();
      HMassFinalNoMassC->DrawClone("same");
      //make final mass plot look 'nice'
      HMassFinalC->SetFillColor(kGreen);
      HMassFinalC->DrawClone("same");        
      ifstream results;

      results.open(_OutputDir+"/Mode"+(long)(10*(Dindex-1)+Modeindex)+"/results.txt");

      char varname[100];
      char varvalue[100];
      TText text;
      text.SetTextSize(.05);
      Int_t count=0;
      results>>varname;
      while(!results.eof()){
	results>>varvalue;
	text.DrawTextNDC(.15,.75-count*.05,TString(varname)+"="+varvalue);
	count++;
	results>>varname;
      }
      results.close();
    }
    Canvas->Print(filename);    
  }
  //Note that pointers were lost
  
  Canvas->Print(filename+"]");
  
  return 1;
}



void DRecoTagAnalysis::SearchOptimizationGrid(RooArgList* vars,DRecoTagOptimizer* fcn,Float_t minpurity=0.){
  if(!vars||!fcn)return;

  TIterator*cutvarIter=vars->createIterator();

  //print out the initial values;
  cout<<"Initial cut values"<<endl;
  cutvarIter->Reset();
  while(RooRealVar* var=(RooRealVar*)cutvarIter->Next()){
    cout<<var->GetName()<<"= "<<var->getVal()<<endl;    
  }
  cout<<endl;

  ///--------------------------------------------------
  //Perform a simple multidimensional grid search
  //---------------------------------------------------
  Int_t nvars=0;
  Int_t nsteps[10];
  Int_t stepindex[10];
  Float_t maxfcncut[10];
  Int_t nsteps_total=1;
  Int_t step_counter=0;
  cutvarIter->Reset();
  while(RooRealVar* var=(RooRealVar*)cutvarIter->Next())
    //OLD: nsteps[nvars++]=(int)((var->getMax()-var->getMin())/var->getError() +.5)+1;//add 1 to check edges
    (var->getMax()==var->getMin()) ? nsteps[nvars++]=1 : nsteps[nvars++]=(int)((var->getMax()-var->getMin())/var->getError() +.5)+1;//add 1 to check edges
  for(Int_t i=0;i<nvars;i++){
    stepindex[i]=0;
    maxfcncut[i]=0.;
  }

  //calculate the total number of steps
  for ( int i=0; i<nvars; i++){
    if (nsteps[i]!=0){nsteps_total=nsteps_total*nsteps[i];}
  }
  cout<<"nsteps total is: "<<nsteps_total<<endl<<endl;

  Float_t fcnmax=0.;
  while(stepindex[0]<nsteps[0]){   
    //cout<<"combination ";for ( int i=0; i<nvars; i++){cout<<stepindex[i]<<" ";} cout<<" "<<step_counter<<endl;
    
    step_counter++;
    if ( step_counter % 100 == 0 ){ cout<<"The step counter is: "<<step_counter<<endl; }
    
    //Change the parameters
    Int_t j=0;cutvarIter->Reset();
    while(RooRealVar* var=(RooRealVar*)cutvarIter->Next())
      var->setVal(var->getMin()+stepindex[j++]*var->getError());//first step is at lower edge
    //evaluate significance
    Float_t val=fcn->GetSignificance();
    //update best parameters
    if(val>fcnmax&&fcn->GetPurity()>minpurity){
      fcnmax=val; 
      Int_t j=0;cutvarIter->Reset();
      while(RooRealVar* var=(RooRealVar*)cutvarIter->Next())
	maxfcncut[j++]=var->getVal();
    }
    
    //    for(Int_t k=0;k<nvars;k++) cout<<stepindex[k]<<" ";
    //     cout<<" fcn="<<fcnmax<<endl;

    //change the step indexes
    stepindex[nvars-1]++;//first add 1 to last variable
    for(Int_t k=nvars-2;k>=0;k--){
      if(stepindex[k+1]==nsteps[k+1])stepindex[k]++;//update previous variables when next var is at max
    }   
    for(Int_t k=1;k<nvars;k++){//do not reset first var (used to terminate loop)
      if(stepindex[k]==nsteps[k])stepindex[k]=0;//reset when at max
    }         

  }

  //set variables to optimal values and print out the values;
  cout<<"Final cut values"<<endl;
  Int_t j=0;cutvarIter->Reset();
  while(RooRealVar* var=(RooRealVar*)cutvarIter->Next()){    
      var->setVal(maxfcncut[j++]);
      cout<<var->GetName()<<"= "<<var->getVal()<<endl;        
  }
  cout<<endl;
 
}


Int_t DRecoTagAnalysis::MakeTagMassPlots()
{  
  if(OpenReducedNtuple()!=1) return 0;
  
  TFile f_Output(_OutputDir+"/TagMassPlots.root","RECREATE");
  if(f_Output.IsZombie())return 0;
  Double_t lowMassValue=0, highMassValue=0;
  for(Int_t Dindex=1;Dindex<=NTagTypes;Dindex++){
      if(Dindex==1){lowMassValue=1.77;highMassValue=1.97;}
      if(Dindex==2){lowMassValue=1.77;highMassValue=1.97;}
      if(Dindex==3){lowMassValue=1.87;highMassValue=2.07;}
      if(Dindex==4){lowMassValue=2.18;highMassValue=2.38;}
      for(Int_t Modeindex=1;Modeindex<=nTagModes[Dindex-1];Modeindex++){
	TH1F H(TagNames[Dindex-1][Modeindex-1],TagNames[Dindex-1][Modeindex-1],200,lowMassValue,highMassValue);
	
	//old line
	ReducedNtuple->Draw(Form("tagmass>>%s",(const char*)TagNames[Dindex-1][Modeindex-1]),Form("abs(taglund)==%d&&tagmode==%d",Dindex,Modeindex));
	//new line, introduced to add pstar cut to compare shane's skim to that of fDs.
	//ReducedNtuple->Draw(Form("tagmass>>%s",(const char*)TagNames[Dindex-1][Modeindex-1]),Form("taglund==%d&&tagmode==%d&&tagpstar>2.4",Dindex,Modeindex));
	H.Write();
      }
  }  
  
  f_Output.Close();
  CloseReducedNtuple();
  
  return 1;
}

Int_t DRecoTagAnalysis::DrawTagMassPlots()
{
  TFile f_Output(_OutputDir+"/TagMassPlots.root","Update");
  if(f_Output.IsZombie())return 0;
 
  filename=_OutputDir+"/TagMassPlots.ps";
  Canvas->Print(filename+"[");
  
  Float_t Max=0;
  Float_t sigmaMultiplier=0;
  Double_t sigYield=0;
  Double_t valStore[4][10][3];//Dindex,Modeindex,(mean & sigma & signal)
  for(Int_t Dindex=1;Dindex<=NTagTypes;Dindex++){
    Canvas->Clear();
    Canvas->Divide(2,3);
    for(Int_t Modeindex=1;Modeindex<=nTagModes[Dindex-1];Modeindex++){
      Canvas->cd(Modeindex);
      TH1F*h=(TH1F*)f_Output.Get(TagNames[Dindex-1][Modeindex-1]);
      if(!h){cout<<"histo not found "<<TagNames[Dindex-1][Modeindex-1]<<endl;}
      //TF1*FitF=FitHistogram1gausPlusPol1(h,h->GetXaxis()->GetXmin(),h->GetXaxis()->GetXmax());
      if(Dindex==4||(Dindex==3&&Modeindex==4)){
	sigmaMultiplier=2.0;
      }else{
	sigmaMultiplier=3.0;
      }
      //Calculate fit limits
      Float_t lowLimitOfFit=TagMean[Dindex-1][Modeindex-1]-sigmaMultiplier*TagWidth[Dindex-1][Modeindex-1];
      Float_t highLimitOfFit=TagMean[Dindex-1][Modeindex-1]+sigmaMultiplier*TagWidth[Dindex-1][Modeindex-1];

      TF1*FitF=FitHistogram1gausPlusPol1(h,lowLimitOfFit,highLimitOfFit);

      //Define a temporary Gaussian that can be integrated to obtain the signal yield
      TF1*sigGauss = new TF1("sigGauss", "[0]*exp(-.5*(x-[1])**2/[2]**2)",lowLimitOfFit,highLimitOfFit);
      sigGauss->SetParameters(FitF->GetParameter(0),FitF->GetParameter(1),FitF->GetParameter(2));
      sigGauss->SetLineColor(7);
      sigYield=sigGauss->Integral(lowLimitOfFit,highLimitOfFit)/h->GetBinWidth(1);
      
      cout<<TagNames[Dindex-1][Modeindex-1]<<" mean "<<FitF->GetParameter(1)<<" sigma "<<FitF->GetParameter(2)<<" signal yield "<<sigYield<<endl;
      //Store the mean sigma and yield for printing at the end of the loop
      valStore[Dindex-1][Modeindex-1][0]=FitF->GetParameter(1);  //mean 
      valStore[Dindex-1][Modeindex-1][1]=FitF->GetParameter(2);  //sigma
      valStore[Dindex-1][Modeindex-1][2]=sigYield;               //signal

      h->SetTitle(TagNames[Dindex-1][Modeindex-1]);
      h->GetYaxis()->SetRangeUser(0,h->GetMaximum());
      h->GetYaxis()->SetTitle(Form("Entries/%d MeV",(int)(h->GetBinWidth(1)*1000)));
      h->GetXaxis()->SetTitle("Tag mass (GeV)");
      h->Draw("pe");
      FitF->Draw("lsame");
      sigGauss->Draw("same");
      if(Max<h->GetMaximum())Max=h->GetMaximum();
      //TotalIntegral+=h->Integral();
      delete FitF;
    }
    Canvas->Print(filename); 
  } 
  
  for(Int_t Dindex=1;Dindex<=NTagTypes;Dindex++){
    Canvas->Clear();
    Canvas->Divide(2,3);
    for(Int_t Modeindex=1;Modeindex<=nTagModes[Dindex-1];Modeindex++){
      Canvas->cd(Modeindex);
      TH1F*h=(TH1F*)f_Output.Get(TagNames[Dindex-1][Modeindex-1]);
      h->GetYaxis()->SetRangeUser(0,Max);
      //h->SetTitle(TString(h->GetTitle())+Form("   Fraction=%.3f",h->Integral()/(float)TotalIntegral));
      h->Draw("pe");
    }
    Canvas->Print(filename); 
  }  

  TText fitInfo;
  fitInfo.SetTextSize(0.02);
  Double_t ypos=0.90;
  Canvas->Clear();
  //Titles
  fitInfo.DrawText(0.05,0.95,"Mode");
  fitInfo.DrawText(0.30,0.95,"Mean (GeV)");
  fitInfo.DrawText(0.50,0.95,"Sigma (GeV)");
  fitInfo.DrawText(0.70,0.95,"Yield");
  for(Int_t Dindex=1;Dindex<=NTagTypes;Dindex++){
    for(Int_t Modeindex=1;Modeindex<=nTagModes[Dindex-1];Modeindex++){
      fitInfo.DrawText(0.05,ypos,TagNames[Dindex-1][Modeindex-1]);                       //name
      fitInfo.DrawText(0.30,ypos,Form("%8.3f",valStore[Dindex-1][Modeindex-1][0]));      //mean 
      fitInfo.DrawText(0.50,ypos,Form("%8.3f",valStore[Dindex-1][Modeindex-1][1]));      //sigma 
      fitInfo.DrawText(0.70,ypos,Form("%8.3f",valStore[Dindex-1][Modeindex-1][2]));      //yield
      ypos-=0.025;
    }
  }
  cout<<"Printing Statistics"<<endl;
  Canvas->Print(filename);   

  Canvas->Print(filename+"]");

  f_Output.Close();  
  return 1;
}


Int_t DRecoTagAnalysis::DrawTagMassPlotsAll(){


  filename=_OutputDir+"/TagMassPlots.ps";
  //filename="DsTolnu/TagOptRaw/TagMassPlots.ps";
  //filename="DsTolnu/OptimizedTags/TagMassPlots.pdf";
  Canvas->SetWindowSize(1200,1200);
  Canvas->Print(filename+"[");

  int along=0;
  int across=0;
  //Float_t fitRangeMultiplier=0;
  Double_t sigYield=0,sigYieldTotal=0,backgroundYield=0,purity=0,significance=0;
  Double_t valStore[4][10][6];//Dindex,Modeindex,(mean & sigma & signal)
  for(Int_t Dindex=1;Dindex<=NTagTypes;Dindex++){
    //for(Int_t Dindex=3;Dindex<=3;Dindex++){
    Canvas->Clear();
    switch (Dindex) {
    case 1 : along=2; across=3; break;
    case 2 : along=2; across=3; break;
    case 3 : along=2; across=4; break;
    case 4 : along=3; across=3; break;
    }
    Canvas->Divide(along,across);
    for(Int_t Modeindex=1;Modeindex<=nTagModes[Dindex-1];Modeindex++){
      Canvas->cd(Modeindex);
      TFile Results(_OutputDir+"/Mode"+(long)(10*(Dindex-1)+Modeindex)+"/TagMassPlots.root","read");
      
      gROOT->cd();

      TH1F* HMass=(TH1F*)Results.Get(TagNames[Dindex-1][Modeindex-1]);
      TH1F* HMassC=(TH1F*)HMass->Clone("HMassC");
      //if you just want raw mass plots uncomment this line
      //HMassC->Draw();Results.Close(); continue;


      /////////////////////////////////////////
      Float_t lowLimitOfFit=-1;
      Float_t highLimitOfFit=-1;

      Int_t FullModeIndex = 10*(Dindex-1)+Modeindex;
      switch(FullModeIndex){
      case 1 :
	lowLimitOfFit=1.78;highLimitOfFit=1.95;
	break;
      case 2 :
	lowLimitOfFit=1.78;highLimitOfFit=1.95;
	break;
      case 3 :
	lowLimitOfFit=1.78;highLimitOfFit=1.95;
	break;
      case 4 :
	lowLimitOfFit=1.78;highLimitOfFit=1.95;
	break;
      case 5 :
	lowLimitOfFit=1.78;highLimitOfFit=1.95;
	break;
      case 6 :
	lowLimitOfFit=1.78;highLimitOfFit=1.95;
	break;
      case 11:
	lowLimitOfFit=1.78;highLimitOfFit=1.95;
	break;
      case 12:
	lowLimitOfFit=1.78;highLimitOfFit=1.95;
	break;
      case 13:
	lowLimitOfFit=1.78;highLimitOfFit=1.95;
	break;
      case 14:
	lowLimitOfFit=1.78;highLimitOfFit=1.95;
	break;
      case 15:
	lowLimitOfFit=1.78;highLimitOfFit=1.95;
	break;
      case 21:
	lowLimitOfFit=1.89;highLimitOfFit=2.05;
	break;
      case 22:
	lowLimitOfFit=1.90;highLimitOfFit=2.04;
	break;
      case 23:
	lowLimitOfFit=1.88;highLimitOfFit=2.06;
	break;
      case 24:
	lowLimitOfFit=1.89;highLimitOfFit=2.05;
	break;
      case 25:
	lowLimitOfFit=1.89;highLimitOfFit=2.05;
	break;      
      case 26:
	lowLimitOfFit=1.90;highLimitOfFit=2.00;
	break;
      case 27:
	lowLimitOfFit=1.89;highLimitOfFit=2.05;
	break;
      case 28:
	lowLimitOfFit=1.89;highLimitOfFit=2.05;
	break;
      case 29:
	lowLimitOfFit=1.89;highLimitOfFit=2.05;
	break;      
      case 30:
	lowLimitOfFit=1.89;highLimitOfFit=2.05;
	break;      
      case 31:
	lowLimitOfFit=2.2;highLimitOfFit=2.36;
	break;      
      case 32:
	lowLimitOfFit=2.2;highLimitOfFit=2.36;
	break;      
      case 33:
	lowLimitOfFit=2.2;highLimitOfFit=2.36;
	break;      
      case 34:
	lowLimitOfFit=2.2;highLimitOfFit=2.36;
	break;
      case 35:
	lowLimitOfFit=2.2;highLimitOfFit=2.36;
	break;      
      case 36:
	lowLimitOfFit=2.20;highLimitOfFit=2.36;
	break;      
      case 37:
	lowLimitOfFit=2.20;highLimitOfFit=2.36;
	break;      
      case 38:
	lowLimitOfFit=2.20;highLimitOfFit=2.36;
	break;      
      case 39:
	lowLimitOfFit=2.20;highLimitOfFit=2.36;
	break;
      default : 
	cout<<"Invalid switch entry"<<endl;
      }
      
      /////////////////////////////////////////

      TF1*FitF=FitHistogram1gausPlusPol1(HMassC,lowLimitOfFit,highLimitOfFit);
      //HMassC->Fit(FitF,"QI","",lowLimitOfFit,highLimitOfFit);

      Int_t status=HMassC->Fit(FitF,"QI","",lowLimitOfFit,highLimitOfFit);
      if (status==0||status==4){
	//Fix mean and sigma rather than letting them float
	//FitF->FixParameter(1,TagMean[Dindex-1][Modeindex-1]);
	//FitF->FixParameter(2,(TagWidth[Dindex-1][Modeindex-1]/3));
	//
	//Let parameters float
	//FitF->SetParameters(norm, mean, sigma, const, slope)
	FitF->SetParameter(1,TagMean[Dindex-1][Modeindex-1]);
	FitF->SetParLimits(1,(TagMean[Dindex-1][Modeindex-1]-(TagWidth[Dindex-1][Modeindex-1]/3)),(TagMean[Dindex-1][Modeindex-1]+(TagWidth[Dindex-1][Modeindex-1]/3)));
	FitF->SetParameter(2,(TagWidth[Dindex-1][Modeindex-1]/3));
	FitF->SetParLimits(2,(TagWidth[Dindex-1][Modeindex-1]/6),(TagWidth[Dindex-1][Modeindex-1]/1.5));
	//
	Float_t slope=0,spanOfXAxis=0,y1=0,y2=0,x1=0,x2=0;	
	//slope=(y2-y1)/(x2-x1)
	spanOfXAxis=HMassC->GetBinCenter(HMassC->GetNbinsX())-HMassC->GetBinCenter(1);
	y2=HMassC->GetBinContent((int)floor(HMassC->GetNbinsX()*((highLimitOfFit-HMassC->GetBinCenter(1))/spanOfXAxis)));
	y1=HMassC->GetBinContent((int)floor(HMassC->GetNbinsX()*((lowLimitOfFit-HMassC->GetBinCenter(1))/spanOfXAxis)));
	x2=highLimitOfFit;
	x1=lowLimitOfFit;
	slope=(y2-y1)/(x2-x1);
	//cout<<"slope HMassC->GetNbinsX() lowLimitOfFit highLimitOfFit spanOfXAxis y2 y1 x2 x1 "<<slope<<" "<<HMassC->GetNbinsX()<<" "<<lowLimitOfFit<<" "<<highLimitOfFit<<" "<<spanOfXAxis<<" "<<y2<<" "<<y1<<" "<<x2<<" "<<x1<<endl; 
	FitF->SetParameter(4,slope);
	//FitF->SetParLimits(4,(slope/4),(slope*4));
	//cout<<"slope is "<<slope<<" Dindex is "<<Dindex<<" mode is "<<Modeindex<<endl;
	HMassC->Fit(FitF,"QI","",lowLimitOfFit,highLimitOfFit);
      }
      
      //Define a temporary Gaussian that can be integrated to obtain the signal yield
      TF1*sigGauss = new TF1("sigGauss", "[0]*exp(-.5*(x-[1])**2/[2]**2)",lowLimitOfFit,highLimitOfFit);
      sigGauss->SetParameters(FitF->GetParameter(0),FitF->GetParameter(1),FitF->GetParameter(2));
      sigGauss->SetLineColor(3);
      //sigYield=sigGauss->Integral(lowLimitOfFit,highLimitOfFit)/HMassC->GetBinWidth(1);
      sigYield=sigGauss->Integral(TagMean[Dindex-1][Modeindex-1]-TagWidth[Dindex-1][Modeindex-1],TagMean[Dindex-1][Modeindex-1]+TagWidth[Dindex-1][Modeindex-1])/HMassC->GetBinWidth(1);
      sigYieldTotal+=sigYield;
      
      //Define a temporary linear background function to plot on canvas (bkg yield will be total - signal)
      //TF1*background = new TF1("background","[0]+[1]*x",HMassC->GetXaxis()->GetXmin(),HMassC->GetXaxis()->GetXmax());
      TF1*background = new TF1("background","[0]+[1]*x",lowLimitOfFit,highLimitOfFit);
      background->SetParameters(FitF->GetParameter(3),FitF->GetParameter(4));
      background->SetLineColor(51);
      //backgroundYield=background->Integral(lowLimitOfFit,highLimitOfFit)/HMassC->GetBinWidth(1);
      //backgroundYield=HMassC->GetEntries()-sigYield;
      backgroundYield=background->Integral(TagMean[Dindex-1][Modeindex-1]-TagWidth[Dindex-1][Modeindex-1],TagMean[Dindex-1][Modeindex-1]+TagWidth[Dindex-1][Modeindex-1])/HMassC->GetBinWidth(1);
      //////
      purity=sigYield/(sigYield+backgroundYield);
      significance=sigYield/sqrt(sigYield+backgroundYield);
      /////
      cout<<TagNames[Dindex-1][Modeindex-1]<<" fit status "<<status<<" mean "<<FitF->GetParameter(1)<<" sigma "<<FitF->GetParameter(2)<<" # signal "<<sigYield<<" # background "<<backgroundYield<<" purity "<<purity<<" significance "<<significance<<endl;

      //Store the mean sigma and yield for printing at the end of the loop
      valStore[Dindex-1][Modeindex-1][0]=FitF->GetParameter(1);  //mean 
      valStore[Dindex-1][Modeindex-1][1]=FitF->GetParameter(2);  //sigma
      valStore[Dindex-1][Modeindex-1][2]=sigYield;               //# signal
      valStore[Dindex-1][Modeindex-1][3]=backgroundYield;        //# background
      valStore[Dindex-1][Modeindex-1][4]=purity;                 //purity
      valStore[Dindex-1][Modeindex-1][5]=significance;           //significance

      HMassC->SetTitle(TagNames[Dindex-1][Modeindex-1]);
      HMassC->GetYaxis()->SetRangeUser(0,HMassC->GetMaximum());
      HMassC->GetYaxis()->SetTitle(Form("Entries/%d MeV",(int)(HMassC->GetBinWidth(1)*1000)));
      HMassC->GetXaxis()->SetTitle("Tag mass (GeV)");
      HMassC->Draw("pe");
      FitF->Draw("lsame");
      sigGauss->Draw("same");
      background->Draw("same");
      delete FitF;
      Results.Close(); 
    }
    Canvas->Print(filename);    
    Canvas->SetWindowSize(1200,900);Canvas->SaveAs(Form("./DsTolnu/TagOptRaw/TagMassPlots/ForType_%d.png",(Dindex-1)));
  }
  //Note that pointers were lost

  //Summary page
  TText fitInfo;
  fitInfo.SetTextSize(0.02);
  Double_t ypos=0.90;
  Canvas->Clear();
  //Titles
  fitInfo.DrawText(0.05,0.95,"Mode");
  fitInfo.DrawText(0.22,0.95,"Mean (GeV)");
  fitInfo.DrawText(0.34,0.95,"Sigma (MeV)");
  fitInfo.DrawText(0.46,0.95,"# Signal");
  //fitInfo.DrawText(0.58,0.95,"# Background");
  fitInfo.DrawText(0.58,0.95,"Purity [S/(S+B)]");
  fitInfo.DrawText(0.70,0.95,"Significance");
  fitInfo.DrawText(0.70,0.93,"[S/sqrt(S+B)]");
  fitInfo.DrawText(0.82,0.95,"Signal yield Rel.");
  fitInfo.DrawText(0.82,0.93,"to whole (%)");
  //
  //Ready output text file
  ofstream outputStats;
  outputStats.open(_OutputDir+"/fittedTagMeansAndWidths.txt");
  //
  for(Int_t Dindex=1;Dindex<=NTagTypes;Dindex++){
    for(Int_t Modeindex=1;Modeindex<=nTagModes[Dindex-1];Modeindex++){
      fitInfo.DrawText(0.05,ypos,TagNames[Dindex-1][Modeindex-1]);                       //name
      fitInfo.DrawText(0.22,ypos,Form("%10.5f",valStore[Dindex-1][Modeindex-1][0]));      //mean 
      fitInfo.DrawText(0.34,ypos,Form("%10.5f",1000*valStore[Dindex-1][Modeindex-1][1]));      //sigma 
      fitInfo.DrawText(0.46,ypos,Form("%10.5f",valStore[Dindex-1][Modeindex-1][2]));      //# signal
      //fitInfo.DrawText(0.58,ypos,Form("%10.5f",valStore[Dindex-1][Modeindex-1][3]));      //# background
      fitInfo.DrawText(0.58,ypos,Form("%10.5f",valStore[Dindex-1][Modeindex-1][4]));      //purity
      fitInfo.DrawText(0.70,ypos,Form("%10.5f",valStore[Dindex-1][Modeindex-1][5]));      //significance
      fitInfo.DrawText(0.82,ypos,Form("%10.5f",100*valStore[Dindex-1][Modeindex-1][2]/sigYieldTotal));      //significance
      ypos-=0.025;

      //send fitted mean and sigma to output text file
      outputStats<<TagNames[Dindex-1][Modeindex-1]<<" "<<valStore[Dindex-1][Modeindex-1][0]<<" "<<valStore[Dindex-1][Modeindex-1][1]<<endl;
    }
  }
  //draw some table lines
  TLine l1(0.05,0.975,0.88,0.975);l1.Draw();
  TLine l2(0.05,0.923,0.88,0.923);l2.Draw();
  TLine l3(0.05,0.773,0.88,0.773);l3.Draw();
  TLine l4(0.05,0.648,0.88,0.648);l4.Draw();
  TLine l5(0.05,0.473,0.88,0.473);l5.Draw();
  TLine l6(0.05,0.273,0.88,0.273);l6.Draw();
  cout<<"Printing Statistics"<<endl;
  Canvas->Print(filename);   
  Canvas->SetWindowSize(1200,900);Canvas->SaveAs("./DsTolnu/TagOptRaw/TagMassPlots/statistics.png");
  //

  Canvas->Print(filename+"]");
  
  return 1;
}


Int_t DRecoTagAnalysis::MakeDstarMass(){


  TH1F HD0("HD0","",200,1.77,1.97);
  HD0.GetYaxis()->SetTitle("Entries/1 MeV");HD0.GetXaxis()->SetTitle("m(D^{0})  (GeV/c^{2})");
  TH1F HDPlus("HDPlus","",200,1.77,1.97);
  HDPlus.GetYaxis()->SetTitle("Entries/1 MeV");HDPlus.GetXaxis()->SetTitle("m(D^{+})  (GeV/c^{2})");

  TH1F HD0Pi("HD0Pi","",300,.13,.16);
  HD0Pi.GetYaxis()->SetTitle("Entries/.1 MeV");HD0Pi.GetXaxis()->SetTitle("m(D^{0}#pi^{+}) - m(D^{0})  (GeV/c^{2})");
  TH1F HD0Pi0("HD0Pi0","",300,.13,.16);
  HD0Pi0.GetYaxis()->SetTitle("Entries/.1 MeV");HD0Pi0.GetXaxis()->SetTitle("m(D^{0}#pi^{0}) - m(D^{0})  (GeV/c^{2})");
  TH1F HDPlusPi0("HDPlusPi0","",300,.13,.16);
  HDPlusPi0.GetYaxis()->SetTitle("Entries/.1 MeV");HDPlusPi0.GetXaxis()->SetTitle("m(D^{+}#pi^{0}) - m(D^{+})  (GeV/c^{2})");
  TH1F HD0Gam("HD0Gam","",100,.09,.19);
  HD0Gam.GetYaxis()->SetTitle("Entries/1 MeV");HD0Gam.GetXaxis()->SetTitle("m(D^{0}#gamma) - m(D^{0})  (GeV/c^{2})");

  if(OpenReducedNtuple()!=1) return 0;

  ReducedNtuple->Draw("tagmass>>HD0","abs(taglund)==1");
  ReducedNtuple->Draw("tagmass>>HDPlus","abs(taglund)==2");
  ReducedNtuple->Draw("tagdstarmass-tagmass>>HD0Pi","abs(tagmass-1.8645)<.025&&tagdstarlund==1");
  ReducedNtuple->Draw("tagdstarmass-tagmass>>HD0Pi0","abs(tagmass-1.8645)<.025&&tagdstarlund==2");
  ReducedNtuple->Draw("tagdstarmass-tagmass>>HDPlusPi0","abs(tagmass-1.8693)<.02&&tagdstarlund==3");
  ReducedNtuple->Draw("tagdstarmass-tagmass>>HD0Gam","abs(tagmass-1.8645)<.025&&tagdstarlund==4&&tagdstargamenergy>.25");
  CloseReducedNtuple();

  TFile f_Output(_OutputDir+"/DstarMassPlots.root","RECREATE");
  if(f_Output.IsZombie())return 0;
  HD0.Write();
  HDPlus.Write();
  HD0Pi.Write();
  HD0Pi0.Write();
  HDPlusPi0.Write();
  HD0Gam.Write();
  f_Output.ls();
  f_Output.Close();
  
  return 1;
}

Int_t DRecoTagAnalysis::DrawDstarMass(){


  TFile f_Output(_OutputDir+"/DstarMassPlots.root","read");
  if(f_Output.IsZombie())return 0;
  TH1F*HD0=(TH1F*)f_Output.Get("HD0");
  TH1F*HDPlus=(TH1F*)f_Output.Get("HDPlus");
  TH1F*HD0Pi=(TH1F*)f_Output.Get("HD0Pi");
  TH1F*HD0Pi0=(TH1F*)f_Output.Get("HD0Pi0");
  TH1F*HDPlusPi0=(TH1F*)f_Output.Get("HDPlusPi0");
  TH1F*HD0Gam=(TH1F*)f_Output.Get("HD0Gam");
  if(!HD0||!HDPlus||!HD0Pi||!HD0Pi0||!HDPlusPi0||!HD0Gam)return 0;
  

  filename=_OutputDir+"/DstarMassPlots.ps";
  Canvas->Print(filename+"[");
  
  TF1 Fgaus("Fgaus","[0]*exp(-.5*(x-[1])**2/[2]**2)+[3]*exp(-.5*(x-[4])**2/[5]**2)",1.78,1.95);
  TF1 FPol1("FPol1","[0]+[1]*x",1.78,1.95);
  FPol1.SetLineColor(1);

  Float_t Nsignal;
  Float_t Purity;

  Canvas->Clear();
  Float_t Par[8]={HD0->GetMaximum()*.4,D0PDGMass,.006,HD0->GetMaximum()*.1,D0PDGMass-.001,.007,1,2};
  TF1*Ffit=FitHistogram2gausPlusPolN(HD0,1.78,1.95,1,Par);    
  Fgaus.SetParameters(Ffit->GetParameter(0),Ffit->GetParameter(1),Ffit->GetParameter(2),Ffit->GetParameter(3),Ffit->GetParameter(4),Ffit->GetParameter(5));
  FPol1.SetParameters(Ffit->GetParameter(6),Ffit->GetParameter(7));
  Nsignal=Fgaus.Integral(D0PDGMass-.025,D0PDGMass+.025)/HD0->GetBinWidth(1);
  Purity=Fgaus.Integral(D0PDGMass-.025,D0PDGMass+.025)/(Fgaus.Integral(D0PDGMass-.025,D0PDGMass+.025)+FPol1.Integral(D0PDGMass-.025,D0PDGMass+.025));
  HD0->SetTitle(TString("Signal Yield=")+(long)Nsignal+"    Purity="+(long)(100*Purity)+"%");
  HD0->Draw("pe");
  cutline.DrawLine(D0PDGMass-.025,0,D0PDGMass-.025,HD0->GetMaximum());
  cutline.DrawLine(D0PDGMass+.025,0,D0PDGMass+.025,HD0->GetMaximum());  
  FPol1.Draw("same");
  Canvas->Update();Canvas->Print(filename);   

  Canvas->Clear();
  Float_t Par2[8]={HDPlus->GetMaximum()*.5,DPlusPDGMass,.006,HDPlus->GetMaximum()*.1,DPlusPDGMass-.001,.001,2500,400};
  Ffit=FitHistogram2gausPlusPolN(HDPlus,1.78,1.95,1,Par2);  
  Fgaus.SetParameters(Ffit->GetParameter(0),Ffit->GetParameter(1),Ffit->GetParameter(2),Ffit->GetParameter(3),Ffit->GetParameter(4),Ffit->GetParameter(5));
  FPol1.SetParameters(Ffit->GetParameter(6),Ffit->GetParameter(7));
  Nsignal=Fgaus.Integral(DPlusPDGMass-.020,DPlusPDGMass+.020)/HDPlus->GetBinWidth(1);
  Purity=Fgaus.Integral(DPlusPDGMass-.020,DPlusPDGMass+.020)/(Fgaus.Integral(DPlusPDGMass-.020,DPlusPDGMass+.020)+FPol1.Integral(DPlusPDGMass-.020,DPlusPDGMass+.020));
  HDPlus->SetTitle(TString("Signal Yield=")+(long)Nsignal+"    Purity="+(long)(100*Purity)+"%");
  HDPlus->Draw("pe");
  cutline.DrawLine(DPlusPDGMass+.02,0,DPlusPDGMass+.02,HDPlus->GetMaximum());
  cutline.DrawLine(DPlusPDGMass-.02,0,DPlusPDGMass-.02,HDPlus->GetMaximum());
  FPol1.Draw("same");
  Canvas->Update();Canvas->Print(filename);   


  TF1 sig;
  TF1 bkg;
  Canvas->Clear();
  FitHistogramDstarDM(HD0Pi,.14,.16,&sig,&bkg);
  Nsignal=sig.Integral(.1454-.0015,.1454+.0015)/HD0Pi->GetBinWidth(1);
  Purity=sig.Integral(.1454-.0015,.1454+.0015)/(sig.Integral(.1454-.0015,.1454+.0015)+bkg.Integral(.1454-.0015,.1454+.0015));
  HD0Pi->SetTitle(TString("Signal Yield=")+(long)Nsignal+"    Purity="+(long)(100*Purity)+"%");
  HD0Pi->Draw("pe");
  cutline.DrawLine(.1454+.0015,0,.1454+.0015,HD0Pi->GetMaximum());
  cutline.DrawLine(.1454-.0015,0,.1454-.0015,HD0Pi->GetMaximum());
  bkg.Draw("same");
  Canvas->Print(filename);

  Canvas->Clear();
  FitHistogramDstar0DM(HD0Pi0,.1351,.16,&sig,&bkg);
  Nsignal=sig.Integral(.14212-.003,.14212+.003)/HD0Pi0->GetBinWidth(1);
  Purity=sig.Integral(.14212-.003,.14212+.003)/(sig.Integral(.14212-.003,.14212+.003)+bkg.Integral(.14212-.003,.14212+.003));
  HD0Pi0->SetTitle(TString("Signal Yield=")+(long)Nsignal+"    Purity="+(long)(100*Purity)+"%");
  HD0Pi0->Draw("pe");
  cutline.DrawLine(.14212+.003,0,.14212+.003,HD0Pi0->GetMaximum());
  cutline.DrawLine(.14212-.003,0,.14212-.003,HD0Pi0->GetMaximum());
  bkg.Draw("same");					   
  Canvas->Print(filename);

  Canvas->Clear();
  FitHistogramDstar0DM(HDPlusPi0,.1351,.16,&sig,&bkg);
  Nsignal=sig.Integral(.14064-.003,.14064+.003)/HDPlusPi0->GetBinWidth(1);
  Purity=sig.Integral(.14064-.003,.14064+.003)/(sig.Integral(.14064-.003,.14064+.003)+bkg.Integral(.14064-.003,.14064+.003));
  HDPlusPi0->SetTitle(TString("Signal Yield=")+(long)Nsignal+"    Purity="+(long)(100*Purity)+"%");
  HDPlusPi0->Draw("pe");
  cutline.DrawLine(.14064+.003,0,.14064+.003,HDPlusPi0->GetMaximum());
  cutline.DrawLine(.14064-.003,0,.14064-.003,HDPlusPi0->GetMaximum());
  bkg.Draw("same");					   
  Canvas->Print(filename);


  TF1 FgausD0Gam("FgausD0Gam","[0]*exp(-.5*(x-[1])**2/[2]**2)+[3]*exp(-.5*(x-[4])**2/[5]**2)",.1,.19);
  TF1 FPol1D0Gam("FPol1","[0]+[1]*x+[2]*x*x",.095,.19);
  FPol1D0Gam.SetLineColor(1);
  Canvas->Clear();
  Float_t Par3[9]={HD0Gam->GetMaximum(),.142,.005,HD0Gam->GetMaximum()*.1,.136,.006,1,2,3};
  Ffit=FitHistogram2gausPlusPolN(HD0Gam,.095,.19,2,Par3);  
  FgausD0Gam.SetParameters(Ffit->GetParameter(0),Ffit->GetParameter(1),Ffit->GetParameter(2),Ffit->GetParameter(3),Ffit->GetParameter(4),Ffit->GetParameter(5));
  FPol1D0Gam.SetParameters(Ffit->GetParameter(6),Ffit->GetParameter(7),Ffit->GetParameter(8));
  Nsignal=FgausD0Gam.Integral(.141-.01,.141+.01)/HD0Gam->GetBinWidth(1);
  Purity=FgausD0Gam.Integral(.141-.01,.141+.01)/(FgausD0Gam.Integral(.141-.01,.141+.01)+FPol1D0Gam.Integral(.141-.01,.141+.01));
  HD0Gam->SetTitle(TString("Signal Yield=")+(long)Nsignal+"    Purity="+(long)(100*Purity)+"%");
  HD0Gam->Draw("pe");
  cutline.DrawLine(.141+.01,0,.141+.01,HD0Gam->GetMaximum());
  cutline.DrawLine(.141-.01,0,.141-.01,HD0Gam->GetMaximum());
  FPol1D0Gam.Draw("same");
  Canvas->Update();Canvas->Print(filename);  

  Canvas->Print(filename+"]");

  f_Output.Close();
  
  return 1;
}
