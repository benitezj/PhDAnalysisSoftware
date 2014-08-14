#
# $Id: BToEtaPiPihPath.tcl,v 1.1 2007/07/26 14:14:14 bloom Exp $
#
path create BToEtaPiPihPath

# setup the path
sourceFoundFile FilterTools/FilterToolsProcs.tcl
setupMulti3BodySkimPath TagBToPPPP BToEtaPiPihPath

# add filter module
path append BToEtaPiPihPath FilterBToEtaPiPih
mod talk FilterBToEtaPiPih
   minNoOfTracks  set 1
   maxTotalEnergy set 20.0
#   verbose        set t
#
   tagbit set BToPPPP__etapi+pi+pi0
   tagbit set BToPPPP__etapi+pi+K_S0
   tagbit set BToPPPP__etapi+pi+pi+
   tagbit set BToPPPP__etapi+pi+K+
exit
