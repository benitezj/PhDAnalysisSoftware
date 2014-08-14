############################################################################
## $Id: DmixD0ToK3PiPath.tcl,v 1.6 2008/11/29 21:31:36 rmwhite Exp $
##
## Defines the DmixD0ToK3PiPath skim filter path
##
## This filter searches for the decay:
##
##   D*+ -> D0 pi+
##          D0 -> K pi pi+ pi-
##
## with a momentum cut on the D0 c.m. mometum, searching for both
## Cabibbo-favored (K-pi+pi+pi-) and doubly-Cabibbo suppressed (K+pi-pi+pi-)
## decays for a D-mixing analysis.
##
## Michael Wilson, UC Santa Cruz
############################################################################

path create DmixD0ToK3PiPath

global BtaCandLists
lappend BtaCandLists DstarToD0Pi_D0ToK3Pi_Dmix


## Cabibbo-favored mode
mod clone SmpRefinerDefiner D0ToK3PiCF_Dmix
path append DmixD0ToK3PiPath D0ToK3PiCF_Dmix
catch { setProduction D0ToK3PiCF_Dmix }
talkto D0ToK3PiCF_Dmix {
    debug              set f
    verbose            set f

    decayMode          set "D0 -> K- pi+ pi- pi+"
    unrefinedListName  set "D0ToK3PiLoose"

    daughterListNames  set "KCombinedSuperLoose"
    daughterListNames  set "piCombinedSuperLoose"
    daughterListNames  set "GoodTracksLoose"
    daughterListNames  set "GoodTracksLoose"
    
    fittingAlgorithm   set "Cascade"

    fitConstraints     set "Geo"
    fitConstraints     set "Momentum"

    postFitSelectors   set "ProbChiSq 0.005:1.0"
    postFitSelectors   set "Mass 1.70:2.00"
    postFitSelectors   set "CmsP 2.2:10.0"
}


## doubly Cabibbo-suppressed mode
mod clone SmpRefinerDefiner D0ToK3PiDCS_Dmix
path append DmixD0ToK3PiPath D0ToK3PiDCS_Dmix
catch { setProduction D0ToK3PiDCS_Dmix }
talkto D0ToK3PiDCS_Dmix {
    debug             set f
    verbose           set f

    decayMode         set "D0 -> K+ pi- pi+ pi-"
    checkType         set false

    unrefinedListName set "D0ToK3PiCF_Dmix"

    fittingAlgorithm   set "Cascade"

    fitConstraints     set "Geo"
    fitConstraints     set "Momentum"
}


mod clone SmpMergerDefiner D0ToK3Pi_Dmix
path append DmixD0ToK3PiPath D0ToK3Pi_Dmix
catch { setProduction D0ToK3Pi_Dmix }
talkto D0ToK3Pi_Dmix {
    debug             set f
    verbose           set f

    ## disableCloneCheck=true is necessary to keep both the
    ## right-sign and wrong-sign combinations
    disableCloneCheck set true

    inputListNames    set "D0ToK3PiCF_Dmix"
    inputListNames    set "D0ToK3PiDCS_Dmix"
}


mod clone SmpMakerDefiner DstarToD0Pi_D0ToK3Pi_Dmix_init
path append DmixD0ToK3PiPath DstarToD0Pi_D0ToK3Pi_Dmix_init
catch { setProduction DstarToD0Pi_D0ToK3Pi_Dmix_init }
talkto DstarToD0Pi_D0ToK3Pi_Dmix_init {
    debug             set f
    verbose           set f

    disableCloneCheck set true

    decayMode         set "D*+ -> D0 pi+"

    daughterListNames set "D0ToK3Pi_Dmix"
    daughterListNames set "GoodTracksVeryLoose"

    fittingAlgorithm  set "Cascade"

    fitConstraints    set "Geo"
    fitConstraints    set "Momentum"
    fitConstraints    set "Beam"

    preFitSelectors   set "DeltaM 0.130:0.170"

    postFitSelectors  set "DeltaM 0.139:0.160"
}


path append DmixD0ToK3PiPath DmixD0ToK3PiSkim
talkto DmixD0ToK3PiSkim {
    inputDstarListName    set "DstarToD0Pi_D0ToK3Pi_Dmix_init"
    finalDstarListName    set "DstarToD0Pi_D0ToK3Pi_Dmix"
}

