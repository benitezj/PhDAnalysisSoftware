//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: TwoPhotonPentaquarkTagFilter.hh,v 1.1 2004/07/27 14:31:27 phnic Exp $
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//     Nicole Chevalier
//
//
// Copyright Information:
//	Copyright (C) 1998      SLAC
//
//------------------------------------------------------------------------

#ifndef TWOPHOTONPENTAQUARKTAGFILTER_HH
#define TWOPHOTONPENTAQUARKTAGFILTER_HH

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

class TwoPhotonPentaquarkTagFilter : public TagFilterModule {

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  TwoPhotonPentaquarkTagFilter( const char* const theName, 
		   const char* const theDescription );

  // Destructor
  virtual ~TwoPhotonPentaquarkTagFilter();

  // Modifiers
  virtual AppResult beginJob( AbsEvent* anEvent );
  virtual AppResult event( AbsEvent* anEvent );
  virtual AppResult endJob( AbsEvent* anEvent );

private:

  // event counters
  int _nReadEvents;
  int _nPassed;

  AbsParmGeneral<double> _minTotalEnergy;
  AbsParmGeneral<double> _maxTotalEnergy;
};
#endif 
