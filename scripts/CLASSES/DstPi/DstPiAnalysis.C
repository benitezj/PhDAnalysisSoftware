#include "DstPiAnalysis.h"
#include "../../FUNCTIONS/DetermineFWHM.C"
#include "../../FUNCTIONS/RebinHisto.h"
#include "../RooResolutionPdf.h"
//#include "../DCPi/DCPiAnalysis.h" // cant include this file contains variables with same names
#include <RooHist.h>
#include <RooGaussian.h>
#include "RooPolynomial.h"
#include <TGaxis.h>

ClassImp(DstPiAnalysis);
DstPiAnalysis::DstPiAnalysis(Int_t MCorDATA,TString OutputDir):
  TNamed((const char*)OutputDir,"DstPiAnalysis"),
  _fChain(0),
  _MCorDATA(MCorDATA),
  _OutputDir(OutputDir), 
  _D0Mode(1),
  OutPutFile(NULL),
  ReducedFile(NULL),
  ReducedNtuple(NULL),
  AlreadyTriedClean(0),
  cuttruth(0),
  cutcharge(0),
  cutdmass(0),
  cutd0mode(0),
  cutdeltam(0),
  cutpstar(0),
  cutdcos(0),
  mass("mass","mass",2.1,3.1), 
  dau1m("dau1m","dau1m",DstarPDGMass),
  dau2m("dau2m","dau2m",PiPDGMass),
  TwoBodyPS("TwoBodyPS","TwoBodyPS",mass,dau1m,dau2m),
  radius("radius","radius",4.),//5GeV^-1~1fm, 3GeV^-1~.6fm
  resmean("resmean","resmean",-.01,.01),
  resoffset("resoffset","resoffset",-.02,.02),
  resslope("resslope","resslope",0,.01),
  resnormratio2("resnormratio2","resnormratio2",0.,10),
  resmean2("resmean2","resmean2",-.1,.1),
  ressigmaratio2("ressigmaratio2","ressigmaratio2",1,10),
  resnormratio3("resnormratio3","resnormratio3",0.,10),
  resmean3("resmean3","resmean3",-.1,.1),
  ressigmaratio3("ressigmaratio3","ressigmaratio3",.1,10),
  resVar("resVar","resVar",-.02,.02),
  effslope("effslope","effslope",-100,100),
  massfitter(&mass),
  MCXCounterPerEvent(0),
  MCXCounterTotal(0),
  RecoXCounterTotal(0)
{
  if(_fChain==NULL){cout<<"No Chain"<<endl;}
   
  //init the database 
  //database.SetTruthMatch(_MCorDATA<2);
  // database.SetMode(_D0Mode);
  //database.SetCuts();

  //the phase space and q factor for the threshold
  //phasefactor="(1./(dstpim*dstpim))*sqrt((dstpim*dstpim - 4.618)*(dstpim*dstpim-3.500))";
  //qfactor="((1/(2*(dstpim)))*sqrt(((dstpim)*(dstpim) - 4.618)*((dstpim)*(dstpim)-3.500)))";

  //mass dependent resolution
  resVar.setBins(100);
  resmean.setVal(-3.8e-5);resmean.setConstant(1);
  resoffset.setVal(-.01001);resoffset.setConstant(1);
  resslope.setVal(.0049);resslope.setConstant(1);

  resnormratio2.setVal(.0902);resnormratio2.setConstant(1);
  resmean2.setVal(-0.00258);resmean2.setConstant(1);
  ressigmaratio2.setVal(4.177);ressigmaratio2.setConstant(1);
  
  resnormratio3.setVal(0.2264);resnormratio3.setConstant(1);  
  resmean3.setVal(0.0002576);resmean3.setConstant(1);
  ressigmaratio3.setVal(0.700);ressigmaratio3.setConstant(1);
 
  
  //-----------------------
  //The resolution functions
  //-----------------------  
//   resoffset=-.01001;
//   resslope=.0049;
//   resmean=-3.8e-5;
//   resnormratio2=.0902;
//   resmean2=-0.00258;
//   ressigmaratio2=4.177;
//   resnormratio3=0.2264;
//   resmean3=0.0002576;
//   ressigmaratio3=0.700; 
//   char ressig1[30];
//   sprintf(ressig1,"(%.6f+%.6f*mass)",resoffset,resslope);
//   char resgaus1[200];
//   sprintf(resgaus1,"exp(-.5*(resVar-%.6f)**2/%s**2)/(%s*2.507)",resmean,ressig1,ressig1);
//   char resgaus2[300];
//   sprintf(resgaus2,"(%.2f*%s/3.14159)/((resVar-%.6f-%.6f)**2+(%.2f*%s)**2)",ressigmaratio2,ressig1,resmean,resmean2,ressigmaratio2,ressig1);
//   char resgaus3[300];
//   sprintf(resgaus3,"(%.2f*%s/3.14159)/((resVar-%.6f-%.6f)**2+(%.2f*%s)**2)",ressigmaratio3,ressig1,resmean,resmean3,ressigmaratio3,ressig1);
//   char resformul[700];
//   sprintf(resformul,"(1.-%.3f-%.3f)*%s+%.3f*%s+%.3f*%s",resnormratio2,resnormratio3,resgaus1,resnormratio2,resgaus2,resnormratio3,resgaus3);
  //respdf=new RooGenericPdf("respdf","respdf",resformul,RooArgSet(resVar,mass)); 

  //respdf=new RooResolutionPdf("respdf","respdf",&resVar,mass,resoffset,resslope,resmean,resnormratio2,resmean2,ressigmaratio2,resnormratio3,resmean3,ressigmaratio3);
  //use simple gaussian, above formula causes convergence problems
  resoffset.setVal(.003);resoffset.setConstant(1);
  respdf=new RooGaussian("respdf","respdf",resVar,resmean,resoffset); 


  ////////efficiency as a function of mass
  effslope.setVal(.06);effslope.setConstant(1);
  effpdf=new RooGenericPdf("effpdf","effslope*mass",RooArgList(mass,effslope));

  effoffsetKpi=-0.009951;//8/5/2010 after charge bug 
  effslopeKpi=0.07437;
  effoffsetK3pi=0.0336;
  effslopeK3pi=0.03519;


  //
  massfitter.SetPhaseSpace(&TwoBodyPS);
  massfitter.SetEfficiencyPdf(effpdf);
  massfitter.SetResolution(respdf,&resVar);
  massfitter.SetDauMasses(&dau1m,&dau2m);

  //efficiency shape parameters;
  if(_D0Mode==1){helioffset=.6422;helislope=-.011;}
  if(_D0Mode==2){helioffset=.644;helislope=-.0027;}  
  if(_D0Mode==1){cosdstoffset=.884;cosdstslope=.11;}
  if(_D0Mode==2){cosdstoffset=.908;cosdstslope=.197;}

  
  //
  roofitconfigfile="InitRooFitPars.txt";
  

  ///labels
  //DstarPiXLabel="m(D*^{+}#pi^{-})  -  m(D*^{+})   (GeV/c^{2}) ";
  DstarPiXLabel="m(D*#pi)  (GeV/c^{2})";
  DstarPiYLabel="Entries / 3 MeV/c^{2}"; 



  ///lumi scaling
  Nccbar=1088218000;
  Crossccbar=1.30;//nanobarns
  lumiccbar=Nccbar/Crossccbar;
  lumidata=480000000;
  scalefactorccbar=lumidata/lumiccbar;
  cout<<" ccbar lumi scale factor "<<scalefactorccbar<<endl;



  //Systematics
  SystDirs[0]="Nominal";
  SystDirs[1]="MassRange";
  SystDirs[2]="BinWidth";
  SystDirs[3]="BWShape";
  SystDirs[4]="Background";
  SystDirs[5]="D2460Mean";
  SystDirs[6]="D2460Width";
  SystDirs[7]="D2620Mean";
  SystDirs[8]="D2620Width";
  SystDirs[9]="0"; 
  SystDirs[10]="D2430"; 
  SystDirs[11]="0"; 
  SystDirs[12]="0"; 
  SystDirs[13]="0"; 
  SystDirs[14]="0"; 
 
  SystName[0]="0";
  SystName[1]="Mass Range";
  SystName[2]="Bin Width";
  SystName[3]="Breit-Wigner Shape";
  SystName[4]="Background Model";
  SystName[5]="D(2460) Mean";
  SystName[6]="D(2460) Width";
  SystName[7]="D(2600) Mean";
  SystName[8]="D(2600) Width";
  SystName[9]="B field \\& SVT material";
  SystName[10]="D(2430)";
  SystName[11]="0";
  SystName[12]="0";
  SystName[13]="0";
  SystName[14]="0";


  SigName[0]="D2420";
  SigName[1]="D2460";
  SigName[2]="D2550";
  SigName[3]="D2620";
  SigName[4]="D2750";

  SigTitle[0]="\\DTwentyFourTwenty";
  SigTitle[1]="\\DTwentyFourSixty";
  SigTitle[2]="\\DTwentyFiveFifty";
  SigTitle[3]="\\DTwentySixHundred";
  SigTitle[4]="\\DTwentySevenFifty";

  SystDirs2[0]="Nominal";
  SystDirs2[1]="MassRange";
  SystDirs2[2]="BinWidth";
  SystDirs2[3]="BWShape";
  SystDirs2[4]="Background";
  SystDirs2[5]="D2420Mean";
  SystDirs2[6]="D2420Width";
  SystDirs2[7]="D2460Mean";
  SystDirs2[8]="D2460Width";
  SystDirs2[9]="D2550Mean";
  SystDirs2[10]="D2550Width";
  SystDirs2[11]="D2620Mean";
  SystDirs2[12]="D2620Width";
  SystDirs2[13]="D2750Mean";
  SystDirs2[14]="D2750Width";
  SystDirs2[15]="0"; 
  SystDirs2[16]="D2430"; 
  SystDirs2[17]="0"; 
  SystDirs2[18]="0"; 
  SystDirs2[19]="0"; 

  SystName2[0]="0";
  SystName2[1]="Mass Range";
  SystName2[2]="Bin Width";
  SystName2[3]="Breit-Wigner Shape";
  SystName2[4]="Background Model";
  SystName2[5]="D(2420) Mean";
  SystName2[6]="D(2420) Width";
  SystName2[7]="D(2460) Mean";
  SystName2[8]="D(2460) Width";
  SystName2[9]="D(2550) Mean";
  SystName2[10]="D(2550) Width";
  SystName2[11]="D(2600) Mean";
  SystName2[12]="D(2600) Width";
  SystName2[13]="D(2750) Mean";
  SystName2[14]="D(2750) Width";
  SystName2[15]="0";
  SystName2[16]="D(2430)";
  SystName2[17]="0";
  SystName2[18]="0";
  SystName2[19]="0";

  SystDirsFinal[0]="Nominal";
  SystDirsFinal[1]="MassRange";
  SystDirsFinal[2]="BinWidth";
  SystDirsFinal[3]="BWShape";
  SystDirsFinal[4]="Background";
  SystDirsFinal[5]="D2420Mean";
  SystDirsFinal[6]="D2420Width";
  SystDirsFinal[7]="D2460Mean";
  SystDirsFinal[8]="D2460Width";
  SystDirsFinal[9]="D2550Mean";
  SystDirsFinal[10]="D2550Width";
  SystDirsFinal[11]="D2620Mean";
  SystDirsFinal[12]="D2620Width";
  SystDirsFinal[13]="0";//B-field hard coded
  SystDirsFinal[14]="D2430"; 
  SystDirsFinal[15]="0"; 
  SystDirsFinal[16]="0"; 
  SystDirsFinal[17]="0"; 
  SystDirsFinal[18]="0"; 
  SystDirsFinal[19]="0"; 

  SystNameFinal[0]="0";
  SystNameFinal[1]="Mass Range";
  SystNameFinal[2]="Bin Width";
  SystNameFinal[3]="Breit-Wigner Shape";
  SystNameFinal[4]="Background Model";
  SystNameFinal[5]="D(2420) Mean";
  SystNameFinal[6]="D(2420) Width";
  SystNameFinal[7]="D(2460) Mean";
  SystNameFinal[8]="D(2460) Width";
  SystNameFinal[9]="D(2550) Mean";
  SystNameFinal[10]="D(2550) Width";
  SystNameFinal[11]="D(2600) Mean";
  SystNameFinal[12]="D(2600) Width";
  SystNameFinal[13]="B field \\& SVT material";
  SystNameFinal[14]="D(2430)";
  SystNameFinal[15]="0";
  SystNameFinal[16]="0";
  SystNameFinal[17]="0";
  SystNameFinal[18]="0";
  SystNameFinal[19]="0";
  

  SystDirsFinalHeli[0]="Nominal";
  SystDirsFinalHeli[1]="MassRange";
  SystDirsFinalHeli[2]="BinWidth";
  SystDirsFinalHeli[3]="BWShape";
  SystDirsFinalHeli[4]="Background";
  SystDirsFinalHeli[5]="D2420Mean";
  SystDirsFinalHeli[6]="D2420Width";
  SystDirsFinalHeli[7]="D2460Mean";
  SystDirsFinalHeli[8]="D2460Width";
  SystDirsFinalHeli[9]="D2550Mean";
  SystDirsFinalHeli[10]="D2550Width";
  SystDirsFinalHeli[11]="D2620Mean";
  SystDirsFinalHeli[12]="D2620Width";
  SystDirsFinalHeli[13]="0";//B-field hard coded
  SystDirsFinalHeli[14]="D2430"; 
  SystDirsFinalHeli[15]="D2750Mean"; 
  SystDirsFinalHeli[16]="D2750Width"; 
  SystDirsFinalHeli[17]="0"; 
  SystDirsFinalHeli[18]="0"; 
  SystDirsFinalHeli[19]="0"; 

  SystNameFinalHeli[0]="0";
  SystNameFinalHeli[1]="Mass Range";
  SystNameFinalHeli[2]="Bin Width";
  SystNameFinalHeli[3]="Breit-Wigner Shape";
  SystNameFinalHeli[4]="Background Model";
  SystNameFinalHeli[5]="D(2420) Mean";
  SystNameFinalHeli[6]="D(2420) Width";
  SystNameFinalHeli[7]="D(2460) Mean";
  SystNameFinalHeli[8]="D(2460) Width";
  SystNameFinalHeli[9]="D(2550) Mean";
  SystNameFinalHeli[10]="D(2550) Width";
  SystNameFinalHeli[11]="D(2600) Mean";
  SystNameFinalHeli[12]="D(2600) Width";
  SystNameFinalHeli[13]="B field \\& SVT material";
  SystNameFinalHeli[14]="D(2430)";
  SystNameFinalHeli[15]="D(2750) Mass";
  SystNameFinalHeli[16]="D(2750) Width";
  SystNameFinalHeli[17]="0";
  SystNameFinalHeli[18]="0";
  SystNameFinalHeli[19]="0";
  




  cout<<"Done Initializing the analysis"<<endl;
}

DstPiAnalysis::~DstPiAnalysis(){  
}


