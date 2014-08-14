# $Id: defineMiniSkims_R16c.tcl,v 1.3 2005/06/15 19:24:32 fnc Exp $
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
declareMiniSkim AllEvents             KanGA/AllEvents           tagOnly         tagOnly
declareMiniSkim XSLBtoXulnuFilter     XSL/XSLBtoXulnuFilter     deepCopyMicro   deepCopyMicro
declareMiniSkim HardElectronFilter    XSL/HardElectronFilter    pointer         pointer
declareMiniSkim InclProtPiPiKsK       PTF/InclProtPiPiKsK       deepCopyMicro   deepCopyMicro
declareMiniSkim IsrGamma              Isr/IsrGamma              deepCopyMicro   deepCopyMicro
declareMiniSkim IsrNoGamma            Isr/IsrNoGamma            pointer         pointer
declareMiniSkim EEToKKEta             IHPS/EEToKKEta            deepCopyMicro   deepCopyMicro {EEToKKEtaLoose EEToKKEta EEToPhiEta}
