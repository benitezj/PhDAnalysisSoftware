//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: InclK0sFilter.cc,v 1.8 2005/01/03 19:17:05 ddujmic Exp $
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
#include "FilterTools/InclK0sFilter.hh"

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
InclK0sFilter::InclK0sFilter( const char* const theName, 
			const char* const theDescription )
  : TagFilterModule( theName, theDescription )
  , _pmax("pmax",this, 3.1)
  , _pmin("pmin",this, 1.9)
  , _cosTcut("cosTcut",this, 1.0)
{
  commands()->append(& _pmax);
  commands()->append(& _pmin);
  commands()->append(& _cosTcut);
}

//--------------
// Destructor --
//--------------
InclK0sFilter::~InclK0sFilter()
{
}

//-------------
// Methods   --
//-------------

AppResult
InclK0sFilter::beginJob( AbsEvent*  anEvent )
{
  if ( _verbose.value() ) cout << name() << ": begin job" << endl;

  // initialize event counters
  _nReadEvents = 0;
  _nPassed = 0;
  return AppResult::OK;
}


AppResult 
InclK0sFilter::event( AbsEvent* anEvent )
{
  _nReadEvents++;
  bool passed = false;
  bool isK0sIncl = false;
  int K0sIncl =0;
  float _pMax = 0.;
  float _cosT = -3.;
  
  // Build the tag accessor using the base class
  TagFilterModule::event( anEvent );

  // get and unpack the tags we need
  if ( tag()->getInt( K0sIncl, "TagK_S0TwoBody_maxCmsP" ) ) {
    _pMax = TagSinglePart::unpack1st(K0sIncl); // highest p*
  }
  
  if ( _verbose.value( ) ) {cout << "K0sIncl : " << (int)K0sIncl << endl;}

  //make the cuts

  if ( _pMax > _pmin.value() && _pMax < _pmax.value() ){
    isK0sIncl = true;
  }

  // check output
  if ( _verbose.value( ) ) {
    cout << "TagKsPengTwoBody_maxCmsP" << K0sIncl << endl;
    cout << "_pMax " << _pMax << "_cosT" << _cosT << endl;
  }

  if ( _verbose.value( ) ) {cout << "isK0sIncl : " << (int)isK0sIncl << endl;}

  // Make decision based on the tag values & set the "passed" flag.
  passed = isK0sIncl;

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
InclK0sFilter::endJob( AbsEvent* anEvent ) {

  cout << name()  << " end job: " << _nPassed << " out of " 
       << _nReadEvents << " passed" << endl;
  
  return AppResult::OK;
}



