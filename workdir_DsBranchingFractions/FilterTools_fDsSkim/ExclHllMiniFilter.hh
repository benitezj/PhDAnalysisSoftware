//--------------------------------------------------------------------------
// File and Version Information:
// 	
//
// Description:
//      Class ExclHllMiniFilter
//
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Jonathan Hollar
//
//
// Copyright Information:
//
//
//------------------------------------------------------------------------

#ifndef EXCLHLLMINIFILTER_HH
#define EXCLHLLMINIFILTER_HH

//----------------------
// Base Class Headers --
//---------------------
#include "CLHEP/Alist/AList.h"
#include "CLHEP/Matrix/Vector.h"
#include "CLHEP/Vector/LorentzVector.h"

#include "Framework/AppFilterModule.hh"
#include "Framework/AbsParmBool.hh"
#include "Framework/AbsParmDouble.hh"
#include "Framework/AbsParmGeneral.hh"
#include "Framework/AbsParmString.hh"
#include "AbsParm/AbsParmIfdStrKey.hh"

#include "TagModules/TagFilterModule.hh"

#include "UsrData/UsrData.hh"

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
class HepHistogram;
class HepTupleManager;
class HepTuple;
class BtaCandidate;
template <class T> class HepAList;

//		---------------------
// 		-- Class Interface --
//		---------------------
 
//class ExclHllMiniFilter : public AppFilterModule {
class ExclHllMiniFilter : public TagFilterModule {

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  ExclHllMiniFilter( const char* const theName, 
		     const char* const theDescription );

  // Destructor
  virtual ~ExclHllMiniFilter( );

  // Operations
  
  virtual AppResult beginJob( AbsEvent* anEvent );
  virtual AppResult event( AbsEvent* anEvent );
  virtual AppResult endJob( AbsEvent* anEvent );
    

protected:
  // helpers
  bool isMember( const BtaCandidate& c,
		const HepAList<BtaCandidate>& l ) const;
  
  bool doHadModes( BtaCandidate* trackPlus, 
		   BtaCandidate* trackMinus, 
		   int leptMode );

  void doCharmonium( BtaCandidate* trackPlus, 
		     BtaCandidate* trackMinus,
		     int leptMode );
  
  bool isKMode( BtaCandidate* trackPlus, BtaCandidate* trackMinus );
  bool isKsMode (BtaCandidate* trackPlus, BtaCandidate* trackMinus );
  bool isKstarChMode( BtaCandidate* trackPlus, BtaCandidate* trackMinus );
  bool isKstarChPi0Mode( BtaCandidate* trackPlus, BtaCandidate* trackMinus );
  bool isKstar0Mode( BtaCandidate* trackPlus, BtaCandidate* trackMinus );
  bool isKstar0Pi0Mode( BtaCandidate* trackPlus, BtaCandidate* trackMinus );
  bool isRho0Mode( BtaCandidate* trackPlus, BtaCandidate* trackMinus );
  bool isRhoChMode( BtaCandidate* trackPlus, BtaCandidate* trackMinus );
  bool isPiChMode( BtaCandidate* trackPlus, BtaCandidate* trackMinus );
  bool isPi0Mode( BtaCandidate* trackPlus, BtaCandidate* trackMinus );
  bool isOmegaMode( BtaCandidate* trackPlus, BtaCandidate* trackMinus );
  bool isPhiMode( BtaCandidate* trackPlus, BtaCandidate* trackMinus );
  bool isEtaMode( BtaCandidate* trackPlus, BtaCandidate* trackMinus );
  bool isEtaPrMode( BtaCandidate* trackPlus, BtaCandidate* trackMinus );

  bool filterCommonInfo( const BtaCandidate* trackPlus, 
			 const BtaCandidate* trackMinus,
			 const BtaCandidate* hadron );

  bool isBremOverlap(const BtaCandidate* trackPlus, 
		     const BtaCandidate* trackMinus,
		     const BtaCandidate* compHad);


protected:
  AbsEvent* _anEvent;

  int   _nReadEvents; 
  int   _nFound; 
  int   _multiCands[32];
  int   _eventCands[32];
  int   _tempHadMode[4];

  float _beamEn;
  HepLorentzVector _BP4;
  AbsParmDouble _deltaEcut;
  AbsParmDouble _mEScut;
  AbsParmDouble _eTrackCut;
  AbsParmDouble _muTrackCut;
  AbsParmDouble _nTrkCut;
  AbsParmDouble _r2Cut;
  
