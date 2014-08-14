#
# $Id: BchToccKstarFinalPath.tcl,v 1.16 2006/02/03 18:43:39 fwinkl Exp $
#
# defines BchToccKstarFinalPath tag filter path
#
#  O.Long, UCSB: June, 2002
#
global tagSkim
global BtaCandLists

   path create BchToccKstarFinalPath

   # only run on MultiHadrons
   path append BchToccKstarFinalPath FilterBGFMultiHadron

   # create tag bit
   sourceFoundFile EventTagTools/TagProcs.tcl
   useTagFromList JpsiKstarp BchToccKstarFinalPath

   # clone and customize filter module
   mod clone TagFilterByName BchToccKstarFinalPreFilter
   path append BchToccKstarFinalPath BchToccKstarFinalPreFilter
   talkto BchToccKstarFinalPreFilter {
        orList set JpsiKstarp
   }

##---ORL: This logic is necessary because I want to do micro
#         level analysis in this path.

   if [ expr ! [ info exists tagSkim ] ] {

      sourceFoundFile CompositionSequences/CompPsiFinalProdSequence.tcl
      path append BchToccKstarFinalPath CompBchToccKstarFinalProdSequence


   } else {

      echo === Not running micro analysis for BchToccKstarFinalPath.  ORL

   }

   # those tag bits are defined in CompositionSequences/CompJPsiKFinalSequence.tcl
   mod clone TagFilterByName BchToccKstarFinalFilter
   path append BchToccKstarFinalPath BchToccKstarFinalFilter
   talkto BchToccKstarFinalFilter {
        orList set JpsiKstarpksFinal
        orList set JpsiKstarpkpFinal
   }


# Add lists of composites to write
lappend BtaCandLists B0JpsiKs B0JpsiPi0 B0ToJPsiKsToPi0Pi0Loose BchJpsiK
lappend BtaCandLists B0JpsiKstarVeryLoose  BchJpsiKstarVeryLoose
lappend BtaCandLists B0JpsiKlLoose  B0Psi2SKs BchPsi2SK B0ChicKs
lappend BtaCandLists BchChicK BchChicKstar B0ChicKstar
