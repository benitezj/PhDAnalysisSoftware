//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: BtaSeperationDist.cc
//
// Description:
//      Class BtaSeperationDist - see .cc 
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Jonathon Coleman
//      Dave Payne (included CLHEP algorithms)
// Copyright Information:
// 
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "BtaV0Tools/BtaSeperationDist.hh"
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
#include "TrkBase/TrkFit.hh"
#include "TrkBase/TrkRecoTrk.hh"
#include "TrkBase/TrkPoca.hh"
#include "TrkBase/TrkDifTraj.hh"
#include "CLHEP/Geometry/HepPoint.h"
#include "ErrLogger/ErrLog.hh"
using std::endl;


//----------------
// Constructors --
//----------------


BtaSeperationDist::BtaSeperationDist(const BtaCandidate *c1,const BtaCandidate *c2)  {
  
  _ok= false;
  
  //note only works for tracks
  if ( !c1 || !c2 ) {
    ErrMsg(error) << "Trying to find seperation distance using a null candidate pointer " 
		  << "(c1 pointer = " << c1 << ", c2 pointer= " << c2 << ")"
		  << endmsg;
    return;
  }

  if ( !c1->recoTrk() || !c2->recoTrk() ) {
    ErrMsg(error) << "Trying to find seperation distance using a null trk pointer " 
		  << "c1 candidate = " << *c1 << endl
		  << "c2 candidate = " << *c2 << endmsg;
    return;
  }
  
  const TrkFit *t1 = c1->recoTrk()->fitResult();
  const TrkFit *t2 = c2->recoTrk()->fitResult();
  
  const TrkDifTraj& traj1 = t1->traj();
  const TrkDifTraj& traj2 = t2->traj();
  
  TrkPoca poca(traj1, 0., traj2, 0.);
  _pntKp = traj1.position(poca.flt1());
  _pntKm = traj2.position(poca.flt2());
  
  _ok = true;

}	 

double BtaSeperationDist::SeperationDistance3d() const {

  /*
   double pntKpx = _pntKp.x(); 
   double pntKpy = _pntKp.y();
   double pntKpz = _pntKp.z();
   
   double pntKmx = _pntKm.x();
   double pntKmy = _pntKm.y();
   double pntKmz = _pntKm.z();

   double doca = sqrt(pow((pntKpx-pntKmx),2)+ pow((pntKpy-pntKmy),2)+pow((pntKpz-pntKmz),2));
  */

  if (_ok) return _pntKp.distanceTo(_pntKm);
  return -999;
 
}
