#
# $Id: BRecoToDKPiPath.tcl,v 1.15 2005/11/16 23:01:56 fwinkl Exp $
#
#
# defines BRecoToDKPiPath tag filter path
#

global BtaCandLists
global writeUsrDataForSkim
global UsrBCandLists

path create BRecoToDKPiPath

# only run on MultiHadrons
path append BRecoToDKPiPath FilterBGFMultiHadron

# create tag bits
sequence create TagBRecoToDKPiSequence

makeTagFromList B0ToDchKsPiDefault       B0ToDchKsPi       TagBRecoToDKPiSequence    ;# B0->DKspi
makeTagFromList B0ToDstarcKsPiDefault    B0ToDstarcKsPi    TagBRecoToDKPiSequence    ;# B0->D*Kspi
makeTagFromList BchToD0KPi0Default       BchToD0KPi0       TagBRecoToDKPiSequence    ;# B->D0Kpi0
makeTagFromList BchToD0KPi0DefaultWS     BchToD0KPi0WS     TagBRecoToDKPiSequence    ;# B->D0barKpi0
makeTagFromList BchToDstar0KPi0Default   BchToDstar0KPi0   TagBRecoToDKPiSequence    ;# B->D*0Kpi0
makeTagFromList BchToDstar0KPi0DefaultWS BchToDstar0KPi0WS TagBRecoToDKPiSequence    ;# B->D*0barKpi0

path append BRecoToDKPiPath TagBRecoToDKPiSequence

# clone and customize filter module
mod clone TagFilterByName BRecoToDKPiFilter
path append BRecoToDKPiPath BRecoToDKPiFilter
talkto BRecoToDKPiFilter {
	orList set B0ToDchKsPi
	orList set B0ToDstarcKsPi
	orList set BchToD0KPi0
	orList set BchToD0KPi0WS
	orList set BchToDstar0KPi0
	orList set BchToDstar0KPi0WS
#	orList set B0ToD0KPi
#	orList set B0ToDstar0KPi
#        assertIfMissing set true
}


# Add lists of composites to write
lappend BtaCandLists B0ToDchKsPiDefault B0ToDstarcKsPiDefault BchToD0KPi0Default BchToD0KPi0DefaultWS BchToDstar0KPi0Default BchToDstar0KPi0DefaultWS

# Add lists of composites with UsrData
set UsrBCandLists $BtaCandLists
# Write UsrData
set writeUsrDataForSkim 1
