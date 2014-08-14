#
# $Id: InclDstRhoPath.tcl,v 1.4 2005/11/16 23:01:58 fwinkl Exp $
#
# defines InclDstRhoPath tag filter path
#

path create InclDstRhoPath

# only run on MultiHadrons
path append InclDstRhoPath FilterBGFMultiHadron

# add module to create tag bit(s)
sourceFoundFile EventTagTools/TagProcs.tcl
addTagModule TagInclDstRho InclDstRhoPath

mod clone TagFilterByName InclDstRhoFilter
path append InclDstRhoPath InclDstRhoFilter
talkto InclDstRhoFilter {andList set incldstrho}
