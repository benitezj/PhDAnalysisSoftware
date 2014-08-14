#include "DRecoilToolsUser/NtupleCode/DRecoDsToKKsAnalysis.hh"

#include "RooDataHist.h"
#include "RooPolynomial.h"
#include "RooHistPdf.h"
#include "RooChi2Var.h"
#include "RooMinuit.h"
#include "RooGenericPdf.h"
#include "RooProdPdf.h"
#include "RooPlot.h"
#include "RooAddPdf.h"
#include "RooGlobalFunc.h"
#include "RooCmdArg.h"
#include "TH2F.h"
#include "TList.h"
using namespace RooFit ;

ClassImp(DRecoDsToKKsAnalysis) ;
DRecoDsToKKsAnalysis::DRecoDsToKKsAnalysis(Int_t DataType, TString OutputDir, Int_t theSPMode, Int_t theRun):
  DRecoDsInclusiveAnalysis(DataType,OutputDir)
{
  cout << "Done Initializing the DRecoDsToKKsAnalysis analysis" << endl ;
}

DRecoDsToKKsAnalysis::~DRecoDsToKKsAnalysis()
{ 
}

Bool_t DRecoDsToKKsAnalysis::DsToKKsPassesCuts()
{
  Bool_t pass = kTRUE ;
  if(!DsPassesCuts()) pass = 0 ;
  //  if(KGLHTight == 0 ) pass = 0 ;
  return pass ;
}

Int_t DRecoDsToKKsAnalysis::ReduceNtuple()
{
  cout<<"Going to reduce the ntuple"<<endl ;

  if(_fChain==NULL)
  {
    cout << "No chain." << endl ;
    return 0 ;
  } 

  // Link the branches in the input chain
  SetDsToKKsBranches() ;

  // The file must be created here so the 'new' histos and Tree is created in hard drive
  TFile OutFile(_OutputDir + "/Ntuple_New.root", "recreate", _OutputDir + "/Ntuple_New.root", 1) ; // 1 = minimal compression
  if(OutFile.IsZombie())
  {
    cout << "Failed to create file" << endl ;
    return 0 ;
  }

  // Create new reduced ntuple
  TTree Ntuple("Ntuple","Ntuple",0) ;
  ReducedNtuple = &Ntuple ;
  //Long64_t maxsize   = 10000000000 ; // 10Gb
  //Long64_t fautosave = 11000000000 ; // 11Gb !! // Disable autosaving
  ReducedNtuple->SetMaxTreeSize(900000000) ; // Split files
  ReducedNtuple->SetAutoSave(900000000) ;

  // Create the branches in the Ntuple
  MakeDsToKKsReducedNtupleBranches() ;
 
  // Counters
  Int_t TotalSaved = 0 ;

  cout << "Total Entries in Chain = " << _fChain->GetEntries() << endl ;
  cout << "Going to start the event loop" << endl ;
  eventid = 0 ;
  Int_t ngoodthisevent = 0 ;
  goodeventid = 0 ;
  while(_fChain->GetEntry(eventid,0)>0)
  {
    eventid++ ;
    if(eventid%1000==0) cout << eventid << " events done.  With " << TotalSaved << " Candidates saved." << endl ;
 
    /// Event vars
    FillDsEventVars() ;
    if(!DsEvtPassesCuts()) continue ;

    ngoodthisevent=0 ;
    if(nEvt>MAXNCANDS) nEvt = MAXNCANDS ; // Protect array size
    
    // Now do real loop
    for(EvtIdx=0 ; EvtIdx<nEvt ; EvtIdx++)
    {
      FillDsToKKsVars() ;
      if(!DsToKKsPassesCuts())continue ;

      // Save
      ngoodthisevent++ ;
      TotalSaved++ ;
      ReducedNtuple->Fill() ;
    }// loop
    if(ngoodthisevent>0) goodeventid++ ;
  }

  // Print summary
  cout << "--------Summary-------"                << endl ;
  cout << "     Total events = "   << eventid     << endl ;
  cout << "Total cands saved = "   << TotalSaved  << endl ;
  cout << "--------End Summary---"                << endl ;

  // Save the Ntuple and histograms
  cout << "Going to save the ntuple and histograms" << endl ;

  //---
  if(ReducedNtuple->AutoSave("Overwrite"))
  {
    cout << "Ntuple written." << endl ;
  }
  else
  {
    cout << "Failed to write ntuple" << endl ;
    return 0 ;
  }

  cout << "Objects in the current file" << endl ;
  OutFile.ls() ;

  OutFile.Close() ; // This will delete all objects created inside the file if they were not written
  cout << OutFile.GetName() << " has been closed." << endl ;

  return 1 ;
}


