#
# $Id: IsrDsmesonPath.tcl,v 1.4 2005/03/14 17:28:09 fnc Exp $
#
##
# defines IsrDsmesonPath tag filter path
#
# Author:  Joe Izen    UTDallas
#

path create IsrDsmesonPath

# append the IsrTagbit Filter
mod clone IsrTagbitFilter IsrDsmesonFilter
path append IsrDsmesonPath IsrDsmesonFilter

# filter for the presence of Ds above ISR threshold
mod clone TagFilterMomentumCut IsrDsFilter
path append IsrDsmesonPath IsrDsFilter


path enable IsrDsmesonPath

#
# Set up the Isr and Ds filters to do our bidding
#

#tell IsrDsmesonFilter to pass both Gamma and NoGamma candidates
mod talk IsrDsmesonFilter
    verbose set f
    enableGammaISR      set t
    enableNoGammaISR    set t
    enableLooseGammaISR set t
    enableRadBhabha     set f
#   echo IsrDsmesonPath has the following settings for IsrCharmFilter
#   show
exit

# Theshold 2 (counting from 0) is 0.85 GeV, the min for ISR production
#  these thresholds and and decay lists must match EventTagTools/TagProdCreateSequence.tcl
talkto IsrDsFilter {
    tagPrefix set Dsmeson
    momentumThresholds clear
    momentumThresholds set -0.1
    momentumThresholds set 0.5
    momentumThresholds set 0.85
    momentumThresholds set 1.3
    momentumThresholds set 1.5
    momentumThresholds set 1.8
    momentumThresholds set 2.2
    decayLists clear
    decayLists set DsToPhiPiLoosePID
    decayLists set Ds3prongLoosePID
    decayLists set DsToPhiPiPiPiLoosePID
    decayLists set DsToPhiRhoCLoosePID
    decayLists set DsToKsKPiPiLoosePID 
    decayLists set DsToEtaPiLoosePID
    decayLists set DsToEtaRhoCLoosePID
    decayLists set DsToEtaPPiLoosePID
    decayLists set DsToEtaPRhoCLoosePID
    decayLists set LambdaCToPKPiLoosePID 

#mom cut 2 (0.85 GeV) is for ISR Ds
    orListCut clear
    orListCut set 2
    orListCut set 2
    orListCut set 2
    orListCut set 2
    orListCut set 2
    orListCut set 2
    orListCut set 2
    orListCut set 2
    orListCut set 2
    orListCut set 2

    doHisto set false
}








