#
# $Id: BchToD0KAllPath.tcl,v 1.18 2005/12/05 01:29:02 fwinkl Exp $
#
#
# defines BchToD0KAll filter path
#

global BtaCandLists
global writeUsrDataForSkim
global UsrBCandLists

sourceFoundFile EventTagTools/TagProcs.tcl

path create BchToD0KAllPath
# only run on MultiHadrons
path append BchToD0KAllPath FilterBGFMultiHadron

sequence create TagBchToD0KAllSequence

useTagFromList  BchToD0K TagBchToD0KAllSequence

makeTagFromList BchToD0CPEvenKDefault    BchToD0CPEvenK          TagBchToD0KAllSequence      ;# B- --> D0(CPEven) K-
makeTagFromList BchToD0CPOddKDefault     BchToD0CPOddK           TagBchToD0KAllSequence      ;# B- --> D0(CPOdd) K-
makeTagFromList BchToD0barKDefault       BchToD0barK             TagBchToD0KAllSequence      ;# B- --> D0bar K-
makeTagFromList BchToDstar0KLoose        BchToDstar0KLoose       TagBchToD0KAllSequence      ;# B- --> D*0 K-
makeTagFromList BchToDstar0CPEvenKLoose  BchToDstar0CPEvenKLoose TagBchToD0KAllSequence      ;# B- --> D*0(CPEven) K-
makeTagFromList BchToDstar0CPOddKLoose   BchToDstar0CPOddKLoose  TagBchToD0KAllSequence      ;# B- --> D*0(CPOdd) K-
makeTagFromList BchToDstar0barKLoose     BchToDstar0barKLoose    TagBchToD0KAllSequence      ;# B- --> D*0bar K-
makeTagFromList BchToD0DalitzKDefault    BchToD0DalitzK          TagBchToD0KAllSequence      ;# B- --> D0(Dalitz) K-
makeTagFromList BchToDstar0DalitzKLoose  BchToDstar0DalitzKLoose TagBchToD0KAllSequence      ;# B- --> D*0(Dalitz) K-

path append BchToD0KAllPath TagBchToD0KAllSequence

mod clone TagFilterByName TagBchToD0KAllFilter
talkto TagBchToD0KAllFilter {
    orList set BchToD0K
    orList set BchToD0CPEvenK
    orList set BchToD0CPOddK
    orList set BchToD0barK	
    orList set BchToDstar0KLoose 	
    orList set BchToDstar0CPEvenKLoose 	
    orList set BchToDstar0CPOddKLoose 	
    orList set BchToDstar0barKLoose 	
    orList set BchToD0DalitzK
    orList set BchToDstar0DalitzKLoose
}

path append  BchToD0KAllPath TagBchToD0KAllFilter

# Add lists of composites to write
lappend BtaCandLists BchToD0KDefault BchToD0CPEvenKDefault BchToD0CPOddKDefault BchToD0barKDefault BchToDstar0KLoose BchToDstar0CPEvenKLoose BchToDstar0CPOddKLoose BchToDstar0barKLoose BchToD0DalitzKDefault BchToDstar0DalitzKLoose

# Add lists of composites with UsrData
set UsrBCandLists $BtaCandLists
# Write UsrData
set writeUsrDataForSkim 1
