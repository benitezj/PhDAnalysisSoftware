############################################################################
##
## Defines the dStarD0ToVEtaPath skim filter path
##
## This filter searches for the decay:
##
##   D*+ -> D0 pi+
##          D0 -> V eta
##
##   Joe Regensburger: October 26, 2005
############################################################################

set mySkimPath DstToD0PiToVEtaPath
set mySlowPion dStarD0ToVEtaSlowPion

set myKStarKPi dStarD0ToVEtaKStarKPi
set myKStarKsPi0 dStarD0ToVEtaKStarKsPi0
set myKStar dStarD0ToVEtaKStar

set myD0Phi dStarD0ToVEtaD0Phi
set myD0Rho dStarD0ToVEtaD0Rho
set myD0Omega dStarD0ToVEtaD0Omega
set myD0KStar dStarD0ToVEtaD0KStar

set myDStarMinusPhi dStarD0ToVEtaDStarPlusPhi
set myDStarPlusPhi dStarD0ToVEtaDStarMinusPhi
set myDStarMinusKStar dStarD0ToVEtaDStarPlusKStar
set myDStarPlusKStar dStarD0ToVEtaDStarMinusKStar
set myDStarMinusRho dStarD0ToVEtaDStarPlusRho
set myDStarPlusRho dStarD0ToVEtaDStarMinusRho
set myDStarMinusOmega dStarD0ToVEtaDStarPlusOmega
set myDStarPlusOmega dStarD0ToVEtaDStarMinusOmega

sourceFoundFile SimpleComposition/SmpCharmlessProdSequence.tcl
sourceFoundFile SimpleComposition/SmpKstarProdSequence.tcl
sourceFoundFile SimpleComposition/SmpEtaProdSequence.tcl

path create $mySkimPath
path append $mySkimPath SmpCharmlessProdSequence
path append $mySkimPath SmpKstarProdSequence
path append $mySkimPath SmpEtaProdSequence

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
set etaList              "etaDefault"

#Vector Meson mass constraints
##
set phiList            "phiDefaultPID"
set rhoList            "rho0VeryLooseMassAndPid"
set omegaList          "omegaVeryLooseAndPid"

set kStarKsPi0List     "KstarKsPi0VeryLoose"
set kStarKPiList       "KstarKPiVeryLoose"
set kaonPidList        "KCombinedSuperLoose"

set kStarMass          "Mass 0.792:0.992"

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

mod clone SmpSubListerDefiner $myKStarKPi
path append $mySkimPath $myKStarKPi
talkto $myKStarKPi {
    debug             set $D0Debug
    verbose           set $D0Verbose
    unrefinedListName set $kStarKPiList
    decayMode          set "K*0 -> K- pi+"
    daughterListNames  set $kaonPidList
    daughterListNames  set "piCombinedSuperLoose"
    selectors          set $kStarMass
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

## D0 -> Phi Eta
##
mod clone SmpMakerDefiner $myD0Phi
path append $mySkimPath $myD0Phi
talkto $myD0Phi {
    debug             set $D0Debug
    verbose           set $D0Verbose
    decayMode         set "D0 -> phi eta"
#    disableCloneCheck set true
    daughterListNames set $phiList
    daughterListNames set $etaList
    fittingAlgorithm  set "Add4"
    postFitSelectors   set $looseMassD0
#    show
}
######################################################################
######################################################################
  
## D0 -> KStar Eta
##
mod clone SmpMakerDefiner $myD0KStar
path append $mySkimPath $myD0KStar
talkto $myD0KStar {
    debug             set $D0Debug
    verbose           set $D0Verbose
    decayMode         set "D0 -> K*0 eta"
#    disableCloneCheck set true
    daughterListNames set $myKStar
    daughterListNames set $etaList
    fittingAlgorithm  set "Add4"
    postFitSelectors   set $looseMassD0
#    show
}
 

######################################################################
######################################################################
 
## D0 -> Rho Eta
##
mod clone SmpMakerDefiner $myD0Rho
path append $mySkimPath $myD0Rho
talkto $myD0Rho {
    debug             set $D0Debug
    verbose           set $D0Verbose
    decayMode         set "D0 -> rho0 eta"
#    disableCloneCheck set true
    daughterListNames set $rhoList
    daughterListNames set $etaList
    fittingAlgorithm  set "Add4"
    postFitSelectors   set $looseMassD0
#    show
}
 
######################################################################
######################################################################
 
## D0 -> Omega Eta
##
mod clone SmpMakerDefiner $myD0Omega
path append $mySkimPath $myD0Omega
talkto $myD0Omega {
    debug             set $D0Debug
    verbose           set $D0Verbose
    decayMode         set "D0 -> omega eta"
#    disableCloneCheck set true
    daughterListNames set $omegaList
    daughterListNames set $etaList
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
    decayMode         set "D*- -> D0 pi-"
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
mod clone SmpMergerDefiner DstToD0PiToPhiEta
path append $mySkimPath DstToD0PiToPhiEta
catch { setProduction DstToD0PiToPhiEta}
talkto DstToD0PiToPhiEta {
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
mod clone SmpMergerDefiner DstToD0PiToKStarEta
path append $mySkimPath DstToD0PiToKStarEta
catch { setProduction DstToD0PiToKStarEta}
talkto DstToD0PiToKStarEta {
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
mod clone SmpMergerDefiner DstToD0PiToRhoEta
path append $mySkimPath DstToD0PiToRhoEta
catch { setProduction DstToD0PiToRhoEta}
talkto DstToD0PiToRhoEta {
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
mod clone SmpMergerDefiner DstToD0PiToOmegaEta
path append $mySkimPath DstToD0PiToOmegaEta
catch { setProduction DstToD0PiToOmegaEta}
talkto DstToD0PiToOmegaEta {
    debug             set $D0Debug
    verbose           set $D0Verbose
#   disableCloneCheck set true
    inputListNames    set $myDStarPlusOmega
    inputListNames    set $myDStarMinusOmega
#    show
}

######################################################################
######################################################################

mod clone TagFromList TagDstToD0PiToPhiEta
path append $mySkimPath TagDstToD0PiToPhiEta
talkto TagDstToD0PiToPhiEta {
  listsToTag set DstToD0PiToPhiEta
  tagName set TagDstToD0PiToPhiEta
}
 
mod clone TagFromList TagDstToD0PiToKStarEta
path append $mySkimPath TagDstToD0PiToKStarEta
talkto TagDstToD0PiToKStarEta {
  listsToTag set DstToD0PiToKStarEta
  tagName set TagDstToD0PiToKStarEta
}
 
mod clone TagFromList TagDstToD0PiToRhoEta
path append $mySkimPath TagDstToD0PiToRhoEta
talkto TagDstToD0PiToRhoEta {
  listsToTag set DstToD0PiToRhoEta
  tagName set TagDstToD0PiToRhoEta
}
 
mod clone TagFromList TagDstToD0PiToOmegaEta
path append $mySkimPath TagDstToD0PiToOmegaEta
talkto TagDstToD0PiToOmegaEta {
  listsToTag set DstToD0PiToOmegaEta
  tagName set TagDstToD0PiToOmegaEta
}
 
module clone TagFilterByName DstToD0PiToVEtaFilter
path append $mySkimPath DstToD0PiToVEtaFilter
talkto DstToD0PiToVEtaFilter {
  orList set TagDstToD0PiToPhiEta
  orList set TagDstToD0PiToKStarEta
  orList set TagDstToD0PiToRhoEta
  orList set TagDstToD0PiToOmegaEta
}
 

set writeUsrDataForSkim 0
