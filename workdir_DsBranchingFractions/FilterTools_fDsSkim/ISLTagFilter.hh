//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: ISLTagFilter.hh,v 1.3 2005/04/29 16:59:10 desilva Exp $
//
// Description:
//	Class ISLTagFilter - fast filter for the ISL analysis
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Daniel Azzopardi                Original author
//
// Copyright Information:
//	Copyright (C) 2000 Queen Mary & Westfield College, U. London
//
//------------------------------------------------------------------------

#ifndef ISLTAGFILTER_HH
#define ISLTAGFILTER_HH

//----------------------
// Base Class Headers --
//----------------------
#include "TagModules/TagFilterModule.hh"
 
//------------------------------------
// Collaborating Class Declarations --
//------------------------------------

class HepTuple;
template <class T> class AbsParmGeneral;

//		---------------------
// 		-- Class Interface --
//		---------------------
 
class ISLTagFilter : public TagFilterModule {

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  ISLTagFilter( const char* const theName, const char* const theDescription );
  
  // Destructor
  virtual ~ISLTagFilter();
  
  // Operations
  virtual AppResult           beginJob( AbsEvent* anEvent );
  virtual AppResult           event( AbsEvent* anEvent );
  virtual AppResult           endJob  ( AbsEvent* anEvent );
  
protected:
  AbsParmGeneral<bool>   *_doNtuple; // For test running *only*
  AbsParmGeneral<int>    *_nTrkCut;
  AbsParmGeneral<float>  *_r2Cut;
  AbsParmGeneral<float>  *_minTagMom;
  HepTuple * _ntuple;
  unsigned   _nPassed;
};
#endif

