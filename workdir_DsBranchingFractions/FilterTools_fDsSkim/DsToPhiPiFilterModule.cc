//--------------------------------------------------------------------------
// File and Version Information:
//
// $Id: DsToPhiPiFilterModule.cc,v 1.13 2005/04/29 16:59:10 desilva Exp $
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Sergey Ganzhur    
//
// Copyright Information:
//	Copyright (C) 1999            CEA - Centre d'Etude de Saclay 
//
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"
//-----------------------
// This Class's Header --
//-----------------------
#include "FilterTools/DsToPhiPiFilterModule.hh"
// also defines the class variables


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
#include "CLHEP/Alist/AList.h"
#include "CLHEP/Alist/AIterator.h"
#include "CLHEP/Vector/LorentzVector.h"
#include "CLHEP/Vector/ThreeVector.h"
#include "CLHEP/Geometry/HepPoint.h"

#include "HepTuple/TupleManager.h"
#include "HepTuple/Tuple.h"
#include "HepTuple/Histogram.h"
#include "AbsEvent/AbsEvent.hh"
#include "AbsEvent/getTmpAList.hh"
#include "CLHEP/Alist/AList.h"
#include "ProxyDict/IfdStrKey.hh"
#include "ProxyDict/IfdKey.hh"
#include "ProxyDict/Ifd.hh"
#include "AbsEnv/AbsEnv.hh"
#include "GenEnv/GenEnv.hh"
#include "PDT/Pdt.hh"
#include "PDT/PdtEntry.hh"
#include "ErrLogger/ErrLog.hh"

#include "Beta/EventInfo.hh"
#include "Beta/BtaCandidate.hh"

#include "BetaCoreTools/BtaOpMakeTree.hh"
#include "VtxFitter/VtxLeastChiVertexer.hh"
#include "ProbTools/probab.hh"

#include "BetaCoreTools/BtaBooster.hh"

#include "BetaMicroAdapter/BtaMicroAdapter.hh"
#include "BetaMicroAdapter/BtaPidQual.hh"
#include "BetaMicroAdapter/BtaPidInfo.hh"
using std::cout;
using std::endl;

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------


//----------------
// Constructors --
//----------------

// in general, a module constructor should not do much.  The begin(job) or
// begin(run) members are better places to put initialization
DsToPhiPiFilterModule::DsToPhiPiFilterModule( 
    const char* const theName, 
    const char* const theDescription )
    : AppFilterModule( theName, theDescription )
    , _eventInfoList("eventInfoList",this,"Default")
    , _btaCandList("BtaCandidates",this,"BtaMicroCandidates")
    , _phiMassCut("phiMassCut",this, 0.02 )
    , _massCutLower("massCutLower",this, 1.7 )
    , _massCutUpper("massCutUpper",this, 2.1 )
    , _pCut("pCut",this, 0 )
    , _checkPiTightKaon("checkPiTightKaon",this,false)

{
  commands()->append(& _eventInfoList);
  commands()->append(& _btaCandList);
  commands()->append(& _phiMassCut);
  commands()->append(& _massCutLower);
  commands()->append(& _massCutUpper);
  commands()->append(& _pCut);
  commands()->append(& _checkPiTightKaon);

}

//--------------
// Destructor --
//--------------

// The destructor should be limited to undoing the work of the constructor
DsToPhiPiFilterModule::~DsToPhiPiFilterModule( )
{
}

//--------------
// Operations --
//--------------

// The begin(AppJob*) member function is run before any events are
// processed.  In this analysis, it opens the output histogram file
// and then books a number of histograms and a ntuple.

