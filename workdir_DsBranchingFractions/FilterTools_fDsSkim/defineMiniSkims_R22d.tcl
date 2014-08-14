# $Id: defineMiniSkims_R22d.tcl,v 1.1 2007/12/06 08:53:20 homer Exp $
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
declareMiniSkim Tracking/TrkEffTau31       deepCopyMicro   deepCopyMicro

###########################################################################################
# PID skims
declareMiniSkim PID/BPCBhabha              deepCopyMicro   deepCopyMicro
declareMiniSkim PID/BPCKaon                deepCopyMicro   deepCopyMicro
declareMiniSkim PID/BPCKLong               deepCopyMicro   deepCopyMicro
declareMiniSkim PID/BPCMuon                deepCopyMicro   deepCopyMicro
declareMiniSkim PID/BPCProton              deepCopyMicro   deepCopyMicro
declareMiniSkim PID/BPCElectron            deepCopyMicro   deepCopyMicro
declareMiniSkim PID/BPCPion                pointer         pointer
declareMiniSkim PID/DcToKPiPiPromptNoPid   pointer         pointer
declareMiniSkim PID/SigmaCToLambdaCNoPidProton      deepCopyMicro   deepCopyMicro

###########################################################################################
# Neutral skims
declareMiniSkim Neutral/NeutralD0toKpipi0    deepCopyMicro  deepCopyMicro
declareMiniSkim Neutral/NeutralDstartoD0pi0  deepCopyMicro  deepCopyMicro

###########################################################################################
# Breco skims (09/05/2003)
declareMiniSkim Breco/BSemiExcl            deepCopyMicro   deepCopyMicro
declareMiniSkim Breco/BRecoToDDstar        pointer         pointer
declareMiniSkim Breco/BRecoToDDstarPi      pointer         pointer
declareMiniSkim Breco/BDLightPiNoPID       deepCopyMicro   deepCopyMicro
declareMiniSkim Breco/BRecoToDsLight       pointer         pointer
declareMiniSkim Breco/BRecoToDPi0          deepCopyMicro   deepCopyMicro
declareMiniSkim Breco/BRecoToDKPi          pointer         pointer
declareMiniSkim Breco/B0DKNeut             deepCopyMicro   deepCopyMicro
declareMiniSkim Breco/BchDKNeut            deepCopyMicro   deepCopyMicro
declareMiniSkim Breco/BToDD                deepCopyMicro   deepCopyMicro
declareMiniSkim Breco/BchToD0KAll          pointer         pointer
declareMiniSkim Breco/BchToD0KstarAll      pointer         pointer
declareMiniSkim Breco/B0ToD0KPi            deepCopyMicro   deepCopyMicro
declareMiniSkim Breco/BToDDK               deepCopyMicro   deepCopyMicro
declareMiniSkim Breco/BchToDKCabNonCP      deepCopyMicro   deepCopyMicro
declareMiniSkim Breco/BchToDKstCabNonCP    deepCopyMicro   deepCopyMicro
declareMiniSkim Breco/BToD2sD              deepCopyMicro   deepCopyMicro
declareMiniSkim Breco/BRecoToDDstarEtaPi   pointer         pointer
declareMiniSkim Breco/BRecoToDsDsstarEtaPi deepCopyMicro   deepCopyMicro
declareMiniSkim Breco/BToDPPbarX           deepCopyMicro   deepCopyMicro {BToD0PPbarX BToDcPPbarX} BGFMultiHadron
declareMiniSkim Breco/B0ToDstarA1CLoose    deepCopyMicro   deepCopyMicro
declareMiniSkim Breco/B0DNeutralLight      deepCopyMicro   pointer
declareMiniSkim Breco/B0ToDsJh             deepCopyMicro   deepCopyMicro
declareMiniSkim Breco/BtoDPiPi             pointer         pointer
declareMiniSkim Breco/Dsppbar              deepCopyMicro   deepCopyMicro
declareMiniSkim Breco/RbD0ToKmunu          deepCopyMicro   deepCopyMicro

###########################################################################################
# sin2beta/mixing skims
# sin2b (Breco) skims
declareMiniSkim S2BMix/BFlav_Final        deepCopyMini    deepCopyMicro
declareMiniSkim S2BMix/BchFlav_Final      deepCopyMini    deepCopyMicro
declareMiniSkim S2BMix/B0ToccKFinal       deepCopyMini    deepCopyMicro
declareMiniSkim S2BMix/BchToccKFinal      deepCopyMini    deepCopyMicro
declareMiniSkim S2BMix/B0ToccKstarFinal   deepCopyMini    deepCopyMicro
declareMiniSkim S2BMix/BchToccKstarFinal  deepCopyMini    deepCopyMicro
declareMiniSkim S2BMix/B0ToEtacKsFinal    deepCopyMini    deepCopyMicro
declareMiniSkim S2BMix/BchToEtacKchFinal  deepCopyMini    deepCopyMicro
declareMiniSkim S2BMix/Dilepton           deepCopyMicro   deepCopyMicro
declareMiniSkim S2BMix/B0ToJpsiPi0Loose   deepCopyMicro   deepCopyMicro
declareMiniSkim S2BMix/B0ToDDstar0h0      deepCopyMicro   deepCopyMicro

