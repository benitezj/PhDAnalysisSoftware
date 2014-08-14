#
# $Id: BToKstarXXPath.tcl,v 1.6 2007/10/26 11:24:44 marchior Exp $
#
path create BToKstarXXPath

# setup the path
sourceFoundFile FilterTools/FilterToolsProcs.tcl
setupMulti3BodySkimPath TagBToKstarXX BToKstarXXPath

# add filter module
path append BToKstarXXPath FilterBToKstarXX
catch {module enable FilterBToKstarXX}
mod talk FilterBToKstarXX
#   verbose        set t
   minNoOfTracks  set 1
   maxTotalEnergy set 20.0
#
   tagbit set BToKstarXX__K_S0K_S0K*0
   tagbit set BToKstarXX__K_S0K_S0K*+
   tagbit set BToKstarXX__phiphiK*0
   tagbit set BToKstarXX__phiphiK*+
exit
