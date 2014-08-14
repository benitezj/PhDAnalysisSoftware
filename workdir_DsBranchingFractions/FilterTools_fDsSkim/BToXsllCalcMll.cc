#include "BaBar/BaBar.hh"
#include "CLHEP/Alist/AIterator.h"
#include "Beta/BtaCandidate.hh"
#include "CLHEP/Vector/LorentzVector.h"
#include "PDT/PdtLund.hh"
#include "FilterTools/BToXsllCalcMll.hh"

double BToXsllCalcMll::calc(IfdProxyDict* event,
			     const BtaCandidate* cand)
{  
   HepAListIterator<BtaCandidate> dauIter(cand->daughterIterator());
   BtaCandidate* dau = 0;
   HepLorentzVector totllp4(0.0);
   while( dau = dauIter() ){
      int daulundId = std::abs((int)dau->pdtEntry()->lundId());
      if( daulundId == PdtLund::e_plus || daulundId == PdtLund::e_minus || daulundId == PdtLund::mu_plus || daulundId == PdtLund::mu_minus )
      {
	 totllp4 += dau->p4();
      }
   }
   
   return totllp4.m(); 
}
