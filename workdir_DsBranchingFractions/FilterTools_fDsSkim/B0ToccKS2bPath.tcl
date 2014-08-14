#
# $Id: B0ToccKS2bPath.tcl,v 1.2 2005/11/16 23:01:55 fwinkl Exp $
#
# defines B0ToccKS2bPath tag filter path
#
#  O.Long, UCSB: June, 2002
#

   global tagSkim
   global BtaCandLists
   global writeUsrDataForSkim

   path create B0ToccKS2bPath

   # only run on MultiHadrons
   path append B0ToccKS2bPath FilterBGFMultiHadron

   # make sure tag bits get created
   sourceFoundFile EventTagTools/TagProcs.tcl
   useTagFromList JpsiKs     B0ToccKS2bPath
   useTagFromList JpsiKl     B0ToccKS2bPath
   useTagFromList JpsiKs2Pi0 B0ToccKS2bPath
   useTagFromList Psi2SKs    B0ToccKS2bPath
   useTagFromList ChicKs     B0ToccKS2bPath

   # clone and customize filter module
   mod clone TagFilterByName B0ToccKS2bPreFilter
   path append B0ToccKS2bPath B0ToccKS2bPreFilter
   talkto B0ToccKS2bPreFilter {
        orList set JpsiKs
        orList set JpsiKl
        orList set JpsiKs2Pi0
        orList set Psi2SKs
        orList set ChicKs
   }

##---ORL: This logic is necessary because I want to do micro
#         level analysis in this path.

   if [ expr ! [ info exists tagSkim ] ] {

      sourceFoundFile CompositionSequences/CompPsiS2bProdSequence.tcl
      path append B0ToccKS2bPath CompB0ToccKS2bProdSequence

   } else {

      echo === Not running micro analysis for B0ToccKS2bFilter.  ORL

   }


   mod clone TagFilterByName B0ToccKS2bFilter
   path append B0ToccKS2bPath B0ToccKS2bFilter
   talkto B0ToccKS2bFilter {
        orList set JpsiKsS2b
        orList set JpsiKlS2b
        orList set JpsiKlS2bSB
        orList set JpsiKs2Pi0S2b
        orList set ChicKsS2b
        orList set Psi2SKsS2b
   }



# (don't) Write UsrData
set writeUsrDataForSkim 0
