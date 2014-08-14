#
# $Id: LambdaCPath.tcl,v 1.13 2008/12/01 22:20:07 gjackson Exp $
#
#
# Define LambdaC tag filter path
# (Skim of LambdaC decays for charm baryon searches and LambdaC studies)
#
# New skim definition for >= 14 series
# See http://www.slac.stanford.edu/BFROOT/www/Physics/Analysis/AWG/Charm/newtag/lcbaryonword.html
#

global BtaCandLists
global writeUsrDataForSkim
global UsrBCandLists

set mypath LambdaCPath
set filtername LcWordFilterForLambdaCSkim

path create $mypath

# only run on MultiHadrons
path append $mypath FilterBGFMultiHadron

# make a clone of the filter module
mod clone TagFilterMomentumCut $filtername

# Source the file which will define a procedure (called setLcDecayModes)
# for initialising the module to read the Lc word 
sourceFoundFile FilterTools/LcCommonSequence.tcl

sourceFoundFile EventTagTools/TagProcs.tcl
addTagModule TagLcbaryon $mypath
setLcDecayModes TagLcbaryon

# Call the tcl proc to initialise the module
setLcDecayModes $filtername

# add filter module
path append $mypath $filtername

# configure filter module
talkto $filtername {
    orListCut clear
#	    decayLists set LambdaCTopKpi
    orListCut set 1
#	    decayLists set LambdaCTopKpipi
    orListCut set 1
#	    decayLists set LambdaCTopKs
    orListCut set 1
#	    decayLists set LambdaCTopKspi
    orListCut set 99
#	    decayLists set LambdaCTopKspipi
    orListCut set 1
#	    decayLists set LambdaCToLzpi
    orListCut set 1
#	    decayLists set LambdaCToLzpipi
    orListCut set 1
#	    decayLists set LambdaCToLzpipipi
    orListCut set 1
#	    decayLists set LambdaCToSzpi
    orListCut set 1
#	    decayLists set LambdaCToSzpipipi
    orListCut set 1
#	    decayLists set LambdaCToLzK
    orListCut set 99
#	    decayLists set LambdaCToLzKpi
    orListCut set 99
#	    decayLists set LambdaCToLzKpipi
    orListCut set 99
#	    decayLists set LambdaCToSzK
    orListCut set 99
#	    decayLists set LambdaCToSzKpi
    orListCut set 99
#	    decayLists set LambdaCToSzKpipi
    orListCut set 99
 
    doHisto   set t
}

# Add lists of composites to write
lappend BtaCandLists KsLoose_LambdaC
lappend BtaCandLists Lambda_LambdaC
lappend BtaCandLists Sigma_LambdaC

lappend BtaCandLists LambdaCTopKpi
lappend BtaCandLists LambdaCTopKs
lappend BtaCandLists LambdaCTopKspipi
lappend BtaCandLists LambdaCToLzpi
lappend BtaCandLists LambdaCToLzpipipi
lappend BtaCandLists LambdaCToSzpi
lappend BtaCandLists LambdaCToSzpipipi
