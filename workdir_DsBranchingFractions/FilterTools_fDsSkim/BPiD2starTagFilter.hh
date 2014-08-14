//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: BPiD2starTagFilter.hh,v 1.1 2001/10/15 15:46:12 zito Exp $
//
// Description:
//	Class BPiD2starTagFilter. An example module that filters on the 
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

#ifndef BPiD2starTagFilter_HH
#define BPiD2starTagFilter_HH

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

class BPiD2starTagFilter : public TagFilterModule {

//--------------------
// Declarations     --
//--------------------

// Typedefs, consts, and enums

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  BPiD2starTagFilter( const char* const theName, 
	      const char* const theDescription );

  // Copy Constructor

  // Destructor
  virtual ~BPiD2starTagFilter();

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

#endif // BPiD2starTagFilter_HH
