###############################################################################
# $Id: DsTo3PSPath.tcl,v 1.4 2007/10/09 08:38:54 marchior Exp $
#
# define DsTo3PS path
#
# Author: V.Ziegler
#
# Simple Composition sequence for D_s+ decays to final states containing
# an eta or an eta' 
#
# D_s+ -->
#  eta  pi+  K0_S 
#  eta  K+   K0_S 
#  eta' pi+  K0_S 
###############################################################################

path create DsTo3PSPath
path append DsTo3PSPath FilterBGFMultiHadron

global BtaCandLists
lappend BtaCandLists DsToEtaPiKs
lappend BtaCandLists DsToEtaKKs
lappend BtaCandLists DsToEta_pPiKs

mod clone SmpMakerDefiner DsToEtaPiKs
mod clone SmpMakerDefiner DsToEtaKKs
mod clone SmpMakerDefiner DsToEta_pPiKs

path append DsTo3PSPath DsToEtaPiKs
path append DsTo3PSPath DsToEtaKKs
path append DsTo3PSPath DsToEta_pPiKs

catch { setProduction DsToEtaPiKs}
catch { setProduction DsToEtaKKs}
catch { setProduction DsToEta_pPiKs}

#define our signal modes
set preMassCut "Mass 1.87:2.07"
set postMassCut "Mass 1.87:2.07"
set postChiSqCut "ProbChiSq 0.001:"
set postCmsPCut "CmsP 1.8:"

talkto DsToEtaPiKs {
    decayMode         set "D_s+ -> eta pi+ K_S0"
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

talkto DsToEta_pPiKs {
    decayMode         set "D_s+ -> eta' pi+ K_S0"
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

talkto DsToEtaKKs {
    decayMode         set "D_s+ -> eta K+ K_S0"
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
mod clone FilterByList DsTo3PSFilter
talkto DsTo3PSFilter {
    orList set DsToEtaPiKs
    orList set DsToEtaKKs
    orList set DsToEta_pPiKs
}

path append DsTo3PSPath DsTo3PSFilter
