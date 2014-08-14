//--------------------------------------------------------------------------
// File and Version Information:
// $Id: CharmSLFilter.cc,v 1.3 2007/01/10 16:36:26 marchior Exp $
//
// Description:
// Charm events with a semileptonic decay.  
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Orsay                        Original Author
//
// Copyright Information:
//	Copyright (C) 2002              UVIC
//
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"
//-----------------------
// This Class's Header --
//-----------------------

#include "FilterTools/CharmSLFilter.hh"

// also defines the class variables

//---------------
// C++ Headers --
//---------------
#include <assert.h>
#include <iostream>
#include <math.h>

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "AbsEnv/AbsEnv.hh"
#include "AbsEvent/AbsEventID.hh"
#include "AbsEvent/AbsEvent.hh"
#include "Beta/EventInfo.hh"
#include "Beta/BtaCandidate.hh"
#include "Beta/BtaConstraint.hh"
#include "Beta/BtaMcTruth.hh"
#include "BetaMicroAdapter/BtaCalQual.hh"
#include "BetaMicroAdapter/BtaTrkQual.hh"
#include "ProbTools/probab.hh"
#include "PDT/Pdt.hh"
#include "BetaCoreTools/BtaPrintTree.hh"
#include "BetaCoreTools/BtaOpMakeTree.hh"
#include "BetaCoreTools/BtaOpAdd4.hh"
#include "BetaCoreTools/BtaMcAssoc.hh"
#include "BetaCoreTools/BtaFoxWolfMom.hh"
#include "BetaCoreTools/BtaBooster.hh"
#include "BetaCoreTools/BtaThrust.hh"
#include "BetaCoreTools/BtaThrustFitter.hh"
#include "BetaCoreTools/BtaLgndrMomFlow.hh"
#include "CLHEP/Alist/AList.h"
#include "CLHEP/Alist/AIterator.h"
#include "ErrLogger/ErrLog.hh"

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

//----------------
// Constructors --
//----------------

// in general, a module constructor should not do much.  The begin(job) or
// begin(run) members are better places to put initialization
//
// This ctor initializes the three sample list-name parameters
CharmSLFilter::CharmSLFilter( const char* const theName, 
			const char* const theDescription )
  : TagFilterModule( theName, theDescription )
  , _eventInfoList(        "eventInfoList",   this, "Default"  )
  , _chargedList(          "chargedList",     this, "ChargedTracks")
  , _neutralList(          "neutralList",     this, "GoodPhotonDefault")
  , _electronList(         "electronList",    this, "PidLHElectrons")
  , _plepCut(              "plepCut",         this, 0.5)
  , _MoppCut(              "MoppCut",         this, 0.5)
  , _h2h0Cut(              "h2h0Cut",         this, 0.2)
  , _MtagCut(              "MtagCut",         this, 0.13)
{
  commands()->append( &_eventInfoList );
  commands()->append( &_chargedList);
  commands()->append( &_neutralList);
  commands()->append( &_electronList);
  commands()->append( &_plepCut);
  commands()->append( &_MoppCut);
  commands()->append( &_h2h0Cut);
  commands()->append( &_MtagCut);
}

//--------------
// Destructor --
//--------------

// The destructor should be limited to undoing the work of the constructor
CharmSLFilter::~CharmSLFilter( )
{
}

//--------------
// Operations --
//--------------

// The begin(AppJob*) member function is run before any events are
// processed.  

AppResult
CharmSLFilter::beginJob( AbsEvent* anEvent )
{
    ErrMsg(routine)<<"begin Job"<<endmsg;
    _NEvents = 0;
    _NPassed = 0;

    return AppResult::OK;
}

AppResult
CharmSLFilter::event( AbsEvent* anEvent )
{
  bool result = pass( anEvent );
  setPassed( result );
  return AppResult::OK;
  cout << "event AppResult::OK" << endl;
}

