//----------------------------------------------------------------------------
//
//Class to calculate the di-lepton vtx probability
//
// Author: Chris Schilling          Revision: 01            Date: 2005-09-30
//
//----------------------------------------------------------------------------

#ifndef BTOXSLLCALCLEP2DOCA_HH
#define BTOXSLLCALCLEP2DOCA_HH

class BtaCandidate;
class IfdProxyDict;
#include "CLHEP/Vector/ThreeVector.h"

class BToXsllCalcLep2Doca
{
  private:

  BToXsllCalcLep2Doca() // do not implement constructor
  {
  }

  public:

  static Hep3Vector calc(
		     IfdProxyDict*,
		     const BtaCandidate*
		     );
};

#endif
