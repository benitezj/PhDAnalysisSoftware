#$Id: BtoDsDsK3prongPath.tcl,v 1.2 2005/12/02 02:40:13 fwinkl Exp $

path create BtoDsDsK3prongPath
#global BtaCandLists

set ds_list Ds3prong

mod clone SmpMergerDefiner Ds3prong
mod talk Ds3prong
  inputListNames set  DsToPhiPiDefault
  inputListNames set  DsToKsKDefault
  inputListNames set  DsToKstarKDefault
  #verbose set true
exit
path append BtoDsDsK3prongPath Ds3prong

mod clone SmpMakerDefiner BtoDsDsKs3prongAdd4
talkto BtoDsDsKs3prongAdd4 {
    decayMode             set "B0 -> D_s+ D_s- K_S0"
    daughterListNames     set $ds_list
    daughterListNames     set $ds_list
    daughterListNames     set "KsDefault"
    fittingAlgorithm      set "Add4"
    postFitSelectors      set "Mes 5.23:"
}
path append BtoDsDsK3prongPath BtoDsDsKs3prongAdd4
#lappend BtaCandLists BtoDsDsKs3prongAdd4

mod clone SmpMakerDefiner BtoDsDsKC3prongAdd4
talkto BtoDsDsKC3prongAdd4 {
    decayMode             set "B+ -> D_s+ D_s- K+"
    daughterListNames     set $ds_list
    daughterListNames     set $ds_list
    daughterListNames     set "KLHNotPionGTL"
    fittingAlgorithm      set "Add4"
    postFitSelectors      set "Mes 5.23:"
}
path append BtoDsDsK3prongPath BtoDsDsKC3prongAdd4
#lappend BtaCandLists BtoDsDsKC3prongAdd4

mod clone SmpMakerDefiner Dsstar3prong
mod talk Dsstar3prong
    decayMode             set "D_s*+ -> D_s+ gamma"
    daughterListNames     set $ds_list
    #daughterListNames     set "GoodPhotonLoose"
    daughterListNames     set "GoodPhotonDefault"
    fittingAlgorithm      set "Add4"
exit
path append BtoDsDsK3prongPath Dsstar3prong

#################
# B -> Ds* Ds* K
#################
mod clone SmpMakerDefiner BtoDsstarDsstarKs3prongAdd4
talkto BtoDsstarDsstarKs3prongAdd4 {
    decayMode             set "B0 -> D_s*+ D_s*- K_S0"
    daughterListNames     set "Dsstar3prong"
    daughterListNames     set "Dsstar3prong"
    daughterListNames     set "KsDefault"
    fittingAlgorithm      set "Add4"
    postFitSelectors      set "Mes 5.23:"
}
path append BtoDsDsK3prongPath BtoDsstarDsstarKs3prongAdd4
#lappend BtaCandLists BtoDsstarDsstarKs3prongAdd4

mod clone SmpMakerDefiner BtoDsstarDsstarKC3prongAdd4
talkto BtoDsstarDsstarKC3prongAdd4 {
    decayMode             set "B+ -> D_s*+ D_s*- K+"
    daughterListNames     set "Dsstar3prong"
    daughterListNames     set "Dsstar3prong"
    daughterListNames     set "KLHNotPionGTL"
    fittingAlgorithm      set "Add4"
    postFitSelectors      set "Mes 5.23:"
}
path append BtoDsDsK3prongPath BtoDsstarDsstarKC3prongAdd4
#lappend BtaCandLists BtoDsstarDsstarKC3prongAdd4

#################
# B -> Ds*+ Ds- K
#################
mod clone SmpMakerDefiner BtoDsstarDsKs3prongAdd4
talkto BtoDsstarDsKs3prongAdd4 {
    decayMode             set "B0 -> D_s*+ D_s- K_S0"
    daughterListNames     set "Dsstar3prong"
    daughterListNames     set $ds_list
    daughterListNames     set "KsDefault"
    fittingAlgorithm      set "Add4"
    postFitSelectors      set "Mes 5.23:"
}
path append BtoDsDsK3prongPath BtoDsstarDsKs3prongAdd4
#lappend BtaCandLists BtoDsstarDsKs3prongAdd4

mod clone SmpMakerDefiner BtoDsstarDsKC3prongAdd4
talkto BtoDsstarDsKC3prongAdd4 {
    decayMode             set "B+ -> D_s*+ D_s- K+"
    daughterListNames     set "Dsstar3prong"
    daughterListNames     set $ds_list
    daughterListNames     set "KLHNotPionGTL"
    fittingAlgorithm      set "Add4"
    postFitSelectors      set "Mes 5.23:"
}
path append BtoDsDsK3prongPath BtoDsstarDsKC3prongAdd4
#lappend BtaCandLists BtoDsstarDsKC3prongAdd4

mod clone SmpMergerDefiner DsDsK3prongBList
mod talk DsDsK3prongBList
  inputListNames set  BtoDsDsKs3prongAdd4
  inputListNames set  BtoDsstarDsKs3prongAdd4
  inputListNames set  BtoDsstarDsstarKs3prongAdd4
  inputListNames set  BtoDsDsKC3prongAdd4
  inputListNames set  BtoDsstarDsKC3prongAdd4
  inputListNames set  BtoDsstarDsstarKC3prongAdd4
  #verbose set true
exit
path append BtoDsDsK3prongPath DsDsK3prongBList

# Filter on the list
mod clone FilterByList BtoDsDsK3prongFilter
talkto BtoDsDsK3prongFilter {
  orList set DsDsK3prongBList
}

path append BtoDsDsK3prongPath BtoDsDsK3prongFilter