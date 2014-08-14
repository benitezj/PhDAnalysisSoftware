//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: BtaMapCoord.cc
//
// Description:
//      For use in calculating BtaCollisionAxisSlope
//      
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
#include "CLHEP/Matrix/Matrix.h"

bool mapCoord(HepMatrix &transMat, int *coordMap)
{
  int i,j=0;
  double sep= sqrt(0.5);
  for (i=0;i<3;i++) {
    for (j=0;j<3;j++) 
      if ( fabs(transMat[i][j]) > sep )
	coordMap[i]= j;
  }
  
  if ( coordMap[0]== coordMap[1] || coordMap[1]== coordMap[2] ||coordMap[2]== coordMap[0] ) 
    return false;
  else
    return true;
}

