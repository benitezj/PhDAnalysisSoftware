
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "DRecoTagDump.hh"

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
#include "BetaCoreTools/BtaOpMakeTree.hh"
#include "CLHEP/Alist/AList.h"
#include "CLHEP/Alist/AIterator.h"
#include "ErrLogger/ErrLog.hh"
#include "PDT/Pdt.hh"
#include "ProxyDict/IfdStrKey.hh"
#include "ProxyDict/IfdKey.hh"
#include "ProxyDict/Ifd.hh"
#include "ProxyDict/IfdHepAListProxy.hh"

#include "TrkBase/TrkAbsFit.hh"
#include "VtxBase/BtaAbsVertexer.hh"
#include "VtxFitter/VtxFitterOper.hh"
#include "VtxBase/VtxAbsAlgorithm.hh"
#include "Beta/BtaAbsVertex.hh"
#include "ProbTools/probab.hh"
#include "BetaCoreTools/BtaBooster.hh"

#include "TrkBase/TrkRecoTrk.hh"

#include "BetaCoreTools/BtaMcAssoc.hh"
#include "BtaTupleMaker/BtuUtil.hh"

#include "BetaCoreTools/BtaPrintTree.hh"

#include "BbrGeom/BbrVectorErr.hh"

using std::cout;
using std::endl;
using std::ostream;

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

//----------------
// Constructors --
//----------------
DRecoTagDump::DRecoTagDump( const char* const theName,const char* const theDescription )
  : TagFilterModule( theName, theDescription )
  , _scalers("DRecoTagDump")
{
}

//--------------
// Destructor --
//--------------
DRecoTagDump::~DRecoTagDump()
{}

//--------------
// Operations --
//--------------
AppResult DRecoTagDump::beginJob( AbsEvent* anEvent )
{ 
  return AppResult::OK;
}

AppResult DRecoTagDump::event( AbsEvent* anEvent )
{
  _scalers.sum("#  events ");

  //create evt list and add to event to be filled later
  HepAList<BtaCandidate>* DRecoTagList = new HepAList<BtaCandidate>;
  if (DRecoTagList == 0) ErrMsg(fatal) << "Failed to create a DRecoTagList list"	<< endmsg;
  IfdHepAListProxy<BtaCandidate>* proxy = new IfdHepAListProxy<BtaCandidate>(DRecoTagList);
  if(proxy == 0) ErrMsg(fatal) << "Failed to create DRecoTagList list proxy" << endmsg;
  if( !Ifd<HepAList<BtaCandidate> >::put(anEvent, proxy,"DRecoTagList"))
    ErrMsg(fatal) << "failed to add DRecoTagList list to event using proxy" << endmsg; 

  //retreive the event candidates
  HepAList< HepAList<BtaCandidate> >* DRecoEvtCandidateList = Ifd<HepAList< HepAList<BtaCandidate> > >::get(anEvent,"DRecoEvtCandidateList");
  if (DRecoEvtCandidateList == 0)  ErrMsg(fatal) <<"DRecoEvtCandidateList  not found" << endmsg;   

  HepAListIterator< HepAList<BtaCandidate> > ListIter(*DRecoEvtCandidateList); 

  while (HepAList<BtaCandidate> * candlist = ListIter()){     
    _scalers.sum("initial lists");   
    HepAListIterator<BtaCandidate> dauIter(*candlist);  
    BtaCandidate* rectagD=dauIter();
    if(!rectagD)
      ErrMsg(fatal) << "Tag not found aborting" << endmsg;  
    //gjackson
    if ( DRecoTagList->length() <= 200 ) {
      *DRecoTagList += new BtaCandidate(*rectagD);
      _scalers.sum("final lists");
    }
  }
  return AppResult::OK;
}

AppResult DRecoTagDump::endJob( AbsEvent* anEvent )
{
  ostream& o=cout;
  _scalers.print(o);
  o << endl;

  return AppResult::OK;
}




