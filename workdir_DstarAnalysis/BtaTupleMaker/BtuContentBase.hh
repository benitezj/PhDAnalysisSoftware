//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: BtuContentBase.hh,v 1.1 2004/09/11 06:24:05 chcheng Exp $
//
// Description:
//      Base class for filling extra TRK or gamma information.
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
#ifndef BTUCONTENTBASE_hh
#define BTUCONTENTBASE_hh

//------------------------------------
// Collaborating Class Header files --
//------------------------------------
#include "CLHEP/Alist/AList.h"
#include <string>
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

class BtuContentBase {
  
public:

  BtuContentBase();
  BtuContentBase(std::string block, std::string nprefix, int maxCands);
  virtual ~BtuContentBase();
  
  virtual void fillNtpColumns(HepTuple *, AbsEvent *, HepAList<BtaCandidate>*) = 0;
  
protected:
  
  std::string _block, _nprefix;
  int _maxCands;

private:
  
};
#endif
