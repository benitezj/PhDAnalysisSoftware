# $Id: defineDatasets.tcl,v 1.10 2007/02/11 02:55:55 marchior Exp $
#
# Dataset configuration file for skim production
#
# The second column indicates into which dataset the skim
# collections are inserted.
# This file is not read by SkimMiniApp at all. I.e. the skims to run
# for a particular skim cycle are still determined by $SkimCyle and
# the corresponding defineMiniSkims_$SkimCycle.tcl file.
# For the future one can think of merging all this information into one file.
#

# AllEvents collection
defineDataset KanGA/AllEvents                          R22a

#
# List of R22a skims (skims originally planned for R20)
#
defineDataset Tracking/TrkEffTau31                     R22a
defineDataset PID/BPCBhabha                            R22a
defineDataset PID/BPCKaon                              R22a
defineDataset PID/BPCKLong                             R22a
defineDataset PID/BPCMuon                              R22a
defineDataset PID/BPCProton                            R22a
defineDataset PID/BPCElectron                          R22a
defineDataset PID/BPCPion                              R22a
defineDataset PID/DcToKPiPiPromptNoPid                 R22a
defineDataset PID/SigmaCToLambdaCNoPidProton           R22a
defineDataset Neutral/NeutralD0toKpipi0                R22a
defineDataset Neutral/NeutralDstartoD0pi0              R22a
defineDataset Breco/BSemiExcl                          R22a
defineDataset S2BMix/BFlav_Final                       R22a
defineDataset S2BMix/B0ToccKFinal                      R22a
defineDataset Charmonium/Jpsitoll                      R22a
defineDataset Charm/LambdaVeryVeryLoose                R22a
defineDataset Charm/XiMinus                            R22a
defineDataset Charm/OmegaMinus                         R22a
defineDataset XSL/BToDlnu                              R22a
defineDataset ClHBD/BToPPP                             R22a
defineDataset ClHBD/BToPP                              R22a
defineDataset TauQED/Tau11                             R22a
defineDataset TauQED/Tau1N                             R22a
defineDataset TauQED/Tau33                             R22a


