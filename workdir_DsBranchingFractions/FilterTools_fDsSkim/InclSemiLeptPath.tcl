#
# $Id: InclSemiLeptPath.tcl,v 1.7 2007/02/23 16:51:37 marchior Exp $
#

path create InclSemiLeptPath

# only run on MultiHadrons
path append InclSemiLeptPath FilterBGFMultiHadron

# add module to create tag bit(s)
sourceFoundFile EventTagTools/TagProcs.tcl
addTagModule TagInclSemiLept InclSemiLeptPath {
  makeTags set true
  OptVtxAn set true
  Loose_YBS set true
  mm2cut set -10.0
  LPiVtxProb set 0.01
  OptSelVar set 0.3
#  eTightMap set PidLHElectrons
  eTightMap set eMicroLoose
#  eTightMap set eMicroTight
#  show
}

#add inclusive semileptonic filter
module clone TagFilterByName InclSemiLeptTagFilter
path append InclSemiLeptPath InclSemiLeptTagFilter
talkto InclSemiLeptTagFilter {

  # TagInclSemiLept doesn't set the bit if there are no candidates on the list:
  # assertIfMissing set false
  # not true anymore..
  assertIfMissing set true

  orList set InclSemiLeptGoodCharge
  orList set InclSemiLeptWrongCharge
}
