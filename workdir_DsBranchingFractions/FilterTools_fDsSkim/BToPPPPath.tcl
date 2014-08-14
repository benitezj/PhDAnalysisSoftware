#
# $Id: BToPPPPath.tcl,v 1.6 2005/11/16 23:01:57 fwinkl Exp $
#
#

path create BToPPPPath

# setup the path
sourceFoundFile FilterTools/FilterToolsProcs.tcl
setupMulti3BodySkimPath TagBToPPP BToPPPPath

# add filter module
path append BToPPPPath FilterBToPPP
catch {module enable FilterBToPPP}
mod talk FilterBToPPP
   minNoOfTracks set 1
   maxTotalEnergy set 20.0
#   verbose set t
#
   tagbit set BToPPP__K_S0K_S0K_S0
   tagbit set BToPPP__K_S0pi0pi0
   tagbit set BToPPP__K_S0K_S0pi0
   tagbit set BToPPP__K_S0pi0eta
   tagbit set BToPPP__K_S0pi0eta'
   tagbit set BToPPP__K_S0pi0phi
   tagbit set BToPPP__K_S0pi0omega
   tagbit set BToPPP__K_S0etaeta
   tagbit set BToPPP__K_S0K_S0eta
   tagbit set BToPPP__K_S0etaeta'
   tagbit set BToPPP__K_S0etaphi
   tagbit set BToPPP__K_S0etaomega
   tagbit set BToPPP__K_S0eta'eta'
   tagbit set BToPPP__K_S0K_S0eta'
   tagbit set BToPPP__K_S0eta'phi
   tagbit set BToPPP__K_S0eta'omega
   tagbit set BToPPP__K_S0phiphi
   tagbit set BToPPP__K_S0K_S0phi
   tagbit set BToPPP__K_S0phiomega
   tagbit set BToPPP__K_S0omegaomega
   tagbit set BToPPP__K_S0K_S0omega
   tagbit set BToPPP__pi0pi0pi0
   tagbit set BToPPP__pi0etaeta
   tagbit set BToPPP__pi0pi0eta
   tagbit set BToPPP__pi0etaeta'
   tagbit set BToPPP__pi0etaphi
   tagbit set BToPPP__pi0etaomega
   tagbit set BToPPP__pi0eta'eta'
   tagbit set BToPPP__pi0pi0eta'
   tagbit set BToPPP__pi0eta'phi
   tagbit set BToPPP__pi0eta'omega
   tagbit set BToPPP__pi0phiphi
   tagbit set BToPPP__pi0pi0phi
   tagbit set BToPPP__pi0phiomega
   tagbit set BToPPP__pi0omegaomega
   tagbit set BToPPP__pi0pi0omega
   tagbit set BToPPP__etaetaeta
   tagbit set BToPPP__etaeta'eta'
   tagbit set BToPPP__etaetaeta'
   tagbit set BToPPP__etaeta'phi
   tagbit set BToPPP__etaeta'omega
   tagbit set BToPPP__etaphiphi
   tagbit set BToPPP__etaetaphi
   tagbit set BToPPP__etaphiomega
   tagbit set BToPPP__etaomegaomega
   tagbit set BToPPP__etaetaomega
   tagbit set BToPPP__eta'eta'eta'
   tagbit set BToPPP__eta'phiphi
   tagbit set BToPPP__eta'eta'phi
   tagbit set BToPPP__eta'phiomega
   tagbit set BToPPP__eta'omegaomega
   tagbit set BToPPP__eta'eta'omega
   tagbit set BToPPP__phiphiphi
   tagbit set BToPPP__phiomegaomega
   tagbit set BToPPP__phiphiomega
   tagbit set BToPPP__omegaomegaomega
exit
