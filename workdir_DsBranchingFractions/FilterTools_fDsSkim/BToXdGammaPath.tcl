
#  Define the BtoXdGamma path
#
#
#


path create BToXdGammaPath

path append BToXdGammaPath FilterBGFMultiHadron
path append BToXdGammaPath BCountMHFilter


### cuts before reconstruction
mod clone BtoRhoGammaBtoXGF BToXdGammaBToXGF
path append  BToXdGammaPath BToXdGammaBToXGF

mod talk BToXdGammaBToXGF 
  activeE1Cut set true
  activeNTrkCut set true
  maxR2All set 0.9
  minNTRK set 2
  minE1 set 1.15
  maxE1 set 3.5
exit



### this is for the list of high energy gammas
mod clone SmpSubListerDefiner GammaFromBToXdGamma
path append BToXdGammaPath GammaFromBToXdGamma

mod talk GammaFromBToXdGamma
  unrefinedListName set "GoodPhotonLoose"
  selectors    set "CmsE 1.15:3.5"
  selectors    set "NumCrystals 4: "
  selectors    set "CalOk 1: "
exit


### this is for the list of daughter pi0s
mod clone SmpSubListerDefiner Pi0FromBToXdGamma
path append BToXdGammaPath Pi0FromBToXdGamma

mod talk Pi0FromBToXdGamma
  unrefinedListName set "pi0DefaultMass"
  selectors   set  "LabP 0.3:"      
exit


### this is for the list of daughter etas
mod clone SmpSubListerDefiner EtaFromBToXdGamma
path append BToXdGammaPath EtaFromBToXdGamma

mod talk EtaFromBToXdGamma
  unrefinedListName set "etaDefault" 
  selectors   set  "LabP 0.3:"      
exit


### this is for the list of daughter pions (charged)
mod clone SmpSubListerDefiner PionFromBToXdGamma
path append BToXdGammaPath PionFromBToXdGamma

mod talk PionFromBToXdGamma
  unrefinedListName set "GoodTracksLoose"
  selectors   set  "LabP 0.3:"     
exit


##Combine neutals and tracks
mod clone SmpMergerDefiner CombinedFromBToXdGamma
path append BToXdGammaPath CombinedFromBToXdGamma

talkto CombinedFromBToXdGamma {
  inputListNames set CalorNeutral
  inputListNames set ChargedTracks
} 



##########################################################
##########
##########  Xdd candidates


sequence create B0ToXddGammaSequence
path append BToXdGammaPath B0ToXddGammaSequence


############  Xdd->pipi  #################################

mod clone SmpMakerDefiner XddToPiPi
sequence append B0ToXddGammaSequence XddToPiPi

mod talk XddToPiPi
  decayMode set "Xdd -> pi+ pi-"
  daughterListNames set "PionFromBToXdGamma"
  daughterListNames set "PionFromBToXdGamma"
  fittingAlgorithm set "Cascade"
  fitConstraints set "Geo"
  fitConstraints set "Momentum"
  postFitSelectors set "ProbChiSq 0.001:"  
  postFitSelectors set "Mass :2.2" 
exit


mod clone SmpMakerDefiner B0ToXddOne
sequence append B0ToXddGammaSequence B0ToXddOne

mod talk B0ToXddOne
  decayMode set "B0 -> Xdd gamma"
  daughterListNames set "XddToPiPi"
  daughterListNames set "GammaFromBToXdGamma"
  evtFinalStateListName set "CombinedFromBToXdGamma"
  fittingAlgorithm set "Add4"
  fitConstraints set "Momentum"
  postFitSelectors set "CosThrust -0.8:0.8" 
  postFitSelectors set "Mes 5.0:5.3" 
  postFitSelectors set "DeltaE -0.3:0.3"
exit
 

############  Xdd->pipipi0  ##############################

mod clone SmpMakerDefiner XddToPiPiPi0
sequence append B0ToXddGammaSequence XddToPiPiPi0

