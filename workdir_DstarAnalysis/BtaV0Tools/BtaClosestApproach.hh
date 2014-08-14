//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: BtaClosestApproach.hh
//
// Description:
//	Class BtaClosestApproach - computes the distance of closest apprach of 
//	2 straight line trajectories (addapted from a Bill DunWoodie FORTRAN routine)
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Jonathon Coleman                 Original author
//
// Copyright Information:
//
//------------------------------------------------------------------------
#ifndef BtaClosestApproach_HH
#define BtaClosestApproach_HH

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
#include "Beta/BtaCandidate.hh"

class BtaCandidate;

//              ---------------------
//              -- Class Interface --
//              ---------------------
 
class BtaClosestApproach {

  //--------------------
  // Instance Members --
  //--------------------

public:
  // Constructors

  BtaClosestApproach( const Hep3Vector& beamSpot, const Hep3Vector& VertexVect,
		     double XZTilt, double YZTilt,
		     const HepLorentzVector& momentum);
  
  BtaClosestApproach( const Hep3Vector& a1, const Hep3Vector& a2,
		      const Hep3Vector& b1, const Hep3Vector& b2);
  
  // Destructor
  virtual ~BtaClosestApproach(){}
  
  // accessors 
   double doca() const ;
   Hep3Vector docaCoords() const;
   Hep3Vector pocaB() const;
   Hep3Vector pocaA() const;

protected:

  void init( const Hep3Vector& a1, const Hep3Vector& a2,
	     const Hep3Vector& b1, const Hep3Vector& b2);

private:

   Hep3Vector _pointOnLineA;
   Hep3Vector _pointOnLineB;

};

#endif
