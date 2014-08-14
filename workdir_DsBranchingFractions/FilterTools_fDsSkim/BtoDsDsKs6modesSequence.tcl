sequence create BtoDsDsKs6modesSequence
#global BtaCandLists

set ds_list Ds6modes

mod clone SmpMergerDefiner Ds6modes
mod talk Ds6modes
  inputListNames set  DsToPhiPiDefault
  inputListNames set  DsToKsKDefault
  inputListNames set  DsToKstarKDefault
  inputListNames set  DsToKsKPiPisupDefault
  inputListNames set  DsToKsKPiPifavDefault
  inputListNames set  DsToPhiRhoCDefault
  #verbose set true
exit
sequence append BtoDsDsKs6modesSequence Ds6modes

mod clone SmpMakerDefiner BtoDsDsKs6modesAdd4
talkto BtoDsDsKs6modesAdd4 {
    decayMode             set "B0 -> D_s+ D_s- K_S0"
    daughterListNames     set $ds_list
    daughterListNames     set $ds_list
    daughterListNames     set "KsDefault"
    fittingAlgorithm      set "Add4"
    postFitSelectors      set "Mes 5.23:"
}
sequence append BtoDsDsKs6modesSequence BtoDsDsKs6modesAdd4
#lappend BtaCandLists BtoDsDsKs6modesAdd4

mod clone SmpMakerDefiner BtoDsDsKC6modesAdd4
talkto BtoDsDsKC6modesAdd4 {
    decayMode             set "B+ -> D_s+ D_s- K+"
    daughterListNames     set $ds_list
    daughterListNames     set $ds_list
    daughterListNames     set "KLHNotPionGTL"
    fittingAlgorithm      set "Add4"
    postFitSelectors      set "Mes 5.23:"
}
sequence append BtoDsDsKs6modesSequence BtoDsDsKC6modesAdd4
#lappend BtaCandLists BtoDsDsKC6modesAdd4

mod clone SmpMakerDefiner Dsstar6modes
mod talk Dsstar6modes
    decayMode             set "D_s*+ -> D_s+ gamma"
    daughterListNames     set $ds_list
    #daughterListNames     set "GoodPhotonLoose"
    daughterListNames     set "GoodPhotonDefault"
    fittingAlgorithm      set "Add4"
exit
sequence append BtoDsDsKs6modesSequence Dsstar6modes

#################
# B -> Ds* Ds* K
#################
mod clone SmpMakerDefiner BtoDsstarDsstarKs6modesAdd4
talkto BtoDsstarDsstarKs6modesAdd4 {
    decayMode             set "B0 -> D_s*+ D_s*- K_S0"
    daughterListNames     set "Dsstar6modes"
    daughterListNames     set "Dsstar6modes"
    daughterListNames     set "KsDefault"
    fittingAlgorithm      set "Add4"
    postFitSelectors      set "Mes 5.23:"
}
sequence append BtoDsDsKs6modesSequence BtoDsstarDsstarKs6modesAdd4
#lappend BtaCandLists BtoDsstarDsstarKs6modesAdd4

mod clone SmpMakerDefiner BtoDsstarDsstarKC6modesAdd4
talkto BtoDsstarDsstarKC6modesAdd4 {
    decayMode             set "B+ -> D_s*+ D_s*- K+"
    daughterListNames     set "Dsstar6modes"
    daughterListNames     set "Dsstar6modes"
    daughterListNames     set "KLHNotPionGTL"
    fittingAlgorithm      set "Add4"
    postFitSelectors      set "Mes 5.23:"
}
sequence append BtoDsDsKs6modesSequence BtoDsstarDsstarKC6modesAdd4
#lappend BtaCandLists BtoDsstarDsstarKC6modesAdd4

#################
# B -> Ds*+ Ds- K
#################
mod clone SmpMakerDefiner BtoDsstarDsKs6modesAdd4
talkto BtoDsstarDsKs6modesAdd4 {
    decayMode             set "B0 -> D_s*+ D_s- K_S0"
    daughterListNames     set "Dsstar6modes"
    daughterListNames     set $ds_list
    daughterListNames     set "KsDefault"
    fittingAlgorithm      set "Add4"
    postFitSelectors      set "Mes 5.23:"
}
sequence append BtoDsDsKs6modesSequence BtoDsstarDsKs6modesAdd4
#lappend BtaCandLists BtoDsstarDsKs6modesAdd4

mod clone SmpMakerDefiner BtoDsstarDsKC6modesAdd4
talkto BtoDsstarDsKC6modesAdd4 {
    decayMode             set "B+ -> D_s*+ D_s- K+"
    daughterListNames     set "Dsstar6modes"
    daughterListNames     set $ds_list
    daughterListNames     set "KLHNotPionGTL"
    fittingAlgorithm      set "Add4"
    postFitSelectors      set "Mes 5.23:"
}
sequence append BtoDsDsKs6modesSequence BtoDsstarDsKC6modesAdd4
#lappend BtaCandLists BtoDsstarDsKC6modesAdd4

mod clone SmpMergerDefiner DsDsK6modesBList
mod talk DsDsK6modesBList
  inputListNames set  BtoDsDsKs6modesAdd4
  inputListNames set  BtoDsstarDsKs6modesAdd4
  inputListNames set  BtoDsstarDsstarKs6modesAdd4
  inputListNames set  BtoDsDsKC6modesAdd4
  inputListNames set  BtoDsstarDsKC6modesAdd4
  inputListNames set  BtoDsstarDsstarKC6modesAdd4
  #verbose set true
exit
sequence append BtoDsDsKs6modesSequence DsDsK6modesBList
