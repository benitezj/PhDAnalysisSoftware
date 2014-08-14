//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: BtoRhoGammaBtoXGF.cc,v 1.3 2007/01/03 19:38:55 krosebe Exp $
//
// Description:
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Paul Harrison and Theresa Champion - based on Original template
//	by Charles Young
//
// Copyright Information:
//	Copyright (C) 1998      SLAC //
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "FilterTools/BtoRhoGammaBtoXGF.hh"

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
#include "ErrLogger/ErrLog.hh"
#include "GenEnv/GenEnv.hh"

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

//----------------
// Constructors --
//----------------
BtoRhoGammaBtoXGF::BtoRhoGammaBtoXGF( const char* const theName, 
			const char* const theDescription )
  : TagFilterModule( theName, theDescription )
  , _activeR2All( "activeR2AllCut", this, false )
  , _activeE1( "activeE1Cut", this, true )
  , _activeNTrk( "activeNTrkCut", this, false )
  , _maxR2All( "maxR2All", this, 1000.0 )
  , _minE1( "minE1", this,  1.5 )
  , _maxE1( "maxE1", this,  3.5 )
  , _minNTrk( "minNTRK", this, -1 )
{
  commands()->append( &_activeR2All );
  commands()->append( &_activeE1 );
  commands()->append( &_activeNTrk );
  commands()->append( &_maxR2All );
  commands()->append( &_minE1 );
  commands()->append( &_maxE1 );
  commands()->append( &_minNTrk );
}
		

//--------------
// Destructor --
//--------------
BtoRhoGammaBtoXGF::~BtoRhoGammaBtoXGF()
{
}

//-------------
// Methods   --
//-------------

AppResult BtoRhoGammaBtoXGF::beginJob( AbsEvent*  anEvent )
{
  ErrMsg(trace) << name() << ": begin job" << endmsg;

  // initialize event counters
  _nReadEvents = 0;
  _nPassed = 0;

  return AppResult::OK;
}


AppResult BtoRhoGammaBtoXGF::event( AbsEvent* anEvent )
{
  float E1 = 0.0;
  float R2All = 0.0;
  int nTRK = 0;
  bool isBtoGamma = true;			// Pass by default.

  _nReadEvents++;
 
  TagFilterModule::event( anEvent );		// Create transient tag

  if( !tag() )					// Failed to create tag
  {
    ErrMsg(error) << " No transient tag.  Failing event." << endmsg;
    setPassed( false );
    return AppResult::OK;
  }

  if( _activeNTrk.value() )			// Apply nTracks cut
  {
    if( !tag( )->getInt( nTRK, "nTracks" ) )	// Non-existent tag
    {
      ErrMsg(error) << " Cannot find Tag int nTracks." << endmsg;
      isBtoGamma = false;
    }
    else if( nTRK < _minNTrk.value() )			// Fails cut.
    {
      ErrMsg(debugging) << name() << "Failing nTracks cut." << endmsg;
      isBtoGamma = false;
    }
  }


  if( _activeE1.value() )			// Apply e1Mag cut
  {
    if( !tag( )->getFloat( E1, "e1Mag" ) )		// Non-existent tag
    {
      ErrMsg(error) << " Cannot find Tag float e1Mag." << endmsg;
      isBtoGamma = false;
    }
    else if( E1 < _minE1.value() || _maxE1.value() <= E1 )	// Fails cut.
    {
      ErrMsg(debugging) << name() << "Failing e1Mag cut." << endmsg;
      isBtoGamma = false;
    }
  }


  if( _activeR2All.value() )			// Apply R2All cut
  {
    if( !tag( )->getFloat( R2All, "R2All" ) )		// Non-existent tag
    {
      ErrMsg(error) << " Cannot find Tag float R2All." << endmsg;
      isBtoGamma = false;
    }
    else if( R2All > _maxR2All.value() )		// Fails cut.
    {
      ErrMsg(debugging) << name() << "Failing R2All cut." << endmsg;
      isBtoGamma = false;
    }
  }


  if( isBtoGamma )				// Counting and reporting
  {
    _nPassed++;

    ErrMsg(debugging) << name() << ": event # " 
    			<< _nReadEvents << " passed." << endmsg;
  }
  else
  {
    ErrMsg(debugging) << name() << ": event # " 
    			<< _nReadEvents << " rejected." << endmsg;
  }

  setPassed( isBtoGamma );

  return AppResult::OK;
}

AppResult BtoRhoGammaBtoXGF::endJob( AbsEvent* anEvent )
{

  ErrMsg(trace) << name()  << " end job: " << _nPassed << " out of " 
		<< _nReadEvents << " passed" << endmsg;
  
  return AppResult::OK;
}


AppModule *BtoRhoGammaBtoXGF::clone( const char *cloneName )
{
  return new BtoRhoGammaBtoXGF( cloneName, "clone of BtoRhoGammaBtoXGF" );
}
