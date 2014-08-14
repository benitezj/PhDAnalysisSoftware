//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: DslnuccTagFilter.cc,v 1.11 2004/09/16 05:32:28 bartoldu Exp $
//
// Description:
//	Class DsStarLNuCCTagFilter - fast filter for the ISL analysis
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Daniel Azzopardi                Original author
//      Joerg Stelzer                   adapted for Ds->l nu
//
// Copyright Information:
//	Copyright (C) 2000 Queen Mary & Westfield College, U. London
//
//------------------------------------------------------------------------

#include "BaBar/BaBar.hh"
//-----------------------
// This Class's Header --
//-----------------------
#include "FilterTools/DslnuccTagFilter.hh" 
// also defines the class variables


//-------------
// C Headers --
//-------------
#include <assert.h>
#include <math.h>
#include <string>

//---------------
// C++ Headers --
//---------------

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "TagData/TagTransient.hh"
#include "TagData/TagTransientBoolIter.hh"
#include "TagData/TagTransientIntIter.hh"
#include "TagData/TagTransientFloatIter.hh"
#include "AbsEnv/AbsEnv.hh"
#include "GenEnv/GenEnv.hh"
#include "HepTuple/Tuple.h"
#include "HepTuple/TupleManager.h"

#include "ErrLogger/ErrLog.hh"

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

//----------------
// Constructors --
//----------------
DslnuccTagFilter::DslnuccTagFilter( const char* const theName, 
				    const char* const theDescription )
  : TagFilterModule( theName, theDescription )
  , _nTrk1        ("nTrk1",  this, 5 )
  , _nTrk2        ("nTrk2",  this, 2 )
  , _nNeut        ("nNeut",  this, 6 )
  , _minLeptMomCm ("minLeptMomCm",  this, 0.5 )
  , _upperBound   ("upperBound",  this, 0.6 )
  , _lowerBound   ("lowerBound",  this, 0.2 )
  , _slope        ("slope",  this, 1./12. )
  , _writeNtuple  ("writeNtuple", this, false )
{
  commands()->append( & _nTrk1);
  commands()->append( & _nTrk2);
  commands()->append( & _nNeut);
  commands()->append( & _minLeptMomCm);
  commands()->append( & _upperBound);
  commands()->append( & _lowerBound);
  commands()->append( & _slope);
  commands()->append( & _writeNtuple);
  _nTried = 0;
  _nPassed = 0;
}

//--------------
// Operations --
//--------------
AppResult
DslnuccTagFilter::beginJob( AbsEvent* anEvent )
{
  ErrMsg(trace)<<"begin Job"<<endmsg;

  if (_writeNtuple.value()) {
    HepTupleManager* manager = gblEnv->getGen()->ntupleManager();
    assert(manager != 0);
    _ntuple = manager->ntuple("DsTolnu ntuple");      
  }


  return AppResult::OK;
}

AppResult
DslnuccTagFilter::event( AbsEvent* anEvent )
{
  _nTried++;  // event counter

  // Build the tag accessor using the base class
  TagFilterModule::event( anEvent );

  bool status(false);
  bool passed(false);

  std::string sErr;

  int nTrk;
  int nNeut;

  float sphericity;
  float highPTightMuon;
  float highPTightElectron;

  if ( 0 != tag( ) ) {
    status = true;
    if( ! (tag( )->getInt( nTrk, "nTracks" )) ) {
      sErr += "nTracks, ";
      nTrk = -1;
      status = false;
    }
    if( ! (tag( )->getInt( nNeut, "ngammaLoose" )) ) {
      sErr += "ngammaLoose, ";
      nNeut = -1;
      status = false;
    }
    if( ! (tag( )->getFloat(sphericity, "sphericityAll")) ) {
      sErr += "sphericityAll, ";
      sphericity = -1.;
      status = false;
    }
    if( ! (tag( )->getFloat(highPTightMuon,"muonTight1cm" )) ) {
      sErr += "muonTight1cm, ";
      highPTightMuon = -1.;
      status = false;
    }
    if( ! (tag( )->getFloat(highPTightElectron,"elecTight1cm" )) ) {
      sErr += "elecTight1cm, ";
      highPTightElectron = -1.;
      status = false;
    }
  }

  if( _writeNtuple.value() ) {
    _ntuple->column("nTracks", nTrk);
    _ntuple->column("ngammaLoose", nNeut);
    _ntuple->column("sphericityAll", sphericity);
    _ntuple->column("muonTight1cm", highPTightMuon);
    _ntuple->column("elecTight1cm", highPTightElectron);
    _ntuple->dumpData();
  }

  if ( status ) {

    bool passedMuon(false);
    bool passedElectron(false);

    float muonCut = sphericity + _slope.value() * fabs(highPTightMuon);
    float electronCut = sphericity + _slope.value() * fabs(highPTightElectron);

    passedMuon = ( ( fabs(highPTightMuon) > _minLeptMomCm.value() ) &&
		   ( muonCut > _lowerBound.value() ) &&
		   ( muonCut < _upperBound.value() ) );

    passedElectron = ( ( fabs(highPTightElectron) > _minLeptMomCm.value() ) &&
		       ( electronCut > _lowerBound.value() ) &&
		       ( electronCut < _upperBound.value() ) );

    passed = ( ( nTrk >= _nTrk1.value() ) || 
		( ( nTrk >= _nTrk2.value() ) && ( nNeut >= _nNeut.value() ) ) );
    if(passed) {
      ErrMsg(trace) << "passed mupli cut" << endmsg;
    } else {
      ErrMsg(trace) << "not passed mupli cut" << endmsg;
    }
   
    if(passedMuon) {
      ErrMsg(trace) << "passed passedMuon cut" << endmsg;
    } else {
      ErrMsg(trace) << "not passed passedMuon cut" << endmsg;
    }
   
    if(passedElectron) {
      ErrMsg(trace) << "passed passedElectron cut" << endmsg;
    } else {
      ErrMsg(trace) << "not passed passedElectron cut" << endmsg;
    }
   
    passed &= ( passedMuon || passedElectron );
  } else {
    if(_verbose.value()) ErrMsg(trace) << "tag information incomplete, could not access "
				       << sErr << endmsg;
  }
  
  if(passed) {_nPassed ++;}
  setPassed(passed);
  return AppResult::OK;
}

AppResult
DslnuccTagFilter::endJob( AbsEvent* anEvent )
{
  ErrMsg(routine) << name() << ": events tried: " << _nTried << ", events passed: " << _nPassed << endmsg;
  return AppResult::OK;
}
 
