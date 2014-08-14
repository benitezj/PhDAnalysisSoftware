#
# $Id: BFourHHHPPath.tcl,v 1.3 2005/11/16 23:01:56 fwinkl Exp $
#
#
# defines BFourHHHPPath tag filter path
#
path create BFourHHHPPath
# only run on MultiHadrons
path append BFourHHHPPath FilterBGFMultiHadron

# append tag module
sourceFoundFile EventTagTools/TagProcs.tcl
addTagModule TagBFourBody BFourHHHPPath

mod clone TagFilterByName BFourHHHPFilter
path append BFourHHHPPath BFourHHHPFilter
talkto BFourHHHPFilter { 
    andList set B4bodyhhhp1
    andList set B4bodyhhhp2
}
