//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: BtuTrkContentBitMap.hh,v 1.3 2006/11/28 21:37:58 desilva Exp $
//
// Description:
//      Class for filling TRK Bit Map information.
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
#ifndef BTUTRKCONTENTBITMAP_hh
#define BTUTRKCONTENTBITMAP_hh

#include "BtaTupleMaker/BtuContentBase.hh"
#include <vector>

class BtuTrkContentBitMap : public BtuContentBase {

public:

  BtuTrkContentBitMap();
  BtuTrkContentBitMap(std::string block, std::string nprefix, int maxCands,
		      std::string args);
  virtual ~BtuTrkContentBitMap();

  virtual void fillNtpColumns(HepTuple *, AbsEvent *, HepAList<BtaCandidate>*);

private:

  std::vector<std::string> _bitmaps;
};
#endif
