#
# $Id: BToDPPbarXPath.tcl,v 1.4 2005/08/03 16:04:32 fnc Exp $
#
#
# BToDPPbarX.tcl, created by mazur
#

path create BToDPPbarXPath
path append BToDPPbarXPath FilterBGFMultiHadron

mod clone TagFromList TagBToDPPbarXAllD
path append BToDPPbarXPath TagBToDPPbarXAllD
mod talk TagBToDPPbarXAllD
  listsToTag set BToD0PPbarX
  listsToTag set BToDcPPbarX
  tagName set TagBToDPPbarXAllB
exit

mod clone TagFilterByName BToDPPbarXFilter
path append BToDPPbarXPath BToDPPbarXFilter
mod talk BToDPPbarXFilter
  andList set TagBToDPPbarXAllB
  assertIfMissing set t
exit
