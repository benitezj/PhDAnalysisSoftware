#################################################################################
## $Id: LambdacToPKsNoPidPath.tcl,v 1.4 2006/09/18 05:35:37 kflood Exp $
##
## Author: Ryan Mackenzie White, University of South Carolina
##
## Simple Composition sequence for Lambdac->pKs w/o proton PID
##
##################################################################################

path create LambdacToPKsNoPidPath
path append LambdacToPKsNoPidPath FilterBGFMultiHadron
global BtaCandLists
lappend BtaCandLists LambdacToPKsNoPid

mod clone SmpRefinerDefiner Ks_PidLambdac
path append LambdacToPKsNoPidPath Ks_PidLambdac
catch { setProduction Ks_PidLambdac }

#Refine the default Ks list
talkto Ks_PidLambdac {
        decayMode         set "K_S0 -> pi+ pi-"
        unrefinedListName set KsDefault
        createUsrData     set true
        fittingAlgorithm  set "TreeFitter"
        postFitSelectors   set "Mass 0.475:0.520"
        postFitSelectors  set "ProbChiSq 0.00001:"
        postFitSelectors  set "FlightSignificance 0:"
}

##############################
#Simple Composition Cuts
#############################

set preMassCut "Mass 1.9:2.6"
set looseCmsP         "CmsP 2.3:"
set postMassCut "Mass 2.18:2.38"
set CmsP              "CmsP 2.7:"
set postChiSqCut "ProbChiSq 0.01:"

mod clone SmpMakerDefiner LambdacToPKsNoPid
path append LambdacToPKsNoPidPath LambdacToPKsNoPid
catch { setProduction LambdacToPKsNoPid }

talkto LambdacToPKsNoPid {
    decayMode         set "Lambda_c+ -> p+ K_S0"
    daughterListNames set "GoodTracksVeryLoose"
    daughterListNames set "Ks_PidLambdac"
    preFitSelectors   set $preMassCut
    preFitSelectors   set $looseCmsP
    postFitSelectors  set $postMassCut
    postFitSelectors  set $postChiSqCut
    postFitSelectors  set $CmsP
    fittingAlgorithm   set "Cascade"
    fitConstraints     set "Geo"
}

#Filter on the list
mod clone FilterByList LambdacToPKsNoPidFilter
talkto LambdacToPKsNoPidFilter {
    orList set LambdacToPKsNoPid
}

path append LambdacToPKsNoPidPath LambdacToPKsNoPidFilter
