#
##  $Id: EEToLightMesonsSequence.tcl,v 1.8 2005/11/03 21:23:52 rolfa Exp $
#
##############################################################################
##
##   e+ e- -> K K pi0, K_S0 K pi, K_S0 K_S0 pi0
##
##
## Giampiero Mancinelli, University of Cincinnati
############################################################################

sequence create EEToLightMesonsSequence

############################################################################
## EEToKKpiSequence
############################################################################

sequence create EEToKKpiSequence
sequence append EEToLightMesonsSequence EEToKKpiSequence

sourceFoundFile SimpleComposition/SmpProcs.tcl

createsmpsublister EEToLightMesonsSequence pi0AllVeryLoose_Ups {
  unrefinedListName set pi0AllVeryLoose
  selectors         set "CmsP 0.3:"
}

createsmpsublister EEToLightMesonsSequence pi0VeryLoose_Ups {
  unrefinedListName set pi0VeryLoose
  selectors         set "CmsP 0.3:"
}

createsmpsublister EEToLightMesonsSequence EEKaonNotMuon {
    unrefinedListName     set KLHNotPion
    isCloneOfListName     set muNNVeryTightFakeRate
    whatToDoWithCloneList set RejectOverlaps
}

createsmpsublister EEToLightMesonsSequence EEKaonNotLepton {
    unrefinedListName     set EEKaonNotMuon
    isCloneOfListName     set eMicroTight
    whatToDoWithCloneList set RejectOverlaps
}

createsmpsublister EEToLightMesonsSequence EEPionNotMuon {
    unrefinedListName     set piLHVeryTight
    isCloneOfListName     set muNNVeryTightFakeRate
    whatToDoWithCloneList set RejectOverlaps
}

createsmpsublister EEToLightMesonsSequence EEPionNotMuonTight {
    unrefinedListName    set piLHVeryTight
    isCloneOfListName         set muNNLooseFakeRate
    whatToDoWithCloneList set RejectOverlaps
}
createsmpsublister EEToLightMesonsSequence EEPionNotLepton {
    unrefinedListName     set EEPionNotMuonTight
    isCloneOfListName     set eMicroTight
    whatToDoWithCloneList set RejectOverlaps
}

mod clone SmpMakerDefiner Y4SToKsKPi_Ups
sequence append EEToKKpiSequence Y4SToKsKPi_Ups
catch { setProduction  Y4SToKsKPi_Ups}
talkto Y4SToKsKPi_Ups {
    decayMode          set "Upsilon(4S) -> K_S0 K+ pi-"

    daughterListNames  set "KsDefault"
    daughterListNames  set "GoodTracksLoose"
    daughterListNames  set "piLHVeryLoose"

    fittingAlgorithm   set "Add4"

    preFitSelectors    set "Mass 9.08:12.08"
    postFitSelectors   set "Mass 9.08:12.08"
    postFitSelectors   set "ProbChiSq 0.005:1.0"
 }

mod clone SmpSubListerDefiner Y4SToKsKPi_Ups_Tight
sequence append EEToKKpiSequence Y4SToKsKPi_Ups_Tight
catch { setProduction  Y4SToKsKPi_Ups_Tight}
talkto Y4SToKsKPi_Ups_Tight {
    decayMode          set "Upsilon(4S) -> K_S0 K+ pi-"

    unrefinedListName  set "Y4SToKsKPi_Ups"
    daughterListNames  set "KsDefault"
    daughterListNames  set "KLHNotPion"
    daughterListNames  set "piLHVeryLoose"

 }
   
mod clone SmpMakerDefiner Y4SToKsKsPi0_Ups
sequence append EEToKKpiSequence Y4SToKsKsPi0_Ups
catch { setProduction  Y4SToKsKsPi0_Ups}
talkto Y4SToKsKsPi0_Ups {
    decayMode          set "Upsilon(4S) -> K_S0 K_S0 pi0"

    daughterListNames  set "KsDefault"
    daughterListNames  set "KsDefault"
    daughterListNames  set "pi0AllVeryLoose_Ups"

    fittingAlgorithm   set "Add4"

    preFitSelectors    set "Mass 9.08:12.08"
    postFitSelectors   set "Mass 9.08:12.08"
    postFitSelectors   set "ProbChiSq 0.005:1.0"
 }

