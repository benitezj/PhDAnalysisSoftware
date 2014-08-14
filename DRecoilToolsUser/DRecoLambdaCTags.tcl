set looseHalfWinWidth          "0.160"
set PostPCM                    "CmsP 1.800:"

# Lambda_c+ -> p+ K- pi+
mod clone SmpMakerDefiner DRecoLambdaCTopKpi
seq append DRecoTagSequence DRecoLambdaCTopKpi
catch { setProduction DRecoLambdaCTopKpi }
talkto DRecoLambdaCTopKpi {
    decayMode         set "Lambda_c+ -> p+ K- pi+"
    #daughterListNames set "pCombinedSuperLoose"
    #daughterListNames set "KCombinedSuperLoose"    
    #daughterListNames set "ChargedTracks"
    daughterListNames set "pKMSuperLoose"
    daughterListNames set "KBDTKaonNotPion"    
    daughterListNames set "GoodTracksVeryLoose"

    fittingAlgorithm  set "Add4"
    postFitSelectors  set "Mass pdg-$looseHalfWinWidth:pdg+$looseHalfWinWidth"
    postFitSelectors  set $PostPCM
}

# Lambda_c+ -> p+ K- pi+ pi0
mod clone SmpMakerDefiner DRecoLambdaCTopKpipi0
seq append DRecoTagSequence DRecoLambdaCTopKpipi0
catch { setProduction DRecoLambdaCTopKpipi0 }
talkto DRecoLambdaCTopKpipi0 {
    decayMode         set "Lambda_c+ -> p+ K- pi+ pi0"
    #daughterListNames set "pCombinedSuperLoose"
    #daughterListNames set "KCombinedSuperLoose"
    #daughterListNames set "ChargedTracks"
    #daughterListNames set "pi0Loose"
    daughterListNames set "pKMSuperLoose"
    daughterListNames set "KBDTKaonNotPion"
    daughterListNames set "GoodTracksVeryLoose"
    daughterListNames set "DRecoPi0SelectList"

    fittingAlgorithm  set "Add4"
    postFitSelectors  set "Mass pdg-$looseHalfWinWidth:pdg+$looseHalfWinWidth"
    postFitSelectors  set $PostPCM
}

# Lambda_c+ -> p+ Ks
mod clone SmpMakerDefiner DRecoLambdaCTopKs
seq append DRecoTagSequence DRecoLambdaCTopKs
catch { setProduction DRecoLambdaCTopKs }
talkto DRecoLambdaCTopKs {
    decayMode         set "Lambda_c+ -> p+ K_S0"
    #daughterListNames set "pCombinedSuperLoose"
    #daughterListNames set "KsLoose_LambdaC"    
    daughterListNames set "pKMSuperLoose"
    daughterListNames set "DRecoKShortMassList"    

    fittingAlgorithm  set "Add4"
    postFitSelectors  set "Mass pdg-$looseHalfWinWidth:pdg+$looseHalfWinWidth"
    postFitSelectors  set $PostPCM
}

# Lambda_c+ -> p+ Ks pi+ pi-
mod clone SmpMakerDefiner DRecoLambdaCTopKspipi
seq append DRecoTagSequence DRecoLambdaCTopKspipi
catch { setProduction DRecoLambdaCTopKspipi }
talkto DRecoLambdaCTopKspipi {
    decayMode         set "Lambda_c+ -> p+ K_S0 pi+ pi-"
    #daughterListNames set "pCombinedSuperLoose"
    #daughterListNames set "KsLoose_LambdaC"
    #daughterListNames set "ChargedTracks"
    #daughterListNames set "ChargedTracks"
    daughterListNames set "pKMSuperLoose"
    daughterListNames set "DRecoKShortMassList"
    daughterListNames set "GoodTracksVeryLoose"
    daughterListNames set "GoodTracksVeryLoose"

    fittingAlgorithm  set "Add4"
    postFitSelectors  set "Mass pdg-$looseHalfWinWidth:pdg+$looseHalfWinWidth"
    postFitSelectors  set $PostPCM
}

