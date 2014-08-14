# -------- myskim.tcl: begin -----------
talkto EvtCounter {
  printFreq set 5000
}

set SkimsToRun MyDstar0Skim
set SkimOutputDir /work/users/benitezj/Dstar0/DATARun3On
set SkimsToWrite MyDstar0Skim
set SkimInputTcl /u/eb/benitezj/ana42/workdir/SKIMS/Dstar0/DATARun3On/SkimData-1.tcl
set SkimLOD cache
set SkimRemovePhysicsSequences CompProdCreateSequence,TagProdCreateSequence
set SkimOutputSuffix _0
set SkimFwkDump yes
set SkimNEvent 15000
sourceFoundFile SkimMini/SkimMiniProduction.tcl
# -------- myskim.tcl: end -----------

