#
# $Id: BKsKsC3bodyPath.tcl,v 1.4 2005/06/24 12:22:43 fnc Exp $
#

# define the proc first
sourceFoundFile FilterTools/FilterToolsProcs.tcl

# defines tag filter path
setup3BodySkimPath BKsKsC3body {
  3bodytagbit set BKsKsC3body
  minNoOfTracks set 4
}
