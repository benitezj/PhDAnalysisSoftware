#
#  Define the BtoPhiKPhiTo3piFilter path
#

# (don't) Write UsrData
global writeUsrDataForSkim
set writeUsrDataForSkim 0

set pathName BtoPhiKPhiTo3piPath
set  modName BtoPhiKPhiTo3piFilter
set phiList   PhiTo3piB2body
set phiKsList BtoPhiKsPhiTo3pi
set phiKpList BtoPhiKpPhiTo3pi

# Add lists of composites to write
global  BtaCandLists
lappend BtaCandLists $phiList $phiKsList $phiKpList

path create $pathName
path append $pathName FilterBGFMultiHadron

#--------------------------------------

set phiTagMod ${phiList}TagFromList
set phiTagBit ${phiList}TagBit

mod clone TagFromList $phiTagMod
path append $pathName $phiTagMod
talkto $phiTagMod {
    listsToTag set $phiList
    tagName    set $phiTagBit
    production set true
    verbose set false
}

#--------------------------------------

set phiKsTagMod ${phiKsList}TagFromList
set phiKsTagBit ${phiKsList}TagBit

mod clone TagFromList $phiKsTagMod
path append $pathName $phiKsTagMod
talkto $phiKsTagMod {
    listsToTag set $phiKsList
    tagName    set $phiKsTagBit
    production set true
    verbose set false
}

#--------------------------------------

set phiKpTagMod ${phiKpList}TagFromList
set phiKpTagBit ${phiKpList}TagBit

mod clone TagFromList $phiKpTagMod
path append $pathName $phiKpTagMod
talkto $phiKpTagMod {
    listsToTag set $phiKpList
    tagName    set $phiKpTagBit
    production set true
    verbose set false
}

#--------------------------------------

mod clone TagFilterByName $modName
path append $pathName $modName
talkto $modName {
    orList set $phiKsTagBit
    orList set $phiKpTagBit
    production set true
    verbose set false
}
