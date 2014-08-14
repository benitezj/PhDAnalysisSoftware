#
# $Id: BchToccKFinalPath.tcl,v 1.15 2005/11/16 23:01:57 fwinkl Exp $
#
# defines BchToccKFinalPath tag filter path
#
#  O.Long, UCSB: June, 2002
#
global tagSkim
global BtaCandLists

   path create BchToccKFinalPath

   # only run on MultiHadrons
   path append BchToccKFinalPath FilterBGFMultiHadron

   # make sure tag bits get created
   sourceFoundFile EventTagTools/TagProcs.tcl
   useTagFromList JpsiKp     BchToccKFinalPath
   useTagFromList Psi2SKp    BchToccKFinalPath
   useTagFromList ChicKp     BchToccKFinalPath

   # clone and customize filter module
   mod clone TagFilterByName BchToccKFinalPreFilter
   path append BchToccKFinalPath BchToccKFinalPreFilter
   talkto BchToccKFinalPreFilter {
        orList set JpsiKp
        orList set Psi2SKp
        orList set ChicKp
   }

##---ORL: This logic is necessary because I want to do micro
#         level analysis in this path.

   if [ expr ! [ info exists tagSkim ] ] {

      sourceFoundFile CompositionSequences/CompPsiFinalProdSequence.tcl
      path append BchToccKFinalPath CompBchToccKFinalProdSequence

   } else {

      echo === Not running micro analysis for BchToccKFinalPath.  ORL

   }

   mod clone TagFilterByName BchToccKFinalFilter
   path append BchToccKFinalPath BchToccKFinalFilter
   talkto BchToccKFinalFilter {
        orList set JpsiKpFinal
        orList set ChicKpFinal
        orList set Psi2SKpFinal
   }


# Add lists of composites to write
lappend BtaCandLists B0JpsiKs B0JpsiPi0 B0ToJPsiKsToPi0Pi0Loose BchJpsiK
lappend BtaCandLists B0JpsiKstarVeryLoose  BchJpsiKstarVeryLoose
lappend BtaCandLists B0JpsiKlLoose  B0Psi2SKs BchPsi2SK B0ChicKs
lappend BtaCandLists BchChicK BchChicKstar B0ChicKstar
