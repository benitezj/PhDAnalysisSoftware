//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: BtoRhoGammaBtoXGF.hh,v 1.3 2007/01/03 19:38:55 krosebe Exp $
//
// Description:
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

#ifndef BTORHOGAMMABTOXGF_HH
#define BTORHOGAMMABTOXGF_HH

//-------------
// C Headers --
//-------------
extern "C" {
}

//----------------------
// Base Class Headers --
//----------------------
#include "TagModules/TagFilterModule.hh"

#include "Framework/AbsParmDouble.hh"
#include "Framework/AbsParmGeneral.hh"

//		---------------------
// 		-- Class Interface --
//		---------------------

class BtoRhoGammaBtoXGF : public TagFilterModule {

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  BtoRhoGammaBtoXGF( const char* const theName, 
	      const char* const theDescription );

  // Destructor
  virtual ~BtoRhoGammaBtoXGF();

  // Modifiers
  virtual AppResult beginJob( AbsEvent* anEvent );
  virtual AppResult event( AbsEvent* anEvent );
  virtual AppResult endJob( AbsEvent* anEvent );
  virtual AppModule *clone( const char *cloneName );

private:
  // Commands list
  AbsParmBool _activeR2All;
  AbsParmBool _activeE1;
  AbsParmBool _activeNTrk;
  AbsParmDouble _maxR2All;
  AbsParmDouble _minE1;
  AbsParmDouble _maxE1;
  AbsParmGeneral<int> _minNTrk;

  // event counters
  long _nReadEvents;
  long _nPassed;

};

#endif 
