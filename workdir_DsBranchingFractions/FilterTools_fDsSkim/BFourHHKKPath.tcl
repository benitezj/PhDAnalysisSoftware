#
# $Id: BFourHHKKPath.tcl,v 1.3 2005/11/16 23:01:56 fwinkl Exp $
#
#
# defines BFourHHKKPath tag filter path
#
path create BFourHHKKPath
# only run on MultiHadrons
path append BFourHHKKPath FilterBGFMultiHadron

# append tag module
sourceFoundFile EventTagTools/TagProcs.tcl
addTagModule TagBFourBody BFourHHKKPath

mod clone TagFilterByName BFourHHKKFilter
path append BFourHHKKPath BFourHHKKFilter
talkto BFourHHKKFilter { 
    andList set B4bodyhhkk1
    andList set B4bodyhhkk2
}
