#
# $Id: BCharmoniumToHadPath.tcl,v 1.3 2005/06/17 13:49:31 fnc Exp $
#
#
# defines paths for charmonium to hadrons analysis
#

path create BCharmoniumToHadPath
# only run on MultiHadrons
path append BCharmoniumToHadPath FilterBGFMultiHadron

#insert the TagCharmoniumToHadProdSequence
sourceFoundFile EventTagTools/TagCharmoniumToHadProdSequence.tcl
path append BCharmoniumToHadPath TagCharmoniumToHadProdSequence

# and now filter
mod clone TagFilterByName BCharmoniumToHadFilter
path append BCharmoniumToHadPath BCharmoniumToHadFilter
talkto BCharmoniumToHadFilter { 
  andList set isBCMultiHadron
  orList set B0CharmoniumHad
  orList set BchCharmoniumHad
}
