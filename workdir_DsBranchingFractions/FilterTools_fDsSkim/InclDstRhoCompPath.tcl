#
# $Id: InclDstRhoCompPath.tcl,v 1.3 2005/11/16 23:01:58 fwinkl Exp $
#
# defines InclDstRhoCompPath tag filter path
#

path create InclDstRhoCompPath

# only run on MultiHadrons
path append InclDstRhoCompPath FilterBGFMultiHadron

# add module to create tag bit(s)
sourceFoundFile EventTagTools/TagProcs.tcl
addTagModule TagInclDstRho InclDstRhoCompPath

mod clone TagFilterByName InclDstRhoCompFilter
path append InclDstRhoCompPath InclDstRhoCompFilter
talkto InclDstRhoCompFilter {andList set incldstrhoComp}