mod clone SmpSubListerDefiner Y4SToKsKsPi0_Ups_Tight
sequence append EEToKKpiSequence Y4SToKsKsPi0_Ups_Tight
catch { setProduction  Y4SToKsKsPi0_Ups_Tight}
talkto Y4SToKsKsPi0_Ups_Tight {
    decayMode          set "Upsilon(4S) -> K_S0 K_S0 pi0"

    unrefinedListName  set "Y4SToKsKsPi0_Ups"
    daughterListNames  set "KsDefault"
    daughterListNames  set "KsDefault"
    daughterListNames  set "pi0VeryLoose_Ups"

 }

mod clone SmpMakerDefiner Y4SToKKPi0_Ups
sequence append EEToKKpiSequence Y4SToKKPi0_Ups
catch { setProduction  Y4SToKKPi0_Ups}
talkto Y4SToKKPi0_Ups {
    decayMode          set "Upsilon(4S) -> K+ K- pi0"

#    daughterListNames  set "GoodTracksLoose"
#    daughterListNames  set "GoodTracksLoose"
    daughterListNames  set "KLHNotPion"
    daughterListNames  set "KLHNotPion"
    daughterListNames  set "pi0AllVeryLoose_Ups"

    fittingAlgorithm   set "Add4"

    preFitSelectors    set "Mass 9.08:12.08"
    postFitSelectors   set "Mass 9.08:12.08"
    postFitSelectors   set "ProbChiSq 0.005:1.0"
 }

mod clone SmpSubListerDefiner Y4SToKKPi0_Ups_Tight
sequence append EEToKKpiSequence Y4SToKKPi0_Ups_Tight
catch { setProduction  Y4SToKKPi0_Ups_Tight}
talkto Y4SToKKPi0_Ups_Tight {
    decayMode          set "Upsilon(4S) -> K+ K- pi0"

    unrefinedListName  set "Y4SToKKPi0_Ups"
    daughterListNames  set "KLHLoose"
    daughterListNames  set "KLHLoose"
    daughterListNames  set "pi0AllVeryLoose_Ups"

 }

mod clone SmpMergerDefiner EEToKKpi
sequence append EEToKKpiSequence EEToKKpi
catch { setProduction EEToKKpi }
talkto EEToKKpi {
    inputListNames     set "Y4SToKsKPi_Ups"
    inputListNames     set "Y4SToKsKsPi0_Ups"
    inputListNames     set "Y4SToKKPi0_Ups"
    inputListNames     set "Y4SToKsKPi_Ups_Tight"
    inputListNames     set "Y4SToKsKsPi0_Ups_Tight"
    inputListNames     set "Y4SToKKPi0_Ups_Tight"
}

############################################################################
## EEToKKEta
##
##     e+ e- -> phi eta 
##     e+ e- -> K K eta 
##                   |
##                   |---> gamma gamma, pi+ pi- pi0
## 
##  G. Finocchiaro, LNF
############################################################################

sequence create EEToKKEtaSequence
sequence append EEToLightMesonsSequence EEToKKEtaSequence

# EE-> eta K+K-
createsmpmaker EEToKKEtaSequence EEToKKEtaLoose {
  decayMode          set "Upsilon(4S) -> K+ K- eta"
  daughterListNames  set "GoodTracksLoose"
  daughterListNames  set "GoodTracksLoose"
  daughterListNames  set "etaDefault"
  preFitSelectors    set "Mass 9.08:12.08"
}

createsmpsublister EEToKKEtaSequence EEToKKEta {
  decayMode          set "Upsilon(4S) -> K+ K- eta"
  unrefinedListName  set "EEToKKEtaLoose"
  daughterListNames  set "EEKaonNotMuon"
  daughterListNames  set "EEKaonNotMuon"
  daughterListNames  set "etaDefault"
}

############################################################################
## EEToPhiEta
##
##     e+ e- -> phi eta 
## 
##  G. Finocchiaro, LNF
############################################################################

sequence create EEToPhiEtaSequence
sequence append EEToLightMesonsSequence EEToPhiEtaSequence

createsmpmaker EEToPhiEtaSequence EEToPhiEta {
  decayMode          set "Upsilon(4S) -> phi eta"
  daughterListNames  set "phiTightPID"
  daughterListNames  set "etaDefault"
  preFitSelectors    set "Mass :12.08"
}

