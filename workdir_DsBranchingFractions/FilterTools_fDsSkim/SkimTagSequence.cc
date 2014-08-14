//--------------------------------------------------------------------------
// File and Version Information:
//  $Id: SkimTagSequence.cc,v 1.108 2008/11/26 22:33:26 dubrovin Exp $
//
// Description:
//  function SkimTagSequence
//
//      This function will add the "standard" set of modules
//      to a user executable.
//
// Environment:
//  Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//  Bob Jacobsen        Original Author
//      Yury Kolomensky                 Implementation for FilterTools
//
// Copyright Information:
//  Copyright (C) 1996    Lawrence Berkeley Laboratory
//      Copyright (C) 2000    California Institute of Technology
//
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "FilterTools/SkimTagSequence.hh"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "Framework/AppUserBuild.hh"

//-----------------------------------------
// include files for user filter modules --
//-----------------------------------------
#include "EventTagTools/TagFilterDstar.hh"
#include "EventTagTools/TagFilterMomentumCut.hh"
#include "EventTagTools/TagFilterEnergy.hh"
#include "TagModules/TagFilterByName.hh"
#include "TagModules/TagBitReset.hh"
#include "TagModules/TagFilterByPrescale.hh"
#include "TagModules/TagFilterMultiModule.hh"
#include "BetaPidCalib/BtaPidCalibAllSequence.hh"
#include "BetaTools/BtaCountFilter.hh"
#include "NuuReco/NuuReco.hh"

#include "FilterTools/SkimPathNtupleModule.hh"
#include "FilterTools/FilterByList.hh"
#include "FilterTools/ISLTagFilter.hh"
#include "FilterTools/ExclEtaPFilter.hh"
#include "FilterTools/A0Filter.hh"
#include "FilterTools/InclEtaFilter.hh"
#include "FilterTools/InclK0sFilter.hh"
#include "FilterTools/InclOmegaFilter.hh"
#include "FilterTools/InclEtapRhoGamFilter.hh"
#include "FilterTools/B3bodyTagFilter.hh"
#include "FilterTools/B3body03aTagFilter.hh"
#include "FilterTools/FilterBToPPP.hh"
#include "FilterTools/FilterBToPPPP.hh"
#include "FilterTools/TagBFourBodyFilter.hh"
#include "FilterTools/InclPhiFilter.hh"
#include "FilterTools/BtoXGammaPi0Filter.hh"
#include "FilterTools/BtoXGammaFilter.hh"
#include "FilterTools/BtoGammaGammaFilter.hh"
#include "FilterTools/BtoRhoGammaFilter.hh"
#include "FilterTools/BtoRhoGammaBtoXGF.hh"
#include "FilterTools/BlnuTagFilter.hh"
#include "FilterTools/DslnuccTagFilter.hh"
#include "FilterTools/IsrTagbitFilter.hh"
#include "FilterTools/KllTagFilter.hh"
#include "FilterTools/BPiDstarTagFilter.hh"
#include "FilterTools/DcToKpipiTagFilter.hh"
#include "FilterTools/TagSkimCounter.hh"
#include "FilterTools/DtoXllTagFilter.hh"
#include "FilterTools/BBaryonic3bodyTagFilter.hh"
#include "FilterTools/B0ToD0KPiTagFilter.hh"
#include "FilterTools/XSLBtoXulnuFilter.hh"
#include "FilterTools/BToDstarlnuXFilter.hh"
#include "FilterTools/Dto3PSFilter.hh"
#include "FilterTools/ExclHllMiniFilter.hh"
#include "FilterTools/BrecoQa.hh"
#include "FilterTools/TwoPhotonTwoTrackTagFilter.hh"
#include "FilterTools/TwoPhotonPi0Pi0TagFilter.hh"
#include "FilterTools/TwoPhotonPentaquarkTagFilter.hh"
#include "FilterTools/DmixD0ToKPiPi0Skim.hh"
#include "FilterTools/DmixD0ToK3PiSkim.hh"
#include "FilterTools/D0To2ProngDcjSkim.hh"
#include "FilterTools/D0To2ProngPi0CSSkim.hh"
#include "FilterTools/DstD0ToKsHpHmSkim.hh"
#include "FilterTools/DstD0ToKsKPiSkim.hh"
#include "FilterTools/DmixD0ToKenuSkim.hh"
#include "FilterTools/DcToKpipiPromptSkim.hh"
#include "FilterTools/DcToKKKPromptSkim.hh"
#include "FilterTools/DcTo3PiPromptSkim.hh"
#include "FilterTools/DmixD0ToKmunuSkim.hh"
#include "FilterTools/AntiDeuteronFilter.hh"
#include "FilterTools/DsppbarFilter.hh"
#include "FilterTools/BtoDPiPiTagFilter.hh"
#include "FilterTools/BtoDPiPiPiTagFilter.hh"
#include "FilterTools/B0ToDstarA1CLoose.hh"
#include "FilterTools/RbD0ToKenuSkim.hh"
#include "FilterTools/RbD0ToKmunuSkim.hh"
#include "FilterTools/DsToKpi0Skim.hh"
#include "FilterTools/ExclMultiTracksHad.hh"
#include "FilterTools/InclppbarFilter.hh"
#include "FilterTools/InclProtTightSkim.hh"
#include "FilterTools/TrkEffTau31Skim.hh"
#include "FilterTools/HardElectronFilter.hh"
#include "FilterTools/CharmSLFilter.hh"
#include "FilterTools/JpsitollFilter.hh"
#include "FilterTools/B0ToDstarhKlnuSkim.hh"
#include "FilterTools/Jpsi_Psi2sToChChGammaFilter.hh"
#include "FilterTools/D0ToK0PiPiSkim.hh"



