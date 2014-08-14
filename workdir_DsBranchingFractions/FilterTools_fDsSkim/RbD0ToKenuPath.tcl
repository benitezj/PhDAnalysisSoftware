#
# $Id: RbD0ToKenuPath.tcl,v 1.4 2005/11/05 01:19:43 rolfa Exp $
#
############################################################################
##
## Defines the RbD0ToKenuPath skim filter path
##
## This filter searches for the decay:
##
##   B+- -> D*0 K+-
##            |
##             -> D0 pi0
##                 |
##                  -> K- e+ nu
##
############################################################################

path create RbD0ToKenuPath

global BtaCandLists
lappend BtaCandLists RbBchToDstarD0Pi0SemiElec
lappend BtaCandLists RbBchToDstarD0Pi0SemiElecCS
lappend BtaCandLists RbBchToDstarD0gammaSemiElec
lappend BtaCandLists RbBchToDstarD0gammaSemiElecCS
lappend BtaCandLists RbBchToDstarD0Pi0SemiElecSS
lappend BtaCandLists RbBchToDstarD0Pi0SemiElecCSSS
lappend BtaCandLists RbBchToDstarD0gammaSemiElecSS
lappend BtaCandLists RbBchToDstarD0gammaSemiElecCSSS
lappend BtaCandLists RbBchToDstarSemiElec

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

#################################
## Hard Tracks
#################################

mod clone SmpSubListerDefiner RbElecHardTracks
path append RbD0ToKenuPath RbElecHardTracks
talkto RbElecHardTracks {
  unrefinedListName set GoodTracksHardVeryLoose
  selectors         set "CmsP 1.8:2.7"
}


# D0 -> K e nu

mod clone SmpMakerDefiner RbD0ToKenu
path append RbD0ToKenuPath RbD0ToKenu
catch { setProduction RbD0ToKenu }
talkto RbD0ToKenu {
  debug             set $D0Debug
  verbose           set $D0Verbose
  disableCloneCheck set true
  decayMode         set "D0 -> K- e+"
  daughterListNames set "KLHNotPion"
  daughterListNames set "PidLHElectrons"
  fittingAlgorithm  set "FastVtx"
  preFitSelectors   set $MassSemiLep
  postFitSelectors  set $MassSemiLep
  postFitSelectors  set $ProbChiSqFastVtx
}

mod clone SmpMakerDefiner RbD0ToKenuSS
path append RbD0ToKenuPath RbD0ToKenuSS
catch { setProduction RbD0ToKenuSS }
talkto RbD0ToKenuSS {
  debug             set $D0Debug
  verbose           set $D0Verbose
  disableCloneCheck set true
  decayMode         set "D0 -> K- e-"
  daughterListNames set "KLHNotPion"
  daughterListNames set "PidLHElectrons"
  checkType         set false
  fittingAlgorithm  set "FastVtx"
  preFitSelectors   set $MassSemiLep
  postFitSelectors  set $MassSemiLep
  postFitSelectors  set $ProbChiSqFastVtx
}

# D*0 -> D0 Pi0

mod clone SmpMakerDefiner RbDstar0ToD0Pi0SemiElec
path append RbD0ToKenuPath RbDstar0ToD0Pi0SemiElec
catch { setProduction RbDstar0ToD0Pi0SemiElec }
talkto RbDstar0ToD0Pi0SemiElec {
  debug             set $D0Debug
  verbose           set $D0Verbose
  disableCloneCheck set true
  decayMode         set "D*0 -> D0 pi0"
  daughterListNames set "RbD0ToKenu"
  daughterListNames set "pi0SoftLoose"
  fittingAlgorithm  set "Add4"
  postFitSelectors  set $DeltaMSemiLep
}

