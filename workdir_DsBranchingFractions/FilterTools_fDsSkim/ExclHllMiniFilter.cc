//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: ExclHllMiniFilter.cc,v 1.26 2004/10/20 12:36:45 fnc Exp $
//
// Description:
//      ExclHllMiniFilter - no particle ID mini filter for
//      exclusive sll and dll. Bitmaps flagging signal candidates and
//      charmonium candidates by mode are attached to the output event 
//      as user data   
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

#include "BaBar/BaBar.hh"
//-----------------------
// This Class's Header --
//-----------------------
#include "FilterTools/ExclHllMiniFilter.hh"   

//-------------
// C Headers --
//-------------
#include <assert.h>

//---------------
// C++ Headers --
//---------------

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "AbsEvent/AbsEvent.hh"
#include "AbsEvent/getTmpAList.hh"

#include "AbsEventTag/AbsEventTag.hh"
#include "AbsEventTag/AbsEventTagBoolIter.hh"
#include "AbsEventTag/AbsEventTagIntIter.hh"
#include "AbsEventTag/AbsEventTagFloatIter.hh"

#include "AbsEnv/AbsEnv.hh"

#include "BbrGeom/BbrDoubleErr.hh"
#include "BbrGeom/BbrLorentzVectorErr.hh"   
#include "BbrGeom/BbrPointErr.hh"
#include "BbrGeom/BbrVectorErr.hh"

#include "Beta/BtaCandidate.hh"

#include "BetaCoreTools/BtaBooster.hh"
#include "BetaCoreTools/BtaDecayMode.hh"
#include "BetaCoreTools/BtaFoxWolfMom.hh"
#include "BetaCoreTools/BtaMcAssoc.hh"
#include "BetaCoreTools/BtaOpAdd4.hh"
#include "BetaCoreTools/BtaOpMakeTree.hh"
#include "BetaCoreTools/BtaThrust.hh"

#include "BtaEnv/BtaEnv.hh" 

#include "CLHEP/Alist/AList.h"
#include "CLHEP/Alist/AIterator.h"

#include "ErrLogger/ErrLog.hh"

#include "GenEnv/GenEnv.hh"

#include "HepTuple/Tuple.h"
#include "HepTuple/TupleManager.h"
#include "HepTuple/Histogram.h"

#include "ProxyDict/IfdStrKey.hh"
#include "ProxyDict/IfdKey.hh"
#include "ProxyDict/Ifd.hh"

#include "PDT/Pdt.hh"
#include "PDT/PdtEntry.hh"
#include "PDT/PdtPid.hh"

#include "VtxFitter/VtxFitterOper.hh" 
using std::cout;
using std::dec;
using std::endl;
using std::hex;

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

//----------------
// Constructors --
//----------------

ExclHllMiniFilter::ExclHllMiniFilter( const char* const theName, 
				      const char* const theDescription )
  : TagFilterModule( theName, theDescription )
  //  : AppFilterModule( theName, theDescription )
  , _deltaEcut("deltaEcut",this,0.25)
  , _mEScut("mEScut",this,5.2)
  , _eTrackCut("eTrackCut",this,0.3)
  , _muTrackCut("muTrackCut",this,0.7)
  , _nTrkCut("nTrkCut",this,4)
  , _r2Cut("r2Cut",this,1.0)
  , _lowKsMass("lowKsMass",this,0.46)
  , _highKsMass("highKsMass",this,0.54)
  , _lowPiZeroMass("lowPiZeroMass",this,0.09)
  , _highPiZeroMass("highPiZeroMass",this,0.165)
  , _lowKstarMass("lowKstarMass",this,0.7)
  , _highKstarMass("highKstarMass",this,1.1)
  , _lowRhoMass("lowRhoMass",this,0.4)
  , _highRhoMass("highRhoMass",this,1.3)
  , _lowPhiMass("lowPhiMass",this,0.96)
  , _highPhiMass("highPhiMass",this,1.06)
  , _lowOmegaMass("lowOmegaMass",this,0.70)
  , _highOmegaMass("highOmegaMass",this,0.85)
  , _lowEtaMass("lowEtaMass",this,0.47)
  , _highEtaMass("highEtaMass",this,0.62)
  , _lowEtaPrMass("lowEtaPrMass",this,0.9)
  , _highEtaPrMass("highEtaPrMass",this,1.0)

  , _eventData("eventData",this,"UsrExclHllData")
  , _candData("candData",this,"UsrExclHllCandData")
  , _isccbar("isccbar")
  , _hadModeEE("hadModeEE")
  , _hadModeMM("hadModeMM")
  , _hadModeEM("hadModeEM")
  , _hadModeME("hadModeME")
  , _candHadMode("candHadMode")
  , _candLeptMode("candLeptMode")
  , _candMes("candMes")
  , _candDeltaE("candDeltaE")
  , _candMesonMass("candMesonMass")
  , _outputBList("outputBlist",this,"ExclHllSkimmedBList")
  , _chargedListKey("chargedList",this,"ChargedTracks")
  , _neutralListKey("neutralList",this,"CalorNeutral")
  , _trkListKey("btaTrackList",this,"ChargedTracks")
  , _idMuListKey("idMuList",this,"muNNVeryLoose")
  , _idElListKey("idElList",this,"eMicroLoose")
  , _ksListKey("ksInputList",this,"KsDefault")
  , _piZeroListKey("piZeroInputList",this,"pi0VeryLoose")
  , _etaListKey("etaInputList",this,"etaggDefault")
  , _kStarCKsListKey("kStarCKsInputList",this,"KstarKsPiDefault")
  , _kStarCKListKey("kStarCKInputList",this,"KstarKPi0Default")
  , _kStar0KListKey("kStar0KInputList",this,"KstarKPiDefault")
  , _kStar0KsListKey("kStar0KsInputList",this,"KstarKsPi0Default")
  , _rhoChListKey("rhoChInputList",this,"rhoCDefault")
  , _rhoZeroListKey("rhoZeroInputList",this,"rho0Default")
  , _omegaListKey("omegaInputList",this,"omegaDefault")
  , _phiListKey("phiInputList",this,"phiDefault")
  , _etaPrListKey("etaPrInputList",this,"etaPeppDefault")

  , _debug("debug",this,0)

  , _firstCandOnly("firstCandOnly",this,false)
  , _doNtp("doNtp",this,false)
  , _doKll("doKll",this,true)
  , _doKsll("doKsll",this,true)
  , _doKstarChll("doKstarChll",this,true)
  , _doKstarChPi0ll("doKstarChPi0ll",this,true)
  , _doKstar0ll("doKstar0ll",this,true)
  , _doKstar0Pi0ll("doKstar0Pi0ll",this,true)
  , _doPiChll("doPiChll",this,true)
  , _doPi0ll("doPi0ll",this,true)
  , _doRhoChll("doRhoChll",this,true)
  , _doRho0ll("doRho0ll",this,true)
  , _doOmegall("doOmegall",this,true)
  , _doPhill("doPhill",this,true)
  , _doEtall("doEtall",this,true)
  , _doEtaPrll("doEtaPrll",this,true)
  , _doEE("doEE",this,true)
  , _doMuMu("doMuMu",this,true)
  , _doEMu("doEMu",this,true)
  , _doMuE("doMuE",this,true)
{
  commands()->append( &_debug );
  commands()->append( &_r2Cut );
  commands()->append( &_nTrkCut );
  commands()->append( &_chargedListKey );
  commands()->append( &_neutralListKey );
  commands()->append( &_trkListKey );
  commands()->append( &_idMuListKey );
  commands()->append( &_idElListKey );
  commands()->append( &_ksListKey );
  commands()->append( &_piZeroListKey );
  commands()->append( &_etaListKey );
  commands()->append( &_kStarCKsListKey );
  commands()->append( &_kStarCKListKey );
  commands()->append( &_kStar0KListKey );
  commands()->append( &_kStar0KsListKey );
  commands()->append( &_rhoChListKey );
  commands()->append( &_rhoZeroListKey );
  commands()->append( &_omegaListKey );
  commands()->append( &_phiListKey );
  commands()->append( &_etaPrListKey );
  commands()->append( &_eventData );
  commands()->append( &_firstCandOnly );
  commands()->append( &_doNtp );
  commands()->append( &_doKll );
  commands()->append( &_doKsll );
  commands()->append( &_doKstarChll );
  commands()->append( &_doKstarChPi0ll );
  commands()->append( &_doKstar0ll );
  commands()->append( &_doKstar0Pi0ll );
  commands()->append( &_doPiChll );
  commands()->append( &_doPi0ll );
  commands()->append( &_doRho0ll );
  commands()->append( &_doRhoChll );
  commands()->append( &_doOmegall );
  commands()->append( &_doPhill );
  commands()->append( &_doEtall );
  commands()->append( &_doEtaPrll );
  commands()->append( &_doEE );
  commands()->append( &_doMuMu );
  commands()->append( &_doEMu );
  commands()->append( &_doMuE ); 
  commands()->append( &_mEScut );
  commands()->append( &_deltaEcut );
  commands()->append( &_eTrackCut );
  commands()->append( &_muTrackCut );
  commands()->append( &_outputBList );

  _eventBlock.addVariable( _isccbar );
  _eventBlock.addVariable( _hadModeEE );
  _eventBlock.addVariable( _hadModeMM );
  _eventBlock.addVariable( _hadModeEM );
  _eventBlock.addVariable( _hadModeME );
  _candBlock.addVariable( _candMes );
  _candBlock.addVariable( _candDeltaE );
  _candBlock.addVariable( _candMesonMass );
  _candBlock.addVariable( _candHadMode );
  _candBlock.addVariable( _candLeptMode );
}