mod talk XddToPiPiPi0
  decayMode set "Xdd -> pi+ pi- pi0"
  daughterListNames set "PionFromBToXdGamma"
  daughterListNames set "PionFromBToXdGamma"
  daughterListNames set "Pi0FromBToXdGamma"
  fittingAlgorithm set "Add4"
  fitConstraints set "Momentum"
  postFitSelectors set "ProbChiSq 0.001:"  
  postFitSelectors set "Mass :2.2"
exit


mod clone SmpMakerDefiner B0ToXddFour
sequence append B0ToXddGammaSequence B0ToXddFour

mod talk B0ToXddFour
  decayMode set "B0 -> Xdd gamma"
  daughterListNames set "XddToPiPiPi0"
  daughterListNames set "GammaFromBToXdGamma"
  evtFinalStateListName set "CombinedFromBToXdGamma"
  fittingAlgorithm set "Add4"
  fitConstraints set "Momentum"
  postFitSelectors set "CosThrust -0.8:0.8"
  postFitSelectors set "Mes 5.0:5.3" 
  postFitSelectors set "DeltaE -0.3:0.3"
exit


############  Xdd->pipipipi  #############################

mod clone SmpMakerDefiner XddToPiPiPiPi
sequence append B0ToXddGammaSequence XddToPiPiPiPi

mod talk XddToPiPiPiPi
  decayMode set "Xdd -> pi+ pi- pi+ pi-"
  daughterListNames set "PionFromBToXdGamma"
  daughterListNames set "PionFromBToXdGamma"
  daughterListNames set "PionFromBToXdGamma"
  daughterListNames set "PionFromBToXdGamma"
  fittingAlgorithm set "Cascade"
  fitConstraints set "Geo"
  fitConstraints set "Momentum"
  postFitSelectors set "ProbChiSq 0.001:"  
  postFitSelectors set "Mass :2.2" 
exit


mod clone SmpMakerDefiner B0ToXddSix
sequence append B0ToXddGammaSequence B0ToXddSix

mod talk B0ToXddSix
  decayMode set "B0 -> Xdd gamma"
  daughterListNames set "XddToPiPiPiPi"
  daughterListNames set "GammaFromBToXdGamma"
  evtFinalStateListName set "CombinedFromBToXdGamma"
  fittingAlgorithm set "Add4"
  fitConstraints set "Momentum"
  postFitSelectors set "CosThrust -0.8:0.8"
  postFitSelectors set "Mes 5.0:5.3" 
  postFitSelectors set "DeltaE -0.3:0.3"
exit


############  Xdd->pipipi0pi0  ###########################

mod clone SmpMakerDefiner XddToPiPiPi0Pi0
sequence append B0ToXddGammaSequence XddToPiPiPi0Pi0

mod talk XddToPiPiPi0Pi0
  decayMode set "Xdd -> pi+ pi- pi0 pi0"
  daughterListNames set "PionFromBToXdGamma"
  daughterListNames set "PionFromBToXdGamma"
  daughterListNames set "Pi0FromBToXdGamma"
  daughterListNames set "Pi0FromBToXdGamma"
  fittingAlgorithm set "Add4"
  fitConstraints set "Momentum"
  postFitSelectors set "ProbChiSq 0.001:"  
  postFitSelectors set "Mass :2.2"
exit


mod clone SmpMakerDefiner B0ToXddEight
sequence append B0ToXddGammaSequence B0ToXddEight

mod talk B0ToXddEight
  decayMode set "B0 -> Xdd gamma"
  daughterListNames set "XddToPiPiPi0Pi0"
  daughterListNames set "GammaFromBToXdGamma"
  evtFinalStateListName set "CombinedFromBToXdGamma"
  fittingAlgorithm set "Add4"
  fitConstraints set "Momentum"
  postFitSelectors set "CosThrust -0.8:0.8"
  postFitSelectors set "Mes 5.0:5.3" 
  postFitSelectors set "DeltaE -0.3:0.3"
exit


##########################################################
####  add the Xdd modules together

