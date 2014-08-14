//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: DslnuccTagFilter.hh,v 1.4 2005/04/29 16:59:10 desilva Exp $
//
// Description:
//	Class ISLTagFilter - fast filter for the ISL analysis
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Daniel Azzopardi                Original author
//      Joerg Stelzer                   adapted for Ds->l nu
//
// Copyright Information:
//	Copyright (C) 2000 Queen Mary & Westfield College, U. London
//
//------------------------------------------------------------------------

#ifndef DSLNUCCTAGFILTER_HH
#define DSLNUCCTAGFILTER_HH

//----------------------
// Base Class Headers --
//----------------------
#include "TagModules/TagFilterModule.hh"
#include "Framework/AbsParmGeneral.hh"
 
//------------------------------------
// Collaborating Class Declarations --
//------------------------------------

class HepTuple;
template <class T> class AbsParmGeneral;

//		---------------------
// 		-- Class Interface --
//		---------------------
 
class DslnuccTagFilter : public TagFilterModule {

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  DslnuccTagFilter( const char* const theName, const char* const theDescription );
  
  // Destructor
  virtual ~DslnuccTagFilter(){};
  
  // Operations
  virtual AppResult           beginJob( AbsEvent* anEvent );
  virtual AppResult           event   ( AbsEvent* anEvent );
  virtual AppResult           endJob  ( AbsEvent* anEvent );
  
protected:
  AbsParmGeneral<int>      _nTrk1;
  AbsParmGeneral<int>      _nTrk2;
  AbsParmGeneral<int>      _nNeut;
  AbsParmGeneral<double>   _minLeptMomCm;
  AbsParmGeneral<double>   _upperBound;
  AbsParmGeneral<double>   _lowerBound;
  AbsParmGeneral<double>   _slope;

  AbsParmBool              _writeNtuple;

  unsigned   _nTried;
  unsigned   _nPassed;

  HepTuple*        _ntuple;
};
#endif






