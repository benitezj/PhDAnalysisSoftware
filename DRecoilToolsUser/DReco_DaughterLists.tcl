set looseWin         0.090
set semilooseWin     0.070
set defaultWin       0.040
set looseWinPi0      0.160
set defaultWinPi0    0.070

seq create DRecoTagSequence

# slow pion list
mod clone SmpSubListerDefiner DRecoSlowPions
seq append DRecoTagSequence DRecoSlowPions
catch { setProduction DRecoSlowPions }
talkto DRecoSlowPions {
  unrefinedListName set "GoodTracksVeryLoose"
  selectors         set "CmsP :0.45"
}

mod clone SmpSubListerDefiner DRecoKaons
seq append DRecoTagSequence DRecoKaons
catch { setProduction DRecoKaons }
talkto DRecoKaons {
  unrefinedListName     set "KLHNotPion"
  isCloneOfListName     set "GoodTracksVeryLoose"
  whatToDoWithCloneList set AcceptClone
}

mod clone SmpSubListerDefiner DRecoKaonsTight
seq append DRecoTagSequence DRecoKaonsTight
catch { setProduction DRecoKaonsTight }
talkto DRecoKaonsTight {
  unrefinedListName     set "KLHTight"
  isCloneOfListName     set "GoodTracksLoose"
  whatToDoWithCloneList set AcceptClone
}

mod clone SmpSubListerDefiner DRecoKCombinedSuperLoose
seq append DRecoTagSequence DRecoKCombinedSuperLoose
catch { setProduction DRecoKCombinedSuperLoose }
talkto DRecoKCombinedSuperLoose {
  unrefinedListName     set "KCombinedSuperLoose"
#  isCloneOfListName     set "GoodTracksLoose"
#  whatToDoWithCloneList set AcceptClone
}

mod clone SmpSubListerDefiner DRecoPions
seq append DRecoTagSequence DRecoPions
catch { setProduction DRecoPions }
talkto DRecoPions {
  unrefinedListName set "GoodTracksVeryLoose"
#  isCloneOfListName set "GoodTracksVeryLoose"
#  whatToDoWithCloneList set AcceptClone
}

mod clone SmpSubListerDefiner DRecoPionsLHVeryLoose
seq append DRecoTagSequence DRecoPionsLHVeryLoose
catch { setProduction DRecoPionsLHVeryLoose }
talkto DRecoPionsLHVeryLoose {
  unrefinedListName set "piLHVeryLoose"
  isCloneOfListName set "GoodTracksLoose"
  whatToDoWithCloneList set AcceptClone
}

mod clone SmpSubListerDefiner DRecoPionsTight
seq append DRecoTagSequence DRecoPionsTight
catch { setProduction DRecoPionsTight }
talkto DRecoPionsTight {
  unrefinedListName set "piLHTight"
  isCloneOfListName set "GoodTracksLoose"
  whatToDoWithCloneList set AcceptClone
}

mod clone SmpSubListerDefiner DRecoProtons
seq append DRecoTagSequence DRecoProtons
catch { setProduction DRecoProtons }
talkto DRecoProtons {
  unrefinedListName set "pLHVeryLoose"
  isCloneOfListName set "GoodTracksLoose"
  whatToDoWithCloneList set AcceptClone
}

######################################################################
# ######################################################################

# # gamma -> e+ e- -- high mass cut for use in making dalitz eta/pi0 decays
# # PDG: M(eta) = 0.54775

# mod clone SmpMakerDefiner DRecoGammaConvDalitz
# seq append DRecoTagSequence DRecoGammaConvDalitz
# catch { setProduction DRecoGammaConvDalitz }
# talkto DRecoGammaConvDalitz {
#   decayMode         set "gamma -> e+ e-"
#   daughterListNames set "ChargedTracks"
#   daughterListNames set "ChargedTracks"
#   preFitSelectors   set "Mass :0.15"
#   fittingAlgorithm  set "GammaConversion"
#   postFitSelectors  set "Mass :0.1"
#   postFitSelectors  set "ProbChiSq 0.001:"
# }

# ######################################################################
# ######################################################################

# # gamma -> e+ e-

# mod clone SmpSubListerDefiner DRecoGammaConv
# seq append DRecoTagSequence DRecoGammaConv
# catch { setProduction DRecoGammaConv }
# talkto DRecoGammaConv {
#   unrefinedListName set "DRecoGammaConvDalitz"
#   selectors  set "Mass :0.05"
# }

