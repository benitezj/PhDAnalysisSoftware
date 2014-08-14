//-----------------------------------------------------------------------
// BrecoQa
//      $Id: BrecoQa.cc,v 1.18 2005/06/17 00:12:59 poireau Exp $
// N. Danielson
// Outputs Breco test histograms
//------------------------------------------------------------------------

#include "BaBar/BaBar.hh"
#include "FilterTools/BrecoQa.hh"

#include <assert.h>
#include "AbsEnv/AbsEnv.hh"
#include "AbsEventTag/AbsEventTag.hh"
#include "BbrGeom/BbrDoubleErr.hh"
#include "Beta/BtaCandidate.hh"
#include "BetaCoreTools/BtaBVariables.hh"
#include "BetaCoreTools/BtaDeltaTConverter.hh"
#include "BetaCoreTools/BtaExclusiveDecayList.hh"
#include "BetaMicroBase/BtaAttributes.hh"
#include "GenEnv/GenEnv.hh"
#include "HepTuple/Tuple.h"
#include "HepTuple/TupleManager.h"
#include "HepTuple/Histogram.h"
#include "PDT/Pdt.hh"
using std::cout;
using std::endl;


BrecoQa::BrecoQa( 
    const char* const theName, 
    const char* const theDescription )
    : AppModule( theName, theDescription )
  , _deCut    ("deCut"     , this, 0.08)
  ,_attributes(0)
  ,_decayList(0)
{
  commands( )->append(& _deCut );
  
}

BrecoQa::~BrecoQa( )
{
  delete _decayList;

}

AppResult BrecoQa::beginJob( AbsEvent* anEvent )
{
  // Book histos...
  HepTupleManager *manager = gblEnv->getGen()->ntupleManager(); 
  if (manager == 0) {
    cout << "BrecoQa::beginRun: ntuple manager doesn't exist!" << endl;
    assert (manager != 0);  
  }
  
  // Book histograms...
  double mesMin = 5.2;
  double mesMax = 5.3;
  double dMassMin = 1.8;
  double dMassMax = 1.9;
  double dStarMin = 0.135;
  double dStarMax = 0.15;
  
  _h_bestCandDecay          = manager->histogram("Decay type"         , 8   ,  0  , 8);


  // mes...
  _h_mesB0                  = manager->histogram("M_{ES} (B^{0} BReco modes)"         , 50   ,  mesMin  , mesMax);
  _h_mesDstar               = manager->histogram("M_{ES} (D^{*} BReco modes)"       , 50   ,  mesMin  , mesMax);
  _h_mesDstarPi             = manager->histogram("M_{ES} (D^{*}#pi BReco modes)"    , 50   ,  mesMin  , mesMax);
  _h_mesNonDstar            = manager->histogram("M_{ES} (Non-D^{*} BReco modes)"   , 50   ,  mesMin  , mesMax);
  _h_mesPi0                 = manager->histogram("M_{ES} (#pi^{0} BReco modes)"     , 50   ,  mesMin  , mesMax);
  _h_mesKs                  = manager->histogram("M_{ES} (K_{S} BReco modes)"       , 50   ,  mesMin  , mesMax);

  // de...
  _h_deB0                   = manager->histogram("#Delta E (B^{0} BReco modes)"                , 40   , -_deCut.value()  , _deCut.value());
  _h_deDstar                = manager->histogram("#Delta E (D^{*} BReco modes)"              , 40   , -_deCut.value()  , _deCut.value());
  _h_deDstarPi              = manager->histogram("#Delta E (D^{*}#pi BReco modes)"           , 40   , -_deCut.value()  , _deCut.value());
  _h_deNonDstar             = manager->histogram("#Delta E (Non-D^{*} BReco modes)"          , 40   , -_deCut.value()  , _deCut.value());
  _h_dePi0                  = manager->histogram("#Delta E (#pi^{0} BReco modes)"            , 40   , -_deCut.value()  , _deCut.value());
  _h_deKs                   = manager->histogram("#Delta E (K_{S} BReco modes)"              , 40   , -_deCut.value()  , _deCut.value());

  // D mass
  _h_dmass_KsPi             = manager->histogram("Mass(D^{+}) (K_{S} #pi BReco modes)"          , 50   ,  dMassMin  , dMassMax);
  _h_dmass_KPiPi            = manager->histogram("Mass(D^{+}) (K #pi#pi BReco modes)"        , 50   ,  dMassMin  , dMassMax);
  _h_dmass_KPi              = manager->histogram("Mass(D^{0}) (K #pi BReco modes)"           , 50   ,  dMassMin  , dMassMax);
  _h_dmass_KPiPi0           = manager->histogram("Mass(D^{0}) (K #pi#pi^{0} BReco modes)"     , 50   ,  dMassMin  , dMassMax);
  _h_dmass_K3Pi             = manager->histogram("Mass(D^{0}) (K 3#pi BReco modes)"          , 50   ,  dMassMin  , dMassMax);
  _h_dmass_KsPiPi           = manager->histogram("Mass(D^{0}) (K_{S} #pi#pi BReco modes)"       , 50   ,  dMassMin  , dMassMax);

  // M(D*)-M(D0)
  _h_dstarmass_KPi          = manager->histogram("M(D^{*+})-M(D^{0}) (K #pi BReco modes)"      , 50   ,  dStarMin  , dStarMax);
  _h_dstarmass_KPiPi0       = manager->histogram("M(D^{*+})-M(D^{0}) (K #pi#pi^{0} BReco modes)" , 50   ,  dStarMin  , dStarMax);
  _h_dstarmass_K3Pi         = manager->histogram("M(D^{*+})-M(D^{0}) (K 3#pi BReco modes)"    , 50   ,  dStarMin  , dStarMax);
  _h_dstarmass_KsPiPi       = manager->histogram("M(D^{*+})-M(D^{0}) (K_{S} #pi#pi BReco modes)" , 50   ,  dStarMin  , dStarMax);

  // Misc...
  _h_recoFlavB0            = manager->histogram("Reco Flavor (B^{0} BReco modes)"         , 2    ,  0    , 2);
  _h_modeB0                = manager->histogram("B^{0} BReco Mode"                        , 40   ,  3100 , 3140);
  
  // Used for getting decay tree...
  _decayList = new BtaExclusiveDecayList();
  
  return AppResult::OK;
}


