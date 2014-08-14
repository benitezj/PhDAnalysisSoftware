####################################################
#
# $Id: B0ToRhoPRhoMPath.tcl,v 1.3 2005/06/13 15:05:37 fnc Exp $
#
# defines the B0ToRhoPRhoMPath tag filter path
#
# There are several lists created in this file:
#
#  Q2B_Rho
#  Q2B_B0ToRhoPRhoM
#  B0toRhoPRhoM_FastTrack
#  B0toRhoPRhoM_Default
#
# and the following tags are made from the lists
#
#    List Name             Tag Name
#    B0toRhoPRhoM_Default    TagB0toRhoPRhoP_Default
#    B0toRhoPRhoM_FastTrack  TagB0toRhoMRhoM_FastTrack
#
# Selection Cuts Applied for the different lists:
#
# rho mesons:
# -----------
#   mass      0.4   -- 1.1
#   fitter    Add4
#
#   The rho meson decays/output lists are:
#
#   Q2B_Rho:   rho+ -> pi+pi0
#
# 'tight' rho mesons:
# -------------------
#   mass      0.45   -- 1.05
#   Helicity -0.95:1.00
#   fitter    Add4
#             <= 50 candidates in list
#
#   The rho meson decays/output lists are:
#
#   Q2BTightRho0:  rho0 -> pi+pi-
#   Q2BTightRho:   rho+ -> pi+pi0
#
# Decay Chain Lists
# -----------------
#   B0toRhoRho_Default:      B0 -> rho+rho-
#                   This list is the combination of two Q2BTightRho particles
#
#   pre-fit selection   Mass:  3.0:7.0
#   fitter    TreeFitter with a Geo constraint
#   post-fit selection   Mass:  3.0:7.0
#                        Mes      5.23:5.3       
#                        DeltaE   -0.2:0.2
#
#   B0toRhoRho_FastTrack:    B0 -> rho+rho-
#                   This list is the combination of two Q2BTightRho particles
#
#   pre-fit selection   Mass:  3.0:7.0
#   fitter    GeoKin with a Geo constraint
#   post-fit selection   Mass:  3.0:7.0
#                        Mes      5.25:5.3       
#                        DeltaE   -0.17:0.17
#
####################################################
sourceFoundFile SimpleComposition/SmpProcs.tcl
sourceFoundFile EventTagTools/TagProcs.tcl

path create B0ToRhoPRhoMPath

# Apply the MultiHadron filter first
path append B0ToRhoPRhoMPath FilterBGFMultiHadron

# Now the RhoPRhoMSequence
sequence create Q2B_RhoPRhoMSequence
path append B0ToRhoPRhoMPath Q2B_RhoPRhoMSequence

catch {module enable TagB0ToRhoPRhoM}
sequence append Q2B_RhoPRhoMSequence TagB0ToRhoPRhoM
talkto TagB0ToRhoPRhoM {
  TrackList  set "GoodTracksVeryLoose"
  Pi0List    set "pi0LooseMass"
  RhoOutList set "Q2B_Rho"
  B0OutList  set "Q2B_B0ToRhoPRhoM"
  rhoMassCutLo set 0.45
  rhoMassCutHi set 1.05
  helicityCutLo set -0.95
  helicityCutHi set 1.00
  mESCutLo set 5.2
  mESCutHi set 5.3
}

createsmprefitter Q2B_RhoPRhoMSequence B0ToRhoPRhoM_Default {
  decayMode set "B0 -> rho+ rho-"
  unrefinedListName set "Q2B_B0ToRhoPRhoM"
  preFitSelectors   set "Mass 3.0:7.0"
  fittingAlgorithm  set "TreeFitter"
  fitConstraints    set "Geo"
  postFitSelectors  set "Mass 3.0:7.0"
  postFitSelectors  set "Mes 5.23:5.3"
  postFitSelectors  set "DeltaE -0.2:0.2"
}

#GF Use sublister to define the sub-list
createsmpsublister Q2B_RhoPRhoMSequence B0ToRhoPRhoM_FastTrack {
  unrefinedListName set "B0ToRhoPRhoM_Default"
  selectors         set "Mes 5.25:5.3"
  selectors         set "DeltaE -0.17:0.17"
}

# Set tag bits out of the above lists
makeTagFromList B0ToRhoPRhoM_Default   TagB0ToRhoPRhoM_Default   Q2B_RhoPRhoMSequence
makeTagFromList B0ToRhoPRhoM_FastTrack TagB0ToRhoPRhoM_FastTrack Q2B_RhoPRhoMSequence

# Finally, filter on the tagbits
module clone TagFilterByName B0ToRhoPRhoMFilter
path append B0ToRhoPRhoMPath B0ToRhoPRhoMFilter
talkto  B0ToRhoPRhoMFilter {
  andList set BGFMultiHadron
  andList set TagB0ToRhoPRhoM_Default
#  andList set TagB0ToRhoPRhoM_FastTrack
}

# Add lists of composites to write.
# These are the new lists to persist and they correspond to 
# the lists defining the tag bits selected.
global BtaCandLists
lappend BtaCandLists B0ToRhoPRhoM_Default B0ToRhoPRhoM_FastTrack
global writeUsrDataForSkim
set writeUsrDataForSkim 0
