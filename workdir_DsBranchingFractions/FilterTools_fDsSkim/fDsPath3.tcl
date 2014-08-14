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

set do_D0To2ProngDcj 1
set do_D0ToKnPiPi0 1
set do_DRecoDmesonTag 1
set do_DRecoDsmesonTag 1
set do_DcTo3PiPrompt 1
set do_DcToKpipiPrompt 1
set do_LambdaC 1
set do_DcToKKPiPrompt 1
#
#For this file
#D0 and Dc settings 
set D_PreHalfWinWidth            0.200
set D_PostHalfWinWidth           0.100
set D_PrePCM                     "CmsP 1.500:"
set D_PostPCM                    "CmsP 2.000:"
set D_Vtx_Fitter                 TreeFitter
set D_ProbChiSq                  "ProbChiSq 0.00001:"
#
#Ds settings 
set Ds_PreHalfWinWidth           0.200
set Ds_PostHalfWinWidth          0.100
set Ds_PrePCM                    "CmsP 1.500:"
set Ds_PostPCM                   "CmsP 2.000:"
set Ds_Vtx_Fitter                TreeFitter
set Ds_ProbChiSq                 "ProbChiSq 0.00001:"
#
#LambdaC settings 
set LambdaC_PreHalfWinWidth      0.200
set LambdaC_PostHalfWinWidth     0.100
set LambdaC_PrePCM               "CmsP 1.500:"
set LambdaC_PostPCM              "CmsP 2.000:"
set LambdaC_Vtx_Fitter           TreeFitter
set LambdaC_ProbChiSq            "ProbChiSq 0.00001:"
#

path create fDsPath

