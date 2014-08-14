# $Id: defineMiniSkims_R24c.tcl,v 1.6 2009/02/21 01:29:48 homer Exp $
#
# this file defines paths for SkimMiniApp
#
# Valid choices for the 2nd and 3rd arguments to declareMiniSkim
#        Name             Components owned (copied)   Components borrowed (pointers back)
# 1. "deepCopyMicro"      tag, aod, cnd, usr, (tru)              esd
# 2. "deepCopyMini"       tag, aod, cnd, usr, (tru), esd         None
# 3. "pointer"            tag, cnd, usr                          aod, (tru) esd
# 4. "tagOnly"            tag                                    aod, cnd, (tru), esd
# 5. "noOutput" Just runs the selection code (and increments the appropriate counters, which
#               get printed out at job completion) but does not produce any skim collection.
# The 2nd argument decides for data skims, the 3rd for MC skims.
#
###########################################################################################
#                                              Output type                  Filter on
#               Skim                        Data             MC          Lists     Tag bits  
#
###########################################################################################
declareMiniSkim KanGA/AllEvents            tagOnly         tagOnly

###########################################################################################
# Tracking/calibration skim
declareMiniSkim Tracking/AlignCal          noOutput        noOutput
declareMiniSkim Tracking/PreVeto           deepCopyMicro   deepCopyMicro

###########################################################################################
# Tracking Efficiency skim
declareMiniSkim Tracking/TrkEffTau31       noOutput   noOutput

###########################################################################################
# PID skims
declareMiniSkim PID/BPCBhabha              deepCopyMicro   deepCopyMicro
declareMiniSkim PID/BPCKaon                noOutput   noOutput
#declareMiniSkim PID/BPCKLong               noOutput   noOutput
declareMiniSkim PID/BPCMuon                deepCopyMicro   deepCopyMicro
declareMiniSkim PID/BPCProton              noOutput   noOutput
#declareMiniSkim PID/BPCElectron            noOutput   noOutput
declareMiniSkim PID/BPCPion                noOutput         noOutput
declareMiniSkim PID/DcToKPiPiPromptNoPid   noOutput         noOutput
#declareMiniSkim PID/SigmaCToLambdaCNoPidProton      noOutput   noOutput

###########################################################################################
# Neutral skims
declareMiniSkim Neutral/NeutralD0toKpipi0    noOutput  noOutput
declareMiniSkim Neutral/NeutralDstartoD0pi0  deepCopyMicro  deepCopyMicro

###########################################################################################
# Breco skims
declareMiniSkim Breco/BSemiExcl            deepCopyMicro   deepCopyMicro
declareMiniSkim Breco/BSemiExclAdd         pointer         pointer
declareMiniSkim Breco/BRecoToDDstar        pointer         pointer
declareMiniSkim Breco/BRecoToDDstarPi      pointer         pointer
declareMiniSkim Breco/BDLightPiNoPID       deepCopyMicro   deepCopyMicro
declareMiniSkim Breco/BRecoToDsLight       pointer         pointer
#declareMiniSkim Breco/BRecoToDPi0          noOutput   noOutput
#declareMiniSkim Breco/BRecoToDKPi          noOutput         noOutput
declareMiniSkim Breco/B0DKNeut             deepCopyMicro   deepCopyMicro
declareMiniSkim Breco/BchDKNeut            deepCopyMicro   deepCopyMicro
declareMiniSkim Breco/BToDD                deepCopyMicro   deepCopyMicro
declareMiniSkim Breco/BchToD0KAll          pointer         pointer
declareMiniSkim Breco/BchToD0KstarAll      noOutput         noOutput
#declareMiniSkim Breco/B0ToD0KPi            noOutput   noOutput
declareMiniSkim Breco/BToDDK               deepCopyMicro   deepCopyMicro
#declareMiniSkim Breco/BchToDKCabNonCP      noOutput   noOutput
#declareMiniSkim Breco/BchToDKstCabNonCP    noOutput   noOutput
declareMiniSkim Breco/BToD2sD              deepCopyMicro   deepCopyMicro
#declareMiniSkim Breco/BRecoToDDstarEtaPi   noOutput         noOutput
#declareMiniSkim Breco/BRecoToDsDsstarEtaPi noOutput   noOutput
declareMiniSkim Breco/BToDPPbarX           deepCopyMicro   deepCopyMicro {BToD0PPbarX BToDcPPbarX} BGFMultiHadron
declareMiniSkim Breco/B0ToDstarA1CLoose    deepCopyMicro   deepCopyMicro
#declareMiniSkim Breco/B0DNeutralLight      noOutput   noOutput
declareMiniSkim Breco/B0ToDsJh             deepCopyMicro   deepCopyMicro
declareMiniSkim Breco/BtoDPiPi             noOutput         noOutput
declareMiniSkim Breco/BtoDPiPiPi           noOutput   noOutput
declareMiniSkim Breco/Dsppbar              deepCopyMicro   deepCopyMicro
#declareMiniSkim Breco/RbD0ToKmunu          noOutput   noOutput

