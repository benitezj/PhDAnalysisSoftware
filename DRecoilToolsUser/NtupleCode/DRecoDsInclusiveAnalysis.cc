#include "DRecoilToolsUser/NtupleCode/DRecoDsInclusiveAnalysis.hh"

#include "DRecoilToolsUser/NtupleCode/DReco2BodyMassPhaseSpace.hh"
#include "DRecoilToolsUser/NtupleCode/DRecoSimulChi2Var.hh"
#include "DRecoilToolsUser/NtupleCode/DRecoHistPdfChi2Var.hh"
#include "DRecoilToolsUser/NtupleCode/DRecoResiduals.hh"
#include "DRecoilToolsUser/NtupleCode/Fits.hh"


#include "TH2F.h"
#include "TList.h"
#include "RooMinuit.h"
#include "RooChi2Var.h"
#include "RooFitResult.h"
#include "TGraph.h"
#include "TStyle.h"
#include "TGraphAsymmErrors.h"
#include "TRandom3.h"
using namespace RooFit;


//////////////////////TMVA
#include <vector>
#include <map>
#include <string>
#include "TMVA/Tools.h"
#include "TMVA/Reader.h"
using namespace TMVA;
/////////////////////////////


#define HistPDFInterpolation 0

ClassImp(DRecoDsInclusiveAnalysis);
DRecoDsInclusiveAnalysis::DRecoDsInclusiveAnalysis(Int_t DataType,TString OutputDir,Int_t SystematicId):
  DRecoTagAnalysis(DataType,OutputDir)
  ,nxr("nxr","n_{X}^{R}",-.5,(float)NXR-.5)
  ,nxt("nxt","nxt",-.5,NXT-.5)//
  ,TotYieldWS("TotYieldWS","TotYieldWS",.01,.25)//
  ,TotYield("TotYield","TotYield",.2481,.001,.4)// 
  ,WStoRSFrac("WStoRSFrac","WStoRSFrac",.1186)//.0887 in R22
  ,cutdsstmass(0)
  ,cutevtvtxprob(0)
  ,cutfragngams(0)
  ,mcscalefactor(.52)//from shane's table
  ,_SystematicId(SystematicId)
{

  //-------------
  // Do not delete!!!!!!!!!!!!!!!!WS Signal Yield Systematic: Using a modified WStoRSFrac!!!!!!!!! 
  //-------
  if(abs(_SystematicId)==4){
    WStoRSFrac.setVal(WStoRSFrac.getVal()*((_SystematicId<0)?.9:1.1));
    cout<<" !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!WARNING WS Signal Yield Systematic Turned On.!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
  }
    
  
  //Fit variables
  nxr.setBins(NXR);
  nset.add(nxr);

  mass.SetTitle("m(D_{s})");
  mass.setUnit("GeV/c^{2}");
  mass.setRange(DsMassMin,DsMassMax);
  mass.setBins(NDsMassBins);  
  nset.add(mass);

  //values for the Ds* mass cuts
  DsStUncMassMean0pi0=2.116;
  DsStUncMassSigma0pi0[0]=.063;
  DsStUncMassSigma0pi0[1]=.050;
  DsStUncMassSigma0pi0[2]=.046;
  DsStUncMassSigma0pi0[3]=.032;
  DsStUncMassSigma0pi0[4]=.0;//more than 3 pi+'s not used
  DsStUncMassMean1pi0=2.144;
  DsStUncMassSigma1pi0=.055;


  //-------------------------------
  //Fit variables and pdfs
  //---------------------------------
  ///Get the WS signal Pdfs from the signal MC 
  TFile PdfsFileWS("/afs/slac.stanford.edu/u/eb/benitezj/anaR24Skim/workdir/DsTolnu/SP1005Skim/DsReco/FinalWS/HistosForFit.root","read"); 
  gROOT->cd();
  TH2F* HDsMassSigWS[NXT];
  totalwws=0;
  if(!PdfsFileWS.IsZombie()){
    for(Int_t j=0;j<NXT;j++){    
      if(_DataType==2)HDsMassSigWS[j]=(TH2F*)PdfsFileWS.Get(TString("HDsMassTrue2D_nXTShifted")+(long)j);
      else HDsMassSigWS[j]=(TH2F*)PdfsFileWS.Get(TString("HDsMassTrue2D_nXT")+(long)j);
      if(!HDsMassSigWS[j]){cout<<TString("HDsMassTrue2D_nXTShifted")+(long)j<<" Not found"<<endl;}    
      DsMassDataSetSigWS[j]=new RooDataHist(TString("DsMassDataSetSigWS")+(long)j,TString("DsMassDataSetSigWS")+(long)j,
					    nset,HDsMassSigWS[j]);      
      SigPdfWS[j]=new DRecoHistPdf(TString("SigPdfWS")+(long)j,"",nset,*DsMassDataSetSigWS[j]);

      //calculate initial weights
      truewws[j]=HDsMassSigWS[j]->Integral();
      totalwws+=HDsMassSigWS[j]->Integral();
    } 
  }  
  for(Int_t j=0;j<NXT;j++)
    truewws[j]/=totalwws;//set as fraction
  PdfsFileWS.Close();  

  //-------------------------------------------------
  //create the background pdf for the RS.
  //-------------------------------------------------
  for(Int_t i=0;i<NXR;i++){
    //this pdf selects one nXR value
    TH2F*  WSBkgnXRHist=new TH2F(TString("WSBkgnXRHist")+(long)i,TString("WSBkgnXRHist")+(long)i
				 ,NXR,-.5,(float)NXR-.5,NDsMassBins,DsMassMin,DsMassMax);
    for(Int_t b=1;b<=NDsMassBins;b++){
      WSBkgnXRHist->SetBinContent(i+1,b,1);
      WSBkgnXRHist->SetBinError(i+1,b,.001);
    }
    WSBkgnXRSelectDat[i]=new RooDataHist(TString("WSBkgnXRSelectDat")+(long)i,TString("WSBkgnXRSelectDat")+(long)i
					 ,nset,WSBkgnXRHist);
    WSBkgnXRSelectPdf[i]=new DRecoHistPdf(TString("WSBkgnXRSelectPdf")+(long)i,TString("WSBkgnXRSelectPdf")+(long)i
					  ,nset,*WSBkgnXRSelectDat[i]);  
    
    delete WSBkgnXRHist;
  }


  //corrections for discrepancies between WS and RS backgrounds
  TFile WSCorrFile("/afs/slac.stanford.edu/u/eb/benitezj/anaR24Skim/workdir/DsTolnu/SP1005Skim/DsReco/FinalWS/WSnXR0Correction.root","read");
  gROOT->cd();
  if(!WSCorrFile.IsZombie()){
    //nXR=0 correction
    TH1F* HWSnXR0Corr=(TH1F*)WSCorrFile.Get("HnXR0Correction");
    if(!HWSnXR0Corr){cout<<"HnXR0Correction Not found"<<endl;}  
    TH2F HWSnXR0Corr2D("HWSnXR0Corr2D","HWSnXR0Corr2D",NXR,-.5,(float)NXR-.5,NDsMassBins,DsMassMin,DsMassMax);  
    for(Int_t b=1;b<=NDsMassBins;b++){
      HWSnXR0Corr2D.SetBinContent(1,b,HWSnXR0Corr->GetBinContent(b));
      HWSnXR0Corr2D.SetBinError(1,b,HWSnXR0Corr->GetBinError(b));
      //HWSnXR0Corr2D.SetBinContent(1,b,1);
    }
    WSnXR0CorrDat=new RooDataHist("WSnXR0CorrDat","WSnXR0CorrDat",nset,&HWSnXR0Corr2D);
    WSnXR0CorrPdf=new DRecoHistPdf("WSnXR0CorrPdf","WSnXR0CorrPdf",nset,*WSnXR0CorrDat);   
    //nXR=1 correction
    TH1F* HWSnXR1Corr=(TH1F*)WSCorrFile.Get("HnXR1Correction");
    if(!HWSnXR1Corr){cout<<"HnXR1Correction Not found"<<endl;}  
    TH2F HWSnXR1Corr2D("HWSnXR1Corr2D","HWSnXR1Corr2D",NXR,-.5,(float)NXR-.5,NDsMassBins,DsMassMin,DsMassMax);  
    for(Int_t b=1;b<=NDsMassBins;b++){
      HWSnXR1Corr2D.SetBinContent(2,b,1);//Dont apply correction to Data
    }
    WSnXR1CorrDat=new RooDataHist("WSnXR1CorrDat","WSnXR1CorrDat",nset,&HWSnXR1Corr2D);
    WSnXR1CorrPdf=new DRecoHistPdf("WSnXR1CorrPdf","WSnXR1CorrPdf",nset,*WSnXR1CorrDat);   
    WSCorrFile.Close();
  }
  //create bkg yields for each nXR
  for(Int_t i=0;i<NXR;i++){
    BkgnXRYield[i]=new RooRealVar(TString("BkgnXRYield")+(long)i,TString("BkgnXRYield")+(long)i,.001,.6);    
  }


  ///Get the RS signal Pdfs from the signal MC 
  TFile PdfsFile("/afs/slac.stanford.edu/u/eb/benitezj/anaR24Skim/workdir/DsTolnu/SP1005Skim/DsReco/Final/HistosForFit.root","read"); 
  gROOT->cd();
  TH2F* HDsMassSig[NXT];
  totalw=0.;
  Float_t totalMCSigYield=0.;
  Float_t totalMCSigYieldFullRange=0.;
  if(!PdfsFile.IsZombie()){
    for(Int_t j=0;j<NXT;j++){    
      if(_DataType==2)HDsMassSig[j]=(TH2F*)PdfsFile.Get(TString("HDsMassTrue2D_nXTShifted")+(long)j);//use shifted pdfs for Data
      else HDsMassSig[j]=(TH2F*)PdfsFile.Get(TString("HDsMassTrue2D_nXT")+(long)j);            
      //Chisquares vs. Shift(MeV): 0.->570.996, -.3->464.89, -.6->408.983, -.9->371.124, -1.2->340.838, -1.5->330.552, -1.8->331.222, -2.2->351.856
      //the shift was done only to the RS signal. If we shift also the WS Signal we get:
      //-1.2->340.22, -1.5->329.585, -1.8->328.758, -2.2->355.248
      //Ds mass in MC=1.96860  Ds mass in PDG2008=1.96849
      //DeltaM in MC=2.1124-1.96860=143.80,  DeltaM in PDG2008=143.8+-.4
    
      //-------
      // Do not delete!!!!!!!!!!!!!!!!Ds Peak Shift Systematic: Using an unshifted peak!!!!!!!!! 
      //-------
      if(abs(_SystematicId)==3){
	HDsMassSig[j]=(TH2F*)PdfsFile.Get(TString("HDsMassTrue2D_nXTShiftedRes")+(long)j);
	cout<<" !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!WARNING Ds Peak resolution Systematic Turned On.!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
      }
    

      //-------
      // Do not delete!!!!!!!!!!!!!!!!nX Resolution Systematic: Using Signal Pdf with modified nX resolution!!!!!!!!! 
      //-------
      if(abs(_SystematicId)==6){
	TH2F*HClone=(TH2F*)HDsMassSig[j]->Clone("HClone");
	for(Int_t i=1;i<=NXR;i++)
	  for(Int_t b=1;b<=NDsMassBins;b++){
	    if(j<4){
	      if(i==j+1){//maximum bin is same as nXT
		HDsMassSig[j]->SetBinContent(i,b,HClone->GetBinContent(i,b)+(_SystematicId<0?-.1:.1)*HClone->GetBinContent(i,b));//remove 10% from Max bin
		HDsMassSig[j]->SetBinError(i,b,HClone->GetBinError(i,b));
	      }else{
		HDsMassSig[j]->SetBinContent(i,b,HClone->GetBinContent(i,b)-(_SystematicId<0?-.1/3:.1/3)*HClone->GetBinContent(i,b));//1/3 because there are 3 other bins and they get the same amount
		HDsMassSig[j]->SetBinError(i,b,HClone->GetBinError(i,b));
	      }
	    }else{
	      if(i==4){//for nXT=4,5 maximum bin is last bin
		HDsMassSig[j]->SetBinContent(i,b,HClone->GetBinContent(i,b)+(_SystematicId<0?-.1:.1)*HClone->GetBinContent(4,b));//remove 10% from Max bin
		HDsMassSig[j]->SetBinError(i,b,HClone->GetBinError(i,b));
	      }else{
		HDsMassSig[j]->SetBinContent(i,b,HClone->GetBinContent(i,b)-(_SystematicId<0?-.1/3:.1/3)*HClone->GetBinContent(4,b));//1/3 because there are 3 other bins and they get the same amount
		HDsMassSig[j]->SetBinError(i,b,HClone->GetBinError(i,b));
	      }	  
	    }
	  } 
	delete HClone;
	cout<<" !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!WARNING nX Resolution Systematic Turned On.!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
      }

      //-------
      // Do not delete!!!!!!!!!!!!!!!!Photon Bkg  Systematic: Using Signal Pdf with larger Pi0+Eta+Track+Klong Bkg!!!!!!!!! 
      //-------
      if(abs(_SystematicId)==7){
	if(_SystematicId>0)HDsMassSig[j]=(TH2F*)PdfsFile.Get(TString("HDsMassTrue2D_nXTShiftedPi0Systematic")+(long)j);
	if(_SystematicId<0)HDsMassSig[j]=(TH2F*)PdfsFile.Get(TString("HDsMassTrue2D_nXTShiftedPi0SystematicM")+(long)j);
	cout<<" !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!WARNING Signal Photon Bkg  Systematic Turned On.!!!!!!!!!!!!!!!!!!!!!!"<<endl;
      }


      if(!HDsMassSig[j]){cout<<TString("HDsMassTrue2D_nXT")+(long)j<<" Not found"<<endl;}    
      DsMassDataSetSig[j]=new RooDataHist(TString("DsMassDataSetSig")+(long)j,TString("DsMassDataSetSig")+(long)j,nset,HDsMassSig[j]); 
      SigPdf[j]=new DRecoHistPdf(TString("SigPdf")+(long)j,TString("SigPdf")+(long)j,nset,*DsMassDataSetSig[j],0);
      //calculate initial weights
      truew[j]=HDsMassSig[j]->Integral();
      totalw+=HDsMassSig[j]->Integral();
      totalMCSigYield+=HDsMassSig[j]->Integral(1,4,DenominatorDsMassBinMin,DenominatorDsMassBinMax);
      totalMCSigYieldFullRange+=HDsMassSig[j]->Integral();
    }    
    for(Int_t j=0;j<NXT;j++)truew[j]/=totalw;
  }
  PdfsFile.Close();
  cout<<"Total Sig MC Signal Yield="<<totalMCSigYield<<"    Signal Yield Full Range="<<totalMCSigYieldFullRange<<endl;
  

  ////MC background shape
  TFile RSBkgFile("/afs/slac.stanford.edu/u/eb/benitezj/anaR24Skim/workdir/DsTolnu/SP1005Skim/DsReco/Final/HistosForFit.root","read"); 
  gROOT->cd();  
  if(!RSBkgFile.IsZombie()){
    TH2F* HDsMassBkg=(TH2F*)RSBkgFile.Get("HDsMass2DNoSig");
    if(!HDsMassBkg){cout<<" HDsMass2DNoSig Not found"<<endl;}     
    for(Int_t i=0;i<NXR;i++){
      truebkgw[i]=HDsMassBkg->Integral(i+1,i+1,1,NDsMassBins)/HDsMassBkg->Integral();
    }
  }
  RSBkgFile.Close();


  //-------------
  // Do not delete!!!!!!!!!!!!!!!!WS weights Systematic: create a TF1 with 40% gradient and multiply the weights!!!!!!!!! 
  //-------
  if(abs(_SystematicId)==1){
    TF1 FWSweightsSystematic("FWSweightsSystematic","1-.9*(x-2.5)/2.5",0-.5,NXT-.5);
    totalwws=0;
    for(Int_t j=0;j<NXT;j++){
      truewws[j]= truewws[j]*FWSweightsSystematic.Eval(j);
      totalwws+=truewws[j];
    }
    for(Int_t j=0;j<NXT;j++)//make sure they are normalized
      truewws[j]/=totalwws;
    cout<<" !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!WARNING WS Weights Systematic Turned On.!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
  }
  
  for(Int_t j=0;j<NXT;j++)
    nXTCoefWS[j]=new RooRealVar(TString("nXTCoefWS")+(long)j,TString("nXTCoefWS")+(long)j,truewws[j]);


  

  //////////////RS Weights
  //first 3 independent weights
//   for(Int_t j=0;j<3;j++){ 
//     nXTCoef[j]=new RooRealVar(TString("nXTCoef")+(long)j,TString("nXTCoef")+(long)j,.01,.5);          
//     nXTCoef[j]->setVal(truew[j]);  
//   }

  //last 3  weights fixed to MC shape
  nxt.setBins(NXT);
  nxtset.add(nxt);

  //-------------
  // Do not delete!!!!!!!!!!!!!!!!RS weights Systematic: create a TF1 with 40% gradient and multiply the weights!!!!!!!!! 
  //-------
//   if(abs(_SystematicId)==2){
//     TF1 FRSweightsSystematic("FWSweightsSystematic",(_SystematicId<0)?"1-.4*(x-4.5)/1.5":"1+.4*(x-4.5)/1.5",0-.5,NXT-.5);
//     for(Int_t j=3;j<NXT;j++){
//       truew[j]= truew[j]*FRSweightsSystematic.Eval(j);//renormalization not necesary
//     }
//     cout<<" !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!WARNING RS Weights Systematic Turned On.!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
//   }
//   Float_t lastweightssum=0.;
//   for(Int_t j=3;j<NXT;j++)
//     lastweightssum+=truew[j];

//   for(Int_t j=3;j<NXT;j++){ 
//     nXTCoefWM[j]=new RooFormulaVar(TString("nXTCoefWM")+(long)j,TString("nXTCoefWM")+(long)j,Form("%.6f*(1-nXTCoef0-nXTCoef1-nXTCoef2)/%.6f",truew[j],lastweightssum),RooArgSet(*nXTCoef[0],*nXTCoef[1],*nXTCoef[2]));  
//     nXTCoef[j]=new RooRealVar(TString("nXTCoef")+(long)j,TString("nXTCoef")+(long)j,truew[j]);          
//   }


  //Use weight model
  nXTa=new RooRealVar("nXTa","nXTa",-1.32);//-2.,-.5);
  if(abs(_SystematicId)==2){
    nXTa->setVal(_SystematicId<0?-10.:-.5);
    cout<<" !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!WARNING RS Weights Systematic Turned On.!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
  }
  nXTb=new RooRealVar("nXTb","nXTb",-5.,10.);nXTb->setVal(3.49);
  nXTg=new RooRealVar("nXTg","nXTg",.01,5.);nXTg->setVal(1.17);
  TString nXTWMNorm="((-nXTa)^nXTb";
  for(Int_t j=1;j<NXT;j++)nXTWMNorm+=TString("+((")+(long)j+"-nXTa)^nXTb)*exp(-nXTg*"+(long)j+")";
  nXTWMNorm+=")";
  cout<<" norm formula = "<<nXTWMNorm<<endl;
  for(Int_t j=0;j<NXT;j++){ 
    nXTCoefWM[j]=new RooFormulaVar(TString("nXTCoefWM")+(long)j,TString("nXTCoefWM")+(long)j,TString("((")+(long)j+"-nXTa)^nXTb)*exp(-nXTg*"+(long)j+")/"+nXTWMNorm,RooArgSet(*nXTa,*nXTb,*nXTg));   
  }
  
//   nXTa=new RooRealVar("nXTa","nXTa",-10.,10.);
//   nXTb=new RooRealVar("nXTb","nXTb",.2,10.);
//   nXTg=new RooRealVar("nXTg","nXTg",1.,20.);
//   TString nXTWMNorm="(exp(-(abs(-nXTa)/nXTb)**nXTg)";
//   for(Int_t j=1;j<NXT;j++)nXTWMNorm+=TString("+exp(-(abs(")+(long)j+"-nXTa)/nXTb)**nXTg)";
//   nXTWMNorm+=")";
//   cout<<" norm formula = "<<nXTWMNorm<<endl;
//   for(Int_t j=0;j<NXT;j++){ 
//     nXTCoefWM[j]=new RooFormulaVar(TString("nXTCoefWM")+(long)j,TString("nXTCoefWM")+(long)j,TString("exp(-(abs(")+(long)j+"-nXTa)/nXTb)**nXTg)/"+nXTWMNorm,RooArgSet(*nXTa,*nXTb,*nXTg));   
//   }

//    nXTa=new RooRealVar("nXTa","nXTa",-10.,10.);
//    nXTb=new RooRealVar("nXTb","nXTb",-10.,10.);
//    nXTg=new RooRealVar("nXTg","nXTg",-10.,10.);
//    TString nXTWMNorm="(1";
//    for(Int_t j=1;j<NXT;j++)nXTWMNorm+=TString("+exp(nXTa*")+(long)j+"+nXTb*"+(long)j+"^2+nXTg*"+(long)j+"^3)";
//    nXTWMNorm+=")";
//    cout<<" norm formula = "<<nXTWMNorm<<endl;
//    for(Int_t j=0;j<NXT;j++){ 
//      nXTCoefWM[j]=new RooFormulaVar(TString("nXTCoefWM")+(long)j,TString("nXTCoefWM")+(long)j,TString("(exp(nXTa*")+(long)j+"+nXTb*"+(long)j+"^2+nXTg*"+(long)j+"^3))/"+nXTWMNorm,RooArgSet(*nXTa,*nXTb,*nXTg));   
//    }

  cout<<"Done Initializing the DRecoDsInclusiveAnalysis analysis"<<endl;
}

DRecoDsInclusiveAnalysis::~DRecoDsInclusiveAnalysis(){ 
//   delete nXTa;
//   delete nXTb;
//   delete nXTg;
//   for(Int_t j=0;j<NXT;j++){
//     delete nXTCoefWM[j];
//     delete nXTCoefWS[j];
//     delete DsMassDataSetSig[j];
//     delete SigPdf[j];
//     delete DsMassDataSetSigWS[j];
//     delete SigPdfWS[j];
//   }


}

Int_t DRecoDsInclusiveAnalysis::ReduceNtuple(){
  cout<<"Going to reduce the ntuple"<<endl;

  if(_fChain==NULL){
    cout<<"No chain."<<endl;
    return 0;
  } 

  ///link the branches in the input chain
  SetDsInclusiveBranches();

  //the file must be created here so the 'new' histos and Tree is created in hard drive
  TFile OutFile(_OutputDir+"/Ntuple_New.root","recreate",_OutputDir+"/Ntuple_New.root",1);//1=minimal compression
  if(OutFile.IsZombie()){cout<<"Failed to create file"<<endl;return 0;}

  //create new reduced ntuple
  TTree Ntuple("Ntuple","Ntuple",0);
  ReducedNtuple=&Ntuple;
  //Long64_t maxsize=1000000000;//10Gb
  ReducedNtuple->SetMaxTreeSize(900000000);//split files
  //Long64_t fautosave=1100000000;//11Gb !!//disable autosaving 
  ReducedNtuple->SetAutoSave(900000000);


  //create the branches in the Ntuple
  MakeDsInclusiveReducedNtupleBranches();
 

  //Counters
  Int_t TotalSaved=0;
  Int_t TotalRawEvtCands=0;

  cout<<"Total Entries in Chain="<<_fChain->GetEntries()<<endl;
  cout<<"Going to start the event loop"<<endl;
  eventid=0;
  goodeventid=0;    
  while(_fChain->GetEntry(eventid,0)>0){   
    eventid++;
    
    if(eventid%1000==0){
      cout<<eventid<<" Events done.   With "<<TotalSaved<<" Candidates saved."<<endl;    
    }
    
    if(!FillDsEventVars()){cout<<" FillDsEventVars()=0 "<<endl;continue; }

    if(!DsEvtPassesCuts())continue;

    if(nEvt>NMAXEVT)nEvt=NMAXEVT;//protect array size
    ngoodthisevent=0; 
    goodrecoid=0;
    for(EvtIdx=0;EvtIdx<nEvt;EvtIdx++){
      TotalRawEvtCands++;
      //
      if(!FillDsInclusiveVars()){cout<<" FillDsInclusiveVars()=0 "<<endl;continue; }
       
      if(!DsPassesCuts())continue;


      //Beware of Array indexes if arrays are created too large then one runs into memory problems and strange things can happen like entries in the WS nXR=0 bin
      //if(fragndaus==0)cout<<fraglund<<" "<<taglund<<" "<<Klund<<" "<<dscharge<<endl;

      //save
      ngoodthisevent++;
      TotalSaved++;
      goodrecoid++;
      ReducedNtuple->Fill();
   
    }// loop    
    if(ngoodthisevent>0)goodeventid++;

  }

  //print summary
  cout<<"--------Summary-------"<<endl;
  cout<<"Total events = "<<eventid<<endl;
  cout<<"Total raw candidates ="<<TotalRawEvtCands<<endl;
  cout<<"Total cands saved ="<<TotalSaved<<endl;
  

  ////Save the Ntuple and histograms
  cout<<"Going to save the ntuple and histograms"<<endl; 

  //---
  if(ReducedNtuple->AutoSave("Overwrite")){ cout<<"Ntuple written."<<endl;}
  else{ cout<<"Failed to write ntuple"<<endl;return 0;}


  cout<<"Objects in the current file"<<endl;
  OutFile.ls();

  OutFile.Close(); //this will delete all objects created inside the file if they were not written  
  cout<<OutFile.GetName()<<" has been closed."<<endl;


  return 1;
}


Bool_t DRecoDsInclusiveAnalysis::FillDsEventVars(){

  ///Tag Evt vars
  if(!FillTagEventVars()){cout<<"FillTagEventVars()=0"<<endl;return 0;}
  //cout<<"Hello"<<endl;
  
  if(_DataType==0||_DataType==1){       ////for MC fill generated quantities
         
    //find gen Ds* Index     
    AllDsMCIdx=-1;
    //DsStGenMCIdx=-1;
    DsGenMCIdx=-1;
    //GamGenMCIdx=-1;

    evthasDs=0;//event has Ds 
    evthassignal=0;//event has Ds*-->Ds gamma
    evthasDspi0=0;//event has Ds*-->Ds pi0
    evthasD0gam=0;//event has D*0-->D0 gamma
    evthasD0pi0=0;//event has D*0-->D0 pi0
    evthasDCpi0=0;//event has D*+-->D+pi0  
    evthasmunu=0;//event has Ds*-->Ds gamma, Ds->mu nu
    evthasKKPi=0;//event has Ds*-->Ds gamma, Ds->KKpi

    kkmassgen=0.;
    kpimassgen=0.;

    //Bool_t SigMCCharmBalance=0; 
    for(Int_t mcc=0;mcc<mcLen;mcc++){
      if(abs(mcLund[mcc])==myDsLund){
	evthasDs=1;
      }
      if(abs(mcLund[mcc])==myDsStLund){
	if(abs(mcLund[dauIdx[mcc]])==myGammaLund){evthassignal=1;   DsGenMCIdx=dauIdx[mcc]+1;}
	if(abs(mcLund[dauIdx[mcc]+1])==myGammaLund){evthassignal=1;   DsGenMCIdx=dauIdx[mcc];}

	if(evthassignal){
	  
	  /////check if this event has a Ds*->Ds g :  Ds-->Mu Nu
	  Int_t nmu=0;
	  Int_t nnu=0;
	  Int_t ngam=0;
	  if(dauLen[DsGenMCIdx]>=1)if(abs(mcLund[dauIdx[DsGenMCIdx]])==13)nmu++;
	  if(dauLen[DsGenMCIdx]>=2)if(abs(mcLund[dauIdx[DsGenMCIdx]+1])==13)nmu++;
	  if(dauLen[DsGenMCIdx]>=3)if(abs(mcLund[dauIdx[DsGenMCIdx]+2])==13)nmu++;
	  if(dauLen[DsGenMCIdx]>=4)if(abs(mcLund[dauIdx[DsGenMCIdx]+3])==13)nmu++;
	  if(dauLen[DsGenMCIdx]>=1)if(abs(mcLund[dauIdx[DsGenMCIdx]])==14)nnu++;
	  if(dauLen[DsGenMCIdx]>=2)if(abs(mcLund[dauIdx[DsGenMCIdx]+1])==14)nnu++;
	  if(dauLen[DsGenMCIdx]>=3)if(abs(mcLund[dauIdx[DsGenMCIdx]+2])==14)nnu++;
	  if(dauLen[DsGenMCIdx]>=4)if(abs(mcLund[dauIdx[DsGenMCIdx]+3])==14)nnu++;
	  if(dauLen[DsGenMCIdx]>=1)if(abs(mcLund[dauIdx[DsGenMCIdx]])==22)ngam++;
	  if(dauLen[DsGenMCIdx]>=2)if(abs(mcLund[dauIdx[DsGenMCIdx]+1])==22)ngam++;
	  if(dauLen[DsGenMCIdx]>=3)if(abs(mcLund[dauIdx[DsGenMCIdx]+2])==22)ngam++;
	  if(dauLen[DsGenMCIdx]>=4)if(abs(mcLund[dauIdx[DsGenMCIdx]+3])==22)ngam++;
	  if(nmu==1 && nnu==1 && ((nmu+nnu+ngam)==dauLen[DsGenMCIdx])) evthasmunu=1;



	  /////Ds*->Ds g : Ds-->KKpi for Signal MC find the MC index
	  Int_t nK=0,nKs=0,nPi=0,nPi0=0,nGam=0;//need to check that its KKpi and not KKpipi0 or KK3pi
	  Int_t KKpi_KPlusMCIdx=-1,KKpi_KMinusMCIdx=-1,KKPi_PiMCIdx=-1,KKPi_GamMCIdx=-1;
	  TLorentzVector GamP4;
	  for(Int_t d1=0;d1<dauLen[DsGenMCIdx];d1++){
	    //cout<<" "<<mcLund[dauIdx[DsGenMCIdx]+d1];
	    if(abs(mcLund[dauIdx[DsGenMCIdx]+d1])==myKLund){
	      nK++; 
	      if(mcLund[dauIdx[DsGenMCIdx]+d1]>0) KKpi_KPlusMCIdx=dauIdx[DsGenMCIdx]+d1;
	      else KKpi_KMinusMCIdx=dauIdx[DsGenMCIdx]+d1;
	    }else if(abs(mcLund[dauIdx[DsGenMCIdx]+d1])==myPiLund){
	      nPi++;
	      KKPi_PiMCIdx=dauIdx[DsGenMCIdx]+d1;
	    }else if(abs(mcLund[dauIdx[DsGenMCIdx]+d1])==myGammaLund){
	      nGam++;
	      KKPi_GamMCIdx=dauIdx[DsGenMCIdx]+d1;
	      TVector3 GAMp3(mcp3CM[KKPi_GamMCIdx]*sin(acos(mccosthCM[KKPi_GamMCIdx]))*cos(mcphiCM[KKPi_GamMCIdx]),
			     mcp3CM[KKPi_GamMCIdx]*sin(acos(mccosthCM[KKPi_GamMCIdx]))*sin(mcphiCM[KKPi_GamMCIdx]),
			     mcp3CM[KKPi_GamMCIdx]*mccosthCM[KKPi_GamMCIdx]);	      
	      GamP4+=TLorentzVector(GAMp3,mcenergyCM[KKPi_GamMCIdx]);
	    }else if(abs(mcLund[dauIdx[DsGenMCIdx]+d1])==myPi0Lund) nPi0++;
	    else if(abs(mcLund[dauIdx[DsGenMCIdx]+d1])==myKS0Lund||abs(mcLund[dauIdx[DsGenMCIdx]+d1])==311) nKs++;
	    else if(dauLen[dauIdx[DsGenMCIdx]+d1]>=2){//necessary for ccbar
	      //cout<<"(";
	      for(Int_t d2=0;d2<dauLen[dauIdx[DsGenMCIdx]+d1];d2++){	  
		//cout<<","<<mcLund[dauIdx[dauIdx[DsGenMCIdx]+d1]+d2];
		if(abs(mcLund[dauIdx[dauIdx[DsGenMCIdx]+d1]+d2])==myKLund){
		  nK++;
		  if(mcLund[dauIdx[dauIdx[DsGenMCIdx]+d1]+d2]>0) KKpi_KPlusMCIdx=dauIdx[dauIdx[DsGenMCIdx]+d1]+d2;
		  else KKpi_KMinusMCIdx=dauIdx[dauIdx[DsGenMCIdx]+d1]+d2;
		}else if(abs(mcLund[dauIdx[dauIdx[DsGenMCIdx]+d1]+d2])==myPiLund){
		  nPi++;
		  KKPi_PiMCIdx=dauIdx[dauIdx[DsGenMCIdx]+d1]+d2;
		}else if(abs(mcLund[dauIdx[dauIdx[DsGenMCIdx]+d1]+d2])==myGammaLund){
		  nGam++;
		  KKPi_GamMCIdx=dauIdx[dauIdx[DsGenMCIdx]+d1]+d2;
		  TVector3 GAMp3(mcp3CM[KKPi_GamMCIdx]*sin(acos(mccosthCM[KKPi_GamMCIdx]))*cos(mcphiCM[KKPi_GamMCIdx]),
				 mcp3CM[KKPi_GamMCIdx]*sin(acos(mccosthCM[KKPi_GamMCIdx]))*sin(mcphiCM[KKPi_GamMCIdx]),
			     mcp3CM[KKPi_GamMCIdx]*mccosthCM[KKPi_GamMCIdx]);	      
		  GamP4+=TLorentzVector(GAMp3,mcenergyCM[KKPi_GamMCIdx]);
		}else if(abs(mcLund[dauIdx[dauIdx[DsGenMCIdx]+d1]+d2])==myPi0Lund) nPi0++;
		else if(abs(mcLund[dauIdx[DsGenMCIdx]+d1])==myKS0Lund) nKs++;
	      }
	      //cout<<")";
	    }
	  }
	  //cout<<endl;
	  //cout<<nK<<" "<<nPi<<" "<<nPi0<<" "<<nKs<<" "<<nGam<<endl;
	  if(nK==2&&nPi==1&&nPi0==0&&nKs==0){//&&nGam<4
	    TVector3 KPlusP3(mcp3CM[KKpi_KPlusMCIdx]*sin(acos(mccosthCM[KKpi_KPlusMCIdx]))*cos(mcphiCM[KKpi_KPlusMCIdx]),
			     mcp3CM[KKpi_KPlusMCIdx]*sin(acos(mccosthCM[KKpi_KPlusMCIdx]))*sin(mcphiCM[KKpi_KPlusMCIdx]),
			     mcp3CM[KKpi_KPlusMCIdx]*mccosthCM[KKpi_KPlusMCIdx]);
	    TLorentzVector KPlusP4(KPlusP3,mcenergyCM[KKpi_KPlusMCIdx]);
	    TVector3 KMinusP3(mcp3CM[KKpi_KMinusMCIdx]*sin(acos(mccosthCM[KKpi_KMinusMCIdx]))*cos(mcphiCM[KKpi_KMinusMCIdx]),
			      mcp3CM[KKpi_KMinusMCIdx]*sin(acos(mccosthCM[KKpi_KMinusMCIdx]))*sin(mcphiCM[KKpi_KMinusMCIdx]),
			      mcp3CM[KKpi_KMinusMCIdx]*mccosthCM[KKpi_KMinusMCIdx]);
	    TLorentzVector KMinusP4(KMinusP3,mcenergyCM[KKpi_KMinusMCIdx]);
	    TVector3 PiP3(mcp3CM[KKPi_PiMCIdx]*sin(acos(mccosthCM[KKPi_PiMCIdx]))*cos(mcphiCM[KKPi_PiMCIdx]),
			  mcp3CM[KKPi_PiMCIdx]*sin(acos(mccosthCM[KKPi_PiMCIdx]))*sin(mcphiCM[KKPi_PiMCIdx]),
			  mcp3CM[KKPi_PiMCIdx]*mccosthCM[KKPi_PiMCIdx]);
	    TLorentzVector PiP4(PiP3,mcenergyCM[KKPi_PiMCIdx]);
	    if(fabs((KPlusP4+KMinusP4+PiP4+GamP4).Mag()-DsPDGMass)<.001){
	      evthasKKPi=1;
	      //calculate the generated dalits masses
	 	   
	      kkmassgen=(KPlusP4+KMinusP4).Mag2();
	      kpimassgen=(mcLund[KKPi_PiMCIdx]<0)?(KPlusP4+PiP4).Mag2():(KMinusP4+PiP4).Mag2();
	    }
	  }    
	}
      }
      
      //check for D*0 -> D0 gamma
      if(abs(mcLund[mcc])==myDstar0Lund){
	if(abs(mcLund[dauIdx[mcc]])==myGammaLund)evthasD0gam=1;
	if(abs(mcLund[dauIdx[mcc]+1])==myGammaLund)evthasD0gam=1;
      }
    }  
    
    //cout<<"Hello"<<endl;
    //generated p*(Ds)
    dspstargen=mcp3CM[DsGenMCIdx];
 
  
    //------------------------------------------------
    //Count the number of fragmentation particles
    //-------------------------------------------------

    //count number daughters of the virtual photon
    //subtract 2 because that includes the 2 c-cbar quarks
    evtnXTgen=dauLen[2]-2;//this is not equal to the number of fragmentation pions because of resonances

    //////////Count the number of pions and gammas which do not come from Tag or Ds
    evtfragnpartgen=0;
    fragnpigen=0;
    fragnpi0gen=0;
    for(Int_t mcc=3;mcc<mcLen;mcc++)
      if(IsFragDaughter(mcc)){
	evtfragnpartgen++;      
	if(abs(mcLund[mcc])==myPiLund) fragnpigen++;
	if(mcLund[mcc]==myPi0Lund) fragnpi0gen++;
      }  
    //if there is a D* subtract 1
    for(Int_t mcc=3;mcc<mcLen;mcc++)
      if(abs(mcLund[mcc])==myDstarLund || abs(mcLund[mcc])==myDstar0Lund)
	if(mcLund[dauIdx[mcc]]!=22 && mcLund[dauIdx[mcc]+1]!=22)evtfragnpartgen--;
    if(evtfragnpartgen<0){cout<<"evtfragnpartgen is negative"<<endl;return 0;}    
    ///For nXT >5 set all to nXT=6 to account for all components
    if(evtfragnpartgen>5)evtfragnpartgen=6;  
    fragnpartgen=evtfragnpartgen;  
      
  }           
  //cout<<"Hello"<<endl;
  return 1;
}


Bool_t DRecoDsInclusiveAnalysis::DsEvtPassesCuts(){
  Bool_t pass=1;
  if(!TagEvtPassesCuts())pass=0;//note that generated Tag  is required in TagAnalysis
  //if(pass)cout<<"passDsTag"<<endl;

  ////Remove events which are not signal events: Ds*      
  if(_DataType==0)
    if(evthassignal!=1)pass=0;
  //if(pass)cout<<"pass evthassignal"<<endl;

  return pass;
}

Bool_t DRecoDsInclusiveAnalysis::FillDsInclusiveVars(){
  if(EvtIdx>=nEvt){cout<<"EvtIdx>=nEvt  "<<endl;return 0;}
  //cout<<"hello"<<endl;
  //cout<<"marker 1"<<endl;
  ///determine the indexes for each particle before used by other methods
  //Note that this order may depend Beta code reconstruction !!
  TagIdx =Evtd1Idx[EvtIdx];
  DsStIdx=Evtd2Idx[EvtIdx];
 
  TagKIdx=-1;
  FragIdx=-1;
  fraglund=0;
  Klund=0;
  pcharge=0;
  ppstar=0;
  //the K index depends on the tag mode type: for D tags it is 3rd daughter,
  if(abs(Evtd3Lund[EvtIdx])==myKLund || abs(Evtd3Lund[EvtIdx])==myKS0Lund){//-->D tags
    TagKIdx=Evtd3Idx[EvtIdx];
    Klund=Evtd3Lund[EvtIdx];
    FragIdx=Evtd4Idx[EvtIdx];  
    fraglund=Evtd4Lund[EvtIdx];
  }else if(abs(Evtd3Lund[EvtIdx])==myXibLund || abs(Evtd3Lund[EvtIdx])==myXib0Lund){//-->Lambda_c tags
    //grab the Kaon because code below was written for D tags  
    TagKIdx=PKd2Idx[Evtd3Idx[EvtIdx]];//proton is dau1
    Klund=PKd2Lund[Evtd3Idx[EvtIdx]];
    pcharge=PKd1Lund[Evtd3Idx[EvtIdx]]/abs(PKd1Lund[Evtd3Idx[EvtIdx]]);
    ppstar=Pp3CM[PKd1Idx[Evtd3Idx[EvtIdx]]];
    FragIdx=Evtd4Idx[EvtIdx];
    fraglund=Evtd4Lund[EvtIdx];  
  }else{
    //for Ds tags K does not exist and the 3rd daughter may be the X if X is not empty
    TagKIdx=-1;
    Klund=0;
    FragIdx=Evtd3Idx[EvtIdx];  
    fraglund=Evtd3Lund[EvtIdx];
  }
  //cout<<"hello"<<endl;
  //fix the Ds tag modes: copy the block contents onto the Tag block and switch the index
  if(abs(Evtd1Lund[EvtIdx])==myDsLund 
     //&& DsnDaus[TagIdx]>0 
     //&& abs(Dsd1Lund[TagIdx])!=12  && abs(Dsd2Lund[TagIdx])!=12
     //&& abs(Dsd1Lund[TagIdx])!=14  && abs(Dsd2Lund[TagIdx])!=14){
    //return 0;
     ){
    TagMass[nTag]=DsMass[TagIdx];     TagenergyCM[nTag]=DsenergyCM[TagIdx];
    Tagp3CM[nTag]=Dsp3CM[TagIdx];     TagcosthCM[nTag]=DscosthCM[TagIdx];       TagphiCM[nTag]=DsphiCM[TagIdx];
    TagLund[nTag]=DsLund[TagIdx];     TagMCIdx[nTag]=DsMCIdx[TagIdx];           TagnDaus[nTag]=DsnDaus[TagIdx];
    Tagd1Lund[nTag]=Dsd1Lund[TagIdx]; Tagd1Idx[nTag]=Dsd1Idx[TagIdx];
    Tagd2Lund[nTag]=Dsd2Lund[TagIdx]; Tagd2Idx[nTag]=Dsd2Idx[TagIdx];
    Tagd3Lund[nTag]=Dsd3Lund[TagIdx]; Tagd3Idx[nTag]=Dsd3Idx[TagIdx];
    Tagd4Lund[nTag]=Dsd4Lund[TagIdx]; Tagd4Idx[nTag]=Dsd4Idx[TagIdx];  
    TagnDof[nTag]=DsnDof[TagIdx];     TagVtxStatus[nTag]=DsVtxStatus[TagIdx];
    TagVtxx[nTag]=DsVtxx[TagIdx];     TagVtxy[nTag]=DsVtxy[TagIdx];             TagVtxz[nTag]=DsVtxz[TagIdx];
    TagFlightBSLen[nTag]=DsFlightBSLen[TagIdx]; TagFlightBSErr[nTag]=DsFlightBSErr[TagIdx];    
    TagIdx=nTag;////
    //cout<<TagIdx<<" "<<TagLund[nTag]<<" "<<Tagd1Lund[nTag]<<" "<<Tagd2Lund[nTag]<<" "<<Tagd3Lund[nTag]<<" "<<Tagd4Lund[nTag]<<endl;
  }
  //cout<<"hello"<<endl;
  //cout<<"marker 2"<<endl;

  DsIdx=DsStd1Idx[DsStIdx];
  GamIdx=DsStd2Idx[DsStIdx];
  if(TagIdx<0||TagIdx>=NMAXTAG
     ||DsStIdx<0||DsStIdx>=NMAXEVT
     ||DsIdx<0||DsIdx>=NMAXTAG
     ||GamIdx<0||GamIdx>=NMAXGAM
     ||TagKIdx>=NMAXK//Note that KIdx can be -1 when for Ds modes
     ||FragIdx>=NMAXEVT//Note that FragIdx can be -1 when nX=0
     ){
     cout<<"Bad indexes: Tag="<<TagIdx<<" K="<<TagKIdx<<" X="<<FragIdx<<" Ds*="<<DsStIdx<<" Ds="<<DsIdx<<" Gam="<<GamIdx<<endl;
     return 0;
  }



  ////////////////Fill Tag variables first (after setting indexes)  
  if(!FillTagVars()){cout<<" FillTagVars()=0"<<endl;return 0;}
  //(Note that with TagAnalysis Tag D*'s are switched to D0,D+,Ds or LambdaC!!!!!)
  //////////////
  
  //redefine variables that change with the new fit
  tagmass=EvtTagUncMass[EvtIdx];
  taglogvtxprob=log10(EvtTagUncVtxProb[EvtIdx]);
  //redefine as charge of dau of Upsilon needed for definition of DKX charge
  //note that this makes tag charge inconsistent with taglund (need to cut on this charge later)
  if(abs(Evtd1Lund[EvtIdx])==myD0Lund||abs(Evtd1Lund[EvtIdx])==myDstar0Lund)tagcharge=0;
  else tagcharge=Evtd1Lund[EvtIdx]/abs(Evtd1Lund[EvtIdx]);
  //Extra Tag quantities
  tagqual=EvtqualD[EvtIdx];  
  tagmctrue=EvtMyTagMCIdx[EvtIdx];//use my better truth-matching
  if(tagmctrue>0)tagmctrueparent=mcLund[mothIdx[tagmctrue]];else tagmctrueparent=0;
  if(evthasD0gam)tagangleToDst0=Dst0p4CM.Vect().Unit()*Tagp3vecCM.Unit(); else tagangleToDst0=100.;

 //cout<<"hello"<<endl;
  //cout<<"marker 3"<<endl;

  //////fill reduced ntuple variables    

  //"Upsilon"
  evtmass=EvtMass[EvtIdx];
  evtpstar=Evtp3CM[EvtIdx];
  evtcosstar=EvtcosthCM[EvtIdx];
  evtphistar=EvtphiCM[EvtIdx];
  evtcmenergy=EvtenergyCM[EvtIdx];
  evtvtxprob=TMath::Prob(EvtChi2[EvtIdx],EvtnDof[EvtIdx]);
  evtvtxx=EvtVtxx[EvtIdx];
  evtvtxy=EvtVtxy[EvtIdx];
  evtvtxz=EvtVtxz[EvtIdx];
  TVector3 Evtp3VCM(Evtp3CM[EvtIdx]*sin(acos(EvtcosthCM[EvtIdx]))*cos(EvtphiCM[EvtIdx]),
		    Evtp3CM[EvtIdx]*sin(acos(EvtcosthCM[EvtIdx]))*sin(EvtphiCM[EvtIdx]),
		    Evtp3CM[EvtIdx]*EvtcosthCM[EvtIdx]);
  TLorentzVector Evtp4(Evtp3VCM,EvtenergyCM[EvtIdx]);
  evtpx=Evtp3VCM.X();
  evtpy=Evtp3VCM.Y();
  evtpz=Evtp3VCM.Z(); 
 
 //cout<<"hello"<<endl;
  //cout<<"marker 4"<<endl;
  //K
  TVector3 Kp3VCM;
  Float_t Kecm;
  if(abs(Klund)==myKLund){
    Kp3VCM.SetXYZ(Kp3CM[TagKIdx]*sin(acos(KcosthCM[TagKIdx]))*cos(KphiCM[TagKIdx]),
		  Kp3CM[TagKIdx]*sin(acos(KcosthCM[TagKIdx]))*sin(KphiCM[TagKIdx]),
		  Kp3CM[TagKIdx]*KcosthCM[TagKIdx]);
    Kecm=KenergyCM[TagKIdx];
  }
  if(abs(Klund)==myKS0Lund){
    Kp3VCM.SetXYZ(Ksp3CM[TagKIdx]*sin(acos(KscosthCM[TagKIdx]))*cos(KsphiCM[TagKIdx]),
		  Ksp3CM[TagKIdx]*sin(acos(KscosthCM[TagKIdx]))*sin(KsphiCM[TagKIdx]),
		  Ksp3CM[TagKIdx]*KscosthCM[TagKIdx]);
    Kecm=KsenergyCM[TagKIdx];
  }
  TLorentzVector Kp4CM(Kp3VCM,Kecm);
  Kpstar=Kp3VCM.Mag();
  TLorentzVector Kp4=Kp4CM; Kp4.Boost(eeVelVec);//boost into lab
  Kp=Kp4.Vect().Mag();
  Kangle=(Tagp3vecCM.Unit())*(Kp3VCM.Unit()); 
  if(EvtMyKMCIdx[EvtIdx]>0)Kmctrue=EvtMyKMCIdx[EvtIdx];else Kmctrue=-1;
  Kqual=EvtqualK[EvtIdx];
  Kcharge= (abs(Klund)==myKLund ? Klund/abs(Klund) : 0 ) ;
  
  if(Klund==myKS0Lund) Kflights=KsFlightBSLen[TagKIdx]/KsFlightBSErr[TagKIdx]; else Kflights=100.;
  Kpid=5;
  if(abs(Klund)==myKLund){
    Kpid=0;    
    for(Int_t i=19;i<=23;i++){
      if((KSelectorsMap[KTrkIdx[TagKIdx]] & (1<<i)) == (1<<i)) Kpid++;//if passes selector increase 
    }
  }
   //cout<<"hello"<<endl;  
  //the Ds
  TVector3 Dsp3VCM(Dsp3CM[DsIdx]*sin(acos(DscosthCM[DsIdx]))*cos(DsphiCM[DsIdx]),
		   Dsp3CM[DsIdx]*sin(acos(DscosthCM[DsIdx]))*sin(DsphiCM[DsIdx]),
		   Dsp3CM[DsIdx]*DscosthCM[DsIdx]);
  Dsp4CM=TLorentzVector(Dsp3VCM,DsenergyCM[DsIdx]);
  TLorentzVector Dsp4Lab=Dsp4CM; Dsp4Lab.Boost(eeVelVec);
  dsmass=Dsp4CM.Mag();
  dsuncmass=EvtDsUncMass[EvtIdx];
  dsunfitmass=EvtDsMassUnFit[EvtIdx];
  dspstar=Dsp3CM[DsIdx];
  dsenergycm=Dsp4CM.T();
  dscharge=DsLund[DsIdx]/abs(DsLund[DsIdx]);
  dsangle=Dsp4CM.Vect().Unit()*Tagp3vecCM.Unit();
  dspdiff=0.;
  if(DsGenMCIdx>0&&DsGenMCIdx<MAXMCLEN){
    TVector3 DsGenPVect(mcp3CM[DsGenMCIdx]*sin(acos(mccosthCM[DsGenMCIdx]))*cos(mcphiCM[DsGenMCIdx]),
			mcp3CM[DsGenMCIdx]*sin(acos(mccosthCM[DsGenMCIdx]))*sin(mcphiCM[DsGenMCIdx]),
			mcp3CM[DsGenMCIdx]*mccosthCM[DsGenMCIdx]);
    dspxdiff=Dsp4CM.Vect().X()-DsGenPVect.X();
    dspydiff=Dsp4CM.Vect().Y()-DsGenPVect.Y();
    dspzdiff=Dsp4CM.Vect().Z()-DsGenPVect.Z();
    dspdiff=sqrt(dspxdiff*dspxdiff+dspydiff*dspydiff+dspzdiff*dspzdiff);
  }
  
  //cout<<"marker 5"<<endl;

  //the Ds*  
  TVector3 DsStp3VCM(DsStp3CM[DsStIdx]*sin(acos(DsStcosthCM[DsStIdx]))*cos(DsStphiCM[DsStIdx]),
		     DsStp3CM[DsStIdx]*sin(acos(DsStcosthCM[DsStIdx]))*sin(DsStphiCM[DsStIdx]),
		     DsStp3CM[DsStIdx]*DsStcosthCM[DsStIdx]);
  TLorentzVector DsStp4CM(DsStp3VCM,DsStenergyCM[DsStIdx]);
  TLorentzVector DsStp4CMConst(DsStp3VCM,sqrt(DsStPDGMass*DsStPDGMass+DsStp3VCM*DsStp3VCM));
  dsstuncmass=EvtDsStUncMass[EvtIdx];  
  dsstmass=DsStp4CM.Mag();
  dsstdeltam=dsstuncmass-dsuncmass;
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
 //cout<<"hello"<<endl;
  //gamma 
  TVector3 Gamp3VCM(Gamp3CM[GamIdx]*sin(acos(GamcosthCM[GamIdx]))*cos(GamphiCM[GamIdx]),
		    Gamp3CM[GamIdx]*sin(acos(GamcosthCM[GamIdx]))*sin(GamphiCM[GamIdx]),
		    Gamp3CM[GamIdx]*GamcosthCM[GamIdx]);
  TLorentzVector Gamp4CM(Gamp3VCM,GamenergyCM[GamIdx]);
  TLorentzVector Gamp4Lab=Gamp4CM;   Gamp4Lab.Boost(eeVelVec);
  dsmassconst=(DsStp4CMConst-Gamp4CM).Mag();
  gamenergy=Gamp4Lab.T();
  gamenergycm=Gamp4CM.T();
  gamangle=Gamp4CM.Vect().Unit()*Tagp3vecCM.Unit();
  gamanglelab=Gamp4Lab.Vect().CosTheta();
  gamKangle=Gamp4CM.Vect().Unit()*Kp4CM.Vect().Unit();
  gamdsstangle=Gamp4CM.Vect().Unit()*DsStp4CM.Vect().Unit();
  gammctrue=EvtMyGamMCIdx[EvtIdx];
  gamlat=GamLat[EvtIdx];
  gamA42=GamA42[EvtIdx];
  gamncrystals=GamnCrys[EvtIdx];
  gambumpdist=GamClosestBumpDist[EvtIdx];
  gamtrkdist=GamClosestTrkDist[EvtIdx];
  //boost the gamma into the rest frame of Ds* then compute angle to original direction of Ds*
  TLorentzVector Gamp4Dst=Gamp4CM;
  TVector3 DsStVelVec(DsStp3VCM*(1./DsStenergyCM[DsStIdx]));
  Gamp4Dst.Boost(-DsStVelVec);
  dsstdecayangle=Gamp4Dst.Vect().Unit()*DsStp4CM.Vect().Unit();  
  if(GamMCIdx[GamIdx]>0){
    Int_t gammcidx=GamMCIdx[GamIdx];
    if(mcLund[gammcidx]==myGammaLund){//its a photon but from where
      Int_t gammothlund=abs(mcLund[mothIdx[gammcidx]]);
      if(gammothlund==myDsStLund) gamsource=0;//signal
      else if(gammothlund==myPi0Lund) gamsource=1;
      else if(gammothlund==myEtaLund) gamsource=2;
      else if(abs(gammothlund)==myELund||abs(gammothlund)==myMuLund||abs(gammothlund)==myPiLund
	      ||abs(mcLund[gammcidx])==myKLund||abs(mcLund[gammcidx])==myPLund) gamsource=3;//radiative 
      else if(abs(gammothlund)==myDstar0Lund||abs(gammothlund)==223//D*0,omega,
	      ||abs(gammothlund)==myEtaPLund||abs(gammothlund)==10433//eta', Ds(2460) decay
	      ||abs(gammothlund)==3212) gamsource=4;//Sigma0,
      else gamsource=5;
    } else {//if its not a photon what is it
      if(abs(mcLund[gammcidx])==myELund||abs(mcLund[gammcidx])==myMuLund||abs(mcLund[gammcidx])==myPiLund
	 ||abs(mcLund[gammcidx])==myKLund||abs(mcLund[gammcidx])==myPLund) gamsource=11;//track producing a cluster in EMC
      else if(mcLund[gammcidx]==130) gamsource=12;//Klong interaction 
      else gamsource=13;
    }
  }else gamsource=-1;
 //cout<<"hello"<<endl;

  //cout<<"marker 6"<<endl;

  ///Fragmentation
  //varibles must be reset to 0. for every candidate otherwise for candidates with no X  will be set to previous one
  TLorentzVector Fragp4CM;
  TLorentzVector Fragp4Lab;    
  fragenergy=0.;
  fragcharge=0;
  fragndaus=0;
  fragangle=0.;
  fragnpis=0;
  fragnpi0s=0;
  fragpstar=0.;
  fragmass=0.;
  fragpimctrue=0;
  fragpi0mctrue=0;
  fragpi0true=-1;

  if(FragIdx>=0){//Evt has X daughter
    //caution because X can be in either the Frag,Pi,Pi0 or Gamma blocks    
    
    //general properties of X   
    TVector3 Fragp3VCM;
    if(fraglund==myPi0Lund){//X is a single Pi0
      Fragp3VCM=TVector3(Pi0p3CM[FragIdx]*sin(acos(Pi0costhCM[FragIdx]))*cos(Pi0phiCM[FragIdx]),
			 Pi0p3CM[FragIdx]*sin(acos(Pi0costhCM[FragIdx]))*sin(Pi0phiCM[FragIdx]),
			 Pi0p3CM[FragIdx]*Pi0costhCM[FragIdx]);  
      fragenergy=Pi0energyCM[FragIdx];
      fragcharge=0;
      fragndaus=1;
      fragnpi0s=1;

      if(_DataType==0||_DataType==1)
	if(FragdauMCIdx[0][EvtIdx]>0){
	  fragpi0mctrue++;
	  fragpi0true=FragdauMCIdx[0][EvtIdx];
	}

    }else if(abs(fraglund)==myPiLund){//X is a single pi+
      Fragp3VCM=TVector3(Pip3CM[FragIdx]*sin(acos(PicosthCM[FragIdx]))*cos(PiphiCM[FragIdx]),
			 Pip3CM[FragIdx]*sin(acos(PicosthCM[FragIdx]))*sin(PiphiCM[FragIdx]),
			 Pip3CM[FragIdx]*PicosthCM[FragIdx]);       
      fragenergy=PienergyCM[FragIdx];
      fragcharge=PiLund[FragIdx]/abs(PiLund[FragIdx]);
      fragndaus=1;
      fragnpis=1;

      if(_DataType==0||_DataType==1)
	if(PiMCIdx[FragIdx]>0)
	  if(abs(mcLund[PiMCIdx[FragIdx]])==211)
	    fragpimctrue++;
	  
      //PID selector on the pion
      fragpipid=0;
      for(Int_t i=10;i<=15;i++)
	if((piSelectorsMap[PiTrkIdx[FragIdx]] & (1<<i)) == (1<<i)) fragpipid++;//if passes selector increase
      fragpipidtrue=-1;
      if(PiMCIdx[FragIdx]>0) if(abs(mcLund[PiMCIdx[FragIdx]])==211) fragpipidtrue=PiMCIdx[FragIdx];

     

    }else if(abs(fraglund)==myD1NeutralLund||abs(fraglund)==myD1ChargedLund){
      Fragp3VCM=TVector3(Fragp3CM[FragIdx]*sin(acos(FragcosthCM[FragIdx]))*cos(FragphiCM[FragIdx]),
			 Fragp3CM[FragIdx]*sin(acos(FragcosthCM[FragIdx]))*sin(FragphiCM[FragIdx]),
			 Fragp3CM[FragIdx]*FragcosthCM[FragIdx]);  
      fragenergy=FragenergyCM[FragIdx];
      fragndaus=FragnDaus[FragIdx];      

      for(Int_t i=0;i<fragndaus;i++){  
	
	////////////////X dau is a pi0
	if(abs(FragdauLund[i][FragIdx])==myPi0Lund){
	  fragnpi0s++;
	  
	  if(_DataType==0||_DataType==1)
	    if(FragdauMCIdx[i][EvtIdx]>0){
	      fragpi0mctrue++;
	      if(fragpi0true==-1)fragpi0true=FragdauMCIdx[i][EvtIdx];
	    }
	}

	////////////////X dau is a pi+
	if(abs(FragdauLund[i][FragIdx])==myPiLund){
	  fragnpis++;
	  //must determine charge here charge of X is up to 3 not consistent with overall pdt type	  
	  fragcharge+=PiLund[FragdauIdx[i][FragIdx]]/abs(PiLund[FragdauIdx[i][FragIdx]]);
	  
	  Int_t fragpipidtemp=0;
	  for(Int_t s=10;s<=15;s++)
	    if((piSelectorsMap[PiTrkIdx[FragdauIdx[i][FragIdx]]] & (1<<s)) == (1<<s)) fragpipidtemp++;//if passes selector increase
	  if(fragpipidtemp<fragpipid){
	    fragpipid= fragpipidtemp;
	    fragpipidtrue=-1;
	    if(PiMCIdx[FragdauIdx[i][FragIdx]]>0)
	      if(abs(mcLund[PiMCIdx[FragdauIdx[i][FragIdx]]])==211) 
		fragpipidtrue=PiMCIdx[FragdauIdx[i][FragIdx]];	    
	  }
	  
	  if(_DataType==0||_DataType==1)
	    if(PiMCIdx[FragdauIdx[i][FragIdx]]>0)
	      if(abs(mcLund[PiMCIdx[FragdauIdx[i][FragIdx]]])==211)
		fragpimctrue++;

	}
	
      }
    }else cout<<" X lund="<<fraglund<<" not recognized !!!!!!!!!!!!!!!!!!!!"<<endl;


    ///
    Fragp4CM=TLorentzVector(Fragp3VCM,fragenergy); 
    fragangle=(Tagp3vecCM.Unit())*(Fragp3VCM.Unit());  
    fragmass=Fragp4CM.Mag();
    fragpstar=Fragp4CM.Vect().Mag();    
    Fragp4Lab=Fragp4CM; Fragp4Lab.Boost(eeVelVec);//boost into lab   
    
  }
 //cout<<"hello"<<endl;
  //other 
  evtDKXcombination=8*abs(tagcharge)+4*abs(Kcharge)+abs(fragcharge);

  //cout<<"marker 7"<<endl;

  return 1;
}



Bool_t DRecoDsInclusiveAnalysis::DsPassesCuts(){
  Bool_t pass=1;

  if(!TagPassesCuts())pass=0;
  //if(pass)cout<<"passes Tag"<<endl;
  
  if(cutdsstmass>=1){
    if(fragnpi0s==0)
      if(fabs(dsstuncmass-DsStUncMassMean0pi0)>2.5*DsStUncMassSigma0pi0[fragnpis])pass=0;    
    if(fragnpi0s==1)
      if(fabs(dsstuncmass-DsStUncMassMean1pi0)>2.5*DsStUncMassSigma1pi0)pass=0;    
    //if(pass)cout<<"passes fragnpi0s"<<endl;

    //need to cut this region otherwise multiplicity weighting later doesnt work
    if(cutdsstmass==1)if(dsmass<DsMassMin)pass=0; 
    //
    if(cutdsstmass==2)if(dsmass>DsMassMin)pass=0;
    //if(pass)cout<<"passes DsMassMin"<<endl;
  }

  if(cutevtvtxprob==1){
  }
  if(cutdspstar==1){
  }
  if(cutfragngams==1){
    if(fragnpi0s==1&&fragnpis>2)pass=0;//cut not imposed in Beta Code
    //if(pass)cout<<"passes fragnpi0s==1&&fragnpis>2"<<endl;
    if(abs(taglund)==3)pass=0;//Ds tags have poor quality
    //if(pass)cout<<"passes abs(taglund)==3"<<endl;

    //make sure charge is right but should have been imposed in beta code (fixes some bug with LambdaC modes)
    if(abs(tagcharge+Kcharge+fragcharge+pcharge)!=1)pass=0;
    //if(pass)cout<<"passes abs(tagcharge+Kcharge+fragcharge+pcharge)!="<<endl;
  }

  return pass;
}


Int_t DRecoDsInclusiveAnalysis::SetDsInclusiveBranches(){
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
  if(_fChain->GetBranch("Evtd2Lund"))_fChain->SetBranchAddress("Evtd2Lund",Evtd2Lund);//Ds*
  if(_fChain->GetBranch("Evtd2Idx"))_fChain->SetBranchAddress("Evtd2Idx",Evtd2Idx);
  if(_fChain->GetBranch("Evtd3Lund"))_fChain->SetBranchAddress("Evtd3Lund",Evtd3Lund);//K,X
  if(_fChain->GetBranch("Evtd3Idx"))_fChain->SetBranchAddress("Evtd3Idx",Evtd3Idx);
  if(_fChain->GetBranch("Evtd4Lund"))_fChain->SetBranchAddress("Evtd4Lund",Evtd4Lund);//X
  if(_fChain->GetBranch("Evtd4Idx"))_fChain->SetBranchAddress("Evtd4Idx",Evtd4Idx);
  if(_fChain->GetBranch("EvtVtxx"))_fChain->SetBranchAddress("EvtVtxx",EvtVtxx);
  if(_fChain->GetBranch("EvtVtxy"))_fChain->SetBranchAddress("EvtVtxy",EvtVtxy);
  if(_fChain->GetBranch("EvtVtxz"))_fChain->SetBranchAddress("EvtVtxz",EvtVtxz);
  if(_fChain->GetBranch("EvtTagUncVtxProb"))_fChain->SetBranchAddress("EvtTagUncVtxProb",EvtTagUncVtxProb);
  if(_fChain->GetBranch("EvtDsStUncMass"))_fChain->SetBranchAddress("EvtDsStUncMass",EvtDsStUncMass);
  if(_fChain->GetBranch("EvtDsUncMass"))_fChain->SetBranchAddress("EvtDsUncMass",EvtDsUncMass);
  if(_fChain->GetBranch("EvtTagUncMass"))_fChain->SetBranchAddress("EvtTagUncMass",EvtTagUncMass);
  if(_fChain->GetBranch("EvtqualD"))_fChain->SetBranchAddress("EvtqualD",EvtqualD);
  if(_fChain->GetBranch("EvtqualK"))_fChain->SetBranchAddress("EvtqualK",EvtqualK);
  if(_fChain->GetBranch("EvtqualX"))_fChain->SetBranchAddress("EvtqualX",EvtqualX);
  if(_fChain->GetBranch("EvtMyGamMCIdx"))_fChain->SetBranchAddress("EvtMyGamMCIdx",EvtMyGamMCIdx);  
  if(_fChain->GetBranch("EvtMyKMCIdx"))_fChain->SetBranchAddress("EvtMyKMCIdx",EvtMyKMCIdx);  
  if(_fChain->GetBranch("EvtMyTagMCIdx"))_fChain->SetBranchAddress("EvtMyTagMCIdx",EvtMyTagMCIdx);  
  //if(_fChain->GetBranch("EvtMyXMCIdx"))_fChain->SetBranchAddress("EvtMyXMCIdx",EvtMyXMCIdx); 
  if(_fChain->GetBranch("EvtDsStPxRaw"))_fChain->SetBranchAddress("EvtDsStPxRaw",EvtDsStPxRaw); 
  if(_fChain->GetBranch("EvtDsStPyRaw"))_fChain->SetBranchAddress("EvtDsStPyRaw",EvtDsStPyRaw); 
  if(_fChain->GetBranch("EvtDsStPzRaw"))_fChain->SetBranchAddress("EvtDsStPzRaw",EvtDsStPzRaw); 
  if(_fChain->GetBranch("EvtDsStERaw"))_fChain->SetBranchAddress("EvtDsStERaw",EvtDsStERaw); 
  if(_fChain->GetBranch("EvtDsStPxCorr"))_fChain->SetBranchAddress("EvtDsStPxCorr",EvtDsStPxCorr); 
  if(_fChain->GetBranch("EvtDsStPyCorr"))_fChain->SetBranchAddress("EvtDsStPyCorr",EvtDsStPyCorr); 
  if(_fChain->GetBranch("EvtDsStPzCorr"))_fChain->SetBranchAddress("EvtDsStPzCorr",EvtDsStPzCorr); 
  if(_fChain->GetBranch("EvtDsStECorr"))_fChain->SetBranchAddress("EvtDsStECorr",EvtDsStECorr); 
  if(_fChain->GetBranch("EvtDsMassUnFit"))_fChain->SetBranchAddress("EvtDsMassUnFit",EvtDsMassUnFit);
  if(_fChain->GetBranch("EvtMyXd1MCIdx"))_fChain->SetBranchAddress("EvtMyXd1MCIdx",FragdauMCIdx[0]);
  if(_fChain->GetBranch("EvtMyXd2MCIdx"))_fChain->SetBranchAddress("EvtMyXd2MCIdx",FragdauMCIdx[1]);
  if(_fChain->GetBranch("EvtMyXd3MCIdx"))_fChain->SetBranchAddress("EvtMyXd3MCIdx",FragdauMCIdx[2]);
  if(_fChain->GetBranch("EvtMyXd4MCIdx"))_fChain->SetBranchAddress("EvtMyXd4MCIdx",FragdauMCIdx[3]);
  if(_fChain->GetBranch("EvtMyXd5MCIdx"))_fChain->SetBranchAddress("EvtMyXd5MCIdx",FragdauMCIdx[4]);
  if(_fChain->GetBranch("EvtMyXd6MCIdx"))_fChain->SetBranchAddress("EvtMyXd6MCIdx",FragdauMCIdx[5]);
  if(_fChain->GetBranch("EvtMyXd7MCIdx"))_fChain->SetBranchAddress("EvtMyXd7MCIdx",FragdauMCIdx[6]);
  if(_fChain->GetBranch("EvtMyXd8MCIdx"))_fChain->SetBranchAddress("EvtMyXd8MCIdx",FragdauMCIdx[7]);
  if(_fChain->GetBranch("EvtMyXd9MCIdx"))_fChain->SetBranchAddress("EvtMyXd9MCIdx",FragdauMCIdx[8]);
  if(_fChain->GetBranch("EvtMyXd10MCIdx"))_fChain->SetBranchAddress("EvtMyXd10MCIdx",FragdauMCIdx[9]);
  if(_fChain->GetBranch("EvtMyXPi0Mass"))_fChain->SetBranchAddress("EvtMyXPi0Mass",FragPi0Mass);


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
  if(_fChain->GetBranch("DsnDaus"))_fChain->SetBranchAddress("DsnDaus",DsnDaus);
  if(_fChain->GetBranch("DsMCIdx"))_fChain->SetBranchAddress("DsMCIdx",DsMCIdx);
  if(_fChain->GetBranch("Dsd1Lund"))_fChain->SetBranchAddress("Dsd1Lund",Dsd1Lund);
  if(_fChain->GetBranch("Dsd1Idx"))_fChain->SetBranchAddress("Dsd1Idx",Dsd1Idx);
  if(_fChain->GetBranch("Dsd2Lund"))_fChain->SetBranchAddress("Dsd2Lund",Dsd2Lund);
  if(_fChain->GetBranch("Dsd2Idx"))_fChain->SetBranchAddress("Dsd2Idx",Dsd2Idx);
  if(_fChain->GetBranch("Dsd3Lund"))_fChain->SetBranchAddress("Dsd3Lund",Dsd3Lund);
  if(_fChain->GetBranch("Dsd3Idx"))_fChain->SetBranchAddress("Dsd3Idx",Dsd3Idx);
  if(_fChain->GetBranch("Dsd4Lund"))_fChain->SetBranchAddress("Dsd4Lund",Dsd4Lund);
  if(_fChain->GetBranch("Dsd4Idx"))_fChain->SetBranchAddress("Dsd4Idx",Dsd4Idx);
  if(_fChain->GetBranch("DsChi2"))_fChain->SetBranchAddress("DsChi2",DsChi2);
  if(_fChain->GetBranch("DsnDof"))_fChain->SetBranchAddress("DsnDof",DsnDof);
  if(_fChain->GetBranch("DsVtxStatus"))_fChain->SetBranchAddress("DsVtxStatus",DsVtxStatus);
  if(_fChain->GetBranch("DsVtxx"))_fChain->SetBranchAddress("DsVtxx",DsVtxx);
  if(_fChain->GetBranch("DsVtxy"))_fChain->SetBranchAddress("DsVtxy",DsVtxy);
  if(_fChain->GetBranch("DsVtxz"))_fChain->SetBranchAddress("DsVtxz",DsVtxz);
  if(_fChain->GetBranch("DsFlightBSLen"))_fChain->SetBranchAddress("DsFlightBSLen",DsFlightBSLen);

  if(_fChain->GetBranch("nPK"))_fChain->SetBranchAddress("nPK",&nPK);
  if(_fChain->GetBranch("PKMass"))_fChain->SetBranchAddress("PKMass",PKMass); 
  if(_fChain->GetBranch("PKp3CM"))_fChain->SetBranchAddress("PKp3CM",PKp3CM);
  if(_fChain->GetBranch("PKcosthCM"))_fChain->SetBranchAddress("PKcosthCM",PKcosthCM);
  if(_fChain->GetBranch("PKphiCM"))_fChain->SetBranchAddress("PKphiCM",PKphiCM);
  if(_fChain->GetBranch("PKenergyCM"))_fChain->SetBranchAddress("PKenergyCM",PKenergyCM);
  if(_fChain->GetBranch("PKLund"))_fChain->SetBranchAddress("PKLund",PKLund); 
  if(_fChain->GetBranch("PKd1Lund"))_fChain->SetBranchAddress("PKd1Lund",PKd1Lund);
  if(_fChain->GetBranch("PKd1Idx"))_fChain->SetBranchAddress("PKd1Idx",PKd1Idx);
  if(_fChain->GetBranch("PKd2Lund"))_fChain->SetBranchAddress("PKd2Lund",PKd2Lund);
  if(_fChain->GetBranch("PKd2Idx"))_fChain->SetBranchAddress("PKd2Idx",PKd2Idx);


  if(_fChain->GetBranch("nGam"))_fChain->SetBranchAddress("nGam",&nGam); 
  if(_fChain->GetBranch("Gamp3CM"))_fChain->SetBranchAddress("Gamp3CM",Gamp3CM);
  if(_fChain->GetBranch("GamcosthCM"))_fChain->SetBranchAddress("GamcosthCM",GamcosthCM);
  if(_fChain->GetBranch("GamphiCM"))_fChain->SetBranchAddress("GamphiCM",GamphiCM);
  if(_fChain->GetBranch("GamenergyCM"))_fChain->SetBranchAddress("GamenergyCM",GamenergyCM);
  if(_fChain->GetBranch("GamLund"))_fChain->SetBranchAddress("GamLund",GamLund);


  cout<<"Done linking DsInclusive Chain Branches"<<endl;
 
  return SetTagBranches();
}



Int_t DRecoDsInclusiveAnalysis::MakeDsInclusiveReducedNtupleBranches(){
 
  ReducedNtuple->Branch("evtmass",&evtmass,"evtmass/F");
  ReducedNtuple->Branch("evtpstar",&evtpstar,"evtpstar/F");
  ReducedNtuple->Branch("evtpx",&evtpx,"evtpx/F");
  ReducedNtuple->Branch("evtpy",&evtpy,"evtpy/F");
  ReducedNtuple->Branch("evtpz",&evtpz,"evtpz/F");
  ReducedNtuple->Branch("evtcmenergy",&evtcmenergy,"evtcmenergy/F");
  ReducedNtuple->Branch("evtcosstar",&evtcosstar,"evtcosstar/F");
  ReducedNtuple->Branch("evtphistar",&evtphistar,"evtphistar/F");
  ReducedNtuple->Branch("evtcharge",&evtcharge,"evtcharge/I");
  ReducedNtuple->Branch("nEvt",&nEvt,"nEvt/I");
  ReducedNtuple->Branch("EvtIdx",&EvtIdx,"EvtIdx/I");
  ReducedNtuple->Branch("evtbestcand",&evtbestcand,"evtbestcand/I");
  ReducedNtuple->Branch("evtvtxprob",&evtvtxprob,"evtvtxprob/F");
  ReducedNtuple->Branch("evtvtxx",&evtvtxx,"evtvtxx/F");
  ReducedNtuple->Branch("evtvtxy",&evtvtxy,"evtvtxy/F");
  ReducedNtuple->Branch("evtvtxz",&evtvtxz,"evtvtxz/F");
  ReducedNtuple->Branch("tagqual",&tagqual,"tagqual/I");
  ReducedNtuple->Branch("evtnXTgen",&evtnXTgen,"evtnXTgen/I");
  ReducedNtuple->Branch("evtDKXcombination",&evtDKXcombination,"evtDKXcombination/I");
  ReducedNtuple->Branch("evthassignal",&evthassignal,"evthassignal/I");
  ReducedNtuple->Branch("evthasKKPi",&evthasKKPi,"evthasKKPi/I");
  ReducedNtuple->Branch("evthasmunu",&evthasmunu,"evthasmunu/I");
  ReducedNtuple->Branch("evthasDs",&evthasDs,"evthasDs/I");
  ReducedNtuple->Branch("evthasKK",&evthasKK,"evthasKK/I");
  ReducedNtuple->Branch("evthasDspi0",&evthasDspi0,"evthasDspi0/I");
  ReducedNtuple->Branch("evthasD0gam",&evthasD0gam,"evthasD0gam/I");
  ReducedNtuple->Branch("evthasD0pi0",&evthasD0pi0,"evthasD0pi0/I");
  ReducedNtuple->Branch("evthasDCpi0",&evthasDCpi0,"evthasDCpi0/I");
  ReducedNtuple->Branch("evtfragnpartgen",&evtfragnpartgen,"evtfragnpartgen/I");
  ReducedNtuple->Branch("evthasDstarPlus",&evthasDstarPlus,"evthasDstarPlus/I");
  ReducedNtuple->Branch("evtmcdatacorrweight",&evtmcdatacorrweight,"evtmcdatacorrweight/F");
  ReducedNtuple->Branch("ngoodthisevent",&ngoodthisevent,"ngoodthisevent/I");

  ReducedNtuple->Branch("DsGenMCIdx",&DsGenMCIdx,"DsGenMCIdx/I");
  ReducedNtuple->Branch("AllDsMCIdx",&AllDsMCIdx,"AllDsMCIdx/I");
  ReducedNtuple->Branch("kkpimassgen",&kkpimassgen,"kkpimassgen/F");

  ReducedNtuple->Branch("tagsidemass",&tagsidemass,"tagsidemass/F");  
  ReducedNtuple->Branch("tagsidepstar",&tagsidepstar,"tagsidepstar/F"); 
  ReducedNtuple->Branch("tagsideenergycm",&tagsideenergycm,"tagsideenergycm/F"); 
  ReducedNtuple->Branch("tagsidedeltae",&tagsidedeltae,"tagsidedeltae/F");  
  ReducedNtuple->Branch("tagsidecosthcm",&tagsidecosthcm,"tagsidecosthcm/F");
 
  ReducedNtuple->Branch("sigsidemass",&sigsidemass,"sigsidemass/F");  
  ReducedNtuple->Branch("sigsidepstar",&sigsidepstar,"sigsidepstar/F"); 
  ReducedNtuple->Branch("sigsideenergycm",&sigsideenergycm,"sigsideenergycm/F"); 
  ReducedNtuple->Branch("sigsidedeltae",&sigsidedeltae,"sigsidedeltae/F");  
  ReducedNtuple->Branch("sigsidecosthcm",&sigsidecosthcm,"sigsidecosthcm/F");

  ReducedNtuple->Branch("dsp",&dsp,"dsp/F"); 
  ReducedNtuple->Branch("dspstar",&dspstar,"dspstar/F"); 
  ReducedNtuple->Branch("dsenergycm",&dsenergycm,"dsenergycm/F"); 
  ReducedNtuple->Branch("dsmass",&dsmass,"dsmass/F"); 
  ReducedNtuple->Branch("dsmassconst",&dsmassconst,"dsmassconst/F");
  ReducedNtuple->Branch("dsangle",&dsangle,"dsangle/F"); 
  ReducedNtuple->Branch("dspxdiff",&dspxdiff,"dspxdiff/F");
  ReducedNtuple->Branch("dspydiff",&dspydiff,"dspydiff/F");
  ReducedNtuple->Branch("dspzdiff",&dspzdiff,"dspzdiff/F");
  ReducedNtuple->Branch("dspdiff",&dspdiff,"dspdiff/F");
  ReducedNtuple->Branch("dsuncmass",&dsuncmass,"dsuncmass/F");
  ReducedNtuple->Branch("dsunfitmass",&dsunfitmass,"dsunfitmass/F");
  ReducedNtuple->Branch("dscharge",&dscharge,"dscharge/I");
  ReducedNtuple->Branch("dspstargen",&dspstargen,"dspstargen/F");


  ReducedNtuple->Branch("dsstp",&dsstp,"dsstp/F"); 
  ReducedNtuple->Branch("dsstpstar",&dsstpstar,"dsstpstar/F"); 
  ReducedNtuple->Branch("dsstenergycm",&dsstenergycm,"dsstenergycm/F"); 
  ReducedNtuple->Branch("dsstmass",&dsstmass,"dsstmass/F"); 
  ReducedNtuple->Branch("dsstuncmass",&dsstuncmass,"dsstuncmass/F"); 
  ReducedNtuple->Branch("dsstdeltam",&dsstdeltam,"dsstdeltam/F"); 
  ReducedNtuple->Branch("dsstangle",&dsstangle,"dsstangle/F"); 
  ReducedNtuple->Branch("dsstdecayangle",&dsstdecayangle,"dsstdecayangle/F");
  ReducedNtuple->Branch("dsstcorrpdiff",&dsstcorrpdiff,"dsstcorrpdiff/F");
  ReducedNtuple->Branch("dsstfitpdiff",&dsstfitpdiff,"dsstfitpdiff/F");

  ReducedNtuple->Branch("gamenergy",&gamenergy,"gamenergy/F");
  ReducedNtuple->Branch("gamenergycm",&gamenergycm,"gamenergycm/F");
  ReducedNtuple->Branch("gamangle",&gamangle,"gamangle/F");
  ReducedNtuple->Branch("gamanglelab",&gamanglelab,"gamanglelab/F");
  ReducedNtuple->Branch("gamKangle",&gamKangle,"gamKangle/F");
  ReducedNtuple->Branch("gamdsstangle",&gamdsstangle,"gamdsstangle/F");
  ReducedNtuple->Branch("gammctrue",&gammctrue,"gammctrue/I");
  ReducedNtuple->Branch("gamsource",&gamsource,"gamsource/I");
  ReducedNtuple->Branch("gamlat",&gamlat,"gamlat/F");
  ReducedNtuple->Branch("gamA42",&gamA42,"gamA42/F");
  ReducedNtuple->Branch("gamncrystals",&gamncrystals,"gamncrystals/F");
  ReducedNtuple->Branch("gambumpdist",&gambumpdist,"gambumpdist/F");
  ReducedNtuple->Branch("gamtrkdist",&gamtrkdist,"gamtrkdist/F");  

  ReducedNtuple->Branch("Kpstar",&Kpstar,"Kpstar/F");
  ReducedNtuple->Branch("Kp",&Kp,"Kp/F");
  ReducedNtuple->Branch("Kangle",&Kangle,"Kangle/F");
  ReducedNtuple->Branch("Kmctrue",&Kmctrue,"Kmctrue/I"); 
  ReducedNtuple->Branch("Klund",&Klund,"Klund/I");
  ReducedNtuple->Branch("Kqual",&Kqual,"Kqual/I");
  ReducedNtuple->Branch("Kcharge",&Kcharge,"Kcharge/I");
  ReducedNtuple->Branch("Ksource",&Ksource,"Ksource/I"); 
  ReducedNtuple->Branch("Kflights",&Kflights,"Kflights/F");
  ReducedNtuple->Branch("Kpid",&Kpid,"Kpid/I");

  ReducedNtuple->Branch("pcharge",&pcharge,"pcharge/I");
  ReducedNtuple->Branch("ppstar",&ppstar,"ppstar/F");

  ReducedNtuple->Branch("fragenergy",&fragenergy,"fragenergy/F");
  ReducedNtuple->Branch("fragpstar",&fragpstar,"fragpstar/F");
  ReducedNtuple->Branch("fragcharge",&fragcharge,"fragcharge/I");
  ReducedNtuple->Branch("fraglund",&fraglund,"fraglund/I");
  ReducedNtuple->Branch("fragndaus",&fragndaus,"fragndaus/I");
  ReducedNtuple->Branch("fragnpis",&fragnpis,"fragnpis/I");
  ReducedNtuple->Branch("fragnpi0s",&fragnpi0s,"fragnpi0s/I");
  ReducedNtuple->Branch("fragngams",&fragngams,"fragngams/I");
  //ReducedNtuple->Branch("fragmctrue",&fragmctrue,"fragmctrue/I");
  ReducedNtuple->Branch("fragpimctrue",&fragpimctrue,"fragpimctrue/I");
  ReducedNtuple->Branch("fragpi0mctrue",&fragpi0mctrue,"fragpi0mctrue/I");
  ReducedNtuple->Branch("fraggammctrue",&fraggammctrue,"fraggammctrue/I");
  ReducedNtuple->Branch("fragangle",&fragangle,"fragangle/F");
  ReducedNtuple->Branch("fragqual",&fragqual,"fragqual/I");
  ReducedNtuple->Branch("fragnpartgen",&fragnpartgen,"fragnpartgen/I");
  ReducedNtuple->Branch("fragnpigen",&fragnpigen,"fragnpigen/I");
  ReducedNtuple->Branch("fragnpi0gen",&fragnpi0gen,"fragnpi0gen/I");
  ReducedNtuple->Branch("fragngamgen",&fragngamgen,"fragngamgen/I");
  ReducedNtuple->Branch("fragmass",&fragmass,"fragmass/F");
  ReducedNtuple->Branch("fragpdiff",&fragpdiff,"fragpdiff/F");
  ReducedNtuple->Branch("fragp4diff",&fragp4diff,"fragp4diff/F");
  ReducedNtuple->Branch("fragangletogen",&fragangletogen,"fragangletogen/F");
  ReducedNtuple->Branch("fragpipid",&fragpipid,"fragpipid/I");
  ReducedNtuple->Branch("fragpipidtrue",&fragpipidtrue,"fragpipidtrue/I");
  ReducedNtuple->Branch("fragD0ToKpi",&fragD0ToKpi,"fragD0ToKpi/F");
  ReducedNtuple->Branch("fragD0ToKpipi0",&fragD0ToKpipi0,"fragD0ToKpipi0/F");
  ReducedNtuple->Branch("fragD0ToK3pi",&fragD0ToK3pi,"fragD0ToK3pi/F");
  ReducedNtuple->Branch("fragDcToKpipi",&fragDcToKpipi,"fragDcToKpipi/F");
  

  ReducedNtuple->Branch("fragpiavgangle",&fragpiavgangle,"fragpiavgangle/F");
  ReducedNtuple->Branch("fragpip",&fragpip,"fragpip/F");
  ReducedNtuple->Branch("fragpipstar",&fragpipstar,"fragpipstar/F");
  ReducedNtuple->Branch("fragpi0avgangle",&fragpi0avgangle,"fragpi0avgangle/F");
  ReducedNtuple->Branch("fragpi0avggamenergy",&fragpi0avggamenergy,"fragpi0avggamenergy/F");
  ReducedNtuple->Branch("fragpi0mass",&fragpi0mass,"fragpi0mass/F");
  ReducedNtuple->Branch("fragpi0p",&fragpi0p,"fragpi0p/F");
  ReducedNtuple->Branch("fragpi0pstar",&fragpi0pstar,"fragpi0pstar/F");
  ReducedNtuple->Branch("fragpi0true",&fragpi0true,"fragpi0true/I");
  ReducedNtuple->Branch("fragpi0gamenergy",&fragpi0gamenergy,"fragpi0gamenergy/F");
  ReducedNtuple->Branch("fragpi0gamenergymodified",&fragpi0gamenergymodified,"fragpi0gamenergymodified/F");
  ReducedNtuple->Branch("fraggamangle",&fraggamangle,"fraggamangle/F");

  ReducedNtuple->Branch("tagangleToDst0",&tagangleToDst0,"tagangleToDst0/F");

  ReducedNtuple->Branch("BDTOutPut",&BDTOutPut,"BDTOutPut/F");


  //Ds-->KKpi eff
  ReducedNtuple->Branch("kkmassgen",&kkmassgen,"kkmassgen/F");
  ReducedNtuple->Branch("kpimassgen",&kpimassgen,"kpimassgen/F");



  MakeTagReducedNtupleBranches();
  cout<<"Done Making DsInclusive Reduced Ntuple Branches"<<endl;
  return 1;
}


Int_t DRecoDsInclusiveAnalysis::ReduceNtupleGen(){
  cout<<"Going to reduce the ntuple"<<endl;
  
  if(_fChain==NULL){
    cout<<"No chain."<<endl;
    return 0;
  } 

  ///link the branches in the input chain
  SetEventBranches();


  //the file must be created here so the 'new' histos and Tree is created in hard drive //OutFile=new 
  TFile OutFile(_OutputDir+"/NtupleGen_New.root","recreate",_OutputDir+"/NtupleGen_New.root",1);//1=minimal compression
  if(OutFile.IsZombie()){cout<<"Failed to create file"<<endl;return 0;}


  //create new reduced ntuple
  TTree Ntuple("Ntuple","Ntuple",0);
  //Long64_t maxsize=10000000000;//10Gb
  Ntuple.SetMaxTreeSize(900000000);//split files
  //disable autosaving 
  //Long64_t fautosave=10000000000;//
  Ntuple.SetAutoSave(900000000);

  //create the branches in the Ntuple
  Float_t tagmass;Ntuple.Branch("tagmass",&tagmass,"tagmass/F");
  Float_t tagpstar;Ntuple.Branch("tagpstar",&tagpstar,"tagpstar/F");
  Float_t gamangle;Ntuple.Branch("gamangle",&gamangle,"gamangle/F");
  Float_t dspstar;Ntuple.Branch("dspstar",&dspstar,"dspstar/F");
  Float_t dsangle;Ntuple.Branch("dsangle",&dsangle,"dsangle/F");
  Float_t dstrkangle;Ntuple.Branch("dstrkangle",&dstrkangle,"dstrkangle/F");
  Int_t dsntrks;Ntuple.Branch("dsntrks",&dsntrks,"dsntrks/I");
  Int_t fragndaus;Ntuple.Branch("fragndaus",&fragndaus,"fragndaus/I");
  Int_t fragnpis;Ntuple.Branch("fragnpis",&fragnpis,"fragnpis/I");
  Int_t fragnpi0s;Ntuple.Branch("fragnpi0s",&fragnpi0s,"fragnpi0s/I");
  Int_t fragngams;Ntuple.Branch("fragngams",&fragngams,"fragngams/I");
  Float_t fragcharge;Ntuple.Branch("fragcharge",&fragcharge,"fragcharge/F");
  Float_t fragangle;Ntuple.Branch("fragangle",&fragangle,"fragangle/F");
  Float_t fragpstar;Ntuple.Branch("fragpstar",&fragpstar,"fragpstar/F");
  Float_t fragpi0lowegam;Ntuple.Branch("fragpi0lowegam",&fragpi0lowegam,"fragpi0lowegam/F");
  Float_t fragpi0angle;Ntuple.Branch("fragpi0angle",&fragpi0angle,"fragpi0angle/F");
  Float_t fragpiangle;Ntuple.Branch("fragpiangle",&fragpiangle,"fragpiangle/F");
  Float_t fragpi0avgegam;Ntuple.Branch("fragpi0avgegam",&fragpi0avgegam,"fragpi0avgegam/F");
  Float_t fragpi0avgangle;Ntuple.Branch("fragpi0avgangle",&fragpi0avgangle,"fragpi0avgangle/F");
  Float_t fragpiavgangle;Ntuple.Branch("fragpiavgangle",&fragpiavgangle,"fragpiavgangle/F");
  Float_t fraggamangle;Ntuple.Branch("fraggamangle",&fraggamangle,"fraggamangle/F");
  Float_t fragpionavgpstar;Ntuple.Branch("fragpionavgpstar",&fragpionavgpstar,"fragpionavgpstar/F");


  Int_t fragcompgam=0;
  Int_t fragcomppi=0;
  Int_t fragcomppi0=0;
  //Gammas from etas should be counted because there can be plus/minus one particle missing in the signal reco.
  //phi-> KK are very small these are the results for 1081 events:
  //Number of gam= 1432
  //Number of pi = 2641
  //Number of pi0= 1100
  //Number of eta= 56
  //Number of phi= 29
  
  
  //Counters
  Int_t MCBCounterTotal=0;


  cout<<"Total Entries in Chain="<<_fChain->GetEntries()<<endl;
  eventid=0;
  while(_fChain->GetEntry(eventid,0)>0){   
    eventid++;
    
    if(eventid%100==0){
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
    tagmass=mcmass[GenTagIdx];
    tagpstar=mcp3CM[GenTagIdx];
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
    dspstar=mcp3CM[DsGenMCIdx];
    TVector3 Dsp3Vec(mcp3CM[DsGenMCIdx]*sin(acos(mccosthCM[DsGenMCIdx]))*cos(mcphiCM[DsGenMCIdx]),
		     mcp3CM[DsGenMCIdx]*sin(acos(mccosthCM[DsGenMCIdx]))*sin(mcphiCM[DsGenMCIdx]),
		     mcp3CM[DsGenMCIdx]*mccosthCM[DsGenMCIdx]);

    dsangle=Dsp3Vec.Unit()*Tagp3Vec.Unit();
    
    //count Ds daughters (pi, K, pi0, e, mu)
    dsntrks=0;     
    //find signal track closest to Tag direction
    dstrkangle=-1.;//set to opposite side of Tag    
    
    Bool_t pass=1;
    for(Int_t dsdauid=dauIdx[DsGenMCIdx];dsdauid<dauIdx[DsGenMCIdx]+dauLen[DsGenMCIdx];dsdauid++){//check first generation
      if(dsdauid<0||dsdauid>=MAXMCLEN){pass=0;continue;}
      if(abs(mcLund[dsdauid])==myPiLund
	 ||abs(mcLund[dsdauid])==myKLund
	 ||abs(mcLund[dsdauid])==myPi0Lund
	 ||abs(mcLund[dsdauid])==myELund
	 ||abs(mcLund[dsdauid])==myMuLund){
      	TVector3 Dsdaup3Vec(mcp3CM[dsdauid]*sin(acos(mccosthCM[dsdauid]))*cos(mcphiCM[dsdauid]),
			    mcp3CM[dsdauid]*sin(acos(mccosthCM[dsdauid]))*sin(mcphiCM[dsdauid]),
			    mcp3CM[dsdauid]*mccosthCM[dsdauid]);
	if(Dsdaup3Vec.Unit()*Tagp3Vec.Unit()>dstrkangle)
	  dstrkangle=Dsdaup3Vec.Unit()*Tagp3Vec.Unit();	        
	dsntrks++;
      }else if(dauLen[dsdauid]>=2){//check 2 generations
	for(Int_t dsdaudauid=dauIdx[dsdauid];dsdaudauid<dauIdx[dsdauid]+dauLen[dsdauid];dsdaudauid++){
	  if(dsdaudauid<0||dsdaudauid>=MAXMCLEN){pass=0;continue;}
	  
	  if(abs(mcLund[dsdaudauid])==myPiLund
	     ||abs(mcLund[dsdaudauid])==myKLund
	     ||abs(mcLund[dsdaudauid])==myPi0Lund
	     ||abs(mcLund[dsdaudauid])==myELund
	     ||abs(mcLund[dsdaudauid])==myMuLund){
	    TVector3 Dsdaudaup3Vec(mcp3CM[dsdaudauid]*sin(acos(mccosthCM[dsdaudauid]))*cos(mcphiCM[dsdaudauid]),
				   mcp3CM[dsdaudauid]*sin(acos(mccosthCM[dsdaudauid]))*sin(mcphiCM[dsdaudauid]),
				   mcp3CM[dsdaudauid]*mccosthCM[dsdaudauid]);
	    if(Dsdaudaup3Vec.Unit()*Tagp3Vec.Unit()>dstrkangle)
	      dstrkangle=Dsdaudaup3Vec.Unit()*Tagp3Vec.Unit();	   
	    dsntrks++;
	  }else if(dauLen[dsdaudauid]>=2){//check 3 generations
 	    for(Int_t dsdaudaudauid=dauIdx[dsdaudauid];dsdaudaudauid<dauIdx[dsdaudauid]+dauLen[dsdaudauid];dsdaudaudauid++){
	      if(dsdaudaudauid<0||dsdaudaudauid>=MAXMCLEN){pass=0;continue;}

	      if(abs(mcLund[dsdaudaudauid])==myPiLund
		 ||abs(mcLund[dsdaudaudauid])==myKLund
		 ||abs(mcLund[dsdaudaudauid])==myPi0Lund
		 ||abs(mcLund[dsdaudaudauid])==myELund
		 ||abs(mcLund[dsdaudaudauid])==myMuLund){
		TVector3 Dsdaudaudaup3Vec(mcp3CM[dsdaudaudauid]*sin(acos(mccosthCM[dsdaudaudauid]))*cos(mcphiCM[dsdaudaudauid]),
					  mcp3CM[dsdaudaudauid]*sin(acos(mccosthCM[dsdaudaudauid]))*sin(mcphiCM[dsdaudaudauid]),
					  mcp3CM[dsdaudaudauid]*mccosthCM[dsdaudaudauid]);
		if(Dsdaudaudaup3Vec.Unit()*Tagp3Vec.Unit()>dstrkangle)
		  dstrkangle=Dsdaudaudaup3Vec.Unit()*Tagp3Vec.Unit();
		dsntrks++;
	      }
	    }
	  }
	}
      }
    }
    if(!pass){continue;}
    

    //compute the X properties
    fragndaus=0;
    fragnpis=0;
    fragnpi0s=0;
    fragngams=0;
    fragcharge=0.;
    fragpi0lowegam=100.;//find smallest gamma energy
    fragpi0angle=100.;//find the one with smallest absolute value
    fragpiangle=100.;//find the one with smallest absolute value
    fragpi0avgegam=0.;//calculate averages
    fragpi0avgangle=0.;
    fragpiavgangle=0.;
    fraggamangle=100.;
    fragpionavgpstar=0.;
    TVector3 FragGenp3VCM;
    for(Int_t mcc=3;mcc<mcLen;mcc++)
      if(IsFragDaughter(mcc)){
	if(abs(mcLund[mcc])==myPiLund){
		   
	  fragndaus++;      
	  fragnpis++;
	  fragcomppi++;
	  fragcharge+=mcLund[mcc]/abs(mcLund[mcc]);
	  TVector3 FragPiGenp3VCM(mcp3CM[mcc]*sin(acos(mccosthCM[mcc]))*cos(mcphiCM[mcc]),
				  mcp3CM[mcc]*sin(acos(mccosthCM[mcc]))*sin(mcphiCM[mcc]),
				  mcp3CM[mcc]*mccosthCM[mcc]);
	  FragGenp3VCM+=FragPiGenp3VCM;

	  //find the pi farthest away from the ccbar axis
	  if(fabs(FragPiGenp3VCM.Unit()*Tagp3Vec.Unit())<fabs(fragpiangle))
	    fragpiangle=FragPiGenp3VCM.Unit()*Tagp3Vec.Unit();

	  ///average of pion angles
	  fragpiavgangle+=fabs(FragPiGenp3VCM.Unit()*Tagp3Vec.Unit());

	  //average the pion momentum
	  fragpionavgpstar+=mcp3CM[mcc];
	}

	if(mcLund[mcc]==myPi0Lund){

	  fragndaus++;      
	  fragnpi0s++;
	  fragcomppi0++;

	  TVector3 FragPi0Genp3VCM(mcp3CM[mcc]*sin(acos(mccosthCM[mcc]))*cos(mcphiCM[mcc]),
				   mcp3CM[mcc]*sin(acos(mccosthCM[mcc]))*sin(mcphiCM[mcc]),
				   mcp3CM[mcc]*mccosthCM[mcc]);
	  FragGenp3VCM+=FragPi0Genp3VCM;
	  //find the pi0 farthest away from the ccbar axis
	  if(fabs(FragPi0Genp3VCM.Unit()*Tagp3Vec.Unit())<fabs(fragpi0angle))
	    fragpi0angle=FragPi0Genp3VCM.Unit()*Tagp3Vec.Unit();

	  //average of pi0 angles
	  fragpi0avgangle+=fabs(FragPi0Genp3VCM.Unit()*Tagp3Vec.Unit());

	  //find the energy of the lowest gamma
	  TLorentzVector GenGam1p4(mcp3[dauIdx[mcc]]*sin(acos(mccosth[dauIdx[mcc]]))*cos(mcphi[dauIdx[mcc]]),
				   mcp3[dauIdx[mcc]]*sin(acos(mccosth[dauIdx[mcc]]))*sin(mcphi[dauIdx[mcc]]),
				   mcp3[dauIdx[mcc]]*mccosth[dauIdx[mcc]],
				   mcenergy[dauIdx[mcc]]);
	  if(GenGam1p4.T()<fragpi0lowegam)fragpi0lowegam=GenGam1p4.T();
	  fragpi0avgegam+=GenGam1p4.T();
	  TLorentzVector GenGam2p4(mcp3[dauIdx[mcc]+1]*sin(acos(mccosth[dauIdx[mcc]+1]))*cos(mcphi[dauIdx[mcc]+1]),
				   mcp3[dauIdx[mcc]+1]*sin(acos(mccosth[dauIdx[mcc]+1]))*sin(mcphi[dauIdx[mcc]+1]),
				   mcp3[dauIdx[mcc]+1]*mccosth[dauIdx[mcc]+1],
				   mcenergy[dauIdx[mcc]+1]);
	  if(GenGam2p4.T()<fragpi0lowegam)fragpi0lowegam=GenGam2p4.T();
	  fragpi0avgegam+=GenGam2p4.T();

	  //average the pion momentum
	  fragpionavgpstar+=mcp3CM[mcc];
	  	  
	}

	if(mcLund[mcc]==myGammaLund /*&&mcp3[mcc]>.15*/ ){
	  fragndaus++;      
	  fragngams++;
	  fragcompgam++;
	
	  TVector3 FragGamGenp3VCM(mcp3CM[mcc]*sin(acos(mccosthCM[mcc]))*cos(mcphiCM[mcc]),
				   mcp3CM[mcc]*sin(acos(mccosthCM[mcc]))*sin(mcphiCM[mcc]),
				   mcp3CM[mcc]*mccosthCM[mcc]);
	  FragGenp3VCM+=FragGamGenp3VCM;
	  fraggamangle=FragGamGenp3VCM.Unit()*Tagp3Vec.Unit();

	}

      }


    fragpi0avgegam=fragnpi0s>0 ? fragpi0avgegam/(2*fragnpi0s) : 0.;
    fragpi0avgangle=fragnpi0s>0 ? fragpi0avgangle/fragnpi0s : 0.;
    fragpiavgangle=fragnpis>0 ? fragpiavgangle/fragnpis : 0.;

    fragpionavgpstar=(fragnpis+fragnpi0s)>0 ? fragpionavgpstar/ (fragnpis+fragnpi0s) : 0.;

    fragangle=FragGenp3VCM.Unit()*Tagp3Vec.Unit();
    fragpstar=FragGenp3VCM.Mag();
    



    //save
    MCBCounterTotal++;
    Ntuple.Fill();
    
  }
  
  //print summary
  cout<<"--------Summary-------"<<endl;
  cout<<"Total events = "<<eventid<<endl;
  cout<<"Total mc ="<<MCBCounterTotal<<endl;
  cout<<"Number of gam= "<<fragcompgam<<endl;
  cout<<"Number of pi = "<<fragcomppi<<endl;
  cout<<"Number of pi0= "<<fragcomppi0<<endl;
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

Bool_t DRecoDsInclusiveAnalysis::IsFragDaughter(Int_t mcid){

  Bool_t isfragdau=1;
  
  //must be  a pion
  if(abs(mcLund[mcid])!=myPi0Lund && abs(mcLund[mcid])!=myPiLund) isfragdau = 0;

  //for everything make sure it does not have a Charm tag ancestor
  Int_t mothidx=mothIdx[mcid];
  while(mothidx>=0 and mothidx<MAXMCLEN){
    if(abs(mcLund[mothidx])==myD0Lund
       ||abs(mcLund[mothidx])==myDPlusLund  
       ||abs(mcLund[mothidx])==myDsLund
       ||abs(mcLund[mothidx])==myLambdaCLund 
       )
      isfragdau=0;
    mothidx=mothIdx[mothidx];
  }
      
  return isfragdau;
}


Int_t DRecoDsInclusiveAnalysis::DetermineFragDauSource(Int_t mcid){
  if(mcid<0||mcid>=MAXMCLEN)return 0;
  Int_t fragdausource=1;//set source to X
  
  Int_t mothidx=mothIdx[mcid];
  while(mothidx>=0 and mothidx<MAXMCLEN){
    if(abs(mcLund[mothidx])==myD0Lund
       ||abs(mcLund[mothidx])==myDPlusLund
       ||abs(mcLund[mothidx])==myLambdaCLund)
      fragdausource=2;//source is D
    
    if(mothidx==KGenMCIdx)
      fragdausource=3;//source is K
    
    if(abs(mcLund[mothidx])==myDsLund)
      fragdausource=4;//source is Ds 
      
    mothidx=mothIdx[mothidx];
  }
      
  return fragdausource;

}


///////////Belle Plots

//   TString BelleData="/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/BelleDsRecoData.dat";
//   TString BelleBkg="/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/BelleDsRecoBkg.dat";

//   ifstream InputStream;
//   ifstream InputStreamBkg;
//   InputStream.open((const char*)(BelleData));
//   InputStreamBkg.open((const char*)(BelleBkg));
//   if(!InputStream.is_open()){cout<<"bad fit file name"<<endl; return;}
//   if(!InputStreamBkg.is_open()){cout<<"bad fit file name"<<endl; return;}
//   Int_t ithpoint=0;
//   Float_t _x,_y,_yb;
//   TH1F HBelleDsDataPdf("HBelleDsDataPdf","",50,1.82,2.12);
//   TH1F HBelleDsSigPdf("HBelleDsSigPdf","",50,1.82,2.12);
//   TH1F HBelleDsBkgPdf("HBelleDsBkgPdf","",50,1.82,2.12);
//   while(!InputStream.eof()&&ithpoint<50){
//     ithpoint++;
//     InputStream>>_x>>_y;
//     InputStreamBkg>>_x>>_yb;
//     HBelleDsDataPdf.SetBinContent(ithpoint,_y);   
//     HBelleDsDataPdf.SetBinError(ithpoint,sqrt(_y));//sqrt(sum errors squared)
//     HBelleDsSigPdf.SetBinContent(ithpoint,_y-_yb);   
//     HBelleDsSigPdf.SetBinError(ithpoint,sqrt(_y+_yb));//sqrt(sum errors squared)
//     HBelleDsBkgPdf.SetBinContent(ithpoint,_yb);    
//     HBelleDsBkgPdf.SetBinError(ithpoint,sqrt(_yb));    
//   }
//   InputStream.close();
//   InputStreamBkg.close();
  
//   Canvas->Clear();
//   HBelleDsDataPdf.GetYaxis()->SetRangeUser(0,HBelleDsDataPdf.GetMaximum()*1.2);
//   HBelleDsDataPdf.Draw("pe");
//   Canvas->Print(filename);

//   if(_DataType==0){
//     Canvas->Clear();
//     HBelleDsSigPdf.GetYaxis()->SetRangeUser(0,HBelleDsSigPdf.GetMaximum()*1.2);
//     HBelleDsSigPdf.Draw("pe");
//     Canvas->Print(filename);

//     Canvas->Clear();
//     HBelleDsBkgPdf.GetYaxis()->SetRangeUser(0,HBelleDsBkgPdf.GetMaximum()*1.2);
//     HBelleDsBkgPdf.Draw("pe");
//     Canvas->Print(filename);
//   }

//   //total pdf
//   RooDataHist BelleDsDataDat("BelleDsDataDat","BelleDsDataDat",RooArgList(mass),&HBelleDsDataPdf);  
//   RooHistPdf  BelleDsDataPdf("BelleDsDataPdf","BelleDsDataPdf",RooArgSet(mass),BelleDsDataDat);

//   //signal pdf
//   RooDataHist BelleDsSigDat("BelleDsSigDat","BelleDsSigDat",RooArgList(mass),&HBelleDsSigPdf);  
//   RooHistPdf  BelleDsSigPdf("BelleDsSigPdf","BelleDsSigPdf",RooArgSet(mass),BelleDsSigDat);

//   //background pdf
//   RooDataHist BelleDsBkgDat("BelleDsBkgDat","BelleDsBkgDat",RooArgList(mass),&HBelleDsBkgPdf);  
//   RooHistPdf  BelleDsBkgPdf("BelleDsBkgPdf","BelleDsBkgPdf",RooArgSet(mass),BelleDsBkgDat);

//   //our data
//   RooDataHist DsData("DsData","DsData",RooArgList(mass),&HMass);  
//   RooPlot* Plot;

//   if(_DataType==1||_DataType==2){
//     //Compare with the total pdf
//     Plot=mass.frame();
//     DsData.plotOn(Plot);
//     BelleDsDataPdf.plotOn(Plot);
//     Canvas->Clear();
//     Plot->Draw();
//     Canvas->Print(filename);
//     delete Plot;
//   }

//   if(_DataType==0){
//     //Compare with the signal pdf
//     Plot=mass.frame();
//     DsData.plotOn(Plot);
//     BelleDsSigPdf.plotOn(Plot);
//     Canvas->Clear();
//     Plot->Draw();
//     Canvas->Print(filename);
//     delete Plot;

//     //compare with the bkg pdf
//     Plot=mass.frame();
//     DsData.plotOn(Plot);
//     //BelleDsBkgPdf.plotOn(Plot);
//     Canvas->Clear();
//     Plot->Draw();
//     Canvas->Print(filename);
//     delete Plot;
//   }
 
//   ///Fit with exponential and Breit-wigner
//   RooRealVar Treshold("Treshold","Treshold",2.08,2.1); 
//   RooRealVar gamMass("gamMass","gamMass",0.);
//   DReco2BodyMassPhaseSpace TwoBPS("TwoBPS","TwoBPS",mass,gamMass,Treshold);  
//   TwoBPS.setInverse(1);

//   RooRealVar DsMass("DsMass","DsMass",1.97);//DsMass.setVal(1.9682);//DsMass.setConstant(1);
//   RooRealVar Width("Width","Width",.005,.09);//Width.setVal(.02);
//   //RooBreitWigner BW("BW","BW",mass,DsMass,Width);
//   RooGenericPdf BW("BW","BW","1./((mass-DsMass)**2+(Width)**2)",RooArgList(mass,DsMass,Width));
 
//   RooProdPdf sigPdf("sigPdf","sigPdf",TwoBPS,BW);
  

//   RooRealVar coef1("coef1","coef1",.001,10.);
//   RooRealVar coef2("coef2","coef2",.001,10.);
//   //  RooGenericPdf ExpPdf("ExpPdf","ExpPdf","exp(-(Treshold-mass)/coef1-((Treshold-mass)/coef2)**2)",RooArgList(mass,coef1,coef2,Treshold));
//   RooGenericPdf ExpPdf("ExpPdf","ExpPdf","exp(-(Treshold-mass)/coef1)",RooArgList(mass,coef1,Treshold));
//   RooProdPdf bkgPdf("bkgPdf","bkgPdf",ExpPdf,TwoBPS);  
//   RooRealVar Sigyield("Sigyield","Sigyield",.0,1.);
//   RooAddPdf TotalPdf("TotalPdf","TotalPdf",sigPdf,bkgPdf,Sigyield);


//   RooChi2Var Bellechi("Bellechi","Bellechi",TotalPdf,BelleDsSigDat,RooFit::DataError(RooAbsData::SumW2),RooFit::NumCPU(1));
//   RooMinuit Bellemin(Bellechi);
//   Bellemin.migrad();
//   Plot=mass.frame();
//   BelleDsSigDat.plotOn(Plot,RooFit::DataError(RooAbsData::SumW2));
//   //BelleDsSigDat.statOn(Plot);
//   TotalPdf.plotOn(Plot);
//   TotalPdf.plotOn(Plot,Components(bkgPdf),LineColor(3));
//   BW.paramOn(Plot,&BelleDsSigDat,"",1,"NE",.6,1.,1.);
//   Canvas->Clear();
//   Plot->SetTitle("");
//   Plot->Draw();
//   Canvas->Print(filename);
//   delete Plot;

//   RooChi2Var chi("chi","chi",TotalPdf,DsData);
//   RooMinuit min(chi);
//   min.migrad();
//   Plot=mass.frame();
//   DsData.plotOn(Plot);
//   //DsData.statOn(Plot);
//   TotalPdf.plotOn(Plot);
//   TotalPdf.plotOn(Plot,Components(bkgPdf),LineColor(3));
//   BW.paramOn(Plot,&DsData,"",1,"NE",.6,1.,1.);
//   Plot->SetTitle(TString("Yield=")+(long)(Sigyield.getVal()*HMass.Integral()));

//   //Make Residuals
//   TH1* HFit=TotalPdf.createHistogram("HFit",mass);
//   HFit->Scale(HMass.Integral());
//   DRecoResiduals residuals("resid",&HMass,HFit);

//   Canvas->Clear();
//   residuals.MakeFitPlotWithResiduals(Canvas,Plot);
//   Canvas->Print(filename);  
//   delete Plot;

  ///----------------------
  ///Fit the full mass range
  //------------------------

//   RooDataHist DsData2("DsData2","DsData2",RooArgList(mass),&HMass2);  

//   RooChi2Var chi2("chi2","chi2",TotalPdf,DsData2);
//   RooMinuit min2(chi2);
//   min2.simplex();
//   min2.migrad();
//   Plot=mass.frame();
//   DsData2.plotOn(Plot);
//   TotalPdf.plotOn(Plot);
//   TotalPdf.plotOn(Plot,Components(bkgPdf),LineColor(3));
//   Plot->SetTitle(TString("Yield=")+(long)(Sigyield.getVal()*HMass2.Integral()));

//   //Make Residuals
//   TH1* HFit2=TotalPdf.createHistogram("HFit2",mass);
//   HFit2->Scale(HMass2.Integral());
//   DRecoResiduals residuals2("resid2",&HMass2,HFit2);
//   Canvas->Clear();
//   residuals2.MakeFitPlotWithResiduals(Canvas,Plot);
//   Canvas->Print(filename);  
//   delete Plot;

    //Plot Mass as a function of n_X^T
//     TH1* HGen[9];
//     RooPlot* PlotTmp[9];
//     Canvas->Clear();
//     Canvas->Divide(3,3);
//     for(Int_t i=0;i<9;i++){
//       Canvas->cd(i+1);
//       HGen[i]=new TH1F(TString("HGen")+(long)i,"",50,1.82,2.12);
//       ReducedNtuple->Draw(TString("dsmass>>HGen")+(long)i,TString("fragnpartgen==")+(long)i,"pe");

//       //fit to get resolution
//       RooDataHist DataTmp("DataTmp","DataTmp",RooArgList(mass),HGen[i]);  
//       RooChi2Var chi2Tmp("chi2Tmp","chi2Tmp",TotalPdf,DataTmp);
//       RooMinuit minTmp(chi2Tmp);
//       minTmp.migrad();
//       PlotTmp[i]=mass.frame();
//       DataTmp.plotOn(PlotTmp[i]);
//       TotalPdf.plotOn(PlotTmp[i]);
//       TotalPdf.plotOn(PlotTmp[i],Components(bkgPdf),LineColor(3));
//       BW.paramOn(PlotTmp[i],&DsData,"",1,"NE",.65,1.,1.);
//       PlotTmp[i]->SetTitle("");
//       PlotTmp[i]->Draw();
//     }
//     Canvas->Print(filename);
//     for(Int_t i=0;i<9;i++){delete HGen[i];  delete PlotTmp[i];}

//     Canvas->Clear();
//     Canvas->Divide(2,2);	
//     TH1F HNGen("HNGen","",10,0.,10.);HNGen.GetXaxis()->SetTitle("n^{T}_{X} (#pi^{+}+#pi^{0}+#gamma)");
//     TH1F HNGenPi("HNGenPi","",10,0.,10.);HNGenPi.GetXaxis()->SetTitle("n^{T}_{X} (#pi^{+})");
//     TH1F HNGenPi0("HNGenPi0","",10,0.,10.);HNGenPi0.GetXaxis()->SetTitle("n^{T}_{X} (#pi^{0})");
//     TH1F HNGenGam("HNGenGam","",10,0.,10.);HNGenGam.GetXaxis()->SetTitle("n^{T}_{X} (#gamma)");
//     Canvas->cd(1);
//     ReducedNtuple->Draw("fragnpartgen>>HNGen");  
//     Canvas->cd(2);
//     ReducedNtuple->Draw("fragnpigen>>HNGenPi");
//     Canvas->cd(3);
//     ReducedNtuple->Draw("fragnpi0gen>>HNGenPi0");
//     Canvas->cd(4);
//     ReducedNtuple->Draw("fragngamgen>>HNGenGam");
//     Canvas->Print(filename);

void Set2DHistXYTitles(TH2F* H2){
  H2->GetYaxis()->SetTitle("m_{recoil}(DKX#gamma)  (GeV/c^{2})");
  H2->GetYaxis()->SetTitleOffset(1.4);
  H2->GetXaxis()->SetTitle("n_{X}^{R}");
  H2->GetXaxis()->SetTitleOffset(1.4);
  H2->SetTitle("");
}

void Set1DHistXYTitles(TH1* H){
  H->GetXaxis()->SetTitle("m_{recoil}(DKX#gamma)  (GeV/c^{2})");
  H->GetYaxis()->SetTitle("Entries/6 MeV");
  H->SetTitle("");
}

void DRecoDsInclusiveAnalysis::DefaultPlots(){

  filename=_OutputDir+"/DefaultPlots.ps";
  Canvas->Print(filename+"[");

  OpenReducedNtuple();
  gROOT->cd();

  TH1F HMass("HMass","",NDsMassBins,DsMassMin,DsMassMax);
  Set1DHistXYTitles(&HMass);
  ReducedNtuple->Draw("dsmass>>HMass");
  Canvas->Clear();
  HMass.Draw("pe");
  Canvas->Print(filename);


  ///Plot the Ds mass  vs the n_X^R
  ///-------------------------
  TH2F HMassVsNX("HMassVsNX","",NXR,NXRMin,NXRMax,NDsMassBins,DsMassMin,DsMassMax);
  Set2DHistXYTitles(&HMassVsNX);
  ReducedNtuple->Draw("dsmass:fragndaus>>HMassVsNX");
  Canvas->Clear();
  HMassVsNX.Draw("lego");
  Canvas->Print(filename);
  Canvas->Clear();
  Canvas->Divide(2,2);
  TH1* H[NXR];
  TLatex labeltext;
  labeltext.SetTextSize(.1);
  for(Int_t i=0;i<NXR;i++){
    Canvas->cd(i+1);
    H[i]=HMassVsNX.ProjectionY(TString("_")+(long)i,i+1,i+1);
    Set1DHistXYTitles(H[i]);
    H[i]->Draw("pe");
    labeltext.DrawLatex(1.85,H[i]->GetMaximum()*.85,TString("n_{X}^{R}=")+(long)(i));
  }
  Canvas->Print(filename);
  for(Int_t i=0;i<NXR;i++)delete H[i];

  Canvas->Clear();
  Canvas->Divide(2,2);		      
  TH1F HNReco("HNReco","",NXR+1,-.5,NXR+1.-.5);HNReco.GetXaxis()->SetTitle("n^{R}_{X} (#pi^{+}+#pi^{0})");
  TH1F HNRecoPi("HNRecoPi","",NXR+1,-.5,NXR+1.-.5);HNRecoPi.GetXaxis()->SetTitle("n^{R}_{X} (#pi^{+})");
  TH1F HNRecoPi0("HNRecoPi0","",NXR+1,-.5,NXR+1.-.5);HNRecoPi0.GetXaxis()->SetTitle("n^{R}_{X} (#pi^{0})");
  Canvas->cd(1);
  ReducedNtuple->Draw("fragndaus>>HNReco");  
  Canvas->cd(2);
  ReducedNtuple->Draw("fragnpis>>HNRecoPi");
  Canvas->cd(3);
  ReducedNtuple->Draw("fragnpi0s>>HNRecoPi0");
  Canvas->cd(4);

  Canvas->Print(filename);
 

  //plot the unconstrained masses
  TH1F HDsStUncMass("HDsStUncMass","",100,1.6,2.6);
  ReducedNtuple->Draw("dsstuncmass>>HDsStUncMass");  
  Canvas->Clear();
  HDsStUncMass.GetYaxis()->SetRangeUser(0,HDsStUncMass.GetMaximum()*1.1);
  HDsStUncMass.GetXaxis()->SetTitle("m_{recoil}(DKX)  (GeV/c^{2})");
  HDsStUncMass.GetYaxis()->SetTitle("Entries/ 10 MeV");
  HDsStUncMass.Draw();
  Canvas->Print(filename);
  TH1F HDsUncMass("HDsUncMass","",100,1.4,2.4);
  ReducedNtuple->Draw("dsuncmass>>HDsUncMass");  
  Canvas->Clear();		      
  HDsUncMass.GetYaxis()->SetRangeUser(0,HDsUncMass.GetMaximum()*1.1);
  HDsUncMass.GetXaxis()->SetTitle("m_{recoil}(DKX#gamma)  (GeV/c^{2})");
  HDsUncMass.GetYaxis()->SetTitle("Entries/ 10 MeV");
  HDsUncMass.Draw();
  Canvas->Print(filename);

  //plot the event multiplicity
  TH1F HEvtId("HEvtId","",10000,0,10000);
  ReducedNtuple->Draw("goodeventid>>HEvtId");
//   Canvas->Clear();
//   HEvtId.Draw();
//   Canvas->Print(filename);  
  TH1F HEvtMultiplicity("HEvtMultiplicity","",10,.5,10.5);
  for(Int_t bin=1;bin<=10000;bin++)
    if(0.<HEvtId.GetBinContent(bin)&&HEvtId.GetBinContent(bin)<=10.)
      HEvtMultiplicity.AddBinContent((int)HEvtId.GetBinContent(bin),1);
  Canvas->Clear();
  HEvtMultiplicity.GetYaxis()->SetRangeUser(0,HEvtMultiplicity.GetMaximum()*1.1);
  HEvtMultiplicity.GetXaxis()->SetTitle("# of reconstructions/event");
  HEvtMultiplicity.Draw();
  Canvas->Print(filename);

  //plot the tag modes
  TH1F HTagMode("HTagMode","",40,-.5,40-.5);
  HTagMode.SetStats(0);
  ReducedNtuple->Draw("tagmode+(abs(taglund)-1)*10>>HTagMode");  
  Canvas->Clear();	
  HTagMode.GetXaxis()->SetTitle("D^{0}                 D^{+}                 D_{s}^{+}               #Lambda_{c}^{+}            "); 
  HTagMode.Draw();
  if(_DataType==0||_DataType==1){
    //gStyle->SetMarkerSize(2);		      
    ReducedNtuple->Draw("tagmode+(abs(taglund)-1)*10","tagmctrue>0","same");
    //ReducedNtuple->Draw("tagmode+(abs(taglund)-1)*10","tagmctruebad>0","pesame");
  }
  Canvas->Print(filename);

  TH1F* HMass_tag[4];
  Canvas->Clear();
  Canvas->Divide(2,2);
  for(Int_t i=1;i<=4;i++){
    Canvas->cd(i);
    HMass_tag[i-1]=new TH1F(TString("HMass_tag")+(long)i,"",NDsMassBins,DsMassMin,DsMassMax);
    Set1DHistXYTitles(HMass_tag[i-1]);
    ReducedNtuple->Draw(TString("dsmass>>HMass_tag")+(long)i,TString("abs(taglund)==")+(long)+i);    
    if(i==1)labeltext.DrawLatex(1.85,HMass_tag[i-1]->GetMaximum()*.85,"D^{0}");
    if(i==2)labeltext.DrawLatex(1.85,HMass_tag[i-1]->GetMaximum()*.85,"D^{+}");
    if(i==3)labeltext.DrawLatex(1.85,HMass_tag[i-1]->GetYaxis()->GetXmax()*.85,"D_{s}^{+}");
    if(i==4)labeltext.DrawLatex(1.85,HMass_tag[i-1]->GetMaximum()*.85,"#Lambda_{c}^{+}");
  }
  Canvas->Print(filename);
  for(Int_t i=1;i<=4;i++)delete HMass_tag[i-1];

  TH1F HKMode("HKMode","",2,0,2);
  HKMode.SetStats(0);
  HKMode.GetXaxis()->SetNdivisions(0);
  HKMode.GetXaxis()->SetTitle("K_{S}                               K^{+}                  ");
  ReducedNtuple->Draw("abs(Kcharge)>>HKMode");  
  Canvas->Clear();
  HKMode.GetYaxis()->SetRangeUser(0,HKMode.GetMaximum()*1.1);		      
  HKMode.Draw();
  if(_DataType==0||_DataType==1){
    ReducedNtuple->Draw("abs(Kcharge)","Kmctrue>0","same");
    //ReducedNtuple->Draw("abs(Kcharge)","Kmctrue>0&&Ksource==1","psame");
  }
  Canvas->Print(filename);

  //Plot the X charge distribution
  TH1F HXCharge("HXCharge","",7,-3.5,3.5);
  HXCharge.SetStats(0);
  HXCharge.GetXaxis()->SetTitle("X Charge");
  ReducedNtuple->Draw("fragcharge>>HXCharge");  
  Canvas->Clear();
  HXCharge.GetYaxis()->SetRangeUser(0,HXCharge.GetMaximum()*1.1);		      
  HXCharge.Draw();
  Canvas->Print(filename);

  TH1F HXMode("HXMode","",NXR+1,-.5,NXR+1.-.5);
  HXMode.SetStats(0);
  HXMode.SetLineColor(4);
  HXMode.GetXaxis()->SetTitle("# of charged pions in X");
  ReducedNtuple->Draw("fragnpis>>HXMode","fragnpi0s==0");  
  HXMode.GetYaxis()->SetRangeUser(0,HXMode.GetMaximum()*1.1);		      
  Canvas->Clear();
  HXMode.Draw();
  ReducedNtuple->Draw("fragnpis","fragnpi0s==1","same");  
  Canvas->Print(filename);

//   //Plot the X modes
//   TH1F HXMode("HXMode","",50,-.5,49.5);
//   HXMode.SetStats(0);
//   HXMode.GetXaxis()->SetTitle("X decay mode");
//   ReducedNtuple->Draw("8*fragnpis+2*fragnpi0s+fragngams>>HXMode","abs(fragcharge)==0||abs(fragcharge)==1");  
//   Canvas->Clear();
//   HXMode.GetYaxis()->SetRangeUser(0,HXMode.GetMaximum()*1.4);		      
//   HXMode.Draw();
//   TString xconfig;
//   text.SetTextSize(.025); 
//   for(Int_t i=0;i<48;i++){
//     if(i>31||i%8>3)continue;
//     xconfig=TString("")+(long)i+" = ";
//     Float_t x=(float)(i/8)*8.;Float_t y=HXMode.GetMaximum()-1.*HXMode.GetMaximum()*.4/8.;      
//     //n pi+    
//     if(i<8){xconfig+="0#pi^{#pm}";}
//     if(8<=i&&i<16){xconfig+="1#pi^{#pm}";}
//     if(16<=i&&i<24){xconfig+="2#pi^{#pm}";}
//     if(24<=i&&i<32){xconfig+="3#pi^{#pm}";}
// //     if(32<=i&&i<40){xconfig+="4#pi^{#pm}";}
// //     if(40<=i&&i<48){xconfig+="5#pi^{#pm}";}
//     //m #pi0
//     if(0<=i%8&&i%8<2){xconfig+="0#pi^{0}";}
//     if(2<=i%8&&i%8<4){xconfig+="1#pi^{0}";}
// //     if(4<=i%8&&i%8<6){xconfig+="2#pi^{0}";}
// //     if(6<=i%8&&i%8<8){xconfig+="3#pi^{0}";}
//     //r gam
// //     if(0==(i%8)%2){xconfig+="0#gamma";}
// //     if(1==(i%8)%2){xconfig+="1#gamma";}
//     y-=(i%8)*HXMode.GetMaximum()*.4/8.;
//     text.DrawLatex(x,y,xconfig);
//   }
//   Canvas->Print(filename);
//   TH1F HXModeOdd("HXModeOdd","",50,-.5,49.5);
//   HXModeOdd.SetStats(0);
//   HXModeOdd.GetXaxis()->SetTitle("X decay mode");
//   ReducedNtuple->Draw("8*fragnpis+2*fragnpi0s+fragngams>>HXModeOdd","abs(fragcharge)==2||abs(fragcharge)==3");  
//   Canvas->Clear();
//   HXModeOdd.GetYaxis()->SetRangeUser(0,HXModeOdd.GetMaximum()*1.4);		      
//   HXModeOdd.Draw();
//   for(Int_t i=0;i<48;i++){
//     if(i>31||i%8>3)continue;
//     xconfig=TString("")+(long)i+" = ";
//     Float_t x=(float)(i/8)*8.;Float_t y=HXModeOdd.GetMaximum()-1.*HXModeOdd.GetMaximum()*.4/8.;  
//     //n pi+    
//     if(i<8){xconfig+="0#pi^{#pm}";}
//     if(8<=i&&i<16){xconfig+="1#pi^{#pm}";}
//     if(16<=i&&i<24){xconfig+="2#pi^{#pm}";}
//     if(24<=i&&i<32){xconfig+="3#pi^{#pm}";}
// //     if(32<=i&&i<40){xconfig+="4#pi^{#pm}";}
// //     if(40<=i&&i<48){xconfig+="5#pi^{#pm}";}
//     //m #pi0
//     if(0<=i%8&&i%8<2){xconfig+="0#pi^{0}";}
//     if(2<=i%8&&i%8<4){xconfig+="1#pi^{0}";}
// //     if(4<=i%8&&i%8<6){xconfig+="2#pi^{0}";}
// //     if(6<=i%8&&i%8<8){xconfig+="3#pi^{0}";}
//     //r gam
// //    if(0==(i%8)%2){xconfig+="0#gamma";}
// //     if(1==(i%8)%2){xconfig+="1#gamma";}
//     y-=(i%8)*HXModeOdd.GetMaximum()*.4/8.;
//     text.DrawLatex(x,y,xconfig);
//   }
//   Canvas->Print(filename);
  
  


  //Plot the DKX charge 
  TH1F HDKXCharge("HDKXCharge","",5,-2.5,2.5);
  HDKXCharge.GetXaxis()->SetTitle("DKX Charge");
  ReducedNtuple->Draw("tagcharge+Kcharge+fragcharge+pcharge>>HDKXCharge");  
  Canvas->Clear();
  HDKXCharge.GetYaxis()->SetRangeUser(0,HDKXCharge.GetMaximum()*1.1);		      
  HDKXCharge.Draw();
  Canvas->Print(filename);
  

  //// D-K-X charge combination
  TH1F HDKXMode("HDKXMode","",16,-.5,15.5);
  HDKXMode.SetStats(0);
  HDKXMode.GetXaxis()->SetTitle("D,K,X charge combination        ");
  ReducedNtuple->Draw("evtDKXcombination>>HDKXMode");  
  Canvas->Clear();
  HDKXMode.GetYaxis()->SetRangeUser(0,HDKXMode.GetMaximum()*1.4);
  HDKXMode.Draw();
  TString dkxconfig;
  text.SetTextSize(.03);  
  for(Int_t i=0;i<16;i++){
    dkxconfig=TString("")+(long)i+" = ";
    Float_t x=(i/4)*4.;
    Float_t y=HDKXMode.GetMaximum()-.5*HDKXMode.GetMaximum()*.4/4.-(i%4)*HDKXMode.GetMaximum()*.4/6.;
    //append D charge   
    if(i/8==0){dkxconfig+="0,";}
    if(i/8==1){dkxconfig+="1,";}
    //append K charge
    if((i%8)/4==0){dkxconfig+="0,";}
    if((i%8)/4==1){dkxconfig+="1,";}
    //append X charge
    if((i%8)%4==0){dkxconfig+="0";}
    if((i%8)%4==1){dkxconfig+="1";}
    if((i%8)%4==2){dkxconfig+="2";}
    if((i%8)%4==3){dkxconfig+="3";}        
    text.DrawLatex(x,y,dkxconfig);
  }
  Canvas->Print(filename);


  if(_DataType==1){//Generic MC plots
    //Plot the Ds mass for the different bacgrounds
    TH1F HMassEvtSource0("HMassEvtSource0","",NDsMassBins,DsMassMin,DsMassMax);HMassEvtSource0.SetFillColor(2);//Ds* signal
    TH1F HMassEvtSource1("HMassEvtSource1","",NDsMassBins,DsMassMin,DsMassMax);HMassEvtSource1.SetFillColor(4);//total background
    //TH1F HMassEvtSource2("HMassEvtSource2","",NDsMassBins,DsMassMin,DsMassMax);HMassEvtSource2.SetFillColor(4);//
    //TH1F HMassEvtSource3("HMassEvtSource3","",NDsMassBins,DsMassMin,DsMassMax);HMassEvtSource3.SetFillColor(4);//
    ReducedNtuple->Draw("dsmass>>HMassEvtSource0","");
    ReducedNtuple->Draw("dsmass>>HMassEvtSource1","!evthassignal");
    //ReducedNtuple->Draw("dsmass>>HMassEvtSource2","!evthassignal&&!(evthasD0gam&&tagangleToDst0<0.&&gamsource==10)");//show the peaking background
    //ReducedNtuple->Draw("dsmass>>HMassEvtSource3","!evthassignal&&!evthasD0gam&&!evthasDs");
    Canvas->Clear();	
    //HMassEvtSource0.SetTitle(TString("Signal=")+(long)(HMassEvtSource0.Integral()-HMassEvtSource1.Integral())
    //+"  Background="+(long)HMassEvtSource1.Integral());
    Set1DHistXYTitles(&HMassEvtSource0);
    HMassEvtSource0.Draw("");
    HMassEvtSource1.Draw("same");
    //HMassEvtSource2.Draw("same");
    //HMassEvtSource3.Draw("same");
    cutline.SetLineColor(1);
    cutline.DrawLine(DenominatorDsMassMin,0,DenominatorDsMassMin,HMassEvtSource0.GetMaximum());
    cutline.DrawLine(DenominatorDsMassMax,0,DenominatorDsMassMax,HMassEvtSource0.GetMaximum());
    Canvas->Print(filename);

    //decompose signal photon in the background
    TH1F HMassBkgGamSource0("HMassBkgGamSource0","",NDsMassBins,DsMassMin,DsMassMax);HMassBkgGamSource0.SetFillColor(3);
    TH1F HMassBkgGamSource1("HMassBkgGamSource1","",NDsMassBins,DsMassMin,DsMassMax);HMassBkgGamSource1.SetFillColor(7);
    TH1F HMassBkgGamSource2("HMassBkgGamSource2","",NDsMassBins,DsMassMin,DsMassMax);HMassBkgGamSource2.SetFillColor(4);
    TH1F HMassBkgGamSource3("HMassBkgGamSource3","",NDsMassBins,DsMassMin,DsMassMax);HMassBkgGamSource3.SetFillColor(6);
    ReducedNtuple->Draw("dsmass>>HMassBkgGamSource0","evthassignal!=1");//pi0,eta decay=1,2
    ReducedNtuple->Draw("dsmass>>HMassBkgGamSource1","evthassignal!=1&&(gamsource>2)");//track radiated=3
    ReducedNtuple->Draw("dsmass>>HMassBkgGamSource2","evthassignal!=1&&(gamsource>3)");//decay of meson/baryon,unknown photon=4,5
    ReducedNtuple->Draw("dsmass>>HMassBkgGamSource3","evthassignal!=1&&(gamsource>10)");//fake photon>10
    Canvas->Clear();	
    Set1DHistXYTitles(&HMassBkgGamSource0);
    HMassBkgGamSource0.Draw("");
    HMassBkgGamSource1.Draw("same");
    HMassBkgGamSource2.Draw("same");
    HMassBkgGamSource3.Draw("same");
    Canvas->Print(filename);

  }

  if(_DataType==1||_DataType==0){// signal MC Plots
    //Compare number of Reco particles with number of Generated
    TH1F HNRec("HNRec","",NXT+1,-.5,NXT+1.-.5);HNRec.GetXaxis()->SetTitle("n^{T}_{X}");
    ReducedNtuple->Draw("fragndaus>>HNRec","evthassignal");  
    TH1F HNGen("HNGen","",NXT+1,-.5,NXT+1.-.5);HNGen.GetXaxis()->SetTitle("n^{T}_{X}");
    ReducedNtuple->Draw("fragnpartgen>>HNGen","evthassignal");  
    Canvas->Clear();
    HNGen.GetXaxis()->SetTitle("n_{X}");
    HNGen.GetYaxis()->SetRangeUser(0,1.1*HNRec.GetMaximum());
    HNGen.Draw();    
    HNRec.SetMarkerSize(2);
    HNRec.Draw("pe same");
    Canvas->Print(filename);
  
    //Plot the number reco particles for each number of generated particles
    TH1F* HRecoVsGen[NXT];
    TH1F* HGenBin[NXT];
    Canvas->Clear();
    Canvas->Divide(2,4);
    for(Int_t j=0;j<NXT;j++){
      Canvas->cd(j+1);
      HRecoVsGen[j]=new TH1F(TString("HRecoVsGen")+(long)j,"",NXT,NXTMin,NXTMax);
      HGenBin[j]=new TH1F(TString("HGenBin")+(long)j,"",NXT,NXTMin,NXTMax);
      ReducedNtuple->Draw(TString("fragndaus>>HRecoVsGen")+(long)j,TString("evthassignal&&fragnpartgen==")+(long)j);
      HGenBin[j]->SetBinContent(j+1,HRecoVsGen[j]->Integral());
      HGenBin[j]->GetYaxis()->SetLabelSize(.07);
      HGenBin[j]->GetYaxis()->SetNdivisions(3);
      HGenBin[j]->GetXaxis()->SetLabelSize(.08);
      HGenBin[j]->GetXaxis()->SetTitle("n_{X}");
      HGenBin[j]->GetXaxis()->SetTitleSize(.1);
      HGenBin[j]->GetXaxis()->SetTitleOffset(.7);
      HGenBin[j]->Draw();
      HRecoVsGen[j]->Draw("pesame");
      labeltext.DrawLatex(3,HGenBin[j]->GetMaximum()*1.1,TString("n_{X}^{T}=")+(long)(j));
    }
    Canvas->Print(filename);
    for(Int_t i=0;i<NXT;i++){delete HRecoVsGen[i];delete HGenBin[i];}
  
    //plot the Ds diff vectors
    TH1F HnXdiff("HnXdiff","",7,-3.5,3.5);
    ReducedNtuple->Draw("fragndaus-fragnpartgen>>HnXdiff","evthassignal");  
    Canvas->Clear();	
    HnXdiff.GetXaxis()->SetTitle("n_{X}^{R}-n_{X}^{T}");	      
    HnXdiff.Draw();
    Canvas->Print(filename);
    
    //plot the Ds diff vectors
    TH1F HDsPdiff("HDsPdiff","",180,0.,9.);
    ReducedNtuple->Draw("dspdiff>>HDsPdiff","evthassignal");  
    Canvas->Clear();
    HDsPdiff.GetXaxis()->SetTitle("|p_{rec}(D_{s})-p_{gen}(D_{s})|   (GeV)");		      
    HDsPdiff.GetYaxis()->SetTitle("Entries/50 MeV");
    HDsPdiff.Draw();
    Canvas->Print(filename);


    //Plot the Ds mass for the different signal gamma sources
    TH1F HMassSigGamSource0("HMassSigGamSource0","",NDsMassBins,DsMassMin,DsMassMax);HMassSigGamSource0.SetFillColor(2);
    TH1F HMassSigGamSource1("HMassSigGamSource1","",NDsMassBins,DsMassMin,DsMassMax);HMassSigGamSource1.SetFillColor(3);
    TH1F HMassSigGamSource2("HMassSigGamSource2","",NDsMassBins,DsMassMin,DsMassMax);HMassSigGamSource2.SetFillColor(7);
    TH1F HMassSigGamSource3("HMassSigGamSource3","",NDsMassBins,DsMassMin,DsMassMax);HMassSigGamSource3.SetFillColor(4);
    TH1F HMassSigGamSource4("HMassSigGamSource4","",NDsMassBins,DsMassMin,DsMassMax);HMassSigGamSource4.SetFillColor(6);//fake 5
    ReducedNtuple->Draw("dsmass>>HMassSigGamSource0","evthassignal");//Ds* signal 0
    ReducedNtuple->Draw("dsmass>>HMassSigGamSource1","evthassignal&&(gamsource>0)");//pi0,eta decay 1,2
    ReducedNtuple->Draw("dsmass>>HMassSigGamSource2","evthassignal&&(gamsource>2)");//track radiated 3
    ReducedNtuple->Draw("dsmass>>HMassSigGamSource3","evthassignal&&(gamsource>3)");//decay of meson/baryon,unknown photon= 4,5
    ReducedNtuple->Draw("dsmass>>HMassSigGamSource4","evthassignal&&(gamsource>10)");//fake photon>10
    Canvas->Clear();
    HMassSigGamSource0.SetTitle(TString("Signal=")+(long)(HMassSigGamSource0.Integral()-HMassSigGamSource1.Integral())
				+"  Background="+(long)HMassSigGamSource1.Integral());
    HMassSigGamSource0.GetXaxis()->SetTitle("m(Ds)   (GeV/c^{2})");
    Set1DHistXYTitles(&HMassSigGamSource0);
    HMassSigGamSource0.Draw("");
    HMassSigGamSource1.Draw("same");
    HMassSigGamSource2.Draw("same");
    HMassSigGamSource3.Draw("same");
    HMassSigGamSource4.Draw("same");
    cutline.SetLineColor(1);
    cutline.DrawLine(DenominatorDsMassMin,0,DenominatorDsMassMin,HMassSigGamSource0.GetMaximum());//plot->GetYaxis()->GetXmax());
    cutline.DrawLine(DenominatorDsMassMax,0,DenominatorDsMassMax,HMassSigGamSource0.GetMaximum());//plot->GetYaxis()->GetXmax());
    labeltext.SetTextSize(.04);
    labeltext.SetTextColor(2);labeltext.DrawLatex(1.83,HMassSigGamSource0.GetMaximum()*.9,"D_{s}^{_{*}}#rightarrow D_{s}#gamma");
    labeltext.SetTextColor(3);labeltext.DrawLatex(1.83,HMassSigGamSource0.GetMaximum()*.83,"#pi^{0}/#eta");
    labeltext.SetTextColor(7);labeltext.DrawLatex(1.83,HMassSigGamSource0.GetMaximum()*.74,"Track FSR");
    labeltext.SetTextColor(4);labeltext.DrawLatex(1.83,HMassSigGamSource0.GetMaximum()*.67,"other hadron decay");
    labeltext.SetTextColor(6);labeltext.DrawLatex(1.83,HMassSigGamSource0.GetMaximum()*.6,"Track/K_{L} interactions");
    Canvas->Print(filename);

    //fake photons vs energy
    TH1F HPhotonEnergySignal("HPhotonEnergySignal","",100,0,1.);HPhotonEnergySignal.SetFillColor(2);
    TH1F HPhotonEnergyTrue("HPhotonEnergyTrue","",100,0,1.);HPhotonEnergyTrue.SetFillColor(3);
    TH1F HPhotonEnergyFake("HPhotonEnergyFake","",100,0,1.);HPhotonEnergyFake.SetFillColor(6);
    ReducedNtuple->Draw("gamenergy>>HPhotonEnergySignal","evthassignal");//signal
    ReducedNtuple->Draw("gamenergy>>HPhotonEnergyTrue","evthassignal&&gamsource>0");//true photons
    ReducedNtuple->Draw("gamenergy>>HPhotonEnergyFake","evthassignal&&gamsource>10");//fake photons
    Canvas->Clear();
    HPhotonEnergySignal.SetStats(0);	
    HPhotonEnergySignal.GetXaxis()->SetTitle("E_{Lab}");
    HPhotonEnergySignal.Draw("");
    HPhotonEnergyTrue.Draw("same");
    HPhotonEnergyFake.Draw("same");
    Canvas->Print(filename);

    //fake photons vs angle 
    TH1F HPhotonAngleSignal("HPhotonAngleSignal","",200,-1.,1.);HPhotonAngleSignal.SetFillColor(2);
    TH1F HPhotonAngleTrue("HPhotonAngleTrue","",200,-1.,1.);HPhotonAngleTrue.SetFillColor(3);
    TH1F HPhotonAngleFake("HPhotonAngleFake","",200,-1.,1.);HPhotonAngleFake.SetFillColor(6);
    ReducedNtuple->Draw("gamanglelab>>HPhotonAngleSignal","evthassignal");//signal
    ReducedNtuple->Draw("gamanglelab>>HPhotonAngleTrue","evthassignal&&gamsource>0");//true photons
    ReducedNtuple->Draw("gamanglelab>>HPhotonAngleFake","evthassignal&&gamsource>10");//fake photons
    Canvas->Clear();
    HPhotonAngleSignal.SetStats(0);	
    HPhotonAngleSignal.GetXaxis()->SetTitle("cos(#theta_{Lab})");
    HPhotonAngleSignal.Draw("");
    HPhotonAngleTrue.Draw("same");
    HPhotonAngleFake.Draw("same");
    cutline.SetLineWidth(3);cutline.SetLineColor(4);//cutline.SetLineStyle(2);
    cutline.DrawLine(.94,0,.94,HPhotonAngleSignal.GetMaximum());
    Canvas->Print(filename);
   
    //fake photons vs lat 
    TH1F HPhotonLatSignal("HPhotonLatSignal","",200,0,1.);HPhotonLatSignal.SetFillColor(2);
    TH1F HPhotonLatTrue("HPhotonLatTrue","",200,0,1.);HPhotonLatTrue.SetFillColor(3);
    TH1F HPhotonLatFake("HPhotonLatFake","",200,0,1.);HPhotonLatFake.SetFillColor(6);
    ReducedNtuple->Draw("gamlat>>HPhotonLatSignal","evthassignal");//signal
    ReducedNtuple->Draw("gamlat>>HPhotonLatTrue","evthassignal&&gamsource>0");//true photons
    ReducedNtuple->Draw("gamlat>>HPhotonLatFake","evthassignal&&gamsource>10");//fake photons
    Canvas->Clear();
    HPhotonLatSignal.SetStats(0);
    HPhotonLatSignal.GetXaxis()->SetTitle("Cluster Lateral Moment");	
    HPhotonLatSignal.Draw("");
    HPhotonLatTrue.Draw("same");
    HPhotonLatFake.Draw("same");
    cutline.DrawLine(.6,0,.6,HPhotonLatSignal.GetMaximum());
    cutline.DrawLine(.01,0,.01,HPhotonLatSignal.GetMaximum());
    Canvas->Print(filename);
   
    //fake photons vs A42
    TH1F HPhotonA42Signal("HPhotonA42Signal","",100,0,.5);HPhotonA42Signal.SetFillColor(2);
    TH1F HPhotonA42True("HPhotonA42True","",100,0,.5);HPhotonA42True.SetFillColor(3);
    TH1F HPhotonA42Fake("HPhotonA42Fake","",100,0,.5);HPhotonA42Fake.SetFillColor(6);
    ReducedNtuple->Draw("gamA42>>HPhotonA42Signal","evthassignal");//signal
    ReducedNtuple->Draw("gamA42>>HPhotonA42True","evthassignal&&gamsource>0");//true photons
    ReducedNtuple->Draw("gamA42>>HPhotonA42Fake","evthassignal&&gamsource>10");//fake photons
    Canvas->Clear();
    HPhotonA42Signal.SetStats(0);
    HPhotonA42Signal.GetXaxis()->SetTitle("A42");	
    HPhotonA42Signal.Draw("");
    HPhotonA42True.Draw("same");
    HPhotonA42Fake.Draw("same");
    Canvas->Print(filename);
   
    //fake photons vs bump dist
    TH1F HPhotonBumpDistSignal("HPhotonBumpDistSignal","",100,0,200);HPhotonBumpDistSignal.SetFillColor(2);
    TH1F HPhotonBumpDistTrue("HPhotonBumpDistTrue","",100,0,200);HPhotonBumpDistTrue.SetFillColor(3);
    TH1F HPhotonBumpDistFake("HPhotonBumpDistFake","",100,0,200);HPhotonBumpDistFake.SetFillColor(6);
    ReducedNtuple->Draw("gambumpdist>>HPhotonBumpDistSignal","evthassignal");//signal
    ReducedNtuple->Draw("gambumpdist>>HPhotonBumpDistTrue","evthassignal&&gamsource>0");//true photons
    ReducedNtuple->Draw("gambumpdist>>HPhotonBumpDistFake","evthassignal&&gamsource>10");//fake photons
    Canvas->Clear();
    HPhotonBumpDistSignal.SetStats(0);	
    HPhotonBumpDistSignal.GetXaxis()->SetTitle("Distance to nearest bump   (cm)");
    HPhotonBumpDistSignal.Draw("");
    HPhotonBumpDistTrue.Draw("same");
    HPhotonBumpDistFake.Draw("same");
    cutline.DrawLine(15,0,15,HPhotonBumpDistSignal.GetMaximum());
    Canvas->Print(filename);
   

    //fake photons vs trk dist 
    TH1F HPhotonTrkDistSignal("HPhotonTrkDistSignal","",100,0,200);HPhotonTrkDistSignal.SetFillColor(2);
    TH1F HPhotonTrkDistTrue("HPhotonTrkDistTrue","",100,0,200);HPhotonTrkDistTrue.SetFillColor(3);
    TH1F HPhotonTrkDistFake("HPhotonTrkDistFake","",100,0,200);HPhotonTrkDistFake.SetFillColor(6);
    ReducedNtuple->Draw("gamtrkdist>>HPhotonTrkDistSignal","evthassignal");//signal
    ReducedNtuple->Draw("gamtrkdist>>HPhotonTrkDistTrue","evthassignal&&gamsource>0");//true photons
    ReducedNtuple->Draw("gamtrkdist>>HPhotonTrkDistFake","evthassignal&&gamsource>10");//fake photons
    Canvas->Clear();
    HPhotonTrkDistSignal.SetStats(0);	
    HPhotonTrkDistSignal.GetXaxis()->SetTitle("Distance to nearest track  (cm)");
    HPhotonTrkDistSignal.Draw("");
    HPhotonTrkDistTrue.Draw("same");
    HPhotonTrkDistFake.Draw("same");
    cutline.DrawLine(15,0,15,HPhotonTrkDistSignal.GetMaximum());
    Canvas->Print(filename);
   
    
    //plot the event multiplicity
    TH1F HEvtIdSig("HEvtIdSig","",10000,0,10000);
    ReducedNtuple->Draw("goodeventid>>HEvtIdSig","evthassignal");
    TH1F HEvtMultiplicitySig("HEvtMultiplicitySig","",10,.5,10.5);
    for(Int_t bin=1;bin<=10000;bin++)
      if(0.<HEvtIdSig.GetBinContent(bin)&&HEvtIdSig.GetBinContent(bin)<=10.)
	HEvtMultiplicitySig.AddBinContent((int)HEvtIdSig.GetBinContent(bin),1);
    Canvas->Clear();
    HEvtMultiplicitySig.GetYaxis()->SetRangeUser(0,HEvtMultiplicitySig.GetMaximum()*1.1);
    HEvtMultiplicitySig.GetXaxis()->SetTitle("# of reconstructions/event");
    HEvtMultiplicitySig.Draw();
    Canvas->Print(filename);

    //plot the correlation between p* and nXT    
    TH1F* HDsPstar[NXT];
    for(Int_t j=0;j<NXT;j++){
      HDsPstar[j]=new TH1F(TString("HDsPstar")+(long)j,"",50,2.5,5.);
      ReducedNtuple->Draw(TString("dspstar>>HDsPstar")+(long)j,TString("evthassignal&&fragnpartgen==")+(long)j);
    }
    HDsPstar[0]->Scale(1./HDsPstar[0]->Integral());
    HDsPstar[0]->SetLineColor(1);
    HDsPstar[0]->GetYaxis()->SetRangeUser(0,HDsPstar[0]->GetMaximum()*1.1);
    HDsPstar[0]->GetXaxis()->SetTitle("p*(D_{s})");
    HDsPstar[0]->GetYaxis()->SetTitle("Entries/50 MeV)");
    Canvas->Clear();
    HDsPstar[0]->Draw();
    for(Int_t j=1;j<NXT;j++){
      HDsPstar[j]->Scale(1./HDsPstar[j]->Integral());
      HDsPstar[j]->SetLineColor(j+1);
      HDsPstar[j]->Draw("same");
    }
    Canvas->Print(filename);
    //for(Int_t j=0;j<NXT;j++)delete HDsPstar[j];
    
    //2D plot of p* vs nXT
    TH2F H2DPstarCorrelation("H2DPstarCorrelation","",NXT,-.5,NXT-.5,50,2.5,5.);
    ReducedNtuple->Draw(TString("dspstar:fragnpartgen>>H2DPstarCorrelation"),TString("evthassignal"));
    H2DPstarCorrelation.GetXaxis()->SetTitle("n_{X}^{T}");
    H2DPstarCorrelation.GetYaxis()->SetTitle("p*(D_{s})");
    Canvas->Clear();
    //Canvas->SetLogz(2);
    H2DPstarCorrelation.SetStats(0);
    H2DPstarCorrelation.Draw("colz");
    Canvas->Print(filename);

    //For Ds-->KKpi eff calculation plot the generated dalitz plot
    if(_OutputDir.Contains("KKpiEff")){
      //dalits plot
      TH2F HDalitsPlot("HDalitsPlot","",130,.9,3.5,100,.3,2.3);
      ReducedNtuple->Draw("kpimassgen:kkmassgen>>HDalitsPlot","evthassignal");
      HDalitsPlot.SetTitle("");
      HDalitsPlot.GetXaxis()->SetTitle("m^{2}(K^{+}K^{-})  (GeV^{2})");
      HDalitsPlot.GetYaxis()->SetTitle("m^{2}(K^{-}#pi^{+})  (GeV^{2})");
      HDalitsPlot.SetStats(0);
      Canvas->Clear(); 
      //Canvas->SetLogz(1);
      HDalitsPlot.Draw("colz");
      DrawDalitzRegions();
      Canvas->Print(filename);  
      //Canvas->SetLogz(0);
    }

  }


  CloseReducedNtuple();
  Canvas->Print(filename+"]");

}


// void DRecoDsInclusiveAnalysis::CreateHistosForFit(){

//   OpenReducedNtuple(); 
//   ReducedNtuple->SetBranchStatus("*",0);
//   ReducedNtuple->SetBranchStatus("dsmass",1);ReducedNtuple->SetBranchAddress("dsmass",&dsmass);
//   ReducedNtuple->SetBranchStatus("goodeventid",1);ReducedNtuple->SetBranchAddress("goodeventid",&goodeventid);
//   ReducedNtuple->SetBranchStatus("evthassignal",1);ReducedNtuple->SetBranchAddress("evthassignal",&evthassignal);
//   ReducedNtuple->SetBranchStatus("fragndaus",1);ReducedNtuple->SetBranchAddress("fragndaus",&fragndaus);
//   ReducedNtuple->SetBranchStatus("fragnpartgen",1);ReducedNtuple->SetBranchAddress("fragnpartgen",&fragnpartgen);
//   ReducedNtuple->SetBranchStatus("gammctrue",1);ReducedNtuple->SetBranchAddress("gammctrue",&gammctrue);
//   ReducedNtuple->SetBranchStatus("kkmassgen",1);ReducedNtuple->SetBranchAddress("kkmassgen",&kkmassgen);
//   ReducedNtuple->SetBranchStatus("kpimassgen",1);ReducedNtuple->SetBranchAddress("kpimassgen",&kpimassgen);
//   ReducedNtuple->SetBranchStatus("gamsource",1);ReducedNtuple->SetBranchAddress("gamsource",&gamsource);  

//   //Total Reco distribution
//   TH1F HDsMass("HDsMass","",NDsMassBins,DsMassMin,DsMassMax);
//   HDsMass.Sumw2();
//   TH1F HDsMassTrue("HDsMassTrue","",NDsMassBins,DsMassMin,DsMassMax);
//   HDsMassTrue.Sumw2();
//   TH1F HDsMassNoSig("HDsMassNoSig","",NDsMassBins,DsMassMin,DsMassMax);
//   HDsMassNoSig.Sumw2();
  
//   //vs nXR
//   TH1F HDsMassvsnXR[NXR];
//   TH1F HDsMassTruevsnXR[NXR];
//   TH1F HDsMassNoSigvsnXR[NXR];
//   for(Int_t i=0;i<NXR;i++){
//     HDsMassvsnXR[i]=TH1F(TString("HDsMass_nXR")+(long)i,"",NDsMassBins,DsMassMin,DsMassMax);
//     HDsMassvsnXR[i].Sumw2();
//     HDsMassTruevsnXR[i]=TH1F(TString("HDsMassTrue_nXR")+(long)i,"",NDsMassBins,DsMassMin,DsMassMax);
//     HDsMassTruevsnXR[i].Sumw2();
//     HDsMassNoSigvsnXR[i]=TH1F(TString("HDsMassNoSig_nXR")+(long)i,"",NDsMassBins,DsMassMin,DsMassMax);
//     HDsMassNoSigvsnXR[i].Sumw2();
//   }

  
//   TH2F HDsMass2D("HDsMass2D","",NXR,-.5,NXR-.5,NDsMassBins,DsMassMin,DsMassMax);
//   HDsMass2D.Sumw2();
  
//   TH2F HDsMass2DNoSig("HDsMass2DNoSig","",NXR,-.5,NXR-.5,NDsMassBins,DsMassMin,DsMassMax);
//   HDsMass2DNoSig.Sumw2();

//   //signal MC 2D mass vs nXT
//   TH2F HDsMassTrue2DvsnXT[NXT]; 
//   for(Int_t j=0;j<NXT;j++){//for each nXT
//     HDsMassTrue2DvsnXT[j]=TH2F(TString("HDsMassTrue2D_nXT")+(long)j,"",NXR,-.5,NXR-.5,NDsMassBins,DsMassMin,DsMassMax);
//     HDsMassTrue2DvsnXT[j].Sumw2();
//   }

//   //signal pdfs with shifted signal component to fit the Data.
//   TH2F HDsMassTrue2DvsnXTShifted[NXT]; 
//   for(Int_t j=0;j<NXT;j++){//for each nXT
//     HDsMassTrue2DvsnXTShifted[j]=TH2F(TString("HDsMassTrue2D_nXTShifted")+(long)j,"",NXR,-.5,NXR-.5,NDsMassBins,DsMassMin,DsMassMax);
//     HDsMassTrue2DvsnXTShifted[j].Sumw2();
//   }

//   //Modified signal pdfs for Photon background systematic: Pi0+Eta+Track+Klong
//   TH2F HDsMassTrue2DvsnXTShiftedPi0Systematic[NXT]; 
//   for(Int_t j=0;j<NXT;j++){//for each nXT
//     HDsMassTrue2DvsnXTShiftedPi0Systematic[j]=TH2F(TString("HDsMassTrue2D_nXTShiftedPi0Systematic")+(long)j,"",NXR,-.5,NXR-.5,NDsMassBins,DsMassMin,DsMassMax);
//     HDsMassTrue2DvsnXTShiftedPi0Systematic[j].Sumw2();
//   }
  
//   TH1F HEvtMultiplicity("HEvtMultiplicity","",10,.5,10.5); 
//   TH1F HEvtMultiplicitySig("HEvtMultiplicitySig","",10,.5,10.5);
//   TH1F HEvtMultiplicityBkg("HEvtMultiplicityBkg","",10,.5,10.5);
//   TH1F HEvtMultiplicitySigMod[21];
//   for(Int_t m=0;m<21;m++)
//     HEvtMultiplicitySigMod[m]=TH1F(TString("HEvtMultiplicitySigMod")+(long)m,"",10,.5,10.5);


//   //Histograms for each Dalitz region of Ds-->KKpi signal MC.
//   TH2F HKKpiDsMassTrue2DvsnXT[5][NXT]; 
//   for(Int_t d=0;d<3;d++){
//     for(Int_t j=0;j<NXT;j++){//for each nXT
//       HKKpiDsMassTrue2DvsnXT[d][j]=TH2F(TString("HKKpiDsMassTrue2D_Dalitz")+(long)d+"_nXT"+(long)j,"",NXR,-.5,NXR-.5,NDsMassBins,DsMassMin,DsMassMax);
//       HKKpiDsMassTrue2DvsnXT[d][j].Sumw2();
//     }
//   }

  
//   Float_t priordsmass[NMAXDSCANDS];
//   Int_t priorfragndaus[NMAXDSCANDS];  
//   Int_t priorfragnpartgen[NMAXDSCANDS];  
//   Int_t priorgammctrue[NMAXDSCANDS];
//   Int_t priorgamsource[NMAXDSCANDS];
//   Int_t nDsCands=0;
//   Int_t prioreventid=1;
//   Int_t priorevthassignal=0;
//   Int_t nDsCandsMod[21];for(Int_t m=0;m<21;m++)nDsCandsMod[m]=0;
//   Int_t ntruephotons=0,nfakephotons=0;
//   Int_t signaleventid=0;
//   Int_t nfakephotonsToAdd[21];
//   for(Int_t m=0;m<21;m++)nfakephotonsToAdd[m]=(int)(291901*.01*abs(m-10));
//   Int_t nfakephotonsAdded[21];
//   for(Int_t m=0;m<21;m++)nfakephotonsAdded[m]=0;
//   Int_t npriorfakephotons=0;

//   Float_t NcandsInMuNuDsMassCuts=0.;//estimate the bias in the MuNu efficiency due to the shift in the mass peak  
//   Float_t NcandsInMuNuDsMassCutsShifted=0.;

//   goodeventid=0;
//   eventid=0;
//   while(ReducedNtuple->GetEntry(eventid++,0)){
//     if(eventid%10000==0)cout<<eventid<<" cands done."<<endl;  


//     if(evthassignal)signaleventid++;
    
//     //-------------------------------------------------------------------------
//     //Fill histograms
//     //-------------------------------------------------------------------------
//     if(goodeventid!=prioreventid){//Fill histogram with masses from previous event
//       for(Int_t i=0;i<nDsCands;i++){
// 	//total 
// 	HDsMass.Fill(priordsmass[i],1./nDsCands);
// 	if((_DataType==0||_DataType==1)){	  
// 	  if(priorevthassignal)HDsMassTrue.Fill(priordsmass[i],1./nDsCands);
// 	  if(!priorevthassignal)HDsMassNoSig.Fill(priordsmass[i],1./nDsCands);
// 	}

// 	//vs nXR
// 	HDsMassvsnXR[priorfragndaus[i]].Fill(priordsmass[i],1./nDsCands);
// 	if(_DataType==0||_DataType==1){//signal pdfs
// 	  if(priorevthassignal)
// 	    HDsMassTruevsnXR[priorfragndaus[i]].Fill(priordsmass[i],1./nDsCands);
// 	  if(!priorevthassignal)
// 	    HDsMassNoSigvsnXR[priorfragndaus[i]].Fill(priordsmass[i],1./nDsCands);
// 	}	  
	
// 	//2D mass vs nXR 
// 	HDsMass2D.Fill(priorfragndaus[i],priordsmass[i],1./nDsCands);
// 	if(_DataType==0||_DataType==1){//signal pdfs
// 	  if(priorevthassignal){
// 	    if(priorfragnpartgen[i]<NXT){//protect array
// 	      HDsMassTrue2DvsnXT[priorfragnpartgen[i]].Fill(priorfragndaus[i],priordsmass[i],1./nDsCands);
// 	      HDsMassTrue2DvsnXTShifted[priorfragnpartgen[i]].Fill(priorfragndaus[i],priordsmass[i]-.0015*(priorgammctrue[i]>0),1./nDsCands);//only shift the true signal photons not the tails

// 	      //determine correction factor for \mu\nu efficiency due to Ds mass peak shift:
// 	      if(DenominatorDsMassMin<priordsmass[i]&&priordsmass[i]<DenominatorDsMassMax)
// 		NcandsInMuNuDsMassCuts+=1.;// /nDsCands;
// 	      if(DenominatorDsMassMin<(priordsmass[i]-.0015*(priorgammctrue[i]>0))
// 		 &&(priordsmass[i]-.0015*(priorgammctrue[i]>0))<DenominatorDsMassMax)
// 		NcandsInMuNuDsMassCutsShifted+=1.;// /nDsCands;

	      
// 	      //photon background systematics--> increase weight of fake photons
// 	      if(0<priorgamsource[i])
// 		HDsMassTrue2DvsnXTShiftedPi0Systematic[priorfragnpartgen[i]].Fill(priorfragndaus[i],priordsmass[i]-.0015*(priorgammctrue[i]>0),1.06/nDsCands);
// 	      else HDsMassTrue2DvsnXTShiftedPi0Systematic[priorfragnpartgen[i]].Fill(priorfragndaus[i],priordsmass[i]-.0015*(priorgammctrue[i]>0),1./nDsCands);
// 	    }
	      
// 	  }
// 	  if(!priorevthassignal)//RS true 2D Background for systematic
// 	    HDsMass2DNoSig.Fill(priorfragndaus[i],priordsmass[i],1./nDsCands);
// 	}	  


// 	//Ds-->KKpi eff.
// 	if(_DataType==0)
// 	  for(Int_t d=0;d<3;d++) 
// 	    if(DsToKKpiPassesDalitzCut(d))
// 	      if(priorfragnpartgen[i]<NXT)
// 		HKKpiDsMassTrue2DvsnXT[d][priorfragnpartgen[i]].Fill(priorfragndaus[i],priordsmass[i],1./nDsCands);
						
//       }


//       //-------------------------------------------------------------------------
//       //Signal MC evt multiplicity 
//       HEvtMultiplicity.Fill(nDsCands);
//       if(priorevthassignal){
// 	//nominal 
// 	HEvtMultiplicitySig.Fill(nDsCands);

// 	//modified
// 	for(Int_t m=0;m<21;m++){
// 	  if(nfakephotonsAdded[m]<nfakephotonsToAdd[m]){//&&nDsCandsMod[m]>0
// 	    for(Int_t rm=0;rm<npriorfakephotons;rm++){
// 	      nDsCandsMod[m]+=(m==10?0:(m<10?-1:1));
// 	      nfakephotonsAdded[m]++;
// 	    }
// 	  }
// 	  if(nDsCandsMod[m]>0)HEvtMultiplicitySigMod[m].Fill(nDsCandsMod[m]);
// 	}
//       }
//       //Bkg MC evt multiplicity 
//       if(!priorevthassignal)HEvtMultiplicityBkg.Fill(nDsCands);

//       //-------------------------------------------------------------------------
//       ///Get ready for next event
//       //--------------------------
//       nDsCands=0;
//       for(Int_t m=0;m<21;m++)
// 	nDsCandsMod[m]=0;
//       npriorfakephotons=0;
//       prioreventid=goodeventid;
//       priorevthassignal=evthassignal;
//       priorkkmassgen=kkmassgen;      
//       priorkpimassgen=kpimassgen;
//     }
    
//     //collect the masses from all Ds cands in this event
//     if(nDsCands<NMAXDSCANDS&&fragndaus<NXR&&(fragnpartgen<NXT||_DataType==2)){//checks are to make sure array indexes are OK 
//       priordsmass[nDsCands]=dsmass;
//       priorfragndaus[nDsCands]=fragndaus;   
//       priorfragnpartgen[nDsCands]=fragnpartgen;
//       priorgammctrue[nDsCands]=gammctrue;
//       priorgamsource[nDsCands]=gamsource;
//       nDsCands++; 
      
//       if(evthassignal){//true=171176   fake=291901
// 	if(gamsource==0)ntruephotons++;
// 	if(gamsource>0){nfakephotons++;npriorfakephotons++;}
// 	for(Int_t m=0;m<21;m++)
// 	  nDsCandsMod[m]++;
//       }

//     }

//   }
  
//   CloseReducedNtuple(); 


//   cout<<"Correction factor to efficiency due to Ds Mass Peak Shift= "<<(NcandsInMuNuDsMassCutsShifted/(float)NcandsInMuNuDsMassCuts)<<endl;


//   TFile HistosFile(_OutputDir+"/HistosForFit.root","recreate");   
//   HDsMass.Write();
//   HDsMassTrue.Write();
//   HDsMassNoSig.Write();

//   for(Int_t i=0;i<NXR;i++){
//     HDsMassvsnXR[i].Write();
//     HDsMassTruevsnXR[i].Write();
//     HDsMassNoSigvsnXR[i].Write();
//   }
  
//   HDsMass2D.Write();
//   HDsMass2DNoSig.Write();
//   for(Int_t j=0;j<NXT;j++){
//     HDsMassTrue2DvsnXT[j].Write();
//     HDsMassTrue2DvsnXTShifted[j].Write();
//     HDsMassTrue2DvsnXTShiftedPi0Systematic[j].Write();
//   }

//   for(Int_t d=0;d<3;d++)
//     for(Int_t j=0;j<NXT;j++)
//       HKKpiDsMassTrue2DvsnXT[d][j].Write();
  
//   HEvtMultiplicity.Write();
//   HEvtMultiplicitySig.Write();
//   for(Int_t m=0;m<21;m++){
//     HEvtMultiplicitySigMod[m].Write();
//   }
//   HEvtMultiplicityBkg.Write();
//   cout<<" signal true="<<ntruephotons<<"   fake="<<nfakephotons<<endl;

//   HistosFile.ls();
//   HistosFile.Close();
// }



void DRecoDsInclusiveAnalysis::CreateHistosForFit(){

  OpenReducedNtuple(); 
  ReducedNtuple->SetBranchStatus("*",0);
  ReducedNtuple->SetBranchStatus("dsmass",1);ReducedNtuple->SetBranchAddress("dsmass",&dsmass);
  ReducedNtuple->SetBranchStatus("goodeventid",1);ReducedNtuple->SetBranchAddress("goodeventid",&goodeventid);
  ReducedNtuple->SetBranchStatus("evthassignal",1);ReducedNtuple->SetBranchAddress("evthassignal",&evthassignal);
  ReducedNtuple->SetBranchStatus("fragndaus",1);ReducedNtuple->SetBranchAddress("fragndaus",&fragndaus);
  ReducedNtuple->SetBranchStatus("fragnpartgen",1);ReducedNtuple->SetBranchAddress("fragnpartgen",&fragnpartgen);
  ReducedNtuple->SetBranchStatus("gammctrue",1);ReducedNtuple->SetBranchAddress("gammctrue",&gammctrue);
  ReducedNtuple->SetBranchStatus("kkmassgen",1);ReducedNtuple->SetBranchAddress("kkmassgen",&kkmassgen);
  ReducedNtuple->SetBranchStatus("kpimassgen",1);ReducedNtuple->SetBranchAddress("kpimassgen",&kpimassgen);
  ReducedNtuple->SetBranchStatus("gamsource",1);ReducedNtuple->SetBranchAddress("gamsource",&gamsource);  
  ReducedNtuple->SetBranchStatus("evthasmunu",1);ReducedNtuple->SetBranchAddress("evthasmunu",&evthasmunu);
  //ReducedNtuple->SetBranchStatus("HasMuNu",1);ReducedNtuple->SetBranchAddress("HasMuNu",&evthasmunu);

  //Total Reco distribution
  TH1F HDsMass("HDsMass","",NDsMassBins,DsMassMin,DsMassMax);
  HDsMass.Sumw2();
  TH1F HDsMassTrue("HDsMassTrue","",NDsMassBins,DsMassMin,DsMassMax);
  HDsMassTrue.Sumw2();

  TH1F HDsMassTrueMuNu("HDsMassTrueMuNu","",NDsMassBins,DsMassMin,DsMassMax);
  HDsMassTrueMuNu.Sumw2();

  TH1F HDsMassTrueKKpi("HDsMassTrueKKpi","",NDsMassBins,DsMassMin,DsMassMax);
  HDsMassTrueKKpi.Sumw2();

  TH2F HDsMass2D("HDsMass2D","",NXR,-.5,NXR-.5,NDsMassBins,DsMassMin,DsMassMax);
  HDsMass2D.Sumw2();
  
  TH2F HDsMass2DNoSig("HDsMass2DNoSig","",NXR,-.5,NXR-.5,NDsMassBins,DsMassMin,DsMassMax);
  HDsMass2DNoSig.Sumw2();

  //signal MC 2D mass vs nXT
  TH2F HDsMassTrue2DvsnXT[NXT]; 
  for(Int_t j=0;j<NXT;j++){//for each nXT
    HDsMassTrue2DvsnXT[j]=TH2F(TString("HDsMassTrue2D_nXT")+(long)j,"",NXR,-.5,NXR-.5,NDsMassBins,DsMassMin,DsMassMax);
    HDsMassTrue2DvsnXT[j].Sumw2();
  }

  //signal pdfs with shifted signal component to fit the Data.
  TH2F HDsMassTrue2DvsnXTShifted[NXT]; 
  for(Int_t j=0;j<NXT;j++){//for each nXT
    HDsMassTrue2DvsnXTShifted[j]=TH2F(TString("HDsMassTrue2D_nXTShifted")+(long)j,"",NXR,-.5,NXR-.5,NDsMassBins,DsMassMin,DsMassMax);
    HDsMassTrue2DvsnXTShifted[j].Sumw2();
  }

  //Modified signal pdf with worse resolution.
  TH2F HDsMassTrue2DvsnXTShiftedRes[NXT]; 
  for(Int_t j=0;j<NXT;j++){//for each nXT
    HDsMassTrue2DvsnXTShiftedRes[j]=TH2F(TString("HDsMassTrue2D_nXTShiftedRes")+(long)j
					 ,"",NXR,-.5,NXR-.5,NDsMassBins,DsMassMin,DsMassMax);
    HDsMassTrue2DvsnXTShiftedRes[j].Sumw2();
  }


  //Modified signal pdfs for Photon background systematic: Pi0+Eta+Track+Klong
  TH2F HDsMassTrue2DvsnXTShiftedPi0Systematic[NXT]; 
  TH2F HDsMassTrue2DvsnXTShiftedPi0SystematicM[NXT]; 
  for(Int_t j=0;j<NXT;j++){//for each nXT
    HDsMassTrue2DvsnXTShiftedPi0Systematic[j]=TH2F(TString("HDsMassTrue2D_nXTShiftedPi0Systematic")+(long)j,"",NXR,-.5,NXR-.5,NDsMassBins,DsMassMin,DsMassMax);
    HDsMassTrue2DvsnXTShiftedPi0Systematic[j].Sumw2();
    
    HDsMassTrue2DvsnXTShiftedPi0SystematicM[j]=TH2F(TString("HDsMassTrue2D_nXTShiftedPi0SystematicM")+(long)j,"",NXR,-.5,NXR-.5,NDsMassBins,DsMassMin,DsMassMax);
    HDsMassTrue2DvsnXTShiftedPi0SystematicM[j].Sumw2();
  }
  
  TH1F HEvtMultiplicity("HEvtMultiplicity","",10,.5,10.5); 
  TH1F HEvtMultiplicitySig("HEvtMultiplicitySig","",10,.5,10.5);
  TH1F HEvtMultiplicityBkg("HEvtMultiplicityBkg","",10,.5,10.5);
  TH1F HEvtMultiplicitySigMod[21];
  for(Int_t m=0;m<21;m++)
    HEvtMultiplicitySigMod[m]=TH1F(TString("HEvtMultiplicitySigMod")+(long)m,"",10,.5,10.5);

  //Histograms for each Dalitz region of Ds-->KKpi signal MC.
  TH2F HKKpiDsMassTrue2DvsnXT[5][NXT]; 
  for(Int_t d=0;d<3;d++){
    for(Int_t j=0;j<NXT;j++){//for each nXT
      HKKpiDsMassTrue2DvsnXT[d][j]=TH2F(TString("HKKpiDsMassTrue2D_Dalitz")+(long)d+"_nXT"+(long)j,"",NXR,-.5,NXR-.5,NDsMassBins,DsMassMin,DsMassMax);
      HKKpiDsMassTrue2DvsnXT[d][j].Sumw2();
    }
  }


  
  Float_t priordsmass[NMAXDSCANDS];
  Int_t priorfragndaus[NMAXDSCANDS];  
  Int_t priorfragnpartgen[NMAXDSCANDS];  
  Int_t priorgammctrue[NMAXDSCANDS];
  Int_t nDsCands=0;
  Int_t prioreventid=1;
  Int_t priorevthassignal=0;
  Int_t priorevthassignalmunu=0;
  Int_t priorevthassignalkkpi=0;
  Int_t priorgamsource[NMAXDSCANDS];

  TF1 fGauss("fGauss","exp(-x**2/.002**2)",-.01,.01);

  goodeventid=0;
  eventid=0;
  while(ReducedNtuple->GetEntry(eventid++,0)){
    if(eventid%10000==0)cout<<eventid<<" cands done."<<endl;  


    //-------------------------------------------------------------------------
    //Fill histograms
    //-------------------------------------------------------------------------
    if(goodeventid!=prioreventid){//Fill histogram with masses from previous event
      //cout<<goodeventid<<" "<<nDsCands<<" "<<endl;
      for(Int_t i=0;i<nDsCands;i++){

	//
	HDsMass.Fill(priordsmass[i],1./nDsCands);
	HDsMass2D.Fill(priorfragndaus[i],priordsmass[i],1./nDsCands);

	//MC true distributions
	if(_DataType==0||_DataType==1){//signal pdfs

	  //true Signal
	  if(priorevthassignal){	  
	    HDsMassTrue.Fill(priordsmass[i],1./nDsCands);
	    if(priorevthassignalmunu)HDsMassTrueMuNu.Fill(priordsmass[i],1./nDsCands);
	    if(priorevthassignalkkpi)HDsMassTrueKKpi.Fill(priordsmass[i],1./nDsCands);

	    HDsMassTrue2DvsnXT[priorfragnpartgen[i]].Fill(priorfragndaus[i],priordsmass[i],1./nDsCands);
	      
	    //only shift the true signal photons not the tails	      
	    HDsMassTrue2DvsnXTShifted[priorfragnpartgen[i]].Fill(priorfragndaus[i],priordsmass[i]-.0015*(priorgammctrue[i]>0),1./nDsCands);

	    //randomize to get worse resolution
	    HDsMassTrue2DvsnXTShiftedRes[priorfragnpartgen[i]].Fill(priorfragndaus[i],priordsmass[i]+(fGauss.GetRandom()-.0015)*(priorgammctrue[i]>0),1./nDsCands);


	    //photon background systematics--> modify weight of fake photons
	    if(0<priorgamsource[i]){
	      HDsMassTrue2DvsnXTShiftedPi0Systematic[priorfragnpartgen[i]].Fill(priorfragndaus[i],priordsmass[i]-.0015*(priorgammctrue[i]>0),1.06/nDsCands);//1.05 factor leads to 4.7% increase
	      HDsMassTrue2DvsnXTShiftedPi0SystematicM[priorfragnpartgen[i]].Fill(priorfragndaus[i],priordsmass[i]-.0015*(priorgammctrue[i]>0),.94/nDsCands);
	    }
	    else {
	      HDsMassTrue2DvsnXTShiftedPi0Systematic[priorfragnpartgen[i]].Fill(priorfragndaus[i],priordsmass[i]-.0015*(priorgammctrue[i]>0),1./nDsCands);
	      HDsMassTrue2DvsnXTShiftedPi0SystematicM[priorfragnpartgen[i]].Fill(priorfragndaus[i],priordsmass[i]-.0015*(priorgammctrue[i]>0),1./nDsCands);
	    }

	  }
	  
	  //true Bkg
	  if(!priorevthassignal){
	    HDsMass2DNoSig.Fill(priorfragndaus[i],priordsmass[i],1./nDsCands);	  
	  }
	 

	  // 	//Ds-->KKpi eff.
	  if(_DataType==0&&_OutputDir.Contains("KKpiEff")&&priorevthassignal){
	    //cout<<priorkkmassgen<<" "<<priorkpimassgen<<endl;
	    for(Int_t d=0;d<3;d++) 
	      if(DsToKKpiPassesDalitzCut(d))
		HKKpiDsMassTrue2DvsnXT[d][priorfragnpartgen[i]].Fill(priorfragndaus[i],priordsmass[i],1./nDsCands);
	  }
	  
 
	}
      }

      //--------------------------
      ///Get ready for next event
      //--------------------------
      nDsCands=0;
      prioreventid=goodeventid;
      priorevthassignal=evthassignal;
      priorevthassignalmunu=evthasmunu;
      priorevthassignalkkpi=kkmassgen>0.;
      priorkkmassgen=kkmassgen;      
      priorkpimassgen=kpimassgen;
    }
    
    //collect the masses from all Ds cands in this event
    //if(nDsCands<NMAXDSCANDS&&fragndaus<NXR&&(fragnpartgen<NXT||_DataType==2)){//checks are to make sure array indexes are OK 
    priordsmass[nDsCands]=dsmass;
    priorfragndaus[nDsCands]=fragndaus;   
    priorfragnpartgen[nDsCands]=fragnpartgen;
    priorgammctrue[nDsCands]=gammctrue;
    priorgamsource[nDsCands]=gamsource;
    nDsCands++;       
    //}

  }
  
  CloseReducedNtuple(); 


  TFile HistosFile(_OutputDir+"/HistosForFit.root","recreate");   
  HDsMass.Write();
  HDsMassTrue.Write();
  HDsMassTrueMuNu.Write();
  HDsMassTrueKKpi.Write();

  HDsMass2D.Write();
  HDsMass2DNoSig.Write();
  for(Int_t j=0;j<NXT;j++){
    HDsMassTrue2DvsnXT[j].Write();
    HDsMassTrue2DvsnXTShifted[j].Write();
    HDsMassTrue2DvsnXTShiftedRes[j].Write();
    HDsMassTrue2DvsnXTShiftedPi0Systematic[j].Write();
    HDsMassTrue2DvsnXTShiftedPi0SystematicM[j].Write();
  }

  
  for(Int_t d=0;d<3;d++)
    for(Int_t j=0;j<NXT;j++)
      HKKpiDsMassTrue2DvsnXT[d][j].Write();
  
  HistosFile.ls();
  HistosFile.Close();
}









Bool_t DRecoDsInclusiveAnalysis::DsToKKpiPassesDalitzCut(Int_t d){
  if(d<0||d>2)return 0;
  //Ds-->KKpi Dalitz regions

  Bool_t pass[3]={0,0,0};
  if(DalitsKPiXMin<priorkkmassgen&&priorkkmassgen<DalitsKPiXMax && DalitsKPiLowYMin<priorkpimassgen&&priorkpimassgen<DalitsKPiHighYMax)
    pass[0]=1;
  if(DalitsKKLowXMin<priorkkmassgen&&priorkkmassgen<DalitsKKHighXMax && DalitsKKYMin<priorkpimassgen&&priorkpimassgen<DalitsKKYMax)
    pass[1]=1; 
  if(!pass[0]&&!pass[1])
    pass[2]=1;
  

  return pass[d];
}



void DRecoDsInclusiveAnalysis::RooFitDsMassSimul(){


  filename=_OutputDir+"/DsMassRooFitSimul.ps";
  Canvas->Clear();
  Canvas->Print(filename+"["); 
  

  RooArgSet nset(mass);

  //Get the WS Signal Pdfs
  TFile BkgPdfsFile("DsTolnu/SP1005Skim/DsReco/FinalWS/HistosForFit.root","read");
  gROOT->cd();  
  TH1F* HDsMassWSTrue=(TH1F*)BkgPdfsFile.Get("HDsMassTrue");
  if(!HDsMassWSTrue){cout<<" Not found"<<endl;return;}    
  RooDataHist DsMassWSTrueDat("DsMassWSTrueDat","DsMassWSTrueDat",RooArgSet(mass),HDsMassWSTrue);
  DRecoHistPdf WSSigPdf(TString("WSSigPdf"),TString("WSSigPdf"),nset,DsMassWSTrueDat,0);
  Canvas->Clear();
  HDsMassWSTrue->Draw();
  Canvas->Print(filename);   

  
  //Get the W.S. data
  TFile DataFileWS(_OutputDir+"/../FinalWS/HistosForFit.root","read");
  gROOT->cd();
  TH1F* HDsMassDataWS=(TH1F*)DataFileWS.Get("HDsMass");  
  RooDataHist DsMassDataSetWS("DsMassDataSetWS","DsMassDataSetWS",RooArgSet(mass),HDsMassDataWS);

   //RooRealVar WSTotalIntegral("WSTotalIntegral","WSTotalIntegral",HDsMassDataWS->Integral());
  //RooRealVar RSTotalIntegral("RSTotalIntegral","RSTotalIntegral",HDsMassData->Integral());
  //RooFormulaVar WSSigYield(TString("WSSigYield"),TString("WSSigYield"),"WStoRSFrac*SigYield*RSTotalIntegral/WSTotalIntegral",RooArgSet(SigYield,WSTotalIntegral,RSTotalIntegral,WStoRSFrac));
  RooRealVar WSSigYield(TString("WSSigYield"),TString("WSSigYield"),.05,.3);
  
  //Create the background pdf out of the subtraction of the wrong-sign signalMC from total wrong-sign.
  //RooRealVar WSFracSigYield("WSFracSigYield","WSFracSigYield",-.2,-.001);    
  RooFormulaVar WSFracSigYield("WSFracSigYield","WSFracSigYield","-(1./(1.-WSSigYield) - 1.)",RooArgSet(WSSigYield));    
  DRecoHistPdf WSDataPdf(TString("WSDataPdf"),TString("WSDataPdf"),nset,DsMassDataSetWS,0);
  DRecoAddPdf WSBkgPdfRaw(TString("WSBkgPdfRaw"),TString("WSBkgPdfRaw"),nset,WSSigPdf,WSDataPdf,WSFracSigYield); 
  //DRecoAddPdf WSBkgPdf(TString("WSBkgPdf"),TString("WSBkgPdf"),nset,WSSigPdf,WSDataPdf,WSFracSigYield);

  //create the total WS. pdf  
  DRecoAddPdf TotalWSPdf(TString("TotalWSPdf"),TString("TotalWSPdf"),nset,WSSigPdf,WSBkgPdfRaw,WSSigYield);

  ///Get the R.S signal Pdf.
  TFile PdfsFile("DsTolnu/SP1005Skim/DsReco/Final/HistosForFit.root","read"); 
  gROOT->cd();
  TH1F* HDsMassTrue=(TH1F*)PdfsFile.Get("HDsMassTrue");
  if(!HDsMassTrue){cout<<" Not found"<<endl;return;}    
  RooDataHist DsMassTrueDat("DsMassTrueDat","DsMassTrueDat",RooArgSet(mass),HDsMassTrue);
  DRecoHistPdf SigPdf("SigPdf","SigPdf",nset,DsMassTrueDat,0);
  Canvas->Clear();
  HDsMassTrue->Draw();
  Canvas->Print(filename); 

  //Correct the WS bkg model for discrepancy with RS bkg.
  TFile WSCorrFile("DsTolnu/SP1005Skim/DsReco/FinalWS/WSTotalBkgCorr.root","read");
  gROOT->cd();
  TH1F* HWSBkgCorr=(TH1F*)WSCorrFile.Get("HWSTotalBkgCorr");
  RooDataHist WSBkgCorrDat("WSBkgCorrDat","WSBkgCorrDat",nset,HWSBkgCorr);
  Canvas->Clear();
  HWSBkgCorr->GetYaxis()->SetRangeUser(0,2);
  HWSBkgCorr->Draw("pe");
  Canvas->Print(filename); 
  DRecoHistPdf WSBkgCorrPdf("WSBkgCorrPdf","WSBkgCorrPdf",nset,WSBkgCorrDat,0);
  DRecoProdPdf WSBkgPdf("WSBkgPdf","WSBkgPdf",nset,WSBkgCorrPdf,WSBkgPdfRaw);

  //create total RS. pdf
  RooRealVar SigYield(TString("SigYield"),TString("SigYield"),.01,.5);
  DRecoAddPdf TotalPdf(TString("TotalPdf"),TString("TotalPdf"),nset,SigPdf,WSBkgPdf,SigYield);

  //Get the R.S. data
  TFile DataFile(_OutputDir+"/HistosForFit.root","read");
  gROOT->cd();
  TH1F* HDsMassData=(TH1F*)DataFile.Get("HDsMass");
  RooDataHist DsMassDataSet(HDsMassData->GetName(),HDsMassData->GetName(),RooArgSet(mass),HDsMassData);


  
  //create a simultaneous chisquare
  TList histlist;
  histlist.Add(HDsMassDataWS);
  histlist.Add(HDsMassData);
  TList pdflist;
  pdflist.Add(&TotalWSPdf);
  pdflist.Add(&TotalPdf);
  TList xvarlist;
  xvarlist.Add(&mass);
  xvarlist.Add(&mass);
  RooArgSet paramset;
  paramset.add(mass);
  //paramset.add(WSFracSigYield);
  paramset.add(WSSigYield);
  paramset.add(SigYield);
  RooGenericPdf dummypdf("dummypdf","dummypdf","mass",paramset);
  DRecoSimulChi2Var chi2("chi2","chi2",dummypdf,DsMassDataSetWS,&histlist,&pdflist,&xvarlist,0);
  
  //minimize				
  RooMinuit minuit(chi2);
  minuit.migrad();


  //make W.S. plot
  RooPlot*plots=mass.frame();
  TH1*TotalWSPdfHisto=TotalWSPdf.CreateHistogram("TotalWSPdfHisto");  
  TotalWSPdfHisto->Scale(HDsMassDataWS->Integral()/TotalWSPdfHisto->Integral()); 
  TotalWSPdfHisto->SetLineColor(2);
  plots->addTH1(TotalWSPdfHisto,"hist");
  TH1*WSBkgPdfHisto=WSBkgPdf.CreateHistogram("WSBkgPdfHisto");  
  WSBkgPdfHisto->Scale((1.-WSSigYield.getVal())*HDsMassDataWS->Integral()/WSBkgPdfHisto->Integral());  
  WSBkgPdfHisto->SetFillColor(4);
  plots->addTH1(WSBkgPdfHisto,"same hist");
  plots->addTH1(HDsMassDataWS,"pe hist");
  Canvas->Clear();
  plots->SetTitle(TString("Yield=")+(long)(WSSigYield.getVal()*HDsMassDataWS->Integral())+" +- "+(long)(WSSigYield.getError()*HDsMassDataWS->Integral()));
  plots->Draw();
  Canvas->Print(filename);  


  delete plots;
  

  //make R.S. plot
  plots=mass.frame();
  TH1*TotalPdfHisto=TotalPdf.CreateHistogram("TotalPdfHisto");  
  TotalPdfHisto->Scale(HDsMassData->Integral()/TotalPdfHisto->Integral());
  TotalPdfHisto->SetLineColor(2);
  plots->addTH1(TotalPdfHisto,"hist");
  TH1*BkgPdfHisto=WSBkgPdf.CreateHistogram("BkgPdfHisto");
  BkgPdfHisto->Scale((1.-SigYield.getVal())*HDsMassData->Integral()/BkgPdfHisto->Integral());  
  BkgPdfHisto->SetFillColor(4);
  plots->addTH1(BkgPdfHisto,"same hist");
  plots->addTH1(HDsMassData,"pe hist");

  DRecoResiduals residuals("resid",HDsMassData,TotalPdfHisto);
  residuals.FitHasError(1);
  Canvas->Clear();
  plots->SetTitle(TString("Yield=")+(long)(SigYield.getVal()*HDsMassData->Integral())+" +- "+(long)(SigYield.getError()*HDsMassData->Integral()));
  residuals.MakeFitPlotWithResiduals(Canvas,plots);
  Canvas->Print(filename);  
  delete plots;


  Canvas->Print(filename+"]");
  DataFile.Close();
  DataFileWS.Close();


}

void DRecoDsInclusiveAnalysis::RooFitDsMassvsnXRSimul(){

  filename=_OutputDir+"/DsMassRooFitvsnXRSimul.ps";
  Canvas->Clear();
  Canvas->Print(filename+"["); 
  

  RooArgSet nset(mass);


  ///Get the WS signal Pdfs
  TFile PdfsFileWS("DsTolnu/SP1005Skim/DsReco/FinalWS/HistosForFit.root","read"); 
  gROOT->cd();
  TH1F* HDsMassTrueWS[NXR];
  RooDataHist* DatahistWS[NXR];
  DRecoHistPdf* SigPdfWS[NXR];
  RooRealVar* SigYieldWS[NXR];
  Canvas->Clear();
  Canvas->Divide(2,2);
  for(Int_t i=0;i<NXR;i++){//there is no WS for nXR=0
    Canvas->cd(i+1); 
    HDsMassTrueWS[i]=(TH1F*)PdfsFileWS.Get(TString("HDsMassTrue_nXR")+(long)i);
    if(!HDsMassTrueWS[i]){cout<<" Not found"<<endl;return;}    
    DatahistWS[i]=new RooDataHist(TString("DatahistWS")+(long)i,TString("DatahistWS")+(long)i,RooArgSet(mass),HDsMassTrueWS[i]);
    SigPdfWS[i]=new DRecoHistPdf(TString("SigPdfWS")+(long)i,TString("SigPdfWS")+(long)i,nset,*DatahistWS[i],0); 
    SigYieldWS[i]=new RooRealVar(TString("SigYieldWS")+(long)i,TString("SigYieldWS")+(long)i,0,.25);   
    HDsMassTrueWS[i]->SetTitle(TString("Integral=")+(long)HDsMassTrueWS[i]->Integral());
    HDsMassTrueWS[i]->Draw();
  }
  Canvas->Print(filename); 
 
  //Get the WS Data histograms
  TFile DataFileWS(_OutputDir+"/../FinalWS/HistosForFit.root","read");
  gROOT->cd();
  TH1F* HDsMassDataWS[NXR];
  RooDataHist* DsMassDataSetWS[NXR];
  Canvas->Clear();
  Canvas->Divide(2,2);
  for(Int_t i=0;i<NXR;i++){
    Canvas->cd(i+1);
    HDsMassDataWS[i]=(TH1F*)DataFileWS.Get(TString("HDsMass_nXR")+(long)i);
    if(!HDsMassDataWS[i]){cout<<" Not found"<<endl;return;}  
    DsMassDataSetWS[i]=new RooDataHist(TString("DsMassDataSetWS")+(long)i,TString("DsMassDataSetWS")+(long)i,
				       RooArgSet(mass),HDsMassDataWS[i]);
    HDsMassDataWS[i]->Draw();
  }
  Canvas->Print(filename);



  //Create a pdf out of the subtraction of the wrong-sign signalMC from total wrong-sign.
  RooFormulaVar* WSFracYield[NXR];
  DRecoHistPdf* WSDataPdf[NXR];
  DRecoAddPdf* WSBkgPdfRaw[NXR];
  DRecoAddPdf* TotalPdfWS[NXR];
  RooRealVar* WSnXR0Coef[NXR];
  for(Int_t i=1;i<NXR;i++){
    WSFracYield[i]=new RooFormulaVar(TString("WSFracYield")+(long)i,TString("WSFracYield")+(long)i,
				     TString("-(1./(1.-SigYieldWS")+(long)i+") - 1.)",RooArgSet(*SigYieldWS[i]));
    WSDataPdf[i]=new DRecoHistPdf(TString("WSDataPdf")+(long)i,TString("WSDataPdf")+(long)i,nset,*DsMassDataSetWS[i],0);

    WSBkgPdfRaw[i]=new DRecoAddPdf(TString("WSBkgPdfRaw")+(long)i,TString("WSBkgPdfRaw")+(long)i,
				   nset,*SigPdfWS[i],*WSDataPdf[i],*WSFracYield[i]);

    TotalPdfWS[i]=new DRecoAddPdf(TString("TotalPdfWS")+(long)i,TString("TotalPdfWS")+(long)i,nset,
				   *SigPdfWS[i],*WSBkgPdfRaw[i],*SigYieldWS[i]);

    WSnXR0Coef[i]=new RooRealVar(TString("WSnXR0Coef")+(long)i,TString("WSnXR0Coef")+(long)i,1./3.);

  }
  
  ///--------------------------------------------------------
  //create a special pdf for nXR=0
  //---------------------------------------------------------
  WSBkgPdfRaw[0]=new DRecoAddPdf("WSBkgPdfRaw0","WSBkgPdfRaw0",nset,
				 RooArgList(*WSBkgPdfRaw[1],*WSBkgPdfRaw[2],*WSBkgPdfRaw[3]),
				 RooArgList(*WSnXR0Coef[1],*WSnXR0Coef[2]));
  

  //correct all nXR pdfs for WS-RS discrepancy
  TH1F* HWSBkgCorr[NXR];

  //
  TFile WSCorrFile("DsTolnu/SP1005Skim/DsReco/FinalWS/WSnXR0Correction.root","read");
  gROOT->cd();
  HWSBkgCorr[0]=(TH1F*)WSCorrFile.Get("HnXR0Correction");
  if(!HWSBkgCorr[0]){cout<<" Not found"<<endl;return;}  


  TFile WSnXRCorrFile("DsTolnu/SP1005Skim/DsReco/FinalWS/WSnXRBkgCorr.root","read");
  gROOT->cd();  
  for(Int_t i=1;i<NXR;i++){
    HWSBkgCorr[i]=(TH1F*)WSnXRCorrFile.Get(TString("HWSBkgCorr")+(long)i);    
  }

  RooDataHist* WSBkgCorrDat[NXR];
  DRecoHistPdf* WSBkgCorrPdf[NXR];
  DRecoProdPdf* WSBkgPdf[NXR];    
  for(Int_t i=0;i<NXR;i++){
    WSBkgCorrDat[i]=new RooDataHist("WSBkgCorrDat","WSBkgCorrDat",nset,HWSBkgCorr[i]);
    WSBkgCorrPdf[i]=new DRecoHistPdf("WSBkgCorrPdf","WSBkgCorrPdf",nset,*WSBkgCorrDat[i],0);
    WSBkgPdf[i]=new DRecoProdPdf(TString("WSBkgPdf")+(long)i,TString("WSBkgPdf")+(long)i,nset,*WSBkgCorrPdf[i],*WSBkgPdfRaw[i]);
  }



  ///Get the RS signal Pdfs
  TFile PdfsFile("DsTolnu/SP1005Skim/DsReco/Final/HistosForFit.root","read"); 
  gROOT->cd();
  TH1F* HDsMassTrue[NXR];
  RooDataHist* Datahist[NXR];
  DRecoHistPdf* SigPdf[NXR];
  RooRealVar* SigYield[NXR];
  Canvas->Clear();
  Canvas->Divide(2,2);
  for(Int_t i=0;i<NXR;i++){
    Canvas->cd(i+1); 
    HDsMassTrue[i]=(TH1F*)PdfsFile.Get(TString("HDsMassTrue_nXR")+(long)i);
    if(!HDsMassTrue[i]){cout<<" Not found"<<endl;return;}    
    Datahist[i]=new RooDataHist(TString("Datahist")+(long)i,TString("Datahist")+(long)i,RooArgSet(mass),HDsMassTrue[i]);
    SigPdf[i]=new DRecoHistPdf(TString("SigPdf")+(long)i,TString("SigPdf")+(long)i,nset,*Datahist[i],0); 
    SigYield[i]=new RooRealVar(TString("SigYield")+(long)i,TString("SigYield")+(long)i,0,.5);   
    HDsMassTrue[i]->Draw();
    HDsMassTrue[i]->SetTitle(TString("Integral=")+(long)HDsMassTrue[i]->Integral());
  }
  Canvas->Print(filename); 
  



  //Get the RS Data histograms
  TFile DataFile(_OutputDir+"/HistosForFit.root","read");
  gROOT->cd();
  TH1F* HDsMassData[NXR];
  RooDataHist* DsMassData[NXR];
  Canvas->Clear();
  Canvas->Divide(2,2);
  for(Int_t i=0;i<NXR;i++){
    Canvas->cd(i+1);
    HDsMassData[i]=(TH1F*)DataFile.Get(TString("HDsMass_nXR")+(long)i);
    if(!HDsMassData[i]){cout<<" Not found"<<endl;return;}      

    DsMassData[i]=new RooDataHist(TString("DsMassData")+(long)i,TString("DsMassData")+(long)i,RooArgSet(mass),HDsMassData[i]);
    HDsMassData[i]->Draw();
  }
  Canvas->Print(filename);


  //create list of histos to be fitted
  TList histlist;
  TList pdflist;
  TList xvarlist;
  RooArgSet paramset;
  paramset.add(mass);
  DRecoAddPdf* TotalPdf[NXR];
  for(Int_t i=0;i<NXR;i++){
    TotalPdf[i]=new DRecoAddPdf(TString("TotalPdf")+(long)i,TString("TotalPdf")+(long)i,nset,*SigPdf[i],*WSBkgPdf[i],*SigYield[i]);

    histlist.Add(HDsMassData[i]);  
    pdflist.Add(TotalPdf[i]);
    xvarlist.Add(&mass);
    paramset.add(*SigYield[i]);
    
    if(i>0){//no WS data for nXR=0
      histlist.Add(HDsMassDataWS[i]);
      pdflist.Add(TotalPdfWS[i]);
      xvarlist.Add(&mass);
      paramset.add(*WSFracYield[i]);
      paramset.add(*SigYieldWS[i]);
    }                     
    
  }
  

  //Create the simultaneous chisquare
  RooGenericPdf dummypdf("dummypdf","dummypdf","mass",paramset);
  DRecoSimulChi2Var chi2("chi2","chi2",dummypdf,*DsMassDataSetWS[3],&histlist,&pdflist,&xvarlist,0);
  
  //minimize				
  RooMinuit minuit(chi2);
  minuit.migrad();

 
 
  //Plot the W.S fits
  Float_t totwsyield=0.;
  Float_t totwsyielder=0.;
  TH1F HBkgFitWS("HBkgFitWS","",NDsMassBins,DsMassMin,DsMassMax);
  TH1F HTotFitWS("HTotFitWS","",NDsMassBins,DsMassMin,DsMassMax);
  TH1F HTotDataWS("HTotDataWS","",NDsMassBins,DsMassMin,DsMassMax);
  TH1* HnXRBkgFitWS[NXR];
  TH1* HnXRTotFitWS[NXR];
  Canvas->Clear();
  Canvas->Divide(2,2);
  for(Int_t i=1;i<NXR;i++){
    Canvas->cd(i+1);    
    
    HnXRTotFitWS[i]=TotalPdfWS[i]->CreateHistogram((const char*)(TString("HnXRTotFitWS")+(long)i));
    HnXRTotFitWS[i]->Scale(HDsMassDataWS[i]->Integral()/HnXRTotFitWS[i]->Integral());
    HnXRTotFitWS[i]->SetLineColor(2);
    HnXRTotFitWS[i]->SetStats(0);
    HnXRTotFitWS[i]->SetTitle(TString("Yield=")+(long)(SigYieldWS[i]->getVal()*HDsMassDataWS[i]->Integral())+" +- "+(long)(SigYieldWS[i]->getError()*HDsMassDataWS[i]->Integral()));
    HnXRTotFitWS[i]->Draw("hist");

    HnXRBkgFitWS[i]=WSBkgPdf[i]->CreateHistogram((const char*)(TString("HnXRBkgFitWS")+(long)i));
    HnXRBkgFitWS[i]->Scale((1.-SigYieldWS[i]->getVal())*HDsMassDataWS[i]->Integral()/HnXRBkgFitWS[i]->Integral());
    HnXRBkgFitWS[i]->SetLineColor(0);
    HnXRBkgFitWS[i]->SetFillColor(4);
    HnXRBkgFitWS[i]->SetStats(0);
    HnXRBkgFitWS[i]->Draw("same hist");

    HDsMassDataWS[i]->SetStats(0);
    HDsMassDataWS[i]->Draw("same pe");

    HTotFitWS.Add(HnXRTotFitWS[i]);
    HBkgFitWS.Add(HnXRBkgFitWS[i]);
    HTotDataWS.Add(HDsMassDataWS[i]);

    totwsyield+=SigYieldWS[i]->getVal()*HDsMassDataWS[i]->Integral();
    totwsyielder+=SigYieldWS[i]->getError()*HDsMassDataWS[i]->Integral()*SigYieldWS[i]->getError()*HDsMassDataWS[i]->Integral();
  }
  Canvas->Print(filename);  


  Canvas->Clear();
  HTotFitWS.SetTitle(TString("Signal Yield=")+(long)(totwsyield)+" +-"+(long)sqrt(totwsyielder));
  HTotFitWS.SetLineColor(2);
  HTotFitWS.SetStats(0);
  HTotFitWS.Draw("hist");
  HBkgFitWS.SetLineColor(0);
  HBkgFitWS.SetFillColor(4);
  HBkgFitWS.Draw("same hist");
  HTotDataWS.Draw("same pe");
  Canvas->Print(filename);  


  RooPlot*plots[NXR];

  //Plot the R.S fits 
  Float_t totyield=0.;
  Float_t totyielder=0.;
  TH1F HBkgFit("HBkgFit","",NDsMassBins,DsMassMin,DsMassMax);
  TH1F HTotFit("HTotFit","",NDsMassBins,DsMassMin,DsMassMax);
  TH1F HTotData("HTotData","",NDsMassBins,DsMassMin,DsMassMax);
  TH1* HnXRBkgFit[NXR];
  TH1* HnXRTotFit[NXR];
  DRecoResiduals* residuals[NXR];
  Canvas->Clear();
  Canvas->Divide(2,2);
  for(Int_t i=0;i<NXR;i++){
    TPad*pad=(TPad*)Canvas->cd(i+1);    
    plots[i]=mass.frame();

    HnXRTotFit[i]=TotalPdf[i]->CreateHistogram((const char*)(TString("HnXRTotFit")+(long)i));
    HnXRTotFit[i]->Scale(HDsMassData[i]->Integral()/HnXRTotFit[i]->Integral());
    HnXRTotFit[i]->SetLineColor(2);
    HnXRTotFit[i]->SetStats(0);
    plots[i]->addTH1(HnXRTotFit[i],"hist");

    if(i==0)HnXRBkgFit[i]=((DRecoProdPdf*)WSBkgPdf[i])->CreateHistogram((const char*)(TString("HnXRBkgFit")+(long)i));
    else HnXRBkgFit[i]=WSBkgPdf[i]->CreateHistogram((const char*)(TString("HnXRBkgFit")+(long)i));
    HnXRBkgFit[i]->Scale((1.-SigYield[i]->getVal())*HDsMassData[i]->Integral()/HnXRBkgFit[i]->Integral());
    HnXRBkgFit[i]->SetLineColor(0);
    HnXRBkgFit[i]->SetFillColor(4);
    HnXRBkgFit[i]->SetStats(0);
    plots[i]->addTH1(HnXRBkgFit[i],"same hist");

    HDsMassData[i]->SetStats(0);
    plots[i]->addTH1(HDsMassData[i],"same pe");

    plots[i]->SetTitle(TString("Yield=")+(long)(SigYield[i]->getVal()*HDsMassData[i]->Integral())
		       +" +- "+(long)(SigYield[i]->getError()*HDsMassData[i]->Integral()));
    //plots[i]->Draw();

    residuals[i]=new DRecoResiduals(TString("resid")+(long)i,HDsMassData[i],HnXRTotFit[i]);   
    residuals[i]->FitHasError(1);
    residuals[i]->MakeFitPlotWithResiduals((TCanvas*)pad,plots[i]);

    HTotFit.Add(HnXRTotFit[i]);
    HBkgFit.Add(HnXRBkgFit[i]);
    HTotData.Add(HDsMassData[i]);

    totyield+=SigYield[i]->getVal()*HDsMassData[i]->Integral();
    totyielder+=SigYield[i]->getError()*HDsMassData[i]->Integral()*SigYield[i]->getError()*HDsMassData[i]->Integral();
    
  }
  Canvas->Print(filename);  
  for(Int_t i=0;i<NXR;i++){
    delete plots[i];
    delete residuals[i];
  }


  RooPlot* totalplot=mass.frame();
  Canvas->Clear();
  HTotFit.SetLineColor(2);
  HTotFit.SetStats(0);
  totalplot->addTH1(&HTotFit,"hist");
  HBkgFit.SetLineColor(0);
  HBkgFit.SetFillColor(4);
  totalplot->addTH1(&HBkgFit,"same hist");
  totalplot->addTH1(&HTotData,"pe hist");
  DRecoResiduals totresid("totresid",&HTotData,&HTotFit); 
  totresid.FitHasError(1);
  totalplot->SetTitle(TString("Signal Yield=")+(long)(totyield)+" +-"+(long)sqrt(totyielder));
  totresid.MakeFitPlotWithResiduals(Canvas,totalplot); 
  Canvas->Print(filename);  
  delete totalplot;


  Canvas->Print(filename+"]");
  PdfsFile.Close(); 
  PdfsFileWS.Close(); 
  DataFile.Close();
  DataFileWS.Close();
  WSCorrFile.Close();


}

void DRecoDsInclusiveAnalysis::RooFitDsMassvsnXRvsnXTSimul(TH2F*HDsMassDataWS,TH2F*HDsMassData,TString resultFileName){

  filename=_OutputDir+"/"+resultFileName+".ps";
  Canvas->Clear();
  Canvas->Print(filename+"["); 
    
  RooArgSet nset;  
  nset.add(nxr);
  nset.add(mass);
  

  ///Get the WS signal Pdfs from the signal MC 
  RooArgList pdflistws;
  RooArgList coeflistWS;
  RooRealVar* nXTCoefWS[NXT];
  Canvas->Clear();
  Canvas->Divide(2,3);
  TH2F* SigWSPdfHisto[NXT];
  for(Int_t j=0;j<NXT;j++){    
    pdflistws.add(*SigPdfWS[j]);    
    Canvas->cd(j+1);
    SigWSPdfHisto[j]=new TH2F(TString("SigPdfWSn_{X}^{T}=")+(long)j,"",NXR,-.5,NXR-.5,NDsMassBins,DsMassMin,DsMassMax);
    SigPdfWS[j]->fillHistogram(SigWSPdfHisto[j],RooArgList(nxr,mass));
    SigWSPdfHisto[j]->Scale(1./SigWSPdfHisto[j]->Integral());
    SigWSPdfHisto[j]->SetTitle(TString("n_{X}^{T}=")+(long)j);
    SigWSPdfHisto[j]->Draw("lego");
 
    //nXTCoefWS[j]=new RooRealVar(TString("nXTCoefWS")+(long)j,TString("nXTCoefWS")+(long)j,-.3,.65);
    nXTCoefWS[j]=new RooRealVar(TString("nXTCoefWS")+(long)j,TString("nXTCoefWS")+(long)j,.6*truewws[j],1.4*truewws[j]);
    nXTCoefWS[j]->setVal(truewws[j]);
    nXTCoefWS[j]->setConstant(1);
    if(j<NXT-1){
      coeflistWS.add(*nXTCoefWS[j]);
    }
  }    
  Canvas->Print(filename); 
  for(Int_t j=0;j<NXT;j++) delete SigWSPdfHisto[j];
  

  //construct the total signal pdf out of the addition of each nXT pdf
  DRecoAddPdf TotalSigWSPdf("TotalSigWSPdf","TotalSigWSPdf",nset,pdflistws,coeflistWS);
  Canvas->Clear();
  TH2F* TotalSigWSPdfHisto=(TH2F*)TotalSigWSPdf.CreateHistogram("TotalSigWSPdfHisto");
  TotalSigWSPdfHisto->Draw("lego");
  Canvas->Print(filename);


  //Get the WSData histograms
  RooDataHist DsMassDataSetWS("DsMassDataSetWS","DsMassDataSetWS",nset,HDsMassDataWS);
  Canvas->Clear();
  HDsMassDataWS->Draw("lego");
  Canvas->Print(filename);

  RooRealVar RSToWSRatio("RSToWSRatio","RSToWSRatio",HDsMassData->Integral()/HDsMassDataWS->Integral());
  RooFormulaVar TotYieldWS("TotYieldWS","TotYieldWS","WStoRSFrac*TotYield*RSToWSRatio",RooArgSet(TotYield,RSToWSRatio,WStoRSFrac));

  //create a 2D background pdf from the WS Data - WS signal pdf
  DRecoHistPdf DsMassWSPdf("DsMassWSPdf","DsMassWSPdf",nset,DsMassDataSetWS);
  RooFormulaVar TotWSFracYield("TotWSFracYield","TotWSFracYield","-(1./(1.-TotYieldWS)-1.)",RooArgSet(TotYieldWS));
  DRecoAddPdf TotWSBkgPdf("TotWSBkgPdf","TotWSBkgPdf",nset,TotalSigWSPdf,DsMassWSPdf,TotWSFracYield);
  Canvas->Clear();
  TH2F* TotWSBkgPdfHisto=(TH2F*)TotWSBkgPdf.CreateHistogram("TotWSBkgPdfHisto");
  TotWSBkgPdfHisto->Draw("lego");
  Canvas->Print(filename);

  //create total WS pdf
  DRecoAddPdf TotPdfWS("TotPdfWS","TotPdfWS",nset,TotalSigWSPdf,TotWSBkgPdf,TotYieldWS);
  Canvas->Clear();
  TH2F* TotPdfWSHisto=(TH2F*)TotPdfWS.CreateHistogram("TotPdfWSHisto");
  TotPdfWSHisto->Draw("lego");
  Canvas->Print(filename);


  //-------------------------------------------------
  //create the background pdf for the RS.
  //-------------------------------------------------
  //The background yield for each nXR will need to be floated so we need to create a 1D background pdf for each nXR
  for(Int_t i=1;i<NXR;i++){//no data for nXR=0 
      WSBkgnXRPdfRaw[i]=new DRecoProdPdf(TString("WSBkgnXRPdfRaw")+(long)i,TString("WSBkgnXRPdfRaw")+(long)i,nset,*WSBkgnXRSelectPdf[i],TotWSBkgPdf);
            
      //Translation pdfs for nXR=0
      delta[i]=new RooRealVar(TString("delta")+(long)i,TString("delta")+(long)i,i);
      WSBkgnXRTransPdf[i]=new DRecoTranslationPdf(TString("WSBkgnXRTransPdf")+(long)i,TString("WSBkgnXRTransPdf")+(long)i,
						  nset,*WSBkgnXRPdfRaw[i],nxr,*delta[i]);
      bkgaddcoef[i]=new RooRealVar(TString("bkgaddcoef")+(long)i,"",1./3.);

      //Translation pdfs for nXR=1
      delta1[i]=new RooRealVar(TString("delta1")+(long)i,TString("delta1")+(long)i,i-1);
      WSBkgnXR1TransPdf[i]=new DRecoTranslationPdf(TString("WSBkgnXR1TransPdf")+(long)i,TString("WSBkgnXR1TransPdf")+(long)i,
						   nset,*WSBkgnXRPdfRaw[i],nxr,*delta1[i]);      
      bkgaddcoef1[i]=new RooRealVar(TString("bkgaddcoef1")+(long)i,"",1./3.);
  }
  //create a bkg pdf for nXR=0
  DRecoAddPdf WSBkgPdfnXR0AvgRaw("WSBkgPdfnXR0AvgRaw","WSBkgPdfnXR0AvgRaw",nset,
				 RooArgList(*WSBkgnXRTransPdf[1],*WSBkgnXRTransPdf[2],*WSBkgnXRTransPdf[3]),
				 RooArgList(*bkgaddcoef[1],*bkgaddcoef[2]));
  WSBkgnXRPdf[0]=new DRecoProdPdf("WSBkgnXRPdf0","WSBkgnXRPdf0",nset,*WSnXR0CorrPdf,WSBkgPdfnXR0AvgRaw);
  
  //create a bkg pdf for nXR=1
  DRecoAddPdf WSBkgPdfnXR1AvgRaw("WSBkgPdfnXR1AvgRaw","WSBkgPdfnXR1AvgRaw",nset,
				 RooArgList(*WSBkgnXR1TransPdf[1],*WSBkgnXR1TransPdf[2],*WSBkgnXR1TransPdf[3]),
				 RooArgList(*bkgaddcoef1[1],*bkgaddcoef1[2]));
  WSBkgnXRPdf[1]=new DRecoProdPdf("WSBkgnXRPdf1","WSBkgnXRPdf1",nset,*WSnXR1CorrPdf,WSBkgPdfnXR1AvgRaw);
                                   
  //no correction is necessary for nXR=2,3 
  WSBkgnXRPdf[2]=WSBkgnXRPdfRaw[2];
  WSBkgnXRPdf[3]=WSBkgnXRPdfRaw[3];

  
  //----------------------------------------------------------------------------
  //add all the nXR backgrounds to create the full 2D RS background
  //-----------------------------------------------------------------------------
  RooArgList rsbkgyieldlist;
  RooArgList rsbkgpdflist;
  TH2F* WSnxRHisto[NXR];
  Canvas->Clear();
  Canvas->Divide(2,2);
  for(Int_t i=0;i<NXR;i++){

    rsbkgpdflist.add(*WSBkgnXRPdf[i]);
    if(i<NXR-1){
      rsbkgyieldlist.add(*BkgnXRYield[i]);
    }

    Canvas->cd(i+1);    
    //show the individual nXR backgrounds
    WSnxRHisto[i]=(TH2F*)WSBkgnXRPdf[i]->CreateHistogram(TString("WSnxRHisto")+(long)i);
    WSnxRHisto[i]->Draw("lego"); 
  }  
  Canvas->Print(filename);
  for(Int_t i=0;i<NXR;i++) delete WSnxRHisto[i];
   

  DRecoAddPdf TotRSBkgPdf("TotRSBkgPdf","TotRSBkgPdf",nset,rsbkgpdflist,rsbkgyieldlist);
  Canvas->Clear();
  TH2F* TotRSBkgPdfHisto=(TH2F*)TotRSBkgPdf.CreateHistogram("TotRSBkgPdfHisto");
  TotRSBkgPdfHisto->Draw("lego");
  Canvas->Print(filename);


  ///Get the signal Pdfs from the signal MC 
  TH2F* HSigPdfHisto[NXT];
  RooArgList pdflist;
  RooRealVar* nXTCoef[NXT];
  RooArgList coeflist;
  Canvas->Clear();
  Canvas->Divide(2,3);
  for(Int_t j=0;j<NXT;j++){    
    pdflist.add(*SigPdf[j]);
    if(j<NXT-1){
      //nXTCoef[j]=new RooRealVar(TString("nXTCoef")+(long)j,TString("nXTCoef")+(long)j,.6*truew[j],1.4*truew[j]);    
      nXTCoef[j]=new RooRealVar(TString("nXTCoef")+(long)j,TString("nXTCoef")+(long)j,.01,.5);          
      nXTCoef[j]->setVal(truew[j]);  
      //nXTCoef[j]->setConstant(1);      
    }

    Canvas->cd(j+1);
    HSigPdfHisto[j]=new TH2F(TString("SigPdfn_{X}^{T}=")+(long)j,"",NXR,-.5,NXR-.5,NDsMassBins,DsMassMin,DsMassMax);
    DsMassDataSetSig[j]->fillHistogram(HSigPdfHisto[j],RooArgList(nxr,mass));
    HSigPdfHisto[j]->SetTitle(TString("n_{X}^{T}=")+(long)j);
    HSigPdfHisto[j]->Draw("lego");      
  }    
  for(Int_t i=0;i<NXR;i++) delete HSigPdfHisto[i];
  Canvas->Print(filename); 
  for(Int_t j=0;j<NXT-1;j++){ 
      coeflist.add(*nXTCoef[j]);    
  }
 

  //construct the total signal pdf out of the addition of each nXT pdf
  DRecoAddPdf TotalSigPdf("TotalSigPdf","TotalSigPdf",nset,pdflist,coeflist);
  Canvas->Clear();
  TH2F* TotalSigPdfHisto=(TH2F*)TotalSigPdf.CreateHistogram("TotalSigPdfHisto");
  TotalSigPdfHisto->Draw("lego");
  Canvas->Print(filename);  
 
  //
  DRecoAddPdf TotPdf("TotPdf","TotPdf",nset,TotalSigPdf,TotRSBkgPdf,TotYield);
  Canvas->Clear();
  TH2F* TotPdfHisto=(TH2F*)TotPdf.CreateHistogram("TotPdfHisto");
  TotPdfHisto->Draw("lego");
  Canvas->Print(filename);


  //Get the Data histograms
  RooDataHist DsMassDataSet(HDsMassData->GetName(),HDsMassData->GetName(),nset,HDsMassData);
  Canvas->Clear();
  HDsMassData->Draw("lego");
  Canvas->Print(filename);
  Canvas->Print(filename+"]");

  ///------------------------------------------------------
  //Fit
  //----------------------------------------------------

  //create a simultaneous chisquare
  TList histlist;
  histlist.Add(HDsMassDataWS);
  histlist.Add(HDsMassData);

  TList pdlist;
  pdlist.Add(&TotPdfWS);
  pdlist.Add(&TotPdf);

  TList varlist;
  varlist.Add(&nxr);
  varlist.Add(&mass);
  varlist.Add(&nxr);
  varlist.Add(&mass);

  RooArgSet paramset;
  paramset.add(nxr);
  paramset.add(mass);
  paramset.add(TotWSFracYield);
  paramset.add(TotYieldWS);
  paramset.add(TotYield);
  for(Int_t i=0;i<NXR-1;i++)
    paramset.add(*BkgnXRYield[i]);

  for(Int_t j=0;j<NXT-1;j++){
    paramset.add(*nXTCoefWS[j]);
    paramset.add(*nXTCoef[j]);
  }

  
  RooGenericPdf dummypdf("dummypdf","dummypdf","mass+nxr",paramset);
  DRecoSimulChi2Var chi2("chi2","chi2",dummypdf,DsMassDataSetWS,&histlist,&pdlist,&varlist,RooFit::NumCPU(2));
  //times for ncpu: 1=490s ,2= ,4= ,5=152s in kbar.  8 cpu doesnt work
  RooMinuit minuit(chi2);

  //minimize
  ULong_t starttime=(ULong_t)(gSystem->Now());//time the fit
  //minuit.simplex();//does not improve fit time
  minuit.migrad();
  ULong_t time=(ULong_t)(gSystem->Now())-starttime;
  cout<<"The fit time is "<<time/1000<<" seconds"<<endl;

  RooFitResult* result=minuit.save("fitresult","fitresult");
  Int_t fitstatus=result->status();
  Int_t covqual=result->covQual();
  Float_t chisq=chi2.getVal();
  cout<<"Chisquare="<<chisq<<" fit status="<<fitstatus<<"  CovQual="<<covqual<<endl;




  //--------------------------------------------------------------------
  //Plot fits
  //--------------------------------------------------------------------

  //WS 2D plots 
  TH2F* TotPdfHistoFitWS=(TH2F*)TotPdfWS.CreateHistogram("TotPdfHistoFitWS");
  TotPdfHistoFitWS->Scale(HDsMassDataWS->Integral()/TotPdfHistoFitWS->Integral());

  TH2F* TotBkgPdfHistoFitWS=(TH2F*)TotWSBkgPdf.CreateHistogram("TotBkgPdfHistoFitWS");
  TotBkgPdfHistoFitWS->Scale((1-TotYieldWS.getVal())*HDsMassDataWS->Integral()/TotBkgPdfHistoFitWS->Integral());

  TH2F* TotSigWSPdfHistoFit=(TH2F*)TotalSigWSPdf.CreateHistogram("TotSigWSPdfHistoFit");
  TotSigWSPdfHistoFit->Scale(TotYieldWS.getVal()*HDsMassDataWS->Integral()/TotSigWSPdfHistoFit->Integral());
  TotSigWSPdfHistoFit->SetBinContent(0,TotYieldWS.getVal()*HDsMassDataWS->Integral());
  //TotSigWSPdfHistoFit->SetBinError(0,TotYieldWS.getError()*HDsMassDataWS->Integral());

  //RS 2D plots
  TH2F* TotPdfHistoFit=(TH2F*)TotPdf.CreateHistogram("TotPdfHistoFit");
  TotPdfHistoFit->Scale(HDsMassData->Integral()/TotPdfHistoFit->Integral());

  TH2F* TotBkgPdfHistoFit=(TH2F*)TotRSBkgPdf.CreateHistogram("TotBkgPdfHistoFit");
  TotBkgPdfHistoFit->Scale((1-TotYield.getVal())*HDsMassData->Integral()/TotBkgPdfHistoFit->Integral());

  TH2F* TotSigPdfHistoFit=(TH2F*)TotalSigPdf.CreateHistogram("TotSigPdfHistoFit");
  TotSigPdfHistoFit->Scale(TotYield.getVal()*HDsMassData->Integral()/TotSigPdfHistoFit->Integral());
  TotSigPdfHistoFit->SetBinContent(0,TotYield.getVal()*HDsMassData->Integral());
  TotSigPdfHistoFit->SetBinError(0,TotYield.getError()*HDsMassData->Integral());  

  TH1F HnXTweightsWS("HnXTweightsWS","",NXT,-.5,NXT-.5);
  TH1F HnXTweightsWSTrue("HnXTweightsWSTrue","",NXT,-.5,NXT-.5); 
  TH1F HnXTweights("HnXTweights","",NXT,-.5,NXT-.5);
  TH1F HnXTweightsTrue("HnXTweightsTrue","",NXT,-.5,NXT-.5);
 

  //compute the last weight
  Float_t lastwTrueWS=1.;
  Float_t lastwWS=1.;
  Float_t lastwerWS=0.;
  Float_t lastwTrue=1.;
  Float_t lastw=1.;
  Float_t lastwer=0.;
  //print out the nXT weight coefficients:
  for(Int_t j=0;j<NXT-1;j++){ 
    cout<<"nXT="<<j<<"  true_w_ws="<<truewws[j]<<"  fit_w_ws="<<nXTCoefWS[j]->getVal()<<"+-"<<nXTCoefWS[j]->getError()<<endl;    
    HnXTweightsWSTrue.SetBinContent(j+1,truewws[j]);
    HnXTweightsWSTrue.SetBinError(j+1,0);
    HnXTweightsWS.SetBinContent(j+1,nXTCoefWS[j]->getVal());
    HnXTweightsWS.SetBinError(j+1,nXTCoefWS[j]->getError());

    lastwTrueWS-=truewws[j];
    lastwWS-=nXTCoefWS[j]->getVal();
    lastwerWS+=nXTCoefWS[j]->getError()*nXTCoefWS[j]->getError();

  }
  for(Int_t j=0;j<NXT-1;j++){ 
    cout<<"nXT="<<j<<"  true_w="<<truew[j]<<"  fit_w="<<nXTCoef[j]->getVal()<<"+-"<<nXTCoef[j]->getError()<<endl;    
    HnXTweightsTrue.SetBinContent(j+1,truew[j]);
    HnXTweightsTrue.SetBinError(j+1,0);
    HnXTweights.SetBinContent(j+1,nXTCoef[j]->getVal());
    HnXTweights.SetBinError(j+1,nXTCoef[j]->getError());
    lastw-=nXTCoef[j]->getVal();
    lastwer+=nXTCoef[j]->getError()*nXTCoef[j]->getError();
    lastwTrue-=truew[j];

  } 


  HnXTweightsWSTrue.SetBinContent(NXT,lastwTrueWS);
  HnXTweightsWS.SetBinContent(NXT,lastwWS);
  HnXTweightsWS.SetBinError(NXT,sqrt(lastwerWS));
  HnXTweightsTrue.SetBinContent(NXT,lastwTrue);
  HnXTweights.SetBinContent(NXT,lastw);
  HnXTweights.SetBinError(NXT,sqrt(lastwer));


  
  //save results into a root file for fast plotting
  TFile FResults(_OutputDir+"/"+resultFileName+".root","recreate");
  HDsMassDataWS->SetName("HDsMassDataWS");HDsMassDataWS->Write();
  HDsMassData->SetName("HDsMassData");HDsMassData->Write();
  TotPdfHistoFitWS->SetName("TotPdfHistoFitWS");TotPdfHistoFitWS->Write();
  TotBkgPdfHistoFitWS->SetName("TotBkgPdfHistoFitWS");TotBkgPdfHistoFitWS->Write();
  TotSigWSPdfHistoFit->SetName("TotSigWSPdfHistoFit");TotSigWSPdfHistoFit->Write();
  TotPdfHistoFit->SetName("TotPdfHistoFit");TotPdfHistoFit->Write();
  TotBkgPdfHistoFit->SetName("TotBkgPdfHistoFit");TotBkgPdfHistoFit->Write();
  TotSigPdfHistoFit->SetName("TotSigPdfHistoFit");TotSigPdfHistoFit->Write();
  HnXTweightsWSTrue.SetName("HnXTweightsWSTrue");HnXTweightsWSTrue.Write();
  HnXTweightsWS.SetName("HnXTweightsWS");HnXTweightsWS.Write();
  HnXTweightsTrue.SetName("HnXTweightsTrue");HnXTweightsTrue.Write();
  HnXTweights.SetName("HnXTweights");HnXTweights.Write();
  result->Write();
  FResults.Close();


}

void DRecoDsInclusiveAnalysis::RooFitData(){
  
  TFile DataFileWS(_OutputDir+"/../FinalWS/HistosForFit.root","read");
  gROOT->cd();
  TH2F* HDsMassDataWS=(TH2F*)DataFileWS.Get("HDsMass2D");  
  if(!HDsMassDataWS)return;

  TFile DataFile(_OutputDir+"/HistosForFit.root","read");
  gROOT->cd();
  TH2F* HDsMassData=(TH2F*)DataFile.Get(TString("HDsMass2D"));
  if(!HDsMassData)return;

  //RooFitDsMassvsnXRvsnXTSimul(HDsMassDataWS,HDsMassData,"FitResult");
  //RooFitDsMassvsnXRvsnXTSimulWeightModel(HDsMassDataWS,HDsMassData,"FitResult");
  //RooFitDsMassvsnXRvsnXTSimulSemiWeightModel(HDsMassDataWS,HDsMassData,"FitResult");
  //RooFitDsMassvsnXRvsnXTSimulSigmaWeightModel(HDsMassDataWS,HDsMassData,"FitResult");
  RooFitDsMassvsnXRvsnXTSimulExpModel(HDsMassDataWS,HDsMassData,"FitResult");
  
  DataFileWS.Close();
  DataFile.Close();
}



void DRecoDsInclusiveAnalysis::RooFitDsMassvsnXRvsnXTSimulSigmaWeightModel(TH2F*HDsMassDataWS,TH2F*HDsMassData,TString resultFileName){


  filename=_OutputDir+"/"+resultFileName+".ps";
  Canvas->Clear();
  Canvas->Print(filename+"["); 
      

  ///Get the WS signal Pdfs from the signal MC 
  RooArgList pdflistws;
  RooArgList coeflistWS;
  TH2F* SigWSPdfHisto[NXT];
  Canvas->Clear();
  Canvas->Divide(2,4);
  for(Int_t j=0;j<NXT;j++){    

    pdflistws.add(*SigPdfWS[j]);      
    if(j<NXT-1)coeflistWS.add(*nXTCoefWS[j]);
    
    Canvas->cd(j+1);
    SigWSPdfHisto[j]=new TH2F(TString("SigPdfWSn_{X}^{T}=")+(long)j,"",NXR,-.5,NXR-.5,NDsMassBins,DsMassMin,DsMassMax);
    SigPdfWS[j]->fillHistogram(SigWSPdfHisto[j],RooArgList(nxr,mass));
    SigWSPdfHisto[j]->Scale(1./SigWSPdfHisto[j]->Integral());
    Set2DHistXYTitles(SigWSPdfHisto[j]);
    SigWSPdfHisto[j]->SetTitle(TString("n_{X}^{T}=")+(long)j);
    SigWSPdfHisto[j]->Draw("lego");
  }    
  Canvas->Print(filename); 
  for(Int_t j=0;j<NXT;j++)  delete SigWSPdfHisto[j];

  //construct the total WS signal pdf out of the addition of each nXT pdf
  DRecoAddPdf TotalSigWSPdf("TotalSigWSPdf","TotalSigWSPdf",nset,pdflistws,coeflistWS);
  Canvas->Clear();
  TH2F* TotalSigWSPdfHisto=(TH2F*)TotalSigWSPdf.CreateHistogram("TotalSigWSPdfHisto");
  Set2DHistXYTitles(TotalSigWSPdfHisto);
  TotalSigWSPdfHisto->Draw("lego");
  Canvas->Print(filename);

  ////////////////Do not delete!!!!!!!!!!!!!!1
  /////////Background Model Systematic modify the WS by the difference in shape between the True WS Bkg and the True RS Bkg
  if(abs(_SystematicId)==5){
    TFile FWSRSBkgDiff("DsTolnu/SP1005Skim/DsReco/FinalWS/WSnXRBkgCorr.root");
    for(Int_t i=1;i<NXR;i++){
      TH1F*HDiff=(TH1F*)FWSRSBkgDiff.Get(TString("HWSBkgCorr")+(long)(i));
      cout<<"WARNING: Background Model Systematic turned on !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
      for(Int_t b=1;b<=NDsMassBins;b++){
	if(HDiff->GetBinContent(b)>0){
	  if(_SystematicId>0)HDsMassDataWS->SetBinContent(i+1,b,HDsMassDataWS->GetBinContent(i+1,b)*HDiff->GetBinContent(b));
	  if(_SystematicId<0)HDsMassDataWS->SetBinContent(i+1,b,HDsMassDataWS->GetBinContent(i+1,b)/HDiff->GetBinContent(b));
	}
      }
    }
  }
  

  //Create the WS Dataset to be fited
  RooDataHist DsMassDataSetWS("DsMassDataSetWS","DsMassDataSetWS",nset,HDsMassDataWS);
  Canvas->Clear();
  Set2DHistXYTitles(HDsMassDataWS);
  HDsMassDataWS->Draw("lego");
  Canvas->Print(filename);
  
  RooRealVar RSToWSRatio("RSToWSRatio","RSToWSRatio",HDsMassData->Integral()/HDsMassDataWS->Integral());
  RooFormulaVar TotYieldWS("TotYieldWS","TotYieldWS","WStoRSFrac*TotYield*RSToWSRatio",RooArgSet(TotYield,RSToWSRatio,WStoRSFrac));
  //RooRealVar TotYieldWS("TotYieldWS","TotYieldWS",.04,.25);

  //create a 2D background pdf from the WS Data - WS signal pdf
  DRecoHistPdf DsMassWSPdf("DsMassWSPdf","DsMassWSPdf",nset,DsMassDataSetWS);
  RooFormulaVar TotWSFracYield("TotWSFracYield","TotWSFracYield","-(1./(1.-TotYieldWS)-1.)",RooArgSet(TotYieldWS));
  DRecoAddPdf TotWSBkgPdf("TotWSBkgPdf","TotWSBkgPdf",nset,TotalSigWSPdf,DsMassWSPdf,TotWSFracYield);
  Canvas->Clear();
  TH2F* TotWSBkgPdfHisto=(TH2F*)TotWSBkgPdf.CreateHistogram("TotWSBkgPdfHisto");
  Set2DHistXYTitles(TotWSBkgPdfHisto);
  TotWSBkgPdfHisto->Draw("lego");
  Canvas->Print(filename);

  //create total WS pdf
  DRecoAddPdf TotPdfWS("TotPdfWS","TotPdfWS",nset,TotalSigWSPdf,TotWSBkgPdf,TotYieldWS);
  Canvas->Clear();
  TH2F* TotPdfWSHisto=(TH2F*)TotPdfWS.CreateHistogram("TotPdfWSHisto");
  Set2DHistXYTitles(TotPdfWSHisto);
  TotPdfWSHisto->Draw("lego");
  Canvas->Print(filename);

  
  //----------------
  //RS background model
  //---------------------
  /////The background yield for each nXR will need to be floated so we need to create a 1D background pdf for each nXR
  
  for(Int_t i=1;i<NXR;i++){//no data for nXR=0 
    WSBkgnXRPdfRaw[i]=new DRecoProdPdf(TString("WSBkgnXRPdfRaw")+(long)i,TString("WSBkgnXRPdfRaw")+(long)i,nset,*WSBkgnXRSelectPdf[i],TotWSBkgPdf);
  
    //Translation pdfs for nXR=0
    delta[i]=new RooRealVar(TString("delta")+(long)i,TString("delta")+(long)i,i);
    WSBkgnXRTransPdf[i]=new DRecoTranslationPdf(TString("WSBkgnXRTransPdf")+(long)i,TString("WSBkgnXRTransPdf")+(long)i,
						nset,*WSBkgnXRPdfRaw[i],nxr,*delta[i]);
    bkgaddcoef[i]=new RooRealVar(TString("bkgaddcoef")+(long)i,"",1./3.);

    //Translation pdfs for nXR=1
    delta1[i]=new RooRealVar(TString("delta1")+(long)i,TString("delta1")+(long)i,i-1);
    WSBkgnXR1TransPdf[i]=new DRecoTranslationPdf(TString("WSBkgnXR1TransPdf")+(long)i,TString("WSBkgnXR1TransPdf")+(long)i,
						 nset,*WSBkgnXRPdfRaw[i],nxr,*delta1[i]);      
    bkgaddcoef1[i]=new RooRealVar(TString("bkgaddcoef1")+(long)i,"",1./3.);
  }
  //create a bkg pdf for nXR=0
  DRecoAddPdf WSBkgPdfnXR0AvgRaw("WSBkgPdfnXR0AvgRaw","WSBkgPdfnXR0AvgRaw",nset,
				 RooArgList(*WSBkgnXRTransPdf[1],*WSBkgnXRTransPdf[2],*WSBkgnXRTransPdf[3]),
				 RooArgList(*bkgaddcoef[1],*bkgaddcoef[2]));
  WSBkgnXRPdf[0]=new DRecoProdPdf("WSBkgnXRPdf0","WSBkgnXRPdf0",nset,*WSnXR0CorrPdf,WSBkgPdfnXR0AvgRaw);
  
  //create a bkg pdf for nXR=1
  DRecoAddPdf WSBkgPdfnXR1AvgRaw("WSBkgPdfnXR1AvgRaw","WSBkgPdfnXR1AvgRaw",nset,
				 RooArgList(*WSBkgnXR1TransPdf[1],*WSBkgnXR1TransPdf[2],*WSBkgnXR1TransPdf[3]),
				 RooArgList(*bkgaddcoef1[1],*bkgaddcoef1[2]));
  WSBkgnXRPdf[1]=new DRecoProdPdf("WSBkgnXRPdf1","WSBkgnXRPdf1",nset,*WSnXR1CorrPdf,WSBkgPdfnXR1AvgRaw);
                                   
  //no correction is necessary for nXR=2,3 
  WSBkgnXRPdf[2]=WSBkgnXRPdfRaw[2];
  WSBkgnXRPdf[3]=WSBkgnXRPdfRaw[3];

  

  //----------------------------------------------------------------------------
  //add all the nXR backgrounds to create the full 2D RS background
  //-----------------------------------------------------------------------------  
  RooArgList rsbkgyieldlist;
  RooArgList rsbkgpdflist;
  TH2F* WSnxRHisto[NXR];
  Canvas->Clear();
  Canvas->Divide(2,2);
  for(Int_t i=0;i<NXR;i++){
    rsbkgpdflist.add(*WSBkgnXRPdf[i]);
    BkgnXRYield[i]->setVal(truebkgw[i]);
    //BkgnXRYield[i]->setConstant(1);
    if(i<NXR-1) rsbkgyieldlist.add(*BkgnXRYield[i]);

    Canvas->cd(i+1);    
    //show the individual nXR backgrounds
    WSnxRHisto[i]=(TH2F*)WSBkgnXRPdf[i]->CreateHistogram(TString("WSnxRHisto")+(long)i);
    Set2DHistXYTitles(WSnxRHisto[i]);
    WSnxRHisto[i]->SetTitle(TString("n_{X}^{R}=")+(long)i);
    WSnxRHisto[i]->Draw("lego"); 
  }  
  Canvas->Print(filename);
  for(Int_t i=0;i<NXR;i++) delete WSnxRHisto[i];
   

  DRecoAddPdf TotRSBkgPdf("TotRSBkgPdf","TotRSBkgPdf",nset,rsbkgpdflist,rsbkgyieldlist);
  Canvas->Clear();
  TH2F* TotRSBkgPdfHisto=(TH2F*)TotRSBkgPdf.CreateHistogram("TotRSBkgPdfHisto");
  Set2DHistXYTitles(TotRSBkgPdfHisto);
  TotRSBkgPdfHisto->Draw("lego");
  Canvas->Print(filename);


  ///Get the signal Pdfs from the signal MC 
  //Use a weight model for the weights
  TH2F*HSigPdfHisto[NXT];
  Canvas->Clear();
  Canvas->Divide(2,4);
  for(Int_t j=0;j<NXT;j++){    
    Canvas->cd(j+1);
    HSigPdfHisto[j]=new TH2F(TString("SigPdfn_{X}^{T}=")+(long)j,"",NXR,-.5,NXR-.5,NDsMassBins,DsMassMin,DsMassMax);
    DsMassDataSetSig[j]->fillHistogram(HSigPdfHisto[j],RooArgList(nxr,mass));
    Set2DHistXYTitles(HSigPdfHisto[j]);
    HSigPdfHisto[j]->SetTitle(TString("n_{X}^{T}=")+(long)j);
    HSigPdfHisto[j]->Draw("lego"); 
  }    
  Canvas->Print(filename); 
  for(Int_t j=0;j<NXT;j++)delete HSigPdfHisto[j]; 

  RooArgList pdflist;
  RooArgList coeflist;
  for(Int_t j=0;j<3;j++){    
    pdflist.add(*SigPdf[j]);
    coeflist.add(*nXTCoef[j]);
    cout<<"independent weights: "<<j<<" "<<truew[j]<<" "<<nXTCoef[j]->getVal()<<endl;
  }  
  //RooArgList lastpdfs;//add the last pdfs into one pdf 
  //RooArgList lastcoefs;
  for(Int_t j=3;j<NXT;j++){    
    cout<<"weight model weights: "<<j<<" "<<truew[j]<<" "<<nXTCoefWM[j]->getVal()<<endl;
    pdflist.add(*SigPdf[j]);
    coeflist.add(*nXTCoefWM[j]);

    // lastpdfs.add(*SigPdf[j]);
    // if(j<NXT-1){
    //   lastcoefs.add(*nXTCoef[j]); 
    //  }    
    // cout<<"fixed weights: "<<j<<" "<<truew[j]<<" "<<nXTCoef[j]->getVal()<<endl;
  }
  //DRecoAddPdf TotalLastPdf("TotalLastPdf","TotalLastPdf",nset,lastpdfs,lastcoefs);
  //pdflist.add(TotalLastPdf);


  //construct the total signal pdf out of the addition of each nXT pdf
  DRecoAddPdf TotalSigPdf("TotalSigPdf","TotalSigPdf",nset,pdflist,coeflist);
  Canvas->Clear();
  TH2F* TotalSigPdfHisto=(TH2F*)TotalSigPdf.CreateHistogram("TotalSigPdfHisto");
  Set2DHistXYTitles(TotalSigPdfHisto);
  TotalSigPdfHisto->Draw("lego");
  Canvas->Print(filename);  
 
  //
  DRecoAddPdf TotPdf("TotPdf","TotPdf",nset,TotalSigPdf,TotRSBkgPdf,TotYield);
  Canvas->Clear();
  TH2F* TotPdfHisto=(TH2F*)TotPdf.CreateHistogram("TotPdfHisto");
  Set2DHistXYTitles(TotPdfHisto);
  TotPdfHisto->Draw("lego");
  Canvas->Print(filename);


  //Get the Data histograms
  RooDataHist DsMassDataSet(HDsMassData->GetName(),HDsMassData->GetName(),nset,HDsMassData);
  Canvas->Clear();
  Set2DHistXYTitles(HDsMassData);
  HDsMassData->Draw("lego");
  Canvas->Print(filename);
  Canvas->Print(filename+"]");

  ///------------------------------------------------------
  //Fit
  //----------------------------------------------------

  //create a simultaneous chisquare
  TList histlist;
  histlist.Add(HDsMassDataWS);
  histlist.Add(HDsMassData);

  TList pdlist;
  pdlist.Add(&TotPdfWS);
  pdlist.Add(&TotPdf);

  TList varlist;
  varlist.Add(&nxr);
  varlist.Add(&mass);
  varlist.Add(&nxr);
  varlist.Add(&mass);

  RooArgSet paramset(nxr,mass);
  //paramset.add(TotYieldWS);
  paramset.add(TotYield);
  for(Int_t i=0;i<NXR-1;i++)
    paramset.add(*BkgnXRYield[i]);
  for(Int_t j=0;j<3;j++)
    paramset.add(*nXTCoef[j]);
    
  RooGenericPdf dummypdf("dummypdf","dummypdf","mass+nxr",paramset);
  DRecoSimulChi2Var chi2("chi2","chi2",dummypdf,DsMassDataSetWS,&histlist,&pdlist,&varlist,RooFit::NumCPU(2));
  //times for ncpu: 1= , 2= ,  >2 cpu doesnt work increase chisquare
  RooMinuit minuit(chi2);

  //minimize
  ULong_t starttime=(ULong_t)(gSystem->Now());//time the fit
  //minuit.simplex();//does not improve fit time
  minuit.migrad();
  ULong_t time=(ULong_t)(gSystem->Now())-starttime;
  cout<<"The fit time is "<<time/1000<<" seconds"<<endl;

  RooFitResult* result=minuit.save("fitresult","fitresult");
  Int_t fitstatus=result->status();
  Int_t covqual=result->covQual();
  Float_t chisq=chi2.getVal();
  cout<<"Chisquare="<<chisq<<" fit status="<<fitstatus<<"  CovQual="<<covqual<<endl;

  //--------------------------------------------------------------------
  //Plot fits
  //--------------------------------------------------------------------

  //WS 2D plots 
  TH2F* TotPdfHistoFitWS=(TH2F*)TotPdfWS.CreateHistogram("TotPdfHistoFitWS");
  TotPdfHistoFitWS->Scale(HDsMassDataWS->Integral()/TotPdfHistoFitWS->Integral());

  TH2F* TotBkgPdfHistoFitWS=(TH2F*)TotWSBkgPdf.CreateHistogram("TotBkgPdfHistoFitWS");
  TotBkgPdfHistoFitWS->Scale((1-TotYieldWS.getVal())*HDsMassDataWS->Integral()/TotBkgPdfHistoFitWS->Integral());

  TH2F* TotSigWSPdfHistoFit=(TH2F*)TotalSigWSPdf.CreateHistogram("TotSigWSPdfHistoFit");
  TotSigWSPdfHistoFit->Scale(TotYieldWS.getVal()*HDsMassDataWS->Integral()/TotSigWSPdfHistoFit->Integral());
  TotSigWSPdfHistoFit->SetBinContent(0,TotYieldWS.getVal()*HDsMassDataWS->Integral());
  //TotSigWSPdfHistoFit->SetBinError(0,TotYieldWS.getError()*HDsMassDataWS->Integral());

  //RS 2D plots
  TH2F* TotPdfHistoFit=(TH2F*)TotPdf.CreateHistogram("TotPdfHistoFit");
  TotPdfHistoFit->Scale(HDsMassData->Integral()/TotPdfHistoFit->Integral());

  TH2F* TotBkgPdfHistoFit=(TH2F*)TotRSBkgPdf.CreateHistogram("TotBkgPdfHistoFit");
  TotBkgPdfHistoFit->Scale((1-TotYield.getVal())*HDsMassData->Integral()/TotBkgPdfHistoFit->Integral());

  TH2F* TotSigPdfHistoFit=(TH2F*)TotalSigPdf.CreateHistogram("TotSigPdfHistoFit");
  TotSigPdfHistoFit->Scale(TotYield.getVal()*HDsMassData->Integral()/TotSigPdfHistoFit->Integral());
  TotSigPdfHistoFit->SetBinContent(0,TotYield.getVal()*HDsMassData->Integral());
  TotSigPdfHistoFit->SetBinError(0,TotYield.getError()*HDsMassData->Integral());  

  

  //Save the Background parameters
  TH1F HBkgYieldsTrue("HBkgYieldsTrue","",NXR,-.5,NXR-.5);
  for(Int_t i=0;i<NXR;i++){
    HBkgYieldsTrue.SetBinContent(i+1,truebkgw[i]);
  }
  TH1F HBkgYields("HBkgYields","",NXR,-.5,NXR-.5);
  Float_t lastbkgweight=1;
  for(Int_t i=0;i<NXR-1;i++){
    HBkgYields.SetBinContent(i+1,BkgnXRYield[i]->getVal());
    HBkgYields.SetBinError(i+1,BkgnXRYield[i]->getError());
    lastbkgweight-=  BkgnXRYield[i]->getVal();  
  }
  HBkgYields.SetBinContent(NXR,lastbkgweight);
  

  //fill the weights
  TH1F HnXTweightsWS("HnXTweightsWS","",NXT,-.5,NXT-.5);
  TH1F HnXTweightsWSTrue("HnXTweightsWSTrue","",NXT,-.5,NXT-.5); 
  TH1F HnXTweights("HnXTweights","",NXT,-.5,NXT-.5);
  TH1F HnXTweightsTrue("HnXTweightsTrue","",NXT,-.5,NXT-.5);
  for(Int_t j=0;j<NXT;j++){     
    HnXTweightsWSTrue.SetBinContent(j+1,truewws[j]);
    HnXTweightsWSTrue.SetBinError(j+1,truewws[j]/100.);//assign small error to be able to fit later
    HnXTweightsWS.SetBinContent(j+1,nXTCoefWS[j]->getVal());
    HnXTweightsWS.SetBinError(j+1,0.);
  }
  for(Int_t j=0;j<3;j++){ 
    HnXTweightsTrue.SetBinContent(j+1,truew[j]);
    HnXTweightsTrue.SetBinError(j+1,truew[j]/100.);
    HnXTweights.SetBinContent(j+1,nXTCoef[j]->getVal());
    HnXTweights.SetBinError(j+1,nXTCoef[j]->getError()); 
  }
  for(Int_t j=3;j<NXT;j++){ 
    HnXTweightsTrue.SetBinContent(j+1,truew[j]);
    HnXTweightsTrue.SetBinError(j+1,truew[j]/100.);
    HnXTweights.SetBinContent(j+1,nXTCoefWM[j]->getVal());
    HnXTweights.SetBinError(j+1,0.); 
  }
  
  //save status and chisquare
  HnXTweights.SetBinContent(0,fitstatus);
  HnXTweights.SetBinError(0,chisq);

  //save results into a root file for fast plotting
  TFile FResults(_OutputDir+"/"+resultFileName+".root","recreate");
  HBkgYieldsTrue.Write();
  HBkgYields.Write();
  HDsMassDataWS->SetName("HDsMassDataWS");HDsMassDataWS->Write();
  HDsMassData->SetName("HDsMassData");HDsMassData->Write();
  TotPdfHistoFitWS->Write();
  TotBkgPdfHistoFitWS->Write();
  TotSigWSPdfHistoFit->Write();
  TotPdfHistoFit->Write();
  TotBkgPdfHistoFit->Write();
  TotSigPdfHistoFit->Write();
  HnXTweightsWSTrue.Write();
  HnXTweightsWS.Write();
  HnXTweightsTrue.Write();
  HnXTweights.Write();
  result->Write();
  FResults.ls();
  FResults.Close();

}


void DRecoDsInclusiveAnalysis::PlotFitDsMassvsnXRvsnXTSimul(){

  filename=_OutputDir+"/PlotFitDsMassvsnXRvsnXTSimul.ps";
  Canvas->Clear();
  Canvas->Print(filename+"["); 
  
  TFile FResults(_OutputDir+"/FitResult.root","read");
  gROOT->cd();

  //WS 2D plots 
  TH2F* HDsMassDataWS=(TH2F*)FResults.Get("HDsMassDataWS");if(!HDsMassDataWS)cout<<"No HDsMassDataWS"<<endl;
  Canvas->Clear();
  Set2DHistXYTitles(HDsMassDataWS);
  HDsMassDataWS->Draw("lego");
  Canvas->Print(filename);  

  Canvas->Clear();
  TH2F* TotPdfHistoFitWS=(TH2F*)FResults.Get("TotPdfHistoFitWS");if(!TotPdfHistoFitWS)cout<<"No TotPdfHistoFitWS"<<endl;
  Set2DHistXYTitles(TotPdfHistoFitWS);
  TotPdfHistoFitWS->Draw("lego");
  Canvas->Print(filename);

  Canvas->Clear();
  TH2F* TotBkgPdfHistoFitWS=(TH2F*)FResults.Get("TotBkgPdfHistoFitWS");if(!TotBkgPdfHistoFitWS)cout<<"No TotBkgPdfHistoFitWS"<<endl;
  Set2DHistXYTitles(TotBkgPdfHistoFitWS);
  TotBkgPdfHistoFitWS->Draw("lego");
  Canvas->Print(filename);

  Canvas->Clear();
  TH2F* TotSigWSPdfHistoFit=(TH2F*)FResults.Get("TotSigWSPdfHistoFit");if(!TotSigWSPdfHistoFit)cout<<"No TotSigWSPdfHistoFit"<<endl;
  Set2DHistXYTitles(TotSigWSPdfHistoFit);
  TotSigWSPdfHistoFit->Draw("lego");
  Canvas->Print(filename);

  //RS 2D plots
  TH2F* HDsMassData=(TH2F*)FResults.Get("HDsMassData");if(!HDsMassData)cout<<"No HDsMassData"<<endl;
  Set2DHistXYTitles(HDsMassData);
  Canvas->Clear();
  HDsMassData->Draw("lego");
  Canvas->Print(filename);  

  Canvas->Clear();
  TH2F* TotPdfHistoFit=(TH2F*)FResults.Get("TotPdfHistoFit");if(!TotPdfHistoFit)cout<<"No TotPdfHistoFit"<<endl;
  Set2DHistXYTitles(TotPdfHistoFit);
  TotPdfHistoFit->Draw("lego");
  Canvas->Print(filename);

  Canvas->Clear();
  TH2F* TotBkgPdfHistoFit=(TH2F*)FResults.Get("TotBkgPdfHistoFit");if(!TotBkgPdfHistoFit)cout<<"No TotBkgPdfHistoFit"<<endl;
  Set2DHistXYTitles(TotBkgPdfHistoFit);
  TotBkgPdfHistoFit->Draw("lego");
  Canvas->Print(filename);

  Canvas->Clear();
  TH2F* TotSigPdfHistoFit=(TH2F*)FResults.Get("TotSigPdfHistoFit");if(!TotSigPdfHistoFit)cout<<"No TotSigPdfHistoFit"<<endl;
  Set2DHistXYTitles(TotSigPdfHistoFit);
  TotSigPdfHistoFit->Draw("lego");
  Canvas->Print(filename);
  

  //collection all bin residuals 
  TH1F HWS2DResidDist("HWS2DResidDist","",20,-5,5);
  TH2F* TotPdfHistoFitWSResid=(TH2F*)HDsMassDataWS->Clone("HDsMassDataWS2DResid");
  TotPdfHistoFitWSResid->GetYaxis()->SetTitleOffset(1.);
  TotPdfHistoFitWSResid->GetXaxis()->SetTitleOffset(1.);
  TotPdfHistoFitWSResid->Add(TotPdfHistoFitWS,-1);
  for(Int_t bx=1;bx<=TotPdfHistoFitWSResid->GetNbinsX();bx++)
    for(Int_t by=1;by<=TotPdfHistoFitWSResid->GetNbinsY();by++){
      if(TotPdfHistoFitWSResid->GetBinError(bx,by)>0.){
	TotPdfHistoFitWSResid->SetBinContent(bx,by,
					     TotPdfHistoFitWSResid->GetBinContent(bx,by)/
					     TotPdfHistoFitWSResid->GetBinError(bx,by));
	HWS2DResidDist.Fill(TotPdfHistoFitWSResid->GetBinContent(bx,by));

      } else {
	TotPdfHistoFitWSResid->SetBinContent(bx,by,0);
	HWS2DResidDist.Fill(-100);
      }
    }
  Canvas->Clear();
  TotPdfHistoFitWSResid->SetStats(0);
  TotPdfHistoFitWSResid->Draw("colz");
  Canvas->Print(filename);

 

  TH1F H2DResidDist("H2DResidDist","",20,-5,5);
  TH2F* TotPdfHistoFitResid=(TH2F*)HDsMassData->Clone("HDsMassData2DResid");
  TotPdfHistoFitResid->GetYaxis()->SetTitleOffset(1.);
  TotPdfHistoFitResid->GetXaxis()->SetTitleOffset(1.);
  TotPdfHistoFitResid->Add(TotPdfHistoFit,-1);
  for(Int_t bx=1;bx<=TotPdfHistoFitResid->GetNbinsX();bx++)
    for(Int_t by=1;by<=TotPdfHistoFitResid->GetNbinsY();by++)
      if(TotPdfHistoFitResid->GetBinError(bx,by)>0.){
	TotPdfHistoFitResid->SetBinContent(bx,by,
					   TotPdfHistoFitResid->GetBinContent(bx,by)/
					   TotPdfHistoFitResid->GetBinError(bx,by));
	H2DResidDist.Fill(TotPdfHistoFitResid->GetBinContent(bx,by));
      } else { 
	TotPdfHistoFitResid->SetBinContent(bx,by,0.);
	H2DResidDist.Fill(-100);
      }
  Canvas->Clear();
  TotPdfHistoFitResid->SetStats(0);
  TotPdfHistoFitResid->GetZaxis()->SetRangeUser(-5,5);
  TotPdfHistoFitResid->Draw("colz");
  Canvas->Print(filename);


  Canvas->Clear();
  //H2DResidDist.Fit("gaus");
  H2DResidDist.Draw();
  Canvas->Print(filename);
  


  ///calculate quantities
  //Int_t sigyieldws=(int)(TotSigWSPdfHistoFit->GetBinContent(0));
  //Int_t sigyieldwserr=(int)(TotSigWSPdfHistoFit->GetBinError(0));
  Float_t sigyield=TotSigPdfHistoFit->Integral(1,4,DenominatorDsMassBinMin,DenominatorDsMassBinMax);
  Float_t sigyielderr=sigyield*TotSigPdfHistoFit->GetBinError(0)/TotSigPdfHistoFit->GetBinContent(0);
  Int_t sigyieldtotal=(int)(TotSigPdfHistoFit->GetBinContent(0));
  Int_t sigyieldtotalerr=(int)(TotSigPdfHistoFit->GetBinError(0));

  ///WS 1D fit plot
  TH1D* HDsMassDataProjWS=HDsMassDataWS->ProjectionY("HDsMassDataprojY");
  TH1D* HTotalFitWS=TotPdfHistoFitWS->ProjectionY("TotPdfHistoFitY");
  TH1D* HBkgFitWS=TotBkgPdfHistoFitWS->ProjectionY("TotBkgPdfHistoFitY");
  Canvas->Clear();
  Set1DHistXYTitles(HTotalFitWS);
  //HTotalFitWS->SetTitle(TString("Signal Yield = ")+(long)sigyieldws+" +- "+(long)sigyieldwserr);
  HTotalFitWS->GetXaxis()->SetTitleOffset(.8);
  HTotalFitWS->SetStats(0);
  HTotalFitWS->SetFillColor(2);
  HTotalFitWS->SetMarkerSize(.001);  
  HTotalFitWS->Draw("pe3 hist");  
  HBkgFitWS->SetLineColor(0);
  HBkgFitWS->SetFillColor(4);
  HBkgFitWS->Draw("same hist");  
  HDsMassDataProjWS->Draw("pe1 same");  
  Canvas->Print(filename); 

  ///RS 1D fit plot
  TH1D* HDsMassDataProj=HDsMassData->ProjectionY("HDsMassDataprojY");
  Set1DHistXYTitles(HDsMassDataProj);
  TH1D* HTotalFit=TotPdfHistoFit->ProjectionY("TotPdfHistoFitjY");
  TH1D* HBkgFit=TotBkgPdfHistoFit->ProjectionY("TotBkgPdfHistoFitY");
  DRecoResiduals resid("resid",HDsMassDataProj,HTotalFit);
  resid.FitHasError(1);  
  RooPlot* plot=mass.frame();
  HTotalFit->SetFillColor(2);
  HTotalFit->SetMarkerSize(.001);
  plot->addTH1(HTotalFit,"pe3 hist");
  HBkgFit->SetLineColor(0);
  HBkgFit->SetFillColor(4);
  plot->addTH1(HBkgFit,"same hist");
  plot->addTH1(HDsMassDataProj,"pe1 same");
  Canvas->Clear();
  plot->GetYaxis()->SetTitle(HDsMassDataProj->GetYaxis()->GetTitle());
  plot->GetXaxis()->SetTitle(HDsMassDataProj->GetXaxis()->GetTitle());
  //plot->SetTitle(TString("Signal Yield = ")+(long)sigyield+" +- "+(long)sigyielderr);
  plot->SetTitle("");
  resid.MakeFitPlotWithResiduals(Canvas,plot);
  Canvas->cd(1);
  cutline.SetLineColor(1);
  cutline.DrawLine(DenominatorDsMassMin,0,DenominatorDsMassMin,HDsMassDataProj->GetMaximum());
  cutline.DrawLine(DenominatorDsMassMax,0,DenominatorDsMassMax,HDsMassDataProj->GetMaximum());
  Canvas->Print(filename); 
  //delete plot;

  //Plot with yield in total region
  Canvas->Clear();
  //plot->SetTitle(TString("Signal Yield = ")+(long)sigyieldtotal+" +- "+(long)sigyieldtotalerr);
  plot->SetTitle("");
  resid.MakeFitPlotWithResiduals(Canvas,plot);
  Canvas->Print(filename); 
  //delete plot;


  ///RS 1D Plot without residuals for the Paper 
  plot=mass.frame();
  plot->addTH1(HTotalFit,"pe3 hist");
  plot->addTH1(HBkgFit,"same hist");
  plot->addTH1(HDsMassDataProj,"pe1 same");
  Canvas->Clear();
  plot->GetYaxis()->SetTitle(HDsMassDataProj->GetYaxis()->GetTitle());
  plot->GetXaxis()->SetTitle(HDsMassDataProj->GetXaxis()->GetTitle());
  plot->SetTitle("");
  plot->Draw();
  cutline.DrawLine(DenominatorDsMassMin,0,DenominatorDsMassMin,HDsMassDataProj->GetMaximum());
  cutline.DrawLine(DenominatorDsMassMax,0,DenominatorDsMassMax,HDsMassDataProj->GetMaximum());
  Canvas->Print(filename); 
  //delete plot;



  //WS 1D fit plot for each nXR
  TH1F* HDatanXRWS[NXR];
  TH1F* HFitnXRWS[NXR];
  TH1F* HFitnXRWSLine[NXR];
  TH1F* HBkgnXRWS[NXR];
  for(Int_t i=1;i<NXR;i++){
    HDatanXRWS[i]=new TH1F(TString("HDatanXRWS")+(long)i,"",NDsMassBins,DsMassMin,DsMassMax);    
    HFitnXRWS[i]=new TH1F(TString("HFitnXRWS")+(long)i,"",NDsMassBins,DsMassMin,DsMassMax);
    HFitnXRWS[i]->GetXaxis()->SetTitle(HDsMassDataWS->GetYaxis()->GetTitle());
    HBkgnXRWS[i]=new TH1F(TString("HBkgnXRWS")+(long)i,"",NDsMassBins,DsMassMin,DsMassMax);
    for(Int_t b=1;b<=NDsMassBins;b++){
      HDatanXRWS[i]->SetBinContent(b,HDsMassDataWS->GetBinContent(i+1,b));
      HDatanXRWS[i]->SetBinError(b,HDsMassDataWS->GetBinError(i+1,b));
      HFitnXRWS[i]->SetBinContent(b,TotPdfHistoFitWS->GetBinContent(i+1,b));
      HFitnXRWS[i]->SetBinError(b,TotPdfHistoFitWS->GetBinError(i+1,b));      
      HBkgnXRWS[i]->SetBinContent(b,TotBkgPdfHistoFitWS->GetBinContent(i+1,b));
      HBkgnXRWS[i]->SetBinError(b,TotBkgPdfHistoFitWS->GetBinError(i+1,b));
    }

    HFitnXRWSLine[i]=(TH1F*)HFitnXRWS[i]->Clone(TString("HFitnXRWSLine")+(long)i);
  }
  
  Canvas->Clear(); 
  Canvas->Divide(2,2);
  for(Int_t i=1;i<NXR;i++){
    Canvas->cd(i+1);
    HFitnXRWS[i]->SetFillColor(2);
    HFitnXRWS[i]->SetLineColor(2);
    HFitnXRWS[i]->SetLineWidth(5);
    HFitnXRWS[i]->SetMarkerSize(0.);
    HFitnXRWS[i]->SetStats(0);
    //HFitnXRWS[i]->SetTitle(TString("Signal Yield=")+(long)(HFitnXRWS[i]->Integral()-HBkgnXRWS[i]->Integral()));
    HFitnXRWS[i]->SetTitle("");
    Set1DHistXYTitles(HFitnXRWS[i]);
    HFitnXRWS[i]->Draw("pe3 hist"); 
    HBkgnXRWS[i]->SetLineColor(0);
    HBkgnXRWS[i]->SetFillColor(4);
    HBkgnXRWS[i]->Draw("same hist");  
    HDatanXRWS[i]->SetStats(0);
    HDatanXRWS[i]->Draw("pe same");
  }  
  Canvas->Print(filename); 
  for(Int_t i=1;i<NXR;i++){
    delete HDatanXRWS[i];
    delete HFitnXRWSLine[i];
    delete HFitnXRWS[i];
    delete HBkgnXRWS[i];
  }
  


  //RS 1D fit plot for each nXR
  TH1F* HDatanXR[NXR];
  TH1F* HFitnXR[NXR];
  TH1F* HFitnXRLine[NXR];
  TH1F* HBkgnXR[NXR];
  DRecoResiduals* ResidnXR[NXR];
  for(Int_t i=0;i<NXR;i++){
    HDatanXR[i]=new TH1F(TString("HDatanXR")+(long)i,"",NDsMassBins,DsMassMin,DsMassMax); 
    Set1DHistXYTitles(HDatanXR[i]);
    HFitnXR[i]=new TH1F(TString("HFitnXR")+(long)i,"",NDsMassBins,DsMassMin,DsMassMax);
    HBkgnXR[i]=new TH1F(TString("HBkgnXR")+(long)i,"",NDsMassBins,DsMassMin,DsMassMax);
    for(Int_t b=1;b<=NDsMassBins;b++){
      HDatanXR[i]->SetBinContent(b,HDsMassData->GetBinContent(i+1,b));
      HDatanXR[i]->SetBinError(b,HDsMassData->GetBinError(i+1,b));
      HFitnXR[i]->SetBinContent(b,TotPdfHistoFit->GetBinContent(i+1,b));
      HFitnXR[i]->SetBinError(b,TotPdfHistoFit->GetBinError(i+1,b));      
      HBkgnXR[i]->SetBinContent(b,TotBkgPdfHistoFit->GetBinContent(i+1,b));
      HBkgnXR[i]->SetBinError(b,TotBkgPdfHistoFit->GetBinError(i+1,b));
      //if(i==1)cout<<b<<" "<<HDsMassData->GetBinError(i+1,b)/HDsMassData->GetBinContent(i+1,b)<<" "<<TotPdfHistoFit->GetBinError(i+1,b)/TotPdfHistoFit->GetBinContent(i+1,b)<<endl;
    }
    HFitnXRLine[i]=(TH1F*)HFitnXR[i]->Clone(TString("HFitnXRLine")+(long)i);

    ResidnXR[i]=new DRecoResiduals(TString("ResidnXR")+(long)i,HDatanXR[i],HFitnXR[i]);
    ResidnXR[i]->FitHasError(1);
  }
  
  Canvas->Clear(); 
  Canvas->Divide(2,2);
  RooPlot* plotnXR[NXR];
  for(Int_t i=0;i<NXR;i++){
    TPad*pad=(TPad*)Canvas->cd(i+1);
    plotnXR[i]=mass.frame();
   
    HFitnXR[i]->SetFillColor(2);
    HFitnXR[i]->SetLineColor(2);
    HFitnXR[i]->SetLineWidth(5);
    HFitnXR[i]->SetMarkerSize(.0001);
    HFitnXR[i]->GetXaxis()->SetTitle();
    plotnXR[i]->addTH1(HFitnXR[i],"pe3 hist");

    HFitnXRLine[i]->SetLineColor(2);
    HFitnXRLine[i]->SetLineWidth(1);
    plotnXR[i]->addTH1(HFitnXRLine[i],"l same hist");

    HBkgnXR[i]->SetLineColor(0);
    HBkgnXR[i]->SetFillColor(4);
    plotnXR[i]->addTH1(HBkgnXR[i],"same hist");

    plotnXR[i]->addTH1(HDatanXR[i],"pe same");
    //Set1DHistXYTitles((TH1*)plotnXR[i]);
    plotnXR[i]->GetYaxis()->SetTitle(HDatanXR[i]->GetYaxis()->GetTitle());
    plotnXR[i]->GetXaxis()->SetTitle(HDatanXR[i]->GetXaxis()->GetTitle());
    //plotnXR[i]->SetTitle(TString("Signal Yield=")+(long)(HFitnXR[i]->Integral(DenominatorDsMassBinMin,DenominatorDsMassBinMax)-HBkgnXR[i]->Integral(DenominatorDsMassBinMin,DenominatorDsMassBinMax)));
    plotnXR[i]->SetTitle("");
    ResidnXR[i]->MakeFitPlotWithResiduals((TCanvas*)pad,plotnXR[i]);
    pad->cd(1);
    cutline.SetLineColor(1);
    cutline.DrawLine(DenominatorDsMassMin,0,DenominatorDsMassMin,HDatanXR[i]->GetMaximum());
    cutline.DrawLine(DenominatorDsMassMax,0,DenominatorDsMassMax,HDatanXR[i]->GetMaximum());
  }
  Canvas->Print(filename); 

  ///plots without residuals for the Paper
  Canvas->Clear(); 
  Canvas->Divide(2,2);
  text.SetTextSize(.1);
  for(Int_t i=0;i<NXR;i++){
    Canvas->cd(i+1);
    plotnXR[i]=mass.frame();
   
    //plotnXR[i]->addTH1(HFitnXR[i],"pe3 hist");
    plotnXR[i]->addTH1(HFitnXRLine[i],"l same hist");
    plotnXR[i]->addTH1(HBkgnXR[i],"same hist");
    plotnXR[i]->addTH1(HDatanXR[i],"pe same");
    plotnXR[i]->GetYaxis()->SetTitle(HDatanXR[i]->GetYaxis()->GetTitle());
    plotnXR[i]->GetXaxis()->SetTitle(HDatanXR[i]->GetXaxis()->GetTitle());
    plotnXR[i]->SetTitle("");
    plotnXR[i]->Draw();
    cutline.DrawLine(DenominatorDsMassMin,0,DenominatorDsMassMin,HDatanXR[i]->GetMaximum());
    cutline.DrawLine(DenominatorDsMassMax,0,DenominatorDsMassMax,HDatanXR[i]->GetMaximum());
    text.DrawLatex(1.83,HDatanXR[i]->GetMaximum()*.9,TString("n_{X}^{R}=")+(long)(i));
  }
  Canvas->Print(filename); 

  for(Int_t i=0;i<NXR;i++){
    delete HDatanXR[i];
    delete HFitnXRLine[i];
    delete HFitnXR[i];
    delete HBkgnXR[i];
    //delete plotnXR[i];
  }

  cout<<"N$_{Tot}^{RS}y^{RS}$ & "<<fixed<<setprecision(1)<<sigyieldtotal/1000.<<" $\\pm$ "<<sigyieldtotalerr/1000.<<" \\\\"<<endl;
  cout<<"N$_{Tot}^{RS}y_{cut}^{RS}$ & "<<fixed<<setprecision(1)<<sigyield/1000.<<"  $\\pm$ "<<sigyielderr/1000.<<" \\\\"<<endl;
    

  //RS weights
  TH1F* HnXTweightsTrue=(TH1F*)FResults.Get("HnXTweightsTrue");if(!HnXTweightsTrue)cout<<"No HnXTweightsTrue"<<endl;
  HnXTweightsTrue->GetXaxis()->SetTitle("n_{X}^{T}");
  TH1F* HnXTweights=(TH1F*)FResults.Get("HnXTweights");if(!HnXTweights)cout<<"No HnXTweights"<<endl;
  HnXTweights->GetXaxis()->SetTitle("n_{X}^{T}");
  HnXTweights->GetYaxis()->SetRangeUser(0,.3);
  HnXTweights->GetYaxis()->SetTitle("weight");
  HnXTweights->SetMarkerSize(2);
  HnXTweightsTrue->SetLineColor(4);
  Canvas->Clear(); 
  HnXTweights->SetStats(0);
  HnXTweights->Draw("pe hist");  
  HnXTweightsTrue->Draw("hist same");
  Canvas->Print(filename);  
  for(Int_t j=0;j<NXT;j++){
    cout<<"w$_"<<j<<"^{RS}$ & "<<fixed<<setprecision(4);
    if(_DataType==1)cout<<HnXTweightsTrue->GetBinContent(j+1)<<" & ";
    cout<<HnXTweights->GetBinContent(j+1);
    if(HnXTweights->GetBinError(j+1)>0)cout<<" $\\pm$ "<<HnXTweights->GetBinError(j+1);
    cout<<" \\\\"<<endl;
  }

  //Print out the parameters
  RooFitResult* result=(RooFitResult*)FResults.Get("fitresult");if(!result)cout<<"No fitresult"<<endl;
  //result->Print();
  nXTb=(RooRealVar*)result->floatParsFinal().find("nXTb");
  nXTg=(RooRealVar*)result->floatParsFinal().find("nXTg");
  cout<<fixed<<setprecision(2)<<"$\\beta$";
  if(_DataType==1)cout<<"& 3.49 ";
  cout<<"& "<<nXTb->getVal()<<" $\\pm$ "<<nXTb->getError()<<" \\\\"<<endl;
  cout<<"$\\gamma$";
  if(_DataType==1)cout<<"& 1.17 ";
  cout<<"& "<<nXTg->getVal()<<" $\\pm$ "<<nXTg->getError()<<" \\\\"<<endl;

  //cout the bkg yields in latex format
  TH1F*HBkgYieldsTrue=(TH1F*)FResults.Get("HBkgYieldsTrue");if(!HBkgYieldsTrue)cout<<"No HBkgYieldsTrue"<<endl;
  TH1F*HBkgYields=(TH1F*)FResults.Get("HBkgYields");if(!HBkgYields)cout<<"No HBkgYields"<<endl;
  for(Int_t i=0;i<NXR-1;i++){
    cout<<"b$_"<<i<<"$ & "<<setprecision(4)<<fixed;
    if(_DataType==1)cout<<HBkgYieldsTrue->GetBinContent(i+1)<<" & ";
    cout<<HBkgYields->GetBinContent(i+1);
    if(HBkgYields->GetBinError(i+1)>0)cout<<" $\\pm$ "<<HBkgYields->GetBinError(i+1);
    cout<<" \\\\"<<endl;
  }

  //WS weights
  TH1F* HnXTweightsWSTrue=(TH1F*)FResults.Get("HnXTweightsWSTrue");if(!HnXTweightsWSTrue)cout<<"No HnXTweightsWSTrue"<<endl;
  Canvas->Clear(); 
  HnXTweightsWSTrue->GetYaxis()->SetRangeUser(0,.4);
  HnXTweightsWSTrue->GetYaxis()->SetTitle("weight");
  HnXTweightsWSTrue->GetXaxis()->SetTitle("n_{X}^{T}");
  HnXTweightsWSTrue->SetLineColor(4);
  HnXTweightsWSTrue->Draw("hist");
  Canvas->Print(filename); 
  for(Int_t j=0;j<NXT;j++){//cout in latex format
    cout<<"w$_"<<j<<"^{WS}$ & "<<setprecision(4)<<fixed<<HnXTweightsWSTrue->GetBinContent(j+1)<<" & fixed \\\\"<<endl;  
  }

  ///fit the weights
  TF1 ExpMod("ExpMod","[0]*((x-[1])**[2])*exp(-[3]*x)",-.5,NXT-.5);
  ExpMod.SetParNames("Norm","#alpha","#beta","#gamma");
  ExpMod.SetParLimits(0,.001,1.0);
  ExpMod.SetParLimits(1,-3.,-.1);
  ExpMod.SetParLimits(2,-2.,10.);
  ExpMod.SetParLimits(3,.01,10.);
  ExpMod.SetLineColor(2);
  ExpMod.SetLineWidth(1);
  HnXTweightsTrue->Fit(&ExpMod,"Q","",-.5,NXT-.5);
  Canvas->Clear();
  HnXTweightsTrue->GetXaxis()->SetTitle("n_{X}^{T}");
  HnXTweightsTrue->GetYaxis()->SetRangeUser(0,.3);
  HnXTweightsTrue->GetYaxis()->SetTitle("weight"); 
  HnXTweightsTrue->Draw("hist");
  ExpMod.Draw("same");
  Canvas->Print(filename);



  Canvas->Print(filename+"]");

  FResults.Close();
}

void DRecoDsInclusiveAnalysis::RooFitDsMassvsnXRvsnXTSimulExpModel(TH2F*HDsMassDataWS,TH2F*HDsMassData,TString resultFileName){

  filename=_OutputDir+"/"+resultFileName+".ps";
  Canvas->Clear();
  Canvas->Print(filename+"["); 
      

  ///Get the WS signal Pdfs from the signal MC 
  RooArgList pdflistws;
  RooArgList coeflistWS;
  TH2F* SigWSPdfHisto[NXT];
  Canvas->Clear();
  Canvas->Divide(2,4);
  for(Int_t j=0;j<NXT;j++){    

    pdflistws.add(*SigPdfWS[j]);      
    if(j<NXT-1)coeflistWS.add(*nXTCoefWS[j]);
    
    Canvas->cd(j+1);
    SigWSPdfHisto[j]=new TH2F(TString("SigPdfWSn_{X}^{T}=")+(long)j,"",NXR,-.5,NXR-.5,NDsMassBins,DsMassMin,DsMassMax);
    SigPdfWS[j]->fillHistogram(SigWSPdfHisto[j],RooArgList(nxr,mass));
    SigWSPdfHisto[j]->Scale(1./SigWSPdfHisto[j]->Integral());
    Set2DHistXYTitles(SigWSPdfHisto[j]);
    SigWSPdfHisto[j]->SetTitle(TString("n_{X}^{T}=")+(long)j);
    SigWSPdfHisto[j]->Draw("lego");
  }    
  Canvas->Print(filename); 
  for(Int_t j=0;j<NXT;j++)  delete SigWSPdfHisto[j];

  //construct the total WS signal pdf out of the addition of each nXT pdf
  DRecoAddPdf TotalSigWSPdf("TotalSigWSPdf","TotalSigWSPdf",nset,pdflistws,coeflistWS);
  Canvas->Clear();
  TH2F* TotalSigWSPdfHisto=(TH2F*)TotalSigWSPdf.CreateHistogram("TotalSigWSPdfHisto");
  Set2DHistXYTitles(TotalSigWSPdfHisto);
  TotalSigWSPdfHisto->Draw("lego");
  Canvas->Print(filename);

  ////////////////Do not delete!!!!!!!!!!!!!!1
  /////////Background Model Systematic modify the WS by the difference in shape between the True WS Bkg and the True RS Bkg
  if(abs(_SystematicId)==5){
    TFile FWSRSBkgDiff("DsTolnu/SP1005Skim/DsReco/FinalWS/WSnXRBkgCorr.root");
    for(Int_t i=1;i<NXR;i++){
      TH1F*HDiff=(TH1F*)FWSRSBkgDiff.Get(TString("HWSBkgCorr")+(long)(i));
      cout<<"WARNING: Background Model Systematic turned on !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
      for(Int_t b=1;b<=NDsMassBins;b++){
	if(HDiff->GetBinContent(b)>0){
	  if(_SystematicId>0)HDsMassDataWS->SetBinContent(i+1,b,HDsMassDataWS->GetBinContent(i+1,b)*HDiff->GetBinContent(b));
	  if(_SystematicId<0)HDsMassDataWS->SetBinContent(i+1,b,HDsMassDataWS->GetBinContent(i+1,b)/HDiff->GetBinContent(b));
	}
      }
    }
  }
  

  //Create the WS Dataset to be fited
  RooDataHist DsMassDataSetWS("DsMassDataSetWS","DsMassDataSetWS",nset,HDsMassDataWS);
  Canvas->Clear();
  Set2DHistXYTitles(HDsMassDataWS);
  HDsMassDataWS->Draw("lego");
  Canvas->Print(filename);
  
  RooRealVar RSToWSRatio("RSToWSRatio","RSToWSRatio",HDsMassData->Integral()/HDsMassDataWS->Integral());
  RooFormulaVar TotYieldWS("TotYieldWS","TotYieldWS","WStoRSFrac*TotYield*RSToWSRatio",RooArgSet(TotYield,RSToWSRatio,WStoRSFrac));
  //RooRealVar TotYieldWS("TotYieldWS","TotYieldWS",.04,.25);

  //create a 2D background pdf from the WS Data - WS signal pdf
  DRecoHistPdf DsMassWSPdf("DsMassWSPdf","DsMassWSPdf",nset,DsMassDataSetWS);
  RooFormulaVar TotWSFracYield("TotWSFracYield","TotWSFracYield","-(1./(1.-TotYieldWS)-1.)",RooArgSet(TotYieldWS));
  DRecoAddPdf TotWSBkgPdf("TotWSBkgPdf","TotWSBkgPdf",nset,TotalSigWSPdf,DsMassWSPdf,TotWSFracYield);
  Canvas->Clear();
  TH2F* TotWSBkgPdfHisto=(TH2F*)TotWSBkgPdf.CreateHistogram("TotWSBkgPdfHisto");
  Set2DHistXYTitles(TotWSBkgPdfHisto);
  TotWSBkgPdfHisto->Draw("lego");
  Canvas->Print(filename);

  //create total WS pdf
  DRecoAddPdf TotPdfWS("TotPdfWS","TotPdfWS",nset,TotalSigWSPdf,TotWSBkgPdf,TotYieldWS);
  Canvas->Clear();
  TH2F* TotPdfWSHisto=(TH2F*)TotPdfWS.CreateHistogram("TotPdfWSHisto");
  Set2DHistXYTitles(TotPdfWSHisto);
  TotPdfWSHisto->Draw("lego");
  Canvas->Print(filename);

  
  //----------------
  //RS background model
  //---------------------
  /////The background yield for each nXR will need to be floated so we need to create a 1D background pdf for each nXR
  
  for(Int_t i=1;i<NXR;i++){//no data for nXR=0 
    WSBkgnXRPdfRaw[i]=new DRecoProdPdf(TString("WSBkgnXRPdfRaw")+(long)i,TString("WSBkgnXRPdfRaw")+(long)i,nset,*WSBkgnXRSelectPdf[i],TotWSBkgPdf);
  
    //Translation pdfs for nXR=0
    delta[i]=new RooRealVar(TString("delta")+(long)i,TString("delta")+(long)i,i);
    WSBkgnXRTransPdf[i]=new DRecoTranslationPdf(TString("WSBkgnXRTransPdf")+(long)i,TString("WSBkgnXRTransPdf")+(long)i,
						nset,*WSBkgnXRPdfRaw[i],nxr,*delta[i]);
    bkgaddcoef[i]=new RooRealVar(TString("bkgaddcoef")+(long)i,"",1./3.);

    //Translation pdfs for nXR=1
    delta1[i]=new RooRealVar(TString("delta1")+(long)i,TString("delta1")+(long)i,i-1);
    WSBkgnXR1TransPdf[i]=new DRecoTranslationPdf(TString("WSBkgnXR1TransPdf")+(long)i,TString("WSBkgnXR1TransPdf")+(long)i,
						 nset,*WSBkgnXRPdfRaw[i],nxr,*delta1[i]);      
    bkgaddcoef1[i]=new RooRealVar(TString("bkgaddcoef1")+(long)i,"",1./3.);
  }
  //create a bkg pdf for nXR=0
  DRecoAddPdf WSBkgPdfnXR0AvgRaw("WSBkgPdfnXR0AvgRaw","WSBkgPdfnXR0AvgRaw",nset,
				 RooArgList(*WSBkgnXRTransPdf[1],*WSBkgnXRTransPdf[2],*WSBkgnXRTransPdf[3]),
				 RooArgList(*bkgaddcoef[1],*bkgaddcoef[2]));
  WSBkgnXRPdf[0]=new DRecoProdPdf("WSBkgnXRPdf0","WSBkgnXRPdf0",nset,*WSnXR0CorrPdf,WSBkgPdfnXR0AvgRaw);
  
  //create a bkg pdf for nXR=1
  DRecoAddPdf WSBkgPdfnXR1AvgRaw("WSBkgPdfnXR1AvgRaw","WSBkgPdfnXR1AvgRaw",nset,
				 RooArgList(*WSBkgnXR1TransPdf[1],*WSBkgnXR1TransPdf[2],*WSBkgnXR1TransPdf[3]),
				 RooArgList(*bkgaddcoef1[1],*bkgaddcoef1[2]));
  WSBkgnXRPdf[1]=new DRecoProdPdf("WSBkgnXRPdf1","WSBkgnXRPdf1",nset,*WSnXR1CorrPdf,WSBkgPdfnXR1AvgRaw);
                                   
  //no correction is necessary for nXR=2,3 
  WSBkgnXRPdf[2]=WSBkgnXRPdfRaw[2];
  WSBkgnXRPdf[3]=WSBkgnXRPdfRaw[3];

  

  //----------------------------------------------------------------------------
  //add all the nXR backgrounds to create the full 2D RS background
  //-----------------------------------------------------------------------------  
  RooArgList rsbkgyieldlist;
  RooArgList rsbkgpdflist;
  TH2F* WSnxRHisto[NXR];
  Canvas->Clear();
  Canvas->Divide(2,2);
  for(Int_t i=0;i<NXR;i++){
    rsbkgpdflist.add(*WSBkgnXRPdf[i]);
    BkgnXRYield[i]->setVal(truebkgw[i]);
    //BkgnXRYield[i]->setConstant(1);
    if(i<NXR-1) rsbkgyieldlist.add(*BkgnXRYield[i]);

    Canvas->cd(i+1);    
    //show the individual nXR backgrounds
    WSnxRHisto[i]=(TH2F*)WSBkgnXRPdf[i]->CreateHistogram(TString("WSnxRHisto")+(long)i);
    Set2DHistXYTitles(WSnxRHisto[i]);
    WSnxRHisto[i]->SetTitle(TString("n_{X}^{R}=")+(long)i);
    WSnxRHisto[i]->Draw("lego"); 
  }  
  Canvas->Print(filename);
  for(Int_t i=0;i<NXR;i++) delete WSnxRHisto[i];
   

  DRecoAddPdf TotRSBkgPdf("TotRSBkgPdf","TotRSBkgPdf",nset,rsbkgpdflist,rsbkgyieldlist);
  Canvas->Clear();
  TH2F* TotRSBkgPdfHisto=(TH2F*)TotRSBkgPdf.CreateHistogram("TotRSBkgPdfHisto");
  Set2DHistXYTitles(TotRSBkgPdfHisto);
  TotRSBkgPdfHisto->Draw("lego");
  Canvas->Print(filename);


  ///Get the signal Pdfs from the signal MC 
  //Use a weight model for the weights
  TH2F*HSigPdfHisto[NXT];
  Canvas->Clear();
  Canvas->Divide(2,4);
  for(Int_t j=0;j<NXT;j++){    
    Canvas->cd(j+1);
    HSigPdfHisto[j]=new TH2F(TString("SigPdfn_{X}^{T}=")+(long)j,"",NXR,-.5,NXR-.5,NDsMassBins,DsMassMin,DsMassMax);
    DsMassDataSetSig[j]->fillHistogram(HSigPdfHisto[j],RooArgList(nxr,mass));
    Set2DHistXYTitles(HSigPdfHisto[j]);
    HSigPdfHisto[j]->SetTitle(TString("n_{X}^{T}=")+(long)j);
    HSigPdfHisto[j]->Draw("lego"); 
  }    
  Canvas->Print(filename); 
  for(Int_t j=0;j<NXT;j++)delete HSigPdfHisto[j]; 

  RooArgList pdflist;
  RooArgList coeflist;
  for(Int_t j=0;j<NXT;j++){  
    pdflist.add(*SigPdf[j]);
    if(j<NXT-1)coeflist.add(*nXTCoefWM[j]);
    cout<<j<<"  true="<<truew[j]<<" model="<<nXTCoefWM[j]->getVal()<<endl;
  }  


  //construct the total signal pdf out of the addition of each nXT pdf
  DRecoAddPdf TotalSigPdf("TotalSigPdf","TotalSigPdf",nset,pdflist,coeflist);
  Canvas->Clear();
  TH2F* TotalSigPdfHisto=(TH2F*)TotalSigPdf.CreateHistogram("TotalSigPdfHisto");
  Set2DHistXYTitles(TotalSigPdfHisto);
  TotalSigPdfHisto->Draw("lego");
  Canvas->Print(filename);  
 
  //
  DRecoAddPdf TotPdf("TotPdf","TotPdf",nset,TotalSigPdf,TotRSBkgPdf,TotYield);
  Canvas->Clear();
  TH2F* TotPdfHisto=(TH2F*)TotPdf.CreateHistogram("TotPdfHisto");
  Set2DHistXYTitles(TotPdfHisto);
  TotPdfHisto->Draw("lego");
  Canvas->Print(filename);


  //Get the Data histograms
  RooDataHist DsMassDataSet(HDsMassData->GetName(),HDsMassData->GetName(),nset,HDsMassData);
  Canvas->Clear();
  Set2DHistXYTitles(HDsMassData);
  HDsMassData->Draw("lego");
  Canvas->Print(filename);
  Canvas->Print(filename+"]");

  ///------------------------------------------------------
  //Fit
  //----------------------------------------------------

  //create a simultaneous chisquare
  TList histlist;
  histlist.Add(HDsMassDataWS);
  histlist.Add(HDsMassData);

  TList pdlist;
  pdlist.Add(&TotPdfWS);
  pdlist.Add(&TotPdf);

  TList varlist;
  varlist.Add(&nxr);
  varlist.Add(&mass);
  varlist.Add(&nxr);
  varlist.Add(&mass);

  RooArgSet paramset(nxr,mass);
  //paramset.add(TotYieldWS);
  paramset.add(TotYield);
  for(Int_t i=0;i<NXR-1;i++)
    paramset.add(*BkgnXRYield[i]);
  paramset.add(*nXTa);
  paramset.add(*nXTb);
  paramset.add(*nXTg);
    
  RooGenericPdf dummypdf("dummypdf","dummypdf","mass+nxr",paramset);
  DRecoSimulChi2Var chi2("chi2","chi2",dummypdf,DsMassDataSetWS,&histlist,&pdlist,&varlist,RooFit::NumCPU(2));
  //times for ncpu: 1= , 2= ,  >2 cpu doesnt work increase chisquare
  RooMinuit minuit(chi2);

  //minimize
  ULong_t starttime=(ULong_t)(gSystem->Now());//time the fit
  //minuit.simplex();//does not improve fit time
  minuit.migrad();
  ULong_t time=(ULong_t)(gSystem->Now())-starttime;
  cout<<"The fit time is "<<time/1000<<" seconds"<<endl;

  RooFitResult* result=minuit.save("fitresult","fitresult");
  Int_t fitstatus=result->status();
  Int_t covqual=result->covQual();
  Float_t chisq=chi2.getVal();
  cout<<"Chisquare="<<chisq<<" fit status="<<fitstatus<<"  CovQual="<<covqual<<endl;

  //--------------------------------------------------------------------
  //Plot fits
  //--------------------------------------------------------------------

  //WS 2D plots 
  TH2F* TotPdfHistoFitWS=(TH2F*)TotPdfWS.CreateHistogram("TotPdfHistoFitWS");
  TotPdfHistoFitWS->Scale(HDsMassDataWS->Integral()/TotPdfHistoFitWS->Integral());

  TH2F* TotBkgPdfHistoFitWS=(TH2F*)TotWSBkgPdf.CreateHistogram("TotBkgPdfHistoFitWS");
  TotBkgPdfHistoFitWS->Scale((1-TotYieldWS.getVal())*HDsMassDataWS->Integral()/TotBkgPdfHistoFitWS->Integral());

  TH2F* TotSigWSPdfHistoFit=(TH2F*)TotalSigWSPdf.CreateHistogram("TotSigWSPdfHistoFit");
  TotSigWSPdfHistoFit->Scale(TotYieldWS.getVal()*HDsMassDataWS->Integral()/TotSigWSPdfHistoFit->Integral());
  TotSigWSPdfHistoFit->SetBinContent(0,TotYieldWS.getVal()*HDsMassDataWS->Integral());
  //TotSigWSPdfHistoFit->SetBinError(0,TotYieldWS.getError()*HDsMassDataWS->Integral());

  //RS 2D plots
  TH2F* TotPdfHistoFit=(TH2F*)TotPdf.CreateHistogram("TotPdfHistoFit");
  TotPdfHistoFit->Scale(HDsMassData->Integral()/TotPdfHistoFit->Integral());

  TH2F* TotBkgPdfHistoFit=(TH2F*)TotRSBkgPdf.CreateHistogram("TotBkgPdfHistoFit");
  TotBkgPdfHistoFit->Scale((1-TotYield.getVal())*HDsMassData->Integral()/TotBkgPdfHistoFit->Integral());

  TH2F* TotSigPdfHistoFit=(TH2F*)TotalSigPdf.CreateHistogram("TotSigPdfHistoFit");
  TotSigPdfHistoFit->Scale(TotYield.getVal()*HDsMassData->Integral()/TotSigPdfHistoFit->Integral());
  TotSigPdfHistoFit->SetBinContent(0,TotYield.getVal()*HDsMassData->Integral());
  TotSigPdfHistoFit->SetBinError(0,TotYield.getError()*HDsMassData->Integral());  

  

  //Save the Background parameters
  TH1F HBkgYieldsTrue("HBkgYieldsTrue","",NXR,-.5,NXR-.5);
  for(Int_t i=0;i<NXR;i++){
    HBkgYieldsTrue.SetBinContent(i+1,truebkgw[i]);
  }
  TH1F HBkgYields("HBkgYields","",NXR,-.5,NXR-.5);
  Float_t lastbkgweight=1;
  for(Int_t i=0;i<NXR-1;i++){
    HBkgYields.SetBinContent(i+1,BkgnXRYield[i]->getVal());
    HBkgYields.SetBinError(i+1,BkgnXRYield[i]->getError());
    lastbkgweight-=  BkgnXRYield[i]->getVal();  
  }
  HBkgYields.SetBinContent(NXR,lastbkgweight);
  
  //
  TH1F HnXTWMPars("HnXTWMPars","",10,0,10);
  HnXTWMPars.SetBinContent(1,nXTa->getVal()); HnXTWMPars.SetBinError(1,nXTa->getError());
  HnXTWMPars.SetBinContent(2,nXTb->getVal()); HnXTWMPars.SetBinError(2,nXTb->getError());
  HnXTWMPars.SetBinContent(3,nXTg->getVal()); HnXTWMPars.SetBinError(3,nXTg->getError());
  cout<<"nXTa="<<nXTa->getVal()<<"  nXTb="<<nXTb->getVal()<<"  nXTg="<<nXTg->getVal()<<endl;

  //fill the weights
  TH1F HnXTweightsWS("HnXTweightsWS","",NXT,-.5,NXT-.5);
  TH1F HnXTweightsWSTrue("HnXTweightsWSTrue","",NXT,-.5,NXT-.5); 
  TH1F HnXTweights("HnXTweights","",NXT,-.5,NXT-.5);
  TH1F HnXTweightsTrue("HnXTweightsTrue","",NXT,-.5,NXT-.5);
  for(Int_t j=0;j<NXT;j++){     
    HnXTweightsWSTrue.SetBinContent(j+1,truewws[j]);
    HnXTweightsWSTrue.SetBinError(j+1,truewws[j]/100.);//assign small error to be able to fit later
    HnXTweightsWS.SetBinContent(j+1,nXTCoefWS[j]->getVal());
    HnXTweightsWS.SetBinError(j+1,0.);

    HnXTweightsTrue.SetBinContent(j+1,truew[j]);
    HnXTweightsTrue.SetBinError(j+1,truew[j]/100.);
    HnXTweights.SetBinContent(j+1,nXTCoefWM[j]->getVal());
    cout<<j<<"truew="<<truew[j]<<" fitw="<<nXTCoefWM[j]->getVal()<<endl;
    HnXTweights.SetBinError(j+1,0.); 
  }

  
  //save status and chisquare
  HnXTweights.SetBinContent(0,fitstatus);
  HnXTweights.SetBinError(0,chisq);

  //save results into a root file for fast plotting
  TFile FResults(_OutputDir+"/"+resultFileName+".root","recreate");
  HBkgYieldsTrue.Write();
  HBkgYields.Write();
  HDsMassDataWS->SetName("HDsMassDataWS");HDsMassDataWS->Write();
  HDsMassData->SetName("HDsMassData");HDsMassData->Write();
  TotPdfHistoFitWS->Write();
  TotBkgPdfHistoFitWS->Write();
  TotSigWSPdfHistoFit->Write();
  TotPdfHistoFit->Write();
  TotBkgPdfHistoFit->Write();
  TotSigPdfHistoFit->Write();
  HnXTweightsWSTrue.Write();
  HnXTweightsWS.Write();
  HnXTweightsTrue.Write();
  HnXTweights.Write();
  HnXTWMPars.Write();
  result->Write();

  FResults.ls();
  FResults.Close();

}


void DRecoDsInclusiveAnalysis::CreateToys(Int_t toyid){
  
  Int_t NTotEvt=(int)(1.381e6*mcscalefactor);// is the integral of the total (sig+bkg) reco MC in the fit range
  RooArgList parList;  
  Float_t range=.2;

  ///Get the WS signal Pdfs from the signal MC 
  ///create the weighting coeficients for each nXT  
  RooArgList pdflistws;
  RooArgList coeflistWS;
  for(Int_t j=0;j<NXT;j++){    
    pdflistws.add(*SigPdfWS[j]);    
    cout<<"nXTCoefWS"<<j<<" = "<<nXTCoefWS[j]->getVal()<<endl;
    if(j<NXT-1){
      coeflistWS.add(*nXTCoefWS[j]);
    }
  }

  //construct the total signal pdf out of the addition of each nXT pdf
  DRecoAddPdf TotalSigWSPdf("TotalSigWSPdf","TotalSigWSPdf",nset,pdflistws,coeflistWS);



  //Get the WSData histograms
  TFile DataFileWS("DsTolnu/SP1005Skim/DsReco/FinalWS/HistosForFit.root","read");
  gROOT->cd();
  TH2F* HDsMassDataWS=(TH2F*)DataFileWS.Get("HDsMass2D");
  RooDataHist DsMassDataSetWS("DsMassDataSetWS","DsMassDataSetWS",nset,HDsMassDataWS);
  
  //Get the Data histograms to determine the fractional total yield
  TFile DataFile("DsTolnu/SP1005Skim/DsReco/Final/HistosForFit.root","read");
  gROOT->cd();
  TH2F* HDsMassData=(TH2F*)DataFile.Get("HDsMass2D");
  TotYield.setMin((1.-range)*totalw/HDsMassData->Integral());
  TotYield.setMax((1.+range)*totalw/HDsMassData->Integral());
  TotYield.setVal(totalw/HDsMassData->Integral());
  cout<<"TotYield="<<TotYield.getVal()<<endl;
  parList.add(TotYield);

	      
  RooRealVar RSWSRatio("RSWSRatio","RSWSRatio",HDsMassData->Integral()/HDsMassDataWS->Integral());
  RooFormulaVar TotYieldWS("TotYieldWS","TotYieldWS","WStoRSFrac*TotYield*RSWSRatio",RooArgSet(TotYield,RSWSRatio,WStoRSFrac));

  //create a 2D background pdf from the WS Data - WS signal pdf
  DRecoHistPdf DsMassWSPdf("DsMassWSPdf","DsMassWSPdf",nset,DsMassDataSetWS);
  RooFormulaVar TotWSFracYield("TotWSFracYield","TotWSFracYield","-(1./(1.-TotYieldWS)-1.)",RooArgSet(TotYieldWS));
  DRecoAddPdf TotWSBkgPdf("TotWSBkgPdf","TotWSBkgPdf",nset,TotalSigWSPdf,DsMassWSPdf,TotWSFracYield);

  //create total WS pdf
  DRecoAddPdf TotPdfWS("TotPdfWS","TotPdfWS",nset,TotalSigWSPdf,TotWSBkgPdf,TotYieldWS);

  //----------------
  //RS background model
  //----------------
  //The background yield for each nXR will need to be floated so we need to create a 1D background pdf for each nXR
  for(Int_t i=1;i<NXR;i++){//no data for nXR=0 
      WSBkgnXRPdfRaw[i]=new DRecoProdPdf(TString("WSBkgnXRPdfRaw")+(long)i,TString("WSBkgnXRPdfRaw")+(long)i,nset,*WSBkgnXRSelectPdf[i],TotWSBkgPdf);
            
      //Translation pdfs for nXR=0
      delta[i]=new RooRealVar(TString("delta")+(long)i,TString("delta")+(long)i,i);
      WSBkgnXRTransPdf[i]=new DRecoTranslationPdf(TString("WSBkgnXRTransPdf")+(long)i,TString("WSBkgnXRTransPdf")+(long)i,
						  nset,*WSBkgnXRPdfRaw[i],nxr,*delta[i]);
      bkgaddcoef[i]=new RooRealVar(TString("bkgaddcoef")+(long)i,"",1./3.);

      //Translation pdfs for nXR=1
      delta1[i]=new RooRealVar(TString("delta1")+(long)i,TString("delta1")+(long)i,i-1);
      WSBkgnXR1TransPdf[i]=new DRecoTranslationPdf(TString("WSBkgnXR1TransPdf")+(long)i,TString("WSBkgnXR1TransPdf")+(long)i,
						   nset,*WSBkgnXRPdfRaw[i],nxr,*delta1[i]);      
      bkgaddcoef1[i]=new RooRealVar(TString("bkgaddcoef1")+(long)i,"",1./3.);
  }
  //create a bkg pdf for nXR=0
  DRecoAddPdf WSBkgPdfnXR0AvgRaw("WSBkgPdfnXR0AvgRaw","WSBkgPdfnXR0AvgRaw",nset,
				 RooArgList(*WSBkgnXRTransPdf[1],*WSBkgnXRTransPdf[2],*WSBkgnXRTransPdf[3]),
				 RooArgList(*bkgaddcoef[1],*bkgaddcoef[2]));
  WSBkgnXRPdf[0]=new DRecoProdPdf("WSBkgnXRPdf0","WSBkgnXRPdf0",nset,*WSnXR0CorrPdf,WSBkgPdfnXR0AvgRaw);
  
  //create a bkg pdf for nXR=1
  DRecoAddPdf WSBkgPdfnXR1AvgRaw("WSBkgPdfnXR1AvgRaw","WSBkgPdfnXR1AvgRaw",nset,
				 RooArgList(*WSBkgnXR1TransPdf[1],*WSBkgnXR1TransPdf[2],*WSBkgnXR1TransPdf[3]),
				 RooArgList(*bkgaddcoef1[1],*bkgaddcoef1[2]));
  WSBkgnXRPdf[1]=new DRecoProdPdf("WSBkgnXRPdf1","WSBkgnXRPdf1",nset,*WSnXR1CorrPdf,WSBkgPdfnXR1AvgRaw);
                                   
  //no correction is necessary for nXR=2,3 
  WSBkgnXRPdf[2]=WSBkgnXRPdfRaw[2];
  WSBkgnXRPdf[3]=WSBkgnXRPdfRaw[3];



  //----------------------------------------------------------------------------
  //add all the nXR backgrounds to create the full 2D RS background
  //-----------------------------------------------------------------------------
  RooArgList rsbkgyieldlist;
  RooArgList rsbkgpdflist;
  for(Int_t i=0;i<NXR;i++){
    rsbkgpdflist.add(*WSBkgnXRPdf[i]);
    if(i<NXR-1){
      BkgnXRYield[i]->setMin((1.-range)*truebkgw[i]);
      BkgnXRYield[i]->setMax((1.+range)*truebkgw[i]);
      BkgnXRYield[i]->setVal(truebkgw[i]);
      cout<<"BkgnXRYield"<<i<<" = "<<BkgnXRYield[i]->getVal()<<endl;
      rsbkgyieldlist.add(*BkgnXRYield[i]);
      parList.add(*BkgnXRYield[i]);
    }
  }     
  DRecoAddPdf TotRSBkgPdf("TotRSBkgPdf","TotRSBkgPdf",nset,rsbkgpdflist,rsbkgyieldlist);



  //construct the total signal pdf out of the addition of each nXT pdf
  RooArgList pdflist;
  RooArgList coeflist;
  for(Int_t j=0;j<NXT;j++){    
    pdflist.add(*SigPdf[j]);
    if(j<NXT-1)coeflist.add(*nXTCoefWM[j]); 
    cout<<"nXTCoef"<<j<<" = "<<nXTCoefWM[j]->getVal()<<endl;
  }
  DRecoAddPdf TotalSigPdf("TotalSigPdf","TotalSigPdf",nset,pdflist,coeflist);
  nXTb->setMin(.01);
  nXTb->setMax(5.0);
  parList.add(*nXTb);
  nXTg->setMin(.01);
  nXTg->setMax(2.);
  parList.add(*nXTg);

  DRecoAddPdf TotPdf("TotPdf","TotPdf",nset,TotalSigPdf,TotRSBkgPdf,TotYield);

  if(toyid>0){//Randomize the parameters if it is not toy 0 which is the nominal    
    TRandom3 rand(toyid);
    TIterator*parIter=parList.createIterator();
    RooRealVar*par;
    while((par=(RooRealVar*)parIter->Next())){
      par->setVal(par->getMin()+rand.Uniform(1)*(par->getMax()-par->getMin()));
      cout<<par->GetName()<<" randomized to value "<<par->getVal()<<endl;
    }
  }

  //make the pdf into a histogram for easy generation
  TH2F*HTotPdfWS=(TH2F*)TotPdfWS.CreateHistogram("HTotPdfWS");
  TH2F*HTotPdf=(TH2F*)TotPdf.CreateHistogram("HTotPdf");
 
  //create file with the generated dataset.
  TFile Toyfile(TString("DsTolnu/SP1005Skim/DsReco/Final/ToyMC/ToyData")+(long)toyid+".root","recreate");
  
  //save the true parameters in histogram
  TH1F HPars("HPars","",100,0,100);
  HPars.SetBinContent(1,TotYield.getVal());
  HPars.SetBinContent(2,TotYieldWS.getVal());
  HPars.SetBinContent(10,nXTa->getVal());
  HPars.SetBinContent(11,nXTb->getVal());
  HPars.SetBinContent(12,nXTg->getVal());
  for(Int_t j=0;j<NXT-1;j++){    
    HPars.SetBinContent(j+20,nXTCoefWS[j]->getVal());
  }  
  for(Int_t j=0;j<NXT;j++){    
    HPars.SetBinContent(j+30,nXTCoefWM[j]->getVal());
  }  
  for(Int_t i=0;i<NXR-1;i++)
    HPars.SetBinContent(i+40,BkgnXRYield[i]->getVal());


  HPars.Write();

  gRandom->SetSeed(toyid+10000);
  //generate WS histo
  TH2F*HWS=(TH2F*)HTotPdfWS->Clone("HWS");
  HWS->Reset();
  HWS->Sumw2();
  Double_t x,y;
  //for(Int_t e=1;e<1.7*NTotEvt*.1605;e++){//1.7 accounts for event multiplicity, .16 accounts for WS/RS ratio
  for(Int_t e=1;e<NTotEvt*.214;e++){//should not multiply by 1.7, sum of weights already assigns proper error
    HTotPdfWS->GetRandom2(x,y);
    HWS->Fill(x,y);
  }
  //HWS->Scale(1./1.7);
  HWS->SetName("HDsMassVsNXRWS");
  HWS->Write();

  //generate RS histo
  TH2F*H=(TH2F*)HTotPdf->Clone("H");
  H->Reset();
  H->Sumw2();
  //for(Int_t e=1;e<1.7*NTotEvt;e++){//1.7 accounts for event multiplicity
  for(Int_t e=1;e<NTotEvt;e++){//
    HTotPdf->GetRandom2(x,y);
    H->Fill(x,y);
  }
  //H->Scale(1./1.7);
  H->SetName("HDsMassVsNXR");  
  H->Write();

  Toyfile.ls();
  Toyfile.Close();

  delete HTotPdfWS;
  delete HTotPdf;
  
}



void DRecoDsInclusiveAnalysis::DetermineBF(){
  TString Denomfilename="/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResults/";
  if(_DataType==1)Denomfilename+="DenominatorFitResult_MC.root";
  if(_DataType==2)Denomfilename+="DenominatorFitResult_Data.root";
  TFile FDsFit(Denomfilename,"read");  
  TString filename="/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResults/";
  if(_DataType==1)filename+="MuNuFitResult_MC.root";
  //if(_DataType==2)filename+="MuNuFitResult_Data.root";
  //if(_DataType==1)filename+="NumeratorFitResults_MC.root";//has lumi weighting 
  if(_DataType==2)filename+="NumeratorFitResults_Data.root";
  TFile FNumeratorFit(filename,"read");
  TFile FDenominatorEff("/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResults/DenominatorEff.root","r");
  //TFile FNumeratorEff("/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResults/MuNuEffNumerator.root","r");
  TFile FNumeratorEff("/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResults/NumeratorEfficiencies.root","r");
  gROOT->cd();
  
  ComputeBF(&FDsFit,&FNumeratorFit,&FDenominatorEff,&FNumeratorEff);
}


Float_t DRecoDsInclusiveAnalysis::ComputeBF(TFile* FDenomFitResults,TFile* FNumFitResults,TFile* FEffDemon,TFile* FEffNum){
    
  cout<<endl;
  //------------------------------
  //muon efficiencies
  //------------------------------
  if(FEffNum->IsZombie()){cout<<"Bad FMuEff File"<<endl; return 0;} gROOT->cd();
  if(FEffDemon->IsZombie()){cout<<"Bad FDsEff File"<<endl; return 0;} gROOT->cd();

  //declare text files with produced Ds and Muon yields as a function of nXT
  ifstream DsProducedFile; 
  DsProducedFile.open("/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResults/DsProduced.txt");
  ifstream MuNuProducedFile; 
  MuNuProducedFile.open("/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResults/MuNuProduced.txt");

  Float_t MuEff[NXT];
  Float_t MuEffErr[NXT];  
  Float_t TotDsProd=0;
  Float_t TotDsRec=0;
  Float_t TotNuProd=0;
  Float_t TotNuRec=0;
  Float_t DsProd[NXT];
  Float_t NuProd[NXT];
  Float_t DsRec[NXT];
  Float_t NuRec[NXT];
  Float_t DsRecErr[NXT];
  Float_t NuRecErr[NXT];


  //
  for(Int_t j=0;j<NXT;j++){
    //Generated Ds
    Int_t indexcheckn;
    Float_t NProdDs=0.;
    DsProducedFile>>indexcheckn>>NProdDs;   
    if(indexcheckn!=j){cout<<" NXT index does not match for Ds produced   j="<<j<<endl;exit(0);}
    DsProd[j]=NProdDs;
    TotDsProd+=NProdDs;  

    //Reconstructed Ds
    TH2F*HDsEff=(TH2F*)FEffDemon->Get(TString("HDsMassTrue2D_nXT")+(long)j);
    if(!HDsEff){cout<<"No HDsEff"<<j<<endl;return 0;}
    DsRec[j]=HDsEff->Integral(1,4,DenominatorDsMassBinMin,DenominatorDsMassBinMax);
    DsRecErr[j]=sqrt(HDsEff->Integral(1,4,DenominatorDsMassBinMin,DenominatorDsMassBinMax));
    TotDsRec+=HDsEff->Integral(1,4,DenominatorDsMassBinMin,DenominatorDsMassBinMax);

    //read in the total munu produced 
    Float_t NProdMuNu=0.;
    MuNuProducedFile>>indexcheckn>>NProdMuNu;   
    if(indexcheckn!=j){cout<<"j="<<j<<endl;exit(0);}
    TotNuProd+=NProdMuNu;             

    
    //Reconstructed MuNu
    //TH1F*HMuEff=(TH1F*)FEffNum->Get(TString("hNXT")+(long)j+"DsToMuNu_EExtraCut");
    TH1F*HMuEff=(TH1F*)FEffNum->Get(TString("DsToMuNuNXT")+(long)j);
    if(!HMuEff){cout<<"No HMuEff"<<j<<endl;return 0;}
    NuRec[j]=HMuEff->Integral();
    NuRecErr[j]=sqrt(HMuEff->Integral());
    TotNuRec+=HMuEff->Integral();
  }
     
  //determine nXT gen weights
  Float_t GenNXTWeights[NXT];
  for(Int_t j=0;j<NXT;j++){
    GenNXTWeights[j]=DsProd[j]/TotDsProd;
    //cout<<","<<GenNXTWeights[j]<<endl; 
  }
  cout<<endl;

  //determine the distribution of MuNu generated
  for(Int_t j=0;j<NXT;j++){ 
    NuProd[j]=TotNuProd*GenNXTWeights[j];
  }
  

  cout<<" nXT efficiencies: "<<endl;
  for(Int_t j=0;j<NXT;j++){ 
    MuEff[j]=(NuRec[j]*DsProd[j]/(NuProd[j]*DsRec[j]));
    MuEffErr[j]=MuEff[j]*sqrt(pow(NuRecErr[j]/NuRec[j],2) + pow(DsRecErr[j]/DsRec[j],2));
    cout<<setprecision(3)<<fixed<<j<<" & "<<MuEff[j]<<" $\\pm$ "<<MuEffErr[j]<<" \\\\"<<endl;
  }
    
  cout<<" \\\\"<<endl;
  cout<<scientific<<"The Ds efficiency is "<<TotDsRec/TotDsProd<<endl;
  cout<<scientific<<"The MuNu efficiency is "<<TotNuRec/TotNuProd<<endl;
  

  /////True MC Reco Weights ------------------------------
  Float_t totMCweight=0.;
  for(Int_t j=0;j<NXT;j++){
    totMCweight+= truew[j];
    cout<<" MCweight"<<j<<"="<<truew[j]<<endl;
  }
  if(fabs(totMCweight-1.)>1e-5){cout<<" MC weights do not add up to 1."<<endl; return 0;}
  Float_t TotMuEff=0.;
  for(Int_t j=0;j<NXT;j++){  
    TotMuEff+= truew[j]*MuEff[j];
  }
  cout<<"The MC true weighted muon efficiency is "<<TotMuEff<<endl;

  cout<<endl;
  //------------------------------
  ///Ds yield 
  //------------------------------
  if(FDenomFitResults->IsZombie()){cout<<"Bad DsFit File"<<endl; return 0;} gROOT->cd();
  RooFitResult* DsFitResult=(RooFitResult*)FDenomFitResults->Get("fitresult");
  if(!DsFitResult){cout<<"No DsFitResult"<<endl;return 0;}
  TH2F*HDsMassData=(TH2F*)FDenomFitResults->Get("HDsMassData");
  if(!HDsMassData){cout<<"No HDsMassData "<<endl;return 0;}
  RooRealVar*DsYield=(RooRealVar*)DsFitResult->floatParsFinal().find("TotYield");
  if(!DsYield){cout<<"DsYield not found"<<endl;return 0;}
  TotYield.setVal(DsYield->getVal()); 
  TotYield.setError(DsYield->getError());
  cout<<"The fitted Ds yield fraction is "<<TotYield.getVal()<<" +- "<<TotYield.getError()<<" --> "<<TotYield.getError()*100./TotYield.getVal()<<"%"<<endl;  
  cout<<endl;
  
  //for MC lumi weight the Ds yield because Numerator is lumi weighted 
//   if(_DataType==1){    
// //     NRecoDs=NRecoDs*(1./2.04+.0974/1.93)/(1+.0974);
// //     NRecoDsErr=NRecoDsErr*(1./2.04+.0974/1.93)/(1+.0974);
//     TotYield.setVal(DsYield->getVal()*(1./2.04+.0974/1.93)/(1+.0974)); 
//     TotYield.setError(DsYield->getError()*(1./2.04+.0974/1.93)/(1+.0974));
// cout<<"The total Ds yield after Lumi Weighting is "<<TotYield.getVal()<<" +- "<<TotYield.getError()<<" --> "<<TotYield.getError()*100./TotYield.getVal()<<"%"<<endl;    
//  cout<<endl;
//   }

 

  TH2F* TotSigPdfHistoFit=(TH2F*)FDenomFitResults->Get("TotSigPdfHistoFit"); if(!TotSigPdfHistoFit)cout<<"No TotSigPdfHistoFit"<<endl;
  Float_t NRecoDs=HDsMassData->Integral()*TotYield.getVal()
    *TotSigPdfHistoFit->Integral(1,4,DenominatorDsMassBinMin,DenominatorDsMassBinMax)/TotSigPdfHistoFit->Integral();
  Float_t NRecoDsErr=HDsMassData->Integral()*TotYield.getError()
    *TotSigPdfHistoFit->Integral(1,4,DenominatorDsMassBinMin,DenominatorDsMassBinMax)/TotSigPdfHistoFit->Integral();

//   //use true Ds yield  
//   TFile FDenomTrue("/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResults/DenominatorEff.root","read");
//   NRecoDs=((TH1F*)FDenomTrue.Get("HDsMassTrue"))->Integral(DenominatorDsMassBinMin,DenominatorDsMassBinMax);   cout<<"!!!!!!!!!!!Using true Ds Yield!!!!!!!!!!!!!"<<endl;

  cout<<"The total Ds yield is "<<NRecoDs<<" +- "<<NRecoDsErr<<" --> "<<NRecoDsErr*100./NRecoDs<<"%"<<endl;  
  cout<<endl;


  //------------------------------
  //Fitted nXT  weights
  //------------------------------  
  //RooRealVar* nXTaFit=NULL;
  RooRealVar* nXTbFit=NULL;
  RooRealVar* nXTgFit=NULL;
  TH1F*HnXTWMPars=(TH1F*)FDenomFitResults->Get("HnXTWMPars");
  //if(!HnXTWMPars){cout<<"HnXTWMPars not found"<<endl;return 0;}
  if(HnXTWMPars)//value of nXTa was changed from its  nominal value --> must be set properly here 
    nXTa->setVal(HnXTWMPars->GetBinContent(1));
  //nXTaFit=((RooRealVar*)DsFitResult->floatParsFinal().find("nXTa"));
  //if(!nXTaFit){cout<<"nXT pars  not found"<<endl;return 0;}
  nXTbFit=((RooRealVar*)DsFitResult->floatParsFinal().find("nXTb"));
  if(!nXTbFit){cout<<"nXTb par not found"<<endl;return 0;}
  nXTb->setVal(nXTbFit->getVal());
  nXTb->setError(nXTbFit->getError());
  nXTgFit=((RooRealVar*)DsFitResult->floatParsFinal().find("nXTg"));
  if(!nXTgFit){cout<<"nXTg par not found"<<endl;return 0;}
  nXTg->setVal(nXTgFit->getVal());
  nXTg->setError(nXTgFit->getError());
  
  //cout<<setprecision(6)<<"nXTb="<<nXTb->getVal()<<"+-"<<nXTb->getError()<<"    nXTg="<<nXTg->getVal()<<"+-"<<nXTg->getError()<<endl;

  TH1F* HnXTWeights=(TH1F*)FDenomFitResults->Get("HnXTweights");
  Float_t totDataweight=0.;
  for(Int_t j=0;j<NXT;j++){
    totDataweight+=nXTCoefWM[j]->getVal();//note that these coefficients are being updated as long as the first three are updated
    cout<<" FitWeight "<<j<<"="<<nXTCoefWM[j]->getVal()<<"   hist="<<HnXTWeights->GetBinContent(j+1)<<endl;
  }
  if(fabs(totDataweight-1.)>1e-5){cout<<" Fit weights do not add up to 1."<<endl; return 0;}
  

  //------------------------------
  //Compute the fit weighted efficiency
  //------------------------------
  TotMuEff=0.;
  for(Int_t j=0;j<NXT;j++)
    TotMuEff+= nXTCoefWM[j]->getVal()*MuEff[j];   
  
//   //use true weights
//   cout<<"!!!!!!!!!!Using true weights!!!!!!!!!!"<<endl;
//   TotMuEff=0.;
//   for(Int_t j=0;j<NXT;j++){
//     TotMuEff+= truew[j]*MuEff[j];
//   }

  cout<<fixed<<setprecision(3)<<"The reweighted muon efficiency is "<<TotMuEff<<endl;

  //Use total efficiency 
//   TotMuEff=(TotNuRec*TotDsProd)/(TotNuProd*TotDsRec);
//   cout<<" !!!!!!!!!!!!!Using Total Efficiency (No nXT reweighting)!!!!!!!!    = "<<TotMuEff<<endl;



  //------------------------------
  //neutrino yield
  //------------------------------
  Float_t NRecoNu=0;
  Float_t NRecoNuErr=0;
  Float_t HNuInteg=0;
  if(FNumFitResults->IsZombie()){cout<<"Bad NuFit File"<<endl; return 0;} gROOT->cd();
  RooFitResult* NuFitResult=NULL;
  if(_DataType==1)NuFitResult=(RooFitResult*)FNumFitResults->Get("fitResultsDsToMuNuRunAll");
  if(_DataType==2)NuFitResult=(RooFitResult*)FNumFitResults->Get("fitResultsDataDsToMuNuRunAll");
  if(!NuFitResult){cout<<"No NuFitResult"<<endl;return 0;}
  RooRealVar*NuYield=NULL;
  if(_DataType==1)NuYield=(RooRealVar*)NuFitResult->floatParsFinal().find("fSig");// fSignalRunAllDsToMuNu_EExtraCut
  if(_DataType==2)NuYield=(RooRealVar*)NuFitResult->floatParsFinal().find("fDataRunAllDsToMuNu_EExtraCut");
  if(!NuYield){cout<<"fSig not found"<<endl;return 0;}
  TH2F*HNuMassData=NULL;
  if(_DataType==1)HNuMassData=(TH2F*)FNumFitResults->Get("hmm2MuonRunAll_EExtraCut");// DsToMuNu_RunAll_EExtraCut 
  if(_DataType==2)HNuMassData=(TH2F*)FNumFitResults->Get("DsToMuNu_Data_RunAll_EExtraCut");
  if(!HNuMassData){cout<<"No HNuMassData "<<endl;return 0;}
  cout<<"extracted MuNu Data: "<<HNuMassData->GetName()<<endl;
  HNuInteg=HNuMassData->Integral();
  NRecoNu=NuYield->getVal()*HNuInteg;
  NRecoNuErr=NuYield->getError()*HNuInteg;

  /////Use the true MC yield
//    TFile FNumTrue("/u/br/aidanrc/ana50/AWG90/aidanrc/Roofit/DsToMuNuTrueSignalRunAll_EExtraCut.root","read");
//    TTree*TNumTrue=(TTree*)FNumTrue.Get("tDsToMuNuTrueSignalRunAll_EExtraCut");
//    TH1F HNumTrue("HNumTrue","",300,-1,4.);
//    TNumTrue->Draw("mm2>>HNumTrue","weight");
//    NRecoNu=HNumTrue.Integral();
//    cout<<"!!!!!!!!!!!Using true MuNu Yield!!!!!!!!!!!!!"<<endl;

  cout<<"The total neutrino yield is "
      <<NRecoNu<<" +- "<<NRecoNuErr
      <<" --> "<<NRecoNuErr*100./NRecoNu<<"%"<<endl;
  

  cout<<endl;
  //---------------------------
  //Compute the Branching fraction
  //----------------------------
  Float_t BF=NRecoNu/(NRecoDs*TotMuEff);
  cout<<"-----------------------------------------------"<<endl;
  cout<<fixed<<setprecision(2)<<" The Branching fraction is = "<<BF*1000<<" \\times 10^{-3} "<<endl;
  cout<<"-----------------------------------------------"<<endl;
  
  //  return BF;

  cout<<endl;
  //----------------------------------
  //Compute the error
  //----------------------------------

  //error on Denominator: DsYield*( weighted MuEfficiency)
  //set these pointers for later calculations
  RooRealVar* FitVar[3];
  FitVar[0]=&TotYield;
  FitVar[1]=nXTb;
  FitVar[2]=nXTg;

  //read in the full correlation matrix
  cout<<"Covariance Matrix:"<<endl;
  Float_t CovMatrix[3][3];
  for(Int_t l=0;l<3;l++){
    cout<<FitVar[l]->GetName()<<"     ";
    for(Int_t k=0;k<3;k++){
      CovMatrix[l][k]=DsFitResult->correlation(*FitVar[l],*FitVar[k]) * (FitVar[l]->getError()) * (FitVar[k]->getError());
      cout<<scientific<<" ("<<CovMatrix[l][k]<<")";
    }
    cout<<endl;
  }
  
  //Compute derivatives w.r.t the floated parameters
  //auxiliary quantities
  Float_t SumW=0.;
  Float_t SumB=0.;
  Float_t SumG=0.;
  for(Int_t k=0;k<NXT;k++){//sum over nXT
    SumW+=pow(k-nXTa->getVal(),nXTb->getVal())*exp(-nXTg->getVal()*k);
    //SumB+=pow(k-nXTa->getVal(),nXTb->getVal()-1)*exp(-nXTg->getVal()*k);
    SumB+=log(k-nXTa->getVal())*pow(k-nXTa->getVal(),nXTb->getVal())*exp(-nXTg->getVal()*k);
    SumG+=k*pow(k-nXTa->getVal(),nXTb->getVal())*exp(-nXTg->getVal()*k);
  }
  //derivatives w_j w.r.t to the beta and gamma
  Float_t DerivWeightB[NXT];
  Float_t DerivWeightG[NXT];
  for(Int_t j=0;j<NXT;j++){//sum over nXT
    //DerivWeightB[j]=nXTCoefWM[j]->getVal()*nXTb->getVal()*(1./(j-nXTa->getVal()) - SumB/SumW);
    DerivWeightB[j]=(nXTCoefWM[j]->getVal()/SumW) * (SumW*log(j-nXTa->getVal()) - SumB);
    DerivWeightG[j]=nXTCoefWM[j]->getVal()*(SumG/SumW-j);
  }

  //finally derivatives of the denominator N*epsilon w.r.t to the floated parameters
  Float_t DerivDenom[3];
  DerivDenom[0]=TotMuEff;
  DerivDenom[1]=0.;
  DerivDenom[2]=0.;
  for(Int_t j=0;j<NXT;j++){
    DerivDenom[1]+=NRecoDs*DerivWeightB[j]*MuEff[j];
    DerivDenom[2]+=NRecoDs*DerivWeightG[j]*MuEff[j];
  }

  cout<<"Denominator error matrix:"<<endl;
  //add the correlated errors
  Float_t TotDenomErr=0.;
  for(Int_t l=0;l<3;l++){
    for(Int_t k=0;k<3;k++){
      cout<<scientific<<"   "<<DerivDenom[l]*DerivDenom[k]*CovMatrix[l][k];
      TotDenomErr+=DerivDenom[l]*DerivDenom[k]*CovMatrix[l][k];
    }
    cout<<endl;
  }  
  //add the un-correlated errors due to the individual efficiencies
  for(Int_t j=0;j<NXT;j++)//sum over nXT
    TotDenomErr+=pow(NRecoDs*nXTCoefWM[j]->getVal()*MuEffErr[j],2);
  
  TotDenomErr=sqrt(TotDenomErr);
  cout<<fixed<<setprecision(2)<<" The BF denominator is = "
      <<NRecoDs*TotMuEff/1000
      <<" $\\pm$ "<<TotDenomErr/1000
      <<" \\times 10^{3}"
      <<"     -->"<<100.*TotDenomErr/(NRecoDs*TotMuEff)<<"%"<<endl;
 
  
  //----------------------------------
  ///add error on Numerator yield + total error on denominator*efficiency
  //---------------------------------
  Float_t BFErr=BF*sqrt(pow(NRecoNuErr/NRecoNu,2) +  pow(TotDenomErr/(NRecoDs*TotMuEff),2));
  cout<<"-----------------------------------------------"<<endl;
  cout<<fixed<<setprecision(2)<<" The Branching fraction Error is ="<<BFErr*1000<<"\\times 10^{-3}    -->"<<100.*BFErr/BF<<"%"<<endl;
  cout<<"-----------------------------------------------"<<endl;


  return BF;
}



void DRecoDsInclusiveAnalysis::DetermineBFKKpi(){

  TString Denomfilename="/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResults/";
  if(_DataType==1)Denomfilename+="DenominatorFitResult_MC.root";
  if(_DataType==2)Denomfilename+="DenominatorFitResult_Data.root";
  TFile FDsFit(Denomfilename,"read");//Denominator

  TString filename="/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResultsKKpi/";
  if(_DataType==1)filename+="KKpiFitResult_MC.root";//Numerator
  if(_DataType==2)filename+="KKpiFitResult_Data.root";
  TFile FNumeratorFit(filename,"read");
  
  TFile FDenominatorEff("/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResults/DenominatorEff.root","r");//Ds efficiency
  TFile FNumeratorEff("/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResultsKKpi/KKpiEffNumerator.root","r");//KKpi efficiency

  TFile FDalitzWeight("/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResultsKKpi/DalitzWeights.root","r");//Dalitz weights contains both MC and Data

  ComputeBFKKpiDalitz(&FDsFit,&FNumeratorFit,&FDenominatorEff,&FNumeratorEff,&FDalitzWeight);
}


Float_t DRecoDsInclusiveAnalysis::ComputeBFKKpiDalitz(TFile* FDenomFitResults,TFile* FNumFitResults,TFile* FEffDemon,TFile* FEffNum,TFile* FDalitzWeights){
    
  cout<<endl;
  //------------------------------
  //Numerator efficiencies
  //------------------------------  
  if(FEffNum->IsZombie()){cout<<"Bad FNumeratorEff File"<<endl; return 0;} gROOT->cd();
  if(FEffDemon->IsZombie()){cout<<"Bad FDenominatorEff File"<<endl; return 0;} gROOT->cd();

  ifstream DsProducedFile; 
  DsProducedFile.open("/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResults/DsProduced.txt");
  ifstream KKpiProducedFile; 
  KKpiProducedFile.open("/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResultsKKpi/KKpiProduced.txt");

  Float_t EffDs[NXT];
  Float_t EffDsErr[NXT];
  Float_t EffKKpi[NXT];
  Float_t EffKKpiErr[NXT];
  Float_t Eff[NXT];
  Float_t EffErr[NXT];
  Float_t KKpiProd[NXT];
  Float_t KKpiRec[NXT];
  Float_t TotKKpiProd=0;//this number is different from the number of events in the BAD (49.409M) because of the RecoBG Filter
  Float_t TotKKpiRec=0;
  Float_t DsProd[NXT];
  Float_t DsRec[NXT];
  Float_t TotDsProd=0;
  Float_t TotDsRec=0;

  ////read in the produced Ds and reco Ds
  for(Int_t j=0;j<NXT;j++){
    Int_t indexcheckn;
    Float_t NProdDs=0.;
    DsProducedFile>>indexcheckn>>NProdDs;   
    if(indexcheckn!=j){cout<<" NXT index does not match for Ds produced   j="<<j<<endl;exit(0);}
    DsProd[j]=NProdDs;
    TotDsProd+=NProdDs;

    TH2F*HDsEff=(TH2F*)FEffDemon->Get(TString("HDsMassTrue2D_nXT")+(long)j);
    if(!HDsEff){cout<<"No HDsEff"<<j<<endl;return 0;}
    DsRec[j]=HDsEff->Integral();
    TotDsRec+=HDsEff->Integral();
  }
     
  //determine nXT gen weights
  Float_t GenNXTWeights[NXT];
  cout<<" Ds Gen weights= "<<endl;
  for(Int_t j=0;j<NXT;j++){
    GenNXTWeights[j]=DsProd[j]/TotDsProd;
    //cout<<","<<GenNXTWeights[j]<<endl; 
  }
  cout<<endl;

  //read in the total KKpi produced 
  for(Int_t d=0;d<3;d++){
    for(Int_t j=0;j<NXT;j++){
      Int_t indexcheckn;
      Int_t indexcheckd;
      Float_t NProdKKpi=0.;
      KKpiProducedFile>>indexcheckd>>indexcheckn>>NProdKKpi;   
      if(indexcheckd!=(d+1)||indexcheckn!=j){cout<<"d,j"<<d<<","<<j<<endl;exit(0);}
      TotKKpiProd+=NProdKKpi;             
    }
  }

  //
  for(Int_t j=0;j<NXT;j++){ 
    KKpiProd[j]=TotKKpiProd*GenNXTWeights[j];

    TH1F*HEff=(TH1F*)FEffNum->Get(TString("HKKpiGamMassSignal_nXT")+(long)j);
    //TH1F*HEff=(TH1F*)FEffNum->Get(TString("HKKpiGamMassTrue_Dalitz")+(long)d+"_nXT"+(long)j);cout<<"!!!using true KKpi eff"<<endl;
    if(!HEff){cout<<"No HEff "<<j<<endl;return 0;}
    KKpiRec[j]=HEff->Integral();
    TotKKpiRec+=HEff->Integral();
  }
  
    
  //compute the efficiency matrix
  Float_t DalitzCorretion[2]={1.01238,1.01686};//determined by calculating (weighted eff./total eff.)
  Float_t PIDCorr=.973522; // = .9866 per Kaon
  for(Int_t j=0;j<NXT;j++){
    EffKKpi[j]=(KKpiRec[j]/KKpiProd[j]);
    EffKKpi[j]=EffKKpi[j]*DalitzCorretion[_DataType-1]; 
    cout<<"MC dalitz correction applied j="<<j<<"  corr="<<DalitzCorretion[_DataType-1]<<endl;
    if(_DataType==2){//correct for PID 
      EffKKpi[j]=EffKKpi[j]*PIDCorr;
      cout<<"PID correction has  been applied j="<<j<<"  corr="<<PIDCorr<<endl;
    }
    EffKKpiErr[j]=sqrt(KKpiProd[j])/KKpiProd[j];

    EffDs[j]=(DsRec[j]/DsProd[j]);
    EffDsErr[j]=sqrt(DsRec[j])/DsProd[j];

    Eff[j]=EffKKpi[j]/EffDs[j];
    EffErr[j]=Eff[j]*sqrt(pow(sqrt(KKpiRec[j])/KKpiRec[j],2)+pow(sqrt(DsRec[j])/DsRec[j],2));  
  }
  cout<<endl;
  //print out the efficiency in latex format
  cout<<"nXT efficiencies: "<<endl;
  for(Int_t j=0;j<NXT;j++){
    printf(" & %.3f $\\pm$ %.3f",Eff[j],EffErr[j]);
  }
  printf(" \\\\ \n");


  ///True NXT weights
  Float_t totMCweight=0.;
  for(Int_t j=0;j<NXT;j++){
    totMCweight+= truew[j];
    cout<<" True Weight "<<j<<"="<<truew[j]<<endl;
  }
  if(fabs(totMCweight-1.)>1e-5){cout<<" MC weights do not add up to 1."<<endl; return 0;}

  //compute the true reweighted efficiency for each dalitz region
  Float_t TotEff=0.;
  Float_t TotEffErr=0.;
  for(Int_t j=0;j<NXT;j++){  
    TotEff+= truew[j]*Eff[j];
    TotEffErr+= pow(truew[j]*EffErr[j],2);
  }
  TotEffErr=sqrt(TotEffErr);
  cout<<" The true weighted efficiency is "<<TotEff<<endl;
  
  cout<<endl;
  //------------------------------
  //reconstructed nXT  weights
  //------------------------------  
  if(FDenomFitResults->IsZombie()){cout<<"Bad DsFit File"<<endl; return 0;} gROOT->cd();
  RooFitResult* DsFitResult=(RooFitResult*)FDenomFitResults->Get("fitresult");
  if(!DsFitResult){cout<<"No DsFitResult"<<endl;return 0;}
  RooRealVar* nXTbFit=NULL;
  RooRealVar* nXTgFit=NULL;
  TH1F*HnXTWMPars=(TH1F*)FDenomFitResults->Get("HnXTWMPars");
  //if(!HnXTWMPars){cout<<"HnXTWMPars not found"<<endl;return 0;}
  if(HnXTWMPars)//value of nXTa was changed from its  nominal value --> must be set properly here 
    nXTa->setVal(HnXTWMPars->GetBinContent(1));
  //nXTaFit=((RooRealVar*)DsFitResult->floatParsFinal().find("nXTa"));
  //if(!nXTaFit){cout<<"nXT pars  not found"<<endl;return 0;}
  nXTbFit=((RooRealVar*)DsFitResult->floatParsFinal().find("nXTb"));
  if(!nXTbFit){cout<<"nXTb par not found"<<endl;return 0;}
  nXTb->setVal(nXTbFit->getVal());
  nXTb->setError(nXTbFit->getError());
  nXTgFit=((RooRealVar*)DsFitResult->floatParsFinal().find("nXTg"));
  if(!nXTgFit){cout<<"nXTg par not found"<<endl;return 0;}
  nXTg->setVal(nXTgFit->getVal());
  nXTg->setError(nXTgFit->getError());   

  Float_t totDataweight=0.;
  for(Int_t j=0;j<NXT;j++){    
    totDataweight+=nXTCoefWM[j]->getVal();
    cout<<" FitWeight"<<j<<"="<<nXTCoefWM[j]->getVal()<<endl;
  }
  //----------------------  
  if(fabs(totDataweight-1.)>1e-5){cout<<" Fit weights do not add up to 1."<<endl; return 0;}
  

  //------------------------------
  //Compute the reconstructed weighted efficiency
  //------------------------------ 
  TotEff=0.;
  TotEffErr=0.;
  for(Int_t j=0;j<NXT;j++){  
    TotEff+= nXTCoefWM[j]->getVal()*Eff[j];
    TotEffErr+= pow(nXTCoefWM[j]->getVal()*EffErr[j],2);
  }
  TotEffErr=sqrt(TotEffErr);
  cout<<" The fit weighted efficiency is "<<TotEff<<endl;


  cout<<endl;
  //------------------------------
  ///Ds yield 
  //------------------------------ 
  TH2F*HDsMassData=(TH2F*)FDenomFitResults->Get("HDsMassData");
  if(!HDsMassData){cout<<"No HDsMassData "<<endl;return 0;}
  RooRealVar*DsYield=(RooRealVar*)DsFitResult->floatParsFinal().find("TotYield");
  if(!DsYield){cout<<"DsYield not found"<<endl;return 0;}
  TotYield.setVal(DsYield->getVal());
  TotYield.setError(DsYield->getError());
  Float_t NRecoDs=HDsMassData->Integral()*TotYield.getVal();
  //TFile FDenominatorTrueYield("/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResults/DenominatorEff.root","r");
  //NRecoDs=((TH1F*)FDenominatorTrueYield.Get("HDsMassTrue"))->Integral();cout<<"!!!!!!!!!!!!Using true Ds yield"<<endl;
  Float_t NRecoDsErr=HDsMassData->Integral()*TotYield.getError();
  cout<<"The inclusive Ds yield is "<<NRecoDs<<" +- "<<NRecoDsErr<<" -->"<<NRecoDsErr*100./NRecoDs<<"%"<<endl;
  

  //------------------------------
  //Numerator yield
  //------------------------------
  if(FNumFitResults->IsZombie()){cout<<"Bad NuFit File"<<endl; return 0;} gROOT->cd();
  RooFitResult* NumeratorFitResult=(RooFitResult*)FNumFitResults->Get(TString("FitResult"));
  if(!NumeratorFitResult){cout<<"No NumeratorFitResult"<<endl;return 0;}
  TH1F* HNumeratorData=(TH1F*)FNumFitResults->Get(TString("HKKpiGamMass"));
  if(!HNumeratorData){cout<<"No HNumeratorData "<<endl;return 0;}
  RooRealVar*NumeratorYield=(RooRealVar*)NumeratorFitResult->floatParsFinal().find(TString("SigYield"));
  if(!NumeratorYield){cout<<"NumeratorYield not found"<<endl;return 0;}

  Float_t NRecoNumerator=NumeratorYield->getVal()*HNumeratorData->Integral();
  //TFile FNumeratorTrueYield("DsTolnu/KKpi/SP1005/HistosForFit.root","r");
  //NRecoNumerator=((TH1F*)FNumeratorTrueYield.Get("HKKpiGamMassSignal"))->Integral();cout<<"!!!!!!Using truth-matched Numerator"<<endl;
  //NRecoNumerator=((TH1F*)FNumeratorTrueYield.Get("HKKpiGamMassTrue"))->Integral();cout<<"!!!!!!!!!!!!Using true Numerator"<<endl;
  Float_t NRecoNumeratorErr=NumeratorYield->getError()*HNumeratorData->Integral();
  cout<<"Numerator yield= "<<NRecoNumerator<<" +- "<<NRecoNumeratorErr
      <<"  -->"<<NRecoNumeratorErr*100./NRecoNumerator<<"%"<<endl;  
  


  cout<<endl;
  //---------------------------
  //Compute the Branching fraction
  //----------------------------
  Float_t BF=NRecoNumerator/(NRecoDs*TotEff);
  cout<<"-----------------------------------------------"<<endl;
  cout<<" The Branching fraction is = "<<setprecision (4)<<fixed<<BF*100<<endl;
  cout<<"-----------------------------------------------"<<endl;
  
  //return BF;
  
//Info from DECAY.DEC Ds decays 
// 0.044    phi   pi+                      SVS; phi--> 0.4910   K+   K-                         VSS;
// 0.040    anti-K*0   K+                  SVS;  K*0 --> 0.6657      K+  pi-                        VSS;
// 0.01    f_0    pi+                      PHSP;  f_0 --> 0.1100   K+   K-                         PHSP
// 0.0023    f_2    pi-                    PHSP;  f_2 --> 0.0230   K+   K-                         TSS;
// 0.011    K+    K-    pi+                PHSP; 
//Total Ds-->KKpi= .044*0.4910 + 0.040*0.6657 + 0.01*0.1100 + .0023*0.0230 + 0.011=6.038e-02


  cout<<endl;
  //----------------------------------
  //Compute the error
  //----------------------------------
  //error on Denominator: DsYield*( weighted MuEfficiency)
  //set these pointers for later calculations
  RooRealVar* FitVar[3];
  FitVar[0]=&TotYield;
  FitVar[1]=nXTb;
  FitVar[2]=nXTg;

  //read in the full correlation matrix
  cout<<"Covariance Matrix:"<<endl;
  Float_t CovMatrix[3][3];
  for(Int_t j=0;j<3;j++){
    cout<<FitVar[j]->GetName()<<"     ";
    for(Int_t k=0;k<3;k++){
      CovMatrix[j][k]=DsFitResult->correlation(*FitVar[j],*FitVar[k]) * (FitVar[j]->getError()) * (FitVar[k]->getError());
      cout<<scientific<<" ("<<CovMatrix[j][k]<<")";
    }
    cout<<endl;
  }
  
  //Compute derivatives w.r.t the floated parameters
  Float_t DerivWeightB[NXT];
  Float_t DerivWeightG[NXT];
  Float_t SumW=0.;
  Float_t SumB=0.;
  Float_t SumG=0.;
  for(Int_t j=0;j<NXT;j++){
    SumW+=pow(j-nXTa->getVal(),nXTb->getVal())*exp(-nXTg->getVal()*j);
    SumB+=log(j-nXTa->getVal())*pow(j-nXTa->getVal(),nXTb->getVal())*exp(-nXTg->getVal()*j);
    SumG+=j*pow(j-nXTa->getVal(),nXTb->getVal())*exp(-nXTg->getVal()*j);
  }
  for(Int_t j=0;j<NXT;j++){
    DerivWeightB[j]=(nXTCoefWM[j]->getVal()/SumW) * (SumW*log(j-nXTa->getVal()) - SumB);
    DerivWeightG[j]=nXTCoefWM[j]->getVal()*(SumG/SumW-j);
  }

  Float_t DerivDenom[3];
  DerivDenom[0]=TotEff;
  DerivDenom[1]=0.;
  DerivDenom[2]=0.;
  for(Int_t j=0;j<NXT;j++){
    DerivDenom[1]+=TotYield.getVal()*DerivWeightB[j]*Eff[j];
    DerivDenom[2]+=TotYield.getVal()*DerivWeightG[j]*Eff[j];
  }

  cout<<"Error matrix:"<<endl;
  //add the correlated errors
  Float_t TotDenomErr=0.;
  for(Int_t l=0;l<3;l++){
    for(Int_t k=0;k<3;k++){
      cout<<scientific<<"   "<<DerivDenom[l]*DerivDenom[k]*CovMatrix[l][k];
      TotDenomErr+=DerivDenom[l]*DerivDenom[k]*CovMatrix[l][k];
    }
    cout<<endl;
  }  

  //add the un-correlated errors due to the individual efficiencies
  for(Int_t j=0;j<NXT;j++)
    TotDenomErr+=pow(TotYield.getVal()*nXTCoefWM[j]->getVal()*EffErr[j],2);
  
  TotDenomErr=sqrt(TotDenomErr);
  cout<<fixed<<scientific<<" The BF denominator is = "<<TotYield.getVal()*HDsMassData->Integral()*TotEff
      <<" +- "<<TotDenomErr*HDsMassData->Integral()
      <<"  -->"<<100.*TotDenomErr/(TotYield.getVal()*TotEff)<<"%"<<endl;
 
  
  //----------------------------------
  ///add error on Numerator yield + total error on denominator*efficiency
  //---------------------------------
  Float_t BFErr=BF*sqrt(pow(NRecoNumeratorErr/NRecoNumerator,2) +  pow(TotDenomErr/(TotYield.getVal()*TotEff),2));
  cout<<"-----------------------------------------------"<<endl;
  cout<<" The Branching fraction Error is ="<<setprecision (4)<<fixed<<BFErr*100
      <<"  -->"<<setprecision (2)<<fixed<<100.*BFErr/BF<<"%"<<endl;
  cout<<"-----------------------------------------------"<<endl;

  return BF;
}


/*
Float_t DRecoDsInclusiveAnalysis::ComputeBF(TFile* FDenomFitResults,TFile* FNumFitResults,TFile* FEffDemon,TFile* FEffNum){
    
  cout<<endl;
  //------------------------------
  //muon efficiencies
  //------------------------------
  if(FEffNum->IsZombie()){cout<<"Bad FMuEff File"<<endl; return 0;} gROOT->cd();
  if(FEffDemon->IsZombie()){cout<<"Bad FDsEff File"<<endl; return 0;} gROOT->cd();

  //declare text files with produced Ds and Muon yields as a function of nXT
  ifstream DsProducedFile; 
  DsProducedFile.open("/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResults/DsProduced.txt");
  ifstream MuNuProducedFile; 
  MuNuProducedFile.open("/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResults/MuNuProduced.txt");

  Float_t MuEff[NXT];
  Float_t MuEffErr[NXT];  
  Float_t TotDsProd=0;
  Float_t TotDsRec=0;
  Float_t TotNuProd=0;
  Float_t TotNuRec=0;
  Float_t DsProd[NXT];
  Float_t NuProd[NXT];
  Float_t DsRec[NXT];
  Float_t NuRec[NXT];
  Float_t DsRecErr[NXT];
  Float_t NuRecErr[NXT];


  //
  for(Int_t j=0;j<NXT;j++){
    //Generated Ds
    Int_t indexcheckn;
    Float_t NProdDs=0.;
    DsProducedFile>>indexcheckn>>NProdDs;   
    if(indexcheckn!=j){cout<<" NXT index does not match for Ds produced   j="<<j<<endl;exit(0);}
    DsProd[j]=NProdDs;
    TotDsProd+=NProdDs;  

    //Reconstructed Ds
    TH2F*HDsEff=(TH2F*)FEffDemon->Get(TString("HDsMassTrue2D_nXT")+(long)j);
    if(!HDsEff){cout<<"No HDsEff"<<j<<endl;return 0;}
    DsRec[j]=HDsEff->Integral(1,4,DenominatorDsMassBinMin,DenominatorDsMassBinMax);
    DsRecErr[j]=sqrt(HDsEff->Integral(1,4,DenominatorDsMassBinMin,DenominatorDsMassBinMax));
    TotDsRec+=HDsEff->Integral(1,4,DenominatorDsMassBinMin,DenominatorDsMassBinMax);

    //read in the total munu produced 
    Float_t NProdMuNu=0.;
    MuNuProducedFile>>indexcheckn>>NProdMuNu;   
    if(indexcheckn!=j){cout<<"j="<<j<<endl;exit(0);}
    TotNuProd+=NProdMuNu;             

    
    //Reconstructed MuNu
    //TH1F*HMuEff=(TH1F*)FEffNum->Get(TString("hNXT")+(long)j+"DsToMuNu_EExtraCut");
    TH1F*HMuEff=(TH1F*)FEffNum->Get(TString("DsToMuNuNXT")+(long)j);
    if(!HMuEff){cout<<"No HMuEff"<<j<<endl;return 0;}
    NuRec[j]=HMuEff->Integral();
    NuRecErr[j]=sqrt(HMuEff->Integral());
    TotNuRec+=HMuEff->Integral();
  }
     
  //determine nXT gen weights
  Float_t GenNXTWeights[NXT];
  for(Int_t j=0;j<NXT;j++){
    GenNXTWeights[j]=DsProd[j]/TotDsProd;
    //cout<<","<<GenNXTWeights[j]<<endl; 
  }
  cout<<endl;

  //determine the distribution of MuNu generated
  for(Int_t j=0;j<NXT;j++){ 
    NuProd[j]=TotNuProd*GenNXTWeights[j];
  }
  

  cout<<" nXT efficiencies: "<<endl;
  for(Int_t j=0;j<NXT;j++){ 
    MuEff[j]=(NuRec[j]*DsProd[j]/(NuProd[j]*DsRec[j]));
    MuEffErr[j]=MuEff[j]*sqrt(pow(NuRecErr[j]/NuRec[j],2) + pow(DsRecErr[j]/DsRec[j],2));
    cout<<setprecision(3)<<fixed<<j<<" & "<<MuEff[j]<<" $\\pm$ "<<MuEffErr[j]<<" \\\\"<<endl;
  }
    
  cout<<" \\\\"<<endl;
  cout<<scientific<<"The Ds efficiency is "<<TotDsRec/TotDsProd<<endl;
  cout<<scientific<<"The MuNu efficiency is "<<TotNuRec/TotNuProd<<endl;
  

  /////True MC Reco Weights ------------------------------
  Float_t totMCweight=0.;
  for(Int_t j=0;j<NXT;j++){
    totMCweight+= truew[j];
    cout<<" MCweight"<<j<<"="<<truew[j]<<endl;
  }
  if(fabs(totMCweight-1.)>1e-5){cout<<" MC weights do not add up to 1."<<endl; return 0;}
  Float_t TotMuEff=0.;
  for(Int_t j=0;j<NXT;j++){  
    TotMuEff+= truew[j]*MuEff[j];
  }
  cout<<"The MC true weighted muon efficiency is "<<TotMuEff<<endl;

  cout<<endl;
  //------------------------------
  ///Ds yield 
  //------------------------------
  if(FDenomFitResults->IsZombie()){cout<<"Bad DsFit File"<<endl; return 0;} gROOT->cd();
  RooFitResult* DsFitResult=(RooFitResult*)FDenomFitResults->Get("fitresult");
  if(!DsFitResult){cout<<"No DsFitResult"<<endl;return 0;}
  TH2F*HDsMassData=(TH2F*)FDenomFitResults->Get("HDsMassData");
  if(!HDsMassData){cout<<"No HDsMassData "<<endl;return 0;}
  RooRealVar*DsYield=(RooRealVar*)DsFitResult->floatParsFinal().find("TotYield");
  if(!DsYield){cout<<"DsYield not found"<<endl;return 0;}
  TotYield.setVal(DsYield->getVal()); 
  TotYield.setError(DsYield->getError());
  cout<<"The fitted Ds yield fraction is "<<TotYield.getVal()<<" +- "<<TotYield.getError()<<" --> "<<TotYield.getError()*100./TotYield.getVal()<<"%"<<endl;  
  cout<<endl;
  
  //for MC lumi weight the Ds yield because Numerator is lumi weighted 
//   if(_DataType==1){    
// //     NRecoDs=NRecoDs*(1./2.04+.0974/1.93)/(1+.0974);
// //     NRecoDsErr=NRecoDsErr*(1./2.04+.0974/1.93)/(1+.0974);
//     TotYield.setVal(DsYield->getVal()*(1./2.04+.0974/1.93)/(1+.0974)); 
//     TotYield.setError(DsYield->getError()*(1./2.04+.0974/1.93)/(1+.0974));
// cout<<"The total Ds yield after Lumi Weighting is "<<TotYield.getVal()<<" +- "<<TotYield.getError()<<" --> "<<TotYield.getError()*100./TotYield.getVal()<<"%"<<endl;    
//  cout<<endl;
//   }

 

  TH2F* TotSigPdfHistoFit=(TH2F*)FDenomFitResults->Get("TotSigPdfHistoFit"); if(!TotSigPdfHistoFit)cout<<"No TotSigPdfHistoFit"<<endl;
  Float_t NRecoDs=HDsMassData->Integral()*TotYield.getVal()
    *TotSigPdfHistoFit->Integral(1,4,DenominatorDsMassBinMin,DenominatorDsMassBinMax)/TotSigPdfHistoFit->Integral();
  Float_t NRecoDsErr=HDsMassData->Integral()*TotYield.getError()
    *TotSigPdfHistoFit->Integral(1,4,DenominatorDsMassBinMin,DenominatorDsMassBinMax)/TotSigPdfHistoFit->Integral();

//   //use true Ds yield  
//   TFile FDenomTrue("/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResults/DenominatorEff.root","read");
//   NRecoDs=((TH1F*)FDenomTrue.Get("HDsMassTrue"))->Integral(DenominatorDsMassBinMin,DenominatorDsMassBinMax);   cout<<"!!!!!!!!!!!Using true Ds Yield!!!!!!!!!!!!!"<<endl;

  cout<<"The total Ds yield is "<<NRecoDs<<" +- "<<NRecoDsErr<<" --> "<<NRecoDsErr*100./NRecoDs<<"%"<<endl;  
  cout<<endl;


  //------------------------------
  //Fitted nXT  weights
  //------------------------------  
  //RooRealVar* nXTaFit=NULL;
  RooRealVar* nXTbFit=NULL;
  RooRealVar* nXTgFit=NULL;
  TH1F*HnXTWMPars=(TH1F*)FDenomFitResults->Get("HnXTWMPars");
  //if(!HnXTWMPars){cout<<"HnXTWMPars not found"<<endl;return 0;}
  if(HnXTWMPars)//value of nXTa was changed from its  nominal value --> must be set properly here 
    nXTa->setVal(HnXTWMPars->GetBinContent(1));
  //nXTaFit=((RooRealVar*)DsFitResult->floatParsFinal().find("nXTa"));
  //if(!nXTaFit){cout<<"nXT pars  not found"<<endl;return 0;}
  nXTbFit=((RooRealVar*)DsFitResult->floatParsFinal().find("nXTb"));
  if(!nXTbFit){cout<<"nXTb par not found"<<endl;return 0;}
  nXTb->setVal(nXTbFit->getVal());
  nXTb->setError(nXTbFit->getError());
  nXTgFit=((RooRealVar*)DsFitResult->floatParsFinal().find("nXTg"));
  if(!nXTgFit){cout<<"nXTg par not found"<<endl;return 0;}
  nXTg->setVal(nXTgFit->getVal());
  nXTg->setError(nXTgFit->getError());
  
  //cout<<setprecision(6)<<"nXTb="<<nXTb->getVal()<<"+-"<<nXTb->getError()<<"    nXTg="<<nXTg->getVal()<<"+-"<<nXTg->getError()<<endl;

  TH1F* HnXTWeights=(TH1F*)FDenomFitResults->Get("HnXTweights");
  Float_t totDataweight=0.;
  for(Int_t j=0;j<NXT;j++){
    totDataweight+=nXTCoefWM[j]->getVal();//note that these coefficients are being updated as long as the first three are updated
    cout<<" FitWeight "<<j<<"="<<nXTCoefWM[j]->getVal()<<"   hist="<<HnXTWeights->GetBinContent(j+1)<<endl;
  }
  if(fabs(totDataweight-1.)>1e-5){cout<<" Fit weights do not add up to 1."<<endl; return 0;}
  

  //------------------------------
  //Compute the fit weighted efficiency
  //------------------------------
  TotMuEff=0.;
  for(Int_t j=0;j<NXT;j++)
    TotMuEff+= nXTCoefWM[j]->getVal()*MuEff[j];   
  
//   //use true weights
//   cout<<"!!!!!!!!!!Using true weights!!!!!!!!!!"<<endl;
//   TotMuEff=0.;
//   for(Int_t j=0;j<NXT;j++){
//     TotMuEff+= truew[j]*MuEff[j];
//   }

  cout<<fixed<<setprecision(3)<<"The reweighted muon efficiency is "<<TotMuEff<<endl;

  //Use total efficiency 
//   TotMuEff=(TotNuRec*TotDsProd)/(TotNuProd*TotDsRec);
//   cout<<" !!!!!!!!!!!!!Using Total Efficiency (No nXT reweighting)!!!!!!!!    = "<<TotMuEff<<endl;



  //------------------------------
  //neutrino yield
  //------------------------------
  Float_t NRecoNu=0;
  Float_t NRecoNuErr=0;
  Float_t HNuInteg=0;
  if(FNumFitResults->IsZombie()){cout<<"Bad NuFit File"<<endl; return 0;} gROOT->cd();
  RooFitResult* NuFitResult=NULL;
  if(_DataType==1)NuFitResult=(RooFitResult*)FNumFitResults->Get("fitResultsDsToMuNuRunAll");
  if(_DataType==2)NuFitResult=(RooFitResult*)FNumFitResults->Get("fitResultsDataDsToMuNuRunAll");
  if(!NuFitResult){cout<<"No NuFitResult"<<endl;return 0;}
  RooRealVar*NuYield=NULL;
  if(_DataType==1)NuYield=(RooRealVar*)NuFitResult->floatParsFinal().find("fSig");// fSignalRunAllDsToMuNu_EExtraCut
  if(_DataType==2)NuYield=(RooRealVar*)NuFitResult->floatParsFinal().find("fDataRunAllDsToMuNu_EExtraCut");
  if(!NuYield){cout<<"fSig not found"<<endl;return 0;}
  TH2F*HNuMassData=NULL;
  if(_DataType==1)HNuMassData=(TH2F*)FNumFitResults->Get("hmm2MuonRunAll_EExtraCut");// DsToMuNu_RunAll_EExtraCut 
  if(_DataType==2)HNuMassData=(TH2F*)FNumFitResults->Get("DsToMuNu_Data_RunAll_EExtraCut");
  if(!HNuMassData){cout<<"No HNuMassData "<<endl;return 0;}
  cout<<"extracted MuNu Data: "<<HNuMassData->GetName()<<endl;
  HNuInteg=HNuMassData->Integral();
  NRecoNu=NuYield->getVal()*HNuInteg;
  NRecoNuErr=NuYield->getError()*HNuInteg;

//    TFile FNumTrue("/u/br/aidanrc/ana50/AWG90/aidanrc/Roofit/DsToMuNuTrueSignalRunAll_EExtraCut.root","read");
//    TTree*TNumTrue=(TTree*)FNumTrue.Get("tDsToMuNuTrueSignalRunAll_EExtraCut");
//    TH1F HNumTrue("HNumTrue","",300,-1,4.);
//    TNumTrue->Draw("mm2>>HNumTrue","weight");
//    NRecoNu=HNumTrue.Integral();
//    cout<<"!!!!!!!!!!!Using true MuNu Yield!!!!!!!!!!!!!"<<endl;

  cout<<"The total neutrino yield is "
      <<NRecoNu<<" +- "<<NRecoNuErr*HNuInteg
      <<" --> "<<NRecoNuErr*100./NRecoNu<<"%"<<endl;
  

  cout<<endl;
  //---------------------------
  //Compute the Branching fraction
  //----------------------------
  Float_t BF=NRecoNu/(NRecoDs*TotMuEff);
  cout<<"-----------------------------------------------"<<endl;
  cout<<fixed<<setprecision(2)<<" The Branching fraction is = "<<BF*1000<<" \\times 10^{-3} "<<endl;
  cout<<"-----------------------------------------------"<<endl;
  
  //  return BF;

  cout<<endl;
  //----------------------------------
  //Compute the error
  //----------------------------------

  //error on Denominator: DsYield*( weighted MuEfficiency)
  //set these pointers for later calculations
  RooRealVar* FitVar[3];
  FitVar[0]=&TotYield;
  FitVar[1]=nXTb;
  FitVar[2]=nXTg;

  //read in the full correlation matrix
  cout<<"Covariance Matrix:"<<endl;
  Float_t CovMatrix[3][3];
  for(Int_t j=0;j<3;j++){
    cout<<FitVar[j]->GetName()<<"     ";
    for(Int_t k=0;k<3;k++){
      CovMatrix[j][k]=DsFitResult->correlation(*FitVar[j],*FitVar[k]) * (FitVar[j]->getError()) * (FitVar[k]->getError());
      cout<<scientific<<" ("<<CovMatrix[j][k]<<")";
    }
    cout<<endl;
  }
  
  //Compute derivatives w.r.t the floated parameters

  Float_t DerivWeightB[NXT];
  Float_t DerivWeightG[NXT];
  Float_t SumW=0.;
  Float_t SumB=0.;
  Float_t SumG=0.;
  for(Int_t k=0;k<NXT;k++){
    SumW+=pow(k-nXTa->getVal(),nXTb->getVal())*exp(-nXTg->getVal()*k);
    SumB+=pow(k-nXTa->getVal(),nXTb->getVal()-1)*exp(-nXTg->getVal()*k);
    SumG+=k*pow(k-nXTa->getVal(),nXTb->getVal())*exp(-nXTg->getVal()*k);
  }
  for(Int_t k=0;k<NXT;k++){
    DerivWeightB[k]=nXTCoefWM[k]->getVal()*nXTb->getVal()*(1./(k-nXTa->getVal()) - SumB/SumW);
    DerivWeightG[k]=nXTCoefWM[k]->getVal()*(SumG/SumW-k);
  }

  Float_t DerivDenom[3];
  DerivDenom[0]=TotMuEff;
  DerivDenom[1]=0.;
  DerivDenom[2]=0.;
  for(Int_t k=0;k<NXT;k++){
    DerivDenom[1]+=NRecoDs*DerivWeightB[k]*MuEff[k];
    DerivDenom[2]+=NRecoDs*DerivWeightG[k]*MuEff[k];
  }

  cout<<"Correlated error matrix:"<<endl;
  //add the correlated errors
  Float_t TotDenomErr=0.;
  for(Int_t j=0;j<3;j++){
    for(Int_t k=0;k<3;k++){
      cout<<scientific<<"   "<<DerivDenom[j]*DerivDenom[k]*CovMatrix[j][k];
      TotDenomErr+=DerivDenom[j]*DerivDenom[k]*CovMatrix[j][k];
    }
    cout<<endl;
  }  
  //add the un-correlated errors due to the individual efficiencies
  for(Int_t j=0;j<NXT;j++)
    TotDenomErr+=pow(NRecoDs*nXTCoefWM[j]->getVal()*MuEffErr[j],2);
  
  TotDenomErr=sqrt(TotDenomErr);
  cout<<fixed<<setprecision(1)<<" The BF denominator is = "
      <<NRecoDs*TotMuEff/1000
      <<" $\\pm$ "<<TotDenomErr/1000
      <<" \\times 10^{3}"
      <<"     -->"<<100.*TotDenomErr/(NRecoDs*TotMuEff)<<"%"<<endl;
 
  
  //----------------------------------
  ///add error on Numerator yield + total error on denominator*efficiency
  //---------------------------------
  Float_t BFErr=BF*sqrt(pow(NRecoNuErr/NRecoNu,2) +  pow(TotDenomErr/(NRecoDs*TotMuEff),2));
  cout<<"-----------------------------------------------"<<endl;
  cout<<fixed<<setprecision(2)<<" The Branching fraction Error is ="<<BFErr*1000<<"\\times 10^{-3}    -->"<<100.*BFErr/BF<<"%"<<endl;
  cout<<"-----------------------------------------------"<<endl;


  return BF;
}
*/

/*
Float_t DRecoDsInclusiveAnalysis::ComputeBFKKpiDalitz(TFile* FDenomFitResults,TFile* FNumFitResults,TFile* FEffDemon,TFile* FEffNum,TFile* FDalitzWeights){
    
  cout<<endl;
  //------------------------------
  //Numerator efficiencies
  //------------------------------  
  if(FEffNum->IsZombie()){cout<<"Bad FNumeratorEff File"<<endl; return 0;} gROOT->cd();
  if(FEffDemon->IsZombie()){cout<<"Bad FDenominatorEff File"<<endl; return 0;} gROOT->cd();

  ifstream DsProducedFile; 
  DsProducedFile.open("/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResults/DsProduced.txt");
  ifstream KKpiProducedFile; 
  KKpiProducedFile.open("/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResultsKKpi/KKpiProduced.txt");

  Float_t Eff[3][NXT];
  Float_t EffErr[3][NXT];
  Float_t TotKKpiGen=49017235.37;//=Tot # of KKpi   //this number is different from the number of events in the BAD (49.409M) because of the Damage Filter
  Float_t TotKKpiProd[3]={0,0,0};
  Float_t TotKKpiRec[3]={0,0,0};
  Float_t KKpiRec[3][NXT];
  Float_t DsProd[NXT];
  Float_t DsRec[NXT];
  Float_t TotDsProd=0;
  Float_t TotDsRec=0;

  //Add up all the Gen Ds 
  for(Int_t j=0;j<NXT;j++){
    Int_t indexcheckn;
    Float_t NProdDs=0.;
    DsProducedFile>>indexcheckn>>NProdDs;   
    if(indexcheckn!=j){cout<<" NXT index does not match for Ds produced   j="<<j<<endl;exit(0);}
    TH2F*HDsEff=(TH2F*)FEffDemon->Get(TString("HDsMassTrue2D_nXT")+(long)j);
    if(!HDsEff){cout<<"No HDsEff"<<j<<endl;return 0;}
    DsProd[j]=NProdDs;
    DsRec[j]=HDsEff->Integral();
    TotDsProd+=NProdDs;
    TotDsRec+=HDsEff->Integral();
  }
     
  //determine nXT gen weights
  Float_t GenNXTWeights[NXT];
  for(Int_t j=0;j<NXT;j++){
    GenNXTWeights[j]=DsProd[j]/TotDsProd;
    //cout<<","<<GenNXTWeights[j];
  }
  cout<<endl;

  //add up all the KKpi for each Dalitz region
  for(Int_t d=0;d<3;d++){
    for(Int_t j=0;j<NXT;j++){ 
       Int_t indexcheckd;  
       Int_t indexcheckj;
       Float_t NProdKKpi=0.;
       KKpiProducedFile>>indexcheckd>>indexcheckj>>NProdKKpi;
       if(indexcheckd!=d+1||indexcheckj!=j){cout<<" NXT index does not match for KKpi produced   d="<<d<<" j="<<j<<endl;return 0;}
       TotKKpiProd[d]+=NProdKKpi;

       TH1F*HEff=(TH1F*)FEffNum->Get(TString("HKKpiGamMassSignal_Dalitz")+(long)d+"_nXT"+(long)j);
       //TH1F*HEff=(TH1F*)FEffNum->Get(TString("HKKpiGamMassTrue_Dalitz")+(long)d+"_nXT"+(long)j);cout<<"!!!using true KKpi eff"<<endl;
       if(!HEff){cout<<"No HEff"<<d<<" "<<j<<endl;return 0;}
       KKpiRec[d][j]=HEff->Integral();
       TotKKpiRec[d]+=HEff->Integral();
     }
  }
  
    
  //compute the efficiency matrix
  Float_t KKpiGenDalitzWeight[3]={0.0556567,0.291852,0.652491};
  for(Int_t d=0;d<3;d++){
    for(Int_t j=0;j<NXT;j++){
      //Eff[d][j]=(KKpiRec[d][j]/(TotKKpiProd[d]*GenNXTWeights[j]))/(DsRec[j]/DsProd[j]);
      Eff[d][j]=(KKpiRec[d][j]/(TotKKpiGen*KKpiGenDalitzWeight[d]*GenNXTWeights[j]))/(DsRec[j]/DsProd[j]);
      if(_DataType==2){
	Eff[d][j]=.9866*Eff[d][j];//correct for PID 
	cout<<"PID correction has  been applied"<<endl;
      }
      EffErr[d][j]=Eff[d][j]*sqrt(pow(sqrt(KKpiRec[d][j])/KKpiRec[d][j],2)+pow(sqrt(DsRec[j])/DsRec[j],2));  
    }
  }
  cout<<endl;
  //print out the efficiency in latex format
  for(Int_t j=0;j<NXT;j++){
    cout<<" $n_X^T$="<<j;
    for(Int_t d=0;d<3;d++)
      printf(" & %.3f $\\pm$ %.3f",Eff[d][j],EffErr[d][j]);
    printf(" \\\\ \n");
  }


  ///True NXT weights
  Float_t totMCweight=0.;
  for(Int_t j=0;j<NXT;j++){
    totMCweight+= truew[j];
    cout<<" True Weight "<<j<<"="<<truew[j]<<endl;
  }
  if(fabs(totMCweight-1.)>1e-5){cout<<" MC weights do not add up to 1."<<endl; return 0;}

  //compute the true reweighted efficiency for each dalitz region
  Float_t TotEff[3]={0.,0.,0.};
  Float_t TotEffErr[3]={0.,0.,0.};
  cout<<" $\\sum w^{true}_j\\epsilon_d^j$";
  for(Int_t d=0;d<3;d++){
    for(Int_t j=0;j<NXT;j++){  
      TotEff[d]+= truew[j]*Eff[d][j];
      TotEffErr[d]+= pow(truew[j]*EffErr[d][j],2);
    }
    TotEffErr[d]=sqrt(TotEffErr[d]);
    
    printf(" & %.3f $\\pm$ %.3f",TotEff[d],TotEffErr[d]);
  }
  printf(" \\\\ \n");

  cout<<endl;
  //------------------------------
  //reconstructed nXT  weights
  //------------------------------  
  if(FDenomFitResults->IsZombie()){cout<<"Bad DsFit File"<<endl; return 0;} gROOT->cd();
  RooFitResult* DsFitResult=(RooFitResult*)FDenomFitResults->Get("fitresult");
  if(!DsFitResult){cout<<"No DsFitResult"<<endl;return 0;}
  RooRealVar* nXTbFit=NULL;
  RooRealVar* nXTgFit=NULL;
  TH1F*HnXTWMPars=(TH1F*)FDenomFitResults->Get("HnXTWMPars");
  //if(!HnXTWMPars){cout<<"HnXTWMPars not found"<<endl;return 0;}
  if(HnXTWMPars)//value of nXTa was changed from its  nominal value --> must be set properly here 
    nXTa->setVal(HnXTWMPars->GetBinContent(1));
  //nXTaFit=((RooRealVar*)DsFitResult->floatParsFinal().find("nXTa"));
  //if(!nXTaFit){cout<<"nXT pars  not found"<<endl;return 0;}
  nXTbFit=((RooRealVar*)DsFitResult->floatParsFinal().find("nXTb"));
  if(!nXTbFit){cout<<"nXTb par not found"<<endl;return 0;}
  nXTb->setVal(nXTbFit->getVal());
  nXTb->setError(nXTbFit->getError());
  nXTgFit=((RooRealVar*)DsFitResult->floatParsFinal().find("nXTg"));
  if(!nXTgFit){cout<<"nXTg par not found"<<endl;return 0;}
  nXTg->setVal(nXTgFit->getVal());
  nXTg->setError(nXTgFit->getError());   

  Float_t totDataweight=0.;
  for(Int_t j=0;j<NXT;j++){    
    totDataweight+=nXTCoefWM[j]->getVal();
    cout<<" FitWeight"<<j<<"="<<nXTCoefWM[j]->getVal()<<endl;
  }
  //----------------------  
  if(fabs(totDataweight-1.)>1e-5){cout<<" Fit weights do not add up to 1."<<endl; return 0;}
  

  //------------------------------
  //Compute the reconstructed weighted efficiency
  //------------------------------ 
  cout<<" $\\sum w^{fit}_j\\epsilon_d^j$";
  for(Int_t d=0;d<3;d++){
    TotEff[d]=0.;
    TotEffErr[d]=0.;
    for(Int_t j=0;j<NXT;j++){  
      TotEff[d]+= nXTCoefWM[j]->getVal()*Eff[d][j];    
      //TotEff[d]+= truew[j]*Eff[d][j];cout<<"!!!!!!!!!!!!Using true weight"<<endl;
      TotEffErr[d]+= pow(nXTCoefWM[j]->getVal()*EffErr[d][j],2);
    }
    TotEffErr[d]=sqrt(TotEffErr[d]);

    //TotEff[d]=TotKKpiRec[d]*TotDsProd/(TotKKpiProd[d]*TotDsRec); cout<<"!!!!!!!!!!Using Efficiency without nXT reweighting "<<TotEff[d]<<endl;
    printf(" & %.3f $\\pm$ %.3f",TotEff[d],TotEffErr[d]);
  }
  printf(" \\\\ \n");


  ///Dalitz reweighting  
  if(FDalitzWeights->IsZombie()){cout<<"Bad FDalitzWeight File"<<endl; return 0;} gROOT->cd();
  Float_t dalitzweight[3];
  Float_t totDalitzweight=0.;
  Float_t TotKKpiEff=0.;
  for(Int_t d=0;d<3;d++){
    TH1F*HDalitzWeight=NULL;
    if(_DataType==1)HDalitzWeight=(TH1F*)FDalitzWeights->Get(TString("HDsMassMCDalitz")+(long)d);
    if(_DataType==2)HDalitzWeight=(TH1F*)FDalitzWeights->Get(TString("HDsMassDataDalitz")+(long)d);
    if(!HDalitzWeight)cout<<"Bad Dalitz Histo"<<d<<endl;
    dalitzweight[d]=HDalitzWeight->GetBinContent(0);//ignore weight errors
    totDalitzweight+= dalitzweight[d];
    TotKKpiEff += dalitzweight[d]*(1./TotEff[d]);
    cout<<" Dalitz weight "<<d<<" ="<<dalitzweight[d]<<endl;
  }
  if(fabs(totDalitzweight-1.)>1e-5){cout<<" Dalitz weights do not add up to 1."<<endl; return 0;}

  cout<<" The reweighted Dalitz efficiency is "<<1/TotKKpiEff<<endl;



  cout<<endl;
  //------------------------------
  ///Ds yield 
  //------------------------------ 
  TH2F*HDsMassData=(TH2F*)FDenomFitResults->Get("HDsMassData");
  if(!HDsMassData){cout<<"No HDsMassData "<<endl;return 0;}
  RooRealVar*DsYield=(RooRealVar*)DsFitResult->floatParsFinal().find("TotYield");
  if(!DsYield){cout<<"DsYield not found"<<endl;return 0;}
  TotYield.setVal(DsYield->getVal());
  TotYield.setError(DsYield->getError());
  Float_t NRecoDs=HDsMassData->Integral()*TotYield.getVal();
  //TFile FDenominatorTrueYield("/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResults/DenominatorEff.root","r");
  //NRecoDs=((TH1F*)FDenominatorTrueYield.Get("HDsMassTrue"))->Integral();cout<<"!!!!!!!!!!!!Using true Ds yield"<<endl;
  Float_t NRecoDsErr=HDsMassData->Integral()*TotYield.getError();
  cout<<"The inclusive Ds yield is "<<NRecoDs<<" +- "<<NRecoDsErr<<" -->"<<NRecoDsErr*100./NRecoDs<<"%"<<endl;
  



  //------------------------------
  //Numerator yield
  //------------------------------
  if(FNumFitResults->IsZombie()){cout<<"Bad NuFit File"<<endl; return 0;} gROOT->cd();
  RooFitResult* NumeratorFitResult=(RooFitResult*)FNumFitResults->Get(TString("FitResult"));
  if(!NumeratorFitResult){cout<<"No NumeratorFitResult"<<endl;return 0;}
  TH1F* HNumeratorData=(TH1F*)FNumFitResults->Get(TString("HKKpiGamMass"));
  if(!HNumeratorData){cout<<"No HNumeratorData "<<endl;return 0;}
  RooRealVar*NumeratorYield=(RooRealVar*)NumeratorFitResult->floatParsFinal().find(TString("SigYield"));
  if(!NumeratorYield){cout<<"NumeratorYield not found"<<endl;return 0;} 
  Float_t NRecoNumerator=NumeratorYield->getVal()*HNumeratorData->Integral();
  //TFile FNumeratorTrueYield("DsTolnu/KKpi/SP1005/HistosForFit.root","r");
  //NRecoNumerator=((TH1F*)FNumeratorTrueYield.Get("HKKpiGamMassSignal"))->Integral();cout<<"!!!!!!Using truth Numerator"<<endl;
  ////NRecoNumerator=((TH1F*)FNumeratorTrueYield.Get("HKKpiGamMassTrue"))->Integral();cout<<"!!!!!!!!!!!!Using true Numerator"<<endl;
  Float_t NRecoNumeratorErr=NumeratorYield->getError()*HNumeratorData->Integral();
  cout<<"Numerator yield= "<<NRecoNumerator<<" +- "<<NRecoNumeratorErr
      <<"  -->"<<NRecoNumeratorErr*100./NRecoNumerator<<"%"<<endl;  
  


  cout<<endl;
  //---------------------------
  //Compute the Branching fraction
  //----------------------------
  Float_t BF=(NRecoNumerator/NRecoDs)*TotKKpiEff;
  cout<<"-----------------------------------------------"<<endl;
  cout<<" The Branching fraction is = "<<setprecision (4)<<fixed<<BF*100<<endl;
  cout<<"-----------------------------------------------"<<endl;
  
  
  return BF;
}


// Float_t DRecoDsInclusiveAnalysis::ComputeBF(TFile* FDenomFitResults,TFile* FNumFitResults,TFile* FEffDemon,TFile* FEffNum){
 
//   //------------------------------
//   //neutrino yield
//   //------------------------------
//   if(FNumFitResults->IsZombie()){cout<<"Bad NuFit File"<<endl; return 0;} gROOT->cd();
//   RooFitResult* NuFitResult=(RooFitResult*)FNumFitResults->Get("fitResultsDsToMuNuRunAll");
//   if(!NuFitResult){cout<<"No NuFitResult"<<endl;return 0;}
//   RooRealVar*NuYield=(RooRealVar*)NuFitResult->floatParsFinal().find("fSig");
//   if(!NuYield){cout<<"fSig not found"<<endl;return 0;}
//   TH2F*HNuMassData=(TH2F*)FNumFitResults->Get("hmm2MuonRunAll_EExtraCut");
//   if(!HNuMassData){cout<<"No HNuMassData "<<endl;return 0;}
//   cout<<"extracted MuNu Data: "<<HNuMassData->GetName()<<endl;
//   Float_t NRecoNu=NuYield->getVal()*HNuMassData->Integral();
//   Float_t NRecoNuErr=NuYield->getError()*HNuMassData->Integral();

// //    TFile FNumTrue("/u/br/aidanrc/ana50/AWG90/aidanrc/Roofit/DsToMuNuTrueSignalRunAll_EExtraCut.root","read");
// //    TTree*TNumTrue=(TTree*)FNumTrue.Get("tDsToMuNuTrueSignalRunAll_EExtraCut");
// //    TH1F HNumTrue("HNumTrue","",300,-1,4.);
// //    TNumTrue->Draw("mm2>>HNumTrue","weight");
// //    NRecoNu=HNumTrue.Integral();
// //    cout<<"!!!!!!!!!!!Using true MuNu Yield!!!!!!!!!!!!!"<<endl;

//   cout<<"The total neutrino yield is "<<NRecoNu<<" +- "<<NuYield->getError()*HNuMassData->Integral()
//       <<" --> "<<NRecoNuErr*100./NRecoNu<<"%"<<endl;
  
   
//   cout<<endl;
//   //------------------------------
//   //muon efficiencies
//   //------------------------------
//   if(FEffNum->IsZombie()){cout<<"Bad FMuEff File"<<endl; return 0;} gROOT->cd();
//   if(FEffDemon->IsZombie()){cout<<"Bad FDsEff File"<<endl; return 0;} gROOT->cd();
//   Float_t MuEff[NXT];
//   Float_t MuEffErr[NXT];

//   //declare text files with produced Ds and Muon yields as a function of nXT
//   ifstream DsProducedFile; 
//   DsProducedFile.open("/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResults/DsProduced.txt");
//   ifstream MuNuProducedFile; 
//   MuNuProducedFile.open("/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResults/MuNuProduced.txt");
  
//   Float_t TotDsProd=0;
//   Float_t TotDsRec=0;
//   Float_t TotNuProd=0;
//   Float_t TotNuRec=0;
//   Float_t DsProd[7];
//   Float_t NuProd[7];
//   Float_t DsRec[7];
//   Float_t NuRec[7];
//   Float_t DsRecErr[7];
//   Float_t NuRecErr[7];
//   Float_t GennXTCorrection[7]={1.093,1.003,1.000,1.009,0.999,0.990,0.981};//correct for discrepancy between gen. Ds nXT weights ans gen MuNu weights.

//   for(Int_t j=0;j<NXT;j++){
//     //Denominators
//     Int_t indexcheck;
//     Float_t NProdDs=0.;
//     DsProducedFile>>indexcheck>>NProdDs; 
//     if(indexcheck!=j){cout<<" NXT index does not match for Ds produced   j="<<j<<endl;exit(0);}
//     Float_t NProdMuNu=0.;
//     MuNuProducedFile>>indexcheck>>NProdMuNu;
//     if(indexcheck!=j){cout<<" NXT index does not match for MuNu produced   j="<<j<<endl;exit(0);}

//     DsProd[j]=NProdDs;
//     NuProd[j]=NProdMuNu;
//     TotDsProd+=NProdDs;   
//     TotNuProd+=NProdMuNu;

//     //Numerators
//     TH1F*HMuEff=(TH1F*)FEffNum->Get(TString("hNXT")+(long)j+"DsToMuNu_EExtraCut");
//     if(!HMuEff){cout<<"No HMuEff"<<j<<endl;return 0;}
//     TH2F*HDsEff=(TH2F*)FEffDemon->Get(TString("HDsMassTrue2D_nXT")+(long)j);
//     if(!HDsEff){cout<<"No HDsEff"<<j<<endl;return 0;}

//     DsRec[j]=HDsEff->Integral(1,4,DenominatorDsMassBinMin,DenominatorDsMassBinMax);
//     NuRec[j]=HMuEff->Integral();
//     DsRecErr[j]=sqrt(HDsEff->Integral(1,4,DenominatorDsMassBinMin,DenominatorDsMassBinMax));
//     NuRecErr[j]=sqrt(HMuEff->Integral());
		     
//     TotDsRec+=HDsEff->Integral(1,4,DenominatorDsMassBinMin,DenominatorDsMassBinMax);
//     TotNuRec+=HMuEff->Integral();
    
//   }
//   Float_t TotNuGenCorr=0.;
//   for(Int_t j=0;j<NXT;j++){
//     NuProd[j]/=GennXTCorrection[j];//reweight
//     TotNuGenCorr+=NuProd[j];
//   }
//   for(Int_t j=0;j<NXT;j++)
//     NuProd[j]*=(TotNuProd/TotNuGenCorr);//rescale to original number

//   Float_t TotNuRecCorr=0.;
//   for(Int_t j=0;j<NXT;j++){
//     NuRec[j]/=GennXTCorrection[j];//reweight reco as well
//     TotNuRecCorr+=NuRec[j];
//   }
//   for(Int_t j=0;j<NXT;j++)
//     NuRec[j]*=(TotNuRec/TotNuRecCorr);//rescale to original number

//   cout<<" nXT efficiencies: "<<endl;
//   for(Int_t j=0;j<NXT;j++){ 
//     MuEff[j]=(_DataType==2 ? 0.999837 : 1. )*(NuRec[j]*DsProd[j]/(NuProd[j]*DsRec[j]));
//     //correct for Ds Mass peak shift in Data
//     MuEffErr[j]=(_DataType==2 ? 0.999837 : 1. )*MuEff[j]*sqrt(pow(NuRecErr[j]/NuRec[j],2) + pow(DsRecErr[j]/DsRec[j],2));
//     cout<<setprecision(3)<<fixed<<" & "<<MuEff[j]<<" $\\pm$ "<<MuEffErr[j];
//   }
    
//   cout<<" \\\\"<<endl;
//   cout<<scientific<<"The Ds efficiency is "<<TotDsRec/TotDsProd<<endl;
//   cout<<scientific<<"The MuNu efficiency is "<<TotNuRec/TotNuProd<<endl;
  

//   /////True MC Weights ------------------------------
//   Float_t totMCweight=0.;
//   for(Int_t j=0;j<NXT;j++){
//     totMCweight+= truew[j];
//     cout<<" MCweight"<<j<<"="<<truew[j]<<endl;
//   }
//   if(fabs(totMCweight-1.)>1e-5){cout<<" MC weights do not add up to 1."<<endl; return 0;}
//   Float_t TotMuEff=0.;
//   for(Int_t j=0;j<NXT;j++){  
//     TotMuEff+= truew[j]*MuEff[j];
//   }
//   cout<<"The MC true weighted muon efficiency is "<<TotMuEff<<endl;

//   cout<<endl;
//   //------------------------------
//   ///Ds yield 
//   //------------------------------
//   if(FDenomFitResults->IsZombie()){cout<<"Bad DsFit File"<<endl; return 0;} gROOT->cd();
//   RooFitResult* DsFitResult=(RooFitResult*)FDenomFitResults->Get("fitresult");
//   if(!DsFitResult){cout<<"No DsFitResult"<<endl;return 0;}
//   TH2F*HDsMassData=(TH2F*)FDenomFitResults->Get("HDsMassData");
//   if(!HDsMassData){cout<<"No HDsMassData "<<endl;return 0;}
//   RooRealVar*DsYield=(RooRealVar*)DsFitResult->floatParsFinal().find("TotYield");
//   if(!DsYield){cout<<"DsYield not found"<<endl;return 0;}
//   TotYield.setVal(DsYield->getVal()); 
//   TotYield.setError(DsYield->getError());
//   TH2F* TotSigPdfHistoFit=(TH2F*)FDenomFitResults->Get("TotSigPdfHistoFit"); if(!TotSigPdfHistoFit)cout<<"No TotSigPdfHistoFit"<<endl;
//   Float_t NRecoDs=HDsMassData->Integral()*TotYield.getVal()
//     *TotSigPdfHistoFit->Integral(1,4,DenominatorDsMassBinMin,DenominatorDsMassBinMax)/TotSigPdfHistoFit->Integral();
//   Float_t NRecoDsErr=HDsMassData->Integral()*TotYield.getError()
//     *TotSigPdfHistoFit->Integral(1,4,DenominatorDsMassBinMin,DenominatorDsMassBinMax)/TotSigPdfHistoFit->Integral();

// //   //use true Ds yield  
// //   TFile FDenomTrue("/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResults/DenominatorEff.root","read");
// //   NRecoDs=((TH1F*)FDenomTrue.Get("HDsMassTrue"))->Integral(DenominatorDsMassBinMin,DenominatorDsMassBinMax);
// //  cout<<"!!!!!!!!!!!Using true Ds Yield!!!!!!!!!!!!!"<<endl;

//   cout<<"The total Ds yield is "<<NRecoDs<<" +- "<<NRecoDsErr<<" --> "<<NRecoDsErr*100./NRecoDs<<"%"<<endl;  
//   cout<<endl;
  
// //   //for MC lumi weight the Ds yield because Aidan has lumi weighted his MC
// //   if(_DataType==1){    
// //     NRecoDs=NRecoDs*(1./2.04+.0974/1.93)/(1+.0974);
// //     NRecoDsErr=NRecoDsErr*(1./2.04+.0974/1.93)/(1+.0974);
// //   }
// //   cout<<"The total Ds yield after Lumi Weighting is "<<NRecoDs<<" +- "<<NRecoDsErr<<" --> "<<NRecoDsErr*100./NRecoDs<<"%"<<endl;  
// //   cout<<endl;

//   //------------------------------
//   //Fitted nXT  weights
//   //------------------------------  
//   //RooRealVar* nXTaFit=NULL;
//   RooRealVar* nXTbFit=NULL;
//   RooRealVar* nXTgFit=NULL;
//   TH1F*HnXTWMPars=(TH1F*)FDenomFitResults->Get("HnXTWMPars");
//   //if(!HnXTWMPars){cout<<"HnXTWMPars not found"<<endl;return 0;}
//   if(HnXTWMPars)//value of nXTa was changed from its  nominal value --> must be set properly here 
//     nXTa->setVal(HnXTWMPars->GetBinContent(1));
//   //nXTaFit=((RooRealVar*)DsFitResult->floatParsFinal().find("nXTa"));
//   //if(!nXTaFit){cout<<"nXT pars  not found"<<endl;return 0;}
//   nXTbFit=((RooRealVar*)DsFitResult->floatParsFinal().find("nXTb"));
//   if(!nXTbFit){cout<<"nXTb par not found"<<endl;return 0;}
//   nXTb->setVal(nXTbFit->getVal());
//   nXTb->setError(nXTbFit->getError());
//   nXTgFit=((RooRealVar*)DsFitResult->floatParsFinal().find("nXTg"));
//   if(!nXTgFit){cout<<"nXTg par not found"<<endl;return 0;}
//   nXTg->setVal(nXTgFit->getVal());
//   nXTg->setError(nXTgFit->getError());
  
//   //  cout<<setprecision(6)<<"nXTb="<<nXTb->getVal()<<"+-"<<nXTb->getError()<<"    nXTg="<<nXTg->getVal()<<"+-"<<nXTg->getError()<<endl;

//   TH1F* HnXTWeights=(TH1F*)FDenomFitResults->Get("HnXTweights");
//   Float_t totDataweight=0.;
//   for(Int_t j=0;j<NXT;j++){
//     totDataweight+=nXTCoefWM[j]->getVal();//note that these coefficients are being updated as long as the first three are updated
//     cout<<" FitWeight "<<j<<"="<<nXTCoefWM[j]->getVal()<<"   hist="<<HnXTWeights->GetBinContent(j+1)<<endl;
//   }
//   if(fabs(totDataweight-1.)>1e-5){cout<<" Fit weights do not add up to 1."<<endl; return 0;}
  

//   //------------------------------
//   //Compute the Data weighted efficiency
//   //------------------------------
//   TotMuEff=0.;
//   for(Int_t j=0;j<NXT;j++)
//     TotMuEff+= nXTCoefWM[j]->getVal()*MuEff[j];   
  
// //   //use true weights
// //   cout<<"!!!!!!!!!!Using true weights!!!!!!!!!!"<<endl;
// //   TotMuEff=0.;
// //   for(Int_t j=0;j<NXT;j++){
// //     TotMuEff+= truew[j]*MuEff[j];
// //   }

//   cout<<"The reweighted muon efficiency is "<<TotMuEff<<endl;

//   //Use total efficiency 
// //   TotMuEff=(TotNuRec*TotDsProd)/(TotNuProd*TotDsRec);
// //   cout<<" !!!!!!!!!!!!!Using Total Efficiency (No nXT reweighting)!!!!!!!!    = "<<TotMuEff<<endl;

//   cout<<endl;
//   //---------------------------
//   //Compute the Branching fraction
//   //----------------------------
//   Float_t BF=NRecoNu/(NRecoDs*TotMuEff);
//   cout<<"-----------------------------------------------"<<endl;
//   cout<<fixed<<" The Branching fraction is = "<<BF*1000<<"\\times 10^{-3} "<<endl;
//   cout<<"-----------------------------------------------"<<endl;
  
//   //  return BF;

//   cout<<endl;
//   //----------------------------------
//   //Compute the error
//   //----------------------------------

//   //error on Denominator: DsYield*( weighted MuEfficiency)
//   //set these pointers for later calculations
//   RooRealVar* FitVar[3];
//   FitVar[0]=&TotYield;
//   FitVar[1]=nXTb;
//   FitVar[2]=nXTg;

//   //read in the full correlation matrix
//   cout<<"Covariance Matrix:"<<endl;
//   Float_t CovMatrix[3][3];
//   for(Int_t j=0;j<3;j++){
//     cout<<FitVar[j]->GetName()<<"     ";
//     for(Int_t k=0;k<3;k++){
//       CovMatrix[j][k]=DsFitResult->correlation(*FitVar[j],*FitVar[k]) * (FitVar[j]->getError()) * (FitVar[k]->getError());
//       cout<<scientific<<" ("<<CovMatrix[j][k]<<")";
//     }
//     cout<<endl;
//   }
  
//   //Compute derivatives w.r.t the floated parameters

//   Float_t DerivWeightB[NXT];
//   Float_t DerivWeightG[NXT];
//   Float_t SumW=0.;
//   Float_t SumB=0.;
//   Float_t SumG=0.;
//   for(Int_t k=0;k<NXT;k++){
//     SumW+=pow(k-nXTa->getVal(),nXTb->getVal())*exp(-nXTg->getVal()*k);
//     SumB+=pow(k-nXTa->getVal(),nXTb->getVal()-1)*exp(-nXTg->getVal()*k);
//     SumG+=k*pow(k-nXTa->getVal(),nXTb->getVal())*exp(-nXTg->getVal()*k);
//   }
//   for(Int_t k=0;k<NXT;k++){
//     DerivWeightB[k]=nXTCoefWM[k]->getVal()*nXTb->getVal()*(1./(k-nXTa->getVal()) - SumB/SumW);
//     DerivWeightG[k]=nXTCoefWM[k]->getVal()*(SumG/SumW-k);
//   }

//   Float_t DerivDenom[3];
//   DerivDenom[0]=TotMuEff;
//   DerivDenom[1]=0.;
//   DerivDenom[2]=0.;
//   for(Int_t k=0;k<NXT;k++){
//     DerivDenom[1]+=TotYield.getVal()*DerivWeightB[k]*MuEff[k];
//     DerivDenom[2]+=TotYield.getVal()*DerivWeightG[k]*MuEff[k];
//   }

//   cout<<"Correlated error matrix:"<<endl;
//   //add the correlated errors
//   Float_t TotDenomErr=0.;
//   for(Int_t j=0;j<3;j++){
//     for(Int_t k=0;k<3;k++){
//       cout<<scientific<<"   "<<DerivDenom[j]*DerivDenom[k]*CovMatrix[j][k];
//       TotDenomErr+=DerivDenom[j]*DerivDenom[k]*CovMatrix[j][k];
//     }
//     cout<<endl;
//   }  
//   //add the un-correlated errors due to the individual efficiencies
//   for(Int_t j=0;j<NXT;j++)
//     TotDenomErr+=pow(TotYield.getVal()*nXTCoefWM[j]->getVal()*MuEffErr[j],2);
  
//   TotDenomErr=sqrt(TotDenomErr);
//   cout<<fixed<<scientific<<" The BF denominator is = "<<TotYield.getVal()*HDsMassData->Integral()*TotMuEff
//       <<" +- "<<TotDenomErr*HDsMassData->Integral()
//       <<"  -->"<<100.*TotDenomErr/(TotYield.getVal()*TotMuEff)<<"%"<<endl;
 
  
//   //----------------------------------
//   ///add error on Numerator yield + total error on denominator*efficiency
//   //---------------------------------
//   Float_t BFErr=BF*sqrt(pow(NRecoNuErr/NRecoNu,2) +  pow(TotDenomErr/(TotYield.getVal()*TotMuEff),2));
//   cout<<"-----------------------------------------------"<<endl;
//   cout<<fixed<<" The Branching fraction Error is ="<<BFErr*1000<<"\\times 10^{-3}    -->"<<100.*BFErr/BF<<"%"<<endl;
//   cout<<"-----------------------------------------------"<<endl;


//   return BF;
// }

*/
// Float_t DRecoDsInclusiveAnalysis::ComputeBF(TFile* FDenomFitResults,TFile* FNumFitResults,TFile* FEffDemon,TFile* FEffNum){
 
//   //------------------------------
//   //neutrino yield
//   //------------------------------
//   if(FNumFitResults->IsZombie()){cout<<"Bad NuFit File"<<endl; return 0;} gROOT->cd();
//   RooFitResult* NuFitResult=(RooFitResult*)FNumFitResults->Get("fitResultsDsToMuNuRunAll");
//   if(!NuFitResult){cout<<"No NuFitResult"<<endl;return 0;}
//   RooRealVar*NuYield=(RooRealVar*)NuFitResult->floatParsFinal().find("fSig");
//   if(!NuYield){cout<<"fSig not found"<<endl;return 0;}
//   TH2F*HNuMassData=(TH2F*)FNumFitResults->Get("hmm2MuonRunAll_EExtraCut");
//   if(!HNuMassData){cout<<"No HNuMassData "<<endl;return 0;}
//   cout<<"extracted MuNu Data: "<<HNuMassData->GetName()<<endl;
//   Float_t NRecoNu=NuYield->getVal()*HNuMassData->Integral();
//   Float_t NRecoNuErr=NuYield->getError()*HNuMassData->Integral();

//    TFile FNumTrue("/u/br/aidanrc/ana50/AWG90/aidanrc/Roofit/trueSigMuonRunAll_EExtraCut.root","read");
//    TTree*TNumTrue=(TTree*)FNumTrue.Get("ttrueSigMuonRunAll_EExtraCut");
//    TH1F HNumTrue("HNumTrue","",300,-1,4.);
//    TNumTrue->Draw("mm2>>HNumTrue","weight");
//    NRecoNu=HNumTrue.Integral();
//    cout<<"!!!!!!!!!!!Using true MuNu Yield!!!!!!!!!!!!!"<<endl;

//   cout<<"The total neutrino yield is "<<NRecoNu<<" +- "<<NuYield->getError()*HNuMassData->Integral()
//       <<" --> "<<NRecoNuErr*100./NRecoNu<<"%"<<endl;
  
   
//   cout<<endl;
//   //------------------------------
//   //muon efficiencies
//   //------------------------------
//   if(FEffNum->IsZombie()){cout<<"Bad FMuEff File"<<endl; return 0;} gROOT->cd();
//   if(FEffDemon->IsZombie()){cout<<"Bad FDsEff File"<<endl; return 0;} gROOT->cd();
//   Float_t MuEff[NXT];
//   Float_t MuEffErr[NXT];

//   //declare text files with produced Ds and Muon yields as a function of nXT
//   ifstream DsProducedFile; 
//   DsProducedFile.open("/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResults/DsProduced.txt");
//   ifstream MuNuProducedFile; 
//   MuNuProducedFile.open("/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResults/MuNuProduced.txt");
  
//   Float_t TotDsProd=0;Float_t TotDsRec=0;

//   cout<<" nXT efficiencies: "<<endl;
//   for(Int_t j=0;j<NXT;j++){
//     //Denominators
//     Int_t indexcheck;
//     Float_t NProdDs=0.;
//     DsProducedFile>>indexcheck>>NProdDs; 
//     if(indexcheck!=j){cout<<" NXT index does not match for Ds produced   j="<<j<<endl;exit(0);}
//     Float_t NProdMuNu=0.;
//     MuNuProducedFile>>indexcheck>>NProdMuNu;
//     if(indexcheck!=j){cout<<" NXT index does not match for MuNu produced   j="<<j<<endl;exit(0);}

//     //Numerators
//     TH1F*HMuEff=(TH1F*)FEffNum->Get(TString("hNXT")+(long)j+"DsToMuNu_EExtraCut");
//     if(!HMuEff){cout<<"No HMuEff"<<j<<endl;return 0;}
//     TH2F*HDsEff=(TH2F*)FEffDemon->Get(TString("HDsMassTrue2D_nXT")+(long)j);
//     if(!HDsEff){cout<<"No HDsEff"<<j<<endl;return 0;}
//     MuEff[j]=(_DataType==2 ? 0.999837 : 1. )*(HMuEff->Integral()*NProdDs
// 					      /(NProdMuNu*HDsEff->Integral(1,4,DenominatorDsMassBinMin,DenominatorDsMassBinMax)));
//     //correct for Ds Mass peak shift in Data
//     MuEffErr[j]=(_DataType==2 ? 0.999837 : 1. )*MuEff[j]*sqrt(pow(sqrt(HMuEff->Integral())/HMuEff->Integral(),2)
// 			      + pow(sqrt(HDsEff->Integral(1,4,DenominatorDsMassBinMin,DenominatorDsMassBinMax))
// 				    /HDsEff->Integral(1,4,DenominatorDsMassBinMin,DenominatorDsMassBinMax),2));
//     cout<<setprecision(3)<<fixed<<" & "<<MuEff[j]<<" $\\pm$ "<<MuEffErr[j];

    
//     TotDsProd+=NProdDs;
//     TotDsRec+=HDsEff->Integral();//1,4,DenominatorDsMassBinMin,DenominatorDsMassBinMax
//   }
//   cout<<" \\\\"<<endl;
//   cout<<scientific<<"The Ds efficiency in the full range is "<<TotDsRec/TotDsProd<<endl;
  

//   /////True MC Weights ------------------------------
//   Float_t totMCweight=0.;
//   for(Int_t j=0;j<NXT;j++){
//     totMCweight+= truew[j];
//     cout<<" MCweight"<<j<<"="<<truew[j]<<endl;
//   }
//   if(fabs(totMCweight-1.)>1e-5){cout<<" MC weights do not add up to 1."<<endl; return 0;}
//   Float_t TotMuEff=0.;
//   for(Int_t j=0;j<NXT;j++){  
//     TotMuEff+= truew[j]*MuEff[j];
//   }
//   cout<<"The MC true weighted muon efficiency is "<<TotMuEff<<endl;

//   cout<<endl;
//   //------------------------------
//   ///Ds yield 
//   //------------------------------
//   if(FDenomFitResults->IsZombie()){cout<<"Bad DsFit File"<<endl; return 0;} gROOT->cd();
//   RooFitResult* DsFitResult=(RooFitResult*)FDenomFitResults->Get("fitresult");
//   if(!DsFitResult){cout<<"No DsFitResult"<<endl;return 0;}
//   TH2F*HDsMassData=(TH2F*)FDenomFitResults->Get("HDsMassData");
//   if(!HDsMassData){cout<<"No HDsMassData "<<endl;return 0;}
//   RooRealVar*DsYield=(RooRealVar*)DsFitResult->floatParsFinal().find("TotYield");
//   if(!DsYield){cout<<"DsYield not found"<<endl;return 0;}
//   TotYield.setVal(DsYield->getVal()); 
//   TotYield.setError(DsYield->getError());
//   TH2F* TotSigPdfHistoFit=(TH2F*)FDenomFitResults->Get("TotSigPdfHistoFit"); if(!TotSigPdfHistoFit)cout<<"No TotSigPdfHistoFit"<<endl;
//   Float_t NRecoDs=HDsMassData->Integral()*TotYield.getVal()
//     *TotSigPdfHistoFit->Integral(1,4,DenominatorDsMassBinMin,DenominatorDsMassBinMax)/TotSigPdfHistoFit->Integral();
//   Float_t NRecoDsErr=HDsMassData->Integral()*TotYield.getError()
//     *TotSigPdfHistoFit->Integral(1,4,DenominatorDsMassBinMin,DenominatorDsMassBinMax)/TotSigPdfHistoFit->Integral();

//   //use true Ds yield  
//   TFile FDenomTrue("DsTolnu/SP1005Skim/DsReco/Final/HistosForFit.root","read");
//   NRecoDs=((TH1F*)FDenomTrue.Get("HDsMassTrue"))->Integral(DenominatorDsMassBinMin,DenominatorDsMassBinMax);
//   cout<<"!!!!!!!!!!!Using true Ds Yield!!!!!!!!!!!!!"<<endl;

//   cout<<"The total Ds yield is "<<NRecoDs<<" +- "<<NRecoDsErr<<" --> "<<NRecoDsErr*100./NRecoDs<<"%"<<endl;  
//   cout<<endl;
  
// //   //for MC lumi weight the Ds yield because Aidan has lumi weighted his MC
// //   if(_DataType==1){    
// //     NRecoDs=NRecoDs*(1./2.04+.0974/1.93)/(1+.0974);
// //     NRecoDsErr=NRecoDsErr*(1./2.04+.0974/1.93)/(1+.0974);
// //   }
// //   cout<<"The total Ds yield after Lumi Weighting is "<<NRecoDs<<" +- "<<NRecoDsErr<<" --> "<<NRecoDsErr*100./NRecoDs<<"%"<<endl;  
// //   cout<<endl;

//   //------------------------------
//   //Fitted nXT  weights
//   //------------------------------  
//   Float_t totDataweight=0.;
//   for(Int_t j=0;j<3;j++){
//     if(!DsFitResult->floatParsFinal().find(TString("nXTCoef")+(long)j)){cout<<"nXTCoef not found"<<endl;return 0;}
//     nXTCoef[j]->setVal(((RooRealVar*)(DsFitResult->floatParsFinal().find(TString("nXTCoef")+(long)j)))->getVal());    
//     nXTCoef[j]->setError(((RooRealVar*)(DsFitResult->floatParsFinal().find(TString("nXTCoef")+(long)j)))->getError());    
//     totDataweight+=nXTCoef[j]->getVal();
//     cout<<" FitWeight"<<j<<"="<<nXTCoef[j]->getVal()<<" +- "<<nXTCoef[j]->getError()<<endl;
//   }  
//   for(Int_t j=3;j<NXT;j++){    
//     totDataweight+=nXTCoefWM[j]->getVal();//note that these coefficients are being updated as long as the first three are updated
//     cout<<" FitWeight"<<j<<"="<<nXTCoefWM[j]->getVal()<<endl;
//   }
//   if(fabs(totDataweight-1.)>1e-5){cout<<" Fit weights do not add up to 1."<<endl; return 0;}
  

//   //------------------------------
//   //Compute the Data weighted efficiency
//   //------------------------------
//   TotMuEff=0.;
//   for(Int_t j=0;j<3;j++)
//     TotMuEff+= nXTCoef[j]->getVal()*MuEff[j];
//   for(Int_t j=3;j<NXT;j++)
//     TotMuEff+= nXTCoefWM[j]->getVal()*MuEff[j];    
  
//   //use true weights
//   TotMuEff=0.;
//   for(Int_t j=0;j<NXT;j++){
//     TotMuEff+= truew[j]*MuEff[j];
//     cout<<"!!!!!!!!!!Using true weights!!!!!!!!!!"<<endl;
//   }

//   cout<<"The reweighted muon efficiency is "<<TotMuEff<<endl;

// //    //Use total efficiency 
// //    Float_t totDsRec=0.;
// //    Float_t totMuNuRec=0.;
// //    for(Int_t j=0;j<NXT;j++){
// //      TH1F*HMuEff=(TH1F*)FEffNum->Get(TString("hNXT")+(long)j+"DsToMuNu_EExtraCut");
// //      if(!HMuEff){cout<<"No HMuEff"<<j<<endl;return 0;}
// //      TH2F*HDsEff=(TH2F*)FEffDemon->Get(TString("HDsMassTrue2D_nXT")+(long)j);
// //      if(!HDsEff){cout<<"No HDsEff"<<j<<endl;return 0;} 
// //      totDsRec+=HDsEff->Integral(1,4,DenominatorDsMassBinMin,DenominatorDsMassBinMax);
// //      totMuNuRec+=HMuEff->Integral();
// //    }
// //    TString TotDsWord; Float_t totDsProd=0.;
// //    TString TotMuNuWord; Float_t totMuNuProd=0.;
// //    DsProducedFile>>TotDsWord>>totDsProd; 
// //    MuNuProducedFile>>TotMuNuWord>>totMuNuProd;
// //    if(TotDsWord!="Total" || TotDsWord!="Total"){cout<<" Read bad Total produced "<<endl;return 0.;}
// //    TotMuEff=(totMuNuRec*totDsProd)/(totMuNuProd*totDsRec);
// //    cout<<" !!!!!!!!!!!!!Using Total Efficiency (No nXT reweighting)!!!!!!!!    = "<<TotMuEff<<endl;

//   cout<<endl;
//   //---------------------------
//   //Compute the Branching fraction
//   //----------------------------
//   Float_t BF=NRecoNu/(NRecoDs*TotMuEff);
//   cout<<"-----------------------------------------------"<<endl;
//   cout<<fixed<<" The Branching fraction is = "<<BF*1000<<"\\times 10^{-3} "<<endl;
//   cout<<"-----------------------------------------------"<<endl;
  
  
//   cout<<endl;
//   //----------------------------------
//   //Compute the error
//   //----------------------------------

//   //error on Denominator: DsYield*( weighted MuEfficiency)
//   //set these pointers for later calculations
//   nXTCoef[3]=&TotYield;

//   //read in the full correlation matrix
//   cout<<"Covariance Matrix:"<<endl;
//   Float_t CorrelMatrix[NXT][NXT];
//   for(Int_t j=0;j<4;j++){
//     cout<<nXTCoef[j]->GetName()<<"     ";
//     for(Int_t k=0;k<4;k++){
//       CorrelMatrix[j][k]=DsFitResult->correlation(*nXTCoef[j],*nXTCoef[k]) * (nXTCoef[j]->getError()) * (nXTCoef[k]->getError());
//       cout<<scientific<<" ("<<CorrelMatrix[j][k]<<")";
//     }
//     cout<<endl;
//   }
  
//   //Compute derivatives w.r.t the floated parameters
//   Float_t DerivDenom[NXT];
//   Float_t TrueWTimesEff=0.;
//   Float_t TrueW=0.;
//   for(Int_t k=3;k<NXT;k++){
//     TrueWTimesEff+=truew[k]*MuEff[k];
//     TrueW+=truew[k];
//   }
//   for(Int_t j=0;j<3;j++){//w.r.t the floated weights   
//     DerivDenom[j]=nXTCoef[3]->getVal()*(MuEff[j]-TrueWTimesEff/TrueW);
//   }
//   DerivDenom[3]=TotMuEff; //derivative w.r.t TotYield

//   cout<<"Correlated error matrix:"<<endl;
//   //add the correlated errors
//   Float_t TotDenomErr=0.;
//   for(Int_t j=0;j<4;j++){
//     //cout<<"Derivative"<<j<<" ="<<DerivDenom[j]<<endl;
//     for(Int_t k=0;k<4;k++){
//       cout<<scientific<<"   "<<DerivDenom[j]*DerivDenom[k]*CorrelMatrix[j][k];
//       TotDenomErr+=DerivDenom[j]*DerivDenom[k]*CorrelMatrix[j][k];
//     }
//     cout<<endl;
//   }  
//   //add the un-correlated errors due to the individual efficiencies
//   for(Int_t j=0;j<3;j++)
//     TotDenomErr+=pow(TotYield.getVal()*nXTCoef[j]->getVal()*MuEffErr[j],2);
//   for(Int_t j=3;j<NXT;j++)
//     TotDenomErr+=pow(TotYield.getVal()*nXTCoefWM[j]->getVal()*MuEffErr[j],2);
  
//   TotDenomErr=sqrt(TotDenomErr);
//   cout<<fixed<<scientific<<" The BF denominator is = "<<TotYield.getVal()*HDsMassData->Integral()*TotMuEff
//       <<" +- "<<TotDenomErr*HDsMassData->Integral()
//       <<"  -->"<<100.*TotDenomErr/(TotYield.getVal()*TotMuEff)<<"%"<<endl;
 
  
//   //----------------------------------
//   ///add error on Numerator yield + total error on denominator*efficiency
//   //---------------------------------
//   Float_t BFErr=BF*sqrt(pow(NRecoNuErr/NRecoNu,2) +  pow(TotDenomErr/(TotYield.getVal()*TotMuEff),2));
//   cout<<"-----------------------------------------------"<<endl;
//   cout<<fixed<<" The Branching fraction Error is ="<<BFErr*1000<<"\\times 10^{-3}    -->"<<100.*BFErr/BF<<"%"<<endl;
//   cout<<"-----------------------------------------------"<<endl;


//   return BF;
// }


// void DRecoDsInclusiveAnalysis::DetermineBF(){
//   TString Denomfilename="/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResults/";
//   if(_DataType==1)Denomfilename+="DenominatorFitResult_MC.root";
//   if(_DataType==2)Denomfilename+="DenominatorFitResult_Data.root";
//   TFile FDsFit(Denomfilename,"read");  
//   TString filename="/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResults/";
//   if(_DataType==1)filename+="MuNuFitResult_MC.root";
//   if(_DataType==2)filename+="MuNuFitResult_Data.root";
//   TFile FNumeratorFit(filename,"read");
//   TFile FDenominatorEff("/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResults/MuNuEffDenominator.root","r");
//   TFile FNumeratorEff("/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResults/MuNuEffNumerator.root","r");
//   gROOT->cd();
  
//   ComputeBF(&FDsFit,&FNumeratorFit,&FDenominatorEff,&FNumeratorEff);
// }

// Float_t DRecoDsInclusiveAnalysis::ComputeBF(TFile* FDenomFitResults,TFile* FNumFitResults,TFile* FEffDemon,TFile* FEffNum){
 
//   //------------------------------
//   //neutrino yield
//   //------------------------------
//   if(FNumFitResults->IsZombie()){cout<<"Bad NuFit File"<<endl; return 0;} gROOT->cd();
//   RooFitResult* NuFitResult=(RooFitResult*)FNumFitResults->Get("fitResultsDsToMuNuRunAll");
//   if(!NuFitResult){cout<<"No NuFitResult"<<endl;return 0;}
//   RooRealVar*NuYield=(RooRealVar*)NuFitResult->floatParsFinal().find("fSig");
//   if(!NuYield){cout<<"fSig not found"<<endl;return 0;}
//   TH2F*HNuMassData=(TH2F*)FNumFitResults->Get("hmm2MuonRunAll_EExtraCut");
//   if(!HNuMassData){cout<<"No HNuMassData "<<endl;return 0;}
//   cout<<"extracted MuNu Data: "<<HNuMassData->GetName()<<endl;
//   Float_t NRecoNu=NuYield->getVal()*HNuMassData->Integral();
//   Float_t NRecoNuErr=NuYield->getError()*HNuMassData->Integral();

//   TFile FNumTrue("/u/br/aidanrc/ana50/AWG90/aidanrc/Roofit/trueSigMuonRunAll_EExtraCut.root","read");
//   TTree*TNumTrue=(TTree*)FNumTrue.Get("ttrueSigMuonRunAll_EExtraCut");
//   TH1F HNumTrue("HNumTrue","",300,-1,4.);
//   TNumTrue->Draw("mm2>>HNumTrue","weight");
//   NRecoNu=HNumTrue.Integral();
//   cout<<"!!!!!!!!!!!Using true MuNu Yield!!!!!!!!!!!!!"<<endl;

//   cout<<"The total neutrino yield is "<<NRecoNu<<" +- "<<NuYield->getError()*HNuMassData->Integral()
//       <<" --> "<<NRecoNuErr*100./NRecoNu<<"%"<<endl;
  
  
 

//   cout<<endl;
//   //------------------------------
//   //muon efficiencies
//   //------------------------------
//   if(FEffNum->IsZombie()){cout<<"Bad FMuEff File"<<endl; return 0;} gROOT->cd();
//   if(FEffDemon->IsZombie()){cout<<"Bad FDsEff File"<<endl; return 0;} gROOT->cd();
//   Float_t MuEff[NXT];
//   Float_t MuEffErr[NXT];
//   cout<<" nXT efficiencies: "<<endl;
//   for(Int_t j=0;j<NXT;j++){
//     TH1F*HMuEff=(TH1F*)FEffNum->Get(TString("hNXT")+(long)j+"DsToMuNu_EExtraCut");
//     if(!HMuEff){cout<<"No HMuEff"<<j<<endl;return 0;}
//     TH2F*HDsEff=(TH2F*)FEffDemon->Get(TString("HDsMassTrue2D_nXT")+(long)j);
//     if(!HDsEff){cout<<"No HDsEff"<<j<<endl;return 0;}
//     MuEff[j]=(_DataType==2 ? 0.999837 : 1. )*HMuEff->Integral()/HDsEff->Integral(1,4,DenominatorDsMassBinMin,DenominatorDsMassBinMax);
//     //correct for Ds Mass peak shift in Data
//     MuEffErr[j]=(_DataType==2 ? 0.999837 : 1. )*MuEff[j]*sqrt(pow(sqrt(HMuEff->Integral())/HMuEff->Integral(),2)
// 			      + pow(sqrt(HDsEff->Integral(1,4,DenominatorDsMassBinMin,DenominatorDsMassBinMax))
// 				    /HDsEff->Integral(1,4,DenominatorDsMassBinMin,DenominatorDsMassBinMax),2));
//     cout<<setprecision(3)<<fixed<<" & "<<MuEff[j]<<" $\\pm$ "<<MuEffErr[j];
//   }
//   cout<<" \\\\"<<endl;

//   /////True MC Weights ------------------------------
//   Float_t totMCweight=0.;
//   for(Int_t j=0;j<NXT;j++){
//     totMCweight+= truew[j];
//     cout<<" MCweight"<<j<<"="<<truew[j]<<endl;
//   }
//   if(fabs(totMCweight-1.)>1e-5){cout<<" MC weights do not add up to 1."<<endl; return 0;}
//   Float_t TotMuEff=0.;
//   for(Int_t j=0;j<NXT;j++){  
//     TotMuEff+= truew[j]*MuEff[j];
//   }
//   cout<<"The MC weighted muon efficiency is "<<TotMuEff<<endl;

//   cout<<endl;
//   //------------------------------
//   ///Ds yield 
//   //------------------------------
//   if(FDenomFitResults->IsZombie()){cout<<"Bad DsFit File"<<endl; return 0;} gROOT->cd();
//   RooFitResult* DsFitResult=(RooFitResult*)FDenomFitResults->Get("fitresult");
//   if(!DsFitResult){cout<<"No DsFitResult"<<endl;return 0;}
//   TH2F*HDsMassData=(TH2F*)FDenomFitResults->Get("HDsMassData");
//   if(!HDsMassData){cout<<"No HDsMassData "<<endl;return 0;}
//   RooRealVar*DsYield=(RooRealVar*)DsFitResult->floatParsFinal().find("TotYield");
//   if(!DsYield){cout<<"DsYield not found"<<endl;return 0;}
//   TotYield.setVal(DsYield->getVal()); 
//   TotYield.setError(DsYield->getError());
//   TH2F* TotSigPdfHistoFit=(TH2F*)FDenomFitResults->Get("TotSigPdfHistoFit"); if(!TotSigPdfHistoFit)cout<<"No TotSigPdfHistoFit"<<endl;
//   Float_t NRecoDs=HDsMassData->Integral()*TotYield.getVal()
//     *TotSigPdfHistoFit->Integral(1,4,DenominatorDsMassBinMin,DenominatorDsMassBinMax)/TotSigPdfHistoFit->Integral();
//   Float_t NRecoDsErr=HDsMassData->Integral()*TotYield.getError()
//     *TotSigPdfHistoFit->Integral(1,4,DenominatorDsMassBinMin,DenominatorDsMassBinMax)/TotSigPdfHistoFit->Integral();

//   //use true yield  
//   TFile FDenomTrue("DsTolnu/SP1005Skim/DsReco/Final/HistosForFit.root","read");
//   NRecoDs=((TH1F*)FDenomTrue.Get("HDsMassTrue"))->Integral(DenominatorDsMassBinMin,DenominatorDsMassBinMax);
//   cout<<"!!!!!!!!!!!Using true Ds Yield!!!!!!!!!!!!!"<<endl;

//   cout<<"The total Ds yield is "<<NRecoDs<<" +- "<<NRecoDsErr<<" --> "<<NRecoDsErr*100./NRecoDs<<"%"<<endl;  
//   cout<<endl;
  
//   //for MC lumi weight the Ds yield because Aidan has lumi weighted his MC
// //   if(_DataType==1){    
// //     NRecoDs=NRecoDs*(1./2.04+.0974/1.93)/(1+.0974);
// //     NRecoDsErr=NRecoDsErr*(1./2.04+.0974/1.93)/(1+.0974);
// //   }
//   cout<<"The total Ds yield after Lumi Weighting is "<<NRecoDs<<" +- "<<NRecoDsErr<<" --> "<<NRecoDsErr*100./NRecoDs<<"%"<<endl;  
//   cout<<endl;

//   //------------------------------
//   //Fitted nXT  weights
//   //------------------------------  
//   Float_t totDataweight=0.;
//   for(Int_t j=0;j<3;j++){
//     if(!DsFitResult->floatParsFinal().find(TString("nXTCoef")+(long)j)){cout<<"nXTCoef not found"<<endl;return 0;}
//     nXTCoef[j]->setVal(((RooRealVar*)(DsFitResult->floatParsFinal().find(TString("nXTCoef")+(long)j)))->getVal());    
//     nXTCoef[j]->setError(((RooRealVar*)(DsFitResult->floatParsFinal().find(TString("nXTCoef")+(long)j)))->getError());    
//     totDataweight+=nXTCoef[j]->getVal();
//     cout<<" FitWeight"<<j<<"="<<nXTCoef[j]->getVal()<<" +- "<<nXTCoef[j]->getError()<<endl;
//   }  
//   for(Int_t j=3;j<NXT;j++){    
//     totDataweight+=nXTCoefWM[j]->getVal();//note that these coefficients are being updated as long as the first three are updated
//     cout<<" FitWeight"<<j<<"="<<nXTCoefWM[j]->getVal()<<endl;
//   }
//   if(fabs(totDataweight-1.)>1e-5){cout<<" Fit weights do not add up to 1."<<endl; return 0;}
  

//   //------------------------------
//   //Compute the Data weighted efficiency
//   //------------------------------
//   TotMuEff=0.;
//   for(Int_t j=0;j<3;j++)
//     TotMuEff+= nXTCoef[j]->getVal()*MuEff[j];
//   for(Int_t j=3;j<NXT;j++)
//     TotMuEff+= nXTCoefWM[j]->getVal()*MuEff[j];    
//   cout<<"The reweighted muon efficiency is "<<TotMuEff<<endl;
  
//   TotMuEff=0.;
//   for(Int_t j=0;j<NXT;j++){
//     TotMuEff+= truew[j]*MuEff[j];
//     cout<<"!!!!!!!!!!Using true weights!!!!!!!!!!"<<endl;
//   }


//   //Use total efficiency 
//   Float_t totMuNuDenom=0.;
//   Float_t totMuNuNum=0.;
//   for(Int_t j=0;j<NXT;j++){
//     TH1F*HMuEff=(TH1F*)FEffNum->Get(TString("hNXT")+(long)j+"DsToMuNu_EExtraCut");
//     if(!HMuEff){cout<<"No HMuEff"<<j<<endl;return 0;}
//     TH2F*HDsEff=(TH2F*)FEffDemon->Get(TString("HDsMassTrue2D_nXT")+(long)j);
//     if(!HDsEff){cout<<"No HDsEff"<<j<<endl;return 0;} 
//     totMuNuDenom+=HDsEff->Integral(1,4,DenominatorDsMassBinMin,DenominatorDsMassBinMax);
//     totMuNuNum+=HMuEff->Integral();
//   }
//   TotMuEff=totMuNuNum/totMuNuDenom;
//   cout<<" !!!!!!!!!!!!!Using Total Efficiency (No nXT reweighting)!!!!!!!!    = "<<totMuNuNum<<"/"<<totMuNuDenom<<" = "<<TotMuEff<<endl;

//   cout<<endl;
//   //---------------------------
//   //Compute the Branching fraction
//   //----------------------------
//   Float_t BF=NRecoNu/(NRecoDs*TotMuEff);
//   cout<<"-----------------------------------------------"<<endl;
//   cout<<" The Branching fraction is = "<<BF*1000<<"\\times 10^{-3} "<<endl;
//   cout<<"-----------------------------------------------"<<endl;
  
  
//   cout<<endl;
//   //----------------------------------
//   //Compute the error
//   //----------------------------------

//   //error on Denominator: DsYield*( weighted MuEfficiency)
//   //set these pointers for later calculations
//   nXTCoef[3]=&TotYield;

//   //read in the full correlation matrix
//   cout<<"Covariance Matrix:"<<endl;
//   Float_t CorrelMatrix[NXT][NXT];
//   for(Int_t j=0;j<4;j++){
//     cout<<nXTCoef[j]->GetName()<<"     ";
//     for(Int_t k=0;k<4;k++){
//       CorrelMatrix[j][k]=DsFitResult->correlation(*nXTCoef[j],*nXTCoef[k]) * (nXTCoef[j]->getError()) * (nXTCoef[k]->getError());
//       cout<<scientific<<" ("<<CorrelMatrix[j][k]<<")";
//     }
//     cout<<endl;
//   }
  
//   //Compute derivatives w.r.t the floated parameters
//   Float_t DerivDenom[NXT];
//   Float_t TrueWTimesEff=0.;
//   Float_t TrueW=0.;
//   for(Int_t k=3;k<NXT;k++){
//     TrueWTimesEff+=truew[k]*MuEff[k];
//     TrueW+=truew[k];
//   }
//   for(Int_t j=0;j<3;j++){//w.r.t the floated weights   
//     DerivDenom[j]=nXTCoef[3]->getVal()*(MuEff[j]-TrueWTimesEff/TrueW);
//   }
//   DerivDenom[3]=TotMuEff; //derivative w.r.t TotYield

//   cout<<"Correlated error matrix:"<<endl;
//   //add the correlated errors
//   Float_t TotDenomErr=0.;
//   for(Int_t j=0;j<4;j++){
//     //cout<<"Derivative"<<j<<" ="<<DerivDenom[j]<<endl;
//     for(Int_t k=0;k<4;k++){
//       cout<<scientific<<"   "<<DerivDenom[j]*DerivDenom[k]*CorrelMatrix[j][k];
//       TotDenomErr+=DerivDenom[j]*DerivDenom[k]*CorrelMatrix[j][k];
//     }
//     cout<<endl;
//   }  
//   //add the un-correlated errors due to the individual efficiencies
//   for(Int_t j=0;j<3;j++)
//     TotDenomErr+=pow(TotYield.getVal()*nXTCoef[j]->getVal()*MuEffErr[j],2);
//   for(Int_t j=3;j<NXT;j++)
//     TotDenomErr+=pow(TotYield.getVal()*nXTCoefWM[j]->getVal()*MuEffErr[j],2);
  
//   TotDenomErr=sqrt(TotDenomErr);
//   cout<<fixed<<scientific<<" The BF denominator is = "<<TotYield.getVal()*HNuMassData->Integral()*TotMuEff
//       <<" +- "<<TotDenomErr*HNuMassData->Integral()
//       <<"  -->"<<100.*TotDenomErr/(TotYield.getVal()*TotMuEff)<<"%"<<endl;
 
  
//   //----------------------------------
//   ///
//   //---------------------------------
//   Float_t BFErr=BF*sqrt(pow(NRecoNuErr/NRecoNu,2) +  pow(TotDenomErr/(TotYield.getVal()*TotMuEff),2));
//   cout<<"-----------------------------------------------"<<endl;
//   cout<<fixed<<" The Branching fraction Error is ="<<BFErr*1000<<"\\times 10^{-3}    -->"<<100.*BFErr/BF<<"%"<<endl;
//   cout<<"-----------------------------------------------"<<endl;


//   return BF;
// }


// Float_t DRecoDsInclusiveAnalysis::ComputeBFKKpiDalitz(TFile* FDenomFitResults,TFile* FNumFitResults,TFile* FEffDemon,TFile* FEffNum,TFile* FDalitzWeights){
    
//   cout<<endl;
//   //------------------------------
//   //Numerator efficiencies
//   //------------------------------  
//   if(FEffNum->IsZombie()){cout<<"Bad FNumeratorEff File"<<endl; return 0;} gROOT->cd();
//   if(FEffDemon->IsZombie()){cout<<"Bad FDenominatorEff File"<<endl; return 0;} gROOT->cd();

//   Float_t Eff[3][NXT];
//   Float_t EffErr[3][NXT];
//   for(Int_t d=0;d<3;d++){
//     for(Int_t j=0;j<NXT;j++){
//       TH1F*HEff=(TH1F*)FEffNum->Get(TString("HKKpiGamMassSignal_Dalitz")+(long)d+"_nXT"+(long)j);
//       if(!HEff){cout<<"No HEff"<<d<<" "<<j<<endl;return 0;}
//       TH2F*HDsEff=(TH2F*)FEffDemon->Get(TString("HKKpiDsMassTrue2D_Dalitz")+(long)d+"_nXT"+(long)j);
//       if(!HDsEff){cout<<"No HDsEff"<<d<<" "<<j<<endl;return 0;}
//       Eff[d][j]=HEff->Integral()/HDsEff->Integral();
//       if(_DataType==2){
// 	Eff[d][j]=.9866*Eff[d][j];//correct for PID 
// 	cout<<"PID correction has  been applied"<<endl;
//       }
//       //EffErr[d][j]=Eff[d][j]*sqrt(pow(sqrt(HEff->Integral())/HEff->Integral(),2)+pow(sqrt(HDsEff->Integral())/HDsEff->Integral(),2));
//       EffErr[d][j]=sqrt(HEff->Integral())/HDsEff->Integral();// add only error on KKpi reco number not Ds reco number
//     }
//   }
//   cout<<endl;

//   ///NXT weights
//   Float_t totMCweight=0.;
//   for(Int_t j=0;j<NXT;j++){
//     totMCweight+= truew[j];
//     cout<<" True Weight "<<j<<"="<<truew[j]<<endl;
//   }
//   if(fabs(totMCweight-1.)>1e-5){cout<<" MC weights do not add up to 1."<<endl; return 0;}

  
//   Float_t TotEff[3]={0.,0.,0.};
//   Float_t TotEffErr[3]={0.,0.,0.};
//   for(Int_t d=0;d<3;d++){
//     for(Int_t j=0;j<NXT;j++){  
//       TotEff[d]+= truew[j]*Eff[d][j];
//       TotEffErr[d]+= pow(truew[j]*EffErr[d][j],2);
//     }
//     TotEffErr[d]=sqrt(TotEffErr[d]);
//     cout<<"The true weighted efficiency Dalitz Region "<<d<<" --> "<<TotEff[d]<<" +- "<<TotEffErr[d]<<endl;
//   }


//   cout<<endl;
//   //------------------------------
//   ///Ds yield 
//   //------------------------------ 
//   if(FDenomFitResults->IsZombie()){cout<<"Bad DsFit File"<<endl; return 0;} gROOT->cd();
//   RooFitResult* DsFitResult=(RooFitResult*)FDenomFitResults->Get("fitresult");
//   if(!DsFitResult){cout<<"No DsFitResult"<<endl;return 0;}
//   TH2F*HDsMassData=(TH2F*)FDenomFitResults->Get("HDsMassData");
//   if(!HDsMassData){cout<<"No HDsMassData "<<endl;return 0;}
//   RooRealVar*DsYield=(RooRealVar*)DsFitResult->floatParsFinal().find("TotYield");
//   if(!DsYield){cout<<"DsYield not found"<<endl;return 0;}
//   TotYield.setVal(DsYield->getVal());
//   TotYield.setError(DsYield->getError());
//   TFile FDenominatorTrueYield("DsTolnu/SP1005Skim/DsReco/Final/HistosForFit.root","r");
//   Float_t NRecoDs=HDsMassData->Integral()*TotYield.getVal();
//   //Float_t NRecoDs=((TH1F*)FDenominatorTrueYield.Get("HDsMassTrue"))->Integral();cout<<"!!!!!!!!!!!!Using true quantity"<<endl;
//   Float_t NRecoDsErr=HDsMassData->Integral()*TotYield.getError();
//   cout<<"The inclusive Ds yield is "<<NRecoDs<<" +- "<<NRecoDsErr<<" -->"<<NRecoDsErr*100./NRecoDs<<"%"<<endl;
  
//   cout<<endl;
//   //------------------------------
//   //Ds weights
//   //------------------------------  
//   Float_t totDataweight=0.;
//   for(Int_t j=0;j<3;j++){
//     if(!DsFitResult->floatParsFinal().find(TString("nXTCoef")+(long)j)){cout<<"nXTCoef not found"<<endl;return 0;}
//     nXTCoef[j]->setVal(((RooRealVar*)(DsFitResult->floatParsFinal().find(TString("nXTCoef")+(long)j)))->getVal());    
//     nXTCoef[j]->setError(((RooRealVar*)(DsFitResult->floatParsFinal().find(TString("nXTCoef")+(long)j)))->getError());    
//     totDataweight+=nXTCoef[j]->getVal();
//     cout<<" FitWeight"<<j<<"="<<nXTCoef[j]->getVal()<<" +- "<<nXTCoef[j]->getError()<<endl;
//   }
//   for(Int_t j=3;j<NXT;j++){    
//     totDataweight+=nXTCoefWM[j]->getVal();
//     cout<<" FitWeight"<<j<<"="<<nXTCoefWM[j]->getVal()<<endl;
//   }
//   //----------------------
  
//   if(fabs(totDataweight-1.)>1e-5){cout<<" Fit weights do not add up to 1."<<endl; return 0;}
  

//   //------------------------------
//   //Compute the Data weighted efficiency
//   //------------------------------ 
//   for(Int_t d=0;d<3;d++){
//     TotEff[d]=0.;
//     TotEffErr[d]=0.;
//     for(Int_t j=0;j<3;j++){  
//       TotEff[d]+= nXTCoef[j]->getVal()*Eff[d][j];
//       //TotEff[d]+= truew[j]*Eff[d][j];cout<<"!!!!!!!!!!!!Using true quantity"<<endl;
//       TotEffErr[d]+= pow(nXTCoef[j]->getVal()*EffErr[d][j],2);
//     }
//     for(Int_t j=3;j<NXT;j++){  
//       TotEff[d]+= nXTCoefWM[j]->getVal()*Eff[d][j];    
//       //TotEff[d]+= truew[j]*Eff[d][j];cout<<"!!!!!!!!!!!!Using true quantity"<<endl;
//       TotEffErr[d]+= pow(nXTCoefWM[j]->getVal()*EffErr[d][j],2);
//     }
//     TotEffErr[d]=sqrt(TotEffErr[d]);
//   }

//   cout<<endl;
//   //print out the efficiency in latex format
//   for(Int_t j=0;j<NXT;j++){
//     cout<<" $n_X^T$="<<j;
//     for(Int_t d=0;d<3;d++)
//       printf(" & %.3f $\\pm$ %.3f",Eff[d][j],EffErr[d][j]);
//     printf(" \\\\ \n");
//   }
//   cout<<" $\\sum w^{fit}_j\\epsilon_d^j$";
//   for(Int_t d=0;d<3;d++)
//     printf(" & %.3f $\\pm$ %.3f",TotEff[d],TotEffErr[d]);
//   printf(" \\\\ \n");


//   ///Dalitz reweighting  
//   if(FDalitzWeights->IsZombie()){cout<<"Bad FDalitzWeight File"<<endl; return 0;} gROOT->cd();
//   Float_t dalitzweight[3];
//   Float_t totDalitzweight=0.;
//   for(Int_t d=0;d<3;d++){
//     TH1F*HDalitzWeight=NULL;
//     if(_DataType==1)HDalitzWeight=(TH1F*)FDalitzWeights->Get(TString("HDsMassMCDalitz")+(long)d);
//     if(_DataType==2)HDalitzWeight=(TH1F*)FDalitzWeights->Get(TString("HDsMassDataDalitz")+(long)d);
//     if(!HDalitzWeight)cout<<"Bad Dalitz Histo"<<d<<endl;
//     dalitzweight[d]=HDalitzWeight->GetBinContent(0);//ignore weight errors
//     totDalitzweight+= dalitzweight[d];
//     cout<<" Dalitz weight "<<d<<" ="<<dalitzweight[d]<<endl;
//   }
//   if(fabs(totDalitzweight-1.)>1e-5){cout<<" Dalitz weights do not add up to 1."<<endl; return 0;}

  
//   Float_t TotKKpiEff=0.;
//   for(Int_t d=0;d<3;d++){
//     TotKKpiEff += dalitzweight[d]*(1./TotEff[d]);
//   }

//   //TotKKpiEff=3.04947451674643155e-01//true eff in Generic MC
//   cout<<" The reweighted Dalitz efficiency is "<<1/TotKKpiEff<<endl;



//   //------------------------------
//   //Numerator yield
//   //------------------------------
//   if(FNumFitResults->IsZombie()){cout<<"Bad NuFit File"<<endl; return 0;} gROOT->cd();
//   RooFitResult* NumeratorFitResult=(RooFitResult*)FNumFitResults->Get(TString("FitResult"));
//   if(!NumeratorFitResult){cout<<"No NumeratorFitResult"<<endl;return 0;}
//   TH1F* HNumeratorData=(TH1F*)FNumFitResults->Get(TString("HKKpiGamMass"));
//   if(!HNumeratorData){cout<<"No HNumeratorData "<<endl;return 0;}
//   RooRealVar*NumeratorYield=(RooRealVar*)NumeratorFitResult->floatParsFinal().find(TString("SigYield"));
//   if(!NumeratorYield){cout<<"NumeratorYield not found"<<endl;return 0;}
//   TFile FNumeratorTrueYield("DsTolnu/KKpi/SP1005/HistosForFit.root","r");
//   Float_t NRecoNumerator=NumeratorYield->getVal()*HNumeratorData->Integral();
//   //Float_t NRecoNumerator=((TH1F*)FNumeratorTrueYield.Get("HKKpiGamMassSignal"))->Integral();cout<<"!!!!!!!!!!!!Using true quantity"<<endl;
//   Float_t NRecoNumeratorErr=NumeratorYield->getError()*HNumeratorData->Integral();
//   cout<<"Numerator yield= "<<NRecoNumerator<<" +- "<<NRecoNumeratorErr
//       <<"  -->"<<NRecoNumeratorErr*100./NRecoNumerator<<"%"<<endl;  
  


//   cout<<endl;
//   //---------------------------
//   //Compute the Branching fraction
//   //----------------------------
//   Float_t BF=(NRecoNumerator/NRecoDs)*TotKKpiEff;
//   cout<<"-----------------------------------------------"<<endl;
//   cout<<" The Branching fraction is = "<<setprecision (4)<<fixed<<BF<<endl;
//   cout<<"-----------------------------------------------"<<endl;
  
  
// //Info from DECAY.DEC Ds decays 
// // 0.044    phi   pi+                      SVS;
// // 0.040    anti-K*0   K+                  SVS;
// // 0.01    f_0    pi+                      PHSP;
// // 0.011    K+    K-    pi+                PHSP;
// //Total Ds-->KKpi= .044*0.4910 + 0.040*0.6657 + 0.01*0.1100 + 0.011=.06033

// //This number has been "confirmed" by looping over the events of a SP1005 collection 
// //Using the DetermineGenKKpiBF.C macro
// //nEvt=600000  nDs=114766  nKKpi=6847  BF=0.0596605 


//   cout<<endl;
//   //----------------------------------
//   //Compute the error
//   //----------------------------------

//   //error on Denominator: DsYield*( weighted Efficiency)
//   //set these pointers for later calculations
//   nXTCoef[3]=&TotYield;

//   //read in the full correlation matrix
//   cout<<setprecision(2)<<scientific;
//   cout<<"Covariance Matrix:"<<endl;
//   Float_t CorrelMatrix[NXT][NXT];
//   for(Int_t j=0;j<4;j++){
//     cout<<nXTCoef[j]->GetName()<<"     ";
//     for(Int_t k=0;k<4;k++){
//       CorrelMatrix[j][k]=DsFitResult->correlation(*nXTCoef[j],*nXTCoef[k]) * (nXTCoef[j]->getError()) * (nXTCoef[k]->getError());
//       cout<<"  "<<CorrelMatrix[j][k];
//     }
//     cout<<endl;
//   }
      
//   //Derivatives of the denominator
//   Float_t TrueWTimesEff[3]={0.,0.,0.};
//   Float_t TrueW=0.;
//   for(Int_t k=3;k<NXT;k++) TrueW+=truew[k];  
//   Float_t DerivDenom[3][4];
//   for(Int_t d=0;d<3;d++){
//     for(Int_t k=3;k<NXT;k++)
//       TrueWTimesEff[d]+=truew[k]*Eff[d][k];

//     for(Int_t j=0;j<3;j++)//w.r.t the floated weights   
//       DerivDenom[d][j]=nXTCoef[3]->getVal()*(Eff[d][j]-TrueWTimesEff[d]/TrueW);
    
//     //derivative w.r.t TotYield
//     DerivDenom[d][3]=TotEff[d];
//   }
//   cout<<endl;

//   cout<<" Errors on the BF denominator:"<<endl;
//   cout<<setprecision (3)<<fixed;
//   Float_t TotDenomErr[3]={0.,0.,0.};
//   for(Int_t d=0;d<3;d++){    
//     //add the correlated errors
//     cout<<" Dalitz Region "<<d;
//     for(Int_t j=0;j<4;j++){
//       //cout<<" "<<DerivDenom[d][j];    
//       for(Int_t k=0;k<4;k++){
// 	TotDenomErr[d]+=DerivDenom[d][j]*DerivDenom[d][k]*CorrelMatrix[j][k];
//       }
//     }  

//     //add the un-correlated errors due to the efficiency errors
//     for(Int_t j=0;j<3;j++)//Not sure about this part
//       TotDenomErr[d]+=pow(NRecoDs*nXTCoef[j]->getVal()*EffErr[d][j],2);
//     for(Int_t j=3;j<NXT;j++)
//       TotDenomErr[d]+=pow(NRecoDs*nXTCoefWM[j]->getVal()*EffErr[d][j],2);
  
//     TotDenomErr[d]=sqrt(TotDenomErr[d]);
//     cout<<" Error = "<<100.*TotDenomErr[d]/(NRecoDs*TotEff[d])<<"%"<<endl;
//   }
  
  
//   //---------------------------------
//   //Compute total BF Error
//   //---------------------------------
//   Float_t BFErr=pow(NRecoNumeratorErr/NRecoNumerator,2);//add error on the KKpi yield
//   for(Int_t d=0;d<3;d++)//add error on each deonminator for each dalitz region 
//     BFErr+= ( pow(dalitzweight[d]/(NRecoDs*TotEff[d]),2) 
// 	      * pow(TotDenomErr[d]/(NRecoDs*TotEff[d]),2) )
//             / pow(TotKKpiEff/NRecoDs,2);      

//   BFErr=sqrt(BFErr)*BF;
//   cout<<"-----------------------------------------------"<<endl;
//   cout<<" The Branching fraction Error is ="<<setprecision (4)<<fixed<<BFErr<<"  -->"<<setprecision (2)<<fixed<<100.*BFErr/BF<<"%"<<endl;
//   cout<<"-----------------------------------------------"<<endl;

//   return BF;
// }




// Int_t DRecoDsInclusiveAnalysis::OptimizeXPi0Cuts(){
  
//   if(OpenReducedNtuple()!=1) return 0;

//   ReducedNtuple->SetBranchStatus("*",0);
//   ReducedNtuple->SetBranchStatus("fragpi0mass",1);
//   ReducedNtuple->SetBranchStatus("fragpi0gamenergymodified",1);

//   Float_t MMean=.1325;
//   Float_t MWidth=.012;
//   Float_t MinSideLimit=.018;
//   TTree*sigt=ReducedNtuple->CopyTree(TCut(Form("abs(fragpi0mass-%.6f)<%.6f",MMean,MWidth)));
//   TTree*bkgt=ReducedNtuple->CopyTree(TCut(Form("%.6f<abs(fragpi0mass-%.6f)&&abs(fragpi0mass-%.6f)<%.6f",MinSideLimit,MMean,MMean,MinSideLimit+MWidth)));
//   cout<<"Entries in signal tree= "<<sigt->GetEntriesFast()<<endl;
//   cout<<"Entries in sideband tree= "<<bkgt->GetEntriesFast()<<endl;


//   //Plots to be saved to root file
//   TFile Results(_OutputDir+"/OptimizeXPi0Cuts.root","recreate");
//   gROOT->cd();
  
//   ///Variables for Minuit Optimization 
//   RooRealVar mwin("mwin",Form("abs(fragpi0mass-%.6f)<%s",MMean,"%.6f"),.005,MWidth);
//   mwin.setUnit("mass window");mwin.setVal(MWidth);mwin.setError(.0005);//mwin name is hard coded in the Optimizer.
//   RooRealVar gamemin("gamemin","fragpi0gamenergymodified>%6f",.05,.2);
//   gamemin.setUnit("E_{lab}");gamemin.setError(.01);gamemin.setVal(.05);//gamemin.setConstant(1);


//   RooArgList cutvars;
//   cutvars.add(mwin);
//   cutvars.add(gamemin);


//   //make plot with initial cuts and sideband definition
//   Int_t nbins=35;
//   Float_t xmin=.092;Float_t xmax=.162;   
//   TH1F HMass("HMass","",nbins,xmin,xmax);
//   TH1F HMassSig("HMassSig","",nbins,xmin,xmax);HMassSig.SetFillColor(2);
//   TH1F HMassSide("HMassSide","",nbins,xmin,xmax);HMassSide.SetFillColor(4);

//   //compare plot with initial cuts: do this because p* distribution is tailing down below skim cut
//   TCut initcutnomass;
//   TIterator*cutvarIter=cutvars.createIterator();
//   char textl[100];
//   while(RooRealVar* var=(RooRealVar*)cutvarIter->Next()){
//     sprintf(textl,var->GetTitle(),var->getVal());
//     if(TString(var->GetName())!="mwin")initcutnomass+=TCut(textl); 
//   }  

//   ReducedNtuple->Draw("fragpi0mass>>HMass",initcutnomass);
//   sigt->Draw("fragpi0mass>>HMassSig",initcutnomass,"same");
//   bkgt->Draw("fragpi0mass>>HMassSide",initcutnomass,"same");
//   Results.cd();
//   HMass.Write();
//   HMassSig.Write();
//   HMassSide.Write();
//   gROOT->cd();


  
//   //the dataset defines the variable (mass) to be excluded from the optimization, it has no other purpose
//   mass.setRange(xmin,xmax);
//   mass.setBins(nbins);
//   TH1F Histo("Histo","",nbins,xmin,xmax);
//   RooDataHist dataset("dataset","dataset",RooArgList(mass),&Histo); 
//   //the pdf needs the mass variable to go with the dataset, cut variables need to be included here.
//   RooArgSet pdfvars(cutvars);
//   pdfvars.add(mass);

//   TString pdfformula="mass";
//   pdfformula+="+mwin";
//   pdfformula+="+gamemin";
//   RooGenericPdf pdf("pdf",pdfformula,pdfvars);
//   DRecoTagOptimizer optimizer("optimizer","optimizer",pdf,dataset,sigt,bkgt,&cutvars,0); 
//   optimizer.SetSigType(1);
//   optimizer.SetMinSideBandLimit(MinSideLimit);

//   //look for the best cuts
//   Float_t initsig=optimizer.GetSignificance();
//   cout<<"Initial Significance= "<<initsig<<endl;
//   ULong_t starttime=(ULong_t)(gSystem->Now());
//   SearchOptimizationGrid(&cutvars,&optimizer,.6);
//   ULong_t tottime=((ULong_t)(gSystem->Now())-starttime)/1000;
//   cout<<"Search time= "<<tottime<<" seconds"<<endl;
//   Float_t finalsig=optimizer.GetSignificance();
//   //print the final significance;
//   cout<<"Final Significance= "<<finalsig<<endl;
 
//   Results.cd();
  
//   ofstream outfile;
//   outfile.open(_OutputDir+"/results.txt"); 
//   outfile<<"initsig"<<" "<<initsig<<endl;
//   //compare plot with final cuts:
//   TCut finalcut;
//   TCut finalcutnomass;
//   cutvarIter->Reset();
//   while(RooRealVar* var=(RooRealVar*)cutvarIter->Next()){
//     sprintf(textl,var->GetTitle(),var->getVal());
//     finalcut+=TCut(textl); 
//     if(TString(var->GetName())!="mwin")finalcutnomass+=TCut(textl); 
    
//     TGraph*Scan=optimizer.ScanVariable(var);   
//     Scan->SetName(TString(var->GetName())+"_Scan");
//     Scan->Write();
    
//     outfile<<var->GetName()<<" "<<var->getVal()<<endl;
//   }  
  
//   outfile<<"finalsig"<<" "<<finalsig<<endl;
//   outfile<<"tottime"<<" "<<tottime<<endl;
//   outfile.close();


//   TH1F HMassFinalNoMass("HMassFinalNoMass","",nbins,xmin,xmax);  
//   ReducedNtuple->Draw("fragpi0mass>>HMassFinalNoMass",finalcutnomass);
//   HMassFinalNoMass.Write();

//   TH1F HMassFinal("HMassFinal","",nbins,xmin,xmax);  
//   ReducedNtuple->Draw("fragpi0mass>>HMassFinal",finalcut);
//   HMassFinal.Write();

//   Results.Close();
  
  
//   delete sigt;delete bkgt;

//   return 1;
  
// }


// Int_t DRecoDsInclusiveAnalysis::DrawOptimizeXPi0Cuts(){

//   //Plots to be saved to root file
//   TFile Results(_OutputDir+"/OptimizeXPi0Cuts.root","read");
//   gROOT->cd();
  
//   filename=_OutputDir+"/OptimizeXPi0Cuts.ps";
//   Canvas->Print(filename+"[");
  
//   TH1F* HMass=(TH1F*)Results.Get("HMass");
//   TH1F* HMassSig=(TH1F*)Results.Get("HMassSig");
//   TH1F* HMassSide=(TH1F*)Results.Get("HMassSide");

//   TH1F* HMassClone=(TH1F*)HMass->Clone();
//   HMass->GetYaxis()->SetRangeUser(0,1.1*HMass->GetMaximum());
//   Canvas->Clear();
//   TF1*Fpi0=FitHistogram1gausPlusPolN(HMass,HMass->GetXaxis()->GetXmin(),.163,2,0);
//   Fpi0->FixParameter(1,.1333);
//   Fpi0->FixParameter(2,.006);
//   //Fpi0->SetParLimits(3,0,HMass->GetMaximum());
//   HMass->Fit(Fpi0,"QI","pe",HMass->GetXaxis()->GetXmin(),.163);
//   HMass->Draw("pe");
//   TF1 FBkg("FBkg","[0]+[1]*x+[2]*x*x",HMass->GetXaxis()->GetXmin(),HMass->GetXaxis()->GetXmax());
//   FBkg.SetParameters(Fpi0->GetParameter(3),Fpi0->GetParameter(4),Fpi0->GetParameter(5));
//   FBkg.SetLineColor(1);
//   FBkg.Draw("lsame");
//   cutline.DrawLine(.115,0,.115,HMass->GetMaximum());
//   cutline.DrawLine(.15,0,.15,HMass->GetMaximum());
//   Canvas->Print(filename);


//   Canvas->Clear();
//   HMass->Draw();
//   HMassSig->Draw("same");
//   HMassSide->Draw("same");
//   Canvas->Print(filename);

//   TH1F* HMassFinalNoMass=(TH1F*)Results.Get("HMassFinalNoMass");
//   TH1F* HMassFinal=(TH1F*)Results.Get("HMassFinal");
//   Canvas->Clear();
//   TF1*Fpi0Final=FitHistogram1gausPlusPolN(HMassFinalNoMass,HMassFinalNoMass->GetXaxis()->GetXmin(),.163,2,0);
//   Fpi0Final->FixParameter(1,.1333);
//   Fpi0Final->FixParameter(2,.006);
//   //Fpi0Final->SetParLimits(3,0,HMassFinalNoMass->GetMaximum());
//   HMassFinalNoMass->Fit(Fpi0,"QI","pe",HMassFinalNoMass->GetXaxis()->GetXmin(),.163);
//   HMassFinalNoMass->Draw("pe");
//   TF1 FBkgFinal("FBkgFinal","[0]+[1]*x+[2]*x*x",HMassFinalNoMass->GetXaxis()->GetXmin(),HMassFinalNoMass->GetXaxis()->GetXmax());
//   FBkgFinal.SetParameters(Fpi0Final->GetParameter(3),Fpi0Final->GetParameter(4),Fpi0Final->GetParameter(5));
//   FBkgFinal.SetLineColor(1); 
//   HMassClone->GetYaxis()->SetRangeUser(0,1.1*HMassClone->GetMaximum());
//   HMassClone->Draw();
//   HMassFinalNoMass->Draw("same");
//   HMassFinal->Draw("same");
//   FBkgFinal.Draw("lsame");
//   Canvas->Print(filename);

//   TGraph*mwinScan=(TGraph*)Results.Get("mwin_Scan");
//   Canvas->Clear();
//   mwinScan->Draw("ap");
//   Canvas->Print(filename);

//   TGraph*pScan=(TGraph*)Results.Get("gamemin_Scan");
//   if(pScan){
//     Canvas->Clear();
//     pScan->Draw("ap");
//     Canvas->Print(filename);
//   }


//   Canvas->Print(filename+"]");
//   Results.Close();


//   return 1;
// }

//   ,HPi0AngleFake("HPi0AngleFake","",100,-1,1)
//   ,HPi0AngleTrue("HPi0AngleTrue","",100,-1,1)
//   ,HPi0GamAngleFake("HPi0GamAngleFake","",100,-1,1)
//   ,HPi0GamAngleTrue("HPi0GamAngleTrue","",100,-1,1)
//   ,HPi0GamEnergyFake("HPi0GamEnergyFake","",100,0,1.)
//   ,HPi0GamEnergyTrue("HPi0GamEnergyTrue","",100,0,1.)
  
//   //install counters for the Frag daughter sources
//   nFragDauTotal=0;
//   nFragDauFake=0;
//   nFragDauFromX=0;  
//   nFragDauFromD=0;
//   nFragDauFromK=0;
//   nFragDauFromDs=0;

//   nFragPiDauTotal=0;
//   nFragPiDauFake=0;
//   nFragPiDauFromX=0;  
//   nFragPiDauFromD=0;
//   nFragPiDauFromK=0;
//   nFragPiDauFromDs=0;

//   nFragPi0DauTotal=0;
//   nFragPi0DauFake=0;
//   nFragPi0DauFromX=0;  
//   nFragPi0DauFromD=0;
//   nFragPi0DauFromK=0;
//   nFragPi0DauFromDs=0;

//   nFragGamDauTotal=0;
//   nFragGamDauFake=0;
//   nFragGamDauFromX=0;  
//   nFragGamDauFromD=0;
//   nFragGamDauFromK=0;
//   nFragGamDauFromDs=0;


//       if(evthassignal){
// 	nFragGamDauFromX+=fragngamX;
// 	nFragGamDauFromD+=fragngamD;
// 	nFragGamDauFromK+=fragngamK;
// 	nFragGamDauFromDs+=fragngamDs;
// 	nFragGamDauFake+=fragngamFake;
// 	nFragGamDauTotal+=fragngams;

// 	nFragPi0DauFromX+=fragnpi0X;
// 	nFragPi0DauFromD+=fragnpi0D;
// 	nFragPi0DauFromK+=fragnpi0K;
// 	nFragPi0DauFromDs+=fragnpi0Ds;
// 	nFragPi0DauFake+=fragnpi0Fake;
// 	nFragPi0DauTotal+=fragnpi0s;

// 	nFragPiDauFromX+=fragnpiX;
// 	nFragPiDauFromD+=fragnpiD;
// 	nFragPiDauFromK+=fragnpiK;
// 	nFragPiDauFromDs+=fragnpiDs;
// 	nFragPiDauFake+=fragnpiFake;
// 	nFragPiDauTotal+=fragnpis;

// 	nFragDauFromX+=(fragngamX+fragnpi0X+fragnpiX);
// 	nFragDauFromD+=(fragngamD+fragnpi0D+fragnpiD);
// 	nFragDauFromK+=(fragngamK+fragnpi0K+fragnpiK);
// 	nFragDauFromDs+=(fragngamDs+fragnpi0Ds+fragnpiDs);
// 	nFragDauFake+=(fragngamFake+fragnpi0Fake+fragnpiFake);
// 	nFragDauTotal+=(fragngams+fragnpi0s+fragnpis);
//       }


//   if(_DataType==0||_DataType==1){
//     filename=_OutputDir+"/XPlots.ps";
//     Canvas->Print(filename+"[");

//     //produce X truth study
//     TH1F HXComp("HXComp","",4,0,4);
//     HXComp.SetBinContent(1,nFragDauTotal);
//     HXComp.SetBinContent(2,nFragPiDauTotal);
//     HXComp.SetBinContent(3,nFragPi0DauTotal);
//     HXComp.SetBinContent(4,nFragGamDauTotal);

//     TH1F HXFake("HXFake","",4,0,4);
//     HXFake.SetBinContent(1,nFragDauFake);
//     HXFake.SetBinContent(2,nFragPiDauFake);
//     HXFake.SetBinContent(3,nFragPi0DauFake);
//     HXFake.SetBinContent(4,nFragGamDauFake);

//     TH1F HXD("HXD","",4,0,4);
//     HXD.SetBinContent(1,HXFake.GetBinContent(1)+nFragDauFromD);
//     HXD.SetBinContent(2,HXFake.GetBinContent(2)+nFragPiDauFromD);
//     HXD.SetBinContent(3,HXFake.GetBinContent(3)+nFragPi0DauFromD);
//     HXD.SetBinContent(4,HXFake.GetBinContent(4)+nFragGamDauFromD);

//     TH1F HXK("HXK","",4,0,4);
//     HXK.SetBinContent(1,HXD.GetBinContent(1)+nFragDauFromK);
//     HXK.SetBinContent(2,HXD.GetBinContent(2)+nFragPiDauFromK);
//     HXK.SetBinContent(3,HXD.GetBinContent(3)+nFragPi0DauFromK);
//     HXK.SetBinContent(4,HXD.GetBinContent(4)+nFragGamDauFromK);

//     TH1F HXDs("HXDs","",4,0,4);
//     HXDs.SetBinContent(1,HXK.GetBinContent(1)+nFragDauFromDs);
//     HXDs.SetBinContent(2,HXK.GetBinContent(2)+nFragPiDauFromDs);
//     HXDs.SetBinContent(3,HXK.GetBinContent(3)+nFragPi0DauFromDs);
//     HXDs.SetBinContent(4,HXK.GetBinContent(4)+nFragGamDauFromDs);

//     TH1F HXX("HXX","",4,0,4);
//     HXX.SetBinContent(1,HXDs.GetBinContent(1)+nFragDauFromX);
//     HXX.SetBinContent(2,HXDs.GetBinContent(2)+nFragPiDauFromX);
//     HXX.SetBinContent(3,HXDs.GetBinContent(3)+nFragPi0DauFromX);
//     HXX.SetBinContent(4,HXDs.GetBinContent(4)+nFragGamDauFromX);

//     HXComp.GetYaxis()->SetRangeUser(0,HXComp.GetMaximum()*1.1);
//     HXComp.GetXaxis()->SetNdivisions(0,0);
//     HXComp.SetStats(0);
//     HXComp.GetXaxis()->SetTitle("#pi^{+}+#pi^{0}+#gamma           #pi^{+}                 #pi^{0}                 #gamma         ");   
//     Canvas->Clear();
//     HXComp.Draw();
//     HXX.SetLineColor(3);HXX.SetFillColor(3);HXX.Draw("same");
//     HXDs.SetLineColor(6);HXDs.SetFillColor(6);HXDs.Draw("same");
//     HXK.SetLineColor(4);HXK.SetFillColor(4);HXK.Draw("same");
//     HXD.SetLineColor(7);HXD.SetFillColor(7);HXD.Draw("same");
//     HXFake.SetLineColor(2);HXFake.SetFillColor(2);HXFake.Draw("same");
//     HXComp.Draw("same");
//     text.SetTextSize(.05);
//     text.SetTextColor(1);text.DrawTextNDC(.65,.8,"Source:");
//     text.SetTextColor(3);text.DrawTextNDC(.7,.75,"X");
//     text.SetTextColor(6);text.DrawTextNDC(.7,.7,"Ds");
//     text.SetTextColor(4);text.DrawTextNDC(.7,.65,"K");
//     text.SetTextColor(7);text.DrawTextNDC(.7,.6,"D");
//     text.SetTextColor(2);text.DrawTextNDC(.7,.55,"Fake");
//     Canvas->Print(filename);
 

//     //produce pi0 plots
//     Canvas->Clear();
//     HPi0AngleFake.GetXaxis()->SetTitle("#pi^{0} angle in the lab");
//     HPi0AngleFake.Draw();
//     HPi0AngleTrue.Draw("pesame");
//     Canvas->Print(filename);

//     Canvas->Clear();
//     HPi0GamAngleFake.GetXaxis()->SetTitle("#pi^{0} #gamma angle in the lab");
//     HPi0GamAngleFake.Draw();
//     HPi0GamAngleTrue.Draw("pesame");
//     Canvas->Print(filename);

//     Canvas->Clear();
//     HPi0GamEnergyFake.GetXaxis()->SetTitle("#pi^{0} #gamma energy in the lab");
//     HPi0GamEnergyFake.Draw();
//     HPi0GamEnergyTrue.Draw("pesame");
//     Canvas->Print(filename);

//     Canvas->Print(filename+"]");
//   }


// void DRecoDsInclusiveAnalysis::RooFitDsMassvsnXRvsnXTSimulWeightModel(TH2F*HDsMassDataWS,TH2F*HDsMassData,TString resultFileName){


//   filename=_OutputDir+"/"+resultFileName+".ps";
//   Canvas->Clear();
//   Canvas->Print(filename+"["); 
      
// //   TRandom3 rand((ULong_t)(gSystem->Now())%1000);  
// //   ///Randomize the data to smooth out the multiple minima.
// //   for(Int_t bx=1;bx<=HDsMassData->GetXaxis()->GetNbins();bx++)
// //     for(Int_t by=1;by<=HDsMassData->GetYaxis()->GetNbins();by++)
// //       HDsMassData->SetBinContent(bx,by,HDsMassData->GetBinContent(bx,by)-HDsMassData->GetBinError(bx,by)/8.
// // 				 +rand.Uniform(1)*HDsMassData->GetBinError(bx,by)/4.);


//   ///Get the WS signal Pdfs from the signal MC 
//   RooArgList pdflistws;
//   RooArgList coeflistWS;
//   RooRealIntegral* nXTCoefWS[NXT];
//   TH2F* SigWSPdfHisto[NXT];
//   Canvas->Clear();
//   Canvas->Divide(2,3);
//   for(Int_t j=0;j<NXT;j++){    

//     pdflistws.add(*SigPdfWS[j]);    

//     nXTCoefWS[j]=new RooRealIntegral(TString("nXTCoefWS")+(long)j,TString("nXTCoefWS")+(long)j,*weightPdfWS,RooArgSet(nxtws),&nxtsetws,0,TString("wrange")+(long)j);
//     if(j<NXT-1){
//       coeflistWS.add(*nXTCoefWS[j]);
//     }

//     Canvas->cd(j+1);
//     SigWSPdfHisto[j]=new TH2F(TString("SigPdfWSn_{X}^{T}=")+(long)j,"",NXR,-.5,NXR-.5,NDsMassBins,DsMassMin,DsMassMax);
//     SigPdfWS[j]->fillHistogram(SigWSPdfHisto[j],RooArgList(nxr,mass));
//     SigWSPdfHisto[j]->Scale(1./SigWSPdfHisto[j]->Integral());
//     SigWSPdfHisto[j]->SetTitle(TString("n_{X}^{T}=")+(long)j);
//     SigWSPdfHisto[j]->Draw("lego");
//   }    
//   Canvas->Print(filename); 
//   for(Int_t j=0;j<NXT;j++)  delete SigWSPdfHisto[j];
//   TH1F HnXTModelParWSTrue("HnXTModelParWSTrue","",NXT,-.5,NXT-.5);
//   HnXTModelParWSTrue.Sumw2();
//   HnXTModelParWSTrue.SetBinContent(1,mean1WS.getVal());HnXTModelParWSTrue.SetBinError(1,.001);
//   HnXTModelParWSTrue.SetBinContent(2,sigma1WS.getVal());HnXTModelParWSTrue.SetBinError(2,.001);
//   HnXTModelParWSTrue.SetBinContent(3,fracWS.getVal());HnXTModelParWSTrue.SetBinError(3,.001);
//   HnXTModelParWSTrue.SetBinContent(4,mean2WS.getVal());HnXTModelParWSTrue.SetBinError(4,.001);
//   HnXTModelParWSTrue.SetBinContent(5,sigma2WS.getVal());HnXTModelParWSTrue.SetBinError(5,.001);


//   //construct the total signal pdf out of the addition of each nXT pdf
//   DRecoAddPdf TotalSigWSPdf("TotalSigWSPdf","TotalSigWSPdf",nset,pdflistws,coeflistWS);
//   Canvas->Clear();
//   TH2F* TotalSigWSPdfHisto=(TH2F*)TotalSigWSPdf.CreateHistogram("TotalSigWSPdfHisto");
//   TotalSigWSPdfHisto->Draw("lego");
//   Canvas->Print(filename);


//   //Create the WS Dataset to be fited
//   RooDataHist DsMassDataSetWS("DsMassDataSetWS","DsMassDataSetWS",nset,HDsMassDataWS);
//   Canvas->Clear();
//   HDsMassDataWS->Draw("lego");
//   Canvas->Print(filename);
  
//   //RooRealVar RSToWSRatio("RSToWSRatio","RSToWSRatio",HDsMassData->Integral()/HDsMassDataWS->Integral());
//   RooRealVar TotYieldWS("TotYieldWS","TotYieldWS",.04,.25);
//   //RooFormulaVar TotYieldWS("TotYieldWS","TotYieldWS","WStoRSFrac*TotYield*RSToWSRatio",RooArgSet(TotYield,RSToWSRatio,WStoRSFrac));

//   //create a 2D background pdf from the WS Data - WS signal pdf
//   DRecoHistPdf DsMassWSPdf("DsMassWSPdf","DsMassWSPdf",nset,DsMassDataSetWS);
//   RooFormulaVar TotWSFracYield("TotWSFracYield","TotWSFracYield","-(1./(1.-TotYieldWS)-1.)",RooArgSet(TotYieldWS));
//   DRecoAddPdf TotWSBkgPdf("TotWSBkgPdf","TotWSBkgPdf",nset,TotalSigWSPdf,DsMassWSPdf,TotWSFracYield);
//   Canvas->Clear();
//   TH2F* TotWSBkgPdfHisto=(TH2F*)TotWSBkgPdf.CreateHistogram("TotWSBkgPdfHisto");
//   TotWSBkgPdfHisto->Draw("lego");
//   Canvas->Print(filename);

//   //create total WS pdf
//   DRecoAddPdf TotPdfWS("TotPdfWS","TotPdfWS",nset,TotalSigWSPdf,TotWSBkgPdf,TotYieldWS);
//   Canvas->Clear();
//   TH2F* TotPdfWSHisto=(TH2F*)TotPdfWS.CreateHistogram("TotPdfWSHisto");
//   TotPdfWSHisto->Draw("lego");
//   Canvas->Print(filename);

  
//   //----------------
//   //RS background model
//   //---------------------
//   //The background yield for each nXR will need to be floated so we need to create a 1D background pdf for each nXR
//   for(Int_t i=1;i<NXR;i++){//no data for nXR=0 
//       WSBkgnXRPdfRaw[i]=new DRecoProdPdf(TString("WSBkgnXRPdfRaw")+(long)i,TString("WSBkgnXRPdfRaw")+(long)i,nset,*WSBkgnXRSelectPdf[i],TotWSBkgPdf);
            
//       //Translation pdfs for nXR=0
//       delta[i]=new RooRealVar(TString("delta")+(long)i,TString("delta")+(long)i,i);
//       WSBkgnXRTransPdf[i]=new DRecoTranslationPdf(TString("WSBkgnXRTransPdf")+(long)i,TString("WSBkgnXRTransPdf")+(long)i,
// 						  nset,*WSBkgnXRPdfRaw[i],nxr,*delta[i]);
//       bkgaddcoef[i]=new RooRealVar(TString("bkgaddcoef")+(long)i,"",1./3.);

//       //Translation pdfs for nXR=1
//       delta1[i]=new RooRealVar(TString("delta1")+(long)i,TString("delta1")+(long)i,i-1);
//       WSBkgnXR1TransPdf[i]=new DRecoTranslationPdf(TString("WSBkgnXR1TransPdf")+(long)i,TString("WSBkgnXR1TransPdf")+(long)i,
// 						   nset,*WSBkgnXRPdfRaw[i],nxr,*delta1[i]);      
//       bkgaddcoef1[i]=new RooRealVar(TString("bkgaddcoef1")+(long)i,"",1./3.);
//   }
//   //create a bkg pdf for nXR=0
//   DRecoAddPdf WSBkgPdfnXR0AvgRaw("WSBkgPdfnXR0AvgRaw","WSBkgPdfnXR0AvgRaw",nset,
// 				 RooArgList(*WSBkgnXRTransPdf[1],*WSBkgnXRTransPdf[2],*WSBkgnXRTransPdf[3]),
// 				 RooArgList(*bkgaddcoef[1],*bkgaddcoef[2]));
//   WSBkgnXRPdf[0]=new DRecoProdPdf("WSBkgnXRPdf0","WSBkgnXRPdf0",nset,*WSnXR0CorrPdf,WSBkgPdfnXR0AvgRaw);

//   //create a bkg pdf for nXR=1
//   DRecoAddPdf WSBkgPdfnXR1AvgRaw("WSBkgPdfnXR1AvgRaw","WSBkgPdfnXR1AvgRaw",nset,
// 				 RooArgList(*WSBkgnXR1TransPdf[1],*WSBkgnXR1TransPdf[2],*WSBkgnXR1TransPdf[3]),
// 				 RooArgList(*bkgaddcoef1[1],*bkgaddcoef1[2]));
//   WSBkgnXRPdf[1]=new DRecoProdPdf("WSBkgnXRPdf1","WSBkgnXRPdf1",nset,*WSnXR1CorrPdf,WSBkgPdfnXR1AvgRaw);

                               
//   //no correction is necessary for nXR=2,3 
//   WSBkgnXRPdf[2]=WSBkgnXRPdfRaw[2];
//   WSBkgnXRPdf[3]=WSBkgnXRPdfRaw[3];



//   //----------------------------------------------------------------------------
//   //add all the nXR backgrounds to create the full 2D RS background
//   //-----------------------------------------------------------------------------  
//   RooArgList rsbkgyieldlist;
//   RooArgList rsbkgpdflist;
//   TH2F* WSnxRHisto[NXR];
//   Canvas->Clear();
//   Canvas->Divide(2,2);
//   for(Int_t i=0;i<NXR;i++){
//     rsbkgpdflist.add(*WSBkgnXRPdf[i]);
//     if(i<NXR-1) rsbkgyieldlist.add(*BkgnXRYield[i]);

//     Canvas->cd(i+1);    
//     //show the individual nXR backgrounds
//     WSnxRHisto[i]=(TH2F*)WSBkgnXRPdf[i]->CreateHistogram(TString("WSnxRHisto")+(long)i);
//     WSnxRHisto[i]->Draw("lego"); 
//   }  
//   Canvas->Print(filename);
//   for(Int_t i=0;i<NXR;i++) delete WSnxRHisto[i];
   

//   DRecoAddPdf TotRSBkgPdf("TotRSBkgPdf","TotRSBkgPdf",nset,rsbkgpdflist,rsbkgyieldlist);
//   Canvas->Clear();
//   TH2F* TotRSBkgPdfHisto=(TH2F*)TotRSBkgPdf.CreateHistogram("TotRSBkgPdfHisto");
//   TotRSBkgPdfHisto->Draw("lego");
//   Canvas->Print(filename);


//   ///Get the signal Pdfs from the signal MC 
//   //Use a weight model for the weights
//   RooRealIntegral* nXTCoef[NXT];
//   RooArgList pdflist;
//   RooArgList coeflist;
//   TH2F*HSigPdfHisto[NXT];
//   Canvas->Clear();
//   Canvas->Divide(2,3);
//   for(Int_t j=0;j<NXT;j++){    
//     pdflist.add(*SigPdf[j]);

//     nXTCoef[j]=new RooRealIntegral(TString("nXTCoef")+(long)j,TString("nXTCoef")+(long)j,*weightPdf,RooArgSet(nxtws),&nxtsetws,0,TString("wrange")+(long)j);
//     if(j<NXT-1)coeflist.add(*nXTCoef[j]);

//     Canvas->cd(j+1);
//     HSigPdfHisto[j]=new TH2F(TString("SigPdfn_{X}^{T}=")+(long)j,"",NXR,-.5,NXR-.5,NDsMassBins,DsMassMin,DsMassMax);
//     DsMassDataSetSig[j]->fillHistogram(HSigPdfHisto[j],RooArgList(nxr,mass));
//     HSigPdfHisto[j]->SetTitle(TString("n_{X}^{T}=")+(long)j);
//     HSigPdfHisto[j]->Draw("lego"); 
//   }    
//   Canvas->Print(filename); 
//   for(Int_t j=0;j<NXT;j++)delete HSigPdfHisto[j]; 
//   TH1F HnXTModelParTrue("HnXTModelParTrue","",NXT,-.5,NXT-.5); 
//   HnXTModelParTrue.Sumw2();
//   HnXTModelParTrue.SetBinContent(1,mean1.getVal());HnXTModelParTrue.SetBinError(1,.001);
//   HnXTModelParTrue.SetBinContent(2,sigma1.getVal());HnXTModelParTrue.SetBinError(2,.001);
//   HnXTModelParTrue.SetBinContent(3,frac.getVal());HnXTModelParTrue.SetBinError(3,.001);
//   HnXTModelParTrue.SetBinContent(4,mean2.getVal());HnXTModelParTrue.SetBinError(4,.001);
//   HnXTModelParTrue.SetBinContent(5,sigma2.getVal());HnXTModelParTrue.SetBinError(5,.001);

//   //construct the total signal pdf out of the addition of each nXT pdf
//   DRecoAddPdf TotalSigPdf("TotalSigPdf","TotalSigPdf",nset,pdflist,coeflist);
//   Canvas->Clear();
//   TH2F* TotalSigPdfHisto=(TH2F*)TotalSigPdf.CreateHistogram("TotalSigPdfHisto");
//   TotalSigPdfHisto->Draw("lego");
//   Canvas->Print(filename);  
 
//   //
//   DRecoAddPdf TotPdf("TotPdf","TotPdf",nset,TotalSigPdf,TotRSBkgPdf,TotYield);
//   Canvas->Clear();
//   TH2F* TotPdfHisto=(TH2F*)TotPdf.CreateHistogram("TotPdfHisto");
//   TotPdfHisto->Draw("lego");
//   Canvas->Print(filename);


//   //Get the Data histograms
//   RooDataHist DsMassDataSet(HDsMassData->GetName(),HDsMassData->GetName(),nset,HDsMassData);
//   Canvas->Clear();
//   HDsMassData->Draw("lego");
//   Canvas->Print(filename);
//   Canvas->Print(filename+"]");

//   ///------------------------------------------------------
//   //Fit
//   //----------------------------------------------------

//   //create a simultaneous chisquare
//   TList histlist;
//   histlist.Add(HDsMassDataWS);
//   histlist.Add(HDsMassData);

//   TList pdlist;
//   pdlist.Add(&TotPdfWS);
//   pdlist.Add(&TotPdf);

//   TList varlist;
//   varlist.Add(&nxr);
//   varlist.Add(&mass);
//   varlist.Add(&nxr);
//   varlist.Add(&mass);

//   RooArgSet paramset(nxr,mass);
//   paramset.add(TotYieldWS);
//   paramset.add(TotYield);
//   for(Int_t i=0;i<NXR-1;i++)
//     paramset.add(*BkgnXRYield[i]);

//   paramset.add(RooArgSet(mean1WS,sigma1WS,fracWS,mean2WS,sigma2WS));
//   paramset.add(RooArgSet(mean1,sigma1,frac,mean2,sigma2));
  
//   RooGenericPdf dummypdf("dummypdf","dummypdf","mass+nxr",paramset);
//   DRecoSimulChi2Var chi2("chi2","chi2",dummypdf,DsMassDataSetWS,&histlist,&pdlist,&varlist,RooFit::NumCPU(2));
//   //times for ncpu: 1= , 2= , 4= , 5= in kbar.  >2 cpu doesnt work increase chisquare
//   RooMinuit minuit(chi2);

//   //minimize
//   ULong_t starttime=(ULong_t)(gSystem->Now());//time the fit
//   //minuit.simplex();//does not improve fit time
//   minuit.migrad();
//   ULong_t time=(ULong_t)(gSystem->Now())-starttime;
//   cout<<"The fit time is "<<time/1000<<" seconds"<<endl;

//   RooFitResult* result=minuit.save("fitresult","fitresult");
//   Int_t fitstatus=result->status();
//   Int_t covqual=result->covQual();
//   Float_t chisq=chi2.getVal();
//   cout<<"Chisquare="<<chisq<<" fit status="<<fitstatus<<"  CovQual="<<covqual<<endl;

//   //--------------------------------------------------------------------
//   //Plot fits
//   //--------------------------------------------------------------------

//   //WS 2D plots 
//   TH2F* TotPdfHistoFitWS=(TH2F*)TotPdfWS.CreateHistogram("TotPdfHistoFitWS");
//   TotPdfHistoFitWS->Scale(HDsMassDataWS->Integral()/TotPdfHistoFitWS->Integral());

//   TH2F* TotBkgPdfHistoFitWS=(TH2F*)TotWSBkgPdf.CreateHistogram("TotBkgPdfHistoFitWS");
//   TotBkgPdfHistoFitWS->Scale((1-TotYieldWS.getVal())*HDsMassDataWS->Integral()/TotBkgPdfHistoFitWS->Integral());

//   TH2F* TotSigWSPdfHistoFit=(TH2F*)TotalSigWSPdf.CreateHistogram("TotSigWSPdfHistoFit");
//   TotSigWSPdfHistoFit->Scale(TotYieldWS.getVal()*HDsMassDataWS->Integral()/TotSigWSPdfHistoFit->Integral());
//   TotSigWSPdfHistoFit->SetBinContent(0,TotYieldWS.getVal()*HDsMassDataWS->Integral());
//   //TotSigWSPdfHistoFit->SetBinError(0,TotYieldWS.getError()*HDsMassDataWS->Integral());

//   //RS 2D plots
//   TH2F* TotPdfHistoFit=(TH2F*)TotPdf.CreateHistogram("TotPdfHistoFit");
//   TotPdfHistoFit->Scale(HDsMassData->Integral()/TotPdfHistoFit->Integral());

//   TH2F* TotBkgPdfHistoFit=(TH2F*)TotRSBkgPdf.CreateHistogram("TotBkgPdfHistoFit");
//   TotBkgPdfHistoFit->Scale((1-TotYield.getVal())*HDsMassData->Integral()/TotBkgPdfHistoFit->Integral());

//   TH2F* TotSigPdfHistoFit=(TH2F*)TotalSigPdf.CreateHistogram("TotSigPdfHistoFit");
//   TotSigPdfHistoFit->Scale(TotYield.getVal()*HDsMassData->Integral()/TotSigPdfHistoFit->Integral());
//   TotSigPdfHistoFit->SetBinContent(0,TotYield.getVal()*HDsMassData->Integral());
//   TotSigPdfHistoFit->SetBinError(0,TotYield.getError()*HDsMassData->Integral());  

  
//   //Save the weight model parameters
//   RooRealVar* WModelParWS[NXT-1];
//   WModelParWS[0]=&mean1WS;WModelParWS[1]=&sigma1WS;WModelParWS[2]=&fracWS;WModelParWS[3]=&mean2WS;WModelParWS[4]=&sigma2WS;
//   RooRealVar* WModelPar[NXT-1];
//   WModelPar[0]=&mean1;WModelPar[1]=&sigma1;WModelPar[2]=&frac;WModelPar[3]=&mean2;WModelPar[4]=&sigma2;

//   TH1F HnXTModelParWS("HnXTModelParWS","",NXT,-.5,NXT-.5);
//   TH1F HnXTModelPar("HnXTModelPar","",NXT,-.5,NXT-.5);
//   Float_t modelparWS[NXT];
//   Float_t modelpar[NXT];
//   for(Int_t j=0;j<NXT-1;j++){
//     HnXTModelParWS.SetBinContent(j+1,WModelParWS[j]->getVal());
//     HnXTModelParWS.SetBinError(j+1,WModelParWS[j]->getError());
//     HnXTModelPar.SetBinContent(j+1,WModelPar[j]->getVal());
//     HnXTModelPar.SetBinError(j+1,WModelPar[j]->getError());

//     modelparWS[j]=WModelParWS[j]->getVal();
//     modelpar[j]=WModelPar[j]->getVal();
//   }
//   Float_t weightWS[NXT];
//   Float_t weightWSMin[NXT];
//   Float_t weightWSMax[NXT];
//   Float_t weight[NXT];
//   Float_t weightMin[NXT];
//   Float_t weightMax[NXT];
//   for(Int_t j=0;j<NXT;j++){     
//     weightWS[j]=nXTCoefWS[j]->getVal();
//     weightWSMin[j]=1.;
//     weightWSMax[j]=0.;

//     weight[j]=nXTCoef[j]->getVal(); 
//     weightMin[j]=1.;
//     weightMax[j]=0.;

//   }


 
//   //compute the errors on the weights
//   //vary the errors on the weight model parameters until you find the largest deviation on the weight	
//   Int_t parsignWS[NXT];
//   Int_t parsign[NXT];
//   for(Int_t j=0;j<NXT-1;j++){//initialize
//     parsignWS[j]=-1;
//     parsign[j]=-1;
//   }
//   Int_t counter=0;
//   while(counter<pow(3.,(double)(NXT-1))){//for each parameter there are 3 possible vales x-e,x,x+e
//     counter++;

//     //cout<<counter;
//     for(Int_t j=0;j<NXT-1;j++){//change the parameters
//       // cout<<" "<<parsignWS[j];
//       if(modelparWS[j]+parsignWS[j]*WModelParWS[j]->getError()<WModelParWS[j]->getMin())
// 	WModelParWS[j]->setVal(WModelParWS[j]->getMin());
//       else if(modelparWS[j]+parsignWS[j]*WModelParWS[j]->getError()>WModelParWS[j]->getMax())
// 	WModelParWS[j]->setVal(WModelParWS[j]->getMax());
//       else WModelParWS[j]->setVal(modelparWS[j]+parsignWS[j]*WModelParWS[j]->getError());
    
//       if(modelpar[j]+parsign[j]*WModelPar[j]->getError()<WModelPar[j]->getMin())
// 	WModelPar[j]->setVal(WModelPar[j]->getMin());
//       else if(modelpar[j]+parsign[j]*WModelPar[j]->getError()>WModelPar[j]->getMax())
// 	WModelPar[j]->setVal(WModelPar[j]->getMax());
//       else WModelPar[j]->setVal(modelpar[j]+parsign[j]*WModelPar[j]->getError());
      
//     }
//     //cout<<endl;
//     for(Int_t j=0;j<NXT;j++){
//       if(weightWSMin[j]>nXTCoefWS[j]->getVal())
// 	weightWSMin[j]=nXTCoefWS[j]->getVal();
//       if(weightMin[j]>nXTCoef[j]->getVal())
// 	weightMin[j]=nXTCoef[j]->getVal();
      
//       if(weightWSMax[j]<nXTCoefWS[j]->getVal())
// 	weightWSMax[j]=nXTCoefWS[j]->getVal();
//       if(weightMax[j]<nXTCoef[j]->getVal())
// 	weightMax[j]=nXTCoef[j]->getVal();
//     }

//     //update signs
//     parsignWS[NXT-2]++;//start with the last one
//     parsign[NXT-2]++;
//     for(Int_t j=NXT-3;j>=0;j--){//update the ones which go outside limit
//       if(parsignWS[j+1]==2){
// 	parsignWS[j]++;
// 	parsignWS[j+1]=-1;
//       }
//       if(parsign[j+1]==2){
// 	parsign[j]++;
// 	parsign[j+1]=-1;
//       }
//     }

//   }
//   //need to restore to original values
//   for(Int_t j=0;j<NXT-1;j++){
//     WModelParWS[j]->setVal(modelparWS[j]);
//     WModelPar[j]->setVal(modelpar[j]);
//   }

//   //fill the weights
//   TH1F HnXTweightsWS("HnXTweightsWS","",NXT,-.5,NXT-.5);
//   TH1F HnXTweightsWSLoEr("HnXTweightsWSLoEr","",NXT,-.5,NXT-.5);
//   TH1F HnXTweightsWSHiEr("HnXTweightsWSHiEr","",NXT,-.5,NXT-.5);
//   TH1F HnXTweightsWSTrue("HnXTweightsWSTrue","",NXT,-.5,NXT-.5); 
//   TH1F HnXTweights("HnXTweights","",NXT,-.5,NXT-.5);
//   TH1F HnXTweightsLoEr("HnXTweightsLoEr","",NXT,-.5,NXT-.5);
//   TH1F HnXTweightsHiEr("HnXTweightsHiEr","",NXT,-.5,NXT-.5);
//   TH1F HnXTweightsTrue("HnXTweightsTrue","",NXT,-.5,NXT-.5);
//   for(Int_t j=0;j<NXT;j++){     
//     HnXTweightsWSTrue.SetBinContent(j+1,truewws[j]);
//     HnXTweightsWSTrue.SetBinError(j+1,truewws[j]/100.);//assign small error to be able to fit later
//     HnXTweightsWS.SetBinContent(j+1,weightWS[j]);
//     HnXTweightsWS.SetBinError(j+1,0);
//     HnXTweightsWSLoEr.SetBinContent(j+1,weightWS[j]-weightWSMin[j]);
//     HnXTweightsWSHiEr.SetBinContent(j+1,weightWSMax[j]-weightWS[j]);    
//     cout<<"nXT="<<j<<"  true_w_ws="<<truewws[j]<<"  fit_w_ws="<<weightWS[j]<<endl;
//   }
//   for(Int_t j=0;j<NXT;j++){ 
//     HnXTweightsTrue.SetBinContent(j+1,truew[j]);
//     HnXTweightsTrue.SetBinError(j+1,truew[j]/100.);
//     HnXTweights.SetBinContent(j+1,weight[j]);
//     HnXTweights.SetBinError(j+1,0); 
//     HnXTweightsLoEr.SetBinContent(j+1,weight[j]-weightMin[j]);
//     HnXTweightsHiEr.SetBinContent(j+1,weightMax[j]-weight[j]);
//     cout<<"nXT="<<j<<"  true_w="<<truew[j]<<"  fit_w="<<weight[j]<<endl;
//   }
  
//   //save status and chisquare
//   HnXTweights.SetBinContent(0,fitstatus);
//   HnXTweights.SetBinError(0,chisq);


//  //  //plot the correlation between two parameters
// //   TH2F*HParsCorrel=chi2.ScanParameter2D(&mean1,&sigma1);
// //   HParsCorrel->SetName("HParsCorrel");

//   //save results into a root file for fast plotting
//   TFile FResults(_OutputDir+"/"+resultFileName+".root","recreate");
//   HDsMassDataWS->SetName("HDsMassDataWS");HDsMassDataWS->Write();
//   HDsMassData->SetName("HDsMassData");HDsMassData->Write();
//   TotPdfHistoFitWS->Write();
//   TotBkgPdfHistoFitWS->Write();
//   TotSigWSPdfHistoFit->Write();
//   TotPdfHistoFit->Write();
//   TotBkgPdfHistoFit->Write();
//   TotSigPdfHistoFit->Write();
//   HnXTweightsWSTrue.Write();
//   HnXTweightsWS.Write();
//   HnXTweightsTrue.Write();
//   HnXTweights.Write();
//   HnXTweightsWSLoEr.Write();
//   HnXTweightsWSHiEr.Write();
//   HnXTweightsLoEr.Write();
//   HnXTweightsHiEr.Write();
//   HnXTModelParWS.Write();
//   HnXTModelPar.Write();
//   HnXTModelParWSTrue.Write();
//   HnXTModelParTrue.Write();
//   result->Write();
//   //HParsCorrel->Write();
//   FResults.Close();

// }

// void DRecoDsInclusiveAnalysis::RooFitDsMassvsnXRvsnXTSimulSemiWeightModel(TH2F*HDsMassDataWS,TH2F*HDsMassData,TString resultFileName){


//   filename=_OutputDir+"/"+resultFileName+".ps";
//   Canvas->Clear();
//   Canvas->Print(filename+"["); 
      

//   ///Get the WS signal Pdfs from the signal MC 
//   RooArgList pdflistws;
//   RooArgList coeflistWS;
//   RooRealIntegral* nXTCoefWS[NXT];
//   TH2F* SigWSPdfHisto[NXT];
//   Canvas->Clear();
//   Canvas->Divide(2,3);
//   for(Int_t j=0;j<NXT;j++){    

//     pdflistws.add(*SigPdfWS[j]);    

//     nXTCoefWS[j]=new RooRealIntegral(TString("nXTCoefWS")+(long)j,TString("nXTCoefWS")+(long)j,*weightPdfWS,RooArgSet(nxtws),&nxtsetws,0,TString("wrange")+(long)j);
//     if(j<NXT-1){
//       coeflistWS.add(*nXTCoefWS[j]);
//     }

//     Canvas->cd(j+1);
//     SigWSPdfHisto[j]=new TH2F(TString("SigPdfWSn_{X}^{T}=")+(long)j,"",NXR,-.5,NXR-.5,NDsMassBins,DsMassMin,DsMassMax);
//     SigPdfWS[j]->fillHistogram(SigWSPdfHisto[j],RooArgList(nxr,mass));
//     SigWSPdfHisto[j]->Scale(1./SigWSPdfHisto[j]->Integral());
//     SigWSPdfHisto[j]->SetTitle(TString("n_{X}^{T}=")+(long)j);
//     SigWSPdfHisto[j]->Draw("lego");
//   }    
//   Canvas->Print(filename); 
//   for(Int_t j=0;j<NXT;j++)  delete SigWSPdfHisto[j];
//   TH1F HnXTModelParWSTrue("HnXTModelParWSTrue","",NXT,-.5,NXT-.5);
//   HnXTModelParWSTrue.Sumw2();
//   HnXTModelParWSTrue.SetBinContent(1,mean1WS.getVal());HnXTModelParWSTrue.SetBinError(1,.001);
//   HnXTModelParWSTrue.SetBinContent(2,sigma1WS.getVal());HnXTModelParWSTrue.SetBinError(2,.001);
//   HnXTModelParWSTrue.SetBinContent(3,fracWS.getVal());HnXTModelParWSTrue.SetBinError(3,.001);
//   HnXTModelParWSTrue.SetBinContent(4,mean2WS.getVal());HnXTModelParWSTrue.SetBinError(4,.001);
//   HnXTModelParWSTrue.SetBinContent(5,sigma2WS.getVal());HnXTModelParWSTrue.SetBinError(5,.001);


//   //construct the total signal pdf out of the addition of each nXT pdf
//   DRecoAddPdf TotalSigWSPdf("TotalSigWSPdf","TotalSigWSPdf",nset,pdflistws,coeflistWS);
//   Canvas->Clear();
//   TH2F* TotalSigWSPdfHisto=(TH2F*)TotalSigWSPdf.CreateHistogram("TotalSigWSPdfHisto");
//   TotalSigWSPdfHisto->Draw("lego");
//   Canvas->Print(filename);


//   //Create the WS Dataset to be fited
//   RooDataHist DsMassDataSetWS("DsMassDataSetWS","DsMassDataSetWS",nset,HDsMassDataWS);
//   Canvas->Clear();
//   HDsMassDataWS->Draw("lego");
//   Canvas->Print(filename);
  
//   RooRealVar RSToWSRatio("RSToWSRatio","RSToWSRatio",HDsMassData->Integral()/HDsMassDataWS->Integral());
//   RooFormulaVar TotYieldWS("TotYieldWS","TotYieldWS","WStoRSFrac*TotYield*RSToWSRatio",RooArgSet(TotYield,RSToWSRatio,WStoRSFrac));
//   //RooRealVar TotYieldWS("TotYieldWS","TotYieldWS",.04,.25);

//   //create a 2D background pdf from the WS Data - WS signal pdf
//   DRecoHistPdf DsMassWSPdf("DsMassWSPdf","DsMassWSPdf",nset,DsMassDataSetWS);
//   RooFormulaVar TotWSFracYield("TotWSFracYield","TotWSFracYield","-(1./(1.-TotYieldWS)-1.)",RooArgSet(TotYieldWS));
//   DRecoAddPdf TotWSBkgPdf("TotWSBkgPdf","TotWSBkgPdf",nset,TotalSigWSPdf,DsMassWSPdf,TotWSFracYield);
//   Canvas->Clear();
//   TH2F* TotWSBkgPdfHisto=(TH2F*)TotWSBkgPdf.CreateHistogram("TotWSBkgPdfHisto");
//   TotWSBkgPdfHisto->Draw("lego");
//   Canvas->Print(filename);

//   //create total WS pdf
//   DRecoAddPdf TotPdfWS("TotPdfWS","TotPdfWS",nset,TotalSigWSPdf,TotWSBkgPdf,TotYieldWS);
//   Canvas->Clear();
//   TH2F* TotPdfWSHisto=(TH2F*)TotPdfWS.CreateHistogram("TotPdfWSHisto");
//   TotPdfWSHisto->Draw("lego");
//   Canvas->Print(filename);

  
//   //----------------
//   //RS background model
//   //---------------------
//   //The background yield for each nXR will need to be floated so we need to create a 1D background pdf for each nXR
//   for(Int_t i=1;i<NXR;i++){//no data for nXR=0 
//       WSBkgnXRPdfRaw[i]=new DRecoProdPdf(TString("WSBkgnXRPdfRaw")+(long)i,TString("WSBkgnXRPdfRaw")+(long)i,nset,*WSBkgnXRSelectPdf[i],TotWSBkgPdf);
            
//       //Translation pdfs for nXR=0
//       delta[i]=new RooRealVar(TString("delta")+(long)i,TString("delta")+(long)i,i);
//       WSBkgnXRTransPdf[i]=new DRecoTranslationPdf(TString("WSBkgnXRTransPdf")+(long)i,TString("WSBkgnXRTransPdf")+(long)i,
// 						  nset,*WSBkgnXRPdfRaw[i],nxr,*delta[i]);
//       bkgaddcoef[i]=new RooRealVar(TString("bkgaddcoef")+(long)i,"",1./3.);

//       //Translation pdfs for nXR=1
//       delta1[i]=new RooRealVar(TString("delta1")+(long)i,TString("delta1")+(long)i,i-1);
//       WSBkgnXR1TransPdf[i]=new DRecoTranslationPdf(TString("WSBkgnXR1TransPdf")+(long)i,TString("WSBkgnXR1TransPdf")+(long)i,
// 						   nset,*WSBkgnXRPdfRaw[i],nxr,*delta1[i]);      
//       bkgaddcoef1[i]=new RooRealVar(TString("bkgaddcoef1")+(long)i,"",1./3.);
//   }
//   //create a bkg pdf for nXR=0
//   DRecoAddPdf WSBkgPdfnXR0AvgRaw("WSBkgPdfnXR0AvgRaw","WSBkgPdfnXR0AvgRaw",nset,
// 				 RooArgList(*WSBkgnXRTransPdf[1],*WSBkgnXRTransPdf[2],*WSBkgnXRTransPdf[3]),
// 				 RooArgList(*bkgaddcoef[1],*bkgaddcoef[2]));
//   WSBkgnXRPdf[0]=new DRecoProdPdf("WSBkgnXRPdf0","WSBkgnXRPdf0",nset,*WSnXR0CorrPdf,WSBkgPdfnXR0AvgRaw);
  
//   //create a bkg pdf for nXR=1
//   DRecoAddPdf WSBkgPdfnXR1AvgRaw("WSBkgPdfnXR1AvgRaw","WSBkgPdfnXR1AvgRaw",nset,
// 				 RooArgList(*WSBkgnXR1TransPdf[1],*WSBkgnXR1TransPdf[2],*WSBkgnXR1TransPdf[3]),
// 				 RooArgList(*bkgaddcoef1[1],*bkgaddcoef1[2]));
//   WSBkgnXRPdf[1]=new DRecoProdPdf("WSBkgnXRPdf1","WSBkgnXRPdf1",nset,*WSnXR1CorrPdf,WSBkgPdfnXR1AvgRaw);
                                   
//   //no correction is necessary for nXR=2,3 
//   WSBkgnXRPdf[2]=WSBkgnXRPdfRaw[2];
//   WSBkgnXRPdf[3]=WSBkgnXRPdfRaw[3];



//   //----------------------------------------------------------------------------
//   //add all the nXR backgrounds to create the full 2D RS background
//   //-----------------------------------------------------------------------------  
//   RooArgList rsbkgyieldlist;
//   RooArgList rsbkgpdflist;
//   TH2F* WSnxRHisto[NXR];
//   Canvas->Clear();
//   Canvas->Divide(2,2);
//   for(Int_t i=0;i<NXR;i++){
//     rsbkgpdflist.add(*WSBkgnXRPdf[i]);
//     if(i<NXR-1) rsbkgyieldlist.add(*BkgnXRYield[i]);

//     Canvas->cd(i+1);    
//     //show the individual nXR backgrounds
//     WSnxRHisto[i]=(TH2F*)WSBkgnXRPdf[i]->CreateHistogram(TString("WSnxRHisto")+(long)i);
//     WSnxRHisto[i]->Draw("lego"); 
//   }  
//   Canvas->Print(filename);
//   for(Int_t i=0;i<NXR;i++) delete WSnxRHisto[i];
   

//   DRecoAddPdf TotRSBkgPdf("TotRSBkgPdf","TotRSBkgPdf",nset,rsbkgpdflist,rsbkgyieldlist);
//   Canvas->Clear();
//   TH2F* TotRSBkgPdfHisto=(TH2F*)TotRSBkgPdf.CreateHistogram("TotRSBkgPdfHisto");
//   TotRSBkgPdfHisto->Draw("lego");
//   Canvas->Print(filename);


//   ///Get the signal Pdfs from the signal MC 
//   //Use a weight model for the weights
//   RooRealVar* nXTCoef[NXT];
//   RooArgList pdflist;
//   RooArgList coeflist;
//   TH2F*HSigPdfHisto[NXT];
//   Canvas->Clear();
//   Canvas->Divide(2,3);
//   for(Int_t j=0;j<NXT;j++){    
//     pdflist.add(*SigPdf[j]);

//     Canvas->cd(j+1);
//     HSigPdfHisto[j]=new TH2F(TString("SigPdfn_{X}^{T}=")+(long)j,"",NXR,-.5,NXR-.5,NDsMassBins,DsMassMin,DsMassMax);
//     DsMassDataSetSig[j]->fillHistogram(HSigPdfHisto[j],RooArgList(nxr,mass));
//     HSigPdfHisto[j]->SetTitle(TString("n_{X}^{T}=")+(long)j);
//     HSigPdfHisto[j]->Draw("lego"); 
//   }    
//   Canvas->Print(filename); 
//   for(Int_t j=0;j<NXT;j++)delete HSigPdfHisto[j]; 

//   for(Int_t j=0;j<3;j++){    
//     nXTCoef[j]=new RooRealVar(TString("nXTCoef")+(long)j,TString("nXTCoef")+(long)j,.01,.5);          
//     nXTCoef[j]->setVal(truew[j]);  
//     //nXTCoef[j]->setConstant(1);  
//     coeflist.add(*nXTCoef[j]);
//   }
//   RooRealIntegral* nXTCoefInt[NXT];
//   RooFormulaVar* nXTCoefWM[NXT];
//   RooFormulaVar WMInteg("WMInteg","WMInteg","1-nXTCoef0-nXTCoef1-nXTCoef2",RooArgSet(*nXTCoef[0],*nXTCoef[1],*nXTCoef[2]));
//   for(Int_t j=3;j<NXT;j++){    
//     nXTCoefInt[j]=new RooRealIntegral(TString("nXTCoefInt")+(long)j,TString("nXTCoefInt")+(long)j,
// 				      *weightPdf,RooArgSet(nxt),&nxtset,0,TString("wrange")+(long)j);
//     nXTCoefWM[j]=new RooFormulaVar(TString("nXTCoefWM")+(long)j,TString("nXTCoefWM")+(long)j,
// 				   TString("WMInteg*nXTCoefInt")+(long)j,RooArgSet(*nXTCoefInt[j],WMInteg));
//     //if(j<NXT-1)
//     coeflist.add(*nXTCoefWM[j]);
//   }

//   TH1F HnXTModelParTrue("HnXTModelParTrue","",NXT,-.5,NXT-.5); 
//   HnXTModelParTrue.Sumw2();
//   HnXTModelParTrue.SetBinContent(1,mean1.getVal());HnXTModelParTrue.SetBinError(1,.001);
//   HnXTModelParTrue.SetBinContent(2,sigma1.getVal());HnXTModelParTrue.SetBinError(2,.001);
//   HnXTModelParTrue.SetBinContent(3,frac.getVal());HnXTModelParTrue.SetBinError(3,.001);
//   HnXTModelParTrue.SetBinContent(4,mean2.getVal());HnXTModelParTrue.SetBinError(4,.001);
//   HnXTModelParTrue.SetBinContent(5,sigma2.getVal());HnXTModelParTrue.SetBinError(5,.001);

//   //construct the total signal pdf out of the addition of each nXT pdf
//   DRecoAddPdf TotalSigPdf("TotalSigPdf","TotalSigPdf",nset,pdflist,coeflist);
//   Canvas->Clear();
//   TH2F* TotalSigPdfHisto=(TH2F*)TotalSigPdf.CreateHistogram("TotalSigPdfHisto");
//   TotalSigPdfHisto->Draw("lego");
//   Canvas->Print(filename);  
 
//   //
//   DRecoAddPdf TotPdf("TotPdf","TotPdf",nset,TotalSigPdf,TotRSBkgPdf,TotYield);
//   Canvas->Clear();
//   TH2F* TotPdfHisto=(TH2F*)TotPdf.CreateHistogram("TotPdfHisto");
//   TotPdfHisto->Draw("lego");
//   Canvas->Print(filename);


//   //Get the Data histograms
//   RooDataHist DsMassDataSet(HDsMassData->GetName(),HDsMassData->GetName(),nset,HDsMassData);
//   Canvas->Clear();
//   HDsMassData->Draw("lego");
//   Canvas->Print(filename);
//   Canvas->Print(filename+"]");

//   ///------------------------------------------------------
//   //Fit
//   //----------------------------------------------------

//   //create a simultaneous chisquare
//   TList histlist;
//   histlist.Add(HDsMassDataWS);
//   histlist.Add(HDsMassData);

//   TList pdlist;
//   pdlist.Add(&TotPdfWS);
//   pdlist.Add(&TotPdf);

//   TList varlist;
//   varlist.Add(&nxr);
//   varlist.Add(&mass);
//   varlist.Add(&nxr);
//   varlist.Add(&mass);

//   RooArgSet paramset(nxr,mass);
//   paramset.add(TotYieldWS);
//   paramset.add(TotYield);
//   for(Int_t i=0;i<NXR-1;i++)
//     paramset.add(*BkgnXRYield[i]);

//   for(Int_t i=0;i<3;i++)
//     paramset.add(*nXTCoef[i]);

//   paramset.add(RooArgSet(mean1WS,sigma1WS,fracWS,mean2WS,sigma2WS));
//   paramset.add(RooArgSet(mean1,sigma1,frac,mean2,sigma2));
  
//   RooGenericPdf dummypdf("dummypdf","dummypdf","mass+nxr",paramset);
//   DRecoSimulChi2Var chi2("chi2","chi2",dummypdf,DsMassDataSetWS,&histlist,&pdlist,&varlist,RooFit::NumCPU(2));
//   //times for ncpu: 1= , 2= ,  >2 cpu doesnt work increase chisquare
//   RooMinuit minuit(chi2);

//   //minimize
//   ULong_t starttime=(ULong_t)(gSystem->Now());//time the fit
//   minuit.simplex();//does not improve fit time
//   minuit.migrad();
//   ULong_t time=(ULong_t)(gSystem->Now())-starttime;
//   cout<<"The fit time is "<<time/1000<<" seconds"<<endl;

//   RooFitResult* result=minuit.save("fitresult","fitresult");
//   Int_t fitstatus=result->status();
//   Int_t covqual=result->covQual();
//   Float_t chisq=chi2.getVal();
//   cout<<"Chisquare="<<chisq<<" fit status="<<fitstatus<<"  CovQual="<<covqual<<endl;

//   //--------------------------------------------------------------------
//   //Plot fits
//   //--------------------------------------------------------------------

//   //WS 2D plots 
//   TH2F* TotPdfHistoFitWS=(TH2F*)TotPdfWS.CreateHistogram("TotPdfHistoFitWS");
//   TotPdfHistoFitWS->Scale(HDsMassDataWS->Integral()/TotPdfHistoFitWS->Integral());

//   TH2F* TotBkgPdfHistoFitWS=(TH2F*)TotWSBkgPdf.CreateHistogram("TotBkgPdfHistoFitWS");
//   TotBkgPdfHistoFitWS->Scale((1-TotYieldWS.getVal())*HDsMassDataWS->Integral()/TotBkgPdfHistoFitWS->Integral());

//   TH2F* TotSigWSPdfHistoFit=(TH2F*)TotalSigWSPdf.CreateHistogram("TotSigWSPdfHistoFit");
//   TotSigWSPdfHistoFit->Scale(TotYieldWS.getVal()*HDsMassDataWS->Integral()/TotSigWSPdfHistoFit->Integral());
//   TotSigWSPdfHistoFit->SetBinContent(0,TotYieldWS.getVal()*HDsMassDataWS->Integral());
//   //TotSigWSPdfHistoFit->SetBinError(0,TotYieldWS.getError()*HDsMassDataWS->Integral());

//   //RS 2D plots
//   TH2F* TotPdfHistoFit=(TH2F*)TotPdf.CreateHistogram("TotPdfHistoFit");
//   TotPdfHistoFit->Scale(HDsMassData->Integral()/TotPdfHistoFit->Integral());

//   TH2F* TotBkgPdfHistoFit=(TH2F*)TotRSBkgPdf.CreateHistogram("TotBkgPdfHistoFit");
//   TotBkgPdfHistoFit->Scale((1-TotYield.getVal())*HDsMassData->Integral()/TotBkgPdfHistoFit->Integral());

//   TH2F* TotSigPdfHistoFit=(TH2F*)TotalSigPdf.CreateHistogram("TotSigPdfHistoFit");
//   TotSigPdfHistoFit->Scale(TotYield.getVal()*HDsMassData->Integral()/TotSigPdfHistoFit->Integral());
//   TotSigPdfHistoFit->SetBinContent(0,TotYield.getVal()*HDsMassData->Integral());
//   TotSigPdfHistoFit->SetBinError(0,TotYield.getError()*HDsMassData->Integral());  

  
//   //Save the weight model parameters
//   RooRealVar* WModelParWS[NXT-1];
//   WModelParWS[0]=&mean1WS;WModelParWS[1]=&sigma1WS;WModelParWS[2]=&fracWS;WModelParWS[3]=&mean2WS;WModelParWS[4]=&sigma2WS;
//   RooRealVar* WModelPar[NXT-1];
//   WModelPar[0]=&mean1;WModelPar[1]=&sigma1;WModelPar[2]=&frac;WModelPar[3]=&mean2;WModelPar[4]=&sigma2;

//   TH1F HnXTModelParWS("HnXTModelParWS","",NXT,-.5,NXT-.5);
//   TH1F HnXTModelPar("HnXTModelPar","",NXT,-.5,NXT-.5);
//   Float_t modelparWS[NXT-1];
//   Float_t modelpar[NXT-1];
//   for(Int_t j=0;j<NXT-1;j++){
//     HnXTModelParWS.SetBinContent(j+1,WModelParWS[j]->getVal());
//     HnXTModelParWS.SetBinError(j+1,WModelParWS[j]->getError());
//     HnXTModelPar.SetBinContent(j+1,WModelPar[j]->getVal());
//     HnXTModelPar.SetBinError(j+1,WModelPar[j]->getError());

//     modelparWS[j]=WModelParWS[j]->getVal();
//     modelpar[j]=WModelPar[j]->getVal();
//   }
//   Float_t weightWS[NXT];
//   //Float_t lastwws=1.;
//   Float_t weightWSMin[NXT];
//   Float_t weightWSMax[NXT];
//   for(Int_t j=0;j<NXT;j++){     
//     weightWS[j]=nXTCoefWS[j]->getVal();
//     weightWSMin[j]=1.;
//     weightWSMax[j]=0.;
//     //    lastwws-=weightWS[j];
//   }
//   //weightWS[NXT-1]=lastwws;

//   Float_t weight[NXT];
//   //Float_t lastw=1.;
//   Float_t weightMin[NXT];
//   Float_t weightMax[NXT];
//   for(Int_t j=0;j<3;j++){     
//     weight[j]=nXTCoef[j]->getVal(); 
//     //lastw-=weight[j];
//   }
//   for(Int_t j=3;j<NXT;j++){     
//     weight[j]=nXTCoefWM[j]->getVal(); 
//     weightMin[j]=1.;
//     weightMax[j]=0.;
//     //lastw-=weight[j];
//   }
//   //weight[NXT-1]=lastw;

 
//   //compute the errors on the weights
// //   weightWSMin[NXT-1]=weightWS[NXT-1]-.5*weightWS[NXT-1];//error on last weight cannot be calculated here it needs full cov. matrix
// //   weightWSMax[NXT-1]=weightWS[NXT-1]+.5*weightWS[NXT-1];

//   for(Int_t j=0;j<3;j++){//first 3 weights have proper errors
//     weightMin[j]=nXTCoef[j]->getVal()-nXTCoef[j]->getError();      
//     weightMax[j]=nXTCoef[j]->getVal()+nXTCoef[j]->getError();
//   }
// //   weightMin[NXT-1]=weight[NXT-1]-.5*weight[NXT-1];
// //   weightMax[NXT-1]=weight[NXT-1]+.5*weight[NXT-1];

//   //vary the errors on the weight model parameters until you find the largest deviation on the weight	
//   Int_t parsignWS[NXT];
//   Int_t parsign[NXT];
//   for(Int_t j=0;j<NXT-1;j++){//initialize
//     parsignWS[j]=-1;
//     parsign[j]=-1;
//   }
//   Int_t counter=0;
//   while(counter<pow(3.,(double)(NXT-1))){//for each parameter there are 3 possible vales x-e,x,x+e
//     counter++;

//     //cout<<counter;
//     for(Int_t j=0;j<NXT-1;j++){//change the parameters
//       // cout<<" "<<parsignWS[j];
//       if(modelparWS[j]+parsignWS[j]*WModelParWS[j]->getError()<WModelParWS[j]->getMin())
// 	WModelParWS[j]->setVal(WModelParWS[j]->getMin());
//       else if(modelparWS[j]+parsignWS[j]*WModelParWS[j]->getError()>WModelParWS[j]->getMax())
// 	WModelParWS[j]->setVal(WModelParWS[j]->getMax());
//       else WModelParWS[j]->setVal(modelparWS[j]+parsignWS[j]*WModelParWS[j]->getError());
    
//       if(modelpar[j]+parsign[j]*WModelPar[j]->getError()<WModelPar[j]->getMin())
// 	WModelPar[j]->setVal(WModelPar[j]->getMin());
//       else if(modelpar[j]+parsign[j]*WModelPar[j]->getError()>WModelPar[j]->getMax())
// 	WModelPar[j]->setVal(WModelPar[j]->getMax());
//       else WModelPar[j]->setVal(modelpar[j]+parsign[j]*WModelPar[j]->getError());
      
//     }
//     //cout<<endl;
//     for(Int_t j=0;j<NXT;j++){
//       if(weightWSMin[j]>nXTCoefWS[j]->getVal())
// 	weightWSMin[j]=nXTCoefWS[j]->getVal();      
//       if(weightWSMax[j]<nXTCoefWS[j]->getVal())
// 	weightWSMax[j]=nXTCoefWS[j]->getVal();
//     }
//     for(Int_t j=3;j<NXT;j++){
//       if(weightMin[j]>nXTCoefWM[j]->getVal())
// 	weightMin[j]=nXTCoefWM[j]->getVal();      
//       if(weightMax[j]<nXTCoefWM[j]->getVal())
// 	weightMax[j]=nXTCoefWM[j]->getVal();
//     }

//     //update signs
//     parsignWS[NXT-2]++;//start with the last one
//     parsign[NXT-2]++;
//     for(Int_t j=NXT-3;j>=0;j--){//update the ones which go outside limit
//       if(parsignWS[j+1]==2){
// 	parsignWS[j]++;
// 	parsignWS[j+1]=-1;
//       }
//       if(parsign[j+1]==2){
// 	parsign[j]++;
// 	parsign[j+1]=-1;
//       }
//     }

//   }
//   //need to restore to original values
//   for(Int_t j=0;j<NXT-1;j++){
//     WModelParWS[j]->setVal(modelparWS[j]);
//     WModelPar[j]->setVal(modelpar[j]);
//   }

//   //fill the weights
//   TH1F HnXTweightsWS("HnXTweightsWS","",NXT,-.5,NXT-.5);
//   TH1F HnXTweightsWSLoEr("HnXTweightsWSLoEr","",NXT,-.5,NXT-.5);
//   TH1F HnXTweightsWSHiEr("HnXTweightsWSHiEr","",NXT,-.5,NXT-.5);
//   TH1F HnXTweightsWSTrue("HnXTweightsWSTrue","",NXT,-.5,NXT-.5); 
//   TH1F HnXTweights("HnXTweights","",NXT,-.5,NXT-.5);
//   TH1F HnXTweightsLoEr("HnXTweightsLoEr","",NXT,-.5,NXT-.5);
//   TH1F HnXTweightsHiEr("HnXTweightsHiEr","",NXT,-.5,NXT-.5);
//   TH1F HnXTweightsTrue("HnXTweightsTrue","",NXT,-.5,NXT-.5);
//   for(Int_t j=0;j<NXT;j++){     
//     HnXTweightsWSTrue.SetBinContent(j+1,truewws[j]);
//     HnXTweightsWSTrue.SetBinError(j+1,truewws[j]/100.);//assign small error to be able to fit later
//     HnXTweightsWS.SetBinContent(j+1,weightWS[j]);
//     HnXTweightsWS.SetBinError(j+1,0);
//     HnXTweightsWSLoEr.SetBinContent(j+1,weightWS[j]-weightWSMin[j]);
//     HnXTweightsWSHiEr.SetBinContent(j+1,weightWSMax[j]-weightWS[j]);    
//     cout<<"nXT="<<j<<"  true_w_ws="<<truewws[j]<<"  fit_w_ws="<<weightWS[j]<<endl;
//   }
//   for(Int_t j=0;j<NXT;j++){ 
//     HnXTweightsTrue.SetBinContent(j+1,truew[j]);
//     HnXTweightsTrue.SetBinError(j+1,truew[j]/100.);
//     HnXTweights.SetBinContent(j+1,weight[j]);
//     HnXTweights.SetBinError(j+1,0); 
//     HnXTweightsLoEr.SetBinContent(j+1,weight[j]-weightMin[j]);
//     HnXTweightsHiEr.SetBinContent(j+1,weightMax[j]-weight[j]);
//     cout<<"nXT="<<j<<"  true_w="<<truew[j]<<"  fit_w="<<weight[j]<<endl;
//   }
  
//   //save status and chisquare
//   HnXTweights.SetBinContent(0,fitstatus);
//   HnXTweights.SetBinError(0,chisq);

//   //plot the correlation between two parameters
//   TGraph*GParScan=chi2.ScanParameter(&frac);
//   if(!GParScan)GParScan=chi2.ScanParameter(&sigma2);
//   TH2F*HParsCorrel=chi2.ScanParameter2D(&frac,&sigma2);  

//   //save results into a root file for fast plotting
//   TFile FResults(_OutputDir+"/"+resultFileName+".root","recreate");
//   HDsMassDataWS->SetName("HDsMassDataWS");HDsMassDataWS->Write();
//   HDsMassData->SetName("HDsMassData");HDsMassData->Write();
//   TotPdfHistoFitWS->Write();
//   TotBkgPdfHistoFitWS->Write();
//   TotSigWSPdfHistoFit->Write();
//   TotPdfHistoFit->Write();
//   TotBkgPdfHistoFit->Write();
//   TotSigPdfHistoFit->Write();
//   HnXTweightsWSTrue.Write();
//   HnXTweightsWS.Write();
//   HnXTweightsTrue.Write();
//   HnXTweights.Write();
//   HnXTweightsWSLoEr.Write();
//   HnXTweightsWSHiEr.Write();
//   HnXTweightsLoEr.Write();
//   HnXTweightsHiEr.Write();
//   HnXTModelParWS.Write();
//   HnXTModelPar.Write();
//   HnXTModelParWSTrue.Write();
//   HnXTModelParTrue.Write();
//   result->Write();
//   if(GParScan){
//     GParScan->SetName("GParScan");
//     GParScan->Write();
//   }
//   if(HParsCorrel){
//     HParsCorrel->SetName("HParsCorrel");
//     HParsCorrel->Write();
//   }
//   FResults.ls();
//   FResults.Close();

// }





// void DRecoDsInclusiveAnalysis::RooFitDsMassWS(){
//   Canvas->Clear();
//   filename=_OutputDir+"/RooFitDsMassWS.ps";
//   Canvas->Print(filename+"[");

//   mass.setRange(DsMassMin,DsMassMax);
//   mass.setBins(NDsMassBins);


//   ///Get signal pdf from signal ws sample
//   TFile PdfsFile("DsTolnu/SP1005Skim/DsReco/FinalWS/HistosForFit.root","read"); 
//   gROOT->cd();
//   TH1F* HDsMassWS=(TH1F*)PdfsFile.Get("HDsMassTrue");
//   if(!HDsMassWS){cout<<" Not found"<<endl;return;}  
//   //HDsMassWS->Smooth(3);  
//   RooDataHist WSSigDataSet(TString("WSSigDataSet"),TString("WSSigDataSet"),RooArgSet(mass),HDsMassWS);
//   DRecoHistPdf WSSigPdf(TString("WSSigPdf"),TString("WSSigPdf"),RooArgSet(mass),WSSigDataSet,0);
//   Canvas->Clear();
//   HDsMassWS->Draw();
//   Canvas->Print(filename); 
 

  
  
//   TFile DataFile(_OutputDir+"/HistosForFit.root","read"); 
//   gROOT->cd();
//   TH1F* HDsMassData=(TH1F*)DataFile.Get("HDsMass");
//   if(!HDsMassData){cout<<" Not found"<<endl;return;} 
//   RooDataHist DsMassDataSet(HDsMassData->GetName(),HDsMassData->GetName(),RooArgSet(mass),HDsMassData);     
//   Canvas->Clear();
//   HDsMassData->Draw();
//   Canvas->Print(filename); 
  

//   RooRealVar SigYield(TString("SigYield"),TString("SigYield"),.01,.2);

//   //Create a pdf out of the subtraction of the wrong-sign signalMC from total wrong-sign.
//   RooRealVar WSSigYield("WSSigYield","WSSigYield",-.2,-.01);  
//   DRecoHistPdf TotWSPdf(TString("TotWspdf"),TString("TotWSpdf"),RooArgSet(mass),DsMassDataSet,0);
//   DRecoAddPdf WSBkgPdf(TString("WSBkgPdf"),TString("WSBkgPdf"),WSSigPdf,TotWSPdf,WSSigYield);

//   //create total pdf.
//   DRecoAddPdf TotalPdf(TString("TotalPdf"),TString("TotalPdf"),WSSigPdf,WSBkgPdf,SigYield);

//   RooPlot*plots=mass.frame();
  
//   Canvas->Clear();
//   WSBkgPdf.plotOn(plots,LineColor(2));
//   WSSigPdf.plotOn(plots);
//   plots->Draw();
//   Canvas->Print(filename);  
//   delete plots;

//   //TotalPdf.fitTo(DsMassDataSet);
//   DRecoHistPdfChi2Var chi2("chi2","chi2",TotalPdf,DsMassDataSet);
//   chi2.SetEType(RooAbsData::SumW2);
//   RooMinuit minuit(chi2);
//   minuit.migrad();

//   plots=mass.frame();
//   DsMassDataSet.plotOn(plots,RooFit::DataError(RooAbsData::SumW2);
//   TotalPdf.plotOn(plots);
//   WSBkgPdf.plotOn(plots,LineColor(2),FillColor(2),FillStyle(1001),DrawOption("LF"),Normalization(1.-SigYield.getVal()));
//   plots->SetTitle(TString("Signal Yield=")+(long)(SigYield.getVal()*HDsMassData->Integral())+" +- "+(long)(SigYield.getError()*HDsMassData->Integral()));


//   //Make Residuals
//   TH1* HFit=TotalPdf.createHistogram("HFit",mass);  
//   HFit->Scale(HDsMassData->Integral());
//   DRecoResiduals residuals("resid",HDsMassData,HFit);

//   Canvas->Clear();
//   residuals.MakeFitPlotWithResiduals(Canvas,plots);
//   Canvas->Print(filename);  
 
//   Canvas->Print(filename+"]");


//   //Save the Wrong-Sign shape
//   TH1* HWSshape= WSBkgPdf.createHistogram("HWSshape",mass);
//   HWSshape->SetName("HDsMass");
//   HWSshape->Scale(((1.-SigYield.getVal())*HDsMassData->Integral())/HWSshape->Integral());
//   for(Int_t b=1;b<=HDsMassData->GetXaxis()->GetNbins();b++)
//     HWSshape->SetBinError(b,sqrt(HDsMassData->GetBinError(b)*HDsMassData->GetBinError(b)
// 				 + HDsMassWS->GetBinError(b)*HDsMassWS->GetBinError(b)));//assign stat errors for now
//   TFile ResultsFile(_OutputDir+"/RooFitDsMassWS.root","recreate"); 
//   HWSshape->Write();
//   plots->SetName("HWSplot");
//   plots->Write();
//   ResultsFile.ls();
//   ResultsFile.Close();

//   delete HWSshape;
//   delete plots;

//   PdfsFile.Close();
//   DataFile.Close(); 

// }


// void DRecoDsInclusiveAnalysis::RooFitDsMass(){

//   filename=_OutputDir+"/DsMassRooFit.ps";
//   Canvas->Clear();
//   Canvas->Print(filename+"["); 
  
//   mass.setRange(DsMassMin,DsMassMax);
//   mass.setBins(NDsMassBins);

//   TFile PdfsFile("DsTolnu/SP1005Skim/DsReco/Final/HistosForFit.root","read"); 
//   gROOT->cd();
//   TH1F* HDsMassTrue=(TH1F*)PdfsFile.Get("HDsMassTrue");
//   if(!HDsMassTrue){cout<<" Not found"<<endl;return;}    
//   RooDataHist Hdatahist("Hdatahist","Hdatahist",RooArgSet(mass),HDsMassTrue);
//   DRecoHistPdf SigPdf("SigPdf","SigPdf",RooArgSet(mass),Hdatahist,0);
//   Canvas->Clear();
//   HDsMassTrue->Draw();
//   Canvas->Print(filename); 
 
  

//   //Get the bkg pdfs
//   TFile BkgPdfsFile("DsTolnu/SP1005Skim/DsReco/FinalWS/RooFitDsMassWS.root","read");
//   gROOT->cd();  
//   TH1F* HDsMassRBkg=(TH1F*)BkgPdfsFile.Get("HDsMass");
//   if(!HDsMassRBkg){cout<<" Not found"<<endl;return;}    
//   RooDataHist HdatahistBkg("HdatahistBkg","HdatahistBkg",RooArgSet(mass),HDsMassRBkg);
//   DRecoHistPdf BkgPdf(TString("BkgPdf"),TString("BkgPdf"),RooArgSet(mass),HdatahistBkg,0);
// //   DRecoHistPdf BkgPdfRaw(TString("HpdfBkgRaw"),TString("HpdfBkgRaw"),RooArgSet(mass),HdatahistBkg,0);
// //   RooRealVar BkgCorrCoef1("BkgCorrCoef1","BkgCorrCoef1",-1,1);
// //   RooRealVar BkgCorrCoef2("BkgCorrCoef2","BkgCorrCoef2",-1,1);
// //   RooGenericPdf BkgSlopePdf("BkgSlopePdf","BkgSlopePdf","1+.5*BkgCorrCoef1*(mass-1.82)/.3+.5*BkgCorrCoef2*((mass-1.82)/.3)**2",RooArgList(mass,BkgCorrCoef1,BkgCorrCoef2));
// //   DRecoProdPdf BkgPdf("BkgPdf","BkgPdf",BkgSlopePdf,BkgPdfRaw);
//   Canvas->Clear();
//   HDsMassRBkg->Draw();
//   Canvas->Print(filename);
 
  
//   RooRealVar SigYield(TString("SigYieldnXR"),TString("SigYieldnXR"),0,.4);
//   DRecoAddPdf TotalPdf(TString("TotalPdf_nXR"),TString("TotalPdf_nXR"),SigPdf,BkgPdf,SigYield);



//   TFile DataFile(_OutputDir+"/HistosForFit.root","read");
//   gROOT->cd();
//   TH1F* HDsMassData=(TH1F*)DataFile.Get("HDsMass");
//   RooDataHist DsMassDataSet(HDsMassData->GetName(),HDsMassData->GetName(),RooArgSet(mass),HDsMassData);
  
 
//   RooPlot*plots=mass.frame();

//   Canvas->Clear();
//   BkgPdf.plotOn(plots);
//   SigPdf.plotOn(plots);
//   plots->Draw();
//   Canvas->Print(filename);  
//   delete plots;

//   //TotalPdf.fitTo(DsMassDataSet);
//   DRecoHistPdfChi2Var chi2("chi2","chi2",TotalPdf,DsMassDataSet);
//   chi2.SetEType(RooAbsData::SumW2);
//   RooMinuit minuit(chi2);
//   minuit.migrad();

//   plots=mass.frame();
//   DsMassDataSet.plotOn(plots,RooFit::DataError(RooAbsData::SumW2));
//   TotalPdf.plotOn(plots);
//   TotalPdf.plotOn(plots,Components(RooArgSet(BkgPdf)),LineColor(2),FillColor(2),FillStyle(1001),DrawOption("LF"));
//   plots->SetTitle(TString("Yield=")+(long)(SigYield.getVal()*HDsMassData->Integral())+" +- "+(long)(SigYield.getError()*HDsMassData->Integral()));

//   //Make plot with Residuals 
//   TH1* HFit=TotalPdf.createHistogram("HFit",mass);
//   HFit->Scale(HDsMassData->Integral());
//   for(Int_t b=1;b<HFit->GetXaxis()->GetNbins();b++)
//     HFit->SetBinError(b,HFit->GetBinContent(b)
// 		      *sqrt(pow(SigYield.getVal()*HDsMassTrue->GetBinError(b)/HDsMassTrue->GetBinContent(b),2)
// 			    + pow((1.-SigYield.getVal())*HDsMassRBkg->GetBinError(b)/HDsMassRBkg->GetBinContent(b),2)));
//   DRecoResiduals residuals("resid",HDsMassData,HFit);
//   residuals.FitHasError(1);
//   Canvas->Clear();
//   residuals.MakeFitPlotWithResiduals(Canvas,plots);
//   Canvas->Print(filename);  
//   delete plots;


//   Canvas->Print(filename+"]");
//   BkgPdfsFile.Close(); 
//   DataFile.Close();
// }



// void DRecoDsInclusiveAnalysis::RooFitDsMassWSvsnXR(){
//   Canvas->Clear();
//   filename=_OutputDir+"/DsMassWSRooFitvsnXR.ps";
//   Canvas->Print(filename+"[");

//   mass.setRange(DsMassMin,DsMassMax);
//   mass.setBins(NDsMassBins);


//   ///Get signal pdf from signal ws sample
//   TFile PdfsFile("DsTolnu/SP1005Skim/DsReco/FinalWS/HistosForFit.root","read"); 
//   gROOT->cd();
//   TH1F* HDsMassWS[NXR];
//   RooDataHist* WSSigDataSet[NXR];
//   DRecoHistPdf* WSSigPdf[NXR];
//   Canvas->Clear();
//   Canvas->Divide(2,2);
//   for(Int_t i=0;i<NXR;i++){    
//     HDsMassWS[i]=(TH1F*)PdfsFile.Get(TString("HDsMassTrue_nXR")+(long)i);
//     if(!HDsMassWS[i]){cout<<" Not found"<<endl;return;}    
//     WSSigDataSet[i]=new RooDataHist(TString("WSSigDataSet")+(long)i,TString("WSSigDataSet")+(long)i,RooArgSet(mass),HDsMassWS[i]);
//     WSSigPdf[i]=new DRecoHistPdf(TString("WSSigPdf")+(long)i,TString("WSSigPdf")+(long)i,RooArgSet(mass),*WSSigDataSet[i],0);
//     Canvas->cd(i+1);
//     HDsMassWS[i]->Draw(); 
//   }  
//   Canvas->Print(filename); 
//   PdfsFile.Close();

  
//   //Get the Data
//   TFile DataFile(_OutputDir+"/HistosForFit.root","read"); 
//   gROOT->cd();
//   TH1F* HDsMassData[NXR];
//   RooDataHist* DsMassDataSet[NXR];
//   for(Int_t i=0;i<NXR;i++){
//     HDsMassData[i]=(TH1F*)DataFile.Get(TString("HDsMass_nXR")+(long)i);
//     if(!HDsMassData[i]){cout<<" Not found"<<endl;return;} 
//     DsMassDataSet[i]=new RooDataHist(HDsMassData[i]->GetName(),HDsMassData[i]->GetName(),RooArgSet(mass),HDsMassData[i]);     
//   }

//   //Create a pdf out of the subtraction of the wrong-sign signalMC from total wrong-sign.
//   RooRealVar* WSSigYield[NXR];
//   DRecoHistPdf* TotWSPdf[NXR];
//   DRecoAddPdf* WSBkgPdf[NXR];
//   RooRealVar* SigYield[NXR];
//   DRecoAddPdf* TotalPdf[NXR];
//   for(Int_t i=0;i<NXR;i++){
//     WSSigYield[i]=new  RooRealVar(TString("WSSigYield")+(long)i,TString("WSSigYield")+(long)i,-.2,-.00001);
//     TotWSPdf[i]=new DRecoHistPdf(TString("HTotWspdf")+(long)i,TString("HTotWSpdf")+(long)i,RooArgSet(mass),*DsMassDataSet[i],0);
//     WSBkgPdf[i]=new DRecoAddPdf(TString("WSBkgPdf")+(long)i,TString("WSBkgPdf")+(long)i,*WSSigPdf[i],*TotWSPdf[i],*WSSigYield[i]);
//     SigYield[i]=new RooRealVar(TString("SigYield")+(long)i,TString("SigYield")+(long)i,.001,.2);
//     TotalPdf[i]=new DRecoAddPdf(TString("TotalPdf")+(long)i,TString("TotalPdf")+(long)i,*WSSigPdf[i],*WSBkgPdf[i],*SigYield[i]);
//   }


//   RooPlot*plots[NXR];
  
//   Canvas->Clear();
//   Canvas->Divide(2,2);
//   for(Int_t i=0;i<NXR;i++){
//     plots[i]=mass.frame();
//     WSBkgPdf[i]->plotOn(plots[i],LineColor(2));
//     WSSigPdf[i]->plotOn(plots[i]);
//     Canvas->cd(i+1);
//     plots[i]->Draw();
//   }
//   Canvas->Print(filename);  

//   for(Int_t i=0;i<NXR;i++){
//     delete plots[i];
//   }

//   for(Int_t i=0;i<NXR;i++){
//     //TotalPdf[i]->fitTo(*DsMassDataSet[i]);
//     DRecoHistPdfChi2Var chi2(TString("chi2")+(long)i,TString("chi2")+(long)i,*TotalPdf[i],*DsMassDataSet[i]);
//     chi2.SetEType(RooAbsData::SumW2);
//     RooMinuit minuit(chi2);
//     minuit.migrad();    

//     plots[i]=mass.frame();
//     DsMassDataSet[i]->plotOn(plots[i]);
//     TotalPdf[i]->plotOn(plots[i]);
//     WSBkgPdf[i]->plotOn(plots[i],LineColor(2),FillColor(2),FillStyle(1001),DrawOption("LF"),Normalization(1.-SigYield[i]->getVal()));
//     plots[i]->SetTitle(TString("Signal Yield=")+(long)(SigYield[i]->getVal()*HDsMassData[i]->Integral())+" +- "+(long)(SigYield[i]->getError()*HDsMassData[i]->Integral()));
//   }

//   //Make Residuals  
//   Canvas->Clear();
//   Canvas->Divide(2,2);
//   for(Int_t i=0;i<NXR;i++){       
//     Canvas->cd(i+1); 
//     plots[i]->Draw();

//   }
//   Canvas->Print(filename);   
  
 
//   Canvas->Print(filename+"]");


//   //Save the Wrong-Sign shape
//   TFile ResultsFile(_OutputDir+"/DsMassWSRooFitvsnXR.root","recreate"); 
//   for(Int_t i=0;i<NXR;i++){
//     TH1* HWSshape= WSBkgPdf[i]->createHistogram("HWSshape",mass);
//     HWSshape->SetName(TString("HDsMass_nXR")+(long)i);    
//     HWSshape->Write();
//     plots[i]->SetName(TString("HWSplot_nXR")+(long)i);
//     plots[i]->Write();
//   }  
//   ResultsFile.ls();
//   ResultsFile.Close();

//   DataFile.Close(); 


//   //clean up
//   for(Int_t i=0;i<NXR;i++){
//     delete plots[i];
//   }


// }



// void DRecoDsInclusiveAnalysis::ConstructWSBkgnXR0(){
  
//   //Use the wrong sign from nXR=1,2,3 to extrapolate into nXR=0

//   //the resulting nXR=0 bkg will overwrite the empty existing histo in the file
//   TFile ResultsFile(_OutputDir+"/DsMassWSRooFitvsnXR.root","update"); 
//   ResultsFile.ls();
//   gROOT->cd();
  
//   TH1F* HWSshape[NXR];
//   for(Int_t i=0;i<NXR;i++){
//     HWSshape[i]=(TH1F*)ResultsFile.Get(TString("HDsMass_nXR")+(long)i);   
//     if(!HWSshape[i]){cout<<"not found histo "<<i<<endl; return;}
//   }  
  
//   //clone the histos 

//   filename=_OutputDir+"/ConstructWSBkgnXR0.ps";
//   Canvas->Print(filename+"[");

//   //Compare the backgrounds to see how they change as a function of nXR
//   TH1F* HWSshapeC[NXR];
//   Canvas->Clear();
//   for(Int_t i=1;i<NXR;i++){
//     HWSshapeC[i]=(TH1F*)HWSshape[i]->Clone();
//     HWSshapeC[i]->Scale(1./HWSshapeC[i]->Integral());
//     HWSshapeC[i]->SetLineColor(i);
//     if(i==1){
//       HWSshapeC[i]->SetStats(0);
//       HWSshapeC[i]->Draw();
//     }else HWSshapeC[i]->Draw("same");
//   } 
//   Canvas->Print(filename);
//   for(Int_t i=1;i<NXR;i++) delete HWSshapeC[i];  

//   ///The backgrounds do not change much across nXR so for nXR=0 we will just use an average of nXR=1,2,3 
//   HWSshapeC[0]=(TH1F*)HWSshape[0]->Clone();
//   for(Int_t i=1;i<NXR;i++)
//     HWSshapeC[0]->Add(HWSshape[i]);


//   Canvas->Print(filename+"]");

//   ResultsFile.cd();
//   ResultsFile.Delete("HDsMass_nXR0;1");
//   HWSshapeC[0]->SetName("HDsMass_nXR0");
//   HWSshapeC[0]->Write();  
//   ResultsFile.ls();
//   ResultsFile.Close();

// }



// void DRecoDsInclusiveAnalysis::RooFitDsMassvsnXR(){

//   filename=_OutputDir+"/DsMassRooFitvsnXR.ps";
//   Canvas->Clear();
//   Canvas->Print(filename+"["); 
  
//   mass.setRange(DsMassMin,DsMassMax);
//   mass.setBins(NDsMassBins);

//   ///Get the signal Pdfs from the signal MC 
//   TFile PdfsFile("DsTolnu/SP1005Skim/DsReco/Final/HistosForFit.root","read"); 
//   gROOT->cd();
//   TH1F* HDsMassR[NXR];
//   RooDataHist* Datahist[NXR];
//   DRecoHistPdf* SigPdf[NXR];
//   Canvas->Clear();
//   Canvas->Divide(2,2);
//   for(Int_t i=0;i<NXR;i++){
//     Canvas->cd(i+1); 
//     HDsMassR[i]=(TH1F*)PdfsFile.Get(TString("HDsMassTrue_nXR")+(long)i);
//     if(!HDsMassR[i]){cout<<" Not found"<<endl;return;}    
//     Datahist[i]=new RooDataHist(TString("Hdata")+(long)i,TString("Hdata")+(long)i,RooArgSet(mass),HDsMassR[i]);
//     SigPdf[i]=new DRecoHistPdf(TString("Hpdf")+(long)i,TString("Hpdf")+(long)i,RooArgSet(mass),*Datahist[i],0);    
//     HDsMassR[i]->Draw();
//   }
//   Canvas->Print(filename); 
//   PdfsFile.Close(); 
  

//   //Get the bkg pdfs
//   TFile BkgPdfsFile("DsTolnu/SP1005Skim/DsReco/FinalWS/DsMassWSRooFitvsnXR.root","read");
//   gROOT->cd();  
//   TH1F* HDsMassRBkg[NXR];
//   RooDataHist* DatahistBkg[NXR];
//   DRecoHistPdf* BkgPdf[NXR];
//   RooRealVar* SigYield[NXR];
//   RooAddPdf* TotalPdf[NXR];
//   Canvas->Clear();
//   Canvas->Divide(2,2);
//   for(Int_t i=0;i<NXR;i++){
//     Canvas->cd(i+1);
//     HDsMassRBkg[i]=(TH1F*)BkgPdfsFile.Get(TString("HDsMass_nXR")+(long)0);
//     if(!HDsMassRBkg[i]){cout<<" Not found"<<endl;return;}    
//     DatahistBkg[i]=new RooDataHist(TString("HdataBkg")+(long)i,TString("HdataBkg")+(long)i,RooArgSet(mass),HDsMassRBkg[i]);
//     BkgPdf[i]=new DRecoHistPdf(TString("HpdfBkg")+(long)i,TString("HpdfBkg")+(long)i,RooArgSet(mass),*DatahistBkg[i],0);
//     HDsMassRBkg[i]->Draw();
//     SigYield[i]=new RooRealVar(TString("SigYieldnXR")+(long)i,TString("SigYieldnXR")+(long)i,0,1);
//     TotalPdf[i]=new RooAddPdf(TString("TotalPdf_nXR")+(long)i,TString("TotalPdf_nXR")+(long)i,*SigPdf[i],*BkgPdf[i],*SigYield[i]);
//   }
//   Canvas->Print(filename);
//   BkgPdfsFile.Close(); 
  
//   //show the signal and bkg pdfs
//   RooPlot*plots[NXR];
//   Canvas->Clear();  
//   Canvas->Divide(2,2);
//   for(Int_t i=0;i<NXR;i++){
//     Canvas->cd(i+1);
//     plots[i]=mass.frame();
//     BkgPdf[i]->plotOn(plots[i]);
//     SigPdf[i]->plotOn(plots[i]);
//     plots[i]->Draw();
//   }
//   Canvas->Print(filename);  
//   for(Int_t i=0;i<NXR;i++)delete plots[i];

//   //Get the Data histograms
//   TFile DataFile(_OutputDir+"/HistosForFit.root","read");
//   gROOT->cd();
//   TH1F* HDsMassData[NXR];
//   RooDataHist* DsMassDataSet[NXR];
//   Canvas->Clear();
//   Canvas->Divide(2,2);
//   for(Int_t i=0;i<NXR;i++){
//     Canvas->cd(i+1);
//     HDsMassData[i]=(TH1F*)DataFile.Get(TString("HDsMass_nXR")+(long)i);
//     DsMassDataSet[i]=new RooDataHist(HDsMassData[i]->GetName(),HDsMassData[i]->GetName(),RooArgSet(mass),HDsMassData[i]);
//     HDsMassData[i]->Draw();
//   }
//   Canvas->Print(filename);

//   //fit the Data 
//   for(Int_t i=0;i<NXR;i++){
//     TotalPdf[i]->fitTo(*DsMassDataSet[i]);
//     plots[i]=mass.frame();
//     DsMassDataSet[i]->plotOn(plots[i]);
//     TotalPdf[i]->plotOn(plots[i]);
//     TotalPdf[i]->plotOn(plots[i],Components(RooArgSet(*BkgPdf[i])),LineColor(2),FillColor(2),FillStyle(1001),DrawOption("LF"));
//     plots[i]->SetTitle(TString("Yield=")+(long)(SigYield[i]->getVal()*HDsMassData[i]->Integral())+" +- "+(long)(SigYield[i]->getError()*HDsMassData[i]->Integral()));
//   }
 

//   //Make plot with Residuals 
//   TH1F* HFitNorRes[NXR];
//   TH1* HTotalSig=SigPdf[0]->createHistogram(TString("HTotalSig"),mass);
//   HTotalSig->Scale(SigYield[0]->getVal()*HDsMassData[0]->Integral());
//   TH1* HTotalBkg=BkgPdf[0]->createHistogram(TString("HTotalBkg"),mass);
//   HTotalBkg->Scale((1-SigYield[0]->getVal())*HDsMassData[0]->Integral());
//   TH1F* HDsMassDataTotal=(TH1F*)HDsMassData[0]->Clone();
//   Canvas->Clear();
//   Canvas->Divide(2,2);
//   for(Int_t i=0;i<NXR;i++){
//     Canvas->cd(i+1);     
//     //add the individual pdfs and data
//     if(i>0){
//       TH1* HFitSig=SigPdf[i]->createHistogram(TString("HFitSig")+(long)i,mass);
//       HFitSig->Scale(SigYield[i]->getVal()*HDsMassData[i]->Integral());
//       HTotalSig->Add(HFitSig);
//       TH1* HFitBkg=BkgPdf[i]->createHistogram(TString("HFitBkg")+(long)i,mass);
//       HFitBkg->Scale((1-SigYield[i]->getVal())*HDsMassData[i]->Integral());
//       HTotalBkg->Add(HFitBkg);
//       HDsMassDataTotal->Add(HDsMassData[i]);
//     }
    
//     TH1* HFit=TotalPdf[i]->createHistogram(TString("HFit")+(long)i,mass);
//     HFit->Scale(HDsMassData[i]->Integral());
//     DRecoResiduals residuals(TString("resid")+(long)i,HDsMassData[i],HFit);   
//     HFitNorRes[i]=residuals.GetNormResidualHisto();
//     //residuals.MakeFitPlotWithResiduals((TCanvas*)pad,plots[i]);    
//     plots[i]->Draw();
//     delete HFit;
//   }
//   Canvas->Print(filename);  

//   cutline.SetLineColor(2);
//   Canvas->Clear();
//   Canvas->Divide(2,2);
//   for(Int_t i=0;i<NXR;i++){
//     Canvas->cd(i+1);
//     HFitNorRes[i]->Draw("p");
//     cutline.DrawLine(HFitNorRes[i]->GetXaxis()->GetXmin(),2,HFitNorRes[i]->GetXaxis()->GetXmax(),2);
//     cutline.DrawLine(HFitNorRes[i]->GetXaxis()->GetXmin(),-2,HFitNorRes[i]->GetXaxis()->GetXmax(),-2);
//   }
//   Canvas->Print(filename);  


//   ///Add the individual pdfs and plot total fit:   
//   RooDataHist TotalHistSig("TotalHistSig","TotalHistSig",RooArgSet(mass),HTotalSig);
//   DRecoHistPdf TotalPdfSig("TotalPdfSig","TotalPdfSig",RooArgSet(mass),TotalHistSig,0);
//   RooDataHist TotalHistBkg("TotalHistBkg","TotalHistBkg",RooArgSet(mass),HTotalBkg);
//   DRecoHistPdf TotalPdfBkg("TotalPdfBkg","TotalPdfBkg",RooArgSet(mass),TotalHistBkg,0);  
//   RooDataHist TotalData("TotalData","TotalData",RooArgSet(mass),HDsMassDataTotal);
//   Float_t totalfracyield=0;
//   for(Int_t i=0;i<NXR;i++)
//     totalfracyield+=SigYield[i]->getVal()*HDsMassData[i]->Integral()/HDsMassDataTotal->Integral();
//   RooRealVar TotalSigYield("TotalSigYield","TotalSigYield",totalfracyield);
//   RooAddPdf TotalPdfTotal("TotalPdfTotal","TotalPdfTotal",TotalPdfSig,TotalPdfBkg,TotalSigYield);
//   RooPlot* totalplot=mass.frame(); 
//   TotalData.plotOn(totalplot);
//   TotalPdfTotal.plotOn(totalplot);
//   TotalPdfTotal.plotOn(totalplot,Components(RooArgSet(TotalPdfBkg)),LineColor(2),FillColor(2),FillStyle(1001),DrawOption("LF"));
//   Canvas->Clear();
//   totalplot->Draw();
//   Canvas->Print(filename);  
//   delete totalplot;

//   //print out the residuals
//   Float_t totalyield=0;
//   Float_t totalerror=0;
//   for(Int_t i=0;i<NXR;i++){
//     cout<<"nXR="<<i<<"   "<<plots[i]->GetTitle()<<endl;
//     totalyield+=SigYield[i]->getVal()*HDsMassData[i]->Integral();
//     totalerror+=(SigYield[i]->getError()*HDsMassData[i]->Integral())*(SigYield[i]->getError()*HDsMassData[i]->Integral());
//   }
//   cout<<"Total Yield = "<<totalyield<<" +-"<<sqrt(totalerror)<<endl;
  


//   for(Int_t i=0;i<NXR;i++){
//     delete plots[i];
//     delete HFitNorRes[i];
//   }

//   Canvas->Print(filename+"]");

//   DataFile.Close();
// }




// void DRecoDsInclusiveAnalysis::RooFitDsMassWSvsnXRvsnXT(){
//   Canvas->Clear();
//   filename=_OutputDir+"/DsMassWSRooFitvsnXRvsnXT.ps";
//   Canvas->Print(filename+"[");

//   mass.setRange(DsMassMin,DsMassMax);
//   mass.setBins(NDsMassBins);

//   RooRealVar nxr("nxr","n_{X}^{R}",-.5,NXR-.5);
//   nxr.setBins(NXR);


//   ///Get signal pdfs from signal ws sample
//   TFile PdfsFile("DsTolnu/SP1005Skim/DsReco/FinalWS/HistosForFit.root","read"); 
//   PdfsFile.ls();
//   gROOT->cd();
//   TH2F* HDsMassWS[NXT];
//   RooDataHist* DsMassDataSetWSSig[NXT];
//   DRecoHistPdf* WSSigPdf[NXT];
//   RooArgList pdflist;
//   Float_t truew[NXT];Float_t totalw=0;
//   Canvas->Clear();
//   Canvas->Divide(2,3);
//   for(Int_t j=0;j<NXT;j++){    
//     HDsMassWS[j]=(TH2F*)PdfsFile.Get(TString("HDsMassTrue2D_nXT")+(long)j);
//     if(!HDsMassWS[j]){cout<<" Not found"<<endl;return;}    

//     DsMassDataSetWSSig[j]=new RooDataHist(TString("WSSigDat")+(long)j,TString("WSSigDat")+(long)j,
// 					  nset,HDsMassWS[j]);  
    
//     WSSigPdf[j]=new DRecoHistPdf(TString("WSSigPdf")+(long)j,TString("WSSigPdf")+(long)j,
// 				 nset,*DsMassDataSetWSSig[j],0);
//     pdflist.add(*WSSigPdf[j]);

    
//     Canvas->cd(j+1);
//     HDsMassWS[j]->SetTitle(TString("n_{X}^{T}=")+(long)j);
//     HDsMassWS[j]->Draw("lego"); 
    
//     truew[j]=HDsMassWS[j]->Integral();
//     totalw+=HDsMassWS[j]->Integral();
//   }    
//   Canvas->Print(filename); 
//   PdfsFile.Close();
//   //calculate initial weights
//   for(Int_t j=0;j<NXT;j++) truew[j]=truew[j]/totalw;


//   ///create the weighting coeficients for each nXT  
//   RooRealVar* nXTCoef[NXT-1];
//   RooArgList coeflist;
//   for(Int_t j=0;j<NXT-1;j++){
//     nXTCoef[j]=new RooRealVar(TString("nXTCoef")+(long)j,TString("nXTCoef")+(long)j,.001,.4);
//     nXTCoef[j]->setVal(truew[j]);
//     //nXTCoef[j]->setConstant(1);
//     coeflist.add(*nXTCoef[j]);
//   } 
//   //construct the total signal pdf out of the addition of each nXT pdf
//   DRecoAddPdf WSTotalSigPdf("WSTotalSigPdf","WSTotalSigPdf",pdflist,coeflist);
  
//   //Get the Data
//   TFile DataFile(_OutputDir+"/HistosForFit.root","read"); 
//   DataFile.ls();
//   gROOT->cd();
//   TH2F* HDsMassData=(TH2F*)DataFile.Get(TString("HDsMass2D"));
//   if(!HDsMassData){cout<<" Not found"<<endl;return;} 
//   RooDataHist DsMassDataSet(HDsMassData->GetName(),HDsMassData->GetName(),nset,HDsMassData);     
//   Canvas->Clear();
//   HDsMassData->SetTitle("Data");
//   HDsMassData->Draw("lego");
//   Canvas->Print(filename); 

//   //show the initial signal pdf
//   TH2F HSignalPdf("HSignalPdf","Signal Fit",
// 		  HDsMassData->GetXaxis()->GetNbins(),HDsMassData->GetXaxis()->GetXmin(),HDsMassData->GetXaxis()->GetXmax(),
// 		  HDsMassData->GetYaxis()->GetNbins(),HDsMassData->GetYaxis()->GetXmin(),HDsMassData->GetYaxis()->GetXmax());  
//   WSTotalSigPdf.fillHistogram(&HSignalPdf,nset,1,0);
//   Canvas->Clear();
//   HSignalPdf.Draw("lego");
//   Canvas->Print(filename);   



//   //Create a pdf out of the subtraction of the wrong-sign signalMC from total wrong-sign.
//   DRecoHistPdf TotWSPdf(TString("HTotWspdf"),TString("HTotWSpdf"),RooArgSet(nxr,mass),DsMassDataSet,0);
//   RooRealVar WSSigYield("WSSigYield","WSSigYield",.01,.2);
//   //RooFormulaVar SigYield("SigYield","SigYield","-WSSigYield",RooArgSet(WSSigYield));
//   RooRealVar SigYield("SigYield","SigYield",-.2,-.01);
//   DRecoAddPdf WSBkgPdf(TString("WSBkgPdf"),TString("WSBkgPdf"),WSTotalSigPdf,TotWSPdf,SigYield);
//   DRecoAddPdf TotalPdf(TString("TotalPdf"),TString("TotalPdf"),WSTotalSigPdf,WSBkgPdf,WSSigYield);
  
  
//   //create chisquare
//   DRecoHistPdfChi2Var chi2("chi2","chi2",TotalPdf,DsMassDataSet);//
//   chi2.SetEType(RooAbsData::SumW2);
 
//   //make a graph of the scan of the chisquare vs the signal yield
//   TGraph GChiVsY;
//   for(Int_t c=0;c<50;c++){
//     WSSigYield.setVal(WSSigYield.getMin()+c*(WSSigYield.getMax()-WSSigYield.getMin())/50.);
//     GChiVsY.SetPoint(c,WSSigYield.getVal(),chi2.getVal());
//   }
//   Canvas->Clear();
//   GChiVsY.Draw("ape");
//   Canvas->Print(filename);


//   //minimize
//   RooMinuit minuit(chi2);
//   minuit.migrad();
  
//   ///plot on 1D mass frame
//   RooPlot* plot=mass.frame();
//   DsMassDataSet.plotOn(plot,RooFit::DataError(RooAbsData::SumW2));//
//   TotalPdf.plotOn(plot);
//   //TotalPdf.plotOn(plot,Components(RooArgSet(WSBkgPdf)),LineColor(2),FillColor(2),FillStyle(1001),DrawOption("LF"));
//   WSBkgPdf.plotOn(plot,Normalization((1-WSSigYield.getVal())),LineColor(2),FillColor(2),FillStyle(1001),DrawOption("LF"));
//   Canvas->Clear();
//   plot->SetTitle(TString("Signal Yield=")+(long)(WSSigYield.getVal()*HDsMassData->Integral())
// 		 +" +- "+(long)(WSSigYield.getError()*HDsMassData->Integral()));
//   plot->Draw();
//   Canvas->Print(filename); 
//   delete plot;

//   //create histogram of total fit
//   TH2F HTotalFit("HTotalFit","Total Fit",
// 		      HDsMassData->GetXaxis()->GetNbins(),HDsMassData->GetXaxis()->GetXmin(),HDsMassData->GetXaxis()->GetXmax(),
// 		      HDsMassData->GetYaxis()->GetNbins(),HDsMassData->GetYaxis()->GetXmin(),HDsMassData->GetYaxis()->GetXmax());  
//   TotalPdf.fillHistogram(&HTotalFit,nset,HDsMassData->Integral(),0);
//   Canvas->Clear();
//   HTotalFit.Draw("lego");
//   Canvas->Print(filename); 

//   //create histogram of signal fit
//   TH2F HSignalFit("HSignalFit","Signal Fit",
// 		      HDsMassData->GetXaxis()->GetNbins(),HDsMassData->GetXaxis()->GetXmin(),HDsMassData->GetXaxis()->GetXmax(),
// 		      HDsMassData->GetYaxis()->GetNbins(),HDsMassData->GetYaxis()->GetXmin(),HDsMassData->GetYaxis()->GetXmax());  
//   WSTotalSigPdf.fillHistogram(&HSignalFit,nset,WSSigYield.getVal()*HDsMassData->Integral(),0);
//   Canvas->Clear();
//   HSignalFit.SetTitle(TString("Signal Yield=")+(long)(WSSigYield.getVal()*HDsMassData->Integral())
// 		       +" +- "+(long)(WSSigYield.getError()*HDsMassData->Integral()));
//   HSignalFit.Draw("lego");
//   Canvas->Print(filename); 

//   //create histogram of background  fit
//   TH2F HBkgFit("HBkgFit","Background Fit",
// 	       HDsMassData->GetXaxis()->GetNbins(),HDsMassData->GetXaxis()->GetXmin(),HDsMassData->GetXaxis()->GetXmax(),
// 	       HDsMassData->GetYaxis()->GetNbins(),HDsMassData->GetYaxis()->GetXmin(),HDsMassData->GetYaxis()->GetXmax());  
//   WSBkgPdf.fillHistogram(&HBkgFit,nset,(1-WSSigYield.getVal())*HDsMassData->Integral(),0);
//   Canvas->Clear();
//   HBkgFit.Draw("lego");
//   Canvas->Print(filename); 

//   //Create 1D plot: Project the Data, total fit and total background:
//   TH1F* HData1DTemp=(TH1F*)HDsMassData->ProjectionY();
//   TH1F HData1D(TString(HDsMassData->GetName())+"mass",HSignalFit.GetTitle(),NDsMassBins,DsMassMin,DsMassMax);
//   for(Int_t b=1;b<=HDsMassData->GetYaxis()->GetNbins();b++){
//     HData1D.SetBinContent(b,HData1DTemp->GetBinContent(b));
//     HData1D.SetBinError(b,HData1DTemp->GetBinError(b));
//     //cout<<"sqrt(bincont)="<<sqrt(HData1DTemp->GetBinContent(b))<<"  error="<<HData1DTemp->GetBinError(b)<<endl;
//   } 
//   delete HData1DTemp;
//   TH1F* HFit1D=(TH1F*)HTotalFit.ProjectionY();
//   TH1F* HBkg1D=(TH1F*)HBkgFit.ProjectionY();
//   Canvas->Clear();
//   HData1D.Draw("pe");
//   HFit1D->SetLineColor(4);
//   HFit1D->Draw("same");
//   HBkg1D->SetFillColor(2);HBkg1D->SetLineColor(0);
//   HBkg1D->Draw("same");
//   Canvas->Print(filename); 
  
//   //create 1D plots for every nXR
//   TH1F* HData1DnXR[NXR];
//   TH1F* HFit1DnXR[NXR];
//   TH1F* HBkg1DnXR[NXR];
//   for(Int_t i=0;i<NXR;i++){ 
//     HData1DnXR[i]=new TH1F(TString(HDsMassData->GetName())+(long)i,TString("nXR=")+(long)i,NDsMassBins,DsMassMin,DsMassMax);
//     HFit1DnXR[i]=new TH1F(TString(HTotalFit.GetName())+(long)i,TString("nXR=")+(long)i,NDsMassBins,DsMassMin,DsMassMax);
//     HBkg1DnXR[i]=new TH1F(TString(HBkgFit.GetName())+(long)i,TString("nXR=")+(long)i,NDsMassBins,DsMassMin,DsMassMax);
//     for(Int_t b=1;b<=HDsMassData->GetYaxis()->GetNbins();b++){
//       HData1DnXR[i]->SetBinContent(b,HDsMassData->GetBinContent(i+1,b));
//       HData1DnXR[i]->SetBinError(b,HDsMassData->GetBinError(i+1,b));
//       HFit1DnXR[i]->SetBinContent(b,HTotalFit.GetBinContent(i+1,b));
//       //HFit1DnXR[i]->SetBinError(b,HTotalFit.GetBinError(i+1,b));
//       HBkg1DnXR[i]->SetBinContent(b,HBkgFit.GetBinContent(i+1,b));
//       //HBkg1DnXR[i]->SetBinError(b,HBkgFit.GetBinError(i+1,b));
//     }
//   }
//   Canvas->Clear();
//   Canvas->Divide(2,2);
//   for(Int_t i=0;i<NXR;i++){ 
//     Canvas->cd(i+1);
//     HData1DnXR[i]->SetTitle(TString("Signal Yield=")+(long)(HFit1DnXR[i]->Integral())+" +- ");
//     HData1DnXR[i]->SetMarkerSize(1); HData1DnXR[i]->SetMarkerStyle(1);
//     HData1DnXR[i]->Draw("pe");
//     HFit1DnXR[i]->SetLineColor(4);
//     HFit1DnXR[i]->Draw("same");
//     HBkg1DnXR[i]->SetFillColor(2);HBkg1DnXR[i]->SetLineColor(0);
//     HBkg1DnXR[i]->Draw("same");
//   }  
//   Canvas->Print(filename);


//   //create a difference between the total fit and the data---> should be 0 by definition
//   TH2F* HFitDiff=(TH2F*)HDsMassData->Clone();
//   HFitDiff->Add((TH1*)&HTotalFit,-1.);
//   HFitDiff->Divide(HDsMassData);
//   Canvas->Clear();
//   HFitDiff->SetTitle("(Data - Fit)/Data");
//   HFitDiff->Draw("colz");
//   Canvas->Print(filename); 
   
//   Canvas->Print(filename+"]");


//   //print out the nXT weight coefficients:
//   for(Int_t j=0;j<NXT-1;j++){ 
//     cout<<"nXT="<<j<<"  fit_w="<<nXTCoef[j]->getVal()<<"  true_w="<<truew[j]<<endl;    
//   }


//   //Save the Wrong-Sign shape
//   TFile ResultsFile(_OutputDir+"/DsMassWSRooFitvsnXRvsnXT.root","recreate"); 
//   for(Int_t i=0;i<NXR;i++){ 
//     TH1*h=HBkgFit.ProjectionY(TString("nXR")+(long)i,i+1,i+1,"");
//     h->SetName(TString("HDsMass_nXR")+(long)i);
//     h->Write();
//   }  
//   ResultsFile.ls();
//   ResultsFile.Close();

//   DataFile.Close(); 



// }



// void DRecoDsInclusiveAnalysis::ConstructWSBkgnXR0vsnXT(){
  
//   //Use the wrong sign from nXR=1,2,3 to extrapolate into nXR=0

//   //the resulting nXR=0 bkg will overwrite the empty existing histo in the file
//   TFile ResultsFile(_OutputDir+"/DsMassWSRooFitvsnXRvsnXT.root","update"); 
//   ResultsFile.ls();
//   gROOT->cd();
  
//   TH1F* HWSshape[NXR];
//   for(Int_t i=0;i<NXR;i++){
//     HWSshape[i]=(TH1F*)ResultsFile.Get(TString("HDsMass_nXR")+(long)i);   
//     if(!HWSshape[i]){cout<<"not found histo "<<i<<endl; return;}
//   }  
  
//   //clone the histos 

//   filename=_OutputDir+"/ConstructWSBkgnXR0vsnXT.ps";
//   Canvas->Print(filename+"[");

//   //Compare the backgrounds to see how they change as a function of nXR
//   TH1F* HWSshapeC[NXR];
//   Canvas->Clear();
//   for(Int_t i=1;i<NXR;i++){
//     HWSshapeC[i]=(TH1F*)HWSshape[i]->Clone();
//     HWSshapeC[i]->Scale(1./HWSshapeC[i]->Integral());
//     HWSshapeC[i]->SetLineColor(i);
//     if(i==1){
//       HWSshapeC[i]->SetStats(0);
//       HWSshapeC[i]->Draw();
//     }else HWSshapeC[i]->Draw("same");
//   } 
//   Canvas->Print(filename);
//   for(Int_t i=1;i<NXR;i++) delete HWSshapeC[i];  

//   ///The backgrounds do not change much across nXR so for nXR=0 we will just use an average of nXR=1,2,3 
//   HWSshapeC[0]=(TH1F*)HWSshape[0]->Clone();
//   for(Int_t i=1;i<NXR;i++)
//     HWSshapeC[0]->Add(HWSshape[i]);


//   Canvas->Print(filename+"]");

//   ResultsFile.cd();
//   ResultsFile.Delete("HDsMass_nXR0;1");
//   HWSshapeC[0]->SetName("HDsMass_nXR0");
//   HWSshapeC[0]->Write();  
//   ResultsFile.ls();
//   ResultsFile.Close();

// }


// void DRecoDsInclusiveAnalysis::RooFitDsMassvsnXRvsnXT(){

//   filename=_OutputDir+"/DsMassRooFitvsnXRvsnXT.ps";
//   Canvas->Clear();
//   Canvas->Print(filename+"["); 
  
//   mass.setRange(DsMassMin,DsMassMax);
//   mass.setBins(NDsMassBins);

//   RooRealVar nxr("nxr","n_{X}^{R}",-.5,NXR-.5);
//   nxr.setBins(NXR);
  

//   ///Get the signal Pdfs from the signal MC 
//   TFile PdfsFile("DsTolnu/SP1005Skim/DsReco/Final/HistosForFit.root","read"); 
//   gROOT->cd();
//   TH2F* HDsMassSig[NXT];
//   RooDataHist* DsMassDataSetWSSig[NXT];
//   DRecoHistPdf* WSSigPdf[NXT];
//   RooArgList pdflist;
//   Float_t truew[NXT]; Float_t totalw=0;
//   Canvas->Clear();
//   Canvas->Divide(2,3);
//   for(Int_t j=0;j<NXT;j++){    
//     HDsMassSig[j]=(TH2F*)PdfsFile.Get(TString("HDsMassTrue2D_nXT")+(long)j);
//     if(!HDsMassSig[j]){cout<<" Not found"<<endl;return;}    

//     DsMassDataSetWSSig[j]=new RooDataHist(TString("WSSigDat")+(long)j,TString("WSSigDat")+(long)j,
// 					  nset,HDsMassSig[j]);  
    
//     WSSigPdf[j]=new DRecoHistPdf(TString("WSSigPdf")+(long)j,TString("WSSigPdf")+(long)j,
// 				 nset,*DsMassDataSetWSSig[j],0);
//     pdflist.add(*WSSigPdf[j]);
    
//     Canvas->cd(j+1);
//     HDsMassSig[j]->SetTitle(TString("n_{X}^{T}=")+(long)j);
//     HDsMassSig[j]->Draw("lego"); 
    
//     truew[j]=HDsMassSig[j]->Integral();
//     totalw+=HDsMassSig[j]->Integral();
//   }    
//   Canvas->Print(filename); 
//   PdfsFile.Close(); 
  
//   ///create the weighting coeficients for each nXT  
//   RooRealVar* nXTCoef[NXT-1];
//   RooArgList coeflist;
//   for(Int_t j=0;j<NXT-1;j++){
//     nXTCoef[j]=new RooRealVar(TString("nXTCoef")+(long)j,TString("nXTCoef")+(long)j,.001,.4);
//     //calculate initial weights
//     truew[j]=truew[j]/totalw;
//     nXTCoef[j]->setVal(truew[j]);
//     //nXTCoef[j]->setConstant(1);
//     coeflist.add(*nXTCoef[j]);
//   } 
//   //construct the total signal pdf out of the addition of each nXT pdf
//   DRecoAddPdf SigTotalSigPdf("SigTotalSigPdf","SigTotalSigPdf",pdflist,coeflist);
  


//   //Get the bkg pdfs
//   TFile BkgPdfsFile("DsTolnu/SP1005Skim/DsReco/FinalWS/DsMassWSRooFitvsnXRvsnXT.root","read");
//   gROOT->cd();  
//   TH1F* HDsMassRBkg[NXR];
//   TH2F* HDsMassRBkg2D[NXR];
//   RooDataHist* DatahistBkg[NXR];
//   DRecoHistPdf* BkgPdf[NXR];
//   RooRealVar* BkgYieldnXR[NXR];
//   RooArgList bkgpdflist;
//   RooArgList bkgcoeflist;
//   Canvas->Clear();
//   Canvas->Divide(2,2);
//   for(Int_t i=0;i<NXR;i++){
//     Canvas->cd(i+1);
//     HDsMassRBkg[i]=(TH1F*)BkgPdfsFile.Get(TString("HDsMass_nXR")+(long)i);    
//     if(!HDsMassRBkg[i]){cout<<" Not found"<<endl;return;}    
//     HDsMassRBkg2D[i]=new TH2F(TString("HDsMass2D_nXT")+(long)i,"",NXR,-.5,NXR-.5,NDsMassBins,DsMassMin,DsMassMax);
//     for(Int_t r=1;r<=NDsMassBins;r++){
//       HDsMassRBkg2D[i]->SetBinContent(i+1,r,HDsMassRBkg[i]->GetBinContent(r));
//       HDsMassRBkg2D[i]->SetBinError(i+1,r,HDsMassRBkg[i]->GetBinError(r));
//     }
//     DatahistBkg[i]=new RooDataHist(TString("HdataBkg")+(long)i,TString("HdataBkg")+(long)i,nset,HDsMassRBkg2D[i]);
//     BkgPdf[i]=new DRecoHistPdf(TString("HpdfBkg")+(long)i,TString("HpdfBkg")+(long)i,nset,*DatahistBkg[i],0);
//     HDsMassRBkg[i]->Draw();
//     BkgYieldnXR[i]=new RooRealVar(TString("BkgYieldnXR")+(long)i,TString("BkgYieldnXR")+(long)i,0,1);
//     bkgpdflist.add(*BkgPdf[i]);
//     if(i<NXR-1)bkgcoeflist.add(*BkgYieldnXR[i]);
//   }
//   Canvas->Print(filename);
//   BkgPdfsFile.Close(); 
  
//   //construct total Bkg pdf
//   DRecoAddPdf BkgTotalPdf("BkgTotalPdf","BkgTotalPdf",bkgpdflist,bkgcoeflist);


//   //Get the Data histograms
//   TFile DataFile(_OutputDir+"/HistosForFit.root","read");
//   gROOT->cd();
//   TH2F* HDsMassData=(TH2F*)DataFile.Get(TString("HDsMass2D"));
//   RooDataHist DsMassDataSet(HDsMassData->GetName(),HDsMassData->GetName(),nset,HDsMassData);
//   Canvas->Clear();
//   HDsMassData->Draw("lego");
//   Canvas->Print(filename);

//   //
//   RooRealVar TotalSigYield(TString("TotalSigYield"),TString("TotalSigYield"),.001,.3); 
//   DRecoAddPdf TotalPdf("TotalPdf","TotalPdf",SigTotalSigPdf,BkgTotalPdf,TotalSigYield);
  
//   //create chisquare
//   DRecoHistPdfChi2Var chi2("chi2","chi2",TotalPdf,DsMassDataSet);//
//   chi2.SetEType(RooAbsData::SumW2);
//   //minimize
//   RooMinuit minuit(chi2);
//   minuit.migrad();
  
//   ///plot on 1D mass frame
//   RooPlot* plot=mass.frame();
//   DsMassDataSet.plotOn(plot,RooFit::DataError(RooAbsData::SumW2));//
//   TotalPdf.plotOn(plot);
//   BkgTotalPdf.plotOn(plot,Normalization((1-TotalSigYield.getVal())),LineColor(2),FillColor(2),FillStyle(1001),DrawOption("LF"));
//   Canvas->Clear();
//   plot->SetTitle(TString("Signal Yield=")+(long)(TotalSigYield.getVal()*HDsMassData->Integral())
// 		 +" +- "+(long)(TotalSigYield.getError()*HDsMassData->Integral()));
//   plot->Draw();
//   Canvas->Print(filename); 
//   delete plot;

//   //create histogram of total fit
//   TH2F HTotalFit("HTotalFit","Total Fit",
// 		      HDsMassData->GetXaxis()->GetNbins(),HDsMassData->GetXaxis()->GetXmin(),HDsMassData->GetXaxis()->GetXmax(),
// 		      HDsMassData->GetYaxis()->GetNbins(),HDsMassData->GetYaxis()->GetXmin(),HDsMassData->GetYaxis()->GetXmax());  
//   TotalPdf.fillHistogram(&HTotalFit,nset,HDsMassData->Integral(),0);
//   Canvas->Clear();
//   HTotalFit.Draw("lego");
//   Canvas->Print(filename); 

//   //create histogram of signal fit
//   TH2F HSignalFit("HSignalFit","Signal Fit",
// 		      HDsMassData->GetXaxis()->GetNbins(),HDsMassData->GetXaxis()->GetXmin(),HDsMassData->GetXaxis()->GetXmax(),
// 		      HDsMassData->GetYaxis()->GetNbins(),HDsMassData->GetYaxis()->GetXmin(),HDsMassData->GetYaxis()->GetXmax());  
//   SigTotalSigPdf.fillHistogram(&HSignalFit,nset,TotalSigYield.getVal()*HDsMassData->Integral(),0);
//   Canvas->Clear();
//   HSignalFit.SetTitle(TString("Signal Yield=")+(long)(TotalSigYield.getVal()*HDsMassData->Integral())
// 		       +" +- "+(long)(TotalSigYield.getError()*HDsMassData->Integral()));
//   HSignalFit.Draw("lego");
//   Canvas->Print(filename); 
  
//   //create histogram of background  fit
//   TH2F HBkgFit("HBkgFit","Background Fit",
// 	       HDsMassData->GetXaxis()->GetNbins(),HDsMassData->GetXaxis()->GetXmin(),HDsMassData->GetXaxis()->GetXmax(),
// 	       HDsMassData->GetYaxis()->GetNbins(),HDsMassData->GetYaxis()->GetXmin(),HDsMassData->GetYaxis()->GetXmax());  
//   BkgTotalPdf.fillHistogram(&HBkgFit,nset,(1-TotalSigYield.getVal())*HDsMassData->Integral(),0);
//   Canvas->Clear();
//   HBkgFit.Draw("lego");
//   Canvas->Print(filename); 

// //   //Create 1D plot: Project the Data, total fit and total background:
// //   TH1F* HData1DTemp=(TH1F*)HDsMassData->ProjectionY();
// //   TH1F HData1D(TString(HDsMassData->GetName())+"mass",HSignalFit.GetTitle(),NDsMassBins,DsMassMin,DsMassMax);
// //   for(Int_t b=1;b<=HDsMassData->GetYaxis()->GetNbins();b++){
// //     HData1D.SetBinContent(b,HData1DTemp->GetBinContent(b));
// //     HData1D.SetBinError(b,HData1DTemp->GetBinError(b));
// //     //cout<<"sqrt(bincont)="<<sqrt(HData1DTemp->GetBinContent(b))<<"  error="<<HData1DTemp->GetBinError(b)<<endl;
// //   } 
// //   delete HData1DTemp;
// //   TH1F* HFit1D=(TH1F*)HTotalFit.ProjectionY();
// //   TH1F* HBkg1D=(TH1F*)HBkgFit.ProjectionY();
// //   Canvas->Clear();
// //   HData1D.Draw("pe");
// //   HFit1D->SetLineColor(4);
// //   HFit1D->Draw("same");
// //   HBkg1D->SetFillColor(2);HBkg1D->SetLineColor(0);
// //   HBkg1D->Draw("same");
// //   Canvas->Print(filename); 
  
//   //create 1D plots for every nXR
//   TH1F* HData1DnXR[NXR];
//   TH1F* HFit1DnXR[NXR];
//   TH1F* HBkg1DnXR[NXR];
//   for(Int_t i=0;i<NXR;i++){ 
//     HData1DnXR[i]=new TH1F(TString(HDsMassData->GetName())+(long)i,TString("nXR=")+(long)i,NDsMassBins,DsMassMin,DsMassMax);
//     HFit1DnXR[i]=new TH1F(TString(HTotalFit.GetName())+(long)i,TString("nXR=")+(long)i,NDsMassBins,DsMassMin,DsMassMax);
//     HBkg1DnXR[i]=new TH1F(TString(HBkgFit.GetName())+(long)i,TString("nXR=")+(long)i,NDsMassBins,DsMassMin,DsMassMax);
//     for(Int_t b=1;b<=HDsMassData->GetYaxis()->GetNbins();b++){
//       HData1DnXR[i]->SetBinContent(b,HDsMassData->GetBinContent(i+1,b));
//       HData1DnXR[i]->SetBinError(b,HDsMassData->GetBinError(i+1,b));
//       HFit1DnXR[i]->SetBinContent(b,HTotalFit.GetBinContent(i+1,b));
//       //HFit1DnXR[i]->SetBinError(b,HTotalFit.GetBinError(i+1,b));
//       HBkg1DnXR[i]->SetBinContent(b,HBkgFit.GetBinContent(i+1,b));
//       //HBkg1DnXR[i]->SetBinError(b,HBkgFit.GetBinError(i+1,b));
//     }
//   }
//   Canvas->Clear();
//   Canvas->Divide(2,2);
//   for(Int_t i=0;i<NXR;i++){ 
//     Canvas->cd(i+1);
//     HData1DnXR[i]->SetTitle(TString("Signal Yield=")+(long)(HFit1DnXR[i]->Integral())+" +- ");
//     HData1DnXR[i]->SetMarkerSize(1); HData1DnXR[i]->SetMarkerStyle(1);
//     HData1DnXR[i]->Draw("pe");
//     HFit1DnXR[i]->SetLineColor(4);
//     HFit1DnXR[i]->Draw("same");
//     HBkg1DnXR[i]->SetFillColor(2);HBkg1DnXR[i]->SetLineColor(0);
//     HBkg1DnXR[i]->Draw("same");
//   }  
//   Canvas->Print(filename);

 

//   //print out the nXT weight coefficients:
//   for(Int_t j=0;j<NXT-1;j++){ 
//     cout<<"nXT="<<j<<"  fit_w="<<nXTCoef[j]->getVal()<<"  true_w="<<truew[j]<<endl;    
//   }
 
//   Canvas->Print(filename+"]");

//   DataFile.Close();
// }



// void DRecoDsInclusiveAnalysis::CreateSigPdfs(){
//   OpenReducedNtuple();  
//   TFile PdfsFile(_OutputDir+"/SignalPdfsFile.root","recreate"); 
//   for(Int_t r=0;r<NXR;r++){
//     for(Int_t t=0;t<NXT;t++){
//       TString hname=TString("HDsMass_R")+(long)r+TString("_T")+(long)t;
//       TH1F HDsMassR(hname,hname,NDsMassBins,DsMassMin,DsMassMax);
//       ReducedNtuple->Draw(TString("dsmass>>")+hname,TString("evthassignal&&fragndaus==")+(long)r+TString("&&fragnpartgen==")+(long)t);
//       HDsMassR.Write();
//     }    
//   }

//   for(Int_t r=0;r<NXR;r++){
//     TString hname=TString("HDsMass_R")+(long)r;
//     TH1F HDsMass(hname,hname,NDsMassBins,DsMassMin,DsMassMax);
//     ReducedNtuple->Draw(TString("dsmass>>")+hname,TString("evthassignal&&fragndaus==")+(long)r);
//     HDsMass.Write();
//   }


//   TH1F HDsMass("HDsMass","HDsMass",NDsMassBins,DsMassMin,DsMassMax);
//   ReducedNtuple->Draw("dsmass>>HDsMass","evthassignal");
//   HDsMass.Write();

//   PdfsFile.ls();
//   PdfsFile.Close();  
//   CloseReducedNtuple();
// }

// void DRecoDsInclusiveAnalysis::PlotSigPdfs(){
//   Canvas->Clear();
//   filename=_OutputDir+"/SignalPdfs.ps";
//   Canvas->Print(filename+"[");
 
//   TFile PdfsFile(_OutputDir+"/SignalPdfsFile.root","read"); 
 

//   /////
//   TH1F* HDsMass=(TH1F*)PdfsFile.Get("HDsMass");
//   Canvas->Clear();
//   HDsMass->Draw();
//   Canvas->Print(filename);

//   ////
//   Canvas->Clear();
//   TH1F* HDsMassR;
//   Canvas->Divide(2,2);
//   for(Int_t r=0;r<4;r++){
//     Int_t pad=r+1;
//     Canvas->cd(pad);
//     TString hname=TString("HDsMass_R")+(long)r;
//     HDsMassR=(TH1F*)PdfsFile.Get(hname);
//     if(!HDsMassR){cout<<hname<<" Not found"<<endl;return;}
//     HDsMassR->SetTitle(TString("R")+(long)r);
//     HDsMassR->Draw("pe");
//   }
//   Canvas->Print(filename); 

  
//   /////
//   Canvas->Clear();
//   TH1F* HDsMassRT;
//   Canvas->Divide(NXT,NXR);
//   for(Int_t r=0;r<NXR;r++){
//     for(Int_t t=0;t<NXT;t++){
//       Int_t pad=NXT*(NXR-1-r)+t+1;
//       Canvas->cd(pad);
//       TString hname=TString("HDsMass_R")+(long)r+TString("_T")+(long)t;
//       HDsMassRT=(TH1F*)PdfsFile.Get(hname);
//       if(!HDsMassRT){cout<<hname<<" Not found"<<endl;return;}
//       HDsMassRT->SetTitle(TString("R")+(long)r+TString("_T")+(long)t);
//       HDsMassRT->Draw("pe");
//     }    
//   }
//   Canvas->Print(filename);

//   Canvas->Print(filename+"]");
//   PdfsFile.Close();
// }


// void DRecoDsInclusiveAnalysis::CreateBkgPdfs(){
//   OpenReducedNtuple();  
//   TFile PdfsFile(_OutputDir+"/BkgPdfsFile.root","recreate"); 
//   for(Int_t r=0;r<NXR;r++){
//     TString hname=TString("HDsMass_R")+(long)r;
//     TH1F HDsMassR(hname,hname,NDsMassBins,DsMassMin,DsMassMax);
//     //ReducedNtuple->Draw(TString("dsmass>>")+hname,TString("fragndaus==")+(long)r);
//     ReducedNtuple->Draw(TString("dsmass>>")+hname,TString("fragndaus==")+(long)r+"&&gammctrue<0");
//     HDsMassR.Write();
//   }

//   TH1F HDs("HDsMass","HDsMass",NDsMassBins,DsMassMin,DsMassMax);
//   //ReducedNtuple->Draw(TString("dsmass>>HDsMass"));
//   ReducedNtuple->Draw(TString("dsmass>>HDsMass"),"gammctrue<0");
//   HDs.Write();

//   PdfsFile.Close();  
//   CloseReducedNtuple();
// }

// void DRecoDsInclusiveAnalysis::PlotBkgPdfs(){
//   Canvas->Clear();
//   filename=_OutputDir+"/BkgPdfs.ps";
//   Canvas->Print(filename+"[");
  
//   TFile PdfsFile(_OutputDir+"/BkgPdfsFile.root","read"); 
//   TH1F* HDsMassR;
//   Canvas->Divide(3,2);

//   for(Int_t r=0;r<NXR;r++){
//       Canvas->cd(r+1);
//       TString hname=TString("HDsMass_R")+(long)r;
//       HDsMassR=(TH1F*)PdfsFile.Get(hname);
//       if(!HDsMassR){cout<<hname<<" Not found"<<endl;return;}
//       HDsMassR->SetTitle(TString("R")+(long)r);
//       HDsMassR->Draw("pe");
//   }
//   Canvas->Print(filename);

//   Canvas->Print(filename+"]");
//   PdfsFile.Close();
// }

// void DRecoDsInclusiveAnalysis::RooFitDsMassWSSimple(){

//   mass.setRange(DsMassMin,DsMassMax);
//   mass.setBins(NDsMassBins);


//   ///Get signal pdf from ws sample
//   TFile PdfsFile("DsTolnu/SigPdfsWS/SignalPdfsFile.root","read"); 
//   gROOT->cd();
//   TH1F* HDsMassWS=(TH1F*)PdfsFile.Get("HDsMass");
//   if(!HDsMassWS){cout<<" Not found"<<endl;return;}    
//   RooDataHist WSSigDataSet(TString("WSSigDataSet"),TString("WSSigDataSet"),RooArgSet(mass),HDsMassWS);
//   RooHistPdf WSSigPdf(TString("WSSigPdf"),TString("WSSigPdf"),RooArgSet(mass),WSSigDataSet,0);
//   PdfsFile.Close(); 

  
//   //Get the Data Histograms  to be fitted
//   OpenReducedNtuple();
//   gROOT->cd();
//   TH1F HDsMassData("HDsMassData","",NDsMassBins,DsMassMin,DsMassMax);
//   ReducedNtuple->Draw("dsmass>>HDsMassData");  
//   RooDataHist DsMassDataSet(HDsMassData.GetName(),HDsMassData.GetName(),RooArgSet(mass),&HDsMassData);
//   RooHistPdf TotWSPdf(TString("HTotWspdf"),TString("HTotWSpdf"),RooArgSet(mass),DsMassDataSet,0);
//   CloseReducedNtuple();  

//   RooRealVar WSSigYield("WSSigYield","WSSigYield",-.5,-.001);
//   DRecoAddPdf WSBkgPdf(TString("WSBkgPdf"),TString("WSBkgPdf"),WSSigPdf,TotWSPdf,WSSigYield);

//   RooRealVar SigYield(TString("SigYield"),TString("SigYield"),.01,1.);
//   RooAddPdf TotalPdf(TString("TotalPdf"),TString("TotalPdf"),WSSigPdf,WSBkgPdf,SigYield);


//   Canvas->Clear();
//   filename=_OutputDir+"/DsMassWSRooFitSimple.ps";
  
//   Canvas->Print(filename+"[");
//   RooPlot*plots=mass.frame();
  
//   Canvas->Clear();
//   WSBkgPdf.plotOn(plots,LineColor(2));
//   WSSigPdf.plotOn(plots);
//   plots->Draw();
//   Canvas->Print(filename);  
//   delete plots;

//   TotalPdf.fitTo(DsMassDataSet);
//   plots=mass.frame();
//   DsMassDataSet.plotOn(plots);
//   TotalPdf.plotOn(plots);
//   WSBkgPdf.plotOn(plots,LineColor(2),FillColor(2),FillStyle(1001),DrawOption("LF"),Normalization(1.-SigYield.getVal()));
//   plots->SetTitle(TString("Signal Yield=")+(long)(SigYield.getVal()*HDsMassData.Integral())+" +- "+(long)(SigYield.getError()*HDsMassData.Integral()));


//   //Make Residuals
//   TH1* HFit=TotalPdf.createHistogram("HFit",mass);
//   HFit->Scale(HDsMassData.Integral());
//   DRecoResiduals residuals("resid",&HDsMassData,HFit);

//   Canvas->Clear();
//   residuals.MakeFitPlotWithResiduals(Canvas,plots);
//   Canvas->Print(filename);  
 
//   Canvas->Print(filename+"]");


//   //Save the Wrong-Sign shape
//   TH1* HWSshape= WSBkgPdf.createHistogram("HWSshape",mass);
//   HWSshape->SetName("HDsMass");
//   TFile ResultsFile(_OutputDir+"/BkgPdfsFile.root","recreate"); 
//   HWSshape->Write();
//   plots->SetName("HWSplot");
//   plots->Write();
//   ResultsFile.ls();
//   ResultsFile.Close();

//   delete plots;

// }


// void DRecoDsInclusiveAnalysis::RooFitDsMassSimple(){


//   mass.setRange(DsMassMin,DsMassMax);
//   mass.setBins(NDsMassBins);

//   TFile PdfsFile("DsTolnu/SigPdfs/SignalPdfsFile.root","read"); 
//   gROOT->cd();
//   TH1F* HDsMassR=(TH1F*)PdfsFile.Get("HDsMass");
//   if(!HDsMassR){cout<<" Not found"<<endl;return;}    
//   RooDataHist Datahist(TString("Hdata"),TString("Hdata"),RooArgSet(mass),HDsMassR);
//   RooHistPdf SigPdf(TString("Hpdf"),TString("Hpdf"),RooArgSet(mass),Datahist,0);
//   PdfsFile.Close(); 


//   //Get the bkg pdfs for each n_X^R
//   TFile BkgPdfsFile("DsTolnu/BkgPdfs/BkgPdfsFile.root","read"); 
//   gROOT->cd();  
//   TH1F* HDsMassRBkg=(TH1F*)BkgPdfsFile.Get("HDsMass");
//   if(!HDsMassRBkg){cout<<" Not found"<<endl;return;}    
//   RooDataHist DatahistBkg(TString("HdataBkg"),TString("HdataBkg"),RooArgSet(mass),HDsMassRBkg);
//   RooHistPdf BkgPdf(TString("HpdfBkg"),TString("HpdfBkg"),RooArgSet(mass),DatahistBkg,0);
  
//   RooRealVar SigYield(TString("SigYieldnXR"),TString("SigYieldnXR"),0,1);
//   RooAddPdf TotalPdf(TString("TotalPdf_nXR"),TString("TotalPdf_nXR"),SigPdf,BkgPdf,SigYield);

//   BkgPdfsFile.Close(); 


//   //Get the Data 
//   OpenReducedNtuple();
//   gROOT->cd();
//   TH1F HDsMassData("HDsMassData","",NDsMassBins,DsMassMin,DsMassMax);
//   ReducedNtuple->Draw("dsmass>>HDsMassData");
//   RooDataHist DsMassDataSet(HDsMassData.GetName(),HDsMassData.GetName(),RooArgSet(mass),&HDsMassData);
//   CloseReducedNtuple();


//   filename=_OutputDir+"/DsMassRooFit.ps";
//   Canvas->Clear();
//   Canvas->Print(filename+"["); 
//   RooPlot*plots=mass.frame();

//   Canvas->Clear();
//   BkgPdf.plotOn(plots);
//   SigPdf.plotOn(plots);
//   plots->Draw();
//   Canvas->Print(filename);  
//   delete plots;

//   TotalPdf.fitTo(DsMassDataSet);
//   plots=mass.frame();
//   DsMassDataSet.plotOn(plots);
//   TotalPdf.plotOn(plots);
//   TotalPdf.plotOn(plots,Components(RooArgSet(BkgPdf)),LineColor(2),FillColor(2),FillStyle(1001),DrawOption("LF"));
//   plots->SetTitle(TString("Yield=")+(long)(SigYield.getVal()*HDsMassData.Integral())+" +- "+(long)(SigYield.getError()*HDsMassData.Integral()));

//   //Make plot with Residuals 
//   TH1* HFit=TotalPdf.createHistogram("HFit",mass);
//   HFit->Scale(HDsMassData.Integral());
//   DRecoResiduals residuals("resid",&HDsMassData,HFit);
//   Canvas->Clear();
//   residuals.MakeFitPlotWithResiduals(Canvas,plots);
//   Canvas->Print(filename);  
//   delete plots;


//   Canvas->Print(filename+"]");

// }


// void DRecoDsInclusiveAnalysis::RooFitDsMass(){


//   mass.setRange(DsMassMin,DsMassMax);
//   mass.setBins(NDsMassBins);
//   RooArgSet paramset;
//   paramset.add(mass);

//   TString dummyformula=TString("mass");
// //   //Declare the Weights for each n_X^T
// //   RooRealVar* nXTWeight[NXT];
// //   for(Int_t t=0;t<NXT;t++){
// //     nXTWeight[t]=new RooRealVar(TString("nXTWeight")+(long)t,TString("nXTWeight")+(long)t,0,.1);
// //     paramset.add(*nXTWeight[t]);
// //     dummyformula+=TString("+nXTWeight")+(long)t;
// //   }


//   //Get the signal pdfs for each n_X^R
//   RooAbsPdf* nXRPdf[NXR];
//   TFile PdfsFile("DsTolnu/SigPdfs/SignalPdfsFile.root","read"); 
//   gROOT->cd();
//   TH1F* HDsMassR[NXR];
//   RooDataHist* Datahist[NXR];
//   TList vars;
//   RooRealVar* Vars[NXR];
//   //RooHistPdf* HPdf[NXR][NXT];
//   for(Int_t r=0;r<NXR;r++){

// //     RooArgList pdflist;
// //     RooArgList weightlist;  
// //     for(Int_t t=0;t<NXT;t++){
// //       TString hid=TString("_R")+(long)r+TString("_T")+(long)t;
// //       TString hname=TString("HDsMass")+hid;
// //       HDsMassR[r][t]=(TH1F*)PdfsFile.Get(hname);
// //       if(!HDsMassR[r][t]){cout<<hname<<" Not found"<<endl;return;}

// //       Datahist[r][t]=new RooDataHist(TString("Hdata")+hid,TString("Hdata")+hid,RooArgSet(mass),HDsMassR[r][t]);
// //       HPdf[r][t]=new RooHistPdf(TString("Hpdf")+hid,TString("Hpdf")+hid,RooArgSet(mass),*Datahist[r][t],0);
// //       pdflist.add(*HPdf[r][t]);
// //       weightlist.add(*nXTWeight[t]);      
// //     }
// //     pdflist.add(*BkgPdf[r]);
// //     nXRPdf[r]=new RooAddPdf(TString("SigPdf_nXR")+(long)r,TString("SigPdf_nXR")+(long)r,pdflist,weightlist);

//     TString hid=TString("_R")+(long)r;
//     TString hname=TString("HDsMass")+hid;
//     HDsMassR[r]=(TH1F*)PdfsFile.Get(hname);
//     if(!HDsMassR[r]){cout<<hname<<" Not found"<<endl;return;}   
//     Vars[r]=new RooRealVar(TString("Var")+hid,TString("Var")+hid,DsMassMin,DsMassMax);
//     Vars[r]->setBins(NDsMassBins);
//     vars.Add(Vars[r]);
//     Datahist[r]=new RooDataHist(TString("Hdata")+hid,TString("Hdata")+hid,RooArgSet(*Vars[r]),HDsMassR[r]);
//     nXRPdf[r]=new RooHistPdf(TString("Hpdf")+hid,TString("Hpdf")+hid,RooArgSet(*Vars[r]),*Datahist[r],0);

//   }
//   PdfsFile.Close(); 



//   //Get the bkg pdfs for each n_X^R
//   TFile BkgPdfsFile("DsTolnu/BkgPdfs/BkgPdfsFile.root","read"); 
//   gROOT->cd();
//   TH1F* HDsMassRBkg[NXR];
//   RooDataHist* DatahistBkg[NXR];
//   RooHistPdf* BkgPdf[NXR];
//   RooRealVar* SigYield[NXR];
//   RooAbsPdf*  TotalPdf[NXR];
//   TList pdfs;
//   for(Int_t r=0;r<NXR;r++){

//     TString hid=TString("_R")+(long)r;    
//     TString hname=TString("HDsMass")+hid;
//     HDsMassRBkg[r]=(TH1F*)BkgPdfsFile.Get(hname);
//     if(!HDsMassRBkg[r]){cout<<hname<<" Not found"<<endl;return;}    
//     DatahistBkg[r]=new RooDataHist(TString("HdataBkg")+hid,TString("HdataBkg")+hid,RooArgSet(mass),HDsMassRBkg[r]);
//     BkgPdf[r]=new RooHistPdf(TString("HpdfBkg")+hid,TString("HpdfBkg")+hid,RooArgSet(mass),*DatahistBkg[r],0);
  
//     SigYield[r]=new RooRealVar(TString("SigYieldnXR")+(long)r,TString("SigYieldnXR")+(long)r,0,1);
//     dummyformula+=TString("+SigYieldnXR")+(long)r;
//     paramset.add(*SigYield[r]);

//     TotalPdf[r]=new RooAddPdf(TString("TotalPdf_nXR")+(long)r,TString("TotalPdf_nXR")+(long)r,*nXRPdf[r],*BkgPdf[r],*SigYield[r]);
//     pdfs.Add(TotalPdf[r]);
//   }
//   BkgPdfsFile.Close(); 



//   //Get the Data Histograms  for each n_X^R
//   OpenReducedNtuple();
//   gROOT->cd();
//   TH2F HMassVsNX("HMassVsNX","",NXR,0,NXR,NDsMassBins,DsMassMin,DsMassMax);
//   HMassVsNX.GetYaxis()->SetTitle("M_{rec}(DKX#gamma)");
//   HMassVsNX.GetXaxis()->SetTitle("n_{X}^{R}");
//   ReducedNtuple->Draw("dsmass:fragndaus>>HMassVsNX");
//   TH1* HDsMassData[NXR];
//   TList hists;   
//   RooDataHist* DsMassDataSet[NXR];
//   for(Int_t i=1;i<=NXR;i++){
//     HDsMassData[i-1]=HMassVsNX.ProjectionY(TString("_")+(long)i,i,i);
//     hists.Add(HDsMassData[i-1]);
//     DsMassDataSet[i-1]=new RooDataHist(HDsMassData[i-1]->GetName(),HDsMassData[i-1]->GetName(),RooArgSet(mass),HDsMassData[i-1]);
//   }
//   CloseReducedNtuple();


//   //Now we have a set of datasets and a set of pdfs (one for each n_X^R).. Perform a simultanous fit  
//   RooDataHist dummydata("dummydata","dummydata",RooArgList(mass),HDsMassData[0]);
//   RooGenericPdf dummypdf("dummypdf","dummypdf",dummyformula,paramset);
//   DRecoSimulChi2Var Chi2("Chi2","Chi2",dummypdf,dummydata,&hists,&pdfs,&vars,0);
  
//   RooMinuit Minuit(Chi2);
//   Minuit.migrad();
   
//   filename=_OutputDir+"/DsMassRooFit.ps";
//   Canvas->Clear();
//   Canvas->Print(filename+"[");
//   Canvas->Divide(3,2);
//   RooPlot*plots[NXR];
//   for(Int_t i=1;i<=NXR;i++){
//     Canvas->cd(i);
//     plots[i-1]=mass.frame();
//     DsMassDataSet[i-1]->plotOn(plots[i-1]);
//     nXRPdf[i-1]->plotOn(plots[i-1]);
//     plots[i-1]->Draw();
//   }
//   Canvas->Print(filename); 
//   for(Int_t i=1;i<=NXR;i++)delete plots[i-1];

// //   ///Plot one sample pdf for nXR=2
// //   Canvas->Clear();
// //   RooPlot*plot=mass.frame();
// //   DsMassDataSet[2]->plotOn(plot);
// //   RooArgSet comps;
// //   for(Int_t t=0;t<NXT;t++){
// //     comps.add(*HPdf[2][t]);
// //     nXRPdf[2]->plotOn(plot,Components(comps),LineColor(t+1));
// //   }
// //   nXRPdf[2]->plotOn(plot);
// //   plot->Draw();
// //   Canvas->Print(filename);
// //   delete plot;


//   Canvas->Print(filename+"]");

// }




