#
# $Id: BchToDKstCabNonCPPath.tcl,v 1.14 2005/11/16 23:01:57 fwinkl Exp $
# 
# defines B+ to DK filter path
#
# added by Qinglin Zeng

global BtaCandLists
global writeUsrDataForSkim
global UsrBCandLists

sourceFoundFile EventTagTools/TagProcs.tcl

path create BchToDKstCabNonCPPath
path append BchToDKstCabNonCPPath FilterBGFMultiHadron

sequence create TagBchToDKstCabNonCPSequence
makeTagFromList BchToDKstCabNonCP  BchToDKstCabNonCP  TagBchToDKstCabNonCPSequence   ;# added by Qinglin Zeng   B+ -> D*0 K+,  B+ -> D0 K*+
path append BchToDKstCabNonCPPath TagBchToDKstCabNonCPSequence

mod clone TagFilterByName BchToDKstCabNonCPFilter
path append BchToDKstCabNonCPPath BchToDKstCabNonCPFilter
talkto BchToDKstCabNonCPFilter {
#     andList set CabNonCPDKst
      andList set BchToDKstCabNonCP
} 
#path append BchToDKstCabNonCPPath BchToDKstCabNonCPMicroFilter

# Add lists of composites to write
lappend BtaCandLists BchToDKstCabNonCP

# Add lists of composites with UsrData
set UsrBCandLists $BtaCandLists
# Write UsrData
set writeUsrDataForSkim 1
