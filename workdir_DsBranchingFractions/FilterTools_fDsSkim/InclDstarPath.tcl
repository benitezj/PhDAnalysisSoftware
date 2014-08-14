#
# $Id: InclDstarPath.tcl,v 1.3 2005/11/16 23:01:58 fwinkl Exp $
#
# defines InclDstar tag+micro filter path
#

path create InclDstarPath

# only run on MultiHadrons
path append InclDstarPath FilterBGFMultiHadron

# add modules to create tag bit(s)
sourceFoundFile EventTagTools/TagProcs.tcl
addTagModule TagInclDstarDstar InclDstarPath {
  fastList set DstarToD0PiMainHardDefault
  slowList set GoodTracksVeryLoose
  filter set false
  makeTags set true
  filterMode set 2
}

addTagModule TagInclDsDstar InclDstarPath {
  mainDecay set "B0 -> D_s*+ D*-"
  partialDecay set "D_s*+ -> D_s+ gamma"
  fastList set DstarToD0PiMainHardDefault
  slowList set GoodPhotonLoose
  tagPrefix set "InclDstarDs"
  pfastMin  set 1.4
  pfastMax  set 2.0
  filter set false
  makeTags set true
  filterMode set 2
}

# add D* filter. 
module clone TagFilterByName InclDstarTagFilter
path append InclDstarPath InclDstarTagFilter
talkto InclDstarTagFilter {
  orList set InclDstarDstarMass
  orList set InclDstarDstarMassFlip
  orList set InclDstarDsMass
  orList set InclDstarDsMassFlip
}
