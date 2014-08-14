#
# $Id: IsrDmesonPath.tcl,v 1.7 2005/03/14 17:28:08 fnc Exp $
#
##
# defines IsrDmesonPath tag filter path
#
# Author:  Joe Izen    UTDallas
#

path create IsrDmesonPath

# append the IsrTagbit Filter
mod clone IsrTagbitFilter IsrDmesonFilter
path append IsrDmesonPath IsrDmesonFilter

# filter for the presence of D mesons above ISR threshold
mod clone TagFilterMomentumCut IsrDFilter
path append IsrDmesonPath IsrDFilter
mod enable IsrDFilter

#
# Set up the Isr and Ds filters to do our bidding
#

#tell IsrDmesonFilter to pass both Gamma and NoGamma candidates
mod talk IsrDmesonFilter
    verbose set f
    enableGammaISR      set t
    enableNoGammaISR    set t
    enableLooseGammaISR set t
    enableRadBhabha     set f
#   echo IsrDmesonPath has the following settings for IsrDmesonFilter
#   show
exit

# Theshold 3 (counting from 0) is 1.30 GeV, the min for ISR production
#  these thresholds and and decay lists must match EventTagTools/TagProdCreateSequence.tcl
talkto IsrDFilter {
    tagPrefix set Dmeson
    momentumThresholds clear
    momentumThresholds set -0.1
    momentumThresholds set 0.5 
    momentumThresholds set 1.0 
    momentumThresholds set 1.3    
    momentumThresholds set 1.5    
    momentumThresholds set 1.8    
    momentumThresholds set 2.2    
    decayLists clear
    decayLists set D0ToKPiLoosePID
    decayLists set D0MainLoosePID
    decayLists set D0CabibboLoosePID
    decayLists set D0LoosePID
    decayLists set Dc3prongLoosePID
    decayLists set DcToKsPiPi0LoosePID
    decayLists set DcToKsPiPiPiLoosePID
    decayLists set DcToKPiPiPi0LoosePID
    decayLists set DcToKsKPiPiLoosePID
    decayLists set DcToKPiPiLoosePID

    orListCut clear
    orListCut set 3
    orListCut set 3
    orListCut set 3
    orListCut set 3
    orListCut set 3
    orListCut set 3
    orListCut set 3
    orListCut set 3
    orListCut set 3
    orListCut set 3

    doHisto set false
}





