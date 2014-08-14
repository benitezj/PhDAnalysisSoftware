//----------------------------------------------------------------------------
//
//Class to calculate the di-lepton vtx probability
//
// Author: Chris Schilling          Revision: 01            Date: 2005-09-30
//
//----------------------------------------------------------------------------

#ifndef BTOXSLLCALCLEPVTX_HH
#define BTOXSLLCALCLEPVTX_HH

class BtaCandidate;
class IfdProxyDict;

class BToXsllCalcLepVtx
{
  private:

  BToXsllCalcLepVtx() // do not implement constructor
  {
  }

  public:

  static double calc(
		     IfdProxyDict*,
		     const BtaCandidate*
		     );
};

#endif