void DRecoDsToKKsAnalysis::FillDsToKKsVars()
{  
  if(EvtIdx>=MAXNCANDS) return ;
  // Fill inclusive fit variables
  FillDsInclusiveVars() ;

  // Fill reduced ntuple variables
  EvtProbChi2MMCFit   =   ProbChi2MMCFit_in[EvtIdx] ;
  EvtProbChi2ListCFit = ProbChi2ListCFit_in[EvtIdx] ;
  EvtProbChi2ListUFit = ProbChi2ListUFit_in[EvtIdx] ;
  EvtFitStatusCFit    =    FitStatusCFit_in[EvtIdx] ;
  EvtFitStatusUFit    =    FitStatusUFit_in[EvtIdx] ;
  EExtra              = EvtEExtraNeutral_in[EvtIdx] ;
  // VtxStatus           =     EvtVtxStatus_in[EvtIdx] ;

  KpxMMRaw      =      KpxMMRaw_in[EvtIdx] ;
  KpyMMRaw      =      KpyMMRaw_in[EvtIdx] ;
  KpzMMRaw      =      KpzMMRaw_in[EvtIdx] ;
  KenergyMMRaw  =  KenergyMMRaw_in[EvtIdx] ;
  KspxMMRaw     =     KspxMMRaw_in[EvtIdx] ;
  KspyMMRaw     =     KspyMMRaw_in[EvtIdx] ;      
  KspzMMRaw     =     KspzMMRaw_in[EvtIdx] ;
  KsenergyMMRaw = KsenergyMMRaw_in[EvtIdx] ;
  KsmassMMRaw2  = KsenergyMMRaw*KsenergyMMRaw - KspxMMRaw*KspxMMRaw - KspyMMRaw*KspyMMRaw - KspzMMRaw*KspzMMRaw ;
  DspxMMRaw     =     DspxMMRaw_in[EvtIdx] ;        
  DspyMMRaw     =     DspyMMRaw_in[EvtIdx] ;        
  DspzMMRaw     =     DspzMMRaw_in[EvtIdx] ;        
  DsenergyMMRaw = DsenergyMMRaw_in[EvtIdx] ;
  DsmassMMRaw   =   DsmassMMRaw_in[EvtIdx] ;

  KpxMMCFit      =      KpxMMCFit_in[EvtIdx] ;
  KpyMMCFit      =      KpyMMCFit_in[EvtIdx] ;
  KpzMMCFit      =      KpzMMCFit_in[EvtIdx] ;
  KenergyMMCFit  =  KenergyMMCFit_in[EvtIdx] ;
  KspxMMCFit     =     KspxMMCFit_in[EvtIdx] ;
  KspyMMCFit     =     KspyMMCFit_in[EvtIdx] ;
  KspzMMCFit     =     KspzMMCFit_in[EvtIdx] ;
  KsenergyMMCFit = KsenergyMMCFit_in[EvtIdx] ;
  DspxMMCFit     =     DspxMMCFit_in[EvtIdx] ;
  DspyMMCFit     =     DspyMMCFit_in[EvtIdx] ;
  DspzMMCFit     =     DspzMMCFit_in[EvtIdx] ;
  DsenergyMMCFit = DsenergyMMCFit_in[EvtIdx] ;
  KsmassMMCFit2  = KsenergyMMCFit*KsenergyMMCFit - KspxMMCFit*KspxMMCFit - KspyMMCFit*KspyMMCFit - KspzMMCFit*KspzMMCFit ;
  DsmassMMCFit2  = DsenergyMMCFit*DsenergyMMCFit - DspxMMCFit*KspxMMCFit - DspyMMCFit*DspyMMCFit - DspzMMCFit*DspzMMCFit ;

  KspxListRaw       = (fabs(KspxListRaw_in[EvtIdx])<1000)       ? KspxListRaw_in[EvtIdx]       : -9 ;
  KspyListRaw       = (fabs(KspyListRaw_in[EvtIdx])<1000)       ? KspyListRaw_in[EvtIdx]       : -9 ;
  KspzListRaw       = (fabs(KspzListRaw_in[EvtIdx])<1000)       ? KspzListRaw_in[EvtIdx]       : -9 ;
  KsenergyListRaw   = (fabs(KsenergyListRaw_in[EvtIdx])<1000)   ? KsenergyListRaw_in[EvtIdx]   : -9 ;
  Ksd1pxListRaw     = (fabs(Ksd1pxListRaw_in[EvtIdx])<1000)     ? Ksd1pxListRaw_in[EvtIdx]     : -9 ;
  Ksd1pyListRaw     = (fabs(Ksd1pyListRaw_in[EvtIdx])<1000)     ? Ksd1pyListRaw_in[EvtIdx]     : -9 ;
  Ksd1pzListRaw     = (fabs(Ksd1pzListRaw_in[EvtIdx])<1000)     ? Ksd1pzListRaw_in[EvtIdx]     : -9 ;
  Ksd1energyListRaw = (fabs(Ksd1energyListRaw_in[EvtIdx])<1000) ? Ksd1energyListRaw_in[EvtIdx] : -9 ;
  Ksd2pxListRaw     = (fabs(Ksd2pxListRaw_in[EvtIdx])<1000)     ? Ksd2pxListRaw_in[EvtIdx]     : -9 ;
  Ksd2pyListRaw     = (fabs(Ksd2pyListRaw_in[EvtIdx])<1000)     ? Ksd2pyListRaw_in[EvtIdx]     : -9 ;
  Ksd2pzListRaw     = (fabs(Ksd2pzListRaw_in[EvtIdx])<1000)     ? Ksd2pzListRaw_in[EvtIdx]     : -9 ;
  Ksd2energyListRaw = (fabs(Ksd2energyListRaw_in[EvtIdx])<1000) ? Ksd2energyListRaw_in[EvtIdx] : -9 ;
  KsenergyListRaw   = Ksd1energyListRaw + Ksd2energyListRaw  ;
  DspxListRaw       = (fabs(DspxListRaw_in[EvtIdx])<1000)       ? DspxListRaw_in[EvtIdx]       : -9 ;
  DspyListRaw       = (fabs(DspyListRaw_in[EvtIdx])<1000)       ? DspyListRaw_in[EvtIdx]       : -9 ;
  DspzListRaw       = (fabs(DspzListRaw_in[EvtIdx])<1000)       ? DspzListRaw_in[EvtIdx]       : -9 ;
  DsenergyListRaw   = (fabs(DsenergyListRaw_in[EvtIdx])<1000)   ? DsenergyListRaw_in[EvtIdx]   : -9 ;
  KsmassListRaw2    = KsenergyListRaw*KsenergyListRaw - KspxListRaw*KspxListRaw - KspyListRaw*KspyListRaw - KspzListRaw*KspzListRaw ; 
  DsmassListRaw2    = DsenergyListRaw*DsenergyListRaw - DspxListRaw*DspxListRaw - DspyListRaw*DspyListRaw - DspzListRaw*DspzListRaw ; 


  KspxListUFit       = (fabs(KspxListUFit_in[EvtIdx])<1000)       ? KspxListUFit_in[EvtIdx]       : -9 ;
  KspyListUFit       = (fabs(KspyListUFit_in[EvtIdx])<1000)       ? KspyListUFit_in[EvtIdx]       : -9 ;
  KspzListUFit       = (fabs(KspzListUFit_in[EvtIdx])<1000)       ? KspzListUFit_in[EvtIdx]       : -9 ;
  KsenergyListUFit   = (fabs(KsenergyListUFit_in[EvtIdx])<1000)   ? KsenergyListUFit_in[EvtIdx]   : -9 ;
  Ksd1pxListUFit     = (fabs(Ksd1pxListUFit_in[EvtIdx])<1000)     ? Ksd1pxListUFit_in[EvtIdx]     : -9 ;
  Ksd1pyListUFit     = (fabs(Ksd1pyListUFit_in[EvtIdx])<1000)     ? Ksd1pyListUFit_in[EvtIdx]     : -9 ;
  Ksd1pzListUFit     = (fabs(Ksd1pzListUFit_in[EvtIdx])<1000)     ? Ksd1pzListUFit_in[EvtIdx]     : -9 ;
  Ksd1energyListUFit = (fabs(Ksd1energyListUFit_in[EvtIdx])<1000) ? Ksd1energyListUFit_in[EvtIdx] : -9 ;
  Ksd2pxListUFit     = (fabs(Ksd2pxListUFit_in[EvtIdx])<1000)     ? Ksd2pxListUFit_in[EvtIdx]     : -9 ;
  Ksd2pyListUFit     = (fabs(Ksd2pyListUFit_in[EvtIdx])<1000)     ? Ksd2pyListUFit_in[EvtIdx]     : -9 ;
  Ksd2pzListUFit     = (fabs(Ksd2pzListUFit_in[EvtIdx])<1000)     ? Ksd2pzListUFit_in[EvtIdx]     : -9 ;
  Ksd2energyListUFit = (fabs(Ksd2energyListUFit_in[EvtIdx])<1000) ? Ksd2energyListUFit_in[EvtIdx] : -9 ;
  KsenergyListUFit   = Ksd1energyListUFit + Ksd2energyListUFit  ;
  DspxListUFit       = (fabs(DspxListUFit_in[EvtIdx])<1000)       ? DspxListUFit_in[EvtIdx]       : -9 ;
  DspyListUFit       = (fabs(DspyListUFit_in[EvtIdx])<1000)       ? DspyListUFit_in[EvtIdx]       : -9 ;
  DspzListUFit       = (fabs(DspzListUFit_in[EvtIdx])<1000)       ? DspzListUFit_in[EvtIdx]       : -9 ;
  DsenergyListUFit   = (fabs(DsenergyListUFit_in[EvtIdx])<1000)   ? DsenergyListUFit_in[EvtIdx]   : -9 ;
  DsmassListUFit2    = DsenergyListUFit*DsenergyListUFit - DspxListUFit*DspxListUFit - DspyListUFit*DspyListUFit - DspzListUFit*DspzListUFit ; 
  KsmassListUFit2    = KsenergyListUFit*KsenergyListUFit - KspxListUFit*KspxListUFit - KspyListUFit*KspyListUFit - KspzListUFit*KspzListUFit ; 

  KspxListCFit       = (fabs(KspxListCFit_in[EvtIdx])<1000)       ? KspxListCFit_in[EvtIdx]       : -9 ;
  KspyListCFit       = (fabs(KspyListCFit_in[EvtIdx])<1000)       ? KspyListCFit_in[EvtIdx]       : -9 ;
  KspzListCFit       = (fabs(KspzListCFit_in[EvtIdx])<1000)       ? KspzListCFit_in[EvtIdx]       : -9 ;
  KsenergyListCFit   = (fabs(KsenergyListCFit_in[EvtIdx])<1000)   ? KsenergyListCFit_in[EvtIdx]   : -9 ;
  Ksd1pxListCFit     = (fabs(Ksd1pxListCFit_in[EvtIdx])<1000)     ? Ksd1pxListCFit_in[EvtIdx]     : -9 ;
  Ksd1pyListCFit     = (fabs(Ksd1pyListCFit_in[EvtIdx])<1000)     ? Ksd1pyListCFit_in[EvtIdx]     : -9 ;
  Ksd1pzListCFit     = (fabs(Ksd1pzListCFit_in[EvtIdx])<1000)     ? Ksd1pzListCFit_in[EvtIdx]     : -9 ;
  Ksd1energyListCFit = (fabs(Ksd1energyListCFit_in[EvtIdx])<1000) ? Ksd1energyListCFit_in[EvtIdx] : -9 ;
  Ksd2pxListCFit     = (fabs(Ksd2pxListCFit_in[EvtIdx])<1000)     ? Ksd2pxListCFit_in[EvtIdx]     : -9 ;
  Ksd2pyListCFit     = (fabs(Ksd2pyListCFit_in[EvtIdx])<1000)     ? Ksd2pyListCFit_in[EvtIdx]     : -9 ;
  Ksd2pzListCFit     = (fabs(Ksd2pzListCFit_in[EvtIdx])<1000)     ? Ksd2pzListCFit_in[EvtIdx]     : -9 ;
  Ksd2energyListCFit = (fabs(Ksd2energyListCFit_in[EvtIdx])<1000) ? Ksd2energyListCFit_in[EvtIdx] : -9 ;
  KsenergyListCFit   = Ksd1energyListCFit + Ksd2energyListCFit  ;
  DspxListCFit       = (fabs(DspxListCFit_in[EvtIdx])<1000)       ? DspxListCFit_in[EvtIdx]       : -9 ;
  DspyListCFit       = (fabs(DspyListCFit_in[EvtIdx])<1000)       ? DspyListCFit_in[EvtIdx]       : -9 ;
  DspzListCFit       = (fabs(DspzListCFit_in[EvtIdx])<1000)       ? DspzListCFit_in[EvtIdx]       : -9 ;
  DsenergyListCFit   = (fabs(DsenergyListCFit_in[EvtIdx])<1000)   ? DsenergyListCFit_in[EvtIdx]   : -9 ;
  DsmassListCFit2    = DsenergyListCFit*DsenergyListCFit - DspxListCFit*DspxListCFit - DspyListCFit*DspyListCFit - DspzListCFit*DspzListCFit ; 
  KsmassListCFit2    = KsenergyListCFit*KsenergyListCFit - KspxListCFit*KspxListCFit - KspyListCFit*KspyListCFit - KspzListCFit*KspzListCFit ;
  
  Int_t dsidx            = -9 ;
  Int_t kidx             = -9 ;
  Int_t ksidx            = -9 ;
  Int_t truemothidx      = -9 ;
  Int_t truegrandmothidx = -9 ;
  truemothlund           = -9 ;
  truegrandmothlund      = -9 ;

  kselectorsmap  = -9 ;
  KsVtxStatus = -2 ;
  //cout << "DsSt daughters: " << DsStd1Lund[DsStIdx] << " " << DsStd2Lund[DsStIdx] << endl ;
  if(abs(DsStd1Lund[DsStIdx])==431 || abs(DsStd2Lund[DsStIdx])==431)
  {
    if     (abs(DsStd1Lund[DsStIdx])==431){ dsidx = DsStd1Idx[DsStIdx] ; }
    else if(abs(DsStd2Lund[DsStIdx])==431){ dsidx = DsStd2Idx[DsStIdx] ; }
    else { cout << "Cannot find dsidx" << endl ; }
    //cout << "dsidx = " << dsidx << endl ;
    if(dsidx!=-9)
    {
      if(abs(Dsd1Lund[dsidx])==321){ kidx  = Dsd1Idx[dsidx] ; }
      if(abs(Dsd2Lund[dsidx])==321){ kidx  = Dsd2Idx[dsidx] ; }
      if(abs(Dsd1Lund[dsidx])==310){ ksidx = Dsd1Idx[dsidx] ; }
      if(abs(Dsd2Lund[dsidx])==310){ ksidx = Dsd2Idx[dsidx] ; }
      //cout << "ksidx is " << ksidx << endl ;
      //cout << "Ds daughters: " << Dsd1Lund[dsidx] << " " << Dsd2Lund[dsidx] << endl ;
      if(kidx!=-9)
      {
	truemothidx       = mothIdx[KMCIdx[kidx]] ;
	truemothlund      = mcLund[truemothidx] ;
	truegrandmothidx  = mothIdx[truemothidx] ;
	truegrandmothlund = mcLund[truegrandmothidx] ;
	kselectorsmap     = KSelectorsMap[kidx] ;
      }
      KsVtxStatus = KsVtxStatus_in[ksidx] ;
      //if(KsVtxStatus>-1) cout << KsVtxStatus_in[ksidx] << endl << endl ;
    }
    else
    {
      cout << "Cannot find dsidx, dsidx=" << dsidx << endl ;
    }
  }
  kselectorsmap  = KSelectorsMap[kidx] ;
  truemothndaus = dauLen[truemothidx] ;
  Int_t truedauidx = dauIdx[truemothidx] ;

  truesib1lund = 0 ; 
  truesib2lund = 0 ; 
  truesib3lund = 0 ; 
  truesib4lund = 0 ; 
  truesib5lund = 0 ; 

  truesib1lund = mcLund[truedauidx] ;
  if(truemothndaus > 1){ truesib2lund = mcLund[truedauidx+1] ; }
  if(truemothndaus > 2){ truesib3lund = mcLund[truedauidx+2] ; }
  if(truemothndaus > 3){ truesib4lund = mcLund[truedauidx+3] ; } 
  if(truemothndaus > 4){ truesib5lund = mcLund[truedauidx+4] ; } 

  Int_t KSelector = KSelectorsMap[kidx] ;
  //cout << "Kselector is " << KSelector << "    kidx is " << kidx << endl;

  KSMSNotAPion  = ((KSelector&(0x1<<0)  )!=0) ;
  KSMSVeryLoose = ((KSelector&(0x1<<1)  )!=0) ;
  KSMSLoose     = ((KSelector&(0x1<<2)  )!=0) ;
  KSMSTight     = ((KSelector&(0x1<<3)  )!=0) ;
  KSMSVeryTight = ((KSelector&(0x1<<4)  )!=0) ;
  KNNNotAPion   = ((KSelector&(0x1<<5)  )!=0) ;
  KNNVeryLoose  = ((KSelector&(0x1<<6)  )!=0) ;
  KNNLoose      = ((KSelector&(0x1<<7)  )!=0) ;
  KNNTight      = ((KSelector&(0x1<<8)  )!=0) ;
  KNNVeryTight  = ((KSelector&(0x1<<9)  )!=0) ;
  KLHNotAPion   = ((KSelector&(0x1<<10) )!=0) ;
  KLHVeryLoose  = ((KSelector&(0x1<<11) )!=0) ;
  KLHLoose      = ((KSelector&(0x1<<12) )!=0) ;
  KLHTight      = ((KSelector&(0x1<<13) )!=0) ;
  KLHVeryTight  = ((KSelector&(0x1<<14) )!=0) ;
  KGLHNotAPion  = ((KSelector&(0x1<<15) )!=0) ;
  KGLHVeryLoose = ((KSelector&(0x1<<16) )!=0) ;
  KGLHLoose     = ((KSelector&(0x1<<17) )!=0) ;
  KGLHTight     = ((KSelector&(0x1<<18) )!=0) ;
  KGLHVeryTight = ((KSelector&(0x1<<19) )!=0) ;
  KBDTNotAPion  = ((KSelector&(0x1<<20) )!=0) ;
  KBDTVeryLoose = ((KSelector&(0x1<<21) )!=0) ;
  KBDTLoose     = ((KSelector&(0x1<<22) )!=0) ;
  KBDTTight     = ((KSelector&(0x1<<23) )!=0) ;
  KBDTVeryTight = ((KSelector&(0x1<<24) )!=0) ;
  KKMNotAPion   = ((KSelector&(0x1<<25) )!=0) ;
  KKMVeryLoose  = ((KSelector&(0x1<<26) )!=0) ;
  KKMLoose      = ((KSelector&(0x1<<27) )!=0) ;
  KKMTight      = ((KSelector&(0x1<<28) )!=0) ;
  KKMVeryTight  = ((KSelector&(0x1<<29) )!=0) ;

}

