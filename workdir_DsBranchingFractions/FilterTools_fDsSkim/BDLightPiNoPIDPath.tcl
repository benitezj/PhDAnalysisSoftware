#
# $Id: BDLightPiNoPIDPath.tcl,v 1.12 2005/11/16 23:01:55 fwinkl Exp $
#
#
# defines BDLightPiNoPIDPath tag filter path
#

sourceFoundFile EventTagTools/TagProcs.tcl

global BtaCandLists
global writeUsrDataForSkim
global UsrBCandLists

path create BDLightPiNoPIDPath
# only run on MultiHadrons
path append BDLightPiNoPIDPath   FilterBGFMultiHadron

# create tag bits
sequence create TagBDLightPiNoPIDSequence
makeTagFromList BchToD0PiNoPIDDefault        BchToD0PiNoPID      TagBDLightPiNoPIDSequence  ;# B->D0pi/K decays with Kpi K3pi Kpipi0 KK PiPi modes and NoPID
makeTagFromList BchToDstar0PiNoPIDDefault    BchToDstar0PiNoPID  TagBDLightPiNoPIDSequence  ;# B0->Dstar0pi/K decays with KK PiPi modes and NoPID
path append  BDLightPiNoPIDPath TagBDLightPiNoPIDSequence


# filter on tag bits
mod clone TagFilterByName TagBchToDLightPiNoPIDFilter
talkto TagBchToDLightPiNoPIDFilter {
    orList set BchToD0PiNoPID
    orList set BchToDstar0PiNoPID
}

path append  BDLightPiNoPIDPath TagBchToDLightPiNoPIDFilter

# Add lists of composites to write
lappend BtaCandLists  BchToD0PiNoPIDDefault BchToDstar0PiNoPIDDefault

# Add lists of composites with UsrData
set UsrBCandLists $BtaCandLists
# Write UsrData
set writeUsrDataForSkim 1

