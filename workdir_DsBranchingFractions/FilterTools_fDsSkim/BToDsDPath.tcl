#
#  Owen Long, UC Riverside
#  March, 2007
#
#    $Id: BToDsDPath.tcl,v 1.3 2007/06/01 21:41:42 owen Exp $
#
#    Skim for B -> Ds D ;  Ds -> phi pi ;  D -> K pi
#       where phi -> K+ K-  or  phi -> Ks Kl
#       and   D -> K- pi+   or  D -> Ks pi+   or  D -> Kl pi+
#
#    Includes the following final states:
#
#       B+ -> Ds+ D0bar ;  Ds+ -> phi pi+ ; D0bar -> K+ pi- ; phi -> K+ K-
#       B+ -> Ds+ D0bar ;  Ds+ -> phi pi+ ; D0bar -> K+ pi- ; phi -> Ks Kl
#       B0 -> Ds+ D-    ;  Ds+ -> phi pi+ ; D-    -> Ks pi- ; phi -> K+ K-
#       B0 -> Ds+ D-    ;  Ds+ -> phi pi+ ; D-    -> Ks pi- ; phi -> Ks Kl
#       B0 -> Ds+ D-    ;  Ds+ -> phi pi+ ; D-    -> Kl pi- ; phi -> K+ K-
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
#


  #--- Choose lists

 # set KpmForPhiList    ChargedTracks
   set KpmForPhiList    KLHNotPion

 # set KpmNotForPhiList ChargedTracks
   set KpmNotForPhiList KLHNotPion

   set piList   ChargedTracks




   path create BToDsDPath

   path append BToDsDPath FilterBGFMultiHadron


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
   path append BToDsDPath KlongEmcAndIfrTight



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
   path append BToDsDPath PhiToKsKl_NOKLKINEMATICS





   #---- D+ -> Kl pi+

   mod clone SmpMakerDefiner DpmToKlPi_NOKLKINEMATICS
   path append BToDsDPath DpmToKlPi_NOKLKINEMATICS
   talkto DpmToKlPi_NOKLKINEMATICS {
      decayMode set "D+ -> K_L0 pi+"
      daughterListNames set "KlongEmcAndIfrTight"
      daughterListNames set $piList
      fittingAlgorithm  set "Add4"
   }




   #---- Ds+ -> phi pi+ ;  phi -> Ks Kl

   mod clone SmpMakerDefiner DsToPhiPi_PhiToKsKl_NOKLKINEMATICS
   path append BToDsDPath DsToPhiPi_PhiToKsKl_NOKLKINEMATICS
   talkto DsToPhiPi_PhiToKsKl_NOKLKINEMATICS {
      decayMode set "D_s+ -> phi pi+"
      daughterListNames set "PhiToKsKl_NOKLKINEMATICS"
      daughterListNames set $piList
      fittingAlgorithm  set "Add4"
   }




  #--- Set up resonances not involving Klongs


  #----   phi -> K+ K-

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

   path append BToDsDPath PhiToKpKm_for_phi3k_and_phi2pi1k



  #---- Ds+ -> phi pi+ ;  phi -> K+ K-

   mod clone SmpMakerDefiner DsToPhiPi
   path append BToDsDPath DsToPhiPi
   talkto DsToPhiPi {
      decayMode set "D_s+ -> phi pi+"
      daughterListNames set "PhiToKpKm_for_phi3k_and_phi2pi1k"
      daughterListNames set $piList
      fittingAlgorithm  set "Add4"
      preFitSelectors   set "Mass pdg-0.090:pdg+0.090"
   }





  #----  D- -> Ks pi-

   mod clone SmpMakerDefiner DpmToKsPipm
   path append BToDsDPath DpmToKsPipm
   talkto DpmToKsPipm {
       decayMode          set "D+ -> K_S0 pi+"
       daughterListNames  set KsDefault
       daughterListNames  set $piList
       fittingAlgorithm   set "Add4"
       preFitSelectors    set "Mass pdg-0.090:pdg+0.090"
       postFitSelectors   set "Mass pdg-0.090:pdg+0.090"
    }




  #----  D0 -> K- pi+

   mod clone SmpMakerDefiner D0ToKpmPipm
   path append BToDsDPath D0ToKpmPipm
   talkto D0ToKpmPipm {
       decayMode          set "D0 -> K- pi+"
       daughterListNames  set $KpmNotForPhiList
       daughterListNames  set $piList
       fittingAlgorithm   set "Add4"
       preFitSelectors    set "Mass pdg-0.090:pdg+0.090"
       postFitSelectors   set "Mass pdg-0.090:pdg+0.090"
    }












  #----- set up B decay selectors


   #---- B0 -> Ds+ D-    ;  Ds+ -> phi pi+ ; D-    -> Kl pi- ; phi -> K+ K-

   mod clone SmpMakerDefiner B0ToDsD_DToKlPi
   path append BToDsDPath B0ToDsD_DToKlPi
   talkto B0ToDsD_DToKlPi {
      decayMode set "B0 -> D_s+ D-"
      daughterListNames set "DsToPhiPi"
      daughterListNames set "DpmToKlPi_NOKLKINEMATICS"
      postFitSelectors  set "KlongDeltaE -0.03:0.10"
      createUsrData     set true
   }



   #---- B0 -> Ds+ D-    ;  Ds+ -> phi pi+ ; D-    -> Ks pi- ; phi -> Ks Kl

   mod clone SmpMakerDefiner B0ToDsD_DsToPhiPi_PhiToKsKl
   path append BToDsDPath B0ToDsD_DsToPhiPi_PhiToKsKl
   talkto B0ToDsD_DsToPhiPi_PhiToKsKl {
      decayMode set "B0 -> D_s+ D-"
      daughterListNames set "DsToPhiPi_PhiToKsKl_NOKLKINEMATICS"
      daughterListNames set "DpmToKsPipm"
      postFitSelectors  set "KlongDeltaE -0.03:0.10"
      createUsrData     set true
   }



   #---- B0 -> Ds+ D-    ;  Ds+ -> phi pi+ ; D-    -> Ks pi- ; phi -> K+ K-

   mod clone SmpMakerDefiner B0ToDsD_DsToPhiPi_PhiToKpKm
   path append BToDsDPath B0ToDsD_DsToPhiPi_PhiToKpKm
   talkto B0ToDsD_DsToPhiPi_PhiToKpKm {
      decayMode set "B0 -> D_s+ D-"
      daughterListNames set "DsToPhiPi"
      daughterListNames set "DpmToKsPipm"
      preFitSelectors   set "DeltaE -0.32:0.32"
      preFitSelectors   set "Mes 5.18:5.32"
      postFitSelectors  set "DeltaE -0.32:0.32"
      postFitSelectors  set "Mes 5.18:5.32"
      createUsrData     set true
   }



   #---- B+ -> Ds+ D0bar ;  Ds+ -> phi pi+ ; D0bar -> K+ pi- ; phi -> Ks Kl
   mod clone SmpMakerDefiner BpToDsD_DsToPhiPi_PhiToKsKl
   path append BToDsDPath BpToDsD_DsToPhiPi_PhiToKsKl
   talkto BpToDsD_DsToPhiPi_PhiToKsKl {
      decayMode set "B- -> D_s- D0"
      daughterListNames set "DsToPhiPi_PhiToKsKl_NOKLKINEMATICS"
      daughterListNames set "D0ToKpmPipm"
      postFitSelectors  set "KlongDeltaE -0.03:0.10"
      createUsrData     set true
   }



   #---- B+ -> Ds+ D0bar ;  Ds+ -> phi pi+ ; D0bar -> K+ pi- ; phi -> K+ K-
   mod clone SmpMakerDefiner BpToDsD_DsToPhiPi_PhiToKpKm
   path append BToDsDPath BpToDsD_DsToPhiPi_PhiToKpKm
   talkto BpToDsD_DsToPhiPi_PhiToKpKm {
      decayMode set "B- -> D_s- D0"
      daughterListNames set "DsToPhiPi"
      daughterListNames set "D0ToKpmPipm"
      preFitSelectors   set "DeltaE -0.32:0.32"
      preFitSelectors   set "Mes 5.18:5.32"
      postFitSelectors  set "DeltaE -0.32:0.32"
      postFitSelectors  set "Mes 5.18:5.32"
      createUsrData     set true
   }



