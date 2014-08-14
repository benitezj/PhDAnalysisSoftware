//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: BtuGamContentEMC.cc,v 1.2 2004/11/10 01:02:50 fwinkl Exp $
//
// Description:
//      Class for filling gamma EMC information.
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

#include "BtaTupleMaker/BtuGamContentEMC.hh"
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
#include "AbsEvent/AbsEvent.hh"
#include "ErrLogger/ErrLog.hh"
#include "BetaMicroAdapter/BtaCalQual.hh"
#include "Beta/BtaCandidate.hh"
#include "CLHEP/Alist/AIterator.h"
#include "HepTuple/HTValOrderedVector.h"
#include "HepTuple/Tuple.h"
#include "CLHEP/Geometry/HepPoint.h"

//----------------
// Constructors --
//----------------
BtuGamContentEMC::BtuGamContentEMC() : BtuContentBase() {
}

BtuGamContentEMC::BtuGamContentEMC(std::string block, std::string nprefix,
				   int maxCands) 
  : BtuContentBase(block, nprefix, maxCands) {
}

//----------------
// Destructors --
//----------------
BtuGamContentEMC::~BtuGamContentEMC() {
}


void BtuGamContentEMC::fillNtpColumns(HepTuple *tuple, AbsEvent *anEvent, 
				      HepAList<BtaCandidate>* candlist) {

  HTValOrderedVector<float> lat,eraw,ecal,centx,centy,centz,a42;
  HTValOrderedVector<int> nCry;

  BtaCandidate *cand;
  HepAListIterator<BtaCandidate> iter(*candlist);
  int counter(0);
  while ( (cand= iter()) ) {
    if ( counter>= _maxCands) {
      ErrMsg(warning) << "Number of candidates ("<< candlist->length() 
                      <<") for ntuple block " 
                      << _block << " exceeds the limit " << _maxCands << endmsg;
      break;
    }
    counter++;

    const BtaCalQual* cQual = cand->getMicroAdapter()->getCalQual();

    lat.append(cQual->lateralMoment());
    ecal.append(cQual->ecalEnergy());
    eraw.append(cQual->rawEnergy());
    centx.append(cQual->centroid().x());
    centy.append(cQual->centroid().y());
    centz.append(cQual->centroid().z());
    a42.append(cQual->absZernike42());
    nCry.append(cQual->nCrystals());

  }

  std::string vname, nCname;
  nCname= _nprefix+_block;

  vname= _block+"Lat";
  tuple->column(vname.c_str(), lat, nCname.c_str(),0,_block.c_str());
  vname= _block+"A42";
  tuple->column(vname.c_str(), a42, nCname.c_str(),0,_block.c_str());
  vname= _block+"ECal";
  tuple->column(vname.c_str(), ecal, nCname.c_str(),0,_block.c_str());
  vname= _block+"ErawCal";
  tuple->column(vname.c_str(), eraw, nCname.c_str(),0,_block.c_str());
  vname= _block+"Centx";
  tuple->column(vname.c_str(), centx, nCname.c_str(),0,_block.c_str());
  vname= _block+"Centy";
  tuple->column(vname.c_str(), centy, nCname.c_str(),0,_block.c_str());
  vname= _block+"Centz";
  tuple->column(vname.c_str(), centz, nCname.c_str(),0,_block.c_str());

  vname= _block+"nCrys";
  tuple->column(vname.c_str(), nCry, nCname.c_str(),0,_block.c_str());

}