//--------------
// Destructor --
//--------------

// The destructor should be limited to undoing the work of the constructor
ExclHllMiniFilter::~ExclHllMiniFilter( )
{
}

//--------------
// Operations --
//--------------

// The beginJob( AbsEvent* ) member function is run before any events are
// processed.  

AppResult
ExclHllMiniFilter::beginJob( AbsEvent* anEvent) 
{
  ErrMsg(trace) << name() << ": begin Job" << endmsg; 
  
  if ( _doNtp.value() ) 
    {
      HepTupleManager* manager = gblEnv->getGen()->ntupleManager();
      assert( 0 != manager );
      _ntuple = manager->ntuple("ExclHllSkim");      
      // book histograms  (arguments are nBins, low edge, high edge,
      //                   comment is the PAW histogram number)
      
      ErrMsg(trace) << name( ) << " ========> histogram booking <========== " 
		    <<endmsg;
      
    }

  // event countings and flags
  _nReadEvents = 0 ; 
  _nFound = 0 ;

  return AppResult::OK;
}

AppResult 
ExclHllMiniFilter::event( AbsEvent* anEvent ) 
{
  bool signalcand(false);
  bool passed(false);
  int i(0);

  setPassed( passed );

  _nReadEvents++;

  // All bit maps are reset to 0 for each event
  _isccbar = 0;
  _hadModeEE = 0;
  _hadModeMM = 0;
  _hadModeEM = 0;
  _hadModeME = 0;

  for(i = 0;i < 32;i++)
    _eventCands[i] = 0;

  for(i = 0;i < 4;i++)
    _tempHadMode[i] = 0;

  if( _debug.value() >= 1)
    cout << "ExclHllMiniFilter: passed tag pre-filter, starting event" << endl;
  
  _anEvent = anEvent;

  // Get beam energy
  _beamEn = gblEnv->getPep()->pepBeams()->energyCMWErr().value()/2.0;

  // get charged tracks
  getTmpAList( anEvent, _chargedList, _chargedListKey.value() );
  
  // get neutrals
  getTmpAList( anEvent, _neutralList, _neutralListKey.value() );

  int nTrks = _chargedList->length();
  int nNeutral = _neutralList->length();
  
  // Get output B candidate list
  getTmpAList( anEvent, _bCandList, _outputBList.value() );

  // Get a list of all "leptons" in the event that pass minimum momentum cut
  BtaCandidate* lepton(0);
  BtaCandidate* tempLepton(0);
  BtaCandidate* baseLepton(0);
  getTmpAList(anEvent, _btaTrackList, _trkListKey.value());

  getTmpAList(anEvent, _idElList, _idElListKey.value());
  getTmpAList(anEvent, _idMuList, _idMuListKey.value());

  HepAListIterator<BtaCandidate> itTrk( *_btaTrackList );
  bool keepThis;
  bool passes;
  while ( (lepton = itTrk()) ) 
    {
      if( _debug.value() >= 2 )
	cout << "lepton cand with p = " << lepton->p() << ", m = " 
	     << lepton->mass() << endl;

      keepThis = true;
      passes = false;

      // Kludge to eliminate low-energy leptons "promoted" into passing by
      // brem-recovery
      if( lepton->isComposite() )
	{
	  HepAListIterator<BtaCandidate> leptIter = 
	    lepton->daughterIterator();
	  
	  while( (tempLepton = leptIter()) )
	    {
	      if( _debug.value() >= 2 )
		cout << "\tlepton daughter with charge = " 
		     << tempLepton->charge() << ", p = " 
		     <<	tempLepton->p() << endl;

	      if( (tempLepton->charge() != 0) &&
		  (tempLepton->p() >= _eTrackCut.value()) )
		{
		  baseLepton = tempLepton;
		  _leptonList.append( *lepton );
		  passes = true;
		}
	    }
	}
      // If not brem-recovered, just require it pass momentum cuts
      else if ( lepton->p() >= _eTrackCut.value() ) 
	{
	  _leptonList.append( lepton );
	  passes = true;
	}

      if(passes == true)
	{
	  // Make a 2nd list with only "leptons" that don't pass 
	  // lepton selectors
	  BtaCandidate* removeLep(0);
	  HepAListIterator<BtaCandidate> itIdElRem( *_idElList );
	  while ((removeLep = itIdElRem()) )
	    {
	      // Include no ID brem-recovered electrons where the only photon 
	      // overlaps with an ID'd brem-recovered electron
	      if(lepton->isComposite())
		{
		  if(baseLepton->overlaps(*removeLep))
		    keepThis = false;
		}
	      else if(lepton->overlaps(*removeLep))
		keepThis = false;
	    }
	  
	  HepAListIterator<BtaCandidate> itIdMuRem( *_idMuList );
	  while ((removeLep = itIdMuRem()) )
	    {
	      if(lepton->overlaps(*removeLep))
		keepThis = false;
	    }

	  if(keepThis == true)
	    _noIDLeptonList.append( lepton );
	}
    }
  
  // if no leptons of any kind, just return
  if (_leptonList.length() == 0) 
    {
      if( _debug.value() >= 1)
	cout << "ExclHllMiniFilter: No lepton candidates found in event"
	     << endl;
      return AppResult::OK;
    } 

  // Get lists from SimpleComposition
  // Step 1: search hypernews for "getTmpAList"
  // Step 2: find it's "bad", "infamous", "deplorable"
  // Step 3: find 8 contradictory opinions on how to replace it
  // Step 4: keep using getTmpAList 
  getTmpAList(anEvent, _piZeroInputList, _piZeroListKey.value());
  getTmpAList(anEvent, _ksInputList, _ksListKey.value());
  getTmpAList(anEvent, _etaInputList, _etaListKey.value());

  getTmpAList(anEvent, _kStarCKsInputList, _kStarCKsListKey.value());
  getTmpAList(anEvent, _kStarCKInputList, _kStarCKListKey.value());
  getTmpAList(anEvent, _kStar0KInputList, _kStar0KListKey.value());
  getTmpAList(anEvent, _kStar0KsInputList, _kStar0KsListKey.value());

  getTmpAList(anEvent, _rhoChInputList, _rhoChListKey.value());
  getTmpAList(anEvent, _rhoZeroInputList, _rhoZeroListKey.value());
  getTmpAList(anEvent, _omegaInputList, _omegaListKey.value());
  getTmpAList(anEvent, _phiInputList, _phiListKey.value());
  getTmpAList(anEvent, _etaPrInputList, _etaPrListKey.value());

  if( _debug.value() >= 1)
    {
      cout << "Found " << nTrks << " tracks in event" << endl;
      cout << "Found " << nNeutral << " calor neutral in event" << endl;
      cout << "Found " << _leptonList.length() << " lepton cands. in event" 
	   << endl;
      cout << "Found " << _idElList->length() << " ID'd electrons" << endl;
      cout << "Found " << _idMuList->length() << " ID'd muons" << endl;
      cout << "After removing ID'd leptons, there are " 
	   << _noIDLeptonList.length() << " no ID leptons left" << endl;

      cout << "pi0 List length = " << _piZeroInputList->length() << endl;
      cout << "Ks List length = " << _ksInputList->length() << endl;
      cout << "eta List length = " << _etaInputList->length() << endl;
      cout << "K* (Ks pi+) List length = " << _kStarCKsInputList->length() 
	   << endl;
      cout << "K* (K+ pi0) List length = " << _kStarCKInputList->length() 
	   << endl;
      cout << "K* (K+ pi-) List length = " << _kStar0KInputList->length() 
	   << endl;
      cout << "K* (Ks pi0) List length = " << _kStar0KsInputList->length() 
	   << endl;
      cout << "rho+ List length = " << _rhoChInputList->length() << endl;
      cout << "rho0 List length = " << _rhoZeroInputList->length() << endl;
      cout << "omega List length = " << _omegaInputList->length() << endl;
      cout << "phi List length = " << _phiInputList->length() << endl;
      cout << "eta' List length = " << _etaPrInputList->length() << endl;
    }

  // -----------------------------------------------------------------------

  UsrIfd<UsrEventBlock>::put( anEvent, &_eventBlock, _eventData.value() );
  UsrIfd<UsrCandBlock>::put( anEvent, &_candBlock, _candData.value() );

  // Single-leg lepton ID
  BtaCandidate* idLepton(0);
  BtaCandidate* otherLepton(0);
  
  // First with ID'd electrons
  HepAListIterator<BtaCandidate> itIdEl( *_idElList );
  while( (idLepton = itIdEl()) ) 
    {
      if(idLepton->p() >= _eTrackCut.value())
	{
	  HepAListIterator<BtaCandidate> iterOtherLep( _noIDLeptonList );
	  while( (otherLepton = iterOtherLep()) )
	    {
	      if( _debug.value() >= 1)
		cout << "ExclHllMiniFilter: checking two tracks" << endl
		     << "p1(e) = " << idLepton->p() << endl
		     << "p2(x) = " << otherLepton->p() << endl;

	      // Check for duplicates and oppositely charged tracks...
	      if( idLepton->overlaps(*otherLepton) )
		continue;

	      if( idLepton->charge() == otherLepton->charge() )
		continue;

	      BtaCandidate theLep;
	      BtaCandidate idLep;
	      theLep = *otherLepton;
	      idLep = *idLepton;

	      // single e ID
	      // e+e-
	      if( (theLep.p() >= _eTrackCut.value()) && (_doEE.value()) )
		{
		  if( theLep.charge() < 0 )
		    {
		      theLep.setType( Pdt::lookup("e-") );
		      idLep.setType( Pdt::lookup("e+") );
		      if( doHadModes( &idLep, &theLep, 1 ) )
			{
			  signalcand = true;
			  doCharmonium( &idLep, &theLep, 1 );
			}
		    }
		  else if( (theLep.charge() > 0) )
		    {
		      theLep.setType( Pdt::lookup("e+") );
		      idLep.setType( Pdt::lookup("e-") );
		      if( doHadModes( &theLep, &idLep, 1 ) )
			{
			  signalcand = true;
			  doCharmonium( &theLep, &idLep, 1 );
			}
		    }
		}
	      
	      // e+mu-
	      if( (theLep.p() >= _muTrackCut.value()) &&
		  (theLep.charge() < 0) && 
		  (theLep.isComposite() == false) && 
		  (_doMuE.value()) )
		{
		  theLep.setType( Pdt::lookup("mu-") );
		  idLep.setType( Pdt::lookup("e+") );
		  if( doHadModes( &idLep, &theLep, 3 ) )
		    {
		      signalcand = true;
		      doCharmonium( &idLep, &theLep, 3 );
		    }
		}
	      // e-mu+
	      else if( (theLep.p() >= _muTrackCut.value()) &&
		       (theLep.charge() > 0) &&
		       (theLep.isComposite() == false) &&  
		       (_doEMu.value()) )
		{
		  theLep.setType( Pdt::lookup("mu+") );
		  idLep.setType( Pdt::lookup("e-") );
		  if( doHadModes( &theLep, &idLep, 4 ) )
		    {
		      signalcand = true;
		      doCharmonium( &theLep, &idLep, 4 );
		    }
		}
	      if( signalcand )
		{
		  passed = true;
		}
	    }
	  
	  // Double e ID case
	  BtaCandidate *idElectron2(0);
	  HepAListIterator<BtaCandidate> itIdEl2( itIdEl );
	  while( (idElectron2 = itIdEl2()) ) 
	    {
	      if( idElectron2->p() >= _eTrackCut.value() )
		{
		  if( _debug.value() >= 1)
		    cout << "ExclHllMiniFilter: checking two tracks" << endl
			 << "p1(e) = " << idLepton->p() << endl
			 << "p2(e) = " << idElectron2->p() << endl;
		  // Check for duplicates and oppositely charged tracks...
		  if( idLepton->overlaps(*idElectron2) )
		    continue;
		  
		  if( idLepton->charge() == idElectron2->charge() )
		    continue;
		  
		  BtaCandidate e1;
		  BtaCandidate e2;
		  e1 = *idLepton;
		  e2 = *idElectron2;
		  // e+e-
		  if( (e2.p() >= _eTrackCut.value()) && 
		      (_doEE.value()) && e1.charge() == 1)
		    {
		      e1.setType( Pdt::lookup("e+") );
		      e2.setType( Pdt::lookup("e-") );
		      if( doHadModes( &e1, &e2, 1 ) )
			{
			  signalcand = true;
			  doCharmonium( &e1, &e2, 1 );
			}
		    }
		  else if( (e2.p() >= _eTrackCut.value()) && 
			   (_doEE.value()) && e2.charge() == 1)
		    {
		      e1.setType( Pdt::lookup("e-") );
		      e2.setType( Pdt::lookup("e+") );
		      if( doHadModes( &e2, &e1, 1 ) )
			{
			  signalcand = true;
			  doCharmonium( &e2, &e1, 1 );
			}
		    }
		}
	    }//double ID

	  // Now the double ID LFV case - argh.
	  BtaCandidate *idOtherMu(0);
	  HepAListIterator<BtaCandidate> itIdOtherMu( *_idMuList );
	  while( (idOtherMu = itIdOtherMu()) ) 
	    {
	      if( idOtherMu->p() >= _muTrackCut.value() )
		{
		  if( _debug.value() >= 1)
		    cout << "ExclHllMiniFilter: checking two tracks" << endl
			 << "p1(e) = " << idLepton->p() << endl
			 << "p2(mu) = " << idOtherMu->p() << endl;
		  // Check for duplicates and oppositely charged tracks...
		  if( idLepton->overlaps(*idOtherMu) )
		    continue;
		  
		  if( idLepton->charge() == idOtherMu->charge() )
		    continue;
		  
		  BtaCandidate elec;
		  BtaCandidate mu;
		  elec = *idLepton;
		  mu = *idOtherMu;
		  
		  // e+mu-
		  if( (mu.charge() == -1) && 
		      (elec.charge() == 1) && 
		      (mu.p() >= _muTrackCut.value()) && 
		      (_doEMu.value()) )
		    {
		      elec.setType( Pdt::lookup("e+") );
		      mu.setType( Pdt::lookup("mu-") );
		      if( doHadModes( &elec, &mu, 3 ) )
			{
			  signalcand = true;
			  doCharmonium( &elec, &mu, 3 );
			}
		    }	  
		  
		  // e-mu+
		  else if( (elec.charge() == -1) && 
			   (mu.charge() == 1) && 
			   (mu.p() >= _muTrackCut.value()) && 
			   (_doMuE.value()) )
		    {
		      elec.setType( Pdt::lookup("e-") );
		      mu.setType( Pdt::lookup("mu+") );
		      if( doHadModes( &mu, &elec, 4 ) )
			{
			  signalcand = true;
			  doCharmonium( &mu, &elec, 4 );
			}
		    }
		}
	    }// double LFV ID	  
	}
    }

  // Now ID muons
  HepAListIterator<BtaCandidate> itIdMu( *_idMuList );
  while( (idLepton = itIdMu()) ) 
    {
      if(idLepton->p() >= _muTrackCut.value())
	{
	  HepAListIterator<BtaCandidate> iterOtherLep( _noIDLeptonList );
	  while( (otherLepton = iterOtherLep()) )
	    {
	      if( _debug.value() >= 1)
		cout << "ExclHllMiniFilter: checking two tracks" << endl
		     << "p1(mu) = " << idLepton->p() << endl
		     << "p2(x) = " << otherLepton->p() << endl;

	      // Check for duplicates and oppositely charged tracks...
	      if( idLepton->overlaps(*otherLepton) )
		continue;
	      if( idLepton->charge() == otherLepton->charge() )
		continue;

	      BtaCandidate theLep;
	      theLep = *otherLepton;
	      BtaCandidate idLep;
	      idLep = *idLepton;

	      // Single mu ID 
	      // mu+mu-
	      if( (theLep.p() >= _muTrackCut.value()) && 
		  (_doEE.value()) && 
		  (theLep.isComposite() == false) )
		{
		  if( theLep.charge() < 0 )
		    {
		      theLep.setType( Pdt::lookup("mu-") );
		      idLep.setType( Pdt::lookup("mu+") );
		      if( doHadModes( &idLep, &theLep, 2 ) )
			{
			  signalcand = true;
			  doCharmonium( &idLep, &theLep, 2 );
			}
		    }
		  else if( (theLep.charge() > 0) )
		    {
		      theLep.setType( Pdt::lookup("mu+") );
		      idLep.setType( Pdt::lookup("mu-") );
		      if( doHadModes( &theLep, &idLep, 2 ) )
			{
			  signalcand = true;
			  doCharmonium( &theLep, &idLep, 2 );
			}
		    }
		}


	      // e+mu-
	      if( (theLep.p() >= _eTrackCut.value()) &&
		  (theLep.charge() > 0) && 
		  (_doMuE.value()) )
		{
		  theLep.setType( Pdt::lookup("e+") );
		  idLep.setType( Pdt::lookup("mu-") );
		  if( doHadModes( &theLep, &idLep, 3 ) )
		    {
		      signalcand = true;
		      doCharmonium( &theLep, &idLep, 3 );
		    }
		}
	      // e-mu+
	      else if( (theLep.p() >= _eTrackCut.value()) &&
		       (theLep.charge() < 0) && 
		       (_doEMu.value()) )
		{
		  theLep.setType( Pdt::lookup("e-") );
		  idLep.setType( Pdt::lookup("mu+") );
		  if( doHadModes( &idLep, &theLep, 4 ) )
		    {
		      signalcand = true;
		      doCharmonium( &idLep, &theLep, 4 );
		    }
		}
	      // We don't need to do the double ID LFV case here since
	      // it's already been done in the electron loop

	      if( signalcand )
		{
		  passed = true;
		}
	    }
	  // Double mu ID case
	  BtaCandidate *idMuon2(0);
	  HepAListIterator<BtaCandidate> itIdMu2( itIdMu );
	  while( (idMuon2 = itIdMu2()) ) 
	    {
	      if(idMuon2->p() >= _muTrackCut.value())
		{
		  if( _debug.value() >= 1)
		    cout << "ExclHllMiniFilter: checking two tracks" << endl
			 << "p1(mu) = " << idLepton->p() << endl
			 << "p2(mu) = " << idMuon2->p() << endl;
		  // Check for duplicates and oppositely charged tracks...
		  if( idLepton->overlaps(*idMuon2) )
		    continue;
		  
		  if( idLepton->charge() == idMuon2->charge() )
		    continue;
		  
		  BtaCandidate mu1;
		  BtaCandidate mu2;
		  mu1 = *idLepton;
		  mu2 = *idMuon2;
		  // mu+mu-
		  if( (mu2.p() >= _muTrackCut.value()) && 
		      (_doMuMu.value()) && mu1.charge() == 1)
		    {
		      mu1.setType( Pdt::lookup("mu+") );
		      mu2.setType( Pdt::lookup("mu-") );
		      if( doHadModes( &mu1, &mu2, 2 ) )
			{
			  signalcand = true;
			  doCharmonium( &mu1, &mu2, 2 );
			}
		    }
		  else if( (mu2.p() >= _muTrackCut.value()) && 
		      (_doMuMu.value()) && mu2.charge() == 1)
		    {
		      mu1.setType( Pdt::lookup("mu-") );
		      mu2.setType( Pdt::lookup("mu+") );
		      if( doHadModes( &mu2, &mu1, 2 ) )
			{
			  signalcand = true;
			  doCharmonium( &mu2, &mu1, 2 );
			}
		    }
		}
	    }// double ID
	}	 
    }

  _hadModeEE = _tempHadMode[0];
  _hadModeMM = _tempHadMode[1];
  _hadModeEM = _tempHadMode[2];
  _hadModeME = _tempHadMode[3];

  // Add user data
  bool ok = true;
  ok &= _eventBlock.put( _isccbar );
  ok &= _eventBlock.put( _hadModeEE );
  ok &= _eventBlock.put( _hadModeMM );
  ok &= _eventBlock.put( _hadModeEM );
  ok &= _eventBlock.put( _hadModeME );

  assert(ok);
  
  if( passed )
    {      
      _nFound++;
      
      if( _debug.value() >= 1 )
	{
	  cout << "Mode\tNumber of passing candidates" << endl;
	  for(i = 0;i < 32;i++)
	    {
	      cout << i << "\t\t" << _eventCands[i] << endl;
	    }

	  cout << "Event passed ExclHllMiniFilter" << endl
	       << "e+e- bitmap: " << hex << _hadModeEE << endl
	       << "mu+mu- bitmap: " << _hadModeMM << endl
	       << "e+mu- bitmap: " << _hadModeEM << endl
	       << "mu+e- bitmap: " << _hadModeME << endl
	       << "and charmonium bit map: " << _isccbar << dec << endl 
	       << endl;
	}
    }

  setPassed( passed ); // skip the event if not passed

  if( _doNtp.value() )
    {
      _ntuple->column("ee", _hadModeEE);
      _ntuple->column("mumu", _hadModeMM);
      _ntuple->column("emu", _hadModeEM);
      _ntuple->column("mue", _hadModeME);
      _ntuple->column("ccbar", _isccbar);
      _ntuple->dumpData();      
    }

  // Clear lists
  _leptonList.removeAll();
  _noIDLeptonList.removeAll();
  
  // done
  return AppResult::OK;
}