mod clone SmpMakerDefiner RbDstar0ToD0Pi0SemiElecSS
path append RbD0ToKenuPath RbDstar0ToD0Pi0SemiElecSS
catch { setProduction RbDstar0ToD0Pi0SemiElecSS }
talkto RbDstar0ToD0Pi0SemiElecSS {
  debug             set $D0Debug
  verbose           set $D0Verbose
  disableCloneCheck set true
  decayMode         set "D*0 -> D0 pi0"
  daughterListNames set "RbD0ToKenuSS"
  daughterListNames set "pi0SoftLoose"
  fittingAlgorithm  set "Add4"
  postFitSelectors  set $DeltaMSemiLep
}

# D*0 -> D0 gamma

mod clone SmpMakerDefiner RbDstar0ToD0gammaSemiElec
path append RbD0ToKenuPath RbDstar0ToD0gammaSemiElec
catch { setProduction RbDstar0ToD0gammaSemiElec }
talkto RbDstar0ToD0gammaSemiElec {
  debug             set $D0Debug
  verbose           set $D0Verbose
  disableCloneCheck set true
  decayMode         set "D*0 -> D0 gamma"
  daughterListNames set "RbD0ToKenu"
  daughterListNames set "GoodPhotonLoose"
  fittingAlgorithm  set "Add4"
  postFitSelectors  set $DeltaMSemiLep
}


mod clone SmpMakerDefiner RbDstar0ToD0gammaSemiElecSS
path append RbD0ToKenuPath RbDstar0ToD0gammaSemiElecSS
catch { setProduction RbDstar0ToD0gammaSemiElecSS }
talkto RbDstar0ToD0gammaSemiElecSS {
  debug             set $D0Debug
  verbose           set $D0Verbose
  disableCloneCheck set true
  decayMode         set "D*0 -> D0 gamma"
  daughterListNames set "RbD0ToKenuSS"
  daughterListNames set "GoodPhotonLoose"
  fittingAlgorithm  set "Add4"
  postFitSelectors  set $DeltaMSemiLep
}

# B- -> D*0 (-> D0 Pi0) K/pi- and color sup

mod clone SmpMakerDefiner RbBchToDstarD0Pi0SemiElec
path append RbD0ToKenuPath RbBchToDstarD0Pi0SemiElec
catch { setProduction RbBchToDstarD0Pi0SemiElec}
talkto RbBchToDstarD0Pi0SemiElec {
    disableCloneCheck set true
    decayMode          set "B- -> D*0 K-"
    daughterListNames  set "RbDstar0ToD0Pi0SemiElec"
    daughterListNames  set "RbElecHardTracks"
    checkType         set false
    fittingAlgorithm   set "Add4"
#    postFitSelectors   set "Mass 3.5:6."
}

mod clone SmpMakerDefiner RbBchToDstarD0Pi0SemiElecCS
path append RbD0ToKenuPath RbBchToDstarD0Pi0SemiElecCS
catch { setProduction RbBchToDstarD0Pi0SemiElecCS}
talkto RbBchToDstarD0Pi0SemiElecCS {
    disableCloneCheck set true
    decayMode          set "B+ -> D*0 K+"
    daughterListNames  set "RbDstar0ToD0Pi0SemiElec"
    daughterListNames  set "RbElecHardTracks"
    checkType         set false
    fittingAlgorithm   set "Add4"
#    postFitSelectors   set "Mass 3.5:6."
}


mod clone SmpMakerDefiner RbBchToDstarD0Pi0SemiElecSS
path append RbD0ToKenuPath RbBchToDstarD0Pi0SemiElecSS
catch { setProduction RbBchToDstarD0Pi0SemiElecSS}
talkto RbBchToDstarD0Pi0SemiElecSS {
    disableCloneCheck set true
    decayMode          set "B- -> D*0 K-"
    daughterListNames  set "RbDstar0ToD0Pi0SemiElecSS"
    daughterListNames  set "RbElecHardTracks"
    checkType         set false
    fittingAlgorithm   set "Add4"
#    postFitSelectors   set "Mass 3.5:6."
}

