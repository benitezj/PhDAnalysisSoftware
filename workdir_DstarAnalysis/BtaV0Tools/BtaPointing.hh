//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: BtaPointing.hh
//
// Description:
//	Class BtaPointing - computes the distance of a composite particle 
//      from the collision axis (defined from the covarince matrix of the beamspot)
//      allowing the user to measure the initial point of production of the pair
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
#ifndef BtaPointing_HH
#define BtaPointing_HH

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
#include "Beta/BtaCandidate.hh"

class BtaCandidate;

//              ---------------------
//              -- Class Interface --
//              ---------------------
 
class BtaPointing {

  //--------------------
  // Instance Members --
  //--------------------

public:
  // Constructors


   BtaPointing(Hep3Vector beamSpot, Hep3Vector pocaKshort, double XZTilt, double YZTilt);
  // Destructor
   virtual ~BtaPointing(){}
  
  // accessors 
   double ZminusZip( ) const;
   double RadiusXY( ) const;

protected:


private:
   Hep3Vector _beamSpot;
   Hep3Vector _pocaKshort; 
   double _XZTilt; 
   double _YZTilt;

};

#endif
