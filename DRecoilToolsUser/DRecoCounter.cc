#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "DRecoCounter.hh"

//-------------
// C Headers --
//-------------
#include <assert.h>

//---------------
// C++ Headers --
//---------------
#include <iostream>
#include <math.h>

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "AbsEvent/AbsEvent.hh"
#include "Beta/EventInfo.hh"
#include "Beta/BtaCandidate.hh"

#include "CLHEP/Alist/AList.h"
#include "CLHEP/Alist/AIterator.h"
#include "ErrLogger/ErrLog.hh"
#include "ProxyDict/IfdStrKey.hh"
#include "ProxyDict/IfdKey.hh"
#include "ProxyDict/Ifd.hh"
#include "ProxyDict/IfdHepAListProxy.hh"

#include "BetaMicroAdapter/BtaMicroAdapter.hh"
#include "BtaTupleMaker/BtuUtil.hh"

using std::cout ;
using std::endl ;
using std::ostream ;

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

//----------------
// Constructors --
//----------------
DRecoCounter::DRecoCounter( const char* const theName , const char* const theDescription )
  :        TagFilterModule (  theName, theDescription  )
  ,          counterNumber ( "counterNumber", this, -1 )
  ,               _scalers ( "DRecoCounter"            )


{
  commands()->append( &counterNumber ) ;
}

//--------------
// Destructor --
//--------------
DRecoCounter::~DRecoCounter()
{}

//--------------
// Operations --
//--------------
AppResult DRecoCounter::beginJob( AbsEvent* anEvent )
{ 
  return AppResult::OK;
}

AppResult DRecoCounter::event( AbsEvent* anEvent )
{
  _scalers.sum("# events ") ;

  double counter = counterNumber.value() ;  
  // Retreive the event candidates
  HepAList <HepAList< BtaCandidate > >* DRecoEvtCandidateList = Ifd<HepAList< HepAList<BtaCandidate > > >::get(anEvent,"DRecoEvtCandidateList") ;
  HepAList <BtaCandidate>* DRecoCandidateList = 0 ;
  DRecoCandidateList = Ifd<HepAList<BtaCandidate> >::get(anEvent,"DsToEllNuCandidateList") ;
  if(DRecoCandidateList==0) DRecoCandidateList = Ifd<HepAList<BtaCandidate> >::get(anEvent,"DRecoCandidateList") ;
  if(DRecoEvtCandidateList==0 && DRecoCandidateList==0) ErrMsg(fatal) <<"DRecoCandidateList not found for counter number " << counter << endmsg ;

  int DRecoEvtLength = 0 ;
  int DRecoLength    = 0 ;
  if(DRecoEvtCandidateList!=0) DRecoEvtLength = DRecoEvtCandidateList->length() ;
  if(DRecoCandidateList!=0)    DRecoLength    = DRecoCandidateList->length()    ;
  
  if(DRecoEvtLength>0)
  {
    if(counter> 9.5 && counter<10.5) _scalers.sum("Counter DRecoTagInterface events passed"   ) ;
    if(counter>19.5 && counter<20.5) _scalers.sum("Counter DRecoDstarTag events passed"       ) ;
    if(counter>29.5 && counter<30.5) _scalers.sum("Counter DRecoTagKaon events passed"        ) ;
    if(counter>39.5 && counter<40.5) _scalers.sum("Counter DRecoFragPions events passed"      ) ;
    if(counter>49.5 && counter<50.5) _scalers.sum("Counter DRecoDsStGam events passed"        ) ;
    if(counter>59.5 && counter<60.5) _scalers.sum("Counter DRecoCheckCharge events passed"    ) ;
  }
  
  if(DRecoLength>0)
  {
    if(counter>69.5 && counter<70.5) _scalers.sum("Counter DRecoDsInclusiveFit events passed" ) ;
    if(counter>79.5 && counter<80.5) _scalers.sum("Counter DRecoDsToEllNuFit events passed"   ) ;
  }
  
  return AppResult::OK ;
}

AppResult DRecoCounter::endJob( AbsEvent* anEvent )
{
  // endJob
  ostream& o=cout ;
  _scalers.print(o) ;
  o << endl ;

  return AppResult::OK ;
}
