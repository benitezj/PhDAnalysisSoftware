//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: BtaBS.cc,v 1.10 2004/08/06 05:53:00 bartoldu Exp $
//
// Description:
//      Class BtaBS - see .hh 
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Bill Lockman
//      Jonathon Coleman
//
// Copyright Information:
// 
//------------------------------------------------------------------------

#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "BtaV0Tools/BtaBS.hh"

//-------------
// C Headers --
//-------------
#include <iostream>
#include <math.h>

//---------------
// C++ Headers --
//---------------

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
#include "BtaV0Tools/BtaClosestApproach.hh"
#include "BtaV0Tools/BtaPointing.hh"
#include "BtaV0Tools/BtaSeperationDist.hh"
#include "BtaV0Tools/BtaSignedFlightLength3d.hh"
#include "BtaV0Tools/BtaCollAxisSlope.hh"
#include "ErrLogger/ErrLog.hh"

//----------------
// Constructors --
//----------------

BtaBS::BtaBS( const BbrPointErr& beamSpot , int alg, bool debug)
  : _beamSpot(beamSpot)
  , _Sep3d(0)
  , _XZTilt(0)
  , _YZTilt(0)
  , _XYTilt(0)
  , _Radius(0)
  , _Zcoll(0)
  , _Doca(0)
  , _SfL3d(0)
{
  if (alg == 0) {
    if (debug) ErrMsg(routine) 
      << "Compute beam axis tilts by diagonalizing the beamSpot matrix" 
      << endmsg;
    BtaCollisionAxisSlope Bcas(beamSpot);
    _XZTilt = Bcas.XZTilt();
    _YZTilt = Bcas.YZTilt();
    _XYTilt = Bcas.XYTilt();

  } else {
    if (debug) ErrMsg(routine)
      << "Compute beam axis tilts using WMD's iterative method" << endmsg;
    BtaCollAxisSlope Bcas1;
    Bcas1.setDebug(debug);
    Bcas1.compute( beamSpot );
    _XZTilt = Bcas1.XZTilt();
    _YZTilt = Bcas1.YZTilt();
    _XYTilt = Bcas1.XYTilt();
  }
  
  if (debug) ErrMsg(routine) << "BtaCollisionAxisSlope XZ, YZ, XYtilt: " 
			     << _XZTilt << " " 
			     << _YZTilt << " " 
			     << _XYTilt <<  endmsg;
}

void
BtaBS::Compute( const BtaCandidate* c , const BtaCandidate* d1, 
		const BtaCandidate* d2) {
  //
  // compute the separation distance
  //
  BtaSeperationDist _separation(d1, d2);
  _Sep3d = _separation.SeperationDistance3d();

  //
  // compute the beam spot point and vector
  //
  const HepPoint bs = _beamSpot;
  const Hep3Vector bsv(bs.x(), bs.y(), bs.z());
  const HepPoint dv = c->decayVtx()->point();
  const Hep3Vector dp(dv.x(), dv.y(), dv.z());
  BtaClosestApproach poca( bsv, dp, _XZTilt, _YZTilt,
			   c->p4());

  Hep3Vector pocaB = poca.pocaB();
  _Doca = poca.doca();

  //
  // get the vertex
  //
  HepPoint vtx = c->decayVtx()->point();
  Hep3Vector vtxVect(vtx.x(), vtx.y(), vtx.z());
  
  //
  // compute and set the radius
  //
  BtaPointing pointing(bsv, pocaB, _XZTilt, _YZTilt);
  _Radius = pointing.RadiusXY();

  // 
  // compute the z along the collision axis
  //
  _Zcoll = pointing.ZminusZip();

  //
  // compute the Sigend Flight Length in 3d
  //
  BtaSignedFlightLength3d FL(bsv, vtxVect, _XZTilt, _YZTilt, c->p4());
  _SfL3d = FL.SignedFlightLength3d();
} 
