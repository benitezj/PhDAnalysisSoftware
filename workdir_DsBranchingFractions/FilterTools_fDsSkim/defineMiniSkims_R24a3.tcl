# $Id: defineMiniSkims_R24a3.tcl,v 1.4 2008/02/11 18:11:20 marchior Exp $
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
declareMiniSkim Tracking/AlignCal          noOutput   noOutput
declareMiniSkim Tracking/PreVeto           noOutput   noOutput

###########################################################################################
# Tracking Efficiency skim
declareMiniSkim Tracking/TrkEffTau31       noOutput   noOutput

###########################################################################################
# PID skims
declareMiniSkim PID/BPCBhabha              noOutput   noOutput
declareMiniSkim PID/BPCKaon                noOutput   noOutput
#declareMiniSkim PID/BPCKLong               noOutput   noOutput
declareMiniSkim PID/BPCMuon                noOutput   noOutput
declareMiniSkim PID/BPCProton              noOutput   noOutput
#declareMiniSkim PID/BPCElectron            noOutput   noOutput
declareMiniSkim PID/BPCPion                noOutput   noOutput
declareMiniSkim PID/DcToKPiPiPromptNoPid   noOutput   noOutput
#declareMiniSkim PID/SigmaCToLambdaCNoPidProton      noOutput   noOutput

###########################################################################################
# Neutral skims
declareMiniSkim Neutral/NeutralD0toKpipi0    noOutput  noOutput
declareMiniSkim Neutral/NeutralDstartoD0pi0  noOutput  noOutput

###########################################################################################
# Breco skims (09/05/2003)
declareMiniSkim Breco/BSemiExcl            noOutput   noOutput
declareMiniSkim Breco/BSemiExclAdd         noOutput   noOutput
declareMiniSkim Breco/BRecoToDDstar        noOutput   noOutput
declareMiniSkim Breco/BRecoToDDstarPi      noOutput   noOutput
declareMiniSkim Breco/BDLightPiNoPID       noOutput   noOutput
declareMiniSkim Breco/BRecoToDsLight       noOutput   noOutput
#declareMiniSkim Breco/BRecoToDPi0          noOutput   noOutput
#declareMiniSkim Breco/BRecoToDKPi          noOutput   noOutput
declareMiniSkim Breco/B0DKNeut             noOutput   noOutput
#declareMiniSkim Breco/BchDKNeut            noOutput   noOutput
declareMiniSkim Breco/BToDD                noOutput   noOutput
declareMiniSkim Breco/BchToD0KAll          noOutput   noOutput
declareMiniSkim Breco/BchToD0KstarAll      noOutput   noOutput
#declareMiniSkim Breco/B0ToD0KPi            noOutput   noOutput
declareMiniSkim Breco/BToDDK               noOutput   noOutput
#declareMiniSkim Breco/BchToDKCabNonCP      noOutput   noOutput
#declareMiniSkim Breco/BchToDKstCabNonCP    noOutput   noOutput
declareMiniSkim Breco/BToD2sD              noOutput   noOutput
#declareMiniSkim Breco/BRecoToDDstarEtaPi   noOutput   noOutput
#declareMiniSkim Breco/BRecoToDsDsstarEtaPi noOutput   noOutput
declareMiniSkim Breco/BToDPPbarX           noOutput   noOutput {BToD0PPbarX BToDcPPbarX} BGFMultiHadron
declareMiniSkim Breco/B0ToDstarA1CLoose    noOutput   noOutput
#declareMiniSkim Breco/B0DNeutralLight      noOutput   noOutput
declareMiniSkim Breco/B0ToDsJh             noOutput   noOutput
declareMiniSkim Breco/BtoDPiPi             noOutput   noOutput
declareMiniSkim Breco/BtoDPiPiPi           noOutput   noOutput
declareMiniSkim Breco/Dsppbar              noOutput   noOutput
#declareMiniSkim Breco/RbD0ToKmunu          noOutput   noOutput

