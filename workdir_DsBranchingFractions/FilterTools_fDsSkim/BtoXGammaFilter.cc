//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: BtoXGammaFilter.cc,v 1.10 2006/10/23 20:59:49 bartoldu Exp $
//
// Description:
//      See corresponding .hh file.
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Rainer Bartoldus
//      Philip Bechtle
//
// Copyright Information:
//      Copyright (C) 2006  Stanford Linear Accelerator Center
//
//--------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "FilterTools/BtoXGammaFilter.hh"

//---------------
// C++ Headers --
//---------------
#include <string>
using std::string;

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "ErrLogger/ErrLog.hh"
#include "Beta/BtaCandidate.hh"
#include "Beta/EventInfo.hh"
#include "BetaCoreTools/BtaBooster.hh"
#include "TagData/TagTransient.hh"
#include "CLHEP/Alist/AIterator.h"

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------
#include <RecoUtils/FltCounter.hh>
typedef FltCounter< 1 > BtoXGammaCounter;

//              ----------------------------------------
//              -- Public Function Member Definitions --
//              ----------------------------------------

//----------------
// Constructors --
//----------------

BtoXGammaFilter::BtoXGammaFilter( const char* const theName,
                                  const char* const theDescription )
  : TagFilterModule( theName, theDescription )
  , _eventInfoList       ( "eventInfoList",        this, "Default"         )
  , _btaPhotonList       ( "btaPhotonList",        this, "GoodPhotonLoose" )
  , _btaTrackList        ( "btaTrackList",         this, "GoodTracksLoose" )
  , _nTrackMinCut        ( "nTrackMinCut",         this,  3    )
  , _heGammaELow         ( "heGammaELow",          this,  1.45 )
  , _heGammaEHigh        ( "heGammaEHigh",         this,  3.5  )
  , _cosThetaGammaLabLow ( "cosThetaGammaLabLow",  this, -0.74 )
  , _cosThetaGammaLabHigh( "cosThetaGammaLabHigh", this,  0.93 )
  , _r2StarMaxCut        ( "r2StarMaxCut",         this,  0.7  )
  , _pTrackMaxCut        ( "pTrackMaxCut",         this,  2.5  )
{
  commands()->append( &_eventInfoList        );
  commands()->append( &_btaPhotonList        );
  commands()->append( &_btaTrackList         );
  commands()->append( &_nTrackMinCut         );
  commands()->append( &_heGammaELow          );
  commands()->append( &_heGammaEHigh         );
  commands()->append( &_cosThetaGammaLabLow  );
  commands()->append( &_cosThetaGammaLabHigh );
  commands()->append( &_r2StarMaxCut         );
  commands()->append( &_pTrackMaxCut         );
}

//--------------
// Destructor --
//--------------

BtoXGammaFilter::~BtoXGammaFilter()
{
}

//-------------
// Methods   --
//-------------

AppResult
BtoXGammaFilter::beginJob( AbsEvent*  anEvent )
{
  // initialize event counters
  _nEvents = 0;
  _nPassed = 0;

  return AppResult::OK;
}


