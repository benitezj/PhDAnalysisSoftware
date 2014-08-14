#
# $Id: BCCKsPi0Pi03bodyPath.tcl,v 1.2 2005/06/24 12:22:42 fnc Exp $
#

# define the proc first
sourceFoundFile FilterTools/FilterToolsProcs.tcl

# defines tag filter path
setup3BodySkimPath BCCKsPi0Pi03body {
  3bodytagbit set BCCKsPi0Pi03body
  minNoOfTracks set 2
}
