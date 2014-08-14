#
# $Id: JpsitollTightPath.tcl,v 1.4 2005/11/16 23:01:58 fwinkl Exp $
#
# a tighter version of the Jpsitoll skim
#

path create JpsitollTightPath


#path append JpsitollTightPath FilterBGFMultiHadron
#     BGFMultiHadron replaced by :

path append JpsitollTightPath JpsitollFilter
talkto JpsitollFilter { 
  R2max set 0.5 
}

# (recovery of nTRacks=2 Evts

# create tag bits
sourceFoundFile EventTagTools/TagProcs.tcl
makeTagFromList JPsiLHEE             JpsiELH     JpsitollTightPath 
makeTagFromList JPsiNNVeryLooseMuMu  JpsiMuNNVL  JpsitollTightPath
makeTagFromList Psi2SLHEE            Psi2SELH    JpsitollTightPath
makeTagFromList Psi2SNNVeryLooseMuMu Psi2SMuNNVL JpsitollTightPath


# tag filter
mod clone TagFilterByName JpsitollTightFilter
path append JpsitollTightPath JpsitollTightFilter
talkto JpsitollTightFilter { 
  orList set JpsiELH
  orList set Psi2SELH
  orList set JpsiMuNNVL
  orList set Psi2SMuNNVL
}


