//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: BtaPointing.cc
//
// Description:
//      Class BtaPointing - see .hh 
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
#include "BtaV0Tools/BtaPointing.hh"
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

//needs only to be called at the begining of an event then all possible candidates can 
BtaPointing::BtaPointing(Hep3Vector beamSpot, Hep3Vector pocaKshort, double XZTilt, double YZTilt) 
{ 
   // get the the tilts
   _beamSpot = beamSpot;
   _pocaKshort = pocaKshort;
   _XZTilt = XZTilt;
   _YZTilt = YZTilt;
}	 

// the following methods take into account the slope of the collision axis

double BtaPointing::ZminusZip() const {
   //calculate the spacing in the Z direction from the nominal IP

   double beamDeltaZ = _pocaKshort.z() - _beamSpot.z();
   return beamDeltaZ;
 
}

double BtaPointing::RadiusXY() const {
   //calculate the radial distance of the composite from the IP in the transverse plain
   double Xbeam = ( _beamSpot.x() + (_XZTilt*(_pocaKshort.z()-_beamSpot.z())) );
   double Ybeam = ( _beamSpot.y() + (_YZTilt*(_pocaKshort.z()-_beamSpot.z())) );
   double deltaXY = sqrt( pow((_pocaKshort.x()-Xbeam), 2) + pow((_pocaKshort.y()-Ybeam), 2) );
   return deltaXY;
 
}
