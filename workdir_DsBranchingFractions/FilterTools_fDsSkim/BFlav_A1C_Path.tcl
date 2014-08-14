#
# $Id: BFlav_A1C_Path.tcl,v 1.4 2005/11/16 23:01:55 fwinkl Exp $
#
# defines BFlav_A1C_Path tag filter path
#
#  O.Long, UCSB
#


##---ORL: This logic is necessary because I want to do micro
#         level analysis in this path.

global tagSkim

path create    BFlav_A1C_Path

if [ expr ! [ info exists tagSkim ] ] {

   path insert    BFlav_A1C_Path    RecEventControlSequence

   path append    BFlav_A1C_Path    FilterBGFMultiHadron

   # Append and enable the tag module
   sourceFoundFile FilterTools/SkimTagBFlavSequence.tcl
   addTagModule TagBFlavDstarA1C  BFlav_A1C_Path
   addTagModule TagBFlavDA1C      BFlav_A1C_Path

   mod clone TagFilterByName    BFlav_A1C_PreFilter
   path append    BFlav_A1C_Path    BFlav_A1C_PreFilter
   talkto    BFlav_A1C_PreFilter {
        orList set B0ToDstarA1C_KPi
        orList set B0ToDstarA1C_KPiPi0
        orList set B0ToDstarA1C_K3Pi
        orList set B0ToDstarA1C_KsPiPi
        orList set B0ToDchA1C_KsPi
        orList set B0ToDchA1C_KPiPi
   }

   sourceFoundFile CompositionSequences/CompBFlavTrkPIDSequence.tcl
   path append    BFlav_A1C_Path    BFlavTrkPIDSequence

   sourceFoundFile CompositionSequences/CompBFlavLHSequences.tcl
   path append    BFlav_A1C_Path    BFlavA1CSequence

} else {

   echo === Not running BFlav_A1C_Path, which needs Micro access.  ORL

   path disable BFlav_A1C_Path

}