  // Hadron mass cuts
  AbsParmDouble _lowKsMass;
  AbsParmDouble _highKsMass;
  AbsParmDouble _lowPiZeroMass;
  AbsParmDouble _highPiZeroMass;
  AbsParmDouble _lowKstarMass;
  AbsParmDouble _highKstarMass;
  AbsParmDouble _lowRhoMass;
  AbsParmDouble _highRhoMass;
  AbsParmDouble _lowPhiMass;
  AbsParmDouble _highPhiMass;
  AbsParmDouble _lowOmegaMass;
  AbsParmDouble _highOmegaMass;
  AbsParmDouble _lowEtaMass;
  AbsParmDouble _highEtaMass;
  AbsParmDouble _lowEtaPrMass;
  AbsParmDouble _highEtaPrMass;
  
  // User data
  AbsParmIfdStrKey    _eventData;
  AbsParmIfdStrKey    _candData;
  UsrEventBlock       _eventBlock;
  UsrCandBlock        _candBlock;
  UsrVariable< int >  _isccbar;       // charmonium bitmap
  UsrVariable< int >  _hadModeEE;     // e+e- bitmap
  UsrVariable< int >  _hadModeMM;     // mu+mu- bitmap
  UsrVariable< int >  _hadModeEM;     // e+mu- bitmap
  UsrVariable< int >  _hadModeME;     // mu+e- bitmap
  UsrVariable< int >   _candHadMode;
  UsrVariable< int >   _candLeptMode;
  UsrVariable< float > _candMes;
  UsrVariable< float > _candDeltaE;
  UsrVariable< float > _candMesonMass;

  AbsParmIfdStrKey _outputBList;
  AbsParmIfdStrKey _chargedListKey;   // list of charged tracks
  AbsParmIfdStrKey _neutralListKey;   // list of neutrals
  AbsParmIfdStrKey _trkListKey;       // list of brem-recoed lepton cands.
  AbsParmIfdStrKey _idMuListKey;
  AbsParmIfdStrKey _idElListKey;
  AbsParmIfdStrKey _ksListKey;        // list of K0s candidates
  AbsParmIfdStrKey _piZeroListKey;    // list of pi0 candidates
  AbsParmIfdStrKey _etaListKey;       // list of eta candidates
  AbsParmIfdStrKey _kStarCKsListKey;
  AbsParmIfdStrKey _kStarCKListKey;
  AbsParmIfdStrKey _kStar0KListKey;
  AbsParmIfdStrKey _kStar0KsListKey;
  AbsParmIfdStrKey _rhoChListKey;
  AbsParmIfdStrKey _rhoZeroListKey;
  AbsParmIfdStrKey _omegaListKey;
  AbsParmIfdStrKey _phiListKey;
  AbsParmIfdStrKey _etaPrListKey;
  
  HepAList< BtaCandidate >* _chargedList;
  HepAList< BtaCandidate >* _neutralList;
  HepAList< BtaCandidate >* _btaTrackList;
  HepAList< BtaCandidate >* _idElList;
  HepAList< BtaCandidate >* _idMuList;
  HepAList< BtaCandidate >  _leptonList;
  HepAList< BtaCandidate >  _noIDLeptonList;
  HepAList< BtaCandidate >* _bCandList;
  HepAList< BtaCandidate >* _ksInputList; 
  HepAList< BtaCandidate >* _piZeroInputList;
  HepAList< BtaCandidate >* _etaInputList;
  HepAList< BtaCandidate >* _kStarCKsInputList;
  HepAList< BtaCandidate >* _kStarCKInputList; 
  HepAList< BtaCandidate >* _kStar0KInputList;
  HepAList< BtaCandidate >* _kStar0KsInputList;
  HepAList< BtaCandidate >* _rhoChInputList;
  HepAList< BtaCandidate >* _rhoZeroInputList;
  HepAList< BtaCandidate >* _omegaInputList;
  HepAList< BtaCandidate >* _phiInputList;
  HepAList< BtaCandidate >* _etaPrInputList;

  AbsParmDouble    _debug;

  // Mode switches 
  AbsParmBool      _firstCandOnly;
  AbsParmBool      _doNtp;
  AbsParmBool      _doKll;
  AbsParmBool      _doKsll;
  AbsParmBool      _doKstarChll;
  AbsParmBool      _doKstarChPi0ll;
  AbsParmBool      _doKstar0ll;
  AbsParmBool      _doKstar0Pi0ll;
  AbsParmBool      _doPiChll;
  AbsParmBool      _doPi0ll;
  AbsParmBool      _doRhoChll;
  AbsParmBool      _doRho0ll;
  AbsParmBool      _doOmegall;
  AbsParmBool      _doPhill;
  AbsParmBool      _doEtall;
  AbsParmBool      _doEtaPrll;
  AbsParmBool      _doEE;
  AbsParmBool      _doMuMu;
  AbsParmBool      _doEMu;
  AbsParmBool      _doMuE;

  // define "local" variables to store from event to event
  // see the "begin" method for a discussion of these
  HepTuple*        _ntuple;
};

#endif

