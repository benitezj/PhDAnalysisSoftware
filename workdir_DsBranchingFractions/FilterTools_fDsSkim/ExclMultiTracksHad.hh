//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: ExclMultiTracksHad.hh,v 1.1 2005/03/17 10:07:53 fnc Exp $
//
// Description:
//     Preselection of multitrack events
//     Reject events with one or more tight leptons
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//     M. Negrini               Originator
//
// Copyright Information:
//      Copyright (C) 2005      INFN Ferrara
//
//------------------------------------------------------------------------

#ifndef EXCLMULTITRACKSHAD_HH
#define EXCLMULTITRACKSHAD_HH

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

class ExclMultiTracksHad : public TagFilterModule {

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  ExclMultiTracksHad( const char* const theName, 
	      const char* const theDescription );

  // Destructor
  virtual ~ExclMultiTracksHad();

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