#
# List of R22b skims
#
defineDataset Tracking/AlignCal                        R22b
defineDataset Tracking/PreVeto                         R22b
defineDataset Breco/BRecoToDDstar                      R22b
defineDataset Breco/BRecoToDDstarPi                    R22b
defineDataset Breco/BDLightPiNoPID                     R22b
defineDataset Breco/BRecoToDsLight                     R22b
defineDataset Breco/BRecoToDPi0                        R22b
defineDataset Breco/BRecoToDKPi                        R22b
defineDataset Breco/B0DKNeut                           R22b
defineDataset Breco/BchDKNeut                          R22b
defineDataset Breco/BToDD                              R22b
defineDataset Breco/BchToD0KAll                        R22b
defineDataset Breco/BchToD0KstarAll                    R22b
defineDataset Breco/B0ToD0KPi                          R22b
defineDataset Breco/BToDDK                             R22b
defineDataset Breco/BchToDKCabNonCP                    R22b
defineDataset Breco/BchToDKstCabNonCP                  R22b
defineDataset Breco/BToD2sD                            R22b
defineDataset Breco/BRecoToDDstarEtaPi                 R22b
defineDataset Breco/BRecoToDsDsstarEtaPi               R22b
defineDataset Breco/BToDPPbarX                         R22b
defineDataset Breco/B0ToDstarA1CLoose                  R22b
defineDataset Breco/B0DNeutralLight                    R22b
defineDataset Breco/B0ToDsJh                           R22b
defineDataset Breco/BtoDPiPi                           R22b
defineDataset Breco/Dsppbar                            R22b
defineDataset Breco/RbD0ToKmunu                        R22b
defineDataset S2BMix/BchFlav_Final                     R22b
defineDataset S2BMix/BchToccKFinal                     R22b
defineDataset S2BMix/B0ToccKstarFinal                  R22b
defineDataset S2BMix/BchToccKstarFinal                 R22b
defineDataset S2BMix/B0ToEtacKsFinal                   R22b
defineDataset S2BMix/BchToEtacKchFinal                 R22b
defineDataset S2BMix/Dilepton                          R22b
defineDataset S2BMix/B0ToJpsiPi0Loose                  R22b
defineDataset S2BMix/B0ToDDstar0h0                     R22b
defineDataset Charmonium/JpsitollTight                 R22b
defineDataset Charmonium/BToJpsiHH                     R22b
defineDataset Charm/LambdaC                            R22b
defineDataset Charm/DtoXll                             R22b
defineDataset Charm/DmixD0ToKPiPi0                     R22b
defineDataset Charm/DmixD0ToK3Pi                       R22b
defineDataset Charm/D0To2ProngDcj                      R22b
defineDataset Charm/D0To2ProngPi0CS                    R22b
defineDataset Charm/DstD0ToKsHpHm                      R22b
defineDataset Charm/DmixD0ToKenu                       R22b
defineDataset Charm/DmixD0ToKmunu                      R22b
defineDataset Charm/DcToKKKPrompt                      R22b
defineDataset Charm/DcToKpipiPrompt                    R22b
defineDataset Charm/DcToKpipiPrompt_DCS                R22b
defineDataset Charm/DcTo3PiPrompt                      R22b
defineDataset Charm/DsToKpi0                           R22b
defineDataset Charm/DstToD0PiToVGamma                  R22b
defineDataset Charm/DstToD0PiToVPi0                    R22b
defineDataset Charm/DstToD0PiToVEta                    R22b
defineDataset Charm/DsstToDsGammaToVGamma              R22b
defineDataset Charm/DstToDPiToVGamma                   R22b
defineDataset Charm/DsToPiKPhiKs                       R22b
defineDataset Charm/DcToKKPiPrompt                     R22b
defineDataset IHBD/InclDstar                           R22b
defineDataset IHBD/InclSemiLept                        R22b
defineDataset IHBD/InclDstA1c                          R22b
defineDataset IHBD/InclDstPi                           R22b
defineDataset IHBD/InclD2stPi                          R22b
defineDataset IHBD/InclDstRho                          R22b
defineDataset IHBD/InclDstRhoComp                      R22b
defineDataset XSL/XSLBtoXulnuFilter                    R22b
defineDataset XSL/HardElectronFilter                   R22b
defineDataset XSL/CharmSL                              R22b
defineDataset ClHBD/BToKsPi                            R22b
defineDataset ClHBD/B0ToPiPi                           R22b
defineDataset ClHBD/B0ToKsKs                           R22b
defineDataset ClHBD/BToPi0Pi                           R22b
defineDataset ClHBD/B0ToPi0Pi0                         R22b
defineDataset ClHBD/B0ToKsPi0                          R22b
defineDataset ClHBD/B0ToPP                             R22b
defineDataset ClHBD/B0ToLambdaLambda                   R22b
defineDataset ClHBD/BToLambdaP                         R22b
defineDataset ClHBD/InclEta                            R22b
defineDataset ClHBD/InclPhi                            R22b
defineDataset ClHBD/A0                                 R22b
defineDataset ClHBD/BFourBody                          R22b
defineDataset ClHBD/BFourHHHH                          R22b
defineDataset ClHBD/BFourHHHP                          R22b
defineDataset ClHBD/BFourHHHK                          R22b
defineDataset ClHBD/BFourHHHE                          R22b
defineDataset ClHBD/BFourHHPP                          R22b
defineDataset ClHBD/BFourHHPK                          R22b
defineDataset ClHBD/BFourHHKK                          R22b
defineDataset ClHBD/BFourHHKE                          R22b
defineDataset ClHBD/BToA1Kst                           R22b
defineDataset ClHBD/B0ToRhoPRhoM                       R22b
defineDataset ClHBD/Bppp0K                             R22b
defineDataset ClHBD/Inclppbar                          R22b
defineDataset ClHBD/InclProtTight                      R22b
defineDataset ClHBD/BCCC3body                          R22b
defineDataset ClHBD/BKsKsKs3body                       R22b
defineDataset ClHBD/BCCPi03body                        R22b
defineDataset ClHBD/BCCKs3body                         R22b
defineDataset ClHBD/BPi0Pi0Ks3body                     R22b
defineDataset ClHBD/BPi0Pi0C3body                      R22b
defineDataset ClHBD/BCPi0Ks3body                       R22b
defineDataset ClHBD/BCCC03a3body                       R22b
defineDataset ClHBD/BKsKsKs03a3body                    R22b
defineDataset ClHBD/BCCPi003a3body                     R22b
defineDataset ClHBD/BCCKs03a3body                      R22b
defineDataset ClHBD/BPi0Pi0Ks03a3body                  R22b
defineDataset ClHBD/BPi0Pi0C03a3body                   R22b
defineDataset ClHBD/BCPi0Ks03a3body                    R22b
defineDataset ClHBD/BtoKKKL                            R22b
defineDataset ClHBD/KsKsKIncl                          R22b
defineDataset ClHBD/BToKsKsKlDefault                   R22b
defineDataset ClHBD/BToCXX                             R22b
defineDataset ClHBD/BToCPP                             R22b
defineDataset ClHBD/BToCCC                             R22b
defineDataset ClHBD/BToPXX                             R22b
defineDataset ClHBD/BToKstarXX                         R22b
defineDataset ClHBD/BToLambda0XX                       R22b
defineDataset RadPenguin/BtoXGammaPi0                  R22b
defineDataset RadPenguin/BtoXGamma                     R22b
defineDataset RadPenguin/BtoGammaGamma                 R22b
defineDataset RadPenguin/Kll                           R22b
defineDataset RadPenguin/BtoRhoGamma                   R22b
defineDataset RadPenguin/ExclHllMini                   R22b
defineDataset RadPenguin/B0toKsPi0Gamma                R22b
defineDataset RadPenguin/B0toKPiGamma                  R22b
defineDataset RadPenguin/BtoKPi0Gamma                  R22b
defineDataset RadPenguin/BtoKsPiGamma                  R22b
defineDataset RadPenguin/B0toKsEtaGamma                R22b
defineDataset RadPenguin/BtoKEtaGamma                  R22b
defineDataset RadPenguin/B0toKsEtaPGamma               R22b
defineDataset RadPenguin/BtoKEtaPGamma                 R22b
defineDataset RadPenguin/B0toKsPhiGamma                R22b
defineDataset RadPenguin/BtoKPhiGamma                  R22b
defineDataset RadPenguin/BtoLambdaPbarGamma            R22b
defineDataset RadPenguin/BtoSigma0PbarGamma            R22b
defineDataset RadPenguin/BToXsll                       R22b
defineDataset RadPenguin/BToXsll_peaking               R22b
defineDataset TauQED/TwoPhotonTwoTrack                 R22b
defineDataset TauQED/TwoPhotonPi0Pi0                   R22b
defineDataset TauQED/TwoPhotonPentaquark               R22b
defineDataset Isr/IsrGamma                             R22b
defineDataset Isr/IsrNoGamma                           R22b
defineDataset Isr/IsrLooseGamma                        R22b
defineDataset Isr/IsrRadBhabha                         R22b
defineDataset Isr/RadTwoProng                          R22b
defineDataset Isr/IsrDsmeson                           R22b
defineDataset Isr/IsrDmeson                            R22b
defineDataset Isr/IsrIncExc                            R22b
defineDataset BC/BCountMHFilter                        R22b
defineDataset BC/BCountMuMuFilter                      R22b
defineDataset Lumi/LumiTag                             R22b
defineDataset TauQED/Prescale                          R22b
defineDataset Trigger/Damage                           R22b
defineDataset EMC/VCS                                  R22b
defineDataset Tracking/L3Cosmic                        R22b
defineDataset Trigger/RandomBackground                 R22b
defineDataset LeptBC/DsToMuNu                          R22b
defineDataset IHPS/AntiDeuteron                        R22b
defineDataset IHPS/InclProtPiPiKsK                     R22b
defineDataset IHPS/EEToKKpi                            R22b
defineDataset IHPS/EEToKKpipi                          R22b
defineDataset IHPS/EEToPSPS                            R22b
defineDataset IHPS/EEToKKEta                           R22b
defineDataset IHPS/EEToPhiEta                          R22b
