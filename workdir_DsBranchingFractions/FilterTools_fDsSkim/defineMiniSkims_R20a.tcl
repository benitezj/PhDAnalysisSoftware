# $Id: defineMiniSkims_R20a.tcl,v 1.3 2006/09/17 07:12:07 kflood Exp $
#
# this file defines paths for SkimMiniApp for TrkFixup tests
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
declareMiniSkim PID/BPCPion                deepCopyMicro   deepCopyMicro
declareMiniSkim PID/DcToKPiPiPromptNoPid   deepCopyMicro   deepCopyMicro
declareMiniSkim PID/LambdacToPKsNoPid      deepCopyMicro   deepCopyMicro

###########################################################################################
# Neutral skims
declareMiniSkim Neutral/NeutralD0toKpipi0    deepCopyMicro   deepCopyMicro
declareMiniSkim Neutral/NeutralDstartoD0pi0  deepCopyMicro   deepCopyMicro

###########################################################################################
# Breco skims (09/05/2003)
declareMiniSkim Breco/BSemiExcl            deepCopyMicro   deepCopyMicro

###########################################################################################
# sin2beta/mixing skims
# sin2b (Breco) skims
declareMiniSkim S2BMix/BFlav_Loose        deepCopyMicro   deepCopyMicro
declareMiniSkim S2BMix/BFlav_Final        deepCopyMicro   deepCopyMicro
declareMiniSkim S2BMix/B0ToccKFinal       deepCopyMini    deepCopyMicro

###########################################################################################
# Charmonium AWG skims
declareMiniSkim Charmonium/Jpsitoll       deepCopyMicro   deepCopyMicro

###########################################################################################
# Charm AWG skims
#
declareMiniSkim Charm/LambdaVeryVeryLoose deepCopyMicro   deepCopyMicro
declareMiniSkim Charm/XiMinus             deepCopyMicro   deepCopyMicro
declareMiniSkim Charm/OmegaMinus          deepCopyMicro   deepCopyMicro


###########################################################################################
# IHBD skims

###########################################################################################
# XSL skims
declareMiniSkim XSL/BToDlnu               deepCopyMicro   deepCopyMicro

###########################################################################################
# ClHBD skims
#
# As of Jan 06 all ClHBD skims stored at RAL must be deepCopy
#

# Quasi Two Body Stuff

# classic charmless 3-body

# multi-list 3-body combinations
declareMiniSkim ClHBD/BToPPP              deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BToPP               deepCopyMicro   deepCopyMicro

###########################################################################################
# RadPenguin skims

###########################################################################################
# TauQED skims
declareMiniSkim TauQED/Tau11                   deepCopyMicro   deepCopyMicro
declareMiniSkim TauQED/Tau1N                   deepCopyMicro   deepCopyMicro
declareMiniSkim TauQED/Tau33                   deepCopyMicro   deepCopyMicro

###########################################################################################
# Isr skims

###########################################################################################
# B-Counting skims

###########################################################################################
# Skims used for CalibStream in Elf that are not already defined  (ShR 6 Jan 2004)

# Skim for random background trigger

###########################################################################################
# DsToMuNu skim

###########################################################################################
# IHPS skims