########################


   mod clone TagFromList TagBpToDsD_D0toKmpip_phitoKpKm
   path append BToDsDPath TagBpToDsD_D0toKmpip_phitoKpKm
   talkto TagBpToDsD_D0toKmpip_phitoKpKm {
      listsToTag set BpToDsD_DsToPhiPi_PhiToKpKm
      tagName set    BpToDsD_D0toKmpip_phitoKpKm
      production set true
      verbose set false
   }

   mod clone TagFromList TagBpToDsD_D0toKmpip_phitoKsKl
   path append BToDsDPath TagBpToDsD_D0toKmpip_phitoKsKl
   talkto TagBpToDsD_D0toKmpip_phitoKsKl {
      listsToTag set BpToDsD_DsToPhiPi_PhiToKsKl
      tagName set    BpToDsD_D0toKmpip_phitoKsKl
      production set true
      verbose set false
   }

   mod clone TagFromList TagB0ToDsD_DptoKspip_phitoKpKm
   path append BToDsDPath TagB0ToDsD_DptoKspip_phitoKpKm
   talkto TagB0ToDsD_DptoKspip_phitoKpKm {
      listsToTag set B0ToDsD_DsToPhiPi_PhiToKpKm
      tagName set    B0ToDsD_DptoKspip_phitoKpKm
      production set true
      verbose set false
   }

   mod clone TagFromList TagB0ToDsD_DptoKspip_phitoKsKl
   path append BToDsDPath TagB0ToDsD_DptoKspip_phitoKsKl
   talkto TagB0ToDsD_DptoKspip_phitoKsKl {
      listsToTag set B0ToDsD_DsToPhiPi_PhiToKsKl
      tagName set    B0ToDsD_DptoKspip_phitoKsKl
      production set true
      verbose set false
   }

   mod clone TagFromList TagB0ToDsD_DptoKlpip_phitoKpKm
   path append BToDsDPath TagB0ToDsD_DptoKlpip_phitoKpKm
   talkto TagB0ToDsD_DptoKlpip_phitoKpKm {
      listsToTag set B0ToDsD_DToKlPi
      tagName set    B0ToDsD_DptoKlpip_phitoKpKm
      production set true
      verbose set false
   }



   mod clone TagFilterByName BToDsDFinalFilter
   path append BToDsDPath BToDsDFinalFilter
   talkto BToDsDFinalFilter {
      orList set BpToDsD_D0toKmpip_phitoKpKm
      orList set BpToDsD_D0toKmpip_phitoKsKl
      orList set B0ToDsD_DptoKspip_phitoKpKm
      orList set B0ToDsD_DptoKspip_phitoKsKl
      orList set B0ToDsD_DptoKlpip_phitoKpKm
      production set true
      verbose set false
   }

########################

 # Save these composites.
###global BtaCandLists
###lappend BtaCandLists B0ToDsD_DToKlPi B0ToDsD_DsToPhiPi_PhiToKsKl B0ToDsD_DsToPhiPi_PhiToKpKm BpToDsD_DsToPhiPi_PhiToKsKl BpToDsD_DsToPhiPi_PhiToKpKm



