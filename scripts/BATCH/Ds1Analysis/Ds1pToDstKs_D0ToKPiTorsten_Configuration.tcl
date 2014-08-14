#-------------------------------------------------- 
#Reconstruction of:
#Ds1(2536)->Dst(2010) Ks
#           Dst(2010)->D0 Pi
#                      D0->K Pi
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
FwkCfgVar histFileName "NtupleFile.root"        

#--------------------------------------------------
#Set Number of events to be analyzed
#--------------------------------------------------
FwkCfgVar NEvent 1000      

#--------------------------------------------------
#should I use refit?
#--------------------------------------------------
FwkCfgVar levelOfDetail "cache"  

#--------------------------------------------------
#Specify the input data
#--------------------------------------------------
source SP-7796.tcl

#--------------------------------------------------
#DATA or MC
#--------------------------------------------------
FwkCfgVar ConfigPatch "MC"
FwkCfgVar MCTruth true
    
#--------------------------------------------------
#configure D0 decay selection
#--------------------------------------------------
module talk D0ToKPi  
   pionList set GoodTracksVeryLoose
   kaonList set GoodTracksVeryLoose
   D0List set D0FromKPiCandList
   tightD0MassCutLo set 1.66
   tightD0MassCutHi set 2.06
   D0MomentumCutLo set 0. 
   FitVtx set false
exit

#--------------------------------------------------
#configure Dstar decay selection
#--------------------------------------------------
module talk DstToD0Pi 
  slowPionList set GoodTracksVeryLoose
  D0CandList set D0FromKPiCandList
  DstarCandList set DstarCandList	
  deltaMassCutLo set .130
  deltaMassCutHi set .160
  D0VtxProbCut set 1e-10
  FitVtx set false
exit

 
#--------------------------------------------------
#configure Ds1p decay selection
#--------------------------------------------------
module talk Ds1pToDstKs
    DstCandList set DstarCandList
    KsList set KsLoose
    massConstrainKs set false 
    Ds1pDeltaMassCutLo set 0
    Ds1pDeltaMassCutHi set .07
    Ds1ppstarCutLo set 0
    KsMassCutLo set .467
    KsMassCutHi set .527
exit

#--------------------------------------------------                            
#create Everything path and add core sequences to it
#--------------------------------------------------
sourceFoundFile BetaMiniUser/btaMiniPhysics.tcl

#--------------------------------------------------
#Actions  (whats this?) 
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

#--------------------------------------------------
#append analysis modules
#--------------------------------------------------
path append Everything D0ToKPi
path append Everything DstToD0Pi
path append Everything Ds1pToDstKs
path append Everything BtuTupleMaker

#--------------------------------------------------
#Configure NtupleMaker
#--------------------------------------------------
talkto BtuTupleMaker {
    listToDump set Ds1pCandList
    ntupleName set "Ntuple"

    writeEveryEvent set $MCTruth

    checkClones set false

    fillMC set $MCTruth  

    eventBlockContents set "EventID CMp4 BeamSpot PrimVtx "

    mcBlockContents  set "Mass CMMomentum Momentum Vertex"
    
    ntpBlockConfigs set "D'_s1+     Ds1p   2 500" 
    ntpBlockConfigs set "D*+     Dstar   2 500" 
    ntpBlockConfigs set "D0     D0    2 500" 
    ntpBlockConfigs set "K_S0    Ks    2 500" 
    ntpBlockConfigs set "pi+     Pi    0 500" 
    ntpBlockConfigs set "K-     K   0 500" 
 
    ntpBlockContents set "Ds1p : MCIdx  Mass Vertex VtxChi2 Momentum Doca Poca CMMomentum UsrData(Ds1pUsrBlock)" 
    ntpBlockContents set "Dstar : MCIdx  Mass Vertex VtxChi2 Momentum Doca Poca CMMomentum UsrData(DstarUsrBlock) "
    ntpBlockContents set "D0 : MCIdx  Mass Vertex VtxChi2 Momentum Doca Poca Flight FlightBS CMMomentum"
    ntpBlockContents set "Ks : MCIdx  Mass Vertex VtxChi2 Momentum Doca Poca Flight FlightBS CMMomentum"
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






