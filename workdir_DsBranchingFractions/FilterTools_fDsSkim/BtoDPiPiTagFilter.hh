//--------------------------------------------------------------------------
// Description:
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Fernando Martinez-Vidal          Original author
//
// Copyright Information:
//
//------------------------------------------------------------------------

#ifndef TAGBtoDPiPi_HH
#define TAGBtoDPiPi_HH

//----------------------
// Base Class Headers --
//----------------------
#include "BaBar/BaBar.hh"

//---------------
// C++ Headers --
//---------------
#include <iostream>
//#include "TStopwatch.h"

#include "Framework/AppFilterModule.hh"
#include "Framework/AbsParmBool.hh"
#include "AbsParm/AbsParmIfdStrKey.hh"
#include "Framework/AbsParmGeneral.hh"

#include "TagModules/TagFilterModule.hh"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "AbsEvent/AbsEvent.hh"
#include "AbsEvent/getTmpAList.hh"
#include "AbsEnv/AbsEnv.hh"
#include "GenEnv/GenEnv.hh"
#include "TagData/TagTransient.hh"
#include "ProxyDict/IfdStrKey.hh"
#include "ProxyDict/IfdKey.hh"
#include "ProxyDict/Ifd.hh"

#include "Beta/BtaCandidate.hh"

#include "HepTuple/TupleManager.h"
#include "HepTuple/Histogram.h"
#include "UsrData/UsrData.hh"

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
class HepTupleManager;
class HepHistogram;
class HepTuple;
class AbsEvent;
class EventInfo;
//		---------------------
// 		-- Class Interface --
//		---------------------

//		---------------------
// 		-- Class Interface --
//		---------------------

using namespace std;
 
class BtoDPiPiTagFilter : public TagFilterModule {

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  BtoDPiPiTagFilter(const char* const theName, 
		      const char* const theDescription);
  
  // Destructor
  virtual ~BtoDPiPiTagFilter();
  
  // Operations
  
  // Modifiers
  virtual AppResult beginJob( AbsEvent* anEvent );
  virtual AppResult event( AbsEvent* anEvent );
  virtual AppResult endJob( AbsEvent* anEvent );

  virtual void     help(int argc, char** argv);
  virtual ostream& print(ostream& os) const;

private:

//  TStopwatch *theWatch;

  AbsParmIfdStrKey _BchDPPList;
  AbsParmIfdStrKey _BchDsPPList;
  AbsParmIfdStrKey _B0DPPList;
  AbsParmIfdStrKey _B0DsPPList;
  UsrVariable<int>  _DPiPiMode;
  int _eventLog;

  // event counter
  int _nReadEvents; 
  int _nBchDPPPassed;
  int _nBchDsPPPassed;
  int _nB0DPPPassed;
  int _nB0DsPPPassed;
  int _nPassed;
//  int _D0PiPiB0;
//  int _D0PiPiAB0;
//  int _DS0PiPiB0;
//  int _DS0PiPiAB0;

  UsrEventBlock     _DPiPiEventBlock;

  HepTupleManager* _manager;
  //
  // histograms to control the skimming
  HepHistogram* _myFilterCounter;
};

#endif
