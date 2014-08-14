#
# $Id: BFlav_B0ToDstarRhoC_Path.tcl,v 1.7 2005/11/16 23:01:55 fwinkl Exp $
#
# defines BFlav_B0ToDstarRhoC_Path tag filter path
#
#  O.Long, UCSB
#


   global tagSkim


   path create    BFlav_B0ToDstarRhoC_Path

   path insert    BFlav_B0ToDstarRhoC_Path    RecEventControlSequence

   path append    BFlav_B0ToDstarRhoC_Path    FilterBGFMultiHadron

   # Append and enable the tag module
   sourceFoundFile FilterTools/SkimTagBFlavSequence.tcl
   addTagModule TagBFlavDstarRhoC BFlav_B0ToDstarRhoC_Path

   mod clone TagFilterByName    BFlav_B0ToDstarRhoC_PreFilter
   path append    BFlav_B0ToDstarRhoC_Path    BFlav_B0ToDstarRhoC_PreFilter
   talkto    BFlav_B0ToDstarRhoC_PreFilter {
        orList set B0ToDstarRhoC_KPi
        orList set B0ToDstarRhoC_KPiPi0
        orList set B0ToDstarRhoC_K3Pi
        orList set B0ToDstarRhoC_KsPiPi
	production set true
	verbose set false
   }

##---ORL: This logic is necessary because I want to do micro
#         level analysis in this path.

   if [ expr ! [ info exists tagSkim ] ] {

#--- This explicitly assumes that the BFlavTrkPIDSequence sequence,
#--- the BFlav_RhoC_Path
#--- and BFlav_D0_*_Path paths have already run.

      sourceFoundFile CompositionSequences/CompBFlavB0Sequences.tcl
      path append BFlav_B0ToDstarRhoC_Path CompBFlavB0Sequence

      mod clone TagFromList TagBFlavB0ToDstarRhoC
      path append    BFlav_B0ToDstarRhoC_Path    TagBFlavB0ToDstarRhoC
      talkto TagBFlavB0ToDstarRhoC {
         listsToTag set B0ToDstarRhoCBFlav
         tagName set B0ToDstarRhoC_FinalBFlav
	 production set true
	 verbose set false
      }

      mod clone TagFromList TagBFlavB0ToDstarRhoC_CTT
      path append    BFlav_B0ToDstarRhoC_Path    TagBFlavB0ToDstarRhoC_CTT
      talkto TagBFlavB0ToDstarRhoC_CTT {
         listsToTag set B0ToDstarRhoCBFlavCTT
         tagName set B0ToDstarRhoC_FinalBFlav_CTT
	 production set true
	 verbose set false
      }

   } else {

      echo === Not running micro analysis for BFlav_B0ToDstarRhoC_Path.  ORL

   }


   mod clone TagFilterByName BFlav_B0ToDstarRhoC_FinalFilter
   path append    BFlav_B0ToDstarRhoC_Path    BFlav_B0ToDstarRhoC_FinalFilter
   talkto BFlav_B0ToDstarRhoC_FinalFilter {
      orList set B0ToDstarRhoC_FinalBFlav
      production set true
      verbose set false
   }