AppResult
ExclHllMiniFilter::endJob( AbsEvent* anEvent ) 
{
  ErrMsg(routine)<<" end Job" 
		 << " with " << _nReadEvents << " events read "   
		 << " and "  << _nFound << " events found " 
		 << endmsg; 

  cout << endl << "ExclHllMiniFilter::endJob" << endl;
  if (_nReadEvents > 0) {
    cout << "# events passed tag filter: " << _nReadEvents << endl 
	 << "# events passed mini filter: " << _nFound << endl
	 << "Relative selection rate: " 
	 << 100.0 * (float)_nFound / (float)_nReadEvents 
	 << " %" << endl;
  }
  else {
    cout << "# events passed tag filter: " << _nReadEvents << endl 
	 << "# events passed mini filter: " << _nFound << endl
	 << "Relative selection rate: 0 %" << endl;
  }

  if( _debug.value() >= 1 )
    {
      cout << "ExclHllMiniFilter: Passing candidates by mode" << endl;
      for (int k = 0;k < 32;k++)
	cout << "Mode: " << k << "\t\t" << _multiCands[k] << endl;
      cout << endl;
    }

  return AppResult::OK;
}

//----------------------------------------------------------------------
// Check each hadron mode for a given dilepton pair
//----------------------------------------------------------------------

