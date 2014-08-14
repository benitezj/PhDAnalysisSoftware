# $Id: defineMiniSkims_R18d.tcl,v 1.2 2006/06/01 00:29:27 fwinkl Exp $
#
# this file defines paths for the R18d skim cycle
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

declareMiniSkim KanGA/AllEvents                tagOnly         tagOnly

declareMiniSkim Charm/XiMinus                  deepCopyMicro   deepCopyMicro
declareMiniSkim Charm/OmegaMinus               deepCopyMicro   deepCopyMicro
declareMiniSkim Charm/DsstToDsGammaToVGamma    pointer         pointer
declareMiniSkim Charm/DstToDPiToVGamma         pointer         pointer

declareMiniSkim Isr/IsrIncExc                  pointer         pointer

declareMiniSkim RadPenguin/BToXsll             deepCopyMicro   deepCopyMicro
declareMiniSkim RadPenguin/BToXsll_peaking     deepCopyMicro   deepCopyMicro

###########################################################################################
# B-Counting skims
declareMiniSkim BC/BCountMHFilter        noOutput        noOutput
declareMiniSkim BC/BCountMuMuFilter      noOutput        noOutput
