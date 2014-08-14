#
# $Id: InclDstA1cPath.tcl,v 1.4 2005/11/16 23:01:58 fwinkl Exp $
#
# defines InclDstA1cPath tag filter path
#

path create InclDstA1cPath

# only run on MultiHadrons
path append InclDstA1cPath FilterBGFMultiHadron

module clone TagFilterByValue TagInclDstA1cNtracks
path append InclDstA1cPath TagInclDstA1cNtracks
talkto TagInclDstA1cNtracks {
  TagValueName set nTracks
  TagValueType set Integer
  TagValueMinimum set 4
  TagValueMaximum set 50
  assertIfMissing set true
}

module clone TagFilterByValue TagInclDstA1cR2
path append InclDstA1cPath TagInclDstA1cR2
talkto TagInclDstA1cR2 {
  TagValueName set R2
  TagValueType set Floating
  TagValueMinimum set -1.
  TagValueMaximum set 0.35
  assertIfMissing set true
}

# add module to create tag bit(s)
sourceFoundFile EventTagTools/TagProcs.tcl
###old addTagModule TagInclDstA1c InclDstA1cPath

#module clone TagInclDstarFilter TagInclDstarA1C
addTagModule TagInclDstarA1C InclDstA1cPath {
  mainDecay set "B0 -> D*+ a_1-"
  partialDecay set "D*+ -> D0 pi+"
  fastList set a1CHardTight
  slowList set GoodTracksVeryLoose
  tagPrefix set "InclDstarA1c"
  pfastMin  set 1.85
  pfastMax  set 2.30
  filter set false
  makeTags set true
  filterMode set 2
}

module clone TagFilterByName InclDstA1cFilter
path append InclDstA1cPath InclDstA1cFilter
###old talkto InclDstA1cFilter {andList set incldsta1c}
talkto InclDstA1cFilter {
  orList set InclDstarA1cMass
  orList set InclDstarA1cMassFlip
}
