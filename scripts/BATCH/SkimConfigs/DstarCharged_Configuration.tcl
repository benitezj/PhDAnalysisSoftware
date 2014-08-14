# -------- myskim.tcl: begin -----------
talkto EvtCounter {
  printFreq set 1000
}

set SkimsToRun MyDstarChargedSkim
set SkimOutputDir /u/eb/benitezj/ana42/workdir/SKIMS/DstarCharged/DATARun1
set SkimsToWrite MyDstarChargedSkim
set SkimInputTcl SKIMS/DstarCharged/DATARun1/SkimData-1.tcl
set SkimLOD cache
set SkimRemovePhysicsSequences CompProdCreateSequence,TagProdCreateSequence
set SkimOutputSuffix _1
set SkimFwkDump yes
set SkimNEvent 10000
sourceFoundFile SkimMini/SkimMiniProduction.tcl
# -------- myskim.tcl: end -----------

