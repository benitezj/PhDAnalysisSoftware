############################################################################
## DcToKPiPiPromptNoPidPath.tcl,v 1.0 2006/08/10 rmwhite fcn Exp $ 
##
## Author: Ryan Mackenzie White, University of South Carolina
##
## Defines the DcToKPiPiPromptNoPidPath skim filter path
##
## This filter searches for the decay:
##
##   D+ -> K- Pi+ Pi+, with p*(D+) > 2.7
##   without requiring any PID -- to be used as PID control sample 
############################################################################

path create DcToKPiPiPromptNoPidPath
path append DcToKPiPiPromptNoPidPath FilterBGFMultiHadron
global BtaCandLists
lappend BtaCandLists DcToKPiPiPromptNoPid

#################################
## global control variables
#################################

  set D0Debug                false
  set D0Verbose              false

#########################
## SimpleComposition cuts
#########################

# FastVtx probability
  set ProbChiSqFastVtxTight  "ProbChiSq 0.005:"

# pre- and post-fit mass, p* cuts
  set verylooseMass          "Mass 1.5:2.3"
  set looseMass              "Mass 1.7:2.1"
  set preCmsP                "CmsP 2.5:"
  set postCmsP               "CmsP 2.7:"

######################################################################
######################################################################

# D+ -> K- Pi+ Pi+

mod clone SmpMakerDefiner DcToKPiPiPromptNoPid
path append DcToKPiPiPromptNoPidPath DcToKPiPiPromptNoPid
catch { setProduction DcToKPiPiPromptNoPid }
talkto DcToKPiPiPromptNoPid {
  debug             set $D0Debug
  verbose           set $D0Verbose
  disableCloneCheck set true

  decayMode         set "D+ -> K- pi+ pi+"

  daughterListNames set "GoodTracksVeryLoose"
  daughterListNames set "GoodTracksVeryLoose"
  daughterListNames set "GoodTracksVeryLoose"

  fittingAlgorithm  set "Cascade"
  fitConstraints    set "Geo"

  preFitSelectors   set $preCmsP
  preFitSelectors   set $verylooseMass

  postFitSelectors  set $ProbChiSqFastVtxTight
  postFitSelectors  set $postCmsP
  postFitSelectors  set $looseMass
}

#Filter on the list
mod clone FilterByList DcToKPiPiPromptNoPidFilter
talkto DcToKPiPiPromptNoPidFilter {
    orList set DcToKPiPiPromptNoPid
}

path append DcToKPiPiPromptNoPidPath DcToKPiPiPromptNoPidFilter
