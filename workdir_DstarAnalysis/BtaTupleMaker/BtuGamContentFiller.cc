//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: BtuGamContentFiller.cc,v 1.1 2004/09/11 06:24:06 chcheng Exp $
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
#include "BaBar/BaBar.hh"

#include "BtaTupleMaker/BtuGamContentFiller.hh"

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

#include "BtaTupleMaker/BtuGamContentEMC.hh"

using namespace babar::String;

//----------------
// Constructors --
//----------------
BtuGamContentFiller::BtuGamContentFiller() : BtuContentFillerBase() {
}

//----------------
// Destructors --
//----------------
BtuGamContentFiller::~BtuGamContentFiller()
{
}

void BtuGamContentFiller::init(std::string block, std::string nprefix, 
			       int maxCands,
			       AbsParmVector<std::string>& pvec) {

  // Each entry should of pvec be either a keyword, e.g., EMC, or
  // a keyword followed by a semicolon followed by arguments
  // e.g., Eff:arg1,arg2
  // Everything after ":" will be passed to the class associated with that 
  // keyword.

  if ( _initialized ) return;

  std::map<std::string, bool> defined;
  defined["EMC"]= true;

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
  
  if ( fillThis["EMC"] )
    _contents.push_back(new BtuGamContentEMC(block,nprefix,maxCands));

  _initialized = true;
}
