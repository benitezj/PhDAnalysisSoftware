//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: BPiDstarTagFilter.hh,v 1.3 2000/11/07 01:18:34 jamesw Exp $
//
// Description:
//	Class BPiDstarTagFilter. An example module that filters on the 
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

#ifndef BPiDstarTagFilter_HH
#define BPiDstarTagFilter_HH

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

//-------------------------------
// Collaborating Class Headers --
//-------------------------------

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
class HepTupleManager;
class HepHistogram;

//		---------------------
// 		-- Class Interface --
//		---------------------

class BPiDstarTagFilter : public TagFilterModule {

//--------------------
// Declarations     --
//--------------------

// Typedefs, consts, and enums

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  BPiDstarTagFilter( const char* const theName, 
	      const char* const theDescription );

  // Copy Constructor

  // Destructor
  virtual ~BPiDstarTagFilter();

  // Operators


  // Selectors (const)

  // Modifiers
  virtual AppResult beginJob( AbsEvent* anEvent );
  virtual AppResult event( AbsEvent* anEvent );
  virtual AppResult endJob( AbsEvent* anEvent );

protected:

  // Helper functions

private:

  // Friends

  // Data members

  // event counters
  int _nReadEvents;
  int _nPassed;
   
  // histograms and ntuple for this example

  HepHistogram*    _HistR2;
  HepHistogram*    _HistP1;


//------------------
// Static Members --
//------------------

public:

  // Selectors (const)

  // Modifiers

private:

  // Data members
  AbsParmGeneral<double> _cutR2;
  AbsParmGeneral<double> _cutP1Min;

};

#endif // BPiDstarTagFilter_HH
