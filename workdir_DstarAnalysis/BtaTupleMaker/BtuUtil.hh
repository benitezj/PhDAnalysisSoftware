//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: BtuUtil.hh,v 1.3 2005/09/30 21:04:52 chcheng Exp $
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
#ifndef BTUUTIL_HH
#define BTUUTIL_HH

#include "Beta/BtaCandidate.hh"
#include "CLHEP/Alist/AList.h"
#include "PDT/PdtEntry.hh"

class BtuUtil {

public:
  
  static BtaCandidate *matchUid(HepAList<BtaCandidate> *alist, const BtaCandidate *cand);

  static BtaCandidate* findClone(HepAList<BtaCandidate> *alist, const BtaCandidate *cand);

  static bool pdtIsAChargedTrack(const PdtEntry *pdt);
  static bool pdtIsAChargedTrack(std::string pdt);

  static void chopEnds(std::string &st, std::string ch= " \t\n");

  static HepAList<BtaCandidate>* rearrangeMcList(HepAList<BtaCandidate>* mcList, bool abortIfMissing= true);
  static int getIndex(BtaCandidate* aCand, HepAList<BtaCandidate>* aList);

private:


};
#endif
