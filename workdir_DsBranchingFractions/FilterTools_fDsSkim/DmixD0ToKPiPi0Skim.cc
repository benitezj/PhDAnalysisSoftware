// ********************************************************************
// CVS $Id: DmixD0ToKPiPi0Skim.cc,v 1.6 2006/11/23 19:05:01 desilva Exp $
//
// Software developed for the BaBar Detector at the SLAC B-Factory
//
// Author: Michael Wilson, UC Santa Cruz
// ********************************************************************

#include "BaBar/BaBar.hh"
#include "FilterTools/DmixD0ToKPiPi0Skim.hh"

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


DmixD0ToKPiPi0Skim::
DmixD0ToKPiPi0Skim( const char* const theName, 
                    const char* const theDescription )
  : TagFilterModule( theName, theDescription )
  , prescaledDstarListName( "prescaledDstarListName", this, "DstarToD0Pi_D0ToKPiPi0VeryLoose_Dmix_init" )
  , inputDstarListName( "inputDstarListName", this, "DstarToD0Pi_D0ToKPiPi0_Dmix_init" )
  , scaledDstarListName( "scaledDstarListName", this, "DstarToD0Pi_D0ToKPiPi0VeryLoose_Dmix_prescale" )
  , finalDstarListName( "finalDstarListName", this, "DstarToD0Pi_D0ToKPiPi0_Dmix" )
  , prescaleFactor( "prescaleFactor", this, 10 )
{
  commands()->append( &prescaledDstarListName );
  commands()->append( &inputDstarListName );
  commands()->append( &scaledDstarListName );
  commands()->append( &finalDstarListName );
  commands()->append( &prescaleFactor );
}


// ********************************************************************
// Public Functions
// ********************************************************************


AppResult
DmixD0ToKPiPi0Skim::
beginJob( AbsEvent* anEvent )
{
  ErrMsg(routine) << "begin filter Job" << endmsg; 
  nEventsRead = 0;
  nEventsPassed = 0;
  nCandsSeen = 0;
  return AppResult::OK;
}


AppResult
DmixD0ToKPiPi0Skim::
endJob( AbsEvent* anEvent )
{
  ErrMsg(routine) << "ending filter job: "
                  << "Read " << nEventsRead << ", "
                  << "Passed " << nEventsPassed << endmsg;
  return AppResult::OK;
}


