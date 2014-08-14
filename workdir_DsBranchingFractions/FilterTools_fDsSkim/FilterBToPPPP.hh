//--------------------------------------------------------------------------
// File and Version Information:
//
// Description:
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//
// Copyright Information:
//	Copyright (C) 2005      SLAC
//
//------------------------------------------------------------------------

#ifndef FILTERBTOPPPP_HH
#define FILTERBTOPPPP_HH

//-------------
// C++ Headers --
//-------------
#include <string>

//----------------------
// Base Class Headers --
//----------------------
#include "TagModules/TagFilterModule.hh"

#include "Framework/AbsParmGeneral.hh"
#include "Framework/AbsParmString.hh"
#include "AbsParm/AbsParmVector.hh"

//		---------------------
// 		-- Class Interface --
//		---------------------

class FilterBToPPPP : public TagFilterModule {

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  FilterBToPPPP( const char* const theName, 
		const char* const theDescription );

  // Destructor
  virtual ~FilterBToPPPP();

  // Modifiers
  virtual AppResult beginJob( AbsEvent* anEvent );
  virtual AppResult event ( AbsEvent* anEvent );
  virtual AppResult endJob( AbsEvent* anEvent );

private:

  // event counters
  int _nReadEvents;
  int _nPassed;

  AbsParmVector<std::string> _tagbit;
  AbsParmGeneral<int>        _minNoOfTracks;
  AbsParmGeneral<double>     _maxTotalEnergy;

};

#endif 
