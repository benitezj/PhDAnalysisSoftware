#
# $Id: BFourHHHHPath.tcl,v 1.3 2005/11/16 23:01:56 fwinkl Exp $
#
#
# defines BFourHHHHPath tag filter path
#
path create BFourHHHHPath
# only run on MultiHadrons
path append BFourHHHHPath FilterBGFMultiHadron

# append tag module
sourceFoundFile EventTagTools/TagProcs.tcl
addTagModule TagBFourBody BFourHHHHPath

mod clone TagFilterByName BFourHHHHFilter
path append BFourHHHHPath BFourHHHHFilter
talkto BFourHHHHFilter { 
    andList set B4bodyhhhh1
    andList set B4bodyhhhh2
}
