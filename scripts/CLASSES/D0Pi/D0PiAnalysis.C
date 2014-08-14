#include "D0PiAnalysis.h"
#include "RooGaussian.h"
#include "../../MACROS/TreeChain.C"


ClassImp(D0PiAnalysis);
D0PiAnalysis::D0PiAnalysis(Bool_t MCorDATA,TString OutputDir):
  D0Analysis(MCorDATA,OutputDir),
  cutcharge(0),
  cutpstar(0),
  cutdmass(0),
  cutdcos(0),
  massfitter(&mass),
  dau1m("dau1m","dau1m",D0PDGMass),
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
  resVar("resVar","resVar",-.04,.04),
  effoffset("effoffset","effoffset",0.266),
  effslope("effslope","effslope",0.044)
{

  ///-------
  mass.setUnit("GeV/c^{2}");


  //-----------------------
  //The resolution functions
  //-----------------------   
  resmean.setVal(5.17889e-05);resmean.setConstant(1);
  resoffset.setVal(-1.007667e-02);resoffset.setConstant(1);
  resslope.setVal(5.307493e-03);resslope.setConstant(1);

  resnormratio2.setVal(5.88106e-02);resnormratio2.setConstant(1);
  resmean2.setVal(-7.68831e-03);resmean2.setConstant(1);
  ressigmaratio2.setVal(1.39336e+00);ressigmaratio2.setConstant(1);
  
  resnormratio3.setVal(2.87224e-02);resnormratio3.setConstant(1);  
  resmean3.setVal(7.25816e-03);resmean3.setConstant(1);
  ressigmaratio3.setVal(8.95818e-01);ressigmaratio3.setConstant(1);
 
//    mass.setVal(2.460);
//    respdf=new RooResolutionPdf("respdf","respdf",&resVar,mass,resoffset,resslope,resmean,resnormratio2,resmean2,ressigmaratio2,resnormratio3,resmean3,ressigmaratio3);  
    
  resmean.setVal(1.03881e-04);resmean.setConstant(1);
  resoffset.setVal(.0037);resoffset.setConstant(1);
  resVar.setRange(-.015,.015);
  resVar.setBins(20);//sigma/2 steps
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

  ///////////
  massfitter.SetResolution(respdf,&resVar);
  massfitter.SetPhaseSpace(&TwoBodyPS);
  massfitter.SetEps(.5);
  massfitter.SetDauMasses(&dau1m,&dau2m);

  effpdf=new RooGenericPdf("effpdf","effoffset + effslope*mass",RooArgList(mass,effoffset,effslope));
  massfitter.SetEfficiencyPdf(effpdf);
 

  ////ccbar scaling factor
  Int_t Nccbar=1088218000;
  Float_t Crossccbar=1.30;//nanobarns
  Float_t lumiccbar=Nccbar/Crossccbar;
  Float_t lumidata=470000000;
  scalefactorccbar=(float)(lumidata/lumiccbar);
  cout<<" ccbar lumi scale factor "<<scalefactorccbar<<endl;




  ///Define cuts on the D0 mass
  DMassCutWindow=2.5*.0076;// 7.6 sigma of D0 mass peak ---> update
  DMassCutCenter=1.8645;// = mean of 1 Gauss fit  ---> update
  DMassCutSBMin=4*.0076;


  //
    ///
  SystDirs[0]="Nominal";
  SystDirs[1]="SystMassRange";
  SystDirs[2]="SystBinWidth";
  SystDirs[3]="SystBWShape";
  SystDirs[4]="SystBkgModel";
  SystDirs[5]="SystD2460Width";
  SystDirs[6]="SystD2620Width";
  SystDirs[7]="SystD2750Width"; 
  SystDirs[8]="SystFeedDownWidth";
  SystDirs[9]="";
  SystDirs[10]="SystFeedD2430";
  SystDirs[11]="";
  SystDirs[12]="";
  SystDirs[13]="";
  SystDirs[14]="";


  SystName[0]="";
  SystName[1]="Mass Range";
  SystName[2]="Bin Width";
  SystName[3]="Breit-Wigner Shape";
  SystName[4]="Background Model";
  SystName[5]="D(2460) Width";
  SystName[6]="D(2620) Width";
  SystName[7]="D(2750) Width";
  SystName[8]="D(2420) Width";
  SystName[9]="B field \\& SVT material";
  SystName[10]="D(2430)";
  SystName[11]="";
  SystName[12]="";
  SystName[13]="";
  SystName[14]="";

  SigName[0]="D2400";
  SigName[1]="D2460";
  SigName[2]="D2620";
  SigName[3]="D2750";
  SigName[4]="";
  SigTitle[0]="\\DTwentyFourHundred";
  SigTitle[1]="\\DTwentyFourSixty";
  SigTitle[2]="\\DTwentySixHundred";
  SigTitle[3]="\\DTwentySevenFifty";
  SigTitle[4]="";

  cout<<"Done Initializing the analysis"<<endl;
}

D0PiAnalysis::~D0PiAnalysis(){  
}

Int_t D0PiAnalysis::ReduceNtuple(){
  cout<<"Going to reduce the ntuple"<<endl;

  if(_fChain==NULL){
    cout<<"No chain."<<endl;
    return 0;
  } 

  ///link the branches in the input chain
  SetD0PiBranches();

  //the file must be created here so the 'new' histos and Tree is created in hard drive //OutFile=new 
  TFile OutFile(_OutputDir+"/Ntuple_New.root","recreate",_OutputDir+"/Ntuple_New.root",1);//1=minimal compression
  if(OutFile.IsZombie()){cout<<"Failed to create file"<<endl;return 0;}


  //create new reduced ntuple
  TTree Ntuple("Ntuple","Ntuple",0);
  Ntuple.SetMaxTreeSize(900000000);//split files

  //disable autosaving 
  Ntuple.SetAutoSave(900000000);

  //create the branches in the Ntuple
  MakeD0PiReducedNtupleBranches(&Ntuple);



  cout<<" MCorDATA "<<_MCorDATA<<endl;

  /////
  cout<<"Will use the following cuts"<<endl;
  cout<<" cutcharge "<<cutcharge
      <<" cutpstar "<<cutpstar
      <<" cutdmass "<<cutdmass
      <<" cutdcos "<<cutdcos<<endl;

  //Counters
  Int_t MCXCounterTotal=0;
  Int_t RecoXCounterTotal=0;
  

  cout<<"Total Entries in Chain="<<_fChain->GetEntries()<<endl;
  cout<<"Going to start the event loop"<<endl;
  eventid=0;
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

    xmultiplicityid=0;
    //now do real loop
    for(XIdx=0;XIdx<nX;XIdx++){
          
      //
      FillD0PiVars();     
    
      if(dpstar<2.4)continue;//skim cut
      if(dparent>0)continue;//remove D*'s
      if(cutcharge==1)if(abs(xcharge)!=1) continue;//right sign
      if(cutcharge==2)if(abs(xcharge)!=2) continue;//select wrong sign
      if(cutpstar==1)if(xpstar<3.0) continue;
      if(cutdmass==1)if(fabs(dmass-DMassCutCenter)>DMassCutWindow) continue;
      if(cutdmass==2)
	if(fabs(dmass-DMassCutCenter)<DMassCutSBMin||DMassCutSBMin+DMassCutWindow<fabs(dmass-DMassCutCenter))continue;
      if(cutdcos==1)if(dcostheta<-.8) continue;

      xmultiplicityid++;
      RecoXCounterTotal++;
      Ntuple.Fill();
   
    }//X loop    
      
 

  }

  //print summary
  cout<<"--------Summary-------"<<endl;
  cout<<"Total events = "<<eventid<<endl;
  cout<<"Total reco ="<<RecoXCounterTotal<<endl;
  cout<<"Total mc ="<<MCXCounterTotal<<endl;
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

