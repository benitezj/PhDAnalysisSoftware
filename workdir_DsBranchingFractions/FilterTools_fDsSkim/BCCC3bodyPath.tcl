#
# $Id: BCCC3bodyPath.tcl,v 1.3 2005/06/24 12:22:41 fnc Exp $
#

# define the proc first
sourceFoundFile FilterTools/FilterToolsProcs.tcl

# defines tag filter path
setup3BodySkimPath BCCC3body {
  3bodytagbit set BCCC3body
  minNoOfTracks set 4
}
