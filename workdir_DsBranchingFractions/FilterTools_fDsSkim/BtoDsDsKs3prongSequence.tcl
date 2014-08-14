sequence create BtoDsDsKs3prongSequence
#global BtaCandLists

set ds_list Ds3prong

mod clone SmpMergerDefiner Ds3prong
mod talk Ds3prong
  inputListNames set  DsToPhiPiDefault
  inputListNames set  DsToKsKDefault
  inputListNames set  DsToKstarKDefault
  #verbose set true
exit
sequence append BtoDsDsKs3prongSequence Ds3prong

mod clone SmpMakerDefiner BtoDsDsKs3prongAdd4
talkto BtoDsDsKs3prongAdd4 {
    decayMode             set "B0 -> D_s+ D_s- K_S0"
    daughterListNames     set $ds_list
    daughterListNames     set $ds_list
    daughterListNames     set "KsDefault"
    fittingAlgorithm      set "Add4"
    postFitSelectors      set "Mes 5.23:"
}
sequence append BtoDsDsKs3prongSequence BtoDsDsKs3prongAdd4
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
sequence append BtoDsDsKs3prongSequence BtoDsDsKC3prongAdd4
#lappend BtaCandLists BtoDsDsKC3prongAdd4

mod clone SmpMakerDefiner Dsstar3prong
mod talk Dsstar3prong
    decayMode             set "D_s*+ -> D_s+ gamma"
    daughterListNames     set $ds_list
    #daughterListNames     set "GoodPhotonLoose"
    daughterListNames     set "GoodPhotonDefault"
    fittingAlgorithm      set "Add4"
exit
sequence append BtoDsDsKs3prongSequence Dsstar3prong

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
sequence append BtoDsDsKs3prongSequence BtoDsstarDsstarKs3prongAdd4
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
sequence append BtoDsDsKs3prongSequence BtoDsstarDsstarKC3prongAdd4
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
sequence append BtoDsDsKs3prongSequence BtoDsstarDsKs3prongAdd4
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
sequence append BtoDsDsKs3prongSequence BtoDsstarDsKC3prongAdd4
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
sequence append BtoDsDsKs3prongSequence DsDsK3prongBList
