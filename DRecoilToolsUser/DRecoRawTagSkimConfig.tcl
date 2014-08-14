#
#  Define the fDs path
#
#The fDs is a merge of the lists from 7 other skims, which are;
#
#1 D0To2ProngDcj
#2 D0ToKnPiPi0
#3 DRecoDmesonTag
#4 DRecoDsmesonTag
#5 DcTo3PiPrompt
#6 DcToKpipiPrompt
#7 LambdaC
#8 DcToKKPiPrompt
#

#For this file
#D0, Dc, Ds and LambdaC settings 
set looseHalfWinWidth            0.160
set tightHalfWinWidth           0.100
set PrePCM                     "CmsP 1.800:"
set PostPCM                    "CmsP 2.000:"
#Remember, TreeFitter DOES NOT need the Geo constraint whereas Cascade DOES.
#set Vtx_Fitter                 "TreeFitter" 
set Vtx_Fitter                 "Cascade"
set ProbChiSq                  "ProbChiSq 0.00001:"


sequence create fDsSequence


##################D0 

mod clone SmpMakerDefiner D0ToKsPimPipPi0
sequence append fDsSequence D0ToKsPimPipPi0
talkto D0ToKsPimPipPi0 {
    decayMode          set "D0 -> K_S0 pi- pi+ pi0"
    daughterListNames  set "KsLoose"
    daughterListNames  set "piCombinedVeryLoose"
    daughterListNames  set "piCombinedVeryLoose"
    daughterListNames  set "pi0VeryLoose"
    fittingAlgorithm  set "Add4"
    postFitSelectors  set "Mass pdg-$looseHalfWinWidth:pdg+$looseHalfWinWidth"  
}

mod clone SmpMakerDefiner D0ToKmPipPipPimPi0
sequence append fDsSequence D0ToKmPipPipPimPi0
talkto D0ToKmPipPipPimPi0 {
    decayMode          set "D0 -> K- pi+ pi+ pi- pi0"
    daughterListNames  set "KCombinedVeryLoose"
    daughterListNames  set "piCombinedVeryLoose"
    daughterListNames  set "piCombinedVeryLoose"
    daughterListNames  set "piCombinedVeryLoose"
    daughterListNames  set "pi0VeryLoose"
    fittingAlgorithm  set "Add4"
    postFitSelectors  set "Mass pdg-$looseHalfWinWidth:pdg+$looseHalfWinWidth"  
}

mod clone SmpMergerDefiner DRecoD0MainPID
sequence append fDsSequence DRecoD0MainPID
catch { setProduction DRecoD0MainPID }
talkto DRecoD0MainPID {
  inputListNames set D0ToKPiLoosePID
  inputListNames set D0ToKPiPi0LoosePID
  inputListNames set D0ToKsPiPiLoosePID
  inputListNames set D0ToK3PiLoose
  inputListNames set D0ToKsPimPipPi0
  inputListNames set D0ToKmPipPipPimPi0
}

mod clone SmpRefinerDefiner DRecoD0MainPIDRefined
sequence append fDsSequence DRecoD0MainPIDRefined
catch { setProduction DRecoD0MainPIDRefined }
talkto DRecoD0MainPIDRefined {
    unrefinedListName  set "DRecoD0MainPID"
    fittingAlgorithm  set $Vtx_Fitter
    fitConstraints    set "Geo"
    preFitSelectors  set $PrePCM
    preFitSelectors  set "Mass pdg-$looseHalfWinWidth:pdg+$looseHalfWinWidth"
    postFitSelectors  set $PostPCM
    postFitSelectors  set "Mass pdg-$tightHalfWinWidth:pdg+$tightHalfWinWidth"
    postFitSelectors  set $ProbChiSq
}

##################Dc 

mod clone SmpMergerDefiner DRecoDcMainPID
sequence append fDsSequence DRecoDcMainPID
catch { setProduction DRecoDcMainPID }
talkto DRecoDcMainPID {
  inputListNames set DcToKsPiLoosePID
  inputListNames set DcToKPiPiLoosePID
  inputListNames set DcToKsPiPi0LoosePID
  inputListNames set DcToKPiPiPi0LoosePID
  inputListNames set DcToKsPiPiPiLoosePID
}


mod clone SmpRefinerDefiner DRecoDcMainPIDRefined
sequence append fDsSequence DRecoDcMainPIDRefined
catch { setProduction DRecoDcMainPIDRefined }
talkto DRecoDcMainPIDRefined {
    unrefinedListName  set "DRecoDcMainPID"
    fittingAlgorithm  set $Vtx_Fitter
    fitConstraints    set "Geo"
    preFitSelectors  set $PrePCM
    preFitSelectors  set "Mass pdg-$looseHalfWinWidth:pdg+$looseHalfWinWidth"
    postFitSelectors  set $PostPCM
    postFitSelectors  set "Mass pdg-$tightHalfWinWidth:pdg+$tightHalfWinWidth"
    postFitSelectors  set $ProbChiSq
}


