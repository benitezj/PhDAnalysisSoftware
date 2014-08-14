//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id:
//
// Description:
//	Class IsrTagbitFilter. 
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// History (add to end):
//      Joe Izen  ~Sep, 2000 - creation
//      Joe Izen   Oct, 2001 - bug fix to IsrNoGamma, add IsrLooseGamma, IsrRadBhabha
//
// Author List:
//	Joe Izen    	                Original Author
//
// Copyright Information:
//	Copyright (C) 2001		University of Texas at Dallas
//
//------------------------------------------------------------------------

#ifndef ISRTAGBITFILTER_HH
#define ISRTAGBITFILTER_HH

//-------------
// C Headers --
//-------------
//extern "C" {
//}

//---------------
// C++ Headers --
//---------------

//----------------------
// Base Class Headers --
//----------------------
#include "AbsParm/AbsParmIfdStrKey.hh"
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

class IsrTagbitFilter : public TagFilterModule {

//--------------------
// Declarations     --
//--------------------

  // Typedefs, consts, and enums

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  IsrTagbitFilter( const char* const theName, 
			    const char* const theDescription );

  // Copy Constructor

  // Destructor
  virtual ~IsrTagbitFilter();

  // Operators

  // Selectors (const)

  // Modifiers
  virtual AppResult beginJob( AbsEvent* anEvent );
  virtual AppResult event   ( AbsEvent* anEvent );

  virtual AppResult endJob  ( AbsEvent* anEvent );
  virtual void help(int argc, char** argv);
  virtual AppModule* clone(const char* cloneName);

protected:

  // Helper functions

private:
  // Data members
  AbsParmBool            _enableGammaISR;      //pass ISR candidates with visible gamma
  AbsParmBool            _enableNoGammaISR;    //pass ISR candidates with no visible gamma
  AbsParmBool            _enableLooseGammaISR; //pass ISR candidates with visible gamma for Noncharm ISR analysis
  AbsParmBool            _enableRadBhabha;     //pass Radiative Bhabhas rejected by enableLooseGammaISR

  int                    _nPassedNG;
  int                    _nPassedG;
  int                    _nPassedLG;
  int                    _nPassedGLG;  //count G and LG 
  int                    _nPassedRBB;
  int                    _nPassedISR;  //count G and/or LG and/or NG 
  int                    _nPrescaledISR;  
  int                    _nPrescaledRBB;  
  int                    _nPassed;     //total passed after prescale taken into account
  int                    _evtNum;
  int                    _nthisrun;
  int                    _thisrun;
  int                    _lastrun;
}; 

#endif 












