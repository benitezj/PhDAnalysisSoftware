//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: BtaTagHadronFilter.cc,v 1.6 2004/08/09 07:00:44 bartoldu Exp $
//
// Description:
//	Class BtaTagHadronFilter
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	David R. Quarrie    	Originator
//
// Copyright Information:
//	Copyright (C) 1998      Lawrence Berkeley National Laboratory
//
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "FilterTools/BtaTagHadronFilter.hh"

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
BtaTagHadronFilter::BtaTagHadronFilter( const char* const theName, 
					const char* const theDescription )
  : TagFilterModule( theName, theDescription )
  , _pTotalCosThCut("pTotalCosThCut",this,0.8)
  , _eNeutralCosThCut("eNeutralCosThCut",this,0.8)
  , _eTotalCut("eTotalCut",this,6.)
  , _pTotalMassCut("pTotalMassCut",this,2.)
  , _nTrkCut("nTrkCut",this,2)
{
  commands()->append(&_pTotalCosThCut);
  commands()->append(&_eNeutralCosThCut);
  commands()->append(&_eTotalCut);
  commands()->append(&_pTotalMassCut);
  commands()->append(&_nTrkCut);
}

//--------------
// Destructor --
//--------------
BtaTagHadronFilter::~BtaTagHadronFilter()
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
BtaTagHadronFilter::beginJob( AbsEvent* anEvent )
{
    if ( _verbose.value() ) cout << name() << ": Begin Job." << endl;
    return AppResult::OK;
}

AppResult
BtaTagHadronFilter::event( AbsEvent* ev )
{
  float pTotalCosTh,eNeutralCosTh,eTotal,pTotalMass;
  bool cosmic;
  int nTrk;

  bool status;
  bool passed = false;

  // Build the tag accessor using the base class
  TagFilterModule::event( ev );
  if ( 0 != tag( ) ) {

    status  = tag( )->getFloat( pTotalCosTh, "pTotalCosTh" );
    status &= tag( )->getFloat( eNeutralCosTh, "eNeutralCosTh" );
    status &= tag( )->getFloat( eTotal, "eTotal" );
    status &= tag( )->getFloat( pTotalMass, "pTotalMass" );
    status &= tag( )->getBool( cosmic, "cosmic" );
    status &= tag( )->getInt( nTrk, "nTracks" );

    cosmic = false;  // ignore

    if ( status && !cosmic && nTrk > _nTrkCut.value() &&
	 ( pTotalCosTh > _pTotalCosThCut.value() || 
	   eNeutralCosTh > _eNeutralCosThCut.value() ) &&
	 eTotal > _eTotalCut.value() && pTotalMass > _pTotalMassCut.value() ) {
      passed = true;
    }
  }
  setPassed( passed );
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
