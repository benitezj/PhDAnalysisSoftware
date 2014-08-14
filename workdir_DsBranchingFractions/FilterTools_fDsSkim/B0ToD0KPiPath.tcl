#
# $Id: B0ToD0KPiPath.tcl,v 1.17 2005/11/17 02:58:03 fwinkl Exp $
#
# defines B0ToD0KPiPath tag filter path
#
#  Chih-hsiang Cheng 2003-06-10
#

## for writing composite candidates
global BtaCandLists
global writeUsrDataForSkim
global UsrBCandLists
##

sourceFoundFile EventTagTools/TagProcs.tcl

path create B0ToD0KPiPath

path append B0ToD0KPiPath FilterBGFMultiHadron

path append B0ToD0KPiPath B0ToD0KPiTagFilter
talkto B0ToD0KPiTagFilter {
    nTracksName set nTracks
    nTrkMinimum set 5
}

### Do I really want to filter on these D0 modes? There is no CP=-1...
##sourceFoundFile FilterTools/DmesonCommonSequence.tcl
##mod clone TagFilterMomentumCut D0KPiD0TagFilter
##setDmesonDecayModes D0KPiD0TagFilter
##talkto D0KPiD0TagFilter {
##    orListCut clear
##    orListCut set 99
##    orListCut set 99
##    orListCut set 99
##    orListCut set 1
##    doHisto set false
##}
##path append B0ToD0KPiPath D0KPiD0TagFilter


# create tag bits from lists
sequence create TagB0ToD0KPiSequence

# ---- B0 -> D0 K+ pi-   C.H.Cheng  2003-06-11
proc makeB0ToD0KPiTag listname {
	module clone TagFromList Tag${listname}
	addTagModule Tag${listname} TagB0ToD0KPiSequence {
		listsToTag   set $listname
		tagName      set $listname
	}
}

set cpstates NonCP,barNonCP,CPEven,CPOdd
foreach cpstate [ split $cpstates ',' ] {
	makeB0ToD0KPiTag B0ToD0${cpstate}KPi
	makeB0ToD0KPiTag B0ToDstar0${cpstate}KPi
	makeB0ToD0KPiTag B0ToD0${cpstate}KstarPi
	makeB0ToD0KPiTag B0ToDstar0${cpstate}KstarPi
}

path append B0ToD0KPiPath TagB0ToD0KPiSequence


mod clone TagFilterByName B0ToD0KPiFilter
path append B0ToD0KPiPath B0ToD0KPiFilter
talkto B0ToD0KPiFilter {
    orList set B0ToD0barNonCPKPi
    orList set B0ToD0NonCPKPi
    orList set B0ToDstar0barNonCPKPi
    orList set B0ToDstar0NonCPKPi
    orList set B0ToD0barNonCPKstarPi
    orList set B0ToD0NonCPKstarPi
    orList set B0ToDstar0barNonCPKstarPi
    orList set B0ToDstar0NonCPKstarPi

    orList set B0ToD0CPEvenKPi
    orList set B0ToDstar0CPEvenKPi
    orList set B0ToD0CPEvenKstarPi
    orList set B0ToDstar0CPEvenKstarPi

    orList set B0ToD0CPOddKPi
    orList set B0ToDstar0CPOddKPi
    orList set B0ToD0CPOddKstarPi
    orList set B0ToDstar0CPOddKstarPi
}

# Add lists of composites to write
lappend BtaCandLists B0ToD0barNonCPKPi B0ToD0NonCPKPi B0ToDstar0barNonCPKPi B0ToDstar0NonCPKPi B0ToD0barNonCPKstarPi B0ToD0NonCPKstarPi B0ToDstar0barNonCPKstarPi B0ToDstar0NonCPKstarPi B0ToD0barCPEvenKPi B0ToD0CPEvenKPi B0ToDstar0barCPEvenKPi B0ToDstar0CPEvenKPi B0ToD0barCPEvenKstarPi B0ToD0CPEvenKstarPi B0ToDstar0barCPEvenKstarPi B0ToDstar0CPEvenKstarPi B0ToD0barCPOddKPi B0ToD0CPOddKPi B0ToDstar0barCPOddKPi B0ToDstar0CPOddKPi B0ToD0barCPOddKstarPi B0ToD0CPOddKstarPi B0ToDstar0barCPOddKstarPi B0ToDstar0CPOddKstarPi

# Add lists of composites with UsrData
set UsrBCandLists $BtaCandLists

lappend BtaCandLists D0List_Vtx_D0KPi Pi0ToGG_D0KPi pi0SoftLoose KsVtx_D0KPi KstarToKsPi_D0KPi
lappend BtaCandLists Rho0ToPiPi_D0KPi eta3piDefault EtaToGG_D0KPi EtaPToPiPiEta_D0KPi OmegaToPiPiPi0_D0KPi PhiToKK_D0KPi

# Write UsrData
set writeUsrDataForSkim 1
