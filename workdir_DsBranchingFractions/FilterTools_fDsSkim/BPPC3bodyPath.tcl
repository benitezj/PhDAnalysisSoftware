#
# $Id: BPPC3bodyPath.tcl,v 1.2 2005/06/24 12:22:44 fnc Exp $
#

# define the proc first
sourceFoundFile FilterTools/FilterToolsProcs.tcl

# defines tag filter path
setup3BodySkimPath BPPC3body {
  3bodytagbit set BPPC3body
  minNoOfTracks set 3
}
