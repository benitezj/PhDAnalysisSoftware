#include "DstPi0Analysis.h"

// /////////
//#include "DstPi0AnalysisPlotDstPi0Mass.C"

ClassImp(DstPi0Analysis);
DstPi0Analysis::DstPi0Analysis(Bool_t MCorDATA,TString OutputDir):
  DstAnalysis(MCorDATA,OutputDir),
  _cutdmass(0),
  _cutdstdeltam(0),
  _cutpstar(0),
  _cutpi0mass(0),
  _cutvtx(0),
  _cutdstcos(0),
  massfitter(&mass),
  dau1m("dau1m","dau1m",DstPDGMass),
  dau2m("dau2m","dau2m",Pi0PDGMass),
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


  DMassCutWindow=.02;
  DMassCutCenter=D0PDGMass;  


  cout<<"Done Initializing the analysis"<<endl;
}

DstPi0Analysis::~DstPi0Analysis(){  
  
}


Int_t DstPi0Analysis::ReduceNtuple(){
  cout<<"Going to reduce the ntuple"<<endl;

  if(_fChain==NULL){
    cout<<"No chain."<<endl;
    return 0;
  } 


  ///link the branches in the input chain
  SetDstPi0Branches();

  //the file must be created here
  TFile File(_OutputDir+"/Ntuple_New.root","recreate",_OutputDir+"/Ntuple_New.root",1);
  if(File.IsZombie()){cout<<"Failed to create file"<<endl;return 0;}

  //create new reduced ntuple
  TTree Ntuple("Ntuple","Reduced Ntuple",99);        
  Long64_t maxsize=10000000000;
  Ntuple.SetMaxTreeSize(maxsize);//split files

  //disable autosaving 
  Long64_t fautosave=10000000000;
  cout<<" autosave "<<fautosave<<endl;
  Ntuple.SetAutoSave(fautosave);//split ntuples in file

  //create the branches in the ReducedNtuple
  MakeDstPi0ReducedNtupleBranches(&Ntuple);

  Int_t RecoXCounterTotal=0;
  cout<<"Going to start the event loop"<<endl;
  cout<<"Total Entries= "<<_fChain->GetEntries()<<endl;
  Int_t eventid=0;
  while(_fChain->GetEntry(eventid,0)>0){   
    eventid++;
     
    if(eventid%50000==0)cout<<eventid<<" Events done.   With "<<RecoXCounterTotal<<" Candidates saved."<<endl;          
    eventnumber=eventid;     
    if(nX>MAXNCANDS)nX=MAXNCANDS;

    FillEventVars();

    for(XIdx=0;XIdx<nX;XIdx++){
 
      FillDstPi0Vars();

      //apply some simple cuts to minimize file size
      if(_cutpstar==1)if(dstpipstar<3.)continue;   
      if(_cutdmass==1)if(fabs(dmass-D0PDGMass)>.02) continue;  
      if(_cutdstdeltam==2)if(fabs(dstdeltam-DstMeanDeltaM)<.002||.003<fabs(dstdeltam-DstMeanDeltaM)) continue;  
      if(_cutpi0mass==2)if(fabs(pi1mass-Pi0MeanMass)<.02||.03<fabs(pi1mass-Pi0MeanMass))continue;

      if(_cutpi0mass==1)if(fabs(pi1mass-Pi0MeanMass)>.01) continue;       
      if(_cutdstdeltam==1)if(fabs(dstdeltam-DstMeanDeltaM)>.001) continue;        
      
//       if(_cutpi0mass==1&&_cutdstdeltam==1)
// 	if(pow((pi1mass-Pi0MeanMass)/.01,2)+pow((dstdeltam-DstMeanDeltaM)/.001,2)>1)continue;
      
//       if(_cutdstdeltam==3&&_cutpi0mass==3)
// 	// 	if(!((fabs(dstdeltam-DstMeanDeltaM)>.002&&.003>fabs(dstdeltam-DstMeanDeltaM))
// 	// 	     ||(fabs(pi1mass-Pi0MeanMass)>.02&&.03>fabs(pi1mass-Pi0MeanMass)))
// 	// 	   ||.003<fabs(dstdeltam-DstMeanDeltaM)
// 	// 	   ||.03<fabs(pi1mass-Pi0MeanMass)) continue;	
// 	//	if(!(pow((pi1mass-Pi0MeanMass)/.02,2)+pow((dstdeltam-DstMeanDeltaM)/.002,2)>1
// 	//   &&pow((pi1mass-Pi0MeanMass)/.03,2)+pow((dstdeltam-DstMeanDeltaM)/.003,2)<1))continue;  
// 	if(!(
// 	     (-.003<dstdeltam-DstMeanDeltaM&&dstdeltam-DstMeanDeltaM<-.002
// 	      &&-.03<pi1mass-Pi0MeanMass&&pi1mass-Pi0MeanMass<-.02)
// 	     ||(-.003<dstdeltam-DstMeanDeltaM&&dstdeltam-DstMeanDeltaM<-.002
// 		&&.02<pi1mass-Pi0MeanMass&&pi1mass-Pi0MeanMass<.03)
// 	     ||(.002<dstdeltam-DstMeanDeltaM&&dstdeltam-DstMeanDeltaM<.003
// 		&&-.03<pi1mass-Pi0MeanMass&&pi1mass-Pi0MeanMass<-.02)
// 	     ||(.002<dstdeltam-DstMeanDeltaM&&dstdeltam-DstMeanDeltaM<.003
// 		&&.02<pi1mass-Pi0MeanMass&&pi1mass-Pi0MeanMass<.03)
// 	     )) continue;
      
      if(_cutvtx==1)if(dstpilogvtxprob<-3.)continue;
      if(_cutdstcos==1)if(dstcostheta>.8) continue;  

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


void DstPi0Analysis::FillDstPi0Vars(){
  ///determine the indexes for each particle
  DstIdx=Xd1Idx[XIdx];
  Pi1Idx=Xd2Idx[XIdx];   
  if(DstIdx>=MAXNCANDS||Pi1Idx>=MAXNCANDS)return;
  
  /////////
  FillDstVars();
         
  //////fill reduced ntuple variables    
  dstpimass=XMass[XIdx];      
  dstpipstar=Xp3CM[XIdx];
  dstpicosstar=XcosthCM[XIdx];
  dstpideltam=XMass[XIdx]-DstMass[DstIdx]-Pi0Mass[Pi1Idx];  
  if(TMath::Prob(XChi2[XIdx],XnDof[XIdx])>0)
    dstpilogvtxprob=log10(TMath::Prob(XChi2[XIdx],XnDof[XIdx]));
  else dstpilogvtxprob=-100;
  dstpicharge=XLund[XIdx]/abs(XLund[XIdx]);
  dstpinX=nX;  
  dstpiIdx=XIdx;
  dstcostheta=ComputeDstAngle();    
  dstpimctrue=(XMCIdx[XIdx]>0);

  //compute the X mass using mass constrained 4-vectors.
  TVector3 Dstp3Vec(Dstp3CM[DstIdx]*sin(acos(DstcosthCM[DstIdx]))*cos(DstphiCM[DstIdx]),
		    Dstp3CM[DstIdx]*sin(acos(DstcosthCM[DstIdx]))*sin(DstphiCM[DstIdx]),
		    Dstp3CM[DstIdx]*DstcosthCM[DstIdx]); 
  TLorentzVector Dstp4(Dstp3Vec,sqrt(Dstp3Vec*Dstp3Vec+DstarPDGMass*DstarPDGMass));  
  
  TVector3 Pi0p3Vec(Pi0p3CM[Pi1Idx]*sin(acos(Pi0costhCM[Pi1Idx]))*cos(Pi0phiCM[Pi1Idx]),
		    Pi0p3CM[Pi1Idx]*sin(acos(Pi0costhCM[Pi1Idx]))*sin(Pi0phiCM[Pi1Idx]),
		    Pi0p3CM[Pi1Idx]*Pi0costhCM[Pi1Idx]); 
  
  TLorentzVector Pi0p4(Pi0p3Vec,sqrt(Pi0p3Vec*Pi0p3Vec+Pi0PDGMass*Pi0PDGMass));
    
  dstpimdm=(Dstp4+Pi0p4).Mag();


  //---Pi1
  pi1p3=Pi0p3[Pi1Idx];
  pi1mass=Pi0Mass[Pi1Idx];
  pi1costheta=ComputePiAngle();
  pi1helicity=ComputePiHelicityAngle();

  
}


Float_t DstPi0Analysis::ComputeDstAngle(){
  TVector3 Xp3vec(Xp3CM[XIdx]*sin(acos(XcosthCM[XIdx]))*cos(XphiCM[XIdx]),
		  Xp3CM[XIdx]*sin(acos(XcosthCM[XIdx]))*sin(XphiCM[XIdx]),
		  Xp3CM[XIdx]*XcosthCM[XIdx]);
  
  Float_t XEnergy=sqrt(Xp3vec*Xp3vec + XMass[XIdx]*XMass[XIdx]);
  TVector3 XVelvec=(1/XEnergy)*Xp3vec;
  
  TVector3 Dstp3vec(Dstp3CM[DstIdx]*sin(acos(DstcosthCM[DstIdx]))*cos(DstphiCM[DstIdx]),
		    Dstp3CM[DstIdx]*sin(acos(DstcosthCM[DstIdx]))*sin(DstphiCM[DstIdx]),
		    Dstp3CM[DstIdx]*DstcosthCM[DstIdx]); 
  
  TLorentzVector Dstp4(Dstp3vec,sqrt(Dstp3vec*Dstp3vec+DstMass[DstIdx]*DstMass[DstIdx]));

  //boost the dstar momentum into rest frame of X
  Dstp4.Boost(-XVelvec);//watch the minus sign
  TVector3 DstBoostedp3vec=Dstp4.Vect();

  //dot product of boosted dstar direction with X direction in CM frame
  Float_t cosine=DstBoostedp3vec.Unit()*XVelvec.Unit();
  
  return cosine;
  
}

Float_t DstPi0Analysis::ComputePiAngle(){
  TVector3 Xp3vec(Xp3CM[XIdx]*sin(acos(XcosthCM[XIdx]))*cos(XphiCM[XIdx]),
		  Xp3CM[XIdx]*sin(acos(XcosthCM[XIdx]))*sin(XphiCM[XIdx]),
		  Xp3CM[XIdx]*XcosthCM[XIdx]);
  
  Float_t XEnergy=sqrt(Xp3vec*Xp3vec + XMass[XIdx]*XMass[XIdx]);
  TVector3 XVelvec=(1/XEnergy)*Xp3vec;

  TVector3 Pi0p3vec(Pi0p3CM[Pi1Idx]*sin(acos(Pi0costhCM[Pi1Idx]))*cos(Pi0phiCM[Pi1Idx]),
		    Pi0p3CM[Pi1Idx]*sin(acos(Pi0costhCM[Pi1Idx]))*sin(Pi0phiCM[Pi1Idx]),
		    Pi0p3CM[Pi1Idx]*Pi0costhCM[Pi1Idx]); 
  
  TLorentzVector Pi0p4(Pi0p3vec,sqrt(Pi0p3vec*Pi0p3vec+Pi0PDGMass*Pi0PDGMass));

  //boost the dstar momentum into rest frame of X
  Pi0p4.Boost(-XVelvec);//watch the minus sign
  TVector3 Pi0Boostedp3vec=Pi0p4.Vect();

  //dot product of boosted dstar direction with X direction 
  return Pi0Boostedp3vec.Unit()*Xp3vec.Unit();
  
}


Float_t DstPi0Analysis::ComputePiHelicityAngle(){
  TVector3 Dstp3vec(Dstp3CM[DstIdx]*sin(acos(DstcosthCM[DstIdx]))*cos(DstphiCM[DstIdx]),
		    Dstp3CM[DstIdx]*sin(acos(DstcosthCM[DstIdx]))*sin(DstphiCM[DstIdx]),
		    Dstp3CM[DstIdx]*DstcosthCM[DstIdx]);
  
  Float_t DstEnergy=sqrt(Dstp3vec*Dstp3vec + DstMass[DstIdx]*DstMass[DstIdx]);
  TVector3 DstVelvec=(1/DstEnergy)*Dstp3vec;
  
  TVector3 Pi0p3vec(Pi0p3CM[Pi1Idx]*sin(acos(Pi0costhCM[Pi1Idx]))*cos(Pi0phiCM[Pi1Idx]),
		    Pi0p3CM[Pi1Idx]*sin(acos(Pi0costhCM[Pi1Idx]))*sin(Pi0phiCM[Pi1Idx]),
		    Pi0p3CM[Pi1Idx]*Pi0costhCM[Pi1Idx]); 
  
  TLorentzVector Pi0p4(Pi0p3vec,sqrt(Pi0p3vec*Pi0p3vec+Pi0PDGMass*Pi0PDGMass));
  
  //boost the dstar momentum into rest frame of Dst
  Pi0p4.Boost(-DstVelvec);//watch the minus sign
  TVector3 Pi0Boostedp3vec=Pi0p4.Vect();

  //dot product of boosted dstar direction with Dst direction 
  return Pi0Boostedp3vec.Unit()*Dstp3vec.Unit();
  
}



Float_t DstPi0Analysis::ComputePiDstAngle(){
  
  TVector3 Dstp3vec(Dstp3CM[DstIdx]*sin(acos(DstcosthCM[DstIdx]))*cos(DstphiCM[DstIdx]),
		      Dstp3CM[DstIdx]*sin(acos(DstcosthCM[DstIdx]))*sin(DstphiCM[DstIdx]),
		      Dstp3CM[DstIdx]*DstcosthCM[DstIdx]); 

  TVector3 Pi0p3vec(Pi0p3CM[Pi1Idx]*sin(acos(Pi0costhCM[Pi1Idx]))*cos(Pi0phiCM[Pi1Idx]),
		   Pi0p3CM[Pi1Idx]*sin(acos(Pi0costhCM[Pi1Idx]))*sin(Pi0phiCM[Pi1Idx]),
		   Pi0p3CM[Pi1Idx]*Pi0costhCM[Pi1Idx]); 
  
  return Dstp3vec.Unit()*Pi0p3vec.Unit();
  
}




Int_t DstPi0Analysis::SetDstPi0Branches(){
  if(_fChain==NULL){cout<<"No Chain"<<endl;return 0;} 
  
  SetDstBranches();
  
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


  _fChain->SetBranchAddress("nPi0",&nPi0);
  _fChain->SetBranchAddress("Pi0Mass",Pi0Mass);
  _fChain->SetBranchAddress("Pi0p3",Pi0p3);
  _fChain->SetBranchAddress("Pi0costh",Pi0costh);
  _fChain->SetBranchAddress("Pi0phi",Pi0phi);
  _fChain->SetBranchAddress("Pi0p3CM",Pi0p3CM);
  _fChain->SetBranchAddress("Pi0costhCM",Pi0costhCM);
  _fChain->SetBranchAddress("Pi0phiCM",Pi0phiCM);
  _fChain->SetBranchAddress("Pi0Lund",Pi0Lund); 
  _fChain->SetBranchAddress("Pi0d1Lund",Pi0d1Lund);
  _fChain->SetBranchAddress("Pi0d1Idx",Pi0d1Idx);
  _fChain->SetBranchAddress("Pi0d2Lund",Pi0d2Lund);
  _fChain->SetBranchAddress("Pi0d2Idx",Pi0d2Idx);
  _fChain->SetBranchAddress("Pi0Vtxx",Pi0Vtxx);
  _fChain->SetBranchAddress("Pi0Vtxy",Pi0Vtxy);
  _fChain->SetBranchAddress("Pi0Vtxz",Pi0Vtxz);

  cout<<"Done linking Chain Branches"<<endl;
  return 1;
}



Int_t DstPi0Analysis::MakeDstPi0ReducedNtupleBranches(TTree*Ntuple){
  cout<<"Creating Reduced Ntuple Branches"<<endl;

  MakeDstReducedNtupleBranches(Ntuple);

  Ntuple->Branch("dstpimass",&dstpimass,"dstpimass/F");
  Ntuple->Branch("dstpideltam",&dstpideltam,"dstpideltam/F");
  Ntuple->Branch("dstpimdm",&dstpimdm,"dstpimdm/F");
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

  Ntuple->Branch("dstcostheta",&dstcostheta,"dstcostheta/F");

  Ntuple->Branch("pi1p3",&pi1p3,"pi1p3/F");
  Ntuple->Branch("pi1mass",&pi1mass,"pi1mass/F");
  Ntuple->Branch("pi1charge",&pi1charge,"pi1charge/F");
  Ntuple->Branch("pi1mctrue",&pi1mctrue,"pi1mctrue/I");
  Ntuple->Branch("pi1cosine",&pi1cosine,"pi1cosine/F");
  Ntuple->Branch("pi1costheta",&pi1costheta,"pi1costheta/F");
  Ntuple->Branch("pi1helicity",&pi1helicity,"pi1helicity/F");

  cout<<"Done Making Reduced Ntuple Branches"<<endl;
  return 1;
}



Int_t DstPi0Analysis::MakeRawDstPi0Mass(){
 
  if(OpenReducedNtuple()!=1) return 0;
  TH1F HDstPi0Mass("HDstPi0Mass","",1500,2.1,3.6);
  //ReducedNtuple->Draw("dstpimdm>>HDstPi0Mass");
  ReducedNtuple->Draw("dstpideltam+2.0099+.13497>>HDstPi0Mass");
  if(CloseReducedNtuple()!=1) return 0;
  
  TFile file(_OutputDir+"/RawMassPlots.root","recreate");
  HDstPi0Mass.Write();
  file.Close();

  return 1;
}

Int_t DstPi0Analysis::PlotRawDstPi0Mass(){
    
  filename=_OutputDir+"/RawDstPi0MassPlots.ps";
  Canvas.Print(filename+"["); 

  TFile file(_OutputDir+"/RawMassPlots.root","read");
  gROOT->cd();
 
  Int_t nbins=300;
  Float_t min=2.1;
  Float_t max=3.6;

  //right sign
  TH1F*HDstPi0Mass=(TH1F*)file.Get("HDstPi0Mass");
  if(!(HDstPi0Mass=(TH1F*)RebinHistoX(HDstPi0Mass,nbins,min,max)))return 0; 
  SetHistoXYLabels(HDstPi0Mass,"m(D^{*+}#pi^{0})","GeV/c^{2}");
  HDstPi0Mass->GetYaxis()->SetRangeUser(0,HDstPi0Mass->GetMaximum()*1.1);

  ///
  Canvas.Clear();
  HDstPi0Mass->Draw("pe");
  Canvas.Print(filename); 

  Canvas.Print(filename+"]");

  file.Close();

  return 1;
}

Int_t DstPi0Analysis::FitMass(TString name){

  TFile file(_OutputDir+"/RawMassPlots.root","read");
  gROOT->cd();

  TFile sidefile(_OutputDir+"/../KpiAndK3piPi0Side/RawMassPlots.root","read");
  gROOT->cd();

  filename=_OutputDir+"/DstPi0MassPlots.ps";
  Canvas.Print(filename+"[");  

  TH1F*HDstPi0Mass=(TH1F*)file.Get(name);
  TH1F*HDstPi0MassSB=NULL;
  if(!sidefile.IsZombie())HDstPi0MassSB=(TH1F*)sidefile.Get(name);    
  if(HDstPi0MassSB){
    cout<<" Side band Histo found. Going to subtract sideband"<<endl;

    HDstPi0MassSB->SetMarkerColor(4);
    HDstPi0MassSB->SetLineColor(4);
    Canvas.Clear();
    HDstPi0Mass->Draw("pe");
    HDstPi0MassSB->Draw("pesame");
    Canvas.Print(filename); 

    //HDstPi0MassSB->Scale((.002*.02)/((.006*.06)-(.004*.04)));//scale down the area in the squares
    //HDstPi0MassSB->Scale((mypi*.001*.01)/((mypi*.003*.03)-(mypi*.002*.02)));//scale down the area in the circles
    HDstPi0MassSB->Scale(.95/*1.04779*/);//scale according to 1-D pi0 fit

    Canvas.Clear();
    HDstPi0Mass->Draw("pe");
    HDstPi0MassSB->Draw("pesame");
    Canvas.Print(filename); 

    HDstPi0Mass->Sumw2();
    HDstPi0MassSB->Sumw2();
    HDstPi0Mass->Add(HDstPi0MassSB,-1);
  }
 
  if(!HDstPi0Mass){cout<<"no histo"<<endl;return 0;}
  SetHistoXYLabels(HDstPi0Mass,"m(D^{*+)#pi^{0})","GeV/c^{2}");
  HDstPi0Mass->SetStats(0);
  
  //massfitter.SetEfficiency(HDstPi0MassEff); 
  if(!massfitter.Initialize(HDstPi0Mass,_OutputDir+"/InitRooFitPars.txt"))return 0;
  if(massfitter.Fit()<0)return 0;
  massfitter.Save(_OutputDir);
 
  Canvas.Clear();
  HDstPi0Mass->Draw("pe");
  Canvas.Print(filename); 

  RooPlot*fitplot=massfitter.GetFitPlot(); 
  Canvas.Clear();
  fitplot->Draw();
  Canvas.Print(filename);
  delete fitplot; 


  Canvas.Print(filename+"]");

 
  file.Close();
  return 1;
}



Int_t DstPi0Analysis::MakeRawPi0Mass(){
 
  if(OpenReducedNtuple()!=1) return 0;
  TH1F HPi0Mass("HPi0Mass","",150,.09,.165);
  ReducedNtuple->Draw("pi1mass>>HPi0Mass");
  if(CloseReducedNtuple()!=1) return 0;
  
  TFile file(_OutputDir+"/RawPi0MassPlots.root","recreate");
  HPi0Mass.Write();
  file.Close();

  return 1;
}

Int_t DstPi0Analysis::PlotRawPi0Mass(){
    

  filename=_OutputDir+"/RawPi0MassPlots.ps";
  Canvas.Print(filename+"["); 

  TFile file(_OutputDir+"/RawPi0MassPlots.root","read");
  gROOT->cd();
 

  //right sign
  TH1F*HPi0Mass=(TH1F*)file.Get("HPi0Mass");
  if(!HPi0Mass){cout<<"no histo"<<endl;return 0;}
  SetHistoXYLabels(HPi0Mass,"m(#gamma#gamma)","GeV/c^{2}");
  HPi0Mass->GetYaxis()->SetRangeUser(0,HPi0Mass->GetMaximum()*1.1);

  ///
  Canvas.Clear();
  HPi0Mass->Draw("pe");
  Canvas.Print(filename); 

  Canvas.Clear();
  HPi0Mass->Draw("pe");
  cutline.SetLineColor(2);
  cutline.DrawLine(Pi0MeanMass-.01,0,Pi0MeanMass-.01,HPi0Mass->GetMaximum());
  cutline.DrawLine(Pi0MeanMass+.01,0,Pi0MeanMass+.01,HPi0Mass->GetMaximum());

  cutline.SetLineColor(1);
  cutline.DrawLine(Pi0MeanMass-.03,0,Pi0MeanMass-.03,.55*HPi0Mass->GetMaximum());
  cutline.DrawLine(Pi0MeanMass-.02,0,Pi0MeanMass-.02,.55*HPi0Mass->GetMaximum());

  cutline.DrawLine(Pi0MeanMass+.02,0,Pi0MeanMass+.02,.55*HPi0Mass->GetMaximum());
  cutline.DrawLine(Pi0MeanMass+.03,0,Pi0MeanMass+.03,.55*HPi0Mass->GetMaximum());
  Canvas.Print(filename); 



  delete HPi0Mass->FindObject("stats");
  Canvas.Clear();
  TF1*fit=FitHistogram1gausPlusPolN(HPi0Mass,.092,.163,2,0);
  Draw1gausPlusPolN(fit,2);
  Canvas.Print(filename); 

  delete HPi0Mass->FindObject("stats");
  Canvas.Clear();
  TF1*fit2=FitHistogram2gausPlusPolN(HPi0Mass,.092,.163,2,0);
  Draw2gausPlusPolN(fit2,2);
  Canvas.Print(filename); 

  //print out the scale factor for the sideband subtraction.
  TF1 Fbkg("Fbkg","[0]+[1]*x+[2]*x*x",fit2->GetXmin(),fit2->GetXmax());
  Fbkg.SetParameters(fit2->GetParameter(6),fit2->GetParameter(7),fit2->GetParameter(8));
  cout<<"Int. Sig= "<<Fbkg.Integral(Pi0MeanMass-.01,Pi0MeanMass+.01)<<endl;
  cout<<"Int. low side= "<<Fbkg.Integral(Pi0MeanMass-.03,Pi0MeanMass-.02)<<endl;
  cout<<"Int. high side= "<<Fbkg.Integral(Pi0MeanMass+.02,Pi0MeanMass+.03)<<endl;
  cout<<" scale factor = "<<Fbkg.Integral(Pi0MeanMass-.01,Pi0MeanMass+.01)/(Fbkg.Integral(Pi0MeanMass-.03,Pi0MeanMass-.02)+Fbkg.Integral(Pi0MeanMass+.02,Pi0MeanMass+.03))<<endl;

  delete HPi0Mass->FindObject("stats");
  HPi0Mass->SetStats(0);
  Canvas.Clear();
  HPi0Mass->Draw("pe");
  cutline.SetLineColor(2);
  cutline.DrawLine(Pi0MeanMass-.01,0,Pi0MeanMass-.01,HPi0Mass->GetMaximum());
  cutline.DrawLine(Pi0MeanMass+.01,0,Pi0MeanMass+.01,HPi0Mass->GetMaximum());
  cutline.SetLineColor(1);
  cutline.DrawLine(Pi0MeanMass-.03,0,Pi0MeanMass-.03,.55*HPi0Mass->GetMaximum());
  cutline.DrawLine(Pi0MeanMass-.02,0,Pi0MeanMass-.02,.55*HPi0Mass->GetMaximum());
  cutline.DrawLine(Pi0MeanMass+.02,0,Pi0MeanMass+.02,.55*HPi0Mass->GetMaximum());
  cutline.DrawLine(Pi0MeanMass+.03,0,Pi0MeanMass+.03,.55*HPi0Mass->GetMaximum());
  Draw2gausPlusPolN(fit2,2);
  Canvas.Print(filename); 




  Canvas.Print(filename+"]");

  file.Close();

  return 1;
}



Int_t DstPi0Analysis::MakeRawDstMassPi0Mass(){
 
  if(OpenReducedNtuple()!=1) return 0;
  TH2F HDstMassPi0Mass("HDstMassPi0Mass","",150,.09,.165,70,.142,.149);
  ReducedNtuple->Draw("dstdeltam:pi1mass>>HDstMassPi0Mass");
  if(CloseReducedNtuple()!=1) return 0;
  
  TFile file(_OutputDir+"/RawDstMassPi0MassPlots.root","recreate");
  HDstMassPi0Mass.Write();
  file.Close();

  return 1;
}

Int_t DstPi0Analysis::PlotRawDstMassPi0Mass(){
    

  filename=_OutputDir+"/RawDstMassPi0MassPlots.ps";
  Canvas.Print(filename+"["); 

  TFile file(_OutputDir+"/RawDstMassPi0MassPlots.root","read");
  gROOT->cd();
 

  //right sign
  TH2F*HDstMassPi0Mass=(TH2F*)file.Get("HDstMassPi0Mass");
  if(!HDstMassPi0Mass){cout<<"no histo"<<endl;return 0;}
  HDstMassPi0Mass->GetXaxis()->SetTitle("m(#gamma#gamma) (GeV/c^{2})");
  HDstMassPi0Mass->GetYaxis()->SetTitle("m(D*)-m(D0) (GeV/c^{2})");

  ///
  HDstMassPi0Mass->SetStats(0);
  Canvas.Clear();
  HDstMassPi0Mass->Draw("colz");
  Canvas.Print(filename); 


  Canvas.Clear();
  HDstMassPi0Mass->Draw("colz");
  TBox box;
  box.SetFillStyle(0);
  box.SetLineWidth(2);
  box.SetLineColor(2);
  box.DrawBox(Pi0MeanMass-.01,DstMeanDeltaM-.001,Pi0MeanMass+.01,DstMeanDeltaM+.001);
  box.SetLineColor(1);
  box.DrawBox(Pi0MeanMass-.02,DstMeanDeltaM-.002,Pi0MeanMass+.02,DstMeanDeltaM+.002);
  box.DrawBox(Pi0MeanMass-.03,DstMeanDeltaM-.003,Pi0MeanMass+.03,DstMeanDeltaM+.003);
  Canvas.Update();
  Canvas.Print(filename); 

  Canvas.Print(filename+"]");

  file.Close();

  return 1;
}



