//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: BtaTagHadronFilter.hh,v 1.2 2000/11/07 01:18:34 jamesw Exp $
//
// Description:
//	Class BtaTagHadronFilter. 
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	Yury Kolomensky    	Originator
//
// Copyright Information:
//	Copyright (C) 1999      Caltech
//
//------------------------------------------------------------------------

#ifndef BTATAGHADRONFILTER_HH
#define BTATAGHADRONFILTER_HH

//-------------
// C Headers --
//-------------
extern "C" {
}

//---------------
// C++ Headers --
//---------------

//----------------------
// Base Class Headers --
//----------------------
#include "TagModules/TagFilterModule.hh"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "Framework/AbsParmGeneral.hh"

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------

//		---------------------
// 		-- Class Interface --
//		---------------------

class BtaTagHadronFilter : public TagFilterModule {

//--------------------
// Declarations     --
//--------------------

  // Typedefs, consts, and enums

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  BtaTagHadronFilter( const char* const theName, 
			const char* const theDescription );

  // Copy Constructor

  // Destructor
  virtual ~BtaTagHadronFilter();

  // Operators

  // Selectors (const)

  // Modifiers
  virtual AppResult beginJob( AbsEvent* anEvent );
  virtual AppResult event( AbsEvent* anEvent );

protected:

  // Helper functions

private:
  // Data members

  AbsParmGeneral<double> _pTotalCosThCut;
  AbsParmGeneral<double> _eNeutralCosThCut;
  AbsParmGeneral<double> _eTotalCut;
  AbsParmGeneral<double> _pTotalMassCut;
  AbsParmGeneral<int>    _nTrkCut;

};

#endif 
