//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: BtuGamContentEMC.hh,v 1.1 2004/09/11 06:24:06 chcheng Exp $
//
// Description:
//      Class for filling gamma EMC information.
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
#ifndef BTUGAMCONTENTEMC_hh
#define BTUGAMCONTENTEMC_hh

#include "BtaTupleMaker/BtuContentBase.hh"

class BtuGamContentEMC : public BtuContentBase {

public:

  BtuGamContentEMC();
  BtuGamContentEMC(std::string block, std::string nprefix, int maxCands);
  virtual ~BtuGamContentEMC();

  virtual void fillNtpColumns(HepTuple *, AbsEvent *, HepAList<BtaCandidate>*);

private:

};
#endif
