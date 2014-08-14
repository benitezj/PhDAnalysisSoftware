# $Id:SigmaCToLambdaCNoPidProtonPath
#Author: Ryan Mackenzie White
#Simple Composition sequence for LambdaC from SigmaC
#LambdaC channels: pKs and pKpi
#SigmaC0 and SigmaC++
#The following skim does not require proton PID
#For the intended use of PID control sample for protons

path create SigmaCToLambdaCNoPidProtonPath
path append SigmaCToLambdaCNoPidProtonPath FilterBGFMultiHadron
global BtaCandLists
lappend BtaCandLists SigmaCToLambdaCNoPidProton

mod clone SmpRefinerDefiner Ks_PidLambdaC
mod clone SmpMakerDefiner BtaPCPidLambdaCTopKs
mod clone SmpMakerDefiner BtaPCPidLambdaCTopKpi
mod clone SmpMergerDefiner BtaPCPidLambdaC
mod clone SmpMakerDefiner BtaPCPidSigmaC++ToLambdaC
mod clone SmpMakerDefiner BtaPCPidSigmaC0ToLambdaC
mod clone SmpMergerDefiner SigmaCToLambdaCNoPidProton

path append SigmaCToLambdaCNoPidProtonPath Ks_PidLambdaC
path append SigmaCToLambdaCNoPidProtonPath BtaPCPidLambdaCTopKs
path append SigmaCToLambdaCNoPidProtonPath BtaPCPidLambdaCTopKpi
path append SigmaCToLambdaCNoPidProtonPath BtaPCPidLambdaC
path append SigmaCToLambdaCNoPidProtonPath BtaPCPidSigmaC++ToLambdaC
path append SigmaCToLambdaCNoPidProtonPath BtaPCPidSigmaC0ToLambdaC
path append SigmaCToLambdaCNoPidProtonPath SigmaCToLambdaCNoPidProton

catch { setProduction Ks_PidLambdaC }
catch { setProduction BtaPCPidLambdaCTopKs }
catch { setProduction BtaPCPidLambdaCTopKpi }
catch { setProduction BtaPCPidLambdaC }
catch { setProduction BtaPCPidSigmaC++ToLambdaC }
catch { setProduction BtaPCPidSigmaC0ToLambdaC }
catch { setProduction SigmaCToLambdaCNoPidProton }

#we refine the standard KsLoose list
talkto Ks_PidLambdaC {
        decayMode         set "K_S0 -> pi+ pi-"
        unrefinedListName set KsDefault
        createUsrData     set true
        fittingAlgorithm  set "TreeFitter"
        postFitSelectors   set "Mass 0.455:0.54"
        postFitSelectors  set "ProbChiSq 0.00001:"
        postFitSelectors  set "FlightSignificance 0:"
}

#define our signal modes
set preMassCut "Mass 1.9:2.6"
set looseCmsP         "CmsP 2.3:"
set postMassCut "Mass 2.18:2.38"
set CmsP              "CmsP 2.7:"
set postChiSqCut "ProbChiSq 0.01:"
talkto BtaPCPidLambdaCTopKs {
    decayMode         set "Lambda_c+ -> p+ K_S0"
    daughterListNames set "GoodTracksVeryLoose"
    daughterListNames set "Ks_PidLambdaC"
    preFitSelectors   set $preMassCut
    preFitSelectors   set $looseCmsP
    postFitSelectors  set $postMassCut
    postFitSelectors  set $postChiSqCut
    postFitSelectors  set $CmsP
    fittingAlgorithm   set "Cascade"
    fitConstraints     set "Geo"
}

talkto BtaPCPidLambdaCTopKpi {
    decayMode         set "Lambda_c+ -> p+ K- pi+"
    daughterListNames set "GoodTracksVeryLoose"
    daughterListNames set "KLHTight"
    daughterListNames set "piLHTight"
    preFitSelectors   set $preMassCut
    preFitSelectors   set $looseCmsP
    postFitSelectors  set $postMassCut
    postFitSelectors  set $postChiSqCut
    postFitSelectors  set $CmsP
    fittingAlgorithm   set "Cascade"
    fitConstraints     set "Geo"
}

#Merge the Lamda lists into one list

talkto BtaPCPidLambdaC {
  inputListNames    set BtaPCPidLambdaCTopKs
  inputListNames    set BtaPCPidLambdaCTopKpi
  disableCloneCheck set true
}


#Build the SigmaC candidate
talkto BtaPCPidSigmaC++ToLambdaC {
    decayMode         set "Sigma_c++ -> Lambda_c+ pi+"
    daughterListNames set "BtaPCPidLambdaC"
    daughterListNames set "piLHTight"
    fittingAlgorithm  set "Add4"
    preFitSelectors   set "CmsP 2:"
    preFitSelectors   set "DeltaM :1"
    postFitSelectors  set "DeltaM :0.3"
    postFitSelectors  set "CmsP 2.7:"
}

#Build the SigmaC candidate
talkto BtaPCPidSigmaC0ToLambdaC {
    decayMode         set "Sigma_c0 -> Lambda_c+ pi-"
    daughterListNames set "BtaPCPidLambdaC"
    daughterListNames set "piLHTight"
    fittingAlgorithm  set "Add4"
    preFitSelectors   set "CmsP 2:"
    preFitSelectors   set "DeltaM :1"
    postFitSelectors  set "DeltaM :0.3"
    postFitSelectors  set "CmsP 2.7:"
}

talkto SigmaCToLambdaCNoPidProton {
    inputListNames    set BtaPCPidSigmaC++ToLambdaC
    inputListNames    set BtaPCPidSigmaC0ToLambdaC
    disableCloneCheck set true
}

#Filter on the list
mod clone FilterByList SigmaCToLambdaCNoPidProtonFilter
talkto SigmaCToLambdaCNoPidProtonFilter {
    orList set SigmaCToLambdaCNoPidProton
}

path append SigmaCToLambdaCNoPidProtonPath SigmaCToLambdaCNoPidProtonFilter