bool
ExclHllMiniFilter::doHadModes(BtaCandidate* trackPlus, 
			     BtaCandidate* trackMinus,
			     int leptMode)
{
  bool isSignal(false);
  int modeBit = 0;

  if( _doKll.value() )
    {
      _candLeptMode = leptMode;
      _candHadMode = (0x1 << modeBit);

      // Is this a B->Kll candidate?
      if( isKMode(trackPlus, trackMinus) )
	{
	  isSignal = true;
	  _tempHadMode[leptMode-1] |= (0x1 << modeBit);
	  if( _debug.value() >= 1 )
	    cout << "Passed as B -> K+ ll" << endl;
	}
    }
  modeBit++;

  if( _doKsll.value() )
    {
      _candLeptMode = leptMode;
      _candHadMode = (0x1 << modeBit);

      // Is this a B->K0sll candidate?
      if( isKsMode(trackPlus, trackMinus) )
	{
	  isSignal = true;
	  _tempHadMode[leptMode-1] |= (0x1 << modeBit);
	  if( _debug.value() >= 1 )
	    cout << "Passed as B -> K0s(->pi+pi-) ll" << endl;
	}
    }
  modeBit++;

  if( _doKstarChll.value() )
    {
      _candLeptMode = leptMode;
      _candHadMode = (0x1 << modeBit);

      // Is this a B->K*+(->K0spi+)ll candidate?
      if( isKstarChMode(trackPlus, trackMinus) )
	{
	  isSignal = true;
	  _tempHadMode[leptMode-1] |= (0x1 << modeBit); 
	  if( _debug.value() >= 1 )
	    cout << "Passed as B -> K*+(->K0spi+) ll" << endl;
	}	      
    }
  modeBit++;

  if( _doKstarChPi0ll.value() )
    {
      _candLeptMode = leptMode;
      _candHadMode = (0x1 << modeBit);

      // Is this a B->K*+(->K+pi0)ll candidate?
      if( isKstarChPi0Mode(trackPlus, trackMinus) )
	{
	  isSignal = true;
	  _tempHadMode[leptMode-1] |= (0x1 << modeBit);
	  if( _debug.value() >= 1 )
	    cout << "Passed as B -> K*+(->K+pi0) ll" << endl;
	}	      
    }
  modeBit++;

  if( _doKstar0ll.value() )
    {
      _candLeptMode = leptMode;
      _candHadMode = (0x1 << modeBit);

      // Is this a B->K*0(->K+pi-)ll candidate?
      if( isKstar0Mode(trackPlus, trackMinus) )
	{
	  isSignal = true;
	  _tempHadMode[leptMode-1] |= (0x1 << modeBit);
	  if( _debug.value() >= 1 )
	    cout << "Passed as B -> K*0(->K+pi-) ll" << endl;
	}
    }
  modeBit++;

  if( _doKstar0Pi0ll.value() )
    {
      _candLeptMode = leptMode;
      _candHadMode = (0x1 << modeBit);

      // Is this a B->K*0(->K0spi0)ll candidate?
      if( isKstar0Pi0Mode(trackPlus, trackMinus) )
	{
	  isSignal = true;
	  _tempHadMode[leptMode-1] |= (0x1 << modeBit);
	  if( _debug.value() >= 1 )
	    cout << "Passed as B -> K*0(->K0spi0) ll" << endl;
	}
    }
  modeBit++;

  if( _doPiChll.value() )
    {
      _candLeptMode = leptMode;
      _candHadMode = (0x1 << modeBit);

      // Is this a B->pi+ll candidate?
      if( isPiChMode(trackPlus, trackMinus) )
	{
	  isSignal = true;
	  _tempHadMode[leptMode-1] |= (0x1 << modeBit);
	  if( _debug.value() >= 1 )
	    cout << "Passed as B -> pi+ ll" << endl;
	}
    }
  modeBit++;

  if( _doPi0ll.value() )
    {
      _candLeptMode = leptMode;
      _candHadMode = (0x1 << modeBit);

      // Is this a B->pi0ll candidate?
      if( isPi0Mode(trackPlus, trackMinus) )
	{
	  isSignal = true;
	  _tempHadMode[leptMode-1] |= (0x1 << modeBit);
	  if( _debug.value() >= 1 )
	    cout << "Passed as B -> pi0(->gamma gamma) ll" << endl;
	}
    }
  modeBit++;

  if( _doRhoChll.value() )
    {
      _candLeptMode = leptMode;
      _candHadMode = (0x1 << modeBit);

      // Is this a B->rho+(->pi+pi0)ll candidate?
      if( isRhoChMode(trackPlus, trackMinus) )
	{
	  isSignal = true;
	  _tempHadMode[leptMode-1] |= (0x1 << modeBit);
	  if( _debug.value() >= 1 )
	    cout << "Passed as B -> rho+(->pi+pi0) ll" << endl;
	}
    }
  modeBit++;

  if( _doRho0ll.value() )
    {
      _candLeptMode = leptMode;
      _candHadMode = (0x1 << modeBit);

      // Is this a B->rho0(->pi+pi-)ll candidate?
      if( isRho0Mode(trackPlus, trackMinus) )
	{
	  isSignal = true;
	  _tempHadMode[leptMode-1] |= (0x1 << modeBit);
	  if( _debug.value() >= 1 )
	    cout << "Passed as B -> rho0(->pi+pi-) ll" << endl;
	}
    }
  modeBit++;

  if( _doOmegall.value() )
    {
      _candLeptMode = leptMode;
      _candHadMode = (0x1 << modeBit);

      // Is this a B->omega(->pi+pi-pi0)ll candidate?
      if( isOmegaMode(trackPlus,trackMinus) )
	{
	  isSignal = true;
	  _tempHadMode[leptMode-1] |= (0x1 << modeBit);
	  if( _debug.value() >= 1 )
	    cout << "Passed as B -> omega(->pi+pi-pi0) ll" << endl;
	}
    }
  modeBit++;

  if( _doPhill.value() )
    {
      _candLeptMode = leptMode;
      _candHadMode = (0x1 << modeBit);

      // Is this a B->phill candidate?
      if( isPhiMode(trackPlus, trackMinus) )
	{
	  isSignal = true;
	  _tempHadMode[leptMode-1] |= (0x1 << modeBit);
	  if( _debug.value() >= 1 )
	    cout << "Passed as B -> phi(->K+K-) ll" << endl;
	}
    }
  modeBit++;

  if( _doEtall.value() )
    {
      _candLeptMode = leptMode;
      _candHadMode = (0x1 << modeBit);

      // Is this a B->etall candidate?
      if( isEtaMode(trackPlus, trackMinus) )
	{
	  isSignal = true;
	  _tempHadMode[leptMode-1] |= (0x1 << modeBit);
	  if( _debug.value()  >= 1 )
	    cout << "Passed as B -> eta(->gamma gamma) ll" << endl;
	}
    }
  modeBit++;

  if( _doEtaPrll.value() )
    {
      _candLeptMode = leptMode;
      _candHadMode = (0x1 << modeBit);

      // Is this a B->eta'll candidate?
      if( isEtaPrMode(trackPlus, trackMinus) )
	{
	  isSignal = true;
	  _tempHadMode[leptMode-1] |= (0x1 << modeBit);
	  if( _debug.value() >= 1 )
	    cout << "Passed as B -> eta'(->pi+pi-eta) ll" << endl;
	}
    }
  modeBit++;

  return(isSignal);
}

