sourceFoundFile DRecoilToolsUser/DsInclusiveReco_Configuration.tcl

#module talk DRecoDsToEllNuFit
# FitProbCut set .00001
# ExGamEnergyCutHi set 0.1
#exit
#sequence append DRecoSequence DRecoDsToEllNuFit

#mod clone DRecoCounter CounterDsToEllNuFit
#module talk CounterDsToEllNuFit
#  counterNumber set 80
#exit
#sequence append DRecoSequence CounterDsToEllNuFit

sequence append DRecoSequence DRecoDsToKKsFit

