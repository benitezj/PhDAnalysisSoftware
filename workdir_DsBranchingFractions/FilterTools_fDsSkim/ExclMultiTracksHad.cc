//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: ExclMultiTracksHad.cc,v 1.2 2007/01/04 12:13:53 negrini Exp $
//
// Description:
//     Preselection of multitrack events
//     Reject events with one or more tight leptons
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//     M. Negrini               Originator
//
// Copyright Information:
//      Copyright (C) 2005      INFN Ferrara
//
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------

#include "FilterTools/ExclMultiTracksHad.hh"

//-------------
// C Headers --
//-------------
#include <assert.h>
extern "C" {
}

//---------------
// C++ Headers --
//---------------
#include <iostream>
using std::cout;
using std::endl;

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "TagData/TagTransient.hh"
#include "TagData/TagTransientBoolIter.hh"
#include "TagData/TagTransientIntIter.hh"
#include "TagData/TagTransientFloatIter.hh"

#include "AbsEnv/AbsEnv.hh"
#include "GenEnv/GenEnv.hh"

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

//----------------
// Constructors --
//----------------
ExclMultiTracksHad::ExclMultiTracksHad( const char* const theName, 
			const char* const theDescription )
  : TagFilterModule( theName, theDescription )
{
}
		

//--------------
// Destructor --
//--------------
ExclMultiTracksHad::~ExclMultiTracksHad()
{
}

//-------------
// Methods   --
//-------------

AppResult
ExclMultiTracksHad::beginJob( AbsEvent*  anEvent )
{
  if ( _verbose.value() ) cout << name() << ": begin job" << endl;

  // initialize event counters
  _nReadEvents = 0;
  _nPassed = 0;
  return AppResult::OK;
}


AppResult 
ExclMultiTracksHad::event( AbsEvent* anEvent )
{
  _nReadEvents++;

  _tag = Ifd<AbsEventTag>::get( anEvent );

  bool  passed = true;

  int tagNumTracks;
  int tagChargeTracks;
  float tagMomElec;
  float tagMomMuon;
  float tagMassTrkTot;

//  tag()->getInt(tagNumTracks,"nGoodTrkLoose");
  tag()->getInt(tagNumTracks,"nTracks");
  tag()->getInt(tagChargeTracks,"totChargeLoose");
  tag()->getFloat(tagMomElec,"elecTight1l");
  tag()->getFloat(tagMomMuon,"muonTight1l");
  tag()->getFloat(tagMassTrkTot,"pTotalMass");


  // Use tag informations for the selection
//  if (tagNumTracks<=3 || tagNumTracks%2!=0) passed = false;
  if (tagNumTracks<=3) passed = false;
//  if (tagChargeTracks!=0) passed = false;
  if (abs(tagChargeTracks)>1) passed = false;
  if (tagMomElec!=0. || tagMomMuon!=0.) passed = false;
  if (tagMassTrkTot<7.0) passed = false;


  if (passed) {
    _nPassed++;
    if ( _verbose.value() ) cout << name() << ": event # " 
				 << _nReadEvents << " passed." << endl;
  } else {
    if ( _verbose.value() ) cout << name() << ": event # " 
				 << _nReadEvents << " rejected." << endl;
  }

  setPassed( passed );
  return AppResult::OK;

}

AppResult
ExclMultiTracksHad::endJob( AbsEvent* anEvent ) {

  cout << name()  << " end job: " << _nPassed << " out of " 
       << _nReadEvents << " passed" << endl;
  
  return AppResult::OK;
}

