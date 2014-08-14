// ********************************************************************
//
// Software developed for the BaBar Detector at the SLAC B-Factory
//
// Author: Giampiero Mancinelli, University of Cincinnati
// ********************************************************************

#include "BaBar/BaBar.hh"
#include "FilterTools/RbD0ToKenuSkim.hh"
#include "CLHEP/Alist/AList.h"
#include "CLHEP/Alist/AIterator.h"
#include "AbsEnv/AbsEnv.hh"
#include "AbsEvent/AbsEvent.hh"
#include "AbsEvent/getTmpAList.hh"
#include "ErrLogger/ErrLog.hh"
#include "AbsEventTag/AbsEventTag.hh"
#include "ProxyDict/IfdStrKey.hh"
#include "ProxyDict/IfdHepAListProxy.hh"
#include "ProxyDict/Ifd.hh"

RbD0ToKenuSkim::RbD0ToKenuSkim( const char* const theName,
            const char* const theDescription )
  : TagFilterModule( theName, theDescription )
  , _inputList("inputList", this, "RbBchToDstarSemiElec")
{
  commands()->append(& _inputList);;
}

RbD0ToKenuSkim::~RbD0ToKenuSkim()
{
}

AppResult
RbD0ToKenuSkim::beginJob( AbsEvent* anEvent )
{
  ErrMsg(routine) << "begin filter Job" << endmsg;
  _nEventsRead = 0;
  _nEventsPassed = 0;
  return AppResult::OK;
}

AppResult
RbD0ToKenuSkim::event( AbsEvent* anEvent )
{
  _nEventsRead++;

  HepAList<BtaCandidate>* theList = Ifd< HepAList<BtaCandidate> >::get( anEvent, _inputList.value() );

  if (!theList) ErrMsg(fatal) << "No input list found in event" << endmsg;

  bool accept(false);

  if ( theList != 0 && theList->length() > 0 ) {
    _nEventsPassed++;
    accept=true;
  }

  setPassed(accept);

  return AppResult::OK;
}

AppResult
RbD0ToKenuSkim::endJob( AbsEvent* anEvent )
{
  ErrMsg(routine) << "ending filter job -- Read: " << _nEventsRead
                                   << ", Passed: " << _nEventsPassed << endmsg;
  return AppResult::OK;
}
