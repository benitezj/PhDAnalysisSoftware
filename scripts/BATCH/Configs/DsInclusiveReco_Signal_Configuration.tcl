#-------------------------------------------------- 
#Reconstruction of:
#--------------------------------------------------

#--------------------------------------------------
#some default stuff
#--------------------------------------------------
sourceFoundFile ErrLogger/ErrLog.tcl
sourceFoundFile FrameScripts/FwkCfgVar.tcl
sourceFoundFile FrameScripts/talkto.tcl
sourceFoundFile FrameScripts/setProduction.tcl

#--------------------------------------------------
#Disable the use of envvars
#--------------------------------------------------
set ProdTclOnly true                                      

#--------------------------------------------------
#set the error logging level to 'warning'
#--------------------------------------------------
ErrLoggingLevel warning

#--------------------------------------------------
#
#--------------------------------------------------
FwkCfgVar BetaMiniReadPersistence "Kan"    

#--------------------------------------------------
#set the print frequency of event counter
#--------------------------------------------------
talkto EvtCounter {
    printFreq set 100
}

#--------------------------------------------------
#Choose Root as output
#--------------------------------------------------
FwkCfgVar BetaMiniTuple "root"
FwkCfgVar histFileName "DsTolnu/SP1005Sig/DsReco/NtuplesSmall/Run3On/Ntuple1.root" 

#--------------------------------------------------
#Set Number of events to be analyzed
#--------------------------------------------------
FwkCfgVar NEvent 2000

#--------------------------------------------------
#Set the first event to be analyzed
#--------------------------------------------------
talkto KanEventInput {
first set 1
}

#--------------------------------------------------
#should I use refit?
#--------------------------------------------------
FwkCfgVar levelOfDetail "cache"  

#--------------------------------------------------
#Specify the input data
#--------------------------------------------------
source DsTolnu/SP1005Sig/DsReco/NtuplesSmall/Run3On/DataSet-1.tcl 

#--------------------------------------------------
#DATA or MC
#--------------------------------------------------
FwkCfgVar ConfigPatch "MC"
FwkCfgVar MCTruth true
    
#--------------------------------------------------                            
#create Everything path and add core sequences to it
#--------------------------------------------------                        
sourceFoundFile BetaMiniUser/btaMiniPhysics.tcl

#--------------------------------------------------
#actions 
#--------------------------------------------------
sourceFoundFile BetaMiniSequences/BetaMiniActions.tcl
action disable HistoDirAction

#--------------------------------------------------
#Enable User defined Data
#--------------------------------------------------
sourceFoundFile UsrTools/UsrDataProcs.tcl
enableReadUsrData


##----------------
##create this analysis sequence
##-------------------------
sequence create DRecoSequence

#------------------------------------
#creat decay modes: 
#-------------------------------------

##-----------------------------------
#create the D0 modes
#------------------------------------
# D0 -> K pi
mod clone SmpMakerDefiner DRecoD0ToKPi
seq append DRecoSequence DRecoD0ToKPi
catch { setProduction DRecoD0ToKPi }
talkto DRecoD0ToKPi {
  decayMode         set "D0 -> K- pi+"
  daughterListNames set "KLHTight"
  daughterListNames set "GoodTracksLoose"
  fittingAlgorithm  set "Add4"
  postFitSelectors  set "Mass 1.7:2.1"
}
# D0 -> K pi pi0
mod clone SmpMakerDefiner DRecoD0ToKPiPi0
seq append DRecoSequence DRecoD0ToKPiPi0
catch { setProduction DRecoD0ToKPiPi0 }
talkto DRecoD0ToKPiPi0 {
  decayMode         set "D0 -> K- pi+ pi0"
  daughterListNames set "KLHTight"
  daughterListNames set "GoodTracksLoose"
  daughterListNames set "pi0DefaultMass"
  fittingAlgorithm  set "Add4"
  postFitSelectors  set "Mass 1.7:2.1"
}
# D0 -> K 3pi
mod clone SmpMakerDefiner DRecoD0ToK3Pi
seq append DRecoSequence DRecoD0ToK3Pi
catch { setProduction DRecoD0ToK3Pi }
talkto DRecoD0ToK3Pi {
  decayMode         set "D0 -> K- pi+ pi- pi+"
  daughterListNames set "KLHTight"
  daughterListNames set "GoodTracksLoose"
  daughterListNames set "GoodTracksLoose"
  daughterListNames set "GoodTracksLoose"
  fittingAlgorithm  set "Add4"
  postFitSelectors  set "Mass 1.7:2.1"
}
# D0 -> K0_S pi+ pi-
mod clone SmpMakerDefiner DRecoD0ToKShortPiPi
seq append DRecoSequence DRecoD0ToKShortPiPi
catch { setProduction DRecoD0ToKShortPiPi }
talkto DRecoD0ToKShortPiPi {
  decayMode         set "D0 -> K_S0 pi+ pi-"
  daughterListNames set "KsDefaultMass"
  daughterListNames set "GoodTracksLoose"
  daughterListNames set "GoodTracksLoose"
  fittingAlgorithm  set "Add4"
  postFitSelectors  set "Mass 1.7:2.1"
}