# ######################################################################
######################################################################

## # pi0 decays

mod clone SmpSubListerDefiner DRecoPhotonList
seq append DRecoTagSequence DRecoPhotonList
catch { setProduction DRecoPhotonList }
talkto DRecoPhotonList {
  unrefinedListName set "GoodPhotonLoose"
  selectors         set "LabE 0.100:10.0"
  selectors         set "Lat 0.01:0.8"
}

# pi0 -> gamma gamma, no mass constraint
mod clone SmpRefinerDefiner DRecoPi0List
seq append DRecoTagSequence DRecoPi0List
catch { setProduction DRecoPi0List }
talkto DRecoPi0List {
  decayMode          set "pi0 -> gamma gamma"
  unrefinedListName  set "pi0VeryLoose"
  daughterListNames  set "DRecoPhotonList"
  daughterListNames  set "DRecoPhotonList"
  fittingAlgorithm   set "Cascade"
#  fitConstraints     set "Mass"
  fitConstraints     set "Momentum"
  fitConstraints     set "PrimaryVertex"
  postFitSelectors   set "ProbChiSq 0.001:"
}

# narrower mass region for above pi0 list
mod clone SmpSubListerDefiner DRecoPi0MassList
seq append DRecoTagSequence DRecoPi0MassList
catch { setProduction DRecoPi0MassList }
talkto DRecoPi0MassList {
  unrefinedListName set "DRecoPi0List"
  selectors         set "Mass 0.115:0.150"
}

# mass constrained fit for narrow mass window list
mod clone SmpRefitterDefiner DRecoPi0SelectList
seq append DRecoTagSequence DRecoPi0SelectList
catch { setProduction DRecoPi0SelectList }
talkto DRecoPi0SelectList {
  decayMode          set "pi0 -> gamma gamma"
  unrefinedListName  set "DRecoPi0MassList"
  fittingAlgorithm   set "Cascade"
  fitConstraints     set "Mass"
  fitConstraints     set "Momentum"
#  fitConstraints     set "PrimaryVertex"
  postFitSelectors   set "ProbChiSq 0.01:"
}

######################################################################
######################################################################

# K_S0 -> pi+ pi-

mod clone SmpMakerDefiner DRecoKShortList
seq append DRecoTagSequence DRecoKShortList
catch { setProduction DRecoKShortList }
talkto DRecoKShortList {
  decayMode         set "K_S0 -> pi+ pi-"
  daughterListNames set "ChargedTracks"
  daughterListNames set "ChargedTracks"
  preFitSelectors   set "Mass 0.3:0.7"
  fittingAlgorithm  set "TreeFitter"
  postFitSelectors  set "Mass 0.44:0.56"
  postFitSelectors  set "ProbChiSq 0.001:"
  postFitSelectors  set "FlightSignificance 3.:"
}

# narrower mass region for above KShort list
mod clone SmpSubListerDefiner DRecoKShortSelectList
seq append DRecoTagSequence DRecoKShortSelectList
catch { setProduction DRecoKShortSelectList }
talkto DRecoKShortSelectList {
  unrefinedListName set "DRecoKShortList"
  selectors         set "Mass 0.4876:0.5076"
}

# mass constrained fit for narrow mass window list
mod clone SmpRefitterDefiner DRecoKShortMassList
seq append DRecoTagSequence DRecoKShortMassList
catch { setProduction DRecoKShortMassList }
talkto DRecoKShortMassList {
  decayMode         set "K_S0 -> pi+ pi-"
  unrefinedListName  set "DRecoKShortSelectList"
  fittingAlgorithm   set "TreeFitter"
  fitConstraints     set "Mass"
}

######################################################################
######################################################################

# K*0-> K+ pi-

mod clone SmpMakerDefiner DRecoKstar0List
seq append DRecoTagSequence DRecoKstar0List
catch { setProduction DRecoKstar0List }
talkto DRecoKstar0List {
  decayMode         set "K*0 -> K- pi+"
  daughterListNames set "KBDTKaonNotPion"
  daughterListNames set "DRecoPions"
  preFitSelectors   set "Mass pdg-0.160:pdg+0.160"
  fittingAlgorithm  set "TreeFitter"
  postFitSelectors  set "Mass pdg-0.075:pdg+0.075"
  postFitSelectors  set "ProbChiSq 0.001:"
}

######################################################################
######################################################################

# phi-> K+ K-