//----------------------------------------------------------------------
// Check for J/Psi, Psi' -> l+l- candidates in passing events
//----------------------------------------------------------------------

void 
ExclHllMiniFilter::doCharmonium(BtaCandidate* trackPlus, 
				BtaCandidate* trackMinus,
				int leptMode)
{
  int shift = (leptMode-1) * 2;

  // Look for charmonium candidates
  float mtt = ( (trackPlus->p4()) + (trackMinus->p4()) ).mag();
  if( (mtt > 2.9) && (mtt < 3.2) )
    {
      _isccbar = _isccbar | (0x1 << shift);

      if( _debug.value() >= 2 )
	cout << "Found J/Psi cand. with mass " << mtt << endl;
    }
  else if( (mtt > 3.6) && (mtt < 3.8) )
    {
      _isccbar = _isccbar | (0x2 << shift);

      if( _debug.value()  >= 2 )
	cout << "Found Psi prime cand. with mass " << mtt << endl;
    }
}

//----------------------------------------------------------------------
// B+ -> K+ l+l-
//----------------------------------------------------------------------

bool
ExclHllMiniFilter::isKMode(BtaCandidate* trackPlus, BtaCandidate* trackMinus)
{
  bool keepEvent(false);

  // Use list of all charged tracks assuming a kaon
  BtaCandidate *kaon(0);
  HepAListIterator< BtaCandidate > iterKaon( *_chargedList );

  while( (kaon = iterKaon()) )
    {
      // Don't use the "lepton" tracks
      if( (kaon->overlaps(*trackPlus)) || (kaon->overlaps(*trackMinus)) )
	continue;
      
      // Can't call setType on core ChargedTracks list
      BtaCandidate localKaon( *kaon );

      if( localKaon.charge() > 0 )
	localKaon.setType( Pdt::lookup("K+") );
      else if( localKaon.charge() < 0 )
	localKaon.setType( Pdt::lookup("K-") );

      if ( _debug.value() >= 2 )
	{
	  cout << "K+ (m,p3,q) = " << localKaon.mass() << ", " 
	       << localKaon.p() << ", " << ", " << localKaon.charge() << endl;

	}

      // Check mES and deltaE
      if( filterCommonInfo(trackPlus, trackMinus, &localKaon) )
	{
	  keepEvent = true;
	  _multiCands[0]++;
	  _eventCands[0]++;
	}

      if( _firstCandOnly.value() && (keepEvent == true) )
	return(keepEvent);
    }

  return(keepEvent);
}

//----------------------------------------------------------------------
// B0 -> K0s l+l- (K0s -> pi+pi-)
//----------------------------------------------------------------------

bool
ExclHllMiniFilter::isKsMode(BtaCandidate* trackPlus, BtaCandidate* trackMinus)
{
  bool keepEvent(false);

  float foundKsMass(0.0);

  BtaCandidate *Ks(0);

  if( _debug.value() >= 2 )
    cout << "isKsMode: found " << _ksInputList->length()
	 << " K0s candidates in event" << endl;

  // Use lists of all charged tracks assuming pions
  HepAListIterator< BtaCandidate > iterKs( *_ksInputList );

  while( (Ks = iterKs()) )
    {
      // Don't use the "lepton" tracks
      if( (Ks->overlaps(*trackPlus)) || (Ks->overlaps(*trackMinus)) )
	 continue;
      
      foundKsMass = Ks->mass();
      
      if( _debug.value() >= 2 )
	cout << "K0s mass = " << foundKsMass << ", charge = " 
	     << Ks->charge() << endl;
      
      // Check mES and deltaE
      if( filterCommonInfo(trackPlus, trackMinus, Ks) )
	{
	  keepEvent = true;
	  _multiCands[1]++;
	  _eventCands[1]++;
	}

      if( _firstCandOnly.value() && (keepEvent == true) )
	return(keepEvent);
    }

  return(keepEvent);
}