########################################################################
#1 D0To2ProngDcj
if { $do_D0To2ProngDcj } {
   mod clone SmpRefinerDefiner D0ToKPiTightPID_DCJ
   path append fDsPath D0ToKPiTightPID_DCJ
   catch { setProduction D0ToKPiTightPID_DCJ }
   talkto D0ToKPiTightPID_DCJ {
       decayMode         set "D0 -> K- pi+"
       unrefinedListName  set "D0ToKPiLoose"
       daughterListNames set "KCombinedSuperLoose"
       daughterListNames set "piCombinedSuperLoose"
       fittingAlgorithm  set $D_Vtx_Fitter
       preFitSelectors  set "Mass pdg-$D_PreHalfWinWidth:pdg+$D_PreHalfWinWidth"
       postFitSelectors  set $D_ProbChiSq
       postFitSelectors  set $D_PostPCM
       postFitSelectors  set "Mass pdg-$D_PostHalfWinWidth:pdg+$D_PostHalfWinWidth"
   }
}
########################################################################
########################################################################
#2 D0ToKnPiPi0
if { $do_D0ToKnPiPi0 } {
mod clone SmpMakerDefiner D0ToKsPimPipPi0
path append fDsPath D0ToKsPimPipPi0
talkto D0ToKsPimPipPi0 {
    decayMode          set "D0 -> K_S0 pi- pi+ pi0"
    daughterListNames  set "KsLoose"
    daughterListNames  set "piCombinedVeryLoose"
    daughterListNames  set "piCombinedVeryLoose"
    daughterListNames  set "pi0VeryLoose"
    fittingAlgorithm  set $D_Vtx_Fitter"Cascade"
    preFitSelectors   set $D_PrePCM
    preFitSelectors  set "Mass pdg-$D_PreHalfWinWidth:pdg+$D_PreHalfWinWidth"
    postFitSelectors  set $ProbChiSqFastVtxTight
    postFitSelectors  set $D_ProbChiSq
    postFitSelectors  set $D_PostPCM
    postFitSelectors  set "Mass pdg-$D_PostHalfWinWidth:pdg+$D_PostHalfWinWidth"  
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
    fittingAlgorithm  set $D_Vtx_Fitter
    preFitSelectors   set $D_PrePCM
    preFitSelectors  set "Mass pdg-$D_PreHalfWinWidth:pdg+$D_PreHalfWinWidth"
    postFitSelectors  set $ProbChiSqFastVtxTight
    postFitSelectors  set $D_ProbChiSq
    postFitSelectors  set $D_PostPCM
    postFitSelectors  set "Mass pdg-$D_PostHalfWinWidth:pdg+$D_PostHalfWinWidth"  
}
}
########################################################################
########################################################################
#3 DRecoDmesonTag
if { $do_DRecoDmesonTag } {
mod clone SmpRefinerDefiner RefinedDRecoDmesonTag_D0ToKPiPi0LoosePID
path append fDsPath RefinedDRecoDmesonTag_D0ToKPiPi0LoosePID
talkto RefinedDRecoDmesonTag_D0ToKPiPi0LoosePID {
    unrefinedListName set "D0ToKPiPi0LoosePID"
    fittingAlgorithm  set $D_Vtx_Fitter
    preFitSelectors   set $D_PrePCM
    postFitSelectors  set $D_ProbChiSq
    postFitSelectors   set $D_PostPCM
    preFitSelectors  set "Mass pdg-$D_PreHalfWinWidth:pdg+$D_PreHalfWinWidth"
    postFitSelectors  set "Mass pdg-$D_PostHalfWinWidth:pdg+$D_PostHalfWinWidth"
}
mod clone SmpRefinerDefiner RefinedDRecoDmesonTag_D0ToK3PiLoosePID
path append fDsPath RefinedDRecoDmesonTag_D0ToK3PiLoosePID
talkto RefinedDRecoDmesonTag_D0ToK3PiLoosePID {
    unrefinedListName set "D0ToK3PiLoosePID"
    fittingAlgorithm  set $D_Vtx_Fitter
    preFitSelectors   set $D_PrePCM
    postFitSelectors  set $D_ProbChiSq
    postFitSelectors   set $D_PostPCM
    preFitSelectors  set "Mass pdg-$D_PreHalfWinWidth:pdg+$D_PreHalfWinWidth"
    postFitSelectors  set "Mass pdg-$D_PostHalfWinWidth:pdg+$D_PostHalfWinWidth"
}
mod clone SmpRefinerDefiner RefinedDRecoDmesonTag_D0ToKsPiPiLoosePID
path append fDsPath RefinedDRecoDmesonTag_D0ToKsPiPiLoosePID
talkto RefinedDRecoDmesonTag_D0ToKsPiPiLoosePID {
    unrefinedListName set "D0ToKsPiPiLoosePID"
    fittingAlgorithm  set $D_Vtx_Fitter
    preFitSelectors   set $D_PrePCM
    postFitSelectors  set $D_ProbChiSq
    postFitSelectors   set $D_PostPCM
    preFitSelectors  set "Mass pdg-$D_PreHalfWinWidth:pdg+$D_PreHalfWinWidth"
    postFitSelectors  set "Mass pdg-$D_PostHalfWinWidth:pdg+$D_PostHalfWinWidth"
}
mod clone SmpRefinerDefiner RefinedDRecoDmesonTag_DcToKsPiLoosePID
path append fDsPath RefinedDRecoDmesonTag_DcToKsPiLoosePID
talkto RefinedDRecoDmesonTag_DcToKsPiLoosePID {
    unrefinedListName set "DcToKsPiLoosePID"
    fittingAlgorithm  set $D_Vtx_Fitter
    preFitSelectors   set $D_PrePCM
    postFitSelectors  set $D_ProbChiSq
    postFitSelectors   set $D_PostPCM
    preFitSelectors  set "Mass pdg-$D_PreHalfWinWidth:pdg+$D_PreHalfWinWidth"
    postFitSelectors  set "Mass pdg-$D_PostHalfWinWidth:pdg+$D_PostHalfWinWidth"
}
mod clone SmpRefinerDefiner RefinedDRecoDmesonTag_DcToKsPiPi0LoosePID
path append fDsPath RefinedDRecoDmesonTag_DcToKsPiPi0LoosePID
talkto RefinedDRecoDmesonTag_DcToKsPiPi0LoosePID {
    unrefinedListName set "DcToKsPiPi0LoosePID"
    fittingAlgorithm  set $D_Vtx_Fitter
    preFitSelectors   set $D_PrePCM
    postFitSelectors  set $D_ProbChiSq
    postFitSelectors   set $D_PostPCM
    preFitSelectors  set "Mass pdg-$D_PreHalfWinWidth:pdg+$D_PreHalfWinWidth"
    postFitSelectors  set "Mass pdg-$D_PostHalfWinWidth:pdg+$D_PostHalfWinWidth"
}
mod clone SmpRefinerDefiner RefinedDRecoDmesonTag_DcToKsPiPiPiLoosePID
path append fDsPath RefinedDRecoDmesonTag_DcToKsPiPiPiLoosePID
talkto RefinedDRecoDmesonTag_DcToKsPiPiPiLoosePID {
    unrefinedListName set "DcToKsPiPiPiLoosePID"
    fittingAlgorithm  set $D_Vtx_Fitter
    preFitSelectors   set $D_PrePCM
    postFitSelectors  set $D_ProbChiSq
    postFitSelectors   set $D_PostPCM
    preFitSelectors  set "Mass pdg-$D_PreHalfWinWidth:pdg+$D_PreHalfWinWidth"
    postFitSelectors  set "Mass pdg-$D_PostHalfWinWidth:pdg+$D_PostHalfWinWidth"
}
mod clone SmpRefinerDefiner RefinedDRecoDmesonTag_DcToKPiPiPi0LoosePID
path append fDsPath RefinedDRecoDmesonTag_DcToKPiPiPi0LoosePID
talkto RefinedDRecoDmesonTag_DcToKPiPiPi0LoosePID {
    unrefinedListName set "DcToKPiPiPi0LoosePID"
    fittingAlgorithm  set $D_Vtx_Fitter
    preFitSelectors   set $D_PrePCM
    postFitSelectors  set $D_ProbChiSq
    postFitSelectors   set $D_PostPCM
    preFitSelectors  set "Mass pdg-$D_PreHalfWinWidth:pdg+$D_PreHalfWinWidth"
    postFitSelectors  set "Mass pdg-$D_PostHalfWinWidth:pdg+$D_PostHalfWinWidth"
}


}
########################################################################
########################################################################
#4 DRecoDsmesonTag
if { $do_DRecoDsmesonTag } {
mod clone SmpRefinerDefiner RefinedDRecoDsmesonTag_DsToKsKLoosePID
path append fDsPath RefinedDRecoDsmesonTag_DsToKsKLoosePID
talkto RefinedDRecoDsmesonTag_DsToKsKLoosePID {
    unrefinedListName set "DsToKsKLoosePID"
    fittingAlgorithm  set $Ds_Vtx_Fitter
    preFitSelectors   set $Ds_PrePCM
    postFitSelectors  set $Ds_ProbChiSq
    postFitSelectors   set $Ds_PostPCM
    preFitSelectors  set "Mass pdg-$Ds_PreHalfWinWidth:pdg+$Ds_PreHalfWinWidth"
    postFitSelectors  set "Mass pdg-$Ds_PostHalfWinWidth:pdg+$Ds_PostHalfWinWidth"
}
mod clone SmpRefinerDefiner RefinedDRecoDsmesonTag_DsToPhiPiPiPiLoosePID
path append fDsPath RefinedDRecoDsmesonTag_DsToPhiPiPiPiLoosePID
talkto RefinedDRecoDsmesonTag_DsToPhiPiPiPiLoosePID {
    unrefinedListName set "DsToPhiPiPiPiLoosePID"
    fittingAlgorithm  set $Ds_Vtx_Fitter
    preFitSelectors   set $Ds_PrePCM
    postFitSelectors  set $Ds_ProbChiSq
    postFitSelectors   set $Ds_PostPCM
    preFitSelectors  set "Mass pdg-$Ds_PreHalfWinWidth:pdg+$Ds_PreHalfWinWidth"
    postFitSelectors  set "Mass pdg-$Ds_PostHalfWinWidth:pdg+$Ds_PostHalfWinWidth"
}
mod clone SmpRefinerDefiner RefinedDRecoDsmesonTag_DsToPhiRhoCLoosePID
path append fDsPath RefinedDRecoDsmesonTag_DsToPhiRhoCLoosePID
talkto RefinedDRecoDsmesonTag_DsToPhiRhoCLoosePID {
    unrefinedListName set "DsToPhiRhoCLoosePID"
    fittingAlgorithm  set $Ds_Vtx_Fitter
    preFitSelectors   set $Ds_PrePCM
    postFitSelectors  set $Ds_ProbChiSq
    postFitSelectors   set $Ds_PostPCM
    preFitSelectors  set "Mass pdg-$Ds_PreHalfWinWidth:pdg+$Ds_PreHalfWinWidth"
    postFitSelectors  set "Mass pdg-$Ds_PostHalfWinWidth:pdg+$Ds_PostHalfWinWidth"
}
mod clone SmpRefinerDefiner RefinedDRecoDsmesonTag_DsToKsKPiPiLoosePID
path append fDsPath RefinedDRecoDsmesonTag_DsToKsKPiPiLoosePID
talkto RefinedDRecoDsmesonTag_DsToKsKPiPiLoosePID {
    unrefinedListName set "DsToKsKPiPiLoosePID"
    fittingAlgorithm  set $Ds_Vtx_Fitter
    preFitSelectors   set $Ds_PrePCM
    postFitSelectors  set $Ds_ProbChiSq
    postFitSelectors   set $Ds_PostPCM
    preFitSelectors  set "Mass pdg-$Ds_PreHalfWinWidth:pdg+$Ds_PreHalfWinWidth"
    postFitSelectors  set "Mass pdg-$Ds_PostHalfWinWidth:pdg+$Ds_PostHalfWinWidth"
}
mod clone SmpRefinerDefiner RefinedDRecoDsmesonTag_DsToEtaPiLoosePID
path append fDsPath RefinedDRecoDsmesonTag_DsToEtaPiLoosePID
talkto RefinedDRecoDsmesonTag_DsToEtaPiLoosePID {
    unrefinedListName set "DsToEtaPiLoosePID"
    fittingAlgorithm  set $Ds_Vtx_Fitter
    preFitSelectors   set $Ds_PrePCM
    postFitSelectors  set $Ds_ProbChiSq
    postFitSelectors   set $Ds_PostPCM
    preFitSelectors  set "Mass pdg-$Ds_PreHalfWinWidth:pdg+$Ds_PreHalfWinWidth"
    postFitSelectors  set "Mass pdg-$Ds_PostHalfWinWidth:pdg+$Ds_PostHalfWinWidth"
  fitConstraints    set "Beam"
  fitSettings       set "UpdateDaughters"
}
mod clone SmpRefinerDefiner RefinedDRecoDsmesonTag_DsToEtaRhoCLoosePID
path append fDsPath RefinedDRecoDsmesonTag_DsToEtaRhoCLoosePID
talkto RefinedDRecoDsmesonTag_DsToEtaRhoCLoosePID {
    unrefinedListName set "DsToEtaRhoCLoosePID"
    fittingAlgorithm  set $Ds_Vtx_Fitter
    preFitSelectors   set $Ds_PrePCM
    postFitSelectors  set $Ds_ProbChiSq
    postFitSelectors   set $Ds_PostPCM
    preFitSelectors  set "Mass pdg-$Ds_PreHalfWinWidth:pdg+$Ds_PreHalfWinWidth"
    postFitSelectors  set "Mass pdg-$Ds_PostHalfWinWidth:pdg+$Ds_PostHalfWinWidth"
  fitConstraints    set "Beam"
  fitSettings       set "UpdateDaughters"
}
mod clone SmpRefinerDefiner RefinedDRecoDsmesonTag_DsToEtaPPiLoosePID
path append fDsPath RefinedDRecoDsmesonTag_DsToEtaPPiLoosePID
talkto RefinedDRecoDsmesonTag_DsToEtaPPiLoosePID {
    unrefinedListName set "DsToEtaPPiLoosePID"
    fittingAlgorithm  set $Ds_Vtx_Fitter
    preFitSelectors   set $Ds_PrePCM
    postFitSelectors  set $Ds_ProbChiSq
    postFitSelectors   set $Ds_PostPCM
    preFitSelectors  set "Mass pdg-$Ds_PreHalfWinWidth:pdg+$Ds_PreHalfWinWidth"
    postFitSelectors  set "Mass pdg-$Ds_PostHalfWinWidth:pdg+$Ds_PostHalfWinWidth"
#  fitConstraints    set "Beam"
#  fitSettings       set "UpdateDaughters"
}
mod clone SmpRefinerDefiner RefinedDRecoDsmesonTag_DsToEtaPRhoCLoosePID
path append fDsPath RefinedDRecoDsmesonTag_DsToEtaPRhoCLoosePID
talkto RefinedDRecoDsmesonTag_DsToEtaPRhoCLoosePID {
    unrefinedListName set "DsToEtaPRhoCLoosePID"
    fittingAlgorithm  set $Ds_Vtx_Fitter
    preFitSelectors   set $Ds_PrePCM
    postFitSelectors  set $Ds_ProbChiSq
    postFitSelectors   set $Ds_PostPCM
    preFitSelectors  set "Mass pdg-$Ds_PreHalfWinWidth:pdg+$Ds_PreHalfWinWidth"
    postFitSelectors  set "Mass pdg-$Ds_PostHalfWinWidth:pdg+$Ds_PostHalfWinWidth"
#  fitConstraints    set "Beam"
#  fitSettings       set "UpdateDaughters"
}
}
########################################################################
########################################################################
#5 DcTo3PiPrompt
if { $do_DcTo3PiPrompt } {
#################################
## global control variables
#################################

  set D0Debug                false
  set D0Verbose              false

#########################
## SimpleComposition cuts
#########################

# FastVtx probability
  set ProbChiSqFastVtxTight  "ProbChiSq 0.005:"

# pre- and post-fit mass, p* cuts
#  set verylooseMass          "Mass 1.5:2.3"
#  set looseMass              "Mass 1.7:2.1"
#  set preCmsP                "CmsP 2.0:"
#  set postCmsP               "CmsP 2.4:"

# D+ -> pi- pi+ pi+

mod clone SmpMakerDefiner DcTo3PiPrompt
path append fDsPath DcTo3PiPrompt
catch { setProduction DcTo3PiPrompt }
talkto DcTo3PiPrompt {
  debug             set $D0Debug
  verbose           set $D0Verbose
  disableCloneCheck set true

  decayMode         set "D+ -> pi- pi+ pi+"

  daughterListNames set "piCombinedSuperLoose"
  daughterListNames set "piCombinedSuperLoose"
  daughterListNames set "piCombinedSuperLoose"

  fittingAlgorithm  set "Cascade"
  fitConstraints    set "Geo"

  preFitSelectors   set $D_PrePCM
  preFitSelectors  set "Mass pdg-$D_PreHalfWinWidth:pdg+$D_PreHalfWinWidth"

  postFitSelectors  set $ProbChiSqFastVtxTight
  postFitSelectors  set $D_ProbChiSq
  postFitSelectors  set $D_PostPCM
  postFitSelectors  set "Mass pdg-$D_PostHalfWinWidth:pdg+$D_PostHalfWinWidth"
}
}
########################################################################
########################################################################
#6 DcToKpipiPrompt
if { $do_DcToKpipiPrompt } {
#################################
## global control variables
#################################

  set D0Debug                false
  set D0Verbose              false

#########################
## SimpleComposition cuts
#########################

# FastVtx probability
  set ProbChiSqFastVtxTight  "ProbChiSq 0.005:"

# pre- and post-fit mass, p* cuts
#  set verylooseMass          "Mass 1.5:2.3"
#  set looseMass              "Mass 1.7:2.1"
#  set preCmsP                "CmsP 2.0:"
#  set postCmsP               "CmsP 2.4:"

# D+ -> K- pi+ pi+

mod clone SmpMakerDefiner DcToKpipiPrompt
path append fDsPath DcToKpipiPrompt
catch { setProduction DcToKpipiPrompt }
talkto DcToKpipiPrompt {
  debug             set $D0Debug
  verbose           set $D0Verbose
  disableCloneCheck set true

  decayMode         set "D+ -> K- pi+ pi+"

  daughterListNames set "KCombinedSuperLoose"
  daughterListNames set "piCombinedSuperLoose"
  daughterListNames set "piCombinedSuperLoose"

  fittingAlgorithm  set "Cascade"
  fitConstraints     set "Geo"

  preFitSelectors   set $D_PrePCM
  preFitSelectors  set "Mass pdg-$D_PostHalfWinWidth:pdg+$D_PostHalfWinWidth"

  postFitSelectors  set $ProbChiSqFastVtxTight
  postFitSelectors  set $D_ProbChiSq
  postFitSelectors  set $D_PostPCM
  postFitSelectors  set "Mass pdg-$D_PostHalfWinWidth:pdg+$D_PostHalfWinWidth"
}
}
########################################################################
########################################################################
#7 LambdaC 
if { $do_LambdaC } {
mod clone SmpRefinerDefiner RefinedLambdaC_LambdaCTopKpi
path append fDsPath RefinedLambdaC_LambdaCTopKpi
talkto RefinedLambdaC_LambdaCTopKpi {
    unrefinedListName set "LambdaCTopKpi"
    fittingAlgorithm  set $LambdaC_Vtx_Fitter
    preFitSelectors   set $LambdaC_PrePCM
    postFitSelectors  set $LambdaC_ProbChiSq
    postFitSelectors   set $LambdaC_PostPCM
    preFitSelectors  set "Mass pdg-$LambdaC_PreHalfWinWidth:pdg+$LambdaC_PreHalfWinWidth"
    postFitSelectors  set "Mass pdg-$LambdaC_PostHalfWinWidth:pdg+$LambdaC_PostHalfWinWidth"
}
mod clone SmpRefinerDefiner RefinedLambdaC_LambdaCTopKpipi
path append fDsPath RefinedLambdaC_LambdaCTopKpipi
talkto RefinedLambdaC_LambdaCTopKpipi {
    unrefinedListName set "LambdaCTopKpipi"
    fittingAlgorithm  set $LambdaC_Vtx_Fitter
    preFitSelectors   set $LambdaC_PrePCM
    postFitSelectors  set $LambdaC_ProbChiSq
    postFitSelectors   set $LambdaC_PostPCM
    preFitSelectors  set "Mass pdg-$LambdaC_PreHalfWinWidth:pdg+$LambdaC_PreHalfWinWidth"
    postFitSelectors  set "Mass pdg-$LambdaC_PostHalfWinWidth:pdg+$LambdaC_PostHalfWinWidth"
}
mod clone SmpRefinerDefiner RefinedLambdaC_LambdaCTopKs
path append fDsPath RefinedLambdaC_LambdaCTopKs
talkto RefinedLambdaC_LambdaCTopKs {
    unrefinedListName set "LambdaCTopKs"
    fittingAlgorithm  set $LambdaC_Vtx_Fitter
    preFitSelectors   set $LambdaC_PrePCM
    postFitSelectors  set $LambdaC_ProbChiSq
    postFitSelectors   set $LambdaC_PostPCM
    preFitSelectors  set "Mass pdg-$LambdaC_PreHalfWinWidth:pdg+$LambdaC_PreHalfWinWidth"
    postFitSelectors  set "Mass pdg-$LambdaC_PostHalfWinWidth:pdg+$LambdaC_PostHalfWinWidth"
}
mod clone SmpRefinerDefiner RefinedLambdaC_LambdaCTopKspipi
path append fDsPath RefinedLambdaC_LambdaCTopKspipi
talkto RefinedLambdaC_LambdaCTopKspipi {
    unrefinedListName set "LambdaCTopKspipi"
    fittingAlgorithm  set $LambdaC_Vtx_Fitter
    preFitSelectors   set $LambdaC_PrePCM
    postFitSelectors  set $LambdaC_ProbChiSq
    postFitSelectors   set $LambdaC_PostPCM
    preFitSelectors  set "Mass pdg-$LambdaC_PreHalfWinWidth:pdg+$LambdaC_PreHalfWinWidth"
    postFitSelectors  set "Mass pdg-$LambdaC_PostHalfWinWidth:pdg+$LambdaC_PostHalfWinWidth"
}
mod clone SmpRefinerDefiner RefinedLambdaC_LambdaCToLzpi
path append fDsPath RefinedLambdaC_LambdaCToLzpi
talkto RefinedLambdaC_LambdaCToLzpi {
    unrefinedListName set "LambdaCToLzpi"
    fittingAlgorithm  set $LambdaC_Vtx_Fitter
    preFitSelectors   set $LambdaC_PrePCM
    postFitSelectors  set $LambdaC_ProbChiSq
    postFitSelectors   set $LambdaC_PostPCM
    preFitSelectors  set "Mass pdg-$LambdaC_PreHalfWinWidth:pdg+$LambdaC_PreHalfWinWidth"
    postFitSelectors  set "Mass pdg-$LambdaC_PostHalfWinWidth:pdg+$LambdaC_PostHalfWinWidth"
}
mod clone SmpRefinerDefiner RefinedLambdaC_LambdaCToLzpipi
path append fDsPath RefinedLambdaC_LambdaCToLzpipi
talkto RefinedLambdaC_LambdaCToLzpipi {
    unrefinedListName set "LambdaCToLzpipi"
    fittingAlgorithm  set $LambdaC_Vtx_Fitter
    preFitSelectors   set $LambdaC_PrePCM
    postFitSelectors  set $LambdaC_ProbChiSq
    postFitSelectors   set $LambdaC_PostPCM
    preFitSelectors  set "Mass pdg-$LambdaC_PreHalfWinWidth:pdg+$LambdaC_PreHalfWinWidth"
    postFitSelectors  set "Mass pdg-$LambdaC_PostHalfWinWidth:pdg+$LambdaC_PostHalfWinWidth"
}
mod clone SmpRefinerDefiner RefinedLambdaC_LambdaCToLzpipipi
path append fDsPath RefinedLambdaC_LambdaCToLzpipipi
talkto RefinedLambdaC_LambdaCToLzpipipi {
    unrefinedListName set "LambdaCToLzpipipi"
    fittingAlgorithm  set $LambdaC_Vtx_Fitter
    preFitSelectors   set $LambdaC_PrePCM
    postFitSelectors  set $LambdaC_ProbChiSq
    postFitSelectors   set $LambdaC_PostPCM
    preFitSelectors  set "Mass pdg-$LambdaC_PreHalfWinWidth:pdg+$LambdaC_PreHalfWinWidth"
    postFitSelectors  set "Mass pdg-$LambdaC_PostHalfWinWidth:pdg+$LambdaC_PostHalfWinWidth"
}
mod clone SmpRefinerDefiner RefinedLambdaC_LambdaCToSzpi
path append fDsPath RefinedLambdaC_LambdaCToSzpi
talkto RefinedLambdaC_LambdaCToSzpi {
    unrefinedListName set "LambdaCToSzpi"
    fittingAlgorithm  set $LambdaC_Vtx_Fitter
    preFitSelectors   set $LambdaC_PrePCM
    postFitSelectors  set $LambdaC_ProbChiSq
    postFitSelectors   set $LambdaC_PostPCM
    preFitSelectors  set "Mass pdg-$LambdaC_PreHalfWinWidth:pdg+$LambdaC_PreHalfWinWidth"
    postFitSelectors  set "Mass pdg-$LambdaC_PostHalfWinWidth:pdg+$LambdaC_PostHalfWinWidth"
}
mod clone SmpRefinerDefiner RefinedLambdaC_LambdaCToSzpipipi
path append fDsPath RefinedLambdaC_LambdaCToSzpipipi
talkto RefinedLambdaC_LambdaCToSzpipipi {
    unrefinedListName set "LambdaCToSzpipipi"
    fittingAlgorithm  set $LambdaC_Vtx_Fitter
    preFitSelectors   set $LambdaC_PrePCM
    postFitSelectors  set $LambdaC_ProbChiSq
    postFitSelectors   set $LambdaC_PostPCM
    preFitSelectors  set "Mass pdg-$LambdaC_PreHalfWinWidth:pdg+$LambdaC_PreHalfWinWidth"
    postFitSelectors  set "Mass pdg-$LambdaC_PostHalfWinWidth:pdg+$LambdaC_PostHalfWinWidth"
}
} 
########################################################################
#8 DcToKKPiPrompt 
if { $do_DcToKKPiPrompt } {
path append fDsPath FilterBGFMultiHadron

#########################
## SimpleComposition cuts
#########################

# FastVtx probability
  set ProbChiSqVtx  "ProbChiSq 0.005:"

# pre- and post-fit mass, p* cuts
#  set preMass       "Mass 1.5:2.3"
#  set postMass      "Mass 1.7:2.1"
#  set preCmsP       "CmsP 2.0:"
#  set postCmsP      "CmsP 2.4:"


# D+ -> K- K+ Pi+

mod clone SmpMakerDefiner DcToKKPiPrompt
path append fDsPath DcToKKPiPrompt
catch { setProduction DcToKKPiPrompt }
talkto DcToKKPiPrompt {
  disableCloneCheck set true

  decayMode         set "D+ -> K+ K- pi+"

  daughterListNames set "KCombinedSuperLoose"
  daughterListNames set "KCombinedSuperLoose"
  daughterListNames set "GoodTracksVeryLoose"

  fittingAlgorithm  set "Cascade"
  preFitSelectors   set $D_PrePCM
  preFitSelectors  set "Mass pdg-$D_PreHalfWinWidth:pdg+$D_PreHalfWinWidth"

  postFitSelectors  set $ProbChiSqVtx
  postFitSelectors  set $D_ProbChiSq
  postFitSelectors  set $D_PostPCM
  postFitSelectors  set "Mass pdg-$D_PostHalfWinWidth:pdg+$D_PostHalfWinWidth"
}
}
########################################################################

