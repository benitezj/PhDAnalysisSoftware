#
# $Id: B0DKNeutPath.tcl,v 1.15 2005/11/16 23:01:54 fwinkl Exp $
#
#
# defines B0DKNeut tag filter path
#

global BtaCandLists
global writeUsrDataForSkim
global UsrBCandLists

sourceFoundFile EventTagTools/TagProcs.tcl

path create B0DKNeutPath
# only run on MultiHadrons
path append B0DKNeutPath FilterBGFMultiHadron

# create tag bits from lists
sequence create TagB0DKNeutSequence

makeTagFromList B0ToD0KsDefault            B0ToD0Ks            TagB0DKNeutSequence ;# B0->D0Ks
makeTagFromList B0ToDstar0KsDefault        B0ToDstar0Ks        TagB0DKNeutSequence ;# B0->D*0Ks
makeTagFromList B0ToD0KstarDefault         B0ToD0Kstar         TagB0DKNeutSequence ;# B0->D0K*0
makeTagFromList B0ToDstar0KstarDefault     B0ToDstar0Kstar     TagB0DKNeutSequence ;# B0->D*0K*0
makeTagFromList B0ToD0barKstarDefault      B0ToD0barKstar      TagB0DKNeutSequence ;# B0->D0barK*0
makeTagFromList B0ToDstar0barKstarDefault  B0ToDstar0barKstar  TagB0DKNeutSequence ;# B0->D*0barK*0
makeTagFromList B0ToDstarstar0barKsDefault B0ToDstarstar0barKs TagB0DKNeutSequence ;# B0->D**0barKs
makeTagFromList B0ToDKDefault              B0ToDK              TagB0DKNeutSequence ;# B0->DK
makeTagFromList B0ToD0CPKsDefault          B0ToD0CPKs          TagB0DKNeutSequence ;# B0->D0Ks, D0->CP
makeTagFromList B0ToD0CPKstar0Default      B0ToD0CPKstar0      TagB0DKNeutSequence ;# B0->D0K*0, D0->CP

path append B0DKNeutPath TagB0DKNeutSequence

# filter on tag bits
mod clone TagFilterByName TagB0DKNeutFilter
talkto TagB0DKNeutFilter {
    orList set B0ToD0Ks
    orList set B0ToDstar0Ks
    orList set B0ToD0Kstar
    orList set B0ToD0barKstar
    orList set B0ToDstar0Kstar
    orList set B0ToDstar0barKstar
    orList set B0ToDstarstar0barKs
    orList set B0ToDK
    orList set B0ToD0CPKs
    orList set B0ToD0CPKstar0
}

path append  B0DKNeutPath TagB0DKNeutFilter

# Add lists of composites to write
lappend BtaCandLists B0ToD0KsDefault B0ToDstar0KsDefault B0ToD0KstarDefault B0ToD0barKstarDefault B0ToDstar0KstarDefault B0ToDstar0barKstarDefault B0ToDstarstar0barKsDefault B0ToDKDefault B0ToD0CPKsDefault B0ToD0CPKstar0Default

# Add lists of composites with UsrData
set UsrBCandLists $BtaCandLists
# Write UsrData
set writeUsrDataForSkim 1
