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

#ifndef TAGBFOURBODYFILTER_HH
#define TAGBFOURBODYFILTER_HH

//-------------
// C Headers --
//-------------
extern "C" {
}

//----------------------
// Base Class Headers --
//----------------------
#include "TagModules/TagFilterModule.hh"

//		---------------------
// 		-- Class Interface --
//		---------------------

class TagBFourBodyFilter : public TagFilterModule {

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  TagBFourBodyFilter( const char* const theName, 
	      const char* const theDescription );

  // Destructor
  virtual ~TagBFourBodyFilter();

  // Modifiers
  virtual AppResult beginJob( AbsEvent* anEvent );
  virtual AppResult event( AbsEvent* anEvent );
  virtual AppResult endJob( AbsEvent* anEvent );

private:

  // event counters
  int _nReadEvents;
  int _nPassed;

};

#endif 
