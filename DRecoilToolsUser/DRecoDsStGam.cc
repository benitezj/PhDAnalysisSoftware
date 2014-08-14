
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "DRecoDsStGam.hh"

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
#include "Beta/EventInfo.hh"
#include "BetaCoreTools/BtaOpMakeTree.hh"
#include "CLHEP/Alist/AList.h"
#include "CLHEP/Alist/AIterator.h"
#include "ErrLogger/ErrLog.hh"
#include "PDT/Pdt.hh"
#include "ProxyDict/IfdStrKey.hh"
#include "ProxyDict/IfdKey.hh"
#include "ProxyDict/Ifd.hh"
#include "ProxyDict/IfdHepAListProxy.hh"
#include "BetaCoreTools/BtaBooster.hh"

#include "BetaMicroAdapter/BtaCalQual.hh"
#include "BtaTupleMaker/BtuClosestBumpDist.hh"
#include "BtaTupleMaker/BtuClosestTrkAtEMC.hh"



using std::cout;
using std::endl;
using std::ostream;

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

//----------------
// Constructors --
//----------------
DRecoDsStGam::DRecoDsStGam( const char* const theName,const char* const theDescription )
  : TagFilterModule( theName, theDescription )
  , GamList( "GamList", this, "GoodPhotonLoose" )
  , GamEnergyCutLo( "GamEnergyCutLo", this, .1)
  , GamEnergyCutHi( "GamEnergyCutHi", this, 2.)
  , GamAngleCut( "GamAngleCut",this,.0)
  , Pi0VetoList("Pi0VetoList",this,"pi0TightMassNoMomentumCut")// must use lists without mass constrain (see below)
  , EtaVetoList("EtaVetoList",this,"etaggDefaultNoMass")
  , GamLabAngleCut("GamLabAngleCut",this,1.)
  , GamLatCutLo("GamLatCutLo",this,0.)
  , GamLatCutHi("GamLatCutHi",this,100.)
  , GamBumpDistCut("GamBumpDistCut",this,0.)
  , GamTrkDistCut("GamTrkDistCut",this,0.)
  , _scalers("DRecoDsStGam")
{
  commands()->append( &GamList );
  commands()->append( &GamEnergyCutLo);
  commands()->append( &GamEnergyCutHi); 
  commands()->append( &GamAngleCut);
  commands()->append( &Pi0VetoList);
  commands()->append( &EtaVetoList); 
  commands()->append( &GamLabAngleCut);
  commands()->append( &GamLatCutLo);
  commands()->append( &GamLatCutHi); 
  commands()->append( &GamBumpDistCut);
  commands()->append( &GamTrkDistCut);  
}

//--------------
// Destructor --
//--------------
DRecoDsStGam::~DRecoDsStGam( )
{}

//--------------
// Operations --
//--------------
AppResult DRecoDsStGam::beginJob( AbsEvent* anEvent )
{ 
  return AppResult::OK;
}

