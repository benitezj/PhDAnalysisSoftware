#
# $Id: BCCKs3bodyPath.tcl,v 1.4 2005/06/24 12:22:41 fnc Exp $
#

# define the proc first
sourceFoundFile FilterTools/FilterToolsProcs.tcl

# defines tag filter path
setup3BodySkimPath BCCKs3body {
  3bodytagbit set BCCKs3body
  minNoOfTracks set 4
}
