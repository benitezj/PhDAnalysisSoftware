#
# $Id: RbD0ToKmunuPath.tcl,v 1.3 2005/11/03 21:23:53 rolfa Exp $
#
############################################################################
##
## Defines the RbD0ToKmunuPath skim filter path
##
## This filter searches for the decay:
##
##   B+- -> D*0 K+-
##            |
##             -> D0 pi0
##                 |
##                  -> K- mu+ nu
##
############################################################################

path create RbD0ToKmunuPath

global BtaCandLists
lappend BtaCandLists RbBchToDstarD0Pi0SemiMuon
lappend BtaCandLists RbBchToDstarD0Pi0SemiMuonCS
lappend BtaCandLists RbBchToDstarD0gammaSemiMuon
lappend BtaCandLists RbBchToDstarD0gammaSemiMuonCS
lappend BtaCandLists RbBchToDstarSemiMuon

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

#################################
## Hard Tracks
#################################

mod clone SmpSubListerDefiner RbMuonHardTracks
path append RbD0ToKmunuPath RbMuonHardTracks
talkto RbMuonHardTracks {
  unrefinedListName set GoodTracksHardVeryLoose
  selectors         set "CmsP 1.8:2.7"
}

# D0 -> K mu nu

mod clone SmpMakerDefiner RbD0ToKmunu
path append RbD0ToKmunuPath RbD0ToKmunu
catch { setProduction RbD0ToKmunu }
talkto RbD0ToKmunu {
  debug             set $D0Debug
  verbose           set $D0Verbose
  disableCloneCheck set true
  decayMode         set "D0 -> K- e+"
  daughterListNames set "KLHNotPion"
  daughterListNames set "muNNLoose"
  fittingAlgorithm  set "FastVtx"
  preFitSelectors   set $MassSemiLep
  postFitSelectors  set $MassSemiLep
  postFitSelectors  set $ProbChiSqFastVtx
}

# D*0 -> D0 Pi0

mod clone SmpMakerDefiner RbDstar0ToD0Pi0SemiMuon
path append RbD0ToKmunuPath RbDstar0ToD0Pi0SemiMuon
catch { setProduction RbDstar0ToD0Pi0SemiMuon }
talkto RbDstar0ToD0Pi0SemiMuon {
  debug             set $D0Debug
  verbose           set $D0Verbose
  disableCloneCheck set true
  decayMode         set "D*0 -> D0 pi0"
  daughterListNames set "RbD0ToKmunu"
  daughterListNames set "pi0SoftLoose"
  fittingAlgorithm  set "Add4"
  postFitSelectors  set $DeltaMSemiLep
}

# D*0 -> D0 gamma

mod clone SmpMakerDefiner RbDstar0ToD0gammaSemiMuon
path append RbD0ToKmunuPath RbDstar0ToD0gammaSemiMuon
catch { setProduction RbDstar0ToD0gammaSemiMuon }
talkto RbDstar0ToD0gammaSemiMuon {
  debug             set $D0Debug
  verbose           set $D0Verbose
  disableCloneCheck set true
  decayMode         set "D*0 -> D0 gamma"
  daughterListNames set "RbD0ToKmunu"
  daughterListNames set "GoodPhotonLoose"
  fittingAlgorithm  set "Add4"
  postFitSelectors  set $DeltaMSemiLep
}

# B- -> D*0 (-> D0 Pi0) K/pi- and color sup

mod clone SmpMakerDefiner RbBchToDstarD0Pi0SemiMuon
path append RbD0ToKmunuPath RbBchToDstarD0Pi0SemiMuon
catch { setProduction RbBchToDstarD0Pi0SemiMuon}
talkto RbBchToDstarD0Pi0SemiMuon {
    disableCloneCheck set true
    decayMode          set "B- -> D*0 K-"
    daughterListNames  set "RbDstar0ToD0Pi0SemiMuon"
    daughterListNames  set "RbMuonHardTracks"
    checkType         set false
    fittingAlgorithm   set "Add4"
#    postFitSelectors   set "Mass 3.5:6."
}

mod clone SmpMakerDefiner RbBchToDstarD0Pi0SemiMuonCS
path append RbD0ToKmunuPath RbBchToDstarD0Pi0SemiMuonCS
catch { setProduction RbBchToDstarD0Pi0SemiMuonCS}
talkto RbBchToDstarD0Pi0SemiMuonCS {
    disableCloneCheck set true
    decayMode          set "B+ -> D*0 K+"
    daughterListNames  set "RbDstar0ToD0Pi0SemiMuon"
    daughterListNames  set "RbMuonHardTracks"
    checkType         set false
    fittingAlgorithm   set "Add4"
#    postFitSelectors   set "Mass 3.5:6."
}

# B- -> D*0 (-> D0 gamma) K/pi- and color sup

mod clone SmpMakerDefiner RbBchToDstarD0gammaSemiMuon
path append RbD0ToKmunuPath RbBchToDstarD0gammaSemiMuon
catch { setProduction RbBchToDstarD0gammaSemiMuon}
talkto RbBchToDstarD0gammaSemiMuon {
    disableCloneCheck set true
    decayMode          set "B- -> D*0 K-"
    daughterListNames  set "RbDstar0ToD0gammaSemiMuon"
    daughterListNames  set "RbMuonHardTracks"
    checkType         set false
    fittingAlgorithm   set "Add4"
#    postFitSelectors   set "Mass 3.5:6."
}

mod clone SmpMakerDefiner RbBchToDstarD0gammaSemiMuonCS
path append RbD0ToKmunuPath RbBchToDstarD0gammaSemiMuonCS
catch { setProduction RbBchToDstarD0gammaSemiMuonCS}
talkto RbBchToDstarD0gammaSemiMuonCS {
    disableCloneCheck set true
    decayMode          set "B+ -> D*0 K+"
    daughterListNames  set "RbDstar0ToD0gammaSemiMuon"
    daughterListNames  set "RbMuonHardTracks"
    checkType         set false
    fittingAlgorithm   set "Add4"
#    postFitSelectors   set "Mass 3.5:6."
}

mod clone SmpMergerDefiner RbBchToDstarSemiMuon
path append RbD0ToKmunuPath RbBchToDstarSemiMuon
catch { setProduction RbBchToDstarSemiMuon }
talkto RbBchToDstarSemiMuon {
  inputListNames set RbBchToDstarD0Pi0SemiMuon
  inputListNames set RbBchToDstarD0Pi0SemiMuonCS
  inputListNames set RbBchToDstarD0gammaSemiMuon
  inputListNames set RbBchToDstarD0gammaSemiMuonCS
}

path append RbD0ToKmunuPath RbD0ToKmunuSkim
talkto RbD0ToKmunuSkim {
  inputList set RbBchToDstarSemiMuon
}
