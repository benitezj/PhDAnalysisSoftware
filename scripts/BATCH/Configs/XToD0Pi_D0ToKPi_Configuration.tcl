#Reconstruction of:X->D0 pi-
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
FwkCfgVar histFileName "Dzero/DzeroPi/MC/SP1005/NtuplesOff/Ntuple1.root" 

#--------------------------------------------------
#Set Number of events to be analyzed
#--------------------------------------------------
FwkCfgVar NEvent 10000


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
source Dzero/DzeroPi/MC/SP1005/NtuplesOff/DataSet-1.tcl 

#--------------------------------------------------
#DATA or MC
#--------------------------------------------------
FwkCfgVar ConfigPatch "MC"
FwkCfgVar MCTruth true
    
#--------------------------------------------------
#configure D decay selection
#--------------------------------------------------
module talk D0ToKPi
   #skim selector
   kaonList set KLHTight
   #skim selector
   pionList set piLHTight
   D0List set MyD0FromKPiList
   massConstrain set false
   beamConstrain set false
   FitVtx set false
   VtxProbCut set 0
   D0MassCutLo set 1.76
   D0MassCutHi set 1.96
   D0MomentumCutLo set 0. 
exit


#--------------------------------------------------
#configure X decay selection
#--------------------------------------------------
module talk XToD0Pi
    DCandList set MyD0FromKPiList
    PionList set piLHTight
    XList set MyXCandList
    KeepWrongSign set true
    beamConstrain set true
    DstVetoPiList set ChargedTracks
    DstVetoPi0List set pi0DefaultMass
    removeDstars set true
    FitVtx set true
    VtxProbCut set .001
    deltaMassCutLo set 0.0
    deltaMassCutHi set 3.0
    XMomentumCutLo set 3.0
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
#Configure NtupleMaker
#--------------------------------------------------
talkto BtuTupleMaker {
    listToDump set MyXCandList
    ntupleName set "ntp1"

    writeEveryEvent set true
    fillMC set $MCTruth

    checkClones set false 

    eventBlockContents set "EventID CMp4 BeamSpot"

    mcBlockContents  set "Mass CMMomentum Momentum Vertex"
 
    ntpBlockConfigs set "B+    X   2 100"
    ntpBlockConfigs set "D0     D   4 100" 
    ntpBlockConfigs set "pi+    Pi  0 100" 
    ntpBlockConfigs set "K+     K   0 100" 

    ntpBlockContents set "X : MCIdx  Mass Vertex VtxChi2 Momentum CMMomentum UsrData(DPiUsrBlock)" 
    ntpBlockContents set "D : MCIdx  Mass Vertex VtxChi2 Momentum Flight FlightBS CMMomentum"
    ntpBlockContents set "Pi: MCIdx  Momentum CMMomentum "
    ntpBlockContents set "K:  MCIdx  Momentum CMMomentum "
    
    checkCloneBlocks set "K Pi"
    ntpBlockToTrk set "K Pi"
    trkExtraContents set HOTS
    trkExtraContents set Dirc

}


#--------------------------------------------------
#append analysis modules
#--------------------------------------------------
path append Everything D0ToKPi
path append Everything XToD0Pi
path append Everything BtuTupleMaker

module disable QaMiniBtaCandidates



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
