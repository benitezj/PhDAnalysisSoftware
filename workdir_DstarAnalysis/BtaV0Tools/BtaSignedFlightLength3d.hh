//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: BtaSignedFlightLength3d.hh
//
// Description:
//	Class BtaSignedFlightLength3d - computes the Flight Length wrt to
//	collision axis in 3d, a sign can also be added to the FL due to
//	momentum and resolution effects
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
#ifndef BtaSignedFlightLength3d_HH
#define BtaSignedFlightLength3d_HH

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
#include "Beta/BtaCandidate.hh"

class BtaCandidate;

//              ---------------------
//              -- Class Interface --
//              ---------------------
 
class BtaSignedFlightLength3d {

  //--------------------
  // Instance Members --
  //--------------------

public:
  // Constructors

   BtaSignedFlightLength3d(Hep3Vector beamSpot, Hep3Vector VertexVect, double XZTilt, double YZTilt, HepLorentzVector momentum);
  
  // Destructor
  virtual ~BtaSignedFlightLength3d(){}

  // compute the length
  double SignedFlightLength3d( ) const ;
  
  


protected:


private:
   Hep3Vector _pocaKshort;
   HepLorentzVector _momentum;
   Hep3Vector _VertexVect;
};

#endif
