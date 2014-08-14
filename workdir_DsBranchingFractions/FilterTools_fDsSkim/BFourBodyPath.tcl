#
# $Id: BFourBodyPath.tcl,v 1.3 2005/11/16 23:01:55 fwinkl Exp $
#
#
# defines BFourBodyPath tag filter path
#

path create BFourBodyPath

# only run on MultiHadrons
path append BFourBodyPath FilterBGFMultiHadron

# append tag module
sourceFoundFile EventTagTools/TagProcs.tcl
addTagModule TagBFourBody BFourBodyPath

path append BFourBodyPath TagBFourBodyFilter
