//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: BtaBS.hh,v 1.8 2004/01/23 22:59:53 lockman Exp $
//
// Description:
//	Class BtaBS - computes the distance of closest apprach of 
//	2 straight line trajectories (addapted from a Bill DunWoodie FORTRAN 
//      routine)
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Jonathon Coleman                 Original author
//
// History:
//      Coleman, Lockman: added class BtaBS:
//         BtaBS(const BbrPointErr&): initialize BeamSpot parameters
//         Compute(const BtaCandidate* c, const BtaCandidate* d1,
//           const BtaCandidate* d2): compute quantities related to the
//                                    Vee and BeamSpot
//      accessors:
//         Hep3Vector Doca(): return the dist. of closest approach vector 
//         double XZTilt(): return the BeamSpot XZ tilt 
//         double YZTilt(): return the BeamSpot YZ tilt
//         double Radius(): return the radial distance of the Vee vertex 
//                          POCA from the collision axis 
//         double Zcoll():  return the z along the collision axis 
//         double Sep3d():  return the separation distance of the daughters 
//         double SfL3d():  return the 3d signed flight length
//
// Copyright Information:
//
//------------------------------------------------------------------------
#ifndef BtaBS_HH
#define BtaBS_HH

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
#include "Beta/BtaCandidate.hh"
#include "CLHEP/Geometry/HepPoint.h"
#include "BtaV0Tools/BtaCollisionAxisSlope.hh"
#include "BbrGeom/BbrPointErr.hh"
class BtaCandidate;

//              ---------------------
//              -- Class Interface --
//              ---------------------
 
class BtaBS {

  //--------------------
  // Instance Members --
  //--------------------
  
public:

  // Constructor
  BtaBS( const BbrPointErr& beamSpot , int alg=0, bool debug=false );
  
  void Compute( const BtaCandidate* c, const BtaCandidate* d1, 
		const BtaCandidate* d2);
  
  // Destructor
  virtual ~BtaBS(){;}
  
  // accessors 
  const Hep3Vector Doca() const { return _Doca; }
  double XZTilt() const { return _XZTilt; }
  double YZTilt() const { return _YZTilt; }
  double XYTilt() const { return _XYTilt; }
  double Radius() const { return _Radius; }
  double Zcoll()  const { return _Zcoll;  }
  double Sep3d()  const { return _Sep3d;  }
  double SfL3d()  const { return _SfL3d;  }

protected:
  
private:

  BbrPointErr _beamSpot;
  double _Sep3d;
  double _XYTilt;
  double _XZTilt;
  double _YZTilt;
  double _Radius, _Zcoll;
  Hep3Vector _Doca;
  double _SfL3d;
};

#endif