# D0 -> K pi pi pi pi0 
mod clone SmpMakerDefiner DRecoD0ToK3PiPi0
seq append DRecoSequence DRecoD0ToK3PiPi0
catch { setProduction DRecoD0ToK3PiPi0 }
talkto DRecoD0ToK3PiPi0 {
  decayMode         set "D0 -> K- pi+ pi- pi+ pi0"
  daughterListNames set "KLHTight"
  daughterListNames set "GoodTracksLoose"
  daughterListNames set "GoodTracksLoose"
  daughterListNames set "GoodTracksLoose"
  daughterListNames set "pi0DefaultMass"
  fittingAlgorithm  set "Add4"
  postFitSelectors  set "Mass 1.7:2.1"
}
# D0 -> K0_S pi+ pi- pi0 
mod clone SmpMakerDefiner DRecoD0ToKShortPiPiPi0
seq append DRecoSequence DRecoD0ToKShortPiPiPi0
catch { setProduction DRecoD0ToKShortPiPiPi0 }
talkto DRecoD0ToKShortPiPiPi0 {
  decayMode         set "D0 -> K_S0 pi+ pi- pi0"
  daughterListNames set "KsDefaultMass"
  daughterListNames set "GoodTracksLoose"
  daughterListNames set "GoodTracksLoose"
  daughterListNames set "pi0DefaultMass"
  fittingAlgorithm  set "Add4"
  postFitSelectors  set "Mass 1.7:2.1"
}
 

##++++++++++++++++++++++++++++++++++++++
### Merge the D0 modes
##----------------------------------------
createsmpmerger DRecoSequence DRecoD0Main {
    inputListNames    set "DRecoD0ToKPi"
    inputListNames    set "DRecoD0ToKPiPi0"
    inputListNames    set "DRecoD0ToK3Pi"
    inputListNames    set "DRecoD0ToKShortPiPi"
    inputListNames    set "DRecoD0ToK3PiPi0"
    inputListNames    set "DRecoD0ToKShortPiPiPi0"
}