###########################################################################################
# Charmonium AWG skims
declareMiniSkim Charmonium/Jpsitoll            pointer         pointer
declareMiniSkim Charmonium/JpsitollTight       deepCopyMicro   deepCopyMicro
declareMiniSkim Charmonium/BToJpsiHH           deepCopyMicro   deepCopyMicro

###########################################################################################
# Charm AWG skims
#
declareMiniSkim Charm/LambdaC                  deepCopyMicro   deepCopyMicro		       
declareMiniSkim Charm/LambdaVeryVeryLoose      deepCopyMicro   deepCopyMicro
declareMiniSkim Charm/XiMinus                  deepCopyMicro   deepCopyMicro
declareMiniSkim Charm/OmegaMinus               deepCopyMicro   deepCopyMicro
declareMiniSkim Charm/DtoXll                   deepCopyMicro   deepCopyMicro
declareMiniSkim Charm/DmixD0ToKPiPi0           deepCopyMicro   deepCopyMicro
declareMiniSkim Charm/DmixD0ToK3Pi             deepCopyMicro   deepCopyMicro
declareMiniSkim Charm/D0To2ProngDcj            deepCopyMicro   deepCopyMicro
declareMiniSkim Charm/D0To2ProngPi0CS          deepCopyMicro   deepCopyMicro
declareMiniSkim Charm/DstD0ToKsHpHm            deepCopyMicro   deepCopyMicro
declareMiniSkim Charm/DmixD0ToKenu             deepCopyMicro   pointer
declareMiniSkim Charm/DmixD0ToKmunu            deepCopyMicro   pointer
declareMiniSkim Charm/DcToKKKPrompt            deepCopyMicro   deepCopyMicro
declareMiniSkim Charm/DcToKpipiPrompt          deepCopyMicro   deepCopyMicro
declareMiniSkim Charm/DcToKpipiPrompt_DCS      deepCopyMicro   deepCopyMicro DcToKpipiPrompt_DCS  
declareMiniSkim Charm/DcTo3PiPrompt            deepCopyMicro   deepCopyMicro
declareMiniSkim Charm/DsToKpi0                 pointer         pointer
declareMiniSkim Charm/DstToD0PiToVGamma        deepCopyMicro   pointer		       
declareMiniSkim Charm/DstToD0PiToVPi0          deepCopyMicro   pointer
declareMiniSkim Charm/DstToD0PiToVEta          pointer         pointer                      
declareMiniSkim Charm/DsstToDsGammaToVGamma    pointer         pointer
declareMiniSkim Charm/DstToDPiToVGamma         deepCopyMicro   deepCopyMicro
declareMiniSkim Charm/DsToPiKPhiKs             pointer         pointer
declareMiniSkim Charm/DcToKKPiPrompt           deepCopyMicro   pointer

###########################################################################################
# IHBD skims
declareMiniSkim IHBD/InclDstar            deepCopyMicro   deepCopyMicro
declareMiniSkim IHBD/InclSemiLept         deepCopyMicro   deepCopyMicro
declareMiniSkim IHBD/InclDstA1c           deepCopyMicro   deepCopyMicro
declareMiniSkim IHBD/InclDstPi            deepCopyMicro   deepCopyMicro
declareMiniSkim IHBD/InclD2stPi           deepCopyMicro   deepCopyMicro
declareMiniSkim IHBD/InclDstRho           deepCopyMicro   deepCopyMicro
declareMiniSkim IHBD/InclDstRhoComp       deepCopyMicro   deepCopyMicro

###########################################################################################
# XSL skims
declareMiniSkim XSL/BToDlnu               pointer         pointer
declareMiniSkim XSL/XSLBtoXulnuFilter     deepCopyMicro   deepCopyMicro
declareMiniSkim XSL/HardElectronFilter    deepCopyMicro   pointer
declareMiniSkim XSL/CharmSL               deepCopyMicro   pointer

