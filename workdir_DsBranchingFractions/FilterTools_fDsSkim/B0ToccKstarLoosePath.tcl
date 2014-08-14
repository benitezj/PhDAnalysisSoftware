#
# $Id: B0ToccKstarLoosePath.tcl,v 1.6 2005/11/16 23:01:55 fwinkl Exp $
#
# defines B0ToccKstarLoosePath tag filter path
#
#  O.Long, UCSB: February, 2003
#

   path create B0ToccKstarLoosePath

   # only run on MultiHadrons
   path append B0ToccKstarLoosePath FilterBGFMultiHadron

   # create tag bit
   sourceFoundFile EventTagTools/TagProcs.tcl
   useTagFromList JpsiKstar0 B0ToccKstarLoosePath

   # clone and customize filter module
   mod clone TagFilterByName B0ToccKstarLooseFilter
   path append B0ToccKstarLoosePath B0ToccKstarLooseFilter
   talkto B0ToccKstarLooseFilter {
        orList set JpsiKstar0
   }

# Add lists of composites to write
global BtaCandLists
lappend BtaCandLists B0JpsiKs B0JpsiPi0 B0ToJPsiKsToPi0Pi0Loose BchJpsiK
lappend BtaCandLists B0JpsiKstarVeryLoose  BchJpsiKstarVeryLoose
lappend BtaCandLists B0JpsiKlLoose  B0Psi2SKs BchPsi2SK B0ChicKs
lappend BtaCandLists BchChicK BchChicKstar B0ChicKstar
