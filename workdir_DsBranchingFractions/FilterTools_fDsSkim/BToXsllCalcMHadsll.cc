#include "BaBar/BaBar.hh"
#include "CLHEP/Alist/AIterator.h"
#include "Beta/BtaCandidate.hh"
#include "CLHEP/Vector/LorentzVector.h"
#include "PDT/PdtLund.hh"
#include "FilterTools/BToXsllCalcMHadsll.hh"

double BToXsllCalcMHadsll::calc(IfdProxyDict* event,
			     const BtaCandidate* cand)
{  
   HepAListIterator<BtaCandidate> dauIter(cand->daughterIterator());
   BtaCandidate* dau = 0;
   HepLorentzVector totDaup4(0.0);
   while( dau = dauIter() ){
      int daulundId = std::abs((int)dau->pdtEntry()->lundId());
      if( daulundId == PdtLund::K_S0 || daulundId == PdtLund::K_plus || daulundId == PdtLund::pi0 || daulundId == PdtLund::pi_plus )
      {
	 totDaup4 += dau->p4();
      }
   }
   
   return totDaup4.m(); 
}
