#
# $Id: BchFlav_FinalPath.tcl,v 1.11 2007/02/23 16:51:36 marchior Exp $
#
# defines BFlav_FinalPath tag filter path
#
#  O.Long, UCSB
#

path create BchFlav_FinalPath
path append BchFlav_FinalPath FilterBGFMultiHadron

sourceFoundFile FilterTools/SkimTagBrecoSequence.tcl

global BCHFLAV_LOOSEPATH
if [ expr ! [ info exists BCHFLAV_LOOSEPATH ] ] {

    #if FilterTools/BchFlav_LoosePath.tcl  hasn't be sourced, define
    #BchFlav_Loose_Tag and BchFlav_LooseFilter

    mod clone TagFromTag BchFlav_Loose_Tag
    talkto BchFlav_Loose_Tag {
	tagName set BchFlav_Loose
	orList set BchToDstar0Pi
	orList set BchToD0Pi
	production set true
	verbose set false
    }

    mod clone TagFilterByName BchFlav_LooseFilter
    talkto BchFlav_LooseFilter {
	orList set BchFlav_Loose
	production set true
	verbose set false
    }
}


# Append and enable the tag module
useTagFromList  BchToDstar0Pi  BchFlav_FinalPath
useTagFromList  BchToD0Pi      BchFlav_FinalPath
path append BchFlav_FinalPath BchFlav_Loose_Tag
path append BchFlav_FinalPath BchFlav_LooseFilter

sourceFoundFile CompositionSequences/CompBFlavTrkPIDSequence.tcl
path append BchFlav_FinalPath BFlavTrkPIDSequence
sourceFoundFile CompositionSequences/CompBFlavBchSequences.tcl
path append BchFlav_FinalPath CompBFlavBchSequence

mod clone TagFromList TagBFlavBchToDPi
talkto TagBFlavBchToDPi {
    listsToTag set BchToDPiBFlav
    tagName set BchToDPi_FinalBFlav
    production set true
    verbose set false
}
path append BchFlav_FinalPath TagBFlavBchToDPi

mod clone TagFromList TagBFlavBchToDstar0Pi
talkto TagBFlavBchToDstar0Pi {
    listsToTag set BchToDstar0PiBFlav
    tagName set BchToDstar0Pi_FinalBFlav
    production set true
    verbose set false
}
path append BchFlav_FinalPath TagBFlavBchToDstar0Pi

mod clone TagFromTag BchFlav_Final_Tag
path append BchFlav_FinalPath BchFlav_Final_Tag
talkto BchFlav_Final_Tag {
    tagName set BchFlav_Final
    orList set BchToDstar0Pi_FinalBFlav
    orList set BchToDPi_FinalBFlav
    production set true
    verbose set false
}
        
mod clone TagFromTag BchFlav_FinalCTT_Tag
path append BchFlav_FinalPath BchFlav_FinalCTT_Tag
talkto BchFlav_FinalCTT_Tag {
    tagName set BchFlav_Final_CTT
    orList set BchToDstar0Pi_FinalBFlav_CTT
    orList set BchToDPi_FinalBFlav_CTT
    production set true
    verbose set false
}

mod clone TagFilterByName BchFlav_FinalFilter
path append BchFlav_FinalPath BchFlav_FinalFilter
talkto BchFlav_FinalFilter {
    orList set BchFlav_Final
    production set true
    verbose set false
}
  
# Add lists of composites to write
lappend BtaCandLists BchToDstar0PiBFlav BchToDPiBFlav
