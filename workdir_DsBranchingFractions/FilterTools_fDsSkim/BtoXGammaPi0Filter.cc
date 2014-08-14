//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: BtoXGammaPi0Filter.cc,v 1.7 2006/10/03 00:36:07 desilva Exp $
//
// Description:
//	An example module that filters on the transient tag.
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Minghui Lu- based on Original template
//	by Charles Young
//
// Copyright Information:
//	Copyright (C) 1998      SLAC //
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "FilterTools/BtoXGammaPi0Filter.hh"

//-------------
// C Headers --
//-------------
#include <assert.h>
extern "C" {
}

//---------------
// C++ Headers --
//---------------

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "BetaCoreTools/BtaOpAdd4.hh"
#include "CLHEP/Alist/AList.h"
#include "CLHEP/Alist/AIterator.h"
#include "AbsEnv/AbsEnv.hh"
#include "AbsEvent/AbsEvent.hh"
#include "AbsEvent/getTmpAList.hh"
#include "ErrLogger/ErrLog.hh"
#include "AbsEventTag/AbsEventTag.hh"
#include "BetaCoreTools/BtaOpAdd4.hh"
#include "Beta/EventInfo.hh"
#include "BetaCoreTools/BtaBooster.hh"
#include "PDT/Pdt.hh"
#include "PDT/PdtEntry.hh"
#include "ProxyDict/IfdHepAListProxy.hh"

BtoXGammaPi0Filter::BtoXGammaPi0Filter( const char* const theName, 
					const char* const theDescription )
  : TagFilterModule( theName, theDescription )
  , _eventInfoList( "eventInfoList", this, "Default" )
  , _outputPi0CandList("outputPi0Candlist",this,"bsgPi0CandList")
  , _outputEtaCandList("outputEtaCandlist",this,"bsgEtaCandList")
  , _photonList("photonList",this,"GoodPhotonLoose")
  , _maxR2( "maxR2", this, 0.9 )
  , _minE1( "minE1", this,  1.0 )
  , _maxE1( "maxE1", this,  3.5 )
  , _minNGtl( "minNGTL", this, 3 )
  , _heGammaELow ("heGammaELow", this, 1.0)
  , _pi0GamELow ("pi0GamELow", this, 0.030)
  , _pi0MassLow ("pi0MassLow", this, 0.05)
  , _pi0MassHigh ("pi0MassHigh", this, 0.25 )
  , _pi0EnergyLow ("pi0EnergyLow", this, 1.0)
  , _pi0EnergyHigh ("pi0EnergyHigh", this, 3.5 )
  , _etaGamELow ("etaGamELow", this, 0.075)
  , _etaMassLow ("etaMassLow", this, 0.4)
  , _etaMassHigh ("etaMassHigh", this, 0.7)
  , _etaEnergyLow ("etaEnergyLow", this, 1.0)
  , _etaEnergyHigh ("etaEnergyHigh", this, 3.5 )
  
{
  commands()->append(&_eventInfoList);
  commands()->append( &_outputPi0CandList);
  commands()->append( &_photonList);
  commands()->append( &_maxR2 );
  commands()->append( &_minE1 );
  commands()->append( &_maxE1 );
  commands()->append( &_minNGtl );
  commands()->append( &_pi0GamELow);
  commands()->append( &_pi0MassLow);
  commands()->append( &_pi0MassHigh);
  commands()->append( &_pi0EnergyLow);
  commands()->append( &_pi0EnergyHigh);
  commands()->append( &_etaGamELow);
  commands()->append( &_etaMassLow);
  commands()->append( &_etaMassHigh);
  commands()->append( &_etaEnergyLow);
  commands()->append( &_etaEnergyHigh);
}
		
//--------------
// Destructor --
//--------------
BtoXGammaPi0Filter::~BtoXGammaPi0Filter()
{
}

//-------------
// Methods   --
//-------------