Int_t DRecoDsToKKsAnalysis::SetDsToKKsBranches()
{


  if(_fChain==NULL)
  {
    cout << "No Chain" << endl ;
    return 0 ;
  } 
  if( _fChain->GetBranch("EvtDsToKKsProbChi2MMFit")    ) _fChain->SetBranchAddress("EvtDsToKKsProbChi2MMFit"    , ProbChi2MMCFit_in    ) ;
  if( _fChain->GetBranch("EvtDsToKKsProbChi2ListUFit") ) _fChain->SetBranchAddress("EvtDsToKKsProbChi2ListUFit" , ProbChi2ListUFit_in  ) ;
  if( _fChain->GetBranch("EvtDsToKKsProbChi2ListCFit") ) _fChain->SetBranchAddress("EvtDsToKKsProbChi2ListCFit" , ProbChi2ListCFit_in  ) ;
  if( _fChain->GetBranch("EvtEExtraNeutral")           ) _fChain->SetBranchAddress("EvtEExtraNeutral"           , EvtEExtraNeutral_in  ) ;
  if( _fChain->GetBranch("EvtDsToKKsFitStatusCFit")    ) _fChain->SetBranchAddress("EvtDsToKKsFitStatusCFit"    , FitStatusCFit_in     ) ;
  if( _fChain->GetBranch("EvtDsToKKsFitStatusUFit")    ) _fChain->SetBranchAddress("EvtDsToKKsFitStatusUFit"    , FitStatusUFit_in     ) ;
  if( _fChain->GetBranch("KsVtxStatus")                ) _fChain->SetBranchAddress("KsVtxStatus"                , KsVtxStatus_in  ) ;
  //  if( _fChain->GetBranch("EvtVtxStatus")            ) _fChain->SetBranchAddress("EvtVtxStatus"               , EvtVtxStatus_in  ) ;

  if( _fChain->GetBranch("EvtKpxMMRaw")            ) _fChain->SetBranchAddress("EvtKpxMMRaw"            , KpxMMRaw_in           ) ;
  if( _fChain->GetBranch("EvtKpyMMRaw")            ) _fChain->SetBranchAddress("EvtKpyMMRaw"            , KpyMMRaw_in           ) ;
  if( _fChain->GetBranch("EvtKpzMMRaw")            ) _fChain->SetBranchAddress("EvtKpzMMRaw"            , KpzMMRaw_in           ) ;
  if( _fChain->GetBranch("EvtKenergyMMRaw")        ) _fChain->SetBranchAddress("EvtKenergyMMRaw"        , KenergyMMRaw_in       ) ;
  if( _fChain->GetBranch("EvtKspxMMRaw")           ) _fChain->SetBranchAddress("EvtKspxMMRaw"           , KspxMMRaw_in          ) ;
  if( _fChain->GetBranch("EvtKspyMMRaw")           ) _fChain->SetBranchAddress("EvtKspyMMRaw"           , KspyMMRaw_in          ) ;
  if( _fChain->GetBranch("EvtKspzMMRaw")           ) _fChain->SetBranchAddress("EvtKspzMMRaw"           , KspzMMRaw_in          ) ;
  if( _fChain->GetBranch("EvtKsenergyMMRaw")       ) _fChain->SetBranchAddress("EvtKsenergyMMRaw"       , KsenergyMMRaw_in      ) ;
  if( _fChain->GetBranch("EvtDspxMMRaw")           ) _fChain->SetBranchAddress("EvtDspxMMRaw"           , DspxMMRaw_in          ) ;
  if( _fChain->GetBranch("EvtDspyMMRaw")           ) _fChain->SetBranchAddress("EvtDspyMMRaw"           , DspyMMRaw_in          ) ;
  if( _fChain->GetBranch("EvtDspzMMRaw")           ) _fChain->SetBranchAddress("EvtDspzMMRaw"           , DspzMMRaw_in          ) ;
  if( _fChain->GetBranch("EvtDsenergyMMRaw")       ) _fChain->SetBranchAddress("EvtDsenergyMMRaw"       , DsenergyMMRaw_in      ) ;
  if( _fChain->GetBranch("EvtDsMass")              ) _fChain->SetBranchAddress("EvtDsMass"              , DsmassMMRaw_in        ) ;  

  if( _fChain->GetBranch("EvtKpxMMCFit")           ) _fChain->SetBranchAddress("EvtKpxMMCFit"           , KpxMMCFit_in          ) ;
  if( _fChain->GetBranch("EvtKpyMMCFit")           ) _fChain->SetBranchAddress("EvtKpyMMCFit"           , KpyMMCFit_in          ) ;
  if( _fChain->GetBranch("EvtKpzMMCFit")           ) _fChain->SetBranchAddress("EvtKpzMMCFit"           , KpzMMCFit_in          ) ;
  if( _fChain->GetBranch("EvtKenergyMMCFit")       ) _fChain->SetBranchAddress("EvtKenergyMMCFit"       , KenergyMMCFit_in      ) ;
  if( _fChain->GetBranch("EvtKspxMMCFit")          ) _fChain->SetBranchAddress("EvtKspxMMCFit"          , KspxMMCFit_in         ) ;
  if( _fChain->GetBranch("EvtKspyMMCFit")          ) _fChain->SetBranchAddress("EvtKspyMMCFit"          , KspyMMCFit_in         ) ;
  if( _fChain->GetBranch("EvtKspzMMCFit")          ) _fChain->SetBranchAddress("EvtKspzMMCFit"          , KspzMMCFit_in         ) ;
  if( _fChain->GetBranch("EvtKsenergyMMCFit")      ) _fChain->SetBranchAddress("EvtKsenergyMMCFit"      , KsenergyMMCFit_in     ) ;  
  if( _fChain->GetBranch("EvtDspxMMCFit")          ) _fChain->SetBranchAddress("EvtDspxMMCFit"          , DspxMMCFit_in         ) ;
  if( _fChain->GetBranch("EvtDspyMMCFit")          ) _fChain->SetBranchAddress("EvtDspyMMCFit"          , DspyMMCFit_in         ) ;
  if( _fChain->GetBranch("EvtDspzMMCFit")          ) _fChain->SetBranchAddress("EvtDspzMMCFit"          , DspzMMCFit_in         ) ;
  if( _fChain->GetBranch("EvtDsenergyMMCFit")      ) _fChain->SetBranchAddress("EvtDsenergyMMCFit"      , DsenergyMMCFit_in     ) ;
					 
  if( _fChain->GetBranch("EvtKspxListRaw" )        ) _fChain->SetBranchAddress("EvtKspxListRaw"         , KspxListRaw_in        ) ;
  if( _fChain->GetBranch("EvtKspyListRaw" )        ) _fChain->SetBranchAddress("EvtKspyListRaw"         , KspyListRaw_in        ) ;
  if( _fChain->GetBranch("EvtKspzListRaw" )        ) _fChain->SetBranchAddress("EvtKspzListRaw"         , KspzListRaw_in        ) ;
  if( _fChain->GetBranch("EvtKsenergyListRaw" )    ) _fChain->SetBranchAddress("EvtKsenergyListRaw"     , KsenergyListRaw_in    ) ;
  if( _fChain->GetBranch("EvtKsd1pxListRaw" )      ) _fChain->SetBranchAddress("EvtKsd1pxListRaw"       , Ksd1pxListRaw_in      ) ;
  if( _fChain->GetBranch("EvtKsd1pyListRaw" )      ) _fChain->SetBranchAddress("EvtKsd1pyListRaw"       , Ksd1pyListRaw_in      ) ;
  if( _fChain->GetBranch("EvtKsd1pzListRaw" )      ) _fChain->SetBranchAddress("EvtKsd1pzListRaw"       , Ksd1pzListRaw_in      ) ;
  if( _fChain->GetBranch("EvtKsd1energyListRaw")   ) _fChain->SetBranchAddress("EvtKsd1energyListRaw"   , Ksd1energyListRaw_in  ) ;
  if( _fChain->GetBranch("EvtKsd2pxListRaw" )      ) _fChain->SetBranchAddress("EvtKsd2pxListRaw"       , Ksd2pxListRaw_in      ) ;
  if( _fChain->GetBranch("EvtKsd2pyListRaw" )      ) _fChain->SetBranchAddress("EvtKsd2pyListRaw"       , Ksd2pyListRaw_in      ) ;
  if( _fChain->GetBranch("EvtKsd2pzListRaw" )      ) _fChain->SetBranchAddress("EvtKsd2pzListRaw"       , Ksd2pzListRaw_in      ) ;
  if( _fChain->GetBranch("EvtKsd2energyListRaw")   ) _fChain->SetBranchAddress("EvtKsd2energyListRaw"   , Ksd2energyListRaw_in  ) ;
  if( _fChain->GetBranch("EvtDspxListRaw" )        ) _fChain->SetBranchAddress("EvtDspxListRaw"         , DspxListRaw_in        ) ;
  if( _fChain->GetBranch("EvtDspyListRaw" )        ) _fChain->SetBranchAddress("EvtDspyListRaw"         , DspyListRaw_in        ) ;
  if( _fChain->GetBranch("EvtDspzListRaw" )        ) _fChain->SetBranchAddress("EvtDspzListRaw"         , DspzListRaw_in        ) ;
  if( _fChain->GetBranch("EvtDsenergyListRaw")     ) _fChain->SetBranchAddress("EvtDsenergyListRaw"     , DsenergyListRaw_in    ) ;

  if( _fChain->GetBranch("EvtKspxListCFit" )       ) _fChain->SetBranchAddress("EvtKspxListCFit"        , KspxListCFit_in       ) ;
  if( _fChain->GetBranch("EvtKspyListCFit" )       ) _fChain->SetBranchAddress("EvtKspyListCFit"        , KspyListCFit_in       ) ;
  if( _fChain->GetBranch("EvtKspzListCFit" )       ) _fChain->SetBranchAddress("EvtKspzListCFit"        , KspzListCFit_in       ) ;
  if( _fChain->GetBranch("EvtKsenergyListCFit" )   ) _fChain->SetBranchAddress("EvtKsenergyListCFit"    , KsenergyListCFit_in   ) ;
  if( _fChain->GetBranch("EvtKsd1pxListCFit" )     ) _fChain->SetBranchAddress("EvtKsd1pxListCFit"      , Ksd1pxListCFit_in     ) ;
  if( _fChain->GetBranch("EvtKsd1pyListCFit" )     ) _fChain->SetBranchAddress("EvtKsd1pyListCFit"      , Ksd1pyListCFit_in     ) ;
  if( _fChain->GetBranch("EvtKsd1pzListCFit" )     ) _fChain->SetBranchAddress("EvtKsd1pzListCFit"      , Ksd1pzListCFit_in     ) ;
  if( _fChain->GetBranch("EvtKsd1energyListCFit" ) ) _fChain->SetBranchAddress("EvtKsd1energyListCFit"  , Ksd1energyListCFit_in ) ;
  if( _fChain->GetBranch("EvtKsd2pxListCFit" )     ) _fChain->SetBranchAddress("EvtKsd2pxListCFit"      , Ksd2pxListCFit_in     ) ;
  if( _fChain->GetBranch("EvtKsd2pyListCFit" )     ) _fChain->SetBranchAddress("EvtKsd2pyListCFit"      , Ksd2pyListCFit_in     ) ;
  if( _fChain->GetBranch("EvtKsd2pzListCFit" )     ) _fChain->SetBranchAddress("EvtKsd2pzListCFit"      , Ksd2pzListCFit_in     ) ;
  if( _fChain->GetBranch("EvtKsd2energyListCFit" ) ) _fChain->SetBranchAddress("EvtKsd2energyListCFit"  , Ksd2energyListCFit_in ) ;
  if( _fChain->GetBranch("EvtDspxListCFit" )       ) _fChain->SetBranchAddress("EvtDspxListCFit"        , DspxListCFit_in       ) ;
  if( _fChain->GetBranch("EvtDspyListCFit" )       ) _fChain->SetBranchAddress("EvtDspyListCFit"        , DspyListCFit_in       ) ;
  if( _fChain->GetBranch("EvtDspzListCFit" )       ) _fChain->SetBranchAddress("EvtDspzListCFit"        , DspzListCFit_in       ) ;
  if( _fChain->GetBranch("EvtDsenergyListCFit" )   ) _fChain->SetBranchAddress("EvtDsenergyListCFit"    , DsenergyListCFit_in   ) ;

  if( _fChain->GetBranch("EvtKspxListUFit" )       ) _fChain->SetBranchAddress("EvtKspxListUFit"        , KspxListUFit_in       ) ;
  if( _fChain->GetBranch("EvtKspyListUFit" )       ) _fChain->SetBranchAddress("EvtKspyListUFit"        , KspyListUFit_in       ) ;
  if( _fChain->GetBranch("EvtKspzListUFit" )       ) _fChain->SetBranchAddress("EvtKspzListUFit"        , KspzListUFit_in       ) ;
  if( _fChain->GetBranch("EvtKsenergyListUFit")    ) _fChain->SetBranchAddress("EvtKsenergyListUFit"    , KsenergyListUFit_in   ) ;
  if( _fChain->GetBranch("EvtKsd1pxListUFit" )     ) _fChain->SetBranchAddress("EvtKsd1pxListUFit"      , Ksd1pxListUFit_in     ) ;
  if( _fChain->GetBranch("EvtKsd1pyListUFit" )     ) _fChain->SetBranchAddress("EvtKsd1pyListUFit"      , Ksd1pyListUFit_in     ) ;
  if( _fChain->GetBranch("EvtKsd1pzListUFit" )     ) _fChain->SetBranchAddress("EvtKsd1pzListUFit"      , Ksd1pzListUFit_in     ) ;
  if( _fChain->GetBranch("EvtKsd1energyListUFit")  ) _fChain->SetBranchAddress("EvtKsd1energyListUFit"  , Ksd1energyListUFit_in ) ;
  if( _fChain->GetBranch("EvtKsd2pxListUFit" )     ) _fChain->SetBranchAddress("EvtKsd2pxListUFit"      , Ksd2pxListUFit_in     ) ;
  if( _fChain->GetBranch("EvtKsd2pyListUFit" )     ) _fChain->SetBranchAddress("EvtKsd2pyListUFit"      , Ksd2pyListUFit_in     ) ;
  if( _fChain->GetBranch("EvtKsd2pzListUFit" )     ) _fChain->SetBranchAddress("EvtKsd2pzListUFit"      , Ksd2pzListUFit_in     ) ;
  if( _fChain->GetBranch("EvtKsd2energyListUFit")  ) _fChain->SetBranchAddress("EvtKsd2energyListUFit"  , Ksd2energyListUFit_in ) ;
  if( _fChain->GetBranch("EvtDspxListUFit" )       ) _fChain->SetBranchAddress("EvtDspxListUFit"        , DspxListUFit_in       ) ;
  if( _fChain->GetBranch("EvtDspyListUFit" )       ) _fChain->SetBranchAddress("EvtDspyListUFit"        , DspyListUFit_in       ) ;
  if( _fChain->GetBranch("EvtDspzListUFit" )       ) _fChain->SetBranchAddress("EvtDspzListUFit"        , DspzListUFit_in       ) ;
  if( _fChain->GetBranch("EvtDsenergyListUFit")    ) _fChain->SetBranchAddress("EvtDsenergyListUFit"    , DsenergyListUFit_in   ) ;

  cout << "Done linking DsToKKs Chain Branches" << endl ;
 
  return SetDsInclusiveBranches() ;
}

