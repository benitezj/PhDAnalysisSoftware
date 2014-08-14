#
# $Id: ExclHllMiniPath.tcl,v 1.16 2005/03/14 17:28:08 fnc Exp $
#
#
# defines the path for ExclHllMiniFilter
# 

# Write UsrData
global writeUsrDataForSkim
set writeUsrDataForSkim 1

# Add lists of composites to write
global BtaCandLists
lappend BtaCandLists ExclHllSkimmedBList

path create ExclHllMiniPath

sourceFoundFile CompositionSequences/CompHllSequence.tcl

# Pre-filter on tag bits
module clone KllTagFilter ExclHllPreFilter
path append ExclHllMiniPath ExclHllPreFilter
path append ExclHllMiniPath CompHllSequence

talkto ExclHllPreFilter {
    R2AllCut set 1.0
    electronPCut set 0.3
    muonPCut set 0.7
    NLepCut set 1
}

## Add brem reco-ed tracks to the list of lepton candidates
set bremRecovery on

if {$bremRecovery == "on"} {
    path append ExclHllMiniPath ExclHllMiniFilter  
    mod talk ExclHllMiniFilter
    btaTrackList set eBremRecoHllMerge
    idElList set eBremRecoPidHllMerge
    idMuList set muNNVeryLoose
    exit
} else {
    path append ExclHllMiniPath ExclHllMiniFilter  
    mod talk ExclHllMiniFilter
    btaTrackList set ChargedTracks
    idElList set PidLHElectrons
    idMuList set muNNVeryLoose
    exit
}
## End brem reco

# Set verbosity level
# 0 = none, 1 = event info, 2 = candidate info
mod talk ExclHllMiniFilter
debug set 0
exit

# Setup candidate lists from composition tools
mod talk ExclHllMiniFilter
production set true
piZeroInputList set pi0VeryLoose
ksInputList set KsToPiPi_Hll
etaInputList set EtaToGG_Hll
kStarCKsInputList set KstarToKsPi_Hll
kStarCKInputList set KstarToKPi0_Hll
kStar0KInputList set KstarToKPi_Hll
kStar0KsInputList set KstarToKsPi0_Hll
rhoChInputList set RhoCToPiPi0_Hll
rhoZeroInputList set Rho0ToPiPi_Hll
omegaInputList set OmegaToPiPiPi0_Hll
phiInputList set PhiToKK_Hll
etaPrInputList set EtaPToPiPiEta_Hll
exit

# Set modes to skim on. All modes are enabled by default
mod talk ExclHllMiniFilter
#firstCandOnly set false
#doKll set false
#doKsll set false
#doKstar0ll set false
#doKstar0Pi0ll set false
#doKstarChll set false
#doKstarChPi0ll set false
#doPiChll set false
#doPi0ll set false
#doRho0ll set false
#doRhoChll set false
doOmegall set false
#doPhill set false
#doEtall set false
doEtaPrll set false
#doEE set false
#doMuMu set false
#doEMu set false
#doMuE set false
exit
