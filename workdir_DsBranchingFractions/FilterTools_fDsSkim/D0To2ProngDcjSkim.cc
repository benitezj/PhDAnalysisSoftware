// ********************************************************************
// Software developed for the BaBar Detector at the SLAC B-Factory
//
// Author: Michael Wilson, UC Santa Cruz
// ********************************************************************

#include "BaBar/BaBar.hh"
#include "FilterTools/D0To2ProngDcjSkim.hh"

#include "AbsEnv/AbsEnv.hh"
#include "AbsEvent/AbsEvent.hh"
#include "AbsEvent/getTmpAList.hh"
#include "AbsEventTag/AbsEventTag.hh"
#include "Beta/EventInfo.hh"
#include "CLHEP/Alist/AIterator.h"
#include "ErrLogger/ErrLog.hh"
#include "ProxyDict/IfdStrKey.hh"
#include "ProxyDict/Ifd.hh"


D0To2ProngDcjSkim::
D0To2ProngDcjSkim( const char* const theName, 
                   const char* const theDescription )
  : TagFilterModule( theName, theDescription )
  , inputDstarListName( "inputDstarListName", this, "DstarToD0Pi_D0To2Prong_DCJ" )
  , inputD0UntaggedListName( "inputD0UntaggedListName", this, "D0To2ProngUntagged_DCJ" )
{
  commands()->append( &inputDstarListName );
  commands()->append( &inputD0UntaggedListName );
}


// ********************************************************************
// Public Functions
// ********************************************************************


AppResult
D0To2ProngDcjSkim::
beginJob( AbsEvent* anEvent )
{
  ErrMsg(routine) << "begin filter Job" << endmsg; 
  nEventsRead = 0;
  nEventsPassed = 0;
  return AppResult::OK;
}


AppResult
D0To2ProngDcjSkim::
endJob( AbsEvent* anEvent )
{
  ErrMsg(routine) << "ending filter job: "
                  << "Read " << nEventsRead << ", "
                  << "Passed " << nEventsPassed << endmsg;
  return AppResult::OK;
}


AppResult
D0To2ProngDcjSkim::
event( AbsEvent* anEvent )
{
  nEventsRead++;

  // ***********************************
  // retrieve information from the event
  // ***********************************

  // EventInfo* eventInfo = FetchEventInfo(anEvent);

  HepAList<BtaCandidate>* inputDstarList =
    FetchList(anEvent,inputDstarListName);

  HepAList<BtaCandidate>* inputD0UntaggedList =
    FetchList(anEvent,inputD0UntaggedListName);

  if( inputDstarList->length() > 0 ) {

    return Passed();
  }

  if( inputD0UntaggedList->length() > 0 ) {

    return Passed();
  }
  
  return Failed();
}


// ********************************************************************
// Protected Functions
// ********************************************************************


AppResult
D0To2ProngDcjSkim::
Passed()
{
  setPassed(true);
  nEventsPassed++;
  return AppResult::OK;
}


AppResult
D0To2ProngDcjSkim::
Failed()
{
  setPassed(false);
  return AppResult::OK;
}


EventInfo*
D0To2ProngDcjSkim::
FetchEventInfo( AbsEvent* anEvent )
{
  HepAList<EventInfo> *eventInfoList;
  static const IfdStrKey defaultKey("Default");
  eventInfoList = Ifd< HepAList<EventInfo> >::get( anEvent, defaultKey );
  EventInfo *eventInfo = eventInfoList->first();
  if( eventInfo == 0 ) {
    ErrMsg(fatal) << "no event information found for this event!!"
                  << endmsg;
  }
  return eventInfo;
}


HepAList<BtaCandidate>*
D0To2ProngDcjSkim::
FetchList( AbsEvent* anEvent, const AbsParmIfdStrKey& key )
{
  HepAList<BtaCandidate>* theList;
  theList = Ifd< HepAList<BtaCandidate> >::get( anEvent, key.value() );
  if( theList == 0 ) {
    ErrMsg(fatal) << "no \"" << key.command() << "\" list found, "
                  << "make sure that you have the correct sequences..."
                  << endmsg;
  }
  return theList;
}

