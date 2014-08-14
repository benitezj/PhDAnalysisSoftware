#
# $Id: InclK0sPath.tcl,v 1.2 2005/11/16 23:01:58 fwinkl Exp $
#
# defines InclK0sPath tag filter path
#

path create InclK0sPath

# only run on MultiHadrons
path append InclK0sPath FilterBGFMultiHadron

# append the tag modules we need from SkimTag2BodySequence
sourceFoundFile EventTagTools/TagProcs.tcl
addTagModule TagK_S0TwoBody InclK0sPath

# filter on tags
path append InclK0sPath InclK0sFilter