AppResult BrecoQa::event( AbsEvent* anEvent ) {

  _attributes  = Ifd<BtaAttributes>::get(anEvent, "Default");
  if ( _attributes == 0 ) {
    ErrMsg(debugging) << "Making new BtaAttributes under key, "
                     << "Default" << endmsg;
    BtaAttributes* at=new BtaAttributes;
    IfdDataProxy<BtaAttributes> *proxyA = new IfdDataProxy<BtaAttributes>(at);
    if ( ! Ifd< BtaAttributes >::put( anEvent, proxyA,"Default" ) ) {
      ErrMsg(fatal) << "Cannot put attributes in the event, aborting." << endmsg;
    }
    _attributes  = Ifd<BtaAttributes>::get(anEvent, "Default");
  }

  // Get BFlav lists...
  HepAList<BtaCandidate>* B0ToDstarPiList        = Ifd< HepAList<BtaCandidate> >::get( anEvent, "B0ToDstarPiBFlavCTT");
  HepAList<BtaCandidate>* B0ToDstarRhoCList      = Ifd< HepAList<BtaCandidate> >::get( anEvent, "B0ToDstarRhoCBFlavCTT");
  HepAList<BtaCandidate>* B0ToDstarA1CList       = Ifd< HepAList<BtaCandidate> >::get( anEvent, "B0ToDstarA1CBFlavCTT"); 
  HepAList<BtaCandidate>* B0ToDPiList            = Ifd< HepAList<BtaCandidate> >::get( anEvent, "B0ToDPiBFlavCTT");
  HepAList<BtaCandidate>* B0ToDRhoCList          = Ifd< HepAList<BtaCandidate> >::get( anEvent, "B0ToDRhoCBFlavCTT");
  HepAList<BtaCandidate>* B0ToDA1CList           = Ifd< HepAList<BtaCandidate> >::get( anEvent, "B0ToDA1CBFlavCTT");

  // Find best candidate and fill ntuple variables...
  BtaCandidate *bestCand = 0;
  processCandList(B0ToDstarPiList   , bestCand, B0TODSTARPI );
  processCandList(B0ToDstarRhoCList , bestCand, B0TODSTARRHOC );
  processCandList(B0ToDstarA1CList  , bestCand, B0TODSTARA1C );
  processCandList(B0ToDPiList       , bestCand, B0TODPI );
  processCandList(B0ToDRhoCList     , bestCand, B0TODRHOC );
  processCandList(B0ToDA1CList      , bestCand, B0TODA1C );

  // Fill histograms and ntuple...
  fillHistograms(bestCand, anEvent);
  
  return AppResult::OK;
}

