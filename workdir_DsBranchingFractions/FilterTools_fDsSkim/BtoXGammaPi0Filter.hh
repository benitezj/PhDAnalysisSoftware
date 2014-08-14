//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: BtoXGammaPi0Filter.hh,v 1.3 2005/06/07 15:20:13 fnc Exp $
//
// Description:
//	Class TwoBody. An example module that filters on the 
//      transient tag.
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Paul Harrison and Theresa Champion - based on Original template
//	by Charles Young
//
// Copyright Information:
//	Copyright (C) 1998      SLAC
//
//------------------------------------------------------------------------

#ifndef BTOXGAMMAPI0FILTER_HH
#define BTOXGAMMAPI0FILTER_HH

//-------------
// C Headers --
//-------------
extern "C" {
}

//----------------------
// Base Class Headers --
//----------------------

#include "CLHEP/Alist/AList.h"
#include "CLHEP/Matrix/Vector.h"
#include "CLHEP/Vector/LorentzVector.h"

#include "Beta/BtaCandidate.hh"
#include "Framework/AppFilterModule.hh"
#include "Framework/AbsParmBool.hh"
#include "Framework/AbsParmDouble.hh"
#include "Framework/AbsParmGeneral.hh"
#include "Framework/AbsParmString.hh"
#include "AbsParm/AbsParmIfdStrKey.hh"

#include "TagModules/TagFilterModule.hh"




//------------------------------------
// Collaborating Class Declarations --
//------------------------------------

class HepHistogram;
class HepTuple;
class AbsEvent;
class BtaMcAssoc;
class EventInfo;
class BtaBooster;


//		---------------------
// 		-- Class Interface --
//		---------------------



class BtoXGammaPi0Filter : public TagFilterModule {

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  BtoXGammaPi0Filter( const char* const theName, 
	      const char* const theDescription );

  // Destructor
  virtual ~BtoXGammaPi0Filter();

  // Modifiers
  virtual AppResult beginJob( AbsEvent* anEvent );
  virtual AppResult event( AbsEvent* anEvent );
  virtual AppResult endJob( AbsEvent* anEvent );
  virtual AppModule *clone( const char *cloneName );

private:

  AbsParmIfdStrKey _eventInfoList;
  AbsParmIfdStrKey _outputPi0CandList;
  AbsParmIfdStrKey _outputEtaCandList;
  AbsParmIfdStrKey _photonList;

  // Commands list
  AbsParmDouble _maxR2;
  AbsParmDouble _minE1;
  AbsParmDouble _maxE1;
  AbsParmGeneral<int> _minNGtl;
  
  AbsParmDouble _heGammaELow;
  AbsParmDouble _pi0GamELow;
  AbsParmDouble _pi0MassLow;
  AbsParmDouble _pi0MassHigh;
  AbsParmDouble _pi0EnergyLow;
  AbsParmDouble _pi0EnergyHigh;
  AbsParmDouble _etaGamELow;
  AbsParmDouble _etaMassLow;
  AbsParmDouble _etaMassHigh;
  AbsParmDouble _etaEnergyLow;
  AbsParmDouble _etaEnergyHigh;

  // event counters
  int _nReadEvents;
  int _nEventsPassed;

};

#endif 
