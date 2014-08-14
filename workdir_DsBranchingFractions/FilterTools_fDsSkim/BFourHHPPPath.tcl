#
# $Id: BFourHHPPPath.tcl,v 1.3 2005/11/16 23:01:56 fwinkl Exp $
#
#
# defines BFourHHPPPath tag filter path
#
path create BFourHHPPPath
# only run on MultiHadrons
path append BFourHHPPPath FilterBGFMultiHadron

# append tag module
sourceFoundFile EventTagTools/TagProcs.tcl
addTagModule TagBFourBody BFourHHPPPath

mod clone TagFilterByName BFourHHPPFilter
path append BFourHHPPPath BFourHHPPFilter
talkto BFourHHPPFilter { 
    andList set B4bodyhhpp1
    andList set B4bodyhhpp2
}
