# $Id: defineMiniSkims_R18bcd.tcl,v 1.1 2007/01/11 21:22:29 homer Exp $
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
declareMiniSkim PID/BPCMuon                pointer         pointer
declareMiniSkim PID/BPCProton              deepCopyMicro   deepCopyMicro
declareMiniSkim PID/BPCElectron            deepCopyMicro   deepCopyMicro
declareMiniSkim PID/BPCPion                deepCopyMicro   pointer

###########################################################################################
# Neutral skims
declareMiniSkim Neutral/NeutralD0toKpipi0    pointer         pointer
declareMiniSkim Neutral/NeutralDstartoD0pi0  pointer         pointer

###########################################################################################
# Breco skims (09/05/2003)
declareMiniSkim Breco/BSemiExcl            deepCopyMicro   deepCopyMicro
declareMiniSkim Breco/BRecoToDDstar        pointer         pointer
declareMiniSkim Breco/BRecoToDDstarPi      deepCopyMicro   pointer
declareMiniSkim Breco/BDLightPiNoPID       deepCopyMicro   deepCopyMicro
declareMiniSkim Breco/BRecoToDsLight       deepCopyMicro   pointer
declareMiniSkim Breco/BRecoToDPi0          deepCopyMicro   deepCopyMicro
declareMiniSkim Breco/BRecoToDKPi          deepCopyMicro   pointer
declareMiniSkim Breco/B0DKNeut             deepCopyMicro   deepCopyMicro
declareMiniSkim Breco/BchDKNeut            deepCopyMicro   deepCopyMicro
declareMiniSkim Breco/BToDD                deepCopyMicro   deepCopyMicro
declareMiniSkim Breco/B0ToDsDs             deepCopyMicro   deepCopyMicro
declareMiniSkim Breco/BchToD0KAll          deepCopyMicro   pointer
declareMiniSkim Breco/BchToD0KstarAll      deepCopyMicro   pointer
declareMiniSkim Breco/B0ToD0KPi            deepCopyMicro   deepCopyMicro
declareMiniSkim Breco/BToDDK               deepCopyMicro   deepCopyMicro
declareMiniSkim Breco/BchToDKCabNonCP      deepCopyMicro   deepCopyMicro
declareMiniSkim Breco/BchToDKstCabNonCP    deepCopyMicro   deepCopyMicro
declareMiniSkim Breco/BToD2sD              deepCopyMicro   deepCopyMicro
declareMiniSkim Breco/BRecoToDDstarEtaPi   deepCopyMicro   pointer
declareMiniSkim Breco/BRecoToDsDsstarEtaPi deepCopyMicro   deepCopyMicro
declareMiniSkim Breco/BToDPPbarX           deepCopyMicro   pointer {BToD0PPbarX BToDcPPbarX} BGFMultiHadron
declareMiniSkim Breco/B0ToDstarA1CLoose    deepCopyMicro   deepCopyMicro
declareMiniSkim Breco/B0DNeutralLight      deepCopyMicro   deepCopyMicro
declareMiniSkim Breco/B0ToDsJh             deepCopyMicro   deepCopyMicro
declareMiniSkim Breco/BtoDPiPi             deepCopyMicro   pointer
declareMiniSkim Breco/Dsppbar              deepCopyMicro   deepCopyMicro
declareMiniSkim Breco/RbD0ToKenu           deepCopyMicro   deepCopyMicro
declareMiniSkim Breco/RbD0ToKmunu          deepCopyMicro   deepCopyMicro
declareMiniSkim Breco/B0ToDstarhKlnu       deepCopyMicro   deepCopyMicro
declareMiniSkim Breco/NuuReco              customOutput    customOutput
declareMiniSkim Breco/BtoDsDsK3prong       deepCopyMicro   deepCopyMicro 
declareMiniSkim Breco/BtoDsDsK6modes       deepCopyMicro   pointer

###########################################################################################
# sin2beta/mixing skims
# sin2b (Breco) skims
declareMiniSkim S2BMix/BFlav_Loose        deepCopyMicro   deepCopyMicro
declareMiniSkim S2BMix/BFlav_Final        deepCopyMini    deepCopyMicro
declareMiniSkim S2BMix/BchFlav_Loose      deepCopyMicro   deepCopyMicro
declareMiniSkim S2BMix/BchFlav_Final      deepCopyMini    deepCopyMicro
declareMiniSkim S2BMix/B0ToccKLoose       deepCopyMicro   deepCopyMicro
declareMiniSkim S2BMix/B0ToccKS2b         deepCopyMini    deepCopyMini
declareMiniSkim S2BMix/BchToccKLoose      deepCopyMicro   deepCopyMicro
declareMiniSkim S2BMix/B0ToccKstarLoose   deepCopyMicro   deepCopyMicro
declareMiniSkim S2BMix/BchToccKstarLoose  deepCopyMicro   deepCopyMicro
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
declareMiniSkim Charmonium/JpsiISR             deepCopyMicro   deepCopyMicro
declareMiniSkim Charmonium/BToJpsiHH           pointer         pointer
declareMiniSkim Charmonium/ExclMultiTracksHad  pointer         pointer

