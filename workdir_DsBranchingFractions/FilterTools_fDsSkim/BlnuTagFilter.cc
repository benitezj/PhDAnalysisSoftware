//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: BlnuTagFilter.cc,v 1.6 2004/09/16 05:32:27 bartoldu Exp $
//
// Description:
//	Class BlnuTagFilter
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	Steven Robertson    	Adapted from BtaTagHadronFilter module
//
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "FilterTools/BlnuTagFilter.hh"

//-------------
// C Headers --
//-------------
extern "C" {
}

//---------------
// C++ Headers --
//---------------

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "AbsEventTag/AbsEventTag.hh"

#include "AbsEnv/AbsEnv.hh"
#include "GenEnv/GenEnv.hh"
#include "HepTuple/Tuple.h"
#include "HepTuple/TupleManager.h"
#include "HepTuple/Histogram.h"
using std::cout;
using std::endl;

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

//----------------
// Constructors --
//----------------
BlnuTagFilter::BlnuTagFilter( const char* const theName, 
					const char* const theDescription )
  : TagFilterModule( theName, theDescription )
  , _doNtuple("doNtuple",this,false)
  , _nTracksCut("nTracksCut",this,4)
  , _r2Cut("r2Cut",this,0.65)
  , _pLabLeptonCutLow("pLabLeptonCutLow",this,0.85)
  , _pCmLeptonCutLow("pCmLeptonCutLow",this,2.3)
  , _pCmLeptonCutHigh("pCmLeptonCutHigh",this,3.2)
  , _eTotalCut("eTotalCut",this,7.0)

{
  commands()->append(&_doNtuple);
  commands()->append(&_nTracksCut);
  commands()->append(&_r2Cut);
  commands()->append(&_pLabLeptonCutLow);
  commands()->append(&_pCmLeptonCutLow);
  commands()->append(&_pCmLeptonCutHigh);
  commands()->append(&_eTotalCut);

}

//--------------
// Destructor --
//--------------
BlnuTagFilter::~BlnuTagFilter()
{
}

//-------------
// Methods   --
//-------------

    
//-------------
// Operators --
//-------------
    
//-------------
// Selectors --
//-------------
    
//-------------
// Modifiers --
//-------------
AppResult
BlnuTagFilter::beginJob( AbsEvent* anEvent )
{
    if ( _verbose.value() ) cout << name() << ": Begin Job." << endl;

    if (_doNtuple.value()) {
      HepTupleManager* manager = gblEnv->getGen()->ntupleManager();
      assert(manager != 0);
      _BlnuNtuple = manager->ntuple("B->lnu ntuple");      
    }

    return AppResult::OK;
}

