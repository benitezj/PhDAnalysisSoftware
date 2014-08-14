############################################################################
##
## Defines the dStarD0ToVPi0Path skim filter path
##
## This filter searches for the decay:
##
##   D*+ -> D0 pi+
##          D0 -> V pi0
##
##   Joe Regensburger: October 26, 2005
############################################################################

set mySkimPath DstToD0PiToVPi0Path
set mySlowPion dStarD0ToVPi0SlowPion
set myPi0 dStarD0ToVPi0Pi0

set myKStarKPi dStarD0ToVPi0KStarKPi
set myKStarKsPi0 dStarD0ToVPi0KStarKsPi0
set myKStar dStarD0ToVPi0KStar

set myD0Phi dStarD0ToVPi0D0Phi
set myD0Rho dStarD0ToVPi0D0Rho
set myD0Omega dStarD0ToVPi0D0Omega
set myD0KStar dStarD0ToVPi0D0KStar

set myDStarMinusPhi dStarD0ToVPi0DStarPlusPhi
set myDStarPlusPhi dStarD0ToVPi0DStarMinusPhi
set myDStarMinusKStar dStarD0ToVPi0DStarPlusKStar
set myDStarPlusKStar dStarD0ToVPi0DStarMinusKStar
set myDStarMinusRho dStarD0ToVPi0DStarPlusRho
set myDStarPlusRho dStarD0ToVPi0DStarMinusRho
set myDStarMinusOmega dStarD0ToVPi0DStarPlusOmega
set myDStarPlusOmega dStarD0ToVPi0DStarMinusOmega

sourceFoundFile SimpleComposition/SmpCharmlessProdSequence.tcl
sourceFoundFile SimpleComposition/SmpKstarProdSequence.tcl

path create $mySkimPath
path append $mySkimPath SmpCharmlessProdSequence
path append $mySkimPath SmpKstarProdSequence

global writeUsrDataForSkim

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

#photon
##
set pi0List            "pi0AllLoose"

#Vector Meson lists
##
set phiList            "phiDefaultPID"
set rhoList            "rho0VeryLooseMassAndPid"
set omegaList          "omegaVeryLooseAndPid"

set kStarKsPi0List      "KstarKsPi0VeryLoose"
set kStarKPiList       "KstarKPiVeryLoose"
set kaonPidList        "KCombinedSuperLoose"
set kStarMass          "Mass 0.792:0.992"

#D0 cuts
##
set looseMassD0        "Mass 1.60:2.10"

## D* cut
##
set DeltaM             "DeltaM 0.00:.190"
set CmsPStar           "CmsP 2.10:"

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

mod clone SmpSubListerDefiner $myKStarKPi
path append $mySkimPath $myKStarKPi
talkto $myKStarKPi {
    debug             set $D0Debug
    verbose           set $D0Verbose
    unrefinedListName set $kStarKPiList
##myKStarKPiPID
    decayMode          set "K*0 -> K- pi+"
    daughterListNames  set $kaonPidList
    daughterListNames  set "piCombinedSuperLoose"
    selectors         set $kStarMass
}
 
mod clone SmpSubListerDefiner $myKStarKsPi0
path append $mySkimPath $myKStarKsPi0
talkto $myKStarKsPi0 {
    debug             set $D0Debug
    verbose           set $D0Verbose
    unrefinedListName set $kStarKsPi0List
    decayMode          set "K*0 -> K_S0 pi0"
    selectors         set $kStarMass
}
 
mod clone SmpMergerDefiner $myKStar
path append $mySkimPath $myKStar
talkto $myKStar {
    debug             set $D0Debug
    verbose           set $D0Verbose
    inputListNames    set $myKStarKPi
    inputListNames    set $myKStarKsPi0
}
  
######################################################################
######################################################################

## D0 -> Phi Pi0
##
mod clone SmpMakerDefiner $myD0Phi
path append $mySkimPath $myD0Phi
talkto $myD0Phi {
    debug             set $D0Debug
    verbose           set $D0Verbose
    decayMode         set "D0 -> phi pi0"
#    disableCloneCheck set true
    daughterListNames set $phiList
    daughterListNames set $pi0List
    fittingAlgorithm  set "Add4"
    postFitSelectors   set $looseMassD0
#    show
}
 
######################################################################
######################################################################
  
## D0 -> KStar Pi0
##
mod clone SmpMakerDefiner $myD0KStar
path append $mySkimPath $myD0KStar
talkto $myD0KStar {
    debug             set $D0Debug
    verbose           set $D0Verbose
    decayMode         set "D0 -> K*0 pi0"
#    disableCloneCheck set true
    daughterListNames set $myKStar
    daughterListNames set $pi0List
    fittingAlgorithm  set "Add4"
    postFitSelectors   set $looseMassD0
#    show
}
 
 
######################################################################
######################################################################
 
