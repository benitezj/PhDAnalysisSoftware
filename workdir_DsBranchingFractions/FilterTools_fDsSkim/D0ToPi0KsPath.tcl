#
#  $Id: D0ToPi0KsPath.tcl,v 1.1 2007/08/02 08:20:25 coleman Exp $
#  Define the D0ToPi0Ks sequence
#  jonathon coleman : coleman@slac.stanford.edu

path create D0ToPi0KsPath
set mySkimSequence D0ToPi0KsSequence

sequence create $mySkimSequence
path append D0ToPi0KsPath $mySkimSequence
 
# fwinkl: disabled persisted candidates in agreement with author
#global BtaCandLists
#lappend BtaCandLists D0ToPi0Pi0_Dstar
#lappend BtaCandLists D0ToPi0Ks_Dstar
#lappend BtaCandLists D0ToKsKs_Dstar

#  Create the D Lists...4 modes in total:
#  1. D0 -> Ks  Ks
#  2. D0 -> Ks  pi0

#already defined elsewhere hence commented out
#  3. D0 -> pi0 pi0


#################################
## SimpleComposition Cuts
#################################

## D0 cuts
##
set pi0ListD           "pi0AllVeryLoose"
set ksListD            "KsLoose"
set looseMassD         "Mass 1.55:2.2"

## D* cut
##
set pionListDstar       "GoodTracksVeryLoose"
set DeltaM             "DeltaM :0.25"
set CmsP               "CmsP 1.95:"


#mod clone SmpMakerDefiner D0ToPiPi0
#sequence append $mySkimSequence D0ToPiPi0
#talkto D0ToPiPi0 {
#    decayMode          set "D0 -> pi0 pi0"
#
#    daughterListNames  set $pi0ListD
#    daughterListNames  set $pi0ListD
#
#    fittingAlgorithm   set "Add4"
#
#    postFitSelectors   set $looseMassD
#}

mod clone SmpMakerDefiner D0ToPi0Ks
sequence append $mySkimSequence D0ToPi0Ks
talkto D0ToPi0Ks {
    decayMode          set "D0 -> pi0 K_S0"

    daughterListNames  set $pi0ListD
    daughterListNames  set $ksListD

    fittingAlgorithm   set "Add4"

    postFitSelectors   set $looseMassD
}


mod clone SmpMakerDefiner D0ToKsKs
sequence append $mySkimSequence D0ToKsKs
talkto D0ToKsKs {
    decayMode          set "D0 -> K_S0 K_S0"

    daughterListNames  set $ksListD
    daughterListNames  set $ksListD

    fittingAlgorithm   set "Add4"

    postFitSelectors   set $looseMassD
}

#  Create the Dstar lists
#  Combine 3 D lists with pi+ list.
#mod clone SmpMakerDefiner D0ToPiPi0_Dstar
#sequence append $mySkimSequence D0ToPi0P0_Dstar
#catch { setProduction D0ToPi0Pi0_Dstar}
#talkto D0ToPi0Pi0_Dstar {
#    decayMode          set "D*+ -> D0 pi+"
#
#    daughterListNames  set "D0ToPi0Pi0"
#    daughterListNames  set $pionListDstar
#
#    fittingAlgorithm   set "Add4"
#
#    postFitSelectors   set $DeltaM
#    postFitSelectors   set $CmsP
#}

mod clone SmpMakerDefiner D0ToPi0Ks_Dstar
sequence append $mySkimSequence D0ToPi0Ks_Dstar
catch { setProduction D0ToPi0Ks_Dstar}
talkto D0ToPi0Ks_Dstar {
    decayMode          set "D*+ -> D0 pi+"

    daughterListNames  set "D0ToPi0Ks"
    daughterListNames  set $pionListDstar

    fittingAlgorithm   set "Add4"

    postFitSelectors   set $DeltaM
    postFitSelectors   set $CmsP
}


mod clone SmpMakerDefiner D0ToKsKs_Dstar
sequence append $mySkimSequence D0ToKsKs_Dstar
catch { setProduction D0ToKsKs_Dstar}
talkto D0ToKsKs_Dstar {
    decayMode          set "D*+ -> D0 pi+"

    daughterListNames  set "D0ToKsKs"
    daughterListNames  set $pionListDstar

    fittingAlgorithm   set "Add4"

    postFitSelectors   set $DeltaM
    postFitSelectors   set $CmsP
}

mod clone FilterByList D0ToPi0KsFilter
path append D0ToPi0KsPath D0ToPi0KsFilter
talkto D0ToPi0KsFilter {
#    orList set D0ToPi0Pi0_Dstar 
    orList set D0ToPi0Ks_Dstar 
    orList set D0ToKsKs_Dstar
}