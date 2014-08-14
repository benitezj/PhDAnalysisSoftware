//----------------------------------------------------------------------------
//
//Class to calculate the invariant mass of the non-lepton daughters of a b->sll (inclusive)
//candidate (we only consider pi's and K's)
//
// Author: Chris Schilling          Revision: 01            Date: 2005-09-30
//
//----------------------------------------------------------------------------

#ifndef BTOXSLLCALCMHADSLL_HH
#define BTOXSLLCALCMHADSLL_HH

class BtaCandidate;
class IfdProxyDict;

class BToXsllCalcMHadsll
{
  private:

  BToXsllCalcMHadsll() // do not implement constructor
  {
  }

  public:

  static double calc(
		     IfdProxyDict*,
		     const BtaCandidate*
		     );
};

#endif
