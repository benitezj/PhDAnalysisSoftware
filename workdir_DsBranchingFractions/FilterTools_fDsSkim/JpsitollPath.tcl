#
# $Id: JpsitollPath.tcl,v 1.8 2005/11/16 23:01:58 fwinkl Exp $
#
# defines JpsitoeePath tag+micro filter path
#

path create JpsitollPath

# only run on MultiHadrons : commented : recovery of nTRacks=2 Evts
#path append JpsitollPath FilterBGFMultiHadron

# we need those tag bits
sourceFoundFile EventTagTools/TagProcs.tcl

useTagFromList  JpsiELoose   JpsitollPath
useTagFromList  JpsiMuLoose  JpsitollPath
useTagFromList  Psi2SELoose  JpsitollPath
useTagFromList  Psi2SMuLoose JpsitollPath

#  tag filter
mod clone TagFilterByName JpsitollTagFilter
path append JpsitollPath JpsitollTagFilter
talkto JpsitollTagFilter {
	orList set JpsiELoose
	orList set Psi2SELoose
	orList set JpsiMuLoose
	orList set Psi2SMuLoose
}

