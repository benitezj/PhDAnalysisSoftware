//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: B0ToDstarA1CLoose.cc,v 1.2 2004/10/03 01:04:30 denardo Exp $
//
// Description:
//	
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//
// Copyright Information:
//	Copyright (C) 2004      SLAC 
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"
#include "FilterTools/B0ToDstarA1CLoose.hh"
#include <assert.h>
#include <iostream>
#include "AbsEnv/AbsEnv.hh"
#include "ErrLogger/ErrLog.hh"
#include "GenEnv/GenEnv.hh"
#include "CLHEP/Alist/AList.h"
#include "CLHEP/Alist/AIterator.h"
#include "BetaCoreTools/BtaPrintTree.hh"

B0ToDstarA1CLoose::B0ToDstarA1CLoose( const char* const theName, 
			const char* const theDescription )
  : TagFilterModule( theName, theDescription )
  , _inputList( "inputList", this, "???" )
  , _maxR2( "maxR2", this, 1000.0 )
{
  commands()->append( &_inputList );
  commands()->append( &_maxR2 );
}
		
B0ToDstarA1CLoose::~B0ToDstarA1CLoose()
{}

AppResult B0ToDstarA1CLoose::event( AbsEvent* anEvent )
{
  bool pass = false;

  BtaList * BList =  Ifd<BtaList>::get( anEvent, _inputList.value() );

  if ( BList != 0 ) {
    int nB = BList->length() ;
    if ( nB>0 )  pass = true ;

    if ( ErrLogging(debugging) ) {
      ErrMsg( debugging ) << "Input List length : " << nB << endmsg;
      static BtaPrintTree printer;
      BtaListIter it( *BList );
      BtaCandidate * c;
      while ( c = it() ) 
	ErrMsg( debugging ) << printer.print( *c ) << endmsg;
    }
  }
  if ( ErrLogging(debugging) ) {
    if ( pass ) 
      ErrMsg( debugging ) << " Event Passed" << endmsg;
    else
      ErrMsg( debugging ) << " Event Rejected" << endmsg;
  }
  setPassed( pass );
  return AppResult::OK;
}

AppModule *B0ToDstarA1CLoose::clone( const char *cloneName )
{
  return new B0ToDstarA1CLoose( cloneName, "clone of B0ToDstarA1CLoose" );
}

AppResult B0ToDstarA1CLoose::endJob( AbsEvent* anEvent )
{ return AppResult::OK; }

AppResult B0ToDstarA1CLoose::beginJob( AbsEvent*  anEvent )
{  return AppResult::OK; }
