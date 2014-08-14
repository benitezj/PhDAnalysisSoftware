############################################################################
## $Id: DstD0ToKsKPiPath.tcl,v 1.1 2008/11/26 22:33:25 dubrovin Exp $
##
## Defines the DstD0ToKsKPiPath skim filter path
##
## This filter searches for the decays:
##
##   D*+ -> D0 pi+              (CF)
##          D0 -> Ks K+ Pi-
##          D0 -> Ks K- Pi+ 
##
##   D*- -> D0 pi-              (CS)
##          D0 -> Ks K+ Pi-
##          D0 -> Ks K- Pi+ 
##
## Mikhail Dubrovin     November 18, 2008      University of Cincinnati
############################################################################

set mySkimPath  DstD0ToKsKPiPath
set mySlowPions DstD0ToKsKPiSlowPions
set myKsList    DstD0ToKsKPiKsDefault

path create $mySkimPath     

global  BtaCandLists
lappend BtaCandLists DstarD0ToKShortKPi
#lappend BtaCandLists $mySlowPions $myKsList D0ToKShortKpPim DstarD0ToKShortKmPip

#################################
## global control variables
#################################

set D0Debug   false
set D0Debug   false
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
#set pionListD0         "piLHVeryLoose"
#set kaonListD0         "KLHVeryLoose"
#set pionListD0         "piLHLoose"
#set kaonListD0         "KLHLoose"
set pionListD0         "piCombinedSuperLoose"
set kaonListD0         "KCombinedSuperLoose"
set looseMassD0        "Mass 1.7:2.03"
set CmsP               "CmsP 2.0:"
set ProbChiSqFastVtx   "ProbChiSq 0.001:1."

## D* cut
##
set DeltaM             "DeltaM 0.:0.2"

######################################################################

## slow pion selectioon
##
mod clone SmpSubListerDefiner $mySlowPions
path append $mySkimPath       $mySlowPions
talkto                        $mySlowPions {
    debug             set $D0Debug
    verbose           set $D0Verbose
    unrefinedListName set $slowPionList
    selectors         set $CmsPSlowPi
#    show
}

######################################################################

