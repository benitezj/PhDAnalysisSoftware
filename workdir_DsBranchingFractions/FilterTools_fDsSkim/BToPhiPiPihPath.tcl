#
# $Id: BToPhiPiPihPath.tcl,v 1.1 2007/07/26 14:14:14 bloom Exp $
#
path create BToPhiPiPihPath

# setup the path
sourceFoundFile FilterTools/FilterToolsProcs.tcl
setupMulti3BodySkimPath TagBToPPPP BToPhiPiPihPath

# add filter module
path append BToPhiPiPihPath FilterBToPhiPiPih
mod talk FilterBToPhiPiPih
   minNoOfTracks  set 1
   maxTotalEnergy set 20.0
#   verbose        set t
#
   tagbit set BToPPPP__phipi+pi+pi0
   tagbit set BToPPPP__phipi+pi+K_S0
   tagbit set BToPPPP__phipi+pi+pi+
   tagbit set BToPPPP__phipi+pi+K+
exit