###########################################################################################
# sin2beta/mixing skims
# sin2b (Breco) skims
declareMiniSkim S2BMix/BFlav_Final        deepCopyMini    deepCopyMicro
declareMiniSkim S2BMix/BchFlav_Final      noOutput    noOutput
declareMiniSkim S2BMix/B0ToccKFinal       noOutput    noOutput
declareMiniSkim S2BMix/BchToccKFinal      noOutput    noOutput
declareMiniSkim S2BMix/B0ToccKstarFinal   noOutput    noOutput
declareMiniSkim S2BMix/BchToccKstarFinal  noOutput    noOutput
declareMiniSkim S2BMix/B0ToEtacKsFinal    noOutput    noOutput
declareMiniSkim S2BMix/BchToEtacKchFinal  noOutput    noOutput
declareMiniSkim S2BMix/Dilepton           noOutput   noOutput
declareMiniSkim S2BMix/B0ToJpsiPi0Loose   noOutput   noOutput
declareMiniSkim S2BMix/B0ToDDstar0h0      deepCopyMicro   deepCopyMicro

###########################################################################################
# Charmonium AWG skims
declareMiniSkim Charmonium/Jpsitoll            noOutput         noOutput
declareMiniSkim Charmonium/JpsitollTight       noOutput        noOutput
#declareMiniSkim Charmonium/BToJpsiHH           noOutput        noOutput

###########################################################################################
# Charm AWG skims
#
declareMiniSkim Charm/LambdaC                  deepCopyMicro   deepCopyMicro		       
declareMiniSkim Charm/LambdaVeryVeryLoose      noOutput        noOutput
declareMiniSkim Charm/XiMinus                  noOutput        noOutput
declareMiniSkim Charm/OmegaMinus               noOutput        noOutput
declareMiniSkim Charm/DtoXll                   noOutput        noOutput
declareMiniSkim Charm/DmixD0ToKPiPi0           deepCopyMicro   deepCopyMicro
declareMiniSkim Charm/DmixD0ToK3Pi             deepCopyMicro   deepCopyMicro
declareMiniSkim Charm/D0To2ProngDcj            deepCopyMicro   deepCopyMicro
declareMiniSkim Charm/D0To2ProngPi0CS          deepCopyMicro   deepCopyMicro
declareMiniSkim Charm/DstD0ToKsHpHm            deepCopyMicro   deepCopyMicro
declareMiniSkim Charm/DstD0ToKsKPi             deepCopyMicro   deepCopyMicro
declareMiniSkim Charm/DmixD0ToKenu             noOutput        noOutput
declareMiniSkim Charm/DmixD0ToKmunu            noOutput        noOutput
declareMiniSkim Charm/DcToKKKPrompt            deepCopyMicro   deepCopyMicro
declareMiniSkim Charm/DcToKpipiPrompt          deepCopyMicro   deepCopyMicro
declareMiniSkim Charm/DcToKpipiPrompt_DCS      deepCopyMicro   deepCopyMicro  DcToKpipiPrompt_DCS 
declareMiniSkim Charm/DcTo3PiPrompt            deepCopyMicro   deepCopyMicro
declareMiniSkim Charm/DsToKpi0                 noOutput        noOutput
declareMiniSkim Charm/DstToD0PiToVGamma        deepCopyMicro   pointer		       
declareMiniSkim Charm/DstToD0PiToVPi0          deepCopyMicro   pointer
declareMiniSkim Charm/DstToD0PiToVEta          pointer         pointer                      
declareMiniSkim Charm/DsstToDsGammaToVGamma    noOutput        noOutput
declareMiniSkim Charm/DstToDPiToVGamma         noOutput        noOutput
declareMiniSkim Charm/DsToPiKPhiKs             pointer         pointer
declareMiniSkim Charm/DcToKKPiPrompt           deepCopyMicro   pointer
declareMiniSkim Charm/D0ToK0PiPiSkim           noOutput        noOutput
declareMiniSkim Charm/DcTo3PS                  noOutput        noOutput
declareMiniSkim Charm/D0To3PS                  noOutput        noOutput
declareMiniSkim Charm/DsTo3PS                  noOutput        noOutput
declareMiniSkim Charm/DstToD0PiToPi0Pi0        noOutput        noOutput
declareMiniSkim Charm/D0ToPi0Ks                noOutput        noOutput
declareMiniSkim Charm/DRecoDmesonTag           pointer         pointer
declareMiniSkim Charm/DRecoDsmesonTag          pointer         pointer
declareMiniSkim Charm/D0ToKnPiPi0              pointer         pointer

