# Setup the path:
source PARENT/GeneratorsQA/EvtGen.tcl

module talk GfiEvtGen

 #####################Spin 1 From here down use Transmogrifier
 #UDECAY set "../ProdDecayFiles/ccbar_Dstar2640+_Dst+pi+pi-_D0pi+_K-pi+.dec"

 #UDECAY set "../ProdDecayFiles/ccbar_Dstar2640-_Dst-pi+pi-_antiD0pi-_K+pi-.dec"

 #UDECAY set "../ProdDecayFiles/ccbar_Dstar2640+_Dst+pi+pi-_D0pi+_K-pi+pi0.dec"

 #UDECAY set "../ProdDecayFiles/ccbar_Dstar2640-_Dst-pi+pi-_antiD0pi-_K+pi-pi0.dec"

 #UDECAY set "../ProdDecayFiles/ccbar_Dstar2640+_Dst+pi+pi-_D0pi+_K-pi+pi+pi-.dec"

 UDECAY set "../ProdDecayFiles/ccbar_Dstar2640-_Dst-pi+pi-_antiD0pi-_K+pi-pi+pi-.dec"

 #generate continuum or Upsilon(4S)
 GENERATE set continuum
exit

#source ../ProdDecayFiles/filter_ccbar_Dstar2S+.tcl
source ../ProdDecayFiles/filter_ccbar_Dstar2S-.tcl

#Run over events:
ev begin -nev 10
exit