//----------------------------------------------------------------------
// B+ -> K*+ l+l- (K*+ -> K0spi+ (K0s -> pi+pi-)) 
//----------------------------------------------------------------------

bool 
ExclHllMiniFilter::isKstarChMode(BtaCandidate* trackPlus, 
				 BtaCandidate* trackMinus)
{
  bool  keepEvent(false);
  float foundKstarMass(0.0);

  BtaCandidate *Kstar(0);

  // Use lists of all charged tracks assuming pions
  HepAListIterator< BtaCandidate > iterKstar( *_kStarCKsInputList );

  while( 0 != (Kstar = iterKstar()) )
    {
      // Don't use the "lepton" tracks
      if( (Kstar->overlaps(*trackPlus)) || (Kstar->overlaps(*trackMinus)) )
	 continue;
      
      foundKstarMass = Kstar->mass();
      
      // Explicit check for K* mass due to CompositionSequences nonsense
      if( (foundKstarMass < _lowKstarMass.value()) ||
	  (foundKstarMass > _highKstarMass.value()) )
	continue;

      if( _debug.value() >= 2 )
	cout << "K*+(Kspi+) mass = " << foundKstarMass << ", charge = " 
	     << Kstar->charge() << endl;
      
      // Check mES and deltaE
      if( filterCommonInfo(trackPlus, trackMinus, Kstar) )
	{
	  keepEvent = true;
	  _multiCands[2]++;
	  _eventCands[2]++;
	}

      if( _firstCandOnly.value() && (keepEvent == true) )
	return(keepEvent);
    }

  return(keepEvent);
}

//----------------------------------------------------------------------
// B+ -> K*+ l+l- (K*+ -> K+pi0)
//----------------------------------------------------------------------

bool 
ExclHllMiniFilter::isKstarChPi0Mode(BtaCandidate* trackPlus, 
				 BtaCandidate* trackMinus)
{
  bool  keepEvent(false);
  float foundKstarMass(0.0);

  BtaCandidate *Kstar(0);

  // Use lists of all charged tracks assuming pions
  HepAListIterator< BtaCandidate > iterKstar( *_kStarCKInputList );

  while( 0 != (Kstar = iterKstar()) )
    {
      // Don't use the "lepton" tracks
      if( (Kstar->overlaps(*trackPlus)) || (Kstar->overlaps(*trackMinus)) )
	 continue;
      
      foundKstarMass = Kstar->mass();
      
      if( (foundKstarMass < _lowKstarMass.value()) ||
	  (foundKstarMass > _highKstarMass.value()) )
	continue;

      if( _debug.value() >= 2 )
	cout << "K*+(K+pi0) mass = " << foundKstarMass << ", charge = " 
	     << Kstar->charge() << endl;
      
      // Check mES and deltaE
      if( filterCommonInfo(trackPlus, trackMinus, Kstar) )
	{
	  keepEvent = true;
	  _multiCands[3]++;
	  _eventCands[3]++;
	}

      if( _firstCandOnly.value() && (keepEvent == true) )
	return(keepEvent);
    }

  return(keepEvent);
}

//----------------------------------------------------------------------
// B0 -> K*0 l+l- (K*0 -> K+pi-) 
//----------------------------------------------------------------------

bool 
ExclHllMiniFilter::isKstar0Mode(BtaCandidate* trackPlus, 
				 BtaCandidate* trackMinus)
{
  bool  keepEvent(false);
  float foundKstarMass(0.0);

  BtaCandidate *Kstar(0);

  // Use lists of all charged tracks assuming pions
  HepAListIterator< BtaCandidate > iterKstar( *_kStar0KInputList );

  while( 0 != (Kstar = iterKstar()) )
    {
      // Don't use the "lepton" tracks
      if( (Kstar->overlaps(*trackPlus)) || (Kstar->overlaps(*trackMinus)) )
	 continue;
      
      foundKstarMass = Kstar->mass();

      if( (foundKstarMass < _lowKstarMass.value()) ||
	  (foundKstarMass > _highKstarMass.value()) )
	continue;
      
      if( _debug.value() >= 2 )
	cout << "K*+(K+pi-) mass = " << foundKstarMass << ", charge = " 
	     << Kstar->charge() << endl;
      
      // Check mES and deltaE
      if( filterCommonInfo(trackPlus, trackMinus, Kstar) )
	{
	  keepEvent = true;
	  _multiCands[4]++;
	  _eventCands[4]++;
	}

      if( _firstCandOnly.value() && (keepEvent == true) )
	return(keepEvent);
    }

  return(keepEvent);
}

//----------------------------------------------------------------------
// B0 -> K*0 l+l- (K*0 -> K0s pi0) 
//----------------------------------------------------------------------

bool 
ExclHllMiniFilter::isKstar0Pi0Mode(BtaCandidate* trackPlus, 
				 BtaCandidate* trackMinus)
{
  bool  keepEvent(false);
  float foundKstarMass(0.0);

  BtaCandidate *Kstar(0);

  // Use lists of all charged tracks assuming pions
  HepAListIterator< BtaCandidate > iterKstar( *_kStar0KsInputList );

  while( 0 != (Kstar = iterKstar()) )
    {
      // Don't use the "lepton" tracks
      if( (Kstar->overlaps(*trackPlus)) || (Kstar->overlaps(*trackMinus)) )
	 continue;
      
      foundKstarMass = Kstar->mass();

      if( (foundKstarMass < _lowKstarMass.value()) ||
	  (foundKstarMass > _highKstarMass.value()) )
	continue;
      
      if( _debug.value() >= 2 )
	cout << "K*+(KsPi0) mass = " << foundKstarMass << ", charge = " 
	     << Kstar->charge() << endl;
      
      // Check mES and deltaE
      if( filterCommonInfo(trackPlus, trackMinus, Kstar) )
	{
	  keepEvent = true;
	  _multiCands[5]++;
	  _eventCands[5]++;
	}

      if( _firstCandOnly.value() && (keepEvent == true) )
	return(keepEvent);
    }

  return(keepEvent);
}

//----------------------------------------------------------------------
// B+ -> pi+ l+l-
//----------------------------------------------------------------------

bool
ExclHllMiniFilter::isPiChMode(BtaCandidate* trackPlus, BtaCandidate* trackMinus)
{
  bool keepEvent(false);

  // Use list of all charged tracks assuming a pion
  BtaCandidate *pion(0);
  HepAListIterator< BtaCandidate > iterPion( *_chargedList );

  while( 0 != (pion = iterPion()) )
    {
      // Don't use the "lepton" tracks
      if( (pion->overlaps(*trackPlus)) || (pion->overlaps(*trackMinus)) )
	continue;
      
      // Can't call setType on core ChargedTracks list
      BtaCandidate localPion( *pion );

      if( localPion.charge() > 0 )
	localPion.setType( Pdt::lookup("pi+") );
      else
	localPion.setType( Pdt::lookup("pi-") );

      if ( _debug.value() >= 2 )
	cout << "pi+ mass = " << pion->mass() << endl;

      // Check mES and deltaE
      if( filterCommonInfo(trackPlus, trackMinus, &localPion) )
	{
	  keepEvent = true;
	  _multiCands[6]++;
	  _eventCands[6]++;
	}

      if( _firstCandOnly.value() && (keepEvent == true) )
	return(keepEvent);
    }

  return(keepEvent);
}

//----------------------------------------------------------------------
// B+ -> pi0 l+l- (pi0 -> gamma gamma)
//----------------------------------------------------------------------

bool
ExclHllMiniFilter::isPi0Mode(BtaCandidate* trackPlus, BtaCandidate* trackMinus)
{
  bool keepEvent(false);

  // Use list of all charged tracks assuming a pion
  BtaCandidate *pion(0);
  HepAListIterator< BtaCandidate > iterPion( *_piZeroInputList );

  while( 0 != (pion = iterPion()) )
    {
      // Don't use the "lepton" tracks
      if( (pion->overlaps(*trackPlus)) || (pion->overlaps(*trackMinus)) )
	continue;
      
      // Double check for duplicated photons
      if( isBremOverlap(trackPlus, trackMinus, pion) )
	continue;

      if ( _debug.value() >= 2 )
	cout << "pi0 mass = " << pion->mass() << endl;

      // Check mES and deltaE
      if( filterCommonInfo(trackPlus, trackMinus, pion) )
	{
	  keepEvent = true;
	  _multiCands[7]++;
	  _eventCands[7]++;
	}

      if( _firstCandOnly.value() && (keepEvent == true) )
	return(keepEvent);
    }

  return(keepEvent);
}

