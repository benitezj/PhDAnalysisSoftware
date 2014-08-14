//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: BtuContentFillerBase.hh,v 1.1 2004/09/11 06:24:05 chcheng Exp $
//
// Description:
//      Class that fills requested extra TRK or gamma information.
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
#ifndef BTUCONTENTFILLERBASE_hh
#define BTUCONTENTFILLERBASE_hh

//------------------------------------
// Collaborating Class Header files --
//------------------------------------
#include "AbsParm/AbsParmVector.hh"
#include "BtaTupleMaker/BtuContentBase.hh"
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

class BtuContentFillerBase {

public:
  
  BtuContentFillerBase();
  virtual ~BtuContentFillerBase();

  virtual void init(std::string block, std::string nprefix, int maxCands,
		    AbsParmVector<std::string>& pvec ) = 0 ;
  void fillNtpColumns(HepTuple *, AbsEvent *, HepAList<BtaCandidate>*);

protected:

  bool _initialized;
  std::vector<BtuContentBase*> _contents;

private:


};
#endif
