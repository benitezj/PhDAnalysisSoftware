# -------- myskim.tcl: begin -----------
talkto EvtCounter {
  printFreq set 5000
}

set SkimsToRun MyDCSkim
set SkimOutputDir SKIMS/DCharged/DATARun1Off
set SkimsToWrite MyDCSkim
set SkimInputTcl /u/eb/benitezj/ana42/workdir/SKIMS/DCharged/DATARun1Off/SkimData-1.tcl
set SkimLOD cache
set SkimRemovePhysicsSequences CompProdCreateSequence,TagProdCreateSequence
set SkimOutputSuffix _1
set SkimFwkDump yes
set SkimNEvent 1500000
sourceFoundFile SkimMini/SkimMiniProduction.tcl
# -------- myskim.tcl: end -----------