//----------------------------------------------------------------------
// B+ -> rho+ l+l- (rho+ -> pi+pi0) 
//----------------------------------------------------------------------

bool 
ExclHllMiniFilter::isRhoChMode(BtaCandidate* trackPlus, 
			      BtaCandidate* trackMinus)
{
  bool  keepEvent(false);
  float foundRhoMass(0.0);
  
  BtaCandidate* rhoCh(0);
  HepAListIterator< BtaCandidate > iterRhoCh( *_rhoChInputList );
  
  while( 0 != (rhoCh = iterRhoCh()) )
    {
      foundRhoMass = rhoCh->mass();

      // Apply omega mass cuts
      if( (foundRhoMass > _lowRhoMass.value()) && 
	  (foundRhoMass < _highRhoMass.value()) )
	{
	  // Don't use the "lepton" tracks
 	  if( (rhoCh->overlaps(*trackPlus)) || 
	      (rhoCh->overlaps(*trackMinus)) )
 	    continue;
	  
	  if ( _debug.value() >= 2 )
	    cout << "rho+ mass = " << rhoCh->mass() << endl;
	  
	  // Check mES and deltaE
	  if( filterCommonInfo(trackPlus, trackMinus, rhoCh) )
	    {
	      keepEvent = true;
	      _multiCands[8]++;
	      _eventCands[8]++;
	    }
	}

      if( _firstCandOnly.value() && (keepEvent == true) )
	return(keepEvent);
    }

  return(keepEvent);
}

//----------------------------------------------------------------------
// B0 -> rho0 l+l- (rho0 -> pi+pi-) 
//----------------------------------------------------------------------

bool 
ExclHllMiniFilter::isRho0Mode(BtaCandidate* trackPlus, 
			      BtaCandidate* trackMinus)
{
  bool  keepEvent(false);
  float foundRhoMass(0.0);

  BtaCandidate* rhoZero(0);
  HepAListIterator< BtaCandidate > iterRho0( *_rhoZeroInputList );
  
  while( 0 != (rhoZero = iterRho0()) )
    {
      foundRhoMass = rhoZero->mass();

      // Apply omega mass cuts
      if( (foundRhoMass > _lowRhoMass.value()) && 
	  (foundRhoMass < _highRhoMass.value()) )
	{
	  // Don't use the "lepton" tracks
 	  if( (rhoZero->overlaps(*trackPlus)) || 
	      (rhoZero->overlaps(*trackMinus)) )
 	    continue;
	  
	  if ( _debug.value() >= 2 )
	    cout << "rho0 mass = " << rhoZero->mass() << endl;
	  
	  // Check mES and deltaE
	  if( filterCommonInfo(trackPlus, trackMinus, rhoZero) )
	    {
	      keepEvent = true;
	      _multiCands[9]++;
	      _eventCands[9]++;
	    }
	}

      if( _firstCandOnly.value() && (keepEvent == true) )
	return(keepEvent);
    }

  return(keepEvent);
}

//----------------------------------------------------------------------
// B0 -> omega l+l- (omega -> pi+pi-pi0)
//----------------------------------------------------------------------

bool
ExclHllMiniFilter::isOmegaMode(BtaCandidate* trackPlus, BtaCandidate* trackMinus)
{
  bool keepEvent(false);
  float foundOmegaMass(0.0);

  BtaCandidate *omega(0);
  HepAListIterator< BtaCandidate > iterOmega( *_omegaInputList );
  
  while( 0 != (omega = iterOmega()) )
    {
      foundOmegaMass = omega->mass();

      // Apply omega mass cuts
      if( (foundOmegaMass > _lowOmegaMass.value()) && 
	  (foundOmegaMass < _highOmegaMass.value()) )
	{
	  // Don't use the "lepton" tracks
 	  if( (omega->overlaps(*trackPlus)) || (omega->overlaps(*trackMinus)) )
 	    continue;

	  if ( _debug.value() >= 2 )
	    cout << "omega mass = " << omega->mass() << endl;
	  
	  // Check mES and deltaE
	  if( filterCommonInfo(trackPlus, trackMinus, omega) )
	    {
	      keepEvent = true;
	      _multiCands[10]++;
	      _eventCands[10]++;
	    }
	}

      if( _firstCandOnly.value() && (keepEvent == true) )
	return(keepEvent);
    }

  return(keepEvent);
}

//----------------------------------------------------------------------
// B0 -> phi l+l- (phi -> K+K-)
//----------------------------------------------------------------------

bool
ExclHllMiniFilter::isPhiMode(BtaCandidate* trackPlus, BtaCandidate* trackMinus)
{
  bool keepEvent(false);
  float foundPhiMass(0.0);

  BtaCandidate *phi(0);

  HepAListIterator< BtaCandidate > iterPhi( *_phiInputList );
  
  while( 0 != (phi = iterPhi()) )
    {
      foundPhiMass = phi->mass();

      // Apply omega mass cuts
      if( (foundPhiMass > _lowPhiMass.value()) && 
	  (foundPhiMass < _highPhiMass.value()) )
	{
	  // Don't use the "lepton" tracks
 	  if( (phi->overlaps(*trackPlus)) || 
	      (phi->overlaps(*trackMinus)) )
 	    continue;
	  
	  if ( _debug.value() >= 2 )
	    cout << "phi mass = " << phi->mass() << endl;
	  
	  // Check mES and deltaE
	  if( filterCommonInfo(trackPlus, trackMinus, phi) )
	    {
	      keepEvent = true;
	      _multiCands[11]++;
	      _eventCands[11]++;
	    }
	}

      if( _firstCandOnly.value() && (keepEvent == true) )
	return(keepEvent);
    }

  return(keepEvent);
}

//----------------------------------------------------------------------
// B0 -> eta l+l- (eta -> gamma gamma)
//----------------------------------------------------------------------

bool
ExclHllMiniFilter::isEtaMode(BtaCandidate* trackPlus, BtaCandidate* trackMinus)
{
  bool keepEvent(false);
  float foundEtaMass(0.0);

  BtaCandidate *eta(0);

  // Use lists of all eta candidates
  HepAListIterator< BtaCandidate > iterEta( *_etaInputList );

  while( (eta = iterEta()) )
    {
      // Don't use the "lepton" tracks
      if( (eta->overlaps(*trackPlus)) || (eta->overlaps(*trackMinus)) )
	continue;
      
      // Double check for duplicated photons
      if( isBremOverlap(trackPlus, trackMinus, eta) )
	continue;

      foundEtaMass = eta->mass();
      
      if( _debug.value() >= 2 )
	cout << "eta mass = " << foundEtaMass << endl;
      
      // Check mES and deltaE
      if( filterCommonInfo(trackPlus, trackMinus, eta) )
	{
	  keepEvent = true;
	  _multiCands[12]++;
	  _eventCands[12]++;
	}

      if( _firstCandOnly.value() && (keepEvent == true) )
	return(keepEvent);
    }

  return(keepEvent);
}

//----------------------------------------------------------------------
// B0 -> eta' l+l- (eta' -> pi+ pi- eta (eta -> gamma gamma))
//----------------------------------------------------------------------

bool
ExclHllMiniFilter::isEtaPrMode(BtaCandidate* trackPlus, BtaCandidate* trackMinus)
{
  bool keepEvent(false);
  float foundEtaPrMass(0.0);

  BtaCandidate *etaPr(0);
    // Use lists of all eta candidates
  HepAListIterator< BtaCandidate > iterEtaPr( *_etaPrInputList );

  while( (etaPr = iterEtaPr()) )
    {
      // Don't use the "lepton" tracks
      if( (etaPr->overlaps(*trackPlus)) || (etaPr->overlaps(*trackMinus)) )
	continue;
      
      foundEtaPrMass = etaPr->mass();
      
      if( _debug.value() >= 2 )
	cout << "eta' mass = " << foundEtaPrMass << endl;
      
      // Check mES and deltaE
      if( filterCommonInfo(trackPlus, trackMinus, etaPr) )
	{
	  keepEvent = true;
	  _multiCands[13]++;
	  _eventCands[13]++;
	}

      if( _firstCandOnly.value() && (keepEvent == true) )
	return(keepEvent);
    }

  return(keepEvent);
}

//----------------------------------------------------------------------
// Check B values (mES and deltaE) - 
// stolen verbatim from PenguinUser/TuxKll.cc
//----------------------------------------------------------------------