##################Ds

mod clone SmpMakerDefiner DsToKKPiPrompt
sequence append fDsSequence DsToKKPiPrompt
catch { setProduction DsToKKPiPrompt }
talkto DsToKKPiPrompt {
  decayMode         set "D_s+ -> K+ K- pi+"
  daughterListNames set "KCombinedSuperLoose"
  daughterListNames set "KCombinedSuperLoose"
  daughterListNames set "GoodTracksVeryLoose"
  fittingAlgorithm  set "Add4"
  postFitSelectors  set "Mass pdg-$looseHalfWinWidth:pdg+$looseHalfWinWidth" 
}

mod clone SmpMakerDefiner DsTo3PiPrompt
sequence append fDsSequence DsTo3PiPrompt
catch { setProduction DsTo3PiPrompt }
talkto DsTo3PiPrompt {
  decayMode         set "D_s+ -> pi- pi+ pi+"
  daughterListNames set "piCombinedSuperLoose"
  daughterListNames set "piCombinedSuperLoose"
  daughterListNames set "piCombinedSuperLoose"
  fittingAlgorithm  set "Add4"
  postFitSelectors  set "Mass pdg-$looseHalfWinWidth:pdg+$looseHalfWinWidth" 
}

mod clone SmpMergerDefiner DRecoDsMainPID
sequence append fDsSequence DRecoDsMainPID
catch { setProduction DRecoDsMainPID }
talkto DRecoDsMainPID {
  inputListNames set DsToKsKLoosePID
  inputListNames set DsToKKPiPrompt
  #DsToPhiRhoCLoosePID is based on phiDefault(phi->KK) rhoCTight(pipi0)
  inputListNames set DsToPhiRhoCLoosePID
  inputListNames set DsToKsKPiPiLoosePID
  inputListNames set DsTo3PiPrompt
  #DsToPhiPiPiPiLoosePID is based on phiDefault(phi->KK)
  inputListNames set DsToPhiPiPiPiLoosePID
  #DsToEtaRhoCLoosePID is based on rhoCTight(pipi0) etaDefault(eta->gg, eta->pipipi0)
  inputListNames set DsToEtaRhoCLoosePID
  #DsToEtaPRhoCLoosePID is based on rhoCTight(pipi0) etaPeppDefault(etaP->pipieta)
  inputListNames set DsToEtaPRhoCLoosePID
  #DsToEtaPiLoosePID is based on etaDefault(eta->gg, eta->pipipi0)
  inputListNames set DsToEtaPiLoosePID
    #DsToEtaPPiLoosePID is based on etaPDefault(etaP->pipieta, etaP->rho0g) [which is based on rho0Default(rho0->pipi)]
  inputListNames set DsToEtaPPiLoosePID
}

mod clone SmpRefinerDefiner DRecoDsMainPIDRefined
sequence append fDsSequence DRecoDsMainPIDRefined
catch { setProduction DRecoDsMainPIDRefined }
talkto DRecoDsMainPIDRefined {
    unrefinedListName  set "DRecoDsMainPID"
    fittingAlgorithm  set "TreeFitter"
    preFitSelectors  set $PrePCM
    preFitSelectors  set "Mass pdg-$looseHalfWinWidth:pdg+$looseHalfWinWidth"
    postFitSelectors  set $PostPCM
    postFitSelectors  set "Mass pdg-$tightHalfWinWidth:pdg+$tightHalfWinWidth"
    postFitSelectors  set $ProbChiSq
    #This constraint was needed because of the following error for eta pi AND eta rho:
    #There are not sufficient geometric constraints to fit this decay tree. Perhaps you should add a beam constraint. 0
    #D_s-
    #|_ eta -----------rho-
    #   |_ gamma gamma |_ pi- pi0
    #                         |_ gamma gamma
    fitConstraints    set "Beam"
}

##################LambdaC

mod clone SmpMergerDefiner LambdaCMainPID
sequence append fDsSequence LambdaCMainPID
catch { setProduction LambdaCMainPID }
talkto LambdaCMainPID {
  inputListNames set LambdaCTopKpi
  inputListNames set LambdaCTopKpipi
  inputListNames set LambdaCTopKs
  inputListNames set LambdaCTopKspipi
  inputListNames set LambdaCToLzpi
  inputListNames set LambdaCToLzpipi
  inputListNames set LambdaCToLzpipipi
  inputListNames set LambdaCToSzpi
  inputListNames set LambdaCToSzpipipi
}

mod clone SmpSubListerDefiner LambdaCMainPIDRefined
sequence append fDsSequence LambdaCMainPIDRefined
catch { setProduction LambdaCMainPIDRefined }
talkto LambdaCMainPIDRefined {
    unrefinedListName  set "LambdaCMainPID"
    selectors  set $PostPCM
}









