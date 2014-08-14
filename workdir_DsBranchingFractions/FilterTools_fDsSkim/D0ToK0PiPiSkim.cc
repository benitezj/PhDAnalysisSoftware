//--------------------------------------------------------------------------
//
// CVS $Id: D0ToK0PiPiSkim.cc,v 1.4 2008/02/05 11:08:42 marchior Exp $
//
// Description:
// The skim contains events with either a D* candidate from the following
// decay chain:
//
//              D*+
//              |->D0 pi+
//                 |->K*- pi+
//                    |-> KL pi-
//
// or a D* candidate from the following decay chain:
//
//              D*+
//              |->D0 pi+
//                 |-> KS pi- pi+
//
//------------------------------------------------------------------------

#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "FilterTools/D0ToK0PiPiSkim.hh"

//---------------
// C++ Headers --
//---------------
#include <math.h>

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "AbsCalo/AbsRecoCalo.hh"
#include "AbsEnv/AbsEnv.hh"
#include "AbsEvent/AbsEvent.hh"
#include "AbsEvent/getTmpAList.hh"
#include "AbsEvent/AbsEventID.hh"
#include "AbsEventTag/AbsEventTag.hh"
#include "BbrGeom/BbrPointErr.hh"
#include "BbrGeom/BbrDoubleErr.hh"
#include "Beta/BtaCandidate.hh"
#include "Beta/EventInfo.hh"
#include "Beta/BtaAbsVertex.hh"
#include "BetaCoreTools/BtaOpMakeTree.hh"
#include "BetaCoreTools/BtaBooster.hh"
#include "BetaCoreTools/BtaTreeNavigator.hh"
#include "BetaMicroAdapter/BtaMicroAdapter.hh"
#include "BetaMicroAdapter/BtaPidQual.hh"
#include "BetaMicroAdapter/BtaCalQual.hh"
#include "BetaMicroAdapter/BtaTrkQual.hh"
#include "BetaMicroAdapter/BtaIfrQual.hh"
#include "BetaRecoAdapter/BtaAbsRecoObject.hh"
#include "BtaEnv/BtaEnv.hh"
#include "CLHEP/Alist/AList.h"
#include "CLHEP/Alist/AIterator.h"
#include "CLHEP/Geometry/HepPoint.h"
#include "CLHEP/Matrix/Vector.h"
#include "CLHEP/Vector/LorentzVector.h"
#include "difAlgebra/DifNumber.hh"
#include "ErrLogger/ErrLog.hh"
#include "GenEnv/GenEnv.hh"
#include "PDT/Pdt.hh"
#include "PDT/PdtEntry.hh"
#include "ProbTools/probab.hh"
#include "ProbTools/Consistency.hh"
#include "ProxyDict/IfdStrKey.hh"
#include "ProxyDict/IfdKey.hh"
#include "ProxyDict/Ifd.hh"
#include "TrkFitter/TrkPocaXY.hh"
#include "TrkBase/TrkAbsFit.hh"
#include "VtxBase/BtaAbsVertexer.hh"
#include "VtxBase/VtxAbsFitter.hh"
#include "VtxFitter/VtxFitterOper.hh"


D0ToK0PiPiSkim::D0ToK0PiPiSkim( const char* const theName, 
				const char* const theDescription )
  : TagFilterModule( theName, theDescription )

  //outputlists
  , _DstrToKLList("DstrToKLList",this,"DstarToKLList")
  , _DstrToKSList("DstrToKSList",this,"DstarToKSList")

  //input lists
  ,  eventIdKey( "EventIdKey", this, "AbsEventID" )
  , _eventInfoList("eventInfoList",this,"Default")  
  , _btaNeutralList("btaNeutralList",this,"CalorClusterNeutral")
  , _btaNeutHadList("btaNeutHadList",this, "NeutralHadrons")
  , _K0SList("K0SList",this,"KsDefaultChisq")
  , _pionList( "pionList", this, "piLHLoose" )

  //fit algorithm
  , vtxFitAlgo("vtxFitAlgo", this, VtxAbsAlgorithm::TreeFitter)
  , vtxFitMode("vtxFitMode", this, VtxAbsAlgorithm::Standard)

  //cuts
  , slowPiMomLo( "slowPiMomLo", this, 0.0 )
  , slowPiMomHi( "slowPiMomHi", this, 1.0 )
  , slowPiRdocaMax( "slowPiRdocaMax", this, 0.2 )
  , slowPiZdocaMax( "slowPiZdocaMax", this, 3.0 )
  , slowPiSvtHit( "slowPiSvtHit", this, 6 )
  , slowPiDchHit( "slowPiDchHit", this, 11 )
  , deltaMassCutHi( "deltaMassCutHi", this, 0.16 )
  , deltaMassCutLo( "deltaMassCutLo", this, 0.138 )
  , pStarCut("pStarCut", this, 2.0)
  , dxymax("dxymax", this, 0.1)
  , dzmax("dzmax", this, 2.)
  , pkmin("pkmin", this, 0.000001)
  , pkmax("pkmax", this, 5.0)
  , pksmin("pksmin", this, 0.0)
  , pksmax("pksmax", this, 5.0)
  , mkstarmin("mkstarmin", this, 0.79)
  , mkstarmax("mkstarmax", this, 1.0)
  , ecalmin("ecalmin", this, 0.0)
  , eksmin("eksmin", this, 0.0)
  , helmin("helmin", this, 0.4)
  
