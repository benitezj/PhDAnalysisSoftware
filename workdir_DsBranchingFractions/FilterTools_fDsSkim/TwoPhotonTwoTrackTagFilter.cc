//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: TwoPhotonTwoTrackTagFilter.cc,v 1.6 2005/04/16 17:37:59 boyd Exp $
//
// Description:
//	An example module that filters on the transient tag.
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Nic
//        Jamie Boyd - changed to use new BGF bit if available (feb 2005)
//
// Copyright Information:
//	Copyright (C) 1998      SLAC
//	Copyright (C) 2000      Wah
//
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "FilterTools/TwoPhotonTwoTrackTagFilter.hh"

//-------------
// C Headers --
//-------------
#include <assert.h>
extern "C" {
}

//---------------
// C++ Headers --
//---------------

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "TagData/TagTransient.hh"
#include "TagData/TagTransientBoolIter.hh"
#include "TagData/TagTransientIntIter.hh"
#include "TagData/TagTransientFloatIter.hh"

#include "AbsEnv/AbsEnv.hh"
#include "GenEnv/GenEnv.hh"
using std::cout;
using std::endl;

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

//----------------
// Constructors --
//----------------
TwoPhotonTwoTrackTagFilter::TwoPhotonTwoTrackTagFilter( const char* const theName, 
			const char* const theDescription )
  : TagFilterModule( theName, theDescription )
  , _maxTotalEnergy("maxTotalEnergy",this, 8.0)
{

  commands()->append(& _maxTotalEnergy);

}
		

//--------------
// Destructor --
//--------------
TwoPhotonTwoTrackTagFilter::~TwoPhotonTwoTrackTagFilter()
{
}

//-------------
// Methods   --
//-------------

AppResult
TwoPhotonTwoTrackTagFilter::beginJob( AbsEvent*  anEvent )
{
  if ( _verbose.value() ) cout << name() << ": begin job" << endl;
  // initialize event counters
  _nReadEvents = 0;
  _nPassed = 0;
  return AppResult::OK;
}


AppResult 
TwoPhotonTwoTrackTagFilter::event( AbsEvent* anEvent )
{
  _nReadEvents++;

  //TwoPhotonTwoTrackTagFilter:
  //      BGFTwoProng || DigiFL3Open
  //      E < 8.0 GeV
  //      P_T < 0.7

  bool tagbool;
  bool haveTagBit1 = false;
  bool haveTagBit2 = false;
  float tagfloat;

  bool passed = false;

  // Build the tag accessor using the base class
  TagFilterModule::event( anEvent );

  // get the tags we need as we go
  if (tag()->getBool( tagbool, "BGFTwoPhotonTwoTrack" ) ) { haveTagBit1 = tagbool; }
  else { if (tag()->getBool( tagbool, "BGFTwoProng" ) ) { haveTagBit1 = tagbool; }}
  if (tag()->getBool( tagbool, "DigiFL3Open" ) ) { haveTagBit2 = tagbool; }
  if(haveTagBit2) {
    float etotal = 100.;
    if ( tag( )->getFloat( tagfloat, "eTotal") ) { etotal = tagfloat; }
    if(etotal < _maxTotalEnergy.value() ) {
      float pTotCosTh = 0;
      if ( tag( )->getFloat( tagfloat, "pTotalCosTh") ) { pTotCosTh = tagfloat; }
      float pTotMag = 100;
      if ( tag( )->getFloat( tagfloat, "pTotalMag") ) { pTotMag = tagfloat; }
      if(sqrt(1-pTotCosTh*pTotCosTh)*pTotMag < 0.7) {
	passed = true; 
      }
    }
  }
  passed|=haveTagBit1;
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
TwoPhotonTwoTrackTagFilter::endJob( AbsEvent* anEvent ) {

  cout << name()  << " end job: " << _nPassed << " out of " 
       << _nReadEvents << " passed" << endl;
  
  return AppResult::OK;
}