AppResult BrecoQa::other( AbsEvent* ) 
{
  return AppResult::OK;
}

AppResult BrecoQa::endJob( AbsEvent* anEvent )
{
  return AppResult::OK;
}

AppResult BrecoQa::abortJob( AbsEvent* anEvent ) 
{
  return AppResult::OK;
}

void BrecoQa::processCandList(HepAList<BtaCandidate>* list, BtaCandidate *&bestCand, int listID) 
{
  if (list == 0) return;

  // Iterate over candidates in list...
  HepAListIterator<BtaCandidate> iter(*list);
  BtaCandidate* brecCand;
 
  while ((brecCand = iter())) {

    // de cut...
    BtaBVariables bVars(brecCand->p4WCov());
    double brecDe = bVars.deltaE();
    if (fabs(brecDe) > _deCut.value()) continue;

    // If there are no "best" candidates, this is it.
    if (bestCand == 0) {
      bestCand = brecCand;
      _bestCandDecay = listID;
      continue;
    }
    
    // Check if its de is better than the "best" candidate so far.
    BtaBVariables bestBVars(bestCand->p4WCov());
    if (fabs(brecDe) < fabs(bestBVars.deltaE())) {
      bestCand = brecCand;
      _bestCandDecay = listID;
    }

  } // loop over candidates
  
}

