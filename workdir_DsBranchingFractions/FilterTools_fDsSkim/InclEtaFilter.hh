//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: InclEtaFilter.hh,v 1.3 2000/11/07 01:18:36 jamesw Exp $
//
// Description:
//	Class InclEtaFilter. An example module that filters on the 
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

#ifndef INCLETAFILTER_HH
#define INCLETAFILTER_HH

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

class InclEtaFilter : public TagFilterModule {

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  InclEtaFilter( const char* const theName, 
	      const char* const theDescription );

  // Destructor
  virtual ~InclEtaFilter();

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
