#
# $Id: BFlav_D0_K3Pi_Path.tcl,v 1.7 2005/11/16 23:01:55 fwinkl Exp $
#
# defines BFlav_D0_K3Pi_Path tag filter path
#
#  O.Long, UCSB
#

##---ORL: This logic is necessary because I want to do micro
#         level analysis in this path.


global BFLAV_D0_K3PI_PATH
if [ expr ! [ info exists BFLAV_D0_K3PI_PATH ] ] {
    set BFLAV_D0_K3PI_PATH 1


global tagSkim

path create    BFlav_D0_K3Pi_Path

if [ expr ! [ info exists tagSkim ] ] {


   path insert    BFlav_D0_K3Pi_Path    RecEventControlSequence

   path append    BFlav_D0_K3Pi_Path    FilterBGFMultiHadron

   # Append and enable the tag module
   sourceFoundFile FilterTools/SkimTagBFlavSequence.tcl
   addTagModule TagBFlavDstarPi   BFlav_D0_K3Pi_Path
   addTagModule TagBFlavDstarRhoC BFlav_D0_K3Pi_Path
   addTagModule TagBFlavDstarA1C  BFlav_D0_K3Pi_Path

   sourceFoundFile FilterTools/SkimTagBrecoSequence.tcl
   useTagFromList BchToDstar0Pi  BFlav_D0_K3Pi_Path
   useTagFromList BchToD0Pi      BFlav_D0_K3Pi_Path
   
   mod clone TagFilterByName    BFlav_D0_K3Pi_PreFilter
   path append    BFlav_D0_K3Pi_Path    BFlav_D0_K3Pi_PreFilter
   talkto    BFlav_D0_K3Pi_PreFilter {
       orList set B0ToDstarPi_K3Pi
       orList set B0ToDstarRhoC_K3Pi
       orList set B0ToDstarA1C_K3Pi
       orList set BchToD0Pi
       orList set BchToDstar0Pi
   }

   sourceFoundFile CompositionSequences/CompBFlavTrkPIDSequence.tcl
   path append    BFlav_D0_K3Pi_Path    BFlavTrkPIDSequence

   sourceFoundFile CompositionSequences/CompBFlavDSequences.tcl
   path append    BFlav_D0_K3Pi_Path    D0ToK3Pi_Vtx_BFlav
   path append    BFlav_D0_K3Pi_Path    D0ToK3Pi_BFlav

} else {

   echo === Not running BFlav_D0_K3Pi_Path, which needs Micro access.  ORL

   path disable BFlav_D0_K3Pi_Path

}

}




