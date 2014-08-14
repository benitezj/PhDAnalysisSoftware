#
# $Id: BFlav_Dc_KsPi_Path.tcl,v 1.5 2005/11/16 23:01:55 fwinkl Exp $
#
# defines BFlav_Dc_KsPi_Path tag filter path
#
#  O.Long, UCSB
#


##---ORL: This logic is necessary because I want to do micro
#         level analysis in this path.

global tagSkim

path create    BFlav_Dc_KsPi_Path

if [ expr ! [ info exists tagSkim ] ] {

   path insert    BFlav_Dc_KsPi_Path    RecEventControlSequence

   path append    BFlav_Dc_KsPi_Path    FilterBGFMultiHadron

   # Append and enable the tag module
   sourceFoundFile FilterTools/SkimTagBFlavSequence.tcl
   addTagModule TagBFlavDPi    BFlav_Dc_KsPi_Path
   addTagModule TagBFlavDRhoC  BFlav_Dc_KsPi_Path
   addTagModule TagBFlavDA1C   BFlav_Dc_KsPi_Path

   mod clone TagFilterByName    BFlav_Dc_KsPi_PreFilter
   path append    BFlav_Dc_KsPi_Path    BFlav_Dc_KsPi_PreFilter
   talkto    BFlav_Dc_KsPi_PreFilter {
        orList set B0ToDchPi_KsPi
        orList set B0ToDchRhoC_KsPi
        orList set B0ToDchA1C_KsPi
   }

   sourceFoundFile CompositionSequences/CompBFlavTrkPIDSequence.tcl
   path append    BFlav_Dc_KsPi_Path    BFlavTrkPIDSequence

   sourceFoundFile CompositionSequences/CompBFlavLHSequences.tcl
   path append    BFlav_Dc_KsPi_Path    KsToPiPi_BFlav

   sourceFoundFile CompositionSequences/CompBFlavDSequences.tcl
   path append    BFlav_Dc_KsPi_Path    DcToKsPi_Vtx_BFlav
   path append    BFlav_Dc_KsPi_Path    DcToKsPi_BFlav

} else {

   echo === Not running BFlav_Dc_KsPi_Path, which needs Micro access.  ORL

   path disable BFlav_Dc_KsPi_Path

}