mod clone SmpMergerDefiner B0ToXddGamma
sequence append B0ToXddGammaSequence B0ToXddGamma

mod talk B0ToXddGamma
  inputListNames set B0ToXddOne
  inputListNames set B0ToXddFour
  inputListNames set B0ToXddSix
  inputListNames set B0ToXddEight
exit



##########################################################
####
####  Xdu candidates

sequence create BChToXduGammaSequence
path append BToXdGammaPath BChToXduGammaSequence


############  Xdu->pipi0  ################################

mod clone SmpMakerDefiner XduToPiPi0
sequence append BChToXduGammaSequence XduToPiPi0

mod talk XduToPiPi0
  decayMode set "Xdu -> pi+ pi0"
  daughterListNames set "PionFromBToXdGamma"
  daughterListNames set "Pi0FromBToXdGamma"
  fittingAlgorithm set "Add4"
  fitConstraints set "Momentum"
  postFitSelectors set "ProbChiSq 0.001:"
  postFitSelectors set "Mass :2.2"
exit


mod clone SmpMakerDefiner BChToXduTwo
sequence append BChToXduGammaSequence BChToXduTwo

mod talk BChToXduTwo
  decayMode set "B+ -> Xdu gamma"
  daughterListNames set "XduToPiPi0"
  daughterListNames set "GammaFromBToXdGamma"
  evtFinalStateListName set "CombinedFromBToXdGamma"
  fittingAlgorithm set "Add4"
  fitConstraints set "Momentum"
  postFitSelectors set "CosThrust -0.8:0.8"
  postFitSelectors set "Mes 5.0:5.3" 
  postFitSelectors set "DeltaE -0.3:0.3"
exit


############  Xdu->pieta  ################################

mod clone SmpMakerDefiner XduToPiEta
sequence append BChToXduGammaSequence XduToPiEta

mod talk XduToPiEta
  decayMode set "Xdu -> pi+ eta"
  daughterListNames set "PionFromBToXdGamma"
  daughterListNames set "EtaFromBToXdGamma"
  fittingAlgorithm set "Add4"
  fitConstraints set "Momentum"
  postFitSelectors set "ProbChiSq 0.001:"
  postFitSelectors set "Mass :2.2" 
exit


mod clone SmpMakerDefiner BChToXduTen
sequence append BChToXduGammaSequence BChToXduTen

mod talk BChToXduTen
  decayMode set "B+ -> Xdu gamma"
  daughterListNames set "XduToPiEta"
  daughterListNames set "GammaFromBToXdGamma"
  evtFinalStateListName set "CombinedFromBToXdGamma"
  fittingAlgorithm set "Add4"
  fitConstraints set "Momentum"
  postFitSelectors set "CosThrust -0.8:0.8"
  postFitSelectors set "Mes 5.0:5.3" 
  postFitSelectors set "DeltaE -0.3:0.3"
exit


############  Xdu->pipipi  ###############################

mod clone SmpMakerDefiner XduToPiPiPi
sequence append BChToXduGammaSequence XduToPiPiPi

mod talk XduToPiPiPi
  decayMode set "Xdu -> pi+ pi- pi+"
  daughterListNames set "PionFromBToXdGamma"
  daughterListNames set "PionFromBToXdGamma"
  daughterListNames set "PionFromBToXdGamma"
  fittingAlgorithm set "Cascade"
  fitConstraints set "Geo"
  fitConstraints set "Momentum"
  postFitSelectors set "ProbChiSq 0.001:"  
  postFitSelectors set "Mass :2.2" 
exit


mod clone SmpMakerDefiner BChToXduThree
sequence append BChToXduGammaSequence BChToXduThree

mod talk BChToXduThree
  decayMode set "B+ -> Xdu gamma"
  daughterListNames set "XduToPiPiPi"
  daughterListNames set "GammaFromBToXdGamma"
  evtFinalStateListName set "CombinedFromBToXdGamma"
  fittingAlgorithm set "Add4"
  fitConstraints set "Momentum"
  postFitSelectors set "CosThrust -0.8:0.8"
  postFitSelectors set "Mes 5.0:5.3" 
  postFitSelectors set "DeltaE -0.3:0.3"
