#
# $Id: BToDOmegaPiPath.tcl,v 1.13 2005/06/25 13:48:27 fnc Exp $
#
# defines BToDstarOmegaPiPath filter path
#

## for writing composite candidates
global BtaCandLists
global writeUsrDataForSkim
global UsrBCandLists
##

sourceFoundFile EventTagTools/TagProcs.tcl

path create BToDOmegaPiPath

# only run on MultiHadrons
path append BToDOmegaPiPath FilterBGFMultiHadron

sourceFoundFile CompositionSequences/CompBToDOmegaPiSequence.tcl
path append BToDOmegaPiPath CompBToDOmegaPiSequence
sequence enable CompBToDOmegaPiSequence

makeTagFromList B0ToDstarOmegaPiLoose    B0ToDstarOmegaPi  CompBToDOmegaPiSequence  ;# B0->D*-OmegaPi+
makeTagFromList B0ToDchOmegaPiLoose      B0ToDchOmegaPi    CompBToDOmegaPiSequence  ;# B0->D-OmegaPi+
makeTagFromList BchToD0OmegaPiLoose      BchToD0OmegaPi    CompBToDOmegaPiSequence  ;# B-->D0OmegaPi-
makeTagFromList BchToDstar0OmegaPiDstar0ToD0Pi0Loose   BchToDstar0OmegaPiDstar0ToD0Pi0   CompBToDOmegaPiSequence ;# B-->D*0OmegaPi- D*0->D0pi0
makeTagFromList BchToDstar0OmegaPiDstar0ToD0GammaLoose BchToDstar0OmegaPiDstar0ToD0Gamma CompBToDOmegaPiSequence ;# tag B-->D*0OmegaPi- D*0->D0 gamma

# and now filter
mod clone TagFilterByName BToDOmegaPiFilter
path append BToDOmegaPiPath BToDOmegaPiFilter
talkto BToDOmegaPiFilter { 
    orList set B0ToDstarOmegaPi
    orList set B0ToDchOmegaPi
    orList set BchToDstar0OmegaPiDstar0ToD0Pi0
    orList set BchToDstar0OmegaPiDstar0ToD0Gamma
    orList set BchToD0OmegaPi
}

# Add lists of composites to write
lappend BtaCandLists B0ToDstarOmegaPiLoose B0ToDchOmegaPiLoose BchToDstar0OmegaPiDstar0ToD0GammaLoose BchToD0OmegaPiLoose

# Add lists of composites with UsrData
set UsrBCandLists $BtaCandLists
# Write UsrData
set writeUsrDataForSkim 1
