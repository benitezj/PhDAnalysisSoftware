//------------------------------------------------------------------------------------
// File and Version Information:
// 	$Id: ISLTagFilter.cc,v 1.9 2005/04/29 16:59:10 desilva Exp $
//
// Description:
//	Class ISLTagFilter - fast filter for the ISL analysis
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Daniel Azzopardi    Original author
//      Valery Miftakhov    moved to Tight tag lepton... (expect 3.7% selection rate)
//
// Copyright Information:
//	Copyright (C) 2000 Queen Mary & Westfield College, U. London
//
//-------------------------------------------------------------------------------------

#include "BaBar/BaBar.hh"
//-----------------------
// This Class's Header --
//-----------------------
#include "FilterTools/ISLTagFilter.hh" 
// also defines the class variables


//-------------
// C Headers --
//-------------
#include <assert.h>
#include <math.h>

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

#include "Framework/AbsParmGeneral.hh"
using std::endl;

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

//----------------
// Constructors --
//----------------
ISLTagFilter::ISLTagFilter( 
    const char* const theName, 
    const char* const theDescription )
    : TagFilterModule( theName, theDescription )
  , _doNtuple  (new AbsParmGeneral<bool>  ("doNtuple", this, false ))
  , _nTrkCut   (new AbsParmGeneral<int>   ("nTrkCut",  this, 3 ))
  , _r2Cut     (new AbsParmGeneral<float> ("r2Cut",    this, 0.9 ))
  , _minTagMom (new AbsParmGeneral<float> ("tagCut",   this, 1.3 ))
{
  commands()->append(_doNtuple);
  commands()->append(_nTrkCut);
  commands()->append(_r2Cut);
  commands()->append(_minTagMom);
  _nPassed = 0;
  _ntuple = 0;
}

//--------------
// Destructor --
//--------------

// The destructor should be limited to undoing the work of the constructor
ISLTagFilter::~ISLTagFilter( )
{
  delete _nTrkCut;
  delete _r2Cut;
  delete _minTagMom;
  delete _doNtuple;
}

//--------------
// Operations --
//--------------
AppResult
ISLTagFilter::beginJob( AbsEvent* anEvent )
{
  ErrMsg(trace)<<"begin Job"<<endmsg;
  if (true == _doNtuple->value()) 
    {
      HepTupleManager* manager = gblEnv->getGen()->ntupleManager();
      _ntuple = manager->ntuple(_name,1);
    }
  return AppResult::OK;
}

AppResult
ISLTagFilter::event( AbsEvent* anEvent )
{
  // Build the tag accessor using the base class
  TagFilterModule::event( anEvent );

  float R2;
  int nTrk;
  bool status(false);
  bool passed(false);
  
  float e1ll(-100.0), e2ll(-100.0), m1ll(-100.0), m2ll(-100.0);
  float e1lc(-100.0), e2lc(-100.0), m1lc(-100.0), m2lc(-100.0);
  float e1tl(-100.0), e2tl(-100.0), m1tl(-100.0), m2tl(-100.0);
  float e1tc(-100.0), e2tc(-100.0), m1tc(-100.0), m2tc(-100.0);

  float highPTightMuon(-10.0), highPTightElectron(-10.0);

  if ( 0 != tag( ) ) 
    {
      status = tag( )->getInt( nTrk, "nTracks" );
      status &= tag( )->getFloat(R2, "R2");
      status &= tag( )->getFloat(highPTightMuon,"muonTight1cm" );
      status &= tag( )->getFloat(highPTightElectron,"elecTight1cm" );

      // Fill the semileptonic tag quantities for ntuple filling;
      if ((true == _doNtuple->value()) &&
	  (0 != _ntuple) )
	{
	  tag( )->getFloat(e1ll,"elecLoose1l");
	  tag( )->getFloat(e2ll,"elecLoose2l");
	  tag( )->getFloat(m1ll,"muonLoose1l");
	  tag( )->getFloat(m2ll,"muonLoose2l");
	  tag( )->getFloat(e1lc,"elecLoose1cm");
	  tag( )->getFloat(e2lc,"elecLoose2cm");
	  tag( )->getFloat(m1lc,"muonLoose1cm");
	  tag( )->getFloat(m2lc,"muonLoose2cm");
	  tag( )->getFloat(e1tl,"elecTight1l");
	  tag( )->getFloat(e2tl,"elecTight2l");
	  tag( )->getFloat(m1tl,"muonTight1l");
	  tag( )->getFloat(m2tl,"muonTight2l");
	  tag( )->getFloat(e1tc,"elecTight1cm");
	  tag( )->getFloat(e2tc,"elecTight2cm");
	  tag( )->getFloat(m1tc,"muonTight1cm");
	  tag( )->getFloat(m2tc,"muonTight2cm");
	}
    }

  bool highMuon(false), highElectron(false);

  if ( ( true == status ) && 
       ( nTrk > _nTrkCut->value() ) && 
       ( R2 <= _r2Cut->value() )  && 
       ( (fabs(highPTightMuon) >= _minTagMom->value() ) 
	 ||(fabs(highPTightElectron) >= _minTagMom->value())))
    {
      passed = true;
      _nPassed ++;
      if (fabs(highPTightMuon) >= _minTagMom->value()) highMuon = true;
      if (fabs(highPTightElectron) >= _minTagMom->value()) highElectron = true;
    }
  
  if ( ( true == status ) && 
       (true == _doNtuple->value()) &&
       (0 != _ntuple) )
    {
      _ntuple->column("nTracks",nTrk);
      _ntuple->column("R2",R2);
      _ntuple->column("pass",passed);
      _ntuple->column("electag",highElectron);
      _ntuple->column("muontag",highMuon);
      _ntuple->column("e1ll",e1ll);
      _ntuple->column("e2ll",e2ll);
      _ntuple->column("m1ll",m1ll);
      _ntuple->column("m2ll",m2ll);
      _ntuple->column("e1lc",e1lc);
      _ntuple->column("e2lc",e2lc);
      _ntuple->column("m1lc",m1lc);
      _ntuple->column("m2lc",m2lc);   
      _ntuple->column("e1tl",e1tl);
      _ntuple->column("e2tl",e2tl);
      _ntuple->column("m1tl",m1tl);
      _ntuple->column("m2tl",m2tl);
      _ntuple->column("e1tc",e1tc);
      _ntuple->column("e2tc",e2tc);
      _ntuple->column("m1tc",m1tc);
      _ntuple->column("m2tc",m2tc);   
      _ntuple->dumpData();
    }
  setPassed (passed);
  return AppResult::OK;
}

AppResult
ISLTagFilter::endJob( AbsEvent* anEvent )
{
  ErrMsg(routine) <<" end ISL Tag Filter Job" << endl
		  <<" EVENTS PASSED: " << _nPassed << endmsg;
  return AppResult::OK;
}

