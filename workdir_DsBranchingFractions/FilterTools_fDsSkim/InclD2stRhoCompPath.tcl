#
# $Id: InclD2stRhoCompPath.tcl,v 1.3 2005/11/16 23:01:58 fwinkl Exp $
#
# defines InclD2stRhoCompPath tag filter path
#

path create InclD2stRhoCompPath

# only run on MultiHadrons
path append InclD2stRhoCompPath FilterBGFMultiHadron

# add module to create tag bit(s)
sourceFoundFile EventTagTools/TagProcs.tcl
addTagModule TagInclDstRho InclD2stRhoCompPath

mod clone TagFilterByName InclD2stRhoCompFilter
path append InclD2stRhoCompPath InclD2stRhoCompFilter
talkto InclD2stRhoCompFilter {andList set incld2strhoComp}
