//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: HardElectronFilter.hh,v 1.2 2005/08/04 16:41:49 desilva Exp $
//
// Description:
//	Class HardElectronFilter - fast filter for the SL analysis
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

#ifndef HARDELECTRONFILTER_HH
#define HARDELECTRONFILTER_HH

//----------------------
// Base Class Headers --
//----------------------
#include "TagModules/TagFilterModule.hh"
 
//------------------------------------
// Collaborating Class Declarations --
//------------------------------------

template <class T> class AbsParmGeneral;

//		---------------------
// 		-- Class Interface --
//		---------------------
 
class HardElectronFilter : public TagFilterModule {

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  HardElectronFilter( const char* const theName, const char* const theDescription );
  
  // Destructor
  virtual ~HardElectronFilter();
  
  // Operations
  virtual AppResult           beginJob( AbsEvent* anEvent );
  virtual AppResult           event( AbsEvent* anEvent );
  virtual AppResult           endJob  ( AbsEvent* anEvent );
  
protected:
  AbsParmGeneral<int>    *_nTrkCut;
  AbsParmGeneral<float>  *_r2Cut;
  AbsParmGeneral<float>  *_minTagMom;
  unsigned   _nPassed;
  unsigned   _nPass1;
  unsigned   _nPass2;
  unsigned   _nPass3;
  unsigned   _nPass4;

};
#endif

