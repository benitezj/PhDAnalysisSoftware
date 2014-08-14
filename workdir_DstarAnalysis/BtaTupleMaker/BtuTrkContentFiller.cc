//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: BtuTrkContentFiller.cc,v 1.2 2004/11/12 01:29:11 chcheng Exp $
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
#include "BaBar/BaBar.hh"

#include "BtaTupleMaker/BtuTrkContentFiller.hh"

//-------------
// C Headers --
//-------------
#include <assert.h>

//---------------
// C++ Headers --
//---------------
#include <math.h>

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "ErrLogger/ErrLog.hh"

#include "AbsEvent/AbsEvent.hh"
#include "BbrStdUtils/Tokenize.hh"
#include "BbrStdUtils/String.hh"
#include "HepTuple/Tuple.h"
#include "Beta/BtaCandidate.hh"

#include "BtaTupleMaker/BtuTrkContentHots.hh"
#include "BtaTupleMaker/BtuTrkContentEff.hh"
#include "BtaTupleMaker/BtuTrkContentBitMap.hh"
#include "BtaTupleMaker/BtuTrkContentDirc.hh"

using namespace babar::String;

//----------------
// Constructors --
//----------------
BtuTrkContentFiller::BtuTrkContentFiller() : BtuContentFillerBase() {
}

//----------------
// Destructors --
//----------------
BtuTrkContentFiller::~BtuTrkContentFiller()
{
}

void BtuTrkContentFiller::init(std::string block, std::string nprefix, 
			       int maxCands,
			       AbsParmVector<std::string>& pvec) {

  // Each entry should of pvec be either a keyword, e.g., HOTS, or
  // a keyword followed by a semicolon followed by arguments
  // e.g., Eff:arg1,arg2
  // Everything after ":" will be passed to the class associated with that 
  // keyword.

  if ( _initialized ) return;

  std::map<std::string, bool> defined;
  defined["HOTS"]= true;
  defined["Eff"]= true;
  defined["BitMap"]= true;
  defined["Dirc"]= true;

  std::map<std::string, bool> fillThis;
  std::map<std::string, std::string> fArgs;

  for ( int i=0; i< pvec.value().size(); i++) {
    std::string token= pvec.value()[i];
    std::string key(""), args("");
    std::string::size_type pos= token.find_first_of(":");
    if ( pos== string::npos ) key= token;
    else {
      key= token.substr(0,pos);
      args= token.substr(pos+1);
    }
    
    if ( !defined[key] )
      ErrMsg(fatal) << "keyword \"" << key << "\" is not defined " << endmsg;
    
    if ( fillThis[key] ) 
      ErrMsg(warning) << "keyword \"" << key << "\" is repeated!" << endmsg;
    
    fillThis[key]= true;
    fArgs[key]= args;
  }
  
  if ( fillThis["HOTS"] )
    _contents.push_back(new BtuTrkContentHots(block,nprefix,maxCands,fArgs["HOTS"]));
  if ( fillThis["Eff"] )
    _contents.push_back(new BtuTrkContentEff(block,nprefix,maxCands,fArgs["Eff"]));
  if ( fillThis["BitMap"] )
    _contents.push_back(new BtuTrkContentBitMap(block,nprefix,maxCands,fArgs["BitMap"]));
  if ( fillThis["Dirc"] )
    _contents.push_back(new BtuTrkContentDirc(block,nprefix,maxCands,fArgs["Dirc"]));


  _initialized = true;
}