mod clone SmpMakerDefiner DRecoPhiList
seq append DRecoTagSequence DRecoPhiList
catch { setProduction DRecoPhiList }
talkto DRecoPhiList {
  decayMode         set "phi -> K+ K-"
  daughterListNames set "KBDTKaonNotPion"
  daughterListNames set "KBDTKaonNotPion"
  preFitSelectors   set "Mass 0.920:1.120"
  fittingAlgorithm  set "TreeFitter"
  postFitSelectors  set "Mass 1.010:1.030"
  postFitSelectors  set "ProbChiSq 0.001:"
}

# mass constrained fit for narrow mass window list
mod clone SmpRefitterDefiner DRecoPhiMassList
seq append DRecoTagSequence DRecoPhiMassList
catch { setProduction DRecoPhiMassList }
talkto DRecoPhiMassList {
  decayMode         set "phi -> K+ K-"
  unrefinedListName  set "DRecoPhiList"
  fittingAlgorithm   set "TreeFitter"
  fitConstraints     set "Mass"
}

# phi-> K+ K- (based on the old pid, needed for Ds->phi rhoC and Ds -> Phi Pi Pi Pi)

mod clone SmpMakerDefiner DRecoPhiListOldPID
seq append DRecoTagSequence DRecoPhiListOldPID
catch { setProduction DRecoPhiListOldPID }
talkto DRecoPhiListOldPID {
  decayMode         set "phi -> K+ K-"
  daughterListNames set "KLHNotPion"
  daughterListNames set "KLHNotPion"
  preFitSelectors   set "Mass 0.920:1.120"
  fittingAlgorithm  set "TreeFitter"
  postFitSelectors  set "Mass 1.010:1.030"
  postFitSelectors  set "ProbChiSq 0.001:"
}

# mass constrained fit for narrow mass window list
mod clone SmpRefitterDefiner DRecoPhiMassListOldPID
seq append DRecoTagSequence DRecoPhiMassListOldPID
catch { setProduction DRecoPhiMassListOldPID }
talkto DRecoPhiMassListOldPID {
  decayMode         set "phi -> K+ K-"
  unrefinedListName  set "DRecoPhiListOldPID"
  fittingAlgorithm   set "TreeFitter"
  fitConstraints     set "Mass"
}

######################################################################
######################################################################

# lambda0 -> p pi-

mod clone SmpMakerDefiner DRecoLambda0List
seq append DRecoTagSequence DRecoLambda0List
catch { setProduction DRecoLambda0List }
talkto DRecoLambda0List {
  decayMode         set "Lambda0 -> p+ pi-"
  daughterListNames set "DRecoProtons"
  daughterListNames set "DRecoPions"
  preFitSelectors   set "Mass 1.05:1.2"
  fittingAlgorithm  set "TreeFitter"
  #postFitSelectors  set "Mass 1.110:1.122"
  postFitSelectors  set "Mass 1.111:1.119"
  postFitSelectors  set "Helicity -0.85:"
  postFitSelectors  set "ProbChiSq 0.001:"
  postFitSelectors  set "FlightSignificance 3:"
}

# mass constrained fit for narrow mass window list
mod clone SmpRefitterDefiner DRecoLambda0SelectList
seq append DRecoTagSequence DRecoLambda0SelectList
catch { setProduction DRecoLambda0SelectList }
talkto DRecoLambda0SelectList {
  decayMode          set "Lambda0 -> p+ pi-"
  unrefinedListName  set "DRecoLambda0List"
  fittingAlgorithm   set "TreeFitter"
  fitConstraints     set "Mass"
}

######################################################################
######################################################################

# sigma0 -> lambda0 gamma

mod clone SmpMakerDefiner DRecoSigma0List
seq append DRecoTagSequence DRecoSigma0List
catch { setProduction DRecoSigma0List }
talkto DRecoSigma0List {
  decayMode         set "Sigma0 -> Lambda0 gamma"
  daughterListNames set "DRecoLambda0SelectList"
  daughterListNames set "DRecoPhotonList"
  fittingAlgorithm  set "Cascade"
  fitConstraints    set "Mass"
  fitConstraints    set "Momentum"
  preFitSelectors   set "Mass 1.180:1.200"
}

######################################################################
######################################################################


## # eta decays

