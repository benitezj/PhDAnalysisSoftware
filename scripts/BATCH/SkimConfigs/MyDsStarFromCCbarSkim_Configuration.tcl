
talkto EvtCounter {
  printFreq set 2000
}

set SkimsToRun MyDsStarFromCCbarSkim
set SkimOutputDir SKIMS/DsStarFromCCbar/Run4On
set SkimsToWrite MyDsStarFromCCbarSkim
set SkimInputTcl SKIMS/DsStarFromCCbar/Run4On/SkimData-1.tcl
set SkimLOD cache
set SkimRemovePhysicsSequences CompProdCreateSequence,TagProdCreateSequence
set SkimOutputSuffix _1
set SkimFwkDump yes
set SkimNEvent 1000
set SkimMC true
sourceFoundFile SkimMini/SkimMiniProduction.tcl


