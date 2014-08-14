#include "DRecoilToolsUser/NtupleCode/DRecoDsToEllNuAnalysis.hh"

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

ClassImp(DRecoDsToEllNuAnalysis) ;
DRecoDsToEllNuAnalysis::DRecoDsToEllNuAnalysis(Int_t DataType, TString OutputDir, Int_t theSPMode, Int_t theRun):
  DRecoDsInclusiveAnalysis(DataType,OutputDir)
{
  blind[0] = kFALSE ;
  blind[1] = kFALSE ;
  blind[2] = kFALSE ;
  blind[3] = kFALSE ;
  blind[4] = kTRUE  ;
  blind[5] = kFALSE ;
  run    = theRun ;
  SPMode = theSPMode ;
  //cout << DsMassMin << " " << DsMassMax << endl ;

  //                OnPeak                   OffPeak
  luminosity[0]  =  20.595 ; luminosity[1]  =  2.621 ;
  luminosity[2]  =  62.066 ; luminosity[3]  =  7.030 ;
  luminosity[4]  =  32.668 ; luminosity[5]  =  2.495 ;
  luminosity[6]  = 100.776 ; luminosity[7]  = 10.228 ;
  luminosity[8]  = 133.854 ; luminosity[9]  = 14.546 ;
  luminosity[10] =  79.030 ; luminosity[11] =  7.886 ;
  luminosity[12] =  12.954 ; luminosity[13] =  1.511 ;
  luminosity[14] =  28.494 ; luminosity[15] =  2.657 ;
  // Total           425.8                      44.4
  // Grand total     470.2
  luminosity[16] =     0.0 ;

  for(Int_t i=0 ; i<NRuns ; i++)
  {
    for(Int_t j=0 ; j<NSPModes ; j++)
    {
      weight[i][j] = 1.0 ;
    }
  }

  // Use weights taken from the BAD, for consistency (and because I'm lazy)
  weight[0][0]  = 0.52 ;
  weight[1][0]  = 0.41 ;
  weight[2][0]  = 0.72 ;
  weight[3][0]  = 0.69 ;
  weight[4][0]  = 1.01 ;
  weight[5][0]  = 2.02 ;
  weight[6][0]  = 1.00 ;
  weight[7][0]  = 0.80 ;
  weight[8][0]  = 0.99 ;
  weight[9][0]  = 1.00 ;
  weight[10][0] = 0.99 ;
  weight[11][0] = 1.00 ;
  weight[12][0] = 0.50 ;
  weight[13][0] = 0.50 ;
  weight[14][0] = 0.57 ;
  weight[15][0] = 0.58 ;

  weight[0][1]  = 2.06 ;
  weight[1][1]  = 1.65 ;
  weight[2][1]  = 2.04 ;
  weight[3][1]  = 1.93 ;
  weight[4][1]  = 2.08 ;
  weight[5][1]  = 2.01 ;
  weight[6][1]  = 2.04 ;
  weight[7][1]  = 1.61 ;
  weight[8][1]  = 1.98 ;
  weight[9][1]  = 2.01 ;
  weight[10][1] = 2.03 ;
  weight[11][1] = 2.00 ;
  weight[12][1] = 2.01 ;
  weight[13][1] = 2.00 ;
  weight[14][1] = 2.39 ;
  weight[15][1] = 2.81 ;

  weight[0][2]  = 0.54 ;
  weight[1][2]  = 0.00 ;
  weight[2][2]  = 0.54 ;
  weight[3][2]  = 0.00 ;
  weight[4][2]  = 0.54 ;
  weight[5][2]  = 0.00 ;
  weight[6][2]  = 3.15 ;
  weight[7][2]  = 0.00 ;
  weight[8][2]  = 0.51 ;
  weight[9][2]  = 0.00 ;
  weight[10][2] = 0.52 ;
  weight[11][2] = 0.00 ;
  weight[12][2] = 0.00 ;
  weight[13][2] = 0.00 ;
  weight[14][2] = 0.00 ;
  weight[15][2] = 0.00 ;

  weight[0][3]  = 0.54 ;
  weight[1][3]  = 0.00 ;
  weight[2][3]  = 0.53 ;
  weight[3][3]  = 0.00 ;
  weight[4][3]  = 0.56 ;
  weight[5][3]  = 0.00 ;
  weight[6][3]  = 3.21 ;
  weight[7][3]  = 0.00 ;
  weight[8][3]  = 0.51 ;
  weight[9][3]  = 0.00 ;
  weight[10][3] = 0.54 ;
  weight[11][3] = 0.00 ;
  weight[12][3] = 0.00 ;
  weight[13][3] = 0.00 ;
  weight[14][3] = 0.00 ;
  weight[15][3] = 0.00 ;

  weight[0][4]  = 1.02 ;
  weight[1][4]  = 0.79 ;
  weight[2][4]  = 0.98 ;
  weight[3][4]  = 1.04 ;
  weight[4][4]  = 1.59 ;
  weight[5][4]  = 3.83 ;
  weight[6][4]  = 1.90 ;
  weight[7][4]  = 1.51 ;
  weight[8][4]  = 1.88 ;
  weight[9][4]  = 1.89 ;
  weight[10][4] = 1.88 ;
  weight[11][4] = 1.89 ;
  weight[12][4] = 0.90 ;
  weight[13][4] = 0.90 ;
  weight[14][4] = 1.08 ;
  weight[15][4] = 1.27 ;

  for(Int_t i=0 ; i<NRuns ; i++)
  {
    for(Int_t j=0 ; j<NSPModes ; j++)
    {
      weight[i][j] = 1.0/weight[i][j] ;
    }
  }

  cout << NSPModes << endl ;
  cout << theRun << " " << theSPMode << endl ;
  cout << "Weight: " << weight[theRun][theSPMode] << endl ;
  cout << "Done Initializing the DRecoDsToEllNuAnalysis analysis" << endl ;
}

DRecoDsToEllNuAnalysis::~DRecoDsToEllNuAnalysis()
{ 
}

Bool_t DRecoDsToEllNuAnalysis::DsToEllNuPassesCuts()
{
  Bool_t pass = kTRUE ;
  if(!DsEvtPassesCuts())    pass = kFALSE ;
  if(!DsPassesCuts())       pass = kFALSE ;
  if(nExtraTracks!=0)       pass = kFALSE ;
  if(mm2<-1)                pass = kFALSE ;
  if(mm2>4)                 pass = kFALSE ;
  if(SPMode==10 && blind[signalMode]==true && mm2<0.2 && mm2>-0.2 && signalMode<2) pass = kFALSE ;
  if(SPMode==10 && blind[signalMode]==true && eExtraNeutral<1     && signalMode>1) pass = kFALSE ;
  return pass ;
}

void DRecoDsToEllNuAnalysis::PrintDaughters(Int_t Idx, Int_t depth)
{
  for(Int_t i=dauIdx[Idx] ; i<dauIdx[Idx]+dauLen[Idx] ; i++)
  {
    cout.width(depth+2) ;
    cout << "" ;
    cout << mcLund[i] ;
    cout << endl ;
    if(dauLen[i]!=0) PrintDaughters(i,depth+2) ;
  }
}

