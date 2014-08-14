
  path create KsKsKInclPath

#
#  Owen Long, UCR
#
#    $Id: KsKsKInclPath.tcl,v 1.2 2006/02/03 18:40:30 fwinkl Exp $
#

  # add tag module
  sourceFoundFile EventTagTools/TagProcs.tcl
  sourceFoundFile FilterTools/SkimTag3BodySequence.tcl
  addTagModule TagKsKsKIncl KsKsKInclPath

  # filter on tag
  mod clone TagFilterByName KsKsKInclFilter
  path append KsKsKInclPath  KsKsKInclFilter
  talkto  KsKsKInclFilter  {
      andList set KsKsKIncl
  }