exit


############  Xdu->pipi0pi0  #############################

mod clone SmpMakerDefiner XduToPiPi0Pi0
sequence append BChToXduGammaSequence XduToPiPi0Pi0

mod talk XduToPiPi0Pi0
  decayMode set "Xdu -> pi+ pi0 pi0"
  daughterListNames set "PionFromBToXdGamma"
  daughterListNames set "Pi0FromBToXdGamma"
  daughterListNames set "Pi0FromBToXdGamma"
  fittingAlgorithm set "Add4"
  fitConstraints set "Momentum"
  postFitSelectors set "ProbChiSq 0.001:"  
  postFitSelectors set "Mass :2.2"
exit


mod clone SmpMakerDefiner BChToXduFive
sequence append BChToXduGammaSequence BChToXduFive

mod talk BChToXduFive
  decayMode set "B+ -> Xdu gamma"
  daughterListNames set "XduToPiPi0Pi0"
  daughterListNames set "GammaFromBToXdGamma"
  evtFinalStateListName set "CombinedFromBToXdGamma"
  fittingAlgorithm set "Add4"
  fitConstraints set "Momentum"
  postFitSelectors set "CosThrust -0.8:0.8"
  postFitSelectors set "Mes 5.0:5.3" 
  postFitSelectors set "DeltaE -0.3:0.3"
exit



############  Xdu->pipipipi0  ############################

mod clone SmpMakerDefiner XduToPiPiPiPi0
sequence append BChToXduGammaSequence XduToPiPiPiPi0

mod talk XduToPiPiPiPi0
  decayMode set "Xdu -> pi+ pi- pi+ pi0"
  daughterListNames set "PionFromBToXdGamma"
  daughterListNames set "PionFromBToXdGamma"
  daughterListNames set "PionFromBToXdGamma"
  daughterListNames set "Pi0FromBToXdGamma"
  fittingAlgorithm set "Add4"
  fitConstraints set "Momentum"
  postFitSelectors set "ProbChiSq 0.001:"  
  postFitSelectors set "Mass :2.2"
exit


mod clone SmpMakerDefiner BChToXduSeven
sequence append BChToXduGammaSequence BChToXduSeven

mod talk BChToXduSeven
  decayMode set "B+ -> Xdu gamma"
  daughterListNames set "XduToPiPiPiPi0"
  daughterListNames set "GammaFromBToXdGamma"
  evtFinalStateListName set "CombinedFromBToXdGamma"
  fittingAlgorithm set "Add4"
  fitConstraints set "Momentum"
  postFitSelectors set "CosThrust -0.8:0.8"
  postFitSelectors set "Mes 5.0:5.3" 
  postFitSelectors set "DeltaE -0.3:0.3"
exit


##########################################################
####  add the Xdu modules together

mod clone SmpMergerDefiner BChToXduGamma
sequence append BChToXduGammaSequence BChToXduGamma

mod talk BChToXduGamma
  inputListNames set BChToXduTwo
  inputListNames set BChToXduThree
  inputListNames set BChToXduFive
  inputListNames set BChToXduSeven
  inputListNames set BChToXduTen
exit


##########################################################
####  add the B0 and Bch

mod clone SmpMergerDefiner BToXdGamma
path append BToXdGammaPath BToXdGamma

mod talk BToXdGamma
  inputListNames set B0ToXddGamma
  inputListNames set BChToXduGamma
exit


##########################################################
####  set tag

mod clone TagFromList TagBToXdGamma

mod talk TagBToXdGamma
  listsToTag set BToXdGamma
  tagName set BToXdGamma
exit


path append BToXdGammaPath TagBToXdGamma

mod clone TagFilterByName BToXdGammaFilter
path append BToXdGammaPath BToXdGammaFilter

talkto BToXdGammaFilter {
    orList set BToXdGamma
}




