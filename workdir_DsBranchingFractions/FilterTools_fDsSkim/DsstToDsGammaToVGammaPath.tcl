############################################################################
## $Id: DsstToDsGammaToVGammaPath.tcl,v 1.1 2006/05/26 18:45:21 fwinkl Exp $
##
## Defines the dStarD0ToVEtaPath skim filter path
##
## This filter searches for the decay:
##
##   Ds* -> Ds Gamma
##          Ds -> V Gamma
##
##   James Morris, 04/17/06
############################################################################


set mySkimPath DsstToDsGammaToVGammaPath

global writeUsrDataForSkim

path create $mySkimPath



#################################
#global control variables
#################################

set D0Debug false
set D0Verbose false



#################################
## SimpleComposition Cuts
#################################

##   Ds* -> Ds Gamma
##          Ds -> V Gamma
## gamma
##
set hardGammaList   "GoodPhotonLoose"
set CmsPGamma "CmsP 0.35:"
#Vector Meson mass constraints
##
set rhoList            "rhoCVeryLooseMassAndPid"
#set kaonList      "KstarKsPiSemiLoose"
#soft gamma
##
set softGammaList    "GoodPhotonLoose"
#Ds cuts
##
set looseMassDs        "Mass 1.7:2.3"
## Ds* cuts
##
set DeltaM             "DeltaM 0.08:.20"
set CmsPStar           "CmsP 2.5:"

###########################################################################
###########################################################################

## photon selection
##
mod clone SmpSubListerDefiner SubGammaForDs
path append $mySkimPath SubGammaForDs
talkto SubGammaForDs {
    debug             set $D0Debug
    verbose           set $D0Verbose
    unrefinedListName set $hardGammaList
    selectors         set $CmsPGamma
}

###########################################################################
###########################################################################

### RHO LIST ###


## Ds -> rho gamma
## 
mod clone SmpMakerDefiner DsToRhoGamma
path append $mySkimPath DsToRhoGamma
talkto DsToRhoGamma {
    decayMode         set "D_s+ -> rho+ gamma"
   disableCloneCheck set true
    daughterListNames set $rhoList
    daughterListNames set "SubGammaForDs"
    fittingAlgorithm  set "Add4"
    postFitSelectors   set $looseMassDs
}


## Ds*+ -> Ds+ Gamma
##            which goes to Ds -> Rho Gamma
##
mod clone SmpMakerDefiner DsToRhoGamma_DsStar
path append $mySkimPath DsToRhoGamma_DsStar
talkto DsToRhoGamma_DsStar {
    debug             set $D0Debug
    verbose           set $D0Verbose
    decayMode         set "D_s*+ -> D_s+ gamma"
  disableCloneCheck set true
    daughterListNames set "DsToRhoGamma"
    daughterListNames set $softGammaList
    fittingAlgorithm  set "Add4"
    postFitSelectors  set $DeltaM
    postFitSelectors  set $CmsPStar
#   show
}

mod clone TagFromList TagDsToRhoGamma_DsStar
path append $mySkimPath TagDsToRhoGamma_DsStar
talkto TagDsToRhoGamma_DsStar {
  listsToTag set DsToRhoGamma_DsStar
  tagName set TagDsToRhoGamma_DsStar
}

###########################################################################
###########################################################################


## Ds -> k* gamma
##
mod clone SmpMakerDefiner DsToKStarGamma
path append $mySkimPath DsToKStarGamma
talkto DsToKStarGamma {
    decayMode         set "D_s+ -> K*+ gamma"
#    disableCloneCheck set true
    daughterListNames set "KstarChrgLooseFixed"
    daughterListNames set "SubGammaForDs"
    fittingAlgorithm  set "Add4"
    postFitSelectors   set $looseMassDs
}

##            which goes to Ds -> K* Gamma
mod clone SmpMakerDefiner DsToKStarGamma_DsStar
path append $mySkimPath DsToKStarGamma_DsStar
talkto DsToKStarGamma_DsStar {
    debug             set $D0Debug
    verbose           set $D0Verbose
    decayMode         set "D_s*+ -> D_s+ gamma"
#   disableCloneCheck set true
    daughterListNames set "DsToKStarGamma"
    daughterListNames set $softGammaList
    fittingAlgorithm  set "Add4"
    postFitSelectors  set $DeltaM
    postFitSelectors  set $CmsPStar
#    show
}

mod clone TagFromList TagDsToKStarGamma_DsStar
path append $mySkimPath TagDsToKStarGamma_DsStar
talkto TagDsToKStarGamma_DsStar {
  listsToTag set DsToKStarGamma_DsStar
  tagName set TagDsToKStarGamma_DsStar
}




##############################################################################
##############################################################################




module clone TagFilterByName DsstToDsGammaToVGammaFilter
path append $mySkimPath DsstToDsGammaToVGammaFilter
talkto DsstToDsGammaToVGammaFilter {
  orList set TagDsToRhoGamma_DsStar
  orList set TagDsToKStarGamma_DsStar
}

set writeUsrDataForSkim 0

