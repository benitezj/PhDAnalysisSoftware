//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppUserBuild.cc,v 1.6 2005/09/02 23:30:15 chcheng Exp $
//
// Description:
//	Class AppUserBuild implementation for the BetaMiniUser package.
//	This defines the link-time contents of the BetaApp application
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	Bill Lockman
//      Akbar Mokhtarani
//
// Copyright Information:
//	Copyright (C) 2002		UC Santa Cruz, LBL
//
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "Framework/AppUserBuild.hh"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
// core sequences
#include "BetaMiniEnvSequences/BetaMiniEnvSequence.hh"
#include "BetaMiniSequences/BetaMiniSequence.hh"

// actions
#include "BetaMiniSequences/BetaMiniActions.hh"

// Minimilist set of physics sequences
#include "BetaMiniSequences/BetaMiniPhysicsSequence.hh"
// Tagging
#include "BTaggingSequences/BtsTaggingSequence.cc"

#include "BtaTupleMaker/BtuMyAnalysis.hh"
#include "BtaTupleMaker/BtuTupleMaker.hh"

#include "UsrTools/UsrMakerTools.hh"

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------
class AppFramework;

//----------------
// Constructors --
//----------------

AppUserBuild::AppUserBuild( AppFramework* theFramework )
    : AppBuild( theFramework )
{
  // core sequence
  BetaMiniEnvSequence(this);
  BetaMiniSequence(this);

  // actions
  BetaMiniActions(theFramework);

  // minimilist physics sequence
  BetaMiniPhysicsSequence(this);

  // Tagging
  BtsTaggingSequence(this);

  // add modules for writing user data
  UsrMakerTools(this);

  add( new BtuMyAnalysis( "BtuMyAnalysis", "MyAnalysis") );
  add( new BtuTupleMaker( "BtuTupleMaker","Tuple maker module for users"));

}

//--------------
// Destructor --
//--------------

AppUserBuild::~AppUserBuild( )
{
}
