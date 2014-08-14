//--------------------------------------------------------------------------
// File and Version Information:
//
// Description:
//	Class A0Filter. Module to filter on the 
//      transient tag.
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Philip J. Clark
//------------------------------------------------------------------------

#ifndef A0FILTER_HH
#define A0FILTER_HH

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

class A0Filter : public TagFilterModule {

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  A0Filter( const char* const theName, 
	      const char* const theDescription );

  // Destructor
  virtual ~A0Filter();

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
