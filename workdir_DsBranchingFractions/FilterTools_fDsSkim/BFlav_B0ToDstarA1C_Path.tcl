#
# $Id: BFlav_B0ToDstarA1C_Path.tcl,v 1.6 2005/11/16 23:01:55 fwinkl Exp $
#
# defines BFlav_B0ToDstarA1C_Path tag filter path
#
#  O.Long, UCSB
#


   global tagSkim


   path create    BFlav_B0ToDstarA1C_Path

   path insert    BFlav_B0ToDstarA1C_Path    RecEventControlSequence

   path append    BFlav_B0ToDstarA1C_Path    FilterBGFMultiHadron

   # Append and enable the tag module
   sourceFoundFile FilterTools/SkimTagBFlavSequence.tcl
   addTagModule TagBFlavDstarA1C  BFlav_B0ToDstarA1C_Path

   mod clone TagFilterByName    BFlav_B0ToDstarA1C_PreFilter
   path append    BFlav_B0ToDstarA1C_Path    BFlav_B0ToDstarA1C_PreFilter
   talkto    BFlav_B0ToDstarA1C_PreFilter {
        orList set B0ToDstarA1C_KPi
        orList set B0ToDstarA1C_KPiPi0
        orList set B0ToDstarA1C_K3Pi
        orList set B0ToDstarA1C_KsPiPi
   }

##---ORL: This logic is necessary because I want to do micro
#         level analysis in this path.

   if [ expr ! [ info exists tagSkim ] ] {

#--- This explicitly assumes that the BFlavTrkPIDSequence sequence,
#--- the BFlav_A1C_Path
#--- and BFlav_D0_*_Path paths have already run.

      sourceFoundFile CompositionSequences/CompBFlavB0Sequences.tcl
      path append BFlav_B0ToDstarA1C_Path CompBFlavB0Sequence

      mod clone TagFromList TagBFlavB0ToDstarA1C
      path append    BFlav_B0ToDstarA1C_Path    TagBFlavB0ToDstarA1C
      talkto TagBFlavB0ToDstarA1C {
         listsToTag set B0ToDstarA1CBFlav
         tagName set B0ToDstarA1C_FinalBFlav
      }

      mod clone TagFromList TagBFlavB0ToDstarA1C_CTT
      path append    BFlav_B0ToDstarA1C_Path    TagBFlavB0ToDstarA1C_CTT
      talkto TagBFlavB0ToDstarA1C_CTT {
         listsToTag set B0ToDstarA1CBFlavCTT
         tagName set B0ToDstarA1C_FinalBFlav_CTT
      }


   } else {

      echo === Not running micro analysis for BFlav_B0ToDstarA1C_Path.  ORL

   }


   mod clone TagFilterByName BFlav_B0ToDstarA1C_FinalFilter
   path append    BFlav_B0ToDstarA1C_Path    BFlav_B0ToDstarA1C_FinalFilter
   talkto BFlav_B0ToDstarA1C_FinalFilter {
      orList set B0ToDstarA1C_FinalBFlav
   }




