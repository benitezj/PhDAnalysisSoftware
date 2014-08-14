//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: BtuTrkContentDirc.hh,v 1.1 2004/11/12 01:29:11 chcheng Exp $
//
// Description:
//      Class for filling DIRC Cherenkov angle
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
#ifndef BTUTRKCONTENTDIRC_hh
#define BTUTRKCONTENTDIRC_hh

#include "BtaTupleMaker/BtuContentBase.hh"
#include "ProxyDict/IfdStrKey.hh"
#include "PDT/PdtPid.hh"
#include <vector>

class BtuTrkContentDirc : public BtuContentBase {
  
public:
  
  BtuTrkContentDirc();
  BtuTrkContentDirc(std::string block, std::string nprefix, int maxCands,
		    std::string args);
  virtual ~BtuTrkContentDirc();
  
  virtual void fillNtpColumns(HepTuple *, AbsEvent *, HepAList<BtaCandidate>*);
  
private:
  
  std::vector<PdtPid::PidType> _types;
  std::vector<std::string> _typeNames;
};

#endif
