####################################################
#
# $Id: BToA1RhoPath.tcl,v 1.3 2006/03/21 22:18:45 fwinkl Exp $
#
#
# see doc/BToA1Rho.readme for details
# 
# Selection Cuts Applied for the different lists:
#
#  a1+  -> pi+pi-pi+
#  a10  -> pi+pi-pi0
#
# Decay Chain Lists
# -----------------
#   B0ToA1Rho:      B0 -> a1+rho-
#                   Mes      5.245:5.3       
#                   DeltaE   -0.20:0.20
#
#   B0ToA10Rho0:    B0 -> a10rho0
#
#   BToA1Rho0:      B+ -> a1+rho0
#
#   BToA10Rho:      B+ -> a10rho+
#
# A. Bevan Summer '04
####################################################
sourceFoundFile SimpleComposition/SmpProcs.tcl
sourceFoundFile EventTagTools/TagProcs.tcl

path create BToA1RhoPath

# Apply the MultiHadron filter first
path append BToA1RhoPath FilterBGFMultiHadron

sourceFoundFile FilterTools/Q2BSequence.tcl
path append BToA1RhoPath Q2BSequence

sequence create Q2B_A1RhoSequence
path append BToA1RhoPath Q2B_A1RhoSequence

#
# tag combinatorics done using modules of type TagBToA1V
#
sequence append Q2B_A1RhoSequence TagCombBToA1Rho0
sequence append Q2B_A1RhoSequence TagCombB0ToA1Rho
sequence append Q2B_A1RhoSequence TagCombB0ToA10Rho0
#sequence append Q2B_A1RhoSequence TagCombBToA10Rho
catch {module enable TagCombBToA1Rho0}
catch {module enable TagCombB0ToA1Rho}
catch {module enable TagCombB0ToA10Rho0}
#catch {module enable TagCombBToA10Rho}

talkto TagCombB0ToA1Rho {
  A1List set "Q2B_A1_PiCPiCPiC"
  VList set "Q2BRhoLH"
  BOutList set "Q2B_B0ToA1CRhoC"
  a1MassCutLo set 0.6
  a1MassCutHi set 1.8
  vMassCutLo set 0.45
  vMassCutHi set 1.1
  mESCutLo set 5.225
  mESCutHi set 5.3
  dEcut set 0.25
  listLengthCut set 1000000000.0
}

talkto TagCombB0ToA10Rho0 {
  A1List set "Q2B_A1_PiCPiCPi0"
  VList set "Q2BRho0LH"
  BOutList set "Q2B_B0ToA10Rho0"
  a1MassCutLo set 0.6
  a1MassCutHi set 1.8
  vMassCutLo set 0.45
  vMassCutHi set 1.1
  mESCutLo set 5.225
  mESCutHi set 5.3
  dEcut set 0.25
  listLengthCut set 1000000000.0
}

# switch off the a10rho+ final state as this has two pi0 mesons
# in it and so this mode is not that interesting for the first 
# round of studies (this also takes up half of the CPU time of 
# the a1rho skim as it is currently implemented)
#
#talkto TagCombBToA10Rho {
#    A1List set "Q2B_A1_PiCPiCPi0"
#    VList set "Q2BRhoLH"
#    BOutList set "Q2B_BToA10Rho"
#    a1MassCutLo set 0.6
#    a1MassCutHi set 1.8
#    vMassCutLo set 0.45
#    vMassCutHi set 1.1
#    mESCutLo set 5.225
#    mESCutHi set 5.3
#    dEcut set 0.25
#    listLengthCut set 1000000000.0
#}

talkto TagCombBToA1Rho0 {
  A1List set "Q2B_A1_PiCPiCPiC"
  VList set "Q2BRho0LH"
  BOutList set "Q2B_BToA1Rho0"
  a1MassCutLo set 0.6
  a1MassCutHi set 1.8
  vMassCutLo set 0.45
  vMassCutHi set 1.1
  mESCutLo set 5.225
  mESCutHi set 5.3
  dEcut set 0.25
  listLengthCut set 1000000000.0
}


####################################################
#  Simple Composition for B0 -> a1+rho-
#####################################################
createsmprefitter Q2B_A1RhoSequence B0ToA1Rho {
  decayMode         set "B0 -> a_1+ rho-"
  unrefinedListName set "Q2B_B0ToA1CRhoC"
  preFitSelectors   set "Mass 3.0:7.0"
  fittingAlgorithm  set "TreeFitter"  
  fitConstraints    set "Geo"
  postFitSelectors  set "Mes 5.245:5.3"
  postFitSelectors  set "DeltaE -0.2:0.2"
}

####################################################
#  Simple Composition for B0 -> a10rho0
#####################################################
createsmprefitter Q2B_A1RhoSequence B0ToA10Rho0 {
  decayMode         set "B0 -> a_10 rho0"
  unrefinedListName set "Q2B_B0ToA10Rho0"
  preFitSelectors   set "Mass 3.0:7.0"
  fittingAlgorithm  set "TreeFitter"  
  fitConstraints    set "Geo"
  postFitSelectors  set "Mes 5.245:5.3"
  postFitSelectors  set "DeltaE -0.2:0.2"
}

####################################################
#  Simple Composition for B+ -> a1+rho0
#####################################################
createsmprefitter Q2B_A1RhoSequence BToA1Rho0 {
  decayMode         set "B0 -> a_1+ rho0"
  unrefinedListName set "Q2B_BToA1Rho0"
  preFitSelectors   set "Mass 3.0:7.0"
  fittingAlgorithm  set "TreeFitter"  
  fitConstraints    set "Geo"
  postFitSelectors  set "Mes 5.245:5.3"
  postFitSelectors  set "DeltaE -0.2:0.2"
}

####################################################
#  Simple Composition for B+ -> a10rho
#####################################################
#mod clone SmpRefitterDefiner BToA10Rho
#talkto BToA10Rho {
#    decayMode         set "B0 -> a_10 rho+"
#    unrefinedListName set "Q2B_BToA10Rho"
#    preFitSelectors   set "Mass 3.0:7.0"
#    fittingAlgorithm  set "TreeFitter"  
#    fitConstraints    set "Geo"
#    postFitSelectors  set "Mes 5.245:5.3"
#    postFitSelectors  set "DeltaE -0.2:0.2"
#}


# Set tag bits out of the above lists
makeTagFromList B0ToA1Rho   TagBToA1Rho   Q2B_A1RhoSequence
makeTagFromList B0ToA10Rho0 TagBToA10Rho0 Q2B_A1RhoSequence
makeTagFromList BToA1Rho0   TagBToA1Rho0  Q2B_A1RhoSequence
#makeTagFromList BToA10Rho   TagBToA10Rho  Q2B_A1RhoSequence

# Finally, filter on the tagbits
module clone TagFilterByName BToA1RhoFilter
path append BToA1RhoPath BToA1RhoFilter

talkto BToA1RhoFilter {
  orList set TagBToA1Rho
  orList set TagBToA10Rho0
  orList set TagBToA1Rho0
#  orList set TagBToA10Rho
}

# Add lists of composites to write.
# These are the new lists to persist and they correspond to 
# the lists defining the tag bits selected.
global BtaCandLists
lappend BtaCandLists B0ToA1Rho B0ToA10Rho0 BToA1Rho0 BToA10Rho
global writeUsrDataForSkim
set writeUsrDataForSkim 0
