//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: BtuEventBlock.hh,v 1.4 2006/11/28 21:37:57 desilva Exp $
//
// Description:
//      Class for filling event block info.
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
#ifndef BTUEVENTBLOCK_HH
#define BTUEVENTBLOCK_HH

//------------------------------------
// Collaborating Class Header files --
//------------------------------------
#include "AbsParm/AbsParmVector.hh"
#include "CLHEP/Alist/AList.h"
#include "UsrData/UsrData.hh"
#include "ProxyDict/IfdKey.hh"
#include "ProxyDict/IfdStrKey.hh"
#include <map>
//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
class HepTuple;
class AbsEvent;
class EventInfo;
class IfdStrKey;
//              ---------------------
//              -- Class Interface --
//              ---------------------

class BtuEventBlock {

public:

  BtuEventBlock();

  virtual ~BtuEventBlock();

  void init(std::string cont, std::string boolTags, std::string intTags,
	    std::string floatTags, std::string eventID="AbsEventID");

  void fillTupleColumns(HepTuple *t, AbsEvent *anEvent, EventInfo* info);

private:

  void fillEventID(HepTuple *t, AbsEvent *anEvent);
  void fillCMp4(HepTuple *t, EventInfo* info);
  void fillBeamSpot(HepTuple *t, EventInfo* info);
  void fillPrimVtx(HepTuple *t, EventInfo* info);
  void fillUsrData(HepTuple *t, AbsEvent *anEvent);

  void errorGetVar(std::string vname, std::string block);

  std::vector<std::string> _boolTags;
  std::vector<std::string> _intTags;
  std::vector<std::string> _floatTags;
  
  bool _initialized;

  std::map<std::string, int> _contMap;

  std::string _eventID;

  std::vector<std::string> _usrEventBlock;

};
#endif

