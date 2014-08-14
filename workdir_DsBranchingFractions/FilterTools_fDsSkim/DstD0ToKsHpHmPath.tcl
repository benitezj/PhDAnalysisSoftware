############################################################################
## $Id: DstD0ToKsHpHmPath.tcl,v 1.8 2008/11/29 21:31:37 rmwhite Exp $
##
## Defines the DstD0ToKsHpHmPath skim filter path
##
## This filter searches for the decay:
##
##   D*+ -> D0 pi+
##          D0 -> Ks H+ H- + c.c., where H = pi or K
##
## Bill Lockman     16Sep2004       UC Santa Cruz
##                  01Oct2004 - make intermediate list names unique
##                  07Oct2004 - remove an extraneous character at the end of
##                              lappend ... DstarD0ToKShortKK line
############################################################################

set mySkimPath DstD0ToKsHpHmPath
set mySlowPions DstD0ToKsHpHmSlowPions
set myKsList DstD0ToKsHpHmKsDefault

path create $mySkimPath

global BtaCandLists
lappend BtaCandLists DstarD0ToKShortPiPi DstarD0ToKShortKK
#lappend BtaCandLists $mySlowPions $myKsList D0ToKShortPiPi D0ToKShortKK

#################################
## global control variables
#################################

set D0Debug false
set D0Debug false
set D0Verbose false

#################################
## SimpleComposition Cuts
#################################

## slow pion CMS cut
##
set slowPionList       "GoodTracksVeryLoose"
set CmsPSlowPi         "CmsP 0.0:0.45"

## Ks cuts
##
set pionListKs         "ChargedTracks"
set looseMassKs        "Mass 0.3:0.8"
set tightMassKs        "Mass 0.47765:0.51765"
set ProbChiSqFastVtxKs "ProbChiSq 0.001:"
set fltSig             "FlightSignificance 0:"

## D0 cuts
##
set pionListD0         "piCombinedSuperLoose"
set kaonListD0         "KCombinedSuperLoose"
set looseMassD0        "Mass 1.7:2.03"
set CmsP               "CmsP 2.0:"
set ProbChiSqFastVtx   "ProbChiSq 0.001:1."

## D* cut
##
set DeltaM             "DeltaM 0.:0.17"

######################################################################
######################################################################

## slow pion selectioon
##
mod clone SmpSubListerDefiner $mySlowPions
path append $mySkimPath $mySlowPions
talkto $mySlowPions {
    debug             set $D0Debug
    verbose           set $D0Verbose
    unrefinedListName set $slowPionList
    selectors         set $CmsPSlowPi
#    show
}

######################################################################
######################################################################

## K0_S -> pi+ pi- selection
##
mod clone SmpSubListerDefiner $myKsList
path append $mySkimPath $myKsList
talkto $myKsList {
    unrefinedListName set KsDefault
    debug             set $D0Debug
#    decayMode         set "K_S0 -> pi+ pi-"
#    daughterListNames set $pionListKs
#    daughterListNames set $pionListKs
    # +/- 20 MeV (~7 sigma) mass window around PDG M(K_S0)
    selectors  set $tightMassKs
    selectors  set $ProbChiSqFastVtxKs
    selectors  set $fltSig
#    show
}

######################################################################
######################################################################

## D0 -> K0_S pi+ pi- selection
##
mod clone SmpMakerDefiner D0ToKShortPiPi
path append $mySkimPath D0ToKShortPiPi
talkto D0ToKShortPiPi {
    debug             set $D0Debug
    verbose           set $D0Verbose
    decayMode         set "D0 -> K_S0 pi+ pi-"
#    disableCloneCheck set true
    daughterListNames set $myKsList
    daughterListNames set $pionListD0
    daughterListNames set $pionListD0
    fitConstraints    set "Geo"
    fittingAlgorithm  set "Cascade"
    preFitSelectors   set $CmsP
    preFitSelectors   set $looseMassD0
    postFitSelectors  set $ProbChiSqFastVtx
    postFitSelectors  set $CmsP
    postFitSelectors  set $looseMassD0
#    show
}

######################################################################
######################################################################

## Dst+ -> D0 pi+, D0 -> K0_S pi+ pi- selection
##
mod clone SmpMakerDefiner DstarPlusD0ToKShortPiPi
path append $mySkimPath DstarPlusD0ToKShortPiPi
talkto DstarPlusD0ToKShortPiPi {
    debug             set $D0Debug
    verbose           set $D0Verbose
    decayMode         set "D*+ -> D0 pi+"
#   disableCloneCheck set true
    daughterListNames set "D0ToKShortPiPi"
    daughterListNames set $mySlowPions
    fittingAlgorithm  set "Add4"
    postFitSelectors  set $DeltaM
#    show
}


