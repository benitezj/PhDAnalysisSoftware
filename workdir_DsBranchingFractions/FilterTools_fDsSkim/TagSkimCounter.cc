//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: TagSkimCounter.cc,v 1.9 2004/11/16 21:35:16 desilva Exp $
//
// Description:
//	Class TagSkimCounter. Sets a tagbit to true whenever event() is called
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

//-----------------
// BaBar Headers --
//-----------------
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "FilterTools/TagSkimCounter.hh"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "AbsEvent/AbsEvent.hh"
#include "AbsEventTag/AbsEventTag.hh"

#include "ErrLogger/ErrLog.hh"
#include "FrameJobReport/FjrFwkJobReport.hh"
#include <stdio.h>
#include <sstream>
#include <iostream>
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
TagSkimCounter::TagSkimCounter( const char* const theName, 
			const char* const theDescription )
  : AppModule( theName, theDescription )
  , _eventScaler(0)
  , _doTag("doTag", this, false)
  , _tagName("tagName", this, "Undefined")
{
  commands()->append(&_doTag);
  commands()->append(&_tagName);
}
		

//--------------
// Destructor --
//--------------
TagSkimCounter::~TagSkimCounter()
{
}

//--------------
// Operations --
//--------------
AppResult 
TagSkimCounter::beginJob( AbsEvent* anEvent ) { 
  _eventScaler = 0;

  if ( _doTag.value() ) {
    if ( _tagName.value() == "Undefined") {
      ErrMsg(fatal) << "Configuration Error: \n"
		    << "\t Value of tagName parameter has to be set through tcl"
		    << endmsg;
    }

    // declare a tag bit
    declareBool(_tagName.value());
  }

  return AppResult::OK;
}



AppResult
TagSkimCounter::event( AbsEvent *ev )
{
  _eventScaler++;
  if ( ErrLogging(trace) ) {
    ErrMsg(trace) << name() << ": passed event " << _eventScaler
		  << endmsg;
  }

  if ( _doTag.value() ) {
    // call base class method
    if ( setup(ev) ) {
      // call the real worker
      setBool(_tagName.value(),true);
    } else {
      ErrMsg(error) << "TagAccessor::setup() failed" << endmsg;
    }
  }

  return AppResult::OK;
}

AppResult 
TagSkimCounter::endJob( AbsEvent* anEvent ) {
  ErrMsg(routine) << name() << ":     total number of events=" 
		  << _eventScaler << endmsg;

  FjrFwkJobReport* theReport = FjrFwkJobReport::getFwkJobReport();
  if (theReport->isEnabled()) jobSummaryReport(); 


  return AppResult::OK;
}

AppModule* 
TagSkimCounter::clone(const char* cloneName)
{
  return new TagSkimCounter(cloneName, "Cloned TagSkimCounter");
}

void
TagSkimCounter::jobSummaryReport() {

  FjrFwkJobReport* theReport = FjrFwkJobReport::getFwkJobReport();

  std::string logOut("PathCounter");
  if ( ! theReport->existsHierarchy(logOut) ) {
    theReport->report(FjrAbsJobReport::Bbr, logOut, "placeHolder","dummy");
  }

  logOut += "::";
  logOut += name();
    
  std::ostringstream streamNum;
  streamNum << _eventScaler;
  theReport->report(FjrAbsJobReport::Bbr, logOut, "PassedEvents",
		    streamNum.str());

  cout << logOut << " " << streamNum << endl;
}
