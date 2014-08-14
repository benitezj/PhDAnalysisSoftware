
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "DRecoCleanUp.hh"

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
#include "Beta/BtaCandidate.hh"
#include "CLHEP/Alist/AList.h"
#include "CLHEP/Alist/AIterator.h"
#include "ErrLogger/ErrLog.hh"
#include "ProxyDict/Ifd.hh"

using std::cout;
using std::endl;
using std::ostream;

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

//----------------
// Constructors --
//----------------
DRecoCleanUp::DRecoCleanUp( const char* const theName,const char* const theDescription )
  : TagFilterModule( theName, theDescription )
  , _scalers("DRecoCleanUp")
{

}

//--------------
// Destructor --
//--------------
DRecoCleanUp::~DRecoCleanUp()
{}

//--------------
// Operations --
//--------------
AppResult DRecoCleanUp::beginJob( AbsEvent* anEvent )
{ 
  return AppResult::OK;
}

AppResult DRecoCleanUp::event( AbsEvent* anEvent )
{
  _scalers.sum("#  events ");

  //retreive the event candidates
  HepAList< HepAList<BtaCandidate> >* DRecoEvtCandidateList = Ifd<HepAList< HepAList<BtaCandidate> > >::get(anEvent,"DRecoEvtCandidateList");
  if (DRecoEvtCandidateList == 0)  ErrMsg(fatal) <<"DRecoEvtCandidateList  not found" << endmsg;   
  if (DRecoEvtCandidateList->length()==0)return AppResult::OK;

  HepAListIterator< HepAList<BtaCandidate> > ListIter(*DRecoEvtCandidateList); 
  while (HepAList<BtaCandidate> * candlist = ListIter()){     
    _scalers.sum(" total");	 
    
    //delete all BtaCandidates    
    HepAListDeleteAll(*candlist);
  }

  //delete all lists 
  HepAListDeleteAll(*DRecoEvtCandidateList);

  return AppResult::OK;
}


AppResult DRecoCleanUp::endJob( AbsEvent* anEvent )
{
  ostream& o=cout;
  _scalers.print(o);
  o << endl;

  return AppResult::OK;
}




