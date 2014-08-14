#
# $Id: BToOmegaPiPihPath.tcl,v 1.1 2007/07/26 14:14:14 bloom Exp $
#
path create BToOmegaPiPihPath

# setup the path
sourceFoundFile FilterTools/FilterToolsProcs.tcl
setupMulti3BodySkimPath TagBToPPPP BToOmegaPiPihPath

# add filter module
path append BToOmegaPiPihPath FilterBToOmegaPiPih
mod talk FilterBToOmegaPiPih
   minNoOfTracks  set 1
   maxTotalEnergy set 20.0
#   verbose        set t
#
   tagbit set BToPPPP__omegapi+pi+pi0
   tagbit set BToPPPP__omegapi+pi+K_S0
   tagbit set BToPPPP__omegapi+pi+pi+
   tagbit set BToPPPP__omegapi+pi+K+
exit
