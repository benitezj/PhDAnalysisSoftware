#include "BaBar/BaBar.hh"

#include "BbrGeom/BbrLorentzVectorErr.hh"
#include "Beta/BtaCandidate.hh"
#include "BetaCoreTools/BtaBVariables.hh"
#include "CLHEP/Alist/AList.h"
#include "CLHEP/Alist/AIterator.h"
#include "CLHEP/Vector/LorentzVector.h"
#include "ProxyDict/Ifd.hh"

#include "FilterTools/BToXsllCalcBVars.hh"

HepLorentzVector BToXsllCalcBVars::calc(IfdProxyDict* event,
				    const BtaCandidate* cand)
{  
  HepAList<BtaCandidate>* gtvlList = Ifd<HepAList<BtaCandidate> >::get(event, "ChargedTracks");
  HepAList<BtaCandidate>* neutList = Ifd< HepAList<BtaCandidate> >::get (event, "CalorNeutral");
  HepAListIterator<BtaCandidate> gtvlIter(*gtvlList);
  HepAListIterator<BtaCandidate> neutIter(*neutList);
  
  BtaCandidate* roeCand(0);
  BbrLorentzVectorErr roep4(0);
  BbrLorentzVectorErr realp4(0);
  gtvlIter.rewind();
  while (roeCand = gtvlIter()) {
    if ( !(cand->overlaps(*roeCand)) ) roep4 += roeCand->p4WCov();
  }
  
  neutIter.rewind();
  while (roeCand = neutIter()) {
    if ( !(cand->overlaps(*roeCand)) ) roep4 += roeCand->p4WCov();
  }

  realp4 = cand->p4WCov();

  double mes(-10000.0);
  double deltae(-10000.0);
  double mesROE(-10000.0);
  double deltaeROE(-10000.0);

  BtaBVariables bVarsROE(roep4);;
  BtaBVariables bVars(realp4);
  
  if (bVars.status() == BtaBVariables::GOOD) {
    mes = bVars.m_ES();
    deltae = bVars.deltaE();
  }

  if (bVarsROE.status() == BtaBVariables::GOOD) {
    mesROE = bVarsROE.m_ES();
    deltaeROE = bVarsROE.deltaE();
  }
  
  HepLorentzVector theVars(0,0,0,0);
  theVars.setX(mes);
  theVars.setY(deltae);
  theVars.setZ(mesROE);
  theVars.setT(deltaeROE);
  return theVars;
}
