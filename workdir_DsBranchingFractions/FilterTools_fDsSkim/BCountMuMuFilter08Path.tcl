#
# $Id: BCountMuMuFilter08Path.tcl,v 1.1 2008/05/02 21:23:34 gmcgreg Exp $
#
#
#
# defines BCountMuMuFilter08Path tag filter path
# 2008 revised BCounting skim
# Grant McGregor, 2008



mod clone TagFilterByName FilterBGFMuMu
mod talk FilterBGFMuMu
    andList set BGFMuMu
exit
mod clone TagFilterByName BCountMuMuFilter08
mod talk BCountMuMuFilter08
    andList set isBCMuMu08
exit

path create BCountMuMuFilter08Path

# only run on BGFMuMu events
path append BCountMuMuFilter08Path FilterBGFMuMu 


# don't need anything else, just add the filter
path append BCountMuMuFilter08Path BCountMuMuFilter08
