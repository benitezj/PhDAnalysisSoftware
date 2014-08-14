#
# $Id: IsrIncExcPath.tcl,v 1.1 2005/11/16 23:51:46 fwinkl Exp $
#
# defines IsrIncExc path
#

sourceFoundFile EventTagTools/TagProcs.tcl

path create IsrIncExcPath

# create tag bits
addTagModule TagIncExcIsr IsrIncExcPath {
  trackList   set "ChargedTracks"
  neutralList set "CalorNeutral"
  doInclusive set true
  doExclusive set true
  doInclusiveEVeto set true
  doInclusiveNeutralModes set true
  electronPrescale set 40;
}

# filter ont tag bits
mod clone TagFilterByName IsrIncExcFilter
addTagModule IsrIncExcFilter IsrIncExcPath {
  orList set InclusiveIsr
  orList set ExclusiveIsr
}

