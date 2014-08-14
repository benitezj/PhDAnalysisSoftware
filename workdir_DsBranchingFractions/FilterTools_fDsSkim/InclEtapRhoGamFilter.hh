//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: InclEtapRhoGamFilter.hh,v 1.1 2003/09/15 03:05:05 pclark Exp $
//
// Description:
//	Class InclOmegaFilter. An example module that filters on the 
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
//	Copyright (C) 2000      Hedgehog concepts :-)
//
//------------------------------------------------------------------------

#ifndef INCLETAPRHOGAMFILTER_HH
#define INCLETAPRHOGAMFILTER_HH

//-------------
// C Headers --
//-------------
extern "C" {
}

//----------------------
// Base Class Headers --
//----------------------
#include "TagModules/TagFilterModule.hh"

#include "Framework/AbsParmGeneral.hh"

//		---------------------
// 		-- Class Interface --
//		---------------------

class InclEtapRhoGamFilter : public TagFilterModule {

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  InclEtapRhoGamFilter( const char* const theName, 
	      const char* const theDescription );

  // Destructor
  virtual ~InclEtapRhoGamFilter();

  // Modifiers
  virtual AppResult beginJob( AbsEvent* anEvent );
  virtual AppResult event( AbsEvent* anEvent );
  virtual AppResult endJob( AbsEvent* anEvent );

private:

  // event counters
  int _nReadEvents;
  int _nPassed;
  AbsParmGeneral<double> _pmax;
  AbsParmGeneral<double> _pmin;
  AbsParmGeneral<double> _cosTcut;

};

#endif 
