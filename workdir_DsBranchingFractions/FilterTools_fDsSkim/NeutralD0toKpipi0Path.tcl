#
# A skim designed to produce an unbiased (and thus somewhat large) sample
# of D0 -> K- pi+ pi0 decays for EMC calibration validation.
#
# D*+ -> D0 pi+ tagging and a p* cut is used to increase the sample purity.
#
# $Id: NeutralD0toKpipi0Path.tcl,v 1.2 2005/10/31 20:18:46 davidw Exp $
#
path create NeutralD0toKpipi0Path

#
# We only bother with hadronic events
#
path append NeutralD0toKpipi0Path FilterBGFMultiHadron

#
# Because we want really huge sidebands, we make our own D0 list
#
# We do insist on p* > 2 GeV
#
module clone SmpMakerDefiner NeutralD0toKpipi0
path append NeutralD0toKpipi0Path NeutralD0toKpipi0
catch { setProduction NeutralD0toKpipi0 }

module talk NeutralD0toKpipi0
  decayMode set "D0 -> K- pi+ pi0"
  
  daughterListNames set "KLHLoose"
  daughterListNames set "GoodTracksLoose"
  daughterListNames set "pi0VeryLoose"
  
  fittingAlgorithm  set "Add4"
  
  preFitSelectors   set "Mass 1.6:2.1"
  preFitSelectors   set "CmsP 2.0:20"
  postFitSelectors  set "Mass 1.6:2.1"
  
  fitConstraints    set "Beam"
  fitConstraints    set "InvalidatePresentFit"
  fitSettings       set "UpdateDaughters"
exit

#
# Standard vertex constrained D* selection
#
module clone SmpMakerDefiner NeutralD0toKpipi0Dstar
path append NeutralD0toKpipi0Path NeutralD0toKpipi0Dstar
catch { setProduction NeutralD0toKpipi0Dstar }

module talk NeutralD0toKpipi0Dstar
  decayMode set "D*+ -> D0 pi+"
  
  daughterListNames set "NeutralD0toKpipi0"
  daughterListNames set "GoodTracksVeryLoose"
  
  fittingAlgorithm  set "Add4"
  
  preFitSelectors   set "DeltaM 0.130:0.190"
  postFitSelectors  set "DeltaM 0.130:0.170"
  postFitSelectors  set "ProbChiSq 0.0005:1"
  
  fitConstraints    set "Beam"
  fitSettings       set "UpdateDaughters"
exit


#
# Make the decision
#
module clone TagFromList NeutralD0toKpipi0Tag
module talk NeutralD0toKpipi0Tag
  listsToTag set NeutralD0toKpipi0Dstar
  tagName    set NeutralD0toKpipi0
exit

module clone TagFilterByName NeutralD0toKpipi0Filter
module talk NeutralD0toKpipi0Filter
  orList set NeutralD0toKpipi0
exit

path append NeutralD0toKpipi0Path NeutralD0toKpipi0Tag
path append NeutralD0toKpipi0Path NeutralD0toKpipi0Filter

#
# Let's save our list to the skim 
#
global BtaCandLists
global writeUsrDataForSkim

set BtaCandLists NeutralD0toKpipi0Dstar
set writeUsrDataForSkim 1
