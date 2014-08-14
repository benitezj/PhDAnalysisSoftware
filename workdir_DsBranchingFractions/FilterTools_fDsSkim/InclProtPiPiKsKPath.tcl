# $Id: InclProtPiPiKsKPath.tcl,v 1.2 2005/08/03 16:04:58 fnc Exp $

path create InclProtPiPiKsKPath

path append InclProtPiPiKsKPath FilterBGFMultiHadron

sourceFoundFile FilterTools/InclProtPiPiKsKSequence.tcl
path append InclProtPiPiKsKPath InclProtPiPiKsKSequence

mod clone TagFilterByValue TagNTracks
path append InclProtPiPiKsKPath TagNTracks
mod talk TagNTracks
  TagValueName set nTracks
  TagValueType set Integer
  TagValueMinimum set 6
  TagValueMaximum set 100
  assertIfMissing set true
  production set true
  verbose set false
exit

mod clone TagFromList TagInclProtPiPiKsK
path append InclProtPiPiKsKPath TagInclProtPiPiKsK

talkto TagInclProtPiPiKsK {
  listsToTag set InclProtPiPiKsK
  tagName set InclProtPiPiKsK_tag
  production set true
  verbose set false
}

mod clone TagFromList TagInclProtPiPiKsKTight
path append InclProtPiPiKsKPath TagInclProtPiPiKsKTight

talkto TagInclProtPiPiKsKTight {
  listsToTag set InclProtPiPiKsKTight
  tagName set InclProtPiPiKsKTight_tag
  production set true
  verbose set false
}

mod clone TagFromList TagInclProtPiKstarK
path append InclProtPiPiKsKPath TagInclProtPiKstarK

talkto TagInclProtPiKstarK {
  listsToTag set InclProtPiKstarK
  tagName set InclProtPiKstarK_tag
  production set true
  verbose set false
}

mod clone TagFromList TagInclProtPiKstarKTight
path append InclProtPiPiKsKPath TagInclProtPiKstarKTight
talkto TagInclProtPiKstarKTight {
  listsToTag set InclProtPiKstarKTight
  tagName set InclProtPiKstarKTight_tag
  production set true
  verbose set false
}

mod clone TagFilterByName InclProtPiPiKsKFilter
path append InclProtPiPiKsKPath InclProtPiPiKsKFilter

talkto InclProtPiPiKsKFilter {
  orList set InclProtPiPiKsK_tag
  orList set InclProtPiKstarK_tag
  production set true
  verbose set false
}
