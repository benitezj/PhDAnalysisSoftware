#
# $Id: B0DNeutralLightPath.tcl,v 1.4 2005/11/16 23:01:54 fwinkl Exp $
#
# defines B0DNeutralLightPath filter path
#

## for writing composite candidates
global BtaCandLists
global writeUsrDataForSkim
global UsrBCandLists

path create B0DNeutralLightPath

# only run on MultiHadrons
path append B0DNeutralLightPath FilterBGFMultiHadron

sourceFoundFile EventTagTools/TagProcs.tcl
# creates tag bits charmNames+lightNames
addTagModule TagDLight B0DNeutralLightPath {
  lightInputLists  set pi0AllDefault
  lightNames       set pi0
  lightInputLists  set rho0Default
  lightNames       set rho0
  lightInputLists  set etaDefault
  lightNames       set eta
  lightInputLists  set etaPDefault
  lightNames       set etaPrime
  lightInputLists  set omegaDefault
  lightNames       set omega
  charmInputLists  set D0ChrgKDefault
  charmNames       set D0
  charmInputLists  set Dstar0ChrgKDefault
  charmNames       set Dstar0
  pstarLightCut    set 1.3
  pstarCharmCutMax set 10.0
}

# and now filter
mod clone TagFilterByName B0DNeutralLightFilter
path append B0DNeutralLightPath B0DNeutralLightFilter
talkto B0DNeutralLightFilter { 
    orList set D0pi0
    orList set D0rho0
    orList set D0eta
    orList set D0etaPrime
    orList set D0omega
    orList set Dstar0pi0
    orList set Dstar0rho0
    orList set Dstar0eta
    orList set Dstar0etaPrime
    orList set Dstar0omega
}

# Add lists of composites to write
lappend BtaCandLists B0ToD0pi0 B0ToD0rho0 B0ToD0eta B0ToD0etaPrime B0ToD0omega B0ToDstar0pi0 B0ToDstar0rho0 B0ToDstar0eta B0ToDstar0etaPrime B0ToDstar0omega  

# Add lists of composites with UsrData
set UsrBCandLists $BtaCandLists
# Write UsrData
set writeUsrDataForSkim 1
