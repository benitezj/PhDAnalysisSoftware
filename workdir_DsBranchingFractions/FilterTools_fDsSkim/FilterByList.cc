//------------------------------------------------------------------------
// File and Version Information:
//      $Id: FilterByList.cc,v 1.3 2006/11/28 21:42:54 desilva Exp $
//
// Description:
//      Class FilterByList
//
//      Filters on non-empty candidates lists
//      The logic is the same as in TagFilterByName
//
// Author List:
//      Frank Winklmeier   (adapted from TagModules/TagFilterByName)
//
// Copyright Information:
//      Copyright (C) 2005     Colorado State University
//
//------------------------------------------------------------------------
//-----------------
// BaBar Headers --
//-----------------
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "FilterTools/FilterByList.hh"


//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "AbsEvent/AbsEvent.hh"
#include "ProxyDict/Ifd.hh"
#include "ProxyDict/IfdStrKey.hh"
#include "ErrLogger/ErrLog.hh"
#include "Beta/BtaCandidate.hh"

#include <string>

using std::endl;

//
// Constructor
//
FilterByList::FilterByList(const char* const name,
                           const char* const descr)
  : AppFilterModule(name, descr),
    _orList("orList",this),
    _andList("andList", this),
    _assertIfMissing("assertIfMissing", this, false),
    _flipSign("flipSign", this, false),
    _prescale("prescale", this, 1),
    _nEvents(0),
    _nPassed(0),
    _nPrescaled(0)
{
  commands()->append(&_orList);
  commands()->append(&_andList);  
  commands()->append(&_assertIfMissing);  
  commands()->append(&_flipSign);
  commands()->append(&_prescale);
}

FilterByList::~FilterByList()
{
}

AppResult FilterByList::beginJob(AbsEvent *event)
{ 
  ErrMsg(trace) << name() << ": beginJob()" << endmsg;

  _nEvents = _nPassed = _nPrescaled = 0;

   return AppResult::OK;
}

AppResult FilterByList::event( AbsEvent *event )
{

  bool passed(true);

  HepAList<BtaCandidate>* list;

  //
  // check all the "and" lists
  //
  for (int i=0; i<_andList.value().size(); i++) {
    const std::string& listName = _andList.value()[i];
    IfdStrKey theStrKey(listName);
    list = Ifd< HepAList<BtaCandidate> >::get(event, theStrKey);
    // assert if list is missing and set to do so
    if (!list && _assertIfMissing.value()) {
      ErrMsg(fatal) << "List " << listName << " does not exist." << endmsg;
    }

    if (!list || list->length()<=0) {
      passed = false;
      ErrMsg(trace) << name() << "failed to due to empty list "
                    << listName << endmsg;
      break;
    }
  }

  //
  // check "or" lists if event has passed so far
  //
  if (passed) {
    passed = false;
    if (_orList.value().size() == 0 ) passed = true;

    for (int i=0; i<_orList.value().size(); i++) {
      const std::string& listName = _orList.value()[i];
      IfdStrKey theStrKey(listName);
      list = Ifd< HepAList<BtaCandidate> >::get(event, theStrKey);
      // assert if list is missing and set to do so
      if (!list && _assertIfMissing.value()) {
        ErrMsg(fatal) << "List " << listName << " does not exist." << endmsg;
      }

      if (list && list->length()>0) {
        passed = true;
        break;
      } 
      else {
        ErrMsg(trace) << name() << "failed to due to empty list "
                      << listName << endmsg;
      }
    }
  }

  // see if need to flip the sense of passed
  if ( _flipSign.value() ) passed = !passed;

  // prescale
  if ( passed ) {
    _nPassed++;
    if ( _nPassed % _prescale.value() == 0 ) {
      _nPrescaled++;
    } 
    else {
      passed = false;
      ErrMsg(trace) << " fail because of prescale"<<endmsg;
    }
  }

  // set the value of passed
  setPassed( passed );
  return AppResult::OK;
}

AppResult FilterByList::endJob( AbsEvent* anEvent )
{
  ErrMsg(routine) << name() << ": endJob summary: " << endl
		  << "\t Events processed: " << _nEvents << endl
		  << "\t Events passed   : " << _nPassed << endl
		  << "\t Events prescaled: " << _nPrescaled << endmsg;
  
  return AppResult::OK;
}

void FilterByList::help( int argc, char** argv )
{
    cout << "This module filters based on candidate lists."
         << endl << endl;

    AppFilterModule::help( argc, argv );
}

AppModule* FilterByList::clone(const char* cloneName)
{
  return new FilterByList(cloneName, "clone of FilterByList");
}