mod clone FilterByList fDsFilter
path append fDsPath fDsFilter
talkto fDsFilter {
    #1 D0To2ProngDcj
    if { $do_D0To2ProngDcj  } {
	orList set "D0ToKPiTightPID_DCJ"
    }
    #2 D0ToKnPiPi0
    if { $do_D0ToKnPiPi0 } {
	orList set "D0ToKsPimPipPi0"
	orList set "D0ToKmPipPipPimPi0"
    }
    #3 DRecoDmesonTag
    if { $do_DRecoDmesonTag } {   
	orList set "RefinedDRecoDmesonTag_D0ToKPiPi0LoosePID"
	orList set "RefinedDRecoDmesonTag_D0ToK3PiLoosePID"
	orList set "RefinedDRecoDmesonTag_D0ToKsPiPiLoosePID"
	orList set "RefinedDRecoDmesonTag_DcToKsPiLoosePID"
	orList set "RefinedDRecoDmesonTag_DcToKsPiPi0LoosePID"
	orList set "RefinedDRecoDmesonTag_DcToKsPiPiPiLoosePID"
	orList set "RefinedDRecoDmesonTag_DcToKPiPiPi0LoosePID"
    }
    #4 DRecoDsmesonTag
    if { $do_DRecoDsmesonTag } {  
	orList set "RefinedDRecoDsmesonTag_DsToKsKLoosePID"
	orList set "RefinedDRecoDsmesonTag_DsToPhiPiPiPiLoosePID"
        orList set "RefinedDRecoDsmesonTag_DsToPhiRhoCLoosePID"
	orList set "RefinedDRecoDsmesonTag_DsToKsKPiPiLoosePID"
	orList set "RefinedDRecoDsmesonTag_DsToEtaPiLoosePID"
	orList set "RefinedDRecoDsmesonTag_DsToEtaRhoCLoosePID"
	orList set "RefinedDRecoDsmesonTag_DsToEtaPPiLoosePID"
	orList set "RefinedDRecoDsmesonTag_DsToEtaPRhoCLoosePID"
    }
    #5 DcTo3PiPrompt
    if { $do_DcTo3PiPrompt } {
	orList set "DcTo3PiPrompt"
    }
    #6 DcToKpipiPrompt
    if { $do_DcToKpipiPrompt } {
	orList set "DcToKpipiPrompt"
    }
    #7 LambdaC
    if { $do_LambdaC } {
	orList set "RefinedLambdaC_LambdaCTopKpi"
	orList set "RefinedLambdaC_LambdaCTopKpipi"
	orList set "RefinedLambdaC_LambdaCTopKs"
	orList set "RefinedLambdaC_LambdaCTopKspipi"
	orList set "RefinedLambdaC_LambdaCToLzpi"
	orList set "RefinedLambdaC_LambdaCToLzpipi"
	orList set "RefinedLambdaC_LambdaCToLzpipipi"
        orList set "RefinedLambdaC_LambdaCToSzpi"
        orList set "RefinedLambdaC_LambdaCToSzpipipi"
    }
    #8 DcToKKPiPrompt
    if { $do_DcToKKPiPrompt } {
	orList set "DcToKKPiPrompt"
    }
}

