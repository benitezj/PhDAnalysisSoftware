//--------------------------------------------------------------------------
// File and Version Information:
//
// $Id: InclProtTightSkim.cc,v 1.5 2006/11/23 19:05:02 desilva Exp $
//
//--------------------------------------------------------------------------
#include "BaBar/BaBar.hh"
#include "FilterTools/InclProtTightSkim.hh"

///////////////
#include "BetaCoreTools/BtaOpAdd4.hh"
#include "CLHEP/Alist/AList.h"
#include "CLHEP/Alist/AIterator.h"
#include "AbsEnv/AbsEnv.hh"
#include "AbsEvent/AbsEvent.hh"
#include "AbsEvent/getTmpAList.hh"
#include "ErrLogger/ErrLog.hh"
#include "AbsEventTag/AbsEventTag.hh"
#include "Beta/EventInfo.hh"

#include "TrkBase/TrkAbsFit.hh"
#include "TrkFitter/TrkPocaXY.hh"
#include "BetaPid/PidProtonLHSelector.hh"
#include "BetaPid/PidSelector.hh"
#include "BetaPid/PidSelFactoryManager.hh"

using std::cout;
using std::endl;

InclProtTightSkim::InclProtTightSkim( const char* const theName, 
			      const char* const theDescription )
  : TagFilterModule( theName, theDescription )
  , _protonLHSelectorVL(0) 
  , _protonLHSelectorT(0)
{
}

InclProtTightSkim::~InclProtTightSkim()
{
  if (_protonLHSelectorVL != 0) delete _protonLHSelectorVL;
  if (_protonLHSelectorT != 0) delete _protonLHSelectorT;
}


AppResult
InclProtTightSkim::beginJob( AbsEvent* anEvent )
{
  _protonLHSelectorVL = newSelector("PidProtonLHSelector");
  _protonLHSelectorVL->setParmValue("criteria","veryLoose");
  _protonLHSelectorT = newSelector("PidProtonLHSelector");
  _protonLHSelectorT->setParmValue("criteria","tight");

  ErrMsg(routine)<<"begin filter Job"<<endmsg; 
  _nEventsRead = 0;
  _nEventsPassed = 0;
  return AppResult::OK; 
}

AppResult
InclProtTightSkim::event( AbsEvent* anEvent )
{
  bool passThisEvent = false;
 
  HepAList< EventInfo >* infoList=0;
  static const IfdStrKey defaultKey("Default");
  getTmpAList(anEvent, infoList, defaultKey);
  EventInfo* eventInfo = infoList->first();
  HepPoint theBeamSpot = eventInfo->beamSpot();

  BtaCandidate* pCand(0);

  // Keep a count of how many events have been processed 
  _nEventsRead++;

  // Declare P list
  HepAList< BtaCandidate > *pList;

  // Get list of proton
  static const IfdStrKey pLHTightKey("pLHTight");
  getTmpAList( anEvent, pList, pLHTightKey );

  // Get iterator for the list
  HepAListIterator<BtaCandidate> iterP( *pList );  

  while( 0 != ( pCand = iterP() ) ) {
    // if ( _protonLHSelectorT->accept(pCand) ) {
      if(pCand->trkAbsFit()) {
        TrkPocaXY pocaxy(pCand->trkAbsFit()->traj(),0,theBeamSpot);
        HepPoint closest2= pCand->trkAbsFit()->position(pocaxy.fltl1());
        float docaCut = sqrt(pow(closest2.x(),2)+pow(closest2.y(),2)+pow(closest2.z(),2));
        if ( docaCut <= 2 ) {
	  passThisEvent = true;
          break;
        }
      }
    // }
  }


  // Now report whether this event passes the skim or not
  if( passThisEvent == false )
    {
      setPassed( false ); // This event passed the skim
    }
  else
    {
      setPassed( true );  // This event did not pass the skim
      _nEventsPassed++;
    }
  return AppResult::OK; 
}

AppResult
InclProtTightSkim::endJob( AbsEvent* anEvent )
{
  // Print out some statistics at the end
  cout << "InclProtTightSkim:" << endl
       << "Read " << _nEventsRead << endl
       << "Passed " << _nEventsPassed << endl;
  return AppResult::OK; 
}

