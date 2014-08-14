#
# $Id: InclD2stRhoPath.tcl,v 1.3 2005/11/16 23:01:58 fwinkl Exp $
#
# defines InclD2stRhoPath tag filter path
#

path create InclD2stRhoPath

# only run on MultiHadrons
path append InclD2stRhoPath FilterBGFMultiHadron

# add module to create tag bit(s)
sourceFoundFile EventTagTools/TagProcs.tcl
addTagModule TagInclDstRho InclD2stRhoPath

mod clone TagFilterByName InclD2stRhoFilter
path append InclD2stRhoPath InclD2stRhoFilter
talkto InclD2stRhoFilter {andList set incld2strho}
