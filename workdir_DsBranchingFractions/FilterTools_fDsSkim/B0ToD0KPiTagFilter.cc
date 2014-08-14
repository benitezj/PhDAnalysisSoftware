//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: B0ToD0KPiTagFilter.cc,v 1.4 2004/08/09 07:00:44 bartoldu Exp $
//
// Description:
//	Class B0ToD0KPiTagFilter
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Chih-hsiang Cheng
//
// Copyright Information:
//	Copyright (C) LLNL
//
//------------------------------------------------------------------------

#include "BaBar/BaBar.hh"
//-----------------------
// This Class's Header --
//-----------------------
#include "FilterTools/B0ToD0KPiTagFilter.hh"

//-------------
// C Headers --
//-------------
#include <assert.h>

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "AbsEventTag/AbsEventTag.hh"
#include "AbsEventTag/AbsEventTagBoolIter.hh"
#include "AbsEventTag/AbsEventTagIntIter.hh"
#include "AbsEventTag/AbsEventTagFloatIter.hh"
#include "Framework/AbsParmBool.hh"
using std::cout;
using std::endl;

//----------------
// Constructors --
//----------------
B0ToD0KPiTagFilter::B0ToD0KPiTagFilter( const char* const theName, 
					const char* const theDescription )
  : TagFilterModule( theName, theDescription ),
    _nTrkMinimum("nTrkMinimum", this, 5 ),
    _nTracksName("nTracksName", this, "nTracks"),
    _r2AllMax("r2AllMax", this, 0.5 )
{
  commands()->append(& _nTrkMinimum);
  commands()->append(& _nTracksName);
  commands()->append(& _r2AllMax);
}

//--------------
// Destructor --
//--------------
B0ToD0KPiTagFilter::~B0ToD0KPiTagFilter()
{
}

AppResult
B0ToD0KPiTagFilter::beginJob( AbsEvent* ev )
{
    if ( _verbose.value() ) cout << name() << ": Begin Job." << endl;
    return AppResult::OK;
}

AppResult
B0ToD0KPiTagFilter::endJob( AbsEvent* ev )
{
    if ( _verbose.value() ) cout << name() << ": End Job." << endl;
    return AppResult::OK;
}

AppResult
B0ToD0KPiTagFilter::event( AbsEvent* ev )
{

  int   anInt(0);
  float r2All(1);
  bool status;
  bool passed = false;

  // Build the tag accessor using the base class
  TagFilterModule::event( ev );
  
  if ( 0 != tag( ) ) {
    status= tag()->getInt(anInt, _nTracksName.value());
    if (status) status= tag()->getFloat(r2All, "R2All");
    if (status && anInt >= _nTrkMinimum.value() &&
	r2All <= _r2AllMax.value() ) passed= true;
  }

  setPassed( passed );
  return AppResult::OK;

}

