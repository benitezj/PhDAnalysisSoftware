#
# $Id: BFlav_D0_KPi_Path.tcl,v 1.7 2005/11/16 23:01:55 fwinkl Exp $
#
# defines BFlav_D0_KPi_Path tag filter path
#
#  O.Long, UCSB
#

global BFLAV_D0_KPI_PATH
if [ expr ! [ info exists BFLAV_D0_KPI_PATH ] ] {
    set BFLAV_D0_KPI_PATH 1


##---ORL: This logic is necessary because I want to do micro
#         level analysis in this path.

global tagSkim

path create    BFlav_D0_KPi_Path

if [ expr ! [ info exists tagSkim ] ] {


   path insert    BFlav_D0_KPi_Path    RecEventControlSequence

   path append    BFlav_D0_KPi_Path    FilterBGFMultiHadron

   # Append and enable the tag module
   sourceFoundFile FilterTools/SkimTagBFlavSequence.tcl
   addTagModule TagBFlavDstarPi   BFlav_D0_KPi_Path
   addTagModule TagBFlavDstarRhoC BFlav_D0_KPi_Path
   addTagModule TagBFlavDstarA1C  BFlav_D0_KPi_Path

   sourceFoundFile FilterTools/SkimTagBrecoSequence.tcl
   useTagFromList BchToDstar0Pi  BFlav_D0_KPi_Path
   useTagFromList BchToD0Pi      BFlav_D0_KPi_Path

   mod clone TagFilterByName    BFlav_D0_KPi_PreFilter
   path append    BFlav_D0_KPi_Path    BFlav_D0_KPi_PreFilter
   talkto    BFlav_D0_KPi_PreFilter {
       orList set B0ToDstarPi_KPi
       orList set B0ToDstarRhoC_KPi
       orList set B0ToDstarA1C_KPi
       orList set BchToD0Pi
       orList set BchToDstar0Pi
   }

   sourceFoundFile CompositionSequences/CompBFlavTrkPIDSequence.tcl
   path append    BFlav_D0_KPi_Path    BFlavTrkPIDSequence

   sourceFoundFile CompositionSequences/CompBFlavDSequences.tcl
   path append    BFlav_D0_KPi_Path    D0ToKPi_Vtx_BFlav
   path append    BFlav_D0_KPi_Path    D0ToKPi_BFlav

} else {

   echo === Not running BFlav_D0_KPi_Path, which needs Micro access.  ORL

   path disable BFlav_D0_KPi_Path

}

}




