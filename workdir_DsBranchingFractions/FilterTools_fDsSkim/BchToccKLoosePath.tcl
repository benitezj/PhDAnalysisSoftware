#
# $Id: BchToccKLoosePath.tcl,v 1.6 2005/11/16 23:01:57 fwinkl Exp $
#
# defines BchToccKLoosePath tag filter path
#
#  O.Long, UCSB: February, 2003
#

   path create BchToccKLoosePath

   # only run on MultiHadrons
   path append BchToccKLoosePath FilterBGFMultiHadron

   # make sure tag bits get created
   sourceFoundFile EventTagTools/TagProcs.tcl
   useTagFromList JpsiKp     BchToccKLoosePath
   useTagFromList Psi2SKp    BchToccKLoosePath
   useTagFromList ChicKp     BchToccKLoosePath

   # clone and customize filter module
   mod clone TagFilterByName BchToccKLooseFilter
   path append BchToccKLoosePath BchToccKLooseFilter
   talkto BchToccKLooseFilter {
        orList set JpsiKp
        orList set Psi2SKp
        orList set ChicKp
   }

# Add lists of composites to write
global BtaCandLists
lappend BtaCandLists B0JpsiKs B0JpsiPi0 B0ToJPsiKsToPi0Pi0Loose BchJpsiK
lappend BtaCandLists B0JpsiKstarVeryLoose  BchJpsiKstarVeryLoose
lappend BtaCandLists B0JpsiKlLoose  B0Psi2SKs BchPsi2SK B0ChicKs
lappend BtaCandLists BchChicK BchChicKstar B0ChicKstar
