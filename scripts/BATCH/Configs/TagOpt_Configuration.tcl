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
FwkCfgVar histFileName "DsTolnu/SP1005Skim/TagOpt/Ntuples/Run3/Ntuple1.root" 

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
#should I use refit?
#--------------------------------------------------
FwkCfgVar levelOfDetail "cache"  

#--------------------------------------------------
#Specify the input data
#--------------------------------------------------
source DsTolnu/SP1005Skim/TagOpt/NtuplesData/Run3/DataSet-1.tcl


#--------------------------------------------------
#DATA or MC
#--------------------------------------------------
FwkCfgVar ConfigPatch "Run2"
FwkCfgVar MCTruth false
    
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
sequence create RecoilSequence


#-------------------------------------
#creat decay modes: According to Shanes skim 
#-------------------------------------
FwkCfgVar writeChmTagNtuples 0
sourceFoundFile CharmTagTools/ChmTagLists.tcl
seq append RecoilSequence ChmDrecoSequence

##++++++++++++++++++++++++++++++++++++++
### Merge the D0 modes
##----------------------------------------
createsmpmerger RecoilSequence RecoilD0Main {
    inputListNames    set "ChmD0ToKPi"
    inputListNames    set "ChmD0ToKPiPi0"
    inputListNames    set "ChmD0ToK3Pi"
    inputListNames    set "ChmD0ToK3PiPi0Tight" 
    inputListNames    set "ChmD0ToKShortPiPi"
    inputListNames    set "ChmD0ToKShortPiPiPi0" 
}


##++++++++++++++++++++++++++++++++++++++
### Merge the D+ modes
##++++++++++++++++++++++++++++++++++++++
createsmpmerger RecoilSequence RecoilDPlusMain {
    inputListNames    set "ChmDPlusToKPiPi"
    inputListNames    set "ChmDPlusToKPiPiPi0"
    inputListNames    set "ChmDPlusToKShortPi"
    inputListNames    set "ChmDPlusToKShortPiPi0"
    inputListNames    set "ChmDPlusToKShort3Pi"
}


#--------------------------------------------------
#configure Modules
#--------------------------------------------------
module talk RecoilTagInterface  
    
     #D0: Kpi,Kpipi0,K3pi,Kspipi,K3pipi0,Kspipipi0
     TagList1 set RecoilD0Main
     TagListFit1 set true
     TagListMass1 set 1.8648
     TagListMassCut1 set .12
     TagListPstarCut1 set 2.0

     #D+: DcToKPiPi, DcToKPiPiPi0, DcToKsPi, DcToKsPiPi0, DcToKs3Pi
     TagList2 set RecoilDPlusMain
     TagListFit2 set true
     TagListMass2 set 1.8692
     TagListMassCut2 set .12
     TagListPstarCut2 set 2.0
exit



#--------------------------------------------------
#Configure NtupleMaker
#--------------------------------------------------
talkto BtuTupleMaker {
    listToDump set RecoilTagList
    ntupleName set "ntp1"

    writeEveryEvent set false
    fillMC set false
    checkClones set false 
    eventBlockContents set "EventID CMp4 BeamSpot PrimVtx"
    mcBlockContents  set "Mass CMMomentum Momentum Vertex"
    
    ntpBlockConfigs set "D+     Tag   5 30" 
    ntpBlockConfigs set "D0     Tag   5 30" 
    ntpBlockConfigs set "K+     K     0 100"  
    ntpBlockConfigs set "K_S0   Ks    2 100" 
    ntpBlockConfigs set "pi+    Pi    0 100" 
    ntpBlockConfigs set "pi0    Pi0   2 200" 
    ntpBlockConfigs set "gamma  Gam   0 100" 
 
    ntpBlockContents set "Tag:  Mass Vertex VtxChi2 FlightBS CMMomentum nDaughters"
    ntpBlockContents set "Ks:   Mass Vertex FlightBS CMMomentum"
    ntpBlockContents set "Pi:   CMMomentum"
    ntpBlockContents set "Pi0:  Mass CMMomentum"
    ntpBlockContents set "K:    CMMomentum"
    ntpBlockContents set "Gam:  CMMomentum"

    ntpBlockToTrk set "K Pi"
    trkExtraContents set HOTS
    trkExtraContents set BitMap:piSelectorsMap,KSelectorsMap   
}

#--------------------------------------------------
#append analysis modules
#--------------------------------------------------
sequence append RecoilSequence RecoilTagInterface
sequence append RecoilSequence RecoilTagDump
sequence append RecoilSequence RecoilCleanUp
sequence append RecoilSequence BtuTupleMaker

path append Everything RecoilSequence 

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






