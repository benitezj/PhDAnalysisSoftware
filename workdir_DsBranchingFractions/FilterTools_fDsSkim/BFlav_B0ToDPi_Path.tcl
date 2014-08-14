#
# $Id: BFlav_B0ToDPi_Path.tcl,v 1.7 2005/11/16 23:01:55 fwinkl Exp $
#
# defines BFlav_B0ToDPi_Path tag filter path
#
#  O.Long, UCSB
#


   global tagSkim


   path create    BFlav_B0ToDPi_Path

   path insert    BFlav_B0ToDPi_Path    RecEventControlSequence

   path append    BFlav_B0ToDPi_Path    FilterBGFMultiHadron

   # Append and enable the tag module
   sourceFoundFile FilterTools/SkimTagBFlavSequence.tcl
   addTagModule TagBFlavDPi  BFlav_B0ToDPi_Path

   mod clone TagFilterByName    BFlav_B0ToDPi_PreFilter
   path append    BFlav_B0ToDPi_Path    BFlav_B0ToDPi_PreFilter
   talkto    BFlav_B0ToDPi_PreFilter {
        orList set B0ToDchPi_KsPi
        orList set B0ToDchPi_KPiPi
   }

##---ORL: This logic is necessary because I want to do micro
#         level analysis in this path.

   if [ expr ! [ info exists tagSkim ] ] {

#--- This explicitly assumes that the BFlavTrkPIDSequence sequence
#--- and BFlav_Dc_*_Path paths have already run.

      sourceFoundFile CompositionSequences/CompBFlavB0Sequences.tcl
      path append BFlav_B0ToDPi_Path CompBFlavB0Sequence

      mod clone TagFromList TagBFlavB0ToDPi
      path append    BFlav_B0ToDPi_Path    TagBFlavB0ToDPi
      talkto TagBFlavB0ToDPi {
         listsToTag set B0ToDPiBFlav
         tagName set B0ToDPi_FinalBFlav
	 production set true
	 verbose set false
      }

      mod clone TagFromList TagBFlavB0ToDPi_CTT
      path append    BFlav_B0ToDPi_Path    TagBFlavB0ToDPi_CTT
      talkto TagBFlavB0ToDPi_CTT {
         listsToTag set B0ToDPiBFlavCTT
         tagName set B0ToDPi_FinalBFlav_CTT
         production set true
	 verbose set false
      }


   } else {

      echo === Not running micro analysis for BFlav_B0ToDPi_Path.  ORL

   }

   mod clone TagFilterByName BFlav_B0ToDPi_FinalFilter
   path append    BFlav_B0ToDPi_Path    BFlav_B0ToDPi_FinalFilter
   talkto BFlav_B0ToDPi_FinalFilter {
      orList set B0ToDPi_FinalBFlav
      production set true
      verbose set false
   }