void D0PiAnalysis::FillD0PiVars(){
  ///determine the indexes for each particle before used by other methods
  DIdx=Xd1Idx[XIdx];
  Pi1Idx=Xd2Idx[XIdx];      
  
  //
  FillD0Vars();

  //////fill reduced ntuple variables    
  xmass=XMass[XIdx];
  xpstar=Xp3CM[XIdx];
  xpstarfrac=xpstar*2*beamenergy/(beamenergy*beamenergy-XMass[XIdx]*XMass[XIdx]);
  xcosstar=XcosthCM[XIdx];
  xphistar=XphiCM[XIdx];
  xdeltam=XMass[XIdx]-DMass[DIdx];  
  xmdm=xdeltam+D0PDGMass;
  xenergy=sqrt(XMass[XIdx]*XMass[XIdx]+Xp3CM[XIdx]*Xp3CM[XIdx]);
  xenergyfrac=xenergy*2*beamenergy/(beamenergy*beamenergy+XMass[XIdx]*XMass[XIdx]);
  if(TMath::Prob(XChi2[XIdx],XnDof[XIdx])>1e-300)
    xlogvtxprob=log10(TMath::Prob(XChi2[XIdx],XnDof[XIdx]));
  else xlogvtxprob=-100;
  //Determine D0pi combination: xcharge= -2=D0pi- , -1=D0pi+  , 1=D0barpi- , 2=D0barpi+
  xcharge=DLund[DIdx]/abs(DLund[DIdx])-PiLund[Pi1Idx]/abs(PiLund[Pi1Idx]);
  if(xcharge==0)xcharge=DLund[DIdx]/abs(DLund[DIdx]);   


  //extra pion
  pi1p3=Pip3[Pi1Idx];      
  pi1charge=PiLund[Pi1Idx]/abs(PiLund[Pi1Idx]);
  pi1mothcosine=ComputePiMotherAngle();
  pi1costheta=ComputePiAngle();
  pi1Kmass=ComputexyMass(Pip3[Pi1Idx],Picosth[Pi1Idx],Piphi[Pi1Idx],PiPDGMass,
			 Kp3[KIdx],Kcosth[KIdx],Kphi[KIdx],KPDGMass);
			 
  pi1pimass=ComputexyMass(Pip3[Pi1Idx],Picosth[Pi1Idx],Piphi[Pi1Idx],PiPDGMass,
			  Pip3[PiIdx],Picosth[PiIdx],Piphi[PiIdx],PiPDGMass);

  //-------D quantities
  dcostheta=ComputeDAngle();
  dparent=XDstParent[XIdx];
  //override some vars
  dlogvtxprob=0;//redefined because I do not fit D vertex

 

  xlund=0;
  xmctrue=0;
  xdmres=1;
  //truth match
  //find the generated D**
  //get the genearted tracks
  //get the true tracks corresponding to the reco tracks
  //compare the two sets 
  if(_MCorDATA==0){
    Int_t TrueCandIdx[3]={PiMCIdx[Pi1Idx],
			  KMCIdx[KIdx],
			  PiMCIdx[PiIdx]};
    for(Int_t m=1;m<mcLen;m++)
      if(abs(mcLund[m])==myD2420ChargedLund || abs(mcLund[m])==myD2460ChargedLund || abs(mcLund[m])==myD2400ChargedLund || abs(mcLund[m])==myD2430ChargedLund){		
	Int_t nfound=0;
	for(Int_t c=0;c<3;c++){
	  Int_t ParentIdx=mothIdx[TrueCandIdx[c]];
	  while(ParentIdx>2){
	    if(ParentIdx==m)nfound++;
	    ParentIdx=mothIdx[ParentIdx];
	  }
	}
	if(nfound==3){
	  xmctrue=1;
	  xlund=mcLund[m];
	  xdmres=xdeltam-(mcmass[m]-mcmass[dauIdx[m]]);    
	}
      }

  } 

      
}



Float_t D0PiAnalysis::ComputeDAngle(){
  //Angle of the D in the X frame
  TVector3 Xp3vec(Xp3CM[XIdx]*sin(acos(XcosthCM[XIdx]))*cos(XphiCM[XIdx]),
                  Xp3CM[XIdx]*sin(acos(XcosthCM[XIdx]))*sin(XphiCM[XIdx]),
                  Xp3CM[XIdx]*XcosthCM[XIdx]);
  
  Float_t XEnergy=sqrt(Xp3vec*Xp3vec + XMass[XIdx]*XMass[XIdx]);
  TVector3 XVelvec=(1/XEnergy)*Xp3vec;
  
  TVector3 Pi1p3vec(Pip3CM[Pi1Idx]*sin(acos(PicosthCM[Pi1Idx]))*cos(PiphiCM[Pi1Idx]),
		    Pip3CM[Pi1Idx]*sin(acos(PicosthCM[Pi1Idx]))*sin(PiphiCM[Pi1Idx]),
		    Pip3CM[Pi1Idx]*PicosthCM[Pi1Idx]);
  
  TLorentzVector Pi1p4(Pi1p3vec,sqrt(Pi1p3vec*Pi1p3vec+PiPDGMass*PiPDGMass));
  
  //boost the dstar momentum into rest frame of X
  Pi1p4.Boost(-XVelvec);//watch the minus sign
  TVector3 Pi1Boostedp3vec=Pi1p4.Vect();
  
  //dot product of boosted dstar direction with X direction
  return Pi1Boostedp3vec.Unit()*Xp3vec.Unit();
  
}


