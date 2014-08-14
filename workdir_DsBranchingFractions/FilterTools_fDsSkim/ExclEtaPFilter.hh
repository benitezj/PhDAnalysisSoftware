//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: ExclEtaPFilter.hh,v 1.3 2000/11/07 01:18:36 jamesw Exp $
//
// Description:
//	Class ExclEtaPFilter. An example module that filters on the 
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

#ifndef EXCLETAPFILTER_HH
#define EXCLETAPFILTER_HH

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

class ExclEtaPFilter : public TagFilterModule {

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  ExclEtaPFilter( const char* const theName, 
	      const char* const theDescription );

  // Destructor
  virtual ~ExclEtaPFilter();

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