###########################################################################################
# sin2beta/mixing skims
# sin2b (Breco) skims
declareMiniSkim S2BMix/BFlav_Final        noOutput    noOutput
declareMiniSkim S2BMix/BchFlav_Final      noOutput    noOutput
declareMiniSkim S2BMix/B0ToccKFinal       noOutput    noOutput
declareMiniSkim S2BMix/BchToccKFinal      noOutput    noOutput
declareMiniSkim S2BMix/B0ToccKstarFinal   noOutput    noOutput
declareMiniSkim S2BMix/BchToccKstarFinal  noOutput    noOutput
declareMiniSkim S2BMix/B0ToEtacKsFinal    noOutput    noOutput
declareMiniSkim S2BMix/BchToEtacKchFinal  noOutput    noOutput
declareMiniSkim S2BMix/Dilepton           noOutput    noOutput
declareMiniSkim S2BMix/B0ToJpsiPi0Loose   noOutput    noOutput
declareMiniSkim S2BMix/B0ToDDstar0h0      noOutput    noOutput

###########################################################################################
# Charmonium AWG skims
declareMiniSkim Charmonium/Jpsitoll            noOutput   noOutput
declareMiniSkim Charmonium/JpsitollTight       noOutput   noOutput
#declareMiniSkim Charmonium/BToJpsiHH           noOutput   noOutput

###########################################################################################
# Charm AWG skims
#
declareMiniSkim Charm/LambdaC                  noOutput   noOutput		       
declareMiniSkim Charm/LambdaVeryVeryLoose      noOutput   noOutput
declareMiniSkim Charm/XiMinus                  noOutput   noOutput
declareMiniSkim Charm/OmegaMinus               noOutput   noOutput
declareMiniSkim Charm/DtoXll                   noOutput   noOutput
declareMiniSkim Charm/DmixD0ToKPiPi0           noOutput   noOutput
declareMiniSkim Charm/DmixD0ToK3Pi             noOutput   noOutput
declareMiniSkim Charm/D0To2ProngDcj            noOutput   noOutput
declareMiniSkim Charm/D0To2ProngPi0CS          noOutput   noOutput
declareMiniSkim Charm/DstD0ToKsHpHm            noOutput   noOutput
declareMiniSkim Charm/DmixD0ToKenu             noOutput   noOutput
declareMiniSkim Charm/DmixD0ToKmunu            noOutput   noOutput
declareMiniSkim Charm/DcToKKKPrompt            noOutput   noOutput
declareMiniSkim Charm/DcToKpipiPrompt          noOutput   noOutput
#declareMiniSkim Charm/DcToKpipiPrompt_DCS      noOutput   noOutput DcToKpipiPrompt_DCS  
declareMiniSkim Charm/DcTo3PiPrompt            noOutput   noOutput
declareMiniSkim Charm/DsToKpi0                 noOutput   noOutput
declareMiniSkim Charm/DstToD0PiToVGamma        noOutput   noOutput		       
declareMiniSkim Charm/DstToD0PiToVPi0          noOutput   noOutput
declareMiniSkim Charm/DstToD0PiToVEta          noOutput   noOutput                      
declareMiniSkim Charm/DsstToDsGammaToVGamma    noOutput   noOutput
declareMiniSkim Charm/DstToDPiToVGamma         noOutput   noOutput
declareMiniSkim Charm/DsToPiKPhiKs             noOutput   noOutput
declareMiniSkim Charm/DcToKKPiPrompt           noOutput   noOutput
declareMiniSkim Charm/D0ToK0PiPiSkim           noOutput   noOutput
declareMiniSkim Charm/DcTo3PS                  noOutput   noOutput
declareMiniSkim Charm/D0To3PS                  noOutput   noOutput
declareMiniSkim Charm/DsTo3PS                  noOutput   noOutput
declareMiniSkim Charm/DstToD0PiToPi0Pi0        noOutput   noOutput
declareMiniSkim Charm/D0ToPi0Ks                noOutput   noOutput


###########################################################################################
# IHBD skims
declareMiniSkim IHBD/InclDstar            noOutput   noOutput
declareMiniSkim IHBD/InclSemiLept         noOutput   noOutput
#declareMiniSkim IHBD/InclDstA1c           noOutput   noOutput
declareMiniSkim IHBD/InclDstPi            noOutput   noOutput
#declareMiniSkim IHBD/InclD2stPi           noOutput   noOutput
#declareMiniSkim IHBD/InclDstRho           noOutput   noOutput
#declareMiniSkim IHBD/InclDstRhoComp       noOutput   noOutput

