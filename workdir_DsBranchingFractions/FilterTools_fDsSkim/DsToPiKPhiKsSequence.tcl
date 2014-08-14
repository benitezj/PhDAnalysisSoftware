#
#  $Id: DsToPiKPhiKsSequence.tcl,v 1.1 2005/10/28 21:17:47 jregens Exp $
#  Define the DsToPiKPhiKs path
#

set mySkimSequence DsToPiKPhiKsSequence
set myPhiList DsToPiKPhiKs_Phi

sequence create $mySkimSequence

global BtaCandLists
lappend BtaCandLists DsToPiPhi_Dsstar
lappend BtaCandLists DsToPiKs_Dsstar
lappend BtaCandLists DsToKPhi_Dsstar
lappend BtaCandLists DsToKKs_Dsstar

#  Create the Ds Lists...4 modes in total:
#  1. D_s+ -> pi+ phi
#  2. D_s+ -> pi+ Ks
#  3. D_s+ -> K+ phi
#  4. D_s+ -> K+ Ks

#################################
## SimpleComposition Cuts
#################################

## phi cuts
##
set kaonListPhi1       "KLHNotPion"
set kaonListPhi2       "GoodTracksLoose"
set looseMassPhi       "Mass 0.98:1.06"

## Ds cuts
##
set pionListDs         "piLHVeryLoose"
set kaonListDs         "KLHNotPion"
set ksListDs           "KsLoose"
set looseMassDs        "Mass 1.65:2.3"

## D* cut
##
set photonList         "GoodPhotonLoose"
set DeltaM             "DeltaM :0.25"
set CmsP               "CmsP 1.95:"


# phi -> K+ K- selection
mod clone SmpMakerDefiner $myPhiList
sequence append $mySkimSequence $myPhiList
talkto $myPhiList {
    decayMode         set "phi -> K+ K-"

    daughterListNames set $kaonListPhi1
    daughterListNames set $kaonListPhi2

    fittingAlgorithm  set "Add4"

    postFitSelectors  set $looseMassPhi
}


mod clone SmpMakerDefiner DsToPiPhi
sequence append $mySkimSequence DsToPiPhi
talkto DsToPiPhi {
    decayMode          set "D_s+ -> pi+ phi"

    daughterListNames  set $pionListDs
    daughterListNames  set $myPhiList

    fittingAlgorithm   set "Add4"

    postFitSelectors   set $looseMassDs
}

mod clone SmpMakerDefiner DsToPiKs
sequence append $mySkimSequence DsToPiKs
talkto DsToPiKs {
    decayMode          set "D_s+ -> pi+ K_S0"

    daughterListNames  set $pionListDs
    daughterListNames  set $ksListDs

    fittingAlgorithm   set "Add4"

    postFitSelectors   set $looseMassDs
}

mod clone SmpMakerDefiner DsToKPhi
sequence append $mySkimSequence DsToKPhi
talkto DsToKPhi {
    decayMode          set "D_s+ -> K+ phi"

    daughterListNames  set $kaonListDs
    daughterListNames  set $myPhiList

    fittingAlgorithm   set "Add4"

    postFitSelectors   set $looseMassDs
}

mod clone SmpMakerDefiner DsToKKs
sequence append $mySkimSequence DsToKKs
talkto DsToKKs {
    decayMode          set "D_s+ -> K+ K_S0"

    daughterListNames  set $kaonListDs
    daughterListNames  set $ksListDs

    fittingAlgorithm   set "Add4"

    postFitSelectors   set $looseMassDs
}

#  Create the Dsstar lists
#  Combine 4 Ds lists with photon list.
mod clone SmpMakerDefiner DsToPiPhi_Dsstar
sequence append $mySkimSequence DsToPiPhi_Dsstar
catch { setProduction DsToPiPhi_Dsstar}
talkto DsToPiPhi_Dsstar {
    decayMode          set "D_s*+ -> D_s+ gamma"

    daughterListNames  set "DsToPiPhi"
    daughterListNames  set $photonList

    fittingAlgorithm   set "Add4"

    postFitSelectors   set $DeltaM
    postFitSelectors   set $CmsP
}

mod clone SmpMakerDefiner DsToPiKs_Dsstar
sequence append $mySkimSequence DsToPiKs_Dsstar
catch { setProduction DsToPiKs_Dsstar}
talkto DsToPiKs_Dsstar {
    decayMode          set "D_s*+ -> D_s+ gamma"

    daughterListNames  set "DsToPiKs"
    daughterListNames  set $photonList

    fittingAlgorithm   set "Add4"

    postFitSelectors   set $DeltaM
    postFitSelectors   set $CmsP
}

mod clone SmpMakerDefiner DsToKPhi_Dsstar
sequence append $mySkimSequence DsToKPhi_Dsstar
catch { setProduction DsToKPhi_Dsstar}
talkto DsToKPhi_Dsstar {
    decayMode          set "D_s*+ -> D_s+ gamma"

    daughterListNames  set "DsToKPhi"
    daughterListNames  set $photonList

    fittingAlgorithm   set "Add4"

    postFitSelectors   set $DeltaM
    postFitSelectors   set $CmsP
}

mod clone SmpMakerDefiner DsToKKs_Dsstar
sequence append $mySkimSequence DsToKKs_Dsstar
catch { setProduction DsToKKs_Dsstar}
talkto DsToKKs_Dsstar {
    decayMode          set "D_s*+ -> D_s+ gamma"

    daughterListNames  set "DsToKKs"
    daughterListNames  set $photonList

    fittingAlgorithm   set "Add4"

    postFitSelectors   set $DeltaM
    postFitSelectors   set $CmsP
}

#
# Set the list names for the skim module
#sequence append $mySkimSequence DsToPiKPhiKsSkim
#catch { setProduction DsToKKs_Dsstar}
#talkto DsToPiKPhiKsSkim {
#    outputList1  set  "DsToPiPhi_Dsstar"
#    outputList2  set  "DsToPiKs_Dsstar"
#    outputList3  set  "DsToKPhi_Dsstar"
#    outputList4  set  "DsToKKs_Dsstar"
#}