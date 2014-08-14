#
# $Id: BFourHHHEPath.tcl,v 1.3 2005/11/16 23:01:55 fwinkl Exp $
#
#
# defines BFourHHHEPath tag filter path
#
path create BFourHHHEPath
# only run on MultiHadrons
path append BFourHHHEPath FilterBGFMultiHadron

# append tag module
sourceFoundFile EventTagTools/TagProcs.tcl
addTagModule TagBFourBody BFourHHHEPath

mod clone TagFilterByName BFourHHHEFilter
path append BFourHHHEPath BFourHHHEFilter
talkto BFourHHHEFilter { 
    andList set B4bodyhhhe1
    andList set B4bodyhhhe2
}
