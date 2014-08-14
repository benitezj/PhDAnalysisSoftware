#include "BaBar/BaBar.hh"
#include "BbrGeom/BbrLorentzVectorErr.hh"
#include "Beta/BtaCandidate.hh"
#include "CLHEP/Alist/AIterator.h"
#include "CLHEP/Vector/ThreeVector.h"
#include "AbsEnv/AbsEnv.hh"
#include "BtaEnv/BtaEnv.hh"

#include "FilterTools/BToXsllCalcLep1Doca.hh"

Hep3Vector BToXsllCalcLep1Doca::calc(IfdProxyDict* event,
				   const BtaCandidate* cand)
{  
  HepAListIterator<BtaCandidate> dauIter(cand->daughterIterator());
  int nDau = cand->nDaughters();
  dauIter.skip(nDau-2);
  BtaCandidate *lep1 = dauIter();
  BtaCandidate *lep2 = dauIter();
  
  double docax1(-10000.0);
  double docay1(-10000.0);
  double docaz1(-10000.0);
  
  if (lep1 && lep2 && lep1->recoObject() != NULL && lep2->recoObject() != NULL) {
    const BbrLorentzVectorErr& ip(gblEnv->getBta()->pepBeams()->interactionPoint());
    const HepPoint beamSpotPoint(ip.x(),ip.y(),ip.z());
    HepPoint closest1(lep1->recoObject()->position(beamSpotPoint,BtaAbsRecoObject::XY));
    const double cdocax1(closest1.x()-beamSpotPoint.x());
    const double cdocay1(closest1.y()-beamSpotPoint.y());
    const double cdocaz1(closest1.z()-beamSpotPoint.z());
    docax1 = cdocax1;
    docay1 = cdocay1;
    docaz1 = cdocaz1;
  }
  
  Hep3Vector lep1Doca(docax1, docay1, docaz1);
  return lep1Doca;
}
