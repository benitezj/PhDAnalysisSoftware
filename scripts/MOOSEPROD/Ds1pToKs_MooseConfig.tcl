
set ProdTclOnly      true
set RUNNUM 987654
set CONDALIAS Aug2001 
set SimAppBkndInputCollection /store/SP/BkgTriggers/BkgTriggers_200108_V01
set SimAppBkndFirstEvent 1
set SimAppDigiMix true
set SimAppKanDigiMix true
set NEVENT  100
set UDECAY PARENT/ProdDecayFiles/ccbar_Ds1+_D0starpi+_D0Ks0pi+_K-pi+pi+pi-pi+.dec
set MooseTuple root  
set MooseHBookFile        Ds1Moose.root
set MooseOutputCollection  /work/users/benitezj/Ds1pToDstKs_Collection
mod talk KanEventOutput
  allowDirectoryCreation set true
exit

module talk GfiEvtGen
GENERATE set continuum
exit

source ../ProdDecayFiles/filter_ccbar_Ds1p+.tcl


sourceFoundFile  Moose/MooseProduction.tcl


