//--------------------------------------------------------------------------
// File and Version Information:
//
// Description:
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//
// Copyright Information:
//	Copyright (C) 2005      SLAC
//
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "FilterTools/FilterBToPPPP.hh"

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
#include "TagData/TagTransient.hh"
#include "TagData/TagTransientBoolIter.hh"
#include "TagData/TagTransientIntIter.hh"
#include "TagData/TagTransientFloatIter.hh"

#include "EventTagTools/TagSinglePart.hh"

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
FilterBToPPPP::FilterBToPPPP( const char* const theName, 
			const char* const theDescription )
  : TagFilterModule( theName, theDescription )
  , _tagbit        ("tagbit",        this)
  , _minNoOfTracks ("minNoOfTracks", this, 1)
  , _maxTotalEnergy("maxTotalEnergy",this, 20.0)
{
  commands()->append(& _tagbit);
  commands()->append(& _minNoOfTracks);
  commands()->append(& _maxTotalEnergy);
}

//--------------
// Destructor --
//--------------
FilterBToPPPP::~FilterBToPPPP()
{
}

//-------------
// Methods   --
//-------------

AppResult
FilterBToPPPP::beginJob( AbsEvent*  anEvent )
{
  if ( _verbose.value() ) cout << name() << ": begin job" << endl;

  // initialize event counters
  _nReadEvents = 0;
  _nPassed = 0;

  if (_verbose.value()) {
    cout << name() << ": Tags included in this skim: " << endl;
    int ntags = _tagbit.value().size();
    for (int i=0; i<ntags; i++) cout << _tagbit.value()[i] << endl;
  }
  
  return AppResult::OK;
}


AppResult 
FilterBToPPPP::event( AbsEvent* anEvent )
{
  _nReadEvents++;
  bool passed = false;
  setPassed( false );
  
  // Event tags
  TagFilterModule::event( anEvent );

  bool aBool; 
  int  ntags = _tagbit.value().size();
  for (int i=0; i<ntags; i++) {
    if (_verbose.value()) cout << name() << ": testing " << _tagbit.value()[i] << endl; 
    if ( tag()->getBool( aBool, _tagbit.value()[i] ) && aBool ) { 
      passed = aBool; 
      if (_verbose.value()) cout << name() << ": Found " << _tagbit.value()[i] << endl; 
    }
  }
  
  if (passed) {
    _nPassed++;
    if (_verbose.value()) cout << name() << ":  Total=" << _nReadEvents << "  accepted=" << _nPassed << endl;
  }

  setPassed( passed );
  return AppResult::OK;

}

AppResult
FilterBToPPPP::endJob( AbsEvent* anEvent ) {

  cout << name()  << " end job: " << _nPassed << " out of " 
       << _nReadEvents << " passed" << endl;
  
  return AppResult::OK;
}



