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
FwkCfgVar histFileName "DsTolnu/SP1005Skim/DsRecoSmall/Ntuples/Ntuple1.root" 

#--------------------------------------------------
#Set Number of events to be analyzed
#--------------------------------------------------
FwkCfgVar NEvent 100

#--------------------------------------------------
#Set the first event to be analyzed
#--------------------------------------------------
talkto KanEventInput {
first set 1
}

#--------------------------------------------------
#
#--------------------------------------------------
FwkCfgVar levelOfDetail "cache"  

#--------------------------------------------------
#Specify the input data
#--------------------------------------------------
source DsTolnu/SP1005Skim/DsRecoSmall/Ntuples/DataSet-1.tcl


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


#-------------------------------------
#creat decay modes: According to Shanes skim 
#-------------------------------------
source SOFTWARE/BATCH/Configs/DReco_GenericTag_Config.tcl
seq append DRecoSequence ChmDrecoSequence

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
 FitProbCut set .001
 #next cut has to match with DRecoFragPionsPatchAll
 ExGamList set GoodPhotonLoose
 ExGamEnergyCutHi set .05
#add cut on Ds p*
exit 


#--------------------------------------------------
#Configure NtupleMaker
#--------------------------------------------------
talkto BtuTupleMaker {
    listToDump set DRecoCandidateList
    ntupleName set "ntp1"

    writeEveryEvent set false
    fillMC set false
    checkClones set false 
    eventBlockContents set "EventID CMp4 BeamSpot PrimVtx"
    mcBlockContents  set "Mass CMMomentum Momentum Vertex"
    
    ntpBlockConfigs set "Upsilon(4S)   Evt   4 30" 
    ntpBlockConfigs set "D_s*+  DsSt  2  30" 
    ntpBlockConfigs set "D_s+   Ds  0  30" 
    ntpBlockConfigs set "D_1+   Frag  20 30" 
    ntpBlockConfigs set "D_10   Frag  20 30"
    ntpBlockConfigs set "D+     Tag   5 30" 
    ntpBlockConfigs set "D0     Tag   5 30" 
    ntpBlockConfigs set "K+     K     0 100"  
    ntpBlockConfigs set "K_S0   Ks    2 100" 
    ntpBlockConfigs set "pi+    Pi    0 200" 
    ntpBlockConfigs set "pi0    Pi0   2 200" 
    ntpBlockConfigs set "gamma  Gam   0 200" 
 
    ntpBlockContents set "Evt:  Mass CMMomentum Vertex VtxChi2 nDaughters UsrData(DRecoUsrBlock)"
    ntpBlockContents set "DsSt: Mass Vertex CMMomentum"
    ntpBlockContents set "Ds:   Mass CMMomentum"
    ntpBlockContents set "Tag:  MCIdx Mass Vertex VtxChi2 FlightBS CMMomentum nDaughters"
    ntpBlockContents set "Frag: Mass CMMomentum Vertex VtxChi2 nDaughters" 
    ntpBlockContents set "Ks:   MCIdx Mass Vertex FlightBS CMMomentum"
    ntpBlockContents set "Pi:   MCIdx CMMomentum"
    ntpBlockContents set "Pi0:  MCIdx Mass CMMomentum"
    ntpBlockContents set "K:    MCIdx CMMomentum"
    ntpBlockContents set "Gam:  MCIdx CMMomentum"
    
    checkCloneBlocks set "Pi Gam"
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
sequence append DRecoSequence BtuTupleMaker

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