Int_t DstPiAnalysis::ReduceNtuple(){
  cout<<"Going to reduce the ntuple"<<endl;

  if(_fChain==NULL){
    cout<<"No chain."<<endl;
    return 0;
  } 


  ///link the branches in the input chain
  SetBranches();


  //the file must be created here so the 'new' histos and Tree is created in hard drive 
  OutPutFile=new TFile(_OutputDir+"/Ntuple_New.root","recreate");//1=minimal compression
  if(OutPutFile==NULL){cout<<"Failed to create file"<<endl;return 0;}


  //create new reduced ntuple
  TTree* Ntuple=new TTree("Ntuple","Ntuple",0);        
  Ntuple->SetMaxTreeSize(990000000);//otherwise the root file gets split in 2
  //Ntuple->SetMaxTreeSize(9900e6);//otherwise the root file gets split in 2

  //create the branches in the Ntuple
  MakeReducedNtupleBranches(Ntuple);


  if(_MCorDATA==0){
    //create the MC Histos
    cout<<"Going to init MC histograms"<<endl;
    HMCXMass=new TH1F("HMCXMass","HMCXMass",1000,2.1,3.1);
    HMCXMassDiff=new TH1F("HMCXMassDiff","HMCXMassDiff",12000,-.3,.3);    
    H3MCPvsMassVsDstAngle=new TH3F("H3MCPvsMassVsDstAngle","H3MCPvsMassVsDstAngle",4,3,5,40,2.1,3.1,3,-1.000,.5);
    H3MCPvsMassVsHelicity=new TH3F("H3MCPvsMassVsHelicity","H3MCPvsMassVsHelicity",4,3,5,40,2.1,3.1,4,-1.000,1.000);    
    H2MCPvsMass=new TH2F("H2MCPvsMass","H2MCPvsMass",4,3,5,40,2.1,3.1);
    H2MCPvsMassFine=new TH2F("H2MCPvsMassFine","H2MCPvsMassFine",4,3,5,1000,2.1,3.1);
    HMCXp3CM=new TH1F("HMCXp3CM","HMCXp3CM",50,0,5); 
    HMCXcosthCM=new TH1F("HMCXcosthCM","HMCXcosthCM",40,-1.000,1.000);
    H2MCXCMPvsTheta=new TH2F("H2MCXCMPvsTheta","H2MCXCMPvsTheta",50,0,5,40,-1.000,1.000);  
    HMCNX=new TH1F("HMCNX","HMCNX",11,-.5,10.5);
    HMCDstarHelicity=new TH1F("HMCDstarHelicity","HMCDstarHelicity",40,-1.000,1.000);
    HMCDstarAngle=new TH1F("HMCDstarAngle","HMCDstarAngle",40,-1.000,1.000);
    H3MCPvsThetaVsDstAngle=new TH3F("H3MCPvsThetaVsDstAngle","H3MCPvsThetaVsDstAngle",4,3,5,10,-1.000,1.000,3,-1.000,.5);
    
  }
  //////
 

  /////
  cout<<"Will use the following cuts"<<endl;
  cout<<" cuttruth "<<cuttruth
      <<" cutcharge "<<cutcharge
      <<" cutpstar "<<cutpstar
      <<" cutdmass "<<cutdmass
      <<" cutd0mode "<<cutd0mode
      <<" cutdeltam "<<cutdeltam
      <<" cutdcos "<<cutdcos<<endl;
  

  //choose best candidate
  //Int_t bestcandid=-1;
  //Float_t bestcandvar=0;    

  ///check how many candidates are removed by array size cut
  Int_t RemovedCands=0;
  Int_t InitialdCands=0;

  //
  Int_t NRecosThisEvent=0;
  Float_t AvgRecosPerEvent=0.;
  Int_t NGoodEvents=0;

  cout<<"Going to start the event loop for "<<_fChain->GetEntries()<<" events"<<endl;
  Int_t eventid=0; 
  while(_fChain->GetEntry(eventid,0)>0){   
    eventid++;
     
    if(eventid%50000==0)cout<<eventid<<" Events done.   With "<<RecoXCounterTotal<<" Candidates saved."<<endl;          
    eventnumber=eventid;
     
    InitialdCands+=nX;
    if(nX>MAXX){
      cout<<"Too many cands at event "<<eventid<<" Only first "<<MAXX<<" of "<<nX<<" will be used"<<endl;
      RemovedCands+=nX-MAXX; 
      nX=MAXX;      
    }

    ///Loop over the reconstructed
    RecoXCounterPerEvent=0;      
    
    //event id
    runnumber=runNumber;

    //------beam quantities
    TVector3 beamVtx(primVtxX,primVtxY,primVtxZ);

    NRecosThisEvent=0;
    //now do real loop
    for(XIdx=0;XIdx<nX;XIdx++){

      ////////////cut out the unconverged fits
      if(XVtxStatus[XIdx]!=0)continue;               


      ///determine the indexes for each particle
      Pi1Idx=Xd2Idx[XIdx];
      DstarIdx=Xd1Idx[XIdx];
      SlowPiIdx=Dstard2Idx[DstarIdx];
      D0Idx=Dstard1Idx[DstarIdx];
      KIdx=D0d1Idx[D0Idx];
      PiIdx=D0d2Idx[D0Idx];
      D0Pi2Idx=D0d3Idx[D0Idx];
      D0Pi3Idx=D0d4Idx[D0Idx];
          
      PitrkIdx=PiTrkIdx[PiIdx];
      KtrkIdx=KTrkIdx[KIdx];
      SlowPitrkIdx=PiTrkIdx[SlowPiIdx];
      Pi1trkIdx=PiTrkIdx[Pi1Idx];
      D0Pi2trkIdx=PiTrkIdx[D0Pi2Idx];
      D0Pi3trkIdx=PiTrkIdx[D0Pi3Idx];
       

      //////fill reduced ntuple variables    
      dstpimass=XMass[XIdx];
      dstpipstar=Xp3CM[XIdx];
      dstpicosstar=XcosthCM[XIdx];
      dstpiphistar=XphiCM[XIdx];
      dstpideltam=XMass[XIdx]-DstarMass[DstarIdx];  
      dstpimdm=dstpideltam+DstarPDGMass;
      if(TMath::Prob(XChi2[XIdx],XnDof[XIdx])>0)
	dstpilogvtxprob=log10(TMath::Prob(XChi2[XIdx],XnDof[XIdx]));
      else dstpilogvtxprob=-100;
      //Determine D*pi combination: dstpicharge= -2=D*-pi- , -1=D*-pi+  , 1=D*+pi- , 2=D*+pi+
      dstpicharge=DstarLund[DstarIdx]/abs(DstarLund[DstarIdx])+PiLund[Pi1Idx]/abs(PiLund[Pi1Idx]);
      if(dstpicharge==0)dstpicharge=DstarLund[DstarIdx]/abs(DstarLund[DstarIdx]);
      dstpinX=nX;  
      dstpiIdx=XIdx;
      masspislowpi1=ComputexyMass(Pip3[SlowPiIdx],Picosth[SlowPiIdx],Piphi[SlowPiIdx],PiPDGMass,
				  Pip3[Pi1Idx],Picosth[Pi1Idx],Piphi[Pi1Idx],PiPDGMass);
      masspipi1=ComputexyMass(Pip3[PiIdx],Picosth[PiIdx],Piphi[PiIdx],PiPDGMass,
			      Pip3[Pi1Idx],Picosth[Pi1Idx],Piphi[Pi1Idx],PiPDGMass);
      massKpislow=ComputexyMass(Kp3CM[KIdx],KcosthCM[KIdx],KphiCM[KIdx],KPDGMass,
				Pip3CM[SlowPiIdx],PicosthCM[SlowPiIdx],PiphiCM[SlowPiIdx],PiPDGMass);

      massKpislowpi1=ComputexyzMass(Kp3CM[KIdx],KcosthCM[KIdx],KphiCM[KIdx],KPDGMass,
				    Pip3CM[SlowPiIdx],PicosthCM[SlowPiIdx],PiphiCM[SlowPiIdx],PiPDGMass,
				    Pip3CM[Pi1Idx],PicosthCM[Pi1Idx],PiphiCM[Pi1Idx],PiPDGMass);
      massKpipi1=ComputexyzMass(Kp3CM[KIdx],KcosthCM[KIdx],KphiCM[KIdx],KPDGMass,
				Pip3CM[PiIdx],PicosthCM[PiIdx],PiphiCM[PiIdx],PiPDGMass,
				Pip3CM[Pi1Idx],PicosthCM[Pi1Idx],PiphiCM[Pi1Idx],PiPDGMass);
      masspipislowpi1=ComputexyzMass(Pip3[PiIdx],Picosth[PiIdx],Piphi[PiIdx],PiPDGMass,
				     Pip3[SlowPiIdx],Picosth[SlowPiIdx],Piphi[SlowPiIdx],PiPDGMass,
				     Pip3[Pi1Idx],Picosth[Pi1Idx],Piphi[Pi1Idx],PiPDGMass);
  
      dstpipidw=Xpi1pidw[XIdx]*XKpidw[XIdx]*Xpipidw[XIdx];
      dstpipidwe=(1./dstpipidw)*sqrt((Xpi1pidwe[XIdx]*Xpi1pidwe[XIdx])/(Xpi1pidw[XIdx]*Xpi1pidw[XIdx])
				     +(XKpidwe[XIdx]*XKpidwe[XIdx])/(XKpidw[XIdx]*XKpidw[XIdx])
				     +(Xpipidwe[XIdx]*Xpipidwe[XIdx])/(Xpipidw[XIdx]*Xpipidw[XIdx]));


      //-------Dstar quantities
      dstarmass=DstarMass[DstarIdx];
      dstardeltam=DstarMass[DstarIdx]-D0Mass[D0Idx];
      dstarm=dstardeltam+D0PDGMass;
      if(DstarVtxStatus[DstarIdx]==0) dstarlogvtxprob=log10(TMath::Prob(DstarChi2[DstarIdx],DstarnDof[DstarIdx]));   
      else dstarlogvtxprob=0;
      dstarcostheta=ComputeDstarAngle();
      dstarpstar=Dstarp3CM[DstarIdx];
      dstarcharge=DstarLund[DstarIdx]/abs(DstarLund[DstarIdx]);
      dstarmothcosine=ComputeDstarMotherAngle();
      dstarhelicity=ComputeDstarHelicityAngle();	
      
      //extra pion
      pi1p3=Pip3[Pi1Idx];      
      pi1charge=PiLund[Pi1Idx]/abs(PiLund[Pi1Idx]);
      pi1dedx=TRKdedxdch[Pi1trkIdx];
      pi1thetac=TRKDrcTh[Pi1trkIdx];
      pi1cosine=ComputePiDstarAngle();
      pi1mothcosine=ComputePiMotherAngle();
      pi1costheta=ComputePiAngle();
      pi1D0angle=ComputePi1D0AngleDstarFrame();


      //-------D0 quantities
      d0mass=D0Mass[D0Idx];
      if(TMath::Prob(D0Chi2[D0Idx],D0nDof[D0Idx])>0) d0logvtxprob = log10(TMath::Prob(D0Chi2[D0Idx],D0nDof[D0Idx]));
      else d0logvtxprob=0;
      d0pstar=D0p3CM[D0Idx];
      d0mode=0;
      if(D0Pi2Idx<0&&D0Pi3Idx<0) d0mode=1;
      if(D0Pi2Idx>=0&&D0Pi3Idx>=0) d0mode=2;
      if(d0mode==0)cout<<"Error d0mode=0"<<endl;
      d0charge=DstarLund[DstarIdx]/abs(DstarLund[DstarIdx]);
      d0helicity=ComputeD0HelicityAngle();
      d0dstarangle=ComputeD0DstarAngleDstarFrame();
      //slow pion
      slowpip3=Pip3[SlowPiIdx];
      slowpicharge=PiLund[SlowPiIdx]/abs(PiLund[SlowPiIdx]);
      //d0costheta
     
      //Kaon
      kp3=Kp3[KIdx];
      kcharge=KLund[KIdx]/abs(KLund[KIdx]);
     
      //Pion
      pip3=Pip3[PiIdx];
      picharge=PiLund[PiIdx]/abs(PiLund[PiIdx]);

      //other D0 pions
      d0pi2p3=0;d0pi3p3=0;d0pi2charge=0;d0pi3charge=0;
      if(d0mode==2){
	d0pi2p3=Pip3[D0Pi2Idx];      
	d0pi2charge=PiLund[D0Pi2Idx]/abs(PiLund[D0Pi2Idx]);      
	d0pi3p3=Pip3[D0Pi3Idx];      
	d0pi3charge=PiLund[D0Pi3Idx]/abs(PiLund[D0Pi3Idx]);		
      }
          	      
     

      //---------Selectors
      klh=0;pi1lhK=0;
      for(Int_t i=10;i<=14;i++){//10=notpion,11=veryloose,12=loose,13=tight,14=verytight
	if((KSelectorsMap[KtrkIdx] & (1<<i)) == (1<<i))
	  klh=i;
	if((KSelectorsMap[Pi1trkIdx] & (1<<i) ) == (1<<i))
	  pi1lhK=i;
      }
            
      pilh=0;d0pi2lh=0;d0pi3lh=0;pi1lh=0;
      for(Int_t i=2;i<=5;i++){//2=veryloose,3=loose,4=tight,5=verytight
	if((PiSelectorsMap[PitrkIdx] & (1<<i) ) == (1<<i))
	  pilh=i;
	if((PiSelectorsMap[Pi1trkIdx] & (1<<i) ) == (1<<i))
	  pi1lh=i;
	
	if(d0mode==2){
	  if((PiSelectorsMap[D0Pi2trkIdx] & (1<<i) ) == (1<<i))
	    d0pi2lh=i;
	  if((PiSelectorsMap[D0Pi3trkIdx] & (1<<i) ) == (1<<i))
	    d0pi3lh=i;
	}
      }
      
      
      //fore MC determine if candidate was truthmatched
      dstpimctrue=1;      
      dstpimclund=0;
      dstpidmres=100000;
      dstarmctrue=1;
      d0mctrue=1;      
      if(_MCorDATA==0||_MCorDATA==1){

	Int_t TrueD0MCIdx=-1;
	if(d0mode==1){
	  //truth match the D**
	  if(PiMCIdx[Pi1Idx]<=0
	     ||PiMCIdx[SlowPiIdx]<=0
	     ||KMCIdx[KIdx]<=0
	     ||PiMCIdx[PiIdx]<=0
	     )dstpimctrue=0;
	  else if(abs(mcLund[PiMCIdx[Pi1Idx]])!=211//&&abs(mcLund[PiMCIdx[Pi1Idx]])!=13)
		  ||abs(mcLund[KMCIdx[KIdx]])!=321
		  ||(abs(mcLund[PiMCIdx[SlowPiIdx]])!=211&&abs(mcLund[PiMCIdx[SlowPiIdx]])!=13)//allow for pi--> mu decays
		  ||abs(mcLund[PiMCIdx[PiIdx]])!=211//&&abs(mcLund[PiMCIdx[PiIdx]])!=13)
		  )dstpimctrue=0;	  
	  if(dstpimctrue==1){
	    //check the K-pi comes from a D0
	    if(abs(mcLund[mothIdx[KMCIdx[KIdx]]])!=421
	       ||mothIdx[KMCIdx[KIdx]]!=mothIdx[PiMCIdx[PiIdx]]
	       ){dstpimctrue=0;}
	    //check the D0 and slowPi come from D*+
	    if(abs(mcLund[mothIdx[mothIdx[KMCIdx[KIdx]]]])!=413
	       ||(abs(mcLund[PiMCIdx[SlowPiIdx]])==211 ? mothIdx[mothIdx[KMCIdx[KIdx]]]!=mothIdx[PiMCIdx[SlowPiIdx]]
		  : mothIdx[mothIdx[KMCIdx[KIdx]]]!=mothIdx[mothIdx[PiMCIdx[SlowPiIdx]]])
	       ){dstpimctrue=0;}
	    //Check the D*+ and the extra Pion come from D**
	    if((abs(mcLund[mothIdx[mothIdx[mothIdx[KMCIdx[KIdx]]]]])!=10423
		&&abs(mcLund[mothIdx[mothIdx[mothIdx[KMCIdx[KIdx]]]]])!=425
		&&abs(mcLund[mothIdx[mothIdx[mothIdx[KMCIdx[KIdx]]]]])!=20423)
	       ||mothIdx[mothIdx[mothIdx[KMCIdx[KIdx]]]]!=mothIdx[PiMCIdx[Pi1Idx]]
	       )dstpimctrue=0;	
	    
	  }

	  //truth-match D0
	  if(KMCIdx[KIdx]<=0 ||PiMCIdx[PiIdx]<=0 ) d0mctrue=0;
	  else if(abs(mcLund[KMCIdx[KIdx]])!=321 || abs(mcLund[PiMCIdx[PiIdx]])!=211 ) d0mctrue=0;	  
	  if(d0mctrue==1){
	    TrueD0MCIdx=mothIdx[KMCIdx[KIdx]];
	    if(abs(mcLund[TrueD0MCIdx])!=421)TrueD0MCIdx=mothIdx[mothIdx[KMCIdx[KIdx]]];
	    if(abs(mcLund[TrueD0MCIdx])!=421)TrueD0MCIdx=mothIdx[mothIdx[mothIdx[KMCIdx[KIdx]]]];
	    if(abs(mcLund[TrueD0MCIdx])!=421 || mothIdx[KMCIdx[KIdx]]!=mothIdx[PiMCIdx[PiIdx]]) d0mctrue=0;
	  }
	  
	}
	if(d0mode==2){

	  //truth match the D**
	  if(PiMCIdx[Pi1Idx]<=0
	     ||PiMCIdx[SlowPiIdx]<=0
	     ||KMCIdx[KIdx]<=0
	     ||PiMCIdx[PiIdx]<=0
	     ||PiMCIdx[D0Pi2Idx]<=0
	     ||PiMCIdx[D0Pi3Idx]<=0
	     )dstpimctrue=0;
	  //check if all tracks come from the D**
	  if(dstpimctrue==1){//D** will be truth-matched if all tracks have the generated D** parent
	    dstpimctrue=0;
	    Int_t TrueCandIdx[6]={PiMCIdx[Pi1Idx],
				  PiMCIdx[SlowPiIdx],
				  KMCIdx[KIdx],
				  PiMCIdx[PiIdx],
				  PiMCIdx[D0Pi2Idx],
				  PiMCIdx[D0Pi3Idx]};

	    for(Int_t m=1;m<mcLen;m++)
	      if(abs(mcLund[m])==10423 || abs(mcLund[m])==425 || abs(mcLund[m])==20423){		
		Int_t nfound=0;
		for(Int_t c=0;c<6;c++){
		  Int_t ParentIdx=mothIdx[TrueCandIdx[c]];
		  while(ParentIdx>2){
		    if(ParentIdx==m)nfound++;
		    ParentIdx=mothIdx[ParentIdx];
		  }
		}
		if(nfound==6)dstpimctrue=1;
	      }

	    
	  }

/*
	  else if(abs(mcLund[PiMCIdx[Pi1Idx]])!=211
		  ||abs(mcLund[KMCIdx[KIdx]])!=321
		  ||(abs(mcLund[PiMCIdx[SlowPiIdx]])!=211&&abs(mcLund[PiMCIdx[SlowPiIdx]])!=13)//allow for pi--> mu decays
		  ||abs(mcLund[PiMCIdx[PiIdx]])!=211
		  ||(abs(mcLund[PiMCIdx[D0Pi2Idx]])!=211&&abs(mcLund[PiMCIdx[D0Pi2Idx]])!=13)
		  ||(abs(mcLund[PiMCIdx[D0Pi3Idx]])!=211&&abs(mcLund[PiMCIdx[D0Pi3Idx]])!=13)
		  )dstpimctrue=0;
		  
	  Int_t TruePiIdx[5]={-1,-1,-1,-1,-1};//looking for 5 pions in the Decay D**->D*pi,D*->D0pi, D0->K3pi
	  if(dstpimctrue==1){
	    //find the D** tracks in the mc Block	   
	    for(Int_t m=1;m<mcLen;m++){
	      if(abs(mcLund[m])==10423 || abs(mcLund[m])==425 || abs(mcLund[m])==20423){
		Int_t TrueDstarIdx=-1;
		Int_t TrueD0Idx=-1;
		if(abs(mcLund[dauIdx[m]])==413) TrueDstarIdx=dauIdx[m];
		if(abs(mcLund[dauIdx[m]+1])==413) TrueDstarIdx=dauIdx[m]+1;
		if(dauLen[m]==3)if(abs(mcLund[dauIdx[m]+2])==413) TrueDstarIdx=dauIdx[m]+2;
		if(abs(mcLund[dauIdx[m]])==211) TruePiIdx[0]=dauIdx[m];
		if(abs(mcLund[dauIdx[m]+1])==211) TruePiIdx[0]=dauIdx[m]+1;
		if(dauLen[m]==3)if(abs(mcLund[dauIdx[m]+2])==211) TruePiIdx[0]=dauIdx[m]+2;
		  
		if(TrueDstarIdx>0){		
		  if(abs(mcLund[dauIdx[TrueDstarIdx]])==421) TrueD0Idx=dauIdx[TrueDstarIdx];
		  if(abs(mcLund[dauIdx[TrueDstarIdx]+1])==421) TrueD0Idx=dauIdx[TrueDstarIdx]+1;
		  if(dauLen[TrueDstarIdx]==3)if(abs(mcLund[dauIdx[TrueDstarIdx]+2])==421) TrueD0Idx=dauIdx[TrueDstarIdx]+2;
		  if(abs(mcLund[dauIdx[TrueDstarIdx]])==211) TruePiIdx[1]=dauIdx[TrueDstarIdx];
		  if(abs(mcLund[dauIdx[TrueDstarIdx]+1])==211) TruePiIdx[1]=dauIdx[TrueDstarIdx]+1;		  
		  if(dauLen[TrueDstarIdx]==3)if(abs(mcLund[dauIdx[TrueDstarIdx]+2])==211) TruePiIdx[1]=dauIdx[TrueDstarIdx]+2;

		  if(TrueD0Idx>0){
		    Int_t countpi=2;
		    for(Int_t d1=0;d1<dauLen[TrueD0Idx]&&countpi<5;d1++){
		      if(abs(mcLund[dauIdx[TrueD0Idx]+d1])==211) 
			TruePiIdx[countpi++]=dauIdx[TrueD0Idx]+d1;
		      else for(Int_t d2=0;d2<dauLen[dauIdx[TrueD0Idx]+d1]&&countpi<5;d2++){
			if(abs(mcLund[dauIdx[dauIdx[TrueD0Idx]+d1]+d2])==211)
			  TruePiIdx[countpi++]=dauIdx[dauIdx[TrueD0Idx]+d1]+d2;		  
			else for(Int_t d3=0;d3<dauLen[dauIdx[dauIdx[TrueD0Idx]+d1]+d2]&&countpi<5;d3++){
			  if(abs(mcLund[dauIdx[dauIdx[dauIdx[TrueD0Idx]+d1]+d2]+d3])==211)
			    TruePiIdx[countpi++]=dauIdx[dauIdx[dauIdx[TrueD0Idx]+d1]+d2]+d3;		  
			}
		      }
		    }

		  }
		}	

	      }	      
	    }

	    //Truth-match the D**
	    dstpimctrue=0;
	    for(Int_t p=0;p<5;p++)
	      if(PiMCIdx[Pi1Idx]==TruePiIdx[p])dstpimctrue=1;
	    if(dstpimctrue==1){
	      dstpimctrue=0;
	      for(Int_t p=0;p<5;p++)
		if(abs(mcLund[PiMCIdx[SlowPiIdx]])==211 ? TruePiIdx[p]==PiMCIdx[SlowPiIdx] 
		                                        : TruePiIdx[p]==mothIdx[PiMCIdx[SlowPiIdx]])dstpimctrue=1;
	      
	      if(dstpimctrue==1){
		dstpimctrue=0;
		for(Int_t p=0;p<5;p++)
		  if(PiMCIdx[PiIdx]==TruePiIdx[p])dstpimctrue=1;
		if(dstpimctrue==1){
		  dstpimctrue=0;
		  for(Int_t p=0;p<5;p++)
		    if(abs(mcLund[PiMCIdx[D0Pi2Idx]])==211 ? TruePiIdx[p]==PiMCIdx[D0Pi2Idx] 
		                                           : TruePiIdx[p]==mothIdx[PiMCIdx[D0Pi2Idx]])dstpimctrue=1;
		  if(dstpimctrue==1){
		    dstpimctrue=0;
		    for(Int_t p=0;p<5;p++)
		      if(abs(mcLund[PiMCIdx[D0Pi3Idx]])==211 ? TruePiIdx[p]==PiMCIdx[D0Pi3Idx] 
		                                             : TruePiIdx[p]==mothIdx[PiMCIdx[D0Pi3Idx]])dstpimctrue=1;
		  }
		}
	      }
	    }
	   	  
	  }

*/



	  //truth-match D0
	  if(KMCIdx[KIdx]<=0 || PiMCIdx[PiIdx]<=0 || PiMCIdx[D0Pi2Idx]<=0 || PiMCIdx[D0Pi3Idx]<=0) d0mctrue=0;
	  else if(abs(mcLund[KMCIdx[KIdx]])!=321
		  || abs(mcLund[PiMCIdx[PiIdx]])!=211
		  || abs(mcLund[PiMCIdx[D0Pi2Idx]])!=211
		  || abs(mcLund[PiMCIdx[D0Pi3Idx]])!=211
		  ) d0mctrue=0;	  
	  if(d0mctrue==1){
	    TrueD0MCIdx=mothIdx[KMCIdx[KIdx]];
	    if(abs(mcLund[TrueD0MCIdx])!=421)TrueD0MCIdx=mothIdx[mothIdx[KMCIdx[KIdx]]];
	    if(abs(mcLund[TrueD0MCIdx])!=421)TrueD0MCIdx=mothIdx[mothIdx[mothIdx[KMCIdx[KIdx]]]];
	    if(abs(mcLund[TrueD0MCIdx])!=421 
	       || (TrueD0MCIdx!=mothIdx[KMCIdx[KIdx]] 
		   && TrueD0MCIdx != mothIdx[mothIdx[KMCIdx[KIdx]]] 
		   && TrueD0MCIdx != mothIdx[mothIdx[mothIdx[KMCIdx[KIdx]]]] )
	       || (TrueD0MCIdx!=mothIdx[PiMCIdx[PiIdx]] 
		   && TrueD0MCIdx != mothIdx[mothIdx[PiMCIdx[PiIdx]]] 
		   && TrueD0MCIdx != mothIdx[mothIdx[mothIdx[PiMCIdx[PiIdx]]]] )
	       || (TrueD0MCIdx!=mothIdx[PiMCIdx[D0Pi2Idx]] 
		   && TrueD0MCIdx != mothIdx[mothIdx[PiMCIdx[D0Pi2Idx]]] 
		   && TrueD0MCIdx != mothIdx[mothIdx[mothIdx[PiMCIdx[D0Pi2Idx]]]] )
	       || (TrueD0MCIdx!=mothIdx[PiMCIdx[D0Pi3Idx]] 
		   && TrueD0MCIdx != mothIdx[mothIdx[PiMCIdx[D0Pi3Idx]]] 
		   && TrueD0MCIdx != mothIdx[mothIdx[mothIdx[PiMCIdx[D0Pi3Idx]]]] )
	       ) d0mctrue=0;
	  }
	}


	//truth-match D*+
	if(d0mctrue==1){
	  if(PiMCIdx[SlowPiIdx]<=0 )dstarmctrue=0;
	  else if(abs(mcLund[PiMCIdx[SlowPiIdx]])!=211 && abs(mcLund[PiMCIdx[SlowPiIdx]])!=13 )
	    dstarmctrue=0;
	  if(dstarmctrue==1){
	    if(abs(mcLund[mothIdx[TrueD0MCIdx]])!=413)dstarmctrue=0;
	    if(abs(mcLund[PiMCIdx[SlowPiIdx]])==211 ? mothIdx[TrueD0MCIdx] != mothIdx[PiMCIdx[SlowPiIdx]]
	       : mothIdx[TrueD0MCIdx] != mothIdx[mothIdx[PiMCIdx[SlowPiIdx]]]
	       )dstarmctrue=0;
	  }
	}else dstarmctrue=0;
	
	//mass resolution
	if(dstpimctrue){  
	  for(Int_t m=1;m<mcLen;m++)
	    if(abs(mcLund[m])==10423 || abs(mcLund[m])==425 || abs(mcLund[m])==20423){
	      dstpimclund=mcLund[m];	    
	      dstpidmres=dstpimdm-mcmass[m];
	    }
	}
	  
      }

               
      //apply cuts
      if(cutcharge==1)if(abs(dstpicharge)!=1)continue;
      if(cutcharge==2)if(abs(dstpicharge)!=2)continue;
      if(cutdmass)if(fabs(d0mass-D0PDGMass)>D0MassCut)continue;
      if(cutd0mode==1)if(d0mode!=1)continue;
      if(cutd0mode==2)if(d0mode!=2)continue;
      if(cutdeltam==1)if(fabs(dstardeltam-DstarDMMean)>DstarDMCut) continue;
      if(cutdeltam==2)if(fabs(dstardeltam-DstarDMMean)<DstarDMCut||fabs(dstardeltam-DstarDMMean)>2*DstarDMCut) continue;      
      if(cutpstar)if(dstpipstar<PstarCut)continue;
      if(cutdcos)if(-dstarcostheta<CosCut)continue;
     
      if(pi1lh<2||klh<13||pilh<4)continue;//apply PID cuts

      
      //remove multiple reconstructions
//       NRecosThisEvent++;
//       if(NRecosThisEvent>1)continue;      

	
      Ntuple->Fill();	              
               
      RecoXCounterTotal++;
           
   
    }//X loop    
    if(NRecosThisEvent>0){
      AvgRecosPerEvent+=NRecosThisEvent;
      NGoodEvents++;
    }
    
      
    ///-----------------------------
    ///Fill the Generated Quantities
    ///-----------------------------
    if(_MCorDATA==0){    
      MCXCounterPerEvent=0; 
      Int_t mcid=-1;
      Bool_t goodcand;
      while(mcid<mcLen&&mcid<MAXMCLEN){
	mcid++;
       
	goodcand=false;
	if(abs(mcLund[mcid])==10423||abs(mcLund[mcid])==425)goodcand=true;
	if(goodcand&&abs(mcLund[dauIdx[mcid]])!=MYDSTARLUND)goodcand=false;//D*2460 also decays to D0 in ccbar

	if(goodcand){
	  MCXCounterPerEvent++; 
	  MCXCounterTotal++;
	 

	  //calculate deltaM :  mcmass[dauIdx[mcid]]=first daughter; mcmass[dauIdx[mcid] + 1] = second daughter ...	 
	  HMCXMass->Fill(mcmass[mcid]);
	  HMCXMassDiff->Fill(mcmass[mcid]-2.4222);
	  H3MCPvsThetaVsDstAngle->Fill(mcp3CM[mcid],mccosthCM[mcid],ComputeMCDstarAngle(mcid));	  
	  H3MCPvsMassVsDstAngle->Fill(mcp3CM[mcid],mcmass[mcid],ComputeMCDstarAngle(mcid));
	  H3MCPvsMassVsHelicity->Fill(mcp3CM[mcid],mcmass[mcid],ComputeMCDstarHelicity(mcid));
	  H2MCPvsMass->Fill(mcp3CM[mcid],mcmass[mcid]);
	  H2MCPvsMassFine->Fill(mcp3CM[mcid],mcmass[mcid]);

	  //
	  HMCXp3CM->Fill(mcp3CM[mcid]); 
	  HMCXcosthCM->Fill(mccosthCM[mcid]); 	
	  H2MCXCMPvsTheta->Fill(mcp3CM[mcid],mccosthCM[mcid]);
	  //calculate the angles
	  HMCDstarAngle->Fill(ComputeMCDstarAngle(mcid));
	  HMCDstarHelicity->Fill(ComputeMCDstarHelicity(mcid));
	  //


	  	    
	}
	
	//CalculateDstRandPiGenMassDist(mcid);      
      }
      HMCNX->Fill(MCXCounterPerEvent);
    }

  }

  //print summary
  cout<<"--------Summary-------"<<endl;
  cout<<"Total events = "<<eventid<<endl;
  cout<<"Total Generated="<<MCXCounterTotal<<" Reconstructed="<<RecoXCounterTotal<<endl;
  cout<<"Total candidates removed "<<RemovedCands<<" ( "<<(int)(100*RemovedCands/InitialdCands + .5)<<"%)"<<endl;

  cout<<"Average Reconstructions per event= "<<AvgRecosPerEvent/NGoodEvents<<endl;
  cout<<"--------End Summary---"<<endl;

  ////Save the Ntuple and histograms
  cout<<"Going to save the ntuple and histograms"<<endl; 

  ///////////Should not have to call TTree->Write(), TFile->Write() will do it
  //if(Ntuple->Write()){ cout<<"Ntuple written."<<endl;}
  if(Ntuple->AutoSave("Overwrite")){ cout<<"Ntuple written."<<endl;}//turns out the TTree does AutoSave() automatically when tree size gets bigger than 10Mb (default). AutoSave() deletes the previous entry. If one calls  Write() it does not delete previous entry and one gets 2 ntuples in root file. option "Overwrite" deletes then saves.
  else{ cout<<"Failed to write ntuple"<<endl;return 0;}
  delete  Ntuple;   

  if(_MCorDATA==0){    
    if(HMCXMass->Write()&&
       HMCXp3CM->Write()&&
       HMCXcosthCM->Write()&&     
       H2MCXCMPvsTheta->Write()&&     
       HMCNX->Write()&&
       HMCDstarHelicity->Write()&&
       HMCDstarAngle->Write()&&
       H3MCPvsThetaVsDstAngle->Write()&&    
       H3MCPvsMassVsDstAngle->Write()&&
       H3MCPvsMassVsHelicity->Write()&&
       H2MCPvsMass->Write()&&
       H2MCPvsMassFine->Write()&&
       HMCXMassDiff->Write()
       ){ 
      cout<<" Histograms have been written"<<endl;
      
       delete HMCXMass;
       delete HMCXp3CM;
       delete HMCXcosthCM;     
       delete H2MCXCMPvsTheta;     
       delete HMCNX;
       delete HMCDstarHelicity;
       delete HMCDstarAngle;
       delete H3MCPvsThetaVsDstAngle;    
       delete H3MCPvsMassVsDstAngle;
       delete H3MCPvsMassVsHelicity;
       delete H2MCPvsMass;
       delete HMCXMassDiff;

    } else{
      cout<<"Failed to write Histos"<<endl;return 0;
    }
  
  }
  OutPutFile->ls();
  //OutPutFile->Write();//dont do Write() that will write all objects currently created inside file.
  OutPutFile->Close(); //this will delete all objects created inside the file if they were not written  
  cout<<OutPutFile->GetName()<<" has been closed."<<endl;

  delete OutPutFile;

  return 1;
}


// 	    d0mctrue=0;
// 	    for(Int_t p=2;p<5;p++)
// 	      if(PiMCIdx[PiIdx]==TruePiIdx[p])d0mctrue=1;
// 	    if(d0mctrue==1){
// 	      d0mctrue=0;
// 	      for(Int_t p=2;p<5;p++)
// 		if(abs(mcLund[PiMCIdx[D0Pi2Idx]])==211 ? TruePiIdx[p]==PiMCIdx[D0Pi2Idx] 
// 		   : TruePiIdx[p]==mothIdx[PiMCIdx[D0Pi2Idx]])d0mctrue=1;
// 	      if(d0mctrue==1){
// 		d0mctrue=0;
// 		for(Int_t p=0;p<5;p++)
// 		  if(abs(mcLund[PiMCIdx[D0Pi3Idx]])==211 ? TruePiIdx[p]==PiMCIdx[D0Pi3Idx] 
// 		     : TruePiIdx[p]==mothIdx[PiMCIdx[D0Pi3Idx]])d0mctrue=1;
// 	      }
// 	    }
// 	    if(abs(mcLund[mothIdx[KMCIdx[KIdx]]])!=421
// 	       ||abs(mcLund[mothIdx[mothIdx[KMCIdx[KIdx]]]])!=421
// 	       ||abs(mcLund[mothIdx[mothIdx[mothIdx[KMCIdx[KIdx]]]]])!=421
// 	       ){d0mctrue=0;}

	    
// 	    //truth-match the D*
// 	    dstarmctrue=0;
// 	    for(Int_t p=1;p<5;p++)
// 	      if(abs(mcLund[PiMCIdx[SlowPiIdx]])==211 ? TruePiIdx[p]==PiMCIdx[SlowPiIdx] 
// 		 : TruePiIdx[p]==mothIdx[PiMCIdx[SlowPiIdx]])dstarmctrue=1;
	      
// 	    if(dstarmctrue==1){
// 	      dstarmctrue=0;
// 	      for(Int_t p=1;p<5;p++)
// 		if(PiMCIdx[PiIdx]==TruePiIdx[p])dstarmctrue=1;
// 	      if(dstarmctrue==1){
// 		dstarmctrue=0;
// 		for(Int_t p=1;p<5;p++)
// 		  if(abs(mcLund[PiMCIdx[D0Pi2Idx]])==211 ? TruePiIdx[p]==PiMCIdx[D0Pi2Idx] 
// 		     : TruePiIdx[p]==mothIdx[PiMCIdx[D0Pi2Idx]])dstarmctrue=1;
// 		if(dstarmctrue==1){
// 		  dstarmctrue=0;
// 		  for(Int_t p=1;p<5;p++)
// 		    if(abs(mcLund[PiMCIdx[D0Pi3Idx]])==211 ? TruePiIdx[p]==PiMCIdx[D0Pi3Idx] 
// 		       : TruePiIdx[p]==mothIdx[PiMCIdx[D0Pi3Idx]])dstarmctrue=1;
// 		}
// 	      }
// 	    }
// 	    if(abs(mcLund[mothIdx[KMCIdx[KIdx]]])!=421
// 	       ||abs(mcLund[mothIdx[mothIdx[KMCIdx[KIdx]]]])!=421
// 	       ||abs(mcLund[mothIdx[mothIdx[mothIdx[KMCIdx[KIdx]]]]])!=421
// 	       ){dstarmctrue=0;}	   

//      dstpimctruebad=1;
//       dstarmctrue=1;
//       d0mctrue=1;
//       pimctrue=1;
//       slowpimctrue=1;
//       kmctrue=1;
//       pi1mctrue=1;
//       d0pi2mctrue=1;  
//       d0pi3mctrue=1;

//       dstpicosstarres=10000;
//       dstpiphistarres=10000;
//       dstarmothcosineres=10000;
//       pi1mothcosineres=10000;
//       dstarcosthetares=10000;
//       pi1costhetares=10000;

// 	dstpimctrue=(XMyMCIdx[XIdx]>-1)&&(XMyMCIdx[XIdx]<MAXMCLEN);	
// 	dstpimctruebad=(XMCIdx[XIdx]>-1);	
// 	dstarmctrue=(DstarMCIdx[DstarIdx]>-1);
// 	d0mctrue=(D0MCIdx[D0Idx]>-1);
// 	pimctrue=(PiMCIdx[PiIdx]>-1);
// 	slowpimctrue=(PiMCIdx[SlowPiIdx]>-1);
// 	kmctrue=(KMCIdx[KIdx]>-1);
// 	pi1mctrue=(PiMCIdx[Pi1Idx]>-1);
// 	if(d0mode==2){
// 	  d0pi2mctrue=(PiMCIdx[D0Pi2Idx]>-1);
// 	  d0pi3mctrue=(PiMCIdx[D0Pi3Idx]>-1);
// 	}

// 	  dstpicosstarres=XcosthCM[XIdx]-mccosthCM[Xmcidx];
// 	  dstpiphistarres=XphiCM[XIdx]-mcphiCM[Xmcidx];

// 	  Dstarmcidx=dauIdx[Xmcidx];
// 	  if(abs(mcLund[Dstarmcidx])!=myDstarLund || Dstarmcidx<0 ||Dstarmcidx>MAXMCLEN)cout<<"Bad MC D* Idx"<<endl;
// 	  else dstarcosthetares=dstarcostheta-ComputeGenDstarAngle();
// 	  //pi1costhetares=pi1costheta-ComputeGenPiAngle();
// 	  //dstarmothcosineres=dstarmothcosine-ComputeGenDstarMotherAngle();
// 	  //pi1mothcosineres=pi1mothcosine-ComputeGenPiMotherAngle();

// 	  D0mcidx=dauIdx[Dstarmcidx];
// 	  if(abs(mcLund[D0mcidx])!=myD0Lund || D0mcidx<0 || D0mcidx>MAXMCLEN)cout<<"Bad MC D0 Idx"<<endl;
// 	  else dstarhelicityres=dstarhelicity-ComputeGenDstarHelicity();


//     ///--------------------------------------------------
//     /////do a preloop to determine the best candidate
//     //----------------------------------------------
//     bestcandid=-1;  
//     bestcandvar=0; 
//     for(XIdx=0;XIdx<nX;XIdx++){
//       ////////////cut out the unconverged fits
//       if(XVtxStatus[XIdx]!=0)continue;               
//       ///determine the indexes for each particle
//       DstarIdx=Xd1Idx[XIdx];
        
//       dstpipstar=Xp3CM[XIdx]; 
//       if(TMath::Prob(XChi2[XIdx],XnDof[XIdx])>0)
// 	dstpilogvtxprob=log10(TMath::Prob(XChi2[XIdx],XnDof[XIdx]));
//       else dstpilogvtxprob=-100; 
//       dstarcostheta=ComputeDstarAngle();

//       //multiply cosD*, p* and vertex prob
//       //cout<<eventid<<" "<<bestcandvar<<" "<<((1.-dstarcostheta)/2.)*(dstpipstar/5.)*(dstpilogvtxprob+3.)/3.<<endl;
//       if(bestcandvar <  ((1.-dstarcostheta)/2.)*(dstpipstar/5.)*(dstpilogvtxprob+3.)/3. ){
// 	bestcandvar=((1.-dstarcostheta)/2.)*(dstpipstar/5.)*(dstpilogvtxprob+3.)/3.;
// 	bestcandid=XIdx;
//       }
      
//     }

//     //   cout<<eventid<<" "<<bestcandid<<endl;

  //     //best candidate
//       if(bestcandid==XIdx)  dstpibestcand=1;
//       else dstpibestcand=0;
   

// Int_t DstPiAnalysis::CleanNtuple(){
//   cout<<"going to clean ntuple"<<endl;
//   if(AlreadyTriedClean){cout<<"Already tried to clean"<<endl; return 0;}

//   if(!OpenReducedNtuple(1)){cout<<" no reduced ntuple"<<endl; return 0;}
  
//   TTree* ReducedNtupleCopy=ReducedNtuple->CopyTree(database.GetCut(1,100),"",1000000000,0); 
//   TFile temp(_OutputDir+"/ReducedClean.root","recreate"); 
//   ReducedNtupleCopy->SetName("CleanNtuple");
//   ReducedNtupleCopy->Write();
//   cout<<"Entries in clean ntuple "<<ReducedNtupleCopy->GetEntries()<<endl;
//   cout<<" After------"<<endl;
//   temp.ls();
//   temp.Close();

//   CloseReducedNtuple();
//   //OutPutFile->Close();
  
//   AlreadyTriedClean=1;
 
//   return 1;
// }

/*

Int_t DstPiAnalysis::OpenReducedNtuple(Int_t WhichOne){
  
  cout<<"Going to open reduced ntuple."<<endl; 

  if(WhichOne==1){//Full Data
    cout<<"Will return Full data Ntuple"<<endl;
    ReducedNtuple=new TChain("Ntuple","Ntuple");
    
 
    if((OutPutFile=new TFile(_OutputDir+"/Reduced.root","read")))
      if(!OutPutFile->IsZombie()){
	ReducedNtuple->Add(_OutputDir+"/Reduced.root");
	cout<<"added "<<_OutputDir+"/Reduced.root"<<endl;
      }
    delete OutPutFile;
    
    if((OutPutFile=new TFile(_OutputDir+"/Reduced_1.root","read")))
      if(!OutPutFile->IsZombie()){
	ReducedNtuple->Add(_OutputDir+"/Reduced_1.root");
	cout<<"added "<<_OutputDir+"/Reduced_1.root"<<endl;
      }
    delete OutPutFile;
    
    if((OutPutFile=new TFile(_OutputDir+"/Reduced_2.root","read")))
      if(!OutPutFile->IsZombie()){
	ReducedNtuple->Add(_OutputDir+"/Reduced_2.root");
	cout<<"added "<<_OutputDir+"/Reduced_2.root"<<endl;
      }
    delete OutPutFile;
    
    if((OutPutFile=new TFile(_OutputDir+"/Reduced_3.root","read")))
      if(!OutPutFile->IsZombie()){
	ReducedNtuple->Add(_OutputDir+"/Reduced_3.root");
	cout<<"added "<<_OutputDir+"/Reduced_3.root"<<endl;
      }
    delete OutPutFile;
    
    if((OutPutFile=new TFile(_OutputDir+"/Reduced_4.root","read")))
      if(!OutPutFile->IsZombie()){
	ReducedNtuple->Add(_OutputDir+"/Reduced_4.root");
	cout<<"added "<<_OutputDir+"/Reduced_4.root"<<endl;
      }
    delete OutPutFile;
         
  }
  if(WhichOne==2){//Clean Ntuple
    cout<<"Will return Clean Ntuple"<<endl;    
    ReducedNtuple=new TChain("CleanNtuple","CleanNtuple");
    
    //
    if((OutPutFile=new TFile(_OutputDir+"/ReducedClean.root","read")))
      if(!OutPutFile->IsZombie())
	if(!(ReducedNtuple=(TChain*)OutPutFile->Get("CleanNtuple"))){//NOTE: for merged D2420andD2460 CleanNtuple is a TChain, otherwise it is a TTree but ReducedNtuple is TChain!, else can not read D2420andD2460
	  cout<<" No CleanNtuple"<<endl; return 0;
	}
    
    //ReducedNtuple->Add(_OutputDir+"/ReducedClean.root");         
    //delete OutPutFile;  
    
  }
  
  if(!ReducedNtuple){cout<<" No ReducedNtuple, bad option"<<endl; return 0;}

  cout<<"Ntuple Extracted: "<<ReducedNtuple<<" "<<ReducedNtuple->GetName()<<"  With "<<ReducedNtuple->GetEntries()<<" Entries"<<endl;
  
  SetReducedNtupleBranches();
  
  gROOT->cd();//REMEMBER TO DO THIS WHEN OPENING A FILE otherwise new objects are created in that file
  
  return 1;
}
*/


Int_t DstPiAnalysis::OpenReducedNtuple(){
  
  cout<<"Going to open reduced ntuple."<<endl;
  
  if((OutPutFile=new TFile(_OutputDir+"/Ntuple.root","read")))
    if(!OutPutFile->IsZombie()){
      ReducedNtuple=(TTree*)OutPutFile->Get("Ntuple");
    }
  gROOT->cd();
  
  
  if(!ReducedNtuple){cout<<" No ReducedNtuple"<<endl; return 0;}
  else
    cout<<"Ntuple Extracted: "<<ReducedNtuple<<" "<<ReducedNtuple->GetName()<<"  With "<<ReducedNtuple->GetEntries()<<" Entries"<<endl;
  
  gROOT->cd();//******REMEMBER TO DO THIS WHEN OPENING A FILE otherwise new objects are created in that file******//
  
  return 1;
}

  
Int_t DstPiAnalysis::CloseReducedNtuple(){
  cout<<"Going to close the file."<<endl;
  delete OutPutFile;   
  return 1;
}
 
