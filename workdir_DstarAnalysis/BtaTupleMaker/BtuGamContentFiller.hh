//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: BtuGamContentFiller.hh,v 1.1 2004/09/11 06:24:06 chcheng Exp $
//
// Description:
//      Class that fills requested extra GAM information.
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
#ifndef BTUGAMCONTENTFILLER_hh
#define BTUGAMCONTENTFILLER_hh

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

class BtuGamContentFiller : public BtuContentFillerBase {

public:
  
  BtuGamContentFiller();
  virtual ~BtuGamContentFiller();

  void init(std::string block, std::string nprefix, int maxCands,
	    AbsParmVector<std::string>& pvec );

private:

};
#endif
