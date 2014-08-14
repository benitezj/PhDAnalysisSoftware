#
# $Id: BFourHHKEPath.tcl,v 1.3 2005/11/16 23:01:56 fwinkl Exp $
#
#
# defines BFourHHKEPath tag filter path
#
path create BFourHHKEPath
# only run on MultiHadrons
path append BFourHHKEPath FilterBGFMultiHadron

# append tag module
sourceFoundFile EventTagTools/TagProcs.tcl
addTagModule TagBFourBody BFourHHKEPath

mod clone TagFilterByName BFourHHKEFilter
path append BFourHHKEPath BFourHHKEFilter
talkto BFourHHKEFilter { 
    andList set B4bodyhhke1
    andList set B4bodyhhke2
}
