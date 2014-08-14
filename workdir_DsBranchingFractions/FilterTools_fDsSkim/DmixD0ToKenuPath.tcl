############################################################################
## $Id: DmixD0ToKenuPath.tcl,v 1.2 2005/06/16 22:59:25 fnc Exp $
##
## Defines the DmixD0ToKenuPath skim filter path
##
## This filter searches for the decay:
##
##   D*+ -> D0 pi+
##          |
##          -> K e nu
##
############################################################################

path create DmixD0ToKenuPath

global BtaCandLists
lappend BtaCandLists DstarToD0PiSemiElec

#################################
## global control variables
#################################

  set D0Debug false
  set D0Verbose false

#################################
## SimpleComposition Cuts
#################################

# FastVtx probability
  set ProbChiSqFastVtx  "ProbChiSq 0.005:"

# D0 -> K e nu cuts
  set DeltaMSemiLep     "DeltaM 0.:0.7"
  set MassSemiLep       "Mass 0.:1.82"

######################################################################
######################################################################

# slow pion list
mod clone SmpSubListerDefiner DmixD0ToKenuSlowPions
path append DmixD0ToKenuPath DmixD0ToKenuSlowPions
catch { setProduction DmixD0ToKenuSlowPions }
talkto DmixD0ToKenuSlowPions {
  debug             set $D0Debug
  verbose           set $D0Verbose
  unrefinedListName set "GoodTracksVeryLoose"
  selectors         set "CmsP 0.0:0.45"
}

# D0 -> K e nu

mod clone SmpMakerDefiner D0ToKenu
path append DmixD0ToKenuPath D0ToKenu
catch { setProduction D0ToKenu }
talkto D0ToKenu {
  debug             set $D0Debug
  verbose           set $D0Verbose
  disableCloneCheck set true
  decayMode         set "D0 -> K- e+"
  daughterListNames set "KLHLoose"
  daughterListNames set "PidLHElectrons"
  fittingAlgorithm  set "Cascade"
  preFitSelectors   set $MassSemiLep
  postFitSelectors  set $MassSemiLep
  postFitSelectors  set $ProbChiSqFastVtx
}

mod clone SmpMakerDefiner DstarPlusToD0PiPlusSemiElec
path append DmixD0ToKenuPath DstarPlusToD0PiPlusSemiElec
catch { setProduction DstarPlusToD0PiPlusSemiElec }
talkto DstarPlusToD0PiPlusSemiElec {
  debug             set $D0Debug
  verbose           set $D0Verbose
  disableCloneCheck set true
  decayMode         set "D*+ -> D0 pi+"
  daughterListNames set "D0ToKenu"
  daughterListNames set "DmixD0ToKenuSlowPions"
  fittingAlgorithm  set "Add4"
  postFitSelectors  set $DeltaMSemiLep
}

mod clone SmpMakerDefiner DstarMinusToD0PiMinusSemiElec
path append DmixD0ToKenuPath DstarMinusToD0PiMinusSemiElec
catch { setProduction DstarMinusToD0PiMinusSemiElec }
talkto DstarMinusToD0PiMinusSemiElec {
  debug             set $D0Debug
  verbose           set $D0Verbose
  disableCloneCheck set true
  decayMode         set "D*- -> D0 pi-"
  daughterListNames set "D0ToKenu"
  daughterListNames set "DmixD0ToKenuSlowPions"
  fittingAlgorithm  set "Add4"
  postFitSelectors  set $DeltaMSemiLep
}

mod clone SmpMergerDefiner DstarToD0PiSemiElec
path append DmixD0ToKenuPath DstarToD0PiSemiElec
catch { setProduction DstarToD0PiSemiElec }
talkto DstarToD0PiSemiElec {
  debug             set $D0Debug
  verbose           set $D0Verbose
  disableCloneCheck set true
  inputListNames    set "DstarPlusToD0PiPlusSemiElec"
  inputListNames    set "DstarMinusToD0PiMinusSemiElec"
}

path append DmixD0ToKenuPath DmixD0ToKenuSkim
talkto DmixD0ToKenuSkim {
  inputList set DstarToD0PiSemiElec
}
