#
# $Id: BchToD0KstarAllPath.tcl,v 1.17 2005/12/05 01:29:02 fwinkl Exp $
#
#
# defines BchToD0KAlltag filter path
#

global BtaCandLists
global writeUsrDataForSkim
global UsrBCandLists

sourceFoundFile EventTagTools/TagProcs.tcl

path create BchToD0KstarAllPath
# only run on MultiHadrons
path append BchToD0KstarAllPath FilterBGFMultiHadron

# Create tags
sequence create TagBchToD0KstarAll

makeTagFromList BchToD0KstarLoose            BchToD0KstarLoose            TagBchToD0KstarAll   ;# B- --> D0 K*-
makeTagFromList BchToD0CPEvenKstarLoose      BchToD0CPEvenKstarLoose      TagBchToD0KstarAll   ;# B- --> D0(CPEven) K*-
makeTagFromList BchToD0CPOddKstarLoose       BchToD0CPOddKstarLoose       TagBchToD0KstarAll   ;# B- --> D0(CPOdd) K*-
makeTagFromList BchToD0barKstarLoose         BchToD0barKstarLoose         TagBchToD0KstarAll   ;# B- --> D0bar K*-
makeTagFromList BchToDstar0KstarLoose        BchToDstar0KstarLoose        TagBchToD0KstarAll   ;# B- --> D*0 K*-
makeTagFromList BchToDstar0CPEvenKstarLoose  BchToDstar0CPEvenKstarLoose  TagBchToD0KstarAll   ;# B- --> D*0(CPEven) K*-
makeTagFromList BchToDstar0CPOddKstarLoose   BchToDstar0CPOddKstarLoose   TagBchToD0KstarAll   ;# B- --> D*0(CPOdd) K*-
makeTagFromList BchToDstar0barKstarLoose     BchToDstar0barKstarLoose     TagBchToD0KstarAll   ;# B- --> D*0bar K*-
makeTagFromList BchToD0DalitzKstarLoose      BchToD0DalitzKstarLoose      TagBchToD0KstarAll   ;# B- --> D0(Dalitz) K*-
makeTagFromList BchToDstar0DalitzKstarLoose  BchToDstar0DalitzKstarLoose  TagBchToD0KstarAll   ;# B- --> D*0(Dalitz) K*-

path append BchToD0KstarAllPath TagBchToD0KstarAll

# Filter on tags
mod clone TagFilterByName TagBchToD0KstarAllFilter
talkto TagBchToD0KstarAllFilter {
    orList set BchToD0KstarLoose 	
    orList set BchToD0CPEvenKstarLoose 	
    orList set BchToD0CPOddKstarLoose 	
    orList set BchToD0barKstarLoose 	
    orList set BchToDstar0KstarLoose 	
    orList set BchToDstar0CPEvenKstarLoose 	
    orList set BchToDstar0CPOddKstarLoose 	
    orList set BchToDstar0barKstarLoose 
    orList set BchToD0DalitzKstarLoose
    orList set BchToDstar0DalitzKstarLoose
}

path append  BchToD0KstarAllPath TagBchToD0KstarAllFilter

# Add lists of composites to write
lappend BtaCandLists BchToD0KstarLoose BchToD0CPEvenKstarLoose BchToD0CPOddKstarLoose BchToD0barKstarLoose BchToDstar0KstarLoose BchToDstar0CPEvenKstarLoose BchToDstar0CPOddKstarLoose BchToDstar0barKstarLoose BchToD0DalitzKstarLoose BchToDstar0DalitzKstarLoose

# Add lists of composites with UsrData
set UsrBCandLists $BtaCandLists
# Write UsrData
set writeUsrDataForSkim 1
