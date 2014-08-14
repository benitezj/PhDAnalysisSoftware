############################################################################ta
##
## Defines the dStarD0ToVGammaPath skim filter path
##
## This filter searches for the decay:
##
##   D*+ -> D0 pi+
##          D0 -> pi0pi0
##
##   James Morris: July 29th, 2007
############################################################################

set mySkimPath DstToD0PiToPi0Pi0Path
set myPion dStarD0ToVPi0Pi0
set mySlowPion dStarD0ToPi0Pi0SlowPion

set myD0Pi0 dStarD0ToPi0Pi0

set myDStarPlusPi0 dStarD0ToDStarPlusPi0Pi0
set myDStarMinusPi0 dStarD0ToDStarMinusPi0Pi0

sourceFoundFile SimpleComposition/SmpCharmlessProdSequence.tcl
sourceFoundFile SimpleComposition/SmpKstarProdSequence.tcl

global writeUsrDataForSkim

path create $mySkimPath
path append $mySkimPath SmpCharmlessProdSequence
path append $mySkimPath SmpKstarProdSequence
#################################
#global control variables
#################################

set D0Debug false
set D0Verbose false

#################################
## SimpleComposition Cuts
#################################

#slow pion
##
set slowPionList       "GoodTracksVeryLoose"
set CmsPSlowPi         "CmsP 0.0:0.45"

# pi0
set pionList       "pi0AllVeryLoose"
 
#D0 cuts
##
set looseMassD0        "Mass 1.60:2.10"

## D* cut
##
set DeltaM             "DeltaM 0.00:.190"
set CmsPStar           "CmsP 2.1:"

######################################################################
######################################################################

## slow pion selection
##
mod clone SmpSubListerDefiner $mySlowPion
path append $mySkimPath $mySlowPion
talkto $mySlowPion {
    debug             set $D0Debug
    verbose           set $D0Verbose
    unrefinedListName set $slowPionList
    selectors         set $CmsPSlowPi
#   show
}

##  pi0 selection
##
mod clone SmpSubListerDefiner $myPion
path append $mySkimPath $myPion
talkto $myPion {
    debug             set $D0Debug
    verbose           set $D0Verbose
    unrefinedListName set $pionList
#   show
}


######################################################################
######################################################################


## D0 -> Pi0 Pi0
##
mod clone SmpMakerDefiner $myD0Pi0
path append $mySkimPath $myD0Pi0
talkto $myD0Pi0 {
    debug             set $D0Debug
    verbose           set $D0Verbose
    decayMode         set "D0 -> pi0 pi0"
#    disableCloneCheck set true
    daughterListNames set $myPion
    daughterListNames set $myPion
    fittingAlgorithm  set "Add4"
    postFitSelectors   set $looseMassD0
#    show
}

######################################################################
######################################################################
 
## Dst+ -> D0 pi+
##
mod clone SmpMakerDefiner $myDStarPlusPi0
path append $mySkimPath $myDStarPlusPi0
talkto $myDStarPlusPi0 {
    debug             set $D0Debug
    verbose           set $D0Verbose
    decayMode         set "D*+ -> D0 pi+"
#   disableCloneCheck set true
    daughterListNames set $myD0Pi0
    daughterListNames set $mySlowPion
    fittingAlgorithm  set "Add4"
    postFitSelectors  set $DeltaM
    postFitSelectors  set $CmsPStar
#    show
}

## Dst- -> D0 pi-
##
mod clone SmpMakerDefiner $myDStarMinusPi0
path append $mySkimPath $myDStarMinusPi0
talkto $myDStarMinusPi0 {
    debug             set $D0Debug
    verbose           set $D0Verbose
    decayMode         set "D*- -> D0 pi-"
#   disableCloneCheck set true
    daughterListNames set $myD0Pi0
    daughterListNames set $mySlowPion
    fittingAlgorithm  set "Add4"
    postFitSelectors  set $DeltaM
    postFitSelectors  set $CmsPStar
#    show
}

######################################################################
######################################################################

## merge the two Dstar -> D0 pi into one list
mod clone SmpMergerDefiner DstToD0PiToPi0Pi0
path append $mySkimPath DstToD0PiToPi0Pi0
catch { setProduction DstToD0PiToPi0Pi0}
talkto DstToD0PiToPi0Pi0 {
    debug             set $D0Debug
    verbose           set $D0Verbose
#   disableCloneCheck set true
    inputListNames    set $myDStarPlusPi0
    inputListNames    set $myDStarMinusPi0
#    show
}


######################################################################
######################################################################

mod clone TagFromList TagDstToD0PiToPi0Pi0
path append $mySkimPath TagDstToD0PiToPi0Pi0
talkto TagDstToD0PiToPi0Pi0 {
  listsToTag set DstToD0PiToPi0Pi0
  tagName set TagDstToD0PiToPi0Pi0
}

module clone TagFilterByName DstToD0PiToPi0Pi0Filter
path append $mySkimPath DstToD0PiToPi0Pi0Filter
talkto DstToD0PiToPi0Pi0Filter {
  orList set TagDstToD0PiToPi0Pi0
}

set writeUsrDataForSkim 0