void BrecoQa::fillHistograms(BtaCandidate *brecCand, AbsEvent *anEvent)
{
  if (brecCand == 0) return;
  if (anEvent == 0) return;

  // Get the important variables...
  BtaBVariables bVars(brecCand->p4WCov());
  
  // Reco flavor and Mode...
  int pB0= (brecCand->pdtEntry()->lundId()<0 || brecCand->isCPAmbiguity()) ? 0 : 1;
  int recType = _decayList->analyzeTree(brecCand);
  // Fill histograms...

  // Fill the histogram to know which decay was involved
  _h_bestCandDecay->accumulate(_bestCandDecay);
  
  if (recType >= 3101 && recType <= 3124) {
  _h_mesB0     ->accumulate(bVars.energySubstitutedMass());
  _h_deB0      ->accumulate(bVars.deltaE());
  _h_recoFlavB0->accumulate(pB0);
  _h_modeB0    ->accumulate(recType);
  }

  // D* modes...
  if (recType >=3101 && recType <=3118) {
    _h_mesDstar->accumulate(bVars.energySubstitutedMass());
    _h_deDstar->accumulate(bVars.deltaE());
  }
  
  // D* Pi Modes (very clean)
  if (recType >=3101 && recType <=3106) {
    _h_mesDstarPi->accumulate(bVars.energySubstitutedMass());
    _h_deDstarPi->accumulate(bVars.deltaE());
  }

  // Non D* modes...
  if (recType >=3119 && recType <= 3124){
    _h_mesNonDstar->accumulate(bVars.energySubstitutedMass());
    _h_deNonDstar->accumulate(bVars.deltaE());
  }
  
  // Modes with a pi0 (or rho+ -> pi0 pi+)
  if (recType == 3102 ||
      recType == 3105 ||
      recType == 3106 ||
      (recType >=3107 && recType <= 3112) ||
      recType == 3114 ||
      recType == 3117 ||
      recType == 3118 ||
      recType == 3121 ||
      recType == 3122) {
    _h_mesPi0->accumulate(bVars.energySubstitutedMass());
    _h_dePi0->accumulate(bVars.deltaE());
  }

  // Modes with a Ks
  if (recType == 3104 ||
      recType == 3106 ||
      recType == 3110 ||
      recType == 3112 ||
      recType == 3116 ||
      recType == 3118 ||
      recType == 3120 ||
      recType == 3122 ||
      recType == 3124) {
    _h_mesKs->accumulate(bVars.energySubstitutedMass());
    _h_deKs->accumulate(bVars.deltaE());
  }
  
  // D mass for D- -> KsPi modes
  if (recType == 3106 || recType == 3112 || recType == 3118 ||
      recType == 3120 || recType == 3122 || recType == 3124) {
    fillDMassHisto(PdtLund::D_plus, *brecCand, _h_dmass_KsPi, anEvent);
  }

  // D mass for D- -> KPiPi modes
  if (recType == 3105 || recType == 3111 || recType == 3117 ||
      recType == 3119 || recType == 3121 || recType == 3123) {
    fillDMassHisto(PdtLund::D_plus, *brecCand, _h_dmass_KPiPi, anEvent);
  }

  // D0 mass for D0 -> KPi modes
  if (recType == 3101 || recType == 3107 || recType == 3113) {
    fillDMassHisto(PdtLund::D0, *brecCand, _h_dmass_KPi, anEvent);
  }

  // D mass for D0 -> KPiPi0 modes
  if (recType == 3102 || recType == 3108 || recType == 3114) {
    fillDMassHisto(PdtLund::D0, *brecCand, _h_dmass_KPiPi0, anEvent);
  }

  // D mass for D0 -> K3Pi modes
  if (recType == 3103 || recType == 3109 || recType == 3115) {
    fillDMassHisto(PdtLund::D0, *brecCand, _h_dmass_K3Pi, anEvent);
  }


  // D*- mass for D0 -> KPi 
  if (recType == 3101 || recType == 3107 || recType == 3113) {
    fillDstarHisto(PdtLund::D_star_plus, *brecCand, _h_dstarmass_KPi, anEvent);
  }
  
  // D*- mass for D0 -> KPiPi0
  if (recType == 3102 || recType == 3108 || recType == 3114) {
    fillDstarHisto(PdtLund::D_star_plus, *brecCand, _h_dstarmass_KPiPi0, anEvent);
  }

  // D*- mass for D0 -> K3Pi
  if (recType == 3103 || recType == 3109 || recType == 3115) {
    fillDstarHisto(PdtLund::D_star_plus, *brecCand, _h_dstarmass_K3Pi, anEvent);
  }

  // D*- mass for D0 -> KsPiPi
  if (recType == 3104 || recType == 3110 || recType == 3116) {
    fillDstarHisto(PdtLund::D_star_plus, *brecCand, _h_dstarmass_KsPiPi, anEvent);
  }

  
}

