// ********************************************************************
// CVS $Id: DcTo3PiPromptSkim.cc,v 1.1 2004/09/30 22:43:22 kflood Exp $
//
// Software developed for the BaBar Detector at the SLAC B-Factory
//
// Author: Michael Wilson, UC Santa Cruz
// ********************************************************************

#include "BaBar/BaBar.hh"
#include "FilterTools/DcTo3PiPromptSkim.hh"
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

DcTo3PiPromptSkim::DcTo3PiPromptSkim( const char* const theName,
            const char* const theDescription ) : TagFilterModule( theName, theDescription )
  , _inputList("inputList", this, "DcTo3PiPrompt")
{
  commands()->append(& _inputList);;
}

DcTo3PiPromptSkim::~DcTo3PiPromptSkim()
{
}

AppResult
DcTo3PiPromptSkim::beginJob( AbsEvent* anEvent )
{
  ErrMsg(routine) << "begin filter Job" << endmsg;
  _nEventsRead = 0;
  _nEventsPassed = 0;
  return AppResult::OK;
}

AppResult
DcTo3PiPromptSkim::event( AbsEvent* anEvent )
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
DcTo3PiPromptSkim::endJob( AbsEvent* anEvent )
{
  ErrMsg(routine) << "ending filter job -- Read: " << _nEventsRead
                                   << ", Passed: " << _nEventsPassed << endmsg;
  return AppResult::OK;
}
