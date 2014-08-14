//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: DsppbarFilter.cc,v 1.4 2006/11/23 19:05:01 desilva Exp $
//
//--------------------------------------------------------------------------
#include "BaBar/BaBar.hh"
#include "FilterTools/DsppbarFilter.hh"

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

using std::cout;
using std::endl;
using std::setbase;

DsppbarFilter::DsppbarFilter( const char* const theName, 
			      const char* const theDescription )
  : TagFilterModule( theName, theDescription )
{
}

DsppbarFilter::~DsppbarFilter()
{
}


AppResult
DsppbarFilter::beginJob( AbsEvent* anEvent )
{
  ErrMsg(routine)<<"begin filter Job"<<endmsg; 
  _nEventsRead = 0;
  _nEventsPassed = 0;
  return AppResult::OK;
}

AppResult
DsppbarFilter::event( AbsEvent* anEvent )
{

  // Keep a count of how many events have been processed 
  _nEventsRead++;

  // Declare Ds and proton lists
  HepAList< BtaCandidate > *protonList;
  HepAList< BtaCandidate > *DsList;

  // Get lists of muons and charged D's
  static const IfdStrKey pLHVeryLooseKey("pLHVeryLoose");
  getTmpAList( anEvent, protonList, pLHVeryLooseKey );
  static const IfdStrKey keyDsDefault("DsDefault");
  getTmpAList( anEvent, DsList, keyDsDefault );

  int passedevent(1);
  
  if(DsList->length()<1) {
    if( _verbose.value() ) { 
      cout << "Failed DsppbarFilter: nDs < 1" << endl; 
    } 

    passedevent=0;
  }



  if(protonList->length()<2) {
    if( _verbose.value() ) { 
      cout << "Failed DsppbarFilter: nProton < 2" << endl; 
    } 

    passedevent=0;
  }


  /////////////////////////////////

  BtaCandidate *trk(NULL);			

  if(passedevent){
    bool allthesamecharge(1);
    int firstcharge;
    HepAListIterator<BtaCandidate> itProton(*protonList);
    trk = itProton();
    firstcharge = (int)trk->charge();
    while( trk = itProton() ) {
      if(trk->charge() != firstcharge) {allthesamecharge=0; break;}
    }
    
    if(allthesamecharge) {
      if( _verbose.value() ) {
	cout << "Failed MicroFilter: all protons have the same charge" << endl;
      }
      //    return false;
      passedevent=0;
    }
  }

  if (passedevent){
    _nEventsPassed++;
    setPassed(1);
  }else   setPassed(0);
  
  return AppResult::OK;
}

AppResult
DsppbarFilter::endJob( AbsEvent* anEvent )
{
  // Print out some statistics at the end
  cout << "DsppbarFilter:" << endl
       << "Read " << _nEventsRead << endl
       << "Passed " << _nEventsPassed << endl;
  return AppResult::OK;
}

