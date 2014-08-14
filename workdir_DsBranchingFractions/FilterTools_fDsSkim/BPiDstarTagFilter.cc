//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: BPiDstarTagFilter.cc,v 1.10 2004/08/09 07:00:44 bartoldu Exp $
//
// Description:
//	An example module that filters on the transient tag. 
// Filter on the value of R2 and ask p1Mag > 2 
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//             Marco Zito
//
//
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "FilterTools/BPiDstarTagFilter.hh"

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
#include "AbsEventTag/AbsEventTag.hh"

#include "AbsEnv/AbsEnv.hh"
#include "GenEnv/GenEnv.hh"
#include "HepTuple/TupleManager.h"
#include "HepTuple/Histogram.h"

#include "ErrLogger/ErrLog.hh"
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
BPiDstarTagFilter::BPiDstarTagFilter( const char* const theName, 
			const char* const theDescription )
  : TagFilterModule( theName, theDescription )
  , _cutR2("cutr2",this,0.6) // cut on R2 (Fox Wolfram
  , _cutP1Min("cutP1Min",this,1.8) // cut on p1Mag momentum 
{
  commands()->append(& _cutR2);
  commands()->append(& _cutP1Min);
}
		

//--------------
// Destructor --
//--------------
BPiDstarTagFilter::~BPiDstarTagFilter()
{
}

//-------------
// Methods   --
//-------------
    
//-------------
// Operators --
//-------------
    
//-------------
// Selectors --
//-------------
    
//-------------
// Modifiers --
//-------------

AppResult
BPiDstarTagFilter::beginJob( AbsEvent*  anEvent )
{
  ErrMsg(trace) << name() << ": begin job" << endmsg;

  // initialize event counters
  _nReadEvents = 0;
  _nPassed = 0;

  // this call sets up the histogram package
  HepTupleManager* manager = gblEnv->getGen()->ntupleManager();
  assert( 0 != manager );

  // define a bunch of histograms
  HepString s0(name());
  _HistR2 = manager->histogram( s0+"; R2 ", 50, 0., 1. );
  _HistP1 = manager->histogram( s0+"; p1Mag ", 50, 0., 5. );


  return AppResult::OK;
}


AppResult 
BPiDstarTagFilter::event( AbsEvent* anEvent )
{
  _nReadEvents++;
  
  float aFloat;
  bool  passed = false;

  // Build the tag accessor using the base class
  TagFilterModule::event( anEvent );

  // get the tags we need : use R2All now 
  float R2 = -1. ; 
  if ( tag( )->getFloat( aFloat, "R2All" ) ) { R2 = aFloat; }
  _HistR2->accumulate(R2) ; 

  float p1Mag = -1. ; 
  if ( tag( )->getFloat( aFloat, "p1Mag" ) ) { p1Mag = aFloat; }
  _HistP1->accumulate(p1Mag) ; 
    if (R2 <_cutR2.value() && p1Mag > _cutP1Min.value() ) passed = true ; 


    if ( _verbose.value( ) ) {
    ErrMsg(trace) << "R2 : " << R2 << endl 
		  << "p1Mag: " << p1Mag << endmsg;
  }
  

  // done
  if (passed) {
    _nPassed++;
    ErrMsg(trace) << name() << ": event # " 
		  << _nReadEvents << " passed." << endmsg;
  } else {
    ErrMsg(trace) << name() << ": event # " 
		  << _nReadEvents << " rejected." << endmsg;
  }
  setPassed( passed );
  return AppResult::OK;

}

AppResult
BPiDstarTagFilter::endJob( AbsEvent* anEvent ) {

  ErrMsg(routine) << name()  << " end job: " << _nPassed << " out of " 
		  << _nReadEvents << " passed" << endmsg;
  
  return AppResult::OK;
}


//		-----------------------------------------------
// 		-- Static Data & Function Member Definitions --
//		-----------------------------------------------

//		-------------------------------------------
// 		-- Protected Function Member Definitions --
//		-------------------------------------------

//		-----------------------------------------
// 		-- Private Function Member Definitions --
//		-----------------------------------------

//		-----------------------------------
// 		-- Internal Function Definitions --
//		-----------------------------------
