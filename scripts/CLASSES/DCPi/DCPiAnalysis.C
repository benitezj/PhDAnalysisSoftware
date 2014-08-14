#include "DCPiAnalysis.h"
#include "../../MACROS/TreeChain.C"
#include <RooGaussian.h>

ClassImp(DCPiAnalysis);
DCPiAnalysis::DCPiAnalysis(Int_t MCorDATA,TString OutputDir):
  DCAnalysis(MCorDATA,OutputDir),
  cuttruth(0),
  cutcharge(0),
  cutpstar(0),
  cutdflight(0),
  cutdmass(0),
  cutdcos(0),
  dau1m("dau1m","dau1m",DPlusPDGMass),
  dau2m("dau2m","dau2m",PiPDGMass),
  TwoBodyPS("TwoBodyPS","TwoBodyPS",mass,dau1m,dau2m),
  resmean("resmean","resmean",-.01,.01),
  resoffset("resoffset","resoffset",-.01,.02),
  resslope("resslope","resslope",0,.01),
  resnormratio2("resnormratio2","resnormratio2",0.,10),
  resmean2("resmean2","resmean2",-.1,.1),
  ressigmaratio2("ressigmaratio2","ressigmaratio2",1,10),
  resnormratio3("resnormratio3","resnormratio3",0.,10),
  resmean3("resmean3","resmean3",-.1,.1),
  ressigmaratio3("ressigmaratio3","ressigmaratio3",.1,10),
  resVar("resVar","resVar",-.02,.02),
  effoffset(0.03993),
  effslope(0.04083),
  massfitter(&mass)  
{

  
  LowMassHist=1.81;
  HighMassHist=1.93;
  MassHistBins=120;

  MeanMass=1.86859;
  LowMassCut=MeanMass-.0062*2.5;
  HighMassCut=MeanMass+.0062*2.5;
  LowSideLowMassCut=MeanMass-.0062*7.5;
  LowSideHighMassCut=MeanMass-.0062*5;
  HighSideLowMassCut=MeanMass+.0062*5;
  HighSideHighMassCut=MeanMass+.0062*7.5;

  DFlightCut=0.;

  PstarMin=3.0;
  DCosMin=-.8;

  ///-------
  //mass.setUnit("GeV/c^{2}");


  //-----------------------
  //The resolution functions
  //-----------------------   
  resmean.setVal(1.03881e-04);resmean.setConstant(1);
  resoffset.setVal(-.00804);resoffset.setConstant(1);
  resslope.setVal(.0043);resslope.setConstant(1);

  resnormratio2.setVal(8.41154e-02);resnormratio2.setConstant(1);
  resmean2.setVal(-5.56512e-03);resmean2.setConstant(1);
  ressigmaratio2.setVal(1.57893e+00);ressigmaratio2.setConstant(1);
  
  resnormratio3.setVal(2.59953e-02);resnormratio3.setConstant(1);  
  resmean3.setVal(6.06654e-03);resmean3.setConstant(1);
  ressigmaratio3.setVal(7.89734e-01);ressigmaratio3.setConstant(1);
 


  mass.setVal(2.460);
  //respdf=new RooResolutionPdf("respdf","respdf",&resVar,mass,resoffset,resslope,resmean,resnormratio2,resmean2,ressigmaratio2,resnormratio3,resmean3,ressigmaratio3);
  //use a simple gaussian above model causes convergence problems !!!
  resoffset.setVal(.0035);resoffset.setConstant(1);
  respdf=new RooGaussian("respdf","respdf",resVar,resmean,resoffset);   

  mass.setVal(2.560);
  //check the normalization
  Float_t step=(resVar.getMax()-resVar.getMin())/300;
  Float_t integ=0;
  for(Int_t i=0;i<300;i++){
    resVar.setVal(resVar.getMin()+step*(i+.5));
    integ+=respdf->getVal()*step;
  }
  cout<<"Check Resolution Normalization at m="<<mass.getVal()<<"  ==>"<<integ<<endl;
  massfitter.SetResolution(respdf,&resVar);
  
  
  
  //massfitter.SetEfficiency(effHisto);

  ///////////
  massfitter.SetPhaseSpace(&TwoBodyPS);
  massfitter.SetEps(.001);
  massfitter.SetDauMasses(&dau1m,&dau2m);


  ////ccbar scaling factor
  Int_t Nccbar=1088218000;
  Float_t Crossccbar=1.30;//nanobarns
  Float_t lumiccbar=Nccbar/Crossccbar;
  Float_t lumidata=470000000;
  scalefactorccbar=(float)(lumidata/lumiccbar);
  cout<<" ccbar lumi scale factor "<<scalefactorccbar<<endl;


///WS MC/DATA Ratio
//   FWSRatio= new TF1("FWSRatio","([0]+[1]*x+[2]*x*x+[3]*x*x*x)*(x<=2.3) + ([0]+[1]*2.3+[2]*2.3*2.3+[3]*2.3*2.3*2.3+[4]*(x-2.3)+[5]*(x-2.3)*(x-2.3)+[6]*(x-2.3)*(x-2.3)*(x-2.3))*(x>2.3)",2.,4);//note pieces join at x=2.3
//   FWSRatio->SetParameters(-69.4335,109.882,-56.2531,9.46979,-0.135379,-0.356922,0.523725);




 
  ///
  SystDirs[0]="Nominal";
  SystDirs[1]="SystMassRange";
  SystDirs[2]="SystBinWidth";
  SystDirs[3]="SystBWShape";
  SystDirs[4]="SystFeedDownMean";
  SystDirs[5]="SystFeedDownWidth";
  SystDirs[6]="SystBkgModel";
  SystDirs[7]="0";
  SystDirs[8]="SystFeedD2430";
  SystDirs[9]="0";
 
  SystName[0]="";
  SystName[1]="Mass Range";
  SystName[2]="Bin Width";
  SystName[3]="Breit-Wigner Shape";
  SystName[4]="Feed-Down Mass";
  SystName[5]="Feed-Down Width";
  SystName[6]="Background Model";
  SystName[7]="B field \\& SVT material";
  SystName[8]="Feed-Down D(2430)";
  SystName[9]="0";

  SigName[0]="D2400";
  SigName[1]="D2460";
  SigName[2]="D2620";
  SigName[3]="D2750";
  SigName[4]="0";

  SigTitle[0]="\\DTwentyFourHundred";
  SigTitle[1]="\\DTwentyFourSixty";
  SigTitle[2]="\\DTwentySixHundred";
  SigTitle[3]="\\DTwentySevenSixty";
  SigTitle[4]="";
  
 

  cout<<"Done Initializing the analysis"<<endl;
}

DCPiAnalysis::~DCPiAnalysis(){  
}

