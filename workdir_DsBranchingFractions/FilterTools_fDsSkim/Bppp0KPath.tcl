#
# $Id: Bppp0KPath.tcl,v 1.5 2005/11/16 23:01:57 fwinkl Exp $
#
#module enable pi0ConvVeryLoose
#
# defines Bppp0KPath tag filter path
#

path create Bppp0KPath

# only run on MultiHadrons
path append Bppp0KPath FilterBGFMultiHadron

# append and enable tag module
sourceFoundFile EventTagTools/TagProcs.tcl
addTagModule TagBppp0K Bppp0KPath

#path append BFourBodyPath Bppp0KFilter
mod clone TagFilterByName Bppp0KFilter
path append Bppp0KPath Bppp0KFilter
talkto Bppp0KFilter { 
    andList set Bppp0KSh
#    orList set Bppp0KCh
}


# Add lists of composites to write
lappend BtaCandLists TagBppp0KShList 