## D0 -> Rho Pi0
##
mod clone SmpMakerDefiner $myD0Rho
path append $mySkimPath $myD0Rho
talkto $myD0Rho {
    debug             set $D0Debug
    verbose           set $D0Verbose
    decayMode         set "D0 -> rho0 pi0"
#    disableCloneCheck set true
    daughterListNames set $rhoList
    daughterListNames set $pi0List
    fittingAlgorithm  set "Add4"
    postFitSelectors   set $looseMassD0
#    show
}
 
######################################################################
######################################################################
 
## D0 -> Omega Pi0
##
mod clone SmpMakerDefiner $myD0Omega
path append $mySkimPath $myD0Omega
talkto $myD0Omega {
    debug             set $D0Debug
    verbose           set $D0Verbose
    decayMode         set "D0 -> omega pi0"
#    disableCloneCheck set true
    daughterListNames set $omegaList
    daughterListNames set $pi0List
    fittingAlgorithm  set "Add4"
    postFitSelectors   set $looseMassD0
#    show
}
 
######################################################################
######################################################################
 
## Dst+ -> D0 pi+
##
mod clone SmpMakerDefiner $myDStarPlusPhi
path append $mySkimPath $myDStarPlusPhi
talkto $myDStarPlusPhi {
    debug             set $D0Debug
    verbose           set $D0Verbose
    decayMode         set "D*+ -> D0 pi+"
#   disableCloneCheck set true
    daughterListNames set $myD0Phi
    daughterListNames set $mySlowPion
    fittingAlgorithm  set "Add4"
    postFitSelectors  set $DeltaM
    postFitSelectors  set $CmsPStar
#    show
}

## Dst- -> D0 pi-
##
mod clone SmpMakerDefiner $myDStarMinusPhi
path append $mySkimPath $myDStarMinusPhi
talkto $myDStarMinusPhi {
    debug             set $D0Debug
    verbose           set $D0Verbose
    decayMode         set "D*- -> D0 pi-"
#   disableCloneCheck set true
    daughterListNames set $myD0Phi
    daughterListNames set $mySlowPion
    fittingAlgorithm  set "Add4"
    postFitSelectors  set $DeltaM
    postFitSelectors  set $CmsPStar
#    show
}

######################################################################
######################################################################
 
## Dst+ -> D0 pi+
##
mod clone SmpMakerDefiner $myDStarPlusKStar
path append $mySkimPath $myDStarPlusKStar
talkto $myDStarPlusKStar {
    debug             set $D0Debug
    verbose           set $D0Verbose
    decayMode         set "D*+ -> D0 pi+"
#   disableCloneCheck set true
    daughterListNames set $myD0KStar
    daughterListNames set $mySlowPion
    fittingAlgorithm  set "Add4"
    postFitSelectors  set $DeltaM
    postFitSelectors  set $CmsPStar
#    show
}

## Dst- -> D0 pi-
##
mod clone SmpMakerDefiner $myDStarMinusKStar
path append $mySkimPath $myDStarMinusKStar
talkto $myDStarMinusKStar {
    debug             set $D0Debug
    verbose           set $D0Verbose
    decayMode         set "D*- -> anti-D0 pi-"
#   disableCloneCheck set true
    daughterListNames set $myD0KStar
    daughterListNames set $mySlowPion
    fittingAlgorithm  set "Add4"
    postFitSelectors  set $DeltaM
    postFitSelectors  set $CmsPStar
#    show
}

######################################################################
######################################################################
 
## Dst+ -> D0 pi+
##
mod clone SmpMakerDefiner $myDStarPlusRho
path append $mySkimPath $myDStarPlusRho
talkto $myDStarPlusRho {
    debug             set $D0Debug
    verbose           set $D0Verbose
    decayMode         set "D*+ -> D0 pi+"
#   disableCloneCheck set true
    daughterListNames set $myD0Rho
    daughterListNames set $mySlowPion
    fittingAlgorithm  set "Add4"
    postFitSelectors  set $DeltaM
    postFitSelectors  set $CmsPStar
#    show
}

## Dst- -> D0 pi-
##
mod clone SmpMakerDefiner $myDStarMinusRho
path append $mySkimPath $myDStarMinusRho
talkto $myDStarMinusRho {
    debug             set $D0Debug
    verbose           set $D0Verbose
    decayMode         set "D*- -> D0 pi-"
#   disableCloneCheck set true
    daughterListNames set $myD0Rho
    daughterListNames set $mySlowPion
    fittingAlgorithm  set "Add4"
    postFitSelectors  set $DeltaM
    postFitSelectors  set $CmsPStar
#    show
}


######################################################################
######################################################################
 
