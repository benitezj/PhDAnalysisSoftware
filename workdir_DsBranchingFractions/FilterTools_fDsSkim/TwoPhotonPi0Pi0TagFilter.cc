//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: TwoPhotonPi0Pi0TagFilter.cc,v 1.3 2004/08/09 07:00:45 bartoldu Exp $
//
// Description:
//	An example module that filters on the transient tag.
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Nic
//
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
#include "FilterTools/TwoPhotonPi0Pi0TagFilter.hh"

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
TwoPhotonPi0Pi0TagFilter::TwoPhotonPi0Pi0TagFilter( const char* const theName, 
			const char* const theDescription )
  : TagFilterModule( theName, theDescription )
   , _maxTotalEnergy("maxTotalEnergy",this, 20.0)
{

  commands()->append(& _maxTotalEnergy);

}
		

//--------------
// Destructor --
//--------------
TwoPhotonPi0Pi0TagFilter::~TwoPhotonPi0Pi0TagFilter()
{
}

//-------------
// Methods   --
//-------------

AppResult
TwoPhotonPi0Pi0TagFilter::beginJob( AbsEvent*  anEvent )
{
  if ( _verbose.value() ) cout << name() << ": begin job" << endl;
  // initialize event counters
  _nReadEvents = 0;
  _nPassed = 0;
  return AppResult::OK;
}


AppResult 
TwoPhotonPi0Pi0TagFilter::event( AbsEvent* anEvent )
{

  _nReadEvents++;

  bool tagbool;
  bool haveTagBit1 = false;
  bool haveTagBit2 = false;
  int tagint;
  float tagfloat;
  bool passed = false;

  // Build the tag accessor using the base class
  TagFilterModule::event( anEvent );

  // get the tags we need as we go
  if (tag()->getBool( tagbool, "BGFAllNeutralTwoPhoton" ) ) { haveTagBit1 = tagbool; }
  if (tag()->getBool( tagbool, "DigiFL3Open" ) ) { haveTagBit2 = tagbool; }
  if(haveTagBit1 || haveTagBit2) {
    int nGoodTrkLoose = 0;
    if( tag()->getInt( tagint, "nGoodTrkLoose" ) ) { nGoodTrkLoose = tagint; }
    if( nGoodTrkLoose<4 ) {
      float etotal = 100;
      if ( tag( )->getFloat( tagfloat, "eTotal") ) { etotal = tagfloat; }
      if(etotal < _maxTotalEnergy.value() ) {
	passed = true; 
      }
    }
  }

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
TwoPhotonPi0Pi0TagFilter::endJob( AbsEvent* anEvent ) {

  cout << name()  << " end job: " << _nPassed << " out of " 
       << _nReadEvents << " passed" << endl;
  
  return AppResult::OK;
}


