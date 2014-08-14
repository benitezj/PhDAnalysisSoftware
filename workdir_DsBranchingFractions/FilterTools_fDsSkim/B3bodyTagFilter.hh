//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: B3bodyTagFilter.hh,v 1.4 2000/11/07 01:18:33 jamesw Exp $
//
// Description:
//	Class B3bodyTagFilter. An example module that filters on the 
//      transient tag.
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

#ifndef B3BODYTAGFILTER_HH
#define B3BODYTAGFILTER_HH

//-------------
// C Headers --
//-------------
extern "C" {
}

//----------------------
// Base Class Headers --
//----------------------
#include "TagModules/TagFilterModule.hh"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "AbsParm/AbsParmIfdStrKey.hh"
#include "Framework/AbsParmGeneral.hh"
#include "Framework/AbsParmString.hh"

//		---------------------
// 		-- Class Interface --
//		---------------------

class B3bodyTagFilter : public TagFilterModule {

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  B3bodyTagFilter( const char* const theName, 
		   const char* const theDescription );

  // Destructor
  virtual ~B3bodyTagFilter();

  // Modifiers
  virtual AppResult beginJob( AbsEvent* anEvent );
  virtual AppResult event( AbsEvent* anEvent );
  virtual AppResult endJob( AbsEvent* anEvent );

private:

  // event counters
  int _nReadEvents;
  int _nPassed;

  AbsParmString _3bodytagbit;

  AbsParmGeneral<int> _minNoOfTracks;
  AbsParmGeneral<double> _maxTotalEnergy;

};

#endif 