{
  commands()->append(&_DstrToKLList);
  commands()->append(&_DstrToKSList);

  commands()->append(&eventIdKey );
  commands()->append(&_eventInfoList);
  commands()->append(&_btaNeutralList);
  commands()->append(&_btaNeutHadList);
  commands()->append(&_K0SList );
  commands()->append(&_pionList );

  commands()->append(&vtxFitAlgo);
  commands()->append(&vtxFitMode);
                                                                    
  commands()->append(&slowPiMomLo);
  commands()->append(&slowPiMomHi);  
  commands()->append(&slowPiRdocaMax); 
  commands()->append(&slowPiZdocaMax); 
  commands()->append(&slowPiSvtHit);
  commands()->append(&slowPiDchHit);
  commands()->append(&deltaMassCutHi);
  commands()->append(&deltaMassCutLo);
  commands()->append(&pStarCut); 
  commands()->append(&dxymax); 
  commands()->append(&dzmax);
  commands()->append(&pkmin);
  commands()->append(&pkmax);
  commands()->append(&pksmin);
  commands()->append(&pksmax);
  commands()->append(&mkstarmin);
  commands()->append(&mkstarmax);
  commands()->append(&ecalmin);
  commands()->append(&eksmin);
  commands()->append(&helmin);
}
		
//--------------
// Destructor --
//--------------
D0ToK0PiPiSkim::~D0ToK0PiPiSkim()
{
}

//-------------
// Methods   --
//-------------

AppResult D0ToK0PiPiSkim::beginRun( AbsEvent* anEvent )
{
  return AppResult::OK;
}

AppResult D0ToK0PiPiSkim::beginJob( AbsEvent*  anEvent )
{
  ErrMsg(routine) << ": begin job" << endmsg;
  
  // initialize event counters
  _nReadEvents = 0;
  _nEventsPassed = 0;
  return AppResult::OK;
}