mod clone SmpMakerDefiner RbBchToDstarD0Pi0SemiElecCSSS
path append RbD0ToKenuPath RbBchToDstarD0Pi0SemiElecCSSS
catch { setProduction RbBchToDstarD0Pi0SemiElecCSSS}
talkto RbBchToDstarD0Pi0SemiElecCSSS {
    disableCloneCheck set true
    decayMode          set "B+ -> D*0 K+"
    daughterListNames  set "RbDstar0ToD0Pi0SemiElecSS"
    daughterListNames  set "RbElecHardTracks"
    checkType         set false
    fittingAlgorithm   set "Add4"
#    postFitSelectors   set "Mass 3.5:6."
}

# B- -> D*0 (-> D0 gamma) K/pi- and color sup

mod clone SmpMakerDefiner RbBchToDstarD0gammaSemiElec
path append RbD0ToKenuPath RbBchToDstarD0gammaSemiElec
catch { setProduction RbBchToDstarD0gammaSemiElec}
talkto RbBchToDstarD0gammaSemiElec {
    disableCloneCheck set true
    decayMode          set "B- -> D*0 K-"
    daughterListNames  set "RbDstar0ToD0gammaSemiElec"
    daughterListNames  set "RbElecHardTracks"
    checkType         set false
    fittingAlgorithm   set "Add4"
#    postFitSelectors   set "Mass 3.5:6."
}

mod clone SmpMakerDefiner RbBchToDstarD0gammaSemiElecCS
path append RbD0ToKenuPath RbBchToDstarD0gammaSemiElecCS
catch { setProduction RbBchToDstarD0gammaSemiElecCS}
talkto RbBchToDstarD0gammaSemiElecCS {
    disableCloneCheck set true
    decayMode          set "B+ -> D*0 K+"
    daughterListNames  set "RbDstar0ToD0gammaSemiElec"
    daughterListNames  set "RbElecHardTracks"
    checkType         set false
    fittingAlgorithm   set "Add4"
#    postFitSelectors   set "Mass 3.5:6."
}

mod clone SmpMakerDefiner RbBchToDstarD0gammaSemiElecSS
path append RbD0ToKenuPath RbBchToDstarD0gammaSemiElecSS
catch { setProduction RbBchToDstarD0gammaSemiElecSS}
talkto RbBchToDstarD0gammaSemiElecSS {
    disableCloneCheck set true
    decayMode          set "B- -> D*0 K-"
    daughterListNames  set "RbDstar0ToD0gammaSemiElecSS"
    daughterListNames  set "RbElecHardTracks"
    checkType         set false
    fittingAlgorithm   set "Add4"
#    postFitSelectors   set "Mass 3.5:6."
}

mod clone SmpMakerDefiner RbBchToDstarD0gammaSemiElecCSSS
path append RbD0ToKenuPath RbBchToDstarD0gammaSemiElecCSSS
catch { setProduction RbBchToDstarD0gammaSemiElecCSSS}
talkto RbBchToDstarD0gammaSemiElecCSSS {
    disableCloneCheck set true
    decayMode          set "B+ -> D*0 K+"
    daughterListNames  set "RbDstar0ToD0gammaSemiElecSS"
    daughterListNames  set "RbElecHardTracks"
    checkType         set false
    fittingAlgorithm   set "Add4"
#    postFitSelectors   set "Mass 3.5:6."
}

mod clone SmpMergerDefiner RbBchToDstarSemiElec
path append RbD0ToKenuPath RbBchToDstarSemiElec
catch { setProduction RbBchToDstarSemiElec }
talkto RbBchToDstarSemiElec {
  inputListNames set RbBchToDstarD0Pi0SemiElec
  inputListNames set RbBchToDstarD0Pi0SemiElecCS
  inputListNames set RbBchToDstarD0gammaSemiElec
  inputListNames set RbBchToDstarD0gammaSemiElecCS
  inputListNames set RbBchToDstarD0Pi0SemiElecSS
  inputListNames set RbBchToDstarD0Pi0SemiElecCSSS
  inputListNames set RbBchToDstarD0gammaSemiElecSS
  inputListNames set RbBchToDstarD0gammaSemiElecCSSS
}

path append RbD0ToKenuPath RbD0ToKenuSkim
talkto RbD0ToKenuSkim {
  inputList set RbBchToDstarSemiElec
}