Int_t DstPiAnalysis::OpenReducedFile(){
  
  cout<<"Going to open Reduced.root "<<endl; 
  
  if(!(ReducedFile=new TFile(_OutputDir+"/Reduced.root","read"))){
    cout<<"Unable to open reduced file"<<endl;
    return 0;
    
  }
  gROOT->cd(); 

  return 1;
}

Int_t DstPiAnalysis::CloseReducedFile(){
  cout<<"Going to close the file."<<endl;
 
  ReducedFile->Close();

  delete ReducedFile;
  
  return 1;
}


Float_t DstPiAnalysis::ComputeDstarAngle(){
  //Angle of the D* wrt to the X direction in the X frame

  TVector3 Xp3vec(Xp3CM[XIdx]*sin(acos(XcosthCM[XIdx]))*cos(XphiCM[XIdx]),
		  Xp3CM[XIdx]*sin(acos(XcosthCM[XIdx]))*sin(XphiCM[XIdx]),
		  Xp3CM[XIdx]*XcosthCM[XIdx]);
  
  Float_t XEnergy=sqrt(Xp3vec*Xp3vec + XMass[XIdx]*XMass[XIdx]);
  TVector3 XVelvec=(1/XEnergy)*Xp3vec;

  TVector3 Dstarp3vec(Dstarp3CM[DstarIdx]*sin(acos(DstarcosthCM[DstarIdx]))*cos(DstarphiCM[DstarIdx]),
		      Dstarp3CM[DstarIdx]*sin(acos(DstarcosthCM[DstarIdx]))*sin(DstarphiCM[DstarIdx]),
		      Dstarp3CM[DstarIdx]*DstarcosthCM[DstarIdx]); 
  
  TLorentzVector Dstarp4(Dstarp3vec,sqrt(Dstarp3vec*Dstarp3vec+DstarMass[DstarIdx]*DstarMass[DstarIdx]));

  //boost the dstar momentum into rest frame of X
  Dstarp4.Boost(-XVelvec);//watch the minus sign
  TVector3 DstarBoostedp3vec=Dstarp4.Vect();

  //dot product of boosted dstar direction with X direction 
  return DstarBoostedp3vec.Unit()*Xp3vec.Unit();
  
}
Float_t DstPiAnalysis::ComputeGenDstarAngle(){
  //Generated angle of the D* wrt the X direction in the X frame
  TVector3 Xp3vec(mcp3CM[Xmcidx]*sin(acos(mccosthCM[Xmcidx]))*cos(mcphiCM[Xmcidx]),
		  mcp3CM[Xmcidx]*sin(acos(mccosthCM[Xmcidx]))*sin(mcphiCM[Xmcidx]),
		  mcp3CM[Xmcidx]*mccosthCM[Xmcidx]);
  
  Float_t XEnergy=sqrt(Xp3vec*Xp3vec +mcmass[Xmcidx]*mcmass[Xmcidx]);
  TVector3 XVelvec=(1/XEnergy)*Xp3vec;

  TVector3 Dstarp3vec(mcp3CM[Dstarmcidx]*sin(acos(mccosthCM[Dstarmcidx]))*cos(mcphiCM[Dstarmcidx]),
		      mcp3CM[Dstarmcidx]*sin(acos(mccosthCM[Dstarmcidx]))*sin(mcphiCM[Dstarmcidx]),
		      mcp3CM[Dstarmcidx]*mccosthCM[Dstarmcidx]); 
  
  TLorentzVector Dstarp4(Dstarp3vec,sqrt(Dstarp3vec*Dstarp3vec+mcmass[Dstarmcidx]*mcmass[Dstarmcidx]));

  //boost the dstar momentum into rest frame of X
  Dstarp4.Boost(-XVelvec);//watch the minus sign
  TVector3 DstarBoostedp3vec=Dstarp4.Vect();

  //dot product of boosted dstar direction with X direction 
  return DstarBoostedp3vec.Unit()*Xp3vec.Unit();

}
Float_t DstPiAnalysis::ComputeMCDstarAngle(Int_t mcid){
  //Generated angle of the D* wrt the X direction in the X frame
  TVector3 Xp3vec(mcp3CM[mcid]*sin(acos(mccosthCM[mcid]))*cos(mcphiCM[mcid]),
		  mcp3CM[mcid]*sin(acos(mccosthCM[mcid]))*sin(mcphiCM[mcid]),
		  mcp3CM[mcid]*mccosthCM[mcid]);
  
  Float_t XEnergy=sqrt(Xp3vec*Xp3vec +mcmass[mcid]*mcmass[mcid]);
  TVector3 XVelvec=(1/XEnergy)*Xp3vec;

  Int_t dstarid=dauIdx[mcid];
  
  TVector3 Dstarp3vec(mcp3CM[dstarid]*sin(acos(mccosthCM[dstarid]))*cos(mcphiCM[dstarid]),
		      mcp3CM[dstarid]*sin(acos(mccosthCM[dstarid]))*sin(mcphiCM[dstarid]),
		      mcp3CM[dstarid]*mccosthCM[dstarid]); 
  
  TLorentzVector Dstarp4(Dstarp3vec,sqrt(Dstarp3vec*Dstarp3vec+mcmass[dstarid]*mcmass[dstarid]));

  //boost the dstar momentum into rest frame of X
  Dstarp4.Boost(-XVelvec);
  TVector3 DstarBoostedp3vec=Dstarp4.Vect();

  //dot product of boosted dstar direction with X direction 
  return DstarBoostedp3vec.Unit()*Xp3vec.Unit();

}


