############################################################################
## $Id: DcToKKKPromptPath.tcl,v 1.3 2008/11/29 21:31:36 rmwhite Exp $
##
## Defines the DcToKKKPromptPath skim filter path
##
## This filter searches for the decay:
##
##   D+ -> K- K+ K+, with p*(D+) > 2.5
##
############################################################################

path create DcToKKKPromptPath

global BtaCandLists
lappend BtaCandLists DcToKKKPrompt

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
  set preCmsP                "CmsP 2.4:"
  set postCmsP               "CmsP 2.5:"

######################################################################
######################################################################

# D+ -> K- K+ K+

mod clone SmpMakerDefiner DcToKKKPrompt
path append DcToKKKPromptPath DcToKKKPrompt
catch { setProduction DcToKKKPrompt }
talkto DcToKKKPrompt {
  debug             set $D0Debug
  verbose           set $D0Verbose
  disableCloneCheck set true

  decayMode         set "D+ -> K- K+ K+"

  daughterListNames set "KCombinedSuperLoose"
  daughterListNames set "KCombinedSuperLoose"
  daughterListNames set "KCombinedSuperLoose"

  fittingAlgorithm  set "Cascade"
  fitConstraints    set "Geo"

  preFitSelectors   set $preCmsP
  preFitSelectors   set $verylooseMass

  postFitSelectors  set $ProbChiSqFastVtxTight
  postFitSelectors  set $postCmsP
  postFitSelectors  set $looseMass
}

path append DcToKKKPromptPath DcToKKKPromptSkim
talkto DcToKKKPromptSkim {
  inputList set "DcToKKKPrompt"
}
