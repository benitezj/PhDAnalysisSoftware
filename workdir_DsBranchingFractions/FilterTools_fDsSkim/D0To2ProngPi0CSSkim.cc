// ********************************************************************
// CVS $Id: D0To2ProngPi0CSSkim.cc,v 1.2 2006/11/23 19:05:01 desilva Exp $
//
// Software developed for the BaBar Detector at the SLAC B-Factory
//
// Author: Michael Wilson, UC Santa Cruz
// ********************************************************************

#include "BaBar/BaBar.hh"
#include "FilterTools/D0To2ProngPi0CSSkim.hh"

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


D0To2ProngPi0CSSkim::
D0To2ProngPi0CSSkim( const char* const theName, 
                     const char* const theDescription )
  : TagFilterModule( theName, theDescription )
  , inputDstarListName1( "inputDstarListName1", this, "DstarToD0Pi_D0ToKKPi0_2PPi0CS_init" )
  , inputDstarListName2( "inputDstarListName2", this, "DstarToD0Pi_D0ToPiPiPi0_2PPi0CS_init" )
  , finalDstarListName1( "finalDstarListName1", this, "DstarToD0Pi_D0ToKKPi0_2PPi0CS" )
  , finalDstarListName2( "finalDstarListName2", this, "DstarToD0Pi_D0ToPiPiPi0_2PPi0CS" )
{
  commands()->append( &inputDstarListName1 );
  commands()->append( &inputDstarListName2 );
  commands()->append( &finalDstarListName1 );
  commands()->append( &finalDstarListName2 );
}


// ********************************************************************
// Public Functions
// ********************************************************************


AppResult
D0To2ProngPi0CSSkim::
beginJob( AbsEvent* anEvent )
{
  ErrMsg(routine) << "begin filter Job" << endmsg; 
  nEventsRead = 0;
  nEventsPassed = 0;
  nCandsSeen = 0;
  return AppResult::OK;
}


AppResult
D0To2ProngPi0CSSkim::
endJob( AbsEvent* anEvent )
{
  ErrMsg(routine) << "ending filter job: "
                  << "Read " << nEventsRead << ", "
                  << "Passed " << nEventsPassed << endmsg;
  return AppResult::OK;
}


AppResult
D0To2ProngPi0CSSkim::
event( AbsEvent* anEvent )
{
  nEventsRead++;

  // ***********************************
  // retrieve information from the event
  // ***********************************
  
  HepAList<BtaCandidate>* inputDstarList1 =
    FetchList(anEvent,inputDstarListName1);

  HepAList<BtaCandidate>* inputDstarList2 =
    FetchList(anEvent,inputDstarListName2);
  
  HepAList<BtaCandidate>* finalDstarList1 = new HepAList<BtaCandidate>;
  IfdHepAListProxy<BtaCandidate>* proxy1 =
    new IfdHepAListProxy<BtaCandidate>( finalDstarList1 );
  if( ! Ifd< HepAList<BtaCandidate> >::put( anEvent, proxy1, finalDstarListName1.value() ) ) {
    ErrMsg(error) << "Unable to put list "
                  << "\"" << finalDstarListName1.value() << "\" "
                  << "into the event"
                  << endmsg;
  }
  
  HepAList<BtaCandidate>* finalDstarList2 = new HepAList<BtaCandidate>;
  IfdHepAListProxy<BtaCandidate>* proxy2 =
    new IfdHepAListProxy<BtaCandidate>( finalDstarList2 );
  if( ! Ifd< HepAList<BtaCandidate> >::put( anEvent, proxy2, finalDstarListName2.value() ) ) {
    ErrMsg(error) << "Unable to put list "
                  << "\"" << finalDstarListName2.value() << "\" "
                  << "into the event"
                  << endmsg;
  }


  if( inputDstarList1->length() > 0 ) {

    HepAListIterator<BtaCandidate> dstarIter(*inputDstarList1);
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
      *finalDstarList1 += new BtaCandidate( *dstarCand );
    }
  }
  
  if( inputDstarList2->length() > 0 ) {
    
    HepAListIterator<BtaCandidate> dstarIter(*inputDstarList2);
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
      *finalDstarList2 += new BtaCandidate( *dstarCand );
    }
  }
  
  if( (finalDstarList1->length() > 0) ||
      (finalDstarList2->length() > 0) )
    return Passed();
  
  return Failed();
}


// ********************************************************************
// Protected Functions
// ********************************************************************


AppResult
D0To2ProngPi0CSSkim::
Passed()
{
  setPassed(true);
  nEventsPassed++;
  return AppResult::OK;
}


AppResult
D0To2ProngPi0CSSkim::
Failed()
{
  setPassed(false);
  return AppResult::OK;
}


EventInfo*
D0To2ProngPi0CSSkim::
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
D0To2ProngPi0CSSkim::
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

