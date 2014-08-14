//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: DcToKpipiTagFilter.hh,v 1.2 2000/11/07 01:18:35 jamesw Exp $
//
// Description:
//	Class DcToKpipiTagFilter. An example module that filters on the 
//      transient tag.
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	Charles Young    	Originator
//
// Copyright Information:
//	Copyright (C) 1998      SLAC
//
//------------------------------------------------------------------------

#ifndef BPIDSTARTAGFILTER_HH
#define BPIDSTARTAGFILTER_HH

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
#include "Framework/AbsParmGeneral.hh"
#include "Framework/AbsParmDouble.hh"
#include "Framework/AbsParmBool.hh"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
class HepHistogram;

//		---------------------
// 		-- Class Interface --
//		---------------------

class DcToKpipiTagFilter : public TagFilterModule {

//--------------------
// Declarations     --
//--------------------

// Typedefs, consts, and enums

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  DcToKpipiTagFilter( const char* const theName, 
		      const char* const theDescription );

  // Destructor
  virtual ~DcToKpipiTagFilter();

  // Operators


  // Selectors (const)

  // Modifiers
  virtual AppResult beginJob( AbsEvent* anEvent );
  virtual AppResult event( AbsEvent* anEvent );
  virtual AppResult endJob( AbsEvent* anEvent );

protected:

  // Helper functions

private:
  // Data members

  // event counters
  int _nReadEvents;
  int _nPassed;

  AbsParmDouble       _r2Cut;
  AbsParmGeneral<int> _nTrkCut;
  AbsParmGeneral<int> _qTotCut;
  

  // histograms and ntuple for this example
  AbsParmBool      _doHist;

  HepHistogram* _foxWolfR2Histo;
  HepHistogram* _thrustMagHisto;
  HepHistogram* _nGoodTrkLoose;
  HepHistogram* _totChargeLoose;

};

#endif 
