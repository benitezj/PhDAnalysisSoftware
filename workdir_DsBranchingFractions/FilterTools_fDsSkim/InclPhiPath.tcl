#
# $Id: InclPhiPath.tcl,v 1.2 2005/11/16 23:01:58 fwinkl Exp $
#
# defines InclPhiPath tag filter path
#

path create InclPhiPath

# only run on MultiHadrons
path append InclPhiPath FilterBGFMultiHadron

# append the tag modules we need from SkimTag2BodySequence
sourceFoundFile EventTagTools/TagProcs.tcl
addTagModule TagPhiTwoBody  InclPhiPath

path append InclPhiPath InclPhiFilter