AppResult BtoXGammaPi0Filter::beginJob( AbsEvent*  anEvent )
{
  ErrMsg(routine) << ": begin job" << endmsg;
  
  // initialize event counters
  _nReadEvents = 0;
  _nEventsPassed = 0;
  return AppResult::OK;
}


AppResult BtoXGammaPi0Filter::event( AbsEvent* anEvent )
{
  float aFloat;
  int anInt;
  float E1 = 0.0;
  float R2 = 10.0;
  int nGTL = 0;
  
  bool passThisEvent = false;
  _nReadEvents++;
  
  TagFilterModule::event( anEvent );
  
  if(tag( )->getInt( anInt, "nGoodTrkLoose")) nGTL = anInt;
  if(tag( )->getFloat( aFloat, "e1Mag"))        E1 = aFloat;
  if(tag( )->getFloat( aFloat, "R2"))           R2 = aFloat;
  
  ErrMsg(debugging) << "nGoodTrkLoose/e1Mag/R2 =" << nGTL
		    << ", " << E1 << ", "  << R2 << endmsg;

  if( _minE1.value() < E1 && E1 < _maxE1.value()
      && R2 <= _maxR2.value() && nGTL >= _minNGtl.value() ) {
    HepAList< EventInfo >* infoList = 
      Ifd<HepAList< EventInfo > >::get(anEvent, _eventInfoList.value() );
    if (infoList == 0) ErrMsg(fatal) << "eventInfoList not in event" << endmsg;
      
    EventInfo* eventInfo= infoList->first();
    
    //  //get the cm frame
    HepLorentzVector cmframe = eventInfo->cmFrame();
    BtaBooster theBooster(cmframe);
      
    // Make composite candidates by adding 4-vectors
    static BtaOpAdd4 c;
      
    //
    HepAList<BtaCandidate>* photonList;
    photonList = Ifd< HepAList<BtaCandidate> >::get( anEvent, _photonList.valueString().c_str());
    assert( photonList!=0 );
    
    // Get list of composite candidates that will be persisted, pi0
    HepAList<BtaCandidate>* outputPi0CandList = new HepAList<BtaCandidate>;

    // Get list of composite candidates that will be persisted, eta
    HepAList<BtaCandidate>* outputEtaCandList = new HepAList<BtaCandidate>;

    BtaCandidate *myGamma1, *myGamma2;
  
    static const PdtEntry* pdtPi0 = Pdt::lookup(PdtLund::pi0);
    static const PdtEntry* pdtEta = Pdt::lookup(PdtLund::eta);

    HepAListIterator<BtaCandidate> iterGam1(*photonList);

    ErrMsg(debugging) << "photon list length=" << photonList->length() << endmsg;
    
    while ((myGamma1=iterGam1()) != 0) {
      float eCMSGam1 = theBooster.boostedP4(*myGamma1).e();
      ErrMsg(debugging) << ": eCMSGam1/_heGammaELow.value(): "
			<< eCMSGam1 << ", " << _heGammaELow.value() << endmsg;
      HepAListIterator<BtaCandidate> iterGam2(iterGam1);
      while ((myGamma2=iterGam2())!=0) {
	float eCMSGam2 = theBooster.boostedP4(*myGamma2).e();
	ErrMsg(debugging) << ": eCMSGam2/_heGammaELow.value(): "
			  << eCMSGam2 << ", " << _heGammaELow.value() << endmsg;
	if (! myGamma1->overlaps(*myGamma2)) {
	  if ((eCMSGam1 >_heGammaELow.value() && myGamma2->energy() >_pi0GamELow.value())
	      || (eCMSGam2> _heGammaELow.value() && myGamma1->energy() >_pi0GamELow.value()) ) {
	    BtaCandidate* theCand=c.create(*myGamma1, *myGamma2);
	    float mass = theCand->mass();
	    float cmEnergy=theBooster.boostedP4(*theCand).e();
	    ErrMsg(debugging) << ": cand mass/cmEnergy: " << mass << ", " << cmEnergy << endmsg;
	    if ((mass > _pi0MassLow.value()) && (mass < _pi0MassHigh.value()) ) {
	      if ((cmEnergy > _pi0EnergyLow.value()) && (cmEnergy <_pi0EnergyHigh.value())) {
		passThisEvent = true;
		ErrMsg(debugging) << ": it's a pi0" << endmsg;
		theCand->setType(pdtPi0);
		outputPi0CandList->append(new BtaCandidate(*theCand));
	      }
	    }
	    delete theCand; theCand = 0;
	  }
	  if ((eCMSGam1 >_heGammaELow.value() && myGamma2->energy() >_etaGamELow.value())
	      || (eCMSGam2> _heGammaELow.value() && myGamma1->energy() >_etaGamELow.value()) ) {	    
	    BtaCandidate* theCand=c.create(*myGamma1, *myGamma2);
	    float mass = theCand->mass();
	    float cmEnergy=theBooster.boostedP4(*theCand).e();
	    ErrMsg(debugging) << ": cand mass/cmEnergy: " << mass << ", " << cmEnergy << endmsg;
	    
	    if ((mass > _etaMassLow.value()) && (mass < _etaMassHigh.value())) {
	      if ((cmEnergy >_etaEnergyLow.value()) && (cmEnergy < _etaEnergyHigh.value()) ) {
		passThisEvent = true;
		ErrMsg(debugging) << ": it's an eta" << endmsg;
		theCand->setType(pdtEta);
		outputEtaCandList->append(new BtaCandidate(*theCand));
	      }
	    }
	    //}
	    delete theCand; theCand = 0;
	  }
	}
      }
    }  
    
    // Now report whether this event passes the skim or not
    if (passThisEvent) {
      setPassed(true);  // This event passed the skim
      _nEventsPassed++;
      ErrMsg(debugging) << " pi0/eta list lenghts=" << outputPi0CandList->length()
			<< ", " << outputEtaCandList->length() << endmsg;
      if (outputPi0CandList->length()>0) {
	IfdHepAListProxy<BtaCandidate>* proxy = new IfdHepAListProxy<BtaCandidate>(outputPi0CandList);
	if (0==proxy) {ErrMsg(fatal) << "proxy==0 in BtoXGammaPi0Filter !!" << endmsg;}
	bool ok = Ifd< HepAList<BtaCandidate> >::put( anEvent, proxy, _outputPi0CandList.value() );
	if (!ok) {ErrMsg(fatal) << "BtoXGammaPi0Filter: outputPi0CandList already in the event !!" << endmsg;}
      }
      if (outputEtaCandList->length()>0) {
	IfdHepAListProxy<BtaCandidate>* proxy = new IfdHepAListProxy<BtaCandidate>(outputEtaCandList);
	if (0==proxy) {ErrMsg(fatal) << "proxy==0 in BtoXGammaPi0Filter !!" << endmsg;}
	bool ok = Ifd< HepAList<BtaCandidate> >::put( anEvent, proxy, _outputEtaCandList.value() );
	if (!ok) {ErrMsg(fatal) << "BtoXGammaPi0Filter: outputEtaCandList already in the event !!" << endmsg;}
      }
    }

  }//end of tag variables

  return AppResult::OK;
}

AppResult BtoXGammaPi0Filter::endJob( AbsEvent* anEvent )
{
  // Print out some statistics at the end
  ErrMsg(routine) << "BtoXGammaPi0Filter:" 
		  << " Read " << _nReadEvents 
		  << " Passed " << _nEventsPassed << endmsg;
  return AppResult::OK;
}


AppModule *BtoXGammaPi0Filter::clone( const char *cloneName )
{
  return new BtoXGammaPi0Filter( cloneName, "clone of BtoXGammaPi0Filter" );
}
