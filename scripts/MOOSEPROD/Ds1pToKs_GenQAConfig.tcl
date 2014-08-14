# Setup the path:
source PARENT/GeneratorsQA/EvtGen.tcl

module talk GfiEvtGen
# Set your decay file:

#UDECAY set "../ProdDecayFiles/ccbar_Ds1+_Dstar+Ks0_D0Ks0pi+_K-pi+.dec"

#UDECAY set "../ProdDecayFiles/ccbar_Ds1-_Dstar-Ks0_antiD0Ks0pi-_K+pi-.dec"

#UDECAY set "../ProdDecayFiles/ccbar_Ds1+_Dstar+Ks0_D0Ks0pi+_K-pi+pi0.dec"

#UDECAY set "../ProdDecayFiles/ccbar_Ds1-_Dstar-Ks0_antiD0Ks0pi-_K+pi-pi0.dec"

UDECAY set "../ProdDecayFiles/ccbar_Ds1+_Dstar+Ks0_D0Ks0pi+_K-pi+pi+pi-pi+pi+pi-.dec"

#UDECAY set "../ProdDecayFiles/ccbar_Ds1-_Dstar-Ks0_antiD0Ks0pi-_K+pi-pi-pi+pi-pi+pi-.dec"

# If you want to generate continuum events:
GENERATE set continuum
exit

## Filter:
source ../ProdDecayFiles/filter_ccbar_Ds1p+.tcl
#source ../ProdDecayFiles/filter_ccbar_Ds1p-.tcl
 

# Run over events:
ev begin -nev 10
exit
