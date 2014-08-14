#
# $Id: DileptonPath.tcl,v 1.5 2005/11/16 23:01:57 fwinkl Exp $
#
# defines Dilepton tag+micro filter path
#

path create DileptonPath

# create tag bit
sourceFoundFile EventTagTools/TagProcs.tcl
addTagModule TagClassifyDilepton DileptonPath

# filter out Dilepton
mod clone TagFilterByName FilterOnDileptonTag
mod talk FilterOnDileptonTag
    andList set Dilepton
exit

path append DileptonPath FilterOnDileptonTag
