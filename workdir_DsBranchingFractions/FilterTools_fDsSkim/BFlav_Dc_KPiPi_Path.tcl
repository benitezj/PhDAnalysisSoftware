#
# $Id: BFlav_Dc_KPiPi_Path.tcl,v 1.5 2005/11/16 23:01:55 fwinkl Exp $
#
# defines BFlav_Dc_KPiPi_Path tag filter path
#
#  O.Long, UCSB
#


##---ORL: This logic is necessary because I want to do micro
#         level analysis in this path.

global tagSkim

path create    BFlav_Dc_KPiPi_Path

if [ expr ! [ info exists tagSkim ] ] {


   path insert    BFlav_Dc_KPiPi_Path    RecEventControlSequence

   path append    BFlav_Dc_KPiPi_Path    FilterBGFMultiHadron

   # Append and enable the tag module
   sourceFoundFile FilterTools/SkimTagBFlavSequence.tcl
   addTagModule TagBFlavDPi    BFlav_Dc_KPiPi_Path
   addTagModule TagBFlavDRhoC  BFlav_Dc_KPiPi_Path
   addTagModule TagBFlavDA1C   BFlav_Dc_KPiPi_Path

   mod clone TagFilterByName    BFlav_Dc_KPiPi_PreFilter
   path append    BFlav_Dc_KPiPi_Path    BFlav_Dc_KPiPi_PreFilter
   talkto    BFlav_Dc_KPiPi_PreFilter {
        orList set B0ToDchPi_KPiPi
        orList set B0ToDchRhoC_KPiPi
        orList set B0ToDchA1C_KPiPi
   }

   sourceFoundFile CompositionSequences/CompBFlavTrkPIDSequence.tcl
   path append    BFlav_Dc_KPiPi_Path    BFlavTrkPIDSequence

   sourceFoundFile CompositionSequences/CompBFlavDSequences.tcl
   path append    BFlav_Dc_KPiPi_Path    DcToKPiPi_Vtx_BFlav
   path append    BFlav_Dc_KPiPi_Path    DcToKPiPi_BFlav

} else {

   echo === Not running BFlav_Dc_KPiPi_Path, which needs Micro access.  ORL

   path disable BFlav_Dc_KPiPi_Path

}






