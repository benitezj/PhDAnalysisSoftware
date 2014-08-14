#
# $Id: PPbarKPath.tcl,v 1.12 2003/10/10 16:07:28 fnc Exp $
#
# defines path for exclusive eta_c analysis
#
global BtaCandLists

path create PPbarKPath

# only run on MultiHadrons
path append PPbarKPath FilterBGFMultiHadron

# and now filter
mod clone TagFilterByName PPbarKFilter
path append PPbarKPath PPbarKFilter
talkto PPbarKFilter { 
    orList set BToPPbarK
}

# Add lists of composites to write
lappend BtaCandLists BToPPbarK