AppResult D0ToK0PiPiSkim::event( AbsEvent* anEvent )
{

  bool passThisEvent = false;
  _nReadEvents++;
  setPassed( false );


  //tag filters
  //===============================================

  AbsEventTag* tag = Ifd<AbsEventTag>::get( anEvent );

  bool hadron(false);
  if(tag->getBool(hadron,"BGFMultiHadron")){
    if(! hadron) return AppResult::OK;
  }

  float R2(0.0);
  if(tag->getFloat(R2,"R2")){
    if(R2 > 0.9) return AppResult::OK;
  }

  int numTracks(0);
  if(tag->getInt(numTracks,"nTracks")){
    if(numTracks < 3) return AppResult::OK;
  }


  //event conditions
  //===============================================

  const PepBeams *theBeams=gblEnv->getPep()->pepBeams();
  Hep3Vector UpsiBoost = theBeams->boostCMtoLab();
  static const IfdStrKey EvtInfoKey("Default");
  HepAList< EventInfo >* infoList =  Ifd<HepAList< EventInfo > >::get(anEvent,_eventInfoList.value());
  if (infoList == 0){ ErrMsg(fatal) << "Could not locate event info list of name ";}
  EventInfo* eventInfo = infoList->first();
  const BbrPointErr beamSpot = eventInfo->beamSpot();
 

  // get list of input charged-track candidates
  //===============================================

  HepAList<BtaCandidate>* PionLHTList = Ifd<HepAList< BtaCandidate > >::get(anEvent,_pionList.value());
  int ntracks = PionLHTList->length();

  // if no pions are in the event or if the number of tracks is less then 3, 
  // skip this event

  if( 0 == PionLHTList || ntracks < 3) {
    setPassed( false );
    return AppResult::OK;
  }

 
  // build pion lists
  //===============================================

  HepAList<BtaCandidate> PionPlusList;
  HepAList<BtaCandidate> PionMinusList;
  HepAList<BtaCandidate> PionSlowList;
 
  // begin loop over charged-track candidates

  HepAListIterator<BtaCandidate> iterpislow(*PionLHTList);
  BtaCandidate* pislow(0);
  while ( pislow=iterpislow()){

    if (pislow->charge() == 0) continue;
    if (pislow->charge()>0) PionPlusList += pislow;
    if (pislow->charge()<0) PionMinusList += pislow;

    bool ispislow(false);

    //pt cut
    HepDouble ptLab = pislow->pt();
    if (ptLab >= slowPiMomLo.value() && ptLab <= slowPiMomHi.value()) {

      //SVT and DCH hits cuts
      const BtaTrkQual* trkQual = pislow->getMicroAdapter()->getTrkQual();
      int nSvt = trkQual->nSvtHits();
      int nDch = trkQual->nDchHits();
      if (nSvt >= slowPiSvtHit.value() && nDch >= slowPiDchHit.value()) {

	//Doca cuts
	TrkPocaXY pocaxy( pislow->trkAbsFit()->traj(), 0, beamSpot);
	double RPoca = fabs(pocaxy.docaXY());
	HepPoint poca = pislow->trkAbsFit()->position(pocaxy.fltl1());
	double ZPoca = fabs(poca.z());
	if (RPoca <= slowPiRdocaMax.value() && ZPoca <= slowPiZdocaMax.value()) {
	  //Seems fine, accept as a slow pion
	  ispislow=true;
	}
      }
    }
    if (ispislow) 
      PionSlowList += pislow;
  }


  // if pi+, pi- or slow pion list is empty skip that event
  //=======================================================

  if(PionPlusList.length() == 0 || PionMinusList.length()== 0 || PionSlowList.length()== 0){
    setPassed( false );
    return AppResult::OK;
  }
 

  // get list of Neutral Hadrons for K0L candidates
  //===============================================

  // get list of Neutral Hadrons for IFR (NHIFR)

  HepAList<BtaCandidate>* NeutHadList;
  NeutHadList =  Ifd< HepAList<BtaCandidate> >::get(anEvent, _btaNeutHadList.value());
  int n_neuthadifr = NeutHadList->length();
  if(!NeutHadList){ ErrMsg(fatal) << "No NeutHadList found!!!  -->"<<_btaNeutHadList.value()<< endmsg; }

  // get list of Neutral Hadrons for EMC (NHEMC)

  HepAList<BtaCandidate>* CalorNeutHadList;
  CalorNeutHadList =  Ifd< HepAList<BtaCandidate> >::get(anEvent, _btaNeutralList.value() );
  int n_neuthademc = CalorNeutHadList->length();
  if(!CalorNeutHadList){ ErrMsg(fatal) << "No CalorNeutHadList found!!!  -->"<<_btaNeutralList.value()<< endmsg; }
 

  // get list of K0S candidates
  //===========================

  HepAList<BtaCandidate>* K0SList;
  K0SList =  Ifd< HepAList<BtaCandidate> >::get(anEvent, _K0SList.value() );
  if(!K0SList){ ErrMsg(fatal) << "No K0SList found!!!  -->"<<_K0SList.value()<< endmsg; }
  int n_K0S = K0SList->length();


  // reject the events which either have no NH candidates (i.e NHEMC or NHIFR)
  // nor K0S candidates
  //==========================================================================

  if(n_neuthademc == 0 && n_neuthadifr == 0 && n_K0S == 0) {
    setPassed( false );
    return AppResult::OK;
  }


  // create Dstar output lists
  //==========================

  //List with D0 decaying to a K0L

  HepAList<BtaCandidate> *myDstrToKLList;
  getTmpAList (anEvent, myDstrToKLList,_DstrToKLList.value());

  //List with D0 decaying to a K0S

  HepAList<BtaCandidate> *myDstrToKSList;
  getTmpAList (anEvent, myDstrToKSList,_DstrToKSList.value());
 


  // begin loop on [pi+] (D0->K0 [pi+] pi- decay)
  //=============================================
  
  HepAListIterator<BtaCandidate> iterTrk1(PionPlusList);
  BtaCandidate* trk1(0);
  while(trk1 = iterTrk1()) {

  
    // begin loop on [pi-] (D0->K0 pi+ [pi-] decay)
    //=============================================
    
    HepAListIterator<BtaCandidate> iterTrk2(PionMinusList);
    BtaCandidate* trk2(0);
    while(trk2 = iterTrk2()) {

      // vertex (pi+ pi-) and do a geometric constraint to the vertex
      BtaOpMakeTree comb;
      BtaCandidate pi_vtx = comb.combine(*trk1,*trk2);
      setGeoConstraint(pi_vtx);
      //VtxFitterOper f_fitter(pi_vtx,vtxFitAlgo.value(),vtxFitMode.value());
      VtxFitterOper f_fitter(pi_vtx,VtxAbsAlgorithm::Cascade,vtxFitMode.value());
      f_fitter.fit();
      BtaCandidate fitted_pivtx = f_fitter.getFittedTree();
      if(fitted_pivtx.decayVtx()->status()!=BtaAbsVertex::Success) continue;
      // impose chisquared requirement on the vertex
      double chi2_1 = fitted_pivtx.decayVtx()->chiSquared();
      int nDOF_1 = fitted_pivtx.decayVtx()->nDof();
      if(nDOF_1 <= 0) continue;
      double prob_1 = probab(nDOF_1,chi2_1);
      if(prob_1 <= 0.001) continue;

      // cut on distance in xy and in z projections between vertex and beamspot
      // orig = vertex position of the (pi+ pi-) vertex in the lab frame
      HepPoint orig = fitted_pivtx.fitParams().pos();
      // 4-momentum of the (pi+ pi-) system after the fit
      HepLorentzVector twopi = fitted_pivtx.fitParams().p4();
      // compute the distance, d,  of the (pi+ pi-) vertex to the beamspot
      Hep3Vector r(orig.x()-beamSpot.x(),orig.y()-beamSpot.y(),orig.z()-beamSpot.z());
      double dist = r.mag();
      double distxy = dist*sin(r.theta());
      double distz = fabs(orig.z() -beamSpot.z());
      if(distxy > dxymax.value() || distz > dzmax.value()) continue;
 

      // create K0L list
      //================

      HepAList<BtaCandidate> myK0LList;

      // K0L from EMC (NHEMC)

      if(n_neuthademc != 0) {

        HepAListIterator<BtaCandidate> iterBtaNHEMC(*CalorNeutHadList);
        BtaCandidate* btaNHEMC(0);
        while(btaNHEMC = iterBtaNHEMC()) {  // starts the loop on the NHEMC

          const BtaCalQual* CalQualNHEMC = btaNHEMC->getMicroAdapter()->getCalQual();
          if(CalQualNHEMC == 0) continue;
          if(CalQualNHEMC->ecalEnergy() <= ecalmin.value()) continue;

          // get the position of the cluster (center?) w.r.t. the lab frame
          HepPoint klongVertexEMC = btaNHEMC->recoCalo() ? btaNHEMC->recoCalo()->position() :  btaNHEMC->fitParams().pos() ;
 
          //get the 4-momentum for that EMC NH
          HepLorentzVector theEMCK0L_h = klfi_h(twopi, orig, klongVertexEMC);
          HepLorentzVector theEMCK0L_l = klfi_l(twopi, orig, klongVertexEMC);
 
          //pass that NH (sol w. high p) if its 3-momentum is within acceptable
	  //range
          if(theEMCK0L_h.rho() > pkmin.value() && theEMCK0L_h.rho() < pkmax.value()) {
	    //create a K0L candidate from that NHEMC
	    BtaCandidate* btaNHKL=new BtaCandidate(*btaNHEMC);
	    btaNHKL->setType(Pdt::lookup(PdtPdg::K_L0));

	    //give this K0L candidate the calculated energy and momentum from
	    //klfi function
	    btaNHKL->setMomentum3(theEMCK0L_h.vect());
	    btaNHKL->setEnergy(theEMCK0L_h.e());

	    //append this K0L candidate to the K0L list
	    myK0LList+=btaNHKL;
	  }

          //pass that NH (sol w. low p) if its 3-momentum is within acceptable
	  //range
          if(theEMCK0L_l.rho() > pkmin.value() && theEMCK0L_l.rho() < pkmax.value()) {
	    //create a K0L candidate from that NHEMC
	    BtaCandidate* btaNHKL=new BtaCandidate(*btaNHEMC);
	    btaNHKL->setType(Pdt::lookup(PdtPdg::K_L0));

	    //give this K0L candidate the calculated energy and momentum from
	    //klfi function
	    btaNHKL->setMomentum3(theEMCK0L_l.vect());
	    btaNHKL->setEnergy(theEMCK0L_l.e());

	    //append this K0L candidate to the K0L list
	    myK0LList+=btaNHKL;
	  }    
	}
      }

      // K0L from IFR (NHIFR)

      if(n_neuthadifr != 0) {

        HepAListIterator<BtaCandidate> iterBtaNHIFR(*NeutHadList);
        BtaCandidate* btaNHIFR(0);
        while(btaNHIFR = iterBtaNHIFR()) {   // starts the loop on the NHIFR

          const BtaIfrQual* IfrQualNHIFR = btaNHIFR->getMicroAdapter()->getIfrQual();
          //### rejection of the NHIFRs which have no hit-layers in the IFR ###
          if(IfrQualNHIFR == 0) continue;
          if(IfrQualNHIFR->IfrLayHits() == 0) continue;
          //**************************************************************
  
          // get the position of the cluster (center?) w.r.t. the lab frame
	  HepPoint klongVertexIFR = btaNHIFR->recoCalo() ? btaNHIFR->recoCalo()->position() :  btaNHIFR->fitParams().pos() ;

          //get the 4-momentum for that IFR NH
          HepLorentzVector theIFRK0L_h = klfi_h(twopi, orig, klongVertexIFR);
          HepLorentzVector theIFRK0L_l = klfi_l(twopi, orig, klongVertexIFR);
 
          //pass that NH (highest p sol) if its 3-momentum is within acceptable
	  //range
          if(theIFRK0L_h.rho() > pkmin.value() && theIFRK0L_h.rho() < pkmax.value()) {
	    //create a K0L candidate from that NHIFR
	    BtaCandidate* btaNHKL=new BtaCandidate(*btaNHIFR);
	    btaNHKL->setType(Pdt::lookup(PdtPdg::K_L0));

	    //give this K0L candidate the calculated energy and momentum from
	    //klfi function
	    btaNHKL->setMomentum3(theIFRK0L_h.vect());
	    btaNHKL->setEnergy(theIFRK0L_h.e());

	    //append this K0L candidate to the K0L list
	    myK0LList+=btaNHKL;
	  }    

          //pass that NH (lowest p sol) if its 3-momentum is within acceptable
	  //range
          if(theIFRK0L_l.rho() > pkmin.value() && theIFRK0L_l.rho() < pkmax.value()) {
	    //create a K0L candidate from that NHIFR
	    BtaCandidate* btaNHKL=new BtaCandidate(*btaNHIFR);
	    btaNHKL->setType(Pdt::lookup(PdtPdg::K_L0));

	    //give this K0L candidate the calculated energy and momentum from
	    //klfi function
	    btaNHKL->setMomentum3(theIFRK0L_l.vect());
	    btaNHKL->setEnergy(theIFRK0L_l.e());

	    //append this K0L candidate to the K0L list
	    myK0LList+=btaNHKL;
	  }
	}
      }


      //begin loops on K0 candidates
      //============================

      // K0L

      int n_K0L = myK0LList.length();
      if(n_K0L != 0) {
        HepAListIterator<BtaCandidate> iterBtaK0L(myK0LList);
        BtaCandidate* btaNHKL(0);
	while ( btaNHKL=iterBtaK0L() ) {  // starts the loop on the K0L

	  //vertex K0L with pi+ pi- to create D0:
	  //pi_vtx.invalidateFit();
	  //trk1->invalidateFit();
	  //trk2->invalidateFit();

	  BtaOpMakeTree combD0;
	  BtaCandidate D0Cand = combD0.combine(*btaNHKL,*trk1,*trk2);
	  D0Cand.setType(Pdt::lookup(PdtPdg::D0));
	  setMassConstraint(D0Cand);
	  //VtxFitterOper D0fit(D0Cand,vtxFitAlgo.value(),vtxFitMode.value());
	  VtxFitterOper D0fit(D0Cand,VtxAbsAlgorithm::Add4,vtxFitMode.value());
	  D0fit.fit();
	  BtaCandidate fitted_D0 = D0fit.getFittedTree();
	  if(fitted_D0.decayVtx()->status()!=BtaAbsVertex::Success) continue;

	  //cut on p*(D0)
          HepLorentzVector quadrip_D0 = fitted_D0.fitParams().p4();
          quadrip_D0.boost(-UpsiBoost);
          if(quadrip_D0.rho() < pStarCut.value()) continue;

	  BtaCandidate fitted_trk1 = D0fit.getFitted(*trk1);
	  BtaCandidate fitted_trk2 = D0fit.getFitted(*trk2);
	  BtaCandidate fitted_klong = D0fit.getFitted(*btaNHKL);

	  HepLorentzVector piplus_4mom = fitted_trk1.fitParams().p4();
	  HepLorentzVector piminus_4mom = fitted_trk2.fitParams().p4();
	  HepLorentzVector KL_4mom = fitted_klong.fitParams().p4();
 
	  HepAListIterator<BtaCandidate> iterTrk3(PionSlowList);
	  BtaCandidate* trk3(0);
	  while(trk3 = iterTrk3()) {

           //check for overlaps
           if(trk3->overlaps(*trk1) || trk3->overlaps(*trk2)) continue;

	    //identify K*->KL pi decay and cut on K* mass and helicity angle

	    HepLorentzVector Kstar_4mom;
	    if(trk3->charge() > 0) Kstar_4mom = KL_4mom + piminus_4mom;
	    if(trk3->charge() < 0) Kstar_4mom = KL_4mom + piplus_4mom;
	    if(Kstar_4mom.mag() < mkstarmin.value() || Kstar_4mom.mag() > mkstarmax.value()) continue;

	    double cosHel = 0.;
	    Hep3Vector beta(Kstar_4mom.boostVector());
	    KL_4mom.boost(-beta);
	    if(trk3->charge() > 0) {
	      piplus_4mom.boost(-beta);
	      cosHel = (KL_4mom.vect().unit()).dot(piplus_4mom.vect().unit());
	    }
	    if(trk3->charge() < 0) {
	      piminus_4mom.boost(-beta);
	      cosHel = (KL_4mom.vect().unit()).dot(piminus_4mom.vect().unit());
	    }
	    if(cosHel < helmin.value()) continue;
               

	    // combine D0 & pi_slow to obtain the Dstar
	    //=========================================

	    //vertex D0 with pi to create D*
	    BtaOpMakeTree combds;
	    BtaCandidate* D_star = combds.create(*trk3,fitted_D0);
	    D_star->setType((D_star->charge() < 0.0) ? Pdt::lookup(PdtPdg::D_star_minus) : Pdt::lookup(PdtPdg::D_star_plus));
	    setGeoConstraint(*D_star);
	    setBeamConstraint(*D_star,eventInfo);
	    //VtxFitterOper Dstarfit(*D_star,vtxFitAlgo.value(),vtxFitMode.value());
	    VtxFitterOper Dstarfit(*D_star,VtxAbsAlgorithm::Add4,vtxFitMode.value());
	    Dstarfit.fit();
	    BtaCandidate fitted_Dstar = Dstarfit.getFitted(*D_star);
	    if(fitted_Dstar.decayVtx()->status()!=BtaAbsVertex::Success) continue;

	    //cut on m(D*)-m(D0)
	    double deltam = fitted_Dstar.mass() - fitted_D0.mass();
	    if(deltaMassCutLo.value() < deltam && deltam < deltaMassCutHi.value()) {
	      // If all the conditions are met, we want to pass this event
	      passThisEvent = true;
	      myDstrToKLList->append( new BtaCandidate(*D_star) );
	    }
	    delete D_star;

	  } // end trk3 loop
	} //end K0L loop
	//cleanup
	HepAListDeleteAll(myK0LList);
      } // n_K0L != 0

      // K0S from KSLIST 
      // Only deltam cut is imposed here!
      if(n_K0S != 0) {

        HepAListIterator<BtaCandidate> iterK0S(*K0SList);
        BtaCandidate* btaK0S(0);
        while(btaK0S = iterK0S()) {  // starts the loop on the K0S

	  //vertex K0 with pi+ pi- to create D0:
          //pi_vtx.invalidateFit();
          //trk1->invalidateFit();
          //trk2->invalidateFit();

          BtaOpMakeTree combD0;
          BtaCandidate D0Cand = combD0.combine(*btaK0S,*trk1,*trk2);
          D0Cand.setType(Pdt::lookup(PdtPdg::D0));
          setMassConstraint(D0Cand);
          //VtxFitterOper D0fit(D0Cand,vtxFitAlgo.value(),vtxFitMode.value());
	  VtxFitterOper D0fit(D0Cand,VtxAbsAlgorithm::Add4,vtxFitMode.value());
          D0fit.fit();
          BtaCandidate fitted_D0 = D0fit.getFittedTree();
          if(fitted_D0.decayVtx()->status()!=BtaAbsVertex::Success) continue;

          HepAListIterator<BtaCandidate> iterTrk3(PionSlowList);
          BtaCandidate* trk3(0);
          while(trk3 = iterTrk3()) {
           //check for overlaps
           if(trk3->overlaps(*trk1) || trk3->overlaps(*trk2)) continue;

	    // combine D0 & pi_slow to obtain the Dstar
	    //=========================================

	    //vertex D0 with pi to create D*
            BtaOpMakeTree combds;
            BtaCandidate* D_star = combds.create(*trk3,fitted_D0);
            D_star->setType((D_star->charge() < 0.0) ? Pdt::lookup(PdtPdg::D_star_minus) : Pdt::lookup(PdtPdg::D_star_plus));
            setGeoConstraint(*D_star);
            setBeamConstraint(*D_star,eventInfo);
            //VtxFitterOper Dstarfit(*D_star,vtxFitAlgo.value(),vtxFitMode.value());
	    VtxFitterOper Dstarfit(*D_star,VtxAbsAlgorithm::Add4,vtxFitMode.value());
            Dstarfit.fit();
            BtaCandidate fitted_Dstar = Dstarfit.getFitted(*D_star);
            if(fitted_Dstar.decayVtx()->status()!=BtaAbsVertex::Success) continue;

	    //cut on m(D*)-m(D0)
            double deltam = fitted_Dstar.mass() - fitted_D0.mass();
            if(deltaMassCutLo.value() < deltam && deltam < deltaMassCutHi.value()) {
	      // If all the conditions are met, we want to pass this event
	      passThisEvent = true;
	      myDstrToKSList->append( new BtaCandidate(*D_star) );
            }
            delete D_star;

          } // end trk3 loop
        } // end K0S loop
      } // n_K0S != 0
    } // end trk2 loop
  } // end trk1 loop


  // Now report whether this event passes the skim or not
  if( passThisEvent == false ) {

    setPassed( false ); // This event did not pass the skim

  } else {

    setPassed( true );  // This event did pass the skim
    _nEventsPassed++;
  
  }
  return AppResult::OK;
}

