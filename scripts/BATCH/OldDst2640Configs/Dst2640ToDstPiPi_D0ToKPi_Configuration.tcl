#-------------------------------------------------- 
#Reconstruction of:
#Dst(2640)->Dst(2010) pi pi
#           Dst(2010)->D0 pi
#                      D0->K pi
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
#whats this?
#--------------------------------------------------
FwkCfgVar BetaMiniReadPersistence "Kan"    

#--------------------------------------------------
#set the print frequency of event counter
#--------------------------------------------------
talkto EvtCounter {
    printFreq set 200
}

#--------------------------------------------------
#Choose Root as output
#--------------------------------------------------
FwkCfgVar BetaMiniTuple "root"                                    
FwkCfgVar histFileName "Ntuple.root"        

#--------------------------------------------------
#Set Number of events to be analyzed
#--------------------------------------------------
FwkCfgVar NEvent 1000      

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
source SP-7787.tcl

#--------------------------------------------------
#DATA or MC
#--------------------------------------------------
FwkCfgVar ConfigPatch "MC"
FwkCfgVar MCTruth true
    
#--------------------------------------------------
#configure D0 decay selection
#--------------------------------------------------
module talk D0ToKPi  
   pionList set piLHTight
   kaonList set KLHTight
   D0List set D0FromKPiCandList
   tightD0MassCutLo set 1.78
   tightD0MassCutHi set 1.94
   D0MomentumCutLo set 2. 
   FitVtx set false
exit

#--------------------------------------------------
#configure Dstar decay selection
#--------------------------------------------------
module talk DstToD0Pi 
  slowPionList set GoodTracksVeryLoose
  D0CandList set D0FromKPiCandList
  DstarCandList set DstarCandList	
  deltaMassCutLo set .13
  deltaMassCutHi set .16
  D0VtxProbCut set 0
  FitVtx set false
exit
 
#--------------------------------------------------
#configure Dst2640 decay selection
#--------------------------------------------------
module talk Dst2640ToDstPiPi
    EventShapeTracksList set GoodTracksLoose
    DstCandList set DstarCandList
    Pion1List set piLHVeryLoose
    Pion2List set piLHVeryLoose
    Dst2640DeltaMassCutLo set 0.0
    Dst2640DeltaMassCutHi set 10.0
    Dst2640pstarCutLo set 2.5
    Dst2640VtxProbCutLo set .001
    D0massCutLo set 1.837
    D0massCutHi set 1.897
    DstardmCutLo set .1435
    DstardmCutHi set .1475
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
#disable the other analysis modules
#--------------------------------------------------
module disable QaMiniBtaCandidates
module disable D0ToK3Pi
module disable Ds1pToDstKs 

#--------------------------------------------------
#append analysis modules
#--------------------------------------------------
path append Everything D0ToKPi
path append Everything DstToD0Pi
path append Everything Dst2640ToDstPiPi
path append Everything BtuTupleMaker

#--------------------------------------------------
#Configure NtupleMaker
#--------------------------------------------------
talkto BtuTupleMaker {
    listToDump set Dst2640CandList
    ntupleName set "Ntuple"

    writeEveryEvent set $MCTruth

    checkClones set false

    fillMC set $MCTruth  

    eventBlockContents set "EventID CMp4 BeamSpot PrimVtx UsrData(EventUsrBlock)"

    mcBlockContents  set "Mass CMMomentum Momentum Vertex"
    
    ntpBlockConfigs set "B*+     Dst2640   3 100" 
    ntpBlockConfigs set "D*+     Dstar   2 100" 
    ntpBlockConfigs set "D0     D0    2 100" 
    ntpBlockConfigs set "pi+     Pi    0 100" 
    ntpBlockConfigs set "K-     K   0 100" 
 
    ntpBlockContents set "Dst2640 : MCIdx  Mass Vertex VtxChi2 Momentum Doca Poca CMMomentum " 
    ntpBlockContents set "Dstar : MCIdx  Mass Vertex VtxChi2 Momentum Doca Poca CMMomentum UsrData(DstarUsrBlock) "
    ntpBlockContents set "D0 : MCIdx  Mass Vertex VtxChi2 Momentum Doca Poca Flight FlightBS CMMomentum"
    ntpBlockContents set "Pi: MCIdx  Momentum Doca Poca CMMomentum "
    ntpBlockContents set "K: MCIdx  Momentum Doca Poca CMMomentum "

    
    checkCloneBlocks set "K Pi"


    ntpBlockToTrk set "K Pi"
    trkExtraContents set HOTS  
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






