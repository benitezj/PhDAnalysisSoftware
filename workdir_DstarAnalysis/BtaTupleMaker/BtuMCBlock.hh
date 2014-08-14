//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: BtuMCBlock.hh,v 1.2 2005/06/24 01:44:40 jreiss Exp $
//
// Description:
//      Class for filling MC block info.
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Chih-hsiang Cheng              Original Author
//
// Copyright Information:
//      Copyright (C) 2004      Lawrence Livermore National Laboratory
//
//------------------------------------------------------------------------
#ifndef BTUMCBLOCK_HH
#define BTUMCBLOCK_HH

//------------------------------------
// Collaborating Class Header files --
//------------------------------------
#include "AbsParm/AbsParmVector.hh"
#include "CLHEP/Alist/AList.h"

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
class HepTuple;
class BtaCandidate;
class EventInfo;

//              ---------------------
//              -- Class Interface --
//              ---------------------

class BtuMCBlock {

public:

  BtuMCBlock();

  virtual ~BtuMCBlock();
  
  void init(std::string content);

  void fillTupleColumns(HepTuple *t, HepAList<BtaCandidate> *mcList,
			EventInfo* info);

private:

  bool _initialized;
  std::string _mclen, _mcblock;

  std::map<std::string, int> _contMap;
  void fillBasic(HepTuple *t, HepAList<BtaCandidate> *mcList);
  void fillMass(HepTuple *t, HepAList<BtaCandidate> *mcList);
  void fillMomentum(HepTuple *t, HepAList<BtaCandidate> *mcList);
  void fillCMMomentum(HepTuple *t, HepAList<BtaCandidate> *mcList,
		      EventInfo* info);
  void fillVertex(HepTuple *t, HepAList<BtaCandidate> *mcList);

};
#endif

