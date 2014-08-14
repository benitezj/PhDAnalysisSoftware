//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: BtuTrkContentHots.hh,v 1.2 2006/08/07 21:39:09 mallen Exp $
//
// Description:
//      Class for filling Hit-on-track information
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
#ifndef BTUTRKCONTENTHOTS_hh
#define BTUTRKCONTENTHOTS_hh

#include "BtaTupleMaker/BtuContentBase.hh"
#include "ProxyDict/IfdStrKey.hh"

class BtuTrkContentHots : public BtuContentBase {

public:

  BtuTrkContentHots();
  BtuTrkContentHots(std::string block, std::string nprefix, int maxCands,
		    std::string args);
  virtual ~BtuTrkContentHots();
  
  virtual void fillNtpColumns(HepTuple *, AbsEvent *, HepAList<BtaCandidate>*);
  
private:

  bool _detailSVT, _detailDCH, _detailEMC;
};
#endif
