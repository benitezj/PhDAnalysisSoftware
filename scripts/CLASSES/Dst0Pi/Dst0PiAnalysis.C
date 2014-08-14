#include "Dst0PiAnalysis.h"

// /////////
#include "Dst0PiAnalysisPlotDst0PiMass.C"
// #include "Dst0PiAnalysisPlotDMVsPstarCosHel.C"

ClassImp(Dst0PiAnalysis);
Dst0PiAnalysis::Dst0PiAnalysis(Bool_t MCorDATA,Bool_t TruthMatch,Int_t MatterOrAntiMatter,TString OutputDir):
  Dst0Analysis(MCorDATA,TruthMatch,MatterOrAntiMatter,OutputDir),
  massfitter(&mass),
  dau1m("dau1m","dau1m",Dst0PDGMass),
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
  resVar("resVar","resVar",-.08,.08)
{
  if(_fChain==NULL){cout<<"No Chain"<<endl;} 
   


   ///-------
  mass.setUnit("GeV/c^{2}");


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
  respdf=new RooResolutionPdf("respdf","respdf",&resVar,mass,resoffset,resslope,resmean,resnormratio2,resmean2,ressigmaratio2,resnormratio3,resmean3,ressigmaratio3);  
    
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
  massfitter.SetEps(.001);



  //the phase space and q factor for the threshold
  phasefactor="((1/(2*(dstpideltam+2.0067)**2))*sqrt(((dstpideltam+2.0067)*(dstpideltam+2.0067) -  4.60403)*((dstpideltam+2.0067)*(dstpideltam+2.0067)- 3.4883)))";
  qfactor="((1/(2*(dstpideltam+2.0067)))*sqrt(((dstpideltam+2.0067)*(dstpideltam+2.0067) -  4.60403)*((dstpideltam+2.0067)*(dstpideltam+2.0067)- 3.4883)))";
  
  ///labels
  Dst0PiXLabel="m(D*^{0}#pi^{+})  -  m(D*^{0})   (GeV/c^{2}) ";
  Dst0PiYLabel="Entries/5 MeV"; 

  cout<<"Done Initializing the analysis"<<endl;
}

Dst0PiAnalysis::~Dst0PiAnalysis(){  
  
}


Int_t Dst0PiAnalysis::ReduceNtuple(){
  cout<<"Going to reduce the ntuple"<<endl;

  if(_fChain==NULL){
    cout<<"No chain."<<endl;
    return 0;
  } 


  ///link the branches in the input chain
  SetDst0PiBranches();

  //the file must be created here
  TFile File(_OutputDir+"/Ntuple.root","recreate",_OutputDir+"/Ntuple.root",1);
  if(File.IsZombie()){cout<<"Failed to create file"<<endl;return 0;}

  //create new reduced ntuple
  TTree Ntuple("Ntuple","Reduced Ntuple",99);        
  Long64_t maxsize=10000000000;//10Gb
  Ntuple.SetMaxTreeSize(maxsize);//split files

  //disable autosaving 
  Long64_t fautosave=20000000000;//20Gb !!
  cout<<" autosave "<<fautosave<<endl;
  Ntuple.SetAutoSave(fautosave);

  //create the branches in the ReducedNtuple
  MakeDst0PiReducedNtupleBranches(&Ntuple);

  Int_t RecoXCounterTotal=0;
  cout<<"Going to start the event loop"<<endl;
  Int_t eventid=0;
  while(_fChain->GetEntry(eventid,0)>0){   
    eventid++;
     
    if(eventid%50000==0)cout<<eventid<<" Events done.   With "<<RecoXCounterTotal<<" Candidates saved."<<endl;          
    eventnumber=eventid;     
    if(nX>MAXNCANDS)continue;

    FillEventVars();

    for(XIdx=0;XIdx<nX;XIdx++){
 
      FillDst0PiVars();

      //apply some simple cuts to minimize file size
      if(dstdeltam>.180) continue;     

      Ntuple.Fill();	              
               
      RecoXCounterTotal++;
           
   
    }//X loop    
      

  }

  //print summary
  cout<<"--------Summary-------"<<endl;
  cout<<"Total events="<<eventid<<endl;
  cout<<"--------End Summary---"<<endl;

  ////Save the Ntuple and histograms
  cout<<"Going to save the ntuple and histograms"<<endl; 

  ///////////Should not have to call TTree->Write(), TFile->Write() will doit
  if(Ntuple.Write()){ cout<<"Ntuple written."<<endl;}
  else{ cout<<"Failed to write ntuple"<<endl;return 0;}
  
  File.ls();//turns out this is quite important for the objects to be written.
  File.Close(); //this will delete all objects created inside the file if they were not written
  cout<<File.GetName()<<" has been closed."<<endl;

  return 1;
}


