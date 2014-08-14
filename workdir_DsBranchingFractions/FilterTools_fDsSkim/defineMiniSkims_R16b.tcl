# $Id: defineMiniSkims_R16b.tcl,v 1.16 2005/06/14 13:58:32 fnc Exp $
#
# this file defines paths for SkimMiniApp
#
# Valid choices for the 3rd and 4th arguments to declareMiniSkim
#        Name             Components owned (copied)   Components borrowed (pointers back)
# 1. "deepCopyMicro"      tag, aod, cnd, usr, (tru)              esd
# 2. "deepCopyMini"       tag, aod, cnd, usr, (tru), esd         None
# 3. "pointer"            tag, cnd, usr                          aod, (tru) esd
# 4. "tagOnly"            tag                                    aod, cnd, (tru), esd
# 5. "noOutput" Just runs the selection code (and increments the appropriate counters, which
#               get printed out at job completion) but does not produce any skim collection.
# The 3rd argument decides for data skims, the 4th for MC skims.
#
###########################################################################################
#
#               Skim                "tagName"                 dataOutputType  MCOutputType
#
###########################################################################################
declareMiniSkim AllEvents           KanGA/AllEvents           tagOnly         tagOnly
# Quarkonium skims
declareMiniSkim BToJpsiHH           Quarkonium/BToJpsiHH      pointer         pointer
declareMiniSkim ExclMultiTracksHad  Quarkonium/ExclMultiTracksHad pointer     pointer
# XSL skims
declareMiniSkim BToDlnu             XSL/BToDlnu               deepCopyMicro   deepCopyMicro
# ClHBD skims
declareMiniSkim BToA1Rho            ClHBD/BToA1Rho            deepCopyMicro   deepCopyMicro
declareMiniSkim BToA1Kst            ClHBD/BToA1Kst            deepCopyMicro   deepCopyMicro
declareMiniSkim B0ToRhoPRhoM        ClHBD/B0ToRhoPRhoM        deepCopyMicro   deepCopyMicro
declareMiniSkim InclPhi             ClHBD/InclPhi             pointer         pointer
declareMiniSkim BCCKsPi0Pi03body    ClHBD/BCCKsPi0Pi03body    deepCopyMicro   deepCopyMicro
declareMiniSkim BCCKsPi0Pi003a3body ClHBD/BCCKsPi0Pi003a3body deepCopyMicro   deepCopyMicro
declareMiniSkim KsKsKIncl           ClHBD/KsKsKIncl           deepCopyMicro   deepCopyMicro
declareMiniSkim BToKsKsKlDefault    ClHBD/BToKsKsKlDefault    deepCopyMicro   deepCopyMicro
# multi-list 3-body combinations
declareMiniSkim BToPPP              ClHBD/BToPPP              deepCopyMicro   deepCopyMicro
declareMiniSkim BToCXX              ClHBD/BToCXX              deepCopyMicro   deepCopyMicro
declareMiniSkim BToCPP              ClHBD/BToCPP              deepCopyMicro   deepCopyMicro
declareMiniSkim BToCCC              ClHBD/BToCCC              deepCopyMicro   deepCopyMicro
declareMiniSkim BToPXX              ClHBD/BToPXX              deepCopyMicro   deepCopyMicro
declareMiniSkim BToKstarXX          ClHBD/BToKstarXX          deepCopyMicro   deepCopyMicro
# multi-list 2-body combinations
declareMiniSkim BToPP               ClHBD/BToPP               deepCopyMicro   deepCopyMicro
# RadPenguin skims
declareMiniSkim BtoXGammaPi0        RadPenguin/BtoXGammaPi0   pointer         pointer
declareMiniSkim BtoXGamma           RadPenguin/BtoXGamma      deepCopyMicro   deepCopyMicro
declareMiniSkim BtoGammaGamma       RadPenguin/BtoGammaGamma  deepCopyMicro   deepCopyMicro
declareMiniSkim BtoRhoGamma         RadPenguin/BtoRhoGamma    deepCopyMicro   deepCopyMicro
# Charm skims
declareMiniSkim DsToKpi0            Charm/DsToKpi0            deepCopyMicro   deepCopyMicro
declareMiniSkim LambdaC             Charm/LambdaC             deepCopyMicro   deepCopyMicro
declareMiniSkim LambdaCSuppressed   Charm/LambdaCSuppressed   deepCopyMicro   deepCopyMicro
declareMiniSkim RbD0ToKenu          Charm/RbD0ToKenu          pointer         pointer
declareMiniSkim RbD0ToKmunu         Charm/RbD0ToKmunu         pointer         pointer
# KKpi KKpipi KK pipi production at the U(4S) skims
declareMiniSkim EEToKKpi            IHPS/EEToKKpi             deepCopyMicro   deepCopyMicro {EEToKKpi Y4SToKsKPi_Ups_Tight Y4SToKsKsPi0_Ups_Tight Y4SToKKPi0_Ups_Tight}
declareMiniSkim EEToKKpipi          IHPS/EEToKKpipi           deepCopyMicro   deepCopyMicro {EEToKKpipi Y4SToKsKPiPi0_Ups_Tight Y4SToKsKsPi0Pi0_Ups_Tight Y4SToKsKsPiPi_Ups_Tight Y4SToKKPi0Pi0_Ups_Tight Y4SToKKPiPi_Ups_Tight}
declareMiniSkim EEToPSPS            IHPS/EEToPSPS             deepCopyMicro   deepCopyMicro {EEToPSPS Y4SToKsKs_Ups Y4SToKK_Ups Y4SToPiPi_Ups Y4SToPi0Pi0_Ups}
# B-Counting skims
declareMiniSkim BCountMHFilter      BC/BCMultiHad             noOutput        noOutput
declareMiniSkim BCountMuMuFilter    BC/BCMuMu                 noOutput        noOutput
