#
# $Id: D0To2ProngDcjPath.tcl,v 1.9 2008/11/29 21:31:36 rmwhite Exp $
#
############################################################################
##
## Defines D0To2ProngDcjPath tag filter path
##
## This filter searches for the decays:
##
##   D*+ -> D0 pi+
##          D0 -> K-  pi+
##          D0 -> K-  K+
##          D0 -> pi- K+
##          D0 -> pi- pi+
##
##
############################################################################

path create D0To2ProngDcjPath

set D0LooseMass "Mass 1.7745:1.9545"

global BtaCandLists
lappend BtaCandLists DstarToD0Pi_D0To2Prong_DCJ
lappend BtaCandLists D0To2ProngUntagged_DCJ
lappend BtaCandLists D0ToKPiTightPID_DCJ
lappend BtaCandLists D0ToKKTightPID_DCJ
lappend BtaCandLists D0ToPiKTightPID_DCJ
lappend BtaCandLists D0ToPiPiTight_DCJ


   ##################
   ## D0 -> K pi
   ##################

    
   mod clone SmpRefinerDefiner D0ToKPiTightPID_DCJ
   path append D0To2ProngDcjPath D0ToKPiTightPID_DCJ
   catch { setProduction D0ToKPiTightPID_DCJ }
   talkto D0ToKPiTightPID_DCJ {
       debug             set f
       verbose           set f
 
       decayMode         set "D0 -> K- pi+"

    unrefinedListName  set "D0ToKPiLoose"

    daughterListNames set "KCombinedSuperLoose"
    daughterListNames set "piCombinedSuperLoose"

##     choose fitting constraint and method

    fittingAlgorithm  set "Cascade"

    fitConstraints    set "Geo"
    fitConstraints    set "Momentum"

##     postFitSelectors chisquare, CMS p*
    postFitSelectors  set "ProbChiSq 0.005:1.0"
    postFitSelectors  set "CmsP 2.4:10.0"


   }
    
    
   ##################
   ## D0 -> K K
   ##################
    
   mod clone SmpRefinerDefiner D0ToKKTightPID_DCJ
   path append D0To2ProngDcjPath D0ToKKTightPID_DCJ
   catch { setProduction D0ToKKTightPID_DCJ }
   talkto D0ToKKTightPID_DCJ {
       debug             set f
       verbose           set f
 
       decayMode         set "D0 -> K+ K-"
       unrefinedListName  set "D0ToKKLoose"
       daughterListNames set "KCombinedSuperLoose"
       daughterListNames set "KCombinedSuperLoose"

##     choose fitting constraint and method

    fittingAlgorithm  set "Cascade"

    fitConstraints    set "Geo"
    fitConstraints    set "Momentum"

##     postFitSelectors chisquare, CMS p*
    postFitSelectors  set "ProbChiSq 0.005:1.0"
    postFitSelectors  set "CmsP 2.4:10.0"


   }


   ##########################################
   ## D0 -> pi K : Doubly Cabibbo Suppressed
   ##########################################

    
   mod clone SmpRefinerDefiner D0ToPiKTightPID_DCJ
   path append D0To2ProngDcjPath D0ToPiKTightPID_DCJ
   catch { setProduction D0ToPiKTightPID_DCJ }
   talkto D0ToPiKTightPID_DCJ {
       debug             set f
       verbose           set f
 
       decayMode         set "D0 -> K+ pi-"
       checkType         set false

       unrefinedListName  set "D0ToKPiTightPID_DCJ"

       fittingAlgorithm  set "Cascade"

       fitConstraints    set "Geo"
       fitConstraints    set "Momentum"
   }


   ##################
   ## D0 -> pi pi
   ##################
    
   mod clone SmpRefinerDefiner D0ToPiPiTight_DCJ
   path append D0To2ProngDcjPath D0ToPiPiTight_DCJ
   catch { setProduction D0ToPiPiTight_DCJ }
   talkto D0ToPiPiTight_DCJ {
       debug             set f
       verbose           set f
 
       decayMode         set "D0 -> pi+ pi-"
       unrefinedListName set "D0ToPiPiLoose"
       daughterListNames set "piCombinedSuperLoose"
       daughterListNames set "piCombinedSuperLoose"

##     choose fitting constraint and method

    fittingAlgorithm  set "Cascade"

    fitConstraints    set "Geo"
    fitConstraints    set "Momentum"

##     postFitSelectors chisquare, CMS p*
    postFitSelectors  set "ProbChiSq 0.005:1.0"
    postFitSelectors  set "CmsP 2.4:10.0"


   }

   ##################
   ## Merge Modes
   ##################

mod clone SmpMergerDefiner D0To2ProngUntagged_DCJ
path append D0To2ProngDcjPath D0To2ProngUntagged_DCJ
catch { setProduction D0To2ProngUntagged_DCJ }
talkto D0To2ProngUntagged_DCJ {
    debug             set f
    verbose           set f

    ## disableCloneCheck=true is necessary to keep both the
    ## right-sign and wrong-sign combinations
    disableCloneCheck set true

    inputListNames    set "D0ToKPiTightPID_DCJ"
    inputListNames    set "D0ToKKTightPID_DCJ"
}

mod clone SmpMergerDefiner D0To2Prong_DCJ
path append D0To2ProngDcjPath D0To2Prong_DCJ
catch { setProduction D0To2Prong_DCJ }
talkto D0To2Prong_DCJ {
    debug             set f
    verbose           set f

    ## disableCloneCheck=true is necessary to keep both the
    ## right-sign and wrong-sign combinations
    disableCloneCheck set true

#    inputListNames    set "D0ToKPiTightPID_DCJ"
#    inputListNames    set "D0ToKKTightPID_DCJ"
#    inputListNames    set "D0ToPiKTightPID_DCJ"
    inputListNames    set "D0To2ProngUntagged_DCJ"
    inputListNames    set "D0ToPiPiTight_DCJ"
    inputListNames    set "D0ToPiKTightPID_DCJ"
}


mod clone SmpMakerDefiner DstarToD0Pi_D0To2Prong_DCJ
path append D0To2ProngDcjPath DstarToD0Pi_D0To2Prong_DCJ
catch { setProduction DstarToD0Pi_D0To2Prong_DCJ }
talkto DstarToD0Pi_D0To2Prong_DCJ {
    debug             set f
    verbose           set f

    decayMode         set "D*+ -> D0 pi+"

    daughterListNames set "D0To2Prong_DCJ"
    daughterListNames set "GoodTracksVeryLoose"

    fittingAlgorithm  set "Cascade"

    fitConstraints    set "Geo"
    fitConstraints    set "Momentum"
    fitConstraints    set "Beam"

    preFitSelectors   set "DeltaM 0.130:0.170"

# disable ProbChiSq cut for now
#    postFitSelectors  set "ProbChiSq 0.01:1.0"
    postFitSelectors  set "DeltaM 0.130:0.160"
#    postFitSelectors  set "CmsP 2.4:10.0"
##   CmsP used in D0 selection instead
}


path append D0To2ProngDcjPath D0To2ProngDcjSkim
talkto D0To2ProngDcjSkim {
    inputDstarListName set "DstarToD0Pi_D0To2Prong_DCJ"
    inputD0UntaggedListName set "D0To2ProngUntagged_DCJ"
}
