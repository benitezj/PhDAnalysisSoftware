//----------------------------------------------------------------------------
//
//Class to calculate the invariant mass of the lepton daughters of a b->sll 
//candidate
//
// Author: Chris Schilling          Revision: 01            Date: 2005-10-05
//
//----------------------------------------------------------------------------

#ifndef BTOXSLLCALCBVARS_HH
#define BTOXSLLCALCBVARS_HH

class BtaCandidate;
class IfdProxyDict;
#include "CLHEP/Vector/LorentzVector.h"

class BToXsllCalcBVars
{
  private:

  BToXsllCalcBVars() // do not implement constructor
  {
  }

  public:

  static HepLorentzVector calc(
			       IfdProxyDict*,
			       const BtaCandidate*
			       );
};

#endif
