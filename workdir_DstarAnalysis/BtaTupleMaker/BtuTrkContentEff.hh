//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: BtuTrkContentEff.hh,v 1.1 2004/09/11 06:24:07 chcheng Exp $
//
// Description:
//      Class for filling TRK tracking efficiency information.
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
#ifndef BTUTRKCONTENTEFF_hh
#define BTUTRKCONTENTEFF_hh

#include "BtaTupleMaker/BtuContentBase.hh"
#include "ProxyDict/IfdStrKey.hh"
#include <vector>

class EffTable;

class BtuTrkContentEff : public BtuContentBase {

public:

  BtuTrkContentEff();
  BtuTrkContentEff(std::string block, std::string nprefix, int maxCands,
		   std::string args);
  virtual ~BtuTrkContentEff();

  virtual void fillNtpColumns(HepTuple *, AbsEvent *, HepAList<BtaCandidate>*);

private:

  std::vector<std::string> _tables;

  EffTable *getEffTable(const char* key);
  EffTable *getEffTable(const IfdStrKey& key);
};
#endif
