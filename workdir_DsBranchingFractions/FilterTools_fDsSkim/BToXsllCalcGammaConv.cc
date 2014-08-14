#include "BaBar/BaBar.hh"
#include "Beta/BtaCandidate.hh"
#include "CLHEP/Alist/AIterator.h"
#include "VtxFitter/VtxFitterOper.hh"
#include "VtxFitter/VtxGammaConvOper.hh"
#include "VtxFitter/VtxGammaConv.hh"
#include "ProxyDict/Ifd.hh"

#include "FilterTools/BToXsllCalcGammaConv.hh"

int BToXsllCalcGammaConv::calc(IfdProxyDict* event,
				     const BtaCandidate* cand)
{  
  HepAListIterator<BtaCandidate> dauIter(cand->daughterIterator());
  const int nDau = cand->nDaughters();
  dauIter.skip(nDau-2);
  const BtaCandidate *lep1 = dauIter();
  const BtaCandidate *lep2 = dauIter();
  
  HepAList<BtaCandidate> * convList =
    Ifd<HepAList<BtaCandidate> >::get(event,"XsllGammaConv");
  HepAListIterator<BtaCandidate> convIter(*convList);
  BtaCandidate* gamma;
  bool isKstar(false);
  if(nDau==4)
    isKstar=true;
  while ( gamma = convIter() ) {
    double x = gamma->decayVtx()->point().x();
    double y = gamma->decayVtx()->point().y();
    double R = sqrt(x*x+y*y);
    double deltaxy = ((VtxGammaConv *)gamma->decayVtx())->dxy();
    double deltaz = ((VtxGammaConv *)gamma->decayVtx())->dz();
    double gmass = gamma->mass();
    if ( fabs(deltaxy)<0.5 && fabs(deltaz)<1.0 && gmass<0.03 &&
	 (!isKstar || (isKstar && R>2.0)) ) {
      BtaCandidate* gdgtr;
      HepAListIterator<BtaCandidate> dgtrIter(gamma->daughterIterator());
      while ( gdgtr = dgtrIter() ) {
        if ( gdgtr->overlaps(*lep1) || gdgtr->overlaps(*lep2) ) return true;
      }
    }
  }
  
  return false;
}
