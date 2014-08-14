//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: BlnuTagFilter.hh,v 1.2 2000/11/07 01:18:34 jamesw Exp $
//
// Description:
//	Class BlnuTagFilter. 
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	Steven Robertson    	Adapted from BtaTagHadronFilter module
//
//------------------------------------------------------------------------

#ifndef BLNUTAGFILTER_HH
#define BLNUTAGFILTER_HH

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
class HepTupleManager;
class HepTuple;

//		---------------------
// 		-- Class Interface --
//		---------------------

class BlnuTagFilter : public TagFilterModule {

//--------------------
// Declarations     --
//--------------------

  // Typedefs, consts, and enums

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  BlnuTagFilter( const char* const theName, 
			const char* const theDescription );

  // Copy Constructor

  // Destructor
  virtual ~BlnuTagFilter();

  // Operators

  // Selectors (const)

  // Modifiers
  virtual AppResult beginJob( AbsEvent* anEvent );
  virtual AppResult event( AbsEvent* anEvent );

protected:

  // Helper functions

private:
  // Data members
  AbsParmBool _doNtuple;
  AbsParmGeneral<int> _nTracksCut;
  AbsParmGeneral<double> _r2Cut;
  AbsParmGeneral<double> _pLabLeptonCutLow;
  AbsParmGeneral<double> _pCmLeptonCutLow;
  AbsParmGeneral<double> _pCmLeptonCutHigh;
  AbsParmGeneral<double> _eTotalCut;

  HepTuple*        _BlnuNtuple;

};

#endif 

