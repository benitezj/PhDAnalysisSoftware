#
# $Id: BtoKKKLPath.tcl,v 1.4 2006/02/03 18:40:30 fwinkl Exp $
#
#
# defines BtoKKKLPath tag filter path
#
global BtaCandLists

path create BtoKKKLPath

sourceFoundFile EventTagTools/TagProcs.tcl
sourceFoundFile FilterTools/SkimTag3BodySequence.tcl
addTagModule TagKKKL BtoKKKLPath

# clone and customize filter module
mod clone TagFilterByName BtoKKKLFilter
path append BtoKKKLPath  BtoKKKLFilter
talkto  BtoKKKLFilter  {
    andList set BtoKKKL 
}


# Add lists of composites to write
lappend BtaCandLists  TagKKKLB0KpKmKsList TagKKKLB0KpKmKlList TagKKKLB0KsKlKsList TagKKKLChBKsKlKChList TagKKKLChBKsKsKChList 
