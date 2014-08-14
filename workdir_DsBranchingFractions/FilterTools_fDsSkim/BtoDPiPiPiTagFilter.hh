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

#ifndef TAGBTODPiPiPi_HH
#define TAGBTODPiPiPi_HH

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

using namespace std;
 
class BtoDPiPiPiTagFilter : public TagFilterModule {

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  BtoDPiPiPiTagFilter(const char* const theName, 
		      const char* const theDescription);
  
  // Destructor
  virtual ~BtoDPiPiPiTagFilter();
  
  // Operations
  
  // Modifiers
  virtual AppResult beginJob( AbsEvent* anEvent );
  virtual AppResult event( AbsEvent* anEvent );
  virtual AppResult endJob( AbsEvent* anEvent );

  virtual void     help(int argc, char** argv);
  virtual ostream& print(ostream& os) const;

private:

//  TStopwatch *theWatch;

  AbsParmIfdStrKey _BchD3PiList;
  AbsParmIfdStrKey _BchDs3PiList;
  AbsParmIfdStrKey _B0D3PiList;
  AbsParmIfdStrKey _B0Ds3PiList;
  UsrVariable<int>  _DPiPiPiMode;
  int _eventLog;

  // event counter
  int _nReadEvents; 
  int _nBchD3PiPassed;
  int _nBchDs3PiPassed;
  int _nB0D3PiPassed;
  int _nB0Ds3PiPassed;
  int _nPassed;

  UsrEventBlock     _DPiPiPiEventBlock;

  HepTupleManager* _manager;
  // histograms to control the skimming
  HepHistogram* _myFilterCounter;
};

#endif
