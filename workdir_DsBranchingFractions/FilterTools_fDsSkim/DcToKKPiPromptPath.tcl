############################################################################
## DcToKKPiPromptPath.tcl,v 1.0 2006/08/03 rmwhite Exp $
##
## Author: Ryan Mackenzie White, University of South Carolina
##
## Defines the DcToKKPiPromptPath skim filter path
##
## This filter searches for the decay:
##
##   D+ -> K- K+ Pi+, with p*(D+) > 2.4
##   
############################################################################

path create DcToKKPiPromptPath
path append DcToKKPiPromptPath FilterBGFMultiHadron
global BtaCandLists
lappend BtaCandLists DcToKKPiPrompt

#########################
## SimpleComposition cuts
#########################

# FastVtx probability
  set ProbChiSqVtx  "ProbChiSq 0.005:"

# pre- and post-fit mass, p* cuts
  set preMass       "Mass 1.5:2.3"
  set postMass      "Mass 1.7:2.1"
  set preCmsP       "CmsP 2.0:"
  set postCmsP      "CmsP 2.4:"


######################################################################
######################################################################

# D+ -> K- K+ Pi+

mod clone SmpMakerDefiner DcToKKPiPrompt
path append DcToKKPiPromptPath DcToKKPiPrompt
catch { setProduction DcToKKPiPrompt }
talkto DcToKKPiPrompt {
  disableCloneCheck set true

  decayMode         set "D+ -> K+ K- pi+"

  daughterListNames set "KCombinedSuperLoose"
  daughterListNames set "KCombinedSuperLoose"
  daughterListNames set "GoodTracksVeryLoose"

  fittingAlgorithm  set "Cascade"
  preFitSelectors   set $preCmsP
  preFitSelectors   set $preMass

  postFitSelectors  set $ProbChiSqVtx
  postFitSelectors  set $postCmsP
  postFitSelectors  set $postMass
}

#Filter on the list
mod clone FilterByList DcToKKPiPromptFilter
talkto DcToKKPiPromptFilter {
    orList set DcToKKPiPrompt
}

path append DcToKKPiPromptPath DcToKKPiPromptFilter
