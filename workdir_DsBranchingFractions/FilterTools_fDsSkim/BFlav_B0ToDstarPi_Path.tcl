#
# $Id: BFlav_B0ToDstarPi_Path.tcl,v 1.7 2005/11/16 23:01:55 fwinkl Exp $
#
# defines BFlav_B0ToDstarPi_Path tag filter path
#
#  O.Long, UCSB
#


   global tagSkim


   path create    BFlav_B0ToDstarPi_Path

   path insert    BFlav_B0ToDstarPi_Path    RecEventControlSequence

   path append    BFlav_B0ToDstarPi_Path    FilterBGFMultiHadron

   # Append and enable the tag module
   sourceFoundFile FilterTools/SkimTagBFlavSequence.tcl
   addTagModule TagBFlavDstarPi BFlav_B0ToDstarPi_Path

   mod clone TagFilterByName    BFlav_B0ToDstarPi_PreFilter
   path append    BFlav_B0ToDstarPi_Path    BFlav_B0ToDstarPi_PreFilter
   talkto    BFlav_B0ToDstarPi_PreFilter {
        orList set B0ToDstarPi_KPi
        orList set B0ToDstarPi_KPiPi0
        orList set B0ToDstarPi_K3Pi
        orList set B0ToDstarPi_KsPiPi
	production set true
	verbose set false
   }

   sourceFoundFile CompositionSequences/CompBFlavDSequences.tcl
   path append    BFlav_B0ToDstarPi_Path CompBFlavDSequence

##---ORL: This logic is necessary because I want to do micro
#         level analysis in this path.

   if [ expr ! [ info exists tagSkim ] ] {

#--- This explicitly assumes that the BFlavTrkPIDSequence sequence
#--- and BFlav_D0_*_Path paths have already run.

      sourceFoundFile CompositionSequences/CompBFlavB0Sequences.tcl
      path append BFlav_B0ToDstarPi_Path CompBFlavB0Sequence

      mod clone TagFromList TagBFlavB0ToDstarPi
      path append    BFlav_B0ToDstarPi_Path    TagBFlavB0ToDstarPi
      talkto TagBFlavB0ToDstarPi {
         listsToTag set B0ToDstarPiBFlav
         tagName set B0ToDstarPi_FinalBFlav
      }

      mod clone TagFromList TagBFlavB0ToDstarPi_CTT
      path append    BFlav_B0ToDstarPi_Path    TagBFlavB0ToDstarPi_CTT
      talkto TagBFlavB0ToDstarPi_CTT {
         listsToTag set B0ToDstarPiBFlavCTT
         tagName set B0ToDstarPi_FinalBFlav_CTT
	 production set true
	 verbose set false
      }

   } else {

      echo === Not running micro analysis for BFlav_B0ToDstarPi_Path.  ORL

   }

   mod clone TagFilterByName BFlav_B0ToDstarPi_FinalFilter
   path append    BFlav_B0ToDstarPi_Path    BFlav_B0ToDstarPi_FinalFilter
   talkto BFlav_B0ToDstarPi_FinalFilter {
      orList set B0ToDstarPi_FinalBFlav
      production set true
      verbose set false
   }
