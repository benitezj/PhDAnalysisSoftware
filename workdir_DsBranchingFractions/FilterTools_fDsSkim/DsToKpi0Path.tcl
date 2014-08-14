#
#  $Id: DsToKpi0Path.tcl,v 1.4 2005/03/14 14:50:03 fnc Exp $
#  Define the DsToKpi0  path
#
path create DsToKpi0Path
path append DsToKpi0Path DsToKpi0Skim

#  Create a Tag Filter
mod clone TagFilterByName DsToKpi0TagFilter
talkto DsToKpi0TagFilter { 
    orList set BGFMultiHadron
    orList set BGFNeutralHadron
    assertIfMissing set true
}
path append DsToKpi0Path DsToKpi0TagFilter

#  Create the Ds Lists...4 modes in total:
#  1. D_s+ -> K+ pi0
#  2. D_s+ -> pi+ pi0
#  3. D_s+ -> Ks K+
#  4. D_s+ -> Ks pi+

mod clone SmpMakerDefiner DsToKpi0
seq append SmpCompositionSequence DsToKpi0
catch { setProduction DsToKpi0 }
talkto DsToKpi0 {
    decayMode          set "D_s+ -> K+ pi0"
    #Lists
    daughterListNames  set "GoodTracksLoose"
    daughterListNames  set "pi0LooseMass"

    fittingAlgorithm   set "Add4"

    postFitSelectors   set "Mass 1.65:2.3"
    postFitSelectors   set "ProbChiSq 0.001:1"
}

mod clone SmpMakerDefiner DsTopipi0
seq append SmpCompositionSequence DsTopipi0
catch { setProduction DsTopipi0 }
talkto DsTopipi0 {
    decayMode          set "D_s+ -> pi+ pi0"
    #Lists
    daughterListNames  set "GoodTracksLoose"
    daughterListNames  set "pi0LooseMass"

    fittingAlgorithm   set "Add4"

    postFitSelectors   set "Mass 1.65:2.3"
    postFitSelectors   set "ProbChiSq 0.001:1"
}

mod clone SmpMakerDefiner DsToKsK
seq append SmpCompositionSequence DsToKsK
catch { setProduction DsToKsK }
talkto DsToKsK {
    decayMode          set "D_s+ -> K_S0 K+"

    daughterListNames  set "KsLoose"
    daughterListNames  set "GoodTracksLoose"

    fittingAlgorithm   set "Add4"

    postFitSelectors   set "Mass 1.65:2.3"
    postFitSelectors   set "ProbChiSq 0.001:1"
}

mod clone SmpMakerDefiner DsToKspi
seq append SmpCompositionSequence DsToKspi
catch { setProduction DsToKspi }
talkto DsToKspi {
    decayMode          set "D_s+ -> K_S0 pi+"

    daughterListNames  set "KsLoose"
    daughterListNames  set "GoodTracksLoose"

    fittingAlgorithm   set "Add4"

    postFitSelectors   set "Mass 1.65:2.3"
    postFitSelectors   set "ProbChiSq 0.001:1"
}

#  Create the Dsstar Lists
#  Combine 4 Ds lists with Photon list.
mod clone SmpMakerDefiner DsstarToDsgammaDsToKpi0
seq append SmpCompositionSequence DsstarToDsgammaDsToKpi0
catch { setProduction DsstarToDsgammaDsToKpi0 }
talkto DsstarToDsgammaDsToKpi0 {
    decayMode          set "D_s*+ -> D_s+ gamma"

    daughterListNames  set "DsToKpi0"
    daughterListNames  set "GoodPhotonLoose"

    fittingAlgorithm   set "Add4"

    postFitSelectors   set "Mass 1.7:2.5"
    postFitSelectors   set "ProbChiSq 0.001:1"
    postFitSelectors   set "DeltaM :0.25"
    postFitSelectors   set "CmsP 2.6:"
}

mod clone SmpMakerDefiner DsstarToDsgammaDsTopipi0
seq append SmpCompositionSequence DsstarToDsgammaDsTopipi0
catch { setProduction DsstarToDsgammaDsTopipi0 }
talkto DsstarToDsgammaDsTopipi0 {
    decayMode          set "D_s*+ -> D_s+ gamma"
    #get some lists
    daughterListNames  set "DsTopipi0"
    daughterListNames  set "GoodPhotonLoose"

    fittingAlgorithm   set "Add4"

    postFitSelectors   set "Mass 1.7:2.4"
    postFitSelectors   set "ProbChiSq 0.001:1"
    postFitSelectors   set "DeltaM :0.25"
    postFitSelectors   set "CmsP 2.6:"
}

mod clone SmpMakerDefiner DsstarToDsgammaDsToKsK
seq append SmpCompositionSequence DsstarToDsgammaDsToKsK
catch { setProduction DsstarToDsgammaDsToKsK }
talkto DsstarToDsgammaDsToKsK {
    decayMode          set "D_s*+ -> D_s+ gamma"
    #get some lists
    daughterListNames  set "DsToKsK"
    daughterListNames  set "GoodPhotonLoose"

    fittingAlgorithm   set "Add4"

    postFitSelectors   set "Mass 1.7:2.4"
    postFitSelectors   set "ProbChiSq 0.001:1"
    postFitSelectors   set "DeltaM :0.25"
    postFitSelectors   set "CmsP 2.6:"
}

mod clone SmpMakerDefiner DsstarToDsgammaDsToKspi
seq append SmpCompositionSequence DsstarToDsgammaDsToKspi
catch { setProduction DsstarToDsgammaDsToKspi }
talkto DsstarToDsgammaDsToKspi {
    decayMode          set "D_s*+ -> D_s+ gamma"
    #get some lists
    daughterListNames  set "DsToKspi"
    daughterListNames  set "GoodPhotonLoose"

    fittingAlgorithm   set "Add4"

    postFitSelectors   set "Mass 1.7:2.4"
    postFitSelectors   set "ProbChiSq 0.001:1"
    postFitSelectors   set "DeltaM :0.25"
    postFitSelectors   set "CmsP 2.6:"
}

#  Set the list names for the skim module
talkto DsToKpi0Skim {
    DsToKpi0ListName  set "DsstarToDsgammaDsToKpi0"
    DsTopipi0ListName set "DsstarToDsgammaDsTopipi0"
    DsToKsKListName   set "DsstarToDsgammaDsToKsK"
    DsToKspiListName   set "DsstarToDsgammaDsToKspi"
}