void Dst0PiAnalysis::FillDst0PiVars(){
  ///determine the indexes for each particle
  Dst0Idx=Xd1Idx[XIdx];
  Pi1Idx=Xd2Idx[XIdx];   
  Pi1trkIdx=PiTrkIdx[Pi1Idx];       
                  
  //////fill reduced ntuple variables    
  dstpimass=XMass[XIdx];      
  dstpipstar=Xp3CM[XIdx];
  dstpicosstar=XcosthCM[XIdx];
  dstpideltam=XMass[XIdx]-Dst0Mass[Dst0Idx];  
  if(TMath::Prob(XChi2[XIdx],XnDof[XIdx])>0)
    dstpilogvtxprob=log10(TMath::Prob(XChi2[XIdx],XnDof[XIdx]));
  else dstpilogvtxprob=-100;
  //Determine D*pi combination: dstpicharge= -2=D*-pi- , -1=D*-pi+  , 1=D*+pi- , 2=D*+pi+
  dstpicharge=Dst0Lund[Dst0Idx]/abs(Dst0Lund[Dst0Idx]);
  if(dstpicharge==0)dstpicharge=Dst0Lund[Dst0Idx]/abs(Dst0Lund[Dst0Idx]);
  dstpinX=nX;  
  dstpiIdx=XIdx;
  dstarcostheta=ComputeDst0Angle();    
  dstpimctrue=(XMCIdx[XIdx]>0);

  //---Pi1
  pi1p3=Pip3[Pi1Idx];
  pi1charge=PiLund[Pi1Idx]/abs(PiLund[Pi1Idx]);
  pi1costheta=ComputePiAngle();
  pi1helicity=ComputePiHelicityAngle();

  FillDst0Vars();
}


Float_t Dst0PiAnalysis::ComputeDst0Angle(){
  TVector3 Xp3vec(Xp3CM[XIdx]*sin(acos(XcosthCM[XIdx]))*cos(XphiCM[XIdx]),
		      Xp3CM[XIdx]*sin(acos(XcosthCM[XIdx]))*sin(XphiCM[XIdx]),
		      Xp3CM[XIdx]*XcosthCM[XIdx]);
  
  Float_t XEnergy=sqrt(Xp3vec*Xp3vec + XMass[XIdx]*XMass[XIdx]);
  TVector3 XVelvec=(1/XEnergy)*Xp3vec;

  TVector3 Dst0p3vec(Dst0p3CM[Dst0Idx]*sin(acos(Dst0costhCM[Dst0Idx]))*cos(Dst0phiCM[Dst0Idx]),
		      Dst0p3CM[Dst0Idx]*sin(acos(Dst0costhCM[Dst0Idx]))*sin(Dst0phiCM[Dst0Idx]),
		      Dst0p3CM[Dst0Idx]*Dst0costhCM[Dst0Idx]); 
  
  TLorentzVector Dst0p4(Dst0p3vec,sqrt(Dst0p3vec*Dst0p3vec+Dst0Mass[Dst0Idx]*Dst0Mass[Dst0Idx]));

  //boost the dstar momentum into rest frame of X
  Dst0p4.Boost(-XVelvec);//watch the minus sign
  TVector3 Dst0Boostedp3vec=Dst0p4.Vect();

  //dot product of boosted dstar direction with X direction in CM frame
  Float_t cosine=Dst0Boostedp3vec.Unit()*XVelvec.Unit();
  
  return cosine;
  
}

