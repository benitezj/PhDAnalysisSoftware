#
# $Id: LumiTagPath.tcl,v 1.2 2005/03/14 17:28:10 fnc Exp $
#
# 
#
# defines path for prescaled Lumi event selection
#
path create LumiTagPath

# and now filter
mod clone TagFilterByName LumiPrescFilter
path append LumiTagPath LumiPrescFilter
talkto LumiPrescFilter { 
    orList set isLumiMuMuPrescale
    orList set isLumiGaGaPrescale
    orList set isLumiBaBaPrescale
}