###########################################################################################
# XSL skims
declareMiniSkim XSL/BToDlnu               noOutput   noOutput
declareMiniSkim XSL/XSLBtoXulnuFilter     noOutput   noOutput
declareMiniSkim XSL/HardElectronFilter    noOutput   noOutput
declareMiniSkim XSL/CharmSL               noOutput   noOutput

###########################################################################################
# ClHBD skims
#
# As of Jan 06 all ClHBD skims stored at RAL must be deepCopy
#
#declareMiniSkim ClHBD/BToKsPi             noOutput   noOutput BToKsPiTag
#declareMiniSkim ClHBD/B0ToPiPi            noOutput   noOutput B0ToPiPiTag
#declareMiniSkim ClHBD/B0ToKsKs            noOutput   noOutput B0ToKsKsTag
#declareMiniSkim ClHBD/BToPi0Pi            noOutput   noOutput BToPi0PiTag
#declareMiniSkim ClHBD/B0ToPi0Pi0          noOutput   noOutput {B0ToPi0Pi0Tag} {RecoEnergyFilter}
#declareMiniSkim ClHBD/B0ToKsPi0           noOutput   noOutput B0ToKsPi0Tag
#declareMiniSkim ClHBD/B0ToPP              noOutput   noOutput B0ToPPTag
#declareMiniSkim ClHBD/B0ToLambdaLambda    noOutput   noOutput B0ToLambdaLambdaTag
#declareMiniSkim ClHBD/BToLambdaP          noOutput   noOutput BToLambdaPTag
declareMiniSkim ClHBD/BTohh               deepCopyMicro   deepCopyMicro

# Quasi Two Body Stuff
declareMiniSkim ClHBD/InclEta             deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/InclPhi             deepCopyMicro   deepCopyMicro
#declareMiniSkim ClHBD/A0                  deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BFourBody           deepCopyMicro   pointer
declareMiniSkim ClHBD/BFourHHHH           deepCopyMicro   deepCopyMicro
#declareMiniSkim ClHBD/BFourHHHP           deepCopyMicro   deepCopyMicro
#declareMiniSkim ClHBD/BFourHHHK           deepCopyMicro   deepCopyMicro
#declareMiniSkim ClHBD/BFourHHHE           deepCopyMicro   deepCopyMicro
#declareMiniSkim ClHBD/BFourHHPP           deepCopyMicro   deepCopyMicro
#declareMiniSkim ClHBD/BFourHHPK           deepCopyMicro   deepCopyMicro
#declareMiniSkim ClHBD/BFourHHKK           deepCopyMicro   deepCopyMicro
#declareMiniSkim ClHBD/BFourHHKE           deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BToA1Kst            deepCopyMicro   deepCopyMicro
#declareMiniSkim ClHBD/B0ToRhoPRhoM        deepCopyMicro   deepCopyMicro

#declareMiniSkim ClHBD/Bppp0K              pointer         pointer
#declareMiniSkim ClHBD/Inclppbar           pointer         pointer
#declareMiniSkim ClHBD/InclProtTight       pointer         pointer

# classic charmless 3-body
#declareMiniSkim ClHBD/BCCC3body           deepCopyMicro   deepCopyMicro
#declareMiniSkim ClHBD/BKsKsKs3body        deepCopyMicro   deepCopyMicro
#declareMiniSkim ClHBD/BCCPi03body         deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BCCKs3body          deepCopyMicro   deepCopyMicro
#declareMiniSkim ClHBD/BPi0Pi0Ks3body      deepCopyMicro   deepCopyMicro
#declareMiniSkim ClHBD/BPi0Pi0C3body       deepCopyMicro   deepCopyMicro
#declareMiniSkim ClHBD/BCPi0Ks3body        deepCopyMicro   deepCopyMicro
#declareMiniSkim ClHBD/BCCC03a3body        deepCopyMicro   deepCopyMicro
#declareMiniSkim ClHBD/BKsKsKs03a3body     deepCopyMicro   deepCopyMicro
#declareMiniSkim ClHBD/BCCPi003a3body      deepCopyMicro   deepCopyMicro
#declareMiniSkim ClHBD/BCCKs03a3body       deepCopyMicro   deepCopyMicro
#declareMiniSkim ClHBD/BPi0Pi0Ks03a3body   deepCopyMicro   deepCopyMicro
#declareMiniSkim ClHBD/BPi0Pi0C03a3body    deepCopyMicro   deepCopyMicro
#declareMiniSkim ClHBD/BCPi0Ks03a3body     deepCopyMicro   deepCopyMicro
#declareMiniSkim ClHBD/BtoKKKL             pointer         pointer
#declareMiniSkim ClHBD/KsKsKIncl           deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BToKsKsKlDefault    deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BToPhi3K            deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BToDsD              deepCopyMicro   deepCopyMicro



