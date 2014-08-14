#
# $Id: BCCPi03bodyPath.tcl,v 1.3 2005/06/24 12:22:42 fnc Exp $
#

# define the proc first
sourceFoundFile FilterTools/FilterToolsProcs.tcl

# defines tag filter path
setup3BodySkimPath BCCPi03body {
  3bodytagbit set BCCPi03body
  minNoOfTracks set 3
}
