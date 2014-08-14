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
FwkCfgVar ConfigPatch "MC"
FwkCfgVar MCTruth true
    
#--------------------------------------------------
#configure D decay selection
#--------------------------------------------------

module talk DCToKPiPi     
   #skim uses KLHLoose
   kaonList set KLHTight
   #skim uses piLHVeryLoose
   pion1List set piLHLoose
   #skim uses piLHVeryLoose
   pion2List set piLHLoose
   DList set MyDCFromKPiPiList 
   massConstrain set false
   beamConstrain set false
   FitVtx set false
   #skim has .005 cut on vtx prob.
   VtxProbCut set 0
   DMassCutLo set 1.77
   DMassCutHi set 1.97
   #skim has cut at 2.7
   DMomentumCutLo set 0
exit


#--------------------------------------------------
#configure X decay selection
#--------------------------------------------------
module talk XToDCPi
    DCandList set MyDCFromKPiPiList
    PionList set piLHVeryTight
    XList set MyXCandList
    XPDTType set 0
    KeepWrongSign set true
    massConstrain set true
    beamConstrain set true
    removeDstars set true
    datatype set GENERIC
    FitVtx set true
    VtxProbCut set .001
    deltaMassCutLo set 0.0
    deltaMassCutHi set 2.0
    XMomentumCutLo set 3.0
    CosCut set 1
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
path append Everything XToDCPi
path append Everything BtuTupleMaker


module disable QaMiniBtaCandidates


#--------------------------------------------------
#Configure NtupleMaker
#--------------------------------------------------
talkto BtuTupleMaker {
    listToDump set MyXCandList
    ntupleName set "ntp1"

writeEveryEvent set false
fillMC set false

    checkClones set false 

    eventBlockContents set "EventID CMp4 BeamSpot"

    mcBlockContents  set "Mass CMMomentum Momentum Vertex"
 
    ntpBlockConfigs set "B*0    X   2 100"
    ntpBlockConfigs set "D+     D   3 100" 
    ntpBlockConfigs set "pi+    Pi  0 100" 
    ntpBlockConfigs set "K+     K   0 100" 

    ntpBlockContents set "X : MCIdx  Mass Vertex VtxChi2 Momentum CMMomentum UsrData(DPiUsrBlock)" 
    ntpBlockContents set "D : MCIdx  Mass Vertex VtxChi2 Momentum Flight FlightBS CMMomentum"
    ntpBlockContents set "Pi: MCIdx  Momentum CMMomentum "
    ntpBlockContents set "K:  MCIdx  Momentum CMMomentum "
    
    checkCloneBlocks set "K Pi"
    
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