# Lambda_c+ -> Lambda0 pi+
mod clone SmpMakerDefiner DRecoLambdaCToLzpi
seq append DRecoTagSequence DRecoLambdaCToLzpi
catch { setProduction DRecoLambdaCToLzpi }
talkto DRecoLambdaCToLzpi {
    decayMode         set "Lambda_c+ -> Lambda0 pi+"
    #daughterListNames set "Lambda_LambdaC"
    #daughterListNames set "ChargedTracks"    
    daughterListNames set "DRecoLambda0SelectList"
    daughterListNames set "GoodTracksVeryLoose"    

    fittingAlgorithm  set "Add4"
    postFitSelectors  set "Mass pdg-$looseHalfWinWidth:pdg+$looseHalfWinWidth"
    postFitSelectors  set $PostPCM
}

# Lambda_c+ -> Lambda0 pi+ pi0
mod clone SmpMakerDefiner DRecoLambdaCToLzpipi0
seq append DRecoTagSequence DRecoLambdaCToLzpipi0
catch { setProduction DRecoLambdaCToLzpipi0 }
talkto DRecoLambdaCToLzpipi0 {
    decayMode         set "Lambda_c+ -> Lambda0 pi+ pi0"
    #daughterListNames set "Lambda_LambdaC"
    #daughterListNames set "ChargedTracks"
    #daughterListNames set "pi0Loose"    
    daughterListNames set "DRecoLambda0SelectList"
    daughterListNames set "GoodTracksVeryLoose"
    daughterListNames set "DRecoPi0SelectList"    

    fittingAlgorithm  set "Add4"
    postFitSelectors  set "Mass pdg-$looseHalfWinWidth:pdg+$looseHalfWinWidth"
    postFitSelectors  set $PostPCM
}

# Lambda_c+ -> Lambda0 pi+ pi+ pi-
mod clone SmpMakerDefiner DRecoLambdaCToLzpipipi
seq append DRecoTagSequence DRecoLambdaCToLzpipipi
catch { setProduction DRecoLambdaCToLzpipipi }
talkto DRecoLambdaCToLzpipipi {
    decayMode         set "Lambda_c+ -> Lambda0 pi+ pi+ pi-"
    daughterListNames set "DRecoLambda0SelectList"
    daughterListNames set "GoodTracksVeryLoose"
    daughterListNames set "GoodTracksVeryLoose"
    daughterListNames set "GoodTracksVeryLoose"

    fittingAlgorithm  set "Add4"
    postFitSelectors  set "Mass pdg-$looseHalfWinWidth:pdg+$looseHalfWinWidth"
    postFitSelectors  set $PostPCM
}

# Lambda_c+ -> Sigma0 pi+
mod clone SmpMakerDefiner DRecoLambdaCToSzpi
seq append DRecoTagSequence DRecoLambdaCToSzpi
catch { setProduction DRecoLambdaCToSzpi }
talkto DRecoLambdaCToSzpi {
    decayMode         set "Lambda_c+ -> Sigma0 pi+"
    #daughterListNames set "Sigma_LambdaC"
    #daughterListNames set "GoodTracksVeryLoose"
    daughterListNames set "DRecoSigma0List"
    daughterListNames set "GoodTracksVeryLoose"

    fittingAlgorithm  set "Add4"
    postFitSelectors  set "Mass pdg-$looseHalfWinWidth:pdg+$looseHalfWinWidth"
    postFitSelectors  set $PostPCM
}

# Lambda_c+ -> Sigma0 pi+ pi+ pi-
mod clone SmpMakerDefiner DRecoLambdaCToSzpipipi
seq append DRecoTagSequence DRecoLambdaCToSzpipipi
catch { setProduction DRecoLambdaCToSzpipipi }
talkto DRecoLambdaCToSzpipipi {
    decayMode         set "Lambda_c+ -> Sigma0 pi+ pi+ pi-"
    #daughterListNames set "Sigma_LambdaC"
    #daughterListNames set "GoodTracksVeryLoose"
    #daughterListNames set "GoodTracksVeryLoose"
    #daughterListNames set "GoodTracksVeryLoose"
    daughterListNames set "DRecoSigma0List"
    daughterListNames set "GoodTracksVeryLoose"
    daughterListNames set "GoodTracksVeryLoose"
    daughterListNames set "GoodTracksVeryLoose"

    fittingAlgorithm  set "Add4"
    postFitSelectors  set "Mass pdg-$looseHalfWinWidth:pdg+$looseHalfWinWidth"
    postFitSelectors  set $PostPCM
}