bool
CharmSLFilter::pass( AbsEvent* anEvent ) 
{
  
  // Input Lists:

  // Event Info: 
  HepAList< EventInfo >* EvtInfoList =
    Ifd<HepAList< EventInfo > >::get(anEvent, _eventInfoList.value() );
  if (EvtInfoList == 0) ErrMsg(fatal) 
    << "eventInfoList not in event" << endmsg;
  EventInfo* EvtInfo = EvtInfoList->first();
  if (EvtInfo == 0) ErrMsg(fatal) 
    << "Nothing in eventInfoList...aborting" << endmsg;
  if (EvtInfo == 0) {  return false; }

  // neutral clusters (for thrust calculation):
  HepAList<BtaCandidate>* neutrals =
    Ifd<HepAList<BtaCandidate> >::get(anEvent, _neutralList.value());
  if (neutrals == 0) ErrMsg(fatal)  << _neutralList.value() 
				    << " list not found" << endmsg;
  
  // charged tracks 
  HepAList<BtaCandidate>* chargedTracks =
    Ifd<HepAList<BtaCandidate> >::get(anEvent, _chargedList.value());
  if (chargedTracks == 0) ErrMsg(fatal)  << _chargedList.value() 
					 << " list not found" << endmsg;
  
  // electrons:
  
  HepAList<BtaCandidate>* electrons =
    Ifd<HepAList< BtaCandidate > >::get(anEvent,_electronList.value());
  if (electrons == 0) ErrMsg(fatal)  << _electronList.value() 
				     << " list not found" << endmsg;
  
  // all particles: charged + neutrals Lists
  
  HepAList<BtaCandidate> allparticles;
  if (chargedTracks)  allparticles+=*chargedTracks;
  if (neutrals) allparticles+=*neutrals; 

  // -----------------------------------------------------------------------------
  
  // static pdtTypes
  const static PdtEntry* piPlus    = Pdt::lookup( PdtPdg::pi_plus );
  const static PdtEntry* piMinus   = Pdt::lookup( PdtPdg::pi_minus );
  
  // cm boost  
  Hep3Vector cmBoost(-((EvtInfo->cmFrame()).boostVector()));
  
  // -----------------------------------------------------------------------------
 
  _NEvents++;

  // Compute the thrust of the event

  BtaThrust th(allparticles,*EvtInfo, BtaThrust::BTAllParticles, false,
               BtaThrust::BTFitter);
  Hep3Vector thruaxis=th.thrust_axis();

  // Compute moments of Fox Wolfram

  int chargedmult = chargedTracks->length();
  int mult=allparticles.length();

  double h2h0=-999.;
  if (mult==0) {} else {
    BtaFoxWolfMom FoxWolfMom( &allparticles , EvtInfo);
    h2h0 = double(FoxWolfMom.FWnorm(2));
  }

  double h2h0ch=-999.;
  if (chargedmult==0) {} else {
    BtaFoxWolfMom ChFoxWolfMom( chargedTracks , EvtInfo);
    h2h0ch = double(ChFoxWolfMom.FWnorm(2));
  }
 
  // Hadronic selection:  R2ch <= 0.98, at least 3 charged tracks 
  
  // if(chargedmult < 3) return false;  
  // if(h2h0ch > 0.98) return false;  
  
  // Examine if there are identified electrons, with minimal p

  double tagcut=-1.;
  double mtag=-1.;
  double mopp=-1.;
  double cltropp=-1.;
  double cltrspec=-1.;
  double nltag=0.;
  
  HepAListIterator<BtaCandidate> lepItertag(*electrons);
  BtaCandidate* leptontag;  
  
  // Loop on lepton candidates  (eventually only electrons)
  while (leptontag=lepItertag()){
    
    // Avoiding the leptons not in allparticles list 
    int checkinall=0;
    HepAListIterator<BtaCandidate> allIter(allparticles);
    while (BtaCandidate* allt = allIter())
      {if(leptontag->uid()==allt->uid()){checkinall++;}}          
    if (checkinall==0) continue;
    //--    

    HepLorentzVector lups4 = leptontag->p4();
    lups4.boost(cmBoost);
    double plcm = lups4.vect().mag() ; 
    if(plcm < _plepCut.value()) continue;         

    nltag++;
    tagcut=h2h0;
    Hep3Vector p3lcm = lups4.vect();      
    double cosal= p3lcm.dot(thruaxis);
    int heml = -999;
    heml = (cosal >= 0.) ? 0 : 1;  
	      
    // Loop on all tracks, but the lepton
    BtaCandidate* itrack;
    HepAListIterator<BtaCandidate> itrackIter(allparticles);
    
    HepLorentzVector p4_heml, p4_hemo, p4_leadch;
    HepLorentzVector p4_leadgam1, p4_leadgam2;
    double pleadch=0., pleadgam1=0., pleadgam2=0.;
    
    while (itrack=itrackIter() ){
      
      //Suppose that charged tracks are pions
      itrack->setType((itrack->charge()) < 0 ? piMinus : piPlus);    
      HepLorentzVector itcm = itrack->p4();
      itcm.boost(cmBoost);
      int hemit=-999;
      Hep3Vector p3itcm = itcm.vect();      
      double cosit= p3itcm.dot(thruaxis);
      hemit = (cosit >= 0.) ? 0 : 1; 
      
      // Hemisphere du lepton
      if(hemit==heml) {
	p4_heml += itcm;
	double psp = p3itcm.mag();
	// Rechercher les leadings charges et neutres		  
	if (itrack->charge() != 0 && itrack->uid() != leptontag->uid()){
	  if(psp > pleadch){
	    pleadch = psp;
	    p4_leadch = itcm;
	  }
	}
	if (itrack->charge() == 0){
	  //les deux leadings gammas
	  if(psp > pleadgam1){
	    pleadgam1 = psp;
	    p4_leadgam1 = itcm;
	  }else if(psp > pleadgam2){
	    pleadgam2 = psp;
	    p4_leadgam2 = itcm;
	  }  
	}
	
	// Hemisphere oppose au lepton
	
      }else {p4_hemo += itcm;}
      
    } // Fin de la boucle sur les traces
    
    // Calcul de la masse lepton - leading
    HepLorentzVector p4_leadgam =  p4_leadgam1 + p4_leadgam2;
    //Keep only the leading gamma if the g1g2 mass is too large
    if(p4_leadgam.mag() >= .2){p4_leadgam = p4_leadgam1;}
    HepLorentzVector p4_ltr;
    // Ajouter le lepton et la tr chargée ou la tr neutre suivant 
    //l'impulsion de cette derniere
    if(p4_leadch.vect().mag() >p4_leadgam.vect().mag() ){
      p4_ltr = lups4 + p4_leadch; 
    }else{
      p4_ltr = lups4 + p4_leadgam; 
    }
    double massltr = p4_ltr.mag();
    // Keep only the highest l-tr mass in the event
    if(massltr < mtag) continue;
    mtag = massltr;
    // Mass of opposite hemisphere
    mopp = p4_hemo.mag();
    // Angle between the l-tr system and the opposite hemisphere 
    if(p4_hemo.vect().mag() >0){
      cltropp = -p4_ltr.vect().dot(p4_hemo.vect())
	/p4_hemo.vect().mag()/p4_ltr.vect().mag();
    }
    // Angle between the l-tr system and the spectator system
    p4_heml = p4_heml - p4_ltr;
    if(p4_heml.vect().mag() >0){
      cltrspec = p4_ltr.vect().dot(p4_heml.vect())
	/p4_heml.vect().mag()/p4_ltr.vect().mag();
    }
    
    // Passing the selected events:
    
    if(mopp > _MoppCut.value() &&
       h2h0 > _h2h0Cut.value() && 
       mtag > _MtagCut.value() ){ 
      
      _NPassed++;
      return true;}
    
  } // End loop on cand leptons
  
  // no good lepton found 
  return false;                                                
}

AppResult
CharmSLFilter::endJob( AbsEvent* anEvent )
{
  ErrMsg(routine)<<" end Job" << endmsg;
  ostream& o= cout;
  
  o << "\nl ************************************ ";
  o << "\nl Numbers of Events " << _NEvents;
  o << "\nl ************************************ ";
  o << "\nl Passing the skim " << _NPassed;
  o << "\nl ************************************ " << endl;

  return AppResult::OK;
}
