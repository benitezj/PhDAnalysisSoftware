set looseHalfWinWidth          "0.160"
set PostPCM                    "CmsP 1.800:"

# D+ -> K0_S pi+
mod clone SmpMakerDefiner DRecoDPlusToKsPi
seq append DRecoTagSequence DRecoDPlusToKsPi
catch { setProduction DRecoDPlusToKsPi }
talkto DRecoDPlusToKsPi {
    decayMode          set "D+ -> K_S0 pi+"
    #daughterListNames  set KsDefault
    #daughterListNames  set GoodTracksVeryLoose
    daughterListNames  set DRecoKShortMassList
    daughterListNames  set GoodTracksVeryLoose

    fittingAlgorithm  set "Add4"
    postFitSelectors  set "Mass pdg-$looseHalfWinWidth:pdg+$looseHalfWinWidth"
    postFitSelectors  set $PostPCM
}

# D+ -> K0_S pi+ pi0
mod clone SmpMakerDefiner DRecoDPlusToKsPiPi0
seq append DRecoTagSequence DRecoDPlusToKsPiPi0
catch { setProduction DRecoDPlusToKsPiPi0 }
talkto DRecoDPlusToKsPiPi0 {
    decayMode          set "D+ -> K_S0 pi+ pi0"
    #daughterListNames  set KsDefault
    #daughterListNames  set GoodTracksVeryLoose
    #daughterListNames  set DRecoPi0SelectList
    daughterListNames  set DRecoKShortMassList
    daughterListNames  set GoodTracksVeryLoose
    daughterListNames  set DRecoPi0SelectList

    fittingAlgorithm  set "Add4"
    postFitSelectors  set "Mass pdg-$looseHalfWinWidth:pdg+$looseHalfWinWidth"
    postFitSelectors  set $PostPCM
}

# D+ -> K- pi+ pi+
mod clone SmpMakerDefiner DRecoDPlusToKPiPi
seq append DRecoTagSequence DRecoDPlusToKPiPi
catch { setProduction DRecoDPlusToKPiPi }
talkto DRecoDPlusToKPiPi {
    disableCloneCheck set true
    decayMode         set "D+ -> K- pi+ pi+"
    #daughterListNames set "KCombinedSuperLoose"
    #daughterListNames set "GoodTracksVeryLoose"
    #daughterListNames set "GoodTracksVeryLoose"
    daughterListNames set "KBDTKaonNotPion"
    daughterListNames set "GoodTracksVeryLoose"
    daughterListNames set "GoodTracksVeryLoose"

    fittingAlgorithm  set "Add4"
    postFitSelectors  set "Mass pdg-$looseHalfWinWidth:pdg+$looseHalfWinWidth"
    postFitSelectors  set $PostPCM
}

# D+ -> K- pi+ pi+ pi0
mod clone SmpMakerDefiner DRecoDPlusToKPiPiPi0
seq append DRecoTagSequence DRecoDPlusToKPiPiPi0
catch { setProduction DRecoDPlusToKPiPiPi0 }
talkto DRecoDPlusToKPiPiPi0 {
    decayMode          set "D+ -> K- pi+ pi+ pi0"
    #daughterListNames  set KCombinedSuperLoose
    #daughterListNames  set GoodTracksVeryLoose
    #daughterListNames  set GoodTracksVeryLoose
    #daughterListNames  set pi0AllDefault
    daughterListNames  set KBDTKaonNotPion
    daughterListNames  set GoodTracksVeryLoose
    daughterListNames  set GoodTracksVeryLoose
    daughterListNames  set DRecoPi0SelectList

    fittingAlgorithm  set "Add4"
    postFitSelectors  set "Mass pdg-$looseHalfWinWidth:pdg+$looseHalfWinWidth"
    postFitSelectors  set $PostPCM
}

# D+ -> K0_S pi+ pi- pi+
mod clone SmpMakerDefiner DRecoDPlusToKs3Pi
seq append DRecoTagSequence DRecoDPlusToKs3Pi
catch { setProduction DRecoDPlusToKs3Pi }
talkto DRecoDPlusToKs3Pi {
    decayMode          set "D+ -> K_S0 pi+ pi+ pi-"
    #daughterListNames  set KsDefault
    #daughterListNames  set GoodTracksVeryLoose
    #daughterListNames  set ChargedTracks
    #daughterListNames  set ChargedTracks
    daughterListNames  set DRecoKShortMassList
    daughterListNames  set GoodTracksVeryLoose
    daughterListNames  set ChargedTracks
    daughterListNames  set ChargedTracks

    fittingAlgorithm  set "Add4"
    postFitSelectors  set "Mass pdg-$looseHalfWinWidth:pdg+$looseHalfWinWidth"
    postFitSelectors  set $PostPCM
}
