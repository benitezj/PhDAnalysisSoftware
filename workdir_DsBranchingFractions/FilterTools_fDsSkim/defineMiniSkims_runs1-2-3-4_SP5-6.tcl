# $Id: defineMiniSkims_runs1-2-3-4_SP5-6.tcl,v 1.7 2005/04/22 22:29:16 bhuyan Exp $
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
declareMiniSkim AllEvents            KanGA/AllEvents            tagOnly         tagOnly
# Breco skims
declareMiniSkim B0DKNeut             Breco/B0DKNeut             deepCopyMicro   deepCopyMicro
declareMiniSkim B0DNeutralLight      Breco/B0DNeutralLight      deepCopyMicro   deepCopyMicro
declareMiniSkim B0ToD0KPi            Breco/B0ToD0KPi            deepCopyMicro   deepCopyMicro
declareMiniSkim B0ToDsDs             Breco/B0ToDsDs             deepCopyMicro   deepCopyMicro
declareMiniSkim B0ToDsJh             Breco/BToDsJh              deepCopyMicro   deepCopyMicro
declareMiniSkim B0ToDstarA1CLoose    Breco/B0ToDstarA1CLoose    deepCopyMicro   deepCopyMicro
declareMiniSkim BDLightPiNoPID       Breco/BDLightPiNoPID       deepCopyMicro   deepCopyMicro
declareMiniSkim BRecoToDDstar        Breco/BRecoToDDstar        pointer         pointer
declareMiniSkim BRecoToDDstarEtaPi   Breco/BRecoToDDstarEtaPi   deepCopyMicro   deepCopyMicro
declareMiniSkim BRecoToDDstarPi      Breco/BRecoToDDstarPi      deepCopyMicro   deepCopyMicro
declareMiniSkim BRecoToDKPi          Breco/BRecoToDKPi          deepCopyMicro   deepCopyMicro
declareMiniSkim BRecoToDPi0          Breco/BRecoToDPi0          deepCopyMicro   deepCopyMicro
declareMiniSkim BRecoToDsDsstarEtaPi Breco/BRecoToDsDsstarEtaPi deepCopyMicro   deepCopyMicro
declareMiniSkim BRecoToDsLight       Breco/BRecoToDsLight       deepCopyMicro   deepCopyMicro
declareMiniSkim BSemiExcl            Breco/BSemiExcl            deepCopyMicro   deepCopyMicro
declareMiniSkim BToD2sD              Breco/BToD2sD              deepCopyMicro   deepCopyMicro
declareMiniSkim BToDD                Breco/BToDD                deepCopyMicro   deepCopyMicro
declareMiniSkim BToDDK               Breco/BToDDK               deepCopyMicro   deepCopyMicro
declareMiniSkim BToDOmegaPi          Breco/BToDOmegaPi          deepCopyMicro   deepCopyMicro
declareMiniSkim BToDPPbarX           Breco/BToDPPbarX           deepCopyMicro   deepCopyMicro
declareMiniSkim BchDKNeut            Breco/BchDKNeut            deepCopyMicro   deepCopyMicro
declareMiniSkim BchToD0KAll          Breco/BchToD0KAll          deepCopyMicro   deepCopyMicro
declareMiniSkim BchToD0KstarAll      Breco/BchToD0KstarAll      deepCopyMicro   deepCopyMicro
declareMiniSkim BchToDKCabNonCP      Breco/BchToDKCabNonCP      deepCopyMicro   deepCopyMicro
declareMiniSkim BchToDKstCabNonCP    Breco/BchToDKstCabNonCP    deepCopyMicro   deepCopyMicro
declareMiniSkim BtoDPiPi             Breco/BtoDPiPi             deepCopyMicro   deepCopyMicro
# Charm skims
declareMiniSkim D0To2ProngDcj        Charm/D0To2ProngDcj        deepCopyMicro   deepCopyMicro
declareMiniSkim D0To2ProngPi0CS      Charm/D0To2ProngPi0CS      deepCopyMicro   deepCopyMicro
declareMiniSkim DcTo3PiPrompt        Charm/DcTo3PiPrompt        deepCopyMicro   deepCopyMicro
declareMiniSkim DcToKKKPrompt        Charm/DcToKKKPrompt        deepCopyMicro   deepCopyMicro
declareMiniSkim DcToKpipiPrompt      Charm/DcToKpipiPrompt      deepCopyMicro   deepCopyMicro
declareMiniSkim DmixD0ToK3Pi         Charm/DmixD0ToK3Pi         deepCopyMicro   deepCopyMicro
declareMiniSkim DmixD0ToKPiPi0       Charm/DmixD0ToKPiPi0       deepCopyMicro   deepCopyMicro
declareMiniSkim DmixD0ToKenu         Charm/DmixD0ToKenu         deepCopyMicro   deepCopyMicro
declareMiniSkim DmixD0ToKmunu        Charm/DmixD0ToKmunu        deepCopyMicro   deepCopyMicro
declareMiniSkim DstD0ToKsHpHm        Charm/DstD0ToKsHpHm        deepCopyMicro   deepCopyMicro
declareMiniSkim LambdaVeryVeryLoose  Charm/LambdaVeryVeryLoose  deepCopyMicro   deepCopyMicro
# Charmonium skims
declareMiniSkim BCharmoniumToHad     Charmonium/BCharmoniumToHad    deepCopyMicro   deepCopyMicro
declareMiniSkim BCharmoniumToHadRad  Charmonium/BCharmoniumToHadRad deepCopyMicro   deepCopyMicro
declareMiniSkim JpsiISR              Charmonium/JpsiISR         deepCopyMicro   deepCopyMicro
declareMiniSkim Jpsitoll             Charmonium/Jpsitoll        deepCopyMicro   deepCopyMicro
declareMiniSkim JpsitollTight        Charmonium/JpsitollTight   deepCopyMicro   deepCopyMicro
# ClHBD skims
declareMiniSkim B0ToKsKs             ClHBD/B0ToKsKs             deepCopyMicro   deepCopyMicro B0ToKsKsTag
declareMiniSkim B0ToKsPi0            ClHBD/B0ToKsPi0            deepCopyMicro   deepCopyMicro B0ToKsPi0Tag
declareMiniSkim B0ToLambdaLambda     ClHBD/B0ToLambdaLambda     deepCopyMicro   deepCopyMicro B0ToLambdaLambdaTag
declareMiniSkim B0ToPP               ClHBD/B0ToPP               deepCopyMicro   deepCopyMicro B0ToPPTag
declareMiniSkim B0ToPi0Pi0           ClHBD/B0ToPi0Pi0           deepCopyMicro   deepCopyMicro B0ToPi0Pi0Tag
declareMiniSkim B0ToPiPi             ClHBD/B0ToPiPi             deepCopyMicro   deepCopyMicro B0ToPiPiTag
declareMiniSkim BCCKs03a3body        ClHBD/BCCKs03a3body        deepCopyMicro   deepCopyMicro
declareMiniSkim BCCKs3body           ClHBD/BCCKs3body           deepCopyMicro   deepCopyMicro
declareMiniSkim BCPi0Ks03a3body      ClHBD/BCPi0Ks03a3body      deepCopyMicro   deepCopyMicro
declareMiniSkim BCPi0Ks3body         ClHBD/BCPi0Ks3body         deepCopyMicro   deepCopyMicro
declareMiniSkim BKsKsC03a3body       ClHBD/BKsKsC03a3body       deepCopyMicro   deepCopyMicro
declareMiniSkim BKsKsC3body          ClHBD/BKsKsC3body          deepCopyMicro   deepCopyMicro
declareMiniSkim BKsKsKs03a3body      ClHBD/BKsKsKs03a3body      deepCopyMicro   deepCopyMicro
declareMiniSkim BKsKsKs3body         ClHBD/BKsKsKs3body         deepCopyMicro   deepCopyMicro
declareMiniSkim BKsKsPi003a3body     ClHBD/BKsKsPi003a3body     deepCopyMicro   deepCopyMicro
declareMiniSkim BKsKsPi03body        ClHBD/BKsKsPi03body        deepCopyMicro   deepCopyMicro
declareMiniSkim BToKsPi              ClHBD/BToKsPi              deepCopyMicro   deepCopyMicro BToKsPiTag
declareMiniSkim BToLambdaP           ClHBD/BToLambdaP           deepCopyMicro   deepCopyMicro BToLambdaPTag
declareMiniSkim BToPi0Pi             ClHBD/BToPi0Pi             deepCopyMicro   deepCopyMicro BToPi0PiTag
declareMiniSkim InclPhiTo3pi         ClHBD/InclPhiTo3pi         pointer         pointer         
declareMiniSkim InclProtTight        ClHBD/InclProtTight        pointer         pointer
declareMiniSkim Inclppbar            ClHBD/Inclppbar            deepCopyMicro   deepCopyMicro
declareMiniSkim BtoKKKL              ClHBD/BtoKKKL              pointer         pointer
# IHBD skims
declareMiniSkim InclDstar            IHBD/InclDstar             deepCopyMicro   deepCopyMicro
declareMiniSkim InclSemiLept         IHBD/InclSemiLept          deepCopyMicro   deepCopyMicro
# Isr
declareMiniSkim IsrIncExc            Isr/IsrIncExc              pointer         pointer       { } { } { } { } { InclusiveIsr ExclusiveIsr }
declareMiniSkim RadTwoProng          Isr/RadTwoProng            deepCopyMicro   deepCopyMicro { } { } { } { } { BGFRadTwoProng }
# LeptNC
declareMiniSkim DsToMuNu             LeptBC/DsToMuNu            deepCopyMicro   deepCopyMicro { } { } { } { } { DsToMuNuCharmTagAndLept DsToMuNuCharmTagAndDsPhiPi DsToMuNuCharmTagAndDKPi }
# PTF
declareMiniSkim AntiDeuteron         PTF/AntiDeuteron           deepCopyMini    deepCopyMicro
declareMiniSkim Dsppbar              PTF/Dsppbar                deepCopyMicro   deepCopyMicro
# RadPenguin
declareMiniSkim BtoKsPi0Gamma        RadPenguin/BtoKsPi0Gamma   deepCopyMicro   deepCopyMicro BtoKsPi0GammaTag
declareMiniSkim ExclHllMini          RadPenguin/ExclHllMini     deepCopyMicro   deepCopyMicro
declareMiniSkim Kll                  RadPenguin/Kll             deepCopyMicro   deepCopyMicro
# S2BMix
declareMiniSkim B0ToJpsiPi0Loose     S2BMix/B0ToJpsiPi0Loose    deepCopyMicro   deepCopyMicro
declareMiniSkim B0ToccKFinal         S2BMix/B0ToccKFinal        deepCopyMini    deepCopyMicro
declareMiniSkim B0ToccKLoose         S2BMix/B0ToccKLoose        deepCopyMicro   deepCopyMicro
declareMiniSkim B0ToccKS2b           S2BMix/B0ToccKS2b          deepCopyMini    deepCopyMini
declareMiniSkim B0ToccKstarFinal     S2BMix/B0ToccKstarFinal    deepCopyMini    deepCopyMicro
declareMiniSkim B0ToccKstarLoose     S2BMix/B0ToccKstarLoose    deepCopyMicro   deepCopyMicro
declareMiniSkim BchToccKFinal        S2BMix/BchToccKFinal       deepCopyMini    deepCopyMicro
declareMiniSkim BchToccKLoose        S2BMix/BchToccKLoose       deepCopyMicro   deepCopyMicro
declareMiniSkim BchToccKstarFinal    S2BMix/BchToccKstarFinal   deepCopyMini    deepCopyMicro
declareMiniSkim BchToccKstarLoose    S2BMix/BchToccKstarLoose   deepCopyMicro   deepCopyMicro
declareMiniSkim Dilepton             S2BMix/DiLepton            deepCopyMicro   deepCopyMicro
# TauQED
declareMiniSkim TwoPhotonPentaquark  TauQED/TwoPhotonPentaquark pointer         pointer
declareMiniSkim TwoPhotonPi0Pi0      TauQED/TwoPhotonPi0Pi0     deepCopyMicro   deepCopyMicro
declareMiniSkim TwoPhotonTwoTrack    TauQED/TwoPhotonTwoTrack   deepCopyMicro   deepCopyMicro
# XSL
declareMiniSkim BToDlnu              XSL/BToDlnu                deepCopyMicro   deepCopyMicro
declareMiniSkim XSLBtoXulnuFilter    XSL/XSLBtoXulnuFilter      deepCopyMicro   deepCopyMicro
#
declareMiniSkim AlignCal             Tracking/AlignCal          noOutput        noOutput
declareMiniSkim BCountMHFilter       BC/BCMultiHad              noOutput        noOutput
declareMiniSkim BCountMuMuFilter     BC/BCMuMu                  noOutput        noOutput
declareMiniSkim LumiTag              Lumi/PreLumiEvents         noOutput        noOutput
declareMiniSkim Prescale             TauQED/Prescale            noOutput        noOutput
declareMiniSkim Damage               Trigger/DamagedEvents      noOutput        noOutput
declareMiniSkim VCS                  EMC/VCS                    noOutput        noOutput
declareMiniSkim L3Cosmic             Tracking/L3OutCosmic       noOutput        noOutput
declareMiniSkim RandomBackground     Trigger/BackgroundTriggers noOutput        noOutput
#############################################################################################
# Tracking Efficiency skim
declareMiniSkim  TrkEffTau31         Tracking/TrkEffTau31       deepCopyMicro   deepCopyMicro

