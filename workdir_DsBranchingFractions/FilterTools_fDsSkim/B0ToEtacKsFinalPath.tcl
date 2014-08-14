#
# $Id: B0ToEtacKsFinalPath.tcl,v 1.7 2006/03/21 22:18:44 fwinkl Exp $
#
# defines B0ToccKFinalPath tag filter path
#
#  O.Long, UCSB: June, 2002
#

   global tagSkim
   global BtaCandLists
   global writeUsrDataForSkim

   path create B0ToEtacKsFinalPath

   # only run on MultiHadrons
   path append B0ToEtacKsFinalPath FilterBGFMultiHadron

   # enable required tag modules
   sourceFoundFile EventTagTools/TagProcs.tcl
   addTagModule TagEtaCExclusive B0ToEtacKsFinalPath

   # clone and customize filter module
   mod clone TagFilterByName B0ToEtacKsFinalPreFilter
   path append B0ToEtacKsFinalPath B0ToEtacKsFinalPreFilter
   talkto B0ToEtacKsFinalPreFilter {

	     # TagEtaCExclusive only sets this tag bit if the event passes some internal criterias
	     assertIfMissing set false

#        orList set JpsiKs
#        orList set JpsiKl
#        orList set JpsiKs2Pi0
#        orList set Psi2SKs
        orList set ExclEtaCToKsKPi
   }

##---ORL: This logic is necessary because I want to do micro
#         level analysis in this path.

   if [ expr ! [ info exists tagSkim ] ] {

      sourceFoundFile CompositionSequences/CompEtacKSequence.tcl
      path append B0ToEtacKsFinalPath EtacKs
      path append B0ToEtacKsFinalPath EtacKsFinalSequence 

   } else {

      echo === Not running micro analysis for B0ToEtacKsFinalFilter.  ORL

   }


   mod clone TagFilterByName B0ToEtacKsFinalFilter
   path append B0ToEtacKsFinalPath B0ToEtacKsFinalFilter
   talkto B0ToEtacKsFinalFilter {
        orList set EtacKsFinal
   }

# Add lists of composites to write
lappend BtaCandLists B0ToEtacKsFinal KsDefault KNNVeryLoose KLHNotPion

# (don't) Write UsrData
set writeUsrDataForSkim 0