## K0_S -> pi+ pi- selection
##
mod clone SmpSubListerDefiner $myKsList
path append $mySkimPath       $myKsList
talkto                        $myKsList {
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

## D0 -> K0_S K+ pi- selection
##
mod clone SmpMakerDefiner D0ToKShortKpPim
path append $mySkimPath   D0ToKShortKpPim
talkto                    D0ToKShortKpPim {
    debug             set $D0Debug
    verbose           set $D0Verbose
    decayMode         set "D0 -> K_S0 K+ pi-"
    daughterListNames set $myKsList
    daughterListNames set $kaonListD0
    daughterListNames set $pionListD0
#    fitConstraints    set "Mass"
    fittingAlgorithm  set "TreeFitter"
    preFitSelectors   set $CmsP
    preFitSelectors   set $looseMassD0
    postFitSelectors  set $ProbChiSqFastVtx
    postFitSelectors  set $CmsP
    postFitSelectors  set $looseMassD0
#    show
}

######################################################################

## D0 -> K0_S K- pi+ selection
##
mod clone SmpMakerDefiner D0ToKShortKmPip
path append $mySkimPath   D0ToKShortKmPip
talkto                    D0ToKShortKmPip {
    debug             set $D0Debug
    verbose           set $D0Verbose
    decayMode         set "D0 -> K_S0 K- pi+"
    daughterListNames set $myKsList
    daughterListNames set $kaonListD0
    daughterListNames set $pionListD0
#    fitConstraints    set "Mass"
    fittingAlgorithm  set "TreeFitter"
    preFitSelectors   set $CmsP
    preFitSelectors   set $looseMassD0
    postFitSelectors  set $ProbChiSqFastVtx
    postFitSelectors  set $CmsP
    postFitSelectors  set $looseMassD0
#    show
}

######################################################################

## Dst+ -> D0 pi+, D0 -> K0_S K+ pi- selection
##
mod clone SmpMakerDefiner DstarPlusD0ToKShortKpPim
path append $mySkimPath   DstarPlusD0ToKShortKpPim
talkto                    DstarPlusD0ToKShortKpPim {
    debug             set $D0Debug
    verbose           set $D0Verbose
    decayMode         set "D*+ -> D0 pi+"
    daughterListNames set "D0ToKShortKpPim"
    daughterListNames set $mySlowPions
    fittingAlgorithm  set "TreeFitter"
    postFitSelectors  set $DeltaM
#    show
}

######################################################################

## Dst+ -> D0 pi+, D0 -> K0_S K- pi+ selection
##
mod clone SmpMakerDefiner DstarPlusD0ToKShortKmPip
path append $mySkimPath   DstarPlusD0ToKShortKmPip
talkto                    DstarPlusD0ToKShortKmPip {
    debug             set $D0Debug
    verbose           set $D0Verbose
    decayMode         set "D*+ -> D0 pi+"
    daughterListNames set "D0ToKShortKmPip"
    daughterListNames set $mySlowPions
    fittingAlgorithm  set "TreeFitter"
    postFitSelectors  set $DeltaM
#    show
}

######################################################################

## Dst- -> D0 pi-, D0 -> K0_S K+ pi- selection
##
mod clone SmpMakerDefiner DstarMinusD0ToKShortKpPim
path append $mySkimPath   DstarMinusD0ToKShortKpPim
talkto                    DstarMinusD0ToKShortKpPim {
    debug             set $D0Debug
    verbose           set $D0Verbose
    decayMode         set "D*- -> D0 pi-"
    daughterListNames set "D0ToKShortKpPim"
    daughterListNames set $mySlowPions
    fittingAlgorithm  set "TreeFitter"
    postFitSelectors  set $DeltaM
#    show
}

######################################################################

## Dst- -> D0 pi-, D0 -> K0_S K- pi+ selection
##
mod clone SmpMakerDefiner DstarMinusD0ToKShortKmPip
path append $mySkimPath   DstarMinusD0ToKShortKmPip
talkto                    DstarMinusD0ToKShortKmPip {
    debug             set $D0Debug
    verbose           set $D0Verbose
    decayMode         set "D*- -> D0 pi-"
    daughterListNames set "D0ToKShortKmPip"
    daughterListNames set $mySlowPions
    fittingAlgorithm  set "TreeFitter"
    postFitSelectors  set $DeltaM
#    show
}

######################################################################

## merge the four Dstar -> D0 pi, D0 -> Ks K pi into one list

mod clone SmpMergerDefiner DstarD0ToKShortKPi
path append $mySkimPath    DstarD0ToKShortKPi
talkto                     DstarD0ToKShortKPi {
    debug             set $D0Debug
    verbose           set $D0Verbose
    disableCloneCheck set false
    inputListNames    set "DstarPlusD0ToKShortKpPim"
    inputListNames    set "DstarPlusD0ToKShortKmPip"
    inputListNames    set "DstarMinusD0ToKShortKpPim"
    inputListNames    set "DstarMinusD0ToKShortKmPip"
#    show
}

######################################################################

## invoke some simple C++ code to scale the number of selected candidates of
## each type and pass if either final list has nonzero entries
##
path append $mySkimPath DstD0ToKsKPiSkim
talkto                  DstD0ToKsKPiSkim {
      outputList1 set DstarD0ToKShortKPi
##    show
}

######################################################################

#Filter on the list
mod clone FilterByList  DstarD0ToKShortKPiFilter 
path append $mySkimPath DstarD0ToKShortKPiFilter 
talkto                  DstarD0ToKShortKPiFilter {
#    orList set DstarPlusD0ToKShortKpPim
#    orList set DstarPlusD0ToKShortKmPip
#    orList set DstarMinusD0ToKShortKpPim
#    orList set DstarMinusD0ToKShortKmPip
     orList set DstarD0ToKShortKPi
}

######################################################################
