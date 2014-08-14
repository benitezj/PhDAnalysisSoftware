#
# $Id: BToEtapPiPihPath.tcl,v 1.1 2007/07/26 14:14:14 bloom Exp $
#
path create BToEtapPiPihPath

# setup the path
sourceFoundFile FilterTools/FilterToolsProcs.tcl
setupMulti3BodySkimPath TagBToPPPP BToEtapPiPihPath

# add filter module
path append BToEtapPiPihPath FilterBToEtapPiPih
mod talk FilterBToEtapPiPih
   minNoOfTracks  set 1
   maxTotalEnergy set 20.0
#   verbose        set t
#
   tagbit set BToPPPP__eta'pi+pi+pi0
   tagbit set BToPPPP__eta'pi+pi+K_S0
   tagbit set BToPPPP__eta'pi+pi+pi+
   tagbit set BToPPPP__eta'pi+pi+K+
#   tagbit set BToPPPP__etapi+pi+pi0
#   tagbit set BToPPPP__etapi+pi+K_S0
#   tagbit set BToPPPP__etapi+pi+pi+
#   tagbit set BToPPPP__etapi+pi+K+
#   tagbit set BToPPPP__omegapi+pi+pi0
#   tagbit set BToPPPP__omegapi+pi+K_S0
#   tagbit set BToPPPP__omegapi+pi+pi+
#   tagbit set BToPPPP__omegapi+pi+K+
#   tagbit set BToPPPP__phipi+pi+pi0
#   tagbit set BToPPPP__phipi+pi+K_S0
#   tagbit set BToPPPP__phipi+pi+pi+
#   tagbit set BToPPPP__phipi+pi+K+
exit
