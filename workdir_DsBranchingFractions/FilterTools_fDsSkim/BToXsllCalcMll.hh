//----------------------------------------------------------------------------
//
//Class to calculate the invariant mass of the lepton daughters of a b->sll 
//candidate
//
// Author: Chris Schilling          Revision: 01            Date: 2005-10-05
//
//----------------------------------------------------------------------------

#ifndef BTOXSLLCALCMLL_HH
#define BTOXSLLCALCMLL_HH

class BtaCandidate;
class IfdProxyDict;

class BToXsllCalcMll
{
  private:

  BToXsllCalcMll() // do not implement constructor
  {
  }

  public:

  static double calc(
		     IfdProxyDict*,
		     const BtaCandidate*
		     );
};

#endif