AppResult D0ToK0PiPiSkim::endJob( AbsEvent* anEvent )
{
  // Print out some statistics at the end
  ErrMsg(routine) << "D0ToK0PiPiSkim:" 
		  << " Read " << _nReadEvents 
		  << " Passed " << _nEventsPassed << endmsg;
  return AppResult::OK;
}

AppResult D0ToK0PiPiSkim::endRun( AbsEvent* anEvent )
{
  return AppResult::OK;
}


HepLorentzVector D0ToK0PiPiSkim::klfi_h(const HepLorentzVector twopi, const HepPoint orig, const HepPoint klCenter){
 
  double m_D0 = Pdt::mass(PdtLund::D0);
  double m_K0L = Pdt::mass(PdtLund::K_L0);
  double m_twopi = twopi.mag();
 
  Hep3Vector klFlight = klCenter - orig;    //From center of the cluster to the PIPI vertex

  double kl_phi = klFlight.phi();
  double kl_theta = klFlight.theta();
 
  //********* 4-momentum conservation *******
  double alpha = (m_D0*m_D0 - m_twopi*m_twopi - m_K0L*m_K0L)/2.0;
  double p_twopi = twopi.rho(); // magnitude of the momentum of pipi system
  // calculation of the angle between the direction of the KL (taken as the
  // vector from the PIPI vertex point to the center of the cluster)
  // and the direction of the PIPI system momentum
  double costh = twopi.x()*cos(kl_phi)*sin(kl_theta) + twopi.y()*sin(kl_phi)*sin(kl_theta) + twopi.z()*cos(kl_theta);
  costh = costh/p_twopi;
  // solve quadratic equation
  double a = twopi.e()*twopi.e() - p_twopi*p_twopi*costh*costh;
  double b = -2.0*alpha*p_twopi*costh;
  double c = twopi.e()*twopi.e()*m_K0L*m_K0L - alpha*alpha;
  //
  double delta = b*b - 4.0*a*c;
  double p_K0L_1 = 0;
  double p_K0L_2 = 0;
  double p_K0L = 0;
  double e_K0L = 0;                                  
  if(delta >= 0.) {
    p_K0L_1 = (-b + sqrt(delta))/(2.0*a);
    p_K0L_2 = (-b - sqrt(delta))/(2.0*a);
  }
  if(p_K0L_1<0) p_K0L_1=0;
  if(p_K0L_2<0) p_K0L_2=0;
  if (p_K0L_1 > p_K0L_2) {
    p_K0L=p_K0L_1;
  } else {
    p_K0L=p_K0L_2;
  }
  if(p_K0L>0) e_K0L = sqrt(p_K0L*p_K0L+m_K0L*m_K0L);
  HepLorentzVector NH_4mom(p_K0L*cos(kl_phi)*sin(kl_theta),p_K0L*sin(kl_phi)*sin(kl_theta),p_K0L*cos(kl_theta),e_K0L);
  return NH_4mom;
}


