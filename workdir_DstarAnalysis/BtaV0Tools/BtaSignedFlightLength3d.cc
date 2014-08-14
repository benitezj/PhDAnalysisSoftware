//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: BtaSignedFlightLength3d.cc
//
// Description:
//      Class BtaSignedFlightLength3d - see .hh 
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Jonathon Coleman
//
// Copyright Information:
//  
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "BtaV0Tools/BtaSignedFlightLength3d.hh"
#include "BtaV0Tools/BtaClosestApproach.hh"
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

//----------------
// Constructors --
//----------------


BtaSignedFlightLength3d::BtaSignedFlightLength3d(Hep3Vector beamSpot,
						 Hep3Vector VertexVect,
						 double XZTilt, double YZTilt,
						 HepLorentzVector momentum) {
  
//    double deltaZ = 1.;
//    Hep3Vector collisionAxisLineCoord(beamSpot.x() + (XZTilt*deltaZ),
//  				    beamSpot.y() + (YZTilt*deltaZ),
//  				    beamSpot.z() + deltaZ);

//    Hep3Vector CompositeLineCoord(VertexVect.x() + deltaZ*(momentum.x()/momentum.z()), 
//  				VertexVect.y() + deltaZ*(momentum.y()/momentum.z()),
//  				VertexVect.z() + deltaZ);

//    BtaClosestApproach vtxInfo(beamSpot, collisionAxisLineCoord, VertexVect, CompositeLineCoord);
  
  BtaClosestApproach vtxInfo(beamSpot, VertexVect, XZTilt, YZTilt, momentum );
  
  _pocaKshort = vtxInfo.pocaB();
  _momentum = momentum;
  _VertexVect = VertexVect;
}	 

double BtaSignedFlightLength3d::SignedFlightLength3d() const {
	       
  Hep3Vector momentum = Hep3Vector(_momentum);
  double KshortDecayLength = (_VertexVect - _pocaKshort).mag() *  (_VertexVect - _pocaKshort).unit().dot(momentum.unit());

  return KshortDecayLength;
}






