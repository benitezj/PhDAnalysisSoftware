//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: BtaCollisionAxisSlope.cc
//
// Description:
//      Class BtaCollisionAxisSlope - see .hh 
//      (ONLY USE ONCE PER EVENT CPU INTENSIVE)
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Tobias Hass
//      Jonathon Coleman
//
// Copyright Information:
//    
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "BtaV0Tools/BtaCollisionAxisSlope.hh"
#include "BtaV0Tools/BtaMapCoord.hh"
//-------------
// C Headers --
//-------------


//---------------
// C++ Headers --
//---------------
#include <iostream>
#include <math.h>

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
#include "Beta/BtaCandidate.hh"
#include "TrajGeom/TrkLineTraj.hh"
#include "ErrLogger/ErrLog.hh"

//----------------
// Constructors --
//----------------

bool BtaCollisionAxisSlope::_ok=false;
int BtaCollisionAxisSlope::_coordMap[3];
HepMatrix BtaCollisionAxisSlope::_transMat(3,3,1);
HepSymMatrix BtaCollisionAxisSlope::_cachedSpotErrMat(3,0);

HepMatrix
BtaCollisionAxisSlope::compute(const BbrPointErr& beamSpot)
{ 
  HepSymMatrix spotErrMat(beamSpot.covMatrix());
  if (spotErrMat(1,1) != _cachedSpotErrMat(1,1) ||
      spotErrMat(1,2) != _cachedSpotErrMat(1,2) ||
      spotErrMat(1,3) != _cachedSpotErrMat(1,3) ||
      spotErrMat(2,2) != _cachedSpotErrMat(2,2) ||
      spotErrMat(2,3) != _cachedSpotErrMat(2,3) ||
      spotErrMat(3,3) != _cachedSpotErrMat(3,3)) {
    _cachedSpotErrMat = spotErrMat;
    _transMat = diagonalize(&spotErrMat);
    
    // This calculates the permuation of the diagonalized matrix:
    _ok=mapCoord(_transMat, _coordMap);
    if (!_ok)
     ErrMsg(error) << "Beam spot calculation has failed" << endmsg;
  }
  return _transMat;
}	 

double 
BtaCollisionAxisSlope::XZTilt() {
  if (_ok)
    return _transMat[0][_coordMap[2]]/_transMat[2][_coordMap[2]];
  
  else
    return 0;
}

double 
BtaCollisionAxisSlope::YZTilt() {
  if (_ok)  
    return _transMat[1][_coordMap[2]]/_transMat[2][_coordMap[2]];
  else
    return 0;
}

double 
BtaCollisionAxisSlope::XYTilt() {
  if (_ok)   
    return _transMat[1][_coordMap[0]]/_transMat[0][_coordMap[0]];
  else
    return 0;
}
