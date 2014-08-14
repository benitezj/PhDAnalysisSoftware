//--------------------------------------------------------------------------
// File and Version Information:
//
// $Id: InclppbarFilter.cc,v 1.5 2006/11/23 19:05:02 desilva Exp $
//
//--------------------------------------------------------------------------
#include "BaBar/BaBar.hh"
#include "FilterTools/InclppbarFilter.hh"

///////////////
#include "CLHEP/Alist/AList.h"
#include "CLHEP/Alist/AIterator.h"
#include "CLHEP/Matrix/Vector.h"
#include "CLHEP/Vector/LorentzVector.h"
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

InclppbarFilter::InclppbarFilter( const char* const theName, 
			      const char* const theDescription )
  : TagFilterModule( theName, theDescription )
{
}

InclppbarFilter::~InclppbarFilter()
{
}


AppResult
InclppbarFilter::beginJob( AbsEvent* anEvent )
{
  ErrMsg(routine)<<"begin filter Job"<<endmsg;
  _nEventsRead = 0;
  _nEventsPassed = 0;

  return AppResult::OK; 
}

AppResult
InclppbarFilter::event( AbsEvent* anEvent )
{
  // Keep a count of how many events have been processed
  _nEventsRead++;
  // get proton list from pLHVeryLoose: 
  HepAList<BtaCandidate>* origprotonList; // protons list
  static const IfdStrKey pLHVeryLooseKey("pLHVeryLoose"); 
  getTmpAList( anEvent, origprotonList, pLHVeryLooseKey); 

  // make separate lists for protons and anti-protons
  HepAList< BtaCandidate > pList;
  HepAList< BtaCandidate > pbarList;
  //

  HepAListIterator< BtaCandidate > origprotonListIter(*origprotonList);
  BtaCandidate* protonCand;
  while(protonCand = origprotonListIter()){
    if(protonCand->charge()>0) {
      pList.append(protonCand);
    } else {
      pbarList.append(protonCand);
    }
  }

  if ( (pList.length()<=0)||(pbarList.length()<=0) ) { 
   setPassed( false );
  }
  else
  {
   setPassed( true );  // This event did not pass the skim
   _nEventsPassed++;
  }
  return AppResult::OK; 
}

AppResult
InclppbarFilter::endJob( AbsEvent* anEvent )
{
  // Print out some statistics at the end
  cout << "Read in Candidate in InclppbarFilter:" << endl
       << "Read " << _nEventsRead << endl
       << "Passed " << _nEventsPassed << endl;
  return AppResult::OK; 
}

