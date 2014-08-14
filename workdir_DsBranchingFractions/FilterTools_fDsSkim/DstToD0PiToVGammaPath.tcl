############################################################################ta
##
## Defines the dStarD0ToVGammaPath skim filter path
##
## This filter searches for the decay:
##
##   D*+ -> D0 pi+
##          D0 -> V gamma
##
##   Joe Regensburger: October 26, 2005
############################################################################

set mySkimPath DstToD0PiToVGammaPath
set mySlowPion dStarD0ToVGammaSlowPion
set myGamma dStarD0ToVGammaGamma

set myKStarKPi dStarD0ToVGammaKStarKPi 
set myKStarKsPi0 dStarD0ToVGammaKStarKsPi0 
set myKStar dStarD0ToVGammaKStar 

set myD0Phi dStarD0ToVGammaD0Phi
set myD0Rho dStarD0ToVGammaD0Rho
set myD0Omega dStarD0ToVGammaD0Omega
set myD0KStar dStarD0ToVGammaD0KStar
set myD0Gamma dStarD0ToVGammaD0Gamma

set myDStarMinusPhi dStarD0ToVGammaDStarPlusPhi
set myDStarPlusPhi dStarD0ToVGammaDStarMinusPhi
set myDStarMinusKStar dStarD0ToVGammaDStarPlusKStar
set myDStarPlusKStar dStarD0ToVGammaDStarMinusKStar
set myDStarMinusRho dStarD0ToVGammaDStarPlusRho
set myDStarPlusRho dStarD0ToVGammaDStarMinusRho
set myDStarMinusOmega dStarD0ToVGammaDStarPlusOmega
set myDStarPlusOmega dStarD0ToVGammaDStarMinusOmega
set myDStarMinusGamma dStarD0ToVGammaDStarPlusGamma
set myDStarPlusGamma dStarD0ToVGammaDStarMinusGammaa

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

#photon
##
set gammaList          "GoodPhotonLoose"
set latMoment          "Lat 0.001:"

#Vector Meson mass constraints
##
set phiList            "phiDefaultPID"
set rhoList            "rho0VeryLooseMassAndPid"
set kStarKsPi0List      "KstarKsPi0VeryLoose"
set kStarKPiList       "KstarKPiVeryLoose"
set kaonPidList        "KCombinedSuperLoose"
set omegaList          "omegaVeryLooseAndPid"

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

