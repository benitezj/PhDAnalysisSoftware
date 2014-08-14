# O. Long - Skim D0Toll 
# $Id: D0TollPath.tcl,v 1.2 2008/05/08 23:12:32 prencipe Exp $
#
############################################################################
##
## Defines D0Toll tag filter path
##
## This filter searches for the decays:
##
##   D*+ -> D0 pi+
##          D0 -> e-  e+
##          D0 -> mu-  mu+
##          D0 -> e- mu+
##          D0 -> mu- e+
##          D0 -> K-  pi+ (control mode)
##
##   Owen Long, U. C. Riverside
##
############################################################################

   path create D0TollPath

   set d0tollElectronList GoodTracksVeryLoose
   set d0tollMuonList     GoodTracksVeryLoose
   set d0tollKaonList     GoodTracksVeryLoose
   set d0tollPionList     GoodTracksVeryLoose

   set looseWin 0.210
   set minD0pstar 2.3

   global BtaCandLists
   lappend BtaCandLists D0ToKPi_dtoll
   lappend BtaCandLists DstarToD0Pi_kpi_dtoll
   lappend BtaCandLists D0Toee_prefit_dtoll
   lappend BtaCandLists D0Toee_dtoll
   lappend BtaCandLists DstarToD0Pi_ee_dtoll
   lappend BtaCandLists D0Tomumu_prefit_dtoll
   lappend BtaCandLists D0Tomumu_dtoll
   lappend BtaCandLists DstarToD0Pi_mumu_dtoll
   lappend BtaCandLists D0Tomue_prefit_dtoll
   lappend BtaCandLists D0Tomue_dtoll
   lappend BtaCandLists DstarToD0Pi_mue_dtoll
   lappend BtaCandLists D0Toemu_prefit_dtoll
   lappend BtaCandLists D0Toemu_dtoll
   lappend BtaCandLists DstarToD0Pi_emu_dtoll




   ##################
   ## D0 -> K pi
   ##################

   mod clone SmpRefinerDefiner D0ToKPi_dtoll
   path append D0TollPath D0ToKPi_dtoll
   catch { setProduction D0ToKPi_dtoll }
   talkto D0ToKPi_dtoll {
       debug             set f
       verbose           set f
       decayMode         set "D0 -> K- pi+"
       unrefinedListName  set "D0ToKPiLoose"
       daughterListNames set "$d0tollKaonList"
       daughterListNames set "$d0tollPionList"
       fittingAlgorithm  set "Cascade"
       fitConstraints    set "Geo"
       fitConstraints    set "Momentum"
       postFitSelectors  set "ProbChiSq 0.005:1.0"
       postFitSelectors  set "CmsP $minD0pstar:10.0"
   }
   mod clone SmpMakerDefiner DstarToD0Pi_kpi_dtoll
   path append D0TollPath DstarToD0Pi_kpi_dtoll
   catch { setProduction DstarToD0Pi_kpi_dtoll }
   talkto DstarToD0Pi_kpi_dtoll {
       debug             set f
       verbose           set f
       decayMode         set "D*+ -> D0 pi+"
       daughterListNames set "D0ToKPi_dtoll"
       daughterListNames set "GoodTracksVeryLoose"
       fittingAlgorithm  set "Cascade"
       fitConstraints    set "Geo"
       fitConstraints    set "Momentum"
       fitConstraints    set "Beam"
       preFitSelectors   set "DeltaM 0.130:0.170"
       postFitSelectors  set "DeltaM 0.130:0.170"
   }




   ##################
   ## D0 -> e e
   ##################

   mod clone SmpMakerDefiner D0Toee_prefit_dtoll
   path append D0TollPath D0Toee_prefit_dtoll
   catch { setProduction D0Toee_prefit_dtoll }
   talkto D0Toee_prefit_dtoll {
      decayMode set "D0 -> e+ e-"
      daughterListNames set "GoodTracksVeryLoose"
      daughterListNames set "GoodTracksVeryLoose"
      preFitSelectors set "Mass pdg-$looseWin:pdg+$looseWin"
   }
   mod clone SmpRefinerDefiner D0Toee_dtoll
   path append D0TollPath D0Toee_dtoll
   catch { setProduction D0Toee_dtoll }
   talkto D0Toee_dtoll {
       debug             set f
       verbose           set f
       decayMode         set "D0 -> e+ e-"
       unrefinedListName  set "D0Toee_prefit_dtoll"
       daughterListNames set "$d0tollElectronList"
       daughterListNames set "$d0tollElectronList"
       fittingAlgorithm  set "Cascade"
       fitConstraints    set "Geo"
       fitConstraints    set "Momentum"
       postFitSelectors  set "ProbChiSq 0.005:1.0"
       postFitSelectors  set "CmsP $minD0pstar:10.0"
   }
   mod clone SmpMakerDefiner DstarToD0Pi_ee_dtoll
   path append D0TollPath DstarToD0Pi_ee_dtoll
   catch { setProduction DstarToD0Pi_ee_dtoll }
   talkto DstarToD0Pi_ee_dtoll {
       debug             set f
       verbose           set f
       decayMode         set "D*+ -> D0 pi+"
       daughterListNames set "D0Toee_dtoll"
       daughterListNames set "GoodTracksVeryLoose"
       fittingAlgorithm  set "Cascade"
       fitConstraints    set "Geo"
       fitConstraints    set "Momentum"
       fitConstraints    set "Beam"
       preFitSelectors   set "DeltaM 0.130:0.170"
       postFitSelectors  set "DeltaM 0.130:0.170"
   }




   ##################
   ## D0 -> mu mu
   ##################

   mod clone SmpMakerDefiner D0Tomumu_prefit_dtoll
   path append D0TollPath D0Tomumu_prefit_dtoll
   catch { setProduction D0Tomumu_prefit_dtoll }
   talkto D0Tomumu_prefit_dtoll {
      decayMode set "D0 -> mu+ mu-"
      daughterListNames set "GoodTracksVeryLoose"
      daughterListNames set "GoodTracksVeryLoose"
      preFitSelectors set "Mass pdg-$looseWin:pdg+$looseWin"
   }
   mod clone SmpRefinerDefiner D0Tomumu_dtoll
   path append D0TollPath D0Tomumu_dtoll
   catch { setProduction D0Tomumu_dtoll }
   talkto D0Tomumu_dtoll {
       debug             set f
       verbose           set f
       decayMode         set "D0 -> mu+ mu-"
       unrefinedListName  set "D0Tomumu_prefit_dtoll"
       daughterListNames set "$d0tollMuonList"
       daughterListNames set "$d0tollMuonList"
       fittingAlgorithm  set "Cascade"
       fitConstraints    set "Geo"
       fitConstraints    set "Momentum"
       postFitSelectors  set "ProbChiSq 0.005:1.0"
       postFitSelectors  set "CmsP $minD0pstar:10.0"
   }
   mod clone SmpMakerDefiner DstarToD0Pi_mumu_dtoll
   path append D0TollPath DstarToD0Pi_mumu_dtoll
   catch { setProduction DstarToD0Pi_mumu_dtoll }
   talkto DstarToD0Pi_mumu_dtoll {
       debug             set f
       verbose           set f
       decayMode         set "D*+ -> D0 pi+"
       daughterListNames set "D0Tomumu_dtoll"
       daughterListNames set "GoodTracksVeryLoose"
       fittingAlgorithm  set "Cascade"
       fitConstraints    set "Geo"
       fitConstraints    set "Momentum"
       fitConstraints    set "Beam"
       preFitSelectors   set "DeltaM 0.130:0.170"
       postFitSelectors  set "DeltaM 0.130:0.170"
   }




   ##################
   ## D0 -> e mu
   ##################

   mod clone SmpMakerDefiner D0Toemu_prefit_dtoll
   path append D0TollPath D0Toemu_prefit_dtoll
   catch { setProduction D0Toemu_prefit_dtoll }
   talkto D0Toemu_prefit_dtoll {
      decayMode set "D0 -> e+ mu-"
      daughterListNames set "GoodTracksVeryLoose"
      daughterListNames set "GoodTracksVeryLoose"
      preFitSelectors set "Mass pdg-$looseWin:pdg+$looseWin"
   }
   mod clone SmpRefinerDefiner D0Toemu_dtoll
   path append D0TollPath D0Toemu_dtoll
   catch { setProduction D0Toemu_dtoll }
   talkto D0Toemu_dtoll {
       debug             set f
       verbose           set f
       decayMode         set "D0 -> e+ mu-"
       unrefinedListName  set "D0Toemu_prefit_dtoll"
       daughterListNames set "$d0tollElectronList"
       daughterListNames set "$d0tollMuonList"
       fittingAlgorithm  set "Cascade"
       fitConstraints    set "Geo"
       fitConstraints    set "Momentum"
       postFitSelectors  set "ProbChiSq 0.005:1.0"
       postFitSelectors  set "CmsP $minD0pstar:10.0"
   }
   mod clone SmpMakerDefiner DstarToD0Pi_emu_dtoll
   path append D0TollPath DstarToD0Pi_emu_dtoll
   catch { setProduction DstarToD0Pi_emu_dtoll }
   talkto DstarToD0Pi_emu_dtoll {
       debug             set f
       verbose           set f
       decayMode         set "D*+ -> D0 pi+"
       daughterListNames set "D0Toemu_dtoll"
       daughterListNames set "GoodTracksVeryLoose"
       fittingAlgorithm  set "Cascade"
       fitConstraints    set "Geo"
       fitConstraints    set "Momentum"
       fitConstraints    set "Beam"
       preFitSelectors   set "DeltaM 0.130:0.170"
       postFitSelectors  set "DeltaM 0.130:0.170"
   }



   ##################
   ## D0 -> mu e
   ##################

   mod clone SmpMakerDefiner D0Tomue_prefit_dtoll
   path append D0TollPath D0Tomue_prefit_dtoll
   catch { setProduction D0Tomue_prefit_dtoll }
   talkto D0Tomue_prefit_dtoll {
      decayMode set "D0 -> mu+ e-"
      daughterListNames set "GoodTracksVeryLoose"
      daughterListNames set "GoodTracksVeryLoose"
      preFitSelectors set "Mass pdg-$looseWin:pdg+$looseWin"
   }
   mod clone SmpRefinerDefiner D0Tomue_dtoll
   path append D0TollPath D0Tomue_dtoll
   catch { setProduction D0Tomue_dtoll }
   talkto D0Tomue_dtoll {
       debug             set f
       verbose           set f
       decayMode         set "D0 -> mu+ e-"
       unrefinedListName  set "D0Tomue_prefit_dtoll"
       daughterListNames set "$d0tollMuonList"
       daughterListNames set "$d0tollElectronList"
       fittingAlgorithm  set "Cascade"
       fitConstraints    set "Geo"
       fitConstraints    set "Momentum"
       postFitSelectors  set "ProbChiSq 0.005:1.0"
       postFitSelectors  set "CmsP $minD0pstar:10.0"
   }
   mod clone SmpMakerDefiner DstarToD0Pi_mue_dtoll
   path append D0TollPath DstarToD0Pi_mue_dtoll
   catch { setProduction DstarToD0Pi_mue_dtoll }
   talkto DstarToD0Pi_mue_dtoll {
       debug             set f
       verbose           set f
       decayMode         set "D*+ -> D0 pi+"
       daughterListNames set "D0Tomue_dtoll"
       daughterListNames set "GoodTracksVeryLoose"
       fittingAlgorithm  set "Cascade"
       fitConstraints    set "Geo"
       fitConstraints    set "Momentum"
       fitConstraints    set "Beam"
       preFitSelectors   set "DeltaM 0.130:0.170"
       postFitSelectors  set "DeltaM 0.130:0.170"
   }




   #######################
   ## Set Tag Bits
   #######################


   mod clone TagFromList TagDstarToD0Pi_kpi_dtoll
   path append D0TollPath TagDstarToD0Pi_kpi_dtoll
   talkto TagDstarToD0Pi_kpi_dtoll {
      listsToTag set DstarToD0Pi_kpi_dtoll
      tagName set DstarToD0Pi_kpi_dtoll
      production set true
      verbose set false
   }
   mod clone TagFromList TagDstarToD0Pi_ee_dtoll
   path append D0TollPath TagDstarToD0Pi_ee_dtoll
   talkto TagDstarToD0Pi_ee_dtoll {
      listsToTag set DstarToD0Pi_ee_dtoll
      tagName set DstarToD0Pi_ee_dtoll
      production set true
      verbose set false
   }
   mod clone TagFromList TagDstarToD0Pi_mumu_dtoll
   path append D0TollPath TagDstarToD0Pi_mumu_dtoll
   talkto TagDstarToD0Pi_mumu_dtoll {
      listsToTag set DstarToD0Pi_mumu_dtoll
      tagName set DstarToD0Pi_mumu_dtoll
      production set true
      verbose set false
   }
   mod clone TagFromList TagDstarToD0Pi_mue_dtoll
   path append D0TollPath TagDstarToD0Pi_mue_dtoll
   talkto TagDstarToD0Pi_mue_dtoll {
      listsToTag set DstarToD0Pi_mue_dtoll
      tagName set DstarToD0Pi_mue_dtoll
      production set true
      verbose set false
   }
   mod clone TagFromList TagDstarToD0Pi_emu_dtoll
   path append D0TollPath TagDstarToD0Pi_emu_dtoll
   talkto TagDstarToD0Pi_emu_dtoll {
      listsToTag set DstarToD0Pi_emu_dtoll
      tagName set DstarToD0Pi_emu_dtoll
      production set true
      verbose set false
   }


   mod clone TagFilterByName D0TollFinalFilter
   path append D0TollPath D0TollFinalFilter
   talkto D0TollFinalFilter {
      orList set DstarToD0Pi_kpi_dtoll
      orList set DstarToD0Pi_ee_dtoll
      orList set DstarToD0Pi_mumu_dtoll
      orList set DstarToD0Pi_emu_dtoll
      orList set DstarToD0Pi_mue_dtoll
      production set true
      verbose set false
   }




