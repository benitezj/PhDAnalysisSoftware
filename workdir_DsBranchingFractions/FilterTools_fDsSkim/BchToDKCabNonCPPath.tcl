#
# $Id: BchToDKCabNonCPPath.tcl,v 1.13 2005/11/16 23:01:57 fwinkl Exp $
# 
# defines B+ to DK filter path
#
# added by Qinglin Zeng

global BtaCandLists
global writeUsrDataForSkim
global UsrBCandLists

sourceFoundFile EventTagTools/TagProcs.tcl

path create BchToDKCabNonCPPath
path append BchToDKCabNonCPPath FilterBGFMultiHadron

# create tag bits from lists
sequence create TagBchToDKCabNonCPSequence
makeTagFromList BchToDKCabNonCP   BchToDKCabNonCP  TagBchToDKCabNonCPSequence     ;# added by Qinglin Zeng B+ -> D0 K+, B+ -> D*0 K+
path append BchToDKCabNonCPPath TagBchToDKCabNonCPSequence

# filter on tag bits
mod clone TagFilterByName BchToDKCabNonCPFilter
path append BchToDKCabNonCPPath BchToDKCabNonCPFilter
talkto BchToDKCabNonCPFilter {
#   andList set CabibboNonCP
   andList set BchToDKCabNonCP
}

#path append BchToDKCabNonCPPath BchToDKCabNonCPMicroFilter

# Add lists of composites to write
lappend BtaCandLists BchToDKCabNonCP

# Add lists of composites with UsrData
set UsrBCandLists $BtaCandLists
# Write UsrData
set writeUsrDataForSkim 1
