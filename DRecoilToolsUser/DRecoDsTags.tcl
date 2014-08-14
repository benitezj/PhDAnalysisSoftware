set looseHalfWinWidth          "0.160"
set PostPCM                    "CmsP 1.800:"

# Ds -> Ks K
mod clone SmpMakerDefiner DRecoDsToKsK
seq append DRecoTagSequence DRecoDsToKsK
catch { setProduction DRecoDsToKsK }
talkto DRecoDsToKsK {
    disableCloneCheck set true
    decayMode         set "D_s+ -> K_S0 K+"
    #daughterListNames set "KsDefault"
    #daughterListNames set "KCombinedSuperLoose"
    daughterListNames set "DRecoKShortMassList"
    daughterListNames set "KBDTKaonNotPion"

    fittingAlgorithm  set "Add4"
    postFitSelectors  set "Mass pdg-$looseHalfWinWidth:pdg+$looseHalfWinWidth"
    postFitSelectors  set $PostPCM
}

# Ds -> phi Pi
mod clone SmpMakerDefiner DRecoDsToPhiPi
seq append DRecoTagSequence DRecoDsToPhiPi
catch { setProduction DRecoDsToPhiPi }
talkto DRecoDsToPhiPi {
    decayMode         set "D_s+ -> phi pi+"
    #daughterListNames set "KCombinedSuperLoose"
    #daughterListNames set "KCombinedSuperLoose"
    #daughterListNames set "GoodTracksVeryLoose"
    daughterListNames set "DRecoPhiMassList" 
    daughterListNames set "GoodTracksVeryLoose"

    fittingAlgorithm  set "Add4"
    postFitSelectors  set "Mass pdg-$looseHalfWinWidth:pdg+$looseHalfWinWidth"
    postFitSelectors  set $PostPCM
}

# Ds -> K*0 K
mod clone SmpMakerDefiner DRecoDsToKstar0K
seq append DRecoTagSequence DRecoDsToKstar0K
catch { setProduction DRecoDsToKstar0K }
talkto DRecoDsToKstar0K {
    decayMode         set "D_s+ -> K*0 K+"
    #daughterListNames set "KCombinedSuperLoose"
    #daughterListNames set "KCombinedSuperLoose"
    #daughterListNames set "GoodTracksVeryLoose"
    daughterListNames set "DRecoKstar0List" 
    daughterListNames set "KBDTKaonNotPion"

    fittingAlgorithm  set "Add4"
    postFitSelectors  set "Mass pdg-$looseHalfWinWidth:pdg+$looseHalfWinWidth"
    postFitSelectors  set $PostPCM
}

# Ds -> Phi RhoC
mod clone SmpMakerDefiner DRecoDsToPhiRhoC
seq append DRecoTagSequence DRecoDsToPhiRhoC
catch { setProduction DRecoDsToPhiRhoC }
talkto DRecoDsToPhiRhoC {
    decayMode         set "D_s+ -> phi rho+"
    #daughterListNames set "phiDefaultPID"
    #daughterListNames set "rhoCTight"
    daughterListNames set "DRecoPhiListOldPID"
    daughterListNames set "DRecoRhoCList"

    fittingAlgorithm  set "Add4"
    postFitSelectors  set "Mass pdg-$looseHalfWinWidth:pdg+$looseHalfWinWidth"
    postFitSelectors  set $PostPCM
}

# Ds -> Ks K Pi Pi (favoured and suppressed)
#favoured
mod clone SmpMakerDefiner DRecoDsToKsKPiPifav
seq append DRecoTagSequence DRecoDsToKsKPiPifav
catch { setProduction DRecoDsToKsKPiPifav }
talkto DRecoDsToKsKPiPifav {
    decayMode         set "D_s+ -> K_S0 K- pi+ pi+" 
    #daughterListNames set "KsDefault"
    #daughterListNames set "GoodTracksLoose"
    #daughterListNames set "GoodTracksVeryLoose"
    #daughterListNames set "GoodTracksVeryLoose"
    daughterListNames set "DRecoKShortMassList"
    daughterListNames set "KBDTKaonNotPion"
    daughterListNames set "GoodTracksVeryLoose"
    daughterListNames set "GoodTracksVeryLoose"

    fittingAlgorithm  set "Add4"
    postFitSelectors  set "Mass pdg-$looseHalfWinWidth:pdg+$looseHalfWinWidth"
    postFitSelectors  set $PostPCM
}
#suppressed
mod clone SmpMakerDefiner DRecoDsToKsKPiPisup
seq append DRecoTagSequence DRecoDsToKsKPiPisup
catch { setProduction DRecoDsToKsKPiPisup }
talkto DRecoDsToKsKPiPisup {
    decayMode         set "D_s+ -> K_S0 K+ pi+ pi-"
    #daughterListNames set "KsDefault"
    #daughterListNames set "KCombinedSuperLoose"
    #daughterListNames set "GoodTracksVeryLoose"
    #daughterListNames set "GoodTracksVeryLoose"
    daughterListNames set "DRecoKShortMassList"
    daughterListNames set "KBDTKaonNotPion"
    daughterListNames set "GoodTracksVeryLoose"
    daughterListNames set "GoodTracksVeryLoose"
}
mod clone SmpMergerDefiner DRecoDsToKsKPiPi
seq append DRecoTagSequence DRecoDsToKsKPiPi
catch { setProduction DRecoDsToKsKPiPi }
talkto DRecoDsToKsKPiPi {
    ## disableCloneCheck=true is necessary to keep both the
    ## right-sign and wrong-sign combinations
    disableCloneCheck set true
    inputListNames    set "DRecoDsToKsKPiPifav"
    inputListNames    set "DRecoDsToKsKPiPisup"
}


