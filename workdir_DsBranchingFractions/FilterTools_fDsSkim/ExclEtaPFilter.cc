//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: ExclEtaPFilter.cc,v 1.8 2004/08/09 07:00:45 bartoldu Exp $
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
//
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "FilterTools/ExclEtaPFilter.hh"

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
ExclEtaPFilter::ExclEtaPFilter( const char* const theName, 
			const char* const theDescription )
  : TagFilterModule( theName, theDescription )
{
}
		

//--------------
// Destructor --
//--------------
ExclEtaPFilter::~ExclEtaPFilter()
{
}

//-------------
// Methods   --
//-------------

AppResult
ExclEtaPFilter::beginJob( AbsEvent*  anEvent )
{
  if ( _verbose.value() ) cout << name() << ": begin job" << endl;

  // initialize event counters
  _nReadEvents = 0;
  _nPassed = 0;
  return AppResult::OK;
}


AppResult 
ExclEtaPFilter::event( AbsEvent* anEvent )
{
  _nReadEvents++;
  
  bool aBool=false;
  bool etapk0=false;
  bool etapkpi=false;
  bool etapkstar=false;
  bool etaprhorho0=false;
  bool etappi0phietap=false;
  bool etapetaomega=false;
  bool passed = false;

  // Build the tag accessor using the base class
  TagFilterModule::event( anEvent );

  // get the tags we need
  if ( tag()->getBool( aBool, "EtapK0" ) ) { etapk0 = aBool; }
  if ( tag()->getBool( aBool, "EtapKPi" ) ) { etapkpi = aBool; }
  if ( tag()->getBool( aBool, "EtapKstar" ) ) { etapkstar = aBool; }
  if ( tag()->getBool( aBool, "EtapRhoRho0" ) ) { etaprhorho0 = aBool; }
  if ( tag()->getBool( aBool, "EtapPi0PhiEtap" ) ) { etappi0phietap = aBool; }
  if ( tag()->getBool( aBool, "EtapEtaOmega" ) ) { etapetaomega = aBool; }

  bool isEtap;
  isEtap = etapk0 || etapkpi || etapkstar || etaprhorho0 || 
      etappi0phietap || etapetaomega;
  
  if ( _verbose.value() ) {cout << "isEtap : " << (int)isEtap << endl;}


  // Make decision based on the tag values & set the "passed" flag.
  passed = isEtap;

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
ExclEtaPFilter::endJob( AbsEvent* anEvent ) {

  cout << name()  << " end job: " << _nPassed << " out of " 
       << _nReadEvents << " passed" << endl;
  
  return AppResult::OK;
}


