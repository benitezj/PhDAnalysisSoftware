// ********************************************************************
// CVS $Id: DmixD0ToK3PiSkim.cc,v 1.2 2006/11/23 19:05:01 desilva Exp $
//
// Software developed for the BaBar Detector at the SLAC B-Factory
//
// Author: Michael Wilson, UC Santa Cruz
// ********************************************************************

#include "BaBar/BaBar.hh"
#include "FilterTools/DmixD0ToK3PiSkim.hh"

#include "AbsEnv/AbsEnv.hh"
#include "AbsEvent/AbsEvent.hh"
#include "AbsEventTag/AbsEventTag.hh"
#include "Beta/EventInfo.hh"
#include "BetaMicroAdapter/BtaTrkQual.hh"
#include "CLHEP/Alist/AIterator.h"
#include "ErrLogger/ErrLog.hh"
#include "ProxyDict/IfdStrKey.hh"
#include "ProxyDict/IfdHepAListProxy.hh"
#include "ProxyDict/Ifd.hh"


DmixD0ToK3PiSkim::
DmixD0ToK3PiSkim( const char* const theName, 
                  const char* const theDescription )
  : TagFilterModule( theName, theDescription )
  , inputDstarListName( "inputDstarListName", this, "DstarToD0Pi_D0ToK3Pi_Dmix_init" )
  , finalDstarListName( "finalDstarListName", this, "DstarToD0Pi_D0ToK3Pi_Dmix" )
{
  commands()->append( &inputDstarListName );
  commands()->append( &finalDstarListName );
}


// ********************************************************************
// Public Functions
// ********************************************************************


AppResult
DmixD0ToK3PiSkim::
beginJob( AbsEvent* anEvent )
{
  ErrMsg(routine) << "begin filter Job" << endmsg; 
  nEventsRead = 0;
  nEventsPassed = 0;
  nCandsSeen = 0;
  return AppResult::OK;
}


AppResult
DmixD0ToK3PiSkim::
endJob( AbsEvent* anEvent )
{
  ErrMsg(routine) << "ending filter job: "
                  << "Read " << nEventsRead << ", "
                  << "Passed " << nEventsPassed << endmsg;
  return AppResult::OK;
}


AppResult
DmixD0ToK3PiSkim::
event( AbsEvent* anEvent )
{
  nEventsRead++;

  // ***********************************
  // retrieve information from the event
  // ***********************************
  
  HepAList<BtaCandidate>* inputDstarList =
    FetchList(anEvent,inputDstarListName);

  HepAList<BtaCandidate>* finalDstarList = new HepAList<BtaCandidate>;
  IfdHepAListProxy<BtaCandidate>* fproxy =
    new IfdHepAListProxy<BtaCandidate>( finalDstarList );
  if( ! Ifd< HepAList<BtaCandidate> >::put( anEvent, fproxy, finalDstarListName.value() ) ) {
    ErrMsg(error) << "Unable to put list "
                  << "\"" << finalDstarListName.value() << "\" "
                  << "into the event"
                  << endmsg;
  }


  if( inputDstarList->length() > 0 ) {

    HepAListIterator<BtaCandidate> dstarIter(*inputDstarList);
    BtaCandidate* dstarCand = 0;

    while( (dstarCand = dstarIter()) != 0 ) {
  
      // This is a sanity check made by BetaMiniData/BtaCandID.cc
      if( dstarCand->pdtEntry() != 0 &&
          (int(dstarCand->pdtEntry()->charge()) != int(dstarCand->charge())) ) {
        ErrMsg(warning) << "Candidate charge = " << dstarCand->charge() << " "
                        << "doesn't match type \"" << dstarCand->pdtEntry()->name() << ", "
                        << "throwing out this candidate to save BetaMiniData "
                        << "some trouble."
                        << endmsg;
        continue;
      }

      nCandsSeen++;
      *finalDstarList += new BtaCandidate( *dstarCand );
    }

    if( (finalDstarList->length() > 0) )
      return Passed();
  }
  
  return Failed();
}


// ********************************************************************
// Protected Functions
// ********************************************************************


AppResult
DmixD0ToK3PiSkim::
Passed()
{
  setPassed(true);
  nEventsPassed++;
  return AppResult::OK;
}


AppResult
DmixD0ToK3PiSkim::
Failed()
{
  setPassed(false);
  return AppResult::OK;
}


EventInfo*
DmixD0ToK3PiSkim::
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
DmixD0ToK3PiSkim::
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

