############################################################################
## $Id: D0To2ProngPi0CSPath.tcl,v 1.6 2008/11/29 21:31:36 rmwhite Exp $
##
## Defines the D0To2ProngPi0CS skim filter path
##
## This filter searches for the decay:
##
##   D*+ -> D0 pi+
##          D0 -> K- K+ pi0
##          D0 -> pi- pi+ pi0
##
## with a momentum cut on the D0 c.m. mometum.
##
## Michael Wilson, UC Santa Cruz
############################################################################

path create D0To2ProngPi0CSPath

global BtaCandLists
lappend BtaCandLists DstarToD0Pi_D0ToKKPi0_2PPi0CS DstarToD0Pi_D0ToPiPiPi0_2PPi0CS


mod clone SmpSubListerDefiner gamma_2PPi0CS
path append D0To2ProngPi0CSPath gamma_2PPi0CS
catch { setProduction gamma_2PPi0CS }
talkto gamma_2PPi0CS {
    debug             set f
    verbose           set f

    unrefinedListName set "GoodPhotonLoose"

    selectors   set "LabE 0.100:10.0"
    selectors   set "Lat 0.01:0.8"
}


mod clone SmpRefinerDefiner pi0Unmerged_2PPi0CS
path append D0To2ProngPi0CSPath pi0Unmerged_2PPi0CS
catch { setProduction pi0Unmerged_2PPi0CS }
talkto pi0Unmerged_2PPi0CS {
    debug              set f
    verbose            set f

    decayMode         set "pi0 -> gamma gamma"

    unrefinedListName  set "pi0VeryLoose"

    daughterListNames set "gamma_2PPi0CS"
    daughterListNames set "gamma_2PPi0CS"

    fittingAlgorithm   set "Cascade"

    fitConstraints     set "Mass"
    fitConstraints     set "Momentum"
    fitConstraints     set "PrimaryVertex"
    
    postFitSelectors   set "ProbChiSq 0.01:1.0"
}


mod clone SmpMergerDefiner pi0All_2PPi0CS
path append D0To2ProngPi0CSPath pi0All_2PPi0CS
catch { setProduction pi0All_2PPi0CS }
talkto pi0All_2PPi0CS {
    debug             set f
    verbose           set f

    inputListNames    set "pi0Unmerged_2PPi0CS"
    inputListNames    set "MergedPi0Loose"
}


mod clone SmpRefinerDefiner D0ToKKPi0_2PPi0CS
path append D0To2ProngPi0CSPath D0ToKKPi0_2PPi0CS
catch { setProduction D0ToKKPi0_2PPi0CS }
talkto D0ToKKPi0_2PPi0CS {
    debug              set f
    verbose            set f

    disableCloneCheck  set true

    decayMode          set "D0 -> K+ K- pi0"

    unrefinedListName  set "D0ToKKPi0VeryLoose"

    daughterListNames  set "KCombinedSuperLoose"
    daughterListNames  set "KCombinedSuperLoose"
    daughterListNames  set "pi0All_2PPi0CS"

    fittingAlgorithm   set "Cascade"

    fitConstraints     set "Geo"
    fitConstraints     set "Momentum"

    postFitSelectors   set "ProbChiSq 0.005:1.0"
    postFitSelectors   set "Mass 1.68:2.12"
    postFitSelectors   set "CmsP 2.4:10.0"
}


mod clone SmpRefinerDefiner D0ToPiPiPi0_2PPi0CS
path append D0To2ProngPi0CSPath D0ToPiPiPi0_2PPi0CS
catch { setProduction D0ToPiPiPi0_2PPi0CS }
talkto D0ToPiPiPi0_2PPi0CS {
    debug              set f
    verbose            set f

    disableCloneCheck  set true

    decayMode          set "D0 -> pi+ pi- pi0"

    unrefinedListName  set "D0ToPiPiPi0VeryLoose"

    daughterListNames  set "piCombinedSuperLoose"
    daughterListNames  set "piCombinedSuperLoose"
    daughterListNames  set "pi0All_2PPi0CS"

    fittingAlgorithm   set "Cascade"

    fitConstraints     set "Geo"
    fitConstraints     set "Momentum"

    postFitSelectors   set "ProbChiSq 0.005:1.0"
    postFitSelectors   set "Mass 1.68:2.12"
    postFitSelectors   set "CmsP 2.4:10.0"
}


mod clone SmpMakerDefiner DstarToD0Pi_D0ToKKPi0_2PPi0CS_init
path append D0To2ProngPi0CSPath DstarToD0Pi_D0ToKKPi0_2PPi0CS_init
catch { setProduction DstarToD0Pi_D0ToKKPi0_2PPi0CS_init }
talkto DstarToD0Pi_D0ToKKPi0_2PPi0CS_init {
    debug             set f
    verbose           set f

    disableCloneCheck set true

    decayMode         set "D*+ -> D0 pi+"

    daughterListNames set "D0ToKKPi0_2PPi0CS"
    daughterListNames set "GoodTracksVeryLoose"

    fittingAlgorithm  set "Cascade"

    fitConstraints    set "Geo"
    fitConstraints    set "Momentum"
    fitConstraints    set "Beam"

    preFitSelectors   set "DeltaM 0.130:0.170"

    postFitSelectors  set "DeltaM 0.139:0.160"
}


mod clone SmpMakerDefiner DstarToD0Pi_D0ToPiPiPi0_2PPi0CS_init
path append D0To2ProngPi0CSPath DstarToD0Pi_D0ToPiPiPi0_2PPi0CS_init
catch { setProduction DstarToD0Pi_D0ToPiPiPi0_2PPi0CS_init }
talkto DstarToD0Pi_D0ToPiPiPi0_2PPi0CS_init {
    debug             set f
    verbose           set f

    disableCloneCheck set true

    decayMode         set "D*+ -> D0 pi+"

    daughterListNames set "D0ToPiPiPi0_2PPi0CS"
    daughterListNames set "GoodTracksVeryLoose"

    fittingAlgorithm  set "Cascade"

    fitConstraints    set "Geo"
    fitConstraints    set "Momentum"
    fitConstraints    set "Beam"

    preFitSelectors   set "DeltaM 0.130:0.170"

    postFitSelectors  set "DeltaM 0.139:0.160"
}


path append D0To2ProngPi0CSPath D0To2ProngPi0CSSkim
talkto D0To2ProngPi0CSSkim {
    inputDstarListName1    set "DstarToD0Pi_D0ToKKPi0_2PPi0CS_init"
    inputDstarListName2    set "DstarToD0Pi_D0ToPiPiPi0_2PPi0CS_init"
    finalDstarListName1    set "DstarToD0Pi_D0ToKKPi0_2PPi0CS"
    finalDstarListName2    set "DstarToD0Pi_D0ToPiPiPi0_2PPi0CS"
}