## photon selection
##
mod clone SmpSubListerDefiner $myGamma
path append $mySkimPath $myGamma
talkto $myGamma {
    debug             set $D0Debug
    verbose           set $D0Verbose
    unrefinedListName set $gammaList
    selectors         set $latMoment
# #    show
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

## D0 -> Phi Gamma
##
mod clone SmpMakerDefiner $myD0Phi
path append $mySkimPath $myD0Phi
talkto $myD0Phi {
    debug             set $D0Debug
    verbose           set $D0Verbose
    decayMode         set "D0 -> phi gamma"
#    disableCloneCheck set true
    daughterListNames set $phiList
    daughterListNames set $myGamma
    fittingAlgorithm  set "Add4"
    postFitSelectors   set $looseMassD0
#    show
}

######################################################################
######################################################################
 
## D0 -> KStar Gamma
##
mod clone SmpMakerDefiner $myD0KStar
path append $mySkimPath $myD0KStar
talkto $myD0KStar {
    debug             set $D0Debug
    verbose           set $D0Verbose
    decayMode         set "D0 -> K*0 gamma"
#    disableCloneCheck set true
    daughterListNames set $myKStar
    daughterListNames set $myGamma
    fittingAlgorithm  set "Add4"
    postFitSelectors   set $looseMassD0
#    show
}

######################################################################
######################################################################
 
## D0 -> Rho Gamma
##
mod clone SmpMakerDefiner $myD0Rho
path append $mySkimPath $myD0Rho
talkto $myD0Rho {
    debug             set $D0Debug
    verbose           set $D0Verbose
    decayMode         set "D0 -> rho0 gamma"
#    disableCloneCheck set true
    daughterListNames set $rhoList
    daughterListNames set $myGamma
    fittingAlgorithm  set "Add4"
    postFitSelectors   set $looseMassD0
#    show
}
 
######################################################################
######################################################################
 
## D0 -> Omega Gamma
##
mod clone SmpMakerDefiner $myD0Omega
path append $mySkimPath $myD0Omega
talkto $myD0Omega {
    debug             set $D0Debug
    verbose           set $D0Verbose
    decayMode         set "D0 -> omega gamma"
#    disableCloneCheck set true
    daughterListNames set $omegaList
    daughterListNames set $myGamma
    fittingAlgorithm  set "Add4"
    postFitSelectors   set $looseMassD0
#    show
}
######################################################################
######################################################################
 
## D0 -> Gamma Gamma
##
mod clone SmpMakerDefiner $myD0Gamma
path append $mySkimPath $myD0Gamma
talkto $myD0Gamma {
    debug             set $D0Debug
    verbose           set $D0Verbose
    decayMode         set "D0 -> gamma gamma"
#    disableCloneCheck set true
    daughterListNames set $myGamma
    daughterListNames set $myGamma
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
 
## Dst+ -> D0 pi+
##
mod clone SmpMakerDefiner $myDStarPlusGamma
path append $mySkimPath $myDStarPlusGamma
talkto $myDStarPlusGamma {
    debug             set $D0Debug
    verbose           set $D0Verbose
    decayMode         set "D*+ -> D0 pi+"
#   disableCloneCheck set true
    daughterListNames set $myD0Gamma
    daughterListNames set $mySlowPion
    fittingAlgorithm  set "Add4"
    postFitSelectors  set $DeltaM
    postFitSelectors  set $CmsPStar
#    show
}

## Dst- -> D0 pi-
##
mod clone SmpMakerDefiner $myDStarMinusGamma
path append $mySkimPath $myDStarMinusGamma
talkto $myDStarMinusGamma {
    debug             set $D0Debug
    verbose           set $D0Verbose
    decayMode         set "D*- -> D0 pi-"
#   disableCloneCheck set true
    daughterListNames set $myD0Gamma
    daughterListNames set $mySlowPion
    fittingAlgorithm  set "Add4"
    postFitSelectors  set $DeltaM
    postFitSelectors  set $CmsPStar
#    show
}

######################################################################
######################################################################

## merge the two Dstar -> D0 pi into one list
mod clone SmpMergerDefiner DstToD0PiToPhiGamma
path append $mySkimPath DstToD0PiToPhiGamma
catch { setProduction DstToD0PiToPhiGamma}
talkto DstToD0PiToPhiGamma {
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
mod clone SmpMergerDefiner DstToD0PiToKStarGamma
path append $mySkimPath DstToD0PiToKStarGamma
catch { setProduction DstToD0PiToKStarGamma}
talkto DstToD0PiToKStarGamma {
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
mod clone SmpMergerDefiner DstToD0PiToRhoGamma
path append $mySkimPath DstToD0PiToRhoGamma
catch { setProduction DstToD0PiToRhoGamma}
talkto DstToD0PiToRhoGamma {
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
mod clone SmpMergerDefiner DstToD0PiToOmegaGamma
path append $mySkimPath DstToD0PiToOmegaGamma
catch { setProduction DstToD0PiToOmegaGamma}
talkto DstToD0PiToOmegaGamma {
    debug             set $D0Debug
    verbose           set $D0Verbose
#   disableCloneCheck set true
    inputListNames    set $myDStarPlusOmega
    inputListNames    set $myDStarMinusOmega
#    show
}

######################################################################
######################################################################

## merge the two Dstar -> D0 pi into one list
mod clone SmpMergerDefiner DstToD0PiToGammaGamma
path append $mySkimPath DstToD0PiToGammaGamma
catch { setProduction DstToD0PiToGammaGamma}
talkto DstToD0PiToGammaGamma {
    debug             set $D0Debug
    verbose           set $D0Verbose
#   disableCloneCheck set true
    inputListNames    set $myDStarPlusGamma
    inputListNames    set $myDStarMinusGamma
#    show
}


######################################################################
######################################################################

mod clone TagFromList TagDstToD0PiToPhiGamma
path append $mySkimPath TagDstToD0PiToPhiGamma
talkto TagDstToD0PiToPhiGamma {
  listsToTag set DstToD0PiToPhiGamma
  tagName set TagDstToD0PiToPhiGamma
}

mod clone TagFromList TagDstToD0PiToKStarGamma
path append $mySkimPath TagDstToD0PiToKStarGamma
talkto TagDstToD0PiToKStarGamma {
  listsToTag set DstToD0PiToKStarGamma
  tagName set TagDstToD0PiToKStarGamma
}

mod clone TagFromList TagDstToD0PiToRhoGamma
path append $mySkimPath TagDstToD0PiToRhoGamma
talkto TagDstToD0PiToRhoGamma {
  listsToTag set DstToD0PiToRhoGamma
  tagName set TagDstToD0PiToRhoGamma
}

mod clone TagFromList TagDstToD0PiToOmegaGamma
path append $mySkimPath TagDstToD0PiToOmegaGamma
talkto TagDstToD0PiToOmegaGamma {
  listsToTag set DstToD0PiToOmegaGamma
  tagName set TagDstToD0PiToOmegaGamma
}

mod clone TagFromList TagDstToD0PiToGammaGamma
path append $mySkimPath TagDstToD0PiToGammaGamma
talkto TagDstToD0PiToGammaGamma {
  listsToTag set DstToD0PiToGammaGamma
  tagName set TagDstToD0PiToGammaGamma
}

module clone TagFilterByName DstToD0PiToVGammaFilter
path append $mySkimPath DstToD0PiToVGammaFilter
talkto DstToD0PiToVGammaFilter {
  orList set TagDstToD0PiToPhiGamma
  orList set TagDstToD0PiToKStarGamma
  orList set TagDstToD0PiToRhoGamma
  orList set TagDstToD0PiToOmegaGamma
  orList set TagDstToD0PiToGammaGamma
}

set writeUsrDataForSkim 0
