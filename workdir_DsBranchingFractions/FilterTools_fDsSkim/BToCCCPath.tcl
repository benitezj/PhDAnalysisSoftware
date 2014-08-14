#
# $Id: BToCCCPath.tcl,v 1.6 2006/08/09 17:42:40 ddujmic Exp $
#
path create BToCCCPath

# setup the path
sourceFoundFile FilterTools/FilterToolsProcs.tcl
setupMulti3BodySkimPath TagBToPPP BToCCCPath

# add filter module
path append BToCCCPath FilterBToCCC
mod talk FilterBToCCC
   minNoOfTracks  set 1
   maxTotalEnergy set 20.0
#   verbose        set t
#
    tagbit set BToPPP__K+K+K+
    tagbit set BToPPP__pi+pi+pi+
    tagbit set BToPPP__pi+K+K+
    tagbit set BToPPP__pi+pi+K+
    tagbit set BToPPP__pi+p+p+
    tagbit set BToPPP__K+p+p+
exit
