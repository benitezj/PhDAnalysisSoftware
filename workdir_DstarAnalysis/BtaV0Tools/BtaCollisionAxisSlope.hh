//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: BtaCollisionAxisSlope.hh
//
// Description:
//	Class BtaCollisionAxisSlope - computes the Slope on the collision axis
//      by Matrix diagnolisation (ONLY USE ONCE PER EVENT CPU INTENSIVE)
//      
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Tobias Hass                 Original author
//      Jonathon Coleman            Converted to Methods for general Useage
//
// Copyright Information:
//    
//------------------------------------------------------------------------
#ifndef BtaCollisionAxisSlope_HH
#define BtaCollisionAxisSlope_HH

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
#include "Beta/BtaCandidate.hh"

class BtaCandidate;

//              ---------------------
//              -- Class Interface --
//              ---------------------
 
class BtaCollisionAxisSlope {

  //--------------------
  // Instance Members --
  //--------------------

public:
  BtaCollisionAxisSlope(const BbrPointErr& beamSpot){
    compute(beamSpot);
  }

  ~BtaCollisionAxisSlope(){;}
  static HepMatrix compute(const BbrPointErr & beamSpot);
  
  // accessors 
  static double XZTilt( );
  static double YZTilt( );
  static double XYTilt( );

private:
  static bool _ok;
  static int _coordMap[3];
  static HepMatrix _transMat;
  static HepSymMatrix _cachedSpotErrMat;
};

#endif
