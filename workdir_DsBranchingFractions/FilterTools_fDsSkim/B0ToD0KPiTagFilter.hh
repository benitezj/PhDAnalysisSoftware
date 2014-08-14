//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: B0ToD0KPiTagFilter.hh,v 1.2 2005/04/29 16:59:09 desilva Exp $
//
// Description:
//	Class B0ToD0KPiTagFilter
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Chih-hsiang Cheng
//
// Copyright Information:
//	Copyright (C) LLNL
//
//------------------------------------------------------------------------
#ifndef B0TOD0KPITAGFILTER_HH
#define B0TOD0KPITAGFILTER_HH

//----------------------
// Base Class Headers --
//----------------------
#include "TagModules/TagFilterModule.hh"
#include "Framework/AbsParmGeneral.hh"
#include "Framework/AbsParmDouble.hh"
#include "Framework/AbsParmString.hh"
 
//------------------------------------
// Collaborating Class Declarations --
//------------------------------------

class B0ToD0KPiTagFilter : public TagFilterModule {


public:

  B0ToD0KPiTagFilter( const char* const theName, const char* const theDescription );

  virtual ~B0ToD0KPiTagFilter();

  // Operations
  virtual AppResult           beginJob( AbsEvent* anEvent );
  virtual AppResult           event   ( AbsEvent* anEvent );
  virtual AppResult           endJob  ( AbsEvent* anEvent );
  
protected:
  AbsParmGeneral<int>      _nTrkMinimum;
  AbsParmString            _nTracksName;
  AbsParmDouble            _r2AllMax;

};

#endif