Int_t DRecoDsToKKsAnalysis::MakeDsToKKsReducedNtupleBranches()
{
  ReducedNtuple->Branch("KBDTNotAPion"      , &KBDTNotAPion        , "KBDTNotAPion/I"     ) ; 
  ReducedNtuple->Branch("KBDTVeryLoose"     , &KBDTVeryLoose       , "KBDTVeryLoose/I"    ) ; 
  ReducedNtuple->Branch("KBDTLoose"         , &KBDTLoose           , "KBDTLoose/I"        ) ; 
  ReducedNtuple->Branch("KBDTTight"         , &KBDTTight           , "KBDTTight/I"        ) ; 
  ReducedNtuple->Branch("KBDTVeryTight"     , &KBDTVeryTight       , "KBDTVeryTight/I"    ) ; 

  ReducedNtuple->Branch("KsVtxStatus"       , &KsVtxStatus         , "KsVtxStatus/I"      ) ; 

  ReducedNtuple->Branch("probchi2MMCFit"    , &EvtProbChi2MMCFit   , "probchi2MMCFit/F"   ) ; 
  ReducedNtuple->Branch("probchi2ListUFit"  , &EvtProbChi2ListUFit , "probchi2ListUFit/F" ) ; 
  ReducedNtuple->Branch("probchi2ListCFit"  , &EvtProbChi2ListCFit , "probchi2ListCFit/F" ) ;
  ReducedNtuple->Branch("eextra"            , &EExtra              , "eextra/F"           ) ; 

  ReducedNtuple->Branch("FitStatusCFit"     , &EvtFitStatusCFit    , "FitStatusCFit/I"    ) ; 
  ReducedNtuple->Branch("FitStatusUFit"     , &EvtFitStatusUFit    , "FitStatusUFit/I"    ) ; 

  ReducedNtuple->Branch("kpxMMRaw"          , &KpxMMRaw            , "kpxMMRaw/F"         ) ;
  ReducedNtuple->Branch("kpyMMRaw"          , &KpyMMRaw            , "kpyMMRaw/F"         ) ;
  ReducedNtuple->Branch("kpzMMRaw"          , &KpzMMRaw            , "kpzMMRaw/F"         ) ;
  ReducedNtuple->Branch("kenergyMMRaw"      , &KenergyMMRaw        , "kenergyMMRaw/F"     ) ;
  ReducedNtuple->Branch("kspxMMRaw"         , &KspxMMRaw           , "kspxMMRaw/F"        ) ;
  ReducedNtuple->Branch("kspyMMRaw"         , &KspyMMRaw           , "kspyMMRaw/F"        ) ;
  ReducedNtuple->Branch("kspzMMRaw"         , &KspzMMRaw           , "kspzMMRaw/F"        ) ;
  ReducedNtuple->Branch("ksenergyMMRaw"     , &KsenergyMMRaw       , "ksenergyMMRaw/F"    ) ;
  ReducedNtuple->Branch("ksmassMMRaw2"      , &KsmassMMRaw2        , "ksmassMMRaw2/F"     ) ;
  ReducedNtuple->Branch("dspxMMRaw"         , &DspxMMRaw           , "dspxMMRaw/F"        ) ;
  ReducedNtuple->Branch("dspyMMRaw"         , &DspyMMRaw           , "dspyMMRaw/F"        ) ;
  ReducedNtuple->Branch("dspzMMRaw"         , &DspzMMRaw           , "dspzMMRaw/F"        ) ;
  ReducedNtuple->Branch("dsenergyMMRaw"     , &DsenergyMMRaw       , "dsenergyMMRaw/F"    ) ;
  ReducedNtuple->Branch("dsmassMMRaw"       , &DsmassMMRaw         , "dsmassMMRaw/F"      ) ;

  ReducedNtuple->Branch("kpxMMCFit"         , &KpxMMCFit            , "kpxMMCFit/F"       ) ;
  ReducedNtuple->Branch("kpyMMCFit"         , &KpyMMCFit            , "kpyMMCFit/F"       ) ;
  ReducedNtuple->Branch("kpzMMCFit"         , &KpzMMCFit            , "kpzMMCFit/F"       ) ;
  ReducedNtuple->Branch("kenergyMMCFit"     , &KenergyMMCFit        , "kenergyMMCFit/F"   ) ;
  ReducedNtuple->Branch("kspxMMCFit"        , &KspxMMCFit           , "kspxMMCFit/F"      ) ;
  ReducedNtuple->Branch("kspyMMCFit"        , &KspyMMCFit           , "kspyMMCFit/F"      ) ;
  ReducedNtuple->Branch("kspzMMCFit"        , &KspzMMCFit           , "kspzMMCFit/F"      ) ;
  ReducedNtuple->Branch("ksenergyMMCFit"    , &KsenergyMMCFit       , "ksenergyMMCFit/F"  ) ;
  ReducedNtuple->Branch("ksmassMMCFit2"     , &KsmassMMCFit2        , "ksmassMMCFit2/F"   ) ;
  ReducedNtuple->Branch("dspxMMCFit"        , &DspxMMCFit           , "dspxMMCFit/F"      ) ;
  ReducedNtuple->Branch("dspyMMCFit"        , &DspyMMCFit           , "dspyMMCFit/F"      ) ;
  ReducedNtuple->Branch("dspzMMCFit"        , &DspzMMCFit           , "dspzMMCFit/F"      ) ;
  ReducedNtuple->Branch("dsenergyMMCFit"    , &DsenergyMMCFit       , "dsenergyMMCFit/F"  ) ;
  ReducedNtuple->Branch("dsmassMMCFit2"     , &DsmassMMCFit2        , "dsmassMMCFit2/F"   ) ;

  ReducedNtuple->Branch("kspxListRaw"       , &KspxListRaw          , "kspxListRaw/F"      ) ;
  ReducedNtuple->Branch("kspyListRaw"       , &KspyListRaw          , "kspyListRaw/F"      ) ;
  ReducedNtuple->Branch("kspzListRaw"       , &KspzListRaw          , "kspzListRaw/F"      ) ;
  ReducedNtuple->Branch("ksenergyListRaw"   , &KsenergyListRaw      , "ksenergyListRaw/F"  ) ;
  ReducedNtuple->Branch("ksmassListRaw2"    , &KsmassListRaw2       , "ksmassListRaw2/F"   ) ;           
  ReducedNtuple->Branch("ksd1pxListRaw"     , &Ksd1pxListRaw        , "ksd1pxListRaw/F"    ) ;
  ReducedNtuple->Branch("ksd1pyListRaw"     , &Ksd1pyListRaw        , "ksd1pyListRaw/F"    ) ;
  ReducedNtuple->Branch("ksd1pzListRaw"     , &Ksd1pzListRaw        , "ksd1pzListRaw/F"    ) ;
  ReducedNtuple->Branch("ksd1energyListRaw" , &Ksd1energyListRaw    , "ksd1energyListRaw/F") ;       
  ReducedNtuple->Branch("ksd2pxListRaw"     , &Ksd2pxListRaw        , "ksd2pxListRaw/F"    ) ;
  ReducedNtuple->Branch("ksd2pyListRaw"     , &Ksd2pyListRaw        , "ksd2pyListRaw/F"    ) ;
  ReducedNtuple->Branch("ksd2pzListRaw"     , &Ksd2pzListRaw        , "ksd2pzListRaw/F"    ) ;
  ReducedNtuple->Branch("ksrd2energyListRaw", &Ksd2energyListRaw    , "ksd2energyListRaw/F") ;		   
  ReducedNtuple->Branch("dspxListRaw"       , &DspxListRaw          , "dspxListRaw/F"      ) ;
  ReducedNtuple->Branch("dspyListRaw"       , &DspyListRaw          , "dspyListRaw/F"      ) ;
  ReducedNtuple->Branch("dspzListRaw"       , &DspzListRaw          , "dspzListRaw/F"      ) ;
  ReducedNtuple->Branch("dsenergyListRaw"   , &DsenergyListRaw      , "dsenergyListRaw/F"  ) ;
  ReducedNtuple->Branch("dsmassListRaw2"    , &DsmassListRaw2       , "dsmassListRaw2/F"   ) ;
  ReducedNtuple->Branch("kpxListRaw"        , &KpxListRaw           , "kpxListRaw/F"       ) ;
  ReducedNtuple->Branch("kpyListRaw"        , &KpyListRaw           , "kpyListRaw/F"       ) ;
  ReducedNtuple->Branch("kpzListRaw"        , &KpzListRaw           , "kpzListRaw/F"       ) ;
  ReducedNtuple->Branch("kenergyListRaw"    , &KenergyListRaw       , "kenergyListRaw/F"   ) ;

  ReducedNtuple->Branch("kspxListUFit"       , &KspxListUFit        , "kspxListUFit/F"      ) ;
  ReducedNtuple->Branch("kspyListUFit"       , &KspyListUFit        , "kspyListUFit/F"      ) ;
  ReducedNtuple->Branch("kspzListUFit"       , &KspzListUFit        , "kspzListUFit/F"      ) ;
  ReducedNtuple->Branch("ksenergyListUFit"   , &KsenergyListUFit    , "ksenergyListUFit/F"  ) ;
  ReducedNtuple->Branch("ksmassListUFit2"    , &KsmassListUFit2     , "ksmassListUFit2/F"   ) ;            
  ReducedNtuple->Branch("ksd1pxListUFit"     , &Ksd1pxListUFit      , "ksd1pxListUFit/F"    ) ;
  ReducedNtuple->Branch("ksd1pyListUFit"     , &Ksd1pyListUFit      , "ksd1pyListUFit/F"    ) ;
  ReducedNtuple->Branch("ksd1pzListUFit"     , &Ksd1pzListUFit      , "ksd1pzListUFit/F"    ) ;
  ReducedNtuple->Branch("ksd1energyListUFit" , &Ksd1energyListUFit  , "ksd1energyListUFit/F") ;    
  ReducedNtuple->Branch("ksd2pxListUFit"     , &Ksd2pxListUFit      , "ksd2pxListUFit/F"    ) ;
  ReducedNtuple->Branch("ksd2pyListUFit"     , &Ksd2pyListUFit      , "ksd2pyListUFit/F"    ) ;
  ReducedNtuple->Branch("ksd2pzListUFit"     , &Ksd2pzListUFit      , "ksd2pzListUFit/F"    ) ;
  ReducedNtuple->Branch("ksrd2energyListUFit", &Ksd2energyListUFit  , "ksd2energyListUFit/F") ;		   
  ReducedNtuple->Branch("dspxListUFit"       , &DspxListUFit        , "dspxListUFit/F"      ) ;
  ReducedNtuple->Branch("dspyListUFit"       , &DspyListUFit        , "dspyListUFit/F"      ) ;
  ReducedNtuple->Branch("dspzListUFit"       , &DspzListUFit        , "dspzListUFit/F"      ) ;
  ReducedNtuple->Branch("dsenergyListUFit"   , &DsenergyListUFit    , "dsenergyListUFit/F"  ) ;
  ReducedNtuple->Branch("dsmassListUFit2"    , &DsmassListUFit2     , "dsmassListUFit2/F"   ) ;
  ReducedNtuple->Branch("kpxListUFit"        , &KpxListUFit         , "kpxListUFit/F"       ) ;
  ReducedNtuple->Branch("kpyListUFit"        , &KpyListUFit         , "kpyListUFit/F"       ) ;
  ReducedNtuple->Branch("kpzListUFit"        , &KpzListUFit         , "kpzListUFit/F"       ) ;
  ReducedNtuple->Branch("kenergyListUFit"    , &KenergyListUFit     , "kenergyListUFit/F"   ) ;

  ReducedNtuple->Branch("kspxListCFit"       , &KspxListCFit        , "kspxListCFit/F"      ) ;
  ReducedNtuple->Branch("kspyListCFit"       , &KspyListCFit        , "kspyListCFit/F"      ) ;
  ReducedNtuple->Branch("kspzListCFit"       , &KspzListCFit        , "kspzListCFit/F"      ) ;
  ReducedNtuple->Branch("ksenergyListCFit"   , &KsenergyListCFit    , "ksenergyListCFit/F"  ) ;
  ReducedNtuple->Branch("ksmassListCFit2"    , &KsmassListCFit2     , "ksmassListCFit2/F"   ) ;            
  ReducedNtuple->Branch("ksd1pxListCFit"     , &Ksd1pxListCFit      , "ksd1pxListCFit/F"    ) ;
  ReducedNtuple->Branch("ksd1pyListCFit"     , &Ksd1pyListCFit      , "ksd1pyListCFit/F"    ) ;
  ReducedNtuple->Branch("ksd1pzListCFit"     , &Ksd1pzListCFit      , "ksd1pzListCFit/F"    ) ;
  ReducedNtuple->Branch("ksd1energyListCFit" , &Ksd1energyListCFit  , "ksd1energyListCFit/F") ;    
  ReducedNtuple->Branch("ksd2pxListCFit"     , &Ksd2pxListCFit      , "ksd2pxListCFit/F"    ) ;
  ReducedNtuple->Branch("ksd2pyListCFit"     , &Ksd2pyListCFit      , "ksd2pyListCFit/F"    ) ;
  ReducedNtuple->Branch("ksd2pzListCFit"     , &Ksd2pzListCFit      , "ksd2pzListCFit/F"    ) ;
  ReducedNtuple->Branch("ksrd2energyListCFit", &Ksd2energyListCFit  , "ksd2energyListCFit/F") ;		   
  ReducedNtuple->Branch("dspxListCFit"       , &DspxListCFit        , "dspxListCFit/F"      ) ;
  ReducedNtuple->Branch("dspyListCFit"       , &DspyListCFit        , "dspyListCFit/F"      ) ;
  ReducedNtuple->Branch("dspzListCFit"       , &DspzListCFit        , "dspzListCFit/F"      ) ;
  ReducedNtuple->Branch("dsenergyListCFit"   , &DsenergyListCFit    , "dsenergyListCFit/F"  ) ;
  ReducedNtuple->Branch("dsmassListCFit2"    , &DsmassListCFit2     , "dsmassListCFit2/F"   ) ;
  ReducedNtuple->Branch("kpxListCFit"        , &KpxListCFit         , "kpxListCFit/F"       ) ;
  ReducedNtuple->Branch("kpyListCFit"        , &KpyListCFit         , "kpyListCFit/F"       ) ;
  ReducedNtuple->Branch("kpzListCFit"        , &KpzListCFit         , "kpzListCFit/F"       ) ;
  ReducedNtuple->Branch("kenergyListCFit"    , &KenergyListCFit     , "kenergyListCFit/F"   ) ;

  ReducedNtuple->Branch("kselectorsmap"      , &kselectorsmap       , "kselectorsmap/I"     ) ;
  ReducedNtuple->Branch("goodeventid"        , &goodeventid         , "goodeventid/I"       ) ;
  ReducedNtuple->Branch("truemothlund"       , &truemothlund        , "truemothlund/I"      ) ;
  ReducedNtuple->Branch("truemothndaus"      , &truemothndaus       , "truemothndaus/I"     ) ;
  ReducedNtuple->Branch("truegrandmothlund"  , &truegrandmothlund   , "truegrandmothlund/I" ) ;
  ReducedNtuple->Branch("candkslund"         , &candkslund          , "candkslund/I"        ) ;
  ReducedNtuple->Branch("candklund"          , &candklund           , "candklund/I"         ) ;
  ReducedNtuple->Branch("truesib1lund"       , &truesib1lund        , "truesib1lund/I"      ) ;
  ReducedNtuple->Branch("truesib2lund"       , &truesib2lund        , "truesib2lund/I"      ) ;
  ReducedNtuple->Branch("truesib3lund"       , &truesib3lund        , "truesib3lund/I"      ) ;
  ReducedNtuple->Branch("truesib4lund"       , &truesib4lund        , "truesib4lund/I"      ) ;
  ReducedNtuple->Branch("truesib5lund"       , &truesib5lund        , "truesib5lund/I"      ) ;

  MakeDsInclusiveReducedNtupleBranches();
  //MakeDsToKKsReducedNtupleBranches() ;
  cout << "Done Making DsToKKs Reduced Ntuple Branches" << endl ;
  return 1 ;
}