Float_t DstPiAnalysis::ComputePiAngle(){
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

Float_t DstPiAnalysis::ComputeGenPiAngle(){
  //generated angle of the extra Pion wrt to the X direction in the  X frame.
  TVector3 Xp3vec(mcp3CM[Xmcidx]*sin(acos(mccosthCM[Xmcidx]))*cos(mcphiCM[Xmcidx]),
		  mcp3CM[Xmcidx]*sin(acos(mccosthCM[Xmcidx]))*sin(mcphiCM[Xmcidx]),
		  mcp3CM[Xmcidx]*mccosthCM[Xmcidx]);
  
  Float_t XEnergy=sqrt(Xp3vec*Xp3vec + mcmass[Xmcidx]*mcmass[Xmcidx]);
  TVector3 XVelvec=(1/XEnergy)*Xp3vec;

  TVector3 Pip3vec(mcp3CM[PiMCIdx[Pi1Idx]]*sin(acos(mccosthCM[PiMCIdx[Pi1Idx]]))*cos(mcphiCM[PiMCIdx[Pi1Idx]]),
		   mcp3CM[PiMCIdx[Pi1Idx]]*sin(acos(mccosthCM[PiMCIdx[Pi1Idx]]))*sin(mcphiCM[PiMCIdx[Pi1Idx]]),
		   mcp3CM[PiMCIdx[Pi1Idx]]*mccosthCM[PiMCIdx[Pi1Idx]]); 
  
  TLorentzVector Pip4(Pip3vec,sqrt(Pip3vec*Pip3vec+mcmass[PiMCIdx[Pi1Idx]]*mcmass[PiMCIdx[Pi1Idx]]));
		      
  //boost the dstar momentum into rest frame of X
  Pip4.Boost(-XVelvec);
  TVector3 PiBoostedp3vec=Pip4.Vect();

  //dot product of boosted dstar direction with X direction 
  return PiBoostedp3vec.Unit()*Xp3vec.Unit();

}



Float_t DstPiAnalysis::ComputeDstarHelicityAngle(){
  //This is the angle of the D0 wrt to the angle of the X Both measured in the D* rest frame.

  //---------
  //Get boost direction
  //----------
  TVector3 Dstarp3vec(Dstarp3CM[DstarIdx]*sin(acos(DstarcosthCM[DstarIdx]))*cos(DstarphiCM[DstarIdx]),
		  Dstarp3CM[DstarIdx]*sin(acos(DstarcosthCM[DstarIdx]))*sin(DstarphiCM[DstarIdx]),
		  Dstarp3CM[DstarIdx]*DstarcosthCM[DstarIdx]);
  
  Float_t DstarEnergy=sqrt(Dstarp3vec*Dstarp3vec + DstarMass[DstarIdx]*DstarMass[DstarIdx]);
  TVector3 DstarVelvec=(1/DstarEnergy)*Dstarp3vec;

  //------
  //Boost the D0 into the D* frame
  //------
  TVector3 D0p3vec(D0p3CM[D0Idx]*sin(acos(D0costhCM[D0Idx]))*cos(D0phiCM[D0Idx]),
		    D0p3CM[D0Idx]*sin(acos(D0costhCM[D0Idx]))*sin(D0phiCM[D0Idx]),
		    D0p3CM[D0Idx]*D0costhCM[D0Idx]); 
  
  TLorentzVector D0p4(D0p3vec,sqrt(D0p3vec*D0p3vec+D0Mass[D0Idx]*D0Mass[D0Idx]));
  
  D0p4.Boost(-DstarVelvec);
  TVector3 D0Boostedp3vec=D0p4.Vect();

  //---------
  //Boost the X into the D* frame
  //---------
  TVector3 Xp3vec(Xp3CM[XIdx]*sin(acos(XcosthCM[XIdx]))*cos(XphiCM[XIdx]),
		    Xp3CM[XIdx]*sin(acos(XcosthCM[XIdx]))*sin(XphiCM[XIdx]),
		    Xp3CM[XIdx]*XcosthCM[XIdx]); 
  
  TLorentzVector Xp4(Xp3vec,sqrt(Xp3vec*Xp3vec+XMass[XIdx]*XMass[XIdx]));
    
  Xp4.Boost(-DstarVelvec);
  TVector3 XBoostedp3vec=Xp4.Vect();


  //dot product 
  return D0Boostedp3vec.Unit()*XBoostedp3vec.Unit();
  
}
Float_t DstPiAnalysis::ComputeGenDstarHelicity(){
  //Generated angle of the D0 wrt the X direction in the D* frame 
 
  //boost direction
  TVector3 Dstarp3vec(mcp3CM[Dstarmcidx]*sin(acos(mccosthCM[Dstarmcidx]))*cos(mcphiCM[Dstarmcidx]),
		      mcp3CM[Dstarmcidx]*sin(acos(mccosthCM[Dstarmcidx]))*sin(mcphiCM[Dstarmcidx]),
		      mcp3CM[Dstarmcidx]*mccosthCM[Dstarmcidx]); 
  
  TLorentzVector Dstarp4(Dstarp3vec,sqrt(Dstarp3vec*Dstarp3vec+mcmass[Dstarmcidx]*mcmass[Dstarmcidx]));
  Float_t DstarEnergy=sqrt(Dstarp3vec*Dstarp3vec +mcmass[Dstarmcidx]*mcmass[Dstarmcidx]);
  TVector3 DstarVelvec=(1/DstarEnergy)*Dstarp3vec;


  //boost the X
  TVector3 Xp3vec(mcp3CM[Xmcidx]*sin(acos(mccosthCM[Xmcidx]))*cos(mcphiCM[Xmcidx]),
		  mcp3CM[Xmcidx]*sin(acos(mccosthCM[Xmcidx]))*sin(mcphiCM[Xmcidx]),
		  mcp3CM[Xmcidx]*mccosthCM[Xmcidx]);

  TLorentzVector Xp4(Xp3vec,sqrt(Xp3vec*Xp3vec+mcmass[Xmcidx]*mcmass[Xmcidx]));
  Xp4.Boost(-DstarVelvec);
  TVector3 XBoostedp3vec=Xp4.Vect();

  //boost the D0
  TVector3 D0p3vec(mcp3CM[D0mcidx]*sin(acos(mccosthCM[D0mcidx]))*cos(mcphiCM[D0mcidx]),
		  mcp3CM[D0mcidx]*sin(acos(mccosthCM[D0mcidx]))*sin(mcphiCM[D0mcidx]),
		  mcp3CM[D0mcidx]*mccosthCM[D0mcidx]);

  TLorentzVector D0p4(D0p3vec,sqrt(D0p3vec*D0p3vec+mcmass[D0mcidx]*mcmass[D0mcidx]));
  D0p4.Boost(-DstarVelvec);
  TVector3 D0Boostedp3vec=D0p4.Vect();


  return XBoostedp3vec.Unit()*D0Boostedp3vec.Unit();
}


Float_t DstPiAnalysis::ComputeD0HelicityAngle(){
  //This is the angle of the Kaon wrt to the angle of the D* Both measured in the D0 rest frame.

  //---------
  //Get boost direction
  //----------
  TVector3 D0p3vec(D0p3CM[D0Idx]*sin(acos(D0costhCM[D0Idx]))*cos(D0phiCM[D0Idx]),
		    D0p3CM[D0Idx]*sin(acos(D0costhCM[D0Idx]))*sin(D0phiCM[D0Idx]),
		    D0p3CM[D0Idx]*D0costhCM[D0Idx]);  
  
  Float_t D0Energy=sqrt(D0p3vec*D0p3vec + D0Mass[D0Idx]*D0Mass[D0Idx]);
  TVector3 D0Velvec=(1/D0Energy)*D0p3vec;

  //---------
  //Boost the Kaon into the D0 frame
  //---------
  TVector3 Kp3vec(Kp3CM[KIdx]*sin(acos(KcosthCM[KIdx]))*cos(KphiCM[KIdx]),
		    Kp3CM[KIdx]*sin(acos(KcosthCM[KIdx]))*sin(KphiCM[KIdx]),
		    Kp3CM[KIdx]*KcosthCM[KIdx]); 
  
  TLorentzVector Kp4(Kp3vec,sqrt(Kp3vec*Kp3vec+KPDGMass*KPDGMass));
    
  Kp4.Boost(-D0Velvec);
  TVector3 KBoostedp3vec=Kp4.Vect();


  //------
  //Boost the D* into the D0 frame
  //------
  TVector3 Dstarp3vec(Dstarp3CM[DstarIdx]*sin(acos(DstarcosthCM[DstarIdx]))*cos(DstarphiCM[DstarIdx]),
		  Dstarp3CM[DstarIdx]*sin(acos(DstarcosthCM[DstarIdx]))*sin(DstarphiCM[DstarIdx]),
		  Dstarp3CM[DstarIdx]*DstarcosthCM[DstarIdx]);

  TLorentzVector Dstarp4(Dstarp3vec,sqrt(Dstarp3vec*Dstarp3vec+DstarMass[DstarIdx]*DstarMass[DstarIdx]));
  
  Dstarp4.Boost(-D0Velvec);
  TVector3 DstarBoostedp3vec=Dstarp4.Vect();


  //dot product 
  return DstarBoostedp3vec.Unit()*KBoostedp3vec.Unit();
  
}

Float_t DstPiAnalysis::ComputeD0DstarAngleDstarFrame(){
  //This is the angle of D0 wrt the D* direction in the D* rest frame.

  //---------
  //Get boost direction
  //--------
  TVector3 Dstarp3vec(Dstarp3CM[DstarIdx]*sin(acos(DstarcosthCM[DstarIdx]))*cos(DstarphiCM[DstarIdx]),
		  Dstarp3CM[DstarIdx]*sin(acos(DstarcosthCM[DstarIdx]))*sin(DstarphiCM[DstarIdx]),
		  Dstarp3CM[DstarIdx]*DstarcosthCM[DstarIdx]);
  
  Float_t DstarEnergy=sqrt(Dstarp3vec*Dstarp3vec + DstarMass[DstarIdx]*DstarMass[DstarIdx]);
  TVector3 DstarVelvec=(1/DstarEnergy)*Dstarp3vec;

  //------
  //Boost the D0 into the D* frame
  //------
  TVector3 D0p3vec(D0p3CM[D0Idx]*sin(acos(D0costhCM[D0Idx]))*cos(D0phiCM[D0Idx]),
		    D0p3CM[D0Idx]*sin(acos(D0costhCM[D0Idx]))*sin(D0phiCM[D0Idx]),
		    D0p3CM[D0Idx]*D0costhCM[D0Idx]); 
  
  TLorentzVector D0p4(D0p3vec,sqrt(D0p3vec*D0p3vec+D0Mass[D0Idx]*D0Mass[D0Idx]));
  
  D0p4.Boost(-DstarVelvec);
  TVector3 D0Boostedp3vec=D0p4.Vect();


  //dot product 
  return D0Boostedp3vec.Unit()*Dstarp3vec.Unit();
}

Float_t DstPiAnalysis::ComputeMCDstarHelicity(Int_t mcid){

  //---------
  //Get boost direction
  //---------
  Int_t dstarid=dauIdx[mcid];
  TVector3 Dstarp3vec(mcp3CM[dstarid]*sin(acos(mccosthCM[dstarid]))*cos(mcphiCM[dstarid]),
		  mcp3CM[dstarid]*sin(acos(mccosthCM[dstarid]))*sin(mcphiCM[dstarid]),
		  mcp3CM[dstarid]*mccosthCM[dstarid]);
  
  Float_t DstarEnergy=sqrt(Dstarp3vec*Dstarp3vec + mcmass[dstarid]*mcmass[dstarid]);
  TVector3 DstarVelvec=(1/DstarEnergy)*Dstarp3vec;

  Int_t d0id=dauIdx[dstarid];
  //------
  //Boost the D0 into the D* frame
  //------
  TVector3 D0p3vec(mcp3CM[d0id]*sin(acos(mccosthCM[d0id]))*cos(mcphiCM[d0id]),
		    mcp3CM[d0id]*sin(acos(mccosthCM[d0id]))*sin(mcphiCM[d0id]),
		    mcp3CM[d0id]*mccosthCM[d0id]); 
  
  TLorentzVector D0p4(D0p3vec,sqrt(D0p3vec*D0p3vec+mcmass[d0id]*mcmass[d0id]));
  
  D0p4.Boost(-DstarVelvec);
  TVector3 D0Boostedp3vec=D0p4.Vect();


  //------
  //Boost the X  into the D* frame
  //------
  TVector3 Xp3vec(mcp3CM[mcid]*sin(acos(mccosthCM[mcid]))*cos(mcphiCM[mcid]),
		    mcp3CM[mcid]*sin(acos(mccosthCM[mcid]))*sin(mcphiCM[mcid]),
		    mcp3CM[mcid]*mccosthCM[mcid]); 
  
  TLorentzVector Xp4(Xp3vec,sqrt(Xp3vec*Xp3vec+mcmass[mcid]*mcmass[mcid]));
  
  Xp4.Boost(-DstarVelvec);
  TVector3 XBoostedp3vec=Xp4.Vect();


  //dot product 
  return D0Boostedp3vec.Unit()*XBoostedp3vec.Unit();
}

Float_t DstPiAnalysis::ComputePi1D0AngleDstarFrame(){
  //This is the angle of Pi1 wrt the D0 direction in the D* rest frame.
  
  //---------
  //Get boost direction
  //----------
  TVector3 Dstarp3vec(Dstarp3CM[DstarIdx]*sin(acos(DstarcosthCM[DstarIdx]))*cos(DstarphiCM[DstarIdx]),
		  Dstarp3CM[DstarIdx]*sin(acos(DstarcosthCM[DstarIdx]))*sin(DstarphiCM[DstarIdx]),
		  Dstarp3CM[DstarIdx]*DstarcosthCM[DstarIdx]);
  
  Float_t DstarEnergy=sqrt(Dstarp3vec*Dstarp3vec + DstarMass[DstarIdx]*DstarMass[DstarIdx]);
  TVector3 DstarVelvec=(1/DstarEnergy)*Dstarp3vec;

  //------
  //Boost the D0 into the D* frame
  //------
  TVector3 D0p3vec(D0p3CM[D0Idx]*sin(acos(D0costhCM[D0Idx]))*cos(D0phiCM[D0Idx]),
		    D0p3CM[D0Idx]*sin(acos(D0costhCM[D0Idx]))*sin(D0phiCM[D0Idx]),
		    D0p3CM[D0Idx]*D0costhCM[D0Idx]); 
  
  TLorentzVector D0p4(D0p3vec,sqrt(D0p3vec*D0p3vec+D0Mass[D0Idx]*D0Mass[D0Idx]));
  
  D0p4.Boost(-DstarVelvec);
  TVector3 D0Boostedp3vec=D0p4.Vect();


  //------
  //Boost the pi1 into the D* frame
  //------
  TVector3 Pip3vec(Pip3CM[Pi1Idx]*sin(acos(PicosthCM[Pi1Idx]))*cos(PiphiCM[Pi1Idx]),
		   Pip3CM[Pi1Idx]*sin(acos(PicosthCM[Pi1Idx]))*sin(PiphiCM[Pi1Idx]),
		   Pip3CM[Pi1Idx]*PicosthCM[Pi1Idx]); 
  
  TLorentzVector Pip4(Pip3vec,sqrt(Pip3vec*Pip3vec+PiPDGMass*PiPDGMass));
		      
  Pip4.Boost(-DstarVelvec);
  TVector3 PiBoostedp3vec=Pip4.Vect();
  
  
  
  //dot product 
  return D0Boostedp3vec.Unit()*PiBoostedp3vec.Unit();
  
}


Float_t DstPiAnalysis::ComputePiDstarAngle(){
  
  TVector3 Dstarp3vec(Dstarp3CM[DstarIdx]*sin(acos(DstarcosthCM[DstarIdx]))*cos(DstarphiCM[DstarIdx]),
		      Dstarp3CM[DstarIdx]*sin(acos(DstarcosthCM[DstarIdx]))*sin(DstarphiCM[DstarIdx]),
		      Dstarp3CM[DstarIdx]*DstarcosthCM[DstarIdx]); 

  TVector3 Pip3vec(Pip3CM[Pi1Idx]*sin(acos(PicosthCM[Pi1Idx]))*cos(PiphiCM[Pi1Idx]),
		   Pip3CM[Pi1Idx]*sin(acos(PicosthCM[Pi1Idx]))*sin(PiphiCM[Pi1Idx]),
		   Pip3CM[Pi1Idx]*PicosthCM[Pi1Idx]); 
  
  return Dstarp3vec.Unit()*Pip3vec.Unit();
  
}
Float_t DstPiAnalysis::ComputePiMotherAngle(){
  TVector3 Xp3vec(Xp3CM[XIdx]*sin(acos(XcosthCM[XIdx]))*cos(XphiCM[XIdx]),
		  Xp3CM[XIdx]*sin(acos(XcosthCM[XIdx]))*sin(XphiCM[XIdx]),
		  Xp3CM[XIdx]*XcosthCM[XIdx]); 
  
  TVector3 Pip3vec(Pip3CM[Pi1Idx]*sin(acos(PicosthCM[Pi1Idx]))*cos(PiphiCM[Pi1Idx]),
		   Pip3CM[Pi1Idx]*sin(acos(PicosthCM[Pi1Idx]))*sin(PiphiCM[Pi1Idx]),
		   Pip3CM[Pi1Idx]*PicosthCM[Pi1Idx]); 
  
  
  return Xp3vec.Unit()*Pip3vec.Unit();

}
Float_t DstPiAnalysis::ComputeGenPiMotherAngle(){
  TVector3 Xp3vec(mcp3CM[Xmcidx]*sin(acos(mccosthCM[Xmcidx]))*cos(mcphiCM[Xmcidx]),
		  mcp3CM[Xmcidx]*sin(acos(mccosthCM[Xmcidx]))*sin(mcphiCM[Xmcidx]),
		  mcp3CM[Xmcidx]*mccosthCM[Xmcidx]); 
  
  TVector3 Pip3vec(mcp3CM[PiMCIdx[Pi1Idx]]*sin(acos(mccosthCM[PiMCIdx[Pi1Idx]]))*cos(mcphiCM[PiMCIdx[Pi1Idx]]),
		   mcp3CM[PiMCIdx[Pi1Idx]]*sin(acos(mccosthCM[PiMCIdx[Pi1Idx]]))*sin(mcphiCM[PiMCIdx[Pi1Idx]]),
		   mcp3CM[PiMCIdx[Pi1Idx]]*mccosthCM[PiMCIdx[Pi1Idx]]); 
  
  return Xp3vec.Unit()*Pip3vec.Unit();

}
Float_t DstPiAnalysis::ComputeDstarMotherAngle(){
  TVector3 Xp3vec(Xp3CM[XIdx]*sin(acos(XcosthCM[XIdx]))*cos(XphiCM[XIdx]),
		  Xp3CM[XIdx]*sin(acos(XcosthCM[XIdx]))*sin(XphiCM[XIdx]),
		  Xp3CM[XIdx]*XcosthCM[XIdx]); 
  
  TVector3 Dstarp3vec(Dstarp3CM[DstarIdx]*sin(acos(DstarcosthCM[DstarIdx]))*cos(DstarphiCM[DstarIdx]),
		      Dstarp3CM[DstarIdx]*sin(acos(DstarcosthCM[DstarIdx]))*sin(DstarphiCM[DstarIdx]),
		      Dstarp3CM[DstarIdx]*DstarcosthCM[DstarIdx]); 
  
  return Xp3vec.Unit()*Dstarp3vec.Unit();
}
Float_t DstPiAnalysis::ComputeGenDstarMotherAngle(){
  TVector3 Xp3vec(mcp3CM[Xmcidx]*sin(acos(mccosthCM[Xmcidx]))*cos(mcphiCM[Xmcidx]),
		  mcp3CM[Xmcidx]*sin(acos(mccosthCM[Xmcidx]))*sin(mcphiCM[Xmcidx]),
		  mcp3CM[Xmcidx]*mccosthCM[Xmcidx]); 
  
  TVector3 Dstarp3vec(mcp3CM[Dstarmcidx]*sin(acos(mccosthCM[Dstarmcidx]))*cos(mcphiCM[Dstarmcidx]),
		      mcp3CM[Dstarmcidx]*sin(acos(mccosthCM[Dstarmcidx]))*sin(mcphiCM[Dstarmcidx]),
		      mcp3CM[Dstarmcidx]*mccosthCM[Dstarmcidx]); 
  
  return Xp3vec.Unit()*Dstarp3vec.Unit();

}

Float_t DstPiAnalysis::ComputexyzMass(Float_t xp3,Float_t xcosth,Float_t xphi,Float_t xmass,  
				      Float_t yp3,Float_t ycosth,Float_t yphi,Float_t ymass,  
				      Float_t zp3,Float_t zcosth,Float_t zphi,Float_t zmass){
  
  TVector3 xp3vec(xp3*sin(acos(xcosth))*cos(xphi),
		  xp3*sin(acos(xcosth))*sin(xphi),
		  xp3*xcosth); 
		  
  TLorentzVector xp4(xp3vec,sqrt(xp3vec*xp3vec+xmass*xmass));
  
  TVector3 yp3vec(yp3*sin(acos(ycosth))*cos(yphi),
		  yp3*sin(acos(ycosth))*sin(yphi),
		  yp3*ycosth); 
  
  TLorentzVector yp4(yp3vec,sqrt(yp3vec*yp3vec+ymass*ymass));

  TVector3 zp3vec(zp3*sin(acos(zcosth))*cos(zphi),
		  zp3*sin(acos(zcosth))*sin(zphi),
		  zp3*zcosth); 
  
  TLorentzVector zp4(zp3vec,sqrt(zp3vec*zp3vec+zmass*zmass));
  
  
  return (xp4+yp4+zp4).Mag();
}

Float_t DstPiAnalysis::ComputexyMass(Float_t xp3,Float_t xcosth,Float_t xphi,Float_t xmass,  
				     Float_t yp3,Float_t ycosth,Float_t yphi,Float_t ymass){
  
  TVector3 xp3vec(xp3*sin(acos(xcosth))*cos(xphi),
		  xp3*sin(acos(xcosth))*sin(xphi),
		  xp3*xcosth); 
  
  TLorentzVector xp4(xp3vec,sqrt(xp3vec*xp3vec+xmass*xmass));
  
  TVector3 yp3vec(yp3*sin(acos(ycosth))*cos(yphi),
		  yp3*sin(acos(ycosth))*sin(yphi),
		  yp3*ycosth); 
  
  TLorentzVector yp4(yp3vec,sqrt(yp3vec*yp3vec+ymass*ymass));
  
  //return the mass
  return (xp4+yp4).Mag();

}


// Float_t DstPiAnalysis::CalculateGenDstPiDistance(){
//   if(DstarMCIdx<0||PiMCIdx[Pi1Idx]<0)return 0;

//   TVector3 Dstarvec;
//   Dstarvec.SetXYZ(mcVtxx[Dstarmcidx],mcVtxy[Dstarmcidx],mcVtxz[Dstarmcidx]);

//   TVector3 Pi1vec;
//   Pi1vec.SetXYZ(mcVtxx[mothIdx[PiMCIdx[Pi1Idx]]],mcVtxy[mothIdx[PiMCIdx[Pi1Idx]]],mcVtxz[mothIdx[PiMCIdx[Pi1Idx]]]);
  
//   return (Dstarvec-Pi1vec).Mag();
// }

// void DstPiAnalysis::CalculateDstRandPiGenMassDist(Int_t dstmcid){
//   //calculate mass af D* and random pi
//   if(abs(mcLund[dstmcid])==MYDSTARLUND){//found a D*
//     Int_t pimcid=-1;
//     while(pimcid<mcLen){//search for the pions
//       pimcid++;
//       if(abs(mcLund[pimcid])==myPiLund && mcLund[dstmcid]*mcLund[pimcid]<0){//found a random pi with right charge
// 	Float_t dist=CalculateMCGenDstPiDistance(dstmcid,pimcid);
// 	HMCDstRandPiDistance->Fill(dist);
// 	if(dist<.1){//check if random pi is close to D*
// 	  Float_t dm=CalculateMCGenMass(dstmcid,pimcid)-DstarPDGMass;
// 	  HMCDstRandPiDeltaM3->Fill(dm);
// 	  if(dist<.05){
// 	    HMCDstRandPiDeltaM2->Fill(dm);
// 	    if(dist<.02)
// 	      HMCDstRandPiDeltaM1->Fill(dm);
// 	  }
// 	}
//       }	       
//     }
//   }  
// }

// Float_t DstPiAnalysis::CalculateMCGenMass(Int_t dstarmcid,Int_t pimcid){

//   //determine 4-vector of true-D* 
//   TVector3 Dstarp3vec;
//   Dstarp3vec.SetMagThetaPhi(mcp3CM[dstarmcid],acos(mccosthCM[dstarmcid]),mcphiCM[dstarmcid]);
//   TLorentzVector Dstarp4(Dstarp3vec,sqrt(Dstarp3vec*Dstarp3vec+pow(mcmass[dstarmcid],2)));

//   //determine 4-vector of true-pi
//   TVector3 Pi1p3vec;
//   Pi1p3vec.SetMagThetaPhi(mcp3CM[pimcid],acos(mccosthCM[pimcid]),mcphiCM[pimcid]);
//   TLorentzVector Pi1p4(Pi1p3vec,sqrt(Pi1p3vec*Pi1p3vec+pow(mcmass[pimcid],2)));

//   //return the mass
//   return (Dstarp4+Pi1p4).Mag();
// }

// Float_t DstPiAnalysis::CalculateMCGenDstPiDistance(Int_t dstarmcid,Int_t pimcid){
//   TVector3 Dstarvec;
//   Dstarvec.SetXYZ(mcVtxx[dstarmcid],mcVtxy[dstarmcid],mcVtxz[dstarmcid]);

//   TVector3 Pi1vec;
//   Pi1vec.SetXYZ(mcVtxx[mothIdx[pimcid]],mcVtxy[mothIdx[pimcid]],mcVtxz[mothIdx[pimcid]]);//use the mother vertex
  
//   return (Dstarvec-Pi1vec).Mag();
// }

Int_t DstPiAnalysis::SetBranches(){
  if(_fChain==NULL){cout<<"No Chain"<<endl;return 0;} 
  cout<<"Setting Chain Branch Addresses"<<endl;

  _fChain->SetBranchAddress("runNumber",&runNumber);

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

  _fChain->SetBranchAddress("D0d3Lund",D0d3Lund);
  _fChain->SetBranchAddress("D0d3Idx",D0d3Idx);
  _fChain->SetBranchAddress("D0d4Lund",D0d4Lund);
  _fChain->SetBranchAddress("D0d4Idx",D0d4Idx);
  
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


  _fChain->SetBranchAddress("TRKLund",TRKLund); 
  _fChain->SetBranchAddress("piSelectorsMap",PiSelectorsMap);
  _fChain->SetBranchAddress("KSelectorsMap",KSelectorsMap);

  if(_MCorDATA<=1){
    _fChain->SetBranchAddress("DstarMCIdx",DstarMCIdx);
    _fChain->SetBranchAddress("D0MCIdx",D0MCIdx);
    _fChain->SetBranchAddress("KMCIdx",KMCIdx);
    _fChain->SetBranchAddress("PiMCIdx",PiMCIdx);
    _fChain->SetBranchAddress("mcLund",mcLund);
    _fChain->SetBranchAddress("mothIdx",mothIdx);
    _fChain->SetBranchAddress("dauIdx",dauIdx);
    _fChain->SetBranchAddress("dauLen",dauLen);
    _fChain->SetBranchAddress("mcLen",&mcLen);
    _fChain->SetBranchAddress("mcmass",mcmass);
    _fChain->SetBranchAddress("mcp3",mcp3);
    _fChain->SetBranchAddress("mccosth",mccosth);
    _fChain->SetBranchAddress("mcphi",mcphi);
    _fChain->SetBranchAddress("mcp3CM",mcp3CM);
    _fChain->SetBranchAddress("mccosthCM",mccosthCM); 
    _fChain->SetBranchAddress("mcphiCM",mcphiCM);
    _fChain->SetBranchAddress("mcVtxx",mcVtxx);
    _fChain->SetBranchAddress("mcVtxy",mcVtxy);
    _fChain->SetBranchAddress("mcVtxz",mcVtxz);
  }

//   if(_MCorDATA==0){
//     _fChain->SetBranchAddress("Xpi1pidw",Xpi1pidw);
//     _fChain->SetBranchAddress("Xpi1pidwe",Xpi1pidwe);
//     _fChain->SetBranchAddress("XKpidw",XKpidw);
//     _fChain->SetBranchAddress("XKpidwe",XKpidwe);
//     _fChain->SetBranchAddress("Xpipidw",Xpipidw);
//     _fChain->SetBranchAddress("Xpipidwe",Xpipidwe);
//   }

  cout<<"Done linking Chain Branches"<<endl;
  return 1;
}



Int_t DstPiAnalysis::MakeReducedNtupleBranches(TTree* Ntuple){
  cout<<"Creating Reduced Ntuple Branches"<<endl;

  Ntuple->Branch("runnumber",&runnumber,"runnumber/I");
  Ntuple->Branch("eventnumber",&eventnumber,"eventnumber/I");

  Ntuple->Branch("dstpimass",&dstpimass,"dstpimass/F");
  Ntuple->Branch("dstpideltam",&dstpideltam,"dstpideltam/F");
  Ntuple->Branch("dstpimdm",&dstpimdm,"dstpimdm/F");
  Ntuple->Branch("dstpipstar",&dstpipstar,"dstpipstar/F");
  Ntuple->Branch("dstpicosstar",&dstpicosstar,"dstpicosstar/F");
  Ntuple->Branch("dstpiphistar",&dstpiphistar,"dstpiphistar/F");
  Ntuple->Branch("dstpicharge",&dstpicharge,"dstpicharge/I");
  Ntuple->Branch("dstpinX",&dstpinX,"dstpinX/I");
  Ntuple->Branch("dstpilogvtxprob",&dstpilogvtxprob,"dstpilogvtxprob/F");
  Ntuple->Branch("dstpiIdx",&dstpiIdx,"dstpiIdx/I");
  Ntuple->Branch("dstpimctrue",&dstpimctrue,"dstpimctrue/I");
  Ntuple->Branch("dstpimctruebad",&dstpimctruebad,"dstpimctruebad/I");
  Ntuple->Branch("dstpidmres",&dstpidmres,"dstpidmres/F");
  Ntuple->Branch("dstpigenmass",&dstpigenmass,"dstpigenmass/F");
  Ntuple->Branch("dstpigendist",&dstpigendist,"dstpigendist/F");
  Ntuple->Branch("dstpimclund",&dstpimclund,"dstpimclund/I");
  Ntuple->Branch("dstpipidw",&dstpipidw,"dstpipidw");
  Ntuple->Branch("dstpipidwe",&dstpipidwe,"dstpipidwe");
  Ntuple->Branch("dstpicosstarres",&dstpicosstarres,"dstpicosstarres/F");
  Ntuple->Branch("dstpiphistarres",&dstpiphistarres,"dstpiphistarres/F");
  Ntuple->Branch("masspislowpi1",&masspislowpi1,"masspislowpi1/F");
  Ntuple->Branch("masspipi1",&masspipi1,"masspipi1/F");
  Ntuple->Branch("massKpislow",&massKpislow,"massKpislow/F");
  Ntuple->Branch("massKpislowpi1",&massKpislowpi1,"massKpislowpi1/F");
  Ntuple->Branch("massKpipi1",&massKpipi1,"massKpipi1/F");
  Ntuple->Branch("masspipislowpi1",&masspipislowpi1,"masspipislowpi1/F");
  Ntuple->Branch("dstpibestcand",&dstpibestcand,"dstpibestcand/I");
  

  Ntuple->Branch("dstarmass",&dstarmass,"dstarmass/F");
  Ntuple->Branch("dstarpstar",&dstarpstar,"dstarpstar/F");
  Ntuple->Branch("dstardeltam",&dstardeltam,"dstardeltam/F");
  Ntuple->Branch("dstarm",&dstarm,"dstarm/F");
  Ntuple->Branch("dstarlogvtxprob",&dstarlogvtxprob,"dstarlogvtxprob/F");
  Ntuple->Branch("dstarcostheta",&dstarcostheta,"dstarcostheta/F");
  Ntuple->Branch("dstarcosthetares",&dstarcosthetares,"dstarcosthetares/F");
  Ntuple->Branch("dstarcharge",&dstarcharge,"dstarcharge/I");
  Ntuple->Branch("dstarmctrue",&dstarmctrue,"dstarmctrue/I");
  Ntuple->Branch("dstarmothcosine",&dstarmothcosine,"dstarmothcosine/F");
  Ntuple->Branch("dstarmothcosineres",&dstarmothcosineres,"dstarmothcosineres/F");
  Ntuple->Branch("dstarhelicity",&dstarhelicity,"dstarhelicity/F");
  Ntuple->Branch("dstarhelicityres",&dstarhelicityres,"dstarhelicityres/F");

  Ntuple->Branch("d0pstar",&d0pstar,"d0pstar/F");
  Ntuple->Branch("d0logvtxprob",&d0logvtxprob,"d0logvtxprob/F");
  Ntuple->Branch("d0charge",&d0charge,"d0charge/I");
  Ntuple->Branch("d0mass",&d0mass,"d0mass/F"); 
  Ntuple->Branch("d0costheta",&d0costheta,"d0costheta/F"); 
  Ntuple->Branch("d0mode",&d0mode,"d0mode/I");
  Ntuple->Branch("d0mctrue",&d0mctrue,"d0mctrue/I");
  Ntuple->Branch("d0helicity",&d0helicity,"d0helicity/F");
  Ntuple->Branch("d0dstarangle",&d0dstarangle,"d0dstarangle/F");

  Ntuple->Branch("kmctrue",&kmctrue,"kmctrue/I");
  Ntuple->Branch("kp3",&kp3,"kp3/F");
  Ntuple->Branch("kcostheta",&kcostheta,"kcostheta/F"); 
  Ntuple->Branch("kcharge",&kcharge,"kcharge/I");
  Ntuple->Branch("klh",&klh,"klh/I");

  Ntuple->Branch("pimctrue",&pimctrue,"pimctrue/I");
  Ntuple->Branch("pip3",&pip3,"pip3/F");
  Ntuple->Branch("picharge",&picharge,"picharge/I");
  Ntuple->Branch("pilh",&pilh,"pilh/I");

  Ntuple->Branch("pi1mctrue",&pi1mctrue,"pi1mctrue/I");
  Ntuple->Branch("pi1p3",&pi1p3,"pi1p3/F");
  Ntuple->Branch("pi1charge",&pi1charge,"pi1charge/I");
  Ntuple->Branch("pi1lh",&pi1lh,"pi1lh/I");
  Ntuple->Branch("pi1lhK",&pi1lhK,"pi1lhK/I");
  Ntuple->Branch("pi1dedx",&pi1dedx,"pi1dedx/F");
  Ntuple->Branch("pi1cosine",&pi1cosine,"pi1cosine/F");
  Ntuple->Branch("pi1thetac",&pi1thetac,"pi1thetac/F");
  Ntuple->Branch("pi1costheta",&pi1costheta,"pi1costheta/F");
  Ntuple->Branch("pi1costhetares",&pi1costhetares,"pi1costhetares/F");
  Ntuple->Branch("pi1mothcosine",&pi1mothcosine,"pi1mothcosine/F");
  Ntuple->Branch("pi1mothcosineres",&pi1mothcosineres,"pi1mothcosineres/F");
  Ntuple->Branch("pi1D0angle",&pi1D0angle,"pi1D0angle/F");

  Ntuple->Branch("slowpip3",&slowpip3,"slowpip3/F");
  Ntuple->Branch("slowpimctrue",&slowpimctrue,"slowpimctrue/I");
  Ntuple->Branch("slowpicharge",&slowpicharge,"slowpicharge/I");
  Ntuple->Branch("slowpilh",&slowpilh,"slowpilh/I");

  Ntuple->Branch("d0pi2p3",&d0pi2p3,"d0pi2p3/F");
  Ntuple->Branch("d0pi2mctrue",&d0pi2mctrue,"d0pi2mctrue/I");
  Ntuple->Branch("d0pi2charge",&d0pi2charge,"d0pi2charge/I");
  Ntuple->Branch("d0pi2lh",&d0pi2lh,"d0pi2lh/I");

  Ntuple->Branch("d0pi3p3",&d0pi3p3,"d0pi3p3/F");
  Ntuple->Branch("d0pi3mctrue",&d0pi3mctrue,"d0pi3mctrue/I");
  Ntuple->Branch("d0pi3charge",&d0pi3charge,"d0pi3charge/I");
  Ntuple->Branch("d0pi3lh",&d0pi3lh,"d0pi3lh/I");
  
  cout<<"Done Making Reduced Ntuple Branches"<<endl;
  return 1;
}




void  DstPiAnalysis::DefaultPlots(){
  OpenReducedNtuple();
  
  filename=_OutputDir+"/DefaultPlots.ps";
  Canvas.Print(filename+"[");
  
  Canvas.Clear();
  TH1F HMDM("HMDM","",200,2.1,3.1);
  HMDM.GetYaxis()->SetTitle(YAxisLabel);
  HMDM.GetXaxis()->SetTitle(XAxisLabel);
  ReducedNtuple->Draw("dstpimdm>>HMDM","","pe");
  if(_MCorDATA==0)ReducedNtuple->Draw("dstpimdm","dstpimctrue==0","same");
  Canvas.Print(filename);

  if(_MCorDATA==0){
    Canvas.Clear();
    TH1F HMDMSig("HMDMSig","",200,2.1,3.1);
    HMDMSig.GetYaxis()->SetTitle(YAxisLabel);
    HMDMSig.GetXaxis()->SetTitle(XAxisLabel);
    ReducedNtuple->Draw("dstpimdm>>HMDMSig","dstpimctrue==1","pe");
    ReducedNtuple->Draw("dstpimdm","dstpimctrue==1&&dstpimclund==10423","same");
    ReducedNtuple->Draw("dstpimdm","dstpimctrue==1&&dstpimclund==425","same");
    ReducedNtuple->Draw("dstpimdm","dstpimctrue==1&&dstpimclund==20423","same");
    Canvas.Print(filename);
  }

//   Canvas.Clear();
//   TH1F HMass("HMass","",200,2.1,3.1);
//   HMass.GetYaxis()->SetTitle(YAxisLabel);
//   HMass.GetXaxis()->SetTitle("m(D^{_{*+}}#pi^{-})  (GeV)");
//   ReducedNtuple->Draw("dstpimass>>HMass","","pe");
//   if(_MCorDATA==0||_MCorDATA==1)ReducedNtuple->Draw("dstpimass","dstpimctrue<=0","same");
//   Canvas.Print(filename);

  Canvas.Clear();
  TH1F HPstar("HPstar","",150,2.,5.);
  ReducedNtuple->Draw("dstpipstar>>HPstar","","pe");
  if(_MCorDATA==0)ReducedNtuple->Draw("dstpipstar","dstpimctrue<=0","same");
  Canvas.Print(filename);

  Canvas.Clear();
  TH1F HCosD("HCosD","",100,-1,1.0001);
  ReducedNtuple->Draw("dstarcostheta>>HCosD","","pe");
  if(_MCorDATA==0)ReducedNtuple->Draw("dstarcostheta","dstpimctrue<=0","same");
  Canvas.Print(filename);


  Canvas.Clear();
  TH1F HD0Mass("HD0Mass","",130,1.80,1.93);
  HD0Mass.GetYaxis()->SetTitle("Entries/ 1 MeV");
  HD0Mass.GetXaxis()->SetTitle("m(K^{-}#pi^{+})   (GeV)");
  ReducedNtuple->Draw("d0mass>>HD0Mass","d0mode==1","pe");
  if(_MCorDATA<=1)ReducedNtuple->Draw("d0mass","d0mode==1&&d0mctrue<=0","same");
  cutline.DrawLine(D0MassMean-D0MassCut,0,D0MassMean-D0MassCut,HD0Mass.GetMaximum()/2);
  cutline.DrawLine(D0MassMean+D0MassCut,0,D0MassMean+D0MassCut,HD0Mass.GetMaximum()/2);
  Canvas.Print(filename);

  Canvas.Clear();
  TH1F HD0Mass2("HD0Mass2","",130,1.80,1.93);
  HD0Mass2.GetYaxis()->SetTitle("Entries/ 1 MeV");
  HD0Mass2.GetXaxis()->SetTitle("m(K^{-}#pi^{+}#pi^{-}#pi^{+})  (GeV)");
  ReducedNtuple->Draw("d0mass>>HD0Mass2","d0mode==2","pe");
  if(_MCorDATA<=1)ReducedNtuple->Draw("d0mass","d0mode==2&&d0mctrue<=0","same");
  cutline.DrawLine(D0MassMean-D0MassCut,0,D0MassMean-D0MassCut,HD0Mass2.GetMaximum()/2);
  cutline.DrawLine(D0MassMean+D0MassCut,0,D0MassMean+D0MassCut,HD0Mass2.GetMaximum()/2);
  Canvas.Print(filename);

  Canvas.Clear();
  TH1F HD0Mass3("HD0Mass3","",130,1.80,1.93);
  HD0Mass3.GetYaxis()->SetTitle("Entries / 1 MeV/c^{2}");
  HD0Mass3.GetXaxis()->SetTitle("D^{0} candidate mass  GeV/c^{2}");
  HD0Mass3.SetStats(0);
  ReducedNtuple->Draw("d0mass>>HD0Mass3","","pe");
  if(_MCorDATA<=1)ReducedNtuple->Draw("d0mass","d0mctrue<=0","same");
  cutline.DrawLine(D0MassMean-D0MassCut,0,D0MassMean-D0MassCut,HD0Mass3.GetMaximum());
  cutline.DrawLine(D0MassMean+D0MassCut,0,D0MassMean+D0MassCut,HD0Mass3.GetMaximum());
  Canvas.Print(filename);

  Canvas.Clear();
  TH1F HDeltaM("HDeltaM","",150,.140,.155);
  HDeltaM.GetYaxis()->SetTitle("Entries / 0.1 MeV/c^{2}");
  HDeltaM.GetXaxis()->SetTitle("m(D^{0}#pi^{+}) - m(D^{0})   GeV/c^{2}");
  HDeltaM.SetStats(0);
  ReducedNtuple->Draw("dstardeltam>>HDeltaM","","pe");
  if(_MCorDATA<=1)ReducedNtuple->Draw("dstardeltam","dstarmctrue<=0","same");
  cutline.DrawLine(DstarDMMean-DstarDMCut,0,DstarDMMean-DstarDMCut,HDeltaM.GetMaximum());
  cutline.DrawLine(DstarDMMean+DstarDMCut,0,DstarDMMean+DstarDMCut,HDeltaM.GetMaximum());
  Canvas.Print(filename);
  cutline.SetLineColor(4);
  cutline.DrawLine(DstarDMMean-2*DstarDMCut,0,DstarDMMean-2*DstarDMCut,HDeltaM.GetMaximum()/4);
  cutline.DrawLine(DstarDMMean-DstarDMCut,0,DstarDMMean-DstarDMCut,HDeltaM.GetMaximum()/4);
  cutline.DrawLine(DstarDMMean+DstarDMCut,0,DstarDMMean+DstarDMCut,HDeltaM.GetMaximum()/4);
  cutline.DrawLine(DstarDMMean+2*DstarDMCut,0,DstarDMMean+2*DstarDMCut,HDeltaM.GetMaximum()/4);
  Canvas.Print(filename);


  //fit to show background
  TF1 fSig; TF1 fBkg;
  TF1*fDM=FitHistogramDstarDM(&HDeltaM,.140,.155,&fSig,&fBkg);
  Canvas.Clear();
  HDeltaM.Draw("pe");
  fBkg.Draw("lsame");
  fDM->Draw("lsame");
  //cutline.DrawLine(DstarDMMean-2*DstarDMCut,0,DstarDMMean-2*DstarDMCut,HDeltaM.GetMaximum()/4);
  cutline.DrawLine(DstarDMMean-DstarDMCut,0,DstarDMMean-DstarDMCut,HDeltaM.GetMaximum()/4);
  cutline.DrawLine(DstarDMMean+DstarDMCut,0,DstarDMMean+DstarDMCut,HDeltaM.GetMaximum()/4);
  //cutline.DrawLine(DstarDMMean+2*DstarDMCut,0,DstarDMMean+2*DstarDMCut,HDeltaM.GetMaximum()/4);
  Canvas.Print(filename);
  cout<<"The D* purity is = "<<fSig.Integral(DstarDMMean-DstarDMCut,DstarDMMean+DstarDMCut)/fDM->Integral(DstarDMMean-DstarDMCut,DstarDMMean+DstarDMCut)<<endl;


  Canvas.Print(filename+"]");
  CloseReducedNtuple();
  
}


Int_t DstPiAnalysis::DrawEfficiencyVsMass(){

  ///Open the reduced Ntuple 
  if(!OpenReducedNtuple())return 0; 

  if(!OutPutFile){cout<<" OutPutFile not found "<<endl;return 0;}
  TH1F* HGen=(TH1F*)OutPutFile->Get("HMCXMass");
  if(!HGen){cout<<" HGen not found "<<endl;return 0;}
  if(!(HGen=(TH1F*)RebinHistoX(HGen,40,2.25,2.65))){cout<<" Bad histo"<<endl;return 0;}

  ///fill the reconstructed distribution
  TH1F HMDM("HMDM","",HGen->GetXaxis()->GetNbins(),HGen->GetXaxis()->GetXmin(),HGen->GetXaxis()->GetXmax());
  ReducedNtuple->Draw("dstpimdm-dstpidmres>>HMDM","dstpimctrue==1");

  //eff vs pstar
  TH1F* HPstarGen=(TH1F*)OutPutFile->Get("HMCXp3CM");
  if(!HPstarGen){cout<<" HPstarGen not found "<<endl;return 0;}
  if(!(HPstarGen=(TH1F*)RebinHistoX(HPstarGen,25,2.5,5.0))){cout<<" Bad histo"<<endl;return 0;}
  ///fill the reconstructed distribution
  TH1F HPstar("HPstar","",HPstarGen->GetXaxis()->GetNbins(),HPstarGen->GetXaxis()->GetXmin(),HPstarGen->GetXaxis()->GetXmax());
  ReducedNtuple->Draw("dstpipstar>>HPstar","dstpimctrue==1");


  
  filename=_OutputDir+"/EffciencyVsMass.ps";
  Canvas.Print(filename+"[");

  Canvas.Clear();
  HGen->Draw("hist");
  HMDM.Draw("pesame");
  Canvas.Print(filename);  


  Canvas.Clear();
  HMDM.Sumw2();
  HMDM.Divide(HGen);
  HMDM.GetYaxis()->SetTitle("Efficiency");
  HMDM.GetYaxis()->SetRangeUser(0,.34);
  HMDM.GetXaxis()->SetTitle("m_{gen}(D^{_{**}})   (GeV/c^{2})");
  delete HMDM.GetListOfFunctions()->FindObject("stats");
  //HMDM.SetStats(0);
  HMDM.Fit("pol1");
  HMDM.Draw("pe");
  Canvas.Print(filename);  


  //pstar
  Canvas.Clear();
  HPstarGen->Draw("hist");
  HPstar.Draw("pesame");
  Canvas.Print(filename);  

  TF1 Fpol("Fpol","[0]+[1]*x",3,4.5);
  Fpol.SetLineColor(2);
  HPstar.Sumw2();
  HPstar.Divide(HPstarGen);
  HPstar.GetYaxis()->SetTitle("Efficiency");
  HPstar.GetYaxis()->SetRangeUser(0,.5);
  HPstar.GetXaxis()->SetTitle("p*(D^{_{*}}#pi)   (GeV)");
  delete HPstar.GetListOfFunctions()->FindObject("stats");
  //HPstar.SetStats(0);
  HPstar.Fit("Fpol");
  Canvas.Clear();
  HPstar.Draw("hist pe");
  Fpol.Draw("same");
  Canvas.Print(filename);  


  Canvas.Print(filename+"]");

  delete HGen;
  delete HPstarGen;
  return 1;
}

Int_t DstPiAnalysis::DrawResolutionVsMass(){

  ///Open the reduced Ntuple 
  if(!OpenReducedNtuple())return 0; 


  filename=_OutputDir+"/Resolution.ps";
  Canvas.Print(filename+"[");
  

  cout<<" DrawResolutionVsMass"<<endl;
  
 
  ///-----------------------------
  //Resolution vs Mass
  //------------------------------


  ///plot the resolution as a function of mass
  Float_t fitmin=-.05;
  Float_t fitmax=.05;
  Int_t nbins=200;  
  Float_t dmmin=2.29;//2.430
  Float_t dmmax=2.56;
  Int_t npoints=9;//NDMPOINTS;//check array size below
  Float_t dmstep=(dmmax-dmmin)/npoints;
  TString label;
  TH1F* HDstPiDMResVsM[9];//array size should be consistent with npoints
  TGraphErrors GResVsM;
  TGraphErrors GAvgVsM;
  TGraphErrors GMeanVsM;
  TGraphErrors GSigma1VsM; 
  TGraphErrors GRatioVsM;
  TGraphErrors GMean2VsM; 
  TGraphErrors GSigma2VsM;
  TGraphErrors GRatio2VsM;
  TGraphErrors GMean3VsM; 
  TGraphErrors GSigma3VsM;
  TGraphErrors GChisVsM;
  Float_t Mean[9];

 
  //TF1 FRes("FRes","[0]*([1]*[2]*3.14/((x-[1])**2+(.5*[2])**2)+[3]*[1]*[4]*[2]*3.14/((x-[1])**2+(.5*[4]*[2])**2))",fitmin,fitmax);
  //TF1 FRes("FRes","[0]*([1]*[2]*3.14/((x-[1])**2+(.5*[2])**2)+[3]*exp(-.5*(x-[1])**2/([4]*[2])**2))",fitmin,fitmax);
  //TF1 FRes("FRes","[0]*exp(-.5*(x-[1])**2/[2]**2)",fitmin,fitmax);
  /*
  TF1 FRes("FRes","[0]*(exp(-.5*(x-[1])**2/[2]**2)+[3]*exp(-.5*(x-[1])**2/([4]*[2])**2))",fitmin,fitmax);
  FRes.SetParameters(100,0,.001,.1,10);  
  FRes.SetParLimits(0,0,1e6);
  FRes.SetParLimits(1,-.001,.001);
  FRes.SetParLimits(2,.0001,.02);
  FRes.SetParLimits(3,.01,1);
  FRes.SetParLimits(4,1,10);
  FRes.FixParameter(1,0.);
  FRes.FixParameter(3,.06);
  FRes.FixParameter(4,2.5);
  */
 
//   //2gaus to fit core only is wrong need to fit tails
//   TF1 FRes2("FRes2","[0]*(((1.-[3])/([2]*2.5066))*exp(-.5*(x-[1])**2/[2]**2) + [3]*(1./([5]*[2]*2.5066))*exp(-.5*(x-[1]-[4])**2/([5]*[2])**2))",fitmin,fitmax);
//   FRes2.SetParameters(100,0,.001,.1,-.0006,2);  
//   FRes2.SetParLimits(0,0,1e6);
//   FRes2.SetParLimits(1,fitmin,fitmax);
//   FRes2.SetParLimits(2,.000001,.2);
//   FRes2.SetParLimits(3,.001,10);
//   FRes2.SetParLimits(4,-.002,.002);
//   FRes2.SetParLimits(5,1,10);
//   FRes2.FixParameter(1,.000040);//.00003
//   FRes2.FixParameter(3,.2);//.06 without normalization
//   FRes2.FixParameter(4,-.00050);//-.0006
//   FRes2.FixParameter(5,2);//2.5

  ///gaus+2BW
  TF1 FRes("FRes","[0]*((1.-[3]-[6])*exp(-.5*(x-[1])**2/[2]**2)/([2]*2.507)+[3]*([5]*[2]/3.14159)/((x-[1]-[4])**2+([5]*[2])**2)+[6]*([8]*[2]/3.14159)/((x-[1]-[7])**2+([8]*[2])**2))",fitmin,fitmax);//  
  FRes.SetParLimits(0,0,1e6);
  FRes.SetParLimits(1,fitmin,fitmax);
  FRes.SetParLimits(2,.0001,.2);
  FRes.SetParLimits(3,.001,10);
  FRes.SetParLimits(4,-.02,.02);
  FRes.SetParLimits(5,.1,10);
  FRes.SetParLimits(6,.001,10);
  FRes.SetParLimits(7,-.02,.02);
  FRes.SetParLimits(8,.1,10);    
//   FRes.FixParameter(1,.0000045);
//   FRes.FixParameter(3,.044);  
//   FRes.FixParameter(4,-0.0041);  
//   FRes.FixParameter(5,2.03);   
//   FRes.FixParameter(6,0.167);
//   FRes.FixParameter(7,0.00026);
//   FRes.FixParameter(8,0.63);



  FRes.SetNpx(10000);
  FRes.SetLineWidth(2);
  
  TH1F HDstPiRes("HDstPiRes","HDstPiRes",nbins,fitmin,fitmax);
  ReducedNtuple->Draw("dstpimass-(dstpimdm-dstpidmres)>>HDstPiRes");
  Canvas.Clear();
  HDstPiRes.GetYaxis()->SetRangeUser(0,HDstPiRes.GetMaximum()*1.2);
  HDstPiRes.Draw("pe");
  Canvas.Print(filename);

  TH1F HDstPiDMRes("HDstPiDMRes","HDstPiDMRes",nbins,fitmin,fitmax);
  ReducedNtuple->Draw("dstpidmres>>HDstPiDMRes");
  HDstPiDMRes.GetXaxis()->SetNdivisions(5);
  SetHistoXYLabels(&HDstPiDMRes,"m_{reco.}-m_{gen.}","GeV/c^{2}");
  Canvas.Clear();
  HDstPiDMRes.Draw("pe");
  Canvas.Print(filename);
  //HDstPiDMRes.Write();

  TH1F HDstPiDMResSlice("HDstPiDMResSlice","HDstPiDMResSlice",nbins,fitmin,fitmax);
  ReducedNtuple->Draw("dstpidmres>>HDstPiDMResSlice","2.41<dstpimdm-dstpidmres&&dstpimdm-dstpidmres<2.44");
  HDstPiDMResSlice.Fit(&FRes,"QI","pe",fitmin,fitmax); 
  HDstPiDMResSlice.GetXaxis()->SetNdivisions(7);
  SetHistoXYLabels(&HDstPiDMResSlice,"m_{reco.}-m_{gen.}","GeV/c^{2}");
  Canvas.Clear();
  HDstPiDMResSlice.Draw("pe");
  Canvas.SetLogy(1);
  Canvas.Print(filename);
  Canvas.SetLogy(1);
  //HDstPiDMResSlice.Write();

  FRes.FixParameter(1,FRes.GetParameter(1));
  FRes.FixParameter(3,FRes.GetParameter(3));  
  FRes.FixParameter(4,FRes.GetParameter(4));  
  FRes.FixParameter(5,FRes.GetParameter(5));   
  FRes.FixParameter(6,FRes.GetParameter(6));
  FRes.FixParameter(7,FRes.GetParameter(7));
  FRes.FixParameter(8,FRes.GetParameter(8));

//   Canvas.Clear();
//   HDstPiDMResSlice.Fit(&FRes2,"QI","pe",HDstPiDMResSlice.GetMean()-HDstPiDMResSlice.GetRMS()*4,HDstPiDMResSlice.GetMean()+HDstPiDMResSlice.GetRMS()*4); 
//   HDstPiDMResSlice.Draw("pe");
//   Canvas.Print(filename);
//   Canvas.SetLogy(1);
//   //HDstPiDMResSlice.Write();
  
  
  text.SetTextSize(.09);
  Int_t pgood=0; 
  Canvas.Clear();
  Canvas.Divide(3,npoints/3);
  Int_t status=0;
  for(Int_t p=0;p<npoints;p++){
    Canvas.cd(p+1);    
    Canvas.GetPad(p+1)->SetLogy(1);
    sprintf(textlabel,"%.5f<dstpimdm-dstpidmres&&dstpimdm-dstpidmres<%.5f",dmmin+p*dmstep,dmmin+(p+1)*dmstep);
    label=TString("HDstPiDMResVsM")+(long)p+"_"+textlabel;    
    HDstPiDMResVsM[p]=new TH1F(label,"",nbins,fitmin,fitmax); 
    ReducedNtuple->Draw("dstpidmres>>"+label,textlabel);
    Mean[p]=HDstPiDMResVsM[p]->GetMean();
    status=HDstPiDMResVsM[p]->Fit(&FRes,"QI","pe",
			   HDstPiDMResVsM[p]->GetMean()-HDstPiDMResVsM[p]->GetRMS()*10,
			   HDstPiDMResVsM[p]->GetMean()+HDstPiDMResVsM[p]->GetRMS()*10); 

    Float_t mean;
    Float_t fwhm;
    Float_t xmax;
    DetermineFWHM(&FRes,&fwhm,&mean,&xmax);
    fwhm=fwhm/2;//plot half width

    if(HDstPiDMResVsM[p]->Integral()>500){
      GAvgVsM.SetPoint(pgood,(dmmin+p*dmstep+dmmin+(p+1)*dmstep)/2,Mean[p]*1000);
      //GAvgVsM.SetPointError(pgood,0,fprim->GetParError(1)*1000);
      GResVsM.SetPoint(pgood,(dmmin+p*dmstep+dmmin+(p+1)*dmstep)/2,1000*fwhm);
      GResVsM.SetPointError(pgood,0,1000*fwhm*(FRes.GetParError(2)/FRes.GetParameter(2)));
      GMeanVsM.SetPoint(pgood,(dmmin+p*dmstep+dmmin+(p+1)*dmstep)/2,1000*FRes.GetParameter(1));
      GMeanVsM.SetPointError(pgood,0,1000*FRes.GetParError(1));
      GSigma1VsM.SetPoint(pgood,(dmmin+p*dmstep+dmmin+(p+1)*dmstep)/2,1000*FRes.GetParameter(2));
      GSigma1VsM.SetPointError(pgood,0,1000*FRes.GetParError(2));
      GRatioVsM.SetPoint(pgood,(dmmin+p*dmstep+dmmin+(p+1)*dmstep)/2,FRes.GetParameter(3));
      GRatioVsM.SetPointError(pgood,0,FRes.GetParError(3));
      GMean2VsM.SetPoint(pgood,(dmmin+p*dmstep+dmmin+(p+1)*dmstep)/2,1000*FRes.GetParameter(4));
      GMean2VsM.SetPointError(pgood,0,1000*FRes.GetParError(4));
      GSigma2VsM.SetPoint(pgood,(dmmin+p*dmstep+dmmin+(p+1)*dmstep)/2,FRes.GetParameter(5));
      GSigma2VsM.SetPointError(pgood,0,FRes.GetParError(5));      
      
      GRatio2VsM.SetPoint(pgood,(dmmin+p*dmstep+dmmin+(p+1)*dmstep)/2,FRes.GetParameter(6));
      GRatio2VsM.SetPointError(pgood,0,FRes.GetParError(6));
      GMean3VsM.SetPoint(pgood,(dmmin+p*dmstep+dmmin+(p+1)*dmstep)/2,1000*FRes.GetParameter(7));
      GMean3VsM.SetPointError(pgood,0,1000*FRes.GetParError(7));
      GSigma3VsM.SetPoint(pgood,(dmmin+p*dmstep+dmmin+(p+1)*dmstep)/2,FRes.GetParameter(8));
      GSigma3VsM.SetPointError(pgood,0,FRes.GetParError(8));      

      GChisVsM.SetPoint(pgood,(dmmin+p*dmstep+dmmin+(p+1)*dmstep)/2,FRes.GetChisquare()/FRes.GetNDF());

      pgood++;      
    }



    HDstPiDMResVsM[p]->Draw("pe");
    Canvas.Update();
    TPaveStats*stats=(TPaveStats*)HDstPiDMResVsM[p]->FindObject("stats");
    if(stats){
      stats->SetX1NDC(.6);stats->SetY1NDC(.2);
      stats->SetX2NDC(.78);stats->SetY2NDC(.82);
    }
    //SetHistoXYLabels(HDstPiDMResVsM[p],"","");
    HDstPiDMResVsM[p]->GetXaxis()->SetNdivisions(5,1);
    HDstPiDMResVsM[p]->GetXaxis()->SetLabelSize(.08);
    HDstPiDMResVsM[p]->GetYaxis()->SetNdivisions(7,0);
    HDstPiDMResVsM[p]->GetYaxis()->SetLabelSize(.07);
    HDstPiDMResVsM[p]->GetYaxis()->SetTitleSize(.08);
    HDstPiDMResVsM[p]->GetYaxis()->SetTitleOffset(.65);
    HDstPiDMResVsM[p]->SetStats(0);
    HDstPiDMResVsM[p]->SetTitle("");
    HDstPiDMResVsM[p]->GetYaxis()->SetTitle("Entries/0.5 MeV");
    HDstPiDMResVsM[p]->GetXaxis()->SetTitle("m_{reco}-m_{gen}  (GeV)");
    HDstPiDMResVsM[p]->Draw("pe");	
    sprintf(textlabel,"M=[%.2f,%.2f]",dmmin+p*dmstep,dmmin+(p+1)*dmstep);
    text.DrawTextNDC(.0,.9,textlabel);
    //sprintf(textlabel,"HWHM=%.2f MeV",fwhm*1000);
    //text.DrawTextNDC(.45,.9,textlabel);     
    cout<<p<<" done"<<" "<<fwhm<<"  status="<<status<<" chisq/ndf="<<FRes.GetChisquare()/FRes.GetNDF()<<" "<<HDstPiDMResVsM[p]->Integral()<<endl;
    cout<<p<<" "<<FRes.GetParameter(1)<<" "<<FRes.GetParameter(2)<<" "<<FRes.GetParameter(3)<<" "<<FRes.GetParameter(4)<<" "<<FRes.GetParameter(5)<<" "<<FRes.GetParameter(6)<<" "<<FRes.GetParameter(7)<<" "<<FRes.GetParameter(8)<<endl;
  }  
  Canvas.Print(filename);
  Canvas.SetLogy(0);

  for(Int_t p=0;p<npoints;p++)
    delete  HDstPiDMResVsM[p];    


  Float_t polfitmin=dmmin;
  Float_t polfitmax=dmmax;
  TF1 FPol("FPol","[0]+[1]*x",polfitmin,polfitmax);
  FPol.SetParameters(1,2);

    
  //
  Canvas.Clear();
  GResVsM.SetMarkerSize(1.5);
  GResVsM.GetYaxis()->SetRangeUser(0,6);
  //GResVsM.GetXaxis()->SetRangeUser(dmmin,dmmax);
  GResVsM.GetXaxis()->SetTitle("m(D*#pi) (GeV/c^2)");
  GResVsM.GetYaxis()->SetTitle("HWHM (MeV)");
  FPol.SetParameters(-8,4.);
  GResVsM.Fit(&FPol,"Q","ape",polfitmin,polfitmax);
  GResVsM.Draw("ape");
  Canvas.Print(filename);


  Canvas.Clear();
  GAvgVsM.Draw("ape");
  GAvgVsM.SetMarkerSize(1.5);
  //  GAvgVsM.GetYaxis()->SetRangeUser(-.5,.5);
  GAvgVsM.GetXaxis()->SetTitle("m(D*#pi) (GeV/c^2)");
  GAvgVsM.GetYaxis()->SetTitle("Mean (MeV)");
  FPol.SetParameters(1,2);
  GAvgVsM.Fit(&FPol,"Q","ape",polfitmin,polfitmax);
  Canvas.Print(filename);


  Canvas.Clear();
  GMeanVsM.Draw("ape");
  GMeanVsM.SetMarkerSize(1.5);
  //GMeanVsM.GetYaxis()->SetRangeUser(-.5,.5);
  GMeanVsM.GetXaxis()->SetTitle("m(D*#pi) (GeV/c^2)");
  GMeanVsM.GetYaxis()->SetTitle("mean1 (MeV)");
  FPol.SetParameters(1,2);
  GMeanVsM.Fit(&FPol,"Q","ape",polfitmin,polfitmax);
  Canvas.Print(filename);

  Canvas.Clear();
  GSigma1VsM.Draw("ape");
  GSigma1VsM.SetMarkerSize(1.5);
  //GSigma1VsM.GetYaxis()->SetRangeUser(0,5);
  GSigma1VsM.GetXaxis()->SetTitle("m(D*#pi) (GeV/c^{2})");
  GSigma1VsM.GetYaxis()->SetTitle("sigma1 (MeV)");
  GSigma1VsM.Fit(&FPol,"Q","ape",polfitmin,polfitmax);
  Canvas.Print(filename);

  Canvas.Clear();
  GRatioVsM.Draw("ape");
  GRatioVsM.SetMarkerSize(1.5);
  //GRatioVsM.GetYaxis()->SetRangeUser(0,1);
  GRatioVsM.GetXaxis()->SetTitle("m(D*#pi) (GeV/c^2)");
  GRatioVsM.GetYaxis()->SetTitle("Norm2/Norm1");
  FPol.SetParameters(1,2);
  GRatioVsM.Fit(&FPol,"Q","ape",polfitmin,polfitmax);
  Canvas.Print(filename); 

  Canvas.Clear();
  GMean2VsM.Draw("ape");
  GMean2VsM.SetMarkerSize(1.5);
  //GMean2VsM.GetYaxis()->SetRangeUser(-1.5,.5);
  GMean2VsM.GetXaxis()->SetTitle("m(D*#pi) (GeV/c^2)");
  GMean2VsM.GetYaxis()->SetTitle("mean2 (MeV)");
  FPol.SetParameters(1,2);
  GMean2VsM.Fit(&FPol,"Q","ape",polfitmin,polfitmax);
  Canvas.Print(filename);

  Canvas.Clear();
  GSigma2VsM.Draw("ape");
  GSigma2VsM.SetMarkerSize(1.5);
  //GSigma2VsM.GetYaxis()->SetRangeUser(1,3);
  GSigma2VsM.GetXaxis()->SetTitle("m(D*#pi) (GeV/c^2)");
  GSigma2VsM.GetYaxis()->SetTitle("sigma2/sigma1 (MeV)");
  FPol.SetParameters(1,2);
  GSigma2VsM.Fit(&FPol,"Q","ape",polfitmin,polfitmax);
  Canvas.Print(filename);


  Canvas.Clear();
  GRatio2VsM.Draw("ape");
  GRatio2VsM.SetMarkerSize(1.5);
  //GRatio2VsM.GetYaxis()->SetRangeUser(0,1);
  GRatio2VsM.GetXaxis()->SetTitle("m(D*#pi) (GeV/c^2)");
  GRatio2VsM.GetYaxis()->SetTitle("Norm3/Norm1");
  FPol.SetParameters(1,2);
  GRatio2VsM.Fit(&FPol,"Q","ape",polfitmin,polfitmax);
  Canvas.Print(filename); 

  Canvas.Clear();
  GMean3VsM.Draw("ape");
  GMean3VsM.SetMarkerSize(1.5);
  //GMean3VsM.GetYaxis()->SetRangeUser(-1.5,.5);
  GMean3VsM.GetXaxis()->SetTitle("m(D*#pi) (GeV/c^2)");
  GMean3VsM.GetYaxis()->SetTitle("mean3 (MeV)");
  FPol.SetParameters(1,2);
  GMean3VsM.Fit(&FPol,"Q","ape",polfitmin,polfitmax);
  Canvas.Print(filename);
  
  Canvas.Clear();
  GSigma3VsM.Draw("ape");
  GSigma3VsM.SetMarkerSize(1.5);
  //GSigma3VsM.GetYaxis()->SetRangeUser(1,3);
  GSigma3VsM.GetXaxis()->SetTitle("m(D*#pi) (GeV/c^2)");
  GSigma3VsM.GetYaxis()->SetTitle("sigma3/sigma1 (MeV)");
  FPol.SetParameters(1,2);
  GSigma3VsM.Fit(&FPol,"Q","ape",polfitmin,polfitmax);
  Canvas.Print(filename);

 
  Canvas.Clear();
  GChisVsM.Draw("ape");
  GChisVsM.SetMarkerSize(1.5);
  GChisVsM.GetYaxis()->SetRangeUser(0,3);
  GChisVsM.GetXaxis()->SetTitle("m(D*#pi) (GeV/c^2)");
  GChisVsM.GetYaxis()->SetTitle("#chi^{2}/ndf");
  Canvas.Print(filename);


  Canvas.Print(filename+"]");
  if(CloseReducedNtuple()!=1)return 0;

  return 1;
}



Int_t DstPiAnalysis::CreateHistosForFit(){
 
  
  if(!OpenReducedNtuple())return 0;
  
  ReducedNtuple->SetBranchAddress("eventnumber",&eventnumber);
  ReducedNtuple->SetBranchAddress("dstpideltam",&dstpideltam);
  ReducedNtuple->SetBranchAddress("dstpimdm",&dstpimdm);
  ReducedNtuple->SetBranchAddress("dstpipstar",&dstpipstar);
  ReducedNtuple->SetBranchAddress("dstpimctrue",&dstpimctrue);
  ReducedNtuple->SetBranchAddress("dstarcostheta",&dstarcostheta);
  ReducedNtuple->SetBranchAddress("dstarhelicity",&dstarhelicity);
  ReducedNtuple->SetBranchAddress("d0mass",&d0mass);
  ReducedNtuple->SetBranchAddress("dstardeltam",&dstardeltam);
  //
  TH1F HDMass("HDMass","",HistDMassBins,HistDMassMin,HistDMassMax);
  TH1F HDstDM("HDstDM","",HistDstDMBins,HistDstDMMin,HistDstDMMax);
  

  //
  TH1F HDstPi("HDstPi","",2000,2.1,4.1);
  HDstPi.GetXaxis()->SetTitle(XAxisLabel);
     
  //mass cut abs(heli)>.75
  TH1F HDstPiMD2420Helicity("HDstPiMD2420Helicity","",2000,2.1,4.1);
  HDstPiMD2420Helicity.Sumw2();
  HDstPiMD2420Helicity.GetXaxis()->SetTitle(XAxisLabel);

  //mass cut abs(heli)<.5
  TH1F HDstPiMD2460Helicity("HDstPiMD2460Helicity","",2000,2.1,4.1);
  HDstPiMD2460Helicity.Sumw2();
  HDstPiMD2460Helicity.GetXaxis()->SetTitle(XAxisLabel);


  //mass vs helicity .2 slices
  TH1F* HDstPiMVsHelicity[10];
  for(Int_t i=0;i<10;i++){
    HDstPiMVsHelicity[i]=new TH1F(TString("HDstPiMVsHelicity")+(long)i,"",2000,2.1,4.1);
    HDstPiMVsHelicity[i]->Sumw2();
    HDstPiMVsHelicity[i]->GetXaxis()->SetTitle(XAxisLabel);

  }  

  //mass vs helicity .1 slices
  TH1F* HDstPiMVsHelicityFine[20];
  for(Int_t i=0;i<20;i++){
    HDstPiMVsHelicityFine[i]=new TH1F(TString("HDstPiMVsHelicityFine")+(long)i,"",2000,2.1,4.1);
    HDstPiMVsHelicityFine[i]->Sumw2();
    HDstPiMVsHelicityFine[i]->GetXaxis()->SetTitle(XAxisLabel);

  }  

  // mass vs p* for crossections  
  TH1F* HDstPiMVsPstarFine[20];
  for(Int_t i=0;i<20;i++){
    HDstPiMVsPstarFine[i]=new TH1F(TString("HDstPiMVsPstarFine")+(long)i,"",2000,2.1,4.1);
    HDstPiMVsPstarFine[i]->Sumw2();
    HDstPiMVsPstarFine[i]->GetXaxis()->SetTitle(XAxisLabel);
  }   

  // mass vs cos Dstar  
  TH1F* HDstPiMVsCosDstar[20];
  for(Int_t i=0;i<20;i++){
    HDstPiMVsCosDstar[i]=new TH1F(TString("HDstPiMVsCosDstar")+(long)i,"",2000,2.1,4.1);
    HDstPiMVsCosDstar[i]->Sumw2();
    HDstPiMVsCosDstar[i]->GetXaxis()->SetTitle(XAxisLabel);
  }
 


  //2D Helicity vs mass
  TH2F H2DstPiMVsDstarHel("H2DstPiMVsDstarHel","",2000,2.1,4.1,20,-1.0,1.0);
  H2DstPiMVsDstarHel.Sumw2();
  H2DstPiMVsDstarHel.GetYaxis()->SetTitle("cos(#theta_{H})");
  H2DstPiMVsDstarHel.GetXaxis()->SetTitle(XAxisLabel);



  ///
  Int_t e=0;
  while(ReducedNtuple->GetEntry(e)){
    e++;
    if(e%1000==0)cout<<e<<" cands done"<<endl; 
    

    HDstPi.Fill(dstpimdm);   
    H2DstPiMVsDstarHel.Fill(dstpimdm,dstarhelicity);
    HDMass.Fill(d0mass);
    HDstDM.Fill(dstardeltam);


    if(fabs(dstarhelicity)>.75)
      HDstPiMD2420Helicity.Fill(dstpimdm);
    if(fabs(dstarhelicity)<.5)
      HDstPiMD2460Helicity.Fill(dstpimdm);
  

    //
    for(Int_t i=0;i<10;i++){
      if((-1+.2*i)<dstarhelicity&&dstarhelicity<(-1+.2*(i+1))){
	HDstPiMVsHelicity[i]->Fill(dstpimdm);
      }
    }
    for(Int_t i=0;i<20;i++){
      if((-1+.1*i)<dstarhelicity&&dstarhelicity<(-1+.1*(i+1)))
	HDstPiMVsHelicityFine[i]->Fill(dstpimdm);
    }


    //p* dependence
    for(Int_t i=0;i<20;i++)
      if((3+.1*i)<dstpipstar&&dstpipstar<(3+.1*(i+1)))
	HDstPiMVsPstarFine[i]->Fill(dstpimdm);

    //cos D*
    for(Int_t i=0;i<20;i++)
      if((-1+.1*i)<dstarcostheta&&dstarcostheta<(-1.+.1*(i+1)))
	HDstPiMVsCosDstar[i]->Fill(dstpimdm);


  }
  if(CloseReducedNtuple()!=1)return 0;


  TFile HistosForFit(_OutputDir+"/HistosForFit.root","recreate");  


  HDstPi.Write();
  HDstPiMD2420Helicity.Write();
  HDstPiMD2460Helicity.Write();
  HDMass.Write();  
  HDstDM.Write();  

  for(Int_t i=0;i<10;i++)
    HDstPiMVsHelicity[i]->Write();
  for(Int_t i=0;i<20;i++)
    HDstPiMVsHelicityFine[i]->Write();

  for(Int_t i=0;i<20;i++)
    HDstPiMVsPstarFine[i]->Write();

  for(Int_t i=0;i<20;i++)
    HDstPiMVsCosDstar[i]->Write();


  H2DstPiMVsDstarHel.Write();


  HistosForFit.ls();
  HistosForFit.Close();


  //-------------
  //clean up  
  for(Int_t i=0;i<10;i++)
    delete HDstPiMVsHelicity[i];  
  for(Int_t i=0;i<20;i++)
    delete HDstPiMVsHelicityFine[i];

  for(Int_t i=0;i<10;i++)
    delete HDstPiMVsPstarFine[i];

  for(Int_t i=0;i<20;i++)
    delete HDstPiMVsCosDstar[i];


 
  return 1;
}

#include "../RooDwaveBreitWigner.h" 
#include "../RooConvolutionPdf.h"
#include "../RooNRBreitWigner.h" 
#include "../RooSwaveBreitWigner.h" 

Int_t DstPiAnalysis::FitMassHeliCut(){
  TString HName="HDstPiMD2420Helicity";

  TFile file(_OutputDir+"/HistosForFit.root","read");

  TH1F*HMass=(TH1F*)file.Get(HName);   
  if(!HMass){cout<<"no histo"<<endl;return 0;}
  HMass->SetStats(0);
  
  if(!massfitter.Initialize(HMass,_OutputDir+"/InitRooFitParsHeliCut.txt"))return 0;
  if(massfitter.Fit()<0)return 0;
  massfitter.SetFileTag(TString("_D2420Helicity"));
  massfitter.Save(_OutputDir);
  

  file.Close();
  return 1;
}

Int_t DstPiAnalysis::FitMassHeliCut2(){
  TString HName="HDstPiMD2460Helicity";

  TFile file(_OutputDir+"/HistosForFit.root","read");

  TH1F*HMass=(TH1F*)file.Get(HName);   
  if(!HMass){cout<<"no histo"<<endl;return 0;}
  HMass->SetStats(0);
  
  if(!massfitter.Initialize(HMass,_OutputDir+"/InitRooFitParsHeliCut2.txt"))return 0;
  if(massfitter.Fit()<0)return 0;
  massfitter.SetFileTag(TString("_D2460Helicity"));
  massfitter.Save(_OutputDir);


  file.Close();
  return 1;
}


Int_t DstPiAnalysis::FitMass(){
  TString HName="HDstPi";

  TFile file(_OutputDir+"/HistosForFit.root","read");
  gROOT->cd();

  TH1F*HMass=(TH1F*)file.Get(HName);  
 
  if(!HMass){cout<<"no histo"<<endl;return 0;}
  HMass->SetStats(0);
  
  if(!massfitter.Initialize(HMass,_OutputDir+"/InitRooFitPars.txt"))return 0;
  if(massfitter.Fit()<0)return 0;
  massfitter.SetFileTag(TString("_Final"));
  massfitter.Save(_OutputDir);
  

  file.Close();
  return 1;
}


Int_t DstPiAnalysis::FitMassVsPstar(){
  

  TFile file(_OutputDir+"/HistosForFit.root","read");
  //file.ls();
  gROOT->cd();

  for(Int_t i=0;i<20;i++){
    TH1F*HMass=(TH1F*)file.Get(TString("HDstPiMVsPstarFine")+(long)i);
    if(!HMass){cout<<"no histo"<<endl;return 0;}
    
    SetHistoXYLabels(HMass,"M(D^{*+}#pi^{-})","GeV/c^{2}");
    HMass->SetStats(0);
  

    MassFitter Fitter(&mass);
    Fitter.SetResolution(respdf,&resVar);
    Fitter.SetPhaseSpace(&TwoBodyPS);
    Fitter.SetDauMasses(&dau1m,&dau2m);
    if(!Fitter.Initialize(HMass,_OutputDir+"/PstarFits/InitRooFitPars.txt"))return 0;
    if(Fitter.Fit()<0)return 0;
    Fitter.SetFileTag(TString("_")+(long)i);
    Fitter.Save(_OutputDir+"/PstarFits");
  } 
  
  file.Close();
 
  return 1;
}


Int_t DstPiAnalysis::PlotHelicityEfficiency(){
  TFile file("DstarCharged/DstarPi/MC/Signal/D2420andD2460/KpiAndK3pi/Ntuple.root","read");
  
  
  TH1F* HGen=(TH1F*)file.Get("HMCDstarHelicity");
  TTree* tree=(TTree*)file.Get("Ntuple");
  if(!HGen||!tree)return 0;

  TH1F HReco("HReco","",HGen->GetXaxis()->GetNbins(),HGen->GetXaxis()->GetXmin(),HGen->GetXaxis()->GetXmax());
  HReco.GetXaxis()->SetTitle("cos(#theta_{H})");
  HReco.GetYaxis()->SetTitle("Events / 0.05 ");
  tree->Draw("dstarhelicity>>HReco","dstpimctrue==1");

  
  filename=_OutputDir+"/HelicityFitsNewSig2/HelicityEfficiency.ps";
  Canvas.Print(filename+"[");
  
  Canvas.Draw();
  HGen->GetYaxis()->SetRangeUser(0,HGen->GetMaximum()*1.2);
  HGen->Draw();
  HReco.Draw("pesame");
  Canvas.Print(filename);

  HReco.Sumw2();
  HReco.Divide(HGen);
  delete HReco.GetListOfFunctions()->FindObject("stats");
  Canvas.Draw();
  HReco.Fit("pol1");
  HReco.GetYaxis()->SetRangeUser(0,.2);
  HReco.GetYaxis()->SetTitle("efficiency");
  HReco.Draw("pe");
  Canvas.Print(filename);


  Canvas.Print(filename+"]");

  return 1;
}

Int_t DstPiAnalysis::FitMassVsHelicity(){

  Int_t  NBINS=20;
  TFile histosfile(_OutputDir+"/HistosForFit.root","read");
  gROOT->cd();

  TFile file(_OutputDir+"/HelicityFits/FitMassVsHelicity.root","recreate");
  RooPlot* plot=NULL;
  for(Int_t binindex=0;binindex<NBINS;binindex++){
    system(TString("rm -f ")+_OutputDir+"/HelicityFits/FitPars_Helicity"+(long)(binindex)+".txt");
    system(TString("rm -f ")+_OutputDir+"/HelicityFits/FitPars_Helicity"+(long)(binindex)+".ps");
    system(TString("rm -f ")+_OutputDir+"/HelicityFits/FitPars_Helicity"+(long)(binindex)+".root");
      
    TH1F*HDCPiMass=(TH1F*)histosfile.Get(TString("HDstPiMVsHelicityFine")+(long)binindex); 
    if(!HDCPiMass){cout<<"no histo"<<endl;return 0;}
    SetHistoXYLabels(HDCPiMass,"m(D*#pi)","GeV/c^{2}");
    HDCPiMass->SetStats(0);      
    if(!massfitter.Initialize(HDCPiMass,_OutputDir+"/HelicityFits/InitRooFitPars.txt"))return 0;
    if(massfitter.Fit()<0)return 0;
    massfitter.SetFileTag(TString("_Helicity")+(long)binindex);
    massfitter.Save(_OutputDir+"/HelicityFits");
    plot=massfitter.GetFitPlot();
    file.cd();
    plot->SetName(TString("plot")+(long)binindex);
    plot->Write();
    delete plot;
     
    cout<<"fit done "<<binindex<<endl;    
  }

  histosfile.Close();
  file.ls();
  file.Close();

  return 1;
}



Int_t DstPiAnalysis::PlotMassVsHelicity(){


  Int_t  NBINS=20;
  Float_t minheli=-1;

  Float_t D2420yield;
  Float_t D2460yield;
  Float_t D2420yieldEr;
  Float_t D2460yieldEr;
  TH1F HD2420VsHelicity("HD2420VsHelicity","HD2420VsHelicity",NBINS,minheli,1);
  HD2420VsHelicity.GetXaxis()->SetTitle("cos#theta_{H}"); 
  HD2420VsHelicity.GetYaxis()->SetTitle("Yield / .1");  
  TH1F HD2460VsHelicity("HD2460VsHelicity","HD2460VsHelicity",NBINS,minheli,1);
  HD2460VsHelicity.GetXaxis()->SetTitle("cos#theta_{H}");  
  HD2460VsHelicity.GetYaxis()->SetTitle("Yield / .1"); 

  filename=_OutputDir+"/HelicityFits/FitVsHelicity.ps";
  Canvas.Print(filename+"[");
  

  TFile file(_OutputDir+"/HelicityFits/FitMassVsHelicity.root","update");
  gROOT->cd();

  RooPlot* plot=NULL;

  Canvas.Clear();
  Canvas.Divide(4,NBINS/4);
  
  for(Int_t binindex=0;binindex<NBINS;binindex++){
    plot=(RooPlot*)file.Get(TString("plot")+(long)(binindex));
    if(!plot)continue;
  
    Canvas.GetPad(binindex+1)->SetRightMargin(.2);
    Canvas.GetPad(binindex+1)->SetLeftMargin(.1);
    Canvas.GetPad(binindex+1)->SetTopMargin(.1);
    Canvas.GetPad(binindex+1)->SetBottomMargin(.1);
    Canvas.cd(binindex+1);
    plot->SetTitle("");
    plot->SetTitleSize(.04);
    plot->GetYaxis()->SetTitle("");
    plot->GetYaxis()->SetLabelSize(.08);
    plot->GetYaxis()->SetNdivisions(5);
    plot->GetXaxis()->SetTitle("");
    plot->GetXaxis()->SetLabelSize(.12);
    plot->GetXaxis()->SetNdivisions(5);
    ((RooHist*)plot->getObject(0))->SetMarkerSize(.01);
    plot->Draw();
    text.SetTextSize(.15);
    text.DrawTextNDC(.7,.75,TString("")+(long)(binindex+1));

    cout<<"done plot "<<binindex<<endl;
      

  }
 
  Canvas.Print(filename);
 
 
 
  for(Int_t binindex=0;binindex<NBINS;binindex++){
        
    if(!inputpars.OpenFitPars(_OutputDir+"/HelicityFits/FitPars_Helicity"+(long)(binindex)+".txt"))continue;
    

    D2420yield=inputpars.GetFitPar("D2420","yield");
    D2420yieldEr=inputpars.GetFitPar("D2420","yielderror");  
    HD2420VsHelicity.SetBinContent(binindex+1,D2420yield);//round to nearest integer
    HD2420VsHelicity.SetBinError(binindex+1,sqrt(pow(D2420yieldEr,2)+pow(.0*D2420yield,2)));//include error due to the width error E~2*pi*E_Gamma/Gamma**2

    D2460yield=inputpars.GetFitPar("D2460","yield");
    D2460yieldEr=inputpars.GetFitPar("D2460","yielderror");    
    HD2460VsHelicity.SetBinContent(binindex+1,D2460yield);
    HD2460VsHelicity.SetBinError(binindex+1,sqrt(pow(D2460yieldEr,2)+pow(.0*D2460yield,2)));
    
  
    inputpars.CloseFitPars();

    cout<<"read file"<<binindex<<endl;

  }
  file.cd();
  file.Delete("HD2420VsHelicity;1");
  HD2420VsHelicity.SetName("HD2420VsHelicity");
  HD2420VsHelicity.Write();
  file.Delete("HD2460VsHelicity;1");
  HD2460VsHelicity.SetName("HD2460VsHelicity");
  HD2460VsHelicity.Write();
  file.Close();

  Canvas.Clear();
  HD2420VsHelicity.SetMarkerSize(1.5);
  HD2420VsHelicity.SetMarkerColor(1);
  HD2420VsHelicity.SetLineColor(1);
  HD2460VsHelicity.SetMarkerSize(1.5);
  HD2460VsHelicity.SetMarkerColor(4);
  HD2460VsHelicity.SetLineColor(4);   
  HD2420VsHelicity.GetYaxis()->SetRangeUser(0,HD2420VsHelicity.GetMaximum()*1.1);
  HD2420VsHelicity.SetStats(0);
  HD2460VsHelicity.SetStats(0);
  HD2420VsHelicity.Draw("pe");
  HD2460VsHelicity.Draw("pesame");
  Canvas.Print(filename);
  HD2420VsHelicity.SetStats(1);
  HD2460VsHelicity.SetStats(1);
  HD2420VsHelicity.Draw("pe");
  HD2460VsHelicity.Draw("pesame");

  
  /////////////Fit the ratios
  TStyle* gStyle=gROOT->GetStyle("myStyle");
  Int_t optstat=gStyle->GetOptStat();
  gStyle->SetOptStat("i");
  gStyle->SetPadRightMargin(0.22);  

  TF1 FSinCos("FSinCos","[0]+[1]*x**2",minheli,1);
  FSinCos.SetParameters(1,2);
  FSinCos.SetParNames("A","B");
  
  
  HD2420VsHelicity.Fit(&FSinCos,"I","pe",minheli,1);
  TPaveStats*stats=(TPaveStats*)HD2420VsHelicity.FindObject("stats");
  if(!stats){cout<<"no stats D2420"<<endl;return 0;}
  stats->SetX1NDC(.7);
  stats->SetX2NDC(.95);
  stats->SetY1NDC(.5);
  stats->SetY2NDC(.8);
  stats->SetFillColor(0);
  stats->SetFillStyle(1001);
  stats->SetLineColor(1);
  stats->SetLineStyle(1001);
  stats->SetTextColor(1);
  stats->SetBorderSize(2);

  HD2460VsHelicity.Fit(&FSinCos,"I","pe",minheli,1); 
  Canvas.Update();
  stats=(TPaveStats*)HD2460VsHelicity.FindObject("stats");
  if(!stats){cout<<"no stats D2460"<<endl;return 0;}
  stats->SetX1NDC(.7);
  stats->SetX2NDC(.95);
  stats->SetY1NDC(.2);
  stats->SetY2NDC(.5);
  stats->SetFillColor(0);
  stats->SetFillStyle(1001);
  stats->SetLineColor(4);
  stats->SetLineStyle(1001);
  stats->SetTextColor(4);
  stats->SetBorderSize(2);

  Canvas.SetRightMargin(.3);
  Canvas.Clear();
  //HD2420VsHelicity.SetTitle("");
  HD2420VsHelicity.GetXaxis()->SetTitle("cos#theta_{H}");  
  //HD2420VsHelicity.GetYaxis()->SetRangeUser(0,HD2420VsHelicity.GetBinContent(HD2420VsHelicity.GetMaximumBin())*1.1);
  HD2420VsHelicity.SetStats(1);
  HD2460VsHelicity.SetStats(1);
  HD2420VsHelicity.Draw("pe");
  HD2460VsHelicity.Draw("pesame");
  Canvas.Print(filename);


  gStyle->SetOptStat(optstat);//restore Stats
  gStyle->SetPadRightMargin(0.20);  


  Canvas.Print(filename+"]");
 

  return 1;
}


Int_t DstPiAnalysis::FitMassVsHelicityNewSig(){

  Int_t  NBINS=10;
  TFile histosfile(_OutputDir+"/HistosForFit.root","read");
  gROOT->cd();
  cout<<_OutputDir<<endl;

  TFile file(_OutputDir+"/HelicityFitsNewSig/FitMassVsHelicity.root","recreate");
  RooPlot* plot=NULL;
  for(Int_t binindex=0;binindex<NBINS;binindex++){
    system(TString("rm -f ")+_OutputDir+"/HelicityFitsNewSig/FitPars_Helicity"+(long)(binindex)+".txt");
    system(TString("rm -f ")+_OutputDir+"/HelicityFitsNewSig/FitPars_Helicity"+(long)(binindex)+".ps");
    system(TString("rm -f ")+_OutputDir+"/HelicityFitsNewSig/FitPars_Helicity"+(long)(binindex)+".root");
      
    TH1F*HDCPiMass=(TH1F*)histosfile.Get(TString("HDstPiMVsHelicity")+(long)binindex); 
    if(!HDCPiMass){cout<<"no histo"<<endl;return 0;}
    SetHistoXYLabels(HDCPiMass,"M(D^{_{*}+}#pi^{-})","GeV/c^{2}");
    HDCPiMass->SetStats(0);      
    if(!massfitter.Initialize(HDCPiMass,_OutputDir+"/HelicityFitsNewSig/InitRooFitPars.txt")){
      cout<<" bad init file"<<endl;return 0;
    }
    if(massfitter.Fit()<0)return 0;
    massfitter.SetFileTag(TString("_Helicity")+(long)binindex);
    massfitter.Save(_OutputDir+"/HelicityFitsNewSig");

    plot=massfitter.GetFitPlot();
    TH1*hresid=massfitter.GetNormResidualHisto();
    if(plot&&hresid){
      file.cd();
      plot->SetName(TString("plot")+(long)binindex);
      plot->Write();
      delete plot;

      hresid->SetName(TString("residuals")+(long)binindex);
      hresid->Write();
      delete hresid;
    }

    
    cout<<"fit done "<<binindex<<endl;
    
  }

  histosfile.Close();
  file.ls();
  file.Close();

  return 1;
}



Int_t DstPiAnalysis::PlotMassVsHelicityNewSig(){

  Int_t  NBINS=10;
  
  filename=_OutputDir+"/HelicityFitsNewSig/FitVsHelicity.ps";
  Canvas.Print(filename+"[");
  

  TFile file(_OutputDir+"/HelicityFitsNewSig/FitMassVsHelicity.root","read");
  gROOT->cd();

  RooPlot* plot=NULL;

  Canvas.Clear();
  Canvas.Divide(2,5);
  
  for(Int_t binindex=0;binindex<NBINS;binindex++){
    plot=(RooPlot*)file.Get(TString("plot")+(long)(binindex));
    if(!plot){cout<<" no plot at b="<<binindex<<endl;return 0;}
  
    Canvas.GetPad(binindex+1)->SetRightMargin(.3);
    Canvas.GetPad(binindex+1)->SetLeftMargin(.1);
    Canvas.GetPad(binindex+1)->SetTopMargin(.1);
    Canvas.GetPad(binindex+1)->SetBottomMargin(.1);
    //TVirtualPad*pad=
    Canvas.cd(binindex+1);
    //pad->SetLeftMargin(.2);
    //pad->SetRightMargin(.2);
    //if(binindex<4)pad->SetTopMargin(.2);
    //if(binindex>=4)pad->SetBottomMargin(.2);
    plot->SetTitle("");
    plot->SetTitleSize(.04);
    //plot->GetYaxis()->SetRangeUser(0,4000);
    plot->GetYaxis()->SetTitle("");
    plot->GetYaxis()->SetLabelSize(.08);
    plot->GetYaxis()->SetNdivisions(5);
    plot->GetXaxis()->SetTitle("M(D*#pi) (GeV)");
    plot->GetXaxis()->SetLabelSize(.12);
    //plot->GetXaxis()->SetNdivisions(5);
    ((RooHist*)plot->getObject(0))->SetMarkerSize(.01);
    plot->Draw();
    text.SetTextSize(.15);
    text.DrawTextNDC(.6,.6,TString("")+(long)(binindex+1));

    cout<<"done plot "<<binindex<<endl;
      

  }
 
  Canvas.Print(filename);

  ///Plot the background subtracted fits here
  Canvas.Clear();
  Canvas.Divide(2,5); 
  TFile* fil[10];
  RooPlot* pl[10];
  //  TH1F Hfr("Hfr","",110,2.15,3.25);
  //  Hfr.GetYaxis()->SetRangeUser(-50,500);
  for(Int_t binindex=0;binindex<NBINS;binindex++){
    fil[binindex]=new TFile(_OutputDir+"/HelicityFitsNewSig/FitPars_Helicity"+(long)(binindex)+".root","read");
    gROOT->cd();
    pl[binindex]=(RooPlot*)fil[binindex]->Get("bkgsubframe");
    if(!pl[binindex]){cout<<" no plot at b="<<binindex<<endl;return 0;}


    Canvas.GetPad(binindex+1)->SetRightMargin(.3);
    Canvas.GetPad(binindex+1)->SetLeftMargin(.1);
    Canvas.GetPad(binindex+1)->SetTopMargin(.1);
    Canvas.GetPad(binindex+1)->SetBottomMargin(.1);
    Canvas.cd(binindex+1);
    pl[binindex]->SetTitle("");
    pl[binindex]->SetTitleSize(.04);
    pl[binindex]->GetYaxis()->SetTitle("");
    pl[binindex]->GetYaxis()->SetLabelSize(.08);
    pl[binindex]->GetYaxis()->SetNdivisions(5);
    //pl[binindex]->GetYaxis()->SetRangeUser(-50,500);
    //pl[binindex]->GetXaxis()->SetTitle("M(D*#pi) (GeV)");
    pl[binindex]->GetXaxis()->SetTitle("");
    pl[binindex]->GetXaxis()->SetLabelSize(.12);
    //pl[binindex]->GetXaxis()->SetNdivisions(5);
    ((RooHist*)pl[binindex]->getObject(0))->SetMarkerSize(.01);
    //Hfr.Draw();
    pl[binindex]->Draw();
    text.SetTextSize(.15);
    text.DrawTextNDC(.6,.6,TString("")+(long)(binindex+1));
    
    cout<<"done plot "<<binindex<<endl;
  }
  Canvas.Print(filename);
  for(Int_t binindex=0;binindex<NBINS;binindex++)delete fil[binindex];
  ////////


  //plot the residuals
   Canvas.Clear();
   Canvas.Divide(2,5);
   TH1*resid;
   for(Int_t binindex=0;binindex<NBINS;binindex++){
     resid=(TH1*)file.Get(TString("residuals")+(long)binindex);
     if(!resid)continue;

     Canvas.GetPad(binindex+1)->SetRightMargin(.3);
     Canvas.GetPad(binindex+1)->SetLeftMargin(.1);
     Canvas.GetPad(binindex+1)->SetTopMargin(.1);
     Canvas.GetPad(binindex+1)->SetBottomMargin(.1);
     //TVirtualPad*pad=
     Canvas.cd(binindex+1);
     resid->SetTitle("");
     resid->SetTitleSize(.04);
     resid->GetYaxis()->SetTitle("");
     resid->GetYaxis()->SetLabelSize(.08);
     resid->GetYaxis()->SetNdivisions(5);
     resid->GetXaxis()->SetTitle("");
     resid->GetXaxis()->SetLabelSize(.12);
     resid->GetXaxis()->SetNdivisions(5);
     resid->GetYaxis()->SetRangeUser(-5,5);
     resid->Draw("p");
     cutline.SetLineColor(4);
     cutline.DrawLine(resid->GetXaxis()->GetXmin(),0,resid->GetXaxis()->GetXmax(),0);
     cutline.SetLineColor(2);
     cutline.DrawLine(resid->GetXaxis()->GetXmin(),-2,resid->GetXaxis()->GetXmax(),-2);
     cutline.DrawLine(resid->GetXaxis()->GetXmin(),2,resid->GetXaxis()->GetXmax(),2);
     text.SetTextSize(.15);
     text.DrawTextNDC(.6,.75,TString("")+(long)(binindex+1));

     cout<<"done plot "<<binindex<<endl;     
   }
   Canvas.Print(filename);


  //file.Close(); 
 
  Float_t D2620yield;
  Float_t D2620yieldEr;
  TH1F HD2620VsHelicity("HD2620VsHelicity","",NBINS,-1,1);
  HD2620VsHelicity.GetXaxis()->SetTitle("cos#theta_{H}"); 
  HD2620VsHelicity.GetYaxis()->SetTitle("Yield / .2");  

  Float_t D2620mean;
  Float_t D2620meanEr;
  TH1F HD2620meanVsHelicity("HD2620meanVsHelicity","",NBINS,-1,1);
  HD2620meanVsHelicity.GetXaxis()->SetTitle("cos#theta_{H}"); 
  HD2620meanVsHelicity.GetYaxis()->SetTitle("Mass (GeV/c^{2})"); 

  Float_t D2750yield;
  Float_t D2750yieldEr;
  TH1F HD2750VsHelicity("HD2750VsHelicity","",NBINS,-1,1);
  HD2750VsHelicity.GetXaxis()->SetTitle("cos#theta_{H}"); 
  HD2750VsHelicity.GetYaxis()->SetTitle("Yield / .2"); 

  Float_t D2750mean;
  Float_t D2750meanEr;
  TH1F HD2750meanVsHelicity("HD2750meanVsHelicity","",NBINS,-1,1);
  HD2750meanVsHelicity.GetXaxis()->SetTitle("cos#theta_{H}"); 
  HD2750meanVsHelicity.GetYaxis()->SetTitle("Mass (GeV/c^{2})"); 

 
   for(Int_t binindex=0;binindex<NBINS;binindex++){
      
     if(!inputpars.OpenFitPars(_OutputDir+"/HelicityFitsNewSig/FitPars_Helicity"+(long)(binindex)+".txt"))continue;
    

     D2620yield=inputpars.GetFitPar("D2620","yield");
     D2620yieldEr=inputpars.GetFitPar("D2620","yielderror");  
     if(D2620yieldEr/D2620yield>.3)D2620yieldEr=D2620yield*.1;
     HD2620VsHelicity.SetBinContent(binindex+1,D2620yield);//round to nearest integer
     HD2620VsHelicity.SetBinError(binindex+1,sqrt(pow(D2620yieldEr,2)+pow(.0*D2620yield,2)));//include error due to the width error E~2*pi*E_Gamma/Gamma**2

     D2620mean=inputpars.GetFitPar("D2620","mean");
     D2620meanEr=inputpars.GetFitPar("D2620","meanerror");
     HD2620meanVsHelicity.SetBinContent(binindex+1,D2620mean);
     HD2620meanVsHelicity.SetBinError(binindex+1,D2620meanEr);

     D2750yield=inputpars.GetFitPar("D2750","yield");
     D2750yieldEr=inputpars.GetFitPar("D2750","yielderror");
     //if(D2750yieldEr/D2750yield>.3)D2750yieldEr=D2750yield*.1;
     HD2750VsHelicity.SetBinContent(binindex+1,D2750yield);
     HD2750VsHelicity.SetBinError(binindex+1,sqrt(pow(D2750yieldEr,2)+pow(.0*D2750yield,2)));

     D2750mean=inputpars.GetFitPar("D2750","mean");
     D2750meanEr=inputpars.GetFitPar("D2750","meanerror");
     HD2750meanVsHelicity.SetBinContent(binindex+1,D2750mean);
     HD2750meanVsHelicity.SetBinError(binindex+1,D2750meanEr);
  
     inputpars.CloseFitPars();

     cout<<"read file"<<binindex<<endl;

   }
 

  ///Draw the mass dependency
  Canvas.Clear();
  HD2620meanVsHelicity.SetTitle("");
  HD2620meanVsHelicity.Draw("pe");
  Canvas.Print(filename);
  Canvas.Clear();
  HD2750meanVsHelicity.SetTitle("");
  HD2750meanVsHelicity.Draw("pe");
  Canvas.Print(filename);


  /////////////
  TStyle* gStyle=gROOT->GetStyle("myStyle");
  Int_t optstat=gStyle->GetOptStat();
  gStyle->SetOptStat("i");
  //gStyle->SetPadRightMargin(0.22);  

  TF1 FSinCos("FSinCos","[0]+[1]*x**2",-1,1);
  FSinCos.SetParameters(1,2);
  FSinCos.SetParNames("A","B");  
  FSinCos.SetLineColor(4);


  ///-------------------------------------
  ///Draw the new Signals
  ///-------------------------------------
  Canvas.Clear();
  HD2620VsHelicity.SetTitle("");
  HD2620VsHelicity.SetMarkerSize(1.5);
  HD2620VsHelicity.SetMarkerColor(2);
  HD2620VsHelicity.SetLineColor(2);
  HD2620VsHelicity.GetYaxis()->SetRangeUser(0,HD2620VsHelicity.GetMaximum()*1.1);
  HD2620VsHelicity.SetStats(0);
  HD2620VsHelicity.Draw("pe");
  Canvas.Print(filename);
  HD2620VsHelicity.SetStats(1);
  HD2620VsHelicity.Draw("pe");
  HD2620VsHelicity.Fit(&FSinCos,"I","pe",-1,1);
  Canvas.Update();
  TPaveStats* stats=(TPaveStats*)HD2620VsHelicity.FindObject("stats");
  if(!stats){cout<<"no stats D2620"<<endl;return 0;}
  stats->SetX1NDC(.5);
  stats->SetX2NDC(.7);
  stats->SetY1NDC(.2);
  stats->SetY2NDC(.4);
  stats->SetFillColor(0);
  stats->SetFillStyle(1001);
  stats->SetLineColor(1);
  stats->SetLineStyle(1001);
  stats->SetTextColor(1);
  stats->SetBorderSize(2);
  //Canvas.SetRightMargin(.1);
  Canvas.Clear();
  HD2620VsHelicity.SetStats(1);
  HD2620VsHelicity.Draw("pe");
  Canvas.Print(filename);

  ////////////////////////
  Canvas.SetRightMargin(0.20);
  Canvas.Clear();
  HD2750VsHelicity.SetTitle("");
  HD2750VsHelicity.SetMarkerSize(1.5);
  HD2750VsHelicity.SetMarkerColor(2);
  HD2750VsHelicity.SetLineColor(2);   
  HD2750VsHelicity.GetYaxis()->SetRangeUser(0,HD2750VsHelicity.GetMaximum()*1.1);
  HD2750VsHelicity.SetStats(0);
  HD2750VsHelicity.Draw("pe");
  Canvas.Print(filename);
  HD2750VsHelicity.SetStats(1);
  HD2750VsHelicity.Draw("pe");
  HD2750VsHelicity.Fit(&FSinCos,"I","pe",-1,1); 
  Canvas.Update();
  stats=(TPaveStats*)HD2750VsHelicity.FindObject("stats");
  if(!stats){cout<<"no stats D2750"<<endl;return 0;}
  stats->SetX1NDC(.5);
  stats->SetX2NDC(.7);
  stats->SetY1NDC(.2);
  stats->SetY2NDC(.4);
  stats->SetFillColor(0);
  stats->SetFillStyle(1001);
  stats->SetLineColor(1);
  stats->SetLineStyle(1001);
  stats->SetTextColor(1);
  stats->SetBorderSize(2);
  //Canvas.SetRightMargin(.3);
  Canvas.Clear();
  HD2750VsHelicity.SetStats(1);
  HD2750VsHelicity.Draw("pe");
  Canvas.Print(filename);




  gStyle->SetOptStat(optstat);//restore Stats
  gStyle->SetPadRightMargin(0.20);  


  Canvas.Print(filename+"]");
 

  return 1;
}


Int_t DstPiAnalysis::FitMassVsHelicityNewSig2(Bool_t fit){

  Int_t  NBINS=10;
  TFile histosfile(_OutputDir+"/HistosForFit.root","read");
  gROOT->cd();

  TFile file(_OutputDir+"/HelicityFitsNewSig2/FitMassVsHelicity.root","update");
  RooPlot* plot=NULL;
  for(Int_t binindex=0;binindex<NBINS;binindex++){
    if(fit){      

      system(TString("rm -f ")+_OutputDir+"/HelicityFitsNewSig2/FitPars_Helicity"+(long)(binindex)+".txt");
      system(TString("rm -f ")+_OutputDir+"/HelicityFitsNewSig2/FitPars_Helicity"+(long)(binindex)+".ps");
      system(TString("rm -f ")+_OutputDir+"/HelicityFitsNewSig2/FitPars_Helicity"+(long)(binindex)+".root");
      file.Delete(TString("plot")+(long)binindex+";1");
 
      
      TH1F*HDCPiMass=(TH1F*)histosfile.Get(TString("HDstPiMVsHelicity")+(long)binindex); 
      if(!HDCPiMass){cout<<"no histo"<<endl;return 0;}
      SetHistoXYLabels(HDCPiMass,"M(D^{_{*}+}#pi^{-})","GeV/c^{2}");
      HDCPiMass->SetStats(0);      
      MassFitter fitter(&mass);
      fitter.SetPhaseSpace(&TwoBodyPS);
      fitter.SetEfficiencyPdf(effpdf);
      fitter.SetResolution(respdf,&resVar);
      fitter.SetDauMasses(&dau1m,&dau2m);      
      if(!fitter.Initialize(HDCPiMass,_OutputDir+"/HelicityFitsNewSig2/InitRooFitPars.txt"))return 0;
      if(fitter.Fit()<0)return 0;
      fitter.SetFileTag(TString("_Helicity")+(long)binindex);
      fitter.Save(_OutputDir+"/HelicityFitsNewSig2");

      plot=fitter.GetFitPlot();
      TH1*hresid=fitter.GetNormResidualHisto();
      RooPlot* plotsig=fitter.GetBkgSubPlot();
      if(plot&&hresid&&plotsig){
	file.cd();
	plot->SetName(TString("plot")+(long)binindex);
	file.Delete(TString("plot")+(long)binindex+";1");
	plot->Write();
	delete plot;

	hresid->SetName(TString("residuals")+(long)binindex);
	file.Delete(TString("residuals")+(long)binindex+";1");
	hresid->Write();
	delete hresid;

	plotsig->SetName(TString("plotsig")+(long)binindex);
	file.Delete(TString("plotsig")+(long)binindex+";1");
	plotsig->Write();
	delete plotsig;
      }
    }   
    
    cout<<"fit done "<<binindex<<endl;
    
  }

  histosfile.Close();
  file.ls();
  file.Close();

  return 1;
}


Int_t DstPiAnalysis::SystematicFitsVsHelicity(Int_t bin,TString id){
  TFile file(_OutputDir+"/HistosForFit.root","read");

  for(Int_t i=0;i<5;i++)
    if(i==bin||bin==5){
      TH1F*HMass=(TH1F*)file.Get(TString("HDstPiMVsHelicity")+(long)i);  
      if(!HMass){cout<<"no histo"<<endl;return 0;}
      HMass->SetStats(0);

      for(Int_t f=0;f<NSystsFinalHeli;f++)
	if(SystDirsFinalHeli[f]!=TString("0")&&(id==SystDirsFinalHeli[f]||id==TString("All"))){      

	  MassFitter Fitter(&mass);
	  Fitter.SetPhaseSpace(&TwoBodyPS);
	  Fitter.SetDauMasses(&dau1m,&dau2m);
	  Fitter.SetResolution(respdf,&resVar);
	  Fitter.SetEfficiencyPdf(effpdf);

	  //first bin has all the fit variations files 
	  if(!Fitter.Initialize(HMass,_OutputDir+"/HelicityFitsNewSig2/Systematics/FitVariations0/"+SystDirsFinalHeli[f]+"/InitRooFitPars.txt")){
	    cout<<"Bad init file: at helicity"<<i<<"  id="<<SystDirsFinalHeli[f]<<endl;return 0;
	  }
	  if(Fitter.Fit()<0){
	    cout<<"Bad fit: helicity"<<i<<" id="<<SystDirsFinalHeli[f]<<endl; return 0;
	  }
	  Fitter.SetFileTag("");
	  Fitter.Save(_OutputDir+"/HelicityFitsNewSig2/Systematics/FitVariations"+(long)i+"/"+SystDirsFinalHeli[f]);
	}
    }
  file.Close();
  return 1;
}

Int_t DstPiAnalysis::PlotMassVsHelicityNewSig2(){

  Int_t  NBINS=10;
  
  filename=_OutputDir+"/HelicityFitsNewSig2/FitVsHelicity.ps";
  Canvas.Print(filename+"[");
  

  TFile file(_OutputDir+"/HelicityFitsNewSig2/FitMassVsHelicity.root","read");
  gROOT->cd();

  RooPlot* plot=NULL;

  Canvas.Clear();
  Canvas.Divide(2,5);
  
  for(Int_t binindex=0;binindex<NBINS;binindex++){
    plot=(RooPlot*)file.Get(TString("plot")+(long)(binindex));
    if(!plot)continue;
  
    Canvas.GetPad(binindex+1)->SetRightMargin(.3);
    Canvas.GetPad(binindex+1)->SetLeftMargin(.1);
    Canvas.GetPad(binindex+1)->SetTopMargin(.1);
    Canvas.GetPad(binindex+1)->SetBottomMargin(.1);
    Canvas.cd(binindex+1);
    plot->SetTitle("");
    plot->SetTitleSize(.04);
    plot->GetYaxis()->SetTitle("");
    plot->GetYaxis()->SetLabelSize(.08);
    plot->GetYaxis()->SetNdivisions(5);
    plot->GetYaxis()->SetRangeUser(-400,7500);
    plot->GetXaxis()->SetTitle("");
    plot->GetXaxis()->SetLabelSize(.12);
    //plot->GetXaxis()->SetNdivisions(5);
    ((RooHist*)plot->getObject(0))->SetMarkerSize(.01);
    plot->Draw();
    text.SetTextSize(.15);
    text.DrawTextNDC(.6,.6,TString("")+(long)(binindex+1));

    cout<<"done plot "<<binindex<<endl;
      

  }
 
  Canvas.Print(filename);


  RooPlot* plotsig=NULL;

  Canvas.Clear();
  Canvas.Divide(2,5);
  
  for(Int_t binindex=0;binindex<NBINS;binindex++){
    plotsig=(RooPlot*)file.Get(TString("plotsig")+(long)(binindex));
    if(!plotsig)continue;
  
    Canvas.GetPad(binindex+1)->SetRightMargin(.3);
    Canvas.GetPad(binindex+1)->SetLeftMargin(.1);
    Canvas.GetPad(binindex+1)->SetTopMargin(.1);
    Canvas.GetPad(binindex+1)->SetBottomMargin(.1);
    //TVirtualPad*pad=
    Canvas.cd(binindex+1);
    //pad->SetLeftMargin(.2);
    //pad->SetRightMargin(.2);
    //if(binindex<4)pad->SetTopMargin(.2);
    //if(binindex>=4)pad->SetBottomMargin(.2);
    plotsig->SetTitle("");
    plotsig->SetTitleSize(.04);
    //plotsig->GetYaxis()->SetRangeUser(0,4000);
    plotsig->GetYaxis()->SetTitle("");
    plotsig->GetYaxis()->SetLabelSize(.08);
    plotsig->GetYaxis()->SetNdivisions(5);
    plotsig->GetXaxis()->SetTitle("");
    plotsig->GetXaxis()->SetLabelSize(.12);
    //plotsig->GetXaxis()->SetNdivisions(5);
    ((RooHist*)plotsig->getObject(0))->SetMarkerSize(.01);
    plotsig->Draw();
    text.SetTextSize(.15);
    text.DrawTextNDC(.6,.6,TString("")+(long)(binindex+1));

    cout<<"done plot "<<binindex<<endl;
      

  }
 
  Canvas.Print(filename);

  //plot the residuals
  Canvas.Clear();
  Canvas.Divide(2,5);
  TH1*resid;
  for(Int_t binindex=0;binindex<NBINS;binindex++){
    resid=(TH1*)file.Get(TString("residuals")+(long)binindex);
    if(!resid)continue;
  
    Canvas.GetPad(binindex+1)->SetRightMargin(.3);
    Canvas.GetPad(binindex+1)->SetLeftMargin(.1);
    Canvas.GetPad(binindex+1)->SetTopMargin(.1);
    Canvas.GetPad(binindex+1)->SetBottomMargin(.1);
    //TVirtualPad*pad=
    Canvas.cd(binindex+1);
    resid->SetTitle("");
    resid->SetTitleSize(.04);
    resid->GetYaxis()->SetTitle("");
    resid->GetYaxis()->SetLabelSize(.08);
    resid->GetYaxis()->SetNdivisions(5);
    resid->GetXaxis()->SetTitle("");
    resid->GetXaxis()->SetLabelSize(.12);
    resid->GetXaxis()->SetNdivisions(5);
    resid->GetYaxis()->SetRangeUser(-5,5);
    resid->Draw("p");
    cutline.SetLineColor(4);
    cutline.DrawLine(resid->GetXaxis()->GetXmin(),0,resid->GetXaxis()->GetXmax(),0);
    cutline.SetLineColor(2);
    cutline.DrawLine(resid->GetXaxis()->GetXmin(),-2,resid->GetXaxis()->GetXmax(),-2);
    cutline.DrawLine(resid->GetXaxis()->GetXmin(),2,resid->GetXaxis()->GetXmax(),2);
    text.SetTextSize(.15);
    text.DrawTextNDC(.6,.75,TString("")+(long)(binindex+1));

    cout<<"done plot "<<binindex<<endl;
      

  }
 
  Canvas.Print(filename);
  file.Close(); 

  Float_t D2420yield;
  Float_t D2420yieldEr;
  TH1F HD2420VsHelicity("HD2420VsHelicity","",NBINS,-1,1);
  HD2420VsHelicity.GetXaxis()->SetTitle("cos#theta_{H}"); 
  HD2420VsHelicity.GetYaxis()->SetTitle("Yield / .2");  

  Float_t D2460yield;
  Float_t D2460yieldEr;
  TH1F HD2460VsHelicity("HD2460VsHelicity","",NBINS,-1,1);
  HD2460VsHelicity.GetXaxis()->SetTitle("cos#theta_{H}"); 
  HD2460VsHelicity.GetYaxis()->SetTitle("Yield / .2");  

 
  Float_t D2550yield;
  Float_t D2550yieldEr;
  TH1F HD2550VsHelicity("HD2550VsHelicity","",NBINS,-1,1);
  HD2550VsHelicity.GetXaxis()->SetTitle("cos#theta_{H}"); 
  HD2550VsHelicity.GetYaxis()->SetTitle("Yield / .2");  

  Float_t D2620yield;
  Float_t D2620yieldEr;
  TH1F HD2620VsHelicity("HD2620VsHelicity","",NBINS,-1,1);
  HD2620VsHelicity.GetXaxis()->SetTitle("cos#theta_{H}"); 
  HD2620VsHelicity.GetYaxis()->SetTitle("Yield / .2");  

  Float_t D2750yield;
  Float_t D2750yieldEr;
  TH1F HD2750VsHelicity("HD2750VsHelicity","",NBINS,-1,1);
  HD2750VsHelicity.GetXaxis()->SetTitle("cos#theta_{H}"); 
  HD2750VsHelicity.GetYaxis()->SetTitle("Yield / .2"); 


  Float_t Backgroundyield;
  Float_t BackgroundyieldEr;
  TH1F HBackgroundVsHelicity("HBackgroundVsHelicity","",NBINS,-1,1);
  HBackgroundVsHelicity.GetXaxis()->SetTitle("cos#theta_{H}"); 
  HBackgroundVsHelicity.GetYaxis()->SetTitle("Yield / .2");  

 
  for(Int_t binindex=0;binindex<NBINS;binindex++){
        
    if(!inputpars.OpenFitPars(_OutputDir+"/HelicityFitsNewSig2/FitPars_Helicity"+(long)(binindex)+".txt"))continue;
      
    Backgroundyield=inputpars.GetFitPar("Range","integral");
    BackgroundyieldEr=0;

    D2420yield=inputpars.GetFitPar("D2420","yield");
    D2420yieldEr=inputpars.GetFitPar("D2420","yielderror");   
    Float_t SignalYield[NSystsFinalHeli+1]; 
    ParameterRead inputparssyst; 
    for(Int_t f=0;f<=NSystsFinalHeli;f++)
      SignalYield[f]=0;
    for(Int_t f=0;f<NSystsFinalHeli;f++){
      if(SystDirsFinalHeli[f]!=TString("0")){
	inputparssyst.OpenFitPars(TString(_OutputDir+"/HelicityFitsNewSig2/Systematics/FitVariations")+(long)( binindex<5 ? binindex : (NBINS - 1  - binindex))+"/"+SystDirsFinalHeli[f]+"/FitPars.txt");
	SignalYield[f]=inputparssyst.GetFitPar("D2420","yield");
	inputparssyst.CloseFitPars();
	if(f>0)SignalYield[NSystsFinalHeli]+=pow(fabs(SignalYield[f]-SignalYield[0]),2);
      }
    }
    cout<<"stat error="<<D2420yieldEr<<"  Syst Error="<<sqrt(SignalYield[NSystsFinalHeli])<<endl;
    HD2420VsHelicity.SetBinContent(binindex+1,D2420yield);//round to nearest integer
    HD2420VsHelicity.SetBinError(binindex+1,sqrt( pow(D2420yieldEr,2) + SignalYield[NSystsFinalHeli]));
    Backgroundyield-=D2420yield;
    BackgroundyieldEr+=pow(D2420yieldEr,2);
    
    D2460yield=inputpars.GetFitPar("D2460","yield");
    D2460yieldEr=inputpars.GetFitPar("D2460","yielderror");  
    HD2460VsHelicity.SetBinContent(binindex+1,D2460yield);//round to nearest integer
    for(Int_t f=0;f<=NSystsFinalHeli;f++)
      SignalYield[f]=0;
    for(Int_t f=0;f<NSystsFinalHeli;f++){
      if(SystDirsFinalHeli[f]!=TString("0")){
	inputparssyst.OpenFitPars(TString(_OutputDir+"/HelicityFitsNewSig2/Systematics/FitVariations")+(long)( binindex<5 ? binindex : (NBINS - 1  - binindex))+"/"+SystDirsFinalHeli[f]+"/FitPars.txt");
	SignalYield[f]=inputparssyst.GetFitPar("D2460","yield");
	inputparssyst.CloseFitPars();
	if(f>0)SignalYield[NSystsFinalHeli]+=pow(fabs(SignalYield[f]-SignalYield[0]),2);
      }
    }
    cout<<"stat error="<<D2460yieldEr<<"  Syst Error="<<sqrt(SignalYield[NSystsFinalHeli])<<endl;
    HD2460VsHelicity.SetBinError(binindex+1,sqrt(pow(D2460yieldEr,2)+SignalYield[NSystsFinalHeli]));
    Backgroundyield-=D2460yield;
    BackgroundyieldEr+=pow(D2420yieldEr,2);

    D2550yield=inputpars.GetFitPar("D2550","yield");
    D2550yieldEr=inputpars.GetFitPar("D2550","yielderror");  
    if(binindex==4)D2550yield-=4000;
    HD2550VsHelicity.SetBinContent(binindex+1,D2550yield);//round to nearest integer
    for(Int_t f=0;f<=NSystsFinalHeli;f++)
      SignalYield[f]=0;
    for(Int_t f=0;f<NSystsFinalHeli;f++){
      if(SystDirsFinalHeli[f]!=TString("0")){
	inputparssyst.OpenFitPars(TString(_OutputDir+"/HelicityFitsNewSig2/Systematics/FitVariations")+(long)( binindex<5 ? binindex : (NBINS - 1  - binindex))+"/"+SystDirsFinalHeli[f]+"/FitPars.txt");
	SignalYield[f]=inputparssyst.GetFitPar("D2550","yield");
	inputparssyst.CloseFitPars();
	if(f>0)SignalYield[NSystsFinalHeli]+=pow(fabs(SignalYield[f]-SignalYield[0]),2);
      }
    }
    cout<<"stat error="<<D2550yieldEr<<"  Syst Error="<<sqrt(SignalYield[NSystsFinalHeli])<<endl;
    SignalYield[NSystsFinalHeli]=pow(4000+fabs(binindex-5.+(binindex>=5?1.:0.))*1000./5.0,2);
    HD2550VsHelicity.SetBinError(binindex+1,sqrt(pow(D2550yieldEr,2)+SignalYield[NSystsFinalHeli]));
    Backgroundyield-=D2550yield;
    BackgroundyieldEr+=pow(D2420yieldEr,2);
 
    D2620yield=inputpars.GetFitPar("D2620","yield");
    D2620yieldEr=inputpars.GetFitPar("D2620","yielderror");      
    HD2620VsHelicity.SetBinContent(binindex+1,D2620yield);//round to nearest integer
    for(Int_t f=0;f<=NSystsFinalHeli;f++)
      SignalYield[f]=0;
    for(Int_t f=0;f<NSystsFinalHeli;f++){
      if(SystDirsFinalHeli[f]!=TString("0")){
	inputparssyst.OpenFitPars(TString(_OutputDir+"/HelicityFitsNewSig2/Systematics/FitVariations")+(long)( binindex<5 ? binindex : (NBINS - 1  - binindex))+"/"+SystDirsFinalHeli[f]+"/FitPars.txt");
	SignalYield[f]=inputparssyst.GetFitPar("D2620","yield");
	inputparssyst.CloseFitPars();
	if(f>0)SignalYield[NSystsFinalHeli]+=pow(fabs(SignalYield[f]-SignalYield[0]),2);
      }
    }
    cout<<"stat error="<<D2620yieldEr<<"  Syst Error="<<sqrt(SignalYield[NSystsFinalHeli])<<endl;
    SignalYield[NSystsFinalHeli]=pow(3000-fabs(binindex-5.+(binindex>=5?1.:0.))*1000./5.0,2);
    HD2620VsHelicity.SetBinError(binindex+1,sqrt(pow(D2620yieldEr,2)+SignalYield[NSystsFinalHeli]));
    Backgroundyield-=D2620yield;
    BackgroundyieldEr+=pow(D2420yieldEr,2);

    D2750yield=inputpars.GetFitPar("D2750","yield");
    D2750yieldEr=inputpars.GetFitPar("D2750","yielderror");
    if(binindex==1)D2750yield+=300;
    HD2750VsHelicity.SetBinContent(binindex+1,D2750yield);
    for(Int_t f=0;f<=NSystsFinalHeli;f++)
      SignalYield[f]=0;
    for(Int_t f=0;f<NSystsFinalHeli;f++){
      if(SystDirsFinalHeli[f]!=TString("0")){
	inputparssyst.OpenFitPars(TString(_OutputDir+"/HelicityFitsNewSig2/Systematics/FitVariations")+(long)( binindex<5 ? binindex : (NBINS - 1  - binindex))+"/"+SystDirsFinalHeli[f]+"/FitPars.txt");
	SignalYield[f]=inputparssyst.GetFitPar("D2750","yield");
	inputparssyst.CloseFitPars();
	if(f>0)SignalYield[NSystsFinalHeli]+=pow(fabs(SignalYield[f]-SignalYield[0]),2);
      }
    }
    cout<<"stat error="<<D2750yieldEr<<"  Syst Error="<<sqrt(SignalYield[NSystsFinalHeli])<<endl;
    SignalYield[NSystsFinalHeli]=pow(600+fabs(binindex-5.+(binindex>=5?1.:0.))*100./5.0,2);
    HD2750VsHelicity.SetBinError(binindex+1,sqrt(pow(D2750yieldEr,2)+SignalYield[NSystsFinalHeli]));
    Backgroundyield-=D2750yield;
    BackgroundyieldEr+=pow(D2420yieldEr,2);
    


    //if(BackgroundyieldEr<Backgroundyield*.02)BackgroundyieldEr=Backgroundyield*.5;
    HBackgroundVsHelicity.SetBinContent(binindex+1,Backgroundyield);//round to nearest integer
    HBackgroundVsHelicity.SetBinError(binindex+1,sqrt(BackgroundyieldEr));


    inputpars.CloseFitPars();

    cout<<"read file"<<binindex<<endl;

  }




  

  TStyle* gStyle=gROOT->GetStyle("myStyle");
  Int_t optstat=gStyle->GetOptStat();
  gStyle->SetOptStat("i");
  //gStyle->SetPadRightMargin(0.22);  

  TF1 FSinCos("FSinCos","[0]*([1]+[2]*x**2)*([3]+[4]*x)",-1,1);
  FSinCos.SetParameters(1,2,3);
  FSinCos.FixParameter(3,0.1444);//efficiency offset 
  FSinCos.FixParameter(4,-0.003243);//efficiency slope
  FSinCos.SetParNames("Norm","A","B");
  FSinCos.SetLineColor(4);
  
  char eqtext[100];
  TLatex equation;   
  equation.SetTextColor(4);
  equation.SetTextSize(.05);

  HD2420VsHelicity.SetTitle("");
  HD2420VsHelicity.SetMarkerSize(1.5);
  HD2420VsHelicity.SetMarkerColor(2);
  HD2420VsHelicity.SetLineColor(2);
  HD2420VsHelicity.SetStats(0);
  FSinCos.FixParameter(1,1);
  FSinCos.ReleaseParameter(2);
  HD2420VsHelicity.Fit(&FSinCos,"I","pe",-1,1);
  Canvas.Clear();
  //delete HD2420VsHelicity.GetListOfFunctions()->FindObject("stats");
  HD2420VsHelicity.GetYaxis()->SetRangeUser(0,HD2420VsHelicity.GetMaximum()*1.2);
  HD2420VsHelicity.GetYaxis()->SetTitle("Signal Yield / 0.2");
  HD2420VsHelicity.Draw("pe");
  sprintf(eqtext,"Y #propto 1 + (%.2f #pm %.2f) cos^{2}(#theta_{H})",fabs(FSinCos.GetParameter(2)),FSinCos.GetParError(2));
  equation.DrawLatex(-0.6,HD2420VsHelicity.GetMaximum()*.9,eqtext);
  sprintf(eqtext,"#chi^{2}/NDF=%d/%d",(int)(FSinCos.GetChisquare()+.5),FSinCos.GetNDF());
  equation.DrawLatex(-0.6,HD2420VsHelicity.GetMaximum()*.8,eqtext);
  Canvas.Print(filename);


  HD2460VsHelicity.SetTitle("");
  HD2460VsHelicity.SetMarkerSize(1.5);
  HD2460VsHelicity.SetMarkerColor(2);
  HD2460VsHelicity.SetLineColor(2);
  HD2460VsHelicity.SetStats(0);
  FSinCos.FixParameter(1,1);
  FSinCos.FixParameter(2,-1);
  HD2460VsHelicity.Fit(&FSinCos,"I","pe",-1,1);
  Canvas.Clear();
  //delete HD2460VsHelicity.GetListOfFunctions()->FindObject("stats");
  HD2460VsHelicity.GetYaxis()->SetRangeUser(0,HD2460VsHelicity.GetMaximum()*1.2);
  HD2460VsHelicity.GetYaxis()->SetTitle("Signal Yield / 0.2");
  HD2460VsHelicity.Draw("pe");
  sprintf(eqtext,"Y #propto sin^{2}(#theta_{H})");//,(int)FSinCos.GetParameter(0),(int)FSinCos.GetParError(0));
  equation.DrawLatex(-0.2,6000,eqtext);
  sprintf(eqtext,"#chi^{2}/NDF=%d/%d",(int)(FSinCos.GetChisquare()+.5),FSinCos.GetNDF());
  equation.DrawLatex(-0.2,3000,eqtext);
  Canvas.Print(filename);


  ///-------------------------------------
  ///Draw the new Signals
  ///-------------------------------------
  Canvas.Clear();
  HD2550VsHelicity.SetTitle("");
  HD2550VsHelicity.SetMarkerSize(1.5);
  HD2550VsHelicity.SetMarkerColor(2);
  HD2550VsHelicity.SetLineColor(2);
  HD2550VsHelicity.GetYaxis()->SetRangeUser(0,HD2550VsHelicity.GetMaximum()*1.4);
  HD2550VsHelicity.SetStats(0);
  HD2550VsHelicity.GetYaxis()->SetTitle("Signal Yield / 0.2");
  HD2550VsHelicity.Draw("pe");
  Canvas.Print(filename);
  //HD2550VsHelicity.SetStats(1);
  HD2550VsHelicity.Draw("pe");
  FSinCos.FixParameter(1,0);
  FSinCos.FixParameter(2,1);
  HD2550VsHelicity.Fit(&FSinCos,"I","pe",-1,1);
//   Canvas.Update();
//   TPaveStats*stats=(TPaveStats*)HD2550VsHelicity.FindObject("stats");
//   if(!stats){cout<<"no stats D2550"<<endl;return 0;}
//   stats->SetX1NDC(.4);
//   stats->SetX2NDC(.6);
//   stats->SetY1NDC(.6);
//   stats->SetY2NDC(.8);
//   stats->SetFillColor(0);
//   stats->SetFillStyle(1001);
//   stats->SetLineColor(1);
//   stats->SetLineStyle(1001);
//   stats->SetTextColor(1);
//   stats->SetBorderSize(2);
//   //Canvas.SetRightMargin(.1);
  Canvas.Clear();
  HD2550VsHelicity.SetStats(0);
  HD2550VsHelicity.Draw("pe");
  //sprintf(eqtext,"Y =  (%.0f #pm %.0f) + (%.0f #pm %.0f) cos^{2}(#theta_{H})",FSinCos.GetParameter(0),FSinCos.GetParError(0),FSinCos.GetParameter(1),FSinCos.GetParError(1));
  //equation.DrawLatex(-0.6,8000,eqtext);
  sprintf(eqtext,"Y #propto cos^{2}(#theta_{H})");//,(int)FSinCos.GetParameter(0),(int)FSinCos.GetParError(0));
  equation.DrawLatex(-0.4,20000,eqtext);
  sprintf(eqtext,"#chi^{2}/NDF=%d/%d",(int)(FSinCos.GetChisquare()+.5),FSinCos.GetNDF());
  equation.DrawLatex(-0.4,18000,eqtext);
  Canvas.Print(filename);

  /////////////
  Canvas.Clear();
  HD2620VsHelicity.SetTitle("");
  HD2620VsHelicity.SetMarkerSize(1.5);
  HD2620VsHelicity.SetMarkerColor(2);
  HD2620VsHelicity.SetLineColor(2);
  HD2620VsHelicity.GetYaxis()->SetRangeUser(0,HD2620VsHelicity.GetMaximum()*1.2);
  HD2620VsHelicity.SetStats(0);
  HD2620VsHelicity.GetYaxis()->SetTitle("Signal Yield / 0.2");
  HD2620VsHelicity.Draw("pe");
  Canvas.Print(filename);
  HD2620VsHelicity.SetStats(1);
  HD2620VsHelicity.Draw("pe");
  FSinCos.FixParameter(1,1);
  FSinCos.FixParameter(2,-1);
  HD2620VsHelicity.Fit(&FSinCos,"I","pe",-1,1);
  Canvas.Update();
//   stats=(TPaveStats*)HD2620VsHelicity.FindObject("stats");
//   if(!stats){cout<<"no stats D2620"<<endl;return 0;}
//   stats->SetX1NDC(.5);
//   stats->SetX2NDC(.7);
//   stats->SetY1NDC(.2);
//   stats->SetY2NDC(.4);
//   stats->SetFillColor(0);
//   stats->SetFillStyle(1001);
//   stats->SetLineColor(1);
//   stats->SetLineStyle(1001);
//   stats->SetTextColor(1);
//   stats->SetBorderSize(2);
//   //Canvas.SetRightMargin(.1);
  Canvas.Clear();
  HD2620VsHelicity.SetStats(0);
  HD2620VsHelicity.Draw("pe");
  //sprintf(eqtext,"Y =  (%.0f #pm %.0f) - (%.0f #pm %.0f) cos^{2}(#theta_{H})",FSinCos.GetParameter(0),FSinCos.GetParError(0),fabs(FSinCos.GetParameter(1)),FSinCos.GetParError(1));
  //equation.DrawLatex(-0.6,1000,eqtext);
  sprintf(eqtext,"Y #propto sin^{2}(#theta_{H})");//,(int)FSinCos.GetParameter(0),(int)FSinCos.GetParError(0));
  equation.DrawLatex(-0.4,4000,eqtext);
  sprintf(eqtext,"#chi^{2}/NDF=%d/%d",(int)(FSinCos.GetChisquare()+.5),FSinCos.GetNDF());
  equation.DrawLatex(-0.4,2000,eqtext);
  Canvas.Print(filename);

  ////////////////////////
  Canvas.SetRightMargin(0.20);
  Canvas.Clear();
  HD2750VsHelicity.SetTitle("");
  HD2750VsHelicity.SetMarkerSize(1.5);
  HD2750VsHelicity.SetMarkerColor(2);
  HD2750VsHelicity.SetLineColor(2);   
  HD2750VsHelicity.GetYaxis()->SetRangeUser(0,HD2750VsHelicity.GetMaximum()*1.1);
  HD2750VsHelicity.SetStats(0);
  HD2750VsHelicity.GetYaxis()->SetTitle("Signal Yield / 0.2");
  HD2750VsHelicity.Draw("pe");
  Canvas.Print(filename);
  //HD2750VsHelicity.SetStats(1);
  //HD2750VsHelicity.Draw("pe");
  FSinCos.FixParameter(1,1);
  FSinCos.ReleaseParameter(2);
  HD2750VsHelicity.Fit(&FSinCos,"I","pe",-1,1); 
//   Canvas.Update();
//   stats=(TPaveStats*)HD2750VsHelicity.FindObject("stats");
//   if(!stats){cout<<"no stats D2750"<<endl;return 0;}
//   stats->SetX1NDC(.5);
//   stats->SetX2NDC(.7);
//   stats->SetY1NDC(.2);
//   stats->SetY2NDC(.4);
//   stats->SetFillColor(0);
//   stats->SetFillStyle(1001);
//   stats->SetLineColor(1);
//   stats->SetLineStyle(1001);
//   stats->SetTextColor(1);
//   stats->SetBorderSize(2);
//   //Canvas.SetRightMargin(.3);
  Canvas.Clear();
  HD2750VsHelicity.SetStats(0);
  HD2750VsHelicity.Draw("pe");
  sprintf(eqtext,"Y =  1 - (%.2f #pm %.2f) cos^{2}(#theta_{H})",fabs(FSinCos.GetParameter(2)),FSinCos.GetParError(2));
  equation.DrawLatex(-0.6,600,eqtext);
  sprintf(eqtext,"#chi^{2}/NDF=%d/%d",(int)(FSinCos.GetChisquare()+.5),FSinCos.GetNDF());
  equation.DrawLatex(-0.6,300,eqtext);
  Canvas.Print(filename);


  //Plot the background yield
  HBackgroundVsHelicity.SetTitle("");
  HBackgroundVsHelicity.SetMarkerSize(1.5);
  HBackgroundVsHelicity.SetMarkerColor(2);
  HBackgroundVsHelicity.SetLineColor(2);
  HBackgroundVsHelicity.SetStats(0);
  //FSinCos.FixParameter(1,1);
  //FSinCos.ReleaseParameter(2);
  //HBackgroundVsHelicity.Fit(&FSinCos,"I","pe",-1,1);
  Canvas.Clear();
  //delete HBackgroundVsHelicity.GetListOfFunctions()->FindObject("stats");
  HBackgroundVsHelicity.GetYaxis()->SetRangeUser(0,HBackgroundVsHelicity.GetMaximum()*1.2);
  HBackgroundVsHelicity.GetYaxis()->SetTitle("Yield / 0.2");
  HBackgroundVsHelicity.Draw("pe");
  //sprintf(eqtext,"Y #propto 1 + (%.2f #pm %.2f) cos^{2}(#theta_{H})",fabs(FSinCos.GetParameter(2)),FSinCos.GetParError(2));
  //equation.DrawLatex(-0.6,HBackgroundVsHelicity.GetMaximum()*.9,eqtext);
  //sprintf(eqtext,"#chi^{2}/NDF=%d/%d",(int)(FSinCos.GetChisquare()+.5),FSinCos.GetNDF());
  //equation.DrawLatex(-0.6,HBackgroundVsHelicity.GetMaximum()*.8,eqtext);
  Canvas.Print(filename);



  gStyle->SetOptStat(optstat);//restore Stats
  gStyle->SetPadRightMargin(0.20);  

  Canvas.Print(filename+"]");
  

  //Paper plot
  TGaxis::SetMaxDigits(3);
  equation.SetTextColor(1);
  equation.SetTextSize(.09);
  TLatex signame;   
  signame.SetTextColor(1);
  signame.SetTextSize(.1);


  TCanvas PaperCanvas("PaperCanvas","",1500,300);
  PaperCanvas.Clear();
  PaperCanvas.Divide(5,1);
  TVirtualPad*p=PaperCanvas.cd(1);  
  p->SetRightMargin(0.);
  p->SetLeftMargin(.14);
  HD2420VsHelicity.GetXaxis()->SetNdivisions(5);
  HD2420VsHelicity.GetXaxis()->SetTitleSize(.09);
  HD2420VsHelicity.GetXaxis()->SetTitleOffset(.7);
  HD2420VsHelicity.GetXaxis()->SetLabelSize(.07);

  HD2420VsHelicity.GetYaxis()->SetNdivisions(5);
  HD2420VsHelicity.GetYaxis()->SetTitleSize(.07);
  HD2420VsHelicity.GetYaxis()->SetTitleOffset(.9);
  HD2420VsHelicity.GetYaxis()->SetLabelSize(.07);
  HD2420VsHelicity.SetMarkerSize(1);
  HD2420VsHelicity.Draw("pe");

  //sprintf(eqtext,"Y #propto [1+(%.2f#pm%.2f)cos^{2}(#theta_{H})]#varepsilon_{H}",fabs(((TF1*)HD2420VsHelicity.GetListOfFunctions()->FindObject("FSinCos"))->GetParameter(2)),((TF1*)HD2420VsHelicity.GetListOfFunctions()->FindObject("FSinCos"))->GetParError(2));
  //equation.DrawLatex(-0.78,HD2420VsHelicity.GetMaximum()*.85,eqtext);

  //split formula into two lines
  sprintf(eqtext,"Y #propto [1+(%.2f#pm%.2f)",fabs(((TF1*)HD2420VsHelicity.GetListOfFunctions()->FindObject("FSinCos"))->GetParameter(2)),((TF1*)HD2420VsHelicity.GetListOfFunctions()->FindObject("FSinCos"))->GetParError(2));
  equation.DrawLatex(-0.84,HD2420VsHelicity.GetMaximum()*.85,eqtext);
  sprintf(eqtext,"#timescos^{2}(#theta_{H})]#varepsilon_{H}");
  equation.DrawLatex(-0.16,HD2420VsHelicity.GetMaximum()*.73,eqtext);
  

  //sprintf(eqtext,"#chi^{2}/NDF=%d/%d",(int)(((TF1*)HD2420VsHelicity.GetListOfFunctions()->FindObject("FSinCos"))->GetChisquare()+.5),((TF1*)HD2420VsHelicity.GetListOfFunctions()->FindObject("FSinCos"))->GetNDF());
  //equation.DrawLatex(-0.78,HD2420VsHelicity.GetMaximum()*.75,eqtext);
  signame.DrawLatex(-.35,HD2420VsHelicity.GetMaximum()*.4,"D_{1}(2420)");

  p=PaperCanvas.cd(2);
  p->SetRightMargin(0.);
  p->SetLeftMargin(.14);
  HD2460VsHelicity.GetXaxis()->SetNdivisions(5);
  HD2460VsHelicity.GetXaxis()->SetTitleSize(.09);
  HD2460VsHelicity.GetXaxis()->SetTitleOffset(.7);
  HD2460VsHelicity.GetXaxis()->SetLabelSize(.07);
  HD2460VsHelicity.GetYaxis()->SetNdivisions(5);
  HD2460VsHelicity.GetYaxis()->SetTitleSize(.07);
  HD2460VsHelicity.GetYaxis()->SetTitleOffset(.9);
  HD2460VsHelicity.GetYaxis()->SetLabelSize(.07);
  HD2460VsHelicity.SetMarkerSize(1);
  HD2460VsHelicity.Draw("pe");
  sprintf(eqtext,"Y #propto sin^{2}(#theta_{H})#varepsilon_{H}");
  equation.DrawLatex(-0.5,3000,eqtext);
  //sprintf(eqtext,"#chi^{2}/NDF=%d/%d",(int)(((TF1*)HD2460VsHelicity.GetListOfFunctions()->FindObject("FSinCos"))->GetChisquare()+.5),((TF1*)HD2460VsHelicity.GetListOfFunctions()->FindObject("FSinCos"))->GetNDF());
  //equation.DrawLatex(-0.5,1000,eqtext);
  signame.DrawLatex(-.35,9000,"D^{_{*}}_{2}(2460)");

  p=PaperCanvas.cd(3);
  p->SetRightMargin(0.);
  p->SetLeftMargin(.14);
  HD2550VsHelicity.GetXaxis()->SetNdivisions(5);
  HD2550VsHelicity.GetXaxis()->SetTitleSize(.09);
  HD2550VsHelicity.GetXaxis()->SetTitleOffset(.7);
  HD2550VsHelicity.GetXaxis()->SetLabelSize(.07);
  HD2550VsHelicity.GetYaxis()->SetNdivisions(5);
  HD2550VsHelicity.GetYaxis()->SetTitleSize(.07);
  HD2550VsHelicity.GetYaxis()->SetTitleOffset(.9);
  HD2550VsHelicity.GetYaxis()->SetLabelSize(.07);
  HD2550VsHelicity.SetMarkerSize(1);
  HD2550VsHelicity.Draw("pe");
  sprintf(eqtext,"Y #propto cos^{2}(#theta_{H})#varepsilon_{H}");
  equation.DrawLatex(-0.7,21000,eqtext);
  //sprintf(eqtext,"#chi^{2}/NDF=%d/%d",(int)(((TF1*)HD2550VsHelicity.GetListOfFunctions()->FindObject("FSinCos"))->GetChisquare()+.5),((TF1*)HD2550VsHelicity.GetListOfFunctions()->FindObject("FSinCos"))->GetNDF());
  //equation.DrawLatex(-0.5,9000,eqtext);
  signame.DrawLatex(-.35,14000,"D(2550)");
  
  p=PaperCanvas.cd(4);
  p->SetRightMargin(0.);
  p->SetLeftMargin(.14);
  HD2620VsHelicity.GetXaxis()->SetNdivisions(5);
  HD2620VsHelicity.GetXaxis()->SetTitleSize(.09);
  HD2620VsHelicity.GetXaxis()->SetTitleOffset(.7);
  HD2620VsHelicity.GetXaxis()->SetLabelSize(.07);
  HD2620VsHelicity.GetYaxis()->SetNdivisions(5);
  HD2620VsHelicity.GetYaxis()->SetTitleSize(.07);
  HD2620VsHelicity.GetYaxis()->SetTitleOffset(.9);
  HD2620VsHelicity.GetYaxis()->SetLabelSize(.07);
  HD2620VsHelicity.SetMarkerSize(1);
  HD2620VsHelicity.Draw("pe");
  sprintf(eqtext,"Y #propto sin^{2}(#theta_{H})#varepsilon_{H}");
  equation.DrawLatex(-0.5,1200,eqtext);
  //sprintf(eqtext,"#chi^{2}/NDF=%d/%d",(int)(((TF1*)HD2620VsHelicity.GetListOfFunctions()->FindObject("FSinCos"))->GetChisquare()+.5),((TF1*)HD2620VsHelicity.GetListOfFunctions()->FindObject("FSinCos"))->GetNDF());
  //equation.DrawLatex(-0.5,600,eqtext);
  signame.DrawLatex(-.3,4000,"D(2600)");
  
  p=PaperCanvas.cd(5);
  p->SetRightMargin(0.);
  p->SetLeftMargin(.14);
  HD2750VsHelicity.GetXaxis()->SetNdivisions(5);
  HD2750VsHelicity.GetXaxis()->SetTitleSize(.09);
  HD2750VsHelicity.GetXaxis()->SetTitleOffset(.7);
  HD2750VsHelicity.GetXaxis()->SetLabelSize(.07);
  HD2750VsHelicity.GetYaxis()->SetNdivisions(5);
  HD2750VsHelicity.GetYaxis()->SetTitleSize(.07);
  HD2750VsHelicity.GetYaxis()->SetTitleOffset(.9);
  HD2750VsHelicity.GetYaxis()->SetLabelSize(.07);
  HD2750VsHelicity.SetMarkerSize(1);
  HD2750VsHelicity.Draw("pe");
//   sprintf(eqtext,"Y #propto [1-(%.2f#pm%.2f)cos^{2}(#theta_{H})]#varepsilon_{H}",fabs(((TF1*)HD2750VsHelicity.GetListOfFunctions()->FindObject("FSinCos"))->GetParameter(2)),((TF1*)HD2750VsHelicity.GetListOfFunctions()->FindObject("FSinCos"))->GetParError(2));
//   equation.DrawLatex(-0.98,220,eqtext);

  sprintf(eqtext,"Y #propto [1-(%.2f#pm%.2f)",fabs(((TF1*)HD2750VsHelicity.GetListOfFunctions()->FindObject("FSinCos"))->GetParameter(2)),((TF1*)HD2750VsHelicity.GetListOfFunctions()->FindObject("FSinCos"))->GetParError(2));
  equation.DrawLatex(-0.98,580,eqtext);
  sprintf(eqtext,"#timescos^{2}(#theta_{H})]#varepsilon_{H}");
  equation.DrawLatex(-0.32,170,eqtext);

  //sprintf(eqtext,"#chi^{2}/NDF=%d/%d",(int)(((TF1*)HD2750VsHelicity.GetListOfFunctions()->FindObject("FSinCos"))->GetChisquare()+.5),((TF1*)HD2750VsHelicity.GetListOfFunctions()->FindObject("FSinCos"))->GetNDF());
  //equation.DrawLatex(-0.75,120,eqtext);
  signame.DrawLatex(-.47,1200,"D(2750)");

  PaperCanvas.Print(_OutputDir+"/HelicityFitsNewSig2/FitVsHelicityPaper.ps");


 

  return 1;
}



Int_t DstPiAnalysis::SystematicFitsHeliCut(TString id){
  TFile file(_OutputDir+"/HistosForFit.root","read");
  TH1F*HMass=(TH1F*)file.Get("HDstPiMD2420Helicity");
  
  if(!HMass){cout<<"no histo"<<endl;return 0;}
  HMass->SetStats(0);

  for(Int_t f=0;f<NSysts;f++){
    //cout<<SystDirs[f]<<" "<<id<<endl;
    if(SystDirs[f]!=TString("0")&&(id==SystDirs[f]||id==TString("All"))){
      //cout<<"   "<<SystDirs[f]<<" "<<id<<endl;

      
      MassFitter Fitter(&mass);
      Fitter.SetPhaseSpace(&TwoBodyPS);
      Fitter.SetDauMasses(&dau1m,&dau2m);
      Fitter.SetResolution(respdf,&resVar);
      Fitter.SetEfficiencyPdf(effpdf);
      if(!Fitter.Initialize(HMass,_OutputDir+"/Systematics/HelicityCutFitVariations/"+SystDirs[f]+"/InitRooFitParsHeliCut.txt")){
	cout<<"Bad init file at "<<SystDirs[f]<<endl;return 0;
      }
      if(Fitter.Fit()<0){
	cout<<"Bad fit "<<SystDirs[f]<<endl; return 0;
      }
      Fitter.SetFileTag("");
      Fitter.Save(_OutputDir+"/Systematics/HelicityCutFitVariations/"+SystDirs[f]);
    }
  }

  file.Close();
  return 1;
}


Int_t DstPiAnalysis::ShowSystematicsHeliCut(){ 

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
      inputpars.OpenFitPars(TString(_OutputDir+"/Systematics/HelicityCutFitVariations/")+SystDirs[f]+"/FitPars.txt");
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
      for(Int_t s=0;s<NSig;s++){
	if(SigName[s]!="0"){ 
// 	  if(SigName[s]=="D2550")
// 	    cout<<" & "<<fabs(SignalYield[s][f]-SignalYield[s][0])/1000;
// 	  if(SigName[s]!="D2460"&&SigName[s]!="D2620"&&SigName[s]!="D2750")
// 	    cout<<" & "<<fabs(SignalMean[s][f]-SignalMean[s][0])*1000;	   
// 	  if(SigName[s]!="D2460"&&SigName[s]!="D2620"&&SigName[s]!="D2750")
// 	    cout<<" & "<<fabs(SignalWidth[s][f]-SignalWidth[s][0])*1000;	   
	  if(SigName[s]=="D2550"||SigName[s]=="D2420"){
	    cout<<" & "<<fabs(SignalYield[s][f]-SignalYield[s][0])/1000;
	    cout<<" & "<<fabs(SignalMean[s][f]-SignalMean[s][0])*1000;	   
	    cout<<" & "<<fabs(SignalWidth[s][f]-SignalWidth[s][0])*1000;	   
	  }

	}
      }
      cout<<" \\\\"<<endl;
    }
  }
  cout<<"\\hline"<<endl;
  cout<<" {\\bf Total}";
  for(Int_t s=0;s<NSig;s++){
    if(SigName[s]!="0"){ 
//       if(SigName[s]=="D2550")
// 	cout<<" & "<<sqrt(SignalYield[s][NSysts])/1000;
//       if(SigName[s]!="D2460"&&SigName[s]!="D2620"&&SigName[s]!="D2750")
// 	cout<<" & "<<sqrt(SignalMean[s][NSysts])*1000;
//       if(SigName[s]!="D2460"&&SigName[s]!="D2620"&&SigName[s]!="D2750")
// 	cout<<" & "<<sqrt(SignalWidth[s][NSysts])*1000;
      if(SigName[s]=="D2550"||SigName[s]=="D2420"){
	cout<<" & "<<sqrt(SignalYield[s][NSysts])/1000;
	cout<<" & "<<sqrt(SignalMean[s][NSysts])*1000;	   
	cout<<" & "<<sqrt(SignalWidth[s][NSysts])*1000;	   
      }
      
    }
  }
  cout<<" \\\\"<<endl;

  return 1;
}


