#
# $Id: AlignCalPath.tcl,v 1.5 2005/03/14 17:28:00 fnc Exp $
#
# 
# Combine Cosmics, MuMu, 4-prong(2-photon) events
# for SVT Local Alignment and Tracking Calibrations
#
# define path 
path create AlignCalPath
# set filter
mod clone TagFilterByName AlignCalFilter
path append AlignCalPath AlignCalFilter
talkto AlignCalFilter { 
    orList set DigiFCosmic
    orList set DigiFWideCosmic
    orList set BGFMuMu
    orList set is2Gam4Prongs
}
