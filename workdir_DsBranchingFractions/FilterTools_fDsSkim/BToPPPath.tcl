#
# $Id: BToPPPath.tcl,v 1.8 2007/10/26 10:29:23 marchior Exp $
#
path create BToPPPath

# only run on MultiHadrons
path append BToPPPath FilterBGFMultiHadron

# add the 2body sequence
sourceFoundFile FilterTools/FilterToolsProcs.tcl
sourceFoundFile FilterTools/SkimTag2BodySequence.tcl
path append BToPPPath SkimTag2BodySequence
enableAll2BodyModules

# add filter module
path append BToPPPath FilterBToPP
mod talk FilterBToPP
   minNoOfTracks  set 1
   maxTotalEnergy set 20.0
#   verbose        set t
#
   tagbit set BToPP__K_S0K_S0
   tagbit set BToPP__K_S0pi0
   tagbit set BToPP__K_S0eta
   tagbit set BToPP__K_S0eta'
   tagbit set BToPP__K_S0phi
   tagbit set BToPP__K_S0omega
   tagbit set BToPP__K_S0pi+
   tagbit set BToPP__K_S0K+
   tagbit set BToPP__pi0pi0
   tagbit set BToPP__pi0eta
   tagbit set BToPP__pi0eta'
   tagbit set BToPP__pi0phi
   tagbit set BToPP__pi0omega
   tagbit set BToPP__pi0pi+
   tagbit set BToPP__pi0K+
   tagbit set BToPP__etaeta
   tagbit set BToPP__etaeta'
   tagbit set BToPP__etaphi
   tagbit set BToPP__etaomega
   tagbit set BToPP__etapi+
   tagbit set BToPP__etaK+
   tagbit set BToPP__eta'eta'
   tagbit set BToPP__eta'phi
   tagbit set BToPP__eta'omega
   tagbit set BToPP__eta'pi+
   tagbit set BToPP__eta'K+
   tagbit set BToPP__phiphi
   tagbit set BToPP__phiomega
   tagbit set BToPP__phipi+
   tagbit set BToPP__phiK+
   tagbit set BToPP__omegaomega
   tagbit set BToPP__omegapi+
   tagbit set BToPP__omegaK+
   tagbit set BToPP__pi+pi+
   tagbit set BToPP__pi+K+
   tagbit set BToPP__K+K+
   tagbit set BToPP__p+p+
exit
