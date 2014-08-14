//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: TwoPhotonPentaquarkTagFilter.cc,v 1.4 2004/08/09 07:00:45 bartoldu Exp $
//
// Description:
//	An example module that filters on the transient tag.
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Nic
//
//
// Copyright Information:
//	Copyright (C) 1998      SLAC
//	Copyright (C) 2000      Wah
//
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "FilterTools/TwoPhotonPentaquarkTagFilter.hh"

//-------------
// C Headers --
//-------------
#include <assert.h>
extern "C" {
}

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
TwoPhotonPentaquarkTagFilter::TwoPhotonPentaquarkTagFilter( const char* const theName, 
			const char* const theDescription )
  : TagFilterModule( theName, theDescription )
  , _minTotalEnergy("minTotalEnergy",this, 1.0)
  , _maxTotalEnergy("maxTotalEnergy",this, 8.0)
{
  commands()->append(& _minTotalEnergy);
  commands()->append(& _maxTotalEnergy);

}
		

//--------------
// Destructor --
//--------------
TwoPhotonPentaquarkTagFilter::~TwoPhotonPentaquarkTagFilter()
{
}

//-------------
// Methods   --
//-------------

AppResult
TwoPhotonPentaquarkTagFilter::beginJob( AbsEvent*  anEvent )
{
  if ( _verbose.value() ) cout << name() << ": begin job" << endl;
  // initialize event counters
  _nReadEvents = 0;
  _nPassed = 0;
  return AppResult::OK;
}


AppResult 
TwoPhotonPentaquarkTagFilter::event( AbsEvent* anEvent )
{
  _nReadEvents++;

  //bool cut1 = R2All<0.85 && R2All>0.02; CHECK
  //bool cut2 = sphericityAll>0.02; CHECK
  //bool cut4 = nGoodTrkLoose>=2;CHECK
  //bool cut5 = false; CHECK
  //if( nGoodTrkLoose%2==0 )
  //  { if( totChargeLoose==0 ) cut5=true; }
  //else
  //  { if( totChargeLoose==1 || totChargeLoose==-1 ) cut5=true; }
  //bool cut6 = p1Mag>0.05 && p1Mag<5; CHECK
  //float r1 = 5;
  //if( p1Mag>0 ) r1 = p1EmcCandEnergy/p1Mag;
  //bool cut7 = r1<0.9;  CHECK
  //bool cut8 = fabs(elecTight1cm)<=0.0005; CHECK
  //bool cut9 = eTotal>1. && eTotal<8.;


  int tagint;
  float tagfloat;
  bool passed = false;

  // Build the tag accessor using the base class
  TagFilterModule::event( anEvent );



  int nGoodTrkLoose = 0;
  if( tag()->getInt( tagint, "nGoodTrkLoose" ) )   { nGoodTrkLoose = tagint; }
  if( nGoodTrkLoose>=2 ) {
    int totChargeLoose = 0;
    if( tag()->getInt( tagint, "totChargeLoose" ) )  { totChargeLoose = tagint; }
    if( (totChargeLoose==0&&nGoodTrkLoose%2==0) || 
	(abs(totChargeLoose)==1&&nGoodTrkLoose%2==1) ) {
      float eTotal = 0.;
      if( tag()->getFloat( tagfloat, "eTotal" ) ) { eTotal = tagfloat; }
      if(eTotal>_minTotalEnergy.value() && eTotal<_maxTotalEnergy.value()) {
	float R2All = 0.;
	if( tag()->getFloat( tagfloat, "R2All" ) )  { R2All = tagfloat; }
	if(R2All<0.85 && R2All>0.02) {
	  float sphericityAll = 0.;
	  if( tag()->getFloat( tagfloat, "sphericityAll" ) )   { sphericityAll = tagfloat; }
	  if(sphericityAll>0.02) {
	    float p1Mag = 0.;
	    if( tag()->getFloat( tagfloat, "p1Mag" ) )  { p1Mag = tagfloat; }
	    if( p1Mag>0.05 && p1Mag<5 ) {
	      float p1EmcCandEnergy = 0.;
	      if( tag()->getFloat( tagfloat, "p1EmcCandEnergy" ) ) { p1EmcCandEnergy = tagfloat; }
	      float r1 = 5;
	      if( p1Mag>0 ) r1 = p1EmcCandEnergy/p1Mag;
	      if( r1<0.9 ) {
		float elecTight1cm = 0.;
		if( tag()->getFloat( tagfloat, "elecTight1cm" ) )  { elecTight1cm = tagfloat; }
		if( elecTight1cm<=0.0005 ) {
		  passed = true;
		}
	      }
	    }
	  }
	}
      }
    }
  }
  
  if (passed) {
    _nPassed++;
    if ( _verbose.value() ) cout << name() << ": event # " 
				 << _nReadEvents << " passed." << endl;
  } else {
    if ( _verbose.value() ) cout << name() << ": event # " 
				 << _nReadEvents << " rejected." << endl;
  }
  setPassed( passed );
  return AppResult::OK;

}

AppResult
TwoPhotonPentaquarkTagFilter::endJob( AbsEvent* anEvent ) {

  cout << name()  << " end job: " << _nPassed << " out of " 
       << _nReadEvents << " passed" << endl;
  
  return AppResult::OK;
}