###########################################################################################
# Charm AWG skims
#
declareMiniSkim Charm/LambdaC                  deepCopyMicro   deepCopyMicro		       
declareMiniSkim Charm/LambdaCSuppressed        deepCopyMicro   deepCopyMicro		       
declareMiniSkim Charm/LambdaVeryVeryLoose      deepCopyMicro   pointer		       
declareMiniSkim Charm/XiMinus                  deepCopyMicro   deepCopyMicro		       
declareMiniSkim Charm/OmegaMinus               deepCopyMicro   deepCopyMicro		       
declareMiniSkim Charm/DtoXll                   deepCopyMicro   deepCopyMicro		       
declareMiniSkim Charm/DmixD0ToKPiPi0           pointer         pointer		       
declareMiniSkim Charm/DmixD0ToK3Pi             pointer         pointer		       
declareMiniSkim Charm/D0To2ProngDcj            pointer         pointer		       
declareMiniSkim Charm/D0To2ProngPi0CS          pointer         pointer		       
declareMiniSkim Charm/DstD0ToKsHpHm            pointer         pointer		       
declareMiniSkim Charm/DmixD0ToKenu             pointer         pointer		       
declareMiniSkim Charm/DmixD0ToKmunu            pointer         pointer		       
declareMiniSkim Charm/DcToKKKPrompt            pointer         pointer		       
declareMiniSkim Charm/DcToKpipiPrompt          pointer         pointer		       
declareMiniSkim Charm/DcToKpipiPrompt_DCS      pointer         pointer DcToKpipiPrompt_DCS  
declareMiniSkim Charm/DcTo3PiPrompt            pointer         pointer		       
declareMiniSkim Charm/DsToKpi0                 deepCopyMicro   pointer		       
declareMiniSkim Charm/DstToD0PiToVGamma        pointer         pointer		       
declareMiniSkim Charm/DstToD0PiToVPi0          pointer         pointer		       
declareMiniSkim Charm/DstToD0PiToVEta          pointer         pointer                      
declareMiniSkim Charm/DsstToDsGammaToVGamma    pointer         pointer
declareMiniSkim Charm/DstToDPiToVGamma         pointer         pointer
declareMiniSkim Charm/DsToPiKPhiKs             pointer         pointer
declareMiniSkim Charm/DchToPiKPi0Ks            pointer         pointer

###########################################################################################
# IHBD skims
declareMiniSkim IHBD/InclDstar            deepCopyMicro   deepCopyMicro
declareMiniSkim IHBD/InclSemiLept         deepCopyMicro   deepCopyMicro
declareMiniSkim IHBD/InclDstA1c           deepCopyMicro   deepCopyMicro
declareMiniSkim IHBD/InclDstPi            deepCopyMicro   deepCopyMicro
declareMiniSkim IHBD/InclD2stPi           deepCopyMicro   deepCopyMicro
declareMiniSkim IHBD/InclDstRho           deepCopyMicro   deepCopyMicro
declareMiniSkim IHBD/InclDstRhoComp       deepCopyMicro   deepCopyMicro
declareMiniSkim IHBD/InclD2stRho          deepCopyMicro   deepCopyMicro
declareMiniSkim IHBD/InclDs               deepCopyMicro   deepCopyMicro DsMainHardTightPID BGFMultiHadron
declareMiniSkim IHBD/InclD2stRhoComp      deepCopyMicro   deepCopyMicro

###########################################################################################
# XSL skims
declareMiniSkim XSL/BToDlnu               deepCopyMicro   deepCopyMicro
declareMiniSkim XSL/XSLBtoXulnuFilter     deepCopyMicro   deepCopyMicro
declareMiniSkim XSL/HardElectronFilter    pointer         pointer
declareMiniSkim XSL/CharmSL               deepCopyMicro   deepCopyMicro

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
declareMiniSkim ClHBD/ExclEtaP            deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/InclEta             deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/InclK0s             deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/InclOmega           deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/InclPhi             deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BtoPhiKPhiTo3pi     deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/A0                  deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BFourBody           deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BFourHHHH           deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BFourHHHP           deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BFourHHHK           deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BFourHHHE           deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BFourHHPP           deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BFourHHPK           deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BFourHHPE           deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BFourHHKK           deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BFourHHKE           deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/InclEtapRhoGam      deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BToA1Rho            deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BToA1Kst            deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/B0ToRhoPRhoM        deepCopyMicro   deepCopyMicro

declareMiniSkim ClHBD/Bppp0K              deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/Inclppbar           deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/InclProtTight       deepCopyMicro   deepCopyMicro

