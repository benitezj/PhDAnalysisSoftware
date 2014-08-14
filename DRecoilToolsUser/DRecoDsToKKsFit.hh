
#ifndef DRecoDsToKKsFit_HH
#define DRecoDsToKKsFit_HH 1

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
 
class DRecoDsToKKsFit : public TagFilterModule {

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  DRecoDsToKKsFit( const char* const N, const char* const D ) ;
  
  // Destructor
  virtual ~DRecoDsToKKsFit( ) ;
  
  // Operations
  
  virtual AppResult beginJob( AbsEvent* anEvent ) ;
  virtual AppResult event   ( AbsEvent* anEvent ) ;
  virtual AppResult endJob  ( AbsEvent* anEvent ) ;
  HepLorentzVector nup4Corrected(HepLorentzVector nup4, HepLorentzVector Ep4, int signal_mode)  ;
  
protected:

  AbsParmDouble    FitProbCut       ;
  AbsParmIfdStrKey KChargeList      ;
  AbsParmIfdStrKey trackList        ;
  AbsParmIfdStrKey chargedList      ;
  AbsParmIfdStrKey neutralList      ;
  AbsParmIfdStrKey KsList           ;

private:
  NamedScalers     _scalers          ; 
  UsrCandBlock     DRecoUsrBlock     ;
  AbsParmIfdStrKey DRecoUsrBlockName ;

  UsrVariable<float> TagUncMass  ;
  UsrVariable<float> DsStUncMass ;
  UsrVariable<float> DsUncMass   ;
  UsrVariable<float> DsMass      ;

  UsrVariable<int>   qualSigK    ;

  UsrVariable<int>   MyGamMCIdx  ;
  UsrVariable<int>   MyTagMCIdx  ;
  UsrVariable<int>   MyKMCIdx    ;
  UsrVariable<int>   MySigKMCIdx ;


  UsrVariable<float> ProbChi2MMRaw     ;
  UsrVariable<float> Chi2MMRaw         ;
  UsrVariable<float> ndofMMRaw         ;
  UsrVariable<float> KsmassMMRaw       ;
  UsrVariable<float> KspxMMRaw         ; 
  UsrVariable<float> KspyMMRaw         ;
  UsrVariable<float> KspzMMRaw         ;
  UsrVariable<float> KsenergyMMRaw     ;
  UsrVariable<float> KpxMMRaw          ;
  UsrVariable<float> KpyMMRaw          ;
  UsrVariable<float> KpzMMRaw          ;
  UsrVariable<float> KenergyMMRaw      ;
  UsrVariable<float> DsmassMMRaw       ;
  UsrVariable<float> DspxMMRaw         ; 
  UsrVariable<float> DspyMMRaw         ;
  UsrVariable<float> DspzMMRaw         ;
  UsrVariable<float> DsenergyMMRaw     ;


  UsrVariable<float> ProbChi2MMCFit     ;
  UsrVariable<float> Chi2MMCFit         ;
  UsrVariable<float> ndofMMCFit         ;
  UsrVariable<float> KsmassMMCFit       ;
  UsrVariable<float> KspxMMCFit         ; 
  UsrVariable<float> KspyMMCFit         ;
  UsrVariable<float> KspzMMCFit         ;
  UsrVariable<float> KsenergyMMCFit     ;
  UsrVariable<float> KpxMMCFit          ;
  UsrVariable<float> KpyMMCFit          ;
  UsrVariable<float> KpzMMCFit          ;
  UsrVariable<float> KenergyMMCFit      ;
  UsrVariable<float> DsmassMMCFit       ;
  UsrVariable<float> DspxMMCFit         ; 
  UsrVariable<float> DspyMMCFit         ;
  UsrVariable<float> DspzMMCFit         ;
  UsrVariable<float> DsenergyMMCFit     ;

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

