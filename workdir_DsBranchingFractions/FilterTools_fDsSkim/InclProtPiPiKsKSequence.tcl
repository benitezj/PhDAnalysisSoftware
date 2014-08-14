sequence create InclProtPiPiKsKSequence
     
mod clone SmpSubListerDefiner MyKStarChgDefault
sequence append InclProtPiPiKsKSequence MyKStarChgDefault
catch { setProduction MyKStarChgDefault}
talkto MyKStarChgDefault {
    decayMode set "K*+ -> K_S0 pi+"
    unrefinedListName set "KstarKsPiVeryLoose"
    daughterListNames set "KsDefaultMass"
    daughterListNames set "piLHVeryLoose"
    selectors set "Mass 0.7166:1.0666"
}

mod clone SmpSubListerDefiner MyKStarChgTight
sequence append InclProtPiPiKsKSequence MyKStarChgTight
catch { setProduction MyKStarChgTight }
talkto MyKStarChgTight {
    decayMode set "K*+ -> K_S0 pi+"
    unrefinedListName set "MyKStarChgDefault"
    daughterListNames set "KsTight"
    daughterListNames set "piLHLoose"
}

mod clone SmpMakerDefiner InclProtPiPiKsK
sequence append InclProtPiPiKsKSequence InclProtPiPiKsK;
catch { setProduction InclProtPiPiKsK }
talkto InclProtPiPiKsK {
    decayMode set "Omega_c0 -> K_S0 pi- pi- p+ K+"
    daughterListNames set "KsDefaultMass"
    daughterListNames set "piLHVeryLoose"
    daughterListNames set "piLHVeryLoose"
    daughterListNames set "pLHLoose"
    daughterListNames set "KLHLoose"
    fittingAlgorithm set "Add4"
}

mod clone SmpSubListerDefiner InclProtPiPiKsKTight
sequence append InclProtPiPiKsKSequence InclProtPiPiKsKTight
catch { setProduction InclProtPiPiKsKTight }
talkto InclProtPiPiKsKTight {
    decayMode set "Omega_c0 -> K_S0 pi- pi- p+ K+"
    unrefinedListName set "InclProtPiPiKsK"
    daughterListNames set "KsTight"
    daughterListNames set "piLHLoose"
    daughterListNames set "piLHLoose"
    daughterListNames set "pLHTight"
    daughterListNames set "KLHTight"
}

mod clone SmpMakerDefiner InclProtPiKstarK
sequence append InclProtPiPiKsKSequence InclProtPiKstarK
catch { setProduction InclProtPiKstarK }
talkto InclProtPiKstarK {
    decayMode set "Omega_c0 -> K*- pi- p+ K+"
    daughterListNames set "MyKStarChgDefault"
    daughterListNames set "piLHVeryLoose"
    daughterListNames set "pLHLoose"
    daughterListNames set "KLHLoose"
    fittingAlgorithm set "Add4"
}

mod clone SmpSubListerDefiner InclProtPiKstarKTight
sequence append InclProtPiPiKsKSequence InclProtPiKstarKTight
catch { setProduction InclProtPiKstarKTight }
talkto InclProtPiKstarKTight {
    decayMode set "Omega_c0 -> K*- pi- p+ K+"
    unrefinedListName set "InclProtPiKstarK"
    daughterListNames set "MyKStarChgTight"
    daughterListNames set "piLHLoose"
    daughterListNames set "pLHTight"
    daughterListNames set "KLHTight"
}
