set looseHalfWinWidth          "0.160"
set PostPCM                    "CmsP 1.800:"

# D0 -> K pi
mod clone SmpMakerDefiner DRecoD0ToKPi
seq append DRecoTagSequence DRecoD0ToKPi
catch { setProduction DRecoD0ToKPi }
talkto DRecoD0ToKPi {
    decayMode         set "D0 -> K- pi+"
    #daughterListNames set "KCombinedSuperLoose"
    #daughterListNames set "GoodTracksVeryLoose"    
    daughterListNames set "KBDTKaonNotPion"
    daughterListNames set "GoodTracksVeryLoose"    

    fittingAlgorithm  set "Add4"
    postFitSelectors  set "Mass pdg-$looseHalfWinWidth:pdg+$looseHalfWinWidth"
    postFitSelectors  set $PostPCM
}

# D0 -> K pi pi0
mod clone SmpMakerDefiner DRecoD0ToKPiPi0
seq append DRecoTagSequence DRecoD0ToKPiPi0
catch { setProduction DRecoD0ToKPiPi0 }
talkto DRecoD0ToKPiPi0 {
	decayMode         set "D0 -> K- pi+ pi0"
	#daughterListNames set "KCombinedSuperLoose"
	#daughterListNames set "GoodTracksVeryLoose"
	#daughterListNames set "pi0AllDefault"
	daughterListNames set "KBDTKaonNotPion"
	daughterListNames set "GoodTracksVeryLoose"
	daughterListNames set "DRecoPi0SelectList"

    fittingAlgorithm  set "Add4"
    postFitSelectors  set "Mass pdg-$looseHalfWinWidth:pdg+$looseHalfWinWidth"
    postFitSelectors  set $PostPCM
}

# D0 -> K0_S pi+ pi-
mod clone SmpMakerDefiner DRecoD0ToKsPiPi
seq append DRecoTagSequence DRecoD0ToKsPiPi
catch { setProduction DRecoD0ToKsPiPi }
talkto DRecoD0ToKsPiPi {
    decayMode         set "D0 -> K_S0 pi+ pi-"
    #daughterListNames set KsDefault
    #daughterListNames set GoodTracksVeryLoose
    #daughterListNames set GoodTracksVeryLoose
    daughterListNames set DRecoKShortMassList
    daughterListNames set GoodTracksVeryLoose
    daughterListNames set GoodTracksVeryLoose

    fittingAlgorithm  set "Add4"
    postFitSelectors  set "Mass pdg-$looseHalfWinWidth:pdg+$looseHalfWinWidth"
    postFitSelectors  set $PostPCM
}

# D0 -> K 3pi
mod clone SmpMakerDefiner DRecoD0ToK3Pi
seq append DRecoTagSequence DRecoD0ToK3Pi
catch { setProduction DRecoD0ToK3Pi }
talkto DRecoD0ToK3Pi {
	decayMode         set "D0 -> K- pi+ pi- pi+"
	daughterListNames set GoodTracksLoose
	daughterListNames set GoodTracksVeryLoose
	daughterListNames set GoodTracksVeryLoose
	daughterListNames set GoodTracksVeryLoose

    fittingAlgorithm  set "Add4"
    postFitSelectors  set "Mass pdg-$looseHalfWinWidth:pdg+$looseHalfWinWidth"
    postFitSelectors  set $PostPCM
}

# D0 -> K0_S pi+ pi- pi0
mod clone SmpMakerDefiner DRecoD0ToKsPiPiPi0
seq append DRecoTagSequence DRecoD0ToKsPiPiPi0
catch { setProduction DRecoD0ToKsPiPiPi0 }
talkto DRecoD0ToKsPiPiPi0 {
  decayMode         set "D0 -> K_S0 pi+ pi- pi0"
  #daughterListNames set KsLoose
  #daughterListNames set piCombinedVeryLoose
  #daughterListNames set piCombinedVeryLoose
  #daughterListNames set pi0VeryLoose
  daughterListNames set DRecoKShortMassList
  daughterListNames set piKMVeryLoose
  daughterListNames set piKMVeryLoose
  daughterListNames set DRecoPi0SelectList

    fittingAlgorithm  set "Add4"
    postFitSelectors  set "Mass pdg-$looseHalfWinWidth:pdg+$looseHalfWinWidth"
    postFitSelectors  set $PostPCM
}

# D0 -> K pi pi pi pi0
mod clone SmpMakerDefiner DRecoD0ToK3PiPi0
seq append DRecoTagSequence DRecoD0ToK3PiPi0
catch { setProduction DRecoD0ToK3PiPi0 }
talkto DRecoD0ToK3PiPi0 {
  decayMode         set "D0 -> K- pi+ pi- pi+ pi0"
  #daughterListNames set KCombinedSuperLoose
  #daughterListNames set piCombinedVeryLoose
  #daughterListNames set piCombinedVeryLoose
  #daughterListNames set piCombinedVeryLoose
  #daughterListNames set pi0VeryLoose
  daughterListNames set KBDTKaonVeryLoose
  daughterListNames set piKMVeryLoose
  daughterListNames set piKMVeryLoose
  daughterListNames set piKMVeryLoose
  daughterListNames set DRecoPi0SelectList

    fittingAlgorithm  set "Add4"
    postFitSelectors  set "Mass pdg-$looseHalfWinWidth:pdg+$looseHalfWinWidth"
    postFitSelectors  set $PostPCM
}
