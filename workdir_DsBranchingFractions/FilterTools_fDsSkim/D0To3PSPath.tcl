###############################################################################
# $Id: D0To3PSPath.tcl,v 1.4 2007/10/09 08:38:53 marchior Exp $
#
# define D0To3PS path
#
# Author: V.Ziegler
#
# Simple Composition sequence for D0 decays to final states containing
# an eta or an eta' 
#
# D0   -->
#  eta  pi+  K- 
#  eta  pi-  K+  (CS) 
#  eta  K0_S pi0 
#  eta  K+   K- 
#  eta  pi+  pi- 
#  eta' pi+  K- 
#  eta' pi-  K+  (CS) 
#  eta' pi+  pi- 
###############################################################################

path create D0To3PSPath
path append D0To3PSPath FilterBGFMultiHadron

global BtaCandLists
lappend BtaCandLists  D0ToEtaPiK
lappend BtaCandLists  D0ToEtaPiK_CS
lappend BtaCandLists  D0ToEtaKsPi0
lappend BtaCandLists  D0ToEtaKK
lappend BtaCandLists  D0ToEtaPiPi
lappend BtaCandLists  D0ToEta_pPiK
lappend BtaCandLists  D0ToEta_pPiK_CS
lappend BtaCandLists  D0ToEta_pPiPi

mod clone SmpMakerDefiner D0ToEtaPiK
mod clone SmpMakerDefiner D0ToEtaPiK_CS
mod clone SmpMakerDefiner D0ToEtaKsPi0
mod clone SmpMakerDefiner D0ToEtaKK
mod clone SmpMakerDefiner D0ToEtaPiPi
mod clone SmpMakerDefiner D0ToEta_pPiK
mod clone SmpMakerDefiner D0ToEta_pPiK_CS
mod clone SmpMakerDefiner D0ToEta_pPiPi

path append D0To3PSPath D0ToEtaPiK
path append D0To3PSPath D0ToEtaPiK_CS
path append D0To3PSPath D0ToEtaKsPi0
path append D0To3PSPath D0ToEtaKK
path append D0To3PSPath D0ToEtaPiPi
path append D0To3PSPath D0ToEta_pPiK
path append D0To3PSPath D0ToEta_pPiK_CS
path append D0To3PSPath D0ToEta_pPiPi

catch { setProduction D0ToEtaPiK }
catch { setProduction D0ToEtaPiK_CS }
catch { setProduction D0ToEtaKsPi0 }
catch { setProduction D0ToEtaKK }
catch { setProduction D0ToEtaPiPi }
catch { setProduction D0ToEta_pPiK }
catch { setProduction D0ToEta_pPiK_CS }
catch { setProduction D0ToEta_pPiPi}

#define our signal modes
set preMassCut "Mass 1.764:1.964"
set postMassCut "Mass 1.764:1.964"
set postChiSqCut "ProbChiSq 0.001:"
set postCmsPCut "CmsP 1.8:"

talkto D0ToEtaPiK {
    decayMode         set "D0 -> eta pi+ K-"
    daughterListNames set "etaDefault"
    daughterListNames set "piLHLoose"
    daughterListNames set "KLHLoose"
    preFitSelectors   set $preMassCut
    fittingAlgorithm  set "Add4"
    postFitSelectors  set $postCmsPCut
    #postFitSelectors  set $postMassCut
    #postFitSelectors  set $postChiSqCut
    #fittingAlgorithm  set "TreeFitter"
    #fitConstraints    set "Geo"
}

talkto D0ToEtaPiK_CS {
    decayMode         set "D0 -> eta pi- K+"
    daughterListNames set "etaDefault"
    daughterListNames set "piLHLoose"
    daughterListNames set "KLHLoose"
    preFitSelectors   set $preMassCut
    fittingAlgorithm  set "Add4"
    postFitSelectors  set $postCmsPCut
    #postFitSelectors  set $postMassCut
    #postFitSelectors  set $postChiSqCut
    #fittingAlgorithm  set "TreeFitter"
    #fitConstraints    set "Geo"
}

