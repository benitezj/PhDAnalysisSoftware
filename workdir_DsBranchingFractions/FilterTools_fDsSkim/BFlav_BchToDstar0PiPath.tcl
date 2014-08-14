#
# $Id: BFlav_BchToDstar0PiPath.tcl,v 1.5 2005/11/16 23:01:55 fwinkl Exp $
#
# defines BFlav_BchToDstar0PiPath tag filter path
#
#  O.Long, UCSB
#

global BFLAV_BCHTODSTAR0PIPATH
if [ expr ! [ info exists BFLAV_BCHTODSTAR0PIPATH ] ] {
    set BFLAV_BCHTODSTAR0PIPATH 1


   global tagSkim


   path create    BFlav_BchToDstar0PiPath

   path append    BFlav_BchToDstar0PiPath    FilterBGFMultiHadron

   # Append and enable the tag module
   sourceFoundFile FilterTools/SkimTagBrecoSequence.tcl
   useTagFromList BchToDstar0Pi BFlav_BchToDstar0PiPath

   mod clone TagFilterByName    BFlav_BchToDstar0Pi_PreFilter
   path append    BFlav_BchToDstar0PiPath    BFlav_BchToDstar0Pi_PreFilter
   talkto    BFlav_BchToDstar0Pi_PreFilter {
        orList set BchToDstar0Pi
	production set true
	verbose set false
   }

##---ORL: This logic is necessary because I want to do micro
#         level analysis in this path.

   if [ expr ! [ info exists tagSkim ] ] {

#--- This explicitly assumes that the BFlavTrkPIDSequence sequence
#--- and BFlav_D0_*_Path paths have already run.

       sourceFoundFile CompositionSequences/CompBFlavBchSequences.tcl
       path append BFlav_BchToDstar0PiPath CompBFlavBchSequence
       
       mod clone TagFromList TagBFlavBchToDstar0Pi
       path append    BFlav_BchToDstar0PiPath    TagBFlavBchToDstar0Pi
       talkto TagBFlavBchToDstar0Pi {
	   listsToTag set BchToDstar0PiBFlav
	   tagName set BchToDstar0Pi_FinalBFlav
	   production set true
	   verbose set false
       }
       
       mod clone TagFromList TagBFlavBchToDstar0Pi_CTT
       path append    BFlav_BchToDstar0PiPath    TagBFlavBchToDstar0Pi_CTT
       talkto TagBFlavBchToDstar0Pi_CTT {
	   listsToTag set BchToDstar0PiBFlavCTT
	   tagName set BchToDstar0Pi_FinalBFlav_CTT
	   production set true
	   verbose set false
       }

   } else {
       
       echo === Not running micro analysis for BFlav_BchToDstar0PiPath.  ORL
       
   }
   
   mod clone TagFilterByName BFlav_BchToDstar0Pi_FinalFilter
   path append    BFlav_BchToDstar0PiPath    BFlav_BchToDstar0Pi_FinalFilter
   talkto BFlav_BchToDstar0Pi_FinalFilter {
       orList set BchToDstar0Pi_FinalBFlav
       production set true
       verbose set false
   }

}
