#
# $Id: BFlav_D0_KPiPi0_Path.tcl,v 1.8 2005/11/16 23:01:55 fwinkl Exp $
#
# defines BFlav_D0_KPiPi0_Path tag filter path
#
#  O.Long, UCSB
#


##---ORL: This logic is necessary because I want to do micro
#         level analysis in this path.

global BFLAV_D0_KPIPI0_PATH
if [ expr ! [ info exists BFLAV_D0_KPIPI0_PATH ] ] {
    set BFLAV_D0_KPIPI0_PATH 1


global tagSkim

path create    BFlav_D0_KPiPi0_Path

if [ expr ! [ info exists tagSkim ] ] {

   path insert    BFlav_D0_KPiPi0_Path    RecEventControlSequence

   path append    BFlav_D0_KPiPi0_Path    FilterBGFMultiHadron

   # Append and enable the tag module
   sourceFoundFile FilterTools/SkimTagBFlavSequence.tcl
   addTagModule TagBFlavDstarPi   BFlav_D0_KPiPi0_Path
   addTagModule TagBFlavDstarRhoC BFlav_D0_KPiPi0_Path
   addTagModule TagBFlavDstarA1C  BFlav_D0_KPiPi0_Path

   sourceFoundFile FilterTools/SkimTagBrecoSequence.tcl
   useTagFromList BchToDstar0Pi  BFlav_D0_KPiPi0_Path
   useTagFromList BchToD0Pi      BFlav_D0_KPiPi0_Path

   mod clone TagFilterByName    BFlav_D0_KPiPi0_PreFilter
   path append    BFlav_D0_KPiPi0_Path    BFlav_D0_KPiPi0_PreFilter
   talkto    BFlav_D0_KPiPi0_PreFilter {
       orList set B0ToDstarPi_KPiPi0
       orList set B0ToDstarRhoC_KPiPi0
       orList set B0ToDstarA1C_KPiPi0
       orList set BchToD0Pi
       orList set BchToDstar0Pi
   }

   sourceFoundFile CompositionSequences/CompBFlavTrkPIDSequence.tcl
   path append    BFlav_D0_KPiPi0_Path    BFlavTrkPIDSequence

   sourceFoundFile CompositionSequences/CompBFlavLHSequences.tcl
   path append    BFlav_D0_KPiPi0_Path    Pi0ToGG_BFlav

   sourceFoundFile CompositionSequences/CompBFlavDSequences.tcl
   path append    BFlav_D0_KPiPi0_Path    D0ToKPiPi0_Vtx_BFlav
   path append    BFlav_D0_KPiPi0_Path    D0ToKPiPi0_BFlav

} else {

   echo === Not running BFlav_D0_KPiPi0_Path, which needs Micro access.  ORL

   path disable BFlav_D0_KPiPi0_Path

}

}
