//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: BtuContentFillerBase.cc,v 1.1 2004/09/11 06:24:05 chcheng Exp $
//
// Description:
//      Class that fills requested extra TRK and gamma information.
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

#include "BtaTupleMaker/BtuContentFillerBase.hh"

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
#include "HepTuple/Tuple.h"
#include "Beta/BtaCandidate.hh"

//----------------
// Constructors --
//----------------
BtuContentFillerBase::BtuContentFillerBase() : _initialized(false) {
}

//----------------
// Destructors --
//----------------
BtuContentFillerBase::~BtuContentFillerBase() {
}

void BtuContentFillerBase::fillNtpColumns(HepTuple *tuple, AbsEvent *anEvent,
					  HepAList<BtaCandidate>* candlist) {

  if ( !candlist ) return ;

  for (int i=0; i< _contents.size(); i++) 
    _contents[i]->fillNtpColumns(tuple,anEvent,candlist);

}
