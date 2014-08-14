############################################################################
## $Id: DstToDPiToVGammaPath.tcl,v 1.1 2006/05/26 18:45:22 fwinkl Exp $
##
## Defines the dStarD0ToVEtaPath skim filter path
##
## This filter searches for the decay:
##
##   D* -> D Pi
##         D -> V Gamma
##
##   James Morris, 04/17/06
############################################################################



set mySkimPath DstToDPiToVGammaPath

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

##   D* -> D Pi
##         D -> V Gamma
#gamma
#
set hardGammaList   "GoodPhotonLoose"
set CmsPGamma "CmsP 0.35:"
#Vector Meson mass constraints
##
set rhoList            "rhoCVeryLooseMassAndPid"
set kaonList      "KstarKsPiSemiLoose"
#soft gamma
##
set piList    "pi0SoftLoose"
#D cuts
##
set looseMassD        "Mass 1.6:2.1"
## D* cuts
##
set DeltaM             "DeltaM 0.00:.170"
set CmsPStar           "CmsP 2.5:"

set KsList   "KsLoose"
set pionList "GoodTracksLoose"

#########################################################################
############################################################################


## photon selection
##
mod clone SmpSubListerDefiner SubGammaForD
path append $mySkimPath SubGammaForD
talkto SubGammaForD {
    debug             set $D0Debug
    verbose           set $D0Verbose
    unrefinedListName set $hardGammaList
    selectors         set $CmsPGamma    
}



###########################################################################
###########################################################################


## D -> rho gamma
## 
mod clone SmpMakerDefiner DToRhoGamma
path append $mySkimPath DToRhoGamma
talkto DToRhoGamma {
    decayMode         set "D+ -> rho+ gamma"
   disableCloneCheck set true
    daughterListNames set $rhoList
    daughterListNames set "SubGammaForD"
    fittingAlgorithm  set "Add4"
    postFitSelectors   set $looseMassD
}


## D*+ -> D+ Gamma
##            which goes to D -> Rho Gamma
##
mod clone SmpMakerDefiner DToRhoGamma_DStar
path append $mySkimPath DToRhoGamma_DStar
talkto DToRhoGamma_DStar {
    debug             set $D0Debug
    verbose           set $D0Verbose
    decayMode         set "D*+ -> D+ pi0"
  disableCloneCheck set true
    daughterListNames set "DToRhoGamma"
    daughterListNames set $piList
    fittingAlgorithm  set "Add4"
    postFitSelectors  set $DeltaM
    postFitSelectors  set $CmsPStar
#   show
}

mod clone TagFromList TagDToRhoGamma_DStar
path append $mySkimPath TagDToRhoGamma_DStar
talkto TagDToRhoGamma_DStar {
  listsToTag set DToRhoGamma_DStar
  tagName set TagDToRhoGamma_DStar
}




##########################################################################
##########################################################################

## D -> k* gamma
##
mod clone SmpMakerDefiner DToKStarGamma
path append $mySkimPath DToKStarGamma
talkto DToKStarGamma {
    decayMode         set "D+ -> K*+ gamma"
#    disableCloneCheck set true
    daughterListNames set "KstarChrgLooseFixed"
    daughterListNames set "SubGammaForD"
    fittingAlgorithm  set "Add4"
    postFitSelectors   set $looseMassD
}



##            which goes to D -> K* Gamma
mod clone SmpMakerDefiner DToKStarGamma_DStar
path append $mySkimPath DToKStarGamma_DStar
talkto DToKStarGamma_DStar {
    debug             set $D0Debug
    verbose           set $D0Verbose
    decayMode         set "D*+ -> D+ pi0"
#   disableCloneCheck set true
    daughterListNames set "DToKStarGamma"
    daughterListNames set $piList
    fittingAlgorithm  set "Add4"
    postFitSelectors  set $DeltaM
    postFitSelectors  set $CmsPStar
#    show
}


mod clone TagFromList TagDToKStarGamma_DStar
path append $mySkimPath TagDToKStarGamma_DStar
talkto TagDToKStarGamma_DStar {
  listsToTag set DToKStarGamma_DStar
  tagName set TagDToKStarGamma_DStar
}


######################################################################################################
######################################################################################################



module clone TagFilterByName DstToDPiToVGammaFilter
path append $mySkimPath DstToDPiToVGammaFilter
talkto DstToDPiToVGammaFilter {
  orList set TagDToRhoGamma_DStar
  orList set TagDToKStarGamma_DStar
}

set writeUsrDataForSkim 0

