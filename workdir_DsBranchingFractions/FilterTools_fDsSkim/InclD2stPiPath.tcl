#
# $Id: InclD2stPiPath.tcl,v 1.5 2005/11/16 23:01:58 fwinkl Exp $
#
# defines DstarPiPath tag+micro filter path
#

path create InclD2stPiPath

# only run on MultiHadrons
path append InclD2stPiPath FilterBGFMultiHadron

# add module to create tag bit(s)
sourceFoundFile EventTagTools/TagProcs.tcl
addTagModule TagInclDstPi InclD2stPiPath

# my own tag filter
#path append InclD2stPiPath BPiD2starTagFilter

# don't need anything else, just add the filter
mod clone TagFilterByName InclD2stPiFilter
talkto InclD2stPiFilter {
    andList set incld2stpi
}
path append InclD2stPiPath InclD2stPiFilter
