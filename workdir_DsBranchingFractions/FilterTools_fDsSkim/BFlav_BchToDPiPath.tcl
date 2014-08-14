#
# $Id: BFlav_BchToDPiPath.tcl,v 1.5 2005/11/16 23:01:55 fwinkl Exp $
#
# defines BFlav_BchToDPiPath tag filter path
#
#  O.Long, UCSB
#

global BFLAV_BCHTODPIPATH
if [ expr ! [ info exists BFLAV_BCHTODPIPATH ] ] {
    set BFLAV_BCHTODPIPATH 1


   global tagSkim


   path create    BFlav_BchToDPiPath

   path append    BFlav_BchToDPiPath    FilterBGFMultiHadron

   # Append and enable the tag module
   sourceFoundFile FilterTools/SkimTagBrecoSequence.tcl
   useTagFromList BchToD0Pi    BFlav_BchToDPiPath

   mod clone TagFilterByName    BFlav_BchToDPi_PreFilter
   path append    BFlav_BchToDPiPath    BFlav_BchToDPi_PreFilter
   talkto    BFlav_BchToDPi_PreFilter {
       orList set BchToD0Pi
       production set true
       verbose set false
   }

##---ORL: This logic is necessary because I want to do micro
#         level analysis in this path.

   if [ expr ! [ info exists tagSkim ] ] {

#--- This explicitly assumes that the BFlavTrkPIDSequence sequence
#--- and BFlav_D_*_Path paths have already run.

      sourceFoundFile CompositionSequences/CompBFlavBchSequences.tcl
      path append BFlav_BchToDPiPath CompBFlavBchSequence

      mod clone TagFromList TagBFlavBchToDPi
      path append    BFlav_BchToDPiPath    TagBFlavBchToDPi
      talkto TagBFlavBchToDPi {
         listsToTag set BchToDPiBFlav
         tagName set BchToDPi_FinalBFlav
	 production set true
	 verbose set false
      }

      mod clone TagFromList TagBFlavBchToDPi_CTT
      path append    BFlav_BchToDPiPath    TagBFlavBchToDPi_CTT
      talkto TagBFlavBchToDPi_CTT {
         listsToTag set BchToDPiBFlavCTT
         tagName set BchToDPi_FinalBFlav_CTT
	 production set true
	 verbose set false
      }


   } else {

      echo === Not running micro analysis for BFlav_BchToDPiPath.  ORL

   }

   mod clone TagFilterByName BFlav_BchToDPi_FinalFilter
   path append    BFlav_BchToDPiPath    BFlav_BchToDPi_FinalFilter
   talkto BFlav_BchToDPi_FinalFilter {
      orList set BchToDPi_FinalBFlav
      production set true
      verbose set false
   }


}
