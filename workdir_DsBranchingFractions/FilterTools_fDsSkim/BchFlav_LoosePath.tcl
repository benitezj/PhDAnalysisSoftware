#
# $Id: BchFlav_LoosePath.tcl,v 1.7 2007/02/23 16:51:36 marchior Exp $
#
# defines BFlav_LoosePath tag filter path
#
#  O.Long, UCSB
#

global BCHFLAV_LOOSEPATH
if [ expr ! [ info exists BCHFLAV_LOOSEPATH ] ] {
  set BCHFLAV_LOOSEPATH 1

  path create BchFlav_LoosePath

  path append BchFlav_LoosePath FilterBGFMultiHadron

  # Append and enable the tag module
  sourceFoundFile FilterTools/SkimTagBrecoSequence.tcl
  useTagFromList  BchToDstar0Pi  BchFlav_LoosePath
  useTagFromList  BchToD0Pi      BchFlav_LoosePath

  mod clone TagFromTag BchFlav_Loose_Tag
  path append BchFlav_LoosePath BchFlav_Loose_Tag
  talkto BchFlav_Loose_Tag {
      tagName set BchFlav_Loose
      orList set BchToDstar0Pi
      orList set BchToD0Pi
      production set true
      verbose set false
  }

  mod clone TagFilterByName BchFlav_LooseFilter
  path append BchFlav_LoosePath BchFlav_LooseFilter
  talkto BchFlav_LooseFilter {
      orList set BchFlav_Loose
      production set true
      verbose set false
  }
}