//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

//----------------
// Constructors --
//----------------

AppSequence* SkimTagSequence(AppUserBuild* forWho)
{

  // common modules

  // generic tag setter module which will be cloned in tcl
  forWho->add(new TagSkimCounter("SkimCounter","skim counter"));

  // generic tag filter module which will be cloned in tcl
  forWho->add(new TagFilterByName("TagFilterByName","filter by name"));
  forWho->add(new TagFilterByPrescale("TagFilterByPrescale","filter by name with prescale"));
  forWho->add(new TagBitReset("TagBitReset","reset tag bits"));

  forWho->add(new FilterByList("FilterByList","filter by candidates lists"));

  forWho->add(new SkimPathNtupleModule("SkimPathNtupleModule", "Creates path correlation ntuple"));

  // Charm AWG filters

  forWho->add(new CharmSLFilter("CharmSLFilter", "Filter Charm s.l. decays"));
  forWho->add(new TagFilterDstar( "TagFilterDstar", "Filter on Dstar integers"));
  forWho->add(new TagFilterMomentumCut( "TagFilterMomentumCut", "Filter on momentum bits"));

  TagFilterModule *tagForDsSkim1 =
  new Dto3PSFilter( "Dto3PSFilterForDs3prongSkim", "Filter on D3PS word");
  TagFilterModule *tagForDsSkim2 =
  new TagFilterMomentumCut( "DsWordFilterForDs3prongSkim", "Filter on Dstar integers");

  TagFilterMultiModule *tagMultiForDsSkim =
  new TagFilterMultiModule( "TagFilterMultiForDs3prongSkim", "Multi module filter for Ds Skim");

  tagMultiForDsSkim->addOrModule( tagForDsSkim1 );
  tagMultiForDsSkim->addOrModule( tagForDsSkim2 );

  forWho->add( tagForDsSkim1 );
  forWho->add( tagForDsSkim2 );
  forWho->add( tagMultiForDsSkim );

  forWho->add(new DmixD0ToKPiPi0Skim("DmixD0ToKPiPi0Skim", "D-mixing D0->K pi pi0 events"));
  forWho->add(new DmixD0ToK3PiSkim("DmixD0ToK3PiSkim", "D-mixing D0->K pi pi+ pi- events"));
  forWho->add(new D0To2ProngDcjSkim("D0To2ProngDcjSkim", "D0 -> K-pi+, K-K+, pi-K+, pi-pi+"));
  forWho->add(new D0To2ProngPi0CSSkim("D0To2ProngPi0CSSkim", "Cabibbo-suppressed D0 -> K+K-pi0, pi+pi-pi0"));
  forWho->add(new DstD0ToKsHpHmSkim("DstD0ToKsHpHmSkim", "D*+ to D0pi+, D0 to KsH+H- + cc"));
  forWho->add(new DstD0ToKsKPiSkim("DstD0ToKsKPiSkim", "D* -> D0 pi, D0 -> Ks K pi, all possible charge combinations"));
  forWho->add(new DcToKpipiPromptSkim("DcToKpipiPromptSkim", "prompt D+ -> K- pi+ pi+"));
  forWho->add(new DcToKKKPromptSkim("DcToKKKPromptSkim", "prompt D+ -> K- K+ K+"));
  forWho->add(new DcTo3PiPromptSkim("DcTo3PiPromptSkim", "prompt D+ -> pi- pi+ pi+"));
  forWho->add(new DmixD0ToKenuSkim("DmixD0ToKenuSkim", "D-mixing D0->K e nu"));
  forWho->add(new DmixD0ToKmunuSkim("DmixD0ToKmunuSkim", "D-mixing D0->K mu nu"));
  forWho->add(new RbD0ToKenuSkim("RbD0ToKenuSkim", "Rb from D0->K e nu"));
  forWho->add(new RbD0ToKmunuSkim("RbD0ToKmunuSkim", "Rb from D0->K mu nu"));
  forWho->add(new D0ToK0PiPiSkim("D0ToK0PiPiSkim", "D*+ -> D0 pi+, D0 -> K0L pi+ pi- or D0 -> K0S pi+ pi-"));


  // Quarkonium filter
  forWho->add(new ExclMultiTracksHad("ExclMultiTracksHad", "MultiHadron Filter for the quarkonium AWG"));

//Jpsitoll skims
forWho->add(new JpsitollFilter("JpsitollFilter", "Filter for Jpsitoll skims")); 
  // TauQED filter
  //forWho->add(new FourProngsTagFilterLoose("FourProngsTagFilterLoose", "4 prong 2-photon events"));

  forWho->add(new AntiDeuteronFilter("AntiDeuteronFilter", "AntiDeuteron Filter fot the PTF group skim"));
  forWho->add(new DsppbarFilter("DsppbarFilter", "Dsppbar Filter fot the PTF group skim"));


  // Inclusive SL filter
  forWho->add(new HardElectronFilter("HardElectronFilter", "Filter hadronic events with high energy electrons"));

  forWho->add(new ExclEtaPFilter("ExclEtaPFilter", "Tag filter for ClHBD group skim"));
  forWho->add(new A0Filter("A0Filter", "Tag filter for ClHBD group skim"));
  forWho->add(new InclEtaFilter("InclEtaFilter", "Tag filter for ClHBD group skim"));
  forWho->add(new InclK0sFilter("InclK0sFilter", "Tag filter for ClHBD group skim"));
  forWho->add(new InclOmegaFilter("InclOmegaFilter", "Tag filter for ClHBD group skim"));
  forWho->add(new InclEtapRhoGamFilter("InclEtapRhoGamFilter", "Tag filter for ClHBD group skim"));
  // classic 3-body charmless
  forWho->add(new B3bodyTagFilter("BCCC3bodyTagFilter","Three body ClHBD tag filter"));
  forWho->add(new B3bodyTagFilter("BKsKsKs3bodyTagFilter","Three body ClHBD tag filter"));
  forWho->add(new B3bodyTagFilter("BPi0Pi0Pi03bodyTagFilter","Three body ClHBD tag filter"));
  forWho->add(new B3bodyTagFilter("BCCPi03bodyTagFilter","Three body ClHBD tag filter"));
  forWho->add(new B3bodyTagFilter("BCCEta3bodyTagFilter","Three body ClHBD tag filter"));
  forWho->add(new B3bodyTagFilter("BCCKs3bodyTagFilter","Three body ClHBD tag filter"));
  forWho->add(new B3bodyTagFilter("BPi0Pi0Ks3bodyTagFilter","Three body ClHBD tag filter"));
  forWho->add(new B3bodyTagFilter("BPi0Pi0C3bodyTagFilter","Three body ClHBD tag filter"));
  forWho->add(new B3bodyTagFilter("BKsKsC3bodyTagFilter","Three body ClHBD tag filter"));
  forWho->add(new B3bodyTagFilter("BKsKsPi03bodyTagFilter","Three body ClHBD tag filter"));
  forWho->add(new B3bodyTagFilter("BCPi0Ks3bodyTagFilter","Three body ClHBD tag filter"));
  forWho->add(new B3bodyTagFilter("BCCKsPi0Pi03bodyTagFilter","Three body ClHBD tag filter with Ks->Pi0Pi0"));

  // new CM2 3-body charmless
  forWho->add(new B3body03aTagFilter("BCCC03a3bodyTagFilter","CM2 3body ClHBD tag filter"));
  forWho->add(new B3body03aTagFilter("BKsKsKs03a3bodyTagFilter","CM2 3body ClHBD tag filter"));
  forWho->add(new B3body03aTagFilter("BPi0Pi0Pi003a3bodyTagFilter","CM2 3body ClHBD tag filter"));
  forWho->add(new B3body03aTagFilter("BCCPi003a3bodyTagFilter","CM2 3body ClHBD tag filter"));
  forWho->add(new B3body03aTagFilter("BCCEta03a3bodyTagFilter","CM2 3body ClHBD tag filter"));
  forWho->add(new B3body03aTagFilter("BCCKs03a3bodyTagFilter","CM2 3body ClHBD tag filter"));
  forWho->add(new B3body03aTagFilter("BPi0Pi0Ks03a3bodyTagFilter","CM2 3body ClHBD tag filter"));
  forWho->add(new B3body03aTagFilter("BPi0Pi0C03a3bodyTagFilter","CM2 3body ClHBD tag filter"));
  forWho->add(new B3body03aTagFilter("BKsKsC03a3bodyTagFilter","CM2 3body ClHBD tag filter"));
  forWho->add(new B3body03aTagFilter("BKsKsPi003a3bodyTagFilter","CM2 3body ClHBD tag filter"));
  forWho->add(new B3body03aTagFilter("BCPi0Ks03a3bodyTagFilter","CM2 3body ClHBD tag filter"));
  forWho->add(new B3body03aTagFilter("BCCKsPi0Pi003a3bodyTagFilter","CM2 3body ClHBD tag filter with Ks->Pi0Pi0"));

  forWho->add(new BBaryonic3bodyTagFilter("BPPC3bodyTagFilter","Three body baryonic ClHBD tag filter"));
  forWho->add(new BBaryonic3bodyTagFilter("BPPKs3bodyTagFilter","Three body baryonic ClHBD tag filter"));
  forWho->add(new BBaryonic3bodyTagFilter("BPPKstar03bodyTagFilter","Three body baryonic ClHBD tag filter"));
  forWho->add(new BBaryonic3bodyTagFilter("BPLC3bodyTagFilter","Three body baryonic ClHBD tag filter"));
  forWho->add(new BBaryonic3bodyTagFilter("BPLKs3bodyTagFilter","Three body baryonic ClHBD tag filter"));
  forWho->add(new BBaryonic3bodyTagFilter("BPLKstar03bodyTagFilter","Three body baryonic ClHBD tag filter"));
  forWho->add(new BBaryonic3bodyTagFilter("BLLC3bodyTagFilter","Three body baryonic ClHBD tag filter"));
  forWho->add(new BBaryonic3bodyTagFilter("BLLKs3bodyTagFilter","Three body baryonic ClHBD tag filter"));
  forWho->add(new BBaryonic3bodyTagFilter("BLLKstar03bodyTagFilter","Three body baryonic ClHBD tag filter"));

  forWho->add(new FilterBToPPPP("FilterBToEtaPiPih",    "Multi-list 4-body tag filter"));
  forWho->add(new FilterBToPPPP("FilterBToPhiPiPih",    "Multi-list 4-body tag filter"));
  forWho->add(new FilterBToPPPP("FilterBToOmegaPiPih",    "Multi-list 4-body tag filter"));
  forWho->add(new FilterBToPPPP("FilterBToEtapPiPih",    "Multi-list 4-body tag filter"));
  forWho->add(new FilterBToPPPP("FilterBToRPiPih",    "Multi-list 4-body tag filter"));
  forWho->add(new FilterBToPPP("FilterBToPPP",    "Multi-list 3-body tag filter"));
  forWho->add(new FilterBToPPP("FilterBToCXX",    "Multi-list 3-body tag filter"));
  forWho->add(new FilterBToPPP("FilterBToPXX",    "Multi-list 3-body tag filter"));
  forWho->add(new FilterBToPPP("FilterBToCPP",    "Multi-list 3-body tag filter"));
  forWho->add(new FilterBToPPP("FilterBToCCC",    "Multi-list 3-body tag filter"));
  forWho->add(new FilterBToPPP("FilterBToKstarXX","Multi-list 3-body tag filter"));
  forWho->add(new FilterBToPPP("FilterBToLambda0XX","Multi-list 3-body tag filter"));
  forWho->add(new FilterBToPPP("FilterBToPP",     "Multi-list 2-body tag filter"));

  forWho->add(new TagBFourBodyFilter("TagBFourBodyFilter", "Tag filter for ClHBD group skim"));
  forWho->add(new InclPhiFilter("InclPhiFilter", "Tag filter for ClHBD group skim"));

  // B to X+gamma Filter
  forWho->add(new BtoXGammaPi0Filter("BtoXGammaPi0Filter", "B to X+Gamma Pi0/eta filter"));
  forWho->add(new BtoXGammaFilter("BtoXGammaFilter", "Tag filter for B to X+Gamma"));
  forWho->add(new BtoGammaGammaFilter("BtoGammaGammaFilter", "Analysis filter B0->GammaGamma"));
  forWho->add(new BtoRhoGammaFilter("BtoRhoGammaFilter", "Analysis filter B0->RhoGamma"));
  forWho->add(new BtoRhoGammaBtoXGF("BtoRhoGammaBtoXGF", "Old Tag filter for B to X+Gamma"));

  // Leptonic b and c decays B->lnu Filter
  forWho->add(new BlnuTagFilter("BlnuTagFilter", "B->lnu Tag filter leptonic b&c group skim"));

  forWho->add(new DslnuccTagFilter("DslnuccTagFilter", "Ds->l nu Tag filter leptonic b&c group skim"));

  // ISR Physics
  forWho->add(new IsrTagbitFilter("IsrTagbitFilter", "Filter on ISR Tagbits"));

  // K(*,etc.)ll filters
  forWho->add(new KllTagFilter("KllTagFilter", "Filter for B->Xll analysis."));
  forWho->add(new ExclHllMiniFilter("ExclHllMiniFilter", "Filter for B->K(etc.)ll analysis."));

  // IHBD filters
  forWho->add(new BPiDstarTagFilter( "BPiDstarTagFilter", "tag D*pi filter"));
  forWho->add(new DcToKpipiTagFilter( "DcToKpipiTagFilter", "tag D+- filter"));
  // D0->ll filter
  forWho->add(new DtoXllTagFilter( "DtoXllTagFilter", "Filter for D->Xll analysis"));

  forWho->add(new B0ToD0KPiTagFilter( "B0ToD0KPiTagFilter", "tag B0ToD0KPi filter"));

  //Semileptonic b->u filter
  forWho->add(new XSLBtoXulnuFilter("XSLBtoXulnuFilter", "filter module for exlusives B->Xulnu decays"));

  //Semileptonic b->c filter
  forWho->add(new BtaCountFilter("BToDlnuXFilter", "filter module for B -> D l nu X"));
  forWho->add(new BToDstarlnuXFilter("BToDstarlnuXFilter", "filter module for B -> Dstar l nu X"));

  // Breco Monitoring
  forWho->add(new BrecoQa("BrecoQa", "BReco Monitoring"));
  // inlusive baryon, at least one proton and one antiproton from protonLHVeryLoose:
  forWho->add(new InclppbarFilter("InclppbarFilter", "ppbar pairs filter"));

  // Two photon physics
  forWho->add(new TwoPhotonTwoTrackTagFilter("TwoPhotonTwoTrackTagFilter",
					     "filter module for two photon events"));
  forWho->add(new TwoPhotonPi0Pi0TagFilter("TwoPhotonPi0Pi0TagFilter",
					   "filter module for two photon events"));
  forWho->add(new TwoPhotonPentaquarkTagFilter("TwoPhotonPentaquarkTagFilter",
					       "filter module for two photon events"));

  // BtoDPiPi Tag Filter
  forWho->add (new BtoDPiPiTagFilter ("BtoDPiPiTagFilter", "BtoDssPi Tag Filter"));

  // BtoDPiPiPi Tag Filter
  forWho->add (new BtoDPiPiPiTagFilter ("BtoDPiPiPiTagFilter", "BtoDPiPiPi Tag Filter"));

  // tight P
  forWho->add(new InclProtTightSkim("InclProtTightSkim", "tight Proton with doca"));

  // Energy Tag Filter
  forWho->add(new TagFilterEnergy("TagFilterEnergy", "Energy Tag Filter"));

  // B0ToDstarA1CLoose
  forWho->add (new B0ToDstarA1CLoose ("B0ToDstarA1CLoose", "B0ToDstarA1CLoose Tag Filter"));

  // B0ToDstarhKlnu
  forWho->add (new B0ToDstarA1CLoose ("B0ToDstarhKlnuSkim", "B0ToDstarhKlnu Tag Filter"));

  // DsToKpi0Skim
  forWho->add(new DsToKpi0Skim("DsToKpi0Skim","DsToKpi0 Skim"));

  // Jpsi_Psi2sToChChGamma
  forWho->add(new Jpsi_Psi2sToChChGammaFilter("Jpsi_Psi2sToChChGammaFilter", "Filter on J/psi->pipigamma and Psi2s ->pipiGamma"));

  // Neutrino reconstruction
  forWho->add(new NuuReco("NuuReco","neutrino reconstruction and selection"));

  //Tracking Efficiency
  forWho->add(new TrkEffTau31Skim("TrkEffTau31Skim", "TrkEff Tau31 Skim"));

  BtaPidPidCalibAllSequence(forWho);

  return 0;
}
