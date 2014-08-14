#$Id: BtoDsDsK6modesPath.tcl,v 1.2 2005/12/02 02:40:14 fwinkl Exp $

path create BtoDsDsK6modesPath
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
path append BtoDsDsK6modesPath Ds6modes

mod clone SmpMakerDefiner BtoDsDsKs6modesAdd4
talkto BtoDsDsKs6modesAdd4 {
    decayMode             set "B0 -> D_s+ D_s- K_S0"
    daughterListNames     set $ds_list
    daughterListNames     set $ds_list
    daughterListNames     set "KsDefault"
    fittingAlgorithm      set "Add4"
    postFitSelectors      set "Mes 5.23:"
}
path append BtoDsDsK6modesPath BtoDsDsKs6modesAdd4
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
path append BtoDsDsK6modesPath BtoDsDsKC6modesAdd4
#lappend BtaCandLists BtoDsDsKC6modesAdd4

mod clone SmpMakerDefiner Dsstar6modes
mod talk Dsstar6modes
    decayMode             set "D_s*+ -> D_s+ gamma"
    daughterListNames     set $ds_list
    #daughterListNames     set "GoodPhotonLoose"
    daughterListNames     set "GoodPhotonDefault"
    fittingAlgorithm      set "Add4"
exit
path append BtoDsDsK6modesPath Dsstar6modes

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
path append BtoDsDsK6modesPath BtoDsstarDsstarKs6modesAdd4
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
path append BtoDsDsK6modesPath BtoDsstarDsstarKC6modesAdd4
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
path append BtoDsDsK6modesPath BtoDsstarDsKs6modesAdd4
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
path append BtoDsDsK6modesPath BtoDsstarDsKC6modesAdd4
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
path append BtoDsDsK6modesPath DsDsK6modesBList


# Filter on the list
mod clone FilterByList BtoDsDsK6modesFilter
talkto BtoDsDsK6modesFilter {
  orList set DsDsK6modesBList
}

path append BtoDsDsK6modesPath BtoDsDsK6modesFilter

