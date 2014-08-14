#
# $Id: BFlav_RhoC_Path.tcl,v 1.4 2005/11/16 23:01:55 fwinkl Exp $
#
# defines BFlav_RhoC_Path tag filter path
#
#  O.Long, UCSB
#


##---ORL: This logic is necessary because I want to do micro
#         level analysis in this path.

global tagSkim

path create    BFlav_RhoC_Path

if [ expr ! [ info exists tagSkim ] ] {

   path insert    BFlav_RhoC_Path    RecEventControlSequence

   path append    BFlav_RhoC_Path    FilterBGFMultiHadron

   # Append and enable the tag module
   sourceFoundFile FilterTools/SkimTagBFlavSequence.tcl
   addTagModule TagBFlavDstarRhoC  BFlav_RhoC_Path
   addTagModule TagBFlavDRhoC      BFlav_RhoC_Path

   mod clone TagFilterByName    BFlav_RhoC_PreFilter
   path append    BFlav_RhoC_Path    BFlav_RhoC_PreFilter
   talkto    BFlav_RhoC_PreFilter {
        orList set B0ToDstarRhoC_KPi
        orList set B0ToDstarRhoC_KPiPi0
        orList set B0ToDstarRhoC_K3Pi
        orList set B0ToDstarRhoC_KsPiPi
        orList set B0ToDchRhoC_KsPi
        orList set B0ToDchRhoC_KPiPi
   }

   sourceFoundFile CompositionSequences/CompBFlavTrkPIDSequence.tcl
   path append    BFlav_RhoC_Path    BFlavTrkPIDSequence

   sourceFoundFile CompositionSequences/CompBFlavLHSequences.tcl

   path append    BFlav_RhoC_Path    BFlavSemiLoosePi0Sequence
   path append    BFlav_RhoC_Path    BFlavRhoCSequence

   path append    BFlav_RhoC_Path    Pi0ToGGHardLoose_BFlav
   path append    BFlav_RhoC_Path    BFlavRhoCHardLoosePi0Sequence

} else {

   echo === Not running BFlav_RhoC_Path, which needs Micro access.  ORL

   path disable BFlav_RhoC_Path

}






