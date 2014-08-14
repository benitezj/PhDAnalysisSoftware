#
# $Id: LambdaCSuppressedPath.tcl,v 1.11 2005/11/16 23:01:58 fwinkl Exp $
#
#
# Define LambdaCSuppressed tag filter path
# (skim of Cabibbo suppressed LambdaC decay modes w/loose PID and p* cut)
#
# New skim definition for >= 14 series
# See http://www.slac.stanford.edu/BFROOT/www/Physics/Analysis/AWG/Charm/newtag/lcbaryonword.html
#
global BtaCandLists

set mypath LambdaCSuppressedPath
set filtername LcWordFilterForLambdaCSupSkim

path create $mypath

# only run on MultiHadrons
path append $mypath FilterBGFMultiHadron

# make a clone of the filter module
mod clone TagFilterMomentumCut $filtername

# Source the file which will define a procedure (called setLcDecayModes)
# for initialising the module to read the Lc word 
sourceFoundFile FilterTools/LcCommonSequence.tcl

path append $mypath TagLcbaryon
setLcDecayModes TagLcbaryon

# Call the tcl proc to initialise the module
setLcDecayModes $filtername

# add filter module
path append $mypath $filtername

# configure filter module
talkto $filtername {
    
    orListCut clear

#	    decayLists set LambdaCTopKpi
    orListCut set 99
#	    decayLists set LambdaCTopKpipi
    orListCut set 99
#	    decayLists set LambdaCTopKs
    orListCut set 99
#	    decayLists set LambdaCTopKspi
    orListCut set 99
#	    decayLists set LambdaCTopKspipi
    orListCut set 99
#	    decayLists set LambdaCToLzpi
    orListCut set 99
#	    decayLists set LambdaCToLzpipi
    orListCut set 99
#	    decayLists set LambdaCToLzpipipi
    orListCut set 99
#	    decayLists set LambdaCToSzpi
    orListCut set 99
#	    decayLists set LambdaCToSzpipipi
    orListCut set 99
#	    decayLists set LambdaCToLzK
    orListCut set 3
#	    decayLists set LambdaCToLzKpi
    orListCut set 3
#	    decayLists set LambdaCToLzKpipi
    orListCut set 3
#	    decayLists set LambdaCToSzK
    orListCut set 3
#	    decayLists set LambdaCToSzKpi
    orListCut set 3
#	    decayLists set LambdaCToSzKpipi
    orListCut set 3

    doHisto   set t
}

# Add lists of composites to write
    
lappend BtaCandLists Lambda_LambdaC
lappend BtaCandLists Sigma_LambdaC

lappend BtaCandLists LambdaCToLzK
lappend BtaCandLists LambdaCToLzKpi
lappend BtaCandLists LambdaCToLzKpipi
lappend BtaCandLists LambdaCToSzK
lappend BtaCandLists LambdaCToSzKpi
lappend BtaCandLists LambdaCToSzKpipi
