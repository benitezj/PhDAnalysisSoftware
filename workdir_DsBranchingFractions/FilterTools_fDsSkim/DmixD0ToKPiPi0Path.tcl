############################################################################
## $Id: DmixD0ToKPiPi0Path.tcl,v 1.9 2008/11/29 21:31:37 rmwhite Exp $
##
## Defines the DmixD0ToKPiPi0Path skim filter path
##
## This filter searches for the decay:
##
##   D*+ -> D0 pi+
##          D0 -> K pi pi0
##
## with a momentum cut on the D0 c.m. mometum, searching for both
## Cabibbo-favored (K-pi+pi0) and doubly-Cabibbo suppressed (K+pi-pi0)
## decays for a D-mixing analysis or a Dalitz analysis.
##
## Michael Wilson, UC Santa Cruz
############################################################################

path create DmixD0ToKPiPi0Path

global BtaCandLists
lappend BtaCandLists DstarToD0Pi_D0ToKPiPi0VeryLoose_Dmix_prescale DstarToD0Pi_D0ToKPiPi0_Dmix

global writeUsrDataForSkim
set writeUsrDataForSkim 1


mod clone SmpSubListerDefiner gamma_Dmix
path append DmixD0ToKPiPi0Path gamma_Dmix
catch { setProduction gamma_Dmix }
talkto gamma_Dmix {
    debug             set f
    verbose           set f

    unrefinedListName set "GoodPhotonLoose"

    selectors   set "LabE 0.100:10.0"
    selectors   set "Lat 0.01:0.8"
}


mod clone SmpRefinerDefiner pi0Unmerged_Dmix
path append DmixD0ToKPiPi0Path pi0Unmerged_Dmix
catch { setProduction pi0Unmerged_Dmix }
talkto pi0Unmerged_Dmix {
    debug              set f
    verbose            set f

    decayMode         set "pi0 -> gamma gamma"

    unrefinedListName  set "pi0VeryLoose"

    daughterListNames set "gamma_Dmix"
    daughterListNames set "gamma_Dmix"

    fittingAlgorithm   set "Cascade"

    fitConstraints     set "Mass"
    fitConstraints     set "Momentum"
    fitConstraints     set "PrimaryVertex"
    
    postFitSelectors   set "ProbChiSq 0.01:1.0"
}


mod clone SmpMergerDefiner pi0All_Dmix
path append DmixD0ToKPiPi0Path pi0All_Dmix
catch { setProduction pi0All_Dmix }
talkto pi0All_Dmix {
    debug             set f
    verbose           set f

    inputListNames    set "pi0Unmerged_Dmix"
    inputListNames    set "MergedPi0Loose"
}


## Cabibbo-favored mode
mod clone SmpRefinerDefiner D0ToKPiPi0CF_Loose_Dmix
path append DmixD0ToKPiPi0Path D0ToKPiPi0CF_Loose_Dmix
catch { setProduction D0ToKPiPi0CF_Loose_Dmix }
talkto D0ToKPiPi0CF_Loose_Dmix {
    debug              set f
    verbose            set f

    decayMode          set "D0 -> K- pi+ pi0"

    unrefinedListName  set "D0ToKPiPi0CFVeryLoose"

    daughterListNames  set "GoodTracksLoose"
    daughterListNames  set "GoodTracksLoose"
    daughterListNames  set "pi0All_Dmix"

    fittingAlgorithm   set "Cascade"

    fitConstraints     set "Geo"
    fitConstraints     set "Momentum"

    postFitSelectors   set "ProbChiSq 0.005:1.0"
    postFitSelectors   set "Mass 1.68:2.02"
    postFitSelectors   set "CmsP 2.2:10.0"
}


mod clone SmpSubListerDefiner D0ToKPiPi0CF_Dmix
path append DmixD0ToKPiPi0Path D0ToKPiPi0CF_Dmix
catch { setProduction D0ToKPiPi0CF_Dmix }
talkto D0ToKPiPi0CF_Dmix {
    debug             set f
    verbose           set f
    
    decayMode         set "D0 -> K- pi+ pi0"
    
    unrefinedListName set "D0ToKPiPi0CF_Loose_Dmix"
    
    daughterListNames set "KLHTight"
    daughterListNames set "piLHTight"
    daughterListNames set "pi0All_Dmix"
}


## doubly Cabibbo-suppressed mode
mod clone SmpRefinerDefiner D0ToKPiPi0DCS_Loose_Dmix
path append DmixD0ToKPiPi0Path D0ToKPiPi0DCS_Loose_Dmix
catch { setProduction D0ToKPiPi0DCS_Loose_Dmix }
talkto D0ToKPiPi0DCS_Loose_Dmix {
    debug              set f
    verbose            set f

    decayMode          set "D0 -> K+ pi- pi0"
    checkType          set false

    unrefinedListName  set "D0ToKPiPi0CF_Loose_Dmix"

    fittingAlgorithm   set "Cascade"

    fitConstraints     set "Geo"
    fitConstraints     set "Momentum"
}


