#
# $Id: DsToPhiPiPath.tcl,v 1.3 2005/11/17 11:26:36 fwinkl Exp $
#
# defines DsToPhiPiPath for tag filter based on "aDsToPhiPi" tag bit
#


path create DsToPhiPiPath

# only run on MultiHadrons
path append DsToPhiPiPath FilterBGFMultiHadron

# make sure tag is created
sourceFoundFile EventTagTools/TagProcs.tcl
useTagFromList aDsToPhiPi DsToPhiPiPath

# add Ds filter. 
module clone TagFilterByName DsToPhiPiTagFilter
path append DsToPhiPiPath DsToPhiPiTagFilter
mod talk DsToPhiPiTagFilter
   andList set aDsToPhiPi
exit