Int_t DRecoDsToEllNuAnalysis::ReduceNtuple()
{
  cout<<"Going to reduce the ntuple"<<endl ;

  if(_fChain==NULL)
  {
    cout << "No chain." << endl ;
    return 0 ;
  } 

  // Link the branches in the input chain
  SetDsToEllNuBranches() ;

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
  Long64_t maxsize   = 1000000000 ; // 10Gb
  Long64_t fautosave = 1100000000 ; // 11Gb !! // Disable autosaving
  ReducedNtuple->SetMaxTreeSize(maxsize) ; // Split files
  ReducedNtuple->SetAutoSave(fautosave) ;
  
  // Create the branches in the Ntuple
  MakeDsToEllNuReducedNtupleBranches() ;
  
  // Counters
  Int_t TotalSaved = 0 ;

  cout << "Total Entries in Chain = " << _fChain->GetEntries() << endl ;
  cout << "Going to start the event loop" << endl ;
  eventid = 0 ;
  Int_t ngoodthisevent = 0 ;
  Int_t TotalRawCands = 0 ;
  goodeventid = 0 ;
  while(_fChain->GetEntry(eventid,0)>0)
  {
    eventid++ ;
    if(eventid%1000==0) cout << eventid << " events done.  With " << TotalSaved << " Candidates saved." << endl ;
 
    /// Event vars
    if(FillDsEventVars()==0) cout << "FillDsEventVars error in event " << eventid << endl  ;
    TotalRawCands += nEvt ;

    ngoodthisevent=0 ;
    if(nEvt>MAXNCANDS) nEvt = MAXNCANDS ; // Protect array size
    if(!DsEvtPassesCuts()) continue ;
    if(false)
    {
      cout << endl << endl ;
      PrintDaughters(2,0) ;
    }
    
    // Now do real loop
    for(EvtIdx=0 ; EvtIdx<nEvt ; EvtIdx++)
    {
      FillDsToEllNuVars() ;
      if(DsToEllNuPassesCuts()==kFALSE) continue ;

      if(HasDsToTauNu==1&&HasDsToTauNuTauToMuNuNu==1&&signalMode==2&&false)
      {
	cout << endl << endl ;
	PrintDaughters(2,0) ;
	cout << "HasDsToMuNu:                 " << HasDsToMuNu                 << endl ;
	cout << "HasDsToENu:                  " << HasDsToENu                  << endl ;
	cout << "HasDsToTauNu:                " << HasDsToTauNu                << endl ;
	cout << "HasDsToTauNuTauToMuNuNu:     " << HasDsToTauNuTauToMuNuNu     << endl ;
	cout << "HasDsToTauNuTauToENuNu:      " << HasDsToTauNuTauToENuNu      << endl ;
	cout << "HasDsToTauNuTauToPiNu:       " << HasDsToTauNuTauToPiNu       << endl ;
	cout << "HasDsToTauNuTauToPiPi0Nu:    " << HasDsToTauNuTauToPiPi0Nu    << endl ;
	cout << "HasDsToTauNuTauToPiPi0Pi0Nu: " << HasDsToTauNuTauToPiPi0Pi0Nu << endl ;
	cout << "HasDsToTauNuTauToRhoNu:      " << HasDsToTauNuTauToRhoNu      << endl ;
	cout << "HasDsToTauNuTauToOmegaPiNu:  " << HasDsToTauNuTauToOmegaPiNu  << endl ;
	cout << endl << endl ;
      }

      
      // Save
      ngoodthisevent++ ;
      TotalSaved++ ;
      ReducedNtuple->Fill() ;
    }// loop
    if(ngoodthisevent>0) goodeventid++ ;
  }

  // Print summary
  cout << "--------Summary-------"                  << endl ;
  cout << "  Total raw cands = "   << TotalRawCands << endl ;
  cout << "     Total events = "   << eventid       << endl ;
  cout << "Total cands saved = "   << TotalSaved    << endl ;
  cout << "--------End Summary---"                  << endl ;

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


void DRecoDsToEllNuAnalysis::FillDsToEllNuVars()
{
  //cout << EvtIdx << endl ;
  //cout << eventid << " " << runNumber << endl << endl ;
  if(EvtIdx>=MAXNCANDS) return ;
  // Fill inclusive fit variables
  //cout << "TagLund: " << Evtd1Lund[EvtIdx] << " " << Evtd1Idx[EvtIdx] << " " << TagLund[0] << " " << endl ;
  if(FillDsInclusiveVars()==0) cout << "FillDsInclusiveVars error in event " << eventid << " " << EvtIdx << endl  ;
  if(false)
  {
    cout << "Evt, Tag, Ds: (" << EvtIdx << " " << nEvt << ") (" << TagIdx << " " << nTag << ") (" << DsIdx << " " << nDs << ")" << endl ;
    cout << "TRK, Pi, K, Mu, E, P: " << nTRK << " " << nPi << " " << nK << " " << nMu << " " << nE << " "  << nP << endl ;
    cout << "TagLund: " << Evtd1Lund[EvtIdx] << endl ;
    cout << "eventid: " << eventid << " " << upperID << " " << lowerID << endl << endl ;
  }

  signalMode = SignalMode[EvtIdx] ;
  if(signalMode>4) return ;
  // Neutrino is first daughter, ell is second daughter
  Int_t NuIdx  = Dsd1Idx[DsIdx] ;
  EllLund = Dsd2Lund[DsIdx] ;
  
  Int_t MuIdx = -999 ;
  Int_t  EIdx = -999 ;
  Int_t PiIdx = -999 ;
  Int_t otherEllLund = -999 ;
  if(signalMode==0||signalMode==2)
  {
    MuIdx = Dsd2Idx[DsIdx] ;
    otherEllLund = 11 ;
  }
  else if(signalMode==1||signalMode==3)
  {
    EIdx  = Dsd2Idx[DsIdx] ;
    otherEllLund = 13 ;
  }
  else if(signalMode==4)
  {
    PiIdx = Dsd2Idx[DsIdx] ;
  }
  
  DsStpxCM = DsStp3CM[DsStIdx]*sin(acos(DsStcosthCM[DsStIdx]))*cos(DsStphiCM[DsStIdx]) ;
  DsStpyCM = DsStp3CM[DsStIdx]*sin(acos(DsStcosthCM[DsStIdx]))*sin(DsStphiCM[DsStIdx]) ;
  DsStpzCM = DsStp3CM[DsStIdx]*DsStcosthCM[DsStIdx] ;
  TVector3 DsStpCM(DsStpxCM, DsStpyCM, DsStpzCM) ;
  
  DspxCM = Dsp3CM[DsIdx]*sin(acos(DscosthCM[DsIdx]))*cos(DsphiCM[DsIdx]) ;
  DspyCM = Dsp3CM[DsIdx]*sin(acos(DscosthCM[DsIdx]))*sin(DsphiCM[DsIdx]) ;
  DspzCM = Dsp3CM[DsIdx]*DscosthCM[DsIdx] ;
  TVector3 DspCM(DspxCM, DspyCM, DspzCM) ;
  
  TagpxCM = Tagp3CM[TagIdx]*sin(acos(TagcosthCM[TagIdx]))*cos(TagphiCM[TagIdx]) ;
  TagpyCM = Tagp3CM[TagIdx]*sin(acos(TagcosthCM[TagIdx]))*sin(TagphiCM[TagIdx]) ;
  TagpzCM = Tagp3CM[TagIdx]*TagcosthCM[TagIdx] ;
  TVector3 TagpCM(TagpxCM, TagpyCM, TagpzCM) ;
  
  GampxCM = Gamp3CM[GamIdx]*sin(acos(GamcosthCM[GamIdx]))*cos(GamphiCM[GamIdx]) ;
  GampyCM = Gamp3CM[GamIdx]*sin(acos(GamcosthCM[GamIdx]))*sin(GamphiCM[GamIdx]) ;
  GampzCM = Gamp3CM[GamIdx]*GamcosthCM[GamIdx] ;
  TVector3 GampCM(GampxCM, GampyCM, GampzCM) ;

  if(abs(Klund)==310 || abs(Klund)==311)
  {
     KpxCM = Ksp3CM[TagKIdx]*sin(acos(KscosthCM[TagKIdx]))*cos(KsphiCM[TagKIdx]) ;
     KpyCM = Ksp3CM[TagKIdx]*sin(acos(KscosthCM[TagKIdx]))*sin(KsphiCM[TagKIdx]) ;
     KpzCM = Ksp3CM[TagKIdx]*KscosthCM[TagKIdx] ;
  }
  if(abs(Klund)==321)
  {
     KpxCM = Kp3CM[TagKIdx]*sin(acos(KcosthCM[TagKIdx]))*cos(KphiCM[TagKIdx]) ;
     KpyCM = Kp3CM[TagKIdx]*sin(acos(KcosthCM[TagKIdx]))*sin(KphiCM[TagKIdx]) ;
     KpzCM = Kp3CM[TagKIdx]*KcosthCM[TagKIdx] ;
  }
  TVector3 KpCM(KpxCM, KpyCM, KpzCM) ;
  
  NupxCM = Nup3CM[NuIdx]*sin(acos(NucosthCM[NuIdx]))*cos(NuphiCM[NuIdx]) ;
  NupyCM = Nup3CM[NuIdx]*sin(acos(NucosthCM[NuIdx]))*sin(NuphiCM[NuIdx]) ;
  NupzCM = Nup3CM[NuIdx]*NucosthCM[NuIdx] ;
  TVector3 NupCM(NupxCM, NupyCM, NupzCM) ;
  NuenergyCM = NuenergyCM_in[NuIdx] ;
  mm2 = NuenergyCM*NuenergyCM - NupxCM*NupxCM - NupyCM*NupyCM - NupzCM*NupzCM ;
  TLorentzVector Nup4CM(NupxCM, NupyCM, NupzCM, NuenergyCM) ;
  
  // Fill reduced ntuple variables
  NupxLabRaw = NupxLabRaw_in[EvtIdx] ;
  NupyLabRaw = NupyLabRaw_in[EvtIdx] ;
  NupzLabRaw = NupzLabRaw_in[EvtIdx] ;
  TVector3 NupLabRaw(NupxLabRaw, NupyLabRaw, NupzLabRaw) ;
  NuenergyLabRaw = NuenergyLabRaw_in[EvtIdx] ;
  mm2Raw =  NuenergyLabRaw*NuenergyLabRaw - NupxLabRaw*NupxLabRaw - NupyLabRaw*NupyLabRaw - NupzLabRaw*NupzLabRaw ;
  
  MupxLabRaw = 0 ;
  MupyLabRaw = 0 ;
  MupzLabRaw = 0 ;
  EpxLabRaw  = 0 ;
  EpyLabRaw  = 0 ;
  EpzLabRaw  = 0 ;

  trackIdx = -999 ;
  TLorentzVector eep4(eePx, eePy, eePz, eeE) ;
  if(signalMode==0||signalMode==2)
  {
    MupxLabRaw     = MupxRaw_in[EvtIdx] ;
    MupyLabRaw     = MupyRaw_in[EvtIdx] ;
    MupzLabRaw     = MupzRaw_in[EvtIdx] ;
    MuenergyLabRaw = MuenergyRaw_in[EvtIdx] ;
    MupxCM = Mup3CM[MuIdx]*sin(acos(MucosthCM[MuIdx]))*cos(MuphiCM[MuIdx]) ;
    MupyCM = Mup3CM[MuIdx]*sin(acos(MucosthCM[MuIdx]))*sin(MuphiCM[MuIdx]) ;
    MupzCM = Mup3CM[MuIdx]*MucosthCM[MuIdx] ;

    Float_t MuECM = sqrt(MupxCM*MupxCM+MupyCM*MupyCM+MupzCM*MupzCM+0.105*0.105) ;
    TLorentzVector Mup4Lab(MupxCM,MupyCM,MupzCM,MuECM) ;
    Mup4Lab.Boost(eep4.BoostVector()) ;
    Ellp3Lab = Mup4Lab.Rho() ;

    trackIdx = MuTrkIdx[MuIdx] ;

    mm2Shift   = mm2 ;
    mm2Stretch = mm2 ;
    Pip3Ds = -1 ;
  }
  else if(signalMode==1||signalMode==3)
  {
    EpxLabRaw     = MupxRaw_in[EvtIdx] ;
    EpyLabRaw     = MupyRaw_in[EvtIdx] ;
    EpzLabRaw     = MupzRaw_in[EvtIdx] ;
    EenergyLabRaw = MuenergyRaw_in[EvtIdx] ;
    EpxCM  = Ep3CM[EIdx]*sin(acos(EcosthCM[EIdx]))*cos(EphiCM[EIdx]) ;
    EpyCM  = Ep3CM[EIdx]*sin(acos(EcosthCM[EIdx]))*sin(EphiCM[EIdx]) ;
    EpzCM  = Ep3CM[EIdx]*EcosthCM[EIdx] ;
    
    Float_t EECM = sqrt(EpxCM*EpxCM+EpyCM*EpyCM+EpzCM*EpzCM+0.000511*0.000511) ;
    TLorentzVector  Ep4CM(EpxCM,EpyCM,EpzCM,EECM) ;
    TLorentzVector  Ep4Lab = Ep4CM ;
    Ep4Lab.Boost(eep4.BoostVector()) ;
    Ellp3Lab = Ep4Lab.Rho() ;
    
    trackIdx = ETrkIdx[EIdx] ;
    
    Float_t scale = (DsPDGMass-MuonPDGMass)/(DsPDGMass-ElectronPDGMass) ;
    Float_t EShift = sqrt(Ep4CM.Rho()*Ep4CM.Rho()+0.105*0.105)-sqrt(Ep4CM.Rho()*Ep4CM.Rho()+0.000511*0.000511) ;
    mm2Shift   = mm2 - NuenergyCM*NuenergyCM + (NuenergyCM-EShift)*(NuenergyCM-EShift) ;
    mm2Stretch = mm2*scale*scale ;
    Pip3Ds = -1 ;
  }
  else if(signalMode==4)
  {
    PipxLabRaw     = MupxRaw_in[EvtIdx] ;
    PipyLabRaw     = MupyRaw_in[EvtIdx] ;
    PipzLabRaw     = MupzRaw_in[EvtIdx] ;
    PienergyLabRaw = MuenergyRaw_in[EvtIdx] ;
    PipxCM  = Pip3CM[PiIdx]*sin(acos(PicosthCM[PiIdx]))*cos(PiphiCM[PiIdx]) ;
    PipyCM  = Pip3CM[PiIdx]*sin(acos(PicosthCM[PiIdx]))*sin(PiphiCM[PiIdx]) ;
    PipzCM  = Pip3CM[PiIdx]*PicosthCM[PiIdx] ;
    
    Float_t PiECM = sqrt(PipxCM*PipxCM+PipyCM*PipyCM+PipzCM*PipzCM+0.139*0.139) ;
    TLorentzVector Pip4CM(PipxCM,PipyCM,PipzCM,PiECM) ;
    TLorentzVector Pip4Lab = Pip4CM ;
    Pip4Lab.Boost(eep4.BoostVector()) ;
    Ellp3Lab = Pip4Lab.Rho() ;
    
    trackIdx = PiTrkIdx[PiIdx] ;
    
    mm2Shift   = mm2 ;
    mm2Stretch = mm2 ;
    TLorentzVector Dsp4CM(DspxCM, DspyCM, DspzCM, sqrt(DspxCM*DspxCM + DspyCM*DspyCM + DspzCM*DspzCM + 1.968*1.968 )     ) ;
    TLorentzVector Pip4Ds = Pip4CM ;
    Pip4Ds.Boost(-Dsp4CM.BoostVector()) ;
    Pip3Ds = Pip4Ds.Rho() ;
  }
  hits = 1 ;
  if(TRKnDchXY[trackIdx]+TRKnDchZ[trackIdx]<12) hits = 0 ;
  if(TRKnSvtXY[trackIdx]+TRKnSvtZ[trackIdx]<4)  hits = 0 ;
  if(TRKnSvtZ[trackIdx]<2)                      hits = 0 ;

  TVector3 MupCM(MupxCM , MupyCM , MupzCM) ;
  TVector3  EpCM( EpxCM ,  EpyCM ,  EpzCM) ;
  TVector3 PipCM(PipxCM , PipyCM , PipzCM) ;
  TLorentzVector Nup4Lab = Nup4CM ;
  Nup4Lab.Boost(eep4.BoostVector()) ;

  thetaDsStDs  = DsStpCM.Angle( DspCM) ;
  thetaDsStMu  = DsStpCM.Angle( MupCM) ;
  thetaDsStE   = DsStpCM.Angle(  EpCM) ;
  thetaDsStPi  = DsStpCM.Angle( PipCM) ;
  thetaDsStNu  = DsStpCM.Angle( NupCM) ;
  thetaDsStGam = DsStpCM.Angle(GampCM) ;
  thetaDsMu    =   DspCM.Angle( MupCM) ;
  thetaDsE     =   DspCM.Angle(  EpCM) ;
  thetaDsPi    =   DspCM.Angle( PipCM) ;
  thetaDsNu    =   DspCM.Angle( NupCM) ;
  thetaDsGam   =   DspCM.Angle(GampCM) ;
  thetaMuNu    =   MupCM.Angle( NupCM) ;
  thetaMuGam   =   MupCM.Angle(GampCM) ;
  thetaENu     =    EpCM.Angle( NupCM) ;
  thetaEGam    =    EpCM.Angle(GampCM) ;
  thetaPiNu    =   PipCM.Angle( NupCM) ;
  thetaPiGam   =   PipCM.Angle(GampCM) ;
  thetaNuGam   =   NupCM.Angle(GampCM) ;
  thetaDsStTag = DsStpCM.Angle(TagpCM) ;
  thetaDsStK   = DsStpCM.Angle(  KpCM) ;
  thetaDsTag   =   DspCM.Angle(TagpCM) ;
  thetaDsK     =   DspCM.Angle(  KpCM) ;
  thetaMuTag   =   MupCM.Angle(TagpCM) ;
  thetaMuK     =   MupCM.Angle(  KpCM) ;
  thetaETag    =    EpCM.Angle(TagpCM) ;
  thetaEK      =    EpCM.Angle(  KpCM) ;
  thetaPiTag   =   PipCM.Angle(TagpCM) ;
  thetaPiK     =   PipCM.Angle(  KpCM) ;
  
  NupxLabCorr = NupxLabCorr_in[EvtIdx] ;
  NupyLabCorr = NupyLabCorr_in[EvtIdx] ;
  NupzLabCorr = NupzLabCorr_in[EvtIdx] ;
  TVector3 NupLabCorr(NupxLabCorr, NupyLabCorr, NupzLabCorr) ;
  
  deltap3      = Deltap3[EvtIdx] ;
  deltap3Joerg = NupLabRaw.Mag() - NupLabCorr.Mag() ;  
  TVector3 NupLab = Nup4Lab.P() ;
  TVector3 deltap3FitVector = NupLabRaw - NupLab ;
  deltap3Fit      = deltap3FitVector.Mag() ;
  deltap3FitJoerg = NupLabRaw.Mag() - NupLab.Mag() ;

  thrustX       =       ThrustX[EvtIdx] ;
  thrustY       =       ThrustY[EvtIdx] ;
  thrustZ       =       ThrustZ[EvtIdx] ;
  thrustMag     =     ThrustMag[EvtIdx] ;
  r0            =            R0[EvtIdx] ;
  r1            =            R1[EvtIdx] ;
  r2            =            R2[EvtIdx] ;
  r3            =            R3[EvtIdx] ;
  r4            =            R4[EvtIdx] ;
  eExtraNeutral = EExtraNeutral[EvtIdx] ;
  nExtraNeutral = NExtraNeutral[EvtIdx] ;
  pMissCosTh    =    PMissCosTh[EvtIdx] ;
  pMissPhi      =      PMissPhi[EvtIdx] ;
  pMiss         =         PMiss[EvtIdx] ;
  eMiss         =         EMiss[EvtIdx] ;
  evtDsMass     =     EvtDsMass[EvtIdx] ;
  probChi2      =      ProbChi2[EvtIdx] ;
  chi2          =          Chi2[EvtIdx] ;
  ndof          =          Ndof[EvtIdx] ;

  pDiff = pMiss - NupLab.Mag() ;

  // Make a cone around the neutrino and look for photons/tracks in the way
  // Add the p4 of anything except the prong and signal photon
  Float_t psi = 3.141592654/2.0 ;
  coneEnergy = 0 ;
  for(Int_t i=0 ; i<nGam ; i++)
  {
    if(i==GamIdx) continue ;
    Float_t GampxCM = Gamp3CM[i]*sin(acos(GamcosthCM[i]))*cos(GamphiCM[i]) ;
    Float_t GampyCM = Gamp3CM[i]*sin(acos(GamcosthCM[i]))*sin(GamphiCM[i]) ;
    Float_t GampzCM = Gamp3CM[i]*GamcosthCM[i] ;
    TLorentzVector gammap4Lab(GampxCM,GampyCM,GampzCM,Gamp3CM[i]) ;
    gammap4Lab.Boost(eep4.BoostVector()) ;
    if(gammap4Lab.Angle(NupLab)<=psi) coneEnergy+=gammap4Lab.E() ;
  }

  // Extra tracks
  nExtraTracks = NExtraTracks[EvtIdx] ;
  extra1px     = Extra1px[EvtIdx] ;
  extra1py     = Extra1py[EvtIdx] ;
  extra1pz     = Extra1pz[EvtIdx] ;
  extra2px     = Extra2px[EvtIdx] ;
  extra2py     = Extra2py[EvtIdx] ;
  extra2pz     = Extra2pz[EvtIdx] ;
  extra1p3     = 0 ;
  extra2p3     = 0 ;
  extraMPiPi   = 0 ;
  extraMPiK    = 0 ;
  extraMKPi    = 0 ;
  extraMKK     = 0 ;
  if(nExtraTracks==2)
  {
    extra1p3 = sqrt(extra1px*extra1px+extra1py*extra1py+extra1pz*extra1pz) ;
    extra2p3 = sqrt(extra2px*extra2px+extra2py*extra2py+extra2pz*extra2pz) ;
    Float_t extra1EPi = sqrt(extra1p3*extra1p3+0.139*0.139) ;
    Float_t extra2EPi = sqrt(extra2p3*extra2p3+0.139*0.139) ;
    Float_t extra1EK  = sqrt(extra1p3*extra1p3+0.494*0.494) ;
    Float_t extra2EK  = sqrt(extra2p3*extra2p3+0.494*0.494) ;
    Float_t extrap2   = (extra1px+extra2px)*(extra1px+extra2px)-(extra1py+extra2py)*(extra1py+extra2py)-(extra1pz+extra2pz)*(extra1pz+extra2pz) ;
    extraMPiPi = (extra1EPi+extra2EPi)*(extra1EPi+extra2EPi)-extrap2 ;
    extraMPiK  = (extra1EPi+extra2EK )*(extra1EPi+extra2EK )-extrap2 ;
    extraMKPi  = (extra1EK +extra2EPi)*(extra1EK +extra2EPi)-extrap2 ;
    extraMKK   = (extra1EK +extra2EK )*(extra1EK +extra2EK )-extrap2 ;
  }

  // PID information
  MuSelector = MuSelectorsMap[trackIdx] ;
  ESelector  =  ESelectorsMap[trackIdx] ;
  KSelector  =  KSelectorsMap[trackIdx] ;
  PiSelector = piSelectorsMap[trackIdx] ;
  
  MuMinimumIonizing      = -999 ;
  MuVeryLoose            = -999 ;
  MuLoose                = -999 ;
  MuTight                = -999 ;
  MuVeryTight            = -999 ;
  MuNNVeryLoose          = -999 ;
  MuNNLoose              = -999 ;
  MuNNTight              = -999 ;
  MuNNVeryTight          = -999 ;
  MuNNVeryLooseFakeRate  = -999 ;
  MuNNLooseFakeRate      = -999 ;
  MuNNTightFakeRate      = -999 ;
  MuNNVeryTightFakeRate  = -999 ;
  MuLikeVeryLoose        = -999 ;
  MuLikeLoose            = -999 ;
  MuLikeTight            = -999 ;
  MuBDTVeryLoose         = -999 ;
  MuBDTLoose             = -999 ;
  MuBDTTight             = -999 ;
  MuBDTVeryTight         = -999 ;
  MuBDTVeryLooseFakeRate = -999 ;
  MuBDTLooseFakeRate     = -999 ;
  MuBDTTightFakeRate     = -999 ;
  MuBDTVeryTightFakeRate = -999 ;
  MuBDTLoPLoose          = -999 ;
  MuBDTLoPTight          = -999 ;
  
  ENoCal                 = -999 ;
  EVeryLoose             = -999 ;
  ELoose                 = -999 ;
  ETight                 = -999 ;
  EVeryTight             = -999 ;
  PidLHElectrons         = -999 ;
  EKMSuperLoose          = -999 ;
  EKMVeryLoose           = -999 ;
  EKMLoose               = -999 ;
  EKMTight               = -999 ;
  EKMVeryTight           = -999 ;
  EKMSuperTight          = -999 ;
  
  KSMSNotAPion           = -999 ;
  KSMSVeryLoose          = -999 ;
  KSMSLoose              = -999 ;
  KSMSTight              = -999 ;
  KSMSVeryTight          = -999 ;
  KNNNotAPion            = -999 ;
  KNNVeryLoose           = -999 ;
  KNNLoose               = -999 ;
  KNNTight               = -999 ;
  KNNVeryTight           = -999 ;
  KLHNotAPion            = -999 ;
  KLHVeryLoose           = -999 ;
  KLHLoose               = -999 ;
  KLHTight               = -999 ;
  KLHVeryTight           = -999 ;
  KGLHNotAPion           = -999 ;
  KGLHVeryLoose          = -999 ;
  KGLHLoose              = -999 ;
  KGLHTight              = -999 ;
  KGLHVeryTight          = -999 ;
  KBDTNotAPion           = -999 ;
  KBDTVeryLoose          = -999 ;
  KBDTLoose              = -999 ;
  KBDTTight              = -999 ;
  KBDTVeryTight          = -999 ;
  KKMNotAPion            = -999 ;
  KKMVeryLoose           = -999 ;
  KKMLoose               = -999 ;
  KKMTight               = -999 ;
  KKMVeryTight           = -999 ;
  
  PiRoyLoose             = -999 ;
  PiRoyNotAKaon          = -999 ;
  PiLHVeryLoose          = -999 ;
  PiLHLoose              = -999 ;
  PiLHTight              = -999 ;
  PiLHVeryTight          = -999 ;
  PiGLHVeryLoose         = -999 ;
  PiGLHLoose             = -999 ;
  PiGLHTight             = -999 ;
  PiGLHVeryTight         = -999 ;
  PiKMSuperLoose         = -999 ;
  PiKMVeryLoose          = -999 ;
  PiKMLoose              = -999 ;
  PiKMTight              = -999 ;
  PiKMVeryTight          = -999 ;
  PiKMSuperTight         = -999 ;
  
  MuMinimumIonizing      = ( (MuSelector&(0x1<<0)  )!=0 ) ;
  MuVeryLoose            = ( (MuSelector&(0x1<<1)  )!=0 ) ;
  MuLoose                = ( (MuSelector&(0x1<<2)  )!=0 ) ;
  MuTight                = ( (MuSelector&(0x1<<3)  )!=0 ) ;
  MuVeryTight            = ( (MuSelector&(0x1<<4)  )!=0 ) ;
  MuNNVeryLoose          = ( (MuSelector&(0x1<<5)  )!=0 ) ;
  MuNNLoose              = ( (MuSelector&(0x1<<6)  )!=0 ) ;
  MuNNTight              = ( (MuSelector&(0x1<<7)  )!=0 ) ;
  MuNNVeryTight          = ( (MuSelector&(0x1<<8)  )!=0 ) ;
  MuNNVeryLooseFakeRate  = ( (MuSelector&(0x1<<9)  )!=0 ) ;
  MuNNLooseFakeRate      = ( (MuSelector&(0x1<<10) )!=0 ) ;
  MuNNTightFakeRate      = ( (MuSelector&(0x1<<11) )!=0 ) ;
  MuNNVeryTightFakeRate  = ( (MuSelector&(0x1<<12) )!=0 ) ;
  MuLikeVeryLoose        = ( (MuSelector&(0x1<<13) )!=0 ) ;
  MuLikeLoose            = ( (MuSelector&(0x1<<14) )!=0 ) ;
  MuLikeTight            = ( (MuSelector&(0x1<<15) )!=0 ) ;
  MuBDTVeryLoose         = ( (MuSelector&(0x1<<16) )!=0 ) ;
  MuBDTLoose             = ( (MuSelector&(0x1<<17) )!=0 ) ;
  MuBDTTight             = ( (MuSelector&(0x1<<18) )!=0 ) ;
  MuBDTVeryTight         = ( (MuSelector&(0x1<<19) )!=0 ) ;
  MuBDTVeryLooseFakeRate = ( (MuSelector&(0x1<<20) )!=0 ) ;
  MuBDTLooseFakeRate     = ( (MuSelector&(0x1<<21) )!=0 ) ;
  MuBDTTightFakeRate     = ( (MuSelector&(0x1<<22) )!=0 ) ;
  MuBDTVeryTightFakeRate = ( (MuSelector&(0x1<<23) )!=0 ) ;
  MuBDTLoPLoose          = ( (MuSelector&(0x1<<24) )!=0 ) ;
  MuBDTLoPTight          = ( (MuSelector&(0x1<<25) )!=0 ) ;
  
  ENoCal                 = ( (ESelector&(0x1<<0)   )!=0 ) ;
  EVeryLoose             = ( (ESelector&(0x1<<1)   )!=0 ) ;
  ELoose                 = ( (ESelector&(0x1<<2)   )!=0 ) ;
  ETight                 = ( (ESelector&(0x1<<3)   )!=0 ) ;
  EVeryTight             = ( (ESelector&(0x1<<4)   )!=0 ) ;
  PidLHElectrons         = ( (ESelector&(0x1<<5)   )!=0 ) ;
  EKMSuperLoose          = ( (ESelector&(0x1<<6)   )!=0 ) ;
  EKMVeryLoose           = ( (ESelector&(0x1<<7)   )!=0 ) ;
  EKMLoose               = ( (ESelector&(0x1<<8)   )!=0 ) ;
  EKMTight               = ( (ESelector&(0x1<<9)   )!=0 ) ;
  EKMVeryTight           = ( (ESelector&(0x1<<10)  )!=0 ) ;
  EKMSuperTight          = ( (ESelector&(0x1<<11)  )!=0 ) ;
  
  KSMSNotAPion           = ( (KSelector&(0x1<<0)   )!=0 ) ;
  KSMSVeryLoose          = ( (KSelector&(0x1<<1)   )!=0 ) ;
  KSMSLoose              = ( (KSelector&(0x1<<2)   )!=0 ) ;
  KSMSTight              = ( (KSelector&(0x1<<3)   )!=0 ) ;
  KSMSVeryTight          = ( (KSelector&(0x1<<4)   )!=0 ) ;
  KNNNotAPion            = ( (KSelector&(0x1<<5)   )!=0 ) ;
  KNNVeryLoose           = ( (KSelector&(0x1<<6)   )!=0 ) ;
  KNNLoose               = ( (KSelector&(0x1<<7)   )!=0 ) ;
  KNNTight               = ( (KSelector&(0x1<<8)   )!=0 ) ;
  KNNVeryTight           = ( (KSelector&(0x1<<9)   )!=0 ) ;
  KLHNotAPion            = ( (KSelector&(0x1<<10)  )!=0 ) ;
  KLHVeryLoose           = ( (KSelector&(0x1<<11)  )!=0 ) ;
  KLHLoose               = ( (KSelector&(0x1<<12)  )!=0 ) ;
  KLHTight               = ( (KSelector&(0x1<<13)  )!=0 ) ;
  KLHVeryTight           = ( (KSelector&(0x1<<14)  )!=0 ) ;
  KGLHNotAPion           = ( (KSelector&(0x1<<15)  )!=0 ) ;
  KGLHVeryLoose          = ( (KSelector&(0x1<<16)  )!=0 ) ;
  KGLHLoose              = ( (KSelector&(0x1<<17)  )!=0 ) ;
  KGLHTight              = ( (KSelector&(0x1<<18)  )!=0 ) ;
  KGLHVeryTight          = ( (KSelector&(0x1<<19)  )!=0 ) ;
  KBDTNotAPion           = ( (KSelector&(0x1<<20)  )!=0 ) ;
  KBDTVeryLoose          = ( (KSelector&(0x1<<21)  )!=0 ) ;
  KBDTLoose              = ( (KSelector&(0x1<<22)  )!=0 ) ;
  KBDTTight              = ( (KSelector&(0x1<<23)  )!=0 ) ;
  KBDTVeryTight          = ( (KSelector&(0x1<<24)  )!=0 ) ;
  KKMNotAPion            = ( (KSelector&(0x1<<25)  )!=0 ) ;
  KKMVeryLoose           = ( (KSelector&(0x1<<26)  )!=0 ) ;
  KKMLoose               = ( (KSelector&(0x1<<27)  )!=0 ) ;
  KKMTight               = ( (KSelector&(0x1<<28)  )!=0 ) ;
  KKMVeryTight           = ( (KSelector&(0x1<<29)  )!=0 ) ;
  
  PiRoyLoose             = ( (PiSelector&(0x1<<0)  )!=0 ) ;
  PiRoyNotAKaon          = ( (PiSelector&(0x1<<1)  )!=0 ) ;
  PiLHVeryLoose          = ( (PiSelector&(0x1<<2)  )!=0 ) ;
  PiLHLoose              = ( (PiSelector&(0x1<<3)  )!=0 ) ;
  PiLHTight              = ( (PiSelector&(0x1<<4)  )!=0 ) ;
  PiLHVeryTight          = ( (PiSelector&(0x1<<5)  )!=0 ) ;
  PiGLHVeryLoose         = ( (PiSelector&(0x1<<6)  )!=0 ) ;
  PiGLHLoose             = ( (PiSelector&(0x1<<7)  )!=0 ) ;
  PiGLHTight             = ( (PiSelector&(0x1<<8)  )!=0 ) ;
  PiGLHVeryTight         = ( (PiSelector&(0x1<<9)  )!=0 ) ;
  PiKMSuperLoose         = ( (PiSelector&(0x1<<10) )!=0 ) ;
  PiKMVeryLoose          = ( (PiSelector&(0x1<<11) )!=0 ) ;
  PiKMLoose              = ( (PiSelector&(0x1<<12) )!=0 ) ;
  PiKMTight              = ( (PiSelector&(0x1<<13) )!=0 ) ;
  PiKMVeryTight          = ( (PiSelector&(0x1<<14) )!=0 ) ;
  PiKMSuperTight         = ( (PiSelector&(0x1<<15) )!=0 ) ;

  DsDecayRateType   = 0 ;
  DsDecayRateWeight = 1 ;
  // For MC fill generated quantities
  if(true)
  {
    // Truthmatching to find Mode
    Int_t EllMCIdx = -999 ;
    if(signalMode==4)
    {
      EllMCIdx = PiMCIdx[PiIdx] ;
    }
    else
    {
      EllMCIdx = (signalMode%2==0) ? MuMCIdx[MuIdx] : EMCIdx[EIdx] ;
    }
    
    Int_t EllGMothIdx  = -1 ;
    Int_t EllG2MothIdx = -1 ;    
    Int_t EllG3MothIdx = -1 ;
    EllMode = 0 ;
    EllMCLund = mcLund[EllMCIdx] ;
    Int_t EllMothIdx = mothIdx[EllMCIdx] ;
    EllMothLund = mcLund[EllMothIdx] ;
    EllGMothLund  = 0 ;    
    EllG2MothLund = 0 ;
    EllG3MothLund = 0 ;
    if(EllMothIdx>-1)
    {
      EllGMothIdx  = mothIdx[EllMothIdx] ;
      EllGMothLund =  mcLund[EllGMothIdx] ;
      if(EllGMothIdx>-1)
      {
	EllG2MothIdx  = mothIdx[EllGMothIdx] ;
	EllG2MothLund =  mcLund[EllG2MothIdx] ;
	if(EllG2MothIdx>-1)
	{
	  EllG3MothIdx  = mothIdx[EllG2MothIdx] ;
	  EllG3MothLund =  mcLund[EllG3MothIdx] ;
	}
      }
    }
    
    // Mode numbers
    //   0 : Unassigned
    //  10 : Ds* -> Ds Gamma , Ds -> Ell Nu
    //  11 : Ds* -> Ds Gamma , Ds -> Ell Nu Gamma
    //  12 : Ds* -> Ds Gamma , Ds -> Ell Nu Gamma Gamma
    //  13 : Ds* -> Ds Gamma , Ds -> Ell Nu Gamma Gamma Gamma
    //  14 : Ds* -> Ds Gamma , Ds -> Ell Nu Gamma Gamma Gamma Gamma
    //  15 : Ds* -> Ds Gamma_wrong , Ds -> Ell Nu
    //  16 : Gamma_virtual -> Ds anything , Ds -> Ell Nu    
    //
    // 110 : Ds -> Ell Nu anything
    // 120 : Ds -> Tau Nu
    // 121 : Ds -> Tau Nu ; Tau -> Mu Nu Nu
    // 122 : Ds -> Tau Nu ; Tau -> E  Nu Nu
    // 123 : Ds -> Tau Nu ; Tau -> Pi Nu
    // 124 : Ds -> Tau Nu ; Tau -> Pi Pi0 Nu
    // 125 : Ds -> Tau Nu ; Tau -> Rho Nu ; Rho -> Pi Pi0
    // 130 : Ds -> Pi/K anything , Ell misIDed as Pi/K
    // 
    // 200 : non Ds parent
    // 210 : D+ -> Ell anything
    // 220 : D0 -> Ell anything
    // 230 : Lambda_C -> Ell anything
    //
    // 300 : gamma -> Ell+ Ell-
    // 
    // 400 : Signal Ell is other lepton type
    // 410 : Ds -> Other Ell semileptonic
    // 420 : Ds -> Other Ell leptonic two body
    
    EllMothDauLen = dauLen[EllMothIdx] ;
    EllSibNGamma = 0 ;
    Int_t EllSibs[14] ;
    for(Int_t i=0 ; i<14 ; i++){ EllSibs[i] = 0 ; }
    for(Int_t i=0 ; i<EllMothDauLen ; i++){ EllSibs[i] = mcLund[dauIdx[EllMothIdx]+i] ; }
    for(Int_t i=EllMothDauLen ; i>-1 ;  i--)
    {
      if(EllSibs[i]==22)
      {
	EllSibNGamma++ ;
	for(Int_t j=i ; j<EllMothDauLen-1 ; j++){ EllSibs[j] = EllSibs[j+1] ; }
	EllSibs[EllMothDauLen-1] = 0 ;
      }
    }
    EllMothDauLen -= EllSibNGamma ;
    EllSib1Lund  = EllSibs[0]  ;
    EllSib2Lund  = EllSibs[1]  ;
    EllSib3Lund  = EllSibs[2]  ;
    EllSib4Lund  = EllSibs[3]  ;
    EllSib5Lund  = EllSibs[4]  ;
    EllSib6Lund  = EllSibs[5]  ;
    EllSib7Lund  = EllSibs[6]  ;
    EllSib8Lund  = EllSibs[7]  ;
    EllSib9Lund  = EllSibs[8]  ;
    EllSib10Lund = EllSibs[9]  ;
    EllSib11Lund = EllSibs[10] ;
    EllSib12Lund = EllSibs[11] ;
    EllSib13Lund = EllSibs[12] ;
    EllSib14Lund = EllSibs[13] ;
    ellLund      = EllLund ;

    if(abs(EllMothLund)!=431)
    {
      EllMode = 200 ;
      if(abs(EllMothLund)==411)  EllMode = 210 ;
      if(abs(EllMothLund)==421)  EllMode = 220 ;
      if(abs(EllMothLund)==4122) EllMode = 230 ;
    }
    
    if(abs(EllMothLund)==431 && abs(EllMCLund)==abs(EllLund) && dauLen[EllMothIdx]>2) EllMode = 110 ;
    if(abs(EllMothLund)==431 && abs(EllMCLund)>100)                                   EllMode = 130 ;
    if(abs(EllMothLund)==15  && abs(EllGMothLund)==431 && abs(EllG2MothLund)==433)
    {
      EllMode = 120 ;
      if(abs(EllMCLund)==13) EllMode = 121 ;
      if(abs(EllMCLund)==11) EllMode = 122 ;
      if(abs(EllMCLund)==211)
      {
	if(EllMothDauLen==2) EllMode = 123 ;
	if(EllMothDauLen==3)
	{
	  if(abs(EllSib1Lund*EllSib2Lund*EllSib3Lund)==211*22*14 ) EllMode = 123 ;
	  if(abs(EllSib1Lund*EllSib2Lund*EllSib3Lund)==211*111*14) EllMode = 124 ;
	}
      }
    }
    if(abs(EllMothLund)==213 && abs(EllGMothLund)==15  && abs(EllG2MothLund)==431 && abs(EllG3MothLund)==433) EllMode = 125 ;
    if(abs(EllMothLund)==431 && abs(EllMCLund)==abs(EllLund) && dauLen[EllMothIdx]==2)
    {
      EllMode = 16 ;
      if(abs(EllGMothLund)==433 && dauLen[EllGMothIdx]==2)
      {
	if( mcLund[dauIdx[EllGMothIdx]+1]==22)
	{
	  EllMode = 10 ; // Assume signal
	  if(dauIdx[EllGMothIdx]+1!=EvtMyGamMCIdx[EvtIdx]) EllMode = 15 ; // Wrong gamma
	}
      }
    }
    
    if(signalMode<4 && abs(EllMCLund)==otherEllLund )
    {
      EllMode = 400 ;
      if(abs(EllMothLund)==431 && dauLen[EllMothLund]>2)  EllMode = 410 ;
      if(abs(EllMothLund)==431 && dauLen[EllMothLund]==2) EllMode = 420 ;
    }
    
    if(abs(EllMothLund)==22) EllMode = 300 ;

    if( abs(EllMothLund)==431 && abs(EllSib1Lund*EllSib2Lund)==321*311 && abs(EllMCLund)==321 && abs(EllMothDauLen)==2)  EllMode = 510 ;
    if( abs(EllMothLund)==431 && abs(EllSib1Lund*EllSib2Lund)==211*221 && abs(EllMCLund)==211 && abs(EllMothDauLen)==2)  EllMode = 520 ;
    if( abs(EllMothLund)==15  && abs(EllGMothLund)==431 && abs(EllMCLund)==211 && abs(EllMothDauLen)==2 && signalMode<5) EllMode = 530 ;

    if(signalMode==1 || signalMode==3) EllMode = -EllMode ;
    if(signalMode==4) EllMode = EllMode + 10000 ;
    //cout << EllMCLund << " " << EllMothDauLen << " " << EllMothLund << " " << EllGMothLund << " " << EllG2MothLund << " " << EllG3MothLund << " " << EllMode << endl ;
    if(EllMode==0)
    {
      cout << "Warning: EllMode not accounted for" << endl ;
      cout << "  EllMode: "       << EllMode ;
      cout << "  EllLund: "       << EllLund ;
      cout << "  EllLund:"        << EllLund  ;
      cout << "  EllMothLund: "   << EllMothLund  ;
      cout << "  EllMothDauLen: " << dauLen[EllMothIdx] ;
      cout << "  EllMothIdx: "    << EllMothIdx ;
      cout << "  EllGMothLund: "  << EllGMothLund ;
      cout << "  EllGMothIdx: "   << EllGMothIdx << endl ;
    }
    
    // Look for a Ds
    DsPresence = 0 ;
    Int_t EllLunds[2] = { 13 , 11 } ;
    Int_t  NuLunds[2] = { 14 , 12 } ;
    if(signalMode<2)
    {
      for(Int_t i=0 ; i<mcLen ; i++)
      {
	if(abs(mcLund[i])==431) // Any Ds
	{
	  DsPresence = 1 ;
	  if(dauLen[i]==2)
	  {
	    if(abs(mcLund[dauIdx[i]])==13 && abs(mcLund[dauIdx[i]+1])==NuLunds[signalMode] || 
	       abs(mcLund[dauIdx[i]])==14 && abs(mcLund[dauIdx[i]+1])==EllLunds[signalMode]) // Ds -> mu nu
	    {
	      DsPresence = 2 ;
	      if(abs(mcLund[mothIdx[i]])==433) DsPresence = 3 ;
	    }
	  }
	  if(dauLen[i]==3)
	  {
	    if(abs(mcLund[dauIdx[i]])==NuLunds[signalMode]  && abs(mcLund[dauIdx[i]+1])==EllLunds[signalMode] && abs(mcLund[dauIdx[i]+2])==22                   ||
	       abs(mcLund[dauIdx[i]])==NuLunds[signalMode]  && abs(mcLund[dauIdx[i]+1])==22                   && abs(mcLund[dauIdx[i]+2])==EllLunds[signalMode] ||
	       abs(mcLund[dauIdx[i]])==EllLunds[signalMode] && abs(mcLund[dauIdx[i]+1])==NuLunds[signalMode]  && abs(mcLund[dauIdx[i]+2])==22                   ||
	       abs(mcLund[dauIdx[i]])==EllLunds[signalMode] && abs(mcLund[dauIdx[i]+1])==22                   && abs(mcLund[dauIdx[i]+2])==NuLunds[signalMode]  ||
	       abs(mcLund[dauIdx[i]])==22                   && abs(mcLund[dauIdx[i]+1])==EllLunds[signalMode] && abs(mcLund[dauIdx[i]+2])==NuLunds[signalMode]  ||
	       abs(mcLund[dauIdx[i]])==22                   && abs(mcLund[dauIdx[i]+1])==NuLunds[signalMode]  && abs(mcLund[dauIdx[i]+2])==EllLunds[signalMode]) // Ds -> mu nu gamma
	    {
	      DsPresence = 4 ;
	      if(abs(mcLund[mothIdx[i]])==433) DsPresence = 5 ;
	    }
	  }
	}
      }
    }
    HasDsSt = 0 ;
    HasDs   = 0 ;
    for(Int_t i=0 ; i<mcLen ; i++)
    {
      if(abs(mcLund[i])==433) HasDsSt = 1 ;
      if(abs(mcLund[i])==431) HasDs   = 1 ;
    }
    HasDsToMuNu                 = 0 ;
    HasDsToENu                  = 0 ;
    HasDsToTauNu                = 0 ;
    HasDsToTauNuTauToMuNuNu     = 0 ;
    HasDsToTauNuTauToENuNu      = 0 ;
    HasDsToTauNuTauToPiNu       = 0 ;
    HasDsToTauNuTauToPiPi0Nu    = 0 ;
    HasDsToTauNuTauToPiPi0Pi0Nu = 0 ;
    HasDsToTauNuTauToRhoNu      = 0 ;
    HasDsToTauNuTauToOmegaPiNu  = 0 ;
    Int_t HasTauToMuNuNu     = 0 ;
    Int_t HasTauToENuNu      = 0 ;
    Int_t HasTauToPiNu       = 0 ;
    Int_t HasTauToPiPi0Nu    = 0 ;
    Int_t HasTauToPiPi0Pi0Nu = 0 ;
    Int_t HasTauToRhoNu      = 0 ;
    Int_t HasTauToOmegaPiNu  = 0 ;
    Int_t nDsToMu            = 0 ;
    Int_t nDsToE             = 0 ;
    Int_t nDsToNu            = 0 ;
    Int_t nDsToGamma         = 0 ;
    Int_t nDsToTau           = 0 ;
    for(Int_t i=0 ; i<dauLen[DsGenMCIdx] ; i++)
    {
      switch(abs(mcLund[dauIdx[DsGenMCIdx]+i]))
      {
      case 13: nDsToMu++    ; break ;
      case 11: nDsToE++     ; break ;
      case 22: nDsToGamma++ ; break ;
      case 12:
      case 14:
      case 16:
	nDsToNu++ ;
	break ;
      case 15:
	nDsToTau++ ;
	Int_t TauGenMCIdx = dauIdx[DsGenMCIdx]+i ;
	Int_t nTauToMu    = 0 ;
	Int_t nTauToE     = 0 ;
	Int_t nTauToNu    = 0 ;
	Int_t nTauToPi    = 0 ;
	Int_t nTauToPi0   = 0 ;
	Int_t nTauToRho   = 0 ;
	Int_t nTauToOmega = 0 ;
	Int_t nTauToGamma = 0 ;
	for(Int_t j=0 ; j<dauLen[TauGenMCIdx] ; j++)
	{
	  switch(abs(mcLund[dauIdx[TauGenMCIdx]+j]))
	  {
	  case  13: nTauToMu++    ; break ;
	  case  11: nTauToE++     ; break ;
	  case 211: nTauToPi++    ; break ;
	  case 111: nTauToPi0++   ; break ;
	  case 213: nTauToRho++   ; break ;
	  case 221: nTauToOmega++ ; break ;
	  case  22: nTauToGamma++ ; break ;
	  case  12: 
	  case  14:
	  case  16:
	    nTauToNu++  ;
	    break ;
	  default: break ;
	  }
	} // Tau daughters
	if(nTauToMu==1                    && nTauToNu==2 && (nTauToMu                + nTauToNu + nTauToGamma==dauLen[TauGenMCIdx]) ) HasTauToMuNuNu      = 1 ;
	if(nTauToE==1                     && nTauToNu==2 && (nTauToE                 + nTauToNu + nTauToGamma==dauLen[TauGenMCIdx]) ) HasTauToENuNu       = 1 ;
	if(nTauToPi==1                    && nTauToNu==1 && (nTauToPi                + nTauToNu + nTauToGamma==dauLen[TauGenMCIdx]) ) HasTauToPiNu        = 1 ;
	if(nTauToPi==1    && nTauToPi0==1 && nTauToNu==1 && (nTauToPi    + nTauToPi0 + nTauToNu + nTauToGamma==dauLen[TauGenMCIdx]) ) HasTauToPiPi0Nu     = 1 ;
	if(nTauToPi==1    && nTauToPi0==2 && nTauToNu==1 && (nTauToPi    + nTauToPi0 + nTauToNu + nTauToGamma==dauLen[TauGenMCIdx]) ) HasTauToPiPi0Pi0Nu  = 1 ;
	if(nTauToRho==1                   && nTauToNu==1 && (nTauToRho               + nTauToNu + nTauToGamma==dauLen[TauGenMCIdx]) ) HasTauToRhoNu       = 1 ;
	if(nTauToGamma==1                 && nTauToNu==1 && (nTauToGamma             + nTauToNu + nTauToGamma==dauLen[TauGenMCIdx]) ) HasTauToOmegaPiNu   = 1 ;
	break ;
      }
    }
    //cout << nDsToMu << " "  << nDsToNu << " " << nDsToGamma << " " << dauLen[DsGenMCIdx] << "(" ;
    //for(Int_t i=0 ; i<dauLen[DsGenMCIdx] ; i++ ) cout << " " << mcLund[i+dauIdx[DsGenMCIdx]] ;
    //cout << ")" << endl ;
    if(nDsToMu==1  && nDsToNu==1 && ((nDsToMu  + nDsToNu + nDsToGamma)==dauLen[DsGenMCIdx])) HasDsToMuNu  = 1 ;
    if(nDsToE==1   && nDsToNu==1 && ((nDsToE   + nDsToNu + nDsToGamma)==dauLen[DsGenMCIdx])) HasDsToENu   = 1 ;
    if(nDsToTau==1 && nDsToNu==1 && ((nDsToTau + nDsToNu + nDsToGamma)==dauLen[DsGenMCIdx])) HasDsToTauNu = 1 ;
    HasDsToTauNuTauToMuNuNu     = HasDsToTauNu*HasTauToMuNuNu     ;
    HasDsToTauNuTauToENuNu      = HasDsToTauNu*HasTauToENuNu      ;
    HasDsToTauNuTauToPiNu       = HasDsToTauNu*HasTauToPiNu       ;
    HasDsToTauNuTauToPiPi0Nu    = HasDsToTauNu*HasTauToPiPi0Nu    ;
    HasDsToTauNuTauToPiPi0Pi0Nu = HasDsToTauNu*HasTauToPiPi0Pi0Nu ;
    HasDsToTauNuTauToRhoNu      = HasDsToTauNu*HasTauToRhoNu      ;
    HasDsToTauNuTauToOmegaPiNu  = HasDsToTauNu*HasTauToOmegaPiNu  ;

    // Get information about specific Ds decays
    nDsToNu             = 0 ;
    nDsToE              = 0 ;
    nDsToMu             = 0 ;
    Int_t nDsToPi       = 0 ;
    Int_t nDsToPi0      = 0 ;
    Int_t nDsToEta      = 0 ;
    Int_t nDsToEtaPrime = 0 ;
    Int_t nDsToOmega    = 0 ;
    Int_t nDsTof0       = 0 ;
    Int_t nDsTof2       = 0 ;
    Int_t nDsToRho0     = 0 ;
    Int_t nDsToPhi      = 0 ;
    Int_t nDsToK        = 0 ;
    Int_t nDsToK0       = 0 ;
    Int_t nDsToKStar0   = 0 ;
    for(Int_t i=0 ; i<dauLen[DsGenMCIdx] ; i++)
    {
      switch(abs(mcLund[dauIdx[DsGenMCIdx]+i]))
      {
      case    12 :
      case    14:
      case    16:
      case    19: nDsToNu++       ; break ;
      case    11: nDsToE++        ; break ;
      case    13: nDsToMu++       ; break ;
      case    22: nDsToGamma++    ; break ;
      case   211: nDsToPi++       ; break ;
      case   111: nDsToPi0++      ; break ;
      case   221: nDsToEta++      ; break ;
      case   331: nDsToEtaPrime++ ; break ;
      case   333: nDsToPhi++      ; break ;
      case   223: nDsToOmega++    ; break ;
      case 10221: nDsTof0++       ; break ;
      case   225: nDsTof2++       ; break ;
      case   113: nDsToRho0++     ; break ;
      case   321: nDsToK++        ; break ;
      case   311: nDsToK0++       ; break ;
      case   313: nDsToKStar0++   ; break ;
      }
      if(      (nDsToGamma+nDsToPi+nDsToEta==dauLen[DsGenMCIdx])&&(nDsToPi==1&&nDsToEta==1)              ){ DsDecayRateType =  1 ; DsDecayRateWeight = 0.75 ; } // eta pi
      if( (nDsToGamma+nDsToPi+nDsToEtaPrime==dauLen[DsGenMCIdx])&&(nDsToPi==1&&nDsToEtaPrime==1)         ){ DsDecayRateType =  2 ; DsDecayRateWeight = 0.81 ; } // eta' pi
      if(    (nDsToGamma+nDsToPi+nDsToOmega==dauLen[DsGenMCIdx])&&(nDsToPi==1&&nDsToOmega==1)            ){ DsDecayRateType =  3 ; DsDecayRateWeight = 0.74 ; } // omega pi
      if(       (nDsToGamma+nDsToPi+nDsTof0==dauLen[DsGenMCIdx])&&(nDsToPi==1&&nDsTof0==1)               ){ DsDecayRateType =  4 ; DsDecayRateWeight = 0.60 ; } // f_0 pi
      if(       (nDsToGamma+nDsToPi+nDsTof2==dauLen[DsGenMCIdx])&&(nDsToPi==1&&nDsTof2==1)               ){ DsDecayRateType =  5 ; DsDecayRateWeight = 0.48 ; } // f_2 pi
      if(               (nDsToGamma+nDsToPi==dauLen[DsGenMCIdx])&&(nDsToPi==3)                           ){ DsDecayRateType =  6 ; DsDecayRateWeight = 222  ; } // pi pi pi
      if(    (nDsToGamma+nDsToKStar0+nDsToK==dauLen[DsGenMCIdx])&&(nDsToKStar0==1&&nDsToK==1)            ){ DsDecayRateType =  7 ; DsDecayRateWeight = 0.65 ; } // K*0 K
      if(        (nDsToGamma+nDsToK0+nDsToK==dauLen[DsGenMCIdx])&&(nDsToK0==1&&nDsToK==1)                ){ DsDecayRateType =  8 ; DsDecayRateWeight = 0.68 ; } // K0 K
      if(  (nDsToGamma+nDsToEtaPrime+nDsToK==dauLen[DsGenMCIdx])&&(nDsToEtaPrime==1&&nDsToK==1)          ){ DsDecayRateType =  9 ; DsDecayRateWeight = 8.0  ; } // eta' K
      if(       (nDsToGamma+nDsToK0+nDsToPi==dauLen[DsGenMCIdx])&&(nDsToK0==1&&nDsToPi==1)               ){ DsDecayRateType = 10 ; DsDecayRateWeight = 6.25 ; } // K0 pi
      if(     (nDsToGamma+nDsToRho0+nDsToPi==dauLen[DsGenMCIdx])&&(nDsToRho0==1&&nDsToPi==1)             ){ DsDecayRateType = 11 ; DsDecayRateWeight = 1.80 ; } // rho0 pi
      if(   (nDsToGamma+nDsToKStar0+nDsToPi==dauLen[DsGenMCIdx])&&(nDsToKStar0==1&&nDsToPi==1)           ){ DsDecayRateType = 12 ; DsDecayRateWeight = 6.25 ; } // K*0 pi
      if(                (nDsToGamma+nDsToK==dauLen[DsGenMCIdx])&&(nDsToK==3)                            ){ DsDecayRateType = 13 ; DsDecayRateWeight = 2.45 ; } // K K K
      if((nDsToGamma+nDsToK0+nDsToK+nDsToPi==dauLen[DsGenMCIdx])&&(nDsToK0==1&&nDsToK==1&&nDsToPi==2)    ){ DsDecayRateType = 14 ; DsDecayRateWeight = 19.2 ; } // K0 K pi pi
      if(        (nDsToGamma+nDsToK+nDsToPi==dauLen[DsGenMCIdx])&&(nDsToK==2&&nDsToPi==3)                ){ DsDecayRateType = 15 ; DsDecayRateWeight = 0.21 ; } // K K pi pi pi
      if(       (nDsToGamma+nDsToK0+nDsToPi==dauLen[DsGenMCIdx])&&(nDsToK0==1&&nDsToPi==1)               ){ DsDecayRateType = 16 ; DsDecayRateWeight = 6.25 ; } // K0 pi
      // Semileptonic modes
      if((      nDsToGamma+nDsToPhi+nDsToE+nDsToNu==dauLen[DsGenMCIdx])&&(nDsToPhi==1&&nDsToE==1&&nDsToNu==1)      ){ DsDecayRateType = 17 ; DsDecayRateWeight = 2.29/2.42 ; } // phi  e  nu
      if((      nDsToGamma+nDsToEta+nDsToE+nDsToNu==dauLen[DsGenMCIdx])&&(nDsToEta==1&&nDsToE==1&&nDsToNu==1)      ){ DsDecayRateType = 18 ; DsDecayRateWeight = 2.48/3.07 ; } // eta  e  nu
      if(( nDsToGamma+nDsToEtaPrime+nDsToE+nDsToNu==dauLen[DsGenMCIdx])&&(nDsToEtaPrime==1&&nDsToE==1&&nDsToNu==1) ){ DsDecayRateType = 19 ; DsDecayRateWeight = 0.91/1.06 ; } // eta' e  nu
      if((       nDsToGamma+nDsToK0+nDsToE+nDsToNu==dauLen[DsGenMCIdx])&&(nDsToK0==1&&nDsToE==1&&nDsToNu==1)       ){ DsDecayRateType = 20 ; DsDecayRateWeight = 0.37/0.27 ; } // K0   e  nu
      if((   nDsToGamma+nDsToKStar0+nDsToE+nDsToNu==dauLen[DsGenMCIdx])&&(nDsToKStar0==1&&nDsToE==1&&nDsToNu==1)   ){ DsDecayRateType = 21 ; DsDecayRateWeight = 0.18/0.10 ; } // K*0  e  nu
      if((     nDsToGamma+nDsToPhi+nDsToMu+nDsToNu==dauLen[DsGenMCIdx])&&(nDsToPhi==1&&nDsToMu==1&&nDsToNu==1)     ){ DsDecayRateType = 22 ; DsDecayRateWeight = 2.29/2.42 ; } // phi  mu nu
      if((     nDsToGamma+nDsToEta+nDsToMu+nDsToNu==dauLen[DsGenMCIdx])&&(nDsToEta==1&&nDsToMu==1&&nDsToNu==1)     ){ DsDecayRateType = 23 ; DsDecayRateWeight = 2.48/3.07 ; } // eta  mu nu
      if((nDsToGamma+nDsToEtaPrime+nDsToMu+nDsToNu==dauLen[DsGenMCIdx])&&(nDsToEtaPrime==1&&nDsToMu==1&&nDsToNu==1)){ DsDecayRateType = 24 ; DsDecayRateWeight = 0.91/1.06 ; } // eta' mu nu
      if((      nDsToGamma+nDsToK0+nDsToMu+nDsToNu==dauLen[DsGenMCIdx])&&(nDsToK0==1&&nDsToMu==1&&nDsToNu==1)      ){ DsDecayRateType = 25 ; DsDecayRateWeight = 0.37/0.27 ; } // K0   mu nu
      if((  nDsToGamma+nDsToKStar0+nDsToMu+nDsToNu==dauLen[DsGenMCIdx])&&(nDsToKStar0==1&&nDsToMu==1&&nDsToNu==1)  ){ DsDecayRateType = 26 ; DsDecayRateWeight = 0.18/0.10 ; } // K*0  mu nu
    }
  } // End of MC matching
}

Int_t DRecoDsToEllNuAnalysis::SetDsToEllNuBranches()
{
  if(_fChain==NULL)
  {
    cout << "No Chain" << endl ;
    return 0 ;
  }
  if( _fChain->GetBranch("EvtNupxRaw")          ) _fChain->SetBranchAddress("EvtNupxRaw"          , NupxLabRaw_in     ) ;
  if( _fChain->GetBranch("EvtNupyRaw")          ) _fChain->SetBranchAddress("EvtNupyRaw"          , NupyLabRaw_in     ) ;
  if( _fChain->GetBranch("EvtNupzRaw")          ) _fChain->SetBranchAddress("EvtNupzRaw"          , NupzLabRaw_in     ) ;
  if( _fChain->GetBranch("EvtNuenergyRaw")      ) _fChain->SetBranchAddress("EvtNuenergyRaw"      , NuenergyLabRaw_in ) ;
  if( _fChain->GetBranch("EvtMupxRaw")          ) _fChain->SetBranchAddress("EvtMupxRaw"          , MupxRaw_in        ) ;
  if( _fChain->GetBranch("EvtMupyRaw")          ) _fChain->SetBranchAddress("EvtMupyRaw"          , MupyRaw_in        ) ;
  if( _fChain->GetBranch("EvtMupzRaw")          ) _fChain->SetBranchAddress("EvtMupzRaw"          , MupzRaw_in        ) ;
  if( _fChain->GetBranch("EvtMuenergyRaw")      ) _fChain->SetBranchAddress("EvtMuenergyRaw"      , MuenergyRaw_in    ) ;
  if( _fChain->GetBranch("EvtNupxCorr")         ) _fChain->SetBranchAddress("EvtNupxCorr"         , NupxLabCorr_in    ) ;
  if( _fChain->GetBranch("EvtNupyCorr")         ) _fChain->SetBranchAddress("EvtNupyCorr"         , NupyLabCorr_in    ) ;
  if( _fChain->GetBranch("EvtNupzCorr")         ) _fChain->SetBranchAddress("EvtNupzCorr"         , NupzLabCorr_in    ) ;
  if( _fChain->GetBranch("Nup3CM")              ) _fChain->SetBranchAddress("Nup3CM"              , Nup3CM            ) ;
  if( _fChain->GetBranch("NucosthCM")           ) _fChain->SetBranchAddress("NucosthCM"           , NucosthCM         ) ;
  if( _fChain->GetBranch("NuphiCM")             ) _fChain->SetBranchAddress("NuphiCM"             , NuphiCM           ) ;
  if( _fChain->GetBranch("NuenergyCM")          ) _fChain->SetBranchAddress("NuenergyCM"          , NuenergyCM_in     ) ;
  if( _fChain->GetBranch("Mup3CM")              ) _fChain->SetBranchAddress("Mup3CM"              , Mup3CM            ) ;
  if( _fChain->GetBranch("MucosthCM")           ) _fChain->SetBranchAddress("MucosthCM"           , MucosthCM         ) ;
  if( _fChain->GetBranch("MuphiCM")             ) _fChain->SetBranchAddress("MuphiCM"             , MuphiCM           ) ;
  if( _fChain->GetBranch("Ep3CM")               ) _fChain->SetBranchAddress("Ep3CM"               , Ep3CM             ) ;
  if( _fChain->GetBranch("EcosthCM")            ) _fChain->SetBranchAddress("EcosthCM"            , EcosthCM          ) ;
  if( _fChain->GetBranch("EphiCM")              ) _fChain->SetBranchAddress("EphiCM"              , EphiCM            ) ;
  
  if( _fChain->GetBranch("MuMCIdx")             ) _fChain->SetBranchAddress("MuMCIdx"             , MuMCIdx           ) ;
  if( _fChain->GetBranch("EMCIdx")              ) _fChain->SetBranchAddress("EMCIdx"              , EMCIdx            ) ;
  
  if( _fChain->GetBranch("EvtDsToEllNuProbChi2")) _fChain->SetBranchAddress("EvtDsToEllNuProbChi2", ProbChi2          ) ;
  if( _fChain->GetBranch("EvtChi2")             ) _fChain->SetBranchAddress("EvtChi2"             , Chi2              ) ;
  if( _fChain->GetBranch("Evtndof")             ) _fChain->SetBranchAddress("Evtndof"             , Ndof              ) ;
  if( _fChain->GetBranch("EvtDeltap3")          ) _fChain->SetBranchAddress("EvtDeltap3"          , Deltap3           ) ;
  
  if( _fChain->GetBranch("EvtThrustX")          ) _fChain->SetBranchAddress("EvtThrustX"          , ThrustX           ) ;
  if( _fChain->GetBranch("EvtThrustY")          ) _fChain->SetBranchAddress("EvtThrustY"          , ThrustY           ) ;
  if( _fChain->GetBranch("EvtThrustZ")          ) _fChain->SetBranchAddress("EvtThrustZ"          , ThrustZ           ) ;
  if( _fChain->GetBranch("EvtThrustMag")        ) _fChain->SetBranchAddress("EvtThrustMag"        , ThrustMag         ) ;
  
  if( _fChain->GetBranch("EvtR0")               ) _fChain->SetBranchAddress("EvtR0"               , R0                ) ;
  if( _fChain->GetBranch("EvtR1")               ) _fChain->SetBranchAddress("EvtR1"               , R1                ) ;
  if( _fChain->GetBranch("EvtR2")               ) _fChain->SetBranchAddress("EvtR2"               , R2                ) ;
  if( _fChain->GetBranch("EvtR3")               ) _fChain->SetBranchAddress("EvtR3"               , R3                ) ;
  if( _fChain->GetBranch("EvtR4")               ) _fChain->SetBranchAddress("EvtR4"               , R4                ) ;
  
  if( _fChain->GetBranch("EvtEExtraNeutral")    ) _fChain->SetBranchAddress("EvtEExtraNeutral"    , EExtraNeutral     ) ;
  if( _fChain->GetBranch("EvtNExtraNeutral")    ) _fChain->SetBranchAddress("EvtNExtraNeutral"    , NExtraNeutral     ) ;
  if( _fChain->GetBranch("EvtNExtraTracks")     ) _fChain->SetBranchAddress("EvtNExtraTracks"     , NExtraTracks      ) ;
  if( _fChain->GetBranch("EvtExtra1px")         ) _fChain->SetBranchAddress("EvtExtra1px"         , Extra1px          ) ;
  if( _fChain->GetBranch("EvtExtra1py")         ) _fChain->SetBranchAddress("EvtExtra1py"         , Extra1py          ) ;
  if( _fChain->GetBranch("EvtExtra1px")         ) _fChain->SetBranchAddress("EvtExtra1pz"         , Extra1pz          ) ;
  if( _fChain->GetBranch("EvtExtra2px")         ) _fChain->SetBranchAddress("EvtExtra2px"         , Extra2px          ) ;
  if( _fChain->GetBranch("EvtExtra2py")         ) _fChain->SetBranchAddress("EvtExtra2py"         , Extra2py          ) ;
  if( _fChain->GetBranch("EvtExtra2pz")         ) _fChain->SetBranchAddress("EvtExtra2pz"         , Extra2pz          ) ;
  
  if( _fChain->GetBranch("EvtDsMass")           ) _fChain->SetBranchAddress("EvtDsMass"           , EvtDsMass         ) ;
  if( _fChain->GetBranch("EvtSignalMode")       ) _fChain->SetBranchAddress("EvtSignalMode"       , SignalMode        ) ;
  
  if( _fChain->GetBranch("EvtPMissCosTh")       ) _fChain->SetBranchAddress("EvtPMissCosTh"       , PMissCosTh        ) ;
  if( _fChain->GetBranch("EvtPMissPhi")         ) _fChain->SetBranchAddress("EvtPMissPhi"         , PMissPhi          ) ;
  if( _fChain->GetBranch("EvtPMiss")            ) _fChain->SetBranchAddress("EvtPMiss"            , PMiss             ) ;
  if( _fChain->GetBranch("EvtEMiss")            ) _fChain->SetBranchAddress("EvtEMiss"            , EMiss             ) ;
  
  if( _fChain->GetBranch("nMu")                 ) _fChain->SetBranchAddress("nMu"                 , &nMu              ) ;
  if( _fChain->GetBranch("nE")                  ) _fChain->SetBranchAddress("nE"                  , &nE               ) ;
  if( _fChain->GetBranch("nTRK")                ) _fChain->SetBranchAddress("nTRK"                , &nTRK             ) ;
  if( _fChain->GetBranch("MuTrkIdx")            ) _fChain->SetBranchAddress("MuTrkIdx"            , MuTrkIdx          ) ;
  if( _fChain->GetBranch("ETrkIdx")             ) _fChain->SetBranchAddress("ETrkIdx"             , ETrkIdx           ) ;
  
  if( _fChain->GetBranch("muSelectorsMap")      ) _fChain->SetBranchAddress("muSelectorsMap"      , MuSelectorsMap    ) ;
  if( _fChain->GetBranch("eSelectorsMap")       ) _fChain->SetBranchAddress("eSelectorsMap"       , ESelectorsMap     ) ;
  
  if( _fChain->GetBranch("upperID")             ) _fChain->SetBranchAddress("upperID"             , &upperID          ) ;
  if( _fChain->GetBranch("lowerID")             ) _fChain->SetBranchAddress("lowerID"             , &lowerID          ) ;
  
  if( _fChain->GetBranch("TRKnDchXY")           ) _fChain->SetBranchAddress("TRKnDchXY"           , &TRKnDchXY        ) ;
  if( _fChain->GetBranch("TRKnDchZ")            ) _fChain->SetBranchAddress("TRKnDchZ"            , &TRKnDchZ         ) ;
  if( _fChain->GetBranch("TRKnSvtXY")           ) _fChain->SetBranchAddress("TRKnSvtXY"           , &TRKnSvtXY        ) ;
  if( _fChain->GetBranch("TRKnSvtZ")            ) _fChain->SetBranchAddress("TRKnSvtZ"            , &TRKnSvtZ         ) ;
  
  cout << "Done linking DsToEllNu Chain Branches" << endl ;
 
  return SetDsInclusiveBranches() ;
}

Int_t DRecoDsToEllNuAnalysis::MakeDsToEllNuReducedNtupleBranches()
{
  ReducedNtuple->Branch("NupxLabRaw"                  , &NupxLabRaw                  , "NupxLabRaw/F"                 ) ;
  ReducedNtuple->Branch("NupyLabRaw"                  , &NupyLabRaw                  , "NupyLabRaw/F"                 ) ;
  ReducedNtuple->Branch("NupzLabRaw"                  , &NupzLabRaw                  , "NupzLabRaw/F"                 ) ;
  ReducedNtuple->Branch("NuenergyLabRaw"              , &NuenergyLabRaw              , "NueneryLabRaw/F"              ) ;
  ReducedNtuple->Branch("MupxLabRaw"                  , &MupxLabRaw                  , "MupxLabRaw/F"                 ) ;
  ReducedNtuple->Branch("MupyLabRaw"                  , &MupyLabRaw                  , "MupyLabRaw/F"                 ) ;
  ReducedNtuple->Branch("MupzLabRaw"                  , &MupzLabRaw                  , "MupzLabRaw/F"                 ) ;
  ReducedNtuple->Branch("NupxCM"                      , &NupxCM                      , "NupxCM/F"                     ) ;
  ReducedNtuple->Branch("NupyCM"                      , &NupyCM                      , "NupyCM/F"                     ) ;
  ReducedNtuple->Branch("NupzCM"                      , &NupzCM                      , "NupzCM/F"                     ) ;
  ReducedNtuple->Branch("NuenergyCM"                  , &NuenergyCM                  , "NuenergyCM/F"                 ) ;
  ReducedNtuple->Branch("MupxCM"                      , &MupxCM                      , "MupxCM/F"                     ) ;
  ReducedNtuple->Branch("MupyCM"                      , &MupyCM                      , "MupyCM/F"                     ) ;
  ReducedNtuple->Branch("MupzCM"                      , &MupzCM                      , "MupzCM/F"                     ) ;
  ReducedNtuple->Branch("EpxCM"                       , &EpxCM                       , "EpxCM/F"                      ) ;
  ReducedNtuple->Branch("EpyCM"                       , &EpyCM                       , "EpyCM/F"                      ) ;
  ReducedNtuple->Branch("EpzCM"                       , &EpzCM                       , "EpzCM/F"                      ) ;
  ReducedNtuple->Branch("DsStpxCM"                    , &DsStpxCM                    , "DsStpxCM/F"                   ) ;
  ReducedNtuple->Branch("DsStpyCM"                    , &DsStpyCM                    , "DsStpyCM/F"                   ) ;
  ReducedNtuple->Branch("DsStpzCM"                    , &DsStpzCM                    , "DsStpzCM/F"                   ) ;
  ReducedNtuple->Branch("DspxCM"                      , &DspxCM                      , "DspxCM/F"                     ) ;
  ReducedNtuple->Branch("DspyCM"                      , &DspyCM                      , "DspyCM/F"                     ) ;
  ReducedNtuple->Branch("DspzCM"                      , &DspzCM                      , "DspzCM/F"                     ) ;
  						      				     				      
  ReducedNtuple->Branch("MupxLabRaw"                  , &MupxLabRaw                  , "MupxLabRaw/F"                 ) ;
  ReducedNtuple->Branch("MupyLabRaw"                  , &MupyLabRaw                  , "MupyLabRaw/F"                 ) ;
  ReducedNtuple->Branch("MupzLabRaw"                  , &MupzLabRaw                  , "MupzLabRaw/F"                 ) ;
  ReducedNtuple->Branch("EpxLabRaw"                   , &EpxLabRaw                   , "EpxLabRaw/F"                  ) ;
  ReducedNtuple->Branch("EpyLabRaw"                   , &EpyLabRaw                   , "EpyLabRaw/F"                  ) ;
  ReducedNtuple->Branch("EpzLabRaw"                   , &EpzLabRaw                   , "EpzLabRaw/F"                  ) ;
  ReducedNtuple->Branch("PipxLabRaw"                  , &PipxLabRaw                  , "PipxLabRaw/F"                 ) ;
  ReducedNtuple->Branch("PipyLabRaw"                  , &PipyLabRaw                  , "PipyLabRaw/F"                 ) ;
  ReducedNtuple->Branch("PipzLabRaw"                  , &PipzLabRaw                  , "PipzLabRaw/F"                 ) ;
  						      				     				      
  ReducedNtuple->Branch("ProbChi2"                    , &ProbChi2                    , "ProbChi2/F"                   ) ;
  ReducedNtuple->Branch("Chi2"                        , &Chi2                        , "Chi2/F"                       ) ;
  ReducedNtuple->Branch("ndof"                        , &ndof                        , "ndof/F"                       ) ;
  						      				     				      
  ReducedNtuple->Branch("mm2"                         , &mm2                         , "mm2/F"                        ) ;
  ReducedNtuple->Branch("mm2Stretch"                  , &mm2Stretch                  , "mm2Stretch/F"                 ) ;
  ReducedNtuple->Branch("mm2Shift"                    , &mm2Shift                    , "mm2Shift/F"                   ) ;
  ReducedNtuple->Branch("mm2Raw"                      , &mm2Raw                      , "mm2Raw/F"                     ) ;
  ReducedNtuple->Branch("Pip3Ds"                      , &Pip3Ds                      , "Pip3Ds/F"                     ) ;
  						      				     				      
  ReducedNtuple->Branch("deltap3"                     , &deltap3                     , "deltap3/F"                    ) ;
  ReducedNtuple->Branch("deltap3Joerg"                , &deltap3Joerg                , "deltap3Joerg/F"               ) ;
  ReducedNtuple->Branch("deltap3Fit"                  , &deltap3Fit                  , "deltap3Fit/F"                 ) ;
  ReducedNtuple->Branch("deltap3FitJoerg"             , &deltap3FitJoerg             , "deltap3FitJoerg/F"            ) ;
  ReducedNtuple->Branch("pDiff"                       , &pDiff                       , "pDiff/F"                      ) ;
  ReducedNtuple->Branch("coneEnergy"                  , &coneEnergy                  , "coneEnergy/F"                 ) ;
  						      				     				      
  ReducedNtuple->Branch("thetaDsStDs"                 , &thetaDsStDs                 , "thetaDsStDs/F"                ) ;
  ReducedNtuple->Branch("thetaDsStMu"                 , &thetaDsStMu                 , "thetaDsStMu/F"                ) ;
  ReducedNtuple->Branch("thetaDsStE"                  , &thetaDsStE                  , "thetaDsStE/F"                 ) ;
  ReducedNtuple->Branch("thetaDsStPi"                 , &thetaDsStPi                 , "thetaDsStPi/F"                ) ;
  ReducedNtuple->Branch("thetaDsStNu"                 , &thetaDsStNu                 , "thetaDsStNu/F"                ) ;
  ReducedNtuple->Branch("thetaDsStGam"                , &thetaDsStGam                , "thetaDsStGam/F"               ) ;
  ReducedNtuple->Branch("thetaDsMu"                   , &thetaDsMu                   , "thetaDsMu/F"                  ) ;
  ReducedNtuple->Branch("thetaDsE"                    , &thetaDsE                    , "thetaDsE/F"                   ) ;
  ReducedNtuple->Branch("thetaDsPi"                   , &thetaDsPi                   , "thetaDsPi/F"                  ) ;
  ReducedNtuple->Branch("thetaDsNu"                   , &thetaDsNu                   , "thetaDsNu/F"                  ) ;
  ReducedNtuple->Branch("thetaDsGam"                  , &thetaDsGam                  , "thetaDsGam/F"                 ) ;
  ReducedNtuple->Branch("thetaMuNu"                   , &thetaMuNu                   , "thetaMuNu/F"                  ) ;
  ReducedNtuple->Branch("thetaMuGam"                  , &thetaMuGam                  , "thetaMuGam/F"                 ) ;
  ReducedNtuple->Branch("thetaENu"                    , &thetaENu                    , "thetaENu/F"                   ) ;
  ReducedNtuple->Branch("thetaEGam"                   , &thetaEGam                   , "thetaEGam/F"                  ) ;
  ReducedNtuple->Branch("thetaPiNu"                   , &thetaPiNu                   , "thetaPiNu/F"                  ) ;
  ReducedNtuple->Branch("thetaPiGam"                  , &thetaPiGam                  , "thetaPiGam/F"                 ) ;
  ReducedNtuple->Branch("thetaNuGam"                  , &thetaNuGam                  , "thetaNuGam/F"                 ) ;
  ReducedNtuple->Branch("thetaDsStTag"                , &thetaDsStTag                , "thetaDsStTag/F"               ) ;
  ReducedNtuple->Branch("thetaDsStK"                  , &thetaDsStK                  , "thetaDsStK/F"                 ) ;
  ReducedNtuple->Branch("thetaDsTag"                  , &thetaDsTag                  , "thetaDsTag/F"                 ) ;
  ReducedNtuple->Branch("thetaDsK"                    , &thetaDsK                    , "thetaDsK/F"                   ) ;
  ReducedNtuple->Branch("thetaMuTag"                  , &thetaMuTag                  , "thetaMuTag/F"                 ) ;
  ReducedNtuple->Branch("thetaMuK"                    , &thetaMuK                    , "thetaMuK/F"                   ) ;
  ReducedNtuple->Branch("thetaETag"                   , &thetaETag                   , "thetaETag/F"                  ) ;
  ReducedNtuple->Branch("thetaEK"                     , &thetaEK                     , "thetaEK/F"                    ) ;
  ReducedNtuple->Branch("thetaPiTag"                  , &thetaPiTag                  , "thetaPiTag/F"                 ) ;
  ReducedNtuple->Branch("thetaPiK"                    , &thetaPiK                    , "thetaPiK/F"                   ) ;
  						      				     				      
  ReducedNtuple->Branch("ThrustX"                     , &thrustX                     , "ThrustX/F"                    ) ;
  ReducedNtuple->Branch("ThrustY"                     , &thrustY                     , "ThrustY/F"                    ) ;
  ReducedNtuple->Branch("ThrustZ"                     , &thrustZ                     , "ThrustZ/F"                    ) ;
  ReducedNtuple->Branch("ThrustMag"                   , &thrustMag                   , "ThrustMag/F"                  ) ;
  						      				     				      
  ReducedNtuple->Branch("R0"                          , &r0                          , "R0/F"                         ) ;
  ReducedNtuple->Branch("R1"                          , &r1                          , "R1/F"                         ) ;
  ReducedNtuple->Branch("R2"                          , &r2                          , "R2/F"                         ) ;
  ReducedNtuple->Branch("R3"                          , &r3                          , "R3/F"                         ) ;
  ReducedNtuple->Branch("R4"                          , &r4                          , "R4/F"                         ) ;
  						      				     				      
  ReducedNtuple->Branch("EExtraNeutral"               , &eExtraNeutral               , "EExtraNeutral/F"              ) ;
  ReducedNtuple->Branch("NExtraNeutral"               , &nExtraNeutral               , "NExtraNeutral/I"              ) ;
  ReducedNtuple->Branch("NExtraTracks"                , &nExtraTracks                , "NExtraTracks/I"               ) ;
  						      				     				      
  ReducedNtuple->Branch("PMissCosTh"                  , &pMissCosTh                  , "PMissCosTh/F"                 ) ;
  ReducedNtuple->Branch("PMissPhi"                    , &pMissPhi                    , "PMissPhi/F"                   ) ;
  ReducedNtuple->Branch("PMiss"                       , &pMiss                       , "PMiss/F"                      ) ;
  ReducedNtuple->Branch("EMiss"                       , &eMiss                       , "EMiss/F"                      ) ;
  						      				     				      
  ReducedNtuple->Branch("Dsd2Idx"                     , &dsd2Idx                     , "Dsd2Idx/I"                    ) ;
  ReducedNtuple->Branch("EllMode"                     , &EllMode                     , "EllMode/I"                    ) ;
  ReducedNtuple->Branch("EllLund"                     , &EllMCLund                   , "EllLund/I"                    ) ;
  ReducedNtuple->Branch("EllMothLund"                 , &EllMothLund                 , "EllMothLund/I"                ) ;
  ReducedNtuple->Branch("EllGMothLund"                , &EllGMothLund                , "EllGMothLund/I"               ) ;
  ReducedNtuple->Branch("EllG2MothLund"               , &EllG2MothLund               , "EllG2MothLund/I"              ) ;
  ReducedNtuple->Branch("EllG3MothLund"               , &EllG3MothLund               , "EllG3MothLund/I"              ) ;
  ReducedNtuple->Branch("EllMothDauLen"               , &EllMothDauLen               , "EllMothDauLen/I"              ) ;
  ReducedNtuple->Branch("EllSib1Lund"                 , &EllSib1Lund                 , "EllSib1Lund/I"                ) ;
  ReducedNtuple->Branch("EllSib2Lund"                 , &EllSib2Lund                 , "EllSib2Lund/I"                ) ;
  ReducedNtuple->Branch("EllSib3Lund"                 , &EllSib3Lund                 , "EllSib3Lund/I"                ) ;
  ReducedNtuple->Branch("EllSib4Lund"                 , &EllSib4Lund                 , "EllSib4Lund/I"                ) ;
  ReducedNtuple->Branch("EllSib5Lund"                 , &EllSib5Lund                 , "EllSib5Lund/I"                ) ;
  ReducedNtuple->Branch("EllSib6Lund"                 , &EllSib6Lund                 , "EllSib6Lund/I"                ) ;
  ReducedNtuple->Branch("EllSib7Lund"                 , &EllSib7Lund                 , "EllSib7Lund/I"                ) ;
  ReducedNtuple->Branch("EllSib8Lund"                 , &EllSib8Lund                 , "EllSib8Lund/I"                ) ;
  ReducedNtuple->Branch("EllSib9Lund"                 , &EllSib9Lund                 , "EllSib9Lund/I"                ) ;
  ReducedNtuple->Branch("EllSib10Lund"                , &EllSib10Lund                , "EllSib10Lund/I"               ) ;
  ReducedNtuple->Branch("EllSib11Lund"                , &EllSib11Lund                , "EllSib11Lund/I"               ) ;
  ReducedNtuple->Branch("EllSib12Lund"                , &EllSib12Lund                , "EllSib12Lund/I"               ) ;
  ReducedNtuple->Branch("EllSib13Lund"                , &EllSib13Lund                , "EllSib13Lund/I"               ) ;
  ReducedNtuple->Branch("EllSib14Lund"                , &EllSib14Lund                , "EllSib14Lund/I"               ) ;
  ReducedNtuple->Branch("EllSibNGamma"                , &EllSibNGamma                , "EllSibNGamma/I"               ) ;
						      				     				      
  ReducedNtuple->Branch("DsPresence"                  , &DsPresence                  , "DsPresence/I"                 ) ;
  ReducedNtuple->Branch("HasDsSt"                     , &HasDsSt                     , "HasDsSt/I"                    ) ;
  ReducedNtuple->Branch("HasDs"                       , &HasDs                       , "HasDs/I"                      ) ;  
  ReducedNtuple->Branch("HasDsToMuNu"                 , &HasDsToMuNu                 , "HasDsToMuNu/I"                ) ;
  ReducedNtuple->Branch("HasDsToENu"                  , &HasDsToENu                  , "HasDsToENu/I"                 ) ;
  ReducedNtuple->Branch("HasDsToTauNu"                , &HasDsToTauNu                , "HasDsToTauNu/I"               ) ;
  ReducedNtuple->Branch("HasDsToTauNuTauToMuNuNu"     , &HasDsToTauNuTauToMuNuNu     , "HasDsToTauNuTauToMuNuNu/I"    ) ;
  ReducedNtuple->Branch("HasDsToTauNuTauToENuNu"      , &HasDsToTauNuTauToENuNu      , "HasDsToTauNuTauToENuNu/I"     ) ;
  ReducedNtuple->Branch("HasDsToTauNuTauToPiNu"       , &HasDsToTauNuTauToPiNu       , "HasDsToTauNuTauToPiNu/I"      ) ;
  ReducedNtuple->Branch("HasDsToTauNuTauToPiPi0Nu"    , &HasDsToTauNuTauToPiPi0Nu    , "HasDsToTauNuTauToPiPi0Nu/I"   ) ;
  ReducedNtuple->Branch("HasDsToTauNuTauToPiPi0Pi0Nu" , &HasDsToTauNuTauToPiPi0Pi0Nu , "HasDsToTauNuTauToPiPi0Pi0Nu/I") ;
  ReducedNtuple->Branch("HasDsToTauNuTauToRhoNu"      , &HasDsToTauNuTauToRhoNu      , "HasDsToTauNuTauToRhoNu/I"     ) ;
  ReducedNtuple->Branch("HasDsToTauNuTauToOmegaPiNu"  , &HasDsToTauNuTauToOmegaPiNu  , "HasDsToTauNuTauToOmegaPiNu/I" ) ;
  ReducedNtuple->Branch("DsDecayRateType"             , &DsDecayRateType             , "DsDecayRateType/I"            ) ;
  ReducedNtuple->Branch("DsDecayRateWeight"           , &DsDecayRateWeight           , "DsDecayRateWeight/F"          ) ;
  
  ReducedNtuple->Branch("MuMinimumIonizing"           , &MuMinimumIonizing           , "MuMinimumIonizing/I"          ) ;
  ReducedNtuple->Branch("MuVeryLoose"                 , &MuVeryLoose                 , "MuVeryLoose/I"                ) ;
  ReducedNtuple->Branch("MuLoose"                     , &MuLoose                     , "MuVeryLoose/I"                ) ;
  ReducedNtuple->Branch("MuTight"                     , &MuTight                     , "MuTight/I"                    ) ;
  ReducedNtuple->Branch("MuVeryTight"                 , &MuVeryTight                 , "MuVeryTight/I"                ) ;
  ReducedNtuple->Branch("MuNNVeryLoose"               , &MuNNVeryLoose               , "MuNNVeryLoose/I"              ) ;
  ReducedNtuple->Branch("MuNNLoose"                   , &MuNNLoose                   , "MuNNVeryLoose/I"              ) ;
  ReducedNtuple->Branch("MuNNTight"                   , &MuNNTight                   , "MuNNTight/I"                  ) ;
  ReducedNtuple->Branch("MuNNVeryTight"               , &MuNNVeryTight               , "MuNNVeryTight/I"              ) ;
  ReducedNtuple->Branch("MuNNVeryLooseFakeRate"       , &MuNNVeryLooseFakeRate       , "MuNNVeryLooseFakeRate/I"      ) ;
  ReducedNtuple->Branch("MuNNLooseFakeRate"           , &MuNNLooseFakeRate           , "MuNNLooseFakeRate/I"          ) ;
  ReducedNtuple->Branch("MuNNTightFakeRate"           , &MuNNTightFakeRate           , "MuNNTightFakeRate/I"          ) ;
  ReducedNtuple->Branch("MuNNVeryTightFakeRate"       , &MuNNVeryTightFakeRate       , "MuNNVeryTightFakeRate/I"      ) ;
  ReducedNtuple->Branch("MuLikeVeryLoose"             , &MuLikeVeryLoose             , "MuLikeVeryLoose/I"            ) ;
  ReducedNtuple->Branch("MuLikeLoose"                 , &MuLikeLoose                 , "MuLikeLoose/I"                ) ;
  ReducedNtuple->Branch("MuLikeTight"                 , &MuLikeTight                 , "MuLikeTight/I"                ) ;
  ReducedNtuple->Branch("MuLikeVeryTight"             , &MuLikeVeryTight             , "MuLikeVeryTight/I"            ) ;
  ReducedNtuple->Branch("MuBDTVeryLoose"              , &MuBDTVeryLoose              , "MuBDTVeryLoose/I"             ) ;
  ReducedNtuple->Branch("MuBDTLoose"                  , &MuBDTLoose                  , "MuBDTLoose/I"                 ) ;
  ReducedNtuple->Branch("MuBDTTight"                  , &MuBDTTight                  , "MuBDTTight/I"                 ) ;
  ReducedNtuple->Branch("MuBDTVeryTight"              , &MuBDTVeryTight              , "MuBDTVeryTight/I"             ) ;
  ReducedNtuple->Branch("MuBDTVeryLooseFakeRate"      , &MuBDTVeryLooseFakeRate      , "MuBDTVeryLooseFakeRate/I"     ) ;
  ReducedNtuple->Branch("MuBDTLooseFakeRate"          , &MuBDTLooseFakeRate          , "MuBDTLooseFakeRate/I"         ) ;
  ReducedNtuple->Branch("MuBDTTightFakeRate"          , &MuBDTTightFakeRate          , "MuBDTTightFakeRate/I"         ) ;
  ReducedNtuple->Branch("MuBDTVeryTightFakeRate"      , &MuBDTVeryTightFakeRate      , "MuBDTVeryTightFakeRate/I"     ) ;
  ReducedNtuple->Branch("MuBDTLoPLoose"               , &MuBDTLoPLoose               , "MuBDTLoPLoose/I"              ) ;
  ReducedNtuple->Branch("MuBDTLoPTight"               , &MuBDTLoPTight               , "MuBDTLoPLoose/I"              ) ;
  ReducedNtuple->Branch("MuSelector"                  , &MuSelector                  , "MuSelector/I"                 ) ;
  						      				     				      
  ReducedNtuple->Branch("ENoCal"                      , &ENoCal                      , "ENoCal/I"                     ) ;
  ReducedNtuple->Branch("EVeryLoose"                  , &EVeryLoose                  , "EVeryLoose/I"                 ) ;
  ReducedNtuple->Branch("ELoose"                      , &ELoose                      , "ELoose/I"                     ) ;
  ReducedNtuple->Branch("ETight"                      , &ETight                      , "ETight/I"                     ) ;
  ReducedNtuple->Branch("EVeryTight"                  , &EVeryTight                  , "EVeryTight/I"                 ) ;
  ReducedNtuple->Branch("PidLHElectrons"              , &PidLHElectrons              , "PidLHElectrons/I"             ) ;
  ReducedNtuple->Branch("EKMSuperLoose"               , &EKMSuperLoose               , "EKMSuperLoose/I"              ) ;
  ReducedNtuple->Branch("EKMVeryLoose"                , &EKMVeryLoose                , "EKMVeryLoose/I"               ) ;	
  ReducedNtuple->Branch("EKMLoose"                    , &EKMLoose                    , "EKMLoose/I"                   ) ;
  ReducedNtuple->Branch("EKMTight"                    , &EKMTight                    , "EKMTight/I"                   ) ;
  ReducedNtuple->Branch("EKMVeryTight"                , &EKMVeryTight                , "EKMVeryTight/I"               ) ;
  ReducedNtuple->Branch("EKMSuperTight"               , &EKMSuperTight               , "EKMSuperTight/I"              ) ;
  ReducedNtuple->Branch("ESelector"                   , &ESelector                   , "ESelector/I"                  ) ;
  						      				     				      
  ReducedNtuple->Branch("KSMSNotAPion"                , &KSMSNotAPion                , "KSMSNotAPion/I"               ) ;
  ReducedNtuple->Branch("KSMSVeryLoose"               , &KSMSVeryLoose               , "KSMSVeryLoose/I"              ) ;
  ReducedNtuple->Branch("KSMSLoose"                   , &KSMSLoose                   , "KSMSLoose/I"                  ) ;
  ReducedNtuple->Branch("KSMSTight"                   , &KSMSTight                   , "KSMSTight/I"                  ) ;
  ReducedNtuple->Branch("KSMSVeryTight"               , &KSMSVeryTight               , "KSMSVeryTight/I"              ) ;
  ReducedNtuple->Branch("KNNNotAPion"                 , &KNNNotAPion                 , "KNNNotAPion/I"                ) ;
  ReducedNtuple->Branch("KNNVeryLoose"                , &KNNVeryLoose                , "KNNVeryLoose/I"               ) ;
  ReducedNtuple->Branch("KNNLoose"                    , &KNNLoose                    , "KNNLoose/I"                   ) ;
  ReducedNtuple->Branch("KNNTight"                    , &KNNTight                    , "KNNTight/I"                   ) ;
  ReducedNtuple->Branch("KNNVeryTight"                , &KNNVeryTight                , "KNNVeryTight/I"               ) ;
  ReducedNtuple->Branch("KLHNotAPion"                 , &KLHNotAPion                 , "KLHNotAPion/I"                ) ;
  ReducedNtuple->Branch("KLHVeryLoose"                , &KLHVeryLoose                , "KLHVeryLoose/I"               ) ;
  ReducedNtuple->Branch("KLHLoose"                    , &KLHLoose                    , "KLHLoose/I"                   ) ;
  ReducedNtuple->Branch("KLHTight"                    , &KLHTight                    , "KLHTight/I"                   ) ;
  ReducedNtuple->Branch("KLHVeryTight"                , &KLHVeryTight                , "KLHVeryTight/I"               ) ;
  ReducedNtuple->Branch("KGLHNotAPion"                , &KGLHNotAPion                , "KGLHNotAPion/I"               ) ;
  ReducedNtuple->Branch("KGLHVeryLoose"               , &KGLHVeryLoose               , "KGLHVeryLoose/I"              ) ;
  ReducedNtuple->Branch("KGLHLoose"                   , &KGLHLoose                   , "KGLHLoose/I"                  ) ;
  ReducedNtuple->Branch("KGLHTight"                   , &KGLHTight                   , "KGLHTight/I"                  ) ;
  ReducedNtuple->Branch("KGLHVeryTight"               , &KGLHVeryTight               , "KGLHVeryTight/I"              ) ;
  ReducedNtuple->Branch("KBDTNotAPion"                , &KBDTNotAPion                , "KBDTNotAPion/I"               ) ;
  ReducedNtuple->Branch("KBDTVeryLoose"               , &KBDTVeryLoose               , "KBDTVeryLoose/I"              ) ;
  ReducedNtuple->Branch("KBDTLoose"                   , &KBDTLoose                   , "KBDTLoose/I"                  ) ;
  ReducedNtuple->Branch("KBDTTight"                   , &KBDTTight                   , "KBDTTight/I"                  ) ;
  ReducedNtuple->Branch("KBDTVeryTight"               , &KBDTVeryTight               , "KBDTVeryTight/I"              ) ;
  ReducedNtuple->Branch("KKMNotAPion"                 , &KKMNotAPion                 , "KKMNotAPion/I"                ) ;
  ReducedNtuple->Branch("KKMVeryLoose"                , &KKMVeryLoose                , "KKMVeryLoose/I"               ) ;
  ReducedNtuple->Branch("KKMLoose"                    , &KKMLoose                    , "KKMLoose/I"                   ) ;
  ReducedNtuple->Branch("KKMTight"                    , &KKMTight                    , "KKMTight/I"                   ) ;
  ReducedNtuple->Branch("KKMVeryTight"                , &KKMVeryTight                , "KKMVeryTight/I"               ) ;
  ReducedNtuple->Branch("KSelector"                   , &KSelector                   , "KSelector/I"                  ) ;
  						      				     				      
  ReducedNtuple->Branch("PiRoyLoose"                  , &PiRoyLoose                  , "PiRoyLoose/I"                 ) ;
  ReducedNtuple->Branch("PiRoyNotAKaon"               , &PiRoyNotAKaon               , "PiRoyNotAKaon/I"              ) ;
  ReducedNtuple->Branch("LHVeryLoose"                 , &PiLHVeryLoose               , "PiLHVeryLoose/I"              ) ;
  ReducedNtuple->Branch("LHLoose"                     , &PiLHLoose                   , "PiLHLoose/I"                  ) ;
  ReducedNtuple->Branch("LHTight"                     , &PiLHTight                   , "PiLHTight/I"                  ) ;
  ReducedNtuple->Branch("LHVeryTight"                 , &PiLHVeryTight               , "PiLHVeryTight/I"              ) ;
  ReducedNtuple->Branch("PiGLHVeryLoose"              , &PiGLHVeryLoose              , "PiGLHVeryLoose/I"             ) ;
  ReducedNtuple->Branch("PiGLHLoose"                  , &PiGLHLoose                  , "PiGLHLoose/I"                 ) ;
  ReducedNtuple->Branch("PiGLHTight"                  , &PiGLHTight                  , "PiGLHTight/I"                 ) ;
  ReducedNtuple->Branch("PiGLHVeryTight"              , &PiGLHVeryTight              , "PiGLHVeryTight/I"             ) ;
  ReducedNtuple->Branch("PiKMSuperLoose"              , &PiKMSuperLoose              , "PiKMSuperLoose/I"             ) ;
  ReducedNtuple->Branch("PiKMVeryLoose"               , &PiKMVeryLoose               , "PiKMVeryLoose/I"              ) ;
  ReducedNtuple->Branch("PiKMLoose"                   , &PiKMLoose                   , "PiKMLoose/I"                  ) ;
  ReducedNtuple->Branch("PiKMTight"                   , &PiKMTight                   , "PiKMTight/I"                  ) ;
  ReducedNtuple->Branch("PiKMVeryTight"               , &PiKMVeryTight               , "PiKMVeryTight/I"              ) ;
  ReducedNtuple->Branch("PiKMSuperTight"              , &PiKMSuperTight              , "PiKMSuperTight/I"             ) ;
  ReducedNtuple->Branch("PiSelector"                  , &PiSelector                  , "PiSelector/I"                 ) ;
  
  ReducedNtuple->Branch("Extra1px"                    , &extra1px                    , "Extra1px/F"                   ) ;
  ReducedNtuple->Branch("Extra1py"                    , &extra1py                    , "Extra1py/F"                   ) ;
  ReducedNtuple->Branch("Extra1pz"                    , &extra1pz                    , "Extra1pz/F"                   ) ;
  ReducedNtuple->Branch("Extra1p3"                    , &extra2p3                    , "Extra2p3/F"                   ) ;
  ReducedNtuple->Branch("Extra2px"                    , &extra2px                    , "Extra2px/F"                   ) ;
  ReducedNtuple->Branch("Extra2py"                    , &extra2py                    , "Extra2py/F"                   ) ;
  ReducedNtuple->Branch("Extra2pz"                    , &extra2pz                    , "Extra2pz/F"                   ) ;
  ReducedNtuple->Branch("Extra2p3"                    , &extra2p3                    , "Extra2p3/F"                   ) ;
  ReducedNtuple->Branch("ExtraMPiPi"                  , &extraMPiPi                  , "ExtraMPiPi/F"                 ) ;
  ReducedNtuple->Branch("ExtraMPiK"                   , &extraMPiK                   , "ExtraMPiK/F"                  ) ;
  ReducedNtuple->Branch("ExtraMKPi"                   , &extraMKPi                   , "ExtraMKPi/F"                  ) ;
  ReducedNtuple->Branch("ExtraMKK"                    , &extraMKK                    , "ExtraMKK/F"                   ) ;

  ReducedNtuple->Branch("eeE"                         , &eeE                         , "eeE/F"                        ) ;
  ReducedNtuple->Branch("DsToEllNuDsMass"             , &evtDsMass                   , "DsToEllNuDsMass/F"            ) ;
  ReducedNtuple->Branch("hits"                        , &hits                        , "hits/I"                       ) ;
  ReducedNtuple->Branch("SignalMode"                  , &signalMode                  , "SignalMode/I"                 ) ;
  ReducedNtuple->Branch("Ellp3Lab"                    , &Ellp3Lab                    , "Ellp3Lab/F"                   ) ;
  ReducedNtuple->Branch("run"                         , &run                         , "run/I"                        ) ;
  ReducedNtuple->Branch("SPMode"                      , &SPMode                      , "SPMode/I"                     ) ;
  ReducedNtuple->Branch("weight"                      , &weight[run][SPMode]         , "weight/F"                     ) ;

  MakeDsInclusiveReducedNtupleBranches() ;
  cout << "Done Making DsToEllNu Reduced Ntuple Branches" << endl ;
  return 1 ;
}