bool
ExclHllMiniFilter::filterCommonInfo(const BtaCandidate* trackPlus, 
				    const BtaCandidate* trackMinus, 
				    const BtaCandidate* hadron)
{
  // Reconstruct the B from the two tracks and the hadron candidate.
  static BtaOpAdd4 Bcombiner;

  _BP4= trackPlus->p4()+trackMinus->p4()+hadron->p4();

  // Now get beam-constrained mass and delta E.

  // Get the beam parameters:
  const PepBeams * theBeams = gblEnv->getBta()->pepBeams(); 
  if (0 == theBeams){
    ErrMsg(error) << "Couldn't get the Pep beams info. Not passing candidate."
		  << endmsg;
    return false;
  }

  const BbrLorentzVectorErr q0(theBeams->total4MomentumWErr());

  // Get the boost parameters:
  const Hep3Vector boostLabToCM = -(theBeams->boostCMtoLab());
  const HepLorentzRotation theBoost(boostLabToCM);  

  // Boost to the CM:
  HepLorentzVector p4 = _BP4;
  p4.transform(theBoost);

    // Set the _eBeam:
  BbrDoubleErr energyCM = theBeams->energyCMWErr();
  //  double eBeam = energyCM.value() / 2.0;

  // Get the _deltaE and its error:
  //  float deltaE = p4.t() - eBeam;

  // Get _m_ES
  const Hep3Vector p0(q0.vect());
  double E0(q0.t());
  const Hep3Vector p3Lab = _BP4.vect();
  double E_ES = (.5*energyCM.value()*energyCM.value()+p0.dot(p3Lab))/E0;
  double m_ES2 = E_ES*E_ES - p3Lab.mag2();
  double m_ES = 0;

  if ( m_ES2 > 0 ) m_ES = sqrt(m_ES2);
  else {
    ErrMsg(debugging) << "m_ES2 is less than zero. Not passing candidate."
		    << endmsg;
    return false;
  }


  
  // For off-resonance running, correct m_ES for the real beam energy so
  // we can can still use the usual m_ES cut value.
  if ( _beamEn < 5.28) 
    {
      if (_debug.value() >= 2 ) 
	cout << " beam energy is less than 5.28 " 
	     << _beamEn << " will rescale m_ES " << m_ES << endl;
      if (m_ES > 0.) 
	{
	  m_ES += (-_beamEn + 5.29);
	  if (_debug.value() >= 2 ) 
	    cout << " m_ES will be shifted by " 
		 << (-_beamEn + 5.29) 
		 << " so it is now " << m_ES << endl;
	}
    }

  // Perform a loose m_ES cut.
  if (m_ES < _mEScut.value()) 
    {
      // Failed the loose m_ES cut.
      if (_debug.value() >= 2 )
	cout << "m_ES = " << m_ES << " fails loose m_ES cut" << endl;
      return false;
    }

  // Compute the B candidate's delta E.
  float FordDeltaE = -999.0;
  const BbrLorentzVectorErr q0all(theBeams->total4MomentumWErr());
  float beam_s = q0all.mag2();
  FordDeltaE = (2.0 * _BP4.dot(q0all) - beam_s) / (2.0 * sqrt(beam_s));
  
  // Perform a loose delta E cut.
  if (fabs(FordDeltaE) > _deltaEcut.value()) 
    {
      // Failed the loose delta E cut.
      if (_debug.value() >= 2 )
	cout << "delta E = " << FordDeltaE << " fails loose delta E cut"
	     << endl;
      return false;
    }
  
  //  bool status = true;

  // If we haven't already failed a cut, pass this event
  if ( _debug.value() >= 2 )
    {
      cout << "m_ES = " << m_ES << " passes loose m_ES cut" << endl;
      cout << "delta E = " << FordDeltaE << " passes loose delta E cut"
	   << endl;
    }
  
  BtaCandidate *theB= Bcombiner.create(*trackPlus,*trackMinus,*hadron);
  
  if( _debug.value() >= 2 )
    {
      cout << "B uid = " << theB->uid() << endl
	   << "B charge = " << theB->charge() << endl
	   << "Hadron uid = " << hadron->uid() << endl
	   << "Hadron charge = " << hadron->charge() << endl;

      cout << "lepton+ uid = " << trackPlus->uid() 
	   << ", m = " << trackPlus->mass() << endl
	   << ", p = " << trackPlus->p() << endl
	   << ", charge = " << trackPlus->charge() << endl;

      cout << "lepton- uid = " << trackMinus->uid() 
	   << ", m = " << trackMinus->mass() << endl
	   << ", p = " << trackMinus->p() << endl
	   << ", charge = " << trackMinus->charge() << endl;      
    }

  if( theB->charge() == 1 )
    theB->setType( Pdt::lookup("B+") );
  else if( theB->charge() == -1 )
    theB->setType( Pdt::lookup("B-") );
  else
    theB->setType( Pdt::lookup("B0") );

  _bCandList->append( theB );
  _candMes = m_ES;
  _candDeltaE = FordDeltaE;
  _candMesonMass = hadron->mass();
  _candBlock.put( *theB, _candDeltaE );
  _candBlock.put( *theB, _candMes );
  _candBlock.put( *theB, _candMesonMass );
  _candBlock.put( *theB, _candHadMode );
  _candBlock.put( *theB, _candLeptMode ); 

  return true;
}

//----------------------------------------------------------------------
// Cross-check that photons from pi0 or eta candidates do not overlap with 
// photons from brem-recovered electrons. The BtaCandidate->overlaps() 
// method could(?) be doing this, but may not always work for neutrals...
//----------------------------------------------------------------------

bool
ExclHllMiniFilter::isBremOverlap(const BtaCandidate* trackPlus, 
				 const BtaCandidate* trackMinus, 
				 const BtaCandidate* compHad)
{
  bool overlaps(false);

  BtaCandidate *hadPhot1(0);
  BtaCandidate *hadPhot2(0);
  BtaCandidate *eDaug(0);
  BtaCandidate *bremPhot1(0);
  BtaCandidate *bremPhot2(0);

  int bID1(0);
  int bID2(0);
  int hID1(0);
  int hID2(0);

  HepAListIterator<BtaCandidate> hadIter = compHad->daughterIterator();
  HepAListIterator<BtaCandidate> e1Iter  = trackPlus->daughterIterator();
  HepAListIterator<BtaCandidate> e2Iter  = trackMinus->daughterIterator();

  // If neither "lepton" is brem recovered, just return
  if( !(trackPlus->isComposite() ) && !(trackMinus->isComposite()) )
    {
      overlaps = false;
      return(overlaps);
    }

  // Check that the hadron is actually a composite...
  if( !(compHad->isComposite()) )
    {
      overlaps = false;
      return(overlaps);
    }
  
  // Get the final state photon daughters of the hadron
  hadPhot1 = hadIter();
  hadPhot2 = hadIter();

  // Get photons from brem-recovered electrons
  if( trackPlus->isComposite() )
    {
      while( (eDaug = e1Iter()) )
	{
	  if( eDaug->mass() == 0 )
	    bremPhot1 = eDaug;
	}
    }

  if( trackMinus->isComposite() )
    {
      while( (eDaug = e2Iter()) )
	{
	  if( eDaug->mass() == 0 )
	    bremPhot2 = eDaug;
	}
    }

  if( bremPhot1 != 0 )
    bID1 = bremPhot1->uid();
  if( bremPhot2 != 0 )
    bID2 = bremPhot2->uid();
  if( hadPhot1 != 0 )
    hID1 = hadPhot1->uid();
  if( hadPhot2 != 0 )
    hID2 = hadPhot2->uid();

  // We have all the photons, so now check for duplicates by uid
  if( (bID1 == hID1) ||
      (bID1 == hID2) ||
      (bID2 == hID1) ||
      (bID2 == hID2) ||
      (bID1 == bID2) ||
      (hID1 == hID2) )
    {
      if( _debug.value() >= 2 )
	cout << "ExclHllMiniFilter::isBremOverlap" << endl
	     << "duplicate photon caught: candidate will be skipped" 
	     << endl << "e+ -> e+ [gamma] uid = " << bID1 << endl
	     << "e- -> e- [gamma] uid = " << bID2 << endl
	     << "X_d -> [gamma] gamma uid = " << hID1 << endl
	     << "X_d -> gamma [gamma] uid = " << hID2 << endl;
      overlaps = true;
    }
  else
    {
      overlaps = false;
    }

  return(overlaps);
}

bool
ExclHllMiniFilter::isMember(const BtaCandidate& cand,
			       const HepAList<BtaCandidate>& list) const
{
  bool result = false;
  for (size_t j=0; j<list.length(); j++) {
    const BtaCandidate* member = list[j];
    if ( cand.uid() == member->uid() ) {
      result = true;
      break;
    }
  }

  return result;
}

