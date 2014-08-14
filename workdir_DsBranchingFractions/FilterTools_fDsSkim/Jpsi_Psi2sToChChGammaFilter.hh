//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: Jpsi_Psi2sToChChGammaFilter.hh,v 1.1 2007/05/25 23:38:59 echenard Exp $
//
// Description:
//	Class Jpsi_Psi2sToChChGammaFilter. An example module that filters on the 
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

#ifndef JPSI_PSI2STOCHCHGAMMAFILTER_HH
#define JPSI_PSI2STOCHCHGAMMAFILTER_HH

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

//		---------------------
// 		-- Class Interface --
//		---------------------

class Jpsi_Psi2sToChChGammaFilter : public TagFilterModule {

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  Jpsi_Psi2sToChChGammaFilter( const char* const theName, 
		   const char* const theDescription );

  // Destructor
  virtual ~Jpsi_Psi2sToChChGammaFilter();

  // Modifiers
  virtual AppResult beginJob( AbsEvent* anEvent );
  virtual AppResult event( AbsEvent* anEvent );
  virtual AppResult endJob( AbsEvent* anEvent );

private:

  AbsParmGeneral<double> _EOrig;
  AbsParmGeneral<double> _ESlope;
  AbsParmGeneral<double> _ThetaMin;

};

#endif 
