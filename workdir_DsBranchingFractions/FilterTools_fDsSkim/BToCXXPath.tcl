#
# $Id: BToCXXPath.tcl,v 1.7 2005/11/16 23:01:56 fwinkl Exp $
#
path create BToCXXPath

# setup the path
sourceFoundFile FilterTools/FilterToolsProcs.tcl
setupMulti3BodySkimPath TagBToPPP BToCXXPath

# add filter module
path append BToCXXPath FilterBToCXX
mod talk FilterBToCXX
   minNoOfTracks  set 1
   maxTotalEnergy set 20.0
#   verbose        set t
#
   tagbit set BToPPP__K_S0etapi+
   tagbit set BToPPP__K_S0etaK+
   tagbit set BToPPP__K_S0eta'pi+
   tagbit set BToPPP__K_S0eta'K+
   tagbit set BToPPP__K_S0phipi+
   tagbit set BToPPP__K_S0phiK+
   tagbit set BToPPP__K_S0omegapi+
   tagbit set BToPPP__K_S0omegaK+
   tagbit set BToPPP__pi0etapi+
   tagbit set BToPPP__pi0etaK+
   tagbit set BToPPP__pi0eta'pi+
   tagbit set BToPPP__pi0eta'K+
   tagbit set BToPPP__pi0phipi+
   tagbit set BToPPP__pi0phiK+
   tagbit set BToPPP__pi0omegapi+
   tagbit set BToPPP__pi0omegaK+
   tagbit set BToPPP__etaeta'pi+
   tagbit set BToPPP__etaeta'K+
   tagbit set BToPPP__etaphipi+
   tagbit set BToPPP__etaphiK+
   tagbit set BToPPP__etaomegapi+
   tagbit set BToPPP__etaomegaK+
   tagbit set BToPPP__etapi+pi+
   tagbit set BToPPP__etaetapi+
   tagbit set BToPPP__etapi+K+
   tagbit set BToPPP__etaK+K+
   tagbit set BToPPP__etaetaK+
   tagbit set BToPPP__eta'phipi+
   tagbit set BToPPP__eta'phiK+
   tagbit set BToPPP__eta'omegapi+
   tagbit set BToPPP__eta'omegaK+
   tagbit set BToPPP__eta'pi+pi+
   tagbit set BToPPP__eta'eta'pi+
   tagbit set BToPPP__eta'pi+K+
   tagbit set BToPPP__eta'K+K+
   tagbit set BToPPP__eta'eta'K+
   tagbit set BToPPP__phiomegapi+
   tagbit set BToPPP__phiomegaK+
   tagbit set BToPPP__phipi+pi+
   tagbit set BToPPP__phiphipi+
   tagbit set BToPPP__phipi+K+
   tagbit set BToPPP__phiK+K+
   tagbit set BToPPP__phiphiK+
   tagbit set BToPPP__omegapi+pi+
   tagbit set BToPPP__omegaomegapi+
   tagbit set BToPPP__omegapi+K+
   tagbit set BToPPP__omegaK+K+
   tagbit set BToPPP__omegaomegaK+
exit
