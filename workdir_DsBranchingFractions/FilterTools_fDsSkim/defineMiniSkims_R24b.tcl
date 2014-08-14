# $Id: defineMiniSkims_R24b.tcl,v 1.4 2008/06/12 21:51:55 homer Exp $
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
# Tracking Efficiency skim
declareMiniSkim Tracking/TrkEffTau31       deepCopyMicro   deepCopyMicro

###########################################################################################
# Neutral skims
declareMiniSkim Neutral/NeutralD0toKpipi0    deepCopyMicro  deepCopyMicro
declareMiniSkim Neutral/NeutralDstartoD0pi0  deepCopyMicro  deepCopyMicro

###########################################################################################
# Charmonium AWG skims
declareMiniSkim Charmonium/JpsitollTight     deepCopyMicro   deepCopyMicro


###########################################################################################
# ClHBD skims
#
declareMiniSkim ClHBD/BCCKs3body           deepCopyMicro   deepCopyMicro

###########################################################################################
# TauQED skims
declareMiniSkim TauQED/Tau11               deepCopyMicro   deepCopyMicro

###########################################################################################
# Isr skims
declareMiniSkim Isr/IsrIncExc              pointer         deepCopyMicro