######################################################################
######################################################################

## Dst- -> D0 pi-, D0 -> K0_S pi+ pi- selection
##
mod clone SmpMakerDefiner DstarMinusD0ToKShortPiPi
path append $mySkimPath DstarMinusD0ToKShortPiPi
talkto DstarMinusD0ToKShortPiPi {
    debug             set $D0Debug
    verbose           set $D0Verbose
    decayMode         set "D*- -> D0 pi-"
#   disableCloneCheck set true
    daughterListNames set "D0ToKShortPiPi"
    daughterListNames set $mySlowPions
    fittingAlgorithm  set "Add4"
    postFitSelectors  set $DeltaM
#    show
}

## merge the two Dstar -> D0 pi, D0 -> Ks pi+ pi- into one list

mod clone SmpMergerDefiner DstarD0ToKShortPiPi
path append $mySkimPath DstarD0ToKShortPiPi
talkto DstarD0ToKShortPiPi {
    debug             set $D0Debug
    verbose           set $D0Verbose
    disableCloneCheck set false
    inputListNames    set "DstarPlusD0ToKShortPiPi"
    inputListNames    set "DstarMinusD0ToKShortPiPi"
#    show
}

######################################################################
######################################################################

## D0 -> K0_S K+ K- selection
##
mod clone SmpMakerDefiner D0ToKShortKK
path append $mySkimPath D0ToKShortKK
talkto D0ToKShortKK {
    debug             set $D0Debug
    verbose           set $D0Verbose
    decayMode         set "D0 -> K_S0 K+ K-"
#    disableCloneCheck set true
    daughterListNames set $myKsList
    daughterListNames set $kaonListD0
    daughterListNames set $kaonListD0
    fitConstraints    set "Geo"
    fittingAlgorithm  set "Cascade"
    preFitSelectors   set $CmsP
    preFitSelectors   set $looseMassD0
    postFitSelectors  set $ProbChiSqFastVtx
    postFitSelectors  set $CmsP
    postFitSelectors  set $looseMassD0
#    show
}

######################################################################
######################################################################

## Dst+ -> D0 pi+, D0 -> Ks K+ K- selection
##
mod clone SmpMakerDefiner DstarPlusD0ToKShortKK
path append $mySkimPath DstarPlusD0ToKShortKK
talkto DstarPlusD0ToKShortKK {
    debug             set $D0Debug
    verbose           set $D0Verbose
    decayMode         set "D*+ -> D0 pi+"
#   disableCloneCheck set true
    daughterListNames set "D0ToKShortKK"
    daughterListNames set $mySlowPions
    fittingAlgorithm  set "Add4"
    postFitSelectors  set $DeltaM
#    show
}

######################################################################
######################################################################

# Dst- -> D0 pi-, D0 -> Ks K+ K- selection

mod clone SmpMakerDefiner DstarMinusD0ToKShortKK
path append $mySkimPath DstarMinusD0ToKShortKK
talkto DstarMinusD0ToKShortKK {
    debug             set $D0Debug
    verbose           set $D0Verbose
    decayMode         set "D*- -> D0 pi-"
#   disableCloneCheck set true
    daughterListNames set "D0ToKShortKK"
    daughterListNames set $mySlowPions
    fittingAlgorithm  set "Add4"
    postFitSelectors  set $DeltaM
#    show
}

## merge the two Dstar -> D0 pi, D0 -> Ks K+ K- into one list

mod clone SmpMergerDefiner DstarD0ToKShortKK
path append $mySkimPath DstarD0ToKShortKK
talkto DstarD0ToKShortKK {
    debug             set $D0Debug
    verbose           set $D0Verbose
    disableCloneCheck set false
    inputListNames    set "DstarPlusD0ToKShortKK"
    inputListNames    set "DstarMinusD0ToKShortKK"
#    show
}

## invoke some simple C++ code to scale the number of selected candidates of
## each type and pass if either final list has nonzero entries
##
path append $mySkimPath DstD0ToKsHpHmSkim
talkto DstD0ToKsHpHmSkim {
    outputList1 set DstarD0ToKShortPiPi
    outputList2 set DstarD0ToKShortKK
#    show
}