mod clone SmpSubListerDefiner D0ToKPiPi0DCS_Dmix
path append DmixD0ToKPiPi0Path D0ToKPiPi0DCS_Dmix
catch { setProduction D0ToKPiPi0DCS_Dmix }
talkto D0ToKPiPi0DCS_Dmix {
    debug             set f
    verbose           set f
    
    decayMode         set "D0 -> K+ pi- pi0"
    
    unrefinedListName set "D0ToKPiPi0DCS_Loose_Dmix"
    
    daughterListNames set "KCombinedSuperLoose"
    daughterListNames set "piCombinedSuperLoose"
    daughterListNames set "pi0All_Dmix"
}


mod clone SmpMergerDefiner D0ToKPiPi0_Loose_Dmix
path append DmixD0ToKPiPi0Path D0ToKPiPi0_Loose_Dmix
catch { setProduction D0ToKPiPi0_Loose_Dmix }
talkto D0ToKPiPi0_Loose_Dmix {
    debug             set f
    verbose           set f

    ## disableCloneCheck=true is necessary to keep both the
    ## right-sign and wrong-sign combinations
    disableCloneCheck set true

    inputListNames    set "D0ToKPiPi0CF_Loose_Dmix"
    inputListNames    set "D0ToKPiPi0DCS_Loose_Dmix"
}


mod clone SmpMergerDefiner D0ToKPiPi0_Dmix
path append DmixD0ToKPiPi0Path D0ToKPiPi0_Dmix
catch { setProduction D0ToKPiPi0_Dmix }
talkto D0ToKPiPi0_Dmix {
    debug             set f
    verbose           set f

    ## disableCloneCheck=true is necessary to keep both the
    ## right-sign and wrong-sign combinations
    disableCloneCheck set true

    inputListNames    set "D0ToKPiPi0CF_Dmix"
    inputListNames    set "D0ToKPiPi0DCS_Dmix"
}


mod clone SmpMakerDefiner DstarToD0Pi_D0ToKPiPi0VeryLoose_Dmix_init
path append DmixD0ToKPiPi0Path DstarToD0Pi_D0ToKPiPi0VeryLoose_Dmix_init
catch { setProduction DstarToD0Pi_D0ToKPiPi0VeryLoose_Dmix_init }
talkto DstarToD0Pi_D0ToKPiPi0VeryLoose_Dmix_init {
    debug             set f
    verbose           set f

    disableCloneCheck set true

    decayMode         set "D*+ -> D0 pi+"

    daughterListNames set "D0ToKPiPi0_Loose_Dmix"
    daughterListNames set "GoodTracksVeryLoose"

    fittingAlgorithm  set "Cascade"

    fitConstraints    set "Geo"
    fitConstraints    set "Momentum"
    fitConstraints    set "Beam"

    preFitSelectors   set "DeltaM 0.130:0.170"

    postFitSelectors  set "DeltaM 0.139:0.160"
}


mod clone SmpSubListerDefiner DstarToD0Pi_D0ToKPiPi0_Dmix_init
path append DmixD0ToKPiPi0Path DstarToD0Pi_D0ToKPiPi0_Dmix_init
catch { setProduction DstarToD0Pi_D0ToKPiPi0_Dmix_init }
talkto DstarToD0Pi_D0ToKPiPi0_Dmix_init {
    debug             set f
    verbose           set f
    
    disableCloneCheck set true

    decayMode         set "D*+ -> D0 pi+"
    
    unrefinedListName set "DstarToD0Pi_D0ToKPiPi0VeryLoose_Dmix_init"
    
    daughterListNames set "D0ToKPiPi0_Dmix"
    daughterListNames set "GoodTracksVeryLoose"
}


path append DmixD0ToKPiPi0Path DmixD0ToKPiPi0Skim
talkto DmixD0ToKPiPi0Skim {
    prescaledDstarListName set "DstarToD0Pi_D0ToKPiPi0VeryLoose_Dmix_init"
    inputDstarListName    set "DstarToD0Pi_D0ToKPiPi0_Dmix_init"
    scaledDstarListName   set "DstarToD0Pi_D0ToKPiPi0VeryLoose_Dmix_prescale"
    finalDstarListName    set "DstarToD0Pi_D0ToKPiPi0_Dmix"

    prescaleFactor        set 10
}


## Make sure the UsrData is confiugred the same as the skim
talkto UsrWriteDmixD0ToKPiPi0 {
    scaledDstarListName   set "DstarToD0Pi_D0ToKPiPi0VeryLoose_Dmix_prescale"
    finalDstarListName    set "DstarToD0Pi_D0ToKPiPi0_Dmix"
    inputPi0ListName      set "pi0Unmerged_Dmix"
}

