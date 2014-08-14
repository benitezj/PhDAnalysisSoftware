#
#  $Id: DchToPiKPi0KsPath.tcl,v 1.1 2005/12/16 22:34:35 fwinkl Exp $
#  Define the DchToPiKPi0Ks sequence
#

path create DchToPiKPi0KsPath
set mySkimSequence DchToPiKPi0KsSequence

sequence create $mySkimSequence
path append DchToPiKPi0KsPath $mySkimSequence
 
# fwinkl: disabled persisted candidates in agreement with author
#global BtaCandLists
#lappend BtaCandLists DchToPiPi0_Dstar
#lappend BtaCandLists DchToPiKs_Dstar
#lappend BtaCandLists DchToKPi0_Dstar
#lappend BtaCandLists DchToKKs_Dstar

#  Create the D Lists...4 modes in total:
#  1. D+ -> pi+ pi0
#  2. D+ -> pi+ Ks
#  3. D+ -> K+ pi0
#  4. D+ -> K+ Ks

#################################
## SimpleComposition Cuts
#################################

## D+ cuts
##
set pionListD          "piLHVeryLoose"
set kaonListD          "KLHNotPion"
set pi0ListD           "pi0AllVeryLoose"
set ksListD            "KsLoose"
set looseMassD         "Mass 1.55:2.2"

## D* cut
##
set pi0ListDstar       "pi0SoftLoose"
set DeltaM             "DeltaM :0.25"
set CmsP               "CmsP 1.95:"


mod clone SmpMakerDefiner DchToPiPi0
sequence append $mySkimSequence DchToPiPi0
talkto DchToPiPi0 {
    decayMode          set "D+ -> pi+ pi0"

    daughterListNames  set $pionListD
    daughterListNames  set $pi0ListD

    fittingAlgorithm   set "Add4"

    postFitSelectors   set $looseMassD
}

mod clone SmpMakerDefiner DchToPiKs
sequence append $mySkimSequence DchToPiKs
talkto DchToPiKs {
    decayMode          set "D+ -> pi+ K_S0"

    daughterListNames  set $pionListD
    daughterListNames  set $ksListD

    fittingAlgorithm   set "Add4"

    postFitSelectors   set $looseMassD
}

mod clone SmpMakerDefiner DchToKPhi
sequence append $mySkimSequence DchToKPhi
talkto DchToKPhi {
    decayMode          set "D+ -> K+ pi0"

    daughterListNames  set $kaonListD
    daughterListNames  set $pi0ListD

    fittingAlgorithm   set "Add4"

    postFitSelectors   set $looseMassD
}

mod clone SmpMakerDefiner DchToKKs
sequence append $mySkimSequence DchToKKs
talkto DchToKKs {
    decayMode          set "D+ -> K+ K_S0"

    daughterListNames  set $kaonListD
    daughterListNames  set $ksListD

    fittingAlgorithm   set "Add4"

    postFitSelectors   set $looseMassD
}

#  Create the Dstar lists
#  Combine 4 D lists with pi0 list.
mod clone SmpMakerDefiner DchToPiPhi_Dstar
sequence append $mySkimSequence DchToPiPhi_Dstar
catch { setProduction DchToPiPhi_Dstar}
talkto DchToPiPhi_Dstar {
    decayMode          set "D*+ -> D+ pi0"

    daughterListNames  set "DchToPiPi0"
    daughterListNames  set $pi0ListDstar

    fittingAlgorithm   set "Add4"

    postFitSelectors   set $DeltaM
    postFitSelectors   set $CmsP
}

mod clone SmpMakerDefiner DchToPiKs_Dstar
sequence append $mySkimSequence DchToPiKs_Dstar
catch { setProduction DchToPiKs_Dstar}
talkto DchToPiKs_Dstar {
    decayMode          set "D*+ -> D+ pi0"

    daughterListNames  set "DchToPiKs"
    daughterListNames  set $pi0ListDstar

    fittingAlgorithm   set "Add4"

    postFitSelectors   set $DeltaM
    postFitSelectors   set $CmsP
}

mod clone SmpMakerDefiner DchToKPhi_Dstar
sequence append $mySkimSequence DchToKPhi_Dstar
catch { setProduction DchToKPhi_Dstar}
talkto DchToKPhi_Dstar {
    decayMode          set "D*+ -> D+ pi0"

    daughterListNames  set "DchToKPhi"
    daughterListNames  set $pi0ListDstar

    fittingAlgorithm   set "Add4"

    postFitSelectors   set $DeltaM
    postFitSelectors   set $CmsP
}

mod clone SmpMakerDefiner DchToKKs_Dstar
sequence append $mySkimSequence DchToKKs_Dstar
catch { setProduction DchToKKs_Dstar}
talkto DchToKKs_Dstar {
    decayMode          set "D*+ -> D+ pi0"

    daughterListNames  set "DchToKKs"
    daughterListNames  set $pi0ListDstar

    fittingAlgorithm   set "Add4"

    postFitSelectors   set $DeltaM
    postFitSelectors   set $CmsP
}

mod clone FilterByList DchToPiKPi0KsFilter
path append DchToPiKPi0KsPath DchToPiKPi0KsFilter
talkto DchToPiKPi0KsFilter {
    orList set DchToPiPhi_Dstar 
    orList set DchToPiKs_Dstar 
    orList set DchToKPhi_Dstar 
    orList set DchToKKs_Dstar
}