AppResult
DsToPhiPiFilterModule::beginJob( AbsEvent* anEvent )
{
  ErrMsg(routine)<<"begin"<<endmsg; 

  HepTupleManager* manager = gblEnv->getGen()->ntupleManager();
  assert(manager != 0);
  
// book histograms  (arguments are nBins, low edge, high edge,
//                   comment is the PAW histogram number)

  _mPhiHisto  = manager->histogram("Phi mass ",                    60, 0.98, 1.1 );
  _mPhiHisto2 = manager->histogram("Phi mass: two loose kaons",    60, 0.98, 1.1 );
  _mDsHisto   = manager->histogram("mass of all Phis",             80, 1.7,  2.1 ); 
  
  
  _nReadEvents = 0 ; 
  _nPassed = 0 ; 

  
  return AppResult::OK;
}

AppResult
DsToPhiPiFilterModule::event( AbsEvent* anEvent )
{
// Event counter"
  _nReadEvents++;

// The key of selected events:
  bool passed(false);

// For convenience later, get a pointer to the event summary info object:

  HepAList< EventInfo >* infoList=NULL;
  infoList =  Ifd< HepAList<EventInfo> >::get( anEvent, _eventInfoList.value());
  assert(infoList != 0);
  EventInfo* eventInfo = infoList->first();
  
//  The Upsilon BtaCandidate for this event:
  BtaCandidate theUps( eventInfo->cmFrame(), 0 );
  BtaBooster booster( &theUps );


// Get BtaMicroCandidate list:
  HepAList<BtaCandidate>* trkList;
  trkList =  Ifd< HepAList<BtaCandidate> >::get( anEvent, _btaCandList.value());
  assert(trkList != 0);
  ErrMsg(trace) << name()<<": given "<<trkList->length()<<" track candidates from "<<_btaCandList.value()<<endmsg;


// Create positive and negative charge lists:

  HepAList<BtaCandidate> kPlusList;
  HepAList<BtaCandidate> kMinusList;
  HepAList<BtaCandidate> chList;

  BtaCandidate* trkPtr;
  HepAListIterator<BtaCandidate> iter(*trkList); // iterate over identified lepton list
  while((trkPtr = iter())) {    
    BtaCandidate tmp(*trkPtr);
    if ((trkPtr->charge()) < 0.1) {
      kMinusList.append(trkPtr);
      chList.append(trkPtr);
    } else if( (trkPtr->charge()) > 0.1 ) {
      kPlusList.append(trkPtr);
      chList.append(trkPtr);
    } 
  }


// Composition of phi's and Ds's
  VtxLeastChiVertexer dummy;
  BtaOpMakeTree vtxV0Op( dummy );

  HepAListIterator<BtaCandidate> iter1(kPlusList); // iterate over positive track list

  BtaCandidate* tr1(0);
  while ((tr1 = iter1())) {
    BtaCandidate t1(*tr1);
    BtaCandidate* trk1 = &t1;

    trk1->setType(Pdt::lookup(PdtLund::K_plus));
    HepAListIterator<BtaCandidate> iter2(kMinusList); // iterate negative track list

    BtaCandidate* tr2(0);
    while ( (tr2 = iter2()) ) {
      BtaCandidate t2(*tr2);
      BtaCandidate* trk2 = &t2;
      trk2->setType(Pdt::lookup(PdtLund::K_minus));


      if( fabs(trk1->charge())+(trk2->charge()) < 0.1 ) {
	BtaCandidate* phiPtr = vtxV0Op.create(*trk1,*trk2);

	if(phiPtr->decayVtx()->status() != BtaAbsVertex::Success) {
	  delete phiPtr;
	  continue;
	}

// Mass and Momentum
	    
	float dEdxDchPos, dEdxDchNeg; 
	float dEdxSvtPos, dEdxSvtNeg ; 
	float thetaPos, thetaNeg; 
	int ringNPhotPos, ringNPhotNeg;
	int ringNExPhotPos, ringNExPhotNeg;

	int isDRCPos, isDRCNeg;
	int isDCHPos, isDCHNeg;    
	int isSVTPos, isSVTNeg;    
	
	double consDRCPos, consDRCNeg;
	double consDCHPos, consDCHNeg;    
	double consSVTPos, consSVTNeg;    
	
	double consPiDRCPos, consPiDRCNeg    ;
	double consPiDCHPos, consPiDCHNeg;    
	double consPiSVTPos, consPiSVTNeg;    
	

// Parameters of the Positive track from microDB

	double pPos = trk1->p();

	{
	  const BtaPidQual* PidQual = trk1->getMicroAdapter()->getPidQual();
	  const BtaPidInfo* PidInfo = trk1->getMicroAdapter()->getPidInfo();
	  
// dE/dx information from DCH and SVT
	  dEdxDchPos = PidQual->dEdXDch();
	  dEdxSvtPos = PidQual->dEdXSvt();
	  
// DIRC PID information
	  thetaPos    = PidQual->thetaC();      // Cosine of the opening angle of the Cernkov ring 
	  ringNPhotPos= PidQual->ringNPhot();   //Number of photons detected in phototubes associated with the Cerenkov ring 
	  ringNExPhotPos = PidQual->ringNExPhot(Pdt::lookup(PdtPdg::pi_plus)); //Number of photons expected to be associated with a ring of this particle's momentum.
	  
// Kaon identification using SVT, DCH and DIRC by consistencies terms
// Contribution of the subsystems to particle identification
	  
	  isDRCPos = PidInfo->consistency(Pdt::lookup(PdtPdg::K_plus), PidSystem::drc).status();
	  isDCHPos = PidInfo->consistency(Pdt::lookup(PdtPdg::K_plus), PidSystem::dch).status();
	  isSVTPos = PidInfo->consistency(Pdt::lookup(PdtPdg::K_plus), PidSystem::svt).status();

// Consistencies using SVT, DCH and DIRC for kaons and pions
	  
	  consDRCPos = PidInfo->consistency(Pdt::lookup(PdtPdg::K_plus), PidSystem::drc).consistency();
	  consDCHPos = PidInfo->consistency(Pdt::lookup(PdtPdg::K_plus), PidSystem::dch).consistency();
	  consSVTPos = PidInfo->consistency(Pdt::lookup(PdtPdg::K_plus), PidSystem::svt).consistency();
	  
	  consPiDRCPos = PidInfo->consistency(Pdt::lookup(PdtPdg::pi_plus), PidSystem::drc).consistency();
	  consPiDCHPos = PidInfo->consistency(Pdt::lookup(PdtPdg::pi_plus), PidSystem::dch).consistency();
	  consPiSVTPos = PidInfo->consistency(Pdt::lookup(PdtPdg::pi_plus), PidSystem::svt).consistency();
	      
	} 

// Parameters of the Negative track from microDB

	double pNeg = trk2->p();

	{
	  const BtaPidQual* PidQual = trk2->getMicroAdapter()->getPidQual();
	  const BtaPidInfo* PidInfo = trk2->getMicroAdapter()->getPidInfo();

// dE/dx information from DCH and SVT
	  dEdxDchNeg = PidQual->dEdXDch();
	  dEdxSvtNeg = PidQual->dEdXSvt();
	  
// DIRC PID information
	  thetaNeg    = PidQual->thetaC();      // Cosine of the opening angle of the Cernkov ring 
	  ringNPhotNeg= PidQual->ringNPhot();   //Number of photons detected in phototubes associated with the Cerenkov ring 
	  ringNExPhotNeg = PidQual->ringNExPhot(Pdt::lookup(PdtPdg::pi_minus)); //Number of photons expected to be associated with a ring of this particle's momentum.
	  
	  
// Kaon identification using SVT, DCH and DIRC by consistencies terms
// Contribution of the subsystems to particle identification
	  
	  isDRCNeg = PidInfo->consistency(Pdt::lookup(PdtPdg::K_minus), PidSystem::drc).status();
	  isDCHNeg = PidInfo->consistency(Pdt::lookup(PdtPdg::K_minus), PidSystem::dch).status();
	  isSVTNeg = PidInfo->consistency(Pdt::lookup(PdtPdg::K_minus), PidSystem::svt).status();
// Consistencies using SVT, DCH and DIRC for kaons and pions
	  
	  consDRCNeg = PidInfo->consistency(Pdt::lookup(PdtPdg::K_minus), PidSystem::drc).consistency();
	  consDCHNeg = PidInfo->consistency(Pdt::lookup(PdtPdg::K_minus), PidSystem::dch).consistency();
	  consSVTNeg = PidInfo->consistency(Pdt::lookup(PdtPdg::K_minus), PidSystem::svt).consistency();
	  
	  consPiDRCNeg = PidInfo->consistency(Pdt::lookup(PdtPdg::pi_minus), PidSystem::drc).consistency();
	  consPiDCHNeg = PidInfo->consistency(Pdt::lookup(PdtPdg::pi_minus), PidSystem::dch).consistency();
	  consPiSVTNeg = PidInfo->consistency(Pdt::lookup(PdtPdg::pi_minus), PidSystem::svt).consistency();
	  
	}
	
// Kaon identification with the use of all subsystems participating in PID: SVT, DCH, DIRC:


	bool isPos(false);
	bool isNeg(false);
	
// Loose kaons limits for consistencies:
	float lowDrc = 0.01, upDrc = 1;
	float lowDch = 0.01, upDch = 1;
	float lowSvt = 0.01, upSvt = 1;
	
// Minimal number of photoelectrons for positive id in the DIRC:

	int nPhotCut = 5;
	float kTh    = 0.47;

// Positive track identification

	if(consDRCPos>lowDrc) {
	  if(consPiDRCPos/consDRCPos<upDrc && ringNPhotPos>nPhotCut && isDRCPos == Consistency::OK && pPos > kTh) 
	    isPos = true;
	}
	if(consDCHPos>lowDch) {
	  if(consPiDCHPos/consDCHPos<upDch && isDCHPos == Consistency::OK) 
	    isPos = true;
	}
	if(consSVTPos>lowSvt) {
	  if(consPiSVTPos/consSVTPos<upSvt && isSVTPos == Consistency::OK) 
	    isPos = true;
	}
	
// DIRC VETO mode ringNExPhot

	if(ringNExPhotPos>10 && pPos>0.7) {
	  if(ringNPhotPos>5) {
	    if( ringNPhotPos < (ringNExPhotPos - 3*sqrt(1.*ringNExPhotPos)) )
	      isPos = true;
	  } else {
	    if( ringNExPhotPos > 15 )
	      isPos = true;
	  }
	}



// Negative track identification

	if(consDRCNeg>lowDrc) {
	  if(consPiDRCNeg/consDRCNeg<upDrc && ringNPhotNeg>nPhotCut && isDRCNeg == Consistency::OK && pNeg > kTh) 
	    isNeg = true;
	}
	if(consDCHNeg>lowDch) {
	  if(consPiDCHNeg/consDCHNeg<upDch && isDCHNeg == Consistency::OK) 
	    isNeg = true;
	}
	if(consSVTNeg>lowSvt) {
	  if(consPiSVTNeg/consSVTNeg<upSvt && isSVTNeg == Consistency::OK) 
	    isNeg = true;
	}
// DIRC VETO mode
	
	if(ringNExPhotNeg>10 && pNeg > 0.7) {
	  if(ringNPhotNeg>5) {
	    if( ringNPhotNeg < (ringNExPhotNeg - 3*sqrt(1.*ringNExPhotNeg)) )
	      isNeg = true;
	  } else {
	    if( ringNExPhotNeg > 15 )
	      isNeg = true;
	  }
	}


// If the second track passed throught the either SVT or DCH or DIRC acceptance and was not  identified ,
// that this is not accepted as phi candidate.

	if (isPos && !isNeg) {
	  if(isDRCNeg  == Consistency::OK || isDCHNeg  == Consistency::OK || isSVTNeg  == Consistency::OK) {
	    delete phiPtr;
	    continue;
	  }
	}

	if (!isPos && isNeg) {
	  if(isDRCPos  == Consistency::OK || isDCHPos  == Consistency::OK || isSVTPos  == Consistency::OK) {
	    delete phiPtr;
	    continue;
	  }
	}
	
// At least one track is kaon

	if(isPos || isNeg) {	  
	  _mPhiHisto->accumulate(phiPtr->mass());
	  
	  if(isPos && isNeg) 
	    _mPhiHisto2->accumulate(phiPtr->mass());


	  phiPtr->setType(Pdt::lookup(PdtLund::phi));

// Combination of phi candidate with all charge tracks in the event

	  VtxLeastChiVertexer dummy2;
	  BtaOpMakeTree op( dummy2 );
	  
	  if ( fabs(phiPtr->mass() - Pdt::mass(PdtLund::phi)) < _phiMassCut.value() ) {
	    HepAListIterator<BtaCandidate> iterPi(*trkList);
	    BtaCandidate* pi(0);
	    while( (pi = iterPi()) ) {
	      BtaCandidate piLoc(*pi);
	      BtaCandidate* piPtr = &piLoc;
	      
	      if ( ! phiPtr->overlaps(*piPtr) ) {
		if(fabs(piPtr->charge()) < 0.1) continue;

		if(piPtr->charge()>0)
		  piPtr->setType(Pdt::lookup(PdtLund::pi_plus));
		else
		  piPtr->setType(Pdt::lookup(PdtLund::pi_minus));
	    

		BtaCandidate* theDs = op.create(*phiPtr, *piPtr);
		
		if(theDs->decayVtx()->status() != BtaAbsVertex::Success) {
		  delete theDs;
		  continue;
		}

		BtaCandidate boostedDs       = booster.boostTo( *theDs );

// Setup of the Loose cuts for Ds:

		if (boostedDs.p() > _pCut.value() && theDs->mass() > _massCutLower.value() && 
		    theDs->mass() < _massCutUpper.value()) {

// Setup the passed key:
		  passed = true;


// Check if pi was identified as tight kaon ( if _isPiTightKaon.value(), otherwise take all combinations)

		  if ( _checkPiTightKaon.value() ) {
// Pion parameters from microDB
		     
		      
		    const BtaPidQual* PidQual = piPtr->getMicroAdapter()->getPidQual();
		    const BtaPidInfo* PidInfo = piPtr->getMicroAdapter()->getPidInfo();

		    float ringNPhotPi   = PidQual->ringNPhot();   
		    
		    int ringNExPhotPi; 
		    if(piPtr->charge() > 0)
		      ringNExPhotPi = PidQual->ringNExPhot(Pdt::lookup(PdtPdg::pi_plus)); 
		    else
		      ringNExPhotPi = PidQual->ringNExPhot(Pdt::lookup(PdtPdg::pi_minus)); 
		    
		    int isDRCPi, isDCHPi, isSVTPi;
		    float consDRCPi,consDCHPi, consSVTPi;
		    float consPiDRCPi,consPiDCHPi, consPiSVTPi;
			
		    if (piPtr->charge() > 0) {
			  
		      isDRCPi = PidInfo->consistency(Pdt::lookup(PdtPdg::K_plus), PidSystem::drc).status();
		      isDCHPi = PidInfo->consistency(Pdt::lookup(PdtPdg::K_plus), PidSystem::dch).status();
		      isSVTPi = PidInfo->consistency(Pdt::lookup(PdtPdg::K_plus), PidSystem::svt).status();
			  
// Consistencies using SVT, DCH and DIRC for kaons and pions
			  
		      consDRCPi = PidInfo->consistency(Pdt::lookup(PdtPdg::K_plus), 
							   PidSystem::drc).consistency();
		      consDCHPi = PidInfo->consistency(Pdt::lookup(PdtPdg::K_plus), 
							   PidSystem::dch).consistency();
		      consSVTPi = PidInfo->consistency(Pdt::lookup(PdtPdg::K_plus), 
							   PidSystem::svt).consistency();

			  
		      consPiDRCPi = PidInfo->consistency(Pdt::lookup(PdtPdg::pi_plus), 
							     PidSystem::drc).consistency();
		      consPiDCHPi = PidInfo->consistency(Pdt::lookup(PdtPdg::pi_plus), 
							 PidSystem::dch).consistency();
		      consPiSVTPi = PidInfo->consistency(Pdt::lookup(PdtPdg::pi_plus), 
							 PidSystem::svt).consistency();
			  
		    } else {
			  
		      isDRCPi = PidInfo->consistency(Pdt::lookup(PdtPdg::K_minus), PidSystem::drc).status();
		      isDCHPi = PidInfo->consistency(Pdt::lookup(PdtPdg::K_minus), PidSystem::dch).status();
		      isSVTPi = PidInfo->consistency(Pdt::lookup(PdtPdg::K_minus), PidSystem::svt).status();
// Consistencies using SVT, DCH and DIRC for kaons and pions
		      
		      consDRCPi = PidInfo->consistency(Pdt::lookup(PdtPdg::K_minus), 
						       PidSystem::drc).consistency();
		      consDCHPi = PidInfo->consistency(Pdt::lookup(PdtPdg::K_minus), 
						       PidSystem::dch).consistency();
		      consSVTPi = PidInfo->consistency(Pdt::lookup(PdtPdg::K_minus), 
						       PidSystem::svt).consistency();

			
		      consPiDRCPi= PidInfo->consistency(Pdt::lookup(PdtPdg::pi_minus), 
							PidSystem::drc).consistency();
		      consPiDCHPi= PidInfo->consistency(Pdt::lookup(PdtPdg::pi_minus), 
							PidSystem::dch).consistency();
		      consPiSVTPi= PidInfo->consistency(Pdt::lookup(PdtPdg::pi_minus), 
							PidSystem::svt).consistency();
		      
		    }

		    bool piIsKaon(false);
		    float lowDrc = 0.01, upDrc = 0.1;
		    float lowDch = 0.01, upDch = 0.1;
		    float lowSvt = 0.01, upSvt = 0.1;
		    int nPhotCut = 10;
		    float kTh    = 0.47;

		    double pPi = piPtr->p();
		    
		    if(consDRCPi>lowDrc) {
		      if(consPiDRCPi/consDRCPi<upDrc && ringNPhotPi>nPhotCut && isDRCPi == Consistency::OK  
			 && pPi > kTh) 
			piIsKaon = true;
		    }
		    if(consDCHPi>lowDch) {
		      if(consPiDCHPi/consDCHPi<upDch && isDCHPi == Consistency::OK) 
			piIsKaon = true;
		    }
		    if(consSVTPi>lowSvt) {
		      if(consPiSVTPi/consSVTPi<upSvt && isSVTPi == Consistency::OK) 
			piIsKaon = true;
		    }
		    
		    
// if pi was identified as tight kaon then  skip ( if _isPiTightKaon.value() option)  

		    if ( piIsKaon ) passed = false;
		    
		  } //end if ( _checkPiTightKaon.value() )		      

		  if(passed) _mDsHisto->accumulate( theDs->mass() );
		  
		}// end of momentum and mass cuts checking
		delete theDs;
	      } // end of overlap checking
	    }  // end of pi iteration
	  }   // end of phi mass cut
	}    // end if(isPos || isNeg)
	delete phiPtr;
      }    // end of loop to look at all Phi
    }
  }


  setPassed( passed ); // skip the event if not passed

  if (passed) _nPassed++ ; 

  return AppResult::OK;
}

AppResult
DsToPhiPiFilterModule::endJob( AbsEvent* anEvent )
{

  ErrMsg(routine) << name( ) << " end Job" << endmsg;
  cout << " with " << _nReadEvents << " events read " << endl ; 
  cout << " and  " << _nPassed << " events passed " << endl ; 

  return AppResult::OK;
}