Int_t DstPiAnalysis::SystematicFitsHeliCut2(TString id){
  TFile file(_OutputDir+"/HistosForFit.root","read");
  TH1F*HMass=(TH1F*)file.Get("HDstPiMD2460Helicity");
  
  if(!HMass){cout<<"no histo"<<endl;return 0;}
  HMass->SetStats(0);

  for(Int_t f=0;f<NSysts2;f++){
    if(SystDirs2[f]!=TString("0")&&(id==SystDirs2[f]||id==TString("All"))){

      MassFitter Fitter(&mass);
      Fitter.SetPhaseSpace(&TwoBodyPS);
      Fitter.SetDauMasses(&dau1m,&dau2m);
      Fitter.SetResolution(respdf,&resVar);
      Fitter.SetEfficiencyPdf(effpdf);
      if(!Fitter.Initialize(HMass,_OutputDir+"/Systematics/HelicityCut2FitVariations/"+SystDirs2[f]+"/InitRooFitParsHeliCut2.txt")){
	cout<<"Bad init file at "<<SystDirs2[f]<<endl;return 0;
      }
      if(Fitter.Fit()<0){
	cout<<"Bad fit "<<SystDirs2[f]<<endl; return 0;
      }
      Fitter.SetFileTag("");
      Fitter.Save(_OutputDir+"/Systematics/HelicityCut2FitVariations/"+SystDirs2[f]);
    }
  }

  file.Close();
  return 1;
}


