//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: B3body03aTagFilter.cc,v 1.3 2004/08/09 07:00:44 bartoldu Exp $
//
// Description:
//	An example module that filters on the transient tag.
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Paul Harrison and Theresa Champion - based on Original template
//	by Charles Young
//       modified by F Wilson for CM2
//
// Copyright Information:
//	Copyright (C) 1998      SLAC
//	Copyright (C) 2000      Hedgehog concepts :-)
//
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "FilterTools/B3body03aTagFilter.hh"

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
B3body03aTagFilter::B3body03aTagFilter( const char* const theName, 
			const char* const theDescription )
  : TagFilterModule( theName, theDescription )
  , _3bodytagbit("3bodytagbit",this, "BCCC03a3body")
  , _minNoOfTracks("minNoOfTracks",this, 1)
  , _maxTotalEnergy("maxTotalEnergy",this, 20.0)
{

  commands()->append(& _3bodytagbit);
  commands()->append(& _minNoOfTracks);
  commands()->append(& _maxTotalEnergy);

}
		

//--------------
// Destructor --
//--------------
B3body03aTagFilter::~B3body03aTagFilter()
{
}

//-------------
// Methods   --
//-------------

AppResult
B3body03aTagFilter::beginJob( AbsEvent*  anEvent )
{
  if ( _verbose.value() ) cout << name() << ": begin job" << endl;

  // initialize event counters
  _nReadEvents = 0;
  _nPassed = 0;
  return AppResult::OK;
}


AppResult 
B3body03aTagFilter::event( AbsEvent* anEvent )
{
  _nReadEvents++;

  int tagint;
  float tagfloat;
  int ntracks = 0;
  float etotal = 0.;
  bool aBool;
  bool haveTagBit = false;
  bool passed = false;

  // Build the tag accessor using the base class
  TagFilterModule::event( anEvent );

  // get the tags we need
  if ( tag( )->getInt( tagint, "nTracks") ) { ntracks = tagint; }
  if ( tag( )->getFloat( tagfloat, "eTotal") ) { etotal = tagfloat; }
  if ( tag( )->getBool( aBool, _3bodytagbit.value() ) ) { haveTagBit = aBool; }

  if (haveTagBit == true && ntracks >= _minNoOfTracks.value() && 
     etotal < _maxTotalEnergy.value()) { passed = true; }

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
B3body03aTagFilter::endJob( AbsEvent* anEvent ) {

  cout << name()  << " end job: " << _nPassed << " out of " 
       << _nReadEvents << " passed" << endl;
  
  return AppResult::OK;
}