Int_t DCPiAnalysis::ReduceNtuple(){
  cout<<"Going to reduce the ntuple"<<endl;

  if(_fChain==NULL){
    cout<<"No chain."<<endl;
    return 0;
  } 

  ///link the branches in the input chain
  SetDCPiBranches();


  //the file must be created here so the 'new' histos and Tree is created in hard drive //OutFile=new 
  TFile OutFile(_OutputDir+"/Ntuple_New.root","recreate",_OutputDir+"/Ntuple_New.root",1);//1=minimal compression
  if(OutFile.IsZombie()){cout<<"Failed to create file"<<endl;return 0;}


  //create new reduced ntuple
  TTree Ntuple("Ntuple","Ntuple",0);
  Ntuple.SetMaxTreeSize(900000000);

  //disable autosaving 
  Ntuple.SetAutoSave(900000000);

  //create the branches in the Ntuple
  MakeDCPiReducedNtupleBranches(&Ntuple);


  cout<<" MCorDATA "<<_MCorDATA<<endl;
  /////
  cout<<"Will use the following cuts"<<endl;
  cout<<" cuttruth "<<cuttruth
      <<" cutcharge "<<cutcharge
      <<" cutpstar "<<cutpstar
      <<" cutdflight "<<cutdflight
      <<" cutdmass "<<cutdmass
      <<" cutcone "<<cutcone
      <<" cutdcos "<<cutdcos<<endl;

  //Counters
  Int_t RecoXCounterTotal=0;
  

  cout<<"Total Entries in Chain="<<_fChain->GetEntries()<<endl;
  cout<<"Going to start the event loop"<<endl;
  eventid=0;
  Int_t totalgoodrecos=0;
  while(_fChain->GetEntry(eventid,0)>0){   
    eventid++;
    
    if(eventid%100000==0){
      cout<<eventid<<" Events done.   With "<<RecoXCounterTotal<<" Candidates saved."<<endl;    
    }

    ///Event vars
    FillEventVars();
   

    ///
    if(nX>MAXNCANDS){
      cout<<"Too many cands at event "<<eventid<<" Only first "<<MAXNCANDS<<" of "<<nX<<" will be used"<<endl;
      nX=MAXNCANDS;      
    }
    
    //now do real loop
    Int_t ngoodrecos=0;
    for(XIdx=0;XIdx<nX;XIdx++){
     
      ///determine the indexes for each particle before used by other methods
      DIdx=Xd1Idx[XIdx];
      Pi1Idx=Xd2Idx[XIdx];      
      KIdx=Dd1Idx[DIdx];
      PiIdx=Dd2Idx[DIdx];
      DPi2Idx=Dd3Idx[DIdx];      

   
      //
      FillDCPiVars();     
      
             
      if(cutcharge==1)if(abs(xcharge)!=1) continue;//right sign D+pi-=+1, D-pi+=-1
      if(cutcharge==2)if(abs(xcharge)!=2) continue;//select wrong sign D+pi+=+2, D-pi-=-2
      if(cutpstar==1)if(xpstar<PstarMin) continue;
      if(cutdflight==1)if(dflights<DFlightCut) continue;      
      if(cutdmass==1)if(dmass<LowMassCut||HighMassCut<dmass) continue;
      if(cutdmass==2)if(!((LowSideLowMassCut<dmass&&dmass<LowSideHighMassCut)||(HighSideLowMassCut<dmass&&dmass<HighSideHighMassCut))) continue;//select sideband                 
      if(cutdcos==1)if(dcostheta<DCosMin) continue;//select DCos


      ngoodrecos++;
      totalgoodrecos++;
      if(ngoodrecos>1)continue;

      RecoXCounterTotal++;
      Ntuple.Fill();
   
    }//X loop    
      

  }
  ///
  //CloseLikelihood();

  //print summary
  cout<<"--------Summary-------"<<endl;
  cout<<"Total events = "<<eventid<<endl;
  cout<<"Total reco ="<<RecoXCounterTotal<<endl;
  cout<<"reconstruction multiplicity ="<<totalgoodrecos/(float)eventid<<endl;
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



void DCPiAnalysis::FillDCPiVars(){

  //
  FillDCVars();
  
  //////fill reduced ntuple variables    
  xmass=XMass[XIdx];
  xpstar=Xp3CM[XIdx];
  xpstarfrac=xpstar*2*beamenergy/(beamenergy*beamenergy-XMass[XIdx]*XMass[XIdx]);
  xcosstar=XcosthCM[XIdx];
  xphistar=XphiCM[XIdx];
  xdeltam=XMass[XIdx]-DMass[DIdx];  
  xmdm=xdeltam+DPlusPDGMass;
  xenergy=sqrt(XMass[XIdx]*XMass[XIdx]+Xp3CM[XIdx]*Xp3CM[XIdx]);
  xenergyfrac=xenergy*2*beamenergy/(beamenergy*beamenergy+XMass[XIdx]*XMass[XIdx]);
  if(TMath::Prob(XChi2[XIdx],XnDof[XIdx])>1e-300)
    xlogvtxprob=log10(TMath::Prob(XChi2[XIdx],XnDof[XIdx]));
  else xlogvtxprob=-100;
  //Determine D*pi combination: xcharge= -2=D*-pi- , -1=D*-pi+  , 1=D*+pi- , 2=D*+pi+
  xcharge=DLund[DIdx]/abs(DLund[DIdx])+PiLund[Pi1Idx]/abs(PiLund[Pi1Idx]);
  if(xcharge==0)xcharge=DLund[DIdx]/abs(DLund[DIdx]);   

  evtmultiplicity=Xexpartn[XIdx];


  //truth match
  xmctrue=0;
  xlund=0;
  xdmres=1;
  dmctrue=0;
  if(_MCorDATA==0){
    xmctrue=(XMyMCIdx[XIdx]>0);
    if(xmctrue) xlund=mcLund[XMyMCIdx[XIdx]];
    if(xmctrue) xdmres=xdeltam-(mcmass[XMyMCIdx[XIdx]]-mcmass[dauIdx[XMyMCIdx[XIdx]]]);   
    //cant tell dmctrue
  }
  if(_MCorDATA==1){//the XMyMCIdx is a bit weird has different meanings in signal vs generic
    xmctrue=(abs(XMyMCIdx[XIdx])>=425);//D10=10423,D2*0=425, D10'=20423, D0*0=10421
    xlund=XMyMCIdx[XIdx];
    dmctrue=(abs(XMyMCIdx[XIdx])==myDPlusLund||abs(XMyMCIdx[XIdx])==myDstarLund||abs(XMyMCIdx[XIdx])==myDstar0Lund||xmctrue);
  }

  //extra pion
  pi1p3=Pip3[Pi1Idx];      
  pi1charge=PiLund[Pi1Idx]/abs(PiLund[Pi1Idx]);
  pi1mothcosine=ComputePiMotherAngle();
  pi1costheta=ComputePiAngle();



  //-------D quantities
  dcostheta=-ComputeDAngle();//redefined as angle of the pion 12/12/2009
  dflight=ComputeDSFDistance();//redefined
  dflights=XDflight[XIdx]/XDflighte[XIdx];//redefined
  //override some vars
  dlogvtxprob=0;//redefined because I do not fit D+ vertex
  Kpimass=XKpimass[XIdx];//redefined because mass constrain D
  Kpi2mass=XKpi2mass[XIdx];
  if(Kpimass>Kpi2mass){
    KpimassFold=Kpimass;
    Kpi2massFold=Kpi2mass;
  }else{
    KpimassFold=Kpi2mass;
    Kpi2massFold=Kpimass;
  }

  kcosine=ComputeKAngle();

  //2-Body Masses
  pi1pimass=ComputexyMass(Pip3CM[Pi1Idx],PicosthCM[Pi1Idx],PiphiCM[Pi1Idx],PiPDGMass,
			  Pip3CM[PiIdx],PicosthCM[PiIdx],PiphiCM[PiIdx],PiPDGMass);
  pi1pi2mass=ComputexyMass(Pip3CM[Pi1Idx],PicosthCM[Pi1Idx],PiphiCM[Pi1Idx],PiPDGMass,
			   Pip3CM[DPi2Idx],PicosthCM[DPi2Idx],PiphiCM[DPi2Idx],PiPDGMass);

  //3-Body masses
  pi1Kmass=ComputexyMass(Kp3CM[KIdx],KcosthCM[KIdx],KphiCM[KIdx],KPDGMass,
			 Pip3CM[Pi1Idx],PicosthCM[Pi1Idx],PiphiCM[Pi1Idx],PiPDGMass);
  pi1Kpimass=ComputexyzMass(Pip3CM[Pi1Idx],PicosthCM[Pi1Idx],PiphiCM[Pi1Idx],PiPDGMass,
			    Kp3CM[KIdx],KcosthCM[KIdx],KphiCM[KIdx],KPDGMass,
			    Pip3CM[PiIdx],PicosthCM[PiIdx],PiphiCM[PiIdx],PiPDGMass);      
  pi1Kpi2mass=ComputexyzMass(Pip3CM[Pi1Idx],PicosthCM[Pi1Idx],PiphiCM[Pi1Idx],PiPDGMass,
			     Kp3CM[KIdx],KcosthCM[KIdx],KphiCM[KIdx],KPDGMass,
			     Pip3CM[DPi2Idx],PicosthCM[DPi2Idx],PiphiCM[DPi2Idx],PiPDGMass); 
  pi1pipi2mass=ComputexyzMass(Pip3CM[Pi1Idx],PicosthCM[Pi1Idx],PiphiCM[Pi1Idx],PiPDGMass,
			      Pip3CM[PiIdx],PicosthCM[PiIdx],PiphiCM[PiIdx],PiPDGMass,
			      Pip3CM[DPi2Idx],PicosthCM[DPi2Idx],PiphiCM[DPi2Idx],PiPDGMass);
  
      
}




Float_t DCPiAnalysis::ComputeDAngle(){
  //Angle of the D in the X frame
  TVector3 Xp3vec(Xp3CM[XIdx]*sin(acos(XcosthCM[XIdx]))*cos(XphiCM[XIdx]),
                  Xp3CM[XIdx]*sin(acos(XcosthCM[XIdx]))*sin(XphiCM[XIdx]),
                  Xp3CM[XIdx]*XcosthCM[XIdx]);
  
  Float_t XEnergy=sqrt(Xp3vec*Xp3vec + XMass[XIdx]*XMass[XIdx]);
  TVector3 XVelvec=(1/XEnergy)*Xp3vec;
  
  TVector3 Dp3vec(Dp3CM[DIdx]*sin(acos(DcosthCM[DIdx]))*cos(DphiCM[DIdx]),
		  Dp3CM[DIdx]*sin(acos(DcosthCM[DIdx]))*sin(DphiCM[DIdx]),
		  Dp3CM[DIdx]*DcosthCM[DIdx]);
  
  TLorentzVector Dp4(Dp3vec,sqrt(Dp3vec*Dp3vec+DMass[DIdx]*DMass[DIdx]));
  
  //boost the dstar momentum into rest frame of X
  Dp4.Boost(-XVelvec);//watch the minus sign
  TVector3 DBoostedp3vec=Dp4.Vect();
  
  //dot product of boosted dstar direction with X direction
  return DBoostedp3vec.Unit()*Xp3vec.Unit();
  
}


Float_t DCPiAnalysis::ComputeMCDAngle(){
  //Angle of the D in the X frame
  TVector3 Xp3vec(Xp3CM[XIdx]*sin(acos(XcosthCM[XIdx]))*cos(XphiCM[XIdx]),
                  Xp3CM[XIdx]*sin(acos(XcosthCM[XIdx]))*sin(XphiCM[XIdx]),
                  Xp3CM[XIdx]*XcosthCM[XIdx]);
  
  Float_t XEnergy=sqrt(Xp3vec*Xp3vec + XMass[XIdx]*XMass[XIdx]);
  TVector3 XVelvec=(1/XEnergy)*Xp3vec;
  
  TVector3 Dp3vec(Dp3CM[DIdx]*sin(acos(DcosthCM[DIdx]))*cos(DphiCM[DIdx]),
		  Dp3CM[DIdx]*sin(acos(DcosthCM[DIdx]))*sin(DphiCM[DIdx]),
		  Dp3CM[DIdx]*DcosthCM[DIdx]);
  
  TLorentzVector Dp4(Dp3vec,sqrt(Dp3vec*Dp3vec+DMass[DIdx]*DMass[DIdx]));
  
  //boost the dstar momentum into rest frame of X
  Dp4.Boost(-XVelvec);//watch the minus sign
  TVector3 DBoostedp3vec=Dp4.Vect();
  
  //dot product of boosted dstar direction with X direction
  return DBoostedp3vec.Unit()*Xp3vec.Unit();
  
}
          




Float_t DCPiAnalysis::ComputePiAngle(){
  //Angle of the Extra pion wrt the X direction in the X frame
  TVector3 Xp3vec(Xp3CM[XIdx]*sin(acos(XcosthCM[XIdx]))*cos(XphiCM[XIdx]),
                  Xp3CM[XIdx]*sin(acos(XcosthCM[XIdx]))*sin(XphiCM[XIdx]),
                  Xp3CM[XIdx]*XcosthCM[XIdx]);
  
  Float_t XEnergy=sqrt(Xp3vec*Xp3vec + XMass[XIdx]*XMass[XIdx]);
  TVector3 XVelvec=(1/XEnergy)*Xp3vec;
  
  TVector3 Pip3vec(Pip3CM[Pi1Idx]*sin(acos(PicosthCM[Pi1Idx]))*cos(PiphiCM[Pi1Idx]),
                   Pip3CM[Pi1Idx]*sin(acos(PicosthCM[Pi1Idx]))*sin(PiphiCM[Pi1Idx]),
                   Pip3CM[Pi1Idx]*PicosthCM[Pi1Idx]);
  
  TLorentzVector Pip4(Pip3vec,sqrt(Pip3vec*Pip3vec+PiPDGMass*PiPDGMass));
  
  //boost the dstar momentum into rest frame of X
  Pip4.Boost(-XVelvec);//watch the minus sign
  TVector3 PiBoostedp3vec=Pip4.Vect();
  
  //dot product of boosted dstar direction with X direction
  return PiBoostedp3vec.Unit()*Xp3vec.Unit();
  
}


Float_t DCPiAnalysis::ComputeKAngle(){
  //Angle of the Extra pion wrt the X direction in the X frame
  TVector3 Dp3vec(Dp3CM[DIdx]*sin(acos(DcosthCM[DIdx]))*cos(DphiCM[DIdx]),
		  Dp3CM[DIdx]*sin(acos(DcosthCM[DIdx]))*sin(DphiCM[DIdx]),
		  Dp3CM[DIdx]*DcosthCM[DIdx]);
                

  Float_t DEnergy=sqrt(Dp3vec*Dp3vec + DMass[DIdx]*DMass[DIdx]);
  TVector3 DVelvec=(1/DEnergy)*Dp3vec;
                                                              
  //boost the Pi in to the D rest frame
  TVector3 Pi1p3vec(Pip3CM[Pi1Idx]*sin(acos(PicosthCM[Pi1Idx]))*cos(PiphiCM[Pi1Idx]),
                  Pip3CM[Pi1Idx]*sin(acos(PicosthCM[Pi1Idx]))*sin(PiphiCM[Pi1Idx]),
                  Pip3CM[Pi1Idx]*PicosthCM[Pi1Idx]);
  
  TLorentzVector Pi1p4(Pi1p3vec,sqrt(Pi1p3vec*Pi1p3vec+PiPDGMass*PiPDGMass));
  Pi1p4.Boost(-DVelvec);//watch the minus sign
  TVector3 Pi1Boostedp3vec=Pi1p4.Vect();
  

  //boost the K into the D rest frame
  TVector3 Kp3vec(Kp3CM[KIdx]*sin(acos(KcosthCM[KIdx]))*cos(KphiCM[KIdx]),
                   Kp3CM[KIdx]*sin(acos(KcosthCM[KIdx]))*sin(KphiCM[KIdx]),
                   Kp3CM[KIdx]*KcosthCM[KIdx]);
  
  TLorentzVector Kp4(Kp3vec,sqrt(Kp3vec*Kp3vec+KPDGMass*KPDGMass));
  Kp4.Boost(-DVelvec);//watch the minus sign
  TVector3 KBoostedp3vec=Kp4.Vect();
  
  //
  return KBoostedp3vec.Unit()*Pi1Boostedp3vec.Unit();
  
}
        



Float_t DCPiAnalysis::ComputePiMotherAngle(){//cone angle in CM frame
  TVector3 Xp3vec(Xp3CM[XIdx]*sin(acos(XcosthCM[XIdx]))*cos(XphiCM[XIdx]),
                  Xp3CM[XIdx]*sin(acos(XcosthCM[XIdx]))*sin(XphiCM[XIdx]),
                  Xp3CM[XIdx]*XcosthCM[XIdx]);
  
  TVector3 Pip3vec(Pip3CM[Pi1Idx]*sin(acos(PicosthCM[Pi1Idx]))*cos(PiphiCM[Pi1Idx]),
                   Pip3CM[Pi1Idx]*sin(acos(PicosthCM[Pi1Idx]))*sin(PiphiCM[Pi1Idx]),
                   Pip3CM[Pi1Idx]*PicosthCM[Pi1Idx]);
  
  
  return Xp3vec.Unit()*Pip3vec.Unit();
  
}


Float_t DCPiAnalysis::ComputeDSFDistance(){

                                                
  TVector3 Dp3Unit(sin(acos(Dcosth[DIdx]))*cos(Dphi[DIdx]),
		  sin(acos(Dcosth[DIdx]))*sin(Dphi[DIdx]),
		  Dcosth[DIdx]);
  

  TVector3 DFlight3D(DVtxx[DIdx]-XVtxx[XIdx],
		     DVtxy[DIdx]-XVtxy[XIdx],
		     DVtxz[DIdx]-XVtxz[XIdx]);
     
  //dot product of boosted dstar direction with X direction
  //if(fabs(fabs(DFlight3D.Unit()*Dp3Unit)-1)>1e-6){cout<<"not parallel  "<<fabs(fabs(DFlight3D.Unit()*Dp3Unit)-1)<<endl;}  
  return DFlight3D*Dp3Unit;
}


Int_t DCPiAnalysis::SetDCPiBranches(){
  if(_fChain==NULL){cout<<"No Chain"<<endl;return 0;} 

  SetDCBranches();

  _fChain->SetBranchAddress("nX",&nX);
  _fChain->SetBranchAddress("XMass",XMass);
  _fChain->SetBranchAddress("Xp3",Xp3);
  _fChain->SetBranchAddress("Xcosth",Xcosth);
  _fChain->SetBranchAddress("Xphi",Xphi);
  _fChain->SetBranchAddress("Xp3CM",Xp3CM);
  _fChain->SetBranchAddress("XcosthCM",XcosthCM);
  _fChain->SetBranchAddress("XphiCM",XphiCM);
  _fChain->SetBranchAddress("XLund",XLund); 
  _fChain->SetBranchAddress("Xd1Lund",Xd1Lund);
  _fChain->SetBranchAddress("Xd1Idx",Xd1Idx);
  _fChain->SetBranchAddress("Xd2Lund",Xd2Lund);
  _fChain->SetBranchAddress("Xd2Idx",Xd2Idx);
  _fChain->SetBranchAddress("XVtxx",XVtxx);
  _fChain->SetBranchAddress("XVtxy",XVtxy);
  _fChain->SetBranchAddress("XVtxz",XVtxz);
  _fChain->SetBranchAddress("XChi2",XChi2);
  _fChain->SetBranchAddress("XnDof",XnDof);
  _fChain->SetBranchAddress("XVtxStatus",XVtxStatus);


  _fChain->SetBranchAddress("DFlightLen",DFlightLen);
  _fChain->SetBranchAddress("XDmass",XDmass);
  _fChain->SetBranchAddress("XKpimass",XKpimass);
  _fChain->SetBranchAddress("XKpi2mass",XKpi2mass);
  _fChain->SetBranchAddress("XDflight",XDflight);
  _fChain->SetBranchAddress("XDflighte",XDflighte);
  _fChain->SetBranchAddress("Xexpartpx",Xexpartpx);
  _fChain->SetBranchAddress("Xexpartpy",Xexpartpy);
  _fChain->SetBranchAddress("Xexpartpz",Xexpartpz);
  _fChain->SetBranchAddress("Xexparte",Xexparte);
  _fChain->SetBranchAddress("Xexpartn",Xexpartn);


  if(_MCorDATA<2){
    _fChain->SetBranchAddress("XMCIdx",XMCIdx);
    _fChain->SetBranchAddress("XMyMCIdx",XMyMCIdx);
  } 

  cout<<"Done linking DCPi  Chain Branches"<<endl;
  return 1;
}



Int_t DCPiAnalysis::MakeDCPiReducedNtupleBranches(TTree* Ntuple){
 
  Ntuple->Branch("xmass",&xmass,"xmass/F");
  Ntuple->Branch("xdeltam",&xdeltam,"xdeltam/F");
  Ntuple->Branch("xmdm",&xmdm,"xmdm/F");
  Ntuple->Branch("xdmres",&xdmres,"xdmres/F");
  Ntuple->Branch("xenergy",&xenergy,"xenergy/F");
  Ntuple->Branch("xenergyfrac",&xenergyfrac,"xenergyfrac/F");
  Ntuple->Branch("xpstar",&xpstar,"xpstar/F");
  Ntuple->Branch("xpstarfrac",&xpstarfrac,"xpstarfrac/F");
  Ntuple->Branch("xcosstar",&xcosstar,"xcosstar/F");
  Ntuple->Branch("xphistar",&xphistar,"xphistar/F");
  Ntuple->Branch("xcharge",&xcharge,"xcharge/I");
  Ntuple->Branch("nX",&nX,"nX/I");
  Ntuple->Branch("xlogvtxprob",&xlogvtxprob,"xlogvtxprob/F");
  Ntuple->Branch("XIdx",&XIdx,"XIdx/I");
  //  Ntuple->Branch("xbestcand",&xbestcand,"xbestcand/I");
  Ntuple->Branch("xmctrue",&xmctrue,"xmctrue/I"); 
  Ntuple->Branch("xlund",&xlund,"xlund/I"); 
  Ntuple->Branch("xvtxstatus",&xvtxstatus,"xvtxstatus/I");
  Ntuple->Branch("xlogL",&xlogL,"xlogL/F");
  Ntuple->Branch("xlogLM",&xlogLM,"xlogLM/F");
  Ntuple->Branch("xlogLF",&xlogLF,"xlogLF/F");
  Ntuple->Branch("xlogLD",&xlogLD,"xlogLD/F");
  Ntuple->Branch("xlogLP",&xlogLP,"xlogLP/F");
  Ntuple->Branch("xlogLC",&xlogLC,"xlogLC/F");

  Ntuple->Branch("dcostheta",&dcostheta,"dcostheta/F");
  Ntuple->Branch("dflight",&dflight,"dflight/F");
  Ntuple->Branch("dflights",&dflights,"dflights/F");
  Ntuple->Branch("kcosine",&kcosine,"kcosine/F");
   
  Ntuple->Branch("pi1p3",&pi1p3,"pi1p3/F");
  Ntuple->Branch("pi1charge",&pi1charge,"pi1charge/I");
  Ntuple->Branch("pi1dedx",&pi1dedx,"pi1dedx/F");
  Ntuple->Branch("pi1thetac",&pi1thetac,"pi1thetac/F");
  Ntuple->Branch("pi1lh",&pi1lh,"pi1lh/I");
  Ntuple->Branch("pi1costheta",&pi1costheta,"pi1costheta/F");
  Ntuple->Branch("pi1mothcosine",&pi1mothcosine,"pi1mothcosine/F");

  Ntuple->Branch("pi1Kmass",&pi1Kmass,"pi1Kmass/F");
  Ntuple->Branch("pi1Kpimass",&pi1Kpimass,"pi1Kpimass/F");
  Ntuple->Branch("pi1Kpi2mass",&pi1Kpi2mass,"pi1Kpi2mass/F");  
  Ntuple->Branch("pi1pimass",&pi1pimass,"pi1pimass/F");
  Ntuple->Branch("pi1pipi2mass",&pi1pipi2mass,"pi1pipi2mass/F");
  Ntuple->Branch("pi1pi2mass",&pi1pi2mass,"pi1pi2mass/F");

  Ntuple->Branch("evtmultiplicity",&evtmultiplicity,"evtmultiplicity/I");


  Ntuple->Branch("recoile",&recoile,"recoile/F");
  Ntuple->Branch("recoilp",&recoilp,"recoilp/F");
  Ntuple->Branch("recoilm",&recoilm,"recoilm/F");
  Ntuple->Branch("recoildmass",&recoildmass,"recoildmass/F");
  Ntuple->Branch("recoilpimass",&recoilpimass,"recoilpimass/F");
  Ntuple->Branch("recoilcosine",&recoilcosine,"recoilcosine/F");
  Ntuple->Branch("recoilpicosine",&recoilpicosine,"recoilpicosine/F");

  MakeDCReducedNtupleBranches(Ntuple);
  cout<<"Done Making  DCPi  Reduced Ntuple Branches"<<endl;
  return 1;
}



Int_t DCPiAnalysis::SetDCPiReducedNtupleBranches(){


  ReducedNtuple->SetBranchAddress("xmass",&xmass);
  ReducedNtuple->SetBranchAddress("xdeltam",&xdeltam);
  ReducedNtuple->SetBranchAddress("xmdm",&xmdm);
  ReducedNtuple->SetBranchAddress("xdmres",&xdmres);
  ReducedNtuple->SetBranchAddress("xenergy",&xenergy);
  ReducedNtuple->SetBranchAddress("xenergyfrac",&xenergyfrac);
  ReducedNtuple->SetBranchAddress("xpstar",&xpstar);
  ReducedNtuple->SetBranchAddress("xpstarfrac",&xpstarfrac);
  ReducedNtuple->SetBranchAddress("xcosstar",&xcosstar);
  ReducedNtuple->SetBranchAddress("xphistar",&xphistar);
  ReducedNtuple->SetBranchAddress("XIdx",&XIdx);
  ReducedNtuple->SetBranchAddress("xcharge",&xcharge);
  ReducedNtuple->SetBranchAddress("nX",&nX);
  ReducedNtuple->SetBranchAddress("xlogvtxprob",&xlogvtxprob);
  //ReducedNtuple->SetBranchAddress("xbestcand",&xbestcand);
  ReducedNtuple->SetBranchAddress("xmctrue",&xmctrue);
  ReducedNtuple->SetBranchAddress("xlund",&xlund);
  ReducedNtuple->SetBranchAddress("xvtxstatus",&xvtxstatus);
//   ReducedNtuple->SetBranchAddress("xlogL",&xlogL);
//   ReducedNtuple->SetBranchAddress("xlogLM",&xlogLM);
//   ReducedNtuple->SetBranchAddress("xlogLF",&xlogLF);
//   ReducedNtuple->SetBranchAddress("xlogLD",&xlogLD);
//   ReducedNtuple->SetBranchAddress("xlogLP",&xlogLP);
//   ReducedNtuple->SetBranchAddress("xlogLC",&xlogLC);

  ReducedNtuple->SetBranchAddress("dcostheta",&dcostheta);
  ReducedNtuple->SetBranchAddress("dflight",&dflight);
  ReducedNtuple->SetBranchAddress("dflights",&dflights);
  ReducedNtuple->SetBranchAddress("kcosine",&kcosine);

  ReducedNtuple->SetBranchAddress("pi1p3",&pi1p3);
  ReducedNtuple->SetBranchAddress("pi1charge",&pi1charge);
  ReducedNtuple->SetBranchAddress("pi1dedx",&pi1dedx);
  ReducedNtuple->SetBranchAddress("pi1thetac",&pi1thetac);
  ReducedNtuple->SetBranchAddress("pi1lh",&pi1lh);
  ReducedNtuple->SetBranchAddress("pi1costheta",&pi1costheta);
  ReducedNtuple->SetBranchAddress("pi1mothcosine",&pi1mothcosine);

  ReducedNtuple->SetBranchAddress("pi1Kmass",&pi1Kmass);
  ReducedNtuple->SetBranchAddress("pi1Kpimass",&pi1Kpimass);
  ReducedNtuple->SetBranchAddress("pi1Kpi2mass",&pi1Kpi2mass);  
  ReducedNtuple->SetBranchAddress("pi1pimass",&pi1pimass);
  ReducedNtuple->SetBranchAddress("pi1pipi2mass",&pi1pipi2mass);
  ReducedNtuple->SetBranchAddress("pi1pi2mass",&pi1pi2mass);

  ReducedNtuple->SetBranchAddress("recoile",&recoile);
  ReducedNtuple->SetBranchAddress("recoilp",&recoilp);
  ReducedNtuple->SetBranchAddress("recoilm",&recoilm);
  ReducedNtuple->SetBranchAddress("recoildmass",&recoildmass);
  ReducedNtuple->SetBranchAddress("recoilpimass",&recoilpimass);
  ReducedNtuple->SetBranchAddress("recoilcosine",&recoilcosine);
  ReducedNtuple->SetBranchAddress("recoilpicosine",&recoilpicosine);


  SetDCReducedNtupleBranches();
  cout<<"Done linking DCPi ReducedNtuple Branches"<<endl;
  return 1;
}



Int_t DCPiAnalysis::DefaultPlots(){
  //RS
  if(OpenReducedNtuple()!=1) return 0;

  filename=_OutputDir+"/DefaultPlots.ps";
  Canvas.Print(filename+"["); 

  TH1F HDCPiMass("HDCPiMass","",XMassBins,XMassMin,XMassMax);
  HDCPiMass.GetXaxis()->SetTitle(XMassLabel);
  HDCPiMass.GetYaxis()->SetTitle("Entries/5 MeV");
  ReducedNtuple->Draw("xmdm>>HDCPiMass");
  Canvas.Clear();
  HDCPiMass.Draw("pe");
  Canvas.Print(filename);
 

  if(_MCorDATA<2){    
    //Show the different signals in the Generic truth
    TH1F HDCPiMassBkg("HDCPiMassBkg","",XMassBins,XMassMin,XMassMax);
    HDCPiMassBkg.GetXaxis()->SetTitle(XMassLabel);
    HDCPiMassBkg.GetYaxis()->SetTitle("Entries/5 MeV");
    ReducedNtuple->Draw("xmdm>>HDCPiMassBkg","xmctrue<=0");
    Canvas.Clear();
    HDCPiMass.Draw("pe");
    HDCPiMassBkg.Draw("same");
    Canvas.Print(filename);
      
    //separate the signal
    TH1F HDCPiMassSig("HDCPiMassSig","",XMassBins,XMassMin,XMassMax);
    HDCPiMassSig.GetXaxis()->SetTitle(XMassLabel);
    HDCPiMassSig.GetYaxis()->SetTitle("Entries/5 MeV");
    ReducedNtuple->Draw("xmdm>>HDCPiMassSig","xmctrue>0");
    Canvas.Clear();
    HDCPiMassSig.Draw("pe");
    ReducedNtuple->Draw("xmdm","abs(xlund)==10423","same");//D(2420)
    ReducedNtuple->Draw("xmdm","abs(xlund)==425","same");//D(2460)
    ReducedNtuple->Draw("xmdm","abs(xlund)==20423","same");//D(2430)
    ReducedNtuple->Draw("xmdm","abs(xlund)==10421","same");//D(2400)
    Canvas.Print(filename);


    //Compare Signal and Bkg p* distribution
    TH1F HDCPiPstarSig("HDCPiPstarSig","",110,2.8,5.);
    HDCPiPstarSig.Sumw2();
    ReducedNtuple->Draw("xpstar>>HDCPiPstarSig","xmctrue>0");
    HDCPiPstarSig.Scale(1./HDCPiPstarSig.Integral());
    TH1F HDCPiPstarBkg("HDCPiPstarBkg","",110,2.8,5.);
    HDCPiPstarBkg.Sumw2();
    ReducedNtuple->Draw("xpstar>>HDCPiPstarBkg","xmctrue<=0");
    HDCPiPstarBkg.Scale(1./HDCPiPstarBkg.Integral());
    Canvas.Clear();
    HDCPiPstarBkg.GetYaxis()->SetRangeUser(0,HDCPiPstarBkg.GetMaximum()*1.2);
    HDCPiPstarBkg.GetXaxis()->SetTitle("p*(D^{+}#pi^{-})  GeV/c");
    //HDCPiPstarBkg.GetYaxis()->SetTitle("");
    HDCPiPstarBkg.Draw("hist ");
    HDCPiPstarSig.Draw("pe same");
    Canvas.Print(filename);

    //Compare Signal and Bkg DCos distribution
    TH1F HDCPiDCosSig("HDCPiDCosSig","",100,-1.001,1.001);
    HDCPiDCosSig.Sumw2();
    ReducedNtuple->Draw("dcostheta>>HDCPiDCosSig","xmctrue>0");
    HDCPiDCosSig.Scale(1./HDCPiDCosSig.Integral());
    TH1F HDCPiDCosBkg("HDCPiDCosBkg","",100,-1.001,1.001);
    HDCPiDCosBkg.Sumw2();
    ReducedNtuple->Draw("dcostheta>>HDCPiDCosBkg","xmctrue<=0");
    HDCPiDCosBkg.Scale(1./HDCPiDCosBkg.Integral());
    Canvas.Clear();
    HDCPiDCosBkg.GetYaxis()->SetRangeUser(0,HDCPiDCosBkg.GetMaximum()*1.2);
    HDCPiDCosBkg.GetXaxis()->SetTitle("cos(#theta_{#pi})");
    //HDCPiDCosBkg.GetYaxis()->SetTitle("");
    HDCPiDCosBkg.Draw("hist ");
    HDCPiDCosSig.Draw("pe same");
    cutline.DrawLine(DCosMin,0,DCosMin,HDCPiDCosBkg.GetMaximum());
    Canvas.Print(filename);


  }

  Canvas.Print(filename+"]");

  return 1;
}


Int_t DCPiAnalysis::MakeHistosForFit(){

  if(OpenReducedNtuple()!=1) return 0;  
  SetDCPiReducedNtupleBranches();
  ///-------------------------------------------
  cout<<"Create root file with histos for fit."<<endl;
  ///------------------------------------------------


  TH1F HDCPi("HMass","",XMassBins,XMassMin,XMassMax);

  TH1F HDCPiSignal("HMassSignal","",XMassBins,XMassMin,XMassMax);
  TH1F HDCPiSignalTrueMass("HMassSignalTrueMass","",XMassBins,XMassMin,XMassMax);
  TH1F HDCPiBkg("HMassBkg","",XMassBins,XMassMin,XMassMax);

  TH1F* HDCPiVsDCosCoarse[10];
  for(Int_t i=0;i<10;i++)
    HDCPiVsDCosCoarse[i]=new TH1F(TString("HMass_DCosCoarse")+(long)i,"",XMassBins,XMassMin,XMassMax);
 

  TH1F* HDCPiVspstar[20];
  for(Int_t i=0;i<20;i++)
    HDCPiVspstar[i]=new TH1F(TString("HMass_pstar")+(long)i,"",XMassBins,XMassMin,XMassMax);
  

  TH1F HpstarTrue("HpstarTrue","",25,2.5,5.0);

  ///
  Int_t e=0;
  while(ReducedNtuple->GetEntry(e)){
    e++;
    if(e%50000==0)cout<<e<<" cands done"<<endl;
    
    HDCPi.Fill(xmdm); 
    if(xmctrue){
      HDCPiSignal.Fill(xmdm);
      HDCPiSignalTrueMass.Fill(xdeltam-xdmres+1.8693);
      //HDCPiSignalTrueMass.Fill(xmdm-xdmres);
      HpstarTrue.Fill(xpstar);
    }
    if(!xmctrue){      
      HDCPiBkg.Fill(xmdm);
    }


    for(Int_t i=0;i<10;i++)
      if((-1.+i*.2)<dcostheta&&dcostheta<=(-1.+(i+1)*.2))
	HDCPiVsDCosCoarse[i]->Fill(xmdm);

    for(Int_t i=0;i<20;i++)
      if((3.+i*.1)<xpstar&&xpstar<=(3.+(i+1)*.1))
	HDCPiVspstar[i]->Fill(xmdm);
    
  }


  TFile temp(_OutputDir+"/HistosForFit.root","recreate"); 
  
  ///
  HDCPi.Write();
  HDCPiSignal.Write();
  HDCPiSignalTrueMass.Write();
  HDCPiBkg.Write();
    
  for(Int_t i=0;i<10;i++)    
    HDCPiVsDCosCoarse[i]->Write();


  for(Int_t i=0;i<20;i++)
    HDCPiVspstar[i]->Write();

  HpstarTrue.Write();
  
  temp.ls();
  temp.Close();

  if(CloseReducedNtuple()!=1) return 0;
  return 1;
}


Int_t DCPiAnalysis::MakeEfficiency(){
  TChain ntp1("ntp1");  
  TreeChain(&ntp1,1,5,"DCharged/DChargedPi/MC/SP-9015");
  
  TH1F HMassGen("HMassGen","",XMassBins/2,XMassMin,XMassMax);
  ntp1.Draw("mcmass>>HMassGen","abs(mcLund)==425");

  TH1F HpstarGen("HpstarGen","",25,2.5,5.0);
  ntp1.Draw("mcp3CM>>HpstarGen","abs(mcLund)==425");
  
  TFile temp(_OutputDir+"/MCGenerated.root","recreate");
  HMassGen.Write();
  HpstarGen.Write();
  temp.ls();

  return 1;
}
Int_t DCPiAnalysis::PlotEfficiency(){

 TFile temp(_OutputDir+"/HistosForFit.root","read"); 
 TH1F* HReco=(TH1F*)temp.Get("HMassSignalTrueMass");
 TH1F* HRecoRebin=(TH1F*)HReco->Rebin(2,"Rebin");


 TFile tempgen(_OutputDir+"/MCGenerated.root","read");
 TH1F* HGen=(TH1F*)tempgen.Get("HMassGen");

 filename=_OutputDir+"/Efficiency.ps";
 Canvas.Print(filename+"[");  
 
 HGen->GetXaxis()->SetRangeUser(2.2,2.8);
 HGen->GetXaxis()->SetTitle(XMassLabel);
 Canvas.Clear();
 HGen->Draw();
 Canvas.Print(filename);

 HRecoRebin->GetXaxis()->SetRangeUser(2.2,2.8);
 HRecoRebin->GetXaxis()->SetTitle(XMassLabel);
 Canvas.Clear();
 HRecoRebin->Draw();
 Canvas.Print(filename);

 Canvas.Clear();
 HGen->Draw();
 HRecoRebin->Draw("same");
 Canvas.Print(filename);

 delete HRecoRebin->GetListOfFunctions()->FindObject("stats");
 HRecoRebin->Sumw2();
 HRecoRebin->Divide(HGen);
 HRecoRebin->Fit("pol1");
 HRecoRebin->GetYaxis()->SetTitle("efficiency");
 HRecoRebin->GetYaxis()->SetRangeUser(0,.3);
 Canvas.Clear(); 
 HRecoRebin->Draw();
 Canvas.Print(filename);

 //efficiency vs p*
 TH1F* HpstarTrue=(TH1F*)temp.Get("HpstarTrue");
 TH1F* HpstarGen=(TH1F*)tempgen.Get("HpstarGen");

 Canvas.Clear();
 HpstarGen->Draw();
 HpstarTrue->Draw("same");
 Canvas.Print(filename);

 delete HpstarTrue->GetListOfFunctions()->FindObject("stats");
 HpstarTrue->Sumw2();
 HpstarTrue->Divide(HpstarGen);
 HpstarTrue->Fit("pol1");
 HpstarTrue->GetYaxis()->SetTitle("efficiency");
 HpstarTrue->GetYaxis()->SetRangeUser(0,.5);
 Canvas.Clear(); 
 HpstarTrue->Draw();
 Canvas.Print(filename);


 Canvas.Print(filename+"]");  
 delete HRecoRebin;

 return 1;
}


void DCPiAnalysis::MakeFeedResolution(){
  
  TFile File("DstarCharged/DstarPi/MC/D2420/Kpi/ReducedClean.root");
  gROOT->cd();
  TTree*t=(TTree*)File.Get("CleanNtuple");
  TH1F HResRaw("HResRaw","",200,-.2,-.1);
  t->Draw("massKpipi1-d0mass+1.8645-(dstpimdm-dstpidmres)+.0046>>HResRaw");//last number corrects for pi+ - pi0 mass difference
  Float_t Mean=HResRaw.GetMean();
  cout<<"The mean value is "<< Mean<<endl;
  TH1F HRes("HRes","",200,-.05,.05);
  sprintf(textlabel,"massKpipi1-d0mass+1.8645-(dstpimdm-dstpidmres)+.0046-%.6f>>HRes",Mean);
  t->Draw(textlabel);//4.6MeV corrects for pi+ - pi0 mass difference
  File.Close();

  //Fit Function
  TF1 fg("fg","[0]/((([1]*asin((x-[2])/.1))**2)**[3] + 1.) + [4]*exp(-.5*(x-[5])**2/[6]**2)",-.2,-.1);///(3.14159/2.)
  fg.SetParLimits(0,0,1e5);
  fg.SetParLimits(1,.1,100);
  fg.SetParLimits(2,-.150,-.135);
  fg.SetParLimits(3,1,5);
  fg.SetParLimits(4,0,1e5);
  fg.SetParLimits(5,-.16,-.12);
  fg.SetParLimits(6,.01,.2);
  HResRaw.Fit(&fg);
  cout<<" chisquare="<<fg.GetChisquare()/(HResRaw.GetNbinsX()-fg.GetNumberFreeParameters())<<endl;
  
  TH1F HResFit("HResFit","",200,-.2,-.1);
  for(Int_t i=1;i<=HResFit.GetNbinsX();i++)
    HResFit.SetBinContent(i,fg.Eval(HResFit.GetBinCenter(i)));
  HResFit.Scale(1./HResFit.Integral());

  TFile FRes(_OutputDir+"/FeedResolution.root","recreate");
  cout<<FRes.GetName()<<" has been created"<<endl;
  HResRaw.Write();
  HRes.Write();
  HResFit.Write();
  FRes.Close();

}

Int_t DCPiAnalysis::FitMass(){
  TString name="HMass";

  TFile file(_OutputDir+"/HistosForFit.root","read");
  //file.ls();
  gROOT->cd();

  TH1F*HDCPiMass=(TH1F*)file.Get(name);

  //TH1F*HDCPiMassEff=(TH1F*)file.Get(name+"Eff");
 
  if(!HDCPiMass){cout<<"no histo"<<endl;return 0;}
  SetHistoXYLabels(HDCPiMass,"M(D^{+}#pi^{-})","GeV/c^{2}");
  HDCPiMass->SetStats(0);
  
  TH1F*HDCPiMassSB=NULL;
  TString sidefilename=_OutputDir+"/../KPiPiSB/HistosForFit.root";
  if(_OutputDir.Contains("KPiPiWS"))sidefilename=_OutputDir+"/../KPiPiWSSB/HistosForFit.root";
  TFile fileside(sidefilename,"read");
  HDCPiMassSB=(TH1F*)fileside.Get(name);
  if(HDCPiMassSB && !_OutputDir.Contains("KPiPiSB")){
    cout<<"Integral before sideband subtraction= "<<HDCPiMass->Integral()<<endl;
    HDCPiMass->Sumw2();
    HDCPiMassSB->Sumw2();
    HDCPiMass->Add(HDCPiMassSB,-1);
    cout<<"Integral after sideband subtraction= "<<HDCPiMass->Integral()<<endl;
  }

  if(!massfitter.Initialize(HDCPiMass,_OutputDir+"/InitRooFitPars.txt"))return 0;
  if(massfitter.Fit()<0)return 0;
  massfitter.Save(_OutputDir);
  
  filename=_OutputDir+"/DCPiMassPlots.ps";
  Canvas.Print(filename+"[");  

  Canvas.Clear();
  HDCPiMass->Draw("pe");
  Canvas.Print(filename); 

  RooPlot*fitplot=massfitter.GetFitPlot(); 
  Canvas.Clear();
  fitplot->Draw();
  Canvas.Print(filename);
  delete fitplot; 

//   TGraph*Scan= massfitter.ScanParameter("D2460","mean");
//   if(Scan){
//     Canvas.Clear();
//     Scan->Draw("ap");
//     Canvas.Print(filename);
//   }

 
//   RooPlot*H2Cont= massfitter.GetContour("D2460","mean","D2460","width");
//   if(H2Cont){
//     Canvas.Clear();
//     H2Cont->Draw();
//     Canvas.Print(filename);  
//     delete H2Cont;
//   }

//   TH2F H2scan("H2","",10,2.456,2.462,10,.025,.035);
//   massfitter.ScanParameter2D("D2460","mean","D2460","width",&H2scan);
//   Canvas.Clear();
//   H2scan.Draw("colz");
//   Canvas.Print(filename);


//    TH2F H2scan("H2","",50,-5,15,50,-4,0);
//    massfitter.ScanParameter2D("Bkg1","coef2","Bkg1","coef3",&H2scan);
//    Canvas.Clear();
//    H2scan.Draw("colz");
//    Canvas.Print(filename); 
  
//   TH2F H2scan("H2","",100,0,10,100,-4,0);
//   massfitter.ScanParameter2D("Bkg1","coef2","Bkg1","coef3",&H2scan);
//   Canvas.Clear();
//   H2scan.Draw("colz");
//   Canvas.Print(filename);  


  Canvas.Print(filename+"]");

 
  file.Close();
  return 1;
}


void DCPiAnalysis::PlotRandFits(TString Dir,TString signame,TString parname,Int_t Nfiles){
  filename=_OutputDir+"/"+Dir+".ps";
  Canvas.Print(filename+"["); 
  

  TH1F HChis("HChis","",60,0,3);
  TGraphErrors GChiVsPar;
  TGraphErrors GCovVsPar;

  
  Float_t chi=0.;
  Float_t ndf=0.;
  Float_t chidf=0.;
  Int_t covq=0;
  Int_t ngood=0;
  for(Int_t i=1;i<=Nfiles;i++){
    inputpars.CloseFitPars();
    if(!inputpars.OpenFitPars(_OutputDir+"/"+Dir+"/FitPars_InitRooFitPars_"+(long)i+".txt"))continue;
    if(inputpars.GetFitPar("Range","status")!=0){cout<<" Status="<<inputpars.GetFitPar("Range","status")<<endl;continue;}
    chi=inputpars.GetFitPar("Range","chisquare");
    ndf=inputpars.GetFitPar("Range","ndf");
    chidf=chi/ndf;
    if(chidf>10.){cout<<"Bad Chi/DF="<<chidf<<"  chi="<<chi<<"  ndf="<<ndf<<endl;inputpars.CloseFitPars();continue;}
    covq=(int)inputpars.GetFitPar("Range","covqual");
    
    HChis.Fill(chidf);
    GChiVsPar.SetPoint(ngood,inputpars.GetFitPar(signame,parname),chidf);
    GCovVsPar.SetPoint(ngood,inputpars.GetFitPar(signame,parname),covq);
    ngood++;
 
  }

  Canvas.Clear();
  HChis.Draw("pe");
  Canvas.Print(filename); 

  Canvas.Clear();
  GChiVsPar.GetYaxis()->SetRangeUser(0,3);
  GChiVsPar.GetYaxis()->SetTitle("#chi^{2}/NDF");
  GChiVsPar.GetXaxis()->SetTitle(signame+" "+parname);
  GChiVsPar.Draw("ape");
  Canvas.Print(filename); 

  Canvas.Clear();
  GCovVsPar.GetYaxis()->SetTitle("Covariance Matrix Quality");
  GCovVsPar.GetXaxis()->SetTitle(signame+" "+parname);
  GCovVsPar.Draw("ape");
  Canvas.Print(filename); 


  Canvas.Print(filename+"]"); 
}


Int_t DCPiAnalysis::FitD3000(){
  
  TFile file(_OutputDir+"/FitPars_NRBW100_NRBW100_NRBW100_NRBW100_NRBW100_NRBW100.root","read");
  if(file.IsZombie())return 0;
  file.ls();
  TH1F* H=(TH1F*)file.Get("HResid");
  if(!H)return 0;

  Float_t fitmin=2.85;
  Float_t fitmax=3.25;  
 
  for(Int_t i=1;i<=H->GetNbinsX();i++)
    if(H->GetXaxis()->GetBinCenter(i)<fitmin||H->GetXaxis()->GetBinCenter(i)>fitmax)
      H->SetBinContent(i,0);

  //Rebin
  H=(TH1F*)H->Rebin(2,"HR");


  Float_t ymax=0;
  Float_t ymin=0;
  for(Int_t i=1;i<=H->GetNbinsX();i++){
    if(ymax<H->GetBinContent(i))ymax=H->GetBinContent(i);
    if(ymin>H->GetBinContent(i))ymin=H->GetBinContent(i);
  }

  filename=_OutputDir+"/D3000ResFit.ps";
  Canvas.Print(filename+"[");   


  
//   TF1 FBW("FBW","([0]*[3]/(2*3.14159*[2]))/((x-[1])*(x-[1])+.25*[2]*[2])",fitmin,fitmax);
//   FBW.SetParLimits(0,1,10000);
//   FBW.SetParLimits(1,2.95,3.05);
//   FBW.SetParLimits(2,.005,.2);
//   FBW.FixParameter(3,H->GetBinWidth(1));
//   FBW.SetParNames("Yield","Mass","Width");


  //FitHistogramBreitWigner(H,fitmin,fitmax);
  //H->Fit(&FBW,"","",fitmin,fitmax);

//   Canvas.Clear();
//   H->GetXaxis()->SetRangeUser(fitmin,fitmax);
//   H->GetYaxis()->SetRangeUser(1.1*ymin,1.75*ymax);
//   H->SetStats(1);
//   H->Draw("pe");
//   cutline.DrawLine(fitmin,0,fitmax,0);
//   Canvas.Print(filename); 


  mass.setRange(fitmin,fitmax);
  RooRealVar Mean("Mean","Mean",fitmin,fitmax);Mean.setUnit("GeV");
  RooRealVar Width("Width","Width",.001,.1);Width.setUnit("GeV");
  RooGenericPdfN FBW("FBW","FBW","1./((mass-Mean)**2+.25*Width*Width)",RooArgList(mass,Mean,Width));// + Yield*(mass>5),Yield

  RooRealVar slope("slope","slope",-10000,10000);
  RooRealVar fmin("fmin","fmin",fitmin);
  RooRealVar fmax("fmax","fmax",fitmax);
  RooGenericPdfN FBkg("FBkg","FBkg","(1-.5*slope*(fmax-fmin)*(fmax-fmin))/(fmax-fmin)+slope*(mass-fmin)",RooArgList(mass,slope,fmin,fmax));
  
  RooRealVar Yield("Yield","Yield",0,10);
  RooAddPdfN totpdf("totpdf","totpdf",FBW,FBkg,Yield);

  RooDataHist dataset("dataset","dataset",mass,H);
  //FBW.fitTo(dataset,"mh",RooFit::DataError(RooAbsData::SumW2));

  RooChi2VarIntegral chi2("chi2","chi2",totpdf,dataset,RooFit::DataError(RooAbsData::SumW2));
  chi2.SetIntegralBins(4);
  RooMinuit minuit(chi2);
  minuit.migrad();
  RooFitResult* result=minuit.save();
  result->Print();


  Canvas.Clear();
  RooPlot*pl=mass.frame();
  dataset.plotOn(pl,RooFit::DataError(RooAbsData::SumW2));
  totpdf.plotOn(pl);
  //FBW.paramOn(pl,&dataset,"",1,"NELU",.45,.78,.83);
  //dataset.statOn(pl);
  totpdf.paramOn(pl,Parameters(RooArgSet(Mean,Width)),Layout(.45,.78,.83),Format("NELU",AutoPrecision(1)));
 
  Yield.setRange(0,(int)(H->Integral()*Yield.getVal()));
  Yield.setVal((int)(H->Integral()*Yield.getVal()));
  totpdf.paramOn(pl,Parameters(RooArgSet(Yield)),Layout(.45,.78,.7),Format("N"));
  pl->SetTitle("");
  pl->GetXaxis()->SetRangeUser(fitmin,fitmax);
  pl->GetYaxis()->SetRangeUser(1.1*ymin,1.75*ymax);
  pl->GetXaxis()->SetTitle("m(D#pi) (GeV)");
  pl->Draw();
  cutline.DrawLine(fitmin,0,fitmax,0);
  Canvas.Print(filename); 
  delete pl;

  Canvas.Print(filename+"]"); 
 
  return 1;
}


#define NRESPOINTS 12
#define RESMIN 2.3
#define RESMAX 2.6

Int_t DCPiAnalysis::MakeResolution(){
  if(!_MCorDATA)return 0;
  
  if(OpenReducedNtuple()!=1) return 0;

  TFile file(_OutputDir+"/Resolution.root","recreate");

  TH1F HResTotal("HResTotal","",100,-.05,.05);
  ReducedNtuple->Draw("xdmres>>HResTotal");
  HResTotal.Write(); 

  Float_t mstep=(RESMAX-RESMIN)/NRESPOINTS;

  for(Int_t i=0;i<NRESPOINTS;i++){
    TH1F HRes(TString("HRes")+(long)i,TString("HRes")+(long)i,100,-.05,.05);
    sprintf(textlabel,"%.6f<xmdm-xdmres&&xmdm-xdmres<%.6f",RESMIN+i*mstep,RESMIN+(i+1)*mstep);
    ReducedNtuple->Draw(TString("xdmres>>HRes")+(long)i,textlabel);
    HRes.Write(); 
  }
  

  file.ls();
  file.Close();

  return 1;

}

Int_t DCPiAnalysis::FitResolutionTotal(){
  if(!_MCorDATA)return 0;
  
  TFile file(_OutputDir+"/Resolution.root","read");
  file.ls();

  TH1F* HResTotal=(TH1F*)file.Get("HResTotal"); 
  
  filename=_OutputDir+"/ResolutionTotal.ps";
  Canvas.Print(filename+"["); 
  Canvas.Clear();
  HResTotal->Draw("pe");
  Canvas.Print(filename);
  
  RooDataHist dataset("dataset","dataset",resVar,HResTotal); 

//use the offset as the sigma variable
  Float_t offsetmin=resoffset.getMin();
  Float_t offsetmax=resoffset.getMax();  
  Float_t offsetval=resoffset.getVal();  
  resoffset.setRange(.0001,.02);
  resoffset.setVal(.002);
  resoffset.setConstant(0);
  Float_t slopeval=resslope.getVal();
  resslope.setVal(0);
  mass.setConstant(1);

  resmean.setConstant(0);
  resnormratio2.setConstant(0);
  resmean2.setConstant(0);
  ressigmaratio2.setConstant(0);
  resnormratio3.setConstant(0);
  resmean3.setConstant(0);
  ressigmaratio3.setConstant(0);


  ///Total Fit
  respdf->fitTo(dataset,"m");
  RooPlot* plot=resVar.frame();
  dataset.plotOn(plot);
  respdf->plotOn(plot);
  Canvas.Clear();
  plot->Draw();
  Canvas.Print(filename);
  Canvas.Clear();
  Canvas.SetLogy(1);
  plot->Draw();
  Canvas.Print(filename);
  delete plot;
  
  
  
  Canvas.Print(filename+"]"); 

  resmean.setConstant(1);
  resoffset.setRange(offsetmin,offsetmax);
  resoffset.setVal(offsetval);
  resoffset.setConstant(1);
  resslope.setVal(slopeval);
  resslope.setConstant(1);
  resnormratio2.setConstant(1);
  resmean2.setConstant(1);
  ressigmaratio2.setConstant(1);
  resnormratio3.setConstant(1);
  resmean3.setConstant(1);
  ressigmaratio3.setConstant(1);
  mass.setConstant(0);


  return 1;
}



Int_t DCPiAnalysis::FitResolution(){
  if(!_MCorDATA)return 0;
  
  TFile file(_OutputDir+"/Resolution.root","read");
  file.ls();

  filename=_OutputDir+"/Resolution.ps";
  Canvas.Print(filename+"["); 

  //use the offset as the sigma variable
  Float_t offsetmin=resoffset.getMin();
  Float_t offsetmax=resoffset.getMax();  
  Float_t offsetval=resoffset.getVal();  
  resoffset.setRange(.0001,.02);
  resoffset.setVal(.002);
  resoffset.setConstant(0);
  Float_t slopeval=resslope.getVal();
  resslope.setVal(0);
  mass.setConstant(1);

  //Fit vs mass
  TH1F HSigma("HSigma","",NRESPOINTS,RESMIN,RESMAX);
  HSigma.GetYaxis()->SetTitle("#sigma (MeV)");
  HSigma.GetXaxis()->SetTitle("m(D#pi) (GeV)");
  HSigma.SetStats(0);
  TH1F HFWHM("HFWHM","",NRESPOINTS,RESMIN,RESMAX);
  HFWHM.GetYaxis()->SetTitle("HWHM (MeV)");
  HFWHM.GetXaxis()->SetTitle("m(D#pi) (GeV)");
  HFWHM.SetStats(0);
  Canvas.Clear();
  Int_t nx=2;Int_t ny=4;
  Canvas.Divide(nx,ny);
  Int_t ngood=0; 
  text.SetTextSize(.2);
  RooPlot* plots[10];
  for(Int_t i=0;i<NRESPOINTS;i++){
 
    TH1F* HRes=(TH1F*)file.Get(TString("HRes")+(long)i);
    if(!HRes){cout<<" no histo"<<endl;return 0;}
    if(HRes->Integral()<1500)continue;
    ngood++;
    if(ngood>nx*ny){cout<<" Ran out of Canvas pads"<<endl;continue;}

    Canvas.cd(ngood);
    Canvas.GetPad(ngood)->SetLogy(1);
    Canvas.GetPad(ngood)->SetTopMargin(.09);
    Canvas.GetPad(ngood)->SetBottomMargin(.2);
    Canvas.GetPad(ngood)->SetLeftMargin(.12);
    Canvas.GetPad(ngood)->SetRightMargin(.12);

    RooDataHist dataset("dataset","dataset",resVar,HRes); 
    respdf->fitTo(dataset,"m");
    plots[ngood-1]=resVar.frame();
    dataset.plotOn(plots[ngood-1]);
    respdf->plotOn(plots[ngood-1]);
   
    plots[ngood-1]->SetTitle("");
    plots[ngood-1]->GetXaxis()->SetTitle("m_{rec}-m_{gen}  (GeV)");
    //plots[ngood-1]->GetXaxis()->SetTitle("");
    plots[ngood-1]->GetXaxis()->SetTitleSize(.1);
    plots[ngood-1]->GetYaxis()->SetTitleSize(.1);
    plots[ngood-1]->GetYaxis()->SetTitleOffset(.5);
    plots[ngood-1]->GetXaxis()->SetLabelSize(.1);
    plots[ngood-1]->GetYaxis()->SetLabelSize(.1);
    plots[ngood-1]->GetYaxis()->SetNdivisions(5);
    plots[ngood-1]->GetXaxis()->SetNdivisions(5);

    plots[ngood-1]->Draw();
    text.DrawTextNDC(.2,.7,TString("")+(long)ngood);

    
    Float_t fwhm;
    Float_t mean;
    Float_t xofymax;
    DetermineFWHM(respdf,&resVar,&fwhm,&mean,&xofymax);
    HFWHM.SetBinContent(i,1000*fwhm/2);
    HSigma.SetBinContent(i,1000*resoffset.getVal());
    HSigma.SetBinError(i,1000*resoffset.getError());

   
    cout<<"Done with point "<<ngood<<" histo:"<<HRes->GetName()<<endl;
    
  }
  Canvas.Print(filename);
  Canvas.SetLogy(0);
 //  for(Int_t i=0;i<10;i++)
//     if(plots[i]) delete plots[i];


  file.Close();
  if(ngood<1)return 0;


  Canvas.Clear();
  HSigma.Fit("pol1");
  HSigma.Draw("pe");
  Canvas.Print(filename);  

  Canvas.Clear();
  HFWHM.Draw("p");
  Canvas.Print(filename);    
  
  Canvas.Print(filename+"]"); 

  //return parameters to initial
  resmean.setConstant(1);
  resoffset.setRange(offsetmin,offsetmax);
  resoffset.setVal(offsetval);
  resoffset.setConstant(1);
  resslope.setVal(slopeval);
  resslope.setConstant(1);
  mass.setConstant(0);


  return 1;
}





Int_t DCPiAnalysis::FitVariations(TString id){
  TFile file(_OutputDir+"/HistosForFit.root","read");
  TH1F*HDCPiMass=(TH1F*)file.Get("HMass");
 
  if(!HDCPiMass){cout<<"no histo"<<endl;return 0;}
  SetHistoXYLabels(HDCPiMass,"M(D^{+}#pi^{-})","GeV/c^{2}");
  HDCPiMass->SetStats(0);
  
  TFile fileside(_OutputDir+"/../KPiPiSB/HistosForFit.root","read");
  TH1F*HDCPiMassSB=(TH1F*)fileside.Get("HMass");
  if(!HDCPiMassSB){
    cout<<"Side-band histo not found"<<endl;
  }
  cout<<"Integral before sideband subtraction= "<<HDCPiMass->Integral()<<endl;
  HDCPiMass->Sumw2();
  HDCPiMassSB->Sumw2();
  HDCPiMass->Add(HDCPiMassSB,-1);
  cout<<"Integral after sideband subtraction= "<<HDCPiMass->Integral()<<endl;
  fileside.Close();
 

  for(Int_t f=0;f<NFITVars;f++){
    if(SystDirs[f]!="0"&&(id==SystDirs[f]||(const char*)id=="")){
      if(!massfitter.Initialize(HDCPiMass,_OutputDir+"/FitVariations/"+SystDirs[f]+"/InitRooFitPars.txt")){
	cout<<"Bad init file at "<<SystDirs[f]<<endl;return 0;
      }
      if(massfitter.Fit()<0){
	cout<<"Bad fit "<<SystDirs[f]<<endl; return 0;
      }
      massfitter.SetFileTag("");
      massfitter.Save(_OutputDir+"/FitVariations/"+SystDirs[f]);
    }
  }

  file.Close();
  return 1;
}

Int_t DCPiAnalysis::ShowSystematics(){ 

  Float_t SignalYield[NSig][NSysts+1];
  Float_t SignalMean[NSig][NSysts+1];
  Float_t SignalWidth[NSig][NSysts+1];
  for(Int_t f=0;f<=NSysts;f++){
    for(Int_t s=0;s<NSig;s++){ 
      if(SigName[s]!="0"){
	SignalYield[s][f]=0;
	SignalMean[s][f]=0;
	SignalWidth[s][f]=0;
      }
    }
  }


  for(Int_t f=0;f<NSysts;f++){
    if(SystDirs[f]!="0"){
      inputpars.OpenFitPars(TString(_OutputDir+"/FitVariations/")+SystDirs[f]+"/FitPars.txt");
      for(Int_t s=0;s<NSig;s++){ 
	if(SigName[s]!="0"){
	  SignalYield[s][f]=inputpars.GetFitPar(SigName[s],"yield");
	  SignalMean[s][f]=inputpars.GetFitPar(SigName[s],"mean");
	  SignalWidth[s][f]=inputpars.GetFitPar(SigName[s],"width");
	}
      }
      inputpars.CloseFitPars();
    }
  }

  ///Add the B-field and SVT Systematic to al signal masses 
  for(Int_t s=0;s<NSig;s++){ 
    if(SigName[s]!="0"){
      SignalYield[s][7]=SignalYield[s][0];
      SignalMean[s][7]=SignalMean[s][0]+sqrt(0.00042*0.00042+0.00065*0.00065);//B=0.42 SVT=0.65MeV
      SignalWidth[s][7]=SignalWidth[s][0];
    }
  }
  

  //Compute the total systematic
  for(Int_t s=0;s<NSig;s++){
    if(SigName[s]!="0"){ 
      for(Int_t f=1;f<NSysts;f++){
	if(SystName[f]!="0"){
	  SignalYield[s][NSysts]+=pow(fabs(SignalYield[s][f]-SignalYield[s][0]),2);
	  SignalMean[s][NSysts]+=pow(fabs(SignalMean[s][f]-SignalMean[s][0]),2);
	  SignalWidth[s][NSysts]+=pow(fabs(SignalWidth[s][f]-SignalWidth[s][0]),2);
	}
      }
    }
  }

  




  ///Latex format
  cout<<fixed<<setprecision(2);
  for(Int_t f=1;f<NSysts;f++){
    if(SystName[f]!="0"){
      cout<<"{\\bf "<<SystName[f]<<"}";
      for(Int_t s=0;s<2;s++){
	if(SigName[s]!="0"){ 
	  cout<<" & "<<fabs(SignalYield[s][f]-SignalYield[s][0])/1000;
	  if(SigName[s]!="D2400")cout<<" & "<<fabs(SignalMean[s][f]-SignalMean[s][0])*1000;	   
	  if(SigName[s]!="D2400")cout<<" & "<<fabs(SignalWidth[s][f]-SignalWidth[s][0])*1000;	   
	}
      }
      cout<<" \\\\"<<endl;
    }
  }
  cout<<"\\hline"<<endl;
  cout<<" {\\bf Total}";
  for(Int_t s=0;s<2;s++){
    if(SigName[s]!="0"){ 
      cout<<" & "<<sqrt(SignalYield[s][NSysts])/1000;
      if(SigName[s]!="D2400")cout<<" & "<<sqrt(SignalMean[s][NSysts])*1000;
      if(SigName[s]!="D2400")cout<<" & "<<sqrt(SignalWidth[s][NSysts])*1000;

    }
  }
  cout<<" \\\\"<<endl;
  //Separate the new signals
  for(Int_t f=1;f<NSysts;f++){
    if(SystName[f]!="0"){
      cout<<"{\\bf "<<SystName[f]<<"}";
      for(Int_t s=2;s<4;s++){
	if(SigName[s]!="0"){ 
	  cout<<" & "<<fabs(SignalYield[s][f]-SignalYield[s][0])/1000
	      <<" & "<<fabs(SignalMean[s][f]-SignalMean[s][0])*1000	   
	      <<" & "<<fabs(SignalWidth[s][f]-SignalWidth[s][0])*1000;	   
	}
      }
      cout<<" \\\\"<<endl;
    }
  }
  cout<<"\\hline"<<endl;
  cout<<" {\\bf Total}";
  for(Int_t s=2;s<4;s++){
    if(SigName[s]!="0"){ 
      cout<<" & "<<sqrt(SignalYield[s][NSysts])/1000
	  <<" & "<<sqrt(SignalMean[s][NSysts])*1000
	  <<" & "<<sqrt(SignalWidth[s][NSysts])*1000;
    }
  }
  cout<<" \\\\"<<endl;

  return 1;
}


Int_t DCPiAnalysis::ShowSignificances(){

  Int_t SignalChi2[NSig];
  Int_t SignalNDF[NSig];
  Float_t SignalYield[NSig];
  Float_t SignalYieldEr[NSig];
  Float_t SignalYieldErSyst[NSig];
  for(Int_t f=0;f<NSig;f++){
    SignalChi2[f]=0;
    SignalNDF[f]=0;
    SignalYield[f]=0;
    SignalYieldEr[f]=0;
    SignalYieldErSyst[f]=0;
  }

  //read in the nominal parameters
  inputpars.OpenFitPars(_OutputDir+"/FitVariations/Nominal/FitPars.txt");
  Int_t NominalChi2=(int)inputpars.GetFitPar("Range","chisquare");
  Int_t NominalNDF=(int)inputpars.GetFitPar("Range","ndf");
  Int_t fitstatus=(int)inputpars.GetFitPar("Range","status");
  cout<<" fitstatus="<<fitstatus
      <<" chi2/ndf="<<NominalChi2<<"/"<<NominalNDF
      <<" prob="<<TMath::Prob(NominalChi2,NominalNDF)<<endl;

  for(Int_t s=0;s<NSig;s++){
   if(SigName[s]!="0"){
     SignalYield[s]=inputpars.GetFitPar(SigName[s],"yield");
     SignalYieldEr[s]=inputpars.GetFitPar(SigName[s],"yielderror");
   }
  }
  inputpars.CloseFitPars();


//   //read in the chisquares for the fits where each resonance is removed
//   for(Int_t f=0;f<NSig;f++){
//     if(SigName[f]!=""){
//       inputpars.OpenFitPars(TString(_OutputDir+"/FitVariations/Remove")+SigName[f]+"/FitPars.txt");
//       Int_t fitstatus=(int)inputpars.GetFitPar("Range","status");
//       SignalChi2[f]=(int)inputpars.GetFitPar("Range","chisquare");
//       SignalNDF[f]=(int)inputpars.GetFitPar("Range","ndf");
//       cout<<" fitstatus="<<fitstatus<<endl;
//       inputpars.CloseFitPars();
//     }
//   }


  /////Compute the significance based on the total error of the yield
  for(Int_t s=0;s<NSig;s++)//first square the statistical error so that systematic errors can be added in quadrature
    if(SigName[s]!="0")
      SignalYieldEr[s]=SignalYieldEr[s]*SignalYieldEr[s];

  ///Read in the systematic errors
  Float_t SystSignalYield[NSig][NSysts];
  for(Int_t f=0;f<NSysts;f++){
    for(Int_t s=0;s<NSig;s++) 
      if(SigName[s]!="0")
	SystSignalYield[s][f]=SignalYield[s];//set to nominal 
  }
  for(Int_t f=0;f<NSysts;f++){
    if(SystDirs[f]!="0"){
      inputpars.OpenFitPars(TString(_OutputDir+"/FitVariations/")+SystDirs[f]+"/FitPars.txt");
      for(Int_t s=0;s<NSig;s++)
	if(SigName[s]!="0")
	  SystSignalYield[s][f]=inputpars.GetFitPar(SigName[s],"yield");	  
      inputpars.CloseFitPars();
    }


    ///Add the B-field and SVT Systematic
    ///--->Systematics on yields were not calculated.

  }


  for(Int_t f=1;f<NSysts;f++)
    if(SystName[f]!="0"){
      cout<<SystName[f]<<" ";
      for(Int_t s=0;s<NSig;s++)
	if(SigName[s]!="0"){
	  cout<<fabs(SystSignalYield[s][f]-SystSignalYield[s][0])<<" ";
	  SignalYieldErSyst[s]+=pow(fabs(SystSignalYield[s][f]-SystSignalYield[s][0]),2);//add the systematic errors
	}
      cout<<endl;
    }


  //Latex format
  for(Int_t s=0;s<NSig;s++){
    if(SigName[s]!="0"){
      cout<<fixed<<setprecision(1)<<SigTitle[s]	
	  <<" & "<<SignalYield[s]/1000<<" $\\pm$ "<<sqrt(SignalYieldEr[s])/1000<<"(stat) $\\pm$ "<<sqrt(SignalYieldErSyst[s])/1000<<"(syst)"
	  <<" & "<<SignalYield[s]/sqrt(SignalYieldEr[s]+SignalYieldErSyst[s])
	//<<" & "<<SignalChi2[s]<<"/"<<SignalNDF[s]
	//<<" & "<<TMath::Prob(SignalChi2[s],SignalNDF[s])
	//<<" & "<<sqrt((float)(SignalChi2[s]-NominalChi2))
	  <<" \\\\"<<endl;
    }
  }

  return 1;
}



Int_t DCPiAnalysis::FitMassVsPstar(){
  

  TFile file(_OutputDir+"/HistosForFit.root","read");
  //file.ls();
  gROOT->cd();

  for(Int_t i=0;i<20;i++){
    TH1F*HDCPiMass=(TH1F*)file.Get(TString("HMass_pstar")+(long)i);
    if(!HDCPiMass){cout<<"no histo"<<endl;return 0;}
    
    SetHistoXYLabels(HDCPiMass,"M(D^{+}#pi^{-})","GeV/c^{2}");
    HDCPiMass->SetStats(0);

    MassFitter Fitter(&mass);
    Fitter.SetResolution(respdf,&resVar);
    Fitter.SetPhaseSpace(&TwoBodyPS);
    Fitter.SetDauMasses(&dau1m,&dau2m);
    if(!Fitter.Initialize(HDCPiMass,_OutputDir+"/PstarFits/InitRooFitPars.txt"))return 0;
    if(Fitter.Fit()<0)return 0;
    Fitter.SetFileTag(TString("_")+(long)i);
    Fitter.Save(_OutputDir+"/PstarFits");
  } 
  
  file.Close();
 
  return 1;
}