AppResult
DmixD0ToKPiPi0Skim::
event( AbsEvent* anEvent )
{
  nEventsRead++;

  // ***********************************
  // retrieve information from the event
  // ***********************************
  
  HepAList<BtaCandidate>* prescaledDstarList =
    FetchList(anEvent,prescaledDstarListName);

  HepAList<BtaCandidate>* inputDstarList =
    FetchList(anEvent,inputDstarListName);

  HepAList<BtaCandidate>* scaledDstarList = new HepAList<BtaCandidate>;
  IfdHepAListProxy<BtaCandidate>* sproxy =
    new IfdHepAListProxy<BtaCandidate>( scaledDstarList );
  if( ! Ifd< HepAList<BtaCandidate> >::put( anEvent, sproxy, scaledDstarListName.value() ) ) {
    ErrMsg(error) << "Unable to put list "
                  << "\"" << scaledDstarListName.value() << "\" "
                  << "into the event"
                  << endmsg;
  }

  HepAList<BtaCandidate>* finalDstarList = new HepAList<BtaCandidate>;
  IfdHepAListProxy<BtaCandidate>* fproxy =
    new IfdHepAListProxy<BtaCandidate>( finalDstarList );
  if( ! Ifd< HepAList<BtaCandidate> >::put( anEvent, fproxy, finalDstarListName.value() ) ) {
    ErrMsg(error) << "Unable to put list "
                  << "\"" << finalDstarListName.value() << "\" "
                  << "into the event"
                  << endmsg;
  }


  if( prescaledDstarList->length() > 0 ) {

    HepAListIterator<BtaCandidate> dstarIter(*prescaledDstarList);
    BtaCandidate* dstarCand = 0;

    while( (dstarCand = dstarIter()) != 0 ) {
  
      BtaCandidate* d0Cand     = 0;
      BtaCandidate* piSlowCand = 0;
      BtaCandidate* kCand      = 0;
      BtaCandidate* piCand     = 0;
      BtaCandidate* pi0Cand    = 0;
      BtaCandidate* gamma1Cand = 0;
      BtaCandidate* gamma2Cand = 0;
    
      bool isMergedPi0 = false;
      
      if( dstarCand->nDaughters() != 2 ) {
        ErrMsg(fatal) << "Dstar candidate from list \""
                      << inputDstarListName.command()
                      << "\" has "
                      << dstarCand->nDaughters()
                      << ", THIS IS NOT CORRECT FOR THIS SKIM!"
                      << endmsg;
      }

      HepAListIterator<BtaCandidate> dstarDaugIter( dstarCand->daughterIterator() );

      d0Cand = dstarDaugIter();
      if( d0Cand == 0 ) {
        ErrMsg(fatal) << "null pointer returned from dstarDaugIter "
                      << "for the D0 candidate, something is wrong."
                      << endmsg;
      }
      piSlowCand = dstarDaugIter();
      if( piSlowCand == 0 ) {
        ErrMsg(fatal) << "null pointer returned from dstarDaugIter "
                      << "for the pi_slow candidate, something is wrong."
                      << endmsg;
      }

      if( d0Cand->nDaughters() != 3 ) {
        ErrMsg(fatal) << "D0 candidate has "
                      << d0Cand->nDaughters()
                      << ", THIS IS NOT CORRECT FOR THIS SKIM!"
                      << endmsg;
      }
      
      HepAListIterator<BtaCandidate> d0DaugIter( d0Cand->daughterIterator() );
      
      kCand   = d0DaugIter();
      if( kCand == 0 ) {
        ErrMsg(fatal) << "null pointer returned from d0DaugIter "
                      << "for the K candidate, something is wrong."
                      << endmsg;
      }
      piCand  = d0DaugIter();
      if( piCand == 0 ) {
        ErrMsg(fatal) << "null pointer returned from d0DaugIter "
                      << "for the pi candidate, something is wrong."
                      << endmsg;
      }
      pi0Cand = d0DaugIter();
      if( pi0Cand == 0 ) {
        ErrMsg(fatal) << "null pointer returned from d0DaugIter "
                      << "for the pi0 candidate, something is wrong."
                      << endmsg;
      }
      
      if( pi0Cand->nDaughters() == 2 ) {
        isMergedPi0 = false;
        HepAListIterator<BtaCandidate> pi0DaugIter( pi0Cand->daughterIterator() );
        gamma1Cand = pi0DaugIter();
        if( gamma1Cand == 0 ) {
          ErrMsg(fatal) << "null pointer returned from pi0DaugIter "
                        << "for the gamma1 candidate, something is wrong."
                        << endmsg;
        }
        gamma2Cand = pi0DaugIter();
        if( gamma2Cand == 0 ) {
          ErrMsg(fatal) << "null pointer returned from pi0DaugIter "
                        << "for the gamma2 candidate, something is wrong."
                        << endmsg;
        }
      }
      else {
        isMergedPi0 = true;
      }
      
      // **************************************
      // We have all the candidate pointers now
      // **************************************

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

      if( nCandsSeen++ % prescaleFactor.value() == 0 ) {
        *scaledDstarList += new BtaCandidate( *dstarCand );
      }

      HepAListIterator<BtaCandidate> dstarTightIter(*inputDstarList);
      BtaCandidate* dstarTightCand = 0;
      while( (dstarTightCand = dstarTightIter()) != 0 ) {
        if( dstarCand->uid() == dstarTightCand->uid() ) {
          *finalDstarList += new BtaCandidate( *dstarCand );
          break;
        }
      }

    }

    if( (scaledDstarList->length() > 0) ||
        (finalDstarList->length() > 0) )
      return Passed();
  }
  
  return Failed();
}


// ********************************************************************
// Protected Functions
// ********************************************************************


AppResult
DmixD0ToKPiPi0Skim::
Passed()
{
  setPassed(true);
  nEventsPassed++;
  return AppResult::OK;
}


AppResult
DmixD0ToKPiPi0Skim::
Failed()
{
  setPassed(false);
  return AppResult::OK;
}


EventInfo*
DmixD0ToKPiPi0Skim::
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
DmixD0ToKPiPi0Skim::
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

