#
# $Id: BKsKsPi03bodyPath.tcl,v 1.3 2005/06/24 12:22:44 fnc Exp $
#

# define the proc first
sourceFoundFile FilterTools/FilterToolsProcs.tcl

# defines tag filter path
setup3BodySkimPath BKsKsPi03body {
  3bodytagbit set BKsKsPi03body
  minNoOfTracks set 3
}
