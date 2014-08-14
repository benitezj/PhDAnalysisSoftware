#
# $Id: BPi0Pi0Pi03bodyPath.tcl,v 1.3 2005/06/24 12:22:46 fnc Exp $
#

# define the proc first
sourceFoundFile FilterTools/FilterToolsProcs.tcl

# defines tag filter path
setup3BodySkimPath BPi0Pi0Pi03body {
  3bodytagbit set BPi0Pi0Pi03body
  minNoOfTracks set 1
}
