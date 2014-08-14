#-------------------------------------------------- 
#Reconstruction of:
#Dst(2640)->Dst(2010) Pi Pi
#           Dst(2010)->D0 Pi
#                      D0->K 3Pi
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
#should I use refit?
#--------------------------------------------------
FwkCfgVar levelOfDetail "cache"  

#--------------------------------------------------
#Specify the input data
#--------------------------------------------------
source SP-7789.tcl

#--------------------------------------------------
#DATA or MC
#--------------------------------------------------
FwkCfgVar ConfigPatch "MC"
FwkCfgVar MCTruth true
    
#--------------------------------------------------
#configure D0 decay selection
#--------------------------------------------------
module talk D0ToKPi  
   #need to use tight selectors at reconstruction because bkgs are too high
   kaonList set KLHTight
   pionList set piLHTight
   D0List set MyD0FromKPiList 
   massConstrain set false
   beamConstrain set true
   FitVtx set true
   VtxProbCut set .005
   D0MassCutLo set 1.78
   D0MassCutHi set 1.94
   D0MomentumCutLo set 2.4
exit

#--------------------------------------------------
#configure Dstar decay selection
#--------------------------------------------------
module talk DstToD0Pi 
   slowPionList set GoodTracksVeryLoose
   D0CandList set MyD0FromKPiList
   DstarCandList set MyDstarCandList
   massConstrain set false
   beamConstrain set true
   FitVtx set true
   VtxProbCut set 0	
   deltaMassCutLo set .13
   deltaMassCutHi set .16
   DstarMomentumCutLo set 0
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
path append Everything D0ToKPi
path append Everything DstToD0Pi
path append Everything BtuTupleMaker


#--------------------------------------------------
#disable the other analysis modules
#--------------------------------------------------
module disable QaMiniBtaCandidates
module disable D0ToK3Pi
module disable XToDstPiPi
module disable Ds1pToDstKs 



#--------------------------------------------------
#Configure NtupleMaker
#--------------------------------------------------
talkto BtuTupleMaker {
    listToDump set MyDstarCandList
    ntupleName set "ntp1"

    writeEveryEvent set $MCTruth

    fillMC set $MCTruth 

    checkClones set false 

    eventBlockContents set "EventID CMp4 BeamSpot PrimVtx "

    mcBlockContents  set "Mass CMMomentum Momentum Vertex"
    
    ntpBlockConfigs set "D*+     Dstar   2 100" 
    ntpBlockConfigs set "D0     D0    2 100" 
    ntpBlockConfigs set "pi+     Pi    0 100" 
    ntpBlockConfigs set "K+     K   0 100" 
 
    ntpBlockContents set "Dstar : MCIdx  Mass Vertex VtxChi2 Momentum Doca Poca CMMomentum"
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






