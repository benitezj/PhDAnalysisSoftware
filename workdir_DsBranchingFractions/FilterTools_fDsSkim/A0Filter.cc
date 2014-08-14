//--------------------------------------------------------------------------
// File and Version Information:
//
// Description:
//	Module to filters on the A0 transient tag.
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Philip J. Clark
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "FilterTools/A0Filter.hh"

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
A0Filter::A0Filter( const char* const theName, 
			const char* const theDescription )
  : TagFilterModule( theName, theDescription )
{
}
		

//--------------
// Destructor --
//--------------
A0Filter::~A0Filter()
{
}

//-------------
// Methods   --
//-------------

AppResult
A0Filter::beginJob( AbsEvent*  anEvent )
{
  if ( _verbose.value() ) cout << name() << ": begin job" << endl;

  // initialize event counters
  _nReadEvents = 0;
  _nPassed = 0;
  return AppResult::OK;
}


AppResult 
A0Filter::event( AbsEvent* anEvent )
{
  _nReadEvents++;
  
  bool aBool=false;
  bool a0k0=false;
  bool a0trk=false;
  bool a0kstar=false;
  bool a0rho=false;
  bool a0pi0=false;
  //  bool a0Excl=false;
  bool a0incl=false;
  bool passed = false;

  // Build the tag accessor using the base class
  TagFilterModule::event( anEvent );

  // get the tags we need
  if ( tag()->getBool( aBool, "a0K0"   ) ) { a0k0    = aBool; }
  if ( tag()->getBool( aBool, "a0Trk"  ) ) { a0trk   = aBool; }
  if ( tag()->getBool( aBool, "a0Kstar") ) { a0kstar = aBool; }
  if ( tag()->getBool( aBool, "a0Rho"  ) ) { a0rho   = aBool; }
  if ( tag()->getBool( aBool, "a0Pi0"  ) ) { a0pi0   = aBool; }
  if ( tag()->getBool( aBool, "a0Incl" ) ) { a0incl  = aBool; }

  bool isA0;
  isA0 = a0k0 || a0trk || a0kstar || a0rho || a0pi0 ;
  
  if ( _verbose.value() ) {cout << "isA0 : " << (int)isA0 << endl;}

  // Make decision based on the tag values & set the "passed" flag.
  passed = isA0;

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
A0Filter::endJob( AbsEvent* anEvent ) {

  cout << name()  << " end job: " << _nPassed << " out of " 
       << _nReadEvents << " passed" << endl;
  
  return AppResult::OK;
}