############################################################################
## EEToKKPiPi
##
##   e+ e- -> K K pi0 pi0, K_S0 K pi pi0, K_S0 K_S0 pi0 pi0, 
##            K_S0 K_S0 pi pi, K K pi pi
##
##
## Giampiero Mancinelli, University of Cincinnati
############################################################################

sequence create EEToKKpipiSequence
sequence append EEToLightMesonsSequence EEToKKpipiSequence

mod clone SmpMakerDefiner Y4SToKsKPiPi0_Ups
sequence append EEToKKpipiSequence Y4SToKsKPiPi0_Ups
catch { setProduction  Y4SToKsKPiPi0_Ups}
talkto Y4SToKsKPiPi0_Ups {
    disableCloneCheck  set true
    decayMode          set "Upsilon(4S) -> K_S0 K+ pi- pi0"

    daughterListNames  set "KsDefault"
    daughterListNames  set "GoodTracksLoose"
    daughterListNames  set "piLHVeryLoose"
    daughterListNames  set "pi0AllVeryLoose_Ups"

    fittingAlgorithm   set "Add4"

    preFitSelectors    set "Mass 9.08:12.08"
    postFitSelectors   set "Mass 9.08:12.08"
    postFitSelectors   set "ProbChiSq 0.005:1.0"
 }
   
mod clone SmpSubListerDefiner Y4SToKsKPiPi0_Ups_Tight
sequence append EEToKKpipiSequence Y4SToKsKPiPi0_Ups_Tight
catch { setProduction  Y4SToKsKPiPi0_Ups_Tight}
talkto Y4SToKsKPiPi0_Ups_Tight {
    decayMode          set "Upsilon(4S) -> K_S0 K+ pi- pi0"

    unrefinedListName  set "Y4SToKsKPiPi0_Ups"
    daughterListNames  set "KsDefault"
    daughterListNames  set "KLHNotPion"
    daughterListNames  set "piLHVeryLoose"
    daughterListNames  set "pi0AllVeryLoose_Ups"
 }
   
mod clone SmpMakerDefiner Y4SToKsKsPi0Pi0_Ups
seq append EEToKKpipiSequence Y4SToKsKsPi0Pi0_Ups
catch { setProduction  Y4SToKsKsPi0Pi0_Ups}
talkto Y4SToKsKsPi0Pi0_Ups {
    decayMode          set "Upsilon(4S) -> K_S0 K_S0 pi0 pi0"

    daughterListNames  set "KsDefault"
    daughterListNames  set "KsDefault"
    daughterListNames  set "pi0AllVeryLoose"
    daughterListNames  set "pi0AllVeryLoose"

    fittingAlgorithm   set "Add4"

    preFitSelectors    set "Mass 9.08:12.08"
    postFitSelectors   set "Mass 9.08:12.08"
    postFitSelectors   set "ProbChiSq 0.005:1.0"
    maxNumberOfCandidates set 100
 }
   
mod clone SmpSubListerDefiner Y4SToKsKsPi0Pi0_Ups_Tight
seq append EEToKKpipiSequence Y4SToKsKsPi0Pi0_Ups_Tight
catch { setProduction  Y4SToKsKsPi0Pi0_Ups_Tight}
talkto Y4SToKsKsPi0Pi0_Ups_Tight {
    decayMode          set "Upsilon(4S) -> K_S0 K_S0 pi0 pi0"
    unrefinedListName  set "Y4SToKsKsPi0Pi0_Ups"
    daughterListNames  set "KsDefault"
    daughterListNames  set "KsDefault"
    daughterListNames  set "pi0AllVeryLoose_Ups"
    daughterListNames  set "pi0AllVeryLoose_Ups"
}

mod clone SmpMakerDefiner Y4SToKsKsPiPi_Ups
seq append EEToKKpipiSequence Y4SToKsKsPiPi_Ups
catch { setProduction  Y4SToKsKsPiPi_Ups}
talkto Y4SToKsKsPiPi_Ups {
    decayMode          set "Upsilon(4S) -> K_S0 K_S0 pi+ pi-"

    daughterListNames  set "KsDefault"
    daughterListNames  set "KsDefault"
    daughterListNames  set "piLHVeryLoose"
    daughterListNames  set "piLHVeryLoose"

    fittingAlgorithm   set "Add4"

    preFitSelectors    set "Mass 9.08:12.08"
    postFitSelectors   set "Mass 9.08:12.08"
    postFitSelectors   set "ProbChiSq 0.005:1.0"
 }

