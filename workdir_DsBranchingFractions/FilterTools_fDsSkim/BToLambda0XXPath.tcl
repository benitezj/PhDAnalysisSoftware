#
# $Id: BToLambda0XXPath.tcl,v 1.4 2005/11/16 23:01:57 fwinkl Exp $
#
path create BToLambda0XXPath

# setup the path
sourceFoundFile FilterTools/FilterToolsProcs.tcl
setupMulti3BodySkimPath TagBToLambda0XX BToLambda0XXPath

# add filter module
path append BToLambda0XXPath FilterBToLambda0XX

catch {module enable FilterBToLambda0XX}
talkto FilterBToLambda0XX {
#   verbose        set t
   minNoOfTracks  set 1
   maxTotalEnergy set 20.0
#
   tagbit set BToLambda0XX__Lambda0Lambda0K+
   tagbit set BToLambda0XX__Lambda0Lambda0K_S0
   tagbit set BToLambda0XX__Lambda0K_S0p+
   tagbit set BToLambda0XX__Lambda0K+p+
   tagbit set BToLambda0XX__Lambda0pi+p+
   tagbit set BToLambda0XX__Lambda0Lambda0pi0
   tagbit set BToLambda0XX__Lambda0Lambda0pi+
   tagbit set BToLambda0XX__Lambda0Lambda0eta
   tagbit set "BToLambda0XX__Lambda0Lambda0eta'"
   tagbit set BToLambda0XX__Lambda0p+eta
   tagbit set "BToLambda0XX__Lambda0p+eta'"
}
