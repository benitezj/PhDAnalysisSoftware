//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: HardElectronFilter.cc,v 1.2 2005/08/04 16:41:48 desilva Exp $
//
// Description:
//	Class HardElectronFilter - fast filter for the SL analysis
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Bob Kowalewski    Adapted from ISLTagFilter; cuts on tight electrons
//    with p*>1.0 GeV (based on tag words); cut on BGFMultiHadron must be
//    done separately (with TCL filter).  Cuts on R2 and nTrk are coded,
//    but the default values amount to no cut for BGFMultiHadron events.
//
//--------------------------------------------------------------------------

#include "BaBar/BaBar.hh"
//-----------------------
// This Class's Header --
//-----------------------
#include "FilterTools/HardElectronFilter.hh" 
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

#include "ErrLogger/ErrLog.hh"

#include "Framework/AbsParmGeneral.hh"
using std::endl;

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

//----------------
// Constructors --
//----------------
HardElectronFilter::HardElectronFilter( 
    const char* const theName, 
    const char* const theDescription )
    : TagFilterModule( theName, theDescription )
  , _nTrkCut   (new AbsParmGeneral<int>   ("nTrkCut",  this, 3 ))
  , _r2Cut     (new AbsParmGeneral<float> ("r2Cut",    this, 1.0 ))
  , _minTagMom (new AbsParmGeneral<float> ("tagCut",   this, 1.0 ))
{
  commands()->append(_nTrkCut);
  commands()->append(_r2Cut);
  commands()->append(_minTagMom);
  _nPassed = 0;
  _nPass1 = 0;
  _nPass2 = 0;
  _nPass3 = 0;
  _nPass4 = 0;
}

//--------------
// Destructor --
//--------------

// The destructor should be limited to undoing the work of the constructor
HardElectronFilter::~HardElectronFilter( )
{
  delete _nTrkCut;
  delete _r2Cut;
  delete _minTagMom;
}

//--------------
// Operations --
//--------------
AppResult
HardElectronFilter::beginJob( AbsEvent* anEvent )
{
  ErrMsg(trace)<<"begin Job"<<endmsg;
  return AppResult::OK;
}

AppResult
HardElectronFilter::event( AbsEvent* anEvent )
{
  // Build the tag accessor using the base class
  TagFilterModule::event( anEvent );

  float R2;
  int nTrk;
  bool status(false);
  bool passed(false);
  
  //  float highPTightMuon(-10.0);
  float highPTightElectron(-10.0);

  if ( 0 != tag( ) ) 
    {
      status = tag( )->getInt( nTrk, "nTracks" );
      status &= tag( )->getFloat(R2, "R2");
      //      status &= tag( )->getFloat(highPTightMuon,"muonTight1cm" );
      status &= tag( )->getFloat(highPTightElectron,"elecTight1cm" );

    }

  //  bool highMuon(false), highElectron(false);
  bool allOk(true);

  if ( true == status ) {
    _nPass1++;
    if ( nTrk > _nTrkCut->value() ) _nPass2++;
    else allOk=false;
    if ( R2 <= _r2Cut->value() ) _nPass3++;
    else allOk=false;
    if ( fabs(highPTightElectron) >= _minTagMom->value() ) _nPass4++;
    else allOk=false; 
  }
  else allOk=false;

  if (allOk)  
  {
      passed = true;
      _nPassed ++;
  }
  
  setPassed (passed);
  return AppResult::OK;
}

AppResult
HardElectronFilter::endJob( AbsEvent* anEvent )
{
  ErrMsg(routine) <<" end HardElectron Filter Job" << endl
		  <<" EVENTS PASSED: " << _nPassed
		  <<" status " << _nPass1
		  <<" nTrk " << _nPass2
		  <<" R2 " << _nPass3
		  <<" elec " << _nPass4
		  << endmsg;
  return AppResult::OK;
}