###########################################################################################
# IHBD skims
declareMiniSkim IHBD/InclDstar             deepCopyMicro   deepCopyMicro
declareMiniSkim IHBD/InclSemiLept          noOutput        noOutput
#declareMiniSkim IHBD/InclDstA1c           noOutput        noOutput
declareMiniSkim IHBD/InclDstPi             noOutput        noOutput
#declareMiniSkim IHBD/InclD2stPi           noOutput        noOutput
#declareMiniSkim IHBD/InclDstRho           noOutput        noOutput
#declareMiniSkim IHBD/InclDstRhoComp       noOutput        noOutput

###########################################################################################
# XSL skims
declareMiniSkim XSL/BToDlnu               pointer         pointer
declareMiniSkim XSL/XSLBtoXulnuFilter     noOutput        noOutput
declareMiniSkim XSL/HardElectronFilter    noOutput        noOutput
declareMiniSkim XSL/CharmSL               noOutput        noOutput


###########################################################################################
# ClHBD skims
#
# As of Jan 06 all ClHBD skims stored at RAL must be deepCopy
#
#declareMiniSkim ClHBD/BToKsPi             noOutput        noOutput BToKsPiTag
#declareMiniSkim ClHBD/B0ToPiPi            noOutput        noOutput B0ToPiPiTag
#declareMiniSkim ClHBD/B0ToKsKs            noOutput        noOutput B0ToKsKsTag
#declareMiniSkim ClHBD/BToPi0Pi            noOutput        noOutput BToPi0PiTag
#declareMiniSkim ClHBD/B0ToPi0Pi0          noOutput        noOutput {B0ToPi0Pi0Tag} {RecoEnergyFilter}
#declareMiniSkim ClHBD/B0ToKsPi0           noOutput        noOutput B0ToKsPi0Tag
#declareMiniSkim ClHBD/B0ToPP              noOutput        noOutput B0ToPPTag
#declareMiniSkim ClHBD/B0ToLambdaLambda    noOutput        noOutput B0ToLambdaLambdaTag
#declareMiniSkim ClHBD/BToLambdaP          noOutput        noOutput BToLambdaPTag
declareMiniSkim ClHBD/BTohh                noOutput        noOutput

# Quasi Two Body Stuff
declareMiniSkim ClHBD/InclEta             noOutput        noOutput
declareMiniSkim ClHBD/InclPhi             noOutput        noOutput
#declareMiniSkim ClHBD/A0                 noOutput        noOutput
declareMiniSkim ClHBD/BFourBody            noOutput        noOutput
declareMiniSkim ClHBD/BFourHHHH           noOutput        noOutput
#declareMiniSkim ClHBD/BFourHHHP           noOutput        noOutput
#declareMiniSkim ClHBD/BFourHHHK           noOutput        noOutput
#declareMiniSkim ClHBD/BFourHHHE           noOutput        noOutput
#declareMiniSkim ClHBD/BFourHHPP           noOutput        noOutput
#declareMiniSkim ClHBD/BFourHHPK           noOutput        noOutput
#declareMiniSkim ClHBD/BFourHHKK           noOutput        noOutput
#declareMiniSkim ClHBD/BFourHHKE           noOutput        noOutput
declareMiniSkim ClHBD/BToA1Kst            noOutput        noOutput
#declareMiniSkim ClHBD/B0ToRhoPRhoM        noOutput        noOutput

#declareMiniSkim ClHBD/Bppp0K              noOutput         noOutput
#declareMiniSkim ClHBD/Inclppbar           noOutput         noOutput
#declareMiniSkim ClHBD/InclProtTight       noOutput         noOutput