mod clone SmpSubListerDefiner Y4SToKsKsPiPi_Ups_Tight
seq append EEToKKpipiSequence Y4SToKsKsPiPi_Ups_Tight
catch { setProduction  Y4SToKsKsPiPi_Ups_Tight}
talkto Y4SToKsKsPiPi_Ups_Tight {
    decayMode          set "Upsilon(4S) -> K_S0 K_S0 pi+ pi-"

    unrefinedListName  set "Y4SToKsKsPiPi_Ups"
    daughterListNames  set "KsDefault"
    daughterListNames  set "KsDefault"
    daughterListNames  set "piLHLoose"
    daughterListNames  set "piLHLoose"
 }

mod clone SmpMakerDefiner Y4SToKKPi0Pi0_Ups
seq append EEToKKpipiSequence Y4SToKKPi0Pi0_Ups
catch { setProduction  Y4SToKKPi0Pi0_Ups}
talkto Y4SToKKPi0Pi0_Ups {
    decayMode          set "Upsilon(4S) -> K+ K- pi0 pi0"

    daughterListNames  set "GoodTracksLoose"
    daughterListNames  set "GoodTracksLoose"
    daughterListNames  set "pi0AllVeryLoose_Ups"
    daughterListNames  set "pi0AllVeryLoose_Ups"

    fittingAlgorithm   set "Add4"

    preFitSelectors    set "Mass 9.08:12.08"
    postFitSelectors   set "Mass 9.08:12.08"
    postFitSelectors   set "ProbChiSq 0.005:1.0"
    maxNumberOfCandidates set 100
}

mod clone SmpSubListerDefiner Y4SToKKPi0Pi0_Ups_Tight
seq append EEToKKpipiSequence Y4SToKKPi0Pi0_Ups_Tight
catch { setProduction  Y4SToKKPi0Pi0_Ups_Tight}
talkto Y4SToKKPi0Pi0_Ups_Tight {
    decayMode          set "Upsilon(4S) -> K+ K- pi0 pi0"

    unrefinedListName  set "Y4SToKKPi0Pi0_Ups"
    daughterListNames  set "KLHNotPion"
    daughterListNames  set "KLHNotPion"
    daughterListNames  set "pi0AllVeryLoose_Ups"
    daughterListNames  set "pi0AllVeryLoose_Ups"
 }

mod clone SmpMakerDefiner Y4SToKKPiPi_Ups
seq append EEToKKpipiSequence Y4SToKKPiPi_Ups
catch { setProduction  Y4SToKKPiPi_Ups}
talkto Y4SToKKPiPi_Ups {
    decayMode          set "Upsilon(4S) -> K+ K- pi+ pi-"

    daughterListNames  set "KLHNotPion"
    daughterListNames  set "KLHNotPion"
    daughterListNames  set "piLHVeryLoose"
    daughterListNames  set "piLHVeryLoose"

    fittingAlgorithm   set "Add4"

    preFitSelectors    set "Mass 9.08:12.08"
    postFitSelectors   set "Mass 9.08:12.08"
    postFitSelectors   set "ProbChiSq 0.005:1.0"
 }

mod clone SmpSubListerDefiner Y4SToKKPiPi_Ups_Tight
seq append EEToKKpipiSequence Y4SToKKPiPi_Ups_Tight
catch { setProduction  Y4SToKKPiPi_Ups_Tight}
talkto Y4SToKKPiPi_Ups_Tight {
    decayMode          set "Upsilon(4S) -> K+ K- pi+ pi-"

    unrefinedListName  set "Y4SToKKPiPi_Ups"
    daughterListNames  set "EEKaonNotMuon"
    daughterListNames  set "EEKaonNotMuon"
    daughterListNames  set "EEPionNotMuon"
    daughterListNames  set "EEPionNotMuon"
 }

