#
# $Id: BRecoToDsDsstarEtaPiPath.tcl,v 1.3 2005/11/16 23:01:56 fwinkl Exp $
#
#
# defines BRecoToDsDsstarEtaPiPath tag filter path
#

global BtaCandLists
global writeUsrDataForSkim
global UsrBCandLists

path create BRecoToDsDsstarEtaPiPath

# only run on MultiHadrons
path append BRecoToDsDsstarEtaPiPath FilterBGFMultiHadron

# create tag bits
sourceFoundFile EventTagTools/TagProcs.tcl
makeTagFromList B0ToDsEtaPiDefault      B0ToDsEtaPi      BRecoToDsDsstarEtaPiPath   ;# B0 -> Ds+ eta pi-
makeTagFromList B0ToDsstarEtaPiDefault  B0ToDsstarEtaPi  BRecoToDsDsstarEtaPiPath   ;# B0 -> Ds*+ eta pi-

# clone and customize filter module
mod clone TagFilterByName BRecoToDsDsstarEtaPiFilter
path append BRecoToDsDsstarEtaPiPath BRecoToDsDsstarEtaPiFilter
talkto BRecoToDsDsstarEtaPiFilter {
# new tagbits
  orList set B0ToDsEtaPi
  orList set B0ToDsstarEtaPi
}

# Add lists of composites to write
lappend BtaCandLists  B0ToDsEtaPi B0ToDsstarEtaPi

# Add lists of composites with UsrData
set UsrBCandLists $BtaCandLists 
# Write UsrData
set writeUsrDataForSkim 1
