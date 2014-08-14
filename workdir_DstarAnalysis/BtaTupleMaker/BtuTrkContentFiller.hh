//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: BtuTrkContentFiller.hh,v 1.1 2004/09/11 06:24:07 chcheng Exp $
//
// Description:
//      Class that fills requested extra TRK information.
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
//--------------------------------------------------------------------------
#ifndef BTUTRKCONTENTFILLER_hh
#define BTUTRKCONTENTFILLER_hh

//------------------------------------
// Collaborating Class Header files --
//------------------------------------
#include "BtaTupleMaker/BtuContentFillerBase.hh"
#include "AbsParm/AbsParmVector.hh"
#include "CLHEP/Alist/AList.h"
#include <string>
#include <vector>
#include <map>

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
class HepTuple;
class AbsEvent;
class BtaCandidate;
//              ---------------------
//              -- Class Interface --
//              ---------------------

class BtuTrkContentFiller : public BtuContentFillerBase {

public:
  
  BtuTrkContentFiller();
  virtual ~BtuTrkContentFiller();

  void init(std::string block, std::string nprefix, int maxCands,
	    AbsParmVector<std::string>& pvec );

private:

};
#endif