AppResult DRecoDsStGam::event( AbsEvent* anEvent )
{
  _scalers.sum("# events ");
 
  HepAList< HepAList<BtaCandidate> >* DRecoEvtCandidateList = Ifd<HepAList< HepAList<BtaCandidate> > >::get(anEvent,"DRecoEvtCandidateList");  
  if(DRecoEvtCandidateList == 0)return AppResult::OK;
  if(DRecoEvtCandidateList->length()==0) return AppResult::OK;//nothing to do

  // Create 
  HepAList< HepAList<BtaCandidate> >  TempEvtCandList ;
  HepAListIterator< HepAList<BtaCandidate> > ListIter(*DRecoEvtCandidateList);
  while (HepAList<BtaCandidate> * candlist = ListIter()){
    TempEvtCandList += new HepAList<BtaCandidate>(*candlist);
  }

  HepAListDeleteAll(*DRecoEvtCandidateList);  
  
  // pointer to the event summary info object
  HepAList< EventInfo >* eventInfoListPtr =  Ifd<HepAList< EventInfo > >::get(anEvent,"Default");
  if (eventInfoListPtr == 0){
    ErrMsg(fatal) << "Could not locate event info list in the event.This is likely to be a configuration error." << endmsg;
  } 
  EventInfo* eventInfo = eventInfoListPtr->first();
  if (eventInfo == 0) {
    ErrMsg(fatal) << "Nothing in eventInfoList...aborting" << endmsg;
  }
  BtaBooster cmbooster(eventInfo->cmFrame());


  //Get the veto lists (must be clean already)
  HepAList<BtaCandidate>* EventPi0VetoList = Ifd<HepAList< BtaCandidate > >::get(anEvent,Pi0VetoList.value());     
  HepAList<BtaCandidate>* EventEtaVetoList = Ifd<HepAList< BtaCandidate > >::get(anEvent,EtaVetoList.value()); 


  //initialize the gamma quality cuts
  BtuClosestBumpDist bumpDistCal;
  bumpDistCal.getEmcBumpList(anEvent,"Default");    
 
  BtuClosestTrkAtEMC trkAtEmcCal;
  trkAtEmcCal.getTrkList(anEvent,"ChargedTracks");



  //Get The signal gamma list
  HepAList<BtaCandidate>  CleanGamList ;
  HepAList<BtaCandidate>* EventGamList  = Ifd<HepAList< BtaCandidate > >::get(anEvent, GamList.value());
  if (EventGamList == 0)  ErrMsg(fatal) << GamList.value() << " list not found" << endmsg;    
  HepAListIterator<BtaCandidate> GamIter(*EventGamList);
  while (BtaCandidate* gamcand = GamIter()){
    _scalers.sum("total gammas");
    
    if(EventPi0VetoList)
      if(gamcand->overlaps(*EventPi0VetoList))continue;
    _scalers.sum("gammas after pi0 veto"); 
    if(EventEtaVetoList)
      if(gamcand->overlaps(*EventEtaVetoList))continue;
    _scalers.sum("gammas after eta veto"); 

    if(gamcand->p3().z()/gamcand->p()>GamLabAngleCut.value())continue;
    _scalers.sum("gammas after Lab angle cut"); 

    const BtaCalQual* cQual =gamcand->getMicroAdapter()->getCalQual();
    if(!cQual) ErrMsg(fatal) << "gamma CalQual not found...aborting" << endmsg;
    if(cQual->lateralMoment()<GamLatCutLo.value()||GamLatCutHi.value()<cQual->lateralMoment())continue;
    _scalers.sum("gammas after Lat cut"); 

    if(bumpDistCal.value(gamcand)<GamBumpDistCut.value())continue;
    _scalers.sum("gammas after bump dist cut"); 

    trkAtEmcCal.findClosestTrk(gamcand);
    if(trkAtEmcCal.distance()<GamTrkDistCut.value())continue;
    _scalers.sum("gammas after track dist cut");     

    if(gamcand->energy()<GamEnergyCutLo.value()) continue;    
    if(gamcand->energy()>GamEnergyCutHi.value()) continue;    
    _scalers.sum("gammas after energy cut");     

    CleanGamList+=gamcand;
  }

     
  HepAListIterator< HepAList<BtaCandidate> > TempListIter(TempEvtCandList);
  while (HepAList<BtaCandidate> * tmpcandlist =TempListIter()){
    _scalers.sum("initial lists");
	  
    HepAListIterator<BtaCandidate> TagDIter(*tmpcandlist);
    BtaCandidate* tagd=TagDIter();//should be the first one
    BtaCandidate boosttagd=cmbooster.boostTo(*tagd);
 
    //get the input list
    HepAListIterator<BtaCandidate> GamIter(CleanGamList);
    while (BtaCandidate* gamcand = GamIter()){
      _scalers.sum("gammas");

      if(gamcand->overlaps(*tmpcandlist))continue; 
      _scalers.sum("gammas after overlap cut");
      
      BtaCandidate boostgam=cmbooster.boostTo(*gamcand);
      if(((boosttagd.p3()).unit())*((boostgam.p3()).unit())>GamAngleCut.value())continue;
      _scalers.sum("gammas after angle cut");      


      gamcand = new BtaCandidate(*gamcand);
      gamcand->removeConstraint(BtaConstraint::Beam);
      //setBeamConstraint(*gamcand,eventInfo);//not necessary because will come from Ds* which is a resonance
      gamcand->setType(Pdt::lookup( PdtPdg::gamma)); 


      HepAList<BtaCandidate>* newcandlist= new HepAList<BtaCandidate>;    
      HepAListIterator< BtaCandidate > candIter(*tmpcandlist);
      while(BtaCandidate * cand = candIter())
	*newcandlist+=new BtaCandidate(*cand);
      *newcandlist  += gamcand;
      *DRecoEvtCandidateList += newcandlist;
      _scalers.sum("final lists");
         
    }   
    
    HepAListDeleteAll(*tmpcandlist);
  }   

         

  HepAListDeleteAll(TempEvtCandList);

  return AppResult::OK;
}


AppResult DRecoDsStGam::endJob( AbsEvent* anEvent )
{
  ostream& o=cout;
  _scalers.print(o);
  o << endl;

  return AppResult::OK;
}




