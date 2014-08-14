#
# $Id: BchToEtacKchFinalPath.tcl,v 1.7 2006/03/21 22:18:45 fwinkl Exp $
#
# defines B0ToccKFinalPath tag filter path
#
#  O.Long, UCSB: June, 2002
#

   global tagSkim
   global BtaCandLists
   global writeUsrDataForSkim

   path create BchToEtacKchFinalPath

   # only run on MultiHadrons
   path append BchToEtacKchFinalPath FilterBGFMultiHadron

   # enable required tag modules
   sourceFoundFile EventTagTools/TagProcs.tcl
   addTagModule TagEtaCExclusive BchToEtacKchFinalPath

   # clone and customize filter module
   mod clone TagFilterByName BchToEtacKchFinalPreFilter
   path append BchToEtacKchFinalPath BchToEtacKchFinalPreFilter
   talkto BchToEtacKchFinalPreFilter {

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
      path append BchToEtacKchFinalPath EtacKch
      path append BchToEtacKchFinalPath EtacKchFinalSequence 

   } else {

      echo === Not running micro analysis for BchToEtacKchFinalFilter.  ORL

   }


   mod clone TagFilterByName BchToEtacKchFinalFilter
   path append BchToEtacKchFinalPath BchToEtacKchFinalFilter
   talkto BchToEtacKchFinalFilter {
        orList set EtacKchFinal
   }


# Add lists of composites to write
lappend BtaCandLists BchToEtacKchFinal KsDefault KNNVeryLoose KLHNotPion

# (don't) Write UsrData
set writeUsrDataForSkim 0
#configureUsrData B0ToccKFinal EventData BasicTestEventData write
