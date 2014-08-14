#
# $Id: BToPXXPath.tcl,v 1.3 2005/11/16 23:01:57 fwinkl Exp $
#
path create BToPXXPath

# setup the path
sourceFoundFile FilterTools/FilterToolsProcs.tcl
setupMulti3BodySkimPath TagBToPPP BToPXXPath

# add filter module
path append BToPXXPath FilterBToPXX
mod talk FilterBToPXX
   minNoOfTracks  set 1
   maxTotalEnergy set 20.0
#   verbose        set t
#
   tagbit set BToPPP__etap+p+
   tagbit set BToPPP__eta'p+p+
   tagbit set BToPPP__phip+p+
   tagbit set BToPPP__omegap+p+
exit