  //new variables
  UsrVariable<float> DspxListUFit        ; 
  UsrVariable<float> DspyListUFit        ;
  UsrVariable<float> DspzListUFit        ;
  UsrVariable<float> DsenergyListUFit    ;
  UsrVariable<float> DsmassListUFit      ;
  UsrVariable<float> KpxListUFit         ;
  UsrVariable<float> KpyListUFit         ;
  UsrVariable<float> KpzListUFit         ;
  UsrVariable<float> KenergyListUFit     ;
  UsrVariable<float> KmassListUFit       ;
  UsrVariable<float> KspxListUFit        ; 
  UsrVariable<float> KspyListUFit        ;
  UsrVariable<float> KspzListUFit        ;
  UsrVariable<float> KsenergyListUFit    ;
  UsrVariable<float> KsmassListUFit      ;
  UsrVariable<float> Ksd1pxListUFit      ; 
  UsrVariable<float> Ksd1pyListUFit      ;
  UsrVariable<float> Ksd1pzListUFit      ;
  UsrVariable<float> Ksd1energyListUFit  ;
  UsrVariable<float> Ksd1massListUFit    ;
  UsrVariable<float> Ksd2pxListUFit      ; 
  UsrVariable<float> Ksd2pyListUFit      ;
  UsrVariable<float> Ksd2pzListUFit      ;
  UsrVariable<float> Ksd2energyListUFit  ;
  UsrVariable<float> Ksd2massListUFit    ;
  UsrVariable<float> ProbChi2ListUFit    ;
  UsrVariable<float> Chi2ListUFit        ;
  UsrVariable<float> ndofListUFit        ;
  UsrVariable<int>   FitStatusUFit       ;

  UsrVariable<float> DspxListCFit        ; 
  UsrVariable<float> DspyListCFit        ;
  UsrVariable<float> DspzListCFit        ;
  UsrVariable<float> DsenergyListCFit    ;
  UsrVariable<float> DsmassListCFit      ;
  UsrVariable<float> KpxListCFit         ;
  UsrVariable<float> KpyListCFit         ;
  UsrVariable<float> KpzListCFit         ;
  UsrVariable<float> KenergyListCFit     ;
  UsrVariable<float> KmassListCFit       ;
  UsrVariable<float> KspxListCFit        ; 
  UsrVariable<float> KspyListCFit        ;
  UsrVariable<float> KspzListCFit        ;
  UsrVariable<float> KsenergyListCFit    ;
  UsrVariable<float> KsmassListCFit      ;
  UsrVariable<float> Ksd1pxListCFit      ; 
  UsrVariable<float> Ksd1pyListCFit      ;
  UsrVariable<float> Ksd1pzListCFit      ;
  UsrVariable<float> Ksd1energyListCFit  ;
  UsrVariable<float> Ksd1massListCFit    ;
  UsrVariable<float> Ksd2pxListCFit      ; 
  UsrVariable<float> Ksd2pyListCFit      ;
  UsrVariable<float> Ksd2pzListCFit      ;
  UsrVariable<float> Ksd2energyListCFit  ;
  UsrVariable<float> Ksd2massListCFit    ;
  UsrVariable<float> ProbChi2ListCFit    ;
  UsrVariable<float> Chi2ListCFit        ;
  UsrVariable<float> ndofListCFit        ;
  UsrVariable<int>   FitStatusCFit       ;

  UsrVariable<float> DspxListRaw        ; 
  UsrVariable<float> DspyListRaw        ;
  UsrVariable<float> DspzListRaw        ;
  UsrVariable<float> DsenergyListRaw    ;
  UsrVariable<float> DsmassListRaw      ;
  UsrVariable<float> KpxListRaw         ;
  UsrVariable<float> KpyListRaw         ;
  UsrVariable<float> KpzListRaw         ;
  UsrVariable<float> KenergyListRaw     ;
  UsrVariable<float> KmassListRaw       ;
  UsrVariable<float> KspxListRaw        ; 
  UsrVariable<float> KspyListRaw        ;
  UsrVariable<float> KspzListRaw        ;
  UsrVariable<float> KsenergyListRaw    ;
  UsrVariable<float> KsmassListRaw      ;
  UsrVariable<float> Ksd1pxListRaw      ; 
  UsrVariable<float> Ksd1pyListRaw      ;
  UsrVariable<float> Ksd1pzListRaw      ;
  UsrVariable<float> Ksd1energyListRaw  ;
  UsrVariable<float> Ksd1massListRaw    ;
  UsrVariable<float> Ksd2pxListRaw      ; 
  UsrVariable<float> Ksd2pyListRaw      ;
  UsrVariable<float> Ksd2pzListRaw      ;
  UsrVariable<float> Ksd2energyListRaw  ;
  UsrVariable<float> Ksd2massListRaw    ;
  UsrVariable<float> ProbChi2ListRaw    ;
  UsrVariable<float> Chi2ListRaw        ;
  UsrVariable<float> ndofListRaw        ;

} ;

#endif
