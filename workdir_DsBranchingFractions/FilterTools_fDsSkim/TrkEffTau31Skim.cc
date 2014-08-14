//--------------------------------------------------------------------------
// File and Version Information:
//      TrkEffTau31Skim.cc
//
// Description:
//
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//     Ian Nugent
//
// Copyright Information:
//      Copyright (C) 2005                          University of Victoria
//
//------------------------------------------------------------------------

#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "FilterTools/TrkEffTau31Skim.hh"

//-------------
// C Headers --
//-------------
#include <assert.h>

//---------------
// C++ Headers --
//---------------
#include <math.h>
#include <string>
#include <map>

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "AbsEnv/AbsEnv.hh"
#include "AbsEvent/AbsEvent.hh"
#include "AbsEvent/AbsEventID.hh"
#include "AbsEvent/getTmpAList.hh"
#include "AbsEventTag/AbsEventTag.hh"
#include "Beta/BtaCandidate.hh"
#include "Beta/BtaMcTruth.hh"
#include "Beta/EventInfo.hh"
#include "BetaCoreTools/BtaCandMap2.hh"
#include "BetaCoreTools/BtaTreeNavigator.hh"
#include "BetaTools/BtaSemilepCand.hh"
#include "CLHEP/Alist/AList.h"
#include "ErrLogger/ErrLog.hh"
#include "GenEnv/GenEnv.hh"
#include "HepTuple/Tuple.h"
#include "HepTuple/HepHistID.hh"
#include "HepTuple/TupleManager.h"
#include "HepTuple/HTValOrderedVector.h"
#include "PDT/Pdt.hh"
#include "PDT/PdtEntry.hh"
#include "PDT/PdtLund.hh"
#include "ProxyDict/IfdStrKey.hh"
#include "ProxyDict/IfdKey.hh"
#include "ProxyDict/Ifd.hh"

// Official PID selectors and related stuff
#include "BetaPid/PidSelector.hh"
#include "BetaPid/PidSelFactoryManager.hh"
#include "BetaPid/PidKaonMicroSelector.hh"
#include "BetaPid/PidKaonLHSelector.hh"
#include "BetaPid/PidMuonMicroSelector.hh"
#include "BetaPid/PidMuonLikeSelector.hh"
#include "BetaPid/PidElectronMicroSelector.hh"
#include "BetaPid/PidLHElectronSelector.hh"
#include "BetaPid/PidProtonLHSelector.hh"
#include "BetaPid/PidPionLHSelector.hh"
#include "BetaTools/BtaOpPidAccessory.hh"
#include "DrcPidUtils/DrcConst.hh"
//#include "BetaPid/PidWeight.hh"

using std::endl;

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

//----------------
// Constructors --
//----------------

TrkEffTau31Skim::TrkEffTau31Skim( const char* const theName, 
				  const char* const theDescription )
  : TagFilterModule( theName, theDescription )
    , _eventInfoList(   "eventInfoList",   this, "Default"  )
    , _CTList(          "CTList",          this, "ChargedTracks")
    , _GTVLList(        "GTVLList",        this, "GoodTracksVeryLoose")
    , _GTLList(         "GTLList",         this, "GoodTracksLoose")
    , _pidTrkList(      "pidTrkList",      this, "ChargedTracks")
    , _numberEvt(0)
    , _numRejectedEvt(0)
    , _numPassedEvt(0)
    , _numPassedChgEvt(0)
    , _numPassedCalNeutEvt(0)
    , _numPassedNeutHadEvt(0)
{
  commands()->append( &_eventInfoList );
  commands()->append( &_CTList);
  commands()->append( &_GTVLList);
  commands()->append( &_GTLList);
  commands()->append( &_pidTrkList);
}

//--------------
// Destructor --
//--------------

TrkEffTau31Skim::~TrkEffTau31Skim( )
{}

//--------------
// Operations --
//--------------

AppResult
TrkEffTau31Skim::beginJob( AbsEvent* anEvent )
{
    ErrMsg(routine)<<"begin Job"<<endmsg;

    _nTrials = 0;
    _cutNChg = 0;
    _cutNCalNeut = 0;
    _cutNNeutHad = 0;
  
  return AppResult::OK;
}

//------------------------------------------------------------

AppResult
TrkEffTau31Skim::event( AbsEvent* anEvent )
{
  _numberEvt++;
  bool result = pass( anEvent );
  setPassed(result);

  return AppResult::OK;
}

//-----------------------------------------------------------
  
AppResult
TrkEffTau31Skim::endJob( AbsEvent* anEvent )
{
  ErrMsg(routine)<<" end Job" << endmsg;

  // Print statistics
  if ( ErrLogging(routine) ) {
    ostream& os = ErrMsg(routine);

    os << endl << "===========================================" << endl
       << " Select candidates selection in " << name()  << endl
       << "===========================================" << endl; 

    os << "---- " << _numberEvt  << "\tevents processed" << endl;
    os << "---- " << _numPassedEvt  << "\tevents passed" << endl;
    os << "---- " << _numPassedChgEvt  << "\tCharged Tracks events passed" << endl;
    os << "---- " << _numPassedCalNeutEvt  << "\tColor Neutral events passed" << endl;
    os << "---- " << _numPassedNeutHadEvt  << "\tNeutral Hadron events passed" << endl;
    os << "---- " << _numRejectedEvt << "\tevents rejected" << endl;
    os << "---- " << _nTrials    << "\ttotal Candidates" << endl;
    if(_numberEvt>0) os << "---- " << ((double) _numPassedEvt)/_numberEvt
		      << "\tFraction" << endl;
    os << endmsg;
  }

  return AppResult::OK;
}




