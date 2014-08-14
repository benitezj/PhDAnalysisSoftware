#
#  Owen Long, UC Riverside
#  March, 2007
#
#    $Id: BToPhi3KPath.tcl,v 1.1 2007/06/01 21:41:43 owen Exp $
#
#    Skim for B -> phi K K K
#
#    Includes the following final states:
#
#       B+ -> phi K+ K- K+ ; phi -> K+ K-  , tagbit BpToPhi3Kpm_PhitoKpKm
#       B+ -> phi K+ K- K+ ; phi -> Ks Kl  , tagbit BpToPhi3Kpm_PhitoKsKl
#       B+ -> phi K+ Ks Kl ; phi -> K+ K-  , tagbit BpToPhiKpKsKl
#       B0 -> phi K+ K- Ks ; phi -> K+ K-  , tagbit B0ToPhiKpKmKs_PhitoKpKm
#       B0 -> phi K+ K- Kl ; phi -> K+ K-  , tagbit B0ToPhiKpKmKl
#       B0 -> phi K+ K- Ks ; phi -> Ks Kl  , tagbit B0ToPhiKpKmKs_PhitoKsKl
#       B0 -> phi Ks Ks Kl ; phi -> K+ K-  , tagbit B0ToPhiKsKsKl
#
#
#    Note: some of the selectors below REQUIRE an updated version of
#      the KlongDeltaE SimpleComposition tools selector.  The updates are in
#      the following versions of the source code:
#
#         SimpleComposition/SmpKlongDeltaESelection.cc   , revision 1.5
#         SimpleComposition/SmpKlongDeltaESelection.hh   , revision 1.4
#
#    If you get errors, please check your SimpleComposition tag.
#


  #--- Choose lists

 # set KpmForPhiList    ChargedTracks
   set KpmForPhiList    KLHNotPion

 # set KpmNotForPhiList ChargedTracks
   set KpmNotForPhiList KLHNotPion





   path create BToPhi3KPath

   path append BToPhi3KPath FilterBGFMultiHadron

  #############################

  #--- Set up resonances involving Klongs


  #--- Merge EMC and IFR Klong lists

   global Combined_Klong_List_defined
   if [ expr ! [ info exists Combined_Klong_List_defined ] ] {

      set Combined_Klong_List_defined 1

      mod clone SmpMergerDefiner KlongEmcAndIfrTight
      talkto KlongEmcAndIfrTight {
         inputListNames set KlongEmcTight
         inputListNames set KlongIfrTight
      }

   }
   path append BToPhi3KPath KlongEmcAndIfrTight



  #--- phi -> Ks Kl

   global PhiToKsKl_nokin_defined
   if [ expr ! [ info exists PhiToKsKl_nokin_defined ] ] {

      set PhiToKsKl_nokin_defined 1

      mod clone SmpMakerDefiner PhiToKsKl_NOKLKINEMATICS
      talkto PhiToKsKl_NOKLKINEMATICS {
         decayMode set "phi -> K_S0 K_L0"
         daughterListNames set "KsDefault"
         daughterListNames set "KlongEmcAndIfrTight"
         fittingAlgorithm  set "Add4"
      }

   }
   path append BToPhi3KPath PhiToKsKl_NOKLKINEMATICS






  #--- Set up resonances not involving Klongs

   global PhiToKpKm_for_phi3k_and_phi2pi1k_defined
   if [ expr ! [ info exists PhiToKpKm_for_phi3k_and_phi2pi1k_defined ] ] {

      set PhiToKpKm_for_phi3k_and_phi2pi1k_defined 1

      mod clone SmpMakerDefiner PhiToKpKm_for_phi3k_and_phi2pi1k
      talkto PhiToKpKm_for_phi3k_and_phi2pi1k {
         decayMode set "phi -> K+ K-"
         daughterListNames set $KpmForPhiList
         daughterListNames set $KpmForPhiList
         fittingAlgorithm  set "Add4"
      }

   }

   path append BToPhi3KPath PhiToKpKm_for_phi3k_and_phi2pi1k




  #----- set up B decay selectors

   mod clone SmpMakerDefiner BpToPhi3Kpm
   path append BToPhi3KPath BpToPhi3Kpm
   talkto BpToPhi3Kpm {
      decayMode set "B+ -> phi K+ K- K+"
      daughterListNames set "PhiToKpKm_for_phi3k_and_phi2pi1k"
      daughterListNames set $KpmNotForPhiList
      daughterListNames set $KpmNotForPhiList
      daughterListNames set $KpmNotForPhiList
      createUsrData     set true
      fittingAlgorithm  set "Add4"
      preFitSelectors   set "DeltaE -0.32:0.32"
      preFitSelectors   set "Mes 5.18:5.32"
      postFitSelectors  set "DeltaE -0.32:0.32"
      postFitSelectors  set "Mes 5.18:5.32"
   }


   mod clone SmpMakerDefiner BpToPhi3Kpm_PhitoKsKl
   path append BToPhi3KPath BpToPhi3Kpm_PhitoKsKl
   talkto BpToPhi3Kpm_PhitoKsKl {
      decayMode set "B+ -> phi K+ K- K+"
      daughterListNames set "PhiToKsKl_NOKLKINEMATICS"
      daughterListNames set $KpmNotForPhiList
      daughterListNames set $KpmNotForPhiList
      daughterListNames set $KpmNotForPhiList
      postFitSelectors  set "KlongDeltaE -0.03:0.10"
      createUsrData     set true
   }


   mod clone SmpMakerDefiner BpToPhiKpKsKl
   path append BToPhi3KPath BpToPhiKpKsKl
   talkto BpToPhiKpKsKl {
      decayMode set "B+ -> phi K+ K_S0 K_L0"
      daughterListNames set "PhiToKpKm_for_phi3k_and_phi2pi1k"
      daughterListNames set $KpmNotForPhiList
      daughterListNames set "KsDefault"
      daughterListNames set "KlongEmcAndIfrTight"
      postFitSelectors  set "KlongDeltaE -0.03:0.10"
      createUsrData     set true
   }




   mod clone SmpMakerDefiner B0ToPhiKpKmKs
   path append BToPhi3KPath B0ToPhiKpKmKs
   talkto B0ToPhiKpKmKs {
      decayMode set "B0 -> phi K+ K- K_S0"
      daughterListNames set "PhiToKpKm_for_phi3k_and_phi2pi1k"
      daughterListNames set $KpmNotForPhiList
      daughterListNames set $KpmNotForPhiList
      daughterListNames set "KsDefault"
      createUsrData     set true
      fittingAlgorithm  set "Add4"
      preFitSelectors   set "DeltaE -0.32:0.32"
      preFitSelectors   set "Mes 5.18:5.32"
      postFitSelectors  set "DeltaE -0.32:0.32"
      postFitSelectors  set "Mes 5.18:5.32"
   }


   mod clone SmpMakerDefiner B0ToPhiKpKmKs_PhitoKsKl
   path append BToPhi3KPath B0ToPhiKpKmKs_PhitoKsKl
   talkto B0ToPhiKpKmKs_PhitoKsKl {
      decayMode set "B0 -> phi K+ K- K_S0"
      daughterListNames set "PhiToKsKl_NOKLKINEMATICS"
      daughterListNames set $KpmNotForPhiList
      daughterListNames set $KpmNotForPhiList
      daughterListNames set "KsDefault"
      postFitSelectors  set "KlongDeltaE -0.03:0.10"
      createUsrData     set true
   }



   mod clone SmpMakerDefiner B0ToPhiKpKmKl
   path append BToPhi3KPath B0ToPhiKpKmKl
   talkto B0ToPhiKpKmKl {
      decayMode set "B0 -> phi K+ K- K_L0"
      daughterListNames set "PhiToKpKm_for_phi3k_and_phi2pi1k"
      daughterListNames set $KpmNotForPhiList
      daughterListNames set $KpmNotForPhiList
      daughterListNames set "KlongEmcAndIfrTight"
      postFitSelectors  set "KlongDeltaE -0.03:0.10"
      createUsrData     set true
   }


   mod clone SmpMakerDefiner B0ToPhiKsKsKl
   path append BToPhi3KPath B0ToPhiKsKsKl
   talkto B0ToPhiKsKsKl {
      decayMode set "B0 -> phi K_S0 K_S0 K_L0"
      daughterListNames set "PhiToKpKm_for_phi3k_and_phi2pi1k"
      daughterListNames set "KsDefault"
      daughterListNames set "KsDefault"
      daughterListNames set "KlongEmcAndIfrTight"
      postFitSelectors  set "KlongDeltaE -0.03:0.10"
      createUsrData     set true
   }


