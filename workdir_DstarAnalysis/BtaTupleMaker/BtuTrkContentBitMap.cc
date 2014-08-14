//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: BtuTrkContentBitMap.cc,v 1.4 2006/11/28 21:37:58 desilva Exp $
//
// Description:
//      Class for filling TRK tracking efficiency information.
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

#include "BtaTupleMaker/BtuTrkContentBitMap.hh"
//-------------
// C Headers --
//-------------
#include <assert.h>

//---------------
// C++ Headers --
//---------------

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "AbsEvent/AbsEvent.hh"
#include "AbsEnv/AbsEnv.hh"
#include "ProxyDict/IfdStrKey.hh"
#include "ErrLogger/ErrLog.hh"
#include "Beta/BtaCandidate.hh"
#include "CLHEP/Alist/AIterator.h"
#include "HepTuple/HTValOrderedVector.h"
#include "HepTuple/Tuple.h"
#include "BbrStdUtils/Tokenize.hh"
#include "BaBar/Constants.hh"
#include "EffTable/EffTable.hh"

using namespace babar::String;

//----------------
// Constructors --
//----------------
BtuTrkContentBitMap::BtuTrkContentBitMap() : BtuContentBase() {
}

BtuTrkContentBitMap::BtuTrkContentBitMap(std::string block, std::string nprefix,int maxCands, std::string args) 
  : BtuContentBase(block, nprefix, maxCands) {
  // -----------------------------------------------------------
  // args is a list of selectors bitmap
  //------------------------------------------------------------

  Tokenize tokens(args);
  std::string token;
  while ( ! (token = tokens(",")).empty() ) {
    _bitmaps.push_back(token);
  }
}

//----------------
// Destructors --
//----------------
BtuTrkContentBitMap::~BtuTrkContentBitMap() {
}


void BtuTrkContentBitMap::fillNtpColumns(HepTuple *tuple, AbsEvent *anEvent, 
					 HepAList<BtaCandidate>* trklist) {

  for (int i=0; i< _bitmaps.size(); i++) {
    // Get a bitmap
    HTValOrderedVector<int> bit_v;
    IfdStrKey theStrKey(_bitmaps[i]);
    std::map<BtaCandidate*, int>* selMap=
      Ifd< std::map<BtaCandidate*, int> >::get(anEvent, theStrKey);
    if ( ! selMap ) 
      ErrMsg(fatal) << "Can't find selectors bitmap \"" << _bitmaps[i] << "\""
		    << endmsg;
    BtaCandidate *trk;
    HepAListIterator<BtaCandidate> iter(*trklist);
    int counter(0);
    while ( (trk= iter()) ) {
      if ( counter>= _maxCands) {
	ErrMsg(warning) << "Number of candidates ("<< trklist->length()
                        <<") for ntuple block "
                        << _block << " exceeds the limit " << _maxCands << endmsg;
	break;
      }
      counter++;
      int bit(0);
      std::map<BtaCandidate*, int>::iterator mapiter;
      mapiter= selMap->find(trk);
      if ( mapiter!= selMap->end() ) bit= mapiter->second;
      else ErrMsg(warning) << "Can't find a track " << trk 
			   << " from the bit map " << _bitmaps[i] << endmsg;
      
      bit_v.append(bit);
    }
    
    std::string vname, nCname;
    nCname= _nprefix+_block;
    vname= _bitmaps[i];
    tuple->column(vname.c_str(), bit_v, nCname.c_str(),0,_block.c_str());
 
  }

}


