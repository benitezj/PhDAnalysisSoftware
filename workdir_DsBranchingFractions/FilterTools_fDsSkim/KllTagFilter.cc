//--------------------------------------------------------------------------
// File and Version Information:
//
//      $Id: KllTagFilter.cc,v 1.13 2005/07/28 22:54:22 fnc Exp $
//
// Description:
//      Class KllTagFilter - tag filtering for K(K*)ll analysis
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Natalia Kuznetsova                Original author
//
// Copyright Information:
//      Copyright (C) 2000                UCSB
//
//------------------------------------------------------------------------
#include"BaBar/BaBar.hh"
#include <iomanip>
//-----------------------
// This Class's Header --
//-----------------------            
#include "FilterTools/KllTagFilter.hh"
//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "TagData/TagTransient.hh"
#include "TagData/TagTransientBoolIter.hh"
#include "TagData/TagTransientIntIter.hh"
#include "TagData/TagTransientFloatIter.hh"

#include "AbsEnv/AbsEnv.hh"
#include "GenEnv/GenEnv.hh"
#include "HepTuple/TupleManager.h"
#include "HepTuple/Histogram.h"

#include "ErrLogger/ErrLog.hh"

#include "HepTuple/Tuple.h"
using std::cout;
using std::endl;

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------
 
//----------------
// Constructors --
//----------------                                   


KllTagFilter::KllTagFilter( const char* const theName, 
			const char* const theDescription )
  : TagFilterModule( theName, theDescription ) 
  , _electronPCut("electronPCut", this, 0.3)
  , _muonPCut("muonPCut", this, 0.7)
  , _R2AllCut("R2AllCut", this, 0.5)
  , _NChargedTrackCut("NChargedTrackCut", this, 4)
  , _NLepCut("NLepCut",this,2)
  , _makeNtuple("makeNtuple", this, false)
  , _passAll("passAll", this, false)
  , _failAll("failAll", this, false)
{ 
   commands()->append( & _passAll ) ;
   commands()->append( & _failAll ) ;
   commands()->append( & _electronPCut ) ;
   commands()->append( & _muonPCut ) ;
   commands()->append( & _R2AllCut ) ;
   commands()->append( & _NChargedTrackCut ) ;
   commands()->append( & _NLepCut );
   commands()->append( & _makeNtuple ) ;
}
		
//--------------
// Destructor --
//--------------
KllTagFilter::~KllTagFilter() 
{ 
}


AppResult KllTagFilter::beginJob( AbsEvent*  anEvent ) {

  if ( _verbose.value() ) cout << name() << ": begin job" << endl;

  _nReadEvents = 0;
  _nPassed = 0;

  HepTupleManager* manager = gblEnv->getGen()->ntupleManager();
  

  if (_makeNtuple.value()) {
     ErrMsg(routine) << "KllTagFilter: will book an ntuple " << endmsg ;
     _tagNtuple = manager->ntuple("Kll tag ntuple") ;            
  }

  return AppResult::OK;
}

//--------------
// Operations --
//--------------
              