HepLorentzVector D0ToK0PiPiSkim::klfi_l(const HepLorentzVector twopi, const HepPoint orig, const HepPoint klCenter){
 
  double m_D0 = Pdt::mass(PdtLund::D0);
  double m_K0L = Pdt::mass(PdtLund::K_L0);
  double m_twopi = twopi.mag();
 
  Hep3Vector klFlight = klCenter - orig;    //From center of the cluster to the PIPI vertex

  double kl_phi = klFlight.phi();
  double kl_theta = klFlight.theta();
 
  //********* 4-momentum conservation *******
  double alpha = (m_D0*m_D0 - m_twopi*m_twopi - m_K0L*m_K0L)/2.0;
  double p_twopi = twopi.rho(); // magnitude of the momentum of pipi system
  // calculation of the angle between the direction of the KL (taken as the
  // vector from the PIPI vertex point to the center of the cluster)
  // and the direction of the PIPI system momentum
  double costh = twopi.x()*cos(kl_phi)*sin(kl_theta) + twopi.y()*sin(kl_phi)*sin(kl_theta) + twopi.z()*cos(kl_theta);
  costh = costh/p_twopi;
  // solve quadratic equation
  double a = twopi.e()*twopi.e() - p_twopi*p_twopi*costh*costh;
  double b = -2.0*alpha*p_twopi*costh;
  double c = twopi.e()*twopi.e()*m_K0L*m_K0L - alpha*alpha;
  //
  double delta = b*b - 4.0*a*c;
  double p_K0L_1 = 0;
  double p_K0L_2 = 0;
  double p_K0L = 0;
  double e_K0L = 0;                                  
  if(delta >= 0.) {
    p_K0L_1 = (-b + sqrt(delta))/(2.0*a);
    p_K0L_2 = (-b - sqrt(delta))/(2.0*a);
  }
  if(p_K0L_1<0) p_K0L_1=0;
  if(p_K0L_2<0) p_K0L_2=0;
  if (p_K0L_1 > p_K0L_2) {
    p_K0L=p_K0L_2;        
  } else {
    p_K0L=p_K0L_1;
  }
  if(p_K0L>0) e_K0L = sqrt(p_K0L*p_K0L+m_K0L*m_K0L);
  HepLorentzVector NH_4mom(p_K0L*cos(kl_phi)*sin(kl_theta),p_K0L*sin(kl_phi)*sin(kl_theta),p_K0L*cos(kl_theta),e_K0L);
  return NH_4mom;
}