AppResult
BtoXGammaFilter::event( AbsEvent* anEvent )
{
  setPassed( false );
  _nEvents++;

  // Cut counters.
  static BtoXGammaCounter allCnt           ( "all"            );
  static BtoXGammaCounter bgfMultiHadronCnt( "BGFMultiHadron" );
  static BtoXGammaCounter nTrackCnt        ( "nTrack"         );
  static BtoXGammaCounter eGammaCnt        ( "eGamma"         );
  static BtoXGammaCounter cosThetaGammaCnt ( "cosThetaGamma"  );
  static BtoXGammaCounter r2StarCnt        ( "r2Star"         );
#if 0
  static BtoXGammaCounter p1MagCnt         ( "p1Mag"          );
#endif
  static BtoXGammaCounter pTrackMaxCnt     ( "pTrackMax"      );

  allCnt++;

  // Create transient tag.

  TagFilterModule::event( anEvent );

  if ( ! tag() )
    {
      ErrMsg(fatal)
        << " No transient tag.  Aborting." << endmsg;
      return AppResult::ERROR;
    }

  // Require BGFilter tag bit.

  const string bgfMultiHadronKey = "BGFMultiHadron";
  bool bgfMultiHadron = false;

  if ( ! tag()->getBool( bgfMultiHadron, bgfMultiHadronKey ) )
    {
      ErrMsg(error) << " Failed to find Tag bool " << bgfMultiHadronKey << endmsg;
      return AppResult::ERROR;
    }

  if ( ! bgfMultiHadron )
    {
      return AppResult::OK;
    }

  bgfMultiHadronCnt++;

  // Get track list from the event.

  const HepAList< BtaCandidate >* btaTrackList =
    Ifd< HepAList< BtaCandidate > >::get( anEvent, _btaTrackList.value() );

  if ( ! btaTrackList )
    {
      ErrMsg(error)
        << " Failed to find BtaCandidate list in event"
        << " using secondary key " << _btaTrackList.value()
        << endmsg;
      return AppResult::ERROR;
    }

  // Require minimum number of tracks.

  if ( ! ( btaTrackList->length() >= _nTrackMinCut.value() ) )
    {
      return AppResult::OK;
    }

  nTrackCnt++;

  // Make a booster into the CM frame.

  const HepAList< EventInfo >* eventInfoList =
    Ifd< HepAList< EventInfo > >::get( anEvent, _eventInfoList.value() );

  if ( ! eventInfoList )
    {
      ErrMsg(error)
        << " Failed to get EventInfo list from the event"
        << " using secondary key " << _eventInfoList.value()
        << endmsg;
      return AppResult::ERROR;
    }

  const EventInfo* eventInfo = eventInfoList->first();

  if ( ! eventInfo )
    {
      ErrMsg(error)
        << " Failed to get EventInfo from list"
        << endmsg;
      return AppResult::ERROR;
    }

  const BtaBooster cmBooster( eventInfo->cmFrame() );

  // Get photon list from the event.

  const HepAList< BtaCandidate >* btaPhotonList =
    Ifd< HepAList< BtaCandidate > >::get( anEvent, _btaPhotonList.value() );

  if ( ! btaPhotonList )
    {
      ErrMsg(error)
        << " Failed to find BtaCandidate list in event"
        << " using secondary key " << _btaPhotonList.value()
        << endmsg;
      return AppResult::ERROR;
    }

  // Find a high-energy photon candidate in the fiducial volume.

  bool foundPhoton = false;
  bool foundPhotonFiducial = false;
  HepAListIterator<BtaCandidate> photonIter( *btaPhotonList );

  while ( const BtaCandidate* photonCand = photonIter() )
    {
      const HepLorentzVector cmP = cmBooster.boostedP4( *photonCand );

      if ( _heGammaELow.value() <= cmP.e() && cmP.e() < _heGammaEHigh.value() )
        {
          foundPhoton = true;
          const double cosThetaGamma = photonCand->p4().cosTheta();

          if ( _cosThetaGammaLabLow.value() <= cosThetaGamma &&
               cosThetaGamma < _cosThetaGammaLabHigh.value() )
            {
              foundPhotonFiducial = true;
            }
        }
    }

  if ( ! foundPhoton )
    {
      return AppResult::OK;
    }

  eGammaCnt++;

  if ( ! foundPhotonFiducial )
    {
      return AppResult::OK;
    }

  cosThetaGammaCnt++;

  // Cut on R2.

  const string r2AllKey = "R2All";
  float r2All = 1.;

  if ( ! tag()->getFloat( r2All, r2AllKey ) )
    {
      ErrMsg(error) << " Failed to find Tag float " << r2AllKey << endmsg;
      return AppResult::ERROR;
    }

  if ( ! ( r2All < _r2StarMaxCut.value() ) )
    {
      return AppResult::OK;
    }

  r2StarCnt++;

  // Reject events with very high momentum tracks.

#if 0
  const string p1MagKey = "p1Mag";
  float p1Mag = 0.;

  if ( ! tag()->getFloat( p1Mag, "p1Mag" ) )
    {
      ErrMsg(error) << " Failed to find Tag float " << p1MagKey << endmsg;
      return AppResult::ERROR;
    }

  if ( ! ( p1Mag < _pTrackMaxCut.value() ) )
    {
      return AppResult::OK;
    }

  p1MagCnt++;
#endif

  double pTrackMax = 0.;
  HepAListIterator<BtaCandidate> trackIter( *btaTrackList );

  while ( const BtaCandidate* trackCand = trackIter() )
    {
      const HepLorentzVector cmP = cmBooster.boostedP4( *trackCand );

      if ( cmP.vect().mag() > pTrackMax )
        {
          pTrackMax = cmP.vect().mag();
        }
    }

  if ( ! ( pTrackMax < _pTrackMaxCut.value() ) )
    {
      return AppResult::OK;
    }

  pTrackMaxCnt++;

  // Keep the event.

  _nPassed++;
  setPassed( true );
  return AppResult::OK;
}


AppResult
BtoXGammaFilter::endJob( AbsEvent* anEvent )
{
  ErrMsg(routine)
    << " End job: " << _nPassed << " out of "
    << _nEvents << " passed" << endmsg;

  ostream& os = ErrMsg(routine);
  BtoXGammaCounter::print( os );
  os << endmsg;
  BtoXGammaCounter::printRatios( os );
  os << endmsg;

  return AppResult::OK;
}


AppModule*
BtoXGammaFilter::clone( const char *cloneName )
{
  return new BtoXGammaFilter( cloneName, "clone of BtoXGammaFilter" );
}