Int_t DstPiAnalysis::ShowSystematicsHeliCut2(){ 

  Float_t SignalYield[NSig][NSysts2+1];
  Float_t SignalMean[NSig][NSysts2+1];
  Float_t SignalWidth[NSig][NSysts2+1];
  for(Int_t f=0;f<=NSysts2;f++){
    for(Int_t s=0;s<NSig;s++){ 
      if(SigName[s]!="0"){
	SignalYield[s][f]=0;
	SignalMean[s][f]=0;
	SignalWidth[s][f]=0;
      }
    }
  }


  for(Int_t f=0;f<NSysts2;f++){
    if(SystDirs2[f]!="0"){
      inputpars.OpenFitPars(TString(_OutputDir+"/Systematics/HelicityCut2FitVariations/")+SystDirs2[f]+"/FitPars.txt");
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

  //Compute the total systematic
  for(Int_t s=0;s<NSig;s++){
    if(SigName[s]!="0"){ 
      for(Int_t f=1;f<NSysts2;f++){
	if(SystDirs2[f]!="0"){
	  SignalYield[s][NSysts2]+=pow(fabs(SignalYield[s][f]-SignalYield[s][0]),2);
	  SignalMean[s][NSysts2]+=pow(fabs(SignalMean[s][f]-SignalMean[s][0]),2);
	  SignalWidth[s][NSysts2]+=pow(fabs(SignalWidth[s][f]-SignalWidth[s][0]),2);
	}
      }
    }
  }


  ///Latex format
  cout<<fixed<<setprecision(2);
  for(Int_t f=1;f<NSysts2;f++){
    if(SystDirs2[f]!="0"){
      cout<<"{\\bf "<<SystName2[f]<<"}";
      for(Int_t s=0;s<NSig;s++){
	if(SigName[s]!="0"){ 
	  if(SigName[s]=="D2620")
	    cout<<" & "<<fabs(SignalYield[s][f]-SignalYield[s][0])/1000;
// 	  if(SigName[s]!="D2460"&&SigName[s]!="D2620"&&SigName[s]!="D2750")
// 	    cout<<" & "<<fabs(SignalMean[s][f]-SignalMean[s][0])*1000;	   
// 	  if(SigName[s]!="D2460"&&SigName[s]!="D2620"&&SigName[s]!="D2750")
// 	    cout<<" & "<<fabs(SignalWidth[s][f]-SignalWidth[s][0])*1000;	   
	}
      }
      cout<<" \\\\"<<endl;
    }
  }
  cout<<"\\hline"<<endl;
  cout<<" {\\bf Total}";
  for(Int_t s=0;s<NSig;s++){
    if(SigName[s]!="0"){ 
      if(SigName[s]=="D2620")
	cout<<" & "<<sqrt(SignalYield[s][NSysts2])/1000;
//       if(SigName[s]!="D2460"&&SigName[s]!="D2620"&&SigName[s]!="D2750")
// 	cout<<" & "<<sqrt(SignalMean[s][NSysts2])*1000;
//       if(SigName[s]!="D2460"&&SigName[s]!="D2620"&&SigName[s]!="D2750")
// 	cout<<" & "<<sqrt(SignalWidth[s][NSysts2])*1000;

    }
  }
  cout<<" \\\\"<<endl;


  return 1;
}


Int_t DstPiAnalysis::SystematicFitsFinal(TString id){
  TFile file(_OutputDir+"/HistosForFit.root","read");
  TH1F*HMass=(TH1F*)file.Get("HDstPi");
  
  if(!HMass){cout<<"no histo"<<endl;return 0;}
  HMass->SetStats(0);

  for(Int_t f=0;f<NSystsFinal;f++){
    //cout<<SystDirsFinal[f]<<" "<<id<<endl;
    if(SystDirsFinal[f]!=TString("0")&&(id==SystDirsFinal[f]||id==TString("All"))){
      //cout<<"   "<<SystDirsFinal[f]<<" "<<id<<endl;
      MassFitter Fitter(&mass);
      Fitter.SetPhaseSpace(&TwoBodyPS);
      Fitter.SetDauMasses(&dau1m,&dau2m);
      Fitter.SetResolution(respdf,&resVar);
      Fitter.SetEfficiencyPdf(effpdf);

      if(!Fitter.Initialize(HMass,_OutputDir+"/Systematics/FinalFitVariations/"+SystDirsFinal[f]+"/InitRooFitPars.txt")){
	cout<<"Bad init file at "<<SystDirsFinal[f]<<endl;return 0;
      }
      if(Fitter.Fit()<0){
	cout<<"Bad fit "<<SystDirsFinal[f]<<endl; return 0;
      }
      Fitter.SetFileTag("");
      Fitter.Save(_OutputDir+"/Systematics/FinalFitVariations/"+SystDirsFinal[f]);
    }
  }

  file.Close();
  return 1;
}


Int_t DstPiAnalysis::ShowSystematicsFinal(){ 

  Float_t SignalYield[NSig][NSystsFinal+1];
  Float_t SignalMean[NSig][NSystsFinal+1];
  Float_t SignalWidth[NSig][NSystsFinal+1];
  for(Int_t f=0;f<=NSystsFinal;f++){
    for(Int_t s=0;s<NSig;s++){ 
      if(SigName[s]!="0"){
	SignalYield[s][f]=0;
	SignalMean[s][f]=0;
	SignalWidth[s][f]=0;
      }
    }
  }


  for(Int_t f=0;f<NSystsFinal;f++){
    if(SystDirsFinal[f]!="0"){
      inputpars.OpenFitPars(TString(_OutputDir+"/Systematics/FinalFitVariations/")+SystDirsFinal[f]+"/FitPars.txt");
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

  ///Add the B-field and SVT Systematic
  for(Int_t s=0;s<NSig;s++){ 
    if(SigName[s]!=""){
      SignalYield[s][13]=SignalYield[s][0];
      SignalMean[s][13]=SignalMean[s][0]+sqrt(0.00042*0.00042+0.00065*0.00065);//B=0.42 SVT=0.65MeV
      SignalWidth[s][13]=SignalWidth[s][0];
    }
  }

  //Compute the total systematic
  for(Int_t s=0;s<NSig;s++){
    if(SigName[s]!="0"){ 
      for(Int_t f=1;f<NSystsFinal;f++){
	if(SystNameFinal[f]!="0"){
	  SignalYield[s][NSystsFinal]+=pow(fabs(SignalYield[s][f]-SignalYield[s][0]),2);
	  SignalMean[s][NSystsFinal]+=pow(fabs(SignalMean[s][f]-SignalMean[s][0]),2);
	  SignalWidth[s][NSystsFinal]+=pow(fabs(SignalWidth[s][f]-SignalWidth[s][0]),2);
	}
      }
    }
  }


  ///Latex format
  cout<<fixed<<setprecision(2);
  for(Int_t f=1;f<NSystsFinal;f++){
    if(SystNameFinal[f]!="0"){
      cout<<"{\\bf "<<SystNameFinal[f]<<"}";
      for(Int_t s=0;s<NSig;s++){
	if(SigName[s]!="0"){ 
	  cout<<" & "<<fabs(SignalYield[s][f]-SignalYield[s][0])/1000;
	  if(SigName[s]!="D2460"&&SigName[s]!="D2550"&&SigName[s]!="D2620"&&SigName[s]!="D2420")
	    cout<<" & "<<fabs(SignalMean[s][f]-SignalMean[s][0])*1000;	   
	  if(SigName[s]!="D2460"&&SigName[s]!="D2550"&&SigName[s]!="D2620"&&SigName[s]!="D2420")
	    cout<<" & "<<fabs(SignalWidth[s][f]-SignalWidth[s][0])*1000;	   
	}
      }
      cout<<" \\\\"<<endl;
    }
  }
  cout<<"\\hline"<<endl;
  cout<<" {\\bf Total}";
  for(Int_t s=0;s<NSig;s++){
    if(SigName[s]!="0"){ 
      cout<<" & "<<sqrt(SignalYield[s][NSystsFinal])/1000;
      if(SigName[s]!="D2460"&&SigName[s]!="D2550"&&SigName[s]!="D2620"&&SigName[s]!="D2420")
	cout<<" & "<<sqrt(SignalMean[s][NSystsFinal])*1000;
      if(SigName[s]!="D2460"&&SigName[s]!="D2550"&&SigName[s]!="D2620"&&SigName[s]!="D2420")
	cout<<" & "<<sqrt(SignalWidth[s][NSystsFinal])*1000;

    }
  }
  cout<<" \\\\"<<endl;



  return 1;
}




Int_t DstPiAnalysis::ShowSignificances(Int_t fittype){

  TString FitType="";
  TString NomFile="";
  TString Dirs[20];
  for(Int_t f=0;f<20;f++)
    Dirs[f]="0";
  Int_t NDirs=0;
  if(fittype==1){
    FitType="HelicityCutFitVariations";
    NomFile=_OutputDir+"/FitPars_D2420Helicity.txt";
    NDirs=NSysts;
    for(Int_t f=0;f<NDirs;f++)
      Dirs[f]=SystDirs[f];
  }
  if(fittype==2){
    FitType="HelicityCut2FitVariations";  
    NomFile=_OutputDir+"/FitPars_D2460Helicity.txt";
    NDirs=NSysts2;
    for(Int_t f=0;f<NDirs;f++)
      Dirs[f]=SystDirs2[f];
  }
  if(fittype==3){
    FitType="FinalFitVariations";
    NomFile=_OutputDir+"/FitPars_Final.txt";
    NDirs=NSystsFinal;
    for(Int_t f=0;f<NDirs;f++)
      Dirs[f]=SystDirsFinal[f];
  }


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
  //inputpars.OpenFitPars(_OutputDir+"/Systematics/"+FitType+"/Nominal/FitPars.txt");
  inputpars.OpenFitPars(NomFile);
  Int_t NominalChi2=(int)inputpars.GetFitPar("Range","chisquare");
  Int_t NominalNDF=(int)inputpars.GetFitPar("Range","ndf");
  Int_t fitstatus=(int)inputpars.GetFitPar("Range","status");
  cout<<" fitstatus="<<fitstatus
      <<" chi2/ndf="<<NominalChi2<<"/"<<NominalNDF
      <<" prob="<<TMath::Prob(NominalChi2,NominalNDF)<<endl;

  for(Int_t f=0;f<NSig;f++){
   if(SigName[f]!="0"){
     SignalYield[f]=inputpars.GetFitPar(SigName[f],"yield");
     SignalYieldEr[f]=inputpars.GetFitPar(SigName[f],"yielderror");
   }
  }
  inputpars.CloseFitPars();


  /////Compute the significance based on the total error of the yield
  for(Int_t s=0;s<NSig;s++)//first square the statistical error so that systematic errors can be added in quadrature
    if(SigName[s]!="0")
      SignalYieldEr[s]=SignalYieldEr[s]*SignalYieldEr[s];

  ///Read in the systematic errors
  Float_t SystSignalYield[NSig][20];
  for(Int_t f=0;f<NDirs;f++){
    for(Int_t s=0;s<NSig;s++) 
      if(SigName[s]!="0")
	SystSignalYield[s][f]=0;
  }
  for(Int_t f=0;f<NDirs;f++){
    if(Dirs[f]!="0"){
      inputpars.OpenFitPars(_OutputDir+"/Systematics/"+FitType+"/"+Dirs[f]+"/FitPars.txt");
      for(Int_t s=0;s<NSig;s++)
	if(SigName[s]!="0")
	  SystSignalYield[s][f]=inputpars.GetFitPar(SigName[s],"yield");	  
      inputpars.CloseFitPars();
    }
  }
  for(Int_t f=1;f<NDirs;f++)
    if(Dirs[f]!="0")
      for(Int_t s=0;s<NSig;s++)
	if(SigName[s]!="0")//add the systematic errors
	  SignalYieldErSyst[s]+=pow(fabs(SystSignalYield[s][f]-SystSignalYield[s][0]),2);


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



Int_t DstPiAnalysis::ComputeBRatio(TString signal){
  //TString signal="D2620";
  Float_t mass=0.;
  if(signal=="D2460")mass=2.460;
  if(signal=="D2620")mass=2.615;
  if(signal=="D2750")mass=2.763;

  
  //Read in the D*pi yield
  inputpars.OpenFitPars("DstarCharged/DstarPi/Data/KpiAndK3pi/FitPars_Final.txt");
  Float_t DstPi=inputpars.GetFitPar(signal,"yield");
  //Stat error on yield
  Float_t DstPiEr=inputpars.GetFitPar(signal,"yielderror");
  inputpars.CloseFitPars();
  //systematic error on yield
  Float_t DstPiErSyst=0;
  if(signal=="D2460")DstPiErSyst=13000;
  if(signal=="D2620")DstPiErSyst=7300;
  if(signal=="D2750")DstPiErSyst=5200;
  cout<<"DstPi yield = "<< DstPi << " +- "<< DstPiEr<< " +- "<< DstPiErSyst<<endl;

  //read in the D*pi efficiency
  Float_t DstPiKpiEff=GetEfficiencyKpi(mass);
  Float_t DstPiKpiEffEr=GetEfficiencyKpiEr(mass);
  Float_t DstPiK3piEff=GetEfficiencyK3pi(mass);
  Float_t DstPiK3piEffEr=GetEfficiencyK3piEr(mass);
  cout<<"DstPi Eff = "<< DstPiKpiEff << " +- "<< DstPiKpiEffEr<<endl
      <<"            "<< DstPiK3piEff << " +- "<< DstPiK3piEffEr<<endl;

  //D*pi D0 BF's
  Float_t DstPiKpiBF=0.0389;//D0 --> Kpi
  Float_t DstPiKpiBFEr=0.0005;
  Float_t DstPiK3piBF=0.0810;//D0 --> K3pi
  Float_t DstPiK3piBFEr=0.0020;
  Float_t DstPiSlowPiBF=0.677;////D*+ -->D0pi+
  Float_t DstPiSlowPiBFEr=0.005;

  ///-----------Compute the D*pi efficiency 
  Float_t EffDstPi=(DstPiKpiBF*DstPiKpiEff+DstPiK3piBF*DstPiK3piEff)*DstPiSlowPiBF;
  Float_t EffDstPiEr=EffDstPi*sqrt(pow(DstPiKpiBFEr/DstPiKpiBF,2)
				   +pow(DstPiKpiEffEr/DstPiKpiEff,2)
				   +pow(DstPiK3piBFEr/DstPiK3piBF,2)
				   +pow(DstPiK3piEffEr/DstPiK3piEff,2)
				   +pow(DstPiSlowPiBFEr/DstPiSlowPiBF,2));
  cout<<"DstPi Final Eff (x100) = "<<fixed<<setprecision(3) << EffDstPi*100 
      << "  \\pm  "<< EffDstPiEr*100<<endl;

  //read in the Dpi yield
  inputpars.OpenFitPars("DCharged/DChargedPi/KPiPi/FitVariations/Nominal/FitPars_true.txt");
  Float_t DPi=inputpars.GetFitPar(signal,"yield");
  //stat error on the yield
  Float_t DPiEr=inputpars.GetFitPar(signal,"yielderror");
  inputpars.CloseFitPars();
  //syst error on the yield
  Float_t DPiErSyst=0;
  if(signal=="D2460")DPiErSyst=3370;
  if(signal=="D2620")DPiErSyst=6550;
  if(signal=="D2750")DPiErSyst=980;
  cout<<"DPi yield = "<< DPi << " +- "<< DPiEr<< " +- "<< DPiErSyst<<endl;

  //read in the Dpi efficiency
  //DCPiAnalysis DPiAnal(0,"");//cant use Class methods bacause cant include files due to variable names 
  Float_t DPiEff=.03993+mass*0.04083;//DPiAnal.GetEfficiency(mass); 
  Float_t DPiEffEr=.001+.015*fabs(mass-2.460)/.6;//DPiAnal.GetEfficiencyEr(mass);
  cout<<"DPi Eff = "<< DPiEff << " +- "<< DPiEffEr<<endl;

  //Dpi D+ BF's
  Float_t DPiBF=0.0922;//D+ --> Kpipi
  Float_t DPiBFEr=0.0021;

  ///Compute the Dpi efficiency 
  Float_t EffDPi=DPiBF*DPiEff;
  Float_t EffDPiEr=EffDPi*sqrt(pow(DPiBFEr/DPiBF,2) + pow(DPiEffEr/DPiEff,2));
  cout<<"DPi Final Eff (x100)= "<<fixed<<setprecision(3)<< EffDPi*100 
      << "  \\pm  "<< EffDPiEr*100<<endl;
  
  
  //--------------------------------------------------------------------
  //                  Compute the BF Ratio  B(D**-->Dpi)/B(D**-->D*pi)
  //--------------------------------------------------------------------
  Float_t Ratio=(DPi*EffDstPi)/(EffDPi*DstPi);


  //compute the statistical error
  cout<<"Stat Errors: "<<fixed<<setprecision(3)<<endl
      <<" Dpi yield "<<DPiEr/DPi<<endl
      <<" D*pi yield "<<DstPiEr/DstPi<<endl
      <<endl;
  Float_t RatioEr=Ratio*sqrt(pow(DPiEr/DPi,2)//Dpi yield 
			     +pow(DstPiEr/DstPi,2)//D*pi yield
			     );
  
  //compute the systematic error
  //note that efficiency systematics are only quoted on the BF ratio as many systematics cancel in the efficiency ratio.
  cout<<"Syst Errors: "<<fixed<<setprecision(3)<<endl
      <<" Dpi yield "<<DPiErSyst/DPi<<endl
      <<" D*pi yield "<<DstPiErSyst/DstPi<<endl
      <<" Tracking "<<0.03<<endl
      <<" PID "<<0.01<<endl
      <<" Dpi yield eff (MC stats + PDG BFs) "<<EffDPiEr/EffDPi<<endl
      <<" D*pi yield eff "<<EffDstPiEr/EffDstPi<<endl
      <<endl;
  Float_t RatioErSyst=Ratio*sqrt(pow(DPiErSyst/DPi,2)//Dpi yield
				 +pow(DstPiErSyst/DstPi,2)//D*pi yield
				 +pow(0.03,2)//Tracking
				 +pow(0.01,2)//PID
				 +pow(EffDPiEr/EffDPi,2)//Dpi eff= MC Stats + PDG BFS
				 +pow(EffDstPiEr/EffDstPi,2)//D*pi eff
				 );


  cout<<"The Branching Ratio = "<<fixed<<setprecision(2)<<Ratio<<" \\pm "<<RatioEr<<"(stat) \\pm "<<RatioErSyst<<"(syst)"<<endl;
  
  
  return 1;
    
}








void DstPiAnalysis::PlotBFRatioVsPstar(){

  
  TH1F HDpiYieldFull("HDpiYieldFull","",20,3.0,5);
  TH1F HDstpiYieldFull("HDstpiYieldFull","",20,3.0,5);
  for(Int_t i=0;i<20;i++){
    //DPi yield
    if(!inputpars.OpenFitPars(TString("DCharged/DChargedPi/KPiPi/PstarFits/FitPars_")+(long)(i)+".txt"))continue;
    HDpiYieldFull.SetBinContent(i+1,inputpars.GetFitPar("D2460","yield"));
    HDpiYieldFull.SetBinError(i+1,inputpars.GetFitPar("D2460","yielderror"));
    inputpars.CloseFitPars();

    //DstPi yield
    if(!inputpars.OpenFitPars(TString("DstarCharged/DstarPi/Data/Kpi/PstarFits/FitPars_")+(long)(i)+".txt"))continue;
    HDstpiYieldFull.SetBinContent(i+1,inputpars.GetFitPar("D2460","yield"));
    HDstpiYieldFull.SetBinError(i+1,inputpars.GetFitPar("D2460","yielderror"));
    inputpars.CloseFitPars();
  }
  Float_t DPiYieldScale=242800/HDpiYieldFull.Integral();
  Float_t DstPiYieldScale=28700/HDstpiYieldFull.Integral();

 


  TH1F HDpiYield("HDpiYield","",15,3.0,4.5);
  HDpiYield.GetXaxis()->SetTitle("p*(D#pi)     (GeV/c)");
  HDpiYield.GetYaxis()->SetTitle("D^{+}#pi^{-} Yield");
  TH1F HDpiYieldCorr("HDpiYieldCorr","",15,3.0,4.5);
  HDpiYieldCorr.GetXaxis()->SetTitle("p*(D#pi)     (GeV/c)");
  HDpiYieldCorr.GetYaxis()->SetTitle("D^{+}#pi^{-} Corrected Yield");
  TF1 DpiEff("DpiEff","0.1479+0.01846*x",3.0,4.5);


  TH1F HDstpiYield("HDstpiYield","",15,3.0,4.5);
  HDstpiYield.GetXaxis()->SetTitle("p*(D^{_{*}}#pi)     (GeV/c)");
  HDstpiYield.GetYaxis()->SetTitle("D^{*+}#pi^{-} Yield");
  TH1F HDstpiYieldCorr("HDstpiYieldCorr","",15,3.0,4.5);
  HDstpiYieldCorr.GetXaxis()->SetTitle("p*(D^{_{*}}#pi)     (GeV/c)");
  HDstpiYieldCorr.GetYaxis()->SetTitle("D^{*+}#pi^{-} Corrected Yield");
  TF1 DstpiEff("DstpiEff","0.1791+0.02316*x",3.0,4.5);


  TH1F HBFRatioVsPstar("HBFRatioVsPstar","",15,3.0,4.5);
  HBFRatioVsPstar.GetXaxis()->SetTitle("p*(D_{^{2}}^{_{*}})     (GeV/c)");
  HBFRatioVsPstar.GetYaxis()->SetTitle("B(D_{^{2}}^{_{*}} #rightarrow D^{+}#pi^{-}) / B(D_{^{2}}^{_{*}} #rightarrow D^{*+}#pi^{-})");


  Float_t PDGBFD0toKpi=0.0389;
  Float_t PDGBFDCtoKpipi=0.0922;
  Float_t PDGBFDsttoD0pi=0.677;

  for(Int_t i=0;i<15;i++){
    //DPi yield
    if(!inputpars.OpenFitPars(TString("DCharged/DChargedPi/KPiPi/PstarFits/FitPars_")+(long)(i)+".txt"))continue;
    Float_t dpiyield=inputpars.GetFitPar("D2460","yield")*DPiYieldScale;
    Float_t dpiyielder=inputpars.GetFitPar("D2460","yielderror")*DPiYieldScale;
    HDpiYield.SetBinContent(i+1,dpiyield);
    HDpiYield.SetBinError(i+1,dpiyielder);
    HDpiYieldCorr.SetBinContent(i+1,dpiyield/DpiEff.Eval(3.0+0.05+i*0.1));
    HDpiYieldCorr.SetBinError(i+1,dpiyielder/DpiEff.Eval(3.0+0.05+i*0.1));
    inputpars.CloseFitPars();

    //DstPi yield
    if(!inputpars.OpenFitPars(TString("DstarCharged/DstarPi/Data/Kpi/PstarFits/FitPars_")+(long)(i)+".txt"))continue;
    Float_t dstpiyield=inputpars.GetFitPar("D2460","yield")*DstPiYieldScale;
    Float_t dstpiyielder=inputpars.GetFitPar("D2460","yielderror")*DstPiYieldScale;
    HDstpiYield.SetBinContent(i+1,dstpiyield);
    HDstpiYield.SetBinError(i+1,dstpiyielder);
    HDstpiYieldCorr.SetBinContent(i+1,dstpiyield/DstpiEff.Eval(3.0+0.05+i*0.1));
    HDstpiYieldCorr.SetBinError(i+1,dstpiyielder/DstpiEff.Eval(3.0+0.05+i*0.1));
    inputpars.CloseFitPars();

    //BF ratio
    HBFRatioVsPstar.SetBinContent(i+1,HDpiYieldCorr.GetBinContent(i+1));
    HBFRatioVsPstar.SetBinError(i+1,HDpiYieldCorr.GetBinError(i+1));
//     HBFRatioVsPstar.SetBinContent(i+1,(dpiyield/(DpiEff.Eval(3.0+0.05+i*0.1)*PDGBFDCtoKpipi))
// 				      /(dstpiyield/(DstpiEff.Eval(3.0+0.05+i*0.1)*PDGBFDsttoD0pi*PDGBFD0toKpi)));
//     HBFRatioVsPstar.SetBinError(i+1,
// 				HBFRatioVsPstar.GetBinContent(i+1)
// 				*sqrt(pow(dpiyielder/dpiyield,2)+pow(dstpiyielder/dstpiyield,2))
// 				);
  }

  filename=_OutputDir+"/BFRatioVsPstar/BFRatioVsPstar.ps";
  Canvas.Print(filename+"[");

  Canvas.Clear();
  HDpiYieldFull.GetYaxis()->SetRangeUser(0,HDpiYieldFull.GetMaximum()*1.2);
  HDpiYieldFull.Draw("hist pe");
  Canvas.Print(filename);
  Canvas.Clear();
  HDstpiYieldFull.GetYaxis()->SetRangeUser(0,HDstpiYieldFull.GetMaximum()*1.2);
  HDstpiYieldFull.Draw("hist pe");
  Canvas.Print(filename);

  Canvas.Clear();
  HDpiYield.GetYaxis()->SetRangeUser(0,HDpiYield.GetMaximum()*1.2);
  HDpiYield.Draw("hist pe");
  Canvas.Print(filename);
//   Canvas.Clear();
//   HDpiYieldCorr.GetYaxis()->SetRangeUser(0,HDpiYieldCorr.GetMaximum()*1.2);
//   HDpiYieldCorr.Draw("hist pe");
//   Canvas.Print(filename);

  Canvas.Clear();
  HDstpiYield.GetYaxis()->SetRangeUser(0,HDstpiYield.GetMaximum()*1.2);
  HDstpiYield.Draw("hist pe");
  Canvas.Print(filename);
//   Canvas.Clear();
//   HDstpiYieldCorr.GetYaxis()->SetRangeUser(0,HDstpiYieldCorr.GetMaximum()*1.2);
//   HDstpiYieldCorr.Draw("hist pe");
//   Canvas.Print(filename);


  TF1 FBFR("FBFR","[0]",3,4.5);
  FBFR.SetLineColor(2);
  Canvas.Clear();
  HBFRatioVsPstar.Sumw2();
  HBFRatioVsPstar.Divide(&HDstpiYieldCorr);
  HBFRatioVsPstar.Scale(PDGBFDsttoD0pi*PDGBFD0toKpi/PDGBFDCtoKpipi);
  HBFRatioVsPstar.SetStats(1);
  HBFRatioVsPstar.GetYaxis()->SetRangeUser(0,HBFRatioVsPstar.GetMaximum()*1.4);
  HBFRatioVsPstar.Fit("FBFR");
  HBFRatioVsPstar.Draw("hist pe");
  FBFR.Draw("same");
  Canvas.Print(filename);


  Canvas.Print(filename+"]");
}
