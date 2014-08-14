//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AntiDeuteronFilter.hh,v 1.4 2007/12/15 15:00:21 avtelnov Exp $
//
// Description:
//	Class AntiDeuteronFilter
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Valerie Halyo    (SLAC)
//
// Copyright Information:
//	Copyright (C) 1997		
//
// History:
//      Alexandre Telnov (Princeton University), 2007/12/14:
//         removed explicit calls to DchBetheBloch::ionization();
//         rewrote all SVT and DCH dE/dx-related functions to use 
//         the new BetaPid/PidDEDXCdbXxx methods.
//               
//------------------------------------------------------------------------

#ifndef AntiDeuteronFilter_hh
#define AntiDeuteronFilter_hh


//----------------------
// Base Class Headers --
//----------------------

#include "CLHEP/Alist/AList.h"
#include "AbsParm/AbsParmIfdStrKey.hh"
#include "Framework/AppFilterModule.hh"
#include "Framework/AbsParmBool.hh"
#include "Framework/AbsParmGeneral.hh"
#include "Framework/AbsParmDouble.hh"
#include "CLHEP/Random/Random.h"
#include "CLHEP/Matrix/Matrix.h"
#include "Beta/BtaCandidate.hh"
#include "TrajGeom/TrkLineTraj.hh"

#include "TagModules/TagFilterModule.hh"

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------

class BtaCandidate;
class HepString;
class AbsEvent;
class EventInfo;
class TrkFit;
class TrkPoca;
class TrkRecoTrk;
class TrkDifTraj;

//		---------------------
// 		-- Class Interface --
//		---------------------
 
class AntiDeuteronFilter : public TagFilterModule {

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  AntiDeuteronFilter( const char* const theName, const char* const theDescription );

  // Destructor
  virtual ~AntiDeuteronFilter( );

  // Operations
  bool mapCoord(HepMatrix &transMat, int *coordMap);

void  CollisionAxisSlope(const BbrPointErr & beamSpot );
void  ClosestApproach( TrkLineTraj BeamTrajectory,
		   BtaCandidate* thecand);

  // accessors 
  double docaxy() const ;
  double docaz() const ;
  HepPoint poca() const;
  double XZTilt( ) const;
  double YZTilt( ) const;
  double XYTilt( ) const;

   // Compute dE/dx (measured-expected)/measured
  double consistency(const BtaCandidate *c) const;
  double consistencysvt(const BtaCandidate *c) const; 

    // Operations
  virtual AppResult           beginJob( AbsEvent* anEvent );
  virtual AppResult           event   ( AbsEvent* anEvent );
  virtual AppResult           endJob  ( AbsEvent* anEvent );

protected:
   

private:
  // data
  AbsParmIfdStrKey       _eventInfoList;
  AbsParmIfdStrKey       _btaChargedList;

  bool _ok;
  int _coordMap[3];
  HepMatrix _transMat;
  double   _Docaxy;
  double   _Docaz;
  HepPoint _poca;
  int _nEventsRead;
  int _nEventsPassed;
};

#endif

