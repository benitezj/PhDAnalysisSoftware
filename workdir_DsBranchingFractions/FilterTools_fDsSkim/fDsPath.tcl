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


path create fDsPath


##################D0 

mod clone SmpMakerDefiner D0ToKsPimPipPi0
path append fDsPath D0ToKsPimPipPi0
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
path append fDsPath D0ToKmPipPipPimPi0
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
path append fDsPath DRecoD0MainPID
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
path append fDsPath DRecoD0MainPIDRefined
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
path append fDsPath DRecoDcMainPID
catch { setProduction DRecoDcMainPID }
talkto DRecoDcMainPID {
  inputListNames set DcToKsPiLoosePID
  inputListNames set DcToKPiPiLoosePID
  inputListNames set DcToKsPiPi0LoosePID
  inputListNames set DcToKPiPiPi0LoosePID
  inputListNames set DcToKsPiPiPiLoosePID
}


mod clone SmpRefinerDefiner DRecoDcMainPIDRefined
path append fDsPath DRecoDcMainPIDRefined
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
path append fDsPath DsToKKPiPrompt
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
path append fDsPath DsTo3PiPrompt
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
path append fDsPath DRecoDsMainPID
catch { setProduction DRecoDsMainPID }
talkto DRecoDsMainPID {
  inputListNames set DsToKsKLoosePID
  inputListNames set DsToKKPiPrompt
  inputListNames set DsToPhiRhoCLoosePID
  inputListNames set DsToKsKPiPiLoosePID
  inputListNames set DsTo3PiPrompt
  inputListNames set DsToPhiPiPiPiLoosePID
  inputListNames set DsToEtaRhoCLoosePID
  inputListNames set DsToEtaPRhoCLoosePID
  inputListNames set DsToEtaPiLoosePID
  inputListNames set DsToEtaPPiLoosePID
}

mod clone SmpRefinerDefiner DRecoDsMainPIDRefined
path append fDsPath DRecoDsMainPIDRefined
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
path append fDsPath LambdaCMainPID
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
path append fDsPath LambdaCMainPIDRefined
catch { setProduction LambdaCMainPIDRefined }
talkto LambdaCMainPIDRefined {
    unrefinedListName  set "LambdaCMainPID"
    selectors  set $PostPCM
}

######################
######################
mod clone FilterByList fDsFilter
path append fDsPath fDsFilter
talkto fDsFilter {
	orList set "DRecoD0MainPIDRefined"
	orList set "DRecoDcMainPIDRefined"
	orList set "DRecoDsMainPIDRefined"
	orList set "LambdaCMainPIDRefined"
}







