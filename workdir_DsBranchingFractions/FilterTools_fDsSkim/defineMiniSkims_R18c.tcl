# $Id: defineMiniSkims_R18c.tcl,v 1.15 2006/04/10 18:44:05 fwinkl Exp $
#
# this file defines paths for the R18c skim cycle
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
declareMiniSkim PID/BPCPion                deepCopyMicro   pointer
declareMiniSkim Neutral/NeutralD0toKpipi0    pointer         pointer
declareMiniSkim Neutral/NeutralDstartoD0pi0  pointer         pointer
declareMiniSkim Breco/BRecoToDDstarPi      deepCopyMicro   pointer
declareMiniSkim Breco/BRecoToDsLight       deepCopyMicro   pointer
declareMiniSkim Breco/BRecoToDKPi          deepCopyMicro   pointer
declareMiniSkim Breco/BchToD0KAll          deepCopyMicro   pointer
declareMiniSkim Breco/BchToD0KstarAll      deepCopyMicro   pointer
declareMiniSkim Breco/BRecoToDDstarEtaPi   deepCopyMicro   pointer
declareMiniSkim Breco/BToDPPbarX           deepCopyMicro   pointer {BToD0PPbarX BToDcPPbarX} BGFMultiHadron
declareMiniSkim Breco/BtoDPiPi             deepCopyMicro   pointer
declareMiniSkim Breco/RbD0ToKenu           deepCopyMicro   deepCopyMicro
declareMiniSkim Breco/RbD0ToKmunu          deepCopyMicro   deepCopyMicro
declareMiniSkim Breco/B0ToDstarhKlnu       deepCopyMicro   deepCopyMicro
declareMiniSkim Breco/BtoDsDsK3prong       deepCopyMicro   deepCopyMicro 
declareMiniSkim Breco/BtoDsDsK6modes       deepCopyMicro   pointer
declareMiniSkim S2BMix/B0ToccKS2b         deepCopyMini    deepCopyMini
declareMiniSkim S2BMix/B0ToccKFinal       deepCopyMini    deepCopyMicro
declareMiniSkim S2BMix/B0ToccKLoose       deepCopyMicro   deepCopyMicro
declareMiniSkim S2BMix/BchToccKLoose      deepCopyMicro   deepCopyMicro
declareMiniSkim S2BMix/BchToccKFinal      deepCopyMini    deepCopyMicro
declareMiniSkim S2BMix/B0ToDDstar0h0      deepCopyMicro   deepCopyMicro
declareMiniSkim Charmonium/Jpsitoll            pointer         pointer
declareMiniSkim Charmonium/JpsitollTight       deepCopyMicro   deepCopyMicro
declareMiniSkim Charm/LambdaVeryVeryLoose deepCopyMicro   pointer
declareMiniSkim Charm/DsToKpi0            deepCopyMicro   pointer
declareMiniSkim Charm/DstToD0PiToVGamma   pointer         pointer
declareMiniSkim Charm/DstToD0PiToVPi0     pointer         pointer
declareMiniSkim Charm/DstToD0PiToVEta     pointer         pointer
declareMiniSkim Charm/DsToPiKPhiKs        pointer         pointer
declareMiniSkim Charm/DchToPiKPi0Ks       pointer         pointer
declareMiniSkim IHBD/InclSemiLept         deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/InclPhi             deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/InclEtapRhoGam      deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/Bppp0K              deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/Inclppbar           deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/InclProtTight       deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BtoKKKL             deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BToCXX              deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BToLambda0XX        deepCopyMicro   deepCopyMicro
declareMiniSkim RadPenguin/BtoXGamma       deepCopyMicro   pointer
declareMiniSkim RadPenguin/BtoLambdaPbarGamma deepCopyMicro   deepCopyMicro {BtoLambdaPbarGamma} {BGFMultiHadron BGFNeutralHadron}
declareMiniSkim RadPenguin/BtoSigma0PbarGamma deepCopyMicro   deepCopyMicro {BtoSigma0PbarGamma} {BGFMultiHadron BGFNeutralHadron}
declareMiniSkim IHPS/InclProtPiPiKsK      deepCopyMicro   pointer
declareMiniSkim IHPS/EEToKKpi             deepCopyMicro   deepCopyMicro {EEToKKpi Y4SToKsKPi_Ups Y4SToKsKPi_Ups_Tight Y4SToKsKsPi0_Ups Y4SToKsKsPi0_Ups_Tight Y4SToKKPi0_Ups Y4SToKKPi0_Ups_Tight}
declareMiniSkim IHPS/EEToKKpipi           deepCopyMicro   deepCopyMicro {EEToKKpipi Y4SToKsKPiPi0_Ups Y4SToKsKPiPi0_Ups_Tight Y4SToKsKsPi0Pi0_Ups Y4SToKsKsPi0Pi0_Ups_Tight Y4SToKsKsPiPi_Ups Y4SToKsKsPiPi_Ups_Tight Y4SToKKPi0Pi0_Ups Y4SToKKPi0Pi0_Ups_Tight Y4SToKKPiPi_Ups Y4SToKKPiPi_Ups_Tight}
declareMiniSkim IHPS/EEToPSPS             deepCopyMicro   deepCopyMicro {EEToPSPS Y4SToKsKs_Ups Y4SToKK_Ups Y4SToPiPi_Ups Y4SToPi0Pi0_Ups}
declareMiniSkim IHPS/EEToKKEta            deepCopyMicro   deepCopyMicro {EEToKKEtaLoose EEToKKEta}
declareMiniSkim IHPS/EEToPhiEta           deepCopyMicro   deepCopyMicro {EEToPhiEta}