##----------------------------------------
#create the D+ modes
#------------------------------------
# D+ -> K- pi+ pi+ 
mod clone SmpMakerDefiner DRecoDPlusToKPiPi
seq append DRecoSequence DRecoDPlusToKPiPi
catch { setProduction DRecoDPlusToKPiPi }
talkto DRecoDPlusToKPiPi {
  decayMode         set "D+ -> K- pi+ pi+"
  daughterListNames set "KLHTight"
  daughterListNames set "GoodTracksLoose"
  daughterListNames set "GoodTracksLoose"
  fittingAlgorithm  set "Add4"
  postFitSelectors  set "Mass 1.7:2.1"
}
# D+ -> K- pi+ pi+ pi0
mod clone SmpMakerDefiner DRecoDPlusToKPiPiPi0
seq append DRecoSequence DRecoDPlusToKPiPiPi0
catch { setProduction DRecoDPlusToKPiPiPi0 }
talkto DRecoDPlusToKPiPiPi0 {
  decayMode         set "D+ -> K- pi+ pi+ pi0"
  daughterListNames set "KLHTight"
  daughterListNames set "GoodTracksLoose"
  daughterListNames set "GoodTracksLoose"
  daughterListNames set "pi0DefaultMass"
  fittingAlgorithm  set "Add4"
  postFitSelectors  set "Mass 1.7:2.1"
}
# D+ -> K0_S pi+ 
mod clone SmpMakerDefiner DRecoDPlusToKShortPi
seq append DRecoSequence DRecoDPlusToKShortPi
catch { setProduction DRecoDPlusToKShortPi }
talkto DRecoDPlusToKShortPi {
  decayMode         set "D+ -> K_S0 pi+"
  daughterListNames set "KsDefaultMass"
  daughterListNames set "GoodTracksLoose"
  fittingAlgorithm  set "Add4"
  postFitSelectors  set "Mass 1.7:2.1"
}
# D+ -> K0_S pi+ pi0 
mod clone SmpMakerDefiner DRecoDPlusToKShortPiPi0
seq append DRecoSequence DRecoDPlusToKShortPiPi0
catch { setProduction DRecoDPlusToKShortPiPi0 }
talkto DRecoDPlusToKShortPiPi0 {
  decayMode         set "D+ -> K_S0 pi+ pi0"
  daughterListNames set "KsDefaultMass"
  daughterListNames set "GoodTracksLoose"
  daughterListNames set "pi0DefaultMass"
  fittingAlgorithm  set "Add4"
  postFitSelectors  set "Mass 1.7:2.1"
}
# D+ -> K0_S pi+ pi- pi+ 
mod clone SmpMakerDefiner DRecoDPlusToKShort3Pi
seq append DRecoSequence DRecoDPlusToKShort3Pi
catch { setProduction DRecoDPlusToKShort3Pi }
talkto DRecoDPlusToKShort3Pi {
  decayMode         set "D+ -> K_S0 pi+ pi- pi+"
  daughterListNames set "KsDefaultMass"
  daughterListNames set "GoodTracksLoose"
  daughterListNames set "GoodTracksLoose"
  daughterListNames set "GoodTracksLoose"
  fittingAlgorithm  set "Add4"
  postFitSelectors  set "Mass 1.7:2.1"
}
##++++++++++++++++++++++++++++++++++++++
### Merge the D+ modes
##++++++++++++++++++++++++++++++++++++++
createsmpmerger DRecoSequence DRecoDPlusMain {
    inputListNames    set "DRecoDPlusToKPiPi"
    inputListNames    set "DRecoDPlusToKPiPiPi0"
    inputListNames    set "DRecoDPlusToKShortPi"
    inputListNames    set "DRecoDPlusToKShortPiPi0"
    inputListNames    set "DRecoDPlusToKShort3Pi"
}


#--------------------------------------------------
#configure Modules
#--------------------------------------------------
module talk DRecoTagInterface  
    
     #D0: Kpi,Kpipi0,K3pi,Kspipi,Kpipipipi0,Kspipipi0
     TagList1 set DRecoD0Main
     TagListFit1 set true
     TagListMass1 set 1.8648
     TagListMassCut1 set .035
     TagListPstarCut1 set 2.0

     #D+: DcToKPiPi, DcToKPiPiPi0, DcToKsPi, DcToKsPiPi0, DcToKsPiPiPi
     TagList2 set DRecoDPlusMain
     TagListFit2 set true
     TagListMass2 set 1.8692
     TagListMassCut2 set .035
     TagListPstarCut2 set 2.0

exit

module talk DRecoTagKaon
  KList set KLHLoose
  K0List set KsDefaultMass
  KaonPstarCutLo set .0
  KaonPstarCutHi set 2.
exit

module talk DRecoFragPions
    PiList set GoodTracksLoose
    PionPCutLo set .1
    PionPCutHi set 2.
    Pi0List set pi0DefaultMass
    Pi0GamEnergyCutLo set .05
    Pi0GamEnergyCutHi set 2.
    #this angle defines signal side (signal tracks are > DivisionAngle)
    DivisionAngle set 1.
    NMaxParticles set 10.
    NMaxSigParticles set 7.
exit

module talk DRecoDsStGam
  GamList set GoodPhotonLoose
  GamEnergyCutLo set .15
  GamEnergyCutHi set 2.
  GamAngleCut set .0
exit

module talk DRecoFragPionsPatchAll
     PatchList set GoodPhotonLoose
     EnergyCutLo set .05
     EnergyCutHi set 10.
     NMaxParticles set 2.
exit

module talk DRecoCheckCharge
  #first particle (tag-D) is last digit: 4=negative,5=neutral,6=positive
  #complex congujate event is automatically included.

  #missing particle is Ds+(c sbar)   
  
  #--------------
  #D0
  #--------------
  #Gam,Frag0(u ubar), K-(s ubar), D0(cbar u)
  ChargeConfig1 set 5545   
  #Gam,Frag-(d ubar), K0(s dbar), D0(cbar u)
  ChargeConfig2 set 5455 

  #---------------------
  #D-
  #---------------------
  #Gam,Frag0(u ubar), K0(s dbar), D-(cbar d)
  ChargeConfig3 set 5554  
  #Gam,Frag+(u dbar), K-(s ubar), D-(cbar d)
  ChargeConfig4 set 5644
  

  CheckFlavor set 1

