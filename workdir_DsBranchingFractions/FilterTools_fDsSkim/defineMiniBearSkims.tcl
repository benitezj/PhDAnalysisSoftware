# $Id: defineMiniBearSkims.tcl,v 1.2 2005/06/12 05:04:06 lange Exp $
#
# This file defines the skim paths for Elf/Moose
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
declareMiniSkim KanGA/AllEvents           tagOnly         tagOnly

###########################################################################################
# Tracking/calibration skim
declareMiniSkim Tracking/AlignCal         noOutput        noOutput
declareMiniSkim Tracking/PreVeto          deepCopyMicro   deepCopyMicro

###########################################################################################
# PID skims
declareMiniSkim PID/BPCBhabha             deepCopyMicro   deepCopyMicro
declareMiniSkim PID/BPCKaon               deepCopyMicro   deepCopyMicro
declareMiniSkim PID/BPCKLong              deepCopyMicro   deepCopyMicro
declareMiniSkim PID/BPCMuon               deepCopyMicro   deepCopyMicro
declareMiniSkim PID/BPCProton             deepCopyMicro   deepCopyMicro
declareMiniSkim PID/BPCElectron           deepCopyMicro   deepCopyMicro
declareMiniSkim PID/BPCPion               deepCopyMicro   pointer

# sin2b (Breco) skims
declareMiniSkim S2BMix/BFlav_Final        deepCopyMini    deepCopyMicro

# Skims used for CalibStream in Elf that are not already defined  (ShR 6 Jan 2004)
declareMiniSkim  Lumi/LumiTag        noOutput        noOutput
declareMiniSkim  TauQED/Prescale           noOutput        noOutput
declareMiniSkim  Trigger/Damage     noOutput        noOutput
declareMiniSkim  EMC/VCS                   noOutput        noOutput
declareMiniSkim  Tracking/L3Cosmic      noOutput        noOutput
# Skim for random background trigger
declareMiniSkim  Trigger/RandomBackground noOutput       noOutput
