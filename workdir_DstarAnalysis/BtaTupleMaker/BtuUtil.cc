//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: BtuUtil.cc,v 1.4 2005/09/30 21:04:52 chcheng Exp $
//
// Description:
//      Some static utility functions.
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Chih-hsiang Cheng              Original Author
//
// Copyright Information:
//      Copyright (C) 2004      Lawrence Livermore National Laboratory
//--------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

#include "BtaTupleMaker/BtuUtil.hh"
#include "PDT/Pdt.hh"
#include "ErrLogger/ErrLog.hh"


BtaCandidate*
BtuUtil::matchUid(HepAList<BtaCandidate> *alist, const BtaCandidate *cand) {

  if ( 0== alist || cand==0 ) return 0;

  HepAListIterator<BtaCandidate> iter(*alist);
  BtaCandidate *ac;
  while ( (ac= iter()) ) {
    if ( ac->uid() == cand->uid()) return ac;
  }

  return 0;

}

BtaCandidate*
BtuUtil::findClone(HepAList<BtaCandidate> *alist, const BtaCandidate *cand) {
  if ( 0== alist || cand==0 ) return 0;

  HepAListIterator<BtaCandidate> iter(*alist);
  BtaCandidate *ac;
  while ( (ac= iter()) ) {
    if ( cand->isCloneOf(*ac, true) ) return ac;  // always checkType
  }

  return 0;

}

bool BtuUtil::pdtIsAChargedTrack(const PdtEntry *pdt) {
  if ( pdt== Pdt::lookup("pi+") || pdt== Pdt::lookup("pi-") ||
       pdt== Pdt::lookup("K+")  || pdt== Pdt::lookup("K-")  ||
       pdt== Pdt::lookup("e+")  || pdt== Pdt::lookup("e-")  ||
       pdt== Pdt::lookup("mu+") || pdt== Pdt::lookup("mu-") ||
       pdt== Pdt::lookup("p+")  || pdt== Pdt::lookup("p-") ) {
    return true;
  }
  return false;
}

bool BtuUtil::pdtIsAChargedTrack(std::string pdt) {
  return BtuUtil::pdtIsAChargedTrack(Pdt::lookup(pdt.c_str()));
}

void BtuUtil::chopEnds(std::string &st, std::string ch) {
  st.erase(0, st.find_first_not_of(ch));
  st.erase(st.find_last_not_of(ch)+1 );
}


HepAList<BtaCandidate>* BtuUtil::rearrangeMcList(HepAList<BtaCandidate>* mcList, bool abortIfMissing){

  // mcList has an order determined by recursion, with each generation of daughters 
  // included immediately in the list.  this method rearranges the list so that siblings
  // are contiguous

  HepAList<BtaCandidate>* orderedMcList = new HepAList<BtaCandidate>;

  if (mcList == NULL) {
    if (abortIfMissing) ErrMsg(fatal) << "BtuUtil::rearrangeMcList were given a NULL pointer" << endmsg;
    return 0;
  }

  BtaCandidate* mcCand(0);

  //bAdded == have we added this guy to orderedMcList?
  int mcLen = mcList->length();
  if ( mcLen == 0 ) return orderedMcList;

  vector<bool> bAdded(mcLen); 
  for (int i=0; i<mcList->length(); i++){
    bAdded[i] = false;
  }

  //bDaugh == have we added this guy's daughters to orderedMcList
  vector<bool> bDaugh(mcLen); 
  for (int i=0; i<mcList->length(); i++){
    bDaugh[i] = false;
  }

  // Do first guy here
  int jCurrent(0);             //jcurrent is the current mother in orderedMcList  
  mcCand = (*mcList)[0];
  orderedMcList->append(mcCand);
  bAdded[0] = true;

  // Do the rest here
  while (mcCand != NULL){

    // now add the daughters of mcCand
    HepAListIterator<BtaCandidate> dauIter = mcCand->daughterIterator();
    BtaCandidate* dauCand(0);
    while (dauCand = dauIter()){
      orderedMcList->append(dauCand);
      int jDau(BtuUtil::getIndex(dauCand,mcList));
      bAdded[jDau] = true;
    }
    bDaugh[jCurrent] = true;

    // get the next mcCand
    //  -- if there are still entries in orderedMcList
    //     that haven't been done, we'll use those
    //     otherwise, check in mcList for another guy
    //     ie. someone without a mother...
    jCurrent++;
    if (jCurrent<orderedMcList->length()){
      mcCand = (*orderedMcList)[jCurrent];
    } else {      
      mcCand = 0;
      for (int jNext=0;jNext<mcList->length();jNext++){
	if (!bAdded[jNext]) {
	  mcCand = (*mcList)[jNext];
	  orderedMcList->append(mcCand);
	  bAdded[jNext] = true;
	  break;
	}
      }
    }

  } 
  if (mcList->length() != orderedMcList->length()){
    ErrMsg(fatal) << "Length of mcList not equal to orderedList" << endmsg;
  }
  return orderedMcList;
}

int BtuUtil::getIndex(BtaCandidate* aCand, HepAList<BtaCandidate>* aList){

  // check the Cand
  if (aCand == NULL){
    return -2;
  }
                                                                                                                                                           
  // equivalent to CompBaseAnalUtil::findIndex for non-composites...
  HepAListIterator<BtaCandidate> aIter(*aList);
  BtaCandidate* cand;
  int index(-1);
  int ind(-1);
  while (cand = aIter()) {
    ind++;
    if (cand->uid() == aCand->uid()){
      index = ind;
    }
  }
  return index;
};


