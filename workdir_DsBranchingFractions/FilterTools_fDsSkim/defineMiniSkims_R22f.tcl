# $Id: defineMiniSkims_R22f.tcl,v 1.1 2008/05/08 22:05:47 prencipe Exp $
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

declareMiniSkim Charm/D0Toll           deepCopyMicro   deepCopyMicro
declareMiniSkim Breco/BSemiExcl        deepCopyMicro   deepCopyMicro
declareMiniSkim Breco/BSemiExclAdd     pointer         pointer 
##########################################################