talkto D0ToEtaKsPi0 {
    decayMode         set "D0 -> eta K_S0 pi0"
    daughterListNames set "etaDefault"
    daughterListNames set "KsCleanLoose"
    daughterListNames set "pi0AllDefault"
    preFitSelectors   set $preMassCut
    fittingAlgorithm  set "Add4"
    postFitSelectors  set $postCmsPCut
    #postFitSelectors  set $postMassCut
    #postFitSelectors  set $postChiSqCut
    #fittingAlgorithm  set "TreeFitter"
    #fitConstraints    set "Geo"
    #fitConstraints    set "Beam"
}

talkto D0ToEtaKK {
    decayMode         set "D0 -> eta K+ K-"
    daughterListNames set "etaDefault"
    daughterListNames set "KLHLoose"
    daughterListNames set "KLHLoose"
    preFitSelectors   set $preMassCut
    fittingAlgorithm  set "Add4"
    postFitSelectors  set $postCmsPCut
    #postFitSelectors  set $postMassCut
    #postFitSelectors  set $postChiSqCut
    #fittingAlgorithm  set "TreeFitter"
    #fitConstraints    set "Geo"
}

talkto D0ToEtaPiPi {
    decayMode         set "D0 -> eta pi+ pi-"
    daughterListNames set "etaDefault"
    daughterListNames set "piLHLoose"
    daughterListNames set "piLHLoose"
    preFitSelectors   set $preMassCut
    fittingAlgorithm  set "Add4"
    postFitSelectors  set $postCmsPCut
    #postFitSelectors  set $postMassCut
    #postFitSelectors  set $postChiSqCut
    #fittingAlgorithm  set "TreeFitter"
    #fitConstraints    set "Geo"
}

talkto D0ToEta_pPiK {
    decayMode         set "D0 -> eta' pi+ K-"
    daughterListNames set "etaPDefault"
    daughterListNames set "piLHLoose"
    daughterListNames set "KLHLoose"
    preFitSelectors   set $preMassCut
    fittingAlgorithm  set "Add4"
    postFitSelectors  set $postCmsPCut
    #postFitSelectors  set $postMassCut
    #postFitSelectors  set $postChiSqCut
    #fittingAlgorithm  set "TreeFitter"
    #fitConstraints    set "Geo"
}

talkto D0ToEta_pPiK_CS {
    decayMode         set "D0 -> eta' pi- K+"
    daughterListNames set "etaPDefault"
    daughterListNames set "piLHLoose"
    daughterListNames set "KLHLoose"
    preFitSelectors   set $preMassCut
    fittingAlgorithm  set "Add4"
    postFitSelectors  set $postCmsPCut
    #postFitSelectors  set $postMassCut
    #postFitSelectors  set $postChiSqCut
    #fittingAlgorithm  set "TreeFitter"
    #fitConstraints    set "Geo"
}

talkto D0ToEta_pPiPi {
    decayMode         set "D0 -> eta' pi+ pi-"
    daughterListNames set "etaPDefault"
    daughterListNames set "piLHLoose"
    daughterListNames set "piLHLoose"
    preFitSelectors   set $preMassCut
    fittingAlgorithm  set "Add4"
    postFitSelectors  set $postCmsPCut
    #postFitSelectors  set $postMassCut
    #postFitSelectors  set $postChiSqCut
    #fittingAlgorithm  set "TreeFitter"
    #fitConstraints    set "Geo"
}

#Filter on the list
mod clone FilterByList D0To3PSFilter
talkto D0To3PSFilter {
    orList set D0ToEtaPiK
    orList set D0ToEtaPiK_CS
    orList set D0ToEtaKsPi0
    orList set D0ToEtaKK
    orList set D0ToEtaPiPi
    orList set D0ToEta_pPiK
    orList set D0ToEta_pPiK_CS
    orList set D0ToEta_pPiPi
}

path append D0To3PSPath D0To3PSFilter
