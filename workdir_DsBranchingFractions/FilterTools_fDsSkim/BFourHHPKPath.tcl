#
# $Id: BFourHHPKPath.tcl,v 1.3 2005/11/16 23:01:56 fwinkl Exp $
#
#
# defines BFourHHPKPath tag filter path
#
path create BFourHHPKPath
# only run on MultiHadrons
path append BFourHHPKPath FilterBGFMultiHadron

# append tag module
sourceFoundFile EventTagTools/TagProcs.tcl
addTagModule TagBFourBody BFourHHPKPath

mod clone TagFilterByName BFourHHPKFilter
path append BFourHHPKPath BFourHHPKFilter
talkto BFourHHPKFilter { 
    andList set B4bodyhhpk1
    andList set B4bodyhhpk2
}
