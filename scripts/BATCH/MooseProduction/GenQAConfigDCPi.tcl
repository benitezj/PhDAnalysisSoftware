# Setup the path:
source PARENT/GeneratorsQA/EvtGen.tcl

module talk GfiEvtGen
 UDECAY set "../ProdDecayFiles/ccbar_D2star0_D+piminus_D+Kpipi.dec"
 GENERATE set continuum
exit


source ../ProdDecayFiles/filter_D2star0_D+piminus_D+Kpipi.tcl

#Run over events:
ev begin -nev 10
exit
