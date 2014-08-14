#
# $Id: BCountMuMuFilterPath.tcl,v 1.4 2005/03/14 17:28:02 fnc Exp $
#
#
#
# defines BCountMuMuPath tag filter path
#

mod clone TagFilterByName FilterBGFMuMu
mod talk FilterBGFMuMu
    andList set BGFMuMu
exit
mod clone TagFilterByName BCountMuMuFilter
mod talk BCountMuMuFilter
    andList set isBCMuMu
exit

path create BCountMuMuFilterPath

# only run on BGFMuMu events
path append BCountMuMuFilterPath FilterBGFMuMu 


# don't need anything else, just add the filter
path append BCountMuMuFilterPath BCountMuMuFilter
