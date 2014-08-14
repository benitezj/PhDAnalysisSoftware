#
# Selection of "slow" pi0 candidates from D*(2007) -> D0 pi0 decay for
# the purposes of pi0 efficiency and EMC calibration validation.
#
# $Id: NeutralDstartoD0pi0Path.tcl,v 1.2 2005/10/31 20:18:46 davidw Exp $
#
path create NeutralDstartoD0pi0Path


#
# We only bother with hadronic events
#
path append NeutralDstartoD0pi0Path FilterBGFMultiHadron


#
# Our D* list
#
# Chose a nice, standard D0 -> K pi list as one of the inputs,
# but stick to p* > 2 GeV
#
module clone SmpMakerDefiner NeutralDstartoD0pi0
path append NeutralDstartoD0pi0Path NeutralDstartoD0pi0
catch { setProduction NeutralDstartoD0pi0 }

module talk NeutralDstartoD0pi0
  decayMode set "D*0 -> D0 pi0"
  
  daughterListNames set "D0ToKPiLoosePID"
  daughterListNames set "pi0VeryLoose"
  
  fittingAlgorithm  set "Add4"
  
  preFitSelectors   set "DeltaM 0.125:0.180"
  preFitSelectors   set "CmsP 2.0:20"
  postFitSelectors  set "DeltaM 0.125:0.170"
  
  fitConstraints    set "Beam"
  fitConstraints    set "InvalidatePresentFit"
  fitSettings       set "UpdateDaughters"
exit

#
# Make the decision
#
module clone TagFromList NeutralDstartoD0pi0Tag
module talk NeutralDstartoD0pi0Tag
  listsToTag set NeutralDstartoD0pi0
  tagName    set NeutralDstartoD0pi0
exit

module clone TagFilterByName NeutralDstartoD0pi0Filter
module talk NeutralDstartoD0pi0Filter
  orList set NeutralDstartoD0pi0
exit

path append NeutralDstartoD0pi0Path NeutralDstartoD0pi0Tag
path append NeutralDstartoD0pi0Path NeutralDstartoD0pi0Filter

#
# Let's save our list to the skim 
#
global BtaCandLists
global writeUsrDataForSkim

set BtaCandLists NeutralDstartoD0pi0
set writeUsrDataForSkim 1
