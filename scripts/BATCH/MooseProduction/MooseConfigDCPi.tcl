
set ProdTclOnly      true
set RUNNUM 987654
set CONDALIAS Aug2001 
set SimAppBkndInputCollection /store/SP/BkgTriggers/BkgTriggers_200108_V01
set SimAppBkndFirstEvent 1
set SimAppDigiMix true
set SimAppKanDigiMix true
set NEVENT  20
set UDECAY ../MyProdDecayFiles/D2star0/DCPi/ccbar_D2star0_D+pi-_K-pi+pi+.dec
set MooseTuple root  
set MooseHBookFile        Moose.root
set MooseOutputCollection  DCharged/DChargedPi/MC/MyGen/Test_Collection

mod talk KanEventOutput
  allowDirectoryCreation set true
exit

module enable GfiEvtGen
module talk GfiEvtGen
  GENERATE set continuum
exit


source ../MyProdDecayFiles/D2star0/filter_ccbar_D2star0.tcl

sourceFoundFile  Moose/MooseProduction.tcl


