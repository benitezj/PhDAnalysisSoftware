//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: InclEtaFilter.cc,v 1.9 2005/01/03 19:17:04 ddujmic Exp $
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
// Changes:
//      J. Roy  30Mar2000
//      Fixed cut-and-paste bug when applying cuts
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
#include "FilterTools/InclEtaFilter.hh"

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
InclEtaFilter::InclEtaFilter( const char* const theName, 
			const char* const theDescription )
  : TagFilterModule( theName, theDescription )
  , _pmax("pmax",this, 3.1)
  , _pmin("pmin",this, 1.9)
  , _cosTcut("cosTcut",this, 0.99)
{
  commands()->append(& _pmax);
  commands()->append(& _pmin);
  commands()->append(& _cosTcut);
}

//--------------
// Destructor --
//--------------
InclEtaFilter::~InclEtaFilter()
{
}

//-------------
// Methods   --
//-------------

AppResult
InclEtaFilter::beginJob( AbsEvent*  anEvent )
{
  if ( _verbose.value() ) cout << name() << ": begin job" << endl;

  // initialize event counters
  _nReadEvents = 0;
  _nPassed = 0;
  return AppResult::OK;
}


AppResult 
InclEtaFilter::event( AbsEvent* anEvent )
{
  _nReadEvents++;

  bool passed = false;
  bool iseta = false;
  int tagint = 0;
  float EtaPmax = 0.;
  float EtaCosT = -3.;
  float EtaPeppPmax = 0.;
  float EtaPeppCosT = -3.;
  
  // Build the tag accessor using the base class
  TagFilterModule::event( anEvent );

  // get and unpack the tags we need
  if ( tag()->getInt( tagint, "TagEtaTwoBody_maxCmsP" ) ) { 
    EtaPmax = TagSinglePart::unpack1st(tagint); // highest p*
  }
  if ( tag()->getInt( tagint, "TagEtaPeppTwoBody_maxCmsP" ) ) { 
    EtaPeppPmax = TagSinglePart::unpack1st(tagint); // highest p*
  }

  //make the cuts

  if ( ( EtaPmax > _pmin.value() && EtaPmax < _pmax.value() )
    || ( EtaPeppPmax > _pmin.value() && EtaPeppPmax < _pmax.value() ) ){
    iseta = true;
  }

  // check output
  if ( _verbose.value( ) ) {
    cout << "EtaPmax " << EtaPmax << "EtaCosT" << EtaCosT << endl;
    cout << "EtaPeppPmax " << EtaPeppPmax << "EtaPeppCosT" << EtaPeppCosT << endl;
  }

  if ( _verbose.value( ) ) {cout << "iseta : " << (int)iseta << endl;}


  // Make decision based on the tag values & set the "passed" flag.
  passed = iseta;

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
InclEtaFilter::endJob( AbsEvent* anEvent ) {

  cout << name()  << " end job: " << _nPassed << " out of " 
       << _nReadEvents << " passed" << endl;
  
  return AppResult::OK;
}


