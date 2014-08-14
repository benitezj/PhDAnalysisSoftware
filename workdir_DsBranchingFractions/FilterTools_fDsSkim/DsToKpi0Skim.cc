#include "BaBar/BaBar.hh"
#include "FilterTools/DsToKpi0Skim.hh"

#include "CLHEP/Alist/AList.h"
#include "CLHEP/Alist/AIterator.h"
#include "AbsEnv/AbsEnv.hh"
#include "AbsEvent/AbsEvent.hh"
#include "AbsEvent/getTmpAList.hh"
#include "ErrLogger/ErrLog.hh"
#include "AbsEventTag/AbsEventTag.hh"
#include "Beta/EventInfo.hh"
#include "BetaCoreTools/BtaBooster.hh"
#include "BetaCoreTools/BtaBVariables.hh" 
#include "BetaCoreTools/BtaOpAdd4.hh"
#include "PDT/Pdt.hh"

using std::cout;
using std::endl;

DsToKpi0Skim::DsToKpi0Skim( const char* const theName, 
			      const char* const theDescription )
  : TagFilterModule( theName, theDescription )
  , _DsToKpi0ListName( "DsToKpi0ListName", this, "DsstarToDsgamma" )
  , _DsTopipi0ListName( "DsTopipi0ListName", this, "DsstarToDsgamma" )
  , _DsToKsKListName( "DsToKsKListName", this, "DsstarToDsgamma" )
  , _DsToKspiListName( "DsToKspiListName", this, "DsstarToDsgamma" )
{
  commands()->append( &_DsToKpi0ListName );
  commands()->append( &_DsTopipi0ListName );
  commands()->append( &_DsToKsKListName );
  commands()->append( &_DsToKspiListName );
}

DsToKpi0Skim::~DsToKpi0Skim()
{
}


AppResult
DsToKpi0Skim::beginJob( AbsEvent* anEvent )
{
  ErrMsg(routine)<<"begin filter Job"<<endmsg; 
  _nEventsRead = 0;
  _nEventsPassed = 0;

  return AppResult::OK;
}

AppResult
DsToKpi0Skim::event( AbsEvent* anEvent )
{
  bool passThisEvent = false;
  _nEventsRead++;

  // Here is my list (D_s*+)
  HepAList<BtaCandidate>* theDsstarDsToKpi0List = 
    locateList(anEvent, _DsToKpi0ListName.valueString());

  HepAList<BtaCandidate>* theDsstarDsTopipi0List =
    locateList(anEvent, _DsTopipi0ListName.valueString());
  
  HepAList<BtaCandidate>* theDsstarDsToKsKList =
    locateList(anEvent, _DsToKsKListName.valueString());

  HepAList<BtaCandidate>* theDsstarDsToKspiList =
    locateList(anEvent, _DsToKspiListName.valueString());

  // Has it passed?
  if(theDsstarDsToKpi0List->length()>0 || 
     theDsstarDsTopipi0List->length()>0 ||
     theDsstarDsToKsKList->length()>0 ||
     theDsstarDsToKspiList->length()>0){
    passThisEvent = true; 
  }

  if( passThisEvent == false )
    {
      setPassed( false ); // This event did not pass the skim
    }
  else
    {
      setPassed( true );  // This event passed the skim
      _nEventsPassed++;
    }

  return AppResult::OK;
}

AppResult
DsToKpi0Skim::endJob( AbsEvent* anEvent )
{
  // Print out some statistics at the end
  cout << "DsToKpi0Skim:" << endl
       << "Read " << _nEventsRead << endl
       << "Passed " << _nEventsPassed << endl;
  
  return AppResult::OK;
}


HepAList<BtaCandidate>*
DsToKpi0Skim::locateList(AbsEvent* anEvent, const std::string& listKey)
{
  HepAList<BtaCandidate>* list = 
    Ifd<HepAList< BtaCandidate > >::get(anEvent, listKey.c_str());
  if (list == 0)
    ErrMsg(fatal) << "Could not locate list " << listKey.c_str()
		  << ". This is likely to be a configuration error." << endmsg;
  
  return list;
}
