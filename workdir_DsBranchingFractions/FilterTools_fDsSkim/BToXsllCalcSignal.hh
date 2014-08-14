//----------------------------------------------------------------------------
//
//Class to calculate the di-lepton vtx probability
//
// Author: Chris Schilling          Revision: 01            Date: 2005-09-30
//
//----------------------------------------------------------------------------

#ifndef BTOXSLLCALCSIGNAL_HH
#define BTOXSLLCALCSIGNAL_HH

class BtaCandidate;
class IfdProxyDict;
#include "CLHEP/Vector/ThreeVector.h"

class BToXsllCalcSignal
{
  private:

  BToXsllCalcSignal() // do not implement constructor
  {
  }

  public:

  static bool calc(
		   IfdProxyDict*
		   );
};

#endif
