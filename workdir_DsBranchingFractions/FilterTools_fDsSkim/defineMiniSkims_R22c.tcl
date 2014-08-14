# $Id: defineMiniSkims_R22c.tcl,v 1.1 2007/05/09 23:19:39 homer Exp $
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
# PID skims
declareMiniSkim PID/BPCBhabha              deepCopyMicro   deepCopyMicro
declareMiniSkim PID/BPCKaon                deepCopyMicro   deepCopyMicro
declareMiniSkim PID/BPCMuon                deepCopyMicro   deepCopyMicro
declareMiniSkim PID/BPCProton              deepCopyMicro   deepCopyMicro
declareMiniSkim PID/BPCPion                pointer         pointer

###########################################################################################
# Breco skims (09/05/2003)
declareMiniSkim Breco/BSemiExcl            deepCopyMicro   deepCopyMicro

###########################################################################################
# sin2beta/mixing skims
# sin2b (Breco) skims
declareMiniSkim S2BMix/BFlav_Final        deepCopyMini    deepCopyMicro
declareMiniSkim S2BMix/BchFlav_Final      deepCopyMini    deepCopyMicro
declareMiniSkim S2BMix/B0ToccKstarFinal   deepCopyMini    deepCopyMicro
declareMiniSkim S2BMix/BchToccKstarFinal  deepCopyMini    deepCopyMicro

###########################################################################################
# Charmonium AWG skims
declareMiniSkim Charmonium/Jpsitoll            pointer         pointer

###########################################################################################
# XSL skims
declareMiniSkim XSL/BToDlnu               pointer         pointer

###########################################################################################
# ClHBD skims
#
# As of Jan 06 all ClHBD skims stored at RAL must be deepCopy
#

# Quasi Two Body Stuff
declareMiniSkim ClHBD/InclPhi             deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BFourHHHH           deepCopyMicro   deepCopyMicro
# multi-list 3-body combinations
declareMiniSkim ClHBD/BToCXX              deepCopyMicro   deepCopyMicro
declareMiniSkim ClHBD/BToCPP              deepCopyMicro   deepCopyMicro
# multi-list 2-body combinations
declareMiniSkim ClHBD/BToPP               deepCopyMicro   deepCopyMicro
# classic charmless 3-body
declareMiniSkim ClHBD/BCCKs3body          deepCopyMicro   deepCopyMicro

###########################################################################################
# RadPenguin skims
declareMiniSkim RadPenguin/BtoRhoGamma         deepCopyMicro   deepCopyMicro
declareMiniSkim RadPenguin/B0toKsPi0Gamma      deepCopyMicro   deepCopyMicro {B0toKsPi0Gamma}  {BGFMultiHadron BGFNeutralHadron}	       
declareMiniSkim RadPenguin/B0toKPiGamma        deepCopyMicro   deepCopyMicro {B0toKPiGamma B0toPiPiGamma} {BGFMultiHadron BGFNeutralHadron}
declareMiniSkim RadPenguin/BtoKPi0Gamma        deepCopyMicro   deepCopyMicro {BtoKPi0Gamma BtoPiPi0Gamma} {BGFMultiHadron BGFNeutralHadron}
declareMiniSkim RadPenguin/BtoKsPiGamma        deepCopyMicro   deepCopyMicro {BtoKsPiGamma}    {BGFMultiHadron BGFNeutralHadron}	       
declareMiniSkim RadPenguin/B0toKsEtaGamma      deepCopyMicro   deepCopyMicro {B0toKsEtaGamma}  {BGFMultiHadron BGFNeutralHadron}	       
declareMiniSkim RadPenguin/BtoKEtaGamma        deepCopyMicro   deepCopyMicro {BtoKEtaGamma}    {BGFMultiHadron BGFNeutralHadron}	       
declareMiniSkim RadPenguin/B0toKsEtaPGamma     deepCopyMicro   deepCopyMicro {B0toKsEtaPGamma} {BGFMultiHadron BGFNeutralHadron}	       
declareMiniSkim RadPenguin/BtoKEtaPGamma       deepCopyMicro   deepCopyMicro {BtoKEtaPGamma}   {BGFMultiHadron BGFNeutralHadron}	       

###########################################################################################
# TauQED skims
declareMiniSkim TauQED/Tau11                   deepCopyMicro   deepCopyMicro
declareMiniSkim TauQED/Tau1N                   deepCopyMicro   deepCopyMicro  