###########################################################################################
# ClHBD skims
#
# As of Jan 06 all ClHBD skims stored at RAL must be deepCopy
#
declareMiniSkim ClHBD/BToKsPi             deepCopyMicro   deepCopyMicro BToKsPiTag
declareMiniSkim ClHBD/B0ToPiPi            deepCopyMicro   deepCopyMicro B0ToPiPiTag
declareMiniSkim ClHBD/B0ToKsKs            deepCopyMicro   deepCopyMicro B0ToKsKsTag
declareMiniSkim ClHBD/BToPi0Pi            deepCopyMicro   deepCopyMicro BToPi0PiTag
declareMiniSkim ClHBD/B0ToPi0Pi0          deepCopyMicro   deepCopyMicro B0ToPi0Pi0Tag
declareMiniSkim ClHBD/B0ToKsPi0           deepCopyMicro   deepCopyMicro B0ToKsPi0Tag
declareMiniSkim ClHBD/B0ToPP              deepCopyMicro   deepCopyMicro B0ToPPTag
declareMiniSkim ClHBD/B0ToLambdaLambda    deepCopyMicro   deepCopyMicro B0ToLambdaLambdaTag
declareMiniSkim ClHBD/BToLambdaP          deepCopyMicro   deepCopyMicro BToLambdaPTag

# Quasi Two Body Stuff
declareMiniSkim ClHBD/InclEta             deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/InclPhi             deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/A0                  deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BFourBody           deepCopyMicro   pointer
declareMiniSkim ClHBD/BFourHHHH           deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BFourHHHP           deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BFourHHHK           deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BFourHHHE           deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BFourHHPP           deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BFourHHPK           deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BFourHHKK           deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BFourHHKE           deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BToA1Kst            deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/B0ToRhoPRhoM        deepCopyMicro   deepCopyMicro

declareMiniSkim ClHBD/Bppp0K              pointer         pointer
declareMiniSkim ClHBD/Inclppbar           pointer         pointer
declareMiniSkim ClHBD/InclProtTight       pointer         pointer

# classic charmless 3-body
declareMiniSkim ClHBD/BCCC3body           deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BKsKsKs3body        deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BCCPi03body         deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BCCKs3body          deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BPi0Pi0Ks3body      deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BPi0Pi0C3body       deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BCPi0Ks3body        deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BCCC03a3body        deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BKsKsKs03a3body     deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BCCPi003a3body      deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BCCKs03a3body       deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BPi0Pi0Ks03a3body   deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BPi0Pi0C03a3body    deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BCPi0Ks03a3body     deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BtoKKKL             pointer         pointer
declareMiniSkim ClHBD/KsKsKIncl           deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BToKsKsKlDefault    deepCopyMicro   deepCopyMicro

# multi-list 3-body combinations
declareMiniSkim ClHBD/BToPPP              pointer         pointer
declareMiniSkim ClHBD/BToCXX              deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BToCPP              deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BToCCC              deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BToPXX              pointer         pointer
declareMiniSkim ClHBD/BToKstarXX          deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BToLambda0XX        deepCopyMicro   deepCopyMicro
# multi-list 2-body combinations
declareMiniSkim ClHBD/BToPP               deepCopyMicro   deepCopyMicro

###########################################################################################
# RadPenguin skims
declareMiniSkim RadPenguin/BtoXGammaPi0        pointer         pointer
declareMiniSkim RadPenguin/BtoXGamma           deepCopyMicro   deepCopyMicro
declareMiniSkim RadPenguin/BtoGammaGamma       deepCopyMicro   deepCopyMicro
declareMiniSkim RadPenguin/Kll                 deepCopyMicro   pointer
declareMiniSkim RadPenguin/BtoRhoGamma         deepCopyMicro   deepCopyMicro
declareMiniSkim RadPenguin/ExclHllMini         deepCopyMicro   deepCopyMicro
declareMiniSkim RadPenguin/B0toKsPi0Gamma      deepCopyMicro   deepCopyMicro {B0toKsPi0Gamma}  {BGFMultiHadron BGFNeutralHadron}	       
declareMiniSkim RadPenguin/B0toKPiGamma        deepCopyMicro   deepCopyMicro {B0toKPiGamma B0toPiPiGamma} {BGFMultiHadron BGFNeutralHadron}
declareMiniSkim RadPenguin/BtoKPi0Gamma        deepCopyMicro   deepCopyMicro {BtoKPi0Gamma BtoPiPi0Gamma} {BGFMultiHadron BGFNeutralHadron}
declareMiniSkim RadPenguin/BtoKsPiGamma        deepCopyMicro   deepCopyMicro {BtoKsPiGamma}    {BGFMultiHadron BGFNeutralHadron}	       
declareMiniSkim RadPenguin/B0toKsEtaGamma      deepCopyMicro   deepCopyMicro {B0toKsEtaGamma}  {BGFMultiHadron BGFNeutralHadron}	       
declareMiniSkim RadPenguin/BtoKEtaGamma        deepCopyMicro   deepCopyMicro {BtoKEtaGamma}    {BGFMultiHadron BGFNeutralHadron}	       
declareMiniSkim RadPenguin/B0toKsEtaPGamma     deepCopyMicro   deepCopyMicro {B0toKsEtaPGamma} {BGFMultiHadron BGFNeutralHadron}	       
declareMiniSkim RadPenguin/BtoKEtaPGamma       deepCopyMicro   deepCopyMicro {BtoKEtaPGamma}   {BGFMultiHadron BGFNeutralHadron}	       
declareMiniSkim RadPenguin/B0toKsPhiGamma      deepCopyMicro   deepCopyMicro {B0toKsPhiGamma}  {BGFMultiHadron BGFNeutralHadron}	       
declareMiniSkim RadPenguin/BtoKPhiGamma        deepCopyMicro   deepCopyMicro {BtoKPhiGamma}    {BGFMultiHadron BGFNeutralHadron}
declareMiniSkim RadPenguin/BtoLambdaPbarGamma  deepCopyMicro   deepCopyMicro {BtoLambdaPbarGamma} {BGFMultiHadron BGFNeutralHadron}
declareMiniSkim RadPenguin/BtoSigma0PbarGamma  deepCopyMicro   deepCopyMicro {BtoSigma0PbarGamma} {BGFMultiHadron BGFNeutralHadron}
declareMiniSkim RadPenguin/BToXsll             deepCopyMicro   deepCopyMicro
declareMiniSkim RadPenguin/BToXsll_peaking     deepCopyMicro   deepCopyMicro

