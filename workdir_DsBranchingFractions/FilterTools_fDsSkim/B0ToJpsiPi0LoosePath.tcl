#
# $Id: B0ToJpsiPi0LoosePath.tcl,v 1.3 2005/11/16 23:01:54 fwinkl Exp $
#
# defines B0ToJpsiPi0LoosePath tag filter path
#
#  K.George, University of Liverpool. September 2004. 
#  Adapted from B0ToccKLoosePath (O.Long, UCSB: February, 2003)
#
   path create B0ToJpsiPi0LoosePath

   # only run on MultiHadrons
   path append B0ToJpsiPi0LoosePath FilterBGFMultiHadron

   # create tag bit
   sourceFoundFile EventTagTools/TagProcs.tcl
   useTagFromList JpsiPi0 B0ToJpsiPi0LoosePath

   # clone and customize filter module
   mod clone TagFilterByName B0ToJpsiPi0LooseFilter
   path append B0ToJpsiPi0LoosePath B0ToJpsiPi0LooseFilter
   talkto B0ToJpsiPi0LooseFilter {
        orList set JpsiPi0
   }

# Add lists of composites to write
global BtaCandLists
lappend BtaCandLists B0JpsiKs B0JpsiPi0 B0ToJPsiKsToPi0Pi0Loose BchJpsiK
lappend BtaCandLists B0JpsiKstarVeryLoose  BchJpsiKstarVeryLoose
lappend BtaCandLists B0JpsiKlLoose  B0Psi2SKs BchPsi2SK B0ChicKs
lappend BtaCandLists BchChicK BchChicKstar B0ChicKstar
lappend BtaCandLists JPsiDefaultMass pi0LooseMass