# multi-list 4-body combinations
declareMiniSkim ClHBD/BToRPiPih         deepCopyMicro   deepCopyMicro

# multi-list 3-body combinations
declareMiniSkim ClHBD/BToPPP              pointer         pointer
declareMiniSkim ClHBD/BToCXX              deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BToCPP              deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BToCCC              deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BToPXX              pointer         pointer
#declareMiniSkim ClHBD/BToKstarXX          deepCopyMicro   deepCopyMicro
#declareMiniSkim ClHBD/BToLambda0XX        deepCopyMicro   deepCopyMicro
# multi-list 2-body combinations
declareMiniSkim ClHBD/BToPP               deepCopyMicro   deepCopyMicro

###########################################################################################
# RadPenguin skims
declareMiniSkim RadPenguin/BtoXGammaPi0        pointer         pointer
declareMiniSkim RadPenguin/BtoXGamma           deepCopyMicro   deepCopyMicro
declareMiniSkim RadPenguin/BtoGammaGamma       deepCopyMicro   deepCopyMicro
#declareMiniSkim RadPenguin/Kll                 deepCopyMicro   pointer
declareMiniSkim RadPenguin/BtoRhoGamma         deepCopyMicro   deepCopyMicro
#declareMiniSkim RadPenguin/ExclHllMini         deepCopyMicro   deepCopyMicro
declareMiniSkim RadPenguin/B0toKsPi0Gamma      deepCopyMicro   deepCopyMicro {B0toKsPi0Gamma}  {BGFMultiHadron BGFNeutralHadron}	       
#declareMiniSkim RadPenguin/B0toKPiGamma        deepCopyMicro   deepCopyMicro {B0toKPiGamma B0toPiPiGamma} {BGFMultiHadron BGFNeutralHadron}
#declareMiniSkim RadPenguin/BtoKPi0Gamma        deepCopyMicro   deepCopyMicro {BtoKPi0Gamma BtoPiPi0Gamma} {BGFMultiHadron BGFNeutralHadron}
#declareMiniSkim RadPenguin/BtoKsPiGamma        deepCopyMicro   deepCopyMicro {BtoKsPiGamma}    {BGFMultiHadron BGFNeutralHadron}	       
#declareMiniSkim RadPenguin/B0toKsEtaGamma      deepCopyMicro   deepCopyMicro {B0toKsEtaGamma}  {BGFMultiHadron BGFNeutralHadron}	       
#declareMiniSkim RadPenguin/BtoKEtaGamma        deepCopyMicro   deepCopyMicro {BtoKEtaGamma}    {BGFMultiHadron BGFNeutralHadron}	       
#declareMiniSkim RadPenguin/B0toKsEtaPGamma     deepCopyMicro   deepCopyMicro {B0toKsEtaPGamma} {BGFMultiHadron BGFNeutralHadron}	       
#declareMiniSkim RadPenguin/BtoKEtaPGamma       deepCopyMicro   deepCopyMicro {BtoKEtaPGamma}   {BGFMultiHadron BGFNeutralHadron}	       
#declareMiniSkim RadPenguin/B0toKsPhiGamma      deepCopyMicro   deepCopyMicro {B0toKsPhiGamma}  {BGFMultiHadron BGFNeutralHadron}	       
#declareMiniSkim RadPenguin/BtoKPhiGamma        deepCopyMicro   deepCopyMicro {BtoKPhiGamma}    {BGFMultiHadron BGFNeutralHadron}
#declareMiniSkim RadPenguin/BtoLambdaPbarGamma  deepCopyMicro   deepCopyMicro {BtoLambdaPbarGamma} {BGFMultiHadron BGFNeutralHadron}
#declareMiniSkim RadPenguin/BtoSigma0PbarGamma  deepCopyMicro   deepCopyMicro {BtoSigma0PbarGamma} {BGFMultiHadron BGFNeutralHadron}
declareMiniSkim RadPenguin/BToXsll             deepCopyMicro   deepCopyMicro
declareMiniSkim RadPenguin/BToXsll_peaking     deepCopyMicro   deepCopyMicro
declareMiniSkim RadPenguin/BToXsllBG           tagOnly         tagOnly
declareMiniSkim RadPenguin/BToXdGamma          deepCopyMicro   deepCopyMicro