Float_t Dst0PiAnalysis::ComputePiAngle(){
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


Float_t Dst0PiAnalysis::ComputePiHelicityAngle(){
  TVector3 Dst0p3vec(Dst0p3CM[Dst0Idx]*sin(acos(Dst0costhCM[Dst0Idx]))*cos(Dst0phiCM[Dst0Idx]),
		  Dst0p3CM[Dst0Idx]*sin(acos(Dst0costhCM[Dst0Idx]))*sin(Dst0phiCM[Dst0Idx]),
		  Dst0p3CM[Dst0Idx]*Dst0costhCM[Dst0Idx]);
  
  Float_t Dst0Energy=sqrt(Dst0p3vec*Dst0p3vec + Dst0Mass[Dst0Idx]*Dst0Mass[Dst0Idx]);
  TVector3 Dst0Velvec=(1/Dst0Energy)*Dst0p3vec;

  TVector3 Pip3vec(Pip3CM[Pi1Idx]*sin(acos(PicosthCM[Pi1Idx]))*cos(PiphiCM[Pi1Idx]),
		    Pip3CM[Pi1Idx]*sin(acos(PicosthCM[Pi1Idx]))*sin(PiphiCM[Pi1Idx]),
		    Pip3CM[Pi1Idx]*PicosthCM[Pi1Idx]); 
  
  TLorentzVector Pip4(Pip3vec,sqrt(Pip3vec*Pip3vec+PiPDGMass*PiPDGMass));
  
  //boost the dstar momentum into rest frame of Dst0
  Pip4.Boost(-Dst0Velvec);//watch the minus sign
  TVector3 PiBoostedp3vec=Pip4.Vect();

  //dot product of boosted dstar direction with Dst0 direction 
  return PiBoostedp3vec.Unit()*Dst0p3vec.Unit();
  
}



Float_t Dst0PiAnalysis::ComputePiDst0Angle(){
  
  TVector3 Dst0p3vec(Dst0p3CM[Dst0Idx]*sin(acos(Dst0costhCM[Dst0Idx]))*cos(Dst0phiCM[Dst0Idx]),
		      Dst0p3CM[Dst0Idx]*sin(acos(Dst0costhCM[Dst0Idx]))*sin(Dst0phiCM[Dst0Idx]),
		      Dst0p3CM[Dst0Idx]*Dst0costhCM[Dst0Idx]); 

  TVector3 Pip3vec(Pip3CM[Pi1Idx]*sin(acos(PicosthCM[Pi1Idx]))*cos(PiphiCM[Pi1Idx]),
		   Pip3CM[Pi1Idx]*sin(acos(PicosthCM[Pi1Idx]))*sin(PiphiCM[Pi1Idx]),
		   Pip3CM[Pi1Idx]*PicosthCM[Pi1Idx]); 
  
  return Dst0p3vec.Unit()*Pip3vec.Unit();
  
}




Int_t Dst0PiAnalysis::SetDst0PiBranches(){
  if(_fChain==NULL){cout<<"No Chain"<<endl;return 0;} 
  cout<<"Setting Chain Branch Addresses"<<endl;

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
 
  _fChain->SetBranchAddress("XMCIdx",XMCIdx);


  cout<<"Done linking Chain Branches"<<endl;
  return 1;
}



Int_t Dst0PiAnalysis::MakeDst0PiReducedNtupleBranches(TTree*Ntuple){
  cout<<"Creating Reduced Ntuple Branches"<<endl;

  Ntuple->Branch("eventnumber",&eventnumber,"eventnumber/I");

  Ntuple->Branch("dstpimass",&dstpimass,"dstpimass/F");
  Ntuple->Branch("dstpideltam",&dstpideltam,"dstpideltam/F");
  Ntuple->Branch("dstpipstar",&dstpipstar,"dstpipstar/F");
  Ntuple->Branch("dstpicosstar",&dstpicosstar,"dstpicosstar/F");
  Ntuple->Branch("dstpicharge",&dstpicharge,"dstpicharge/I");
  Ntuple->Branch("dstpinX",&dstpinX,"dstpinX/I");
  Ntuple->Branch("dstpilogvtxprob",&dstpilogvtxprob,"dstpilogvtxprob/F");
  Ntuple->Branch("dstpiIdx",&dstpiIdx,"dstpiIdx/I");
  Ntuple->Branch("dstpimctrue",&dstpimctrue,"dstpimctrue/I");
  Ntuple->Branch("dstpidmres",&dstpidmres,"dstpidmres/F");
  Ntuple->Branch("dstpigenmass",&dstpigenmass,"dstpigenmass/F");
  Ntuple->Branch("dstpigendist",&dstpigendist,"dstpigendist/F");
  Ntuple->Branch("dstpimclund",&dstpimclund,"dstpimclund/I");

  Ntuple->Branch("dstarcostheta",&dstarcostheta,"dstarcostheta/F");

  Ntuple->Branch("pi1p3",&pi1p3,"pi1p3/F");
  Ntuple->Branch("pi1charge",&pi1charge,"pi1charge/F");
  Ntuple->Branch("pi1mctrue",&pi1mctrue,"pi1mctrue/I");
  Ntuple->Branch("pi1lh",&pi1lh,"pi1lh/F");
  Ntuple->Branch("pi1lhK",&pi1lhK,"pi1lhK/F"); 
  Ntuple->Branch("pi1dedx",&pi1dedx,"pi1dedx/F");
  Ntuple->Branch("pi1cosine",&pi1cosine,"pi1cosine/F");
  Ntuple->Branch("pi1thetac",&pi1thetac,"pi1thetac/F");
  Ntuple->Branch("pi1costheta",&pi1costheta,"pi1costheta/F");
  Ntuple->Branch("pi1helicity",&pi1helicity,"pi1helicity/F");

  cout<<"Done Making Reduced Ntuple Branches"<<endl;
  return 1;
}



Int_t Dst0PiAnalysis::SetDst0PiReducedNtupleBranches(){
  cout<<"Setting Reduced Ntuple Branch Addresses"<<endl;

  ReducedNtuple->SetBranchAddress("dstpimass",&dstpimass);
  ReducedNtuple->SetBranchAddress("dstpideltam",&dstpideltam);
  ReducedNtuple->SetBranchAddress("dstpipstar",&dstpipstar);
  ReducedNtuple->SetBranchAddress("dstpicosstar",&dstpicosstar);
  ReducedNtuple->SetBranchAddress("dstpiIdx",&dstpiIdx);
  ReducedNtuple->SetBranchAddress("dstpimctrue",&dstpimctrue);
  ReducedNtuple->SetBranchAddress("dstpicharge",&dstpicharge);
  ReducedNtuple->SetBranchAddress("dstpinX",&dstpinX);
  ReducedNtuple->SetBranchAddress("dstpilogvtxprob",&dstpilogvtxprob);
  ReducedNtuple->SetBranchAddress("dstpidmres",&dstpidmres);
  ReducedNtuple->SetBranchAddress("dstpigenmass",&dstpigenmass);
  ReducedNtuple->SetBranchAddress("dstpigendist",&dstpigendist);
  ReducedNtuple->SetBranchAddress("dstpimclund",&dstpimclund);

  ReducedNtuple->SetBranchAddress("pi1p3",&pi1p3);
  ReducedNtuple->SetBranchAddress("pi1charge",&pi1charge);
  ReducedNtuple->SetBranchAddress("pi1mctrue",&pi1mctrue);
  ReducedNtuple->SetBranchAddress("pi1lh",&pi1lh);
  ReducedNtuple->SetBranchAddress("pi1lhK",&pi1lhK); 
  ReducedNtuple->SetBranchAddress("pi1dedx",&pi1dedx);
  ReducedNtuple->SetBranchAddress("pi1cosine",&pi1cosine);
  ReducedNtuple->SetBranchAddress("pi1thetac",&pi1thetac);
  ReducedNtuple->SetBranchAddress("pi1costheta",&pi1costheta);
  ReducedNtuple->SetBranchAddress("pi1helicity",&pi1helicity);


  
  cout<<"Done linking ReducedNtuple Branches"<<endl;
  return 1;
}
