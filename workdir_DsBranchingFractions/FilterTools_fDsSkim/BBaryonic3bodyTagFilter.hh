//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: BBaryonic3bodyTagFilter.hh,v 1.1 2003/08/08 20:25:49 thrynova Exp $
//
// Description:
//	Class BBaryonic3bodyTagFilter. An example module that filters on the 
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

#ifndef BBaryonic3BODYTAGFILTER_HH
#define BBaryonic3BODYTAGFILTER_HH

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

class BBaryonic3bodyTagFilter : public TagFilterModule {

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  BBaryonic3bodyTagFilter( const char* const theName, 
		   const char* const theDescription );

  // Destructor
  virtual ~BBaryonic3bodyTagFilter();

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

};

#endif 