exit

module talk DRecoDsInclusiveFit
 DsStMassCutLo set 1.6
 DsStMassCutHi set 2.6
 FitEvt set 1
 FitProbCut set .0001
 #next cut has to match with DRecoFragPionsPatchAll
 ExGamList set GoodPhotonLoose
 ExGamEnergyCutHi set .05
 CorrDsSt set 1
exit 


#--------------------------------------------------
#Configure NtupleMaker
#--------------------------------------------------
talkto BtuTupleMaker {
    listToDump set DRecoCandidateList
    ntupleName set "ntp1"

    writeEveryEvent set false
    fillMC set true
    checkClones set false 
    eventBlockContents set "EventID CMp4 BeamSpot PrimVtx"
    mcBlockContents  set "Mass CMMomentum Momentum Vertex"
    
    ntpBlockConfigs set "Upsilon(4S)   Evt   4 30" 
    ntpBlockConfigs set "D_s*+  DsSt  2  30" 
    ntpBlockConfigs set "D_s+  Ds  0  30" 
    ntpBlockConfigs set "D_1+   Frag  20 30" 
    ntpBlockConfigs set "D_10   Frag  20 30"
    ntpBlockConfigs set "D+     Tag   5 30" 
    ntpBlockConfigs set "D0     Tag   5 30" 
    ntpBlockConfigs set "K+     K     0 100"  
    ntpBlockConfigs set "K_S0   Ks    2 100" 
    ntpBlockConfigs set "pi+    Pi    0 100" 
    ntpBlockConfigs set "pi0    Pi0   2 200" 
    ntpBlockConfigs set "gamma  Gam   0 100" 
 
    ntpBlockContents set "Evt:  Mass CMMomentum Vertex VtxChi2 nDaughters UsrData(DRecoUsrBlock)"
    ntpBlockContents set "DsSt: Mass Vertex CMMomentum"
    ntpBlockContents set "Ds:   Mass CMMomentum"
    ntpBlockContents set "Tag:  MCIdx Mass Vertex VtxChi2 Flight FlightBS CMMomentum nDaughters"
    ntpBlockContents set "Frag: Mass CMMomentum Vertex VtxChi2 nDaughters" 
    ntpBlockContents set "Ks:   MCIdx Mass Vertex FlightBS CMMomentum"
    ntpBlockContents set "Pi:   MCIdx CMMomentum"
    ntpBlockContents set "Pi0:  MCIdx Mass CMMomentum"
    ntpBlockContents set "K:    MCIdx CMMomentum"
    ntpBlockContents set "Gam:  MCIdx CMMomentum"

    checkCloneBlocks set "K Pi Gam"
}

#--------------------------------------------------
#append analysis modules
#--------------------------------------------------

sequence append DRecoSequence DRecoTagInterface
sequence append DRecoSequence DRecoTagKaon
sequence append DRecoSequence DRecoFragPions
#This module prints the candidate lists for debugging
#sequence append DRecoSequence DRecoPrintTree
sequence append DRecoSequence DRecoDsStGam
sequence append DRecoSequence DRecoFragPionsPatchAll
sequence append DRecoSequence DRecoCheckCharge
sequence append DRecoSequence DRecoDsInclusiveFit
#This module deletes all the candidate lists otherwise memory leaks
sequence append DRecoSequence DRecoCleanUp
#sequence append DRecoSequence BtuTupleMaker

path append Everything DRecoSequence 

#--------------------------------------------------
#disable the other analysis modules
#--------------------------------------------------
module disable QaMiniBtaCandidates
#these produce ntuples in root file
module disable ChmPi0Analysis
module disable ChmMiniAnalysisDeltaM
module disable ChmMiniAnalysis

#--------------------------------------------------
#start the job
#--------------------------------------------------
path list

if [ info exists NEvent ] {
    ErrMsg trace " Processing $NEvent events"
    ev begin -nev $NEvent
} else {
    ErrMsg trace " Processing all events"
    ev begin
}

ErrMsg trace "completed OK"
exit