# eta -> gamma gamma, mass constrained
mod clone SmpRefinerDefiner DRecoEtaggList
seq append DRecoTagSequence DRecoEtaggList
catch { setProduction DRecoEtaggList }
talkto DRecoEtaggList {
  decayMode          set "eta -> gamma gamma"
  unrefinedListName  set "etaggDefaultNoMass"
  daughterListNames  set "DRecoPhotonList"
  daughterListNames  set "DRecoPhotonList"
  fittingAlgorithm   set "Cascade"
  fitConstraints     set "Mass"
  fitConstraints     set "Momentum"
  fitConstraints     set "PrimaryVertex"
  preFitSelectors    set "Mass .480:.610"
  postFitSelectors   set "Mass .500:.590"
  postFitSelectors   set "ProbChiSq 0.001:"
}

# eta -> pi pi pi0, mass constrained
mod clone SmpRefinerDefiner DRecoEta3piList
seq append DRecoTagSequence DRecoEta3piList
catch { setProduction DRecoEta3piList }
talkto DRecoEta3piList {
  decayMode          set "eta -> pi+ pi- pi0"
  unrefinedListName  set "eta3piDefault"
  daughterListNames  set "GoodTracksLoose"
  daughterListNames  set "GoodTracksLoose"
  daughterListNames  set "DRecoPi0SelectList"
  fittingAlgorithm   set "Cascade"
  fitConstraints     set "Mass"
  fitConstraints     set "Momentum"
  fitConstraints     set "PrimaryVertex"
  preFitSelectors    set "Mass .515:.575"
  postFitSelectors   set "Mass .515:.575"
  postFitSelectors   set "ProbChiSq 0.001:"
}

# merged eta list
mod clone SmpMergerDefiner DRecoEtaList
seq append SmpEtaProdSequence DRecoEtaList
catch { setProduction DRecoEtaList }
talkto DRecoEtaList {
    debug             set f
    verbose           set f   
    inputListNames    set DRecoEtaggList
    inputListNames    set DRecoEta3piList  
}

######################################################################
######################################################################


## # eta' decays 
# eta' -> eta pi pi, no mass constraint
mod clone SmpMakerDefiner DRecoEtaPeppList
seq append DRecoTagSequence DRecoEtaPeppList
catch { setProduction DRecoEtaPeppList }
talkto DRecoEtaPeppList {
  decayMode          set "eta' -> eta pi+ pi-"
  daughterListNames  set "DRecoEtaList"
  daughterListNames  set "GoodTracksLoose"
  daughterListNames  set "GoodTracksLoose"
  fittingAlgorithm   set "Cascade"
#  fitConstraints     set "Mass"
  fitConstraints     set "Momentum"
  fitConstraints     set "PrimaryVertex"
  preFitSelectors   set "Mass pdg-0.050:pdg+0.050"
  postFitSelectors  set "Mass pdg-0.040:pdg+0.040"
  postFitSelectors   set "ProbChiSq 0.001:"
}
# mass constrained fit for narrow mass window list
mod clone SmpRefitterDefiner DRecoEtaPeppSelectList
seq append DRecoTagSequence DRecoEtaPeppSelectList
catch { setProduction DRecoEtaPeppSelectList }
talkto DRecoEtaPeppSelectList {
    decayMode          set "eta' -> eta pi+ pi-"
  unrefinedListName  set "DRecoEtaPeppList"
  fittingAlgorithm   set "Cascade"
  fitConstraints     set "Mass"
  fitConstraints     set "Momentum"
#  fitConstraints     set "PrimaryVertex"
  postFitSelectors   set "ProbChiSq 0.01:"
}

# eta' -> rho0 gamma, no mass constraint
mod clone SmpMakerDefiner DRecoEtaPrgList
seq append DRecoTagSequence DRecoEtaPrgList
catch { setProduction DRecoEtaPrgList }
talkto DRecoEtaPrgList {
  decayMode          set "eta' -> rho0 gamma"
  daughterListNames  set "DRecoRho0SelectList"
  daughterListNames  set "DRecoPhotonList"
  fittingAlgorithm   set "Cascade"
#  fitConstraints     set "Mass"
  fitConstraints     set "Momentum"
  fitConstraints     set "PrimaryVertex"
  preFitSelectors   set "Mass pdg-0.050:pdg+0.050"
  postFitSelectors  set "Mass pdg-0.040:pdg+0.040"
  postFitSelectors   set "ProbChiSq 0.001:"
}
# mass constrained fit for narrow mass window list
mod clone SmpRefitterDefiner DRecoEtaPrgSelectList
seq append DRecoTagSequence DRecoEtaPrgSelectList
catch { setProduction DRecoEtaPrgSelectList }
talkto DRecoEtaPrgSelectList {
    decayMode          set "eta' -> eta pi+ pi-"
  unrefinedListName  set "DRecoEtaPrgList"
  fittingAlgorithm   set "Cascade"
  fitConstraints     set "Mass"
  fitConstraints     set "Momentum"
#  fitConstraints     set "PrimaryVertex"
  postFitSelectors   set "ProbChiSq 0.01:"
}

