#include "BaBar/BaBar.hh"
#include "Beta/BtaCandidate.hh"
#include "BetaCoreTools/BtaOpMakeTree.hh"
#include "CLHEP/Alist/AIterator.h"
#include "CLHEP/Vector/LorentzVector.h"
#include "PDT/PdtLund.hh"
#include "ProbTools/probab.hh"
#include "VtxFitter/VtxFitterOper.hh"

#include "FilterTools/BToXsllCalcLepVtx.hh"

double BToXsllCalcLepVtx::calc(IfdProxyDict* event,
			     const BtaCandidate* cand)
{  
   HepAListIterator<BtaCandidate> dauIter(cand->daughterIterator());
   int nDau = cand->nDaughters();
   dauIter.skip(nDau-2);
   const BtaCandidate *lep1 = dauIter();
   const BtaCandidate *lep2 = dauIter();
   //std::cout<<"lep1 PdtEntry = "<<lep1->pdtEntry()->name()<<std::endl;
   //std::cout<<"lep2 PdtEntry = "<<lep2->pdtEntry()->name()<<std::endl;
   BtaOpMakeTree comb;
   BtaCandidate* dilep = comb.create(*lep1, *lep2);
   setGeoConstraint(*dilep);
   VtxFitterOper fitter(*dilep, VtxAbsAlgorithm::TreeFitter);
   fitter.fit();
   BtaCandidate fitDiLep = fitter.getFitted(*dilep);
   double dilepProb = probab(fitDiLep.decayVtx()->nDof(), fitDiLep.decayVtx()->chiSquared());
   delete dilep;
   return dilepProb;
}