Float_t D0PiAnalysis::ComputeMCDAngle(){
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
          

Float_t D0PiAnalysis::ComputePiAngle(){
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

Float_t D0PiAnalysis::ComputePiMotherAngle(){//cone angle in CM frame
  TVector3 Xp3vec(Xp3CM[XIdx]*sin(acos(XcosthCM[XIdx]))*cos(XphiCM[XIdx]),
                  Xp3CM[XIdx]*sin(acos(XcosthCM[XIdx]))*sin(XphiCM[XIdx]),
                  Xp3CM[XIdx]*XcosthCM[XIdx]);
  
  TVector3 Pip3vec(Pip3CM[Pi1Idx]*sin(acos(PicosthCM[Pi1Idx]))*cos(PiphiCM[Pi1Idx]),
                   Pip3CM[Pi1Idx]*sin(acos(PicosthCM[Pi1Idx]))*sin(PiphiCM[Pi1Idx]),
                   Pip3CM[Pi1Idx]*PicosthCM[Pi1Idx]);
  
  
  return Xp3vec.Unit()*Pip3vec.Unit();
  
}




Int_t D0PiAnalysis::SetD0PiBranches(){
  if(_fChain==NULL){cout<<"No Chain"<<endl;return 0;} 

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
  _fChain->SetBranchAddress("XDstParent",XDstParent);

  if(_MCorDATA==0){
    _fChain->SetBranchAddress("XMCIdx",XMCIdx);
    _fChain->SetBranchAddress("XMyMCIdx",XMyMCIdx);
  } 

  SetD0Branches();
  cout<<"Done linking D0Pi  Chain Branches"<<endl;
  return 1;
}



Int_t D0PiAnalysis::MakeD0PiReducedNtupleBranches(TTree* Ntuple){
 
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
  Ntuple->Branch("xbestcand",&xbestcand,"xbestcand/I");
  Ntuple->Branch("xmctrue",&xmctrue,"xmctrue/I"); 
  Ntuple->Branch("xlund",&xlund,"xlund/I"); 
  Ntuple->Branch("xvtxstatus",&xvtxstatus,"xvtxstatus/I");
  Ntuple->Branch("xmultiplicityid",&xmultiplicityid,"xmultiplicityid/I");
  Ntuple->Branch("xmcidx",&xmcidx,"xmcidx/I");

  Ntuple->Branch("dcostheta",&dcostheta,"dcostheta/F");
  Ntuple->Branch("dparent",&dparent,"dparent/I");
  Ntuple->Branch("kcosine",&kcosine,"kcosine/F");
   
  Ntuple->Branch("pi1p3",&pi1p3,"pi1p3/F");
  Ntuple->Branch("pi1charge",&pi1charge,"pi1charge/I");
  Ntuple->Branch("pi1dedx",&pi1dedx,"pi1dedx/F");
  Ntuple->Branch("pi1thetac",&pi1thetac,"pi1thetac/F");
  Ntuple->Branch("pi1costheta",&pi1costheta,"pi1costheta/F");
  Ntuple->Branch("pi1mothcosine",&pi1mothcosine,"pi1mothcosine/F");
  Ntuple->Branch("pi1Kmass",&pi1Kmass,"pi1Kmass/F");
  Ntuple->Branch("pi1pimass",&pi1pimass,"pi1pimass/F");


  cout<<"Done Making  D0Pi  Reduced Ntuple Branches"<<endl;
  MakeD0ReducedNtupleBranches(Ntuple);
  
  return 1;
}



Int_t D0PiAnalysis::SetD0PiReducedNtupleBranches(){


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
  ReducedNtuple->SetBranchAddress("xbestcand",&xbestcand);
  ReducedNtuple->SetBranchAddress("xmctrue",&xmctrue);
  ReducedNtuple->SetBranchAddress("xlund",&xlund);
  ReducedNtuple->SetBranchAddress("xvtxstatus",&xvtxstatus);
	       
  ReducedNtuple->SetBranchAddress("dcostheta",&dcostheta);
  ReducedNtuple->SetBranchAddress("kcosine",&kcosine);

  ReducedNtuple->SetBranchAddress("pi1p3",&pi1p3);
  ReducedNtuple->SetBranchAddress("pi1charge",&pi1charge);
  ReducedNtuple->SetBranchAddress("pi1dedx",&pi1dedx);
  ReducedNtuple->SetBranchAddress("pi1thetac",&pi1thetac);
  ReducedNtuple->SetBranchAddress("pi1costheta",&pi1costheta);
  ReducedNtuple->SetBranchAddress("pi1mothcosine",&pi1mothcosine);


  SetD0ReducedNtupleBranches();
  cout<<"Done linking D0Pi ReducedNtuple Branches"<<endl;
  return 1;
}


Int_t D0PiAnalysis::DefaultPlots(){
    
  if(OpenReducedNtuple()!=1) return 0;

  filename=_OutputDir+"/DefaultPlots.ps";
  Canvas.Print(filename+"["); 

  TH1F HD0Mass("HD0Mass","",120,1.8,1.92);
  HD0Mass.GetYaxis()->SetTitle("Entries/1 MeV");
  HD0Mass.GetXaxis()->SetTitle("m(K^{-}#pi^{+})   (GeV/c^{2})");
  ReducedNtuple->Draw("dmass>>HD0Mass");
  Canvas.Clear();
  HD0Mass.Draw("pe");
  if(_MCorDATA==0){
    ReducedNtuple->Draw("dmass","dmctrue==0","same");
  }
  Canvas.Print(filename);

  TH1F HD0PiMass("HD0PiMass","",240,2.0,3.2);
  ReducedNtuple->Draw("xmdm>>HD0PiMass");
  HD0PiMass.GetYaxis()->SetTitle("Entries/5 MeV");
  HD0PiMass.GetXaxis()->SetTitle("M(D^{0}#pi^{+})   (GeV/c^{2})");
  HD0PiMass.GetYaxis()->SetRangeUser(0,HD0PiMass.GetMaximum()*1.1);
  Canvas.Clear();
  HD0PiMass.Draw("pe");
  if(_MCorDATA==0){
    ReducedNtuple->Draw("xmdm","xmctrue==0","same");
  }
  Canvas.Print(filename);
 

  Canvas.Print(filename+"]");

  if(CloseReducedNtuple()!=1) return 0;
  return 1;
}


Int_t D0PiAnalysis::MakeHistosForFit(){

  if(OpenReducedNtuple()!=1) return 0;  
  SetD0PiReducedNtupleBranches();
  ///-------------------------------------------
  cout<<"Create root file with histos for fit."<<endl;
  ///-------------------------------------------


  TH1F HDMass("HDMass","",110,1.81,1.92);
  TH1F HDMassBkg("HDMassBkg","",110,1.81,1.92);
  TH1F HMass("HMass","",XMassBins,XMassMin,XMassMax);
  TH1F HMassSignal("HMassSignal","",XMassBins,XMassMin,XMassMax);
  TH1F HMassSignalTrueMass("HMassSignalTrueMass","",XMassBins,XMassMin,XMassMax);
  TH1F HMassBkg("HMassBkg","",XMassBins,XMassMin,XMassMax);
  TH1F HMassLoSide("HMassLoSide","",XMassBins,XMassMin,XMassMax);
  TH1F HMassHiSide("HMassHiSide","",XMassBins,XMassMin,XMassMax);
  TH1F HMassSide("HMassSide","",XMassBins,XMassMin,XMassMax);



  ///
  Int_t e=0;
  while(ReducedNtuple->GetEntry(e)){
    e++;
    if(e%50000==0)cout<<e<<" cands done"<<endl;
    
    HDMass.Fill(dmass);
    if(!dmctrue)HDMassBkg.Fill(dmass);

    HMass.Fill(xmdm); 
    if(xmctrue){
      HMassSignal.Fill(xmdm);
      HMassSignalTrueMass.Fill(xdeltam-xdmres+D0PDGMass);
    }
    if(!xmctrue){      
      HMassBkg.Fill(xmdm);
    }

    if(DMassCutCenter-(DMassCutSBMin+DMassCutWindow) < dmass && dmass < DMassCutCenter-DMassCutSBMin) HMassLoSide.Fill(xmdm);
    if(DMassCutCenter+DMassCutSBMin < dmass && dmass < DMassCutCenter+(DMassCutSBMin+DMassCutWindow) ) HMassHiSide.Fill(xmdm);
    if(DMassCutSBMin < fabs(dmass-DMassCutCenter) 
       && fabs(dmass-DMassCutCenter) < (DMassCutSBMin+DMassCutWindow) ) HMassSide.Fill(xmdm);

    
  }


  TFile temp(_OutputDir+"/HistosForFit.root","recreate"); 
  
  ///
  HDMass.Write();
  HDMassBkg.Write();
  HMass.Write();
  HMassSignal.Write();
  HMassSignalTrueMass.Write();
  HMassBkg.Write();
  HMassLoSide.Write();
  HMassHiSide.Write();
  HMassSide.Write();
  
  temp.ls();
  temp.Close();

  if(CloseReducedNtuple()!=1) return 0;
  return 1;
}



Int_t D0PiAnalysis::MakeEfficiency(){
  TChain ntp1("ntp1");  
  TreeChain(&ntp1,1,4,"Dzero/DzeroPi/Signal/Ntuples/Off");
  TreeChain(&ntp1,1,35,"Dzero/DzeroPi/Signal/Ntuples/On");
  
  TH1F HMassGen("HMassGen","",XMassBins/4,XMassMin,XMassMax);
  ntp1.Draw("mcmass>>HMassGen","abs(mcLund)==415");
  
  TFile temp(_OutputDir+"/MCGenerated.root","recreate");
  HMassGen.Write();
  temp.ls();

  return 1;
}


Int_t D0PiAnalysis::PlotEfficiency(){

 TFile temp(_OutputDir+"/HistosForFit.root","read"); 
 TH1F* HReco=(TH1F*)temp.Get("HMassSignalTrueMass");
 TH1F* HRecoRebin=(TH1F*)HReco->Rebin(4,"Rebin");


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
 HRecoRebin->SetStats(0);
 HRecoRebin->Fit("pol1");
 HRecoRebin->GetYaxis()->SetTitle("efficiency");
 HRecoRebin->GetYaxis()->SetRangeUser(0,.3);
 Canvas.Clear(); 
 HRecoRebin->Draw();
 Canvas.Print(filename);

 Canvas.Print(filename+"]");  
 delete HRecoRebin;

 return 1;
}

#define NRESPOINTS 12
#define RESMIN 2.3
#define RESMAX 2.6

Int_t D0PiAnalysis::MakeResolution(){
  if(_MCorDATA!=0)return 0;
  
  if(OpenReducedNtuple()!=1) return 0;

  TH1F HResTotal("HResTotal","",100,-.05,.05);
  ReducedNtuple->Draw("xdmres>>HResTotal");

  TFile file(_OutputDir+"/Resolution.root","recreate");

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

Int_t D0PiAnalysis::FitResolutionTotal(){
  if(_MCorDATA!=0)return 0;
  
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



Int_t D0PiAnalysis::FitResolution(){
  if(_MCorDATA!=0)return 0;
  
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
  Int_t nx=2;Int_t ny=5;
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
    HFWHM.SetBinContent(i+1,1000*fwhm/2);
    HSigma.SetBinContent(i+1,1000*resoffset.getVal());
    HSigma.SetBinError(i+1,1000*resoffset.getError());

   
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



// Int_t D0PiAnalysis::FitMass(){
//   TString name="";
  
//   TFile file(_OutputDir+"/HistosForFit.root","read");
//   gROOT->cd();

//   TH1F*HD0PiMass=(TH1F*)file.Get(name);
//   TFile fileSB(_OutputDir+"/../KpiSB/HistosForFit.root","read");
//   TH1F*HD0PiMassSB=(TH1F*)file.Get(name);
//   if(!HD0PiMassSB){cout<<"Side-band histo not found"<<endl;return 0;}
//   HD0PiMass->Sumw2();
//   if(HD0PiMassSB)HD0PiMass->Add(HD0PiMassSB,-1);
 
//   if(!HD0PiMass){cout<<"no histo"<<endl;return 0;}
//   SetHistoXYLabels(HD0PiMass,"m(D#pi)","GeV/c^{2}");
//   HD0PiMass->SetStats(0);
  
//   //massfitter.SetEfficiency(HD0PiMassEff); 
//   if(!massfitter.Initialize(HD0PiMass,_OutputDir+"/InitRooFitPars.txt"))return 0;
//   if(massfitter.Fit()<0)return 0;
//   massfitter.Save(_OutputDir);
  
//   filename=_OutputDir+"/D0PiMassPlots.ps";
//   Canvas.Print(filename+"[");  

//   Canvas.Clear();
//   HD0PiMass->Draw("pe");
//   Canvas.Print(filename); 

//   RooPlot*fitplot=massfitter.GetFitPlot(); 
//   Canvas.Clear();
//   fitplot->Draw();
//   Canvas.Print(filename);
//   delete fitplot; 


//   Canvas.Print(filename+"]");

 
//   file.Close();
//   return 1;
// }


Int_t D0PiAnalysis::PlotD0PiMass(){
  TFile file(_OutputDir+"/HistosForFit.root","read");
  gROOT->cd();

  
  filename=_OutputDir+"/PlotD0PiMass.ps";
  Canvas.Print(filename+"[");  
 
  TH1F*HD0PiMass=(TH1F*)file.Get("HMass");   
  if(!HD0PiMass){cout<<"Histo not found"<<endl; return 0;}
  HD0PiMass->GetYaxis()->SetTitle("Entries / 5 MeV");
  HD0PiMass->GetXaxis()->SetTitle(XMassLabel);
  //HD0PiMass->SetMaximum(1000);
  Canvas.Clear();
  HD0PiMass->Draw("pe");
  //plot the truth-matched background
  TH1F*HD0PiMassBkg=(TH1F*)file.Get("HMassBkg");   
  if(HD0PiMassBkg){
    HD0PiMassBkg->Draw("hist same");
  }
  Canvas.Print(filename); 


  //plot each side-band
  TH1F*HD0PiMassLo=(TH1F*)file.Get("HMassLoSide");   
  TH1F*HD0PiMassHi=(TH1F*)file.Get("HMassHiSide");   
  if(HD0PiMassLo&&HD0PiMassHi){
    HD0PiMassLo->GetYaxis()->SetTitle("Entries / 5 MeV");
    HD0PiMassLo->GetXaxis()->SetTitle(XMassLabel);
    HD0PiMassHi->GetYaxis()->SetTitle("Entries / 5 MeV");
    HD0PiMassHi->GetXaxis()->SetTitle(XMassLabel);
    Canvas.Clear();
    HD0PiMassLo->Draw("hist p");
    Canvas.Print(filename); 
    Canvas.Clear();
    HD0PiMassHi->Draw("hist p");
    Canvas.Print(filename); 

    Canvas.Clear();
    HD0PiMass->Draw("pe");
    HD0PiMassLo->Draw("hist same");
    HD0PiMassHi->Draw("hist same");
    Canvas.Print(filename); 

    Canvas.Clear();
    HD0PiMass->GetXaxis()->SetRangeUser(2.04,3.24);
    HD0PiMass->Draw("pe");
    HD0PiMassLo->Draw("hist same");
    HD0PiMassHi->Draw("hist same");
    Canvas.Print(filename); 

  }
  Canvas.Print(filename+"]");

  file.Close();
  return 1;

}

Int_t D0PiAnalysis::FitMassAntimo(){
  TFile file(_OutputDir+"/HistosForFit.root","read");
  TH1F*HD0PiMass=(TH1F*)file.Get("HMass");
  if(!HD0PiMass){cout<<"Histo not found"<<endl; return 0;}
  HD0PiMass->Sumw2();

  TFile filesb(_OutputDir+"SB/HistosForFit.root","read");
  TH1F*HD0PiMassSB=(TH1F*)filesb.Get("HMass");
  if(!HD0PiMassSB)cout<<"Side-band histo not found"<<endl;
  else {
    cout<<"Integral before side-band subtraction ="<<HD0PiMass->Integral()<<endl;
    HD0PiMass->Add(HD0PiMassSB,-1);
    cout<<"Integral after side-band subtraction ="<<HD0PiMass->Integral()<<endl;
  }
 
  SetHistoXYLabels(HD0PiMass,"M(D^{0}#pi^{+})","GeV/c^{2}");
  HD0PiMass->SetStats(0);
  
  if(!massfitter.Initialize(HD0PiMass,_OutputDir+"/InitRooFitPars.txt"))return 0;
  if(massfitter.Fit()<0)return 0;
  massfitter.Save(_OutputDir);
  
//   filename=_OutputDir+"/D0PiMassPlots.ps";
//   Canvas.Print(filename+"[");  

//   Canvas.Clear();
//   HD0PiMass->Draw("pe");
//   Canvas.Print(filename); 

//   RooPlot*fitplot=massfitter.GetFitPlot(); 
//   Canvas.Clear();
//   fitplot->Draw();
//   Canvas.Print(filename);
//   delete fitplot; 


//   Canvas.Print(filename+"]");

 
  file.Close();
  return 1;
}


Int_t D0PiAnalysis::FitVariations(TString id){
  TFile file(_OutputDir+"/HistosForFit.root","read");
  TH1F*HD0PiMass=(TH1F*)file.Get("HMass"); 
  if(!HD0PiMass){cout<<"no histo"<<endl;return 0;}
  SetHistoXYLabels(HD0PiMass,"M(D^{0}#pi^{+})","GeV/c^{2}");
  HD0PiMass->SetStats(0);
  
  TFile fileside(_OutputDir+"/../KpiSB/HistosForFit.root","read");
  TH1F*HD0PiMassSB=(TH1F*)fileside.Get("HMass");
  if(!HD0PiMassSB){
    cout<<"Side-band histo not found"<<endl;
  }
  cout<<"Integral before sideband subtraction= "<<HD0PiMass->Integral()<<endl;
  HD0PiMass->Sumw2();
  HD0PiMassSB->Sumw2();
  HD0PiMass->Add(HD0PiMassSB,-1);
  cout<<"Integral after sideband subtraction= "<<HD0PiMass->Integral()<<endl;
  fileside.Close();
  
 
  TString FitVarDirs[NFITVars];  
  for(Int_t f=0;f<NFITVars;f++){
    FitVarDirs[f]="";
  }
  for(Int_t f=0;f<NSysts;f++){
    FitVarDirs[f]=SystDirs[f];
  }
  FitVarDirs[NSysts]="RemoveD2400";
  FitVarDirs[NSysts+1]="RemoveD2620";
  FitVarDirs[NSysts+2]="RemoveD2750";


  for(Int_t f=0;f<NFITVars;f++){
    if(FitVarDirs[f]!=""&&(id==FitVarDirs[f]||(const char*)id=="")){
      //if(FitVarDirs[f]!=""){
      if(!massfitter.Initialize(HD0PiMass,_OutputDir+"/FitVariations/"+FitVarDirs[f]+"/InitRooFitPars.txt")){
	cout<<"Bad init file at "<<FitVarDirs[f]<<endl;return 0;
      }
      if(massfitter.Fit()<0){
	cout<<"Bad fit "<<FitVarDirs[f]<<endl; return 0;
      }
      massfitter.SetFileTag("");
      massfitter.Save(_OutputDir+"/FitVariations/"+FitVarDirs[f]);
    }
  }

  file.Close();
  return 1;
}

Int_t D0PiAnalysis::ShowSystematics(){ 

  Float_t SignalYield[NSig][NSysts+1];
  Float_t SignalMean[NSig][NSysts+1];
  Float_t SignalWidth[NSig][NSysts+1];
  for(Int_t f=0;f<=NSysts;f++){
    for(Int_t s=0;s<NSig;s++){ 
      if(SigName[s]!=""){
	SignalYield[s][f]=0;
	SignalMean[s][f]=0;
	SignalWidth[s][f]=0;
      }
    }
  }


  for(Int_t f=0;f<NSysts;f++){
    if(SystDirs[f]!=""){
      inputpars.OpenFitPars(TString(_OutputDir+"/FitVariations/")+SystDirs[f]+"/FitPars.txt");
      for(Int_t s=0;s<NSig;s++){ 
	if(SigName[s]!=""){
	  SignalYield[s][f]=inputpars.GetFitPar(SigName[s],"yield");
	  SignalMean[s][f]=inputpars.GetFitPar(SigName[s],"mean");
	  SignalWidth[s][f]=inputpars.GetFitPar(SigName[s],"width");
	}
      }
      inputpars.CloseFitPars();
    }
  }

  ///Add the B-field and SVT Systematic
  for(Int_t s=0;s<NSig;s++){ 
    if(SigName[s]!=""){
      SignalYield[s][9]=SignalYield[s][0];
      SignalMean[s][9]=SignalMean[s][0]+sqrt(0.00042*0.00042+0.00065*0.00065);//B=0.42 SVT=0.65MeV
      SignalWidth[s][9]=SignalWidth[s][0];
    }
  }
  




  //Compute the total systematic
  for(Int_t s=0;s<NSig;s++){
    if(SigName[s]!=""){ 
      for(Int_t f=1;f<NSysts;f++){
	if(SystName[f]!=""){
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
    if(SystName[f]!=""){
      cout<<"{\\bf "<<SystName[f]<<"}";
      for(Int_t s=0;s<2;s++){
	if(SigName[s]!=""){ 
	  cout<<" & "<<setprecision(1)<<fabs(SignalYield[s][f]-SignalYield[s][0])/1000;
	  if(SigName[s]!="D2400")cout<<" & "<<setprecision(2)<<fabs(SignalMean[s][f]-SignalMean[s][0])*1000;	   
	  //cout<<" & "<<fabs(SignalWidth[s][f]-SignalWidth[s][0])*1000;	   
	}
      }
      cout<<" \\\\"<<endl;
    }
  }
  cout<<"\\hline"<<endl;
  cout<<" {\\bf Total}";
  for(Int_t s=0;s<2;s++){
    if(SigName[s]!=""){ 
      cout<<" & "<<setprecision(1)<<sqrt(SignalYield[s][NSysts])/1000;
      if(SigName[s]!="D2400")cout<<" & "<<setprecision(2)<<sqrt(SignalMean[s][NSysts])*1000;
      //cout<<" & "<<sqrt(SignalWidth[s][NSysts])*1000;

    }
  }
  cout<<" \\\\"<<endl;
  //Separate the new signals
  for(Int_t f=1;f<NSysts;f++){
    if(SystName[f]!=""){
      cout<<"{\\bf "<<SystName[f]<<"}";
      for(Int_t s=2;s<NSig;s++){
	if(SigName[s]!=""){ 
	  cout<<" & "<<setprecision(1)<<fabs(SignalYield[s][f]-SignalYield[s][0])/1000;
	  cout<<" & "<<setprecision(2)<<fabs(SignalMean[s][f]-SignalMean[s][0])*1000;
	  //cout<<" & "<<fabs(SignalWidth[s][f]-SignalWidth[s][0])*1000;	   
	}
      }
      cout<<" \\\\"<<endl;
    }
  }
  cout<<"\\hline"<<endl;
  cout<<" {\\bf Total}";
  for(Int_t s=2;s<NSig;s++){
    if(SigName[s]!=""){ 
      cout<<" & "<<setprecision(1)<<sqrt(SignalYield[s][NSysts])/1000;
      cout<<" & "<<setprecision(2)<<sqrt(SignalMean[s][NSysts])*1000;
      //cout <<" & "<<sqrt(SignalWidth[s][NSysts])*1000;
    }
  }
  cout<<" \\\\"<<endl;

  return 1;
}


Int_t D0PiAnalysis::ShowSignificances(){

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
  inputpars.OpenFitPars(_OutputDir+"/FitVariations/Nominal/FitPars_true.txt");
  Int_t NominalChi2=(int)inputpars.GetFitPar("Range","chisquare");
  Int_t NominalNDF=(int)inputpars.GetFitPar("Range","ndf");
  Int_t fitstatus=(int)inputpars.GetFitPar("Range","status");
  cout<<" fitstatus="<<fitstatus
      <<" chi2/ndf="<<NominalChi2<<"/"<<NominalNDF
      <<" prob="<<TMath::Prob(NominalChi2,NominalNDF)<<endl;

  for(Int_t s=0;s<NSig;s++){
   if(SigName[s]!=""){
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
    if(SigName[s]!="")
      SignalYieldEr[s]=SignalYieldEr[s]*SignalYieldEr[s];

  ///Read in the systematic errors
  Float_t SystSignalYield[NSig][NSysts];
  for(Int_t f=0;f<NSysts;f++){
    for(Int_t s=0;s<NSig;s++) 
      if(SigName[s]!="")
	SystSignalYield[s][f]=SignalYield[s];
  }
  for(Int_t f=0;f<NSysts;f++){
    if(SystDirs[f]!=""){
      inputpars.OpenFitPars(TString(_OutputDir+"/FitVariations/")+SystDirs[f]+"/FitPars.txt");
      for(Int_t s=0;s<NSig;s++)
	if(SigName[s]!="")
	  SystSignalYield[s][f]=inputpars.GetFitPar(SigName[s],"yield");	  
      inputpars.CloseFitPars();
    }
  }
  for(Int_t f=1;f<NSysts;f++)
    if(SystDirs[f]!="")
      for(Int_t s=0;s<NSig;s++)
	if(SigName[s]!="")//add the systematic errors
	  SignalYieldErSyst[s]+=pow(fabs(SystSignalYield[s][f]-SystSignalYield[s][0]),2);


  //Latex format
  for(Int_t s=0;s<NSig;s++){
    if(SigName[s]!=""){
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







// Int_t D0PiAnalysis::PlotRawD0PiMassRatio(){
//   if(_TruthMatch)return 0;   

//   filename=_OutputDir+"/RawD0PiMassRatioPlots.ps";
//   Canvas.Print(filename+"["); 

//   TFile file(_OutputDir+"/RawMassPlots.root","read");
//   gROOT->cd();
 

//   ///
//   Int_t nbins=100;
//   Float_t min=2.0;
//   Float_t max=3.0;

//   //right sign
//   TH1F*HD0PiMass=(TH1F*)file.Get("HD0PiMass");
//   if(!(HD0PiMass=(TH1F*)RebinHistoX(HD0PiMass,nbins,min,max)))return 0; 
//   SetHistoXYLabels(HD0PiMass,"m(D#pi)","GeV/c^{2}");
//   HD0PiMass->GetYaxis()->SetRangeUser(0,HD0PiMass->GetMaximum()*1.1);

//   ///
//   Canvas.Clear();
//   HD0PiMass->Draw("");
//   Canvas.Print(filename);
 

//   //DATA
//   TFile fileDATA("D0harged/D0hargedPi/KPiPiWS/RawMassPlots.root","read");
//   if(fileDATA.IsZombie())return 0;
//   gROOT->cd();
//   TH1F*HD0PiMassDATA=(TH1F*)fileDATA.Get("HD0PiMass");
//   if(!(HD0PiMassDATA=(TH1F*)RebinHistoX(HD0PiMassDATA,nbins,min,max)))return 0;
//   HD0PiMassDATA->SetMarkerColor(2);
//   HD0PiMassDATA->SetLineColor(2);
    
//   ///
//   Canvas.Clear();
//   HD0PiMass->Draw("pe");
//   if(_MCorDATA==0)HD0PiMassDATA->Scale(1./scalefactorccbar);
//   HD0PiMassDATA->Draw("pesame");
//   Canvas.Print(filename);

//   //scale to first 10 bins
//   Canvas.Clear();
//   HD0PiMassDATA->Scale(HD0PiMass->Integral(1,10)/HD0PiMassDATA->Integral(1,10));
//   HD0PiMassDATA->Draw("pe");
//   HD0PiMass->Draw("pesame");  
//   Canvas.Print(filename);

//   HD0PiMass->Sumw2();
//   HD0PiMass->Divide(HD0PiMassDATA);
//   Canvas.Clear();
//   HD0PiMass->GetYaxis()->SetRangeUser(0,1.2);
//   HD0PiMass->GetYaxis()->SetTitle("ratio");
//   HD0PiMass->SetStats(0);
//   HD0PiMass->Draw("pe");  
//   Canvas.Print(filename);
//   Canvas.Clear();
//   HD0PiMass->Fit(FWSRatio);
//   HD0PiMass->Draw("pe");  
//   Canvas.Print(filename);
//   for(Int_t i=0;i<FWSRatio->GetNpar();i++)    
//     cout<<FWSRatio->GetParameter(i)<<",";
//   cout<<endl;

//   Canvas.Print(filename+"]");


//   file.Close();
//   fileDATA.Close();
//   return 1;
// }




// Int_t D0PiAnalysis::MakeFinalStateMasses(){
//   if(_TruthMatch)return 0;

//   if(OpenReducedNtuple()!=1) return 0;

//   TFile file(_OutputDir+"/FinalStateMassPlots.root","recreate");

//   TH1F HKPiMass("HKPiMass","",3000,0,3);
//   ReducedNtuple->Draw("Kpimass>>HKPiMass");
//   HKPiMass.Write(); 

//   TH1F HKPi2Mass("HKPi2Mass","",3000,0,3);
//   ReducedNtuple->Draw("Kpi2mass>>HKPi2Mass");
//   HKPi2Mass.Write();

//   TH1F HPi1KMass("HPi1KMass","",3000,0,3);
//   ReducedNtuple->Draw("pi1Kmass>>HPi1KMass");
//   HPi1KMass.Write();

//   TH1F HPi1KPiMass("HPi1KPiMass","",3000,0,3);
//   ReducedNtuple->Draw("pi1Kpimass>>HPi1KPiMass");
//   HPi1KPiMass.Write();

//   TH1F HPi1KPi2Mass("HPi1KPi2Mass","",3000,0,3);
//   ReducedNtuple->Draw("pi1Kpi2mass>>HPi1KPi2Mass");
//   HPi1KPi2Mass.Write();

//   TH1F HPi1PiMass("HPi1PiMass","",3000,0,3);
//   ReducedNtuple->Draw("pi1pimass>>HPi1PiMass");
//   HPi1PiMass.Write();

//   TH1F HPi1Pi2Mass("HPi1Pi2Mass","",3000,0,3);
//   ReducedNtuple->Draw("pi1pi2mass>>HPi1Pi2Mass");
//   HPi1Pi2Mass.Write();

 
//   if(CloseReducedNtuple()!=1) return 0;
   
 
//   file.Close();

//   return 1;
// }

// Int_t D0PiAnalysis::PlotFinalStateMasses(){
//   if(_TruthMatch)return 0;
  
 
//   TFile file(_OutputDir+"/FinalStateMassPlots.root","read");
//   gROOT->cd();
  
//   TString name[10];
//   name[0]="HKPiMass";
//   name[1]="HKPi2Mass";
//   name[2]="HPi1KMass";
//   name[3]="HPi1KPiMass";
//   name[4]="HPi1KPi2Mass";
//   name[5]="HPi1PiMass";
//   name[6]="HPi1Pi2Mass";
//   name[7]="";
//   name[8]="";
//   name[9]="";

//   filename=_OutputDir+"/FinalStateMassPlots.ps";
//   Canvas.Print(filename+"["); 

//   TH1F*Histo=NULL;
//   Int_t id=0;
//   while((Histo=(TH1F*)file.Get(name[id]))&&id<9){
//     if(!(Histo=(TH1F*)RebinHistoX(Histo,300,0,3)))return 0;
//     SetHistoXYLabels(Histo,name[id],"GeV/c^{2}");
//     Canvas.Clear();
//     Histo->Draw("pe");
//     Canvas.Print(filename);
//     delete Histo; 
//     id++;
//   }

//   Canvas.Print(filename+"]");

//   file.Close();
//   return 1;
// }


// void D0PiAnalysis::MakeFeedResolution(){
  
//   TFile File("DstarCharged/DstarPi/MC/D2420/Kpi/ReducedClean.root");
//   gROOT->cd();
//   TTree*t=(TTree*)File.Get("CleanNtuple");
//   TH1F HResRaw("HResRaw","",200,-.2,-.1);
//   t->Draw("massKpipi1-d0mass+1.8645-(dstpimdm-dstpidmres)+.0046>>HResRaw");//last number corrects for pi+ - pi0 mass difference
//   Float_t Mean=HResRaw.GetMean();
//   cout<<"The mean value is "<< Mean<<endl;
//   TH1F HRes("HRes","",200,-.05,.05);
//   sprintf(textlabel,"massKpipi1-d0mass+1.8645-(dstpimdm-dstpidmres)+.0046-%.6f>>HRes",Mean);
//   t->Draw(textlabel);//4.6MeV corrects for pi+ - pi0 mass difference
//   File.Close();

//   //Fit Function
//   TF1 fg("fg","[0]/((([1]*asin((x-[2])/.1))**2)**[3] + 1.) + [4]*exp(-.5*(x-[5])**2/[6]**2)",-.2,-.1);///(3.14159/2.)
//   fg.SetParLimits(0,0,1e5);
//   fg.SetParLimits(1,.1,100);
//   fg.SetParLimits(2,-.150,-.135);
//   fg.SetParLimits(3,1,5);
//   fg.SetParLimits(4,0,1e5);
//   fg.SetParLimits(5,-.16,-.12);
//   fg.SetParLimits(6,.01,.2);
//   HResRaw.Fit(&fg);
//   cout<<" chisquare="<<fg.GetChisquare()/(HResRaw.GetNbinsX()-fg.GetNumberFreeParameters())<<endl;
  
//   TH1F HResFit("HResFit","",200,-.2,-.1);
//   for(Int_t i=1;i<=HResFit.GetNbinsX();i++)
//     HResFit.SetBinContent(i,fg.Eval(HResFit.GetBinCenter(i)));
//   HResFit.Scale(1./HResFit.Integral());

//   TFile FRes(_OutputDir+"/FeedResolution.root","recreate");
//   cout<<FRes.GetName()<<" has been created"<<endl;
//   HResRaw.Write();
//   HRes.Write();
//   HResFit.Write();
//   FRes.Close();

// }


// void D0PiAnalysis::PlotRandFits(TString Dir,TString signame,TString parname,Int_t Nfiles){
//   filename=_OutputDir+"/"+Dir+".ps";
//   Canvas.Print(filename+"["); 
  

//   TH1F HChis("HChis","",60,0,3);
//   TGraphErrors GChiVsPar;
//   TGraphErrors GCovVsPar;

  
//   Float_t chi=0.;
//   Float_t ndf=0.;
//   Float_t chidf=0.;
//   Int_t covq=0;
//   Int_t ngood=0;
//   for(Int_t i=1;i<=Nfiles;i++){
//     inputpars.CloseFitPars();
//     if(!inputpars.OpenFitPars(_OutputDir+"/"+Dir+"/FitPars_InitRooFitPars_"+(long)i+".txt"))continue;
//     if(inputpars.GetFitPar("Range","status")!=0){cout<<" Status="<<inputpars.GetFitPar("Range","status")<<endl;continue;}
//     chi=inputpars.GetFitPar("Range","chisquare");
//     ndf=inputpars.GetFitPar("Range","ndf");
//     chidf=chi/ndf;
//     if(chidf>10.){cout<<"Bad Chi/DF="<<chidf<<"  chi="<<chi<<"  ndf="<<ndf<<endl;inputpars.CloseFitPars();continue;}
//     covq=(int)inputpars.GetFitPar("Range","covqual");
    
//     HChis.Fill(chidf);
//     GChiVsPar.SetPoint(ngood,inputpars.GetFitPar(signame,parname),chidf);
//     GCovVsPar.SetPoint(ngood,inputpars.GetFitPar(signame,parname),covq);
//     ngood++;
 
//   }

//   Canvas.Clear();
//   HChis.Draw("pe");
//   Canvas.Print(filename); 

//   Canvas.Clear();
//   GChiVsPar.GetYaxis()->SetRangeUser(0,3);
//   GChiVsPar.GetYaxis()->SetTitle("#chi^{2}/NDF");
//   GChiVsPar.GetXaxis()->SetTitle(signame+" "+parname);
//   GChiVsPar.Draw("ape");
//   Canvas.Print(filename); 

//   Canvas.Clear();
//   GCovVsPar.GetYaxis()->SetTitle("Covariance Matrix Quality");
//   GCovVsPar.GetXaxis()->SetTitle(signame+" "+parname);
//   GCovVsPar.Draw("ape");
//   Canvas.Print(filename); 


//   Canvas.Print(filename+"]"); 
// }


// Int_t D0PiAnalysis::FitD3000(){
  
//   TFile file(_OutputDir+"/FitPars_NRBW100_NRBW100_NRBW100_NRBW100_NRBW100_NRBW100.root","read");
//   if(file.IsZombie())return 0;
//   file.ls();
//   TH1F* H=(TH1F*)file.Get("HResid");
//   if(!H)return 0;

//   Float_t fitmin=2.85;
//   Float_t fitmax=3.25;  
 
//   for(Int_t i=1;i<=H->GetNbinsX();i++)
//     if(H->GetXaxis()->GetBinCenter(i)<fitmin||H->GetXaxis()->GetBinCenter(i)>fitmax)
//       H->SetBinContent(i,0);

//   //Rebin
//   H=(TH1F*)H->Rebin(2,"HR");


//   Float_t ymax=0;
//   Float_t ymin=0;
//   for(Int_t i=1;i<=H->GetNbinsX();i++){
//     if(ymax<H->GetBinContent(i))ymax=H->GetBinContent(i);
//     if(ymin>H->GetBinContent(i))ymin=H->GetBinContent(i);
//   }

//   filename=_OutputDir+"/D3000ResFit.ps";
//   Canvas.Print(filename+"[");   


  
// //   TF1 FBW("FBW","([0]*[3]/(2*3.14159*[2]))/((x-[1])*(x-[1])+.25*[2]*[2])",fitmin,fitmax);
// //   FBW.SetParLimits(0,1,10000);
// //   FBW.SetParLimits(1,2.95,3.05);
// //   FBW.SetParLimits(2,.005,.2);
// //   FBW.FixParameter(3,H->GetBinWidth(1));
// //   FBW.SetParNames("Yield","Mass","Width");


//   //FitHistogramBreitWigner(H,fitmin,fitmax);
//   //H->Fit(&FBW,"","",fitmin,fitmax);

// //   Canvas.Clear();
// //   H->GetXaxis()->SetRangeUser(fitmin,fitmax);
// //   H->GetYaxis()->SetRangeUser(1.1*ymin,1.75*ymax);
// //   H->SetStats(1);
// //   H->Draw("pe");
// //   cutline.DrawLine(fitmin,0,fitmax,0);
// //   Canvas.Print(filename); 


//   mass.setRange(fitmin,fitmax);
//   RooRealVar Mean("Mean","Mean",fitmin,fitmax);Mean.setUnit("GeV");
//   RooRealVar Width("Width","Width",.001,.1);Width.setUnit("GeV");
//   RooGenericPdfN FBW("FBW","FBW","1./((mass-Mean)**2+.25*Width*Width)",RooArgList(mass,Mean,Width));// + Yield*(mass>5),Yield

//   RooRealVar slope("slope","slope",-10000,10000);
//   RooRealVar fmin("fmin","fmin",fitmin);
//   RooRealVar fmax("fmax","fmax",fitmax);
//   RooGenericPdfN FBkg("FBkg","FBkg","(1-.5*slope*(fmax-fmin)*(fmax-fmin))/(fmax-fmin)+slope*(mass-fmin)",RooArgList(mass,slope,fmin,fmax));
  
//   RooRealVar Yield("Yield","Yield",0,10);
//   RooAddPdfN totpdf("totpdf","totpdf",FBW,FBkg,Yield);

//   RooDataHist dataset("dataset","dataset",mass,H);
//   //FBW.fitTo(dataset,"mh",RooFit::DataError(RooAbsData::SumW2));

//   RooChi2VarIntegral chi2("chi2","chi2",totpdf,dataset,RooFit::DataError(RooAbsData::SumW2));
//   chi2.SetIntegralBins(4);
//   RooMinuit minuit(chi2);
//   minuit.migrad();
//   RooFitResult* result=minuit.save();
//   result->Print();


//   Canvas.Clear();
//   RooPlot*pl=mass.frame();
//   dataset.plotOn(pl,RooFit::DataError(RooAbsData::SumW2));
//   totpdf.plotOn(pl);
//   //FBW.paramOn(pl,&dataset,"",1,"NELU",.45,.78,.83);
//   //dataset.statOn(pl);
//   totpdf.paramOn(pl,Parameters(RooArgSet(Mean,Width)),Layout(.45,.78,.83),Format("NELU",AutoPrecision(1)));
 
//   Yield.setRange(0,(int)(H->Integral()*Yield.getVal()));
//   Yield.setVal((int)(H->Integral()*Yield.getVal()));
//   totpdf.paramOn(pl,Parameters(RooArgSet(Yield)),Layout(.45,.78,.7),Format("N"));
//   pl->SetTitle("");
//   pl->GetXaxis()->SetRangeUser(fitmin,fitmax);
//   pl->GetYaxis()->SetRangeUser(1.1*ymin,1.75*ymax);
//   pl->GetXaxis()->SetTitle("m(D#pi) (GeV)");
//   pl->Draw();
//   cutline.DrawLine(fitmin,0,fitmax,0);
//   Canvas.Print(filename); 
//   delete pl;

//   Canvas.Print(filename+"]"); 
 
//   return 1;
// }