AppResult KllTagFilter::event( AbsEvent* anEvent ) {

  _nReadEvents++;

  if (_verbose.value()){
    ErrMsg(routine) << "KllTagFilter: processing " 
		    << _nReadEvents << endmsg;
  }

  if (_verbose.value()&&(_nReadEvents%1000==0)) {
    ErrMsg(routine) << " KllTagFilter:event   " 
	 << _nReadEvents << endmsg;
  }
  bool  status;
  bool passed=false;

  // Build the tag accessor using the base class
  TagFilterModule::event( anEvent );
  
  if ( ! tag() ) {
    ErrMsg(error) << " KllTagFilter::event *** Error! "
		  << ": No tag! " << endmsg;
    setPassed(false);
    return AppResult::OK;
  }

  int nChgTrk;
  float r2All;
  float pE1,pE2,pMu1,pMu2;

  bool l3Dch,l3Emc,muHad;


  status=tag()->getInt(nChgTrk,"nTracks");
  status&=tag()->getFloat(r2All,"R2All");
  status&=tag()->getFloat(pE1,"elecLoose1l");
  status&=tag()->getFloat(pE2,"elecLoose2l");
  status&=tag()->getFloat(pMu1,"muonLoose1l");
  status&=tag()->getFloat(pMu2,"muonLoose2l");
  status&=tag()->getBool(l3Dch,"L3OutDch");
  status&=tag()->getBool(l3Emc,"L3OutEmc");
  status&=tag()->getBool(muHad,"BGFMultiHadron");

  if (!status){
    ErrMsg(error) << " KllTagFilter::tagbit missing:" << endmsg;

    if (!tag()->getInt(nChgTrk,"nTracks")){
      ErrMsg(error) <<"nTracks"<<endmsg;
    }

    if (!tag()->getFloat(r2All,"R2All")){
      ErrMsg(error) << "R2All"<<endmsg;
    }

    if (!tag()->getFloat(pE1,"elecLoose1l")){
      ErrMsg(error) << "elecLoose1l"<<endmsg;
    }

    if (!tag()->getFloat(pE2,"elecLoose2l")){ 
      ErrMsg(error) << "elecLoose2l"<<endmsg;
    }

    if (!tag()->getFloat(pMu1,"muonLoose1l")){ 
      ErrMsg(error) << "muonLoose1l"<<endmsg;
    }

    if (!tag()->getFloat(pMu2,"muonLoose2l")){ 
      ErrMsg(error) << "muonLoose2l"<<endmsg;
    }

    if (!tag()->getBool(l3Dch,"L3OutDch")){
      ErrMsg(error) << "L3OutDch"<<endmsg;
    }

    if (!tag()->getBool(l3Emc,"L3OutEmc")){
      ErrMsg(error) << "L3OutEmc"<<endmsg;
    }

    if (!tag()->getBool(muHad,"BGFMultiHadron")){ 
      ErrMsg(error) << "BGFMultiHadron"<<endmsg;
    }

    setPassed(false);
    return AppResult::OK;
  }

  int nElec=0;
  int nMuon=0;

  if (fabs(pE1)>_electronPCut.value()) nElec++; 
  if (fabs(pE2)>_electronPCut.value()) nElec++; 

  if (fabs(pMu1)>_muonPCut.value()) nMuon++; 
  if (fabs(pMu2)>_muonPCut.value()) nMuon++; 

  int nLep=nElec+nMuon;
  
  //first require that L3 triggered event (for MC primarily)
  passed=(l3Dch||l3Emc);

  //require BGFMultiHadron
  passed=passed&&muHad;

  //require charged tracks
  passed=passed&&(nChgTrk>=_NChargedTrackCut.value());

  //require R2All cut
  passed=passed&&(r2All<=_R2AllCut.value());

  //require 2 leptons
  //  passed=passed&&(nLep>=2);
  passed=passed&&(nLep>=_NLepCut.value());

  setPassed( passed );  

  if (passed) _nPassed++;

  if (_makeNtuple.value()) {

    int tmpMuHa=0;
    tmpMuHa=muHad?1:-1;

    int tmpL3Dch=0;
    tmpL3Dch=l3Dch?1:-1;

    int tmpL3Emc=0;
    tmpL3Emc=l3Emc?1:-1;

    _tagNtuple->column("R2All",r2All);
    _tagNtuple->column("MuHa",(float)tmpMuHa);
    _tagNtuple->column("L3Dch",(float)tmpL3Dch);
    _tagNtuple->column("L3Emc",(float)tmpL3Emc);
    _tagNtuple->column("nElec",(float)nElec);
    _tagNtuple->column("nMuon",(float)nMuon);
    _tagNtuple->column("nLep",(float)nLep);
    _tagNtuple->column("nChgTrk",(float)nChgTrk);

    _tagNtuple->dumpData();
 
  } // ntuple.
                                                                                                 

  return AppResult::OK;

}

AppResult KllTagFilter::endJob( AbsEvent* anEvent ) {

  ErrMsg(routine) << name()  << " end job: " << _nPassed << " out of " 
       << _nReadEvents << " passed" << endmsg;
  
  return AppResult::OK;
}

// clone
//
AppModule* 
KllTagFilter::clone(const char* cloneName)
{
  return new KllTagFilter(cloneName, "clone of KllTagFilter");
}