mod clone SmpMergerDefiner EEToKKpipi
seq append EEToKKpipiSequence EEToKKpipi
catch { setProduction EEToKKpipi}
talkto EEToKKpipi {
    inputListNames     set "Y4SToKsKPiPi0_Ups"
    inputListNames     set "Y4SToKsKsPi0Pi0_Ups"
    inputListNames     set "Y4SToKsKsPiPi_Ups"
    inputListNames     set "Y4SToKKPi0Pi0_Ups"
    inputListNames     set "Y4SToKKPiPi_Ups"
    inputListNames     set "Y4SToKsKPiPi0_Ups_Tight"
    inputListNames     set "Y4SToKsKsPi0Pi0_Ups_Tight"
    inputListNames     set "Y4SToKsKsPiPi_Ups_Tight"
    inputListNames     set "Y4SToKKPi0Pi0_Ups_Tight"
    inputListNames     set "Y4SToKKPiPi_Ups_Tight"
}


############################################################################
##
##   e+ e- -> K K, pi pi
##
##
## Giampiero Mancinelli, University of Cincinnati
############################################################################

sequence create EEToPSPSSequence
sequence append EEToLightMesonsSequence EEToPSPSSequence

mod clone SmpMakerDefiner Y4SToKsKs_Ups
seq append EEToPSPSSequence Y4SToKsKs_Ups
catch { setProduction  Y4SToKsKs_Ups}
talkto Y4SToKsKs_Ups {
    decayMode          set "Upsilon(4S) -> K_S0 K_S0"

    daughterListNames  set "KsDefault"
    daughterListNames  set "KsDefault"

    fittingAlgorithm   set "Add4"

    preFitSelectors    set "Mass 9.08:12.08"
    postFitSelectors   set "Mass 9.08:12.08"
    postFitSelectors   set "ProbChiSq 0.005:1.0"
 }
   
mod clone SmpMakerDefiner Y4SToPiPi_Ups
seq append EEToPSPSSequence Y4SToPiPi_Ups
catch { setProduction  Y4SToPiPi_Ups}
talkto Y4SToPiPi_Ups {
    debug              set f
    verbose            set f
    disableCloneCheck  set true
    decayMode          set "Upsilon(4S) -> pi+ pi-"

    daughterListNames  set "EEPionNotLepton"
    daughterListNames  set "EEPionNotLepton"

    fittingAlgorithm   set "Add4"

    preFitSelectors    set "Mass 9.58:11.58"
    postFitSelectors   set "Mass 9.58:11.58"
    postFitSelectors   set "ProbChiSq 0.005:1.0"
 }

mod clone SmpMakerDefiner Y4SToKK_Ups
seq append EEToPSPSSequence Y4SToKK_Ups
catch { setProduction  Y4SToKK_Ups}
talkto Y4SToKK_Ups {
    debug              set f
    verbose            set f
    disableCloneCheck  set true
    decayMode          set "Upsilon(4S) -> K+ K- "

    daughterListNames  set "EEKaonNotLepton"
    daughterListNames  set "EEKaonNotLepton"

    fittingAlgorithm   set "Add4"

    preFitSelectors    set "Mass 9.08:12.08"
    postFitSelectors   set "Mass 9.08:12.08"
    postFitSelectors   set "ProbChiSq 0.005:1.0"
 }

mod clone SmpMakerDefiner Y4SToPi0Pi0_Ups
seq append EEToPSPSSequence Y4SToPi0Pi0_Ups
catch { setProduction  Y4SToPi0Pi0_Ups}
talkto Y4SToPi0Pi0_Ups {
    decayMode          set "Upsilon(4S) -> pi0 pi0 "

    daughterListNames  set "pi0AllVeryLoose_Ups"
    daughterListNames  set "pi0VeryLoose_Ups"

    fittingAlgorithm   set "Add4"

    preFitSelectors    set "Mass 9.08:12.08"
    postFitSelectors   set "Mass 9.08:12.08"
    postFitSelectors   set "ProbChiSq 0.005:1.0"
 }

mod clone SmpMergerDefiner EEToPSPS
seq append EEToPSPSSequence EEToPSPS
catch { setProduction EEToPSPS}
talkto EEToPSPS {
    inputListNames     set "Y4SToKsKs_Ups"
    inputListNames     set "Y4SToPi0Pi0_Ups"
    inputListNames     set "Y4SToKK_Ups"
    inputListNames     set "Y4SToPiPi_Ups"
}
