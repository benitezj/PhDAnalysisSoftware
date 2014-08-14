#
# $Id: B0ToccKstarFinalPath.tcl,v 1.15 2006/02/03 18:40:30 fwinkl Exp $
#
# defines B0ToccKstarFinalPath tag filter path
#
#  O.Long, UCSB: June, 2002
#
global tagSkim
global BtaCandLists

   path create B0ToccKstarFinalPath

   # only run on MultiHadrons
   path append B0ToccKstarFinalPath FilterBGFMultiHadron

   # create tag bit
   sourceFoundFile EventTagTools/TagProcs.tcl
   useTagFromList JpsiKstar0 B0ToccKstarFinalPath

   # clone and customize filter module
   mod clone TagFilterByName B0ToccKstarFinalPreFilter
   path append B0ToccKstarFinalPath B0ToccKstarFinalPreFilter
   talkto B0ToccKstarFinalPreFilter {
        orList set JpsiKstar0
   }

##---ORL: This logic is necessary because I want to do micro
#         level analysis in this path.

   if [ expr ! [ info exists tagSkim ] ] {

      sourceFoundFile CompositionSequences/CompPsiFinalProdSequence.tcl
      path append B0ToccKstarFinalPath CompB0ToccKstarFinalProdSequence

   } else {

      echo === Not running micro analysis for B0ToccKstarFinalFilter.  ORL

   }

   # those tag bits are defined in CompositionSequences/CompJPsiKFinalSequence.tcl
   mod clone TagFilterByName B0ToccKstarFinalFilter
   path append B0ToccKstarFinalPath B0ToccKstarFinalFilter
   talkto B0ToccKstarFinalFilter {
        orList set JpsiKstar0ksFinal
        orList set JpsiKstar0kpFinal
   }


# Add lists of composites to write
lappend BtaCandLists B0JpsiKs B0JpsiPi0 B0ToJPsiKsToPi0Pi0Loose BchJpsiK
lappend BtaCandLists B0JpsiKstarVeryLoose  BchJpsiKstarVeryLoose
lappend BtaCandLists B0JpsiKlLoose  B0Psi2SKs BchPsi2SK B0ChicKs
lappend BtaCandLists BchChicK BchChicKstar B0ChicKstar
