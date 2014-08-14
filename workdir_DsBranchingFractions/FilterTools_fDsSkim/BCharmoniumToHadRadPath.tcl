#
# $Id: BCharmoniumToHadRadPath.tcl,v 1.3 2005/06/17 13:49:31 fnc Exp $
#
#
# defines paths for charmonium to hadrons analysis
#

path create BCharmoniumToHadRadPath
# only run on MultiHadrons
path append BCharmoniumToHadRadPath FilterBGFMultiHadron

#insert the TagCharmoniumToHadProdSequence
sourceFoundFile EventTagTools/TagCharmoniumToHadProdSequence.tcl
path append BCharmoniumToHadRadPath TagCharmoniumToHadProdSequence

# and now filter
mod clone TagFilterByName BCharmoniumToHadRadFilter
path append BCharmoniumToHadRadPath BCharmoniumToHadRadFilter
talkto BCharmoniumToHadRadFilter { 
  andList set isBCMultiHadron
  orList set B0CharmoniumHadRad
  orList set BchCharmoniumHadRad
}