## Dst+ -> D0 pi+
##
mod clone SmpMakerDefiner $myDStarPlusOmega
path append $mySkimPath $myDStarPlusOmega
talkto $myDStarPlusOmega {
    debug             set $D0Debug
    verbose           set $D0Verbose
    decayMode         set "D*+ -> D0 pi+"
#   disableCloneCheck set true
    daughterListNames set $myD0Omega
    daughterListNames set $mySlowPion
    fittingAlgorithm  set "Add4"
    postFitSelectors  set $DeltaM
    postFitSelectors  set $CmsPStar
#    show
}

## Dst- -> D0 pi-
##
mod clone SmpMakerDefiner $myDStarMinusOmega
path append $mySkimPath $myDStarMinusOmega
talkto $myDStarMinusOmega {
    debug             set $D0Debug
    verbose           set $D0Verbose
    decayMode         set "D*- -> D0 pi-"
#   disableCloneCheck set true
    daughterListNames set $myD0Omega
    daughterListNames set $mySlowPion
    fittingAlgorithm  set "Add4"
    postFitSelectors  set $DeltaM
    postFitSelectors  set $CmsPStar
#    show
}

######################################################################
######################################################################

## merge the two Dstar -> D0 pi into one list
mod clone SmpMergerDefiner DstToD0PiToPhiPi0
path append $mySkimPath DstToD0PiToPhiPi0
catch { setProduction DstToD0PiToPhiPi0}
talkto DstToD0PiToPhiPi0 {
    debug             set $D0Debug
    verbose           set $D0Verbose
#   disableCloneCheck set true
    inputListNames    set $myDStarPlusPhi
    inputListNames    set $myDStarMinusPhi
#    show
}

######################################################################
######################################################################

## merge the two Dstar -> D0 pi into one list
mod clone SmpMergerDefiner DstToD0PiToKStarPi0
path append $mySkimPath DstToD0PiToKStarPi0
catch { setProduction DstToD0PiToKStarPi0}
talkto DstToD0PiToKStarPi0 {
    debug             set $D0Debug
    verbose           set $D0Verbose
#   disableCloneCheck set true
    inputListNames    set $myDStarPlusKStar
    inputListNames    set $myDStarMinusKStar
#    show
}

######################################################################
######################################################################

## merge the two Dstar -> D0 pi into one list
mod clone SmpMergerDefiner DstToD0PiToRhoPi0
path append $mySkimPath DstToD0PiToRhoPi0
catch { setProduction DstToD0PiToRhoPi0}
talkto DstToD0PiToRhoPi0 {
    debug             set $D0Debug
    verbose           set $D0Verbose
#   disableCloneCheck set true
    inputListNames    set $myDStarPlusRho
    inputListNames    set $myDStarMinusRho
#    show
}

######################################################################
######################################################################

## merge the two Dstar -> D0 pi into one list
mod clone SmpMergerDefiner DstToD0PiToOmegaPi0
path append $mySkimPath DstToD0PiToOmegaPi0
catch { setProduction DstToD0PiToOmegaPi0}
talkto DstToD0PiToOmegaPi0 {
    debug             set $D0Debug
    verbose           set $D0Verbose
#   disableCloneCheck set true
    inputListNames    set $myDStarPlusOmega
    inputListNames    set $myDStarMinusOmega
#    show
}

######################################################################
######################################################################
mod clone TagFromList TagDstToD0PiToPhiPi0
path append $mySkimPath TagDstToD0PiToPhiPi0
talkto TagDstToD0PiToPhiPi0 {
  listsToTag set DstToD0PiToPhiPi0
  tagName set TagDstToD0PiToPhiPi0
}
 
mod clone TagFromList TagDstToD0PiToKStarPi0
path append $mySkimPath TagDstToD0PiToKStarPi0
talkto TagDstToD0PiToKStarPi0 {
  listsToTag set DstToD0PiToKStarPi0
  tagName set TagDstToD0PiToKStarPi0
}
 
mod clone TagFromList TagDstToD0PiToRhoPi0
path append $mySkimPath TagDstToD0PiToRhoPi0
talkto TagDstToD0PiToRhoPi0 {
  listsToTag set DstToD0PiToRhoPi0
  tagName set TagDstToD0PiToRhoPi0
}
 
mod clone TagFromList TagDstToD0PiToOmegaPi0
path append $mySkimPath TagDstToD0PiToOmegaPi0
talkto TagDstToD0PiToOmegaPi0 {
  listsToTag set DstToD0PiToOmegaPi0
  tagName set TagDstToD0PiToOmegaPi0
}
 
module clone TagFilterByName DstToD0PiToVPi0Filter
path append $mySkimPath DstToD0PiToVPi0Filter
talkto DstToD0PiToVPi0Filter {
  orList set TagDstToD0PiToPhiPi0
  orList set TagDstToD0PiToKStarPi0
  orList set TagDstToD0PiToRhoPi0
  orList set TagDstToD0PiToOmegaPi0
}
 

set writeUsrDataForSkim 0
                                                                                                                                                          
