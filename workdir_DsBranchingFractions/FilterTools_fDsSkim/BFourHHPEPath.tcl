#
# $Id: BFourHHPEPath.tcl,v 1.3 2005/11/16 23:01:56 fwinkl Exp $
#
#
# defines BFourHHPEPath tag filter path
#
path create BFourHHPEPath
# only run on MultiHadrons
path append BFourHHPEPath FilterBGFMultiHadron

# append tag module
sourceFoundFile EventTagTools/TagProcs.tcl
addTagModule TagBFourBody BFourHHPEPath

mod clone TagFilterByName BFourHHPEFilter
path append BFourHHPEPath BFourHHPEFilter
talkto BFourHHPEFilter { 
    andList set B4bodyhhpe1
    andList set B4bodyhhpe2
}
