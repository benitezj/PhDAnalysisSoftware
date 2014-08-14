#
# $Id: B0ToccKLoosePath.tcl,v 1.9 2005/11/16 23:01:54 fwinkl Exp $
#
# defines B0ToccKLoosePath tag filter path
#
#  O.Long, UCSB: February, 2003
#

   path create B0ToccKLoosePath

   # only run on MultiHadrons
   path append B0ToccKLoosePath FilterBGFMultiHadron

   # make sure tag bits get created
   sourceFoundFile EventTagTools/TagProcs.tcl
   useTagFromList JpsiKs     B0ToccKLoosePath
   useTagFromList JpsiKl     B0ToccKLoosePath
   useTagFromList JpsiKs2Pi0 B0ToccKLoosePath
   useTagFromList Psi2SKs    B0ToccKLoosePath
   useTagFromList ChicKs     B0ToccKLoosePath

   # clone and customize filter module
   mod clone TagFilterByName B0ToccKLooseFilter
   path append B0ToccKLoosePath B0ToccKLooseFilter
   talkto B0ToccKLooseFilter {
        orList set JpsiKs
        orList set JpsiKs2Pi0
        orList set Psi2SKs
        orList set ChicKs
        orList set JpsiKl
   }


# Add lists of composites to write
global BtaCandLists
lappend BtaCandLists B0JpsiKs B0JpsiPi0 B0ToJPsiKsToPi0Pi0Loose BchJpsiK
lappend BtaCandLists B0JpsiKstarVeryLoose BchJpsiKstarVeryLoose
lappend BtaCandLists B0JpsiKlLoose  B0Psi2SKs BchPsi2SK B0ChicKs
lappend BtaCandLists BchChicK BchChicKstar B0ChicKstar