###########################################################################################
# TauQED skims
declareMiniSkim TauQED/Tau11                   deepCopyMicro   deepCopyMicro
declareMiniSkim TauQED/Tau1N                   deepCopyMicro   deepCopyMicro  
declareMiniSkim TauQED/Tau33                   deepCopyMicro   deepCopyMicro  
# new two photon skims			                                      
declareMiniSkim TauQED/TwoPhotonTwoTrack       deepCopyMicro   deepCopyMicro
declareMiniSkim TauQED/TwoPhotonPi0Pi0         deepCopyMicro   deepCopyMicro
#declareMiniSkim TauQED/TwoPhotonPentaquark     pointer         pointer        

###########################################################################################
# Isr skims
#declareMiniSkim Isr/IsrGamma               deepCopyMicro   deepCopyMicro
#declareMiniSkim Isr/IsrNoGamma             pointer         pointer
declareMiniSkim Isr/IsrLooseGamma          deepCopyMicro   deepCopyMicro
#declareMiniSkim Isr/IsrRadBhabha           deepCopyMicro   deepCopyMicro
#declareMiniSkim Isr/RadTwoProng            deepCopyMicro   deepCopyMicro { } {BGFRadTwoProng}
#declareMiniSkim Isr/IsrDsmeson             pointer         pointer
#declareMiniSkim Isr/IsrDmeson              pointer         pointer
declareMiniSkim Isr/IsrIncExc              pointer         deepCopyMicro
declareMiniSkim Isr/Jpsi_Psi2sToKsKsGamma  deepCopyMicro   deepCopyMicro
declareMiniSkim Isr/Jpsi_Psi2sToChChGamma  deepCopyMicro   deepCopyMicro

###########################################################################################
# B-Counting skims
declareMiniSkim BC/BCountMHFilter        pointer        pointer
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
#declareMiniSkim IHPS/AntiDeuteron         deepCopyMini    deepCopyMicro
#declareMiniSkim IHPS/InclProtPiPiKsK      pointer         pointer
#declareMiniSkim IHPS/EEToKKpi             deepCopyMicro   deepCopyMicro {EEToKKpi Y4SToKsKPi_Ups Y4SToKsKPi_Ups_Tight Y4SToKsKsPi0_Ups Y4SToKsKsPi0_Ups_Tight Y4SToKKPi0_Ups Y4SToKKPi0_Ups_Tight}
#declareMiniSkim IHPS/EEToKKpipi           deepCopyMicro   deepCopyMicro {EEToKKpipi Y4SToKsKPiPi0_Ups Y4SToKsKPiPi0_Ups_Tight Y4SToKsKsPi0Pi0_Ups Y4SToKsKsPi0Pi0_Ups_Tight Y4SToKsKsPiPi_Ups Y4SToKsKsPiPi_Ups_Tight Y4SToKKPi0Pi0_Ups Y4SToKKPi0Pi0_Ups_Tight Y4SToKKPiPi_Ups Y4SToKKPiPi_Ups_Tight}
#declareMiniSkim IHPS/EEToPSPS             deepCopyMicro   deepCopyMicro {EEToPSPS Y4SToKsKs_Ups Y4SToKK_Ups Y4SToPiPi_Ups Y4SToPi0Pi0_Ups}
#declareMiniSkim IHPS/EEToKKEta            deepCopyMicro   deepCopyMicro {EEToKKEtaLoose EEToKKEta}
#declareMiniSkim IHPS/EEToPhiEta           deepCopyMicro   deepCopyMicro {EEToPhiEta}
