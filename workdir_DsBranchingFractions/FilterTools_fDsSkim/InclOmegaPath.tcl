#
# $Id: InclOmegaPath.tcl,v 1.2 2005/11/16 23:01:58 fwinkl Exp $
#
# defines InclOmegaPath tag filter path
#

path create InclOmegaPath

# only run on MultiHadrons
path append InclOmegaPath FilterBGFMultiHadron

# append the tag modules we need from SkimTag2BodySequence
sourceFoundFile EventTagTools/TagProcs.tcl
addTagModule TagOmegaTwoBody  InclOmegaPath

path append InclOmegaPath InclOmegaFilter
