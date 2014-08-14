//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: DcToKpipiTagFilter.cc,v 1.3 2004/05/18 14:42:20 fnc Exp $
//
// Description:
//	An example module that filters on the transient tag. 
//      Filter on the value of R2 and nGoodTrkLoose
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
#include "FilterTools/DcToKpipiTagFilter.hh"

//-------------
// C Headers --
//-------------
extern "C" {
#include <assert.h>
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

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

//----------------
// Constructors --
//----------------
DcToKpipiTagFilter::DcToKpipiTagFilter( const char* const theName, 
					const char* const theDescription )
  : TagFilterModule( theName, theDescription )
  , _r2Cut("r2Cut",this,0.9) 
  , _nTrkCut("nTrkCut",this,2)
  , _qTotCut("qTotCut",this,4)
  , _doHist("doHist", this, true)
{
  commands()->append(& _r2Cut);
  commands()->append(& _nTrkCut);
  commands()->append(& _qTotCut);
  commands()->append(& _doHist);
}
		

//--------------
// Destructor --
//--------------
DcToKpipiTagFilter::~DcToKpipiTagFilter()
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
DcToKpipiTagFilter::beginJob( AbsEvent*  anEvent )
{
  ErrMsg(trace) << name() << ": begin job" << endmsg;

  // initialize event counters
  _nReadEvents = 0;
  _nPassed = 0;
  

  if ( _doHist.value() ) {
    // this call sets up the histogram package
    HepTupleManager* manager = gblEnv->getGen()->ntupleManager();
    assert( 0 != manager );
    
    // define a bunch of histograms
    _foxWolfR2Histo         = manager->histogram("R2", 40, 0.,  1. ) ;
    _thrustMagHisto         = manager->histogram("thrust", 40, 0.,  1. ) ;
    _nGoodTrkLoose          = manager->histogram("n Good trk loose", 
						 30, 0.,  30. ) ;
    _totChargeLoose         = manager->histogram("tot charge loose", 
						 20, -10., 10. ) ;
  }

  return AppResult::OK;
}

AppResult 
DcToKpipiTagFilter::event( AbsEvent* anEvent )
{
  _nReadEvents++;
  
  bool  passed = false;

  // Build the tag accessor using the base class
  TagFilterModule::event( anEvent );

  float foxWolfR2(1.) ; 
  float thrustMag(1.) ;
  int nGoodTrkLoose(0) ;
  int totChargeLoose(0) ;
  
  bool tag_success = false ; 

  tag_success  = tag()->getFloat( foxWolfR2,"R2") ;
  tag_success &= tag()->getFloat(thrustMag , "thrustMag" );
  tag_success &= tag()->getInt(nGoodTrkLoose , "nGoodTrkLoose" );
  tag_success &= tag()->getInt(totChargeLoose , "totChargeLoose" );

  //make some histograms on general shape variables

  _foxWolfR2Histo->accumulate(foxWolfR2) ;
  _thrustMagHisto->accumulate(thrustMag) ;
  _nGoodTrkLoose->accumulate(nGoodTrkLoose) ;
  _totChargeLoose->accumulate(totChargeLoose) ;

  if ( tag_success && 
       foxWolfR2 < _r2Cut.value() &&
       nGoodTrkLoose > _nTrkCut.value() &&
       abs(totChargeLoose) < _qTotCut.value() ) {
    passed = true;
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
DcToKpipiTagFilter::endJob( AbsEvent* anEvent ) {

  ErrMsg(routine) << " end job: " << _nPassed << " out of " 
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
