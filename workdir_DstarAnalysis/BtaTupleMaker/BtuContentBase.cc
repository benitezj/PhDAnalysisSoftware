//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: BtuContentBase.cc,v 1.1 2004/09/11 06:24:05 chcheng Exp $
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
//--------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

#include "BtaTupleMaker/BtuContentBase.hh"

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

//----------------
// Constructors --
//----------------
BtuContentBase::BtuContentBase() : _block(""), _nprefix(""), _maxCands(0)
{
}

BtuContentBase::BtuContentBase(std::string block, std::string nprefix,
			       int maxCands) 
  : _block(block), _nprefix(nprefix), _maxCands(maxCands) {
}

//----------------
// Destructors --
//----------------
BtuContentBase::~BtuContentBase() {
}

