#
# $Id: BchToccKstarLoosePath.tcl,v 1.6 2005/11/16 23:01:57 fwinkl Exp $
#
# defines BchToccKstarLoosePath tag filter path
#
#  O.Long, UCSB: February, 2003
#

   path create BchToccKstarLoosePath

   # only run on MultiHadrons
   path append BchToccKstarLoosePath FilterBGFMultiHadron

   # create tag bit
   sourceFoundFile EventTagTools/TagProcs.tcl
   useTagFromList JpsiKstarp BchToccKstarLoosePath

   # clone and customize filter module
   mod clone TagFilterByName BchToccKstarLooseFilter
   path append BchToccKstarLoosePath BchToccKstarLooseFilter
   talkto BchToccKstarLooseFilter {
        orList set JpsiKstarp
   }

# Add lists of composites to write
global BtaCandLists
lappend BtaCandLists B0JpsiKs B0JpsiPi0 B0ToJPsiKsToPi0Pi0Loose BchJpsiK
lappend BtaCandLists B0JpsiKstarVeryLoose  BchJpsiKstarVeryLoose
lappend BtaCandLists B0JpsiKlLoose  B0Psi2SKs BchPsi2SK B0ChicKs
lappend BtaCandLists BchChicK BchChicKstar B0ChicKstar
