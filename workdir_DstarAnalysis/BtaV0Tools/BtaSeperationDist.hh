//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: BtaSeperationDist.hh
//
// Description:
//	Class BtaSeperationDist - computes the distance between 2 tracks t1 & t2
//	given two BetaCandidates from a track list (will not work for neutrals!!!)
//	developed as chi^2 from the vetexer cannot be relied upon!!!!!
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
#ifndef BtaSeperationDist_HH
#define BtaSeperationDist_HH

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
#include "Beta/BtaCandidate.hh"

class BtaCandidate;

//              ---------------------
//              -- Class Interface --
//              ---------------------
 
class BtaSeperationDist {

  //--------------------
  // Instance Members --
  //--------------------

public:
  // Constructors

  BtaSeperationDist(const BtaCandidate *c1,const BtaCandidate *c2) ;
  
  // Destructor
  virtual ~BtaSeperationDist(){}
  
  // accessors 
  double SeperationDistance3d( ) const;

  bool Ok() const { return _ok; }

protected:


private:

  HepPoint _pntKp;
  HepPoint _pntKm;

  bool     _ok;

};

#endif
