#
#  Owen Long, UC Riverside
#  February, 2005
#
#    $Id: BToKsKsKlDefaultPath.tcl,v 1.5 2007/10/26 11:23:53 marchior Exp $
#
#    Skim for B0->Ks Ks Kl
#
#    Prerequisite is the inclusive B->KsKsK tagbit (KsKsKIncl)
#
#

   path create BToKsKsKlDefaultPath

   addTagModule TagKsKsKIncl BToKsKsKlDefaultPath

   mod clone TagFilterByName BToKsKsKlDefaultPreFilter
   path append BToKsKsKlDefaultPath BToKsKsKlDefaultPreFilter
   talkto BToKsKsKlDefaultPreFilter {
      andList set KsKsKIncl
      production set true
      verbose set false
   }

   mod clone SmpMakerDefiner  BToKsKsKlEmcDefault
   path append BToKsKsKlDefaultPath BToKsKsKlEmcDefault
   talkto BToKsKsKlEmcDefault {
      decayMode set  "B0 -> K_S0 K_S0 K_L0"
      daughterListNames set "KsDefault"
      daughterListNames set "KsDefault"
      daughterListNames set "KlongEmcTight"
      postFitSelectors set "KlongDeltaE -0.03:0.10"
      createUsrData set true
   }

   mod clone SmpMakerDefiner  BToKsKsKlIfrDefault
   path append BToKsKsKlDefaultPath BToKsKsKlIfrDefault
   talkto BToKsKsKlIfrDefault {
      decayMode set  "B0 -> K_S0 K_S0 K_L0"
      daughterListNames set "KsDefault"
      daughterListNames set "KsDefault"
      daughterListNames set "KlongIfrTight"
      postFitSelectors set "KlongDeltaE -0.03:0.10"
      createUsrData set true
   }

   mod clone TagFromList TagBToKsKsKlDefault
   path append BToKsKsKlDefaultPath TagBToKsKsKlDefault
   talkto TagBToKsKsKlDefault {
      listsToTag set BToKsKsKlEmcDefault
      listsToTag set BToKsKsKlIfrDefault
      tagName set BToKsKsKlDefault
      production set true
      verbose set false
   }


   mod clone TagFilterByName BToKsKsKlDefaultFinalFilter
   path append BToKsKsKlDefaultPath BToKsKsKlDefaultFinalFilter
   talkto BToKsKsKlDefaultFinalFilter {
      andList set BToKsKsKlDefault
      production set true
      verbose set false
   }


 # Save these composites.
   global BtaCandLists
   lappend BtaCandLists BToKsKsKlEmcDefault BToKsKsKlIfrDefault
