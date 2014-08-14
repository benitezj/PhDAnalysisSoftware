//--------------------------------------------------------------------------
// File and Version Information:
//
// Description:
//	Module that filters on the transient tag to get B => 4 body 
//      final states.
//      http://www.slac.stanford.edu/~gritsan/protected/B4body.html
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Andrei Gritsan
//
// Copyright Information:
//
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------

#include "FilterTools/TagBFourBodyFilter.hh"

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
TagBFourBodyFilter::TagBFourBodyFilter( const char* const theName, 
			const char* const theDescription )
  : TagFilterModule( theName, theDescription )
{
}
		

//--------------
// Destructor --
//--------------
TagBFourBodyFilter::~TagBFourBodyFilter()
{
}

//-------------
// Methods   --
//-------------

AppResult
TagBFourBodyFilter::beginJob( AbsEvent*  anEvent )
{
  if ( _verbose.value() ) cout << name() << ": begin job" << endl;

  // initialize event counters
  _nReadEvents = 0;
  _nPassed = 0;
  return AppResult::OK;
}


AppResult 
TagBFourBodyFilter::event( AbsEvent* anEvent )
{
  _nReadEvents++;
  bool  aBool;
  bool  passed = false;

  // Build the tag accessor using the base class
  TagFilterModule::event( anEvent );

  // get the tags we need
  bool isB4bodyhhhh1 = false;
  bool isB4bodyhhhp1 = false;
  bool isB4bodyhhhk1 = false;
  bool isB4bodyhhhe1 = false;
  bool isB4bodyhhpp1 = false;
  bool isB4bodyhhpk1 = false;
  bool isB4bodyhhpe1 = false;
  bool isB4bodyhhkk1 = false;
  bool isB4bodyhhke1 = false;
  bool isB4bodyhhhh2 = false;
  bool isB4bodyhhhp2 = false;
  bool isB4bodyhhhk2 = false;
  bool isB4bodyhhhe2 = false;
  bool isB4bodyhhpp2 = false;
  bool isB4bodyhhpk2 = false;
  bool isB4bodyhhpe2 = false;
  bool isB4bodyhhkk2 = false;
  bool isB4bodyhhke2 = false;

  if ( tag( )->getBool( aBool, "B4bodyhhhh1" ) ) { isB4bodyhhhh1 = aBool; }
  if ( tag( )->getBool( aBool, "B4bodyhhhp1" ) ) { isB4bodyhhhp1 = aBool; }
  if ( tag( )->getBool( aBool, "B4bodyhhhk1" ) ) { isB4bodyhhhk1 = aBool; }
  if ( tag( )->getBool( aBool, "B4bodyhhhe1" ) ) { isB4bodyhhhe1 = aBool; }
  if ( tag( )->getBool( aBool, "B4bodyhhpp1" ) ) { isB4bodyhhpp1 = aBool; }
  if ( tag( )->getBool( aBool, "B4bodyhhpk1" ) ) { isB4bodyhhpk1 = aBool; }
  if ( tag( )->getBool( aBool, "B4bodyhhpe1" ) ) { isB4bodyhhpe1 = aBool; }
  if ( tag( )->getBool( aBool, "B4bodyhhkk1" ) ) { isB4bodyhhkk1 = aBool; }
  if ( tag( )->getBool( aBool, "B4bodyhhke1" ) ) { isB4bodyhhke1 = aBool; }
  if ( tag( )->getBool( aBool, "B4bodyhhhh2" ) ) { isB4bodyhhhh2 = aBool; }
  if ( tag( )->getBool( aBool, "B4bodyhhhp2" ) ) { isB4bodyhhhp2 = aBool; }
  if ( tag( )->getBool( aBool, "B4bodyhhhk2" ) ) { isB4bodyhhhk2 = aBool; }
  if ( tag( )->getBool( aBool, "B4bodyhhhe2" ) ) { isB4bodyhhhe2 = aBool; }
  if ( tag( )->getBool( aBool, "B4bodyhhpp2" ) ) { isB4bodyhhpp2 = aBool; }
  if ( tag( )->getBool( aBool, "B4bodyhhpk2" ) ) { isB4bodyhhpk2 = aBool; }
  if ( tag( )->getBool( aBool, "B4bodyhhpe2" ) ) { isB4bodyhhpe2 = aBool; }
  if ( tag( )->getBool( aBool, "B4bodyhhkk2" ) ) { isB4bodyhhkk2 = aBool; }
  if ( tag( )->getBool( aBool, "B4bodyhhke2" ) ) { isB4bodyhhke2 = aBool; }

  // Make decision based on the tag values & set the "passed" flag.
  if ( (isB4bodyhhhh1 && isB4bodyhhhh2) ||
       (isB4bodyhhhp1 && isB4bodyhhhp2) ||
       (isB4bodyhhhk1 && isB4bodyhhhk2) ||
       (isB4bodyhhhe1 && isB4bodyhhhe2) ||
       (isB4bodyhhpp1 && isB4bodyhhpp2) ||
       (isB4bodyhhpk1 && isB4bodyhhpk2) ||
       (isB4bodyhhpe1 && isB4bodyhhpe2) ||
       (isB4bodyhhkk1 && isB4bodyhhkk2) ||
       (isB4bodyhhke1 && isB4bodyhhke2) ) {passed = true;}

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
TagBFourBodyFilter::endJob( AbsEvent* anEvent ) {

  cout << name()  << " end job: " << _nPassed << " out of " 
       << _nReadEvents << " passed" << endl;
  
  return AppResult::OK;
}

