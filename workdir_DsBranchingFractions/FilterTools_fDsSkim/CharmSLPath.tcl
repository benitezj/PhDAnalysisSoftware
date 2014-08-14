# $Id: CharmSLPath.tcl,v 1.2 2005/06/11 12:50:48 fnc Exp $
#
# defines CharmSL filter path
#

path create CharmSLPath

# only run on MultiHadrons
path append CharmSLPath FilterBGFMultiHadron

# and now filter
path append CharmSLPath CharmSLFilter
