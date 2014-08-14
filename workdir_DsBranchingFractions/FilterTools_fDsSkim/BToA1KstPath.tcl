#
# $Id: BToA1KstPath.tcl,v 1.2 2005/06/13 15:05:37 fnc Exp $
#
#
# see doc/BToA1Kst.readme for details
# 


sourceFoundFile SimpleComposition/SmpProcs.tcl
sourceFoundFile EventTagTools/TagProcs.tcl

path create BToA1KstPath

# Apply the MultiHadron filter first
path append BToA1KstPath FilterBGFMultiHadron

sourceFoundFile FilterTools/Q2BSequence.tcl
path append BToA1KstPath Q2BSequence

sequence create Q2B_A1KstSequence
path append BToA1KstPath Q2B_A1KstSequence

sequence append Q2B_A1KstSequence TagCombBToA1Kst0
sequence append Q2B_A1KstSequence TagCombBToA10Kst
sequence append Q2B_A1KstSequence TagCombB0ToA1Kst
sequence append Q2B_A1KstSequence TagCombB0ToA10Kst0

catch {module enable TagCombBToA1Kst0}
catch {module enable TagCombBToA10Kst}
catch {module enable TagCombB0ToA1Kst}
catch {module enable TagCombB0ToA10Kst0}

talkto TagCombB0ToA1Kst {
  A1List set "Q2B_A1_PiCPiCPiC"
  VList set "Q2BKst_K0pi"
  BOutList set "Q2B_B0ToA1CKstC"
  a1MassCutLo set 0.6
  a1MassCutHi set 1.8
  vMassCutLo set 0.45
  vMassCutHi set 1.1
  mESCutLo set 5.225
  mESCutHi set 5.3
  dEcut set 0.25
  listLengthCut set 1000000000.0
}

talkto TagCombB0ToA10Kst0 {
  A1List set "Q2B_A1_PiCPiCPi0"
  VList set "Q2BKst0_K0pi0"
  BOutList set "Q2B_B0ToA10Kst0"
  a1MassCutLo set 0.6
  a1MassCutHi set 1.8
  vMassCutLo set 0.45
  vMassCutHi set 1.1
  mESCutLo set 5.225
  mESCutHi set 5.3
  dEcut set 0.25
  listLengthCut set 1000000000.0
}

talkto TagCombBToA10Kst {
  A1List set "Q2B_A1_PiCPiCPi0"
  VList set "Q2BKst_K0pi"
  BOutList set "Q2B_BToA10Kst"
  a1MassCutLo set 0.6
  a1MassCutHi set 1.8
  vMassCutLo set 0.45
  vMassCutHi set 1.1
  mESCutLo set 5.225
  mESCutHi set 5.3
  dEcut set 0.25
  listLengthCut set 1000000000.0
}

talkto TagCombBToA1Kst0 {
  A1List set "Q2B_A1_PiCPiCPiC"
  VList set "Q2BKst0_K0pi0"
  BOutList set "Q2B_BToA1Kst0"
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
#  Simple Composition for B0 -> a1+Kst
#####################################################
createsmprefitter Q2B_A1KstSequence B0ToA1Kst {
  decayMode         set "B0 -> a_1+ K*-"
  unrefinedListName set "Q2B_B0ToA1CKstC"
  preFitSelectors   set "Mass 3.0:7.0"
  fittingAlgorithm  set "TreeFitter"  
  fitConstraints    set "Geo"
  postFitSelectors  set "Mes 5.245:5.3"
  postFitSelectors  set "DeltaE -0.2:0.2"
}

####################################################
#  Simple Composition for B0 -> a10Kst0
#####################################################
createsmprefitter Q2B_A1KstSequence B0ToA10Kst0 {
  decayMode         set "B0 -> a_10 K*0"
  unrefinedListName set "Q2B_B0ToA10Kst0"
  preFitSelectors   set "Mass 3.0:7.0"
  fittingAlgorithm  set "TreeFitter"  
  fitConstraints    set "Geo"
  postFitSelectors  set "Mes 5.245:5.3"
  postFitSelectors  set "DeltaE -0.2:0.2"
}

####################################################
#  Simple Composition for B+ -> a1+Kst0
#####################################################
createsmprefitter Q2B_A1KstSequence BToA1Kst0 {
  decayMode         set "B+ -> a_1+ K*0"
  unrefinedListName set "Q2B_BToA1Kst0"
  preFitSelectors   set "Mass 3.0:7.0"
  fittingAlgorithm  set "TreeFitter"  
  fitConstraints    set "Geo"
  postFitSelectors  set "Mes 5.245:5.3"
  postFitSelectors  set "DeltaE -0.2:0.2"
}

####################################################
#  Simple Composition for B+ -> a10Kst
#####################################################
createsmprefitter Q2B_A1KstSequence BToA10Kst {
  decayMode         set "B+ -> a_10 K*+"
  unrefinedListName set "Q2B_BToA10Kst"
  preFitSelectors   set "Mass 3.0:7.0"
  fittingAlgorithm  set "TreeFitter"  
  fitConstraints    set "Geo"
  postFitSelectors  set "Mes 5.245:5.3"
  postFitSelectors  set "DeltaE -0.2:0.2"
}

# Set tag bits out of the above lists
makeTagFromList B0ToA1Kst   TagBToA1Kst   Q2B_A1KstSequence
makeTagFromList B0ToA10Kst0 TagBToA10Kst0 Q2B_A1KstSequence
makeTagFromList BToA1Kst0   TagBToA1Kst0  Q2B_A1KstSequence
makeTagFromList BToA10Kst   TagBToA10Kst  Q2B_A1KstSequence

# Finally, filter on the tagbits
module clone TagFilterByName BToA1KstFilter
path append BToA1KstPath BToA1KstFilter

talkto BToA1KstFilter {
  orList set TagBToA1Kst
  orList set TagBToA10Kst0
  orList set TagBToA1Kst0
  orList set TagBToA10Kst
}

# Add lists of composites to write.
# These are the new lists to persist and they correspond to 
# the lists defining the tag bits selected.
global BtaCandLists
lappend BtaCandLists B0ToA1Kst B0ToA10Kst0 BToA1Kst0 BToA10Kst
global writeUsrDataForSkim
set writeUsrDataForSkim 0
