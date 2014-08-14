#ifndef DRecoDsToEllNuFit_HH
#define DRecoDsToEllNuFit_HH 1

//----------------------
// Base Class Headers --
//----------------------
#include "TagModules/TagFilterModule.hh"

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------

#include "AbsParm/AbsParmIfdStrKey.hh"
#include "BetaMiniUser/NamedScalers.hh"
#include "Framework/AbsParmDouble.hh"
#include "Framework/AbsParmBool.hh"
#include "UsrData/UsrData.hh"
class AbsEvent ;

//---------------------
// Class Interface --
//---------------------
 
class DRecoDsToEllNuFit : public TagFilterModule {

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  DRecoDsToEllNuFit( const char* const N, const char* const D ) ;
  
  // Destructor
  virtual ~DRecoDsToEllNuFit( ) ;
  
  // Operations
  
  virtual AppResult beginJob( AbsEvent* anEvent ) ;
  virtual AppResult event   ( AbsEvent* anEvent ) ;
  virtual AppResult endJob  ( AbsEvent* anEvent ) ;
  HepLorentzVector nup4Corrected(HepLorentzVector nup4, HepLorentzVector Ep4, int signal_mode)  ;
  
protected:

  AbsParmDouble    FitProbCut       ;
  AbsParmIfdStrKey muonList         ;
  AbsParmIfdStrKey electronList     ;
  AbsParmIfdStrKey trackList        ;
  AbsParmIfdStrKey chargedList      ;
  AbsParmIfdStrKey neutralList      ;
  AbsParmDouble    ExGamEnergyCutHi ;

private:
  NamedScalers     _scalers          ; 
  UsrCandBlock     DRecoUsrBlock     ;
  AbsParmIfdStrKey DRecoUsrBlockName ;

  UsrVariable<float> TagUncMass  ;
  UsrVariable<float> DsStUncMass ;
  UsrVariable<float> DsUncMass   ;
  UsrVariable<float> DsMass      ;

  UsrVariable<int>   qualMu      ;
  UsrVariable<int>   qualE       ;

  UsrVariable<int>   MyGamMCIdx  ;
  UsrVariable<int>   MyTagMCIdx  ;
  UsrVariable<int>   MyKMCIdx    ;
  UsrVariable<int>   MyMuMCIdx   ;
  UsrVariable<int>   MyEMCIdx    ;

  UsrVariable<float> mm2corrected ;
  UsrVariable<float> Deltap3      ;
  UsrVariable<int>   DsToMuNu     ;
  UsrVariable<int>   DsToENu      ;

  UsrVariable<float> ProbChi2     ;
  UsrVariable<float> Chi2         ;
  UsrVariable<float> ndof         ;

  UsrVariable<float> NuMassRaw    ;
  UsrVariable<int>   SignalMode   ;

  UsrVariable<float> NupxRaw      ; 
  UsrVariable<float> NupyRaw      ;
  UsrVariable<float> NupzRaw      ;
  UsrVariable<float> NuenergyRaw  ;
  UsrVariable<float> MupxRaw      ;
  UsrVariable<float> MupyRaw      ;
  UsrVariable<float> MupzRaw      ;
  UsrVariable<float> MuenergyRaw  ;
  UsrVariable<float> NupxCorr     ;
  UsrVariable<float> NupyCorr     ;
  UsrVariable<float> NupzCorr     ;
  UsrVariable<float> NuenergyCorr ;

  UsrVariable<float> ThrustX   ;
  UsrVariable<float> ThrustY   ;
  UsrVariable<float> ThrustZ   ;
  UsrVariable<float> ThrustMag ;

  UsrVariable<float> R0 ;
  UsrVariable<float> R1 ;
  UsrVariable<float> R2 ;
  UsrVariable<float> R3 ;
  UsrVariable<float> R4 ;

  UsrVariable<float> EExtraNeutral ;
  UsrVariable<int>   NExtraNeutral ;

  UsrVariable<float> PMissCosTh ;
  UsrVariable<float> PMissPhi   ;
  UsrVariable<float> PMiss      ;
  UsrVariable<float> EMiss      ;

} ;

#endif
