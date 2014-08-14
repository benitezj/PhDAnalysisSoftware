#
# $Id: B0ToccKFinalPath.tcl,v 1.21 2005/11/16 23:01:54 fwinkl Exp $
#
# defines B0ToccKFinalPath tag filter path
#
#  O.Long, UCSB: June, 2002
#

   global tagSkim
   global BtaCandLists
   global writeUsrDataForSkim

   path create B0ToccKFinalPath

   # only run on MultiHadrons
   path append B0ToccKFinalPath FilterBGFMultiHadron

   # make sure tag bits get created
   sourceFoundFile EventTagTools/TagProcs.tcl
   useTagFromList JpsiKs     B0ToccKFinalPath
   useTagFromList JpsiKl     B0ToccKFinalPath
   useTagFromList JpsiKs2Pi0 B0ToccKFinalPath
   useTagFromList Psi2SKs    B0ToccKFinalPath
   useTagFromList ChicKs     B0ToccKFinalPath

   # clone and customize filter module
   mod clone TagFilterByName B0ToccKFinalPreFilter
   path append B0ToccKFinalPath B0ToccKFinalPreFilter
   talkto B0ToccKFinalPreFilter {
        orList set JpsiKs
        orList set JpsiKl
        orList set JpsiKs2Pi0
        orList set Psi2SKs
        orList set ChicKs
   }

##---ORL: This logic is necessary because I want to do micro
#         level analysis in this path.

   if [ expr ! [ info exists tagSkim ] ] {

      sourceFoundFile CompositionSequences/CompPsiFinalProdSequence.tcl
      path append B0ToccKFinalPath CompB0ToccKFinalProdSequence

   } else {

      echo === Not running micro analysis for B0ToccKFinalFilter.  ORL

   }


   mod clone TagFilterByName B0ToccKFinalFilter
   path append B0ToccKFinalPath B0ToccKFinalFilter
   talkto B0ToccKFinalFilter {
        orList set JpsiKsFinal
        orList set JpsiKlFinal
        orList set JpsiKlFinalSB
        orList set JpsiKs2Pi0Final
        orList set ChicKsFinal
        orList set Psi2SKsFinal
   }


# Add lists of composites to write
lappend BtaCandLists B0JpsiKs B0JpsiPi0 B0ToJPsiKsToPi0Pi0Loose BchJpsiK
lappend BtaCandLists B0JpsiKstarVeryLoose BchJpsiKstarVeryLoose
lappend BtaCandLists B0JpsiKlLoose  B0Psi2SKs BchPsi2SK B0ChicKs
lappend BtaCandLists BchChicK BchChicKstar B0ChicKstar

# (don't) Write UsrData
set writeUsrDataForSkim 0
