#
# $Id: InclDstPiPath.tcl,v 1.4 2005/11/16 23:01:58 fwinkl Exp $
#
# defines DstarPiPath tag+micro filter path
#

path create InclDstPiPath

# only run on MultiHadrons
path append InclDstPiPath FilterBGFMultiHadron

# add module to create tag bit(s)
sourceFoundFile EventTagTools/TagProcs.tcl
addTagModule TagInclDstPiMrec InclDstPiPath

# my own tag filter
path append InclDstPiPath BPiDstarTagFilter

# don't need anything else, just add the filter
mod clone TagFilterByName InclDstPiFilter
talkto InclDstPiFilter {
    andList set incldstpimrec
}
path append InclDstPiPath InclDstPiFilter
