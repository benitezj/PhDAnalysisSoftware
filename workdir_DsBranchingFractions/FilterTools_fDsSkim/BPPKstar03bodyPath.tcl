#
# $Id: BPPKstar03bodyPath.tcl,v 1.2 2005/06/24 12:22:45 fnc Exp $
#

# define the proc first
sourceFoundFile FilterTools/FilterToolsProcs.tcl

# defines tag filter path
setup3BodySkimPath BPPKstar03body {
  3bodytagbit set BPPKstar03body
  minNoOfTracks set 4
}
