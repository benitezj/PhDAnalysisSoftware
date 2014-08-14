# $Id: defineMiniSkims_R22e.tcl,v 1.6 2007/10/09 08:23:49 marchior Exp $
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
# Breco skims
declareMiniSkim Breco/BtoDPiPiPi           deepCopyMicro   deepCopyMicro

###########################################################################################
# sin2beta/mixing skims
# sin2b (Breco) skims
declareMiniSkim S2BMix/B0ToDDstar0h0      deepCopyMicro   deepCopyMicro

###########################################################################################
# Charm skims
declareMiniSkim Charm/D0ToK0PiPiSkim           deepCopyMicro   deepCopyMicro
declareMiniSkim Charm/DcTo3PS                  deepCopyMicro   pointer
declareMiniSkim Charm/D0To3PS                  pointer         pointer
declareMiniSkim Charm/DsTo3PS                  deepCopyMicro   pointer
declareMiniSkim Charm/DstToD0PiToPi0Pi0        deepCopyMicro   pointer
declareMiniSkim Charm/D0ToPi0Ks                pointer         pointer

###########################################################################################
# XSL skims
declareMiniSkim XSL/XSLBtoXulnuFilter     deepCopyMicro   deepCopyMicro

###########################################################################################
# ClHBD skims
#
# As of Jan 06 all ClHBD skims stored at RAL must be deepCopy
#
declareMiniSkim ClHBD/BToPhi3K            deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BToDsD              deepCopyMicro   deepCopyMicro
# multi-list 4-body combinations
declareMiniSkim ClHBD/BToEtapPiPih        deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BToOmegaPiPih       deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BToPhiPiPih         deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BToEtaPiPih         deepCopyMicro   deepCopyMicro

###########################################################################################
# RadPenguin skims
declareMiniSkim RadPenguin/BToXsll             deepCopyMicro   deepCopyMicro
declareMiniSkim RadPenguin/BToXsll_peaking     deepCopyMicro   deepCopyMicro
declareMiniSkim RadPenguin/BToXsllBG           tagOnly         tagOnly

###########################################################################################
# Isr skims
declareMiniSkim Isr/Jpsi_Psi2sToKsKsGamma  deepCopyMicro   deepCopyMicro
declareMiniSkim Isr/Jpsi_Psi2sToChChGamma  deepCopyMicro   deepCopyMicro