########################


   mod clone TagFromList TagBpToPhi3Kpm_PhitoKpKm
   path append BToPhi3KPath TagBpToPhi3Kpm_PhitoKpKm
   talkto TagBpToPhi3Kpm_PhitoKpKm {
      listsToTag set BpToPhi3Kpm
      tagName set BpToPhi3Kpm_PhitoKpKm
      production set true
      verbose set false
   }

   mod clone TagFromList TagBpToPhi3Kpm_PhitoKsKl
   path append BToPhi3KPath TagBpToPhi3Kpm_PhitoKsKl
   talkto TagBpToPhi3Kpm_PhitoKsKl {
      listsToTag set BpToPhi3Kpm_PhitoKsKl
      tagName set BpToPhi3Kpm_PhitoKsKl
      production set true
      verbose set false
   }

   mod clone TagFromList TagBpToPhiKpKsKl
   path append BToPhi3KPath TagBpToPhiKpKsKl
   talkto TagBpToPhiKpKsKl {
      listsToTag set BpToPhiKpKsKl
      tagName set BpToPhiKpKsKl
      production set true
      verbose set false
   }


   mod clone TagFromList TagB0ToPhiKpKmKs_PhitoKpKm
   path append BToPhi3KPath TagB0ToPhiKpKmKs_PhitoKpKm
   talkto TagB0ToPhiKpKmKs_PhitoKpKm {
      listsToTag set B0ToPhiKpKmKs
      tagName set B0ToPhiKpKmKs_PhitoKpKm
      production set true
      verbose set false
   }

   mod clone TagFromList TagB0ToPhiKpKmKs_PhitoKsKl
   path append BToPhi3KPath TagB0ToPhiKpKmKs_PhitoKsKl
   talkto TagB0ToPhiKpKmKs_PhitoKsKl {
      listsToTag set B0ToPhiKpKmKs_PhitoKsKl
      tagName set B0ToPhiKpKmKs_PhitoKsKl
      production set true
      verbose set false
   }


   mod clone TagFromList TagB0ToPhiKpKmKl
   path append BToPhi3KPath TagB0ToPhiKpKmKl
   talkto TagB0ToPhiKpKmKl {
      listsToTag set B0ToPhiKpKmKl
      tagName set B0ToPhiKpKmKl
      production set true
      verbose set false
   }

   mod clone TagFromList TagB0ToPhiKsKsKl
   path append BToPhi3KPath TagB0ToPhiKsKsKl
   talkto TagB0ToPhiKsKsKl {
      listsToTag set B0ToPhiKsKsKl
      tagName set B0ToPhiKsKsKl
      production set true
      verbose set false
   }

   mod clone TagFilterByName BToPhi3KFinalFilter
   path append BToPhi3KPath BToPhi3KFinalFilter
   talkto BToPhi3KFinalFilter {
      orList set BpToPhi3Kpm_PhitoKpKm
      orList set BpToPhi3Kpm_PhitoKsKl
      orList set BpToPhiKpKsKl
      orList set B0ToPhiKpKmKs_PhitoKpKm
      orList set B0ToPhiKpKmKs_PhitoKsKl
      orList set B0ToPhiKpKmKl
      orList set B0ToPhiKsKsKl
      production set true
      verbose set false
   }

########################

 # Save these composites.
###global BtaCandLists
###lappend BtaCandLists BpToPhi3Kpm BpToPhi3Kpm_PhitoKsKl B0ToPhiKpKmKs B0ToPhiKpKmKs_PhitoKsKl B0ToPhiKpKmKl