void BrecoQa::fillDstarHisto(PdtLund::LundType particle, BtaCandidate brecCand, HepHistogram *histo, AbsEvent *anEvent)
{
  // Fills D* delta m histogram.
  const BtaCandidate *dstar = 0;
  const BtaCandidate *dCand = 0;
  
  // Get dstar...
  dstar = brecCand.getDaughter(particle);
  if (dstar == 0) dstar = brecCand.getDaughter(Pdt::antiCode(particle));
  if (dstar == 0) {
    cout << "BrecoQa Error: No D* in this event" << endl;
    return;
  }
  
  // Get its daughter d
  dCand = dstar->getDaughter(PdtLund::D0);
  if (dCand == 0) dCand = dstar->getDaughter(PdtLund::anti_D0);
  if (dCand == 0) {
    cout << "BrecoQa Error: D* doesn't decay into D0 as expected." << endl;
    return;
  }
  
  // At this point dstar is a pointer to the mass constrained Dstar candidate
  // We want the non mass constrained D candidate.
  HepAList<BtaCandidate> *vtxdslist = Ifd< HepAList<BtaCandidate> >::get( anEvent, "DstarVtxBFlav");
   
  // Find this D in the list...
 if (vtxdslist == 0) return;
  BtaCandidate* vtxds(0) ;
  HepAListIterator<BtaCandidate> vtxdsiter(*vtxdslist) ;
  while( (vtxds=vtxdsiter() ) ) {
    if( vtxds->isCloneOf( *dstar, true ) ) break ;
  }
  
  if (vtxds == 0) return;
  
  // And the un mass constrained d
  HepAList<BtaCandidate> *vtxdlist = Ifd< HepAList<BtaCandidate> >::get( anEvent, "D0VtxBFlav");
  
  // Find this D in the list...
 if (vtxdlist == 0) return;
  BtaCandidate* vtxd(0) ;
  HepAListIterator<BtaCandidate> vtxditer(*vtxdlist) ;
  while( (vtxd=vtxditer() ) ) {
    if( vtxd->isCloneOf( *dCand, true ) ) break ;
  }
  
  if (vtxd == 0) return;


  histo->accumulate(vtxds->mass()-vtxd->mass());
}
  

void BrecoQa::fillDMassHisto(PdtLund::LundType particle, BtaCandidate brecCand, HepHistogram *histo, AbsEvent *anEvent)
{
  // Fills D mass histogram for D+/D-, D0.

  // Start going down the B decay chain till we find a particle of type "particle"
  const BtaCandidate *dCand = 0;
  const BtaCandidate *dstar = 0;

  // Since this function can be used for D*+/- and D+/-, first find out which we have...
  // We allow for the possibility it decays into "particle" or first into a D*, which
  // then decays to "particle".
  
  dCand = brecCand.getDaughter(particle);
  if (dCand == 0) dCand = brecCand.getDaughter(Pdt::antiCode(particle));
  if (dCand == 0) {
    dstar = brecCand.getDaughter(PdtLund::D_star_plus);
    if (dstar == 0) dstar = brecCand.getDaughter(PdtLund::D_star_minus);
    if (dstar == 0) dstar = brecCand.getDaughter(PdtLund::D_star0);
    if (dstar == 0) dstar = brecCand.getDaughter(PdtLund::anti_D_star0);
    if (dstar == 0) {
      cout << "Error: B candidate doesn't decay into desired particle nor D*+-" << endl;
      return;
    }    
    dCand = dstar->getDaughter(particle);
    if (dCand == 0) dCand = dstar->getDaughter(Pdt::antiCode(particle));
    if (dCand == 0) {
      cout << "Error: Neither B candidate nor D* decays into desired particle" << endl;
      return;
    }
  }

  // At this point dCand is a pointer to the mass constrained D candidate
  // We want the non mass constrained D candidate.
  HepAList<BtaCandidate>* vtxlist(0) ;
  if(dCand->pdtEntry()->lundId() == PdtLund::D_plus || dCand->pdtEntry()->lundId() == PdtLund::D_minus) {
    vtxlist = Ifd< HepAList<BtaCandidate> >::get( anEvent, "DcVtxBFlav");
  } else if(dCand->pdtEntry()->lundId() == PdtLund::D0 || dCand->pdtEntry()->lundId() == PdtLund::anti_D0) {
    vtxlist = Ifd< HepAList<BtaCandidate> >::get( anEvent, "D0VtxBFlav");
  }

  if (vtxlist == 0) return;
  BtaCandidate* vtxd(0) ;
  HepAListIterator<BtaCandidate> vtxiter(*vtxlist) ;
  while( (vtxd=vtxiter() ) ) {
    if( vtxd->isCloneOf( *dCand, true ) ) break ;
  }
  
  if (vtxd == 0) return;
  
  histo->accumulate(vtxd->mass());
}









