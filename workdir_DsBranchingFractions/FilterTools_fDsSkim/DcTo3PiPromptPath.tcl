############################################################################
## $Id: DcTo3PiPromptPath.tcl,v 1.4 2008/12/01 22:20:06 gjackson Exp $
##
## Defines the DcTo3PiPromptPath skim filter path
##
## This filter searches for the decay:
##
##   D+ -> pi- pi+ pi+, with p*(D+) > 2.7
##
############################################################################

path create DcTo3PiPromptPath

global BtaCandLists
lappend BtaCandLists DcTo3PiPrompt

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
  set preCmsP                "CmsP 2.0:"
  set postCmsP               "CmsP 2.4:"

######################################################################
######################################################################

# D+ -> pi- pi+ pi+

mod clone SmpMakerDefiner DcTo3PiPrompt
path append DcTo3PiPromptPath DcTo3PiPrompt
catch { setProduction DcTo3PiPrompt }
talkto DcTo3PiPrompt {
  debug             set $D0Debug
  verbose           set $D0Verbose
  disableCloneCheck set true

  decayMode         set "D+ -> pi- pi+ pi+"

  daughterListNames set "piCombinedSuperLoose"
  daughterListNames set "piCombinedSuperLoose"
  daughterListNames set "piCombinedSuperLoose"

  fittingAlgorithm  set "Cascade"
  fitConstraints    set "Geo"

  preFitSelectors   set $preCmsP
  preFitSelectors   set $verylooseMass

  postFitSelectors  set $ProbChiSqFastVtxTight
  postFitSelectors  set $postCmsP
  postFitSelectors  set $looseMass
}

path append DcTo3PiPromptPath DcTo3PiPromptSkim
talkto DcTo3PiPromptSkim {
  inputList set "DcTo3PiPrompt"
}