# Ds -> Phi rho0 Pi
mod clone SmpMakerDefiner DRecoDsToPhiRho0Pi
seq append DRecoTagSequence DRecoDsToPhiRho0Pi
catch { setProduction DRecoDsToPhiRho0Pi }
talkto DRecoDsToPhiRho0Pi {
    decayMode         set "D_s+ -> phi rho0 pi+"
    #daughterListNames set "phiDefaultPID"
    #daughterListNames set "GoodTracksVeryLoose"
    #daughterListNames set "GoodTracksVeryLoose"
    #daughterListNames set "GoodTracksVeryLoose"
    daughterListNames set "DRecoPhiListOldPID"
    daughterListNames set "DRecoRho0SelectList"
    daughterListNames set "GoodTracksVeryLoose"

    fittingAlgorithm  set "Add4"
    postFitSelectors  set "Mass pdg-$looseHalfWinWidth:pdg+$looseHalfWinWidth"
    postFitSelectors  set $PostPCM
}

# Ds -> Eta' Pi
mod clone SmpMakerDefiner DRecoDsToEtaPPi
seq append DRecoTagSequence DRecoDsToEtaPPi
catch { setProduction DRecoDsToEtaPPi }
talkto DRecoDsToEtaPPi {
    decayMode         set "D_s+ -> eta' pi+"
    #daughterListNames set "etaPDefault"
    #daughterListNames set "GoodTracksVeryLoose"
    daughterListNames set "DRecoEtaPSelectList"
    daughterListNames set "GoodTracksVeryLoose"

    fittingAlgorithm  set "Add4"
    postFitSelectors  set "Mass pdg-$looseHalfWinWidth:pdg+$looseHalfWinWidth"
    postFitSelectors  set $PostPCM
}


# Ds -> Eta RhoC
mod clone SmpMakerDefiner DRecoDsToEtaRhoC
seq append DRecoTagSequence DRecoDsToEtaRhoC
catch { setProduction DRecoDsToEtaRhoC }
talkto DRecoDsToEtaRhoC {
    decayMode         set "D_s+ -> eta rho+"
    #daughterListNames set "etaDefault"
    #daughterListNames set "rhoCTight"
    daughterListNames set "DRecoEtaList"
    daughterListNames set "DRecoRhoCList"

    fittingAlgorithm  set "Add4"
    postFitSelectors  set "Mass pdg-$looseHalfWinWidth:pdg+$looseHalfWinWidth"
    postFitSelectors  set $PostPCM
}

# Ds -> Eta' RhoC
mod clone SmpMakerDefiner DRecoDsToEtaPRhoC
seq append DRecoTagSequence DRecoDsToEtaPRhoC
catch { setProduction DRecoDsToEtaPRhoC }
talkto DRecoDsToEtaPRhoC {
    decayMode         set "D_s+ -> eta' rho+"
    #daughterListNames set "etaPeppDefault"
    #daughterListNames set "rhoCTight"
    daughterListNames set "DRecoEtaPSelectList"
    daughterListNames set "DRecoRhoCList"

    fittingAlgorithm  set "Add4"
    postFitSelectors  set "Mass pdg-$looseHalfWinWidth:pdg+$looseHalfWinWidth"
    postFitSelectors  set $PostPCM
}

# Ds -> Eta Pi
mod clone SmpMakerDefiner DRecoDsToEtaPi
seq append DRecoTagSequence DRecoDsToEtaPi
catch { setProduction DRecoDsToEtaPi }
talkto DRecoDsToEtaPi {
    decayMode         set "D_s+ -> eta pi+"
    #daughterListNames set "etaDefault"
    #daughterListNames set "GoodTracksVeryLoose"
    daughterListNames set "DRecoEtaList"
    daughterListNames set "GoodTracksVeryLoose"

    fittingAlgorithm  set "Add4"
    postFitSelectors  set "Mass pdg-$looseHalfWinWidth:pdg+$looseHalfWinWidth"
    postFitSelectors  set $PostPCM
}


# Ds -> Pi Pi Pi
mod clone SmpMakerDefiner DRecoDsTo3Pi
seq append DRecoTagSequence DRecoDsTo3Pi
catch { setProduction DRecoDsTo3Pi }
talkto DRecoDsTo3Pi {
    decayMode         set "D_s+ -> pi+ pi+ pi-"
    #daughterListNames set "piCombinedSuperLoose"
    #daughterListNames set "piCombinedSuperLoose"
    #daughterListNames set "piCombinedSuperLoose"
    #daughterListNames set "DRecof0SelectList"
    daughterListNames set "piKMSuperLoose"
    daughterListNames set "piKMSuperLoose"
    daughterListNames set "piKMSuperLoose"

    fittingAlgorithm  set "Add4"
    postFitSelectors  set "Mass pdg-$looseHalfWinWidth:pdg+$looseHalfWinWidth"
    postFitSelectors  set $PostPCM
}

