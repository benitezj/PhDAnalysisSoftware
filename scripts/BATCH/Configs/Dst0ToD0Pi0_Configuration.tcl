#-------------------------------------------------- 
#Reconstruction of:
#           D*0->D0 Pi0
#                D0->K Pi
#                D0->K 3Pi
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
   kaonList set KLHTight
   pionList set piLHTight
   D0List set MyD0FromKPiList 
   massConstrain set false
   beamConstrain set false
   FitVtx set false
   VtxProbCut set .0
   D0MassCutLo set 1.825
   D0MassCutHi set 1.905
   D0MomentumCutLo set .5
exit

module talk D0ToK3Pi     
   kaonList set KLHTight
   pion1List set piLHTight
   pion2List set GoodTracksLoose
   pion3List set GoodTracksLoose
   D0List set MyD0FromK3PiList 
   massConstrain set false
   beamConstrain set false
   FitVtx set false
   VtxProbCut set .0
   D0MassCutLo set 1.825
   D0MassCutHi set 1.905
   D0MomentumCutLo set .5
exit

##################
## Merge the 2 modes
##################
mod clone SmpMergerDefiner MyMergedD0List
catch { setProduction MyMergedD0List }
talkto MyMergedD0List {
   debug             set f
   verbose           set f
    
   ## disableCloneCheck=true is necessary to keep both the
   ## right-sign and wrong-sign combinations
   disableCloneCheck set true
    
   inputListNames    set "MyD0FromKPiList"
   inputListNames    set "MyD0FromK3PiList"
}

#--------------------------------------------------
#configure Dstar decay selection
#--------------------------------------------------
module talk Dst0ToD0Pi0 
   slowPionList set pi0DefaultMass
   D0CandList set MyMergedD0List
   DstarCandList set MyDstarCandList
   massConstrain set false
   beamConstrain set true
   FitVtx set true
   VtxProbCut set .0005	
   deltaMassCutLo set .130
   deltaMassCutHi set .180
   DstarMomentumCutLo set 1.5
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
path append Everything D0ToK3Pi
path append Everything MyMergedD0List
path append Everything Dst0ToD0Pi0
path append Everything BtuTupleMaker


#--------------------------------------------------
#disable the other analysis modules
#--------------------------------------------------
module disable QaMiniBtaCandidates
module disable XToDstPiPi
module disable XToDstPi
module disable Ds1pToDstKs 
module disable DstToD0Pi




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
    
    ntpBlockConfigs set "D*0    Dstar 2 100"  
    ntpBlockConfigs set "D0     D0    4 100" 
    ntpBlockConfigs set "pi0    Pi0   2 100"  
    ntpBlockConfigs set "pi+    Pi    0 100" 
    ntpBlockConfigs set "K+     K     0 100" 
    ntpBlockConfigs set "gamma  Gam   0 100" 
 
    ntpBlockContents set "Dstar: MCIdx  Mass Vertex VtxChi2 Momentum CMMomentum"
    ntpBlockContents set "D0:    MCIdx  Mass Vertex VtxChi2 Momentum Flight FlightBS CMMomentum nDaughters"
    ntpBlockContents set "Pi0:   MCIdx  Mass Vertex VtxChi2 Momentum CMMomentum "
    ntpBlockContents set "Pi:  MCIdx  Momentum Doca Poca CMMomentum "
    ntpBlockContents set "K: MCIdx  Momentum Doca Poca CMMomentum "
    ntpBlockContents set "Gam: MCIdx  Momentum CMMomentum " 

    
    checkCloneBlocks set "K Pi Gam"


    ntpBlockToTrk set "K Pi"
    trkExtraContents set HOTS  
    trkExtraContents set BitMap:piSelectorsMap,KSelectorsMap
    gamExtraContents set EMC
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






