//--------------------------------------------------------------------------
// File and Version Information:
//
// Description:
//      Module to filter on Eta'(rho gamma) inclusive tag
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
#include "FilterTools/InclEtapRhoGamFilter.hh"

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
InclEtapRhoGamFilter::InclEtapRhoGamFilter( const char* const theName, 
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
InclEtapRhoGamFilter::~InclEtapRhoGamFilter()
{
}

//-------------
// Methods   --
//-------------

AppResult
InclEtapRhoGamFilter::beginJob( AbsEvent*  anEvent )
{
  if ( _verbose.value() ) cout << name() << ": begin job" << endl;

  // initialize event counters
  _nReadEvents = 0;
  _nPassed = 0;
  return AppResult::OK;
}


AppResult 
InclEtapRhoGamFilter::event( AbsEvent* anEvent )
{
  _nReadEvents++;
  bool passed = false;
  bool isEtapRhoGamIncl = false;
  int  EtapRhoGamIncl = 0;
  float _pMax = 0.;

  // Build the tag accessor using the base class
  TagFilterModule::event( anEvent );

  // get and unpack the tags we need
  if ( tag()->getInt(EtapRhoGamIncl, "TagEtaPrhogTwoBody_maxCmsP" ) ) { 
    _pMax = TagSinglePart::unpack1st(EtapRhoGamIncl); // highest p*
  }

  if ( _verbose.value( ) ) {cout << "EtapRhoGamIncl : " << (int)EtapRhoGamIncl << endl;}

  //make the cuts

  if ( _pMax > _pmin.value() && _pMax < _pmax.value() ){
    isEtapRhoGamIncl = true;
  }

  if ( _verbose.value( ) ) {cout << "isEtapRhoGamIncl : " << (int)isEtapRhoGamIncl << endl;}


  // Make decision based on the tag values & set the "passed" flag.
  passed = isEtapRhoGamIncl;

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
InclEtapRhoGamFilter::endJob( AbsEvent* anEvent ) {

  cout << name()  << " end job: " << _nPassed << " out of " 
       << _nReadEvents << " passed" << endl;
  
  return AppResult::OK;
}



