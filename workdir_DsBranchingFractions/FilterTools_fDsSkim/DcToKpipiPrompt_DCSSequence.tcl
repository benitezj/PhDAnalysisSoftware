############################################################################
## $Id: DcToKpipiPrompt_DCSSequence.tcl,v 1.4 2008/11/29 21:31:36 rmwhite Exp $
##
## Defines the DcToKpipiPromptPath skim filter path
##
## This filter searches for the double Cabibbo-suppressed decay:
##
##   D+ -> K+ pi+ pi-, with p*(D+) > 2.7
##
############################################################################

sequence create DcToKpipiPrompt_DCSSequence

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

# D+ -> K+ pi+ pi-

mod clone SmpMakerDefiner DcToKpipiPrompt_DCS
sequence append DcToKpipiPrompt_DCSSequence DcToKpipiPrompt_DCS
catch { setProduction DcToKpipiPrompt_DCS }
talkto DcToKpipiPrompt_DCS {
  debug             set $D0Debug
  verbose           set $D0Verbose
  disableCloneCheck set true

  decayMode         set "D+ -> K+ pi+ pi-"

  daughterListNames set "KCombinedSuperLoose"
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
