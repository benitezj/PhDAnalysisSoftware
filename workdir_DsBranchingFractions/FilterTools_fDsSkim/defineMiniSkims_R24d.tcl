# $Id: defineMiniSkims_R24d.tcl,v 1.2 2009/02/21 01:41:53 homer Exp $
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
# Neutral skims
declareMiniSkim Neutral/NeutralD0toKpipi0    deepCopyMicro  deepCopyMicro
declareMiniSkim Neutral/NeutralDstartoD0pi0  deepCopyMicro  deepCopyMicro

###########################################################################################
# Charmonium AWG skims
declareMiniSkim Charmonium/Jpsitoll            pointer         pointer
declareMiniSkim Charmonium/JpsitollTight       deepCopyMicro   deepCopyMicro

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
declareMiniSkim Charm/DstD0ToKsKPi             deepCopyMicro   deepCopyMicro
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
declareMiniSkim Charm/D0ToK0PiPiSkim           deepCopyMicro   deepCopyMicro
declareMiniSkim Charm/DcTo3PS                  deepCopyMicro   pointer
declareMiniSkim Charm/D0To3PS                  pointer         pointer
declareMiniSkim Charm/DsTo3PS                  deepCopyMicro   pointer
declareMiniSkim Charm/DstToD0PiToPi0Pi0        deepCopyMicro   pointer
declareMiniSkim Charm/D0ToPi0Ks                pointer         pointer
declareMiniSkim Charm/DRecoDmesonTag           pointer         pointer
declareMiniSkim Charm/DRecoDsmesonTag          pointer         pointer
declareMiniSkim Charm/D0ToKnPiPi0              pointer         pointer

###########################################################################################
# IHBD skims
declareMiniSkim IHBD/InclDstar            deepCopyMicro   deepCopyMicro
declareMiniSkim IHBD/InclSemiLept         deepCopyMicro   deepCopyMicro
#declareMiniSkim IHBD/InclDstA1c           deepCopyMicro   deepCopyMicro
declareMiniSkim IHBD/InclDstPi            deepCopyMicro   deepCopyMicro
#declareMiniSkim IHBD/InclD2stPi           deepCopyMicro   deepCopyMicro
#declareMiniSkim IHBD/InclDstRho           deepCopyMicro   deepCopyMicro
#declareMiniSkim IHBD/InclDstRhoComp       deepCopyMicro   deepCopyMicro

###########################################################################################
# ClHBD skims
#
# As of Jan 06 all ClHBD skims stored at RAL must be deepCopy
#
declareMiniSkim ClHBD/BCCKs3body          deepCopyMicro   deepCopyMicro

###########################################################################################
# TauQED skims
declareMiniSkim TauQED/Tau11                   deepCopyMicro   deepCopyMicro

###########################################################################################
# Isr skims
declareMiniSkim Isr/IsrIncExc              pointer         deepCopyMicro
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