AppResult
BlnuTagFilter::event( AbsEvent* ev )
{
  float pElectronCm(0), pMuonCm(0), p2ElectronCm(0), p2MuonCm(0);
  float pElectronLab(0), pMuonLab(0), p2ElectronLab(0), p2MuonLab(0);
  float r2(0), eTotal(0), pTotalMag(0), pTotalCosTh(0);
  float xPrimaryVtx(0), yPrimaryVtx(0), zPrimaryVtx(0), probPrimaryVtx(0);
  bool bgfmultihadron;
  int nTracks(0), nGoodTrkLoose(0), totChargeLoose(0);
  float bTotalE(99.0);
  float leptonTotalE(0);

  bool status;
  bool passed = false;
  
  // Build the tag accessor using the base class
  TagFilterModule::event( ev );
  if ( 0 != tag( ) ) {
    status  = tag( )->getBool( bgfmultihadron, "BGFMultiHadron" );
    status &= tag( )->getInt( nTracks, "nTracks" );
    status &= tag( )->getInt( nGoodTrkLoose, "nGoodTrkLoose" );
    status &= tag( )->getFloat( r2, "R2");
    status &= tag( )->getInt( totChargeLoose, "totChargeLoose");
    status &= tag( )->getFloat( pTotalMag, "pTotalMag");
    status &= tag( )->getFloat( pTotalCosTh, "pTotalCosTh");
    status &= tag( )->getFloat( pMuonCm, "muonTight1cm" );
    status &= tag( )->getFloat( pElectronCm, "elecTight1cm" );
    status &= tag( )->getFloat( p2MuonCm, "muonTight2cm" );
    status &= tag( )->getFloat( p2ElectronCm, "elecTight2cm" );
    status &= tag( )->getFloat( pMuonLab,"muonTight1l" );
    status &= tag( )->getFloat( pElectronLab,"elecTight1l" );   
    status &= tag( )->getFloat( p2MuonLab,"muonTight2l" );
    status &= tag( )->getFloat( p2ElectronLab,"elecTight2l" );   
    status &= tag( )->getFloat( eTotal,"eTotal" );   
    status &= tag( )->getFloat( xPrimaryVtx,"xPrimaryVtx" ); 
    status &= tag( )->getFloat( yPrimaryVtx,"yPrimaryVtx" ); 
    status &= tag( )->getFloat( zPrimaryVtx,"zPrimaryVtx" ); 
    status &= tag( )->getFloat( probPrimaryVtx,"probPrimaryVtx" ); 

    if ( status && nTracks >= _nTracksCut.value() &&
	 r2 < _r2Cut.value() &&
	 ( (fabs(pElectronLab) > _pLabLeptonCutLow.value() )
	   || (fabs(pMuonLab) > _pLabLeptonCutLow.value())) ) {
      
      if ((fabs(pMuonCm) < _pCmLeptonCutHigh.value())
	  && (fabs(pElectronCm) < _pCmLeptonCutHigh.value()) ) {
	
	if (fabs(pMuonCm)>fabs(pElectronCm)) {
	  leptonTotalE=fabs(pMuonLab);
	} else {
	  leptonTotalE=fabs(pElectronLab);
	}
	// Just in case...allow for two high momentum leptons
       if ( ((fabs(pMuonCm)>_pCmLeptonCutLow.value()) && (fabs(p2MuonCm) >_pCmLeptonCutLow.value())) || 
	    ((fabs(pElectronCm)>_pCmLeptonCutLow.value()) && (fabs(p2ElectronCm) >_pCmLeptonCutLow.value())) ||
	    ((fabs(pMuonCm)>_pCmLeptonCutLow.value()) && (fabs(pElectronCm) >_pCmLeptonCutLow.value()))  ) {
	 leptonTotalE = 2*leptonTotalE;
       }

       bTotalE=eTotal-leptonTotalE;

       if (bTotalE < _eTotalCut.value()) {
	 passed = true;
       }
      }
    }
  }
  setPassed( passed );
  
  if (_doNtuple.value()) {
    _BlnuNtuple->column("passed",passed);
    _BlnuNtuple->column("BGFMultiHadron",bgfmultihadron);
    _BlnuNtuple->column("nTracks",nTracks);
    _BlnuNtuple->column("nGoodTrkLoose",nGoodTrkLoose);
    _BlnuNtuple->column("r2",r2);
    _BlnuNtuple->column("totChargeLoose",totChargeLoose);
    _BlnuNtuple->column("pTotalMag",pTotalMag);
    _BlnuNtuple->column("pTotalCosTh",pTotalCosTh);

    _BlnuNtuple->column("pTightMuonCm",pMuonCm);
    _BlnuNtuple->column("pTightElectronCm",pElectronCm);
    _BlnuNtuple->column("pTightMuonLab",pMuonLab);
    _BlnuNtuple->column("pTightElectronLab",pElectronLab);
    _BlnuNtuple->column("eTotal",eTotal);
    _BlnuNtuple->column("leptonTotalE",leptonTotalE);
    _BlnuNtuple->column("bTotalE",bTotalE);
    _BlnuNtuple->column("xPrimaryVtx",xPrimaryVtx);
    _BlnuNtuple->column("yPrimaryVtx",yPrimaryVtx);
    _BlnuNtuple->column("zPrimaryVtx",zPrimaryVtx);
    _BlnuNtuple->column("probPrimaryVtx",probPrimaryVtx);

    _BlnuNtuple->dumpData(); 
  }
  
  return AppResult::OK;
}


//		-----------------------------------------------
// 		-- Static Data & Function Member Definitions --
//		-----------------------------------------------

//		-------------------------------------------
// 		-- Protected Function Member Definitions --
//		-------------------------------------------

//		-----------------------------------------
// 		-- Private Function Member Definitions --
//		-----------------------------------------

//		-----------------------------------
// 		-- Internal Function Definitions --
//		-----------------------------------
