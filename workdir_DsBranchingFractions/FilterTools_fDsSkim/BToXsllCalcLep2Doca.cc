#include "BaBar/BaBar.hh"
#include "BbrGeom/BbrLorentzVectorErr.hh"
#include "Beta/BtaCandidate.hh"
#include "CLHEP/Alist/AIterator.h"
#include "CLHEP/Vector/ThreeVector.h"
#include "AbsEnv/AbsEnv.hh"
#include "BtaEnv/BtaEnv.hh"

#include "FilterTools/BToXsllCalcLep2Doca.hh"

Hep3Vector BToXsllCalcLep2Doca::calc(IfdProxyDict* event,
				   const BtaCandidate* cand)
{  
  HepAListIterator<BtaCandidate> dauIter(cand->daughterIterator());
  int nDau = cand->nDaughters();
  dauIter.skip(nDau-2);
  BtaCandidate *lep1 = dauIter();
  BtaCandidate *lep2 = dauIter();
  
  double docax2(-10000.0);
  double docay2(-10000.0);
  double docaz2(-10000.0);
  
  if (lep1 && lep2 && lep1->recoObject() != NULL && lep2->recoObject() != NULL) {
    const BbrLorentzVectorErr& ip(gblEnv->getBta()->pepBeams()->interactionPoint());
    const HepPoint beamSpotPoint(ip.x(),ip.y(),ip.z());
    HepPoint closest2(lep2->recoObject()->position(beamSpotPoint,BtaAbsRecoObject::XY));
    const double cdocax2(closest2.x()-beamSpotPoint.x());
    const double cdocay2(closest2.y()-beamSpotPoint.y());
    const double cdocaz2(closest2.z()-beamSpotPoint.z());
    docax2 = cdocax2;
    docay2 = cdocay2;
    docaz2 = cdocaz2;
  }
  
  Hep3Vector lep2Doca(docax2, docay2, docaz2);
  return lep2Doca;
}
