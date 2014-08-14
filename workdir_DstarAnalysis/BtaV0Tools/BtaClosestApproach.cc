//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: BtaClosestApproach.cc
//
// Description:
//      Class BtaClosestApproach - see .hh 
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Bill Dunwoodie (orignal FORTRAN routine)
//      Jonathon Coleman (converted to C++)
//
// Copyright Information:
// 
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
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

/* 
   Test numbers to return dist = 0, lineA = lineB = (1,1,1):
   Hep3Vector a1(6., 4., 3.);
   Hep3Vector a2(16., 10., 7.);
   Hep3Vector b1(8., -5., 5.);
   Hep3Vector b2(16.75, -12.50, 10.);
*/  

BtaClosestApproach::BtaClosestApproach( const Hep3Vector& beamSpot,
					const Hep3Vector& VertexVect,
					double XZTilt, double YZTilt,
					const HepLorentzVector& momentum) {
  
  double deltaZ = 1.;
  Hep3Vector collisionAxisLineCoord(beamSpot.x() + (XZTilt*deltaZ),
				    beamSpot.y() + (YZTilt*deltaZ),
				    beamSpot.z() + deltaZ);

  Hep3Vector CompositeLineCoord(VertexVect.x() + deltaZ*(momentum.x()/momentum.z()), 
				VertexVect.y() + deltaZ*(momentum.y()/momentum.z()),
				VertexVect.z() + deltaZ);

  init(beamSpot, collisionAxisLineCoord, VertexVect, CompositeLineCoord);
}

//a1 & a2 are points one line and b1 & b2 are points on another
BtaClosestApproach::BtaClosestApproach( const Hep3Vector& a1,
					const Hep3Vector& a2,
					const Hep3Vector& b1,
					const Hep3Vector& b2) {
  init( a1, a2, b1, b2 );
}

void
BtaClosestApproach::init( const Hep3Vector& a1, const Hep3Vector& a2,
			  const Hep3Vector& b1, const Hep3Vector& b2) {
  
  Hep3Vector lineA = a1 - a2;
  Hep3Vector lineB = b1 - b2;
  
  double rl1sq = lineA.mag2();
  double rl2sq = lineB.mag2();
  double dsq   = lineA*lineB;
 
  double denom(0.), ax(0.), ay(0.), az(0.), bx(0.), by(0.), bz(0.), d(0.);
  double term1(0.), term2(0.), term3(0.), term4(0.), term5(0.),
    term6(0.), term7(0.), term8(0.), term9(0.), term10(0.);
  
  // in this calculation, the only division carried out is by 'denom';
  // this becomes zero only when the two lines are exactly parallel,
  // and so a check for this situation is performed here
  double lowNum = 0.0000000000001;
  denom = rl1sq*rl2sq-(dsq*dsq);
  if (denom < lowNum )
    {
      d =-999;
      ax=-999;
      ay=-999;
      az=-999;
      bx=-999;
      by=-999;
      bz=-999;
    }
  
  term1= ( (b2.x()-b1.x())*(a1.x()-b1.x())+(b2.y()-b1.y())*(a1.y()-b1.y()) ) *dsq;
  term2= ( (a2.x()-a1.x())*(a1.x()-b1.x())+(a2.y()-a1.y())*(a1.y()-b1.y()) ) *rl2sq;
  term3= ( term1-term2 )*(a2.z()-a1.z());
  term4= (a1.z()-b1.z())*( (b2.z()-b1.z())*dsq - (a2.z()-a1.z())*rl2sq );
  term5= (a2.z()-a1.z())*term4;
  
  term6= ( (a2.x()-a1.x())*(b1.x()-a1.x())+(a2.y()-a1.y())*(b1.y()-a1.y()) ) *dsq;
  term7= ( (b2.x()-b1.x())*(b1.x()-a1.x())+(b2.y()-b1.y())*(b1.y()-a1.y()) ) *rl1sq;
  term8= (term6-term7)*(b2.z()-b1.z());
  term9= (b1.z()-a1.z())*( (a2.z()-a1.z())*dsq - (b2.z()-b1.z())*rl1sq );
  term10= (b2.z()-b1.z())*term9;

  bz   = b1.z()+(term8+term10)/denom;
  bx   = b1.x()+(b2.x()-b1.x())*(term6-term7+term9)/denom;
  by   = b1.y()+(b2.y()-b1.y())*(term6-term7+term9)/denom;

  _pointOnLineB = Hep3Vector(bx,by,bz);

  az   = a1.z()+(term3+term5)/denom;
  ax   = a1.x()+(a2.x()-a1.x())*(term1-term2+term4)/denom;
  ay   = a1.y()+(a2.y()-a1.y())*(term1-term2+term4)/denom;

  _pointOnLineA = Hep3Vector(ax,ay,az);
}	 

double BtaClosestApproach::doca() const {
  return (_pointOnLineB - _pointOnLineA).mag();

}

Hep3Vector BtaClosestApproach::docaCoords() const {
  return _pointOnLineB - _pointOnLineA ;
}

Hep3Vector BtaClosestApproach::pocaB() const {
  return _pointOnLineB;
}

Hep3Vector BtaClosestApproach::pocaA() const {
  return _pointOnLineA;
}

