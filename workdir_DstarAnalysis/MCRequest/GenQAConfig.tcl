# Setup the path:
source PARENT/GeneratorsQA/EvtGen.tcl

module talk GfiEvtGen
 #UDECAY set "../MyProdDecayFiles/ccbar_D2star+_D0pi+_K-pi+.dec"
 #UDECAY set "../MyProdDecayFiles/ccbar_D2star0_D0pi0_K-pi+.dec"
 #UDECAY set "../MyProdDecayFiles/ccbar_D2star+_Dstar0pi+_D0pi0_K-pi+.dec"
 UDECAY set "../MyProdDecayFiles/ccbar_D2star0_Dstar+pi-_D+pi0_K-pi+pi+.dec"

 GENERATE set continuum
exit

source ../MyProdDecayFiles/filter_ccbar_D2star0.tcl

#Run over events:
ev begin -nev 2000
exit
