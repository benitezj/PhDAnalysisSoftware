//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: TagSkimCounter.hh,v 1.4 2004/08/16 01:57:09 bartoldu Exp $
//
// Description:
//	Class TagSkimCounter. If doTag parameter is set to true 
//     (default is false), sets a tagbit to true whenever event() is called.
//      Useful for setting tagbits for specific skims. This module will count
//      events as well, so we will not need a separate counter
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	Yury Kolomensky     	Originator
//
// Copyright Information:
//	Copyright (C) 2001      LBNL
//
//------------------------------------------------------------------------

#ifndef TAGSKIMCOUNTER_HH
#define TAGSKIMCOUNTER_HH

//-----------------
// BaBar Headers --
//-----------------
#include "BaBar/BaBar.hh"

//----------------------
// Base Class Headers --
//----------------------
#include "Framework/AppModule.hh"
#include "TagData/TagAccessor.hh"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "Framework/AbsParmBool.hh"
#include "Framework/AbsParmString.hh"

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------

//		---------------------
// 		-- Class Interface --
//		---------------------

class TagSkimCounter : public AppModule,
		       public TagAccessor 
{

//--------------------
// Declarations     --
//--------------------
  
  // Typedefs, consts, and enums

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  TagSkimCounter( const char* const theName, 
		  const char* const theDescription );

  // Copy Constructor

  // Destructor
  virtual ~TagSkimCounter();

  // Operators

  // Selectors (const)

  // Modifiers
  virtual AppResult beginJob( AbsEvent* anEvent );
  virtual AppResult event( AbsEvent* anEvent );
  virtual AppResult endJob( AbsEvent* anEvent );

  virtual AppModule* clone(const char* cloneName);

  void jobSummaryReport();

private:
  // data members
  int  _eventScaler;

  AbsParmBool   _doTag;
  AbsParmString _tagName;
};

#endif 