###########################################################################################
# TauQED skims
declareMiniSkim TauQED/Tau11                   deepCopyMicro   deepCopyMicro
declareMiniSkim TauQED/Tau1N                   deepCopyMicro   deepCopyMicro  
declareMiniSkim TauQED/Tau33                   deepCopyMicro   deepCopyMicro  
# new two photon skims			                                      
declareMiniSkim TauQED/TwoPhotonTwoTrack       deepCopyMicro   deepCopyMicro
declareMiniSkim TauQED/TwoPhotonPi0Pi0         deepCopyMicro   deepCopyMicro
declareMiniSkim TauQED/TwoPhotonPentaquark     pointer         pointer        

###########################################################################################
# Isr skims
declareMiniSkim Isr/IsrGamma               deepCopyMicro   deepCopyMicro
declareMiniSkim Isr/IsrNoGamma             pointer         pointer
declareMiniSkim Isr/IsrLooseGamma          deepCopyMicro   deepCopyMicro
declareMiniSkim Isr/IsrRadBhabha           deepCopyMicro   deepCopyMicro
declareMiniSkim Isr/RadTwoProng            deepCopyMicro   deepCopyMicro { } {BGFRadTwoProng}
declareMiniSkim Isr/IsrDsmeson             pointer         pointer
declareMiniSkim Isr/IsrDmeson              pointer         pointer
declareMiniSkim Isr/IsrIncExc              pointer         deepCopyMicro

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
declareMiniSkim IHPS/AntiDeuteron         deepCopyMini    deepCopyMicro
declareMiniSkim IHPS/InclProtPiPiKsK      pointer         pointer
declareMiniSkim IHPS/EEToKKpi             deepCopyMicro   deepCopyMicro {EEToKKpi Y4SToKsKPi_Ups Y4SToKsKPi_Ups_Tight Y4SToKsKsPi0_Ups Y4SToKsKsPi0_Ups_Tight Y4SToKKPi0_Ups Y4SToKKPi0_Ups_Tight}
declareMiniSkim IHPS/EEToKKpipi           deepCopyMicro   deepCopyMicro {EEToKKpipi Y4SToKsKPiPi0_Ups Y4SToKsKPiPi0_Ups_Tight Y4SToKsKsPi0Pi0_Ups Y4SToKsKsPi0Pi0_Ups_Tight Y4SToKsKsPiPi_Ups Y4SToKsKsPiPi_Ups_Tight Y4SToKKPi0Pi0_Ups Y4SToKKPi0Pi0_Ups_Tight Y4SToKKPiPi_Ups Y4SToKKPiPi_Ups_Tight}
declareMiniSkim IHPS/EEToPSPS             deepCopyMicro   deepCopyMicro {EEToPSPS Y4SToKsKs_Ups Y4SToKK_Ups Y4SToPiPi_Ups Y4SToPi0Pi0_Ups}
declareMiniSkim IHPS/EEToKKEta            deepCopyMicro   deepCopyMicro {EEToKKEtaLoose EEToKKEta}
declareMiniSkim IHPS/EEToPhiEta           deepCopyMicro   deepCopyMicro {EEToPhiEta}
