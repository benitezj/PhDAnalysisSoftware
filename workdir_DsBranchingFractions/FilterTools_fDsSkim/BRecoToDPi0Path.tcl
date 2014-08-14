#
# $Id: BRecoToDPi0Path.tcl,v 1.2 2005/11/16 23:01:56 fwinkl Exp $
#
# defines BRecoToDPi0Path tag filter path
#
#  A.Volk
#


global BtaCandLists
global writeUsrDataForSkim
global UsrBCandLists

path create    BRecoToDPi0Path

path append    BRecoToDPi0Path    FilterBGFMultiHadron

# sourceFoundFile CompositionSequences/CompBRecoSequence.tcl
# path append    BRecoToDPi0Path    BchToDPi0_Default
# module enable BchToDPi0_Default

# create tag bits
sequence create TagBRecoToDPi0Sequence

useTagFromList BchToDsPi0 TagBRecoToDPi0Sequence
makeTagFromList BchToDPi0Default   BchToDPi0     TagBRecoToDPi0Sequence      ;# B->DPi0

path append BRecoToDPi0Path TagBRecoToDPi0Sequence

# filter on tag bits
mod clone TagFilterByName    BRecoToDPi0Filter
path append    BRecoToDPi0Path    BRecoToDPi0Filter
talkto    BRecoToDPi0Filter {
    orList set BchToDsPi0
    orList set BchToDPi0
    
    assertIfMissing set true
}



lappend BtaCandLists BchToDsPi0Default BchToDPi0Default

set UsrBCandLists $BtaCandLists
set writeUsrDataForSkim 1