# classic charmless 3-body
declareMiniSkim ClHBD/BCCC3body           deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BKsKsKs3body        deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BCCPi03body         deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BCCKs3body          deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BCCKsPi0Pi03body    deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BPi0Pi0Ks3body      deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BPi0Pi0C3body       deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BCPi0Ks3body        deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BCCC03a3body        deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BKsKsKs03a3body     deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BCCPi003a3body      deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BCCKs03a3body       deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BCCKsPi0Pi003a3body deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BPi0Pi0Ks03a3body   deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BPi0Pi0C03a3body    deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BCPi0Ks03a3body     deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BtoKKKL             deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/KsKsKIncl           deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BToKsKsKlDefault    deepCopyMicro   deepCopyMicro

# multi-list 3-body combinations
declareMiniSkim ClHBD/BToPPP              deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BToCXX              deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BToCPP              deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BToCCC              deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BToPXX              deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BToKstarXX          deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BToLambda0XX        deepCopyMicro   deepCopyMicro
# multi-list 2-body combinations
declareMiniSkim ClHBD/BToPP               deepCopyMicro   deepCopyMicro

###########################################################################################
# RadPenguin skims
declareMiniSkim RadPenguin/BtoXGammaPi0        pointer         pointer								       
declareMiniSkim RadPenguin/BtoXGamma           deepCopyMicro   pointer								       
declareMiniSkim RadPenguin/BtoGammaGamma       deepCopyMicro   deepCopyMicro							       
declareMiniSkim RadPenguin/Kll                 pointer         pointer								       
declareMiniSkim RadPenguin/BtoRhoGamma         deepCopyMicro   deepCopyMicro							       
declareMiniSkim RadPenguin/ExclHllMini         deepCopyMicro   deepCopyMicro							       
declareMiniSkim RadPenguin/B0toOmegaGamma      deepCopyMicro   deepCopyMicro {B0toOmegaGamma}  {BGFMultiHadron BGFNeutralHadron}	       
declareMiniSkim RadPenguin/B0toPhiGamma        deepCopyMicro   deepCopyMicro {B0toPhiGamma}    {BGFMultiHadron BGFNeutralHadron}	       
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
#declareMiniSkim RadPenguin/BToXsll             deepCopyMicro   deepCopyMicro
#declareMiniSkim RadPenguin/BToXsll_peaking     deepCopyMicro   deepCopyMicro

###########################################################################################
# TauQED skims
declareMiniSkim TauQED/Tau11                   pointer         pointer	  
declareMiniSkim TauQED/Tau1N                   deepCopyMicro   deepCopyMicro  
declareMiniSkim TauQED/Tau33                   deepCopyMicro   deepCopyMicro  
# new two photon skims			                                      
declareMiniSkim TauQED/TwoPhotonTwoTrack       pointer         pointer	  
declareMiniSkim TauQED/TwoPhotonPi0Pi0         pointer         pointer	  
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
declareMiniSkim Isr/IsrIncExc              pointer         pointer

###########################################################################################
# B-Counting skims
declareMiniSkim BC/BCountMHFilter        noOutput        noOutput
declareMiniSkim BC/BCountMuMuFilter      noOutput        noOutput

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
declareMiniSkim IHPS/InclProtPiPiKsK      deepCopyMicro   pointer
declareMiniSkim IHPS/EEToKKpi             deepCopyMicro   deepCopyMicro {EEToKKpi Y4SToKsKPi_Ups Y4SToKsKPi_Ups_Tight Y4SToKsKsPi0_Ups Y4SToKsKsPi0_Ups_Tight Y4SToKKPi0_Ups Y4SToKKPi0_Ups_Tight}
declareMiniSkim IHPS/EEToKKpipi           deepCopyMicro   deepCopyMicro {EEToKKpipi Y4SToKsKPiPi0_Ups Y4SToKsKPiPi0_Ups_Tight Y4SToKsKsPi0Pi0_Ups Y4SToKsKsPi0Pi0_Ups_Tight Y4SToKsKsPiPi_Ups Y4SToKsKsPiPi_Ups_Tight Y4SToKKPi0Pi0_Ups Y4SToKKPi0Pi0_Ups_Tight Y4SToKKPiPi_Ups Y4SToKKPiPi_Ups_Tight}
declareMiniSkim IHPS/EEToPSPS             deepCopyMicro   deepCopyMicro {EEToPSPS Y4SToKsKs_Ups Y4SToKK_Ups Y4SToPiPi_Ups Y4SToPi0Pi0_Ups}
declareMiniSkim IHPS/EEToKKEta            deepCopyMicro   deepCopyMicro {EEToKKEtaLoose EEToKKEta}
declareMiniSkim IHPS/EEToPhiEta           deepCopyMicro   deepCopyMicro {EEToPhiEta}
