#
# $Id: BFlav_B0ToDRhoC_Path.tcl,v 1.7 2005/11/16 23:01:55 fwinkl Exp $
#
# defines BFlav_B0ToDRhoC_Path tag filter path
#
#  O.Long, UCSB
#


   global tagSkim


   path create    BFlav_B0ToDRhoC_Path

   path insert    BFlav_B0ToDRhoC_Path    RecEventControlSequence

   path append    BFlav_B0ToDRhoC_Path    FilterBGFMultiHadron

   # Append and enable the tag module
   sourceFoundFile FilterTools/SkimTagBFlavSequence.tcl
   addTagModule TagBFlavDRhoC   BFlav_B0ToDRhoC_Path

   mod clone TagFilterByName    BFlav_B0ToDRhoC_PreFilter
   path append    BFlav_B0ToDRhoC_Path    BFlav_B0ToDRhoC_PreFilter
   talkto    BFlav_B0ToDRhoC_PreFilter {
        orList set B0ToDchRhoC_KsPi
        orList set B0ToDchRhoC_KPiPi
	production set true
	verbose set false
   }

##---ORL: This logic is necessary because I want to do micro
#         level analysis in this path.

   if [ expr ! [ info exists tagSkim ] ] {

#--- This explicitly assumes that the BFlavTrkPIDSequence sequence,
#--- the BFlav_RhoC_Path
#--- and BFlav_Dc_*_Path paths have already run.

      sourceFoundFile CompositionSequences/CompBFlavB0Sequences.tcl
      path append BFlav_B0ToDRhoC_Path CompBFlavB0Sequence

      mod clone TagFromList TagBFlavB0ToDRhoC
      path append    BFlav_B0ToDRhoC_Path    TagBFlavB0ToDRhoC
      talkto TagBFlavB0ToDRhoC {
         listsToTag set B0ToDRhoCBFlav
         tagName set B0ToDRhoC_FinalBFlav
	 production set true
	 verbose set false
      }

      mod clone TagFromList TagBFlavB0ToDRhoC_CTT
      path append    BFlav_B0ToDRhoC_Path    TagBFlavB0ToDRhoC_CTT
      talkto TagBFlavB0ToDRhoC_CTT {
         listsToTag set B0ToDRhoCBFlavCTT
         tagName set B0ToDRhoC_FinalBFlav_CTT
 	 production set true
	 verbose set false
      }


   } else {

      echo === Not running micro analysis for BFlav_B0ToDRhoC_Path.  ORL

   }


   mod clone TagFilterByName BFlav_B0ToDRhoC_FinalFilter
   path append    BFlav_B0ToDRhoC_Path    BFlav_B0ToDRhoC_FinalFilter
   talkto BFlav_B0ToDRhoC_FinalFilter {
      orList set B0ToDRhoC_FinalBFlav
      production set true
      verbose set false
   }
