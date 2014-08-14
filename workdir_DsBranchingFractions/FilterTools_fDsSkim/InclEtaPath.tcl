#
# $Id: InclEtaPath.tcl,v 1.2 2005/11/16 23:01:58 fwinkl Exp $
#
# defines InclEtaPath tag filter path
#

path create InclEtaPath

# only run on MultiHadrons
path append InclEtaPath FilterBGFMultiHadron

# append the tag modules we need from SkimTag2BodySequence
sourceFoundFile EventTagTools/TagProcs.tcl
addTagModule TagEtaTwoBody     InclEtaPath
addTagModule TagEtaPeppTwoBody InclEtaPath

# filter on tags
path append InclEtaPath InclEtaFilter
