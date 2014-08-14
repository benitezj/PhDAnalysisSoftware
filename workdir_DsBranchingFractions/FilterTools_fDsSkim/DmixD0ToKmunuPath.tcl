############################################################################
## $Id: DmixD0ToKmunuPath.tcl,v 1.3 2005/06/16 22:59:25 fnc Exp $
##
## Defines the DmixD0ToKmunuPath skim filter path
##
## This filter searches for the decay:
##
##   D*+ -> D0 pi+
##          |
##          -> K mu nu
##
############################################################################

path create DmixD0ToKmunuPath

global BtaCandLists
lappend BtaCandLists DstarToD0PiSemiMuon

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

# D0 -> K mu nu cuts
  set DeltaMSemiLep     "DeltaM 0.:0.7"
  set MassSemiLep       "Mass 0.:1.95"

######################################################################
######################################################################

# slow pion list
mod clone SmpSubListerDefiner DmixD0ToKmunuSlowPions
path append DmixD0ToKmunuPath DmixD0ToKmunuSlowPions
catch { setProduction DmixD0ToKmunuSlowPions }
talkto DmixD0ToKmunuSlowPions {
  debug             set $D0Debug
  verbose           set $D0Verbose
  unrefinedListName set "GoodTracksVeryLoose"
  selectors         set "CmsP 0.0:0.45"
}

# D0 -> K mu nu

mod clone SmpMakerDefiner D0ToKmunu
path append DmixD0ToKmunuPath D0ToKmunu
catch { setProduction D0ToKmunu }
talkto D0ToKmunu {
  debug             set $D0Debug
  verbose           set $D0Verbose
  disableCloneCheck set true
  decayMode         set "D0 -> K- mu+"
  daughterListNames set "KLHLoose"
  daughterListNames set "muNNLoose"
  fittingAlgorithm  set "Cascade"
  preFitSelectors   set $MassSemiLep
  postFitSelectors  set $MassSemiLep
  postFitSelectors  set $ProbChiSqFastVtx
}

mod clone SmpMakerDefiner DstarPlusToD0PiPlusSemiMuon
path append DmixD0ToKmunuPath DstarPlusToD0PiPlusSemiMuon
catch { setProduction DstarPlusToD0PiPlusSemiMuon }
talkto DstarPlusToD0PiPlusSemiMuon {
  debug             set $D0Debug
  verbose           set $D0Verbose
  disableCloneCheck set true
  decayMode         set "D*+ -> D0 pi+"
  daughterListNames set "D0ToKmunu"
  daughterListNames set "DmixD0ToKmunuSlowPions"
  fittingAlgorithm  set "Add4"
  postFitSelectors  set $DeltaMSemiLep
}

mod clone SmpMakerDefiner DstarMinusToD0PiMinusSemiMuon
path append DmixD0ToKmunuPath DstarMinusToD0PiMinusSemiMuon
catch { setProduction DstarMinusToD0PiMinusSemiMuon }
talkto DstarMinusToD0PiMinusSemiMuon {
  debug             set $D0Debug
  verbose           set $D0Verbose
  disableCloneCheck set true
  decayMode         set "D*- -> D0 pi-"
  daughterListNames set "D0ToKmunu"
  daughterListNames set "DmixD0ToKmunuSlowPions"
  fittingAlgorithm  set "Add4"
  postFitSelectors  set $DeltaMSemiLep
}

mod clone SmpMergerDefiner DstarToD0PiSemiMuon
path append DmixD0ToKmunuPath DstarToD0PiSemiMuon
catch { setProduction DstarToD0PiSemiMuon }
talkto DstarToD0PiSemiMuon {
  debug             set $D0Debug
  verbose           set $D0Verbose
  disableCloneCheck set true
  inputListNames    set "DstarPlusToD0PiPlusSemiMuon"
  inputListNames    set "DstarMinusToD0PiMinusSemiMuon"
}

path append DmixD0ToKmunuPath DmixD0ToKmunuSkim
talkto DmixD0ToKmunuSkim {
  inputList set DstarToD0PiSemiMuon
}
