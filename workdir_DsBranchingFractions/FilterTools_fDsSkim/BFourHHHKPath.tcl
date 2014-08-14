#
# $Id: BFourHHHKPath.tcl,v 1.3 2005/11/16 23:01:56 fwinkl Exp $
#
#
# defines BFourHHHKPath tag filter path
#
path create BFourHHHKPath
# only run on MultiHadrons
path append BFourHHHKPath FilterBGFMultiHadron

# append tag module
sourceFoundFile EventTagTools/TagProcs.tcl
addTagModule TagBFourBody BFourHHHKPath

mod clone TagFilterByName BFourHHHKFilter
path append BFourHHHKPath BFourHHHKFilter
talkto BFourHHHKFilter { 
    andList set B4bodyhhhk1
    andList set B4bodyhhhk2
}
