
set ProdTclOnly      true
set RUNNUM 987654
set CONDALIAS Aug2001 
set SimAppBkndInputCollection /store/SP/BkgTriggers/BkgTriggers_200108_V01
set SimAppBkndFirstEvent 1
set SimAppDigiMix true
set SimAppKanDigiMix true
set NEVENT  10
set UDECAY ../ProdDecayFiles/ccbar_Dstar2640+_Dst+pi+pi-_D0pi+_K-pi+.dec
set MooseTuple root  
set MooseHBookFile        Ds1Moose.root
set MooseOutputCollection  /work/users/benitezj/SP-10001_Collection

mod talk KanEventOutput
  allowDirectoryCreation set true
exit

module enable GfiEvtGen
module talk GfiEvtGen
 # maxNumFilterTrials set 10000
  GENERATE set continuum
exit


source ../ProdDecayFiles/filter_ccbar_Dstar2S+.tcl

sourceFoundFile  Moose/MooseProduction.tcl