# merged eta' list
mod clone SmpMergerDefiner DRecoEtaPSelectList
seq append SmpEtaProdSequence DRecoEtaPSelectList
catch { setProduction DRecoEtaPSelectList }
talkto DRecoEtaPSelectList {
    debug             set f
    verbose           set f   
    inputListNames    set DRecoEtaPeppSelectList
    inputListNames    set DRecoEtaPrgSelectList
}
######################################################################
######################################################################

# rhoC -> pi pi0

mod clone SmpMakerDefiner DRecoRhoCList
seq append DRecoTagSequence DRecoRhoCList
catch { setProduction DRecoRhoCList }
talkto DRecoRhoCList {
  decayMode         set "rho+ -> pi+ pi0"
  daughterListNames set "GoodTracksVeryLoose"
  daughterListNames set "DRecoPi0SelectList"
  preFitSelectors   set "Mass 0.450:1.100"
  fittingAlgorithm  set "TreeFitter"
  fitConstraints    set "Beam"
  postFitSelectors  set "Mass pdg-0.160:pdg+0.160"
  postFitSelectors  set "ProbChiSq 0.001:"
}

# # mass constrained fit for narrow mass window list
# mod clone SmpRefitterDefiner DRecoRhoCSelectList
# seq append DRecoTagSequence DRecoRhoCSelectList
# catch { setProduction DRecoRhoCSelectList }
# talkto DRecoRhoCSelectList {
#   decayMode          set "rho+ -> pi+ pi0"
#   unrefinedListName  set "DRecoRhoCList"
#   fittingAlgorithm   set "TreeFitter"
# #  fitConstraints     set "Mass"
# }

######################################################################
######################################################################

# rho0 -> pi pi

mod clone SmpMakerDefiner DRecoRho0List
seq append DRecoTagSequence DRecoRho0List
catch { setProduction DRecoRho0List }
talkto DRecoRho0List {
  decayMode         set "rho0 -> pi+ pi-"
  daughterListNames set "GoodTracksVeryLoose"
  daughterListNames set "GoodTracksVeryLoose"
  preFitSelectors   set "Mass 0.450:1.100"
  fittingAlgorithm  set "TreeFitter"
  postFitSelectors  set "Mass pdg-0.160:pdg+0.160"
  postFitSelectors  set "ProbChiSq 0.001:"
}

# mass constrained fit for narrow mass window list
mod clone SmpRefitterDefiner DRecoRho0SelectList
seq append DRecoTagSequence DRecoRho0SelectList
catch { setProduction DRecoRho0SelectList }
talkto DRecoRho0SelectList {
  decayMode          set "rho0 -> pi+ pi-"
  unrefinedListName  set "DRecoRho0List"
  fittingAlgorithm   set "TreeFitter"
#  fitConstraints     set "Mass"
}

######################################################################
# ######################################################################

# # f_0 -> pi pi

# mod clone SmpMakerDefiner DRecof0List
# seq append DRecoTagSequence DRecof0List
# catch { setProduction DRecof0List }
# talkto DRecof0List {
#   decayMode         set "f_0 -> pi+ pi-"
#   daughterListNames set "piKMSuperLoose"
#   daughterListNames set "piKMSuperLoose"
#   preFitSelectors   set "Mass 0.580:1.380"
#   fittingAlgorithm  set "TreeFitter"
#   postFitSelectors  set "Mass 0.830:1.130"
#   postFitSelectors  set "ProbChiSq 0.001:"
# }

# # mass constrained fit for narrow mass window list
# mod clone SmpRefitterDefiner DRecof0SelectList
# seq append DRecoTagSequence DRecof0SelectList
# catch { setProduction DRecof0SelectList }
# talkto DRecof0SelectList {
#   decayMode          set "f_0 -> pi+ pi-"
#   unrefinedListName  set "DRecof0List"
#   fittingAlgorithm   set "TreeFitter"
#   fitConstraints     set "Mass"
# }

# ######################################################################