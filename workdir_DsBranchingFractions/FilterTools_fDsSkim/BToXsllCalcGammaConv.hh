//----------------------------------------------------------------------------
//
//Class to calculate the di-lepton vtx probability
//
// Author: Chris Schilling          Revision: 01            Date: 2005-09-30
//
//----------------------------------------------------------------------------

#ifndef BTOXSLLCALCGAMMACONV_HH
#define BTOXSLLCALCGAMMACONV_HH

class BtaCandidate;
class IfdProxyDict;

class BToXsllCalcGammaConv
{
  private:

  BToXsllCalcGammaConv() // do not implement constructor
  {
  }

  public:

  static int calc(
		  IfdProxyDict*,
		  const BtaCandidate*
		  );
};

#endif
