#
# $Id: BchDKNeutPath.tcl,v 1.15 2005/11/16 23:01:57 fwinkl Exp $
#
#
# defines BchDKNeut tag filter path
#

global BtaCandLists
global writeUsrDataForSkim
global UsrBCandLists

sourceFoundFile EventTagTools/TagProcs.tcl

path create BchDKNeutPath
# only run on MultiHadrons
path append BchDKNeutPath FilterBGFMultiHadron

# create tag bits from list
sequence create TagBchDKNeutSequence

makeTagFromList BchToDcKsDefault           BchToDcKs       TagBchDKNeutSequence     ;# B->DKs
makeTagFromList BchToDstarKsDefault        BchToDstarKs    TagBchDKNeutSequence     ;# B->D*Ks
makeTagFromList BchToDcKstarDefault        BchToDcKstar    TagBchDKNeutSequence     ;# B->DK*0
makeTagFromList BchToDstarKstarDefault     BchToDstarKstar TagBchDKNeutSequence     ;# B->D*K*0

path append BchDKNeutPath TagBchDKNeutSequence

# filter on tag bits
mod clone TagFilterByName TagBchDKNeut
talkto TagBchDKNeut {
    orList set BchToDcKs
    orList set BchToDstarKs
    orList set BchToDcKstar
    orList set BchToDstarKstar
}

path append  BchDKNeutPath TagBchDKNeut

# Add lists of composites to write
lappend BtaCandLists BchToDcKsDefault BchToDstarKsDefault BchToDcKstarDefault BchToDstarKstarDefault 

# Add lists of composites with UsrData
set UsrBCandLists $BtaCandLists
# Write UsrData
set writeUsrDataForSkim 1
