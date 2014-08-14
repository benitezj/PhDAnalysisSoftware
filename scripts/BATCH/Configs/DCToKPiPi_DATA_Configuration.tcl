#Reconstruction of:X->D+ pi-
#Author: Jose Benitez
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
    printFreq set 1000
}

#--------------------------------------------------
#Choose Root as output
#--------------------------------------------------
FwkCfgVar BetaMiniTuple "root"                                    
FwkCfgVar histFileName "DstarCharged/DstarPi/DATARun1On/Ntuple1.root" 

#--------------------------------------------------
#Set Number of events to be analyzed
#--------------------------------------------------
FwkCfgVar NEvent 10000000


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
source /u/eb/benitezj/ana42/workdir/DstarCharged/DstarPi/DATARun1On/DataSet-1.tcl 

#--------------------------------------------------
#DATA or MC
#--------------------------------------------------
FwkCfgVar ConfigPatch "Run2"
FwkCfgVar MCTruth false
    
#--------------------------------------------------
#configure D decay selection
#--------------------------------------------------

module talk DCToKPiPi     
   #skim uses KLHLoose
   kaonList set KLHLoose
   #skim uses piLHVeryLoose
   pion1List set piLHVeryLoose
   #skim uses piLHVeryLoose
   pion2List set piLHVeryLoose
   DList set MyDCFromKPiPiList 
   massConstrain set false
   beamConstrain set true
   FitVtx set true
   #skim has .005 cut on vtx prob.
   VtxProbCut set 0
   DMassCutLo set 1.77
   DMassCutHi set 1.97
   #skim has cut at 2.7
   DMomentumCutLo set 2.5
exit




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



#--------------------------------------------------
#append analysis modules
#--------------------------------------------------
path append Everything DCToKPiPi
path append Everything BtuTupleMaker


module disable QaMiniBtaCandidates


#--------------------------------------------------
#Configure NtupleMaker
#--------------------------------------------------
talkto BtuTupleMaker {
    listToDump set MyDCFromKPiPiList
    ntupleName set "ntp1"

writeEveryEvent set $MCTruth
fillMC set $MCTruth 

    checkClones set false 

    eventBlockContents set "EventID CMp4 BeamSpot PrimVtx "

    mcBlockContents  set "Mass CMMomentum Momentum Vertex"
 
    ntpBlockConfigs set "D+     D   3 100" 
    ntpBlockConfigs set "pi+     Pi    0 100" 
    ntpBlockConfigs set "K+     K   0 100" 

    ntpBlockContents set "D : MCIdx  Mass Vertex VtxChi2 Momentum Doca Poca Flight FlightBS CMMomentum"
    ntpBlockContents set "Pi: MCIdx  Momentum Doca Poca CMMomentum "
    ntpBlockContents set "K: MCIdx  Momentum Doca Poca CMMomentum "
    
    checkCloneBlocks set "K Pi"

    ntpBlockToTrk set "K Pi"
    trkExtraContents set HOTS  
    trkExtraContents set Dirc
    trkExtraContents set BitMap:piSelectorsMap,KSelectorsMap
    
}


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