# classic charmless 3-body
#declareMiniSkim ClHBD/BCCC3body           noOutput        noOutput
#declareMiniSkim ClHBD/BKsKsKs3body        noOutput        noOutput
#declareMiniSkim ClHBD/BCCPi03body         noOutput        noOutput
declareMiniSkim ClHBD/BCCKs3body          noOutput        noOutput
#declareMiniSkim ClHBD/BPi0Pi0Ks3body      noOutput        noOutput
#declareMiniSkim ClHBD/BPi0Pi0C3body       noOutput        noOutput
#declareMiniSkim ClHBD/BCPi0Ks3body        noOutput        noOutput
#declareMiniSkim ClHBD/BCCC03a3body        noOutput        noOutput
#declareMiniSkim ClHBD/BKsKsKs03a3body     noOutput        noOutput
#declareMiniSkim ClHBD/BCCPi003a3body      noOutput        noOutput
#declareMiniSkim ClHBD/BCCKs03a3body       noOutput        noOutput
#declareMiniSkim ClHBD/BPi0Pi0Ks03a3body   noOutput        noOutput
#declareMiniSkim ClHBD/BPi0Pi0C03a3body    noOutput        noOutput
#declareMiniSkim ClHBD/BCPi0Ks03a3body     noOutput        noOutput
#declareMiniSkim ClHBD/BtoKKKL             noOutput         noOutput
#declareMiniSkim ClHBD/KsKsKIncl           noOutput        noOutput
declareMiniSkim ClHBD/BToKsKsKlDefault    noOutput        noOutput
declareMiniSkim ClHBD/BToPhi3K            noOutput        noOutput
declareMiniSkim ClHBD/BToDsD              noOutput        noOutput



# multi-list 4-body combinations
declareMiniSkim ClHBD/BToRPiPih         noOutput        noOutput

# multi-list 3-body combinations
declareMiniSkim ClHBD/BToPPP              noOutput         noOutput
declareMiniSkim ClHBD/BToCXX              noOutput        noOutput
declareMiniSkim ClHBD/BToCPP              noOutput        noOutput
declareMiniSkim ClHBD/BToCCC              noOutput        noOutput
declareMiniSkim ClHBD/BToPXX              noOutput         noOutput
#declareMiniSkim ClHBD/BToKstarXX          noOutput        noOutput
#declareMiniSkim ClHBD/BToLambda0XX        noOutput        noOutput
# multi-list 2-body combinations
declareMiniSkim ClHBD/BToPP               deepCopyMicro   deepCopyMicro

###########################################################################################
# RadPenguin skims
declareMiniSkim RadPenguin/BtoXGammaPi0        noOutput         noOutput
declareMiniSkim RadPenguin/BtoXGamma           noOutput        noOutput
declareMiniSkim RadPenguin/BtoGammaGamma       noOutput        noOutput
#declareMiniSkim RadPenguin/Kll                 noOutput        noOutput
declareMiniSkim RadPenguin/BtoRhoGamma         noOutput        noOutput
#declareMiniSkim RadPenguin/ExclHllMini         noOutput        noOutput
declareMiniSkim RadPenguin/B0toKsPi0Gamma      noOutput        noOutput {B0toKsPi0Gamma}  {BGFMultiHadron BGFNeutralHadron}	       
#declareMiniSkim RadPenguin/B0toKPiGamma        noOutput        noOutput {B0toKPiGamma B0toPiPiGamma} {BGFMultiHadron BGFNeutralHadron}
#declareMiniSkim RadPenguin/BtoKPi0Gamma        noOutput        noOutput {BtoKPi0Gamma BtoPiPi0Gamma} {BGFMultiHadron BGFNeutralHadron}
#declareMiniSkim RadPenguin/BtoKsPiGamma        noOutput        noOutput {BtoKsPiGamma}    {BGFMultiHadron BGFNeutralHadron}	       
#declareMiniSkim RadPenguin/B0toKsEtaGamma      noOutput        noOutput {B0toKsEtaGamma}  {BGFMultiHadron BGFNeutralHadron}	       
#declareMiniSkim RadPenguin/BtoKEtaGamma        noOutput        noOutput {BtoKEtaGamma}    {BGFMultiHadron BGFNeutralHadron}	       
#declareMiniSkim RadPenguin/B0toKsEtaPGamma     noOutput        noOutput {B0toKsEtaPGamma} {BGFMultiHadron BGFNeutralHadron}	       
#declareMiniSkim RadPenguin/BtoKEtaPGamma       noOutput        noOutput {BtoKEtaPGamma}   {BGFMultiHadron BGFNeutralHadron}	       
#declareMiniSkim RadPenguin/B0toKsPhiGamma      noOutput        noOutput {B0toKsPhiGamma}  {BGFMultiHadron BGFNeutralHadron}	       
#declareMiniSkim RadPenguin/BtoKPhiGamma        noOutput        noOutput {BtoKPhiGamma}    {BGFMultiHadron BGFNeutralHadron}
#declareMiniSkim RadPenguin/BtoLambdaPbarGamma  noOutput        noOutput {BtoLambdaPbarGamma} {BGFMultiHadron BGFNeutralHadron}
#declareMiniSkim RadPenguin/BtoSigma0PbarGamma  noOutput        noOutput {BtoSigma0PbarGamma} {BGFMultiHadron BGFNeutralHadron}
declareMiniSkim RadPenguin/BToXsll             deepCopyMicro   deepCopyMicro
declareMiniSkim RadPenguin/BToXsll_peaking     noOutput        noOutput
declareMiniSkim RadPenguin/BToXsllBG           tagOnly         tagOnly
declareMiniSkim RadPenguin/BToXdGamma          noOutput        noOutput

