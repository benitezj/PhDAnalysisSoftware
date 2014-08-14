###############################################################################
# $Id: DcTo3PSPath.tcl,v 1.4 2007/10/09 08:38:54 marchior Exp $
#
# define DcTo3PS path
#
# Author: V.Ziegler
#
# Simple Composition sequence for D+ decays to final states containing
# an eta or an eta' 
#
# D+ -->
#  eta  pi+  K0_S 
#  eta' pi+  K0_S 
#  eta  K+   K0_S 
###############################################################################

path create DcTo3PSPath
path append DcTo3PSPath FilterBGFMultiHadron

global BtaCandLists
lappend BtaCandLists DcToEtaPiKs
lappend BtaCandLists DcToEta_pPiKs
lappend BtaCandLists DcToEtaKKs

  
mod clone SmpMakerDefiner DcToEtaPiKs
mod clone SmpMakerDefiner DcToEta_pPiKs
mod clone SmpMakerDefiner DcToEtaKKs

path append DcTo3PSPath DcToEtaPiKs
path append DcTo3PSPath DcToEta_pPiKs
path append DcTo3PSPath DcToEtaKKs

catch { setProduction DcToEtaPiKs }
catch { setProduction DcToEta_pPiKs }
catch { setProduction DcToEtaKKs }

#define our signal modes
set preMassCut "Mass 1.77:1.97"
set postMassCut "Mass 1.77:1.97"
set postChiSqCut "ProbChiSq 0.001:"
set postCmsPCut "CmsP 1.8:"

talkto DcToEtaPiKs {
    decayMode         set "D+ -> eta pi+ K_S0"
    daughterListNames set "etaDefault"
    daughterListNames set "piLHLoose"
    daughterListNames set "KsCleanLoose"
    preFitSelectors   set $preMassCut
    fittingAlgorithm  set "Add4"
    postFitSelectors  set $postCmsPCut
    #postFitSelectors  set $postMassCut
    #postFitSelectors  set $postChiSqCut
    #fittingAlgorithm  set "TreeFitter"
    #fitConstraints    set "Geo"
}

talkto DcToEta_pPiKs {
    decayMode         set "D+ -> eta' pi+ K_S0"
    daughterListNames set "etaPDefault"
    daughterListNames set "piLHLoose"
    daughterListNames set "KsCleanLoose"
    preFitSelectors   set $preMassCut
    fittingAlgorithm  set "Add4"
    postFitSelectors  set $postCmsPCut
    #postFitSelectors  set $postMassCut
    #postFitSelectors  set $postChiSqCut
    #fittingAlgorithm  set "TreeFitter"
    #fitConstraints    set "Geo"
}

talkto DcToEtaKKs {
    decayMode         set "D+ -> eta K+ K_S0"
    daughterListNames set "etaDefault"
    daughterListNames set "KLHLoose"
    daughterListNames set "KsCleanLoose"
    preFitSelectors   set $preMassCut
    fittingAlgorithm  set "Add4"
    postFitSelectors  set $postCmsPCut
    #postFitSelectors  set $postMassCut
    #postFitSelectors  set $postChiSqCut
    #fittingAlgorithm  set "TreeFitter"
    #fitConstraints    set "Geo"
}


#Filter on the list
mod clone FilterByList DcTo3PSFilter
talkto DcTo3PSFilter {
    orList set DcToEtaPiKs
    orList set DcToEta_pPiKs
    orList set DcToEtaKKs
}

path append DcTo3PSPath DcTo3PSFilter