//-------------------------------------------------------------------

bool
TrkEffTau31Skim::pass( AbsEvent* anEvent )
{
    // For convenience later, get a pointer to the event summary info object
    HepAList< EventInfo >* infoList=0;
    getTmpAList(anEvent, infoList,_eventInfoList.value());
    EventInfo* eventInfo = infoList->first();

    HepAList<BtaCandidate>* CTList;
    getTmpAList (anEvent, CTList, _CTList.value());

    HepAList<BtaCandidate>* GTVLList;
    getTmpAList (anEvent, GTVLList, _GTVLList.value());

    HepAList<BtaCandidate>* GTLList;
    getTmpAList (anEvent, GTLList, _GTLList.value());

    //GTVL>=3
    if (GTVLList->length()<3 ) {
      _numRejectedEvt++;
      return false;
    }

    if (CTList->length()>5 ) {
      _numRejectedEvt++;
      return false;
    }

    if(!(Charge_Cut(anEvent,CTList))){
      _numRejectedEvt++;
      return false;
    }

    if(!Topology_Cut(anEvent,eventInfo,CTList)){
      _numRejectedEvt++;
      return false;
    }
    
    
    _numPassedEvt++;
    return true;
}

bool TrkEffTau31Skim::Topology_Cut(AbsEvent* anEvent,EventInfo* eventInfo,
			      HepAList<BtaCandidate>* CandList){
  // particle id maps
  HepAList<BtaCandidate>* pidTrkList;
  getTmpAList (anEvent, pidTrkList, _pidTrkList.value());
  HepAListIterator<BtaCandidate> pidTrkIterator(*pidTrkList);
  IfdStrKey muonKey(std::string("muSelectorsMap"));
  std::map<BtaCandidate*, int>* muSelectorsMap =
    Ifd< std::map<BtaCandidate*, int> >::get(anEvent,muonKey);
  IfdStrKey elecKey(std::string("eSelectorsMap"));
  std::map<BtaCandidate*, int>* eSelectorsMap =
    Ifd<std::map<BtaCandidate*, int> >::get(anEvent,elecKey);
  
  // get a boost vector
  BtaCandidate* cand;
  HepAListIterator<BtaCandidate> microCand(*CandList);
  
  // calculates mometum of each track in cm frame
  HepAList<HepLorentzVector> P4cmList;
  int ntrk=0;
  int p1trkptr = -1;
  int p1prong[10], p1prongPid[10], i;
  bool p1prongE[10];
  for (i=0;i < 10;i++){ 
    p1prong[i]=0;
    p1prongPid[i]=0;
    p1prongE[i]=false;
  }
  while (cand = microCand()){
    //Find it the trk is a electron or muon  
    bool foundIt=false;
    pidTrkIterator.rewind();
    BtaCandidate* pidTrk(0);
    while(pidTrk = pidTrkIterator()) {
      if(pidTrk->uid() == cand->uid()) {
	foundIt = true;
	break;
      }
    }
    if(foundIt){
      if(muSelectorsMap!=0 || eSelectorsMap!=0) {
	p1prongPid[ntrk]=1;
      }
    }
    //Get the momentum vector an boost it into the cm
    HepLorentzVector* P4cm = new HepLorentzVector(cand->p4());
    P4cm->boost(-(eventInfo->cmFrame().boostVector()));
    P4cmList.append(P4cm);
    //determine if lepton has Etau*0.20<Elepton<Etau*0.80 in cm
    double momentum=sqrt(pow(P4cm->px(),2.0)+pow(P4cm->py(),2.0)+pow(P4cm->pz(),2.0));
    if(momentum>(gblEnv->getPep()->pepBeams()->energyCM()/2)*0.20 &&
       momentum<(gblEnv->getPep()->pepBeams()->energyCM()/2)*0.80){
      p1prongE[ntrk]=true;
    }
    ntrk++;
    if(ntrk>10){
      break;
    }
  }
  
  // performs 1 vs nprong selection 
  if(ntrk<=10 && ntrk>=2){
    // determines if there is a track which is more than 90 from 
    // all other track and 90 from all neutrals E>100MeV
    p1trkptr=-1;
    
    for (i=0;i < P4cmList.length(); i++) {
      Hep3Vector Vector1 = Hep3Vector(*P4cmList[i]).unit(); 
      for (int j=0;j<P4cmList.length();j++) {
	Hep3Vector Vector2 = Hep3Vector(*P4cmList[j]).unit(); 
	if (Vector2.dot(Vector1)<0.0) {
	  p1prong[i]++;
	}
      }
    }
    
    // preforms n-prong selection cut
    for(i=0; i<10; i++){
      if(p1prong[i]==ntrk-1 && p1prongPid[i]!=0 && p1prongE[i]){
	return true;
      }
    }
  }
  return false;
}




bool TrkEffTau31Skim::Charge_Cut(AbsEvent* anEvent,
				 HepAList<BtaCandidate>* CandList){
    BtaCandidate* cand;
    HepAListIterator<BtaCandidate> microCand(*CandList);
   
    int ntrk_=0;
    int charge=0;
    while (cand = microCand()){
	ntrk_++;
	charge+=(int)cand->charge();
    }
    if(((ntrk_==3 || ntrk_==5) && (charge==1||charge==-1)) 
	 || (ntrk_==4 && charge==0)){
      return true;
    }
    return false;
}








