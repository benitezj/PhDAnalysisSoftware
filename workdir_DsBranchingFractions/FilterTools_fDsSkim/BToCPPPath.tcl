#
# $Id: BToCPPPath.tcl,v 1.5 2005/11/16 23:01:56 fwinkl Exp $
#
path create BToCPPPath

# setup the path
sourceFoundFile FilterTools/FilterToolsProcs.tcl
setupMulti3BodySkimPath TagBToPPP BToCPPPath

# add filter module
path append BToCPPPath FilterBToCPP
mod talk FilterBToCPP
   minNoOfTracks  set 1
   maxTotalEnergy set 20.0
#   verbose        set t
#
   tagbit set BToPPP__K_S0pi0pi+
   tagbit set BToPPP__K_S0pi0K+
   tagbit set BToPPP__K_S0pi+pi+
   tagbit set BToPPP__K_S0K_S0pi+
   tagbit set BToPPP__K_S0pi+K+
   tagbit set BToPPP__K_S0K+K+
   tagbit set BToPPP__K_S0K_S0K+
   tagbit set BToPPP__K_S0p+p+
   tagbit set BToPPP__pi0pi+pi+
   tagbit set BToPPP__pi0pi0pi+
   tagbit set BToPPP__pi0pi+K+
   tagbit set BToPPP__pi0K+K+
   tagbit set BToPPP__pi0pi0K+
   tagbit set BToPPP__pi0p+p+
exit
