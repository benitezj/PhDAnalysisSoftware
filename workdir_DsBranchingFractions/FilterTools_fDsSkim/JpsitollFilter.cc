//------------------------------------------------------------------------
// File and Version Information:
//      $Id: JpsitollFilter.cc,v 1.2 2005/11/04 20:37:26 fwinkl Exp $
//
// Description:
//      Class JpsitollFilter
//
//      A tag filter module for the inclusive J/psi skims
//      Filters on:
//              (0 <= R2 <= R2max) ||  BGFMultiHadron
//
// Author List:
//      Frank Winklmeier
//
// Copyright Information:
//      Copyright (C) 2005     Colorado State University
//
//------------------------------------------------------------------------

#include "BaBar/BaBar.hh"

#include "FilterTools/JpsitollFilter.hh"
#include "AbsEventTag/AbsEventTag.hh"
#include "ErrLogger/ErrLog.hh"

using std::endl;

//
// Constructor
//
JpsitollFilter::JpsitollFilter( const char * const name,
								const char * const descr )
	: TagFilterModule( name, descr ),
	  _R2max("R2max", this, 0.5),
	  _nEvents(0),
	  _nPassed(0)
{
  commands()->append(&_R2max);
}


AppResult JpsitollFilter::beginJob(AbsEvent *event)
{
  _nEvents = _nPassed = 0;
  return AppResult::OK;
}


AppResult JpsitollFilter::event( AbsEvent *event )
{

	// Build the tag accessor using the base class
  TagFilterModule::event( event );

  _nEvents++;
  bool passed(true);

  if (tag() != 0) {
		bool status(false);
		
		status = tag()->getBool(passed, "BGFMultiHadron");
		if (!status) 
			ErrMsg(fatal) << "Tag value BGFMultiHadron does not exist"<<endmsg;
		
		float R2;
		status = tag()->getFloat(R2, "R2");
		if (status) passed = passed || ((R2>=0) && (R2<=_R2max.value())); 
		else ErrMsg(fatal) << "Tag value R2 does not exist"<<endmsg;
  }
  setPassed(passed);
  if (passed) _nPassed++;
	
  return AppResult::OK;
}

AppResult JpsitollFilter::endJob( AbsEvent* anEvent )
{
  ErrMsg(routine) << name() << ": endJob summary: " << endl
                  << "\t Events processed: " << _nEvents << endl
                  << "\t Events passed   : " << _nPassed << endmsg;
  
  return AppResult::OK;
}