###########################################################################################
# TauQED skims
declareMiniSkim TauQED/Tau11                   noOutput        noOutput
declareMiniSkim TauQED/Tau1N                   noOutput        noOutput  
declareMiniSkim TauQED/Tau33                   noOutput        noOutput  
# new two photon skims			                                      
declareMiniSkim TauQED/TwoPhotonTwoTrack       noOutput        noOutput
declareMiniSkim TauQED/TwoPhotonPi0Pi0         noOutput        noOutput
#declareMiniSkim TauQED/TwoPhotonPentaquark     noOutput         noOutput        

###########################################################################################
# Isr skims
#declareMiniSkim Isr/IsrGamma               noOutput        noOutput
#declareMiniSkim Isr/IsrNoGamma             noOutput         noOutput
declareMiniSkim Isr/IsrLooseGamma          noOutput        noOutput
#declareMiniSkim Isr/IsrRadBhabha           noOutput        noOutput
#declareMiniSkim Isr/RadTwoProng            noOutput        noOutput { } {BGFRadTwoProng}
#declareMiniSkim Isr/IsrDsmeson             noOutput         noOutput
#declareMiniSkim Isr/IsrDmeson              noOutput         noOutput
declareMiniSkim Isr/IsrIncExc              noOutput         noOutput
declareMiniSkim Isr/Jpsi_Psi2sToKsKsGamma  noOutput        noOutput
declareMiniSkim Isr/Jpsi_Psi2sToChChGamma  noOutput        noOutput

###########################################################################################
# B-Counting skims
declareMiniSkim BC/BCountMHFilter        noOutput        noOutput
declareMiniSkim BC/BCountMuMuFilter      pointer        pointer

###########################################################################################
# Skims used for CalibStream in Elf that are not already defined  (ShR 6 Jan 2004)
declareMiniSkim Lumi/LumiTag             noOutput        noOutput
declareMiniSkim TauQED/Prescale          noOutput        noOutput
declareMiniSkim Trigger/Damage           noOutput        noOutput
declareMiniSkim EMC/VCS                  noOutput        noOutput
declareMiniSkim Tracking/L3Cosmic        noOutput        noOutput

# Skim for random background trigger
declareMiniSkim Trigger/RandomBackground noOutput       noOutput

###########################################################################################
# DsToMuNu skim
declareMiniSkim LeptBC/DsToMuNu           pointer         pointer

###########################################################################################
# IHPS skims
#declareMiniSkim IHPS/AntiDeuteron         noOutput    noOutput
#declareMiniSkim IHPS/InclProtPiPiKsK      noOutput         noOutput
#declareMiniSkim IHPS/EEToKKpi             noOutput        noOutput {EEToKKpi Y4SToKsKPi_Ups Y4SToKsKPi_Ups_Tight Y4SToKsKsPi0_Ups Y4SToKsKsPi0_Ups_Tight Y4SToKKPi0_Ups Y4SToKKPi0_Ups_Tight}
#declareMiniSkim IHPS/EEToKKpipi           noOutput        noOutput {EEToKKpipi Y4SToKsKPiPi0_Ups Y4SToKsKPiPi0_Ups_Tight Y4SToKsKsPi0Pi0_Ups Y4SToKsKsPi0Pi0_Ups_Tight Y4SToKsKsPiPi_Ups Y4SToKsKsPiPi_Ups_Tight Y4SToKKPi0Pi0_Ups Y4SToKKPi0Pi0_Ups_Tight Y4SToKKPiPi_Ups Y4SToKKPiPi_Ups_Tight}
#declareMiniSkim IHPS/EEToPSPS             noOutput        noOutput {EEToPSPS Y4SToKsKs_Ups Y4SToKK_Ups Y4SToPiPi_Ups Y4SToPi0Pi0_Ups}
#declareMiniSkim IHPS/EEToKKEta            noOutput        noOutput {EEToKKEtaLoose EEToKKEta}
#declareMiniSkim IHPS/EEToPhiEta           noOutput        noOutput {EEToPhiEta}
