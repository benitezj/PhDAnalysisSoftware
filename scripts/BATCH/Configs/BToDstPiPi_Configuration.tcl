#Reconstruction of:
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
FwkCfgVar ConfigPatch "Run2"
FwkCfgVar MCTruth false
    
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
   VtxProbCut set 0
   D0MassCutLo set 1.81
   D0MassCutHi set 1.91
   D0MomentumCutLo set 0
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
   VtxProbCut set 0
   D0MassCutLo set 1.81
   D0MassCutHi set 1.91
   D0MomentumCutLo set 0
exit
                                                                                                                                          
##################
## Merge the 2 modes
##################
mod clone SmpMergerDefiner MyMergedD0List
catch { setProduction MyMergedD0List }
talkto MyMergedD0List {
   debug             set f
   verbose           set f
                                                                                                                                       
   disableCloneCheck set true
                                                                                                                                    
   inputListNames    set "MyD0FromKPiList"
   inputListNames    set "MyD0FromK3PiList"
}



#--------------------------------------------------
#configure Dstar decay selection
#--------------------------------------------------
module talk DstToD0Pi 
   slowPionList set GoodTracksVeryLoose
   D0CandList set MyMergedD0List
   DstarCandList set MyDstarCandList
   massConstrain set false
   beamConstrain set false
   FitVtx set false
   VtxProbCut set 0	
   deltaMassCutLo set .13
   deltaMassCutHi set .16
   DstarMomentumCutLo set 0
exit
#--------------------------------------------------
#configure X decay selection
#--------------------------------------------------
module talk BToDstPiPi
    DstCandList set MyDstarCandList
    PionList set GoodTracksLoose
    RemoveFakeTrack set false
    RemoveFakeParent set true
    KeepWrongSign set true
    massConstrain set true
    beamConstrain set true
    FitVtx set true
    VtxProbCut set .001
    MesCut set 5.179
    DeltaECut set .2
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


###----------------
##disable sequences and add only the necessary ones
##----------------
# seq disable BetaMiniPhysicsSequence
# path append Everything NotPionLHKaonMicroSelection
# path append Everything TightLHKaonMicroSelection
# path append Everything TightLHPionMicroSelection

#--------------------------------------------------
#append analysis modules
#--------------------------------------------------
path append Everything D0ToKPi
path append Everything D0ToK3Pi
path append Everything MyMergedD0List
path append Everything DstToD0Pi
path append Everything BToDstPiPi
path append Everything BtuTupleMaker


#--------------------------------------------------
#disable the other analysis modules
#--------------------------------------------------
module disable QaMiniBtaCandidates
#module disable D0ToK3Pi
module disable Ds1pToDstKs 



#--------------------------------------------------
#Configure NtupleMaker
#--------------------------------------------------
talkto BtuTupleMaker {
    listToDump set BToDstPiPi
    ntupleName set "ntp1"

writeEveryEvent set false
fillMC set false

    checkClones set false 

    eventBlockContents set "EventID CMp4 BeamSpot"

    mcBlockContents  set "Mass CMMomentum Momentum Vertex"
 
    ntpBlockConfigs set "B+    B     3 20" 
    ntpBlockConfigs set "D*+   Dst   2 20" 
    ntpBlockConfigs set "D0    D     4 20" 
    ntpBlockConfigs set "pi+   Pi    0 80" 
    ntpBlockConfigs set "K+    K     0 20" 

    ntpBlockContents set "B:   Mass Vertex VtxChi2 Momentum CMMomentum FlightBS Dalitz UsrData(UsrBlock)" 
    ntpBlockContents set "Dst: Mass Vertex VtxChi2 Momentum CMMomentum "
    ntpBlockContents set "D:   Mass Vertex VtxChi2 Momentum CMMomentum Flight "
    ntpBlockContents set "Pi:  Momentum Doca Poca CMMomentum "
    ntpBlockContents set "K:   Momentum Doca Poca CMMomentum "

    
    checkCloneBlocks set "K Pi"


    ntpBlockToTrk set "K Pi"
    #trkExtraContents set HOTS 
    trkExtraContents set Dirc:pi,K  
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